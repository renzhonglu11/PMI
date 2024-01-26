#ifndef COMMON_H
#define COMMON_H

#include "graph_utils.h"
#include "ili9341.h"
#include "pmi.h"
#include <string.h>
#include "common.h"
#include "global_common.h"

#define CURVE_COLOR         ILI9341_COLOR_WHITE
#define BG_COLOR            ILI9341_COLOR_BLACK
#define TXT_COLOR           ILI9341_COLOR_WHITE

#define NUM_DISPLAY_LINES 6


extern uint8_t error_flag;



void get_metrics(uint32_t *average_voltage, float *time_period, float *timeSpan, float *capacitanceValue);
uint32_t display_template();


#endif /* COMMON_H */