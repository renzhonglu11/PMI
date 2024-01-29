#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include "pmi_stddefs.h"


void displayValues(uint8_t zoomLevel,uint16_t txt_color);
uint32_t draw_graph(uint16_t* buffer, int buffer_size, uint16_t color, uint16_t line_color);
// uint32_t draw_graph(uint16_t* buffer, int buffer_size, uint16_t color, uint8_t triiger_index);

#endif /* GRAPH_UTILS_H */