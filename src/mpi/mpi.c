#include <mpi.h>
#include <stm32l0xx.h>
#include <stm32l0xx.h>

int32_t spi_init_adxl345 ( void )
 {
    // Enable the SPI1 peripheral clock
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;     // Enable GPIOA clock

    // 2. Configure GPIO pins for SPI functions
    // Configure PA5, PA6, PA7 as Alternate function for SPI1 (AF0)
    GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
    GPIOA->MODER |= (GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1); // Set to alternate function
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL5 | GPIO_AFRL_AFSEL6 | GPIO_AFRL_AFSEL7);     // Reset alternate function bits

    // Configure PA12 as GPIO output for CS
    GPIOA->MODER &= ~GPIO_MODER_MODE12;
    GPIOA->MODER |= GPIO_MODER_MODE12_0;   // Set to output mode 01 (first bit 1)

    // 3. Configure and Initialize SPI
    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI; // Master mode, software slave management 11  high: idle, even: data sampled at 2nd edge
    SPI1->CR1 |= SPI_CR1_BR_0;             // Baud rate: fPCLK/4 ??
    SPI1->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA; // Clock polarity and phase, change as needed

    SPI1->CR1 &= ~SPI_CR1_DFF;              // 8-bit data frame format
    SPI1->CR1 |= SPI_CR1_SPE;              // Enable SPI

    return RC_SUCC;
 }


 int32_t spi_txrx(uint8_t *buf, uint32_t size)
 {
//   The function iterates over the buffer, processing one byte at a time.
// Transmit: It waits for the transmit buffer to be empty (checked using SPI_SR_TXE in the SPI1->SR register) and then writes a byte from the buffer to the SPI1->DR register to transmit it.
// Receive: It waits for the receive buffer to be not empty (checked using SPI_SR_RXNE in the SPI1->SR register) and then reads a byte from the SPI1->DR register. The received byte can be stored back in the buffer or processed as needed.


 if (buf == NULL || size == 0) {
        return RC_ERR; // Return error if buffer is null or size is 0
    }

    for (uint8_t i = 0; i < size; i++) {
        // Wait until the transmit buffer is empty
        while (!(SPI1->SR & SPI_SR_TXE));

        // Send a byte
        SPI1->DR = buf[i];

        // Wait until the receive buffer is not empty
        while (!(SPI1->SR & SPI_SR_RXNE));

        // Read the received byte
        buf[i] = SPI1->DR;
    }

    return RC_SUCC;
 }