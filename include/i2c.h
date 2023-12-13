#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>

/* Time in milliseconds to abort communication if no acknowledge is sent */
#define I2C_BUS_TIMEOUT_MS      10

// Use SDA as input or output
#define SDA_INPUT GPIOB->MODER &= ~(GPIO_MODER_MODE9) // set PB9 to 00
#define SDA_OUTPUT GPIOB->MODER |= GPIO_MODER_MODE9_0
#define SCL_INPUT GPIOB->MODER &= ~(GPIO_MODER_MODE8) // set PB8 

// check if SDA is high or low (IDR?)
#define IS_SDA_HIGH GPIOB->IDR & GPIO_IDR_ID9_Msk
#define IS_SCL_HIGH GPIOB->IDR & GPIO_IDR_ID8_Msk

// SDA: low, high; SCL: low, high
#define SDA_HIGH GPIOB->BSRR |= GPIO_BSRR_BS_9
#define SDA_LOW GPIOB->BSRR |= GPIO_BSRR_BR_9
#define SCL_HIGH GPIOB->BSRR |= GPIO_BSRR_BS_8
#define SCL_LOW GPIOB->BSRR |= GPIO_BSRR_BR_8




#endif /* I2C_H */
