#include <qmc5883l.h>

void qmc5883l_write_reg(uint8_t reg, uint8_t data)
{
  uint8_t buf[2];
  buf[0] = reg;
  buf[1] = data;

  i2c_write(QMC5883L_ADDR, buf, 2);
}

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

int32_t qmc5883l_init(void)
{
  i2c_init();
  return RC_SUCC;
}

int32_t _qmc5883l_internal_test()
{
  uint8_t data_buf[1];
  char hexStr[10];

  qmc5883l_read_reg(0x0D, data_buf, elements_of(data_buf));
  
  sprintf(hexStr, "0x%X", data_buf[0]);
  uart_tx_str("device_id: ");
  uart_tx_str(hexStr);
  uart_tx_char('\n');

  return RC_SUCC;
}