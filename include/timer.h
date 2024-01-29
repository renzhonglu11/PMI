#ifndef TIMER_H
#define TIMER_H

#include <stm32l0xx.h>
#include "pmi_stddefs.h"
#include "pmi.h"




uint32_t TIM6_init();
uint32_t TIM2_init();
uint32_t TIM21_init();
uint32_t reset_TIM2_zoom_level();


#endif /* TIMER_H */