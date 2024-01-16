#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <pmi_stddefs.h>
#include "ili9341.h"
#include <stm32l0xx.h>
#include <systick.h>
#include "pmi.h"

#define CURVE_COLOR         ILI9341_COLOR_WHITE
#define BG_COLOR            ILI9341_COLOR_BLACK
#define TXT_COLOR           ILI9341_COLOR_WHITE


uint32_t draw_graph(uint16_t* buffer, int buffer_size, uint16_t color, uint16_t line_color);
void get_metrics(uint32_t *average_voltage, uint32_t *rc_time_microseconds, float *timeSpan, uint32_t *capacitanceValue);
void displayValues(uint8_t zoomLevel);
// uint32_t draw_graph(uint16_t* buffer, int buffer_size, uint16_t color, uint8_t triiger_index);

#endif /* GRAPH_UTILS_H */