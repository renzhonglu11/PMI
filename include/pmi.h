#ifndef PMI_H
#define PMI_H

#include <stm32l0xx.h>
#include "pmi_stddefs.h"
#include "uart.h"
#include "adc.h"






uint32_t TIM6_init();
uint32_t TIM2_init();
uint32_t initialize_gpio();

#endif /* PMI_H */