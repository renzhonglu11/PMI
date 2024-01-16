#include "graph_utils.h"

/**
 * @brief Necessary utilities to draw a graph and its metrics on the LCD
 */


void ili9341_draw_thick_line_horizontal(int16_t x, int16_t y, uint16_t color, uint8_t thickness);




uint32_t draw_graph(uint16_t buffer[], int buffer_size, uint16_t color, uint16_t line_color)
{

  // Clear the display or just the area where you will draw the graph
  ili9341_rect_fill(0, 0, ili9341_display_info_get().width, ili9341_display_info_get().height, BG_COLOR);

  struct display_info_s display_info = ili9341_display_info_get();

  // Determine scaling factors based on the display size and ADC range
  uint16_t x_scale = display_info.width / buffer_size;
  // uint16_t x_scale = display_info.width / 120;
  // uint16_t y_scale = 62 / 4096;  // Assuming 12-bit ADC
  float y_scale = 64.0 / 4096.0; // Now y_scale is a floating-point number

  uint16_t initial_val = 0;
  uint16_t y_max = 0;
  uint8_t rc_flag = 0;
  int8_t find_flag = 0;
  uint8_t x_max = 0;
  uint16_t y_min = 999;

  // Plot the ADC values
  for (uint8_t i = 0; i < buffer_size; i++)
  {

    int16_t x = i * x_scale;
    int16_t y = 140 - (int16_t)(buffer[i] * y_scale); // set 140 as the offset first
    if (y_min > buffer[i])
    {
      y_min = buffer[i];
    }

    // charging
    if (i > 0 && buffer[i] > buffer[i - 1])
    {
      initial_val = i;
      y_max = buffer[i];
      x_max = i;
      find_flag = 1;
    }

    // discharging
    if (i > 0 && !find_flag && !rc_flag)
    {
      uint16_t target_val = (y_max * 368) / 1000; // easy numeric approximation
      if (buffer[i] <= target_val)
      {
        rc_flag = 1;
        rc_range = i - x_max;
        ili9341_line_draw(x, 140, x, 140 - 64, line_color);
      }
    }

    // ili9341_pixel_set(x, y, ILI9341_COLOR_BLUE);
    ili9341_draw_thick_line_horizontal(x, y, color, 2);
    find_flag = 0;
  }

  p2p_val = (y_max - y_min) * 5;

  ili9341_line_draw(initial_val, 140,
                    initial_val, 140 - 64, line_color);


  if (GPIOC->ODR & GPIO_ODR_OD6)
  {
    GPIOC->BSRR = GPIO_BSRR_BR_6; // Reset PC4 (set it to 0) if it is currently set
  }

  return RC_SUCC;
}

void ili9341_draw_thick_line_horizontal(int16_t x, int16_t y, uint16_t color, uint8_t thickness)
{
  for (int i = 0; i < thickness; i++)
  {
    ili9341_pixel_set(x, y + i, color);
  }
}

void get_metrics(uint32_t *average_voltage, uint32_t *rc_time_microseconds, float *timeSpan, uint32_t *capacitanceValue)
{

  *timeSpan = (float)(BUFFER_SIZE * 400 / 1000);

  uint32_t sampling_time_microseconds = 400; // 400 microseconds
  *rc_time_microseconds = rc_range * sampling_time_microseconds;
  *capacitanceValue = *rc_time_microseconds;

  // sample frequence = 400us
  // 1/400us = 2500Hz, sample time = 1/2500 = 0.4ms

  uint32_t average_voltage_tmp = 0;

  for (uint8_t i = 0; i < elements_of(extracted_data); i++)
  {
    average_voltage_tmp += extracted_data[i];
  }

  average_voltage_tmp = average_voltage_tmp * 5 / elements_of(extracted_data);
  *average_voltage = average_voltage_tmp;
}

void displayValues(uint8_t zoomLevel)
{
  // TODO: display all the values on the LCD

  uint32_t averageValue;
  uint32_t peakToPeakValue;
  float timeSpan;
  uint32_t rcTime;
  uint32_t capacitanceValue;


  get_metrics(&averageValue, &rcTime, &timeSpan, &capacitanceValue);

  char displayString[30];
  char floatBuf[32];
  int yPos = 8; // Example Y position, adjust based on your graph position

  peakToPeakValue = p2p_val;
  
  float2str(floatBuf, 30, (float)averageValue / DIVISOR_MV, 2);
  sprintf(displayString, "Avg: %s mV", floatBuf);
  ili9341_text_pos_set(0, yPos++);
  ili9341_str_print(displayString, TXT_COLOR, BG_COLOR);

  float2str(floatBuf, 30, (float)peakToPeakValue / DIVISOR_MV, 2);
  sprintf(displayString, "P-P: %s mV", floatBuf);
  ili9341_text_pos_set(0, yPos++);
  ili9341_str_print(displayString, TXT_COLOR, BG_COLOR);

  float2str(floatBuf, 30, (float)rcTime, 2);
  sprintf(displayString, "RC: %s us", floatBuf);
  ili9341_text_pos_set(0, yPos++);
  ili9341_str_print(displayString, TXT_COLOR, BG_COLOR);

  float2str(floatBuf, 30, (float)capacitanceValue, 2);
  sprintf(displayString, "C: %s nF", floatBuf);
  ili9341_text_pos_set(0, yPos++);
  ili9341_str_print(displayString, TXT_COLOR, BG_COLOR);

  float2str(floatBuf, 30, timeSpan, 2);
  sprintf(displayString, "Span: %s ms", floatBuf);
  ili9341_text_pos_set(0, yPos++);
  ili9341_str_print(displayString, TXT_COLOR, BG_COLOR);

  sprintf(displayString, "Zoom: %d", zoomLevel);
  ili9341_text_pos_set(0, yPos);
  ili9341_str_print(displayString, TXT_COLOR, BG_COLOR);
}