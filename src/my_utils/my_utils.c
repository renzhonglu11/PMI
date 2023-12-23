#include <my_utils.h>

uint32_t _get_formated_qmc5883_data(char *output_str);
uint32_t _get_formated_adxl345_data(char *output_str);

/// @brief Output the formating string of data from all sensors
/// @param
/// @return RC_SUCC
uint32_t output_data(void)
{
  float x = 0.0;
  float y = 0.0;
  float z = 0.0;

  char output_str[50];

  _get_formated_adxl345_data(output_str);
  uart_tx_str(output_str);
  uart_tx_char('\n');

  memset(output_str, 0, sizeof(output_str)); // empty output_str

  _get_formated_qmc5883_data(output_str);
  uart_tx_str(output_str);
  uart_tx_char('\n');

  return RC_SUCC;
}

uint32_t _get_formated_adxl345_data(char *output_str)
{
  float x = 0.0;
  float y = 0.0;
  float z = 0.0;

  // TODO: formating string better

  int32_t success_get_data = adxl345_acc_data(&x, &y, &z);


  char x_accel_str[30];
  char y_accel_str[30];
  char z_accel_str[30];
  char x_str[30];
  char y_str[30];
  char z_str[30];
  float2str(x_accel_str, elements_of(x_accel_str), x, 4);
  float2str(y_accel_str, elements_of(y_accel_str), y, 4);
  float2str(z_accel_str, elements_of(z_accel_str), z, 4);

  sprintf(x_str, "x  %s \n", x_accel_str);
  uart_tx_str(x_str);
  sprintf(y_str, "y  %s \n", y_accel_str);
  uart_tx_str(y_str);
  sprintf(z_str, "z  %s \n", z_accel_str);
  uart_tx_str(z_str);

  // strncat(output_str, ", ", sizeof(output_str) - strlen(output_str) - 1);

  // // Convert y to string and append to output_str
  // float2str(output_str + strlen(output_str), elements_of(output_str) - strlen(output_str), y, 3);

  // // Append another delimiter
  // strncat(output_str, ", ", sizeof(output_str) - strlen(output_str) - 1);

  // // Convert z to string and append to output_str
  // float2str(output_str + strlen(output_str), elements_of(output_str) - strlen(output_str), z, 3);

  return RC_SUCC;
}

uint32_t _get_formated_qmc5883_data(char *output_str)
{
  int16_t mag_x, mag_y, mag_z;
  qmc5883l_get_raw_data(&mag_x, &mag_y, &mag_z);
  sprintf(output_str, "x = %d, y= %d, z= %d", mag_x, mag_y, mag_z);

  return RC_SUCC;
}
