#include <i2c.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>
#include <systick.h>
#include <string.h>

/**
 * This is a specific I2C software implementation for use with PMI-BoB2:
 * * SDA is on GPIOB pin 8
 * * SCL is on GPIOB pin 9
 */


/**
 * @brief Initialises gpios for I2C1 on ports PB8 (SCL), PB9 (SDA)
 */
static void gpio_init(void)
{
    /* Enable GPIO clock for port B */
    RCC->IOPENR |= RCC_IOPENR_IOPBEN;

    /* Reset mode for PB8, PB9 */
    GPIOB->MODER &= ~(GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0);
    /* Set alternative function  mode for PB8, PB9 */
    GPIOB->MODER |= (GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1);

    /* Set output type to open drain for PB8, PB9 */
    GPIOB->OTYPER |= (GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);

    /* Set speed mode to medium speed */
    GPIOB->OSPEEDR |= GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0;

    /* Set alternative function to AF4 for pin PB8, PB9. See datasheet. */
    GPIOB->AFR[1] |= (4 << GPIO_AFRH_AFSEL8_Pos) | (4 << GPIO_AFRH_AFSEL9_Pos);
}

/**
 * @brief Initialises hardware i2c for pmi purposes
 * @return See @ref PMI_RC_E
 */
int32_t i2c_hw_init(void)
{
    /* Init gpios */
    gpio_init();

    /* Enable I2C1 clock */
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    /* Disable I2C Module for configuration */
    I2C1->CR1 &= ~I2C_CR1_PE;

    /* Set prescaler and timings */
    I2C1->TIMINGR |=
        (0x01 << I2C_TIMINGR_PRESC_Pos) | (0x02 << I2C_TIMINGR_SCLDEL_Pos)
        | (0x02 << I2C_TIMINGR_SDADEL_Pos) | (0x07 << I2C_TIMINGR_SCLL_Pos)
        | (0x07 << I2C_TIMINGR_SCLH_Pos);

    /* Automatic end mode: STOP condition is automatically sent after tx */
    I2C1->CR2 &= ~I2C_CR2_AUTOEND;

    /* Enable I2C module */
    I2C1->CR1 |= I2C_CR1_PE;

    return RC_SUCC;
}

/**
 * @brief Sets number of bytes to receive / transmit
 * @param size[in] Number of bytes
 */
static void config_nbytes(uint8_t size)
{
    if (!size)
    {
        return;
    }

    /* Clear all bits */
    I2C1->CR2 &= ~I2C_CR2_NBYTES;

    /* Set size */
    I2C1->CR2 |= (size << I2C_CR2_NBYTES_Pos);
    /* Disable reload transfer finished after */
    I2C1->CR2 &= ~I2C_CR2_RELOAD;
}

/**
 * @brief Transmits an amount of data over i2c
 * @param addr 7 bit device address
 * @param *buf Data buffer
 * @param buf_size Size of buffer
 * @param reg_addr Register address of device
 * @param use_reg_addr Set to 0 to disable setting register address of device
 */
int32_t __hw_tx(uint8_t addr, uint8_t *buf,  uint8_t buf_size, uint8_t reg_addr,
    uint8_t use_reg_addr)
{
    int32_t rc = RC_TIMEOUT;
    uint64_t t;

    t = systick_get() + I2C_BUS_TIMEOUT_MS;
    /* Check if busy */
    while (I2C1->ISR & I2C_ISR_BUSY)
    {
        if (t > systick_get())
        {
            return rc;
        }
    }

    /* Set slave address */
    /* First clear all address bits */
    I2C1->CR2 &= ~I2C_CR2_SADD;
    /* Set slave address (shifted 1 bit to the left) */
    I2C1->CR2 |=  ((addr << 1) << I2C_CR2_SADD_Pos);

    /* Clear read bit for writing mode */
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;

    /* In case of using the register functionality, buf_size has to be
     * increment by one.
     */
    if (use_reg_addr)
    {
        if (buf_size == 0xFF)
        {
            return RC_BUFF_OVERFLOW;
        }

        buf_size++;
    }

    /* Set number of bytes to transmit */
    config_nbytes(buf_size);

    /* Send start condition */
    I2C1->CR2 |= I2C_CR2_START;

    t = systick_get() + I2C_BUS_TIMEOUT_MS;
    do {
        /* Check for errors */
        if (I2C1->ISR & I2C_ISR_NACKF || I2C1->ISR & I2C_ISR_ARLO)
        {
            /* In case of errors restart the I2C peripheral */
            I2C1->CR1 &= ~I2C_CR1_PE;
            /* Ugly, but when cleared, PE must be kept low for at least 3 APB
            *  clock cycles. This is not time critical so use systick here.
            */
            systick_delay_ms(1);
            I2C1->CR1 |= I2C_CR1_PE;

            /* Send Stop condition */
            I2C1->CR2 |= I2C_CR2_STOP;

            return RC_ERR;
        }

        if (use_reg_addr)
        {
            I2C1->TXDR = reg_addr;
            use_reg_addr = 0;
        }
        else if (I2C1->ISR & I2C_ISR_TXIS)
        {
            /* Transmit data */
            I2C1->TXDR = *buf;
            buf++;
            buf_size--;
        }

        /* Configure size of next block if required */
        if (I2C1->ISR & I2C_ISR_TCR)
        {
            config_nbytes(buf_size);
        }

        /* Check for timeout */
        if (t < systick_get())
        {
            return rc;
        }

    /* Loop until transfer is complete */
    } while (!(I2C1->ISR & I2C_ISR_TC));

    /* Send Stop condition */
    I2C1->CR2 |= I2C_CR2_STOP;

    return RC_SUCC;
}

