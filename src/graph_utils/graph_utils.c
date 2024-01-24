#include "common.h"


/**
 * @brief Necessary utilities to draw a graph and its metrics on the LCD
 */




uint8_t error_flag = 0;
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

  int16_t last_rising_edge = -1;
  uint16_t total_time_period = 0;
  uint32_t time_period_counter = 0;

  uint16_t y_max = 0;
  uint8_t rc_flag = 0;
  int8_t find_flag = 0;
  uint8_t x_max = 0;
  uint16_t y_min = 4095;

  if(buffer[0] <= 3 && buffer[2] <= 3 && buffer[4] <= 3)
  {
    error_flag = 1;
    ili9341_text_pos_set(5, 3);
    ili9341_str_print("ERROR!!!", ILI9341_COLOR_RED, BG_COLOR);
    return RC_PARAM_INVALID;
  }


  error_flag = 0;

  // Plot the ADC values
  for (uint8_t i = 0; i < buffer_size; i++)
  {

    int16_t x = i * x_scale;
    int16_t y = 140 - (int16_t)(buffer[i] * y_scale); // set 140 as the offset first
    if (y_min >= buffer[i])
    {
      y_min = buffer[i];
    }

    if (y_max <= buffer[i])
    {
      y_max = buffer[i];
    }

    // charging
    if (i > 0 && buffer[i] > buffer[i - 1])
    {
      
      // y_max = buffer[i];
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
        // ili9341_line_draw(x, 140, x, 140 - 64, line_color);
      }
    }

    // maybe there are still electric charge in the capacitor??
    // There will be part of the discharge curve on the left most side of the screen. 
    // That means several values at the beginning of buffer are greater than 2048, we dont want to draw line for such values. 
    if(buffer[i] >= 2048 && buffer[i-1] <= 2048 && i > 5)   
    { 
      uint16_t x_tmp = i * x_scale;
      ili9341_line_draw(x_tmp, 140, x_tmp, 140 - 64, line_color);

      if(last_rising_edge!=-1)
      {
        uint16_t cur_time = (i-last_rising_edge);
        total_time_period += cur_time;
      }

      last_rising_edge = i;
      time_period_counter++;
    }


   
    ili9341_draw_thick_line_horizontal(x, y, color, 2);
    find_flag = 0;
  }






  p2p_val = (y_max - y_min) * 5;
  final_time_period = total_time_period/(time_period_counter-1);
  // ili9341_line_draw(initial_val, 140,
  //                   initial_val, 140 - 64, line_color);


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



void displayValues(uint8_t zoomLevel)
{
  // TODO: display all the values on the LCD

  uint32_t averageValue;
  uint32_t peakToPeakValue;
  float timeSpan;
  float time_period;
  float capacitanceValue;

  if(error_flag)
  {
    return;
  }


  get_metrics(&averageValue, &time_period, &timeSpan, &capacitanceValue);

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

  float2str(floatBuf, 30, (float)time_period, 2);
  sprintf(displayString, "T: %s us", floatBuf);
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