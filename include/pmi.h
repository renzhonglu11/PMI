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
#include "timer.h"
#include "global_common.h"



uint16_t adc_buffer[BUFFER_SIZE];
uint16_t extracted_data[BUFFER_SIZE];



extern volatile uint8_t graph_ready;
extern volatile uint8_t data_ready;



uint32_t initialize_gpio();
uint32_t extract_samples(uint16_t *extracted_data);
uint32_t initialize_project();

#endif /* PMI_H */