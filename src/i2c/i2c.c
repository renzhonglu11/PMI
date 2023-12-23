#include <i2c.h>
#include <uart.h>

/// @brief Initialize software-based I2C using open-drain and push-up
/// @return RC_ERR
int32_t i2c_init()
{
  // enable clock of port B
  SET_BIT(RCC->IOPENR, RCC_IOPENR_IOPBEN);
  // PB8->SCL, PB9->SDA
  CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODE8 | GPIO_MODER_MODE9); // Clear mode bits for pins 8 and 9
  SET_BIT(GPIOB->MODER, GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0); // Set pins as output

  SET_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);    // Set pins as open-drain
  SET_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD9_0); // Enable pull-up

  // Set SDA and SCL to high (idle state)
  SET_BIT(GPIOB->BSRR, SDA_PIN | SCL_PIN);


  return RC_ERR;
}

/// @brief Start state of I2C
/// @param  
void I2C_start(void)
{
  // HIGH to LOW transition of SDA while SCL is HIGH
  // ensure SDA is high first
  SDA_HIGH();
  SCL_HIGH();
  I2C_DELAY;
  SDA_LOW();
  I2C_DELAY;
  SCL_LOW();
  I2C_DELAY;
}


/// @brief Stop state of I2C
/// @param  
void I2C_stop(void)
{
  // LOW to HIGH transition of SDA while SCL is HIGH
  SDA_LOW();
  SCL_HIGH();
  I2C_DELAY;
  SDA_HIGH();
  I2C_DELAY;
}

/// @brief Write a bit with I2C
/// @param bit 
void I2C_WriteBit(uint8_t bit)
{
  if (bit)
  {
    SDA_HIGH();
  }
  else
  {
    SDA_LOW();
  }
  I2C_DELAY;
  SCL_HIGH();
  I2C_DELAY;
  SCL_LOW();
  I2C_DELAY;
}

/// @brief Write a byte with I2C
/// @param byte byte to be written to I2C
/// @return ACK = 0 if slave replay or NACK = 1 if no reply
uint8_t I2C_WriteByte(uint8_t byte)
{
  
  for (int i = 0; i < 8; i++)
  {
    I2C_WriteBit((byte & 0x80) != 0); // get MSB
    byte <<= 1;
  }
  uint8_t ack =  I2C_ReadBit(); // Ack bit
  return ack==0;
}

/// @brief Read a bit with I2C
/// @param  
/// @return Bit to be read
uint8_t I2C_ReadBit(void)
{
  uint8_t bit;
  SDA_HIGH();   // master must first release SDA so that the slave can pull down the line
  I2C_DELAY;
  SCL_HIGH();   // master release SCL and is ready to read SDA
  I2C_DELAY;
  bit = SDA_READ();   // it makes sense here, it wont always 1 because slave may pull SDA low
  SCL_LOW();
  I2C_DELAY;
  return bit;
}

/// @brief Read a byte with I2C
/// @param ack Ack bit. 1: stop reading, 0: continue to read
/// @return 
uint8_t I2C_ReadByte(uint8_t ack)
{
  uint8_t byte = 0;
  for (int i = 0; i < 8; i++)
  {
    byte = (byte << 1) | I2C_ReadBit();
  }
  I2C_WriteBit(ack);  // 1 bit ACK
  return byte;
}

/// @brief Read Ack
/// @param  
/// @return ACK=0 or NACK=1
uint8_t I2C_ReadAck(void) {
    uint8_t ack;

    SDA_HIGH();  // Release the SDA line for the slave to pull it low
    SCL_HIGH();  // Clock out the acknowledgment bit
    ack = SDA_READ();  // Read the acknowledgment bit
    SCL_LOW();   // Complete the clock cycle

    return ack == 0;  // An ACK is indicated by a low level on SDA
}

// This function is not used currently
void i2c_write(int8_t address, uint8_t* buf, int16_t size){

  // send start condition
  I2C_start();
  // send slave address
  I2C_WriteByte((address<<1));
  // send data
  for (int i = 0; i < size; i++)
  {
    I2C_WriteByte(buf[i]);
  }
  // send stop condition
  I2C_stop();
}