/**
 * @brief Transmits an amount of data over i2c
 * @param addr 7 bit device address
 * @param *buf Data buffer
 * @param buf_size Size of buffer
 */
int32_t i2c_hw_tx(uint8_t addr, uint8_t *buf,  uint8_t buf_size)
{
    return __hw_tx(addr, buf, buf_size, 0, 0);
}

/**
 * @brief Transmits an amount of data over i2c
 * @param addr 7 bit device address
 * @param reg Register address to write to
 * @param *buf Data buffer
 * @param buf_size Size of buffer
 */
int32_t i2c_hw_tx_device(uint8_t addr, uint8_t reg, uint8_t *buf,
    uint8_t buf_size)
{
    return __hw_tx(addr, buf, buf_size, reg, 1);
}

/**
 * @brief Receives an amount of data over i2c
 * @param addr 7 bit device address
 * @param *buf Data buffer
 * @param buf_size: Size of buffer
 * @return See @ref PMI_RC_E
 */
int32_t i2c_hw_rx(uint8_t addr, uint8_t *buf, uint8_t buf_size)
{
    int32_t rc = RC_TIMEOUT;
    uint64_t t;

    t = systick_get() + I2C_BUS_TIMEOUT_MS;
    /* Check if busy */
    while (I2C1->ISR & I2C_ISR_BUSY)
    {
        if (t > systick_get())
        {
            return rc;
        }
    }

    /* Clear buffer */
    memset(buf, 0, buf_size);

    /* Set slave address */
    /* First clear all address bits */
    I2C1->CR2 &= ~I2C_CR2_SADD;
    /* Set slave address (shifted 1 bit to the left), set read bit */
    I2C1->CR2 |= ((addr << 1) << I2C_CR2_SADD_Pos) | I2C_CR2_RD_WRN;

    /* Set number of bytes to receive */
    config_nbytes(buf_size);

    /* Send start condition */
    I2C1->CR2 |= I2C_CR2_START;

    t = systick_get() + I2C_BUS_TIMEOUT_MS;
    do
    {
        /* Check for errors */
        if (I2C1->ISR & I2C_ISR_ARLO)
        {
            /* In case of errors restart the I2C peripheral */
            I2C1->CR1 &= ~I2C_CR1_PE;
            /* Ugly, but when cleared, PE must be kept low for at least 3 APB
            *  clock cycles. This is not time critical so use systick here.
            */
            systick_delay_ms(1);
            I2C1->CR1 |= I2C_CR1_PE;

            /* Send Stop condition */
            I2C1->CR2 |= I2C_CR2_STOP;

            return RC_ERR;
        }

        /* Fetch received bytes */
        if (I2C1->ISR & I2C_ISR_RXNE)
        {
            *buf = (I2C1->RXDR & 0xFF);
            buf++;
            buf_size--;
        }

        /* Configure size of next block if required */
        if (I2C1->ISR & I2C_ISR_TCR)
        {
            config_nbytes(buf_size);
        }

        /* Check for timeout */
        if (t < systick_get())
        {
            return rc;
        }

    /* Loop until transfer is complete */
    } while (!(I2C1->ISR & I2C_ISR_TC));


    /* Send Stop condition */
    I2C1->CR2 |= I2C_CR2_STOP;

    return RC_SUCC;
}

/**
 * @brief Receives an amount of data over i2c
 * @param addr 7 bit device address
 * @param reg Register address to read from
 * @param *buf Data buffer
 * @param buf_size: Size of buffer
 * @return See @ref PMI_RC_E
 */
int32_t i2c_hw_rx_device(uint8_t addr, uint8_t reg, uint8_t *buf,
    uint8_t buf_size)
{
    int32_t rc;

    rc = i2c_hw_tx(addr, &reg, 1);
    if (rc)
    {
        return rc;
    }

    rc = i2c_hw_rx(addr, buf, buf_size);
    return rc;
}
