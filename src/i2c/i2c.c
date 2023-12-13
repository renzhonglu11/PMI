#include <i2c.h>


/**
 * init i2c
 * i2c start state, i2c stop state, i2c write bits/bytes, i2c read bits bytes
*/

int32_t i2c_init()
{
  // enable clock of port B
  RCC->IOPENR |= RCC_IOPENR_IOPBEN; 
  // PB8->SCL, PB9->SDA
  SCL_INPUT;


  

  // set SCL and SDA as idle high
  SDA_HIGH;
  SCL_HIGH;

  if(IS_SDA_HIGH && IS_SCL_HIGH)
  {
    // SCL and SDA should be high at start condition
    return RC_SUCC;
  }

  return RC_ERR;

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