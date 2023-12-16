#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>
#include <qmc5883l.h>

/* Time in milliseconds to abort communication if no acknowledge is sent */
#define I2C_BUS_TIMEOUT_MS 10


#define QMC5883L_ADDR 0x0D     //address for the sensor 0x0D = 000 1101



#define GPIO_PIN_8 (1U << 8) // User-defined macro for GPIO pin 8
#define GPIO_PIN_9 (1U << 9) // User-defined macro for GPIO pin 8

#define SCL_PIN GPIO_PIN_8
#define SDA_PIN GPIO_PIN_9
#define I2C_GPIO_PORT GPIOB

// Use SDA as input or output
#define SDA_INPUT GPIOB->MODER &= ~(GPIO_MODER_MODE9) // set PB9 to 00
#define SDA_OUTPUT GPIOB->MODER |= GPIO_MODER_MODE9_0
#define SCL_INPUT GPIOB->MODER &= ~(GPIO_MODER_MODE8) // set PB8

// check if SDA is high or low (IDR?)
#define IS_SDA_HIGH GPIOB->IDR &GPIO_IDR_ID9_Msk
#define IS_SCL_HIGH GPIOB->IDR &GPIO_IDR_ID8_Msk

// Macros for bit manipulation
#define SDA_HIGH() (I2C_GPIO_PORT->BSRR = SDA_PIN)
#define SDA_LOW()  (I2C_GPIO_PORT->BRR = SDA_PIN)
// #define SDA_LOW() (I2C_GPIO_PORT->BSRR = (uint32_t)SDA_PIN << 16U)
#define SCL_HIGH() (I2C_GPIO_PORT->BSRR = SCL_PIN)
#define SCL_LOW()  (I2C_GPIO_PORT->BRR = SCL_PIN)
// #define SCL_LOW() (I2C_GPIO_PORT->BSRR = (uint32_t)SCL_PIN << 16U)
#define SDA_READ() ((I2C_GPIO_PORT->IDR & SDA_PIN) != 0)



#define I2C_DELAY nop_20()

// SDA: low, high; SCL: low, high
// #define SDA_HIGH GPIOB->BSRR |= GPIO_BSRR_BS_9
// #define SDA_LOW GPIOB->BSRR |= GPIO_BSRR_BR_9
// #define SCL_HIGH GPIOB->BSRR |= GPIO_BSRR_BS_8
// #define SCL_LOW GPIOB->BSRR |= GPIO_BSRR_BR_8


int32_t i2c_init();
void I2C_start(void);
void I2C_stop(void);
void I2C_WriteBit(uint8_t bit);
uint8_t I2C_WriteByte(uint8_t byte);
uint8_t I2C_ReadBit(void);
uint8_t I2C_ReadByte(uint8_t ack);
void i2c_write(int8_t address, uint8_t* buf, int16_t size);



#endif /* I2C_H */
