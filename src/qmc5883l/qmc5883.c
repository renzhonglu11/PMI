#include <qmc5883l.h>


/// @brief write data to register of qmc5883
/// @param reg Register address to be written
/// @param data Data to be read
void qmc5883l_write_reg(uint8_t reg, uint8_t data)
{
  I2C_start();
  I2C_WriteByte(QMC5883L_ADDR << 1);
  I2C_WriteByte(reg);
  I2C_WriteByte(data);
  I2C_stop();
}


/// @brief Read data from register of qmc5883
/// @param reg_address Register address to be read
/// @param data Data buffer to store read data
/// @param numBytes Size of data buffer
void qmc5883l_read_reg(uint8_t reg_address, uint8_t *data, uint8_t numBytes)
{
  // Start, write the device address with write bit
  I2C_start();
  I2C_WriteByte(QMC5883L_ADDR << 1);

  // Write the register address
  I2C_WriteByte(reg_address);

  // Start again, write the device address with read bit
  I2C_start();
  I2C_WriteByte((QMC5883L_ADDR << 1) | 1);

  // Read the data bytes
  for (uint8_t i = 0; i < numBytes; i++)
  {
    if (i == (numBytes - 1))
    {
      // Send NACK after the last byte
      data[i] = I2C_ReadByte(1);
    }
    else
    {
      // Send ACK after all bytes except the last
      data[i] = I2C_ReadByte(0);
    }
  }

  // Stop the communication
  I2C_stop();
}

/// @brief Initialize qmc5883
/// @param  
/// @return RC_SUCC
int32_t qmc5883l_init(void)
{
  i2c_init();

  // config qmc5883l (other configurations are set to default first)
  // TODO: whether need to set other configuration

  qmc5883l_write_reg(ConfigReg1,ContinuousConversion|Full_Scale_2G);  // set the mode to continuous mode


  return RC_SUCC;
}



/// @brief Testing the connection to qmc5883 with I2C
/// @return 
int32_t _qmc5883l_internal_test()
{
  uint8_t data_buf[1];
  char hexStr[10];

  qmc5883l_read_reg(0x0D, data_buf, elements_of(data_buf));
  
  sprintf(hexStr, "0x%02X", data_buf[0]);   // change bin to hex with format 0x00
  uart_tx_str("device_id: ");
  uart_tx_str(hexStr);
  uart_tx_char('\n');

  return RC_SUCC;
}

/// @brief Get the raw data x, y, z from the magnet sensor.
/// @param mag_x x coordination
/// @param mag_y y coordination
/// @param mag_z z coordination
void qmc5883l_get_raw_data(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z)
{
  uint8_t data_buf[6];   // sign 8-bits integer: -32768 and 32767

  qmc5883l_read_reg(MAG_DATA_REGISTER,data_buf,elements_of(data_buf));

  *mag_x = (int16_t)(data_buf[0] | data_buf[1] << 8);
  *mag_y = (int16_t)(data_buf[2] | data_buf[3] << 8);
  *mag_z = (int16_t)(data_buf[4] | data_buf[5] << 8);


  // convert data into milli gauss
  // 0.92?? 
  *mag_x *= 0.92;
  *mag_y *= 0.92;
  *mag_z *= 0.92;


}

