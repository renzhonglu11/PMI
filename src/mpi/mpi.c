#include <mpi.h>
#include <stm32l0xx.h>

 int32_t spi_init_adxl345 ( void )
 {
    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR; 
    SPI1->CR2 = SPI_CR2_SSOE | SPI_CR2_RXNEIE; 
    SPI1->CR1 |= SPI_CR1_SPE; 
 }