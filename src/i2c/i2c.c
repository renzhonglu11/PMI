#include <i2c.h>


/**
 * init i2c
 * i2c start state, i2c stop state, i2c write bits/bytes, i2c read bits bytes
*/

int32_t i2c_init()
{
  // enable clock

  // PB8->SCL, PB9->SDA

  // set SCL and SDA as idle high

}


void I2C_start(void)
{
  // HIGH to LOW transition of SDA while SCL is HIGH
  
}


void I2C_stop(void)
{
  // LOW to HIGH transition of SDA while SCL is HIGH
}


void I2C_WriteBit(uint8_t bit) 
{}


uint8_t I2C_ReadBit(void) 
{}



void I2C_WriteByte(uint8_t byte)
{}

uint8_t I2C_ReadByte(uint8_t ack)
{}