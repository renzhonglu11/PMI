#include <my_utils.h>

#define MAX_STRING_LENGTH 200

uint32_t _get_formated_qmc5883_data(char *output_str);
uint32_t _get_formated_adxl345_data(char *output_str);

/// @brief Output the formating string of data from all sensors
/// @param
/// @return RC_SUCC
uint32_t get_output_data(char *outputString)
{

  float acc_x, acc_y, acc_z;
  int16_t mag_x, mag_y, mag_z;
  float temperature;
  char tempString[MAX_STRING_LENGTH];
  char floatBuf[32]; // Buffer for individual float-to-string conversions

  adxl345_acc_data(&acc_x, &acc_y, &acc_z);
  qmc5883l_get_raw_data(&mag_x, &mag_y, &mag_z);
  DS18B20_Get_Temp(&temperature);

  // Convert accelerometer data
  float2str(floatBuf, sizeof(floatBuf), acc_x, 2);
  snprintf(tempString, MAX_STRING_LENGTH, "ACC: X=%s ", floatBuf);
  float2str(floatBuf, sizeof(floatBuf), acc_y, 2);
  strncat(tempString, "Y=", MAX_STRING_LENGTH - strlen(tempString));
  strncat(tempString, floatBuf, MAX_STRING_LENGTH - strlen(tempString));
  float2str(floatBuf, sizeof(floatBuf), acc_z, 2);
  strncat(tempString, " Z=", MAX_STRING_LENGTH - strlen(tempString));
  strncat(tempString, floatBuf, MAX_STRING_LENGTH - strlen(tempString));

  // Add magnetometer data
  snprintf(floatBuf, sizeof(floatBuf), " | MAG: X=%d Y=%d Z=%d", mag_x, mag_y, mag_z);
  strncat(tempString, floatBuf, MAX_STRING_LENGTH - strlen(tempString));

  // Convert and add temperature data
  float2str(floatBuf, sizeof(floatBuf), temperature, 2);
  strncat(tempString, " | TEMP: ", MAX_STRING_LENGTH - strlen(tempString));
  strncat(tempString, floatBuf, MAX_STRING_LENGTH - strlen(tempString));
  strncat(tempString, "C", MAX_STRING_LENGTH - strlen(tempString));

  // Copy to output string
  strncpy(outputString, tempString, MAX_STRING_LENGTH);



  return RC_SUCC;
}

void send_sensor_data_over_UART()
{
  char sensorDataString[MAX_STRING_LENGTH];
  get_output_data(sensorDataString);

  // Send the string over UART
  uart_tx_str(sensorDataString);
}

