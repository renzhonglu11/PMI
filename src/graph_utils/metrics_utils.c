#include "common.h"


void get_metrics(uint32_t *average_voltage, float *time_period, float *timeSpan, float *capacitanceValue)
{

  *timeSpan = (float)(BUFFER_SIZE * 400 / 1000);
  
  uint32_t sampling_time_microseconds = 400; // 400 microseconds
  
  *time_period = final_time_period * sampling_time_microseconds;

  // *capacitanceValue = *time_period;
  
  *capacitanceValue = (float)(*time_period) / 2.197224577;         // nanofarads
  
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