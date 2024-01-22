#ifndef GLOBAL_COMMON_H
#define GLOBAL_COMMON_H

#include <stm32l0xx.h>
#include "pmi_stddefs.h"

extern const float DIVISOR_MV;
extern uint8_t rc_range;
extern uint32_t p2p_val;
extern volatile uint8_t zoom_lvl;
extern uint16_t final_time_period;


#define BUFFER_SIZE 240
#define PRE_TRIGGER_COUNT 120
#define POST_TRIGGER_COUNT 120


#endif /* GLOBAL_COMMON_H */