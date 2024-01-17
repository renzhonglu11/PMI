#include "common.h"


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