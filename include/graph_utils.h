#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <pmi_stddefs.h>
#include "ili9341.h"
#include <stm32l0xx.h>



#define CURVE_COLOR         ILI9341_COLOR_BLUE
#define BG_COLOR            ILI9341_COLOR_WHITE


uint32_t draw_graph(uint16_t* buffer, int buffer_size, uint16_t color);

#endif /* GRAPH_UTILS_H */