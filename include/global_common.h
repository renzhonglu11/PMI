#ifndef GLOBAL_COMMON_H
#define GLOBAL_COMMON_H

#include <stm32l0xx.h>
#include "pmi_stddefs.h"
#include "ili9341.h"

#define BUFFER_SIZE 240
#define PRE_TRIGGER_COUNT 120
#define POST_TRIGGER_COUNT 120


extern const float DIVISOR_MV;
extern uint8_t rc_range;
extern uint32_t p2p_val;
extern volatile uint8_t zoom_lvl;
extern uint16_t final_time_period;
extern volatile uint8_t debounce_in_progress;
extern uint16_t trigger_index;

uint16_t adc_buffer[BUFFER_SIZE];
uint16_t extracted_data[BUFFER_SIZE];


extern volatile uint8_t graph_ready;
extern volatile uint8_t data_ready;



uint32_t extract_samples(uint16_t *extracted_data);
uint8_t power_of_2(uint8_t exponent);

#endif /* GLOBAL_COMMON_H */