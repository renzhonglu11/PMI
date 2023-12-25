#include <my_utils.h>

#define MAX_STRING_LENGTH 200



/**
 * @brief Clears the specified line on the display.
 *
 * @param line The line number to clear.
 */
void ClearLine(uint8_t line)
{
  struct display_info_s dispInfo = ili9341_display_info_get();
    uint16_t lineHeight = dispInfo.font_height; // Assuming one line per font height
    uint16_t yStart = line * lineHeight;

    ili9341_rect_fill(0, yStart, dispInfo.width, lineHeight, ILI9341_COLOR_BLACK);
}




/**
 * @brief Displays the sensor data.
 *
 * This function takes in the accelerometer and magnetometer data along with the temperature
 * and displays it.
 *
 * @param acc_x The X-axis accelerometer value.
 * @param acc_y The Y-axis accelerometer value.
 * @param acc_z The Z-axis accelerometer value.
 * @param mag_x The X-axis magnetometer value.
 * @param mag_y The Y-axis magnetometer value.
 * @param mag_z The Z-axis magnetometer value.
 * @param temperature The temperature value.
 */
void display_sensor_data(float acc_x, float acc_y, float acc_z, int16_t mag_x, int16_t mag_y, int16_t mag_z, float temperature)
{
  char strBuffer[120];
  char floatBuf[32]; // Buffer for individual float-to-string conversions
  uint16_t strLength = sizeof(strBuffer);

  // Clear lines before updating
  for (uint8_t i = 0; i < 7; i++)
  {
    ClearLine(i);
  }

  // Convert and format Accelerometer Data
  float2str(floatBuf, sizeof(floatBuf), acc_x, 2);
  snprintf(strBuffer, strLength, "ACC X = \t %s", floatBuf);
  ili9341_text_pos_set(0, 0); // Set position for first line
  ili9341_str_print(strBuffer, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

  float2str(floatBuf, sizeof(floatBuf), acc_y, 2);
  snprintf(strBuffer, strLength, "ACC Y = \t %s", floatBuf);
  ili9341_text_pos_set(0, 1); // Set position for second line
  ili9341_str_print(strBuffer, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

  float2str(floatBuf, sizeof(floatBuf), acc_z, 2);
  snprintf(strBuffer, strLength, "ACC Z = \t %s", floatBuf);
  ili9341_text_pos_set(0, 2); // Set position for third line
  ili9341_str_print(strBuffer, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

  // Format and display Magnetometer Data
  snprintf(strBuffer, strLength, "MAG X = \t %d", mag_x);
  ili9341_text_pos_set(0, 3); // Set position for fourth line
  ili9341_str_print(strBuffer, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

  snprintf(strBuffer, strLength, "MAG Y = \t %d", mag_y);
  ili9341_text_pos_set(0, 4); // Set position for fifth line
  ili9341_str_print(strBuffer, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

  snprintf(strBuffer, strLength, "MAG Z = \t %d", mag_z);
  ili9341_text_pos_set(0, 5); // Set position for sixth line
  ili9341_str_print(strBuffer, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

  // Convert and format Temperature Data
  float2str(floatBuf, sizeof(floatBuf), temperature, 2);
  snprintf(strBuffer, strLength, "TEMP: %s C", floatBuf);
  ili9341_text_pos_set(0, 6); // Set position for seventh line
  ili9341_str_print(strBuffer, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

}

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

  display_sensor_data(acc_x, acc_y, acc_z, mag_x, mag_y, mag_z, temperature);
 

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


/**
 * @brief Sends sensor data over UART.
 * 
 * This function is responsible for sending sensor data over UART communication.
 * It can be used to transmit sensor readings to another device or system.
 * 
 * @note Make sure to initialize the UART communication before calling this function.
 * 
 * @return None.
 */
void send_sensor_data_over_UART()
{
  char sensorDataString[MAX_STRING_LENGTH];
  get_output_data(sensorDataString);

  // Send the string over UART
  uart_tx_str(sensorDataString);
}
