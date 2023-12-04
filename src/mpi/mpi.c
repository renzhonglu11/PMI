#include <mpi.h>
#include <stm32l0xx.h>
#include <stdio.h>


// for more information see datasheet adxl345 since page 14


int32_t spi_init_adxl345 ( void )
 {
   /*
      1. configure clock
      2. configure CPOL and CPHA
      3. configure SSM, SSI and MSTR
      4. set data frame size DFF
   */



    // Enable the SPI1 peripheral clock
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
   //  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;     // Enable GPIOA clock
   RCC->APB2ENR |= RCC_IOPENR_GPIOAEN; //enable GPIOA clk


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
    
    SPI1->CR1 |= SPI_CR1_BR_1;       // Baud rate: fPCLK/8

    SPI1->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA; // Clock polarity and phase, change as needed 11

    SPI1->CR1 &= ~SPI_CR1_DFF;              // 8-bit data frame format
    SPI1->CR1 |= SPI_CR1_SPE;              // Enable SPI

    return RC_SUCC;
 }


 int32_t spi_txrx(uint8_t *buf, uint32_t size)
 {
   /*
   The function iterates over the buffer, processing one byte at a time.
   Transmit: It waits for the transmit buffer to be empty (checked using SPI_SR_TXE in the SPI1->SR register) and then writes
   a byte from the buffer to the SPI1->DR register to transmit it.
   Receive: It waits for the receive buffer to be not empty (checked using SPI_SR_RXNE in the SPI1->SR register) 
   and then reads a byte from the SPI1->DR register. The received byte can be stored back in the buffer or processed as needed.
   */


   

   // pull down CS line of the corresponding GPIO
   GPIOA->BSRR |= GPIO_BSRR_BR_12;   // resset PA12



   if (buf == NULL || size == 0) {
         return RC_ERR; // Return error if buffer is null or size is 0
      }

      while (((SPI1->SR)&(SPI_SR_BSY))) {};
      
      for (uint8_t i = 0; i < size; i++) {
         // Wait until the transmit buffer is empty
         while (!(SPI1->SR & SPI_SR_TXE));

         // Send a byte
         SPI1->DR = buf[i];

         // Wait until the receive buffer is not empty
         while (!(SPI1->SR & SPI_SR_RXNE));

         // Read the received byte
         buf[i] = SPI1->DR;
         // uart_tx_int(buf[i]);
      }


      // pull up CS again
      GPIOA->BSRR |= GPIO_BSRR_BS_12;    // set PA12 

      return RC_SUCC;
 }