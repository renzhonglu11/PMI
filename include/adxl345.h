#ifndef ADXL345_H
#define ADXL345_H

#include <pmi_stddefs.h>
#include <stm32l0xx.h>


// Registers' Address 
#define DEVID (uint8_t)0x0
#define BW_RATE (uint8_t)0x2C 
#define POWER_CTL (uint8_t)0x2D   // power-saving feature control





// GPIO definition
#define ADXLCS_GPIO_Port     GPIOA






#endif /* ADXL345_H */