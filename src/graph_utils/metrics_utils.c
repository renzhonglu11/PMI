#include "common.h"

/**
 * @brief Calculates various metrics based on the given data.
 *
 * This function calculates the average voltage, time period, time span, and capacitance value
 * based on the provided data. The average voltage is calculated by summing up all the extracted
 * data points and dividing it by the number of elements. 
 * The time period is calculated using the final time period and the zoom level. 
 * The time span is * calculated by multiplying the buffer size with the sampling time. 
 * The capacitance value is calculated by dividing the time period by a
 * constant value. All the calculated metrics are stored in the respective output parameters.
 *
 * @param average_voltage Pointer to store the calculated average voltage.
 * @param time_period Pointer to store the calculated time period.
 * @param timeSpan Pointer to store the calculated time span.
 * @param capacitanceValue Pointer to store the calculated capacitance value.
 */
void get_metrics(uint32_t *average_voltage, float *time_period, float *timeSpan, float *capacitanceValue)
{
  // sample frequence = 400us
  // 1/400us = 2500Hz, sample time = 1/2500 = 0.4ms
  // total sampling time = 400us * 240 = 96ms
  *timeSpan = (float)(BUFFER_SIZE * 400 / 1000);

  uint32_t sampling_time_microseconds = 400; // 400 microseconds

  // time period = (final time period * sampling time) / 2^zoom level
  // final_time_period = the average of the time periods
  *time_period = (final_time_period * sampling_time_microseconds) / power_of_2(zoom_lvl);

  // (delta t)/ R*ln(3)
  *capacitanceValue = (float)(*time_period) / (2.197224577); // numerical approximation



  uint32_t average_voltage_tmp = 0;

  for (uint8_t i = 0; i < elements_of(extracted_data); i++)
  {
    average_voltage_tmp += extracted_data[i];
  }

  average_voltage_tmp = average_voltage_tmp * 5 / elements_of(extracted_data);
  *average_voltage = average_voltage_tmp;    // 5 * average_voltage_tmp
}



/**
 * @brief Displays a template on the ILI9341 display.
 * 
 * This function displays a template on the ILI9341 display. It prints various labels and units on the display.
 * The labels include "AVG:", "P-P:", "T:", "C:", "Span:", and "Zoom:". The units include "mV", "us", "nF", and "ms".
 * 
 * @return Returns RC_SUCC indicating successful execution.
 */
uint32_t display_template()
{

  const uint8_t column_units = 15;
  uint16_t bg_clr = ILI9341_COLOR_BLACK;
  uint16_t txt_clr = ILI9341_COLOR_WHITE;

  ili9341_text_pos_set(0, 8);
  ili9341_str_print("AVG:", txt_clr, bg_clr);

  ili9341_text_pos_set(column_units, 8);
  ili9341_str_print("mV", txt_clr, bg_clr);

  ili9341_text_pos_set(0, 9);
  ili9341_str_print("P-P:", txt_clr, bg_clr);

  ili9341_text_pos_set(column_units, 9);
  ili9341_str_print("mV", txt_clr, bg_clr);

  ili9341_text_pos_set(0, 10);
  ili9341_str_print("T:", txt_clr, bg_clr);

  ili9341_text_pos_set(column_units, 10);
  ili9341_str_print("us", txt_clr, bg_clr);

  ili9341_text_pos_set(0, 11);
  ili9341_str_print("C:", txt_clr, bg_clr);

  ili9341_text_pos_set(column_units, 11);
  ili9341_str_print("nF", txt_clr, bg_clr);

  ili9341_text_pos_set(0, 12);
  ili9341_str_print("Span:", txt_clr, bg_clr);

  ili9341_text_pos_set(column_units, 12);
  ili9341_str_print("ms", txt_clr, bg_clr);

  ili9341_text_pos_set(0, 13);
  ili9341_str_print("Zoom:", txt_clr, bg_clr);

  return RC_SUCC;
}