#include "common.h"

/**
 * @brief Necessary utilities to draw a graph and its metrics on the LCD
 */

void ili9341_draw_thick_line_horizontal(int16_t x, int16_t y, uint16_t color, uint8_t thickness);
uint32_t display_error(uint16_t buffer[], uint16_t line_color);

uint8_t error_flag = 0;
uint16_t bg_color = BG_COLOR;

uint32_t draw_graph(uint16_t buffer[], int buffer_size, uint16_t curve_color, uint16_t line_color)
{

  if (GPIOC->ODR & GPIO_ODR_OD6)
  {
    GPIOC->BSRR = GPIO_BSRR_BR_6; // Reset PC4 (set it to 0) if it is currently set
  }
  else
  {
    GPIOC->BSRR = GPIO_BSRR_BS_6; // Set PC4 (set it to 1) if it is currently reset
  }

  struct display_info_s display_info = ili9341_display_info_get();

  // Determine scaling factors based on the display size and ADC range
  uint16_t x_scale = display_info.width / buffer_size;
  float y_scale = 64.0 / 4096.0; // Now y_scale is a floating-point number

  uint16_t initial_val = 0;
  uint16_t y_max = 0;
  uint8_t rc_flag = 0;
  int8_t find_flag = 0;
  uint8_t x_max = 0;
  uint16_t y_min = 999;

  error_flag = 0;
  display_error(buffer, line_color);
  if (error_flag)
  {
    return RC_PARAM_INVALID;
  }

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
    if (i > 0 && (buffer[i] >= buffer[i - 1]) && y_max <= buffer[i])
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
    ili9341_draw_thick_line_horizontal(x, y, curve_color, 2);
    find_flag = 0;
  }

  p2p_val = (y_max - y_min) * 5;

  if (initial_val != 0)
  {
    ili9341_line_draw(initial_val, 140,
                      initial_val, 140 - 64, line_color);
  }

  return RC_SUCC;
}

void ili9341_draw_thick_line_horizontal(int16_t x, int16_t y, uint16_t curve_color, uint8_t thickness)
{
  for (int i = 0; i < thickness; i++)
  {
    ili9341_pixel_set(x, y + i, curve_color);
  }
}

uint32_t display_error(uint16_t buffer[], uint16_t line_color)
{
  uint8_t only_falling_edge = 1;
  // no charging edge, return error
  for (uint8_t i = 1; i < 10; i++)
  {
    if (buffer[i - 1] < buffer[i])
    {
      only_falling_edge = 0;
    }
  }


  

  if (only_falling_edge || (buffer[119] - buffer[120] > 4000))
  {
    error_flag = 1;
    ili9341_text_pos_set(5, 3);
    ili9341_str_print("ERROR!!!", line_color, BG_COLOR);
    return RC_PARAM_INVALID;
  }

  return RC_SUCC;
}

void displayValues(uint8_t zoomLevel, uint16_t txt_color)
{
  int yPos = 8; // Example Y position, adjust based on your graph position

  // display template
  display_template();
  uint32_t averageValue, peakToPeakValue, rcTime, capacitanceValue;
  float timeSpan;
  char valueString[30], labelString[30], floatBuf[32];

  // Clear and refresh only values, keep labels static
  for (int i = 0; i < NUM_DISPLAY_LINES; i++)
  {
    ili9341_text_pos_set(7, yPos + i); // 50 is an arbitrary value, adjust as per your layout
    ili9341_str_clear(8, bg_color);    // Clear space for the value
  }

  if (error_flag)
  {
    sprintf(labelString, "%d", zoomLevel);
    ili9341_text_pos_set(7, yPos + 5);
    ili9341_str_print(labelString, txt_color, bg_color);
    return;
  }

  get_metrics(&averageValue, &rcTime, &timeSpan, &capacitanceValue);
  peakToPeakValue = p2p_val;

  float values[NUM_DISPLAY_LINES] = {
      (float)averageValue / DIVISOR_MV,
      (float)peakToPeakValue / DIVISOR_MV,
      (float)rcTime,
      (float)capacitanceValue,
      timeSpan,
      (float)zoomLevel};

  for (int i = 0; i < NUM_DISPLAY_LINES - 1; i++)
  {
    float2str(floatBuf, 30, values[i], 1);
    sprintf(valueString, "%s", floatBuf);
    ili9341_text_pos_set(7, yPos + i);
    ili9341_str_print(valueString, txt_color, bg_color);
  }
  // For Zoom level, no need for float conversion
  sprintf(valueString, "%d", zoomLevel);
  ili9341_text_pos_set(7, yPos + 5);
  ili9341_str_print(valueString, txt_color, bg_color);
}