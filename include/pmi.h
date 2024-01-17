#ifndef PMI_H
#define PMI_H

#include <stm32l0xx.h>
#include "pmi_stddefs.h"
#include "uart.h"
#include "adc.h"
#include <systick.h>
#include <graph_utils.h>
#include <pmi_string.h>
#include <stdio.h>


extern const float DIVISOR_MV;
extern uint8_t rc_range;
extern uint32_t p2p_val;
extern uint8_t zoom_lvl;


#define BUFFER_SIZE 240
#define PRE_TRIGGER_COUNT 120
#define POST_TRIGGER_COUNT 120

uint16_t adc_buffer[BUFFER_SIZE];
uint16_t extracted_data[BUFFER_SIZE];



extern uint8_t graph_ready;
extern uint8_t data_ready;

uint32_t TIM6_init();
uint32_t TIM2_init();
uint32_t initialize_gpio();
uint32_t extract_samples(uint16_t *extracted_data);


#endif /* PMI_H */