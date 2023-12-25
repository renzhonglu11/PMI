#ifndef SPI_H
#define SPI_H

#include <pmi_stddefs.h>
#include <uart.h>
#include <stm32l0xx.h>
#include <ili9341.h>
#include <clocks.h>
#include <adxl345.h>
#include <stdio.h>
#include <pmi_string.h>
#include <systick.h>
#include <my_utils.h>
#include <i2c.h>


int32_t spi_init_adxl345 ( void );
int32_t spi_txrx ( uint8_t * buf , uint32_t size );
uint32_t init_proj_2(void);

#endif /* SPI_H */