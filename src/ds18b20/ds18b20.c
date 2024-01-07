#include <ds18b20.h>


void DS18B20_Reset();
uint8_t DS18B20_Check(void);


/**
 * @brief Initializes the DS18B20 temperature sensor.
 *
 * This function initializes the DS18B20 temperature sensor by performing any necessary setup
 * operations. It returns a uint8_t value indicating the success or failure of the initialization.
 *
 * @return uint8_t Returns 0 if the initialization is successful, otherwise returns an error code.
 */
uint8_t init_ds18b20()
{
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN; // enable clock for GPIOB

  ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13);  // clear mode bits for pin 13
  ONE_WIRE_PORT->MODER |= (GPIO_MODER_MODE13_0); // set pin as output

  ONE_WIRE_PORT->OTYPER |= (GPIO_OTYPER_OT_13); // set pin as open-drain

  // ONE_WIRE_PORT->PUPDR &= ~(GPIO_PUPDR_PUPD13); // clear bits for pull-up
  // ONE_WIRE_PORT->PUPDR |= GPIO_PUPDR_PUPD13_0;  // enable pull-up
  SET_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPD13_0); // Enable pull-up

  ONE_WIRE_PORT->BSRR = OW_PIN;
  DS18B20_Reset();

  return DS18B20_Check();
}

/**
 * @brief Resets the DS18B20 temperature sensor.
 */
void DS18B20_Reset()
{

  ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13);  // clear mode bits for pin 13
  ONE_WIRE_PORT->MODER |= (GPIO_MODER_MODE13_0); // set pin as output

  // pull the line low
  ONE_WIRE_PORT->BRR = OW_PIN;
  OW_DELAY_RST();
  // release the line
  ONE_WIRE_PORT->BSRR = OW_PIN;

  // set pin 13 as input mode
  OW_DELAY_RST_WAIT();


}

/**
 * @brief Writes a single bit to the DS18B20 sensor.
 *
 * This function is used to write a single bit to the DS18B20 sensor.
 *
 * @param bit The bit value to be written (0 or 1).
 */
void DS18B20_WriteBit(uint8_t bit)
{
  // Set the pin as output
  ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13);  // clear mode bits for pin 13
  ONE_WIRE_PORT->MODER |= (GPIO_MODER_MODE13_0); // set pin as output

  if (bit)
  {
    // Write '1'
    GPIOB->BRR |= GPIO_BRR_BR_13; // Pull low
    DELAY1US();
    GPIOB->BSRR |= GPIO_BSRR_BS_13; // Release line
    OW_DELAY_WRITE();
  }
  else
  {
    // Write '0'
    GPIOB->BRR |= GPIO_BRR_BR_13; // Pull low
    OW_DELAY_WRITE();
    GPIOB->BSRR |= GPIO_BSRR_BS_13; // Release line
    DELAY1US();
  }

}

/**
 * @brief Writes a byte to the DS18B20 sensor.
 *
 * This function is used to write a byte of data to the DS18B20 sensor.
 *
 * @param byte The byte of data to be written.
 */
void DS18B20_WriteByte(uint8_t byte)
{
  uint8_t byte_mask = 0x01;

  for (int i = 0; i < 8; i++)
  {
    DS18B20_WriteBit(byte & byte_mask); // Write the LSB
    byte_mask = byte_mask << 1;
  }
}

/**
 * @brief Reads a single bit from the DS18B20 temperature sensor.
 * 
 * @return The value of the read bit (0 or 1).
 */
uint8_t DS18B20_ReadBit(void)
{
  uint8_t bit;

  ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13);  // clear mode bits for pin 13
  ONE_WIRE_PORT->MODER |= (GPIO_MODER_MODE13_0); // set pin as output

  // Initiate read timeslot
  ONE_WIRE_PORT->BRR = OW_PIN; // Pull low
  DELAY2US(); 
  ONE_WIRE_PORT->BSRR = OW_PIN; // release line

  // Configure PB13 as input
  ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13); // clear mode bits for pin 13

  // DELAY12US();
  nop_100(); nop_50(); nop_30();  // better!!

  bit = (GPIOB->IDR & OW_PIN) ? 1 : 0;

  // OW_DELAY_READ(); // 50 us
  nop_100(); nop_50(); nop_30();nop_500();nop_50();  // better!!

  return bit;
}

/**
 * @brief Reads a byte from the DS18B20 temperature sensor.
 *
 * @return The byte read from the sensor.
 */
uint8_t DS18B20_ReadByte(void)
{
  
  uint8_t i = 8, byte = 0;
	while (i--) {
		byte >>= 1;
		byte |= (DS18B20_ReadBit() << 7);
	}
	
	return byte;
}

/**
 * @brief Checks the status of the DS18B20 temperature sensor.
 * @return The status of the DS18B20 temperature sensor.
 */
uint8_t DS18B20_Check(void)
{
  uint8_t retry = 0;
  // set pin 13 as input mode
  ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13); // clear mode bits for pin 13

  while (((ONE_WIRE_PORT->IDR & OW_PIN) == OW_PIN) && retry < 220)
  {
    retry++;
    DELAY1US();
  }
  if (retry >= 220)
    return 1;
  else
    retry = 0;

  while (((ONE_WIRE_PORT->IDR & OW_PIN) != OW_PIN) && retry < 255)
  {
    retry++;
    DELAY1US();
  }
  if (retry > 255)
    return 1;
  return 0;
}


/**
 * @brief Enables the strong pull-up feature for the DS18B20 temperature sensor.
 * 
 * This function is used to enable the strong pull-up feature of the DS18B20 temperature sensor.
 * The strong pull-up feature is used to provide additional power to the sensor during temperature
 * conversion, which can reduce conversion time. This function should be called before starting
 * temperature conversion.
 */
void EnableStrongPullUp(void) {
    ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13); // Clear mode bits for PB13
    ONE_WIRE_PORT->MODER |= GPIO_MODER_MODE13_0; // Set PB13 as General Purpose Output Mode
    ONE_WIRE_PORT->OTYPER &= ~(GPIO_OTYPER_OT_13); // Set to Push-Pull
    ONE_WIRE_PORT->BSRR = OW_PIN; // Drive the line high
}

/**
 * @brief Disables the strong pull-up feature of the DS18B20 temperature sensor.
 */
void DisableStrongPullUp(void) {
    ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13); // Clear mode bits for PB13
    ONE_WIRE_PORT->MODER |= GPIO_MODER_MODE13_0; // Set PB13 as Output
    ONE_WIRE_PORT->OTYPER |= GPIO_OTYPER_OT_13; // Set back to Open-Drain
    ONE_WIRE_PORT->PUPDR |= GPIO_PUPDR_PUPD13_0; // Enable Pull-Up
}


/**
 * @brief Starts the DS185B20 temperature sensor.
 */
void DS185B20_Start()
{
  DS18B20_Reset();
  DS18B20_Check();
  DS18B20_WriteByte(0xCC);
  DS18B20_WriteByte(DS18B20_CMD_CONVERTTEMP);
  
}


/**
 * @brief Retrieves the temperature from the DS18B20 sensor.
 *
 * This function reads the temperature value from the DS18B20 sensor and stores it in the provided variable.
 *
 * @param temperature Pointer to a float variable where the temperature value will be stored.
 * @return uint32_t Returns a status code indicating the success or failure of the operation.
 */
uint32_t DS18B20_Get_Temp(float * temperature)
{
  uint16_t temp_raw = 0;  
  uint8_t lsb, msb;
  uint8_t temp;

  // Start temperature conversion
  DS185B20_Start();
  
  // WE NEED STRONG PULL UP TO ENSURE SUFFICIENT SUPPLY CURRENT!! (ds18b20 page 7)
  EnableStrongPullUp();
  // systick_delay_ms(200); // after changing to faster resolution, delay is now shorter now.
  systick_delay_ms(800);  
  DisableStrongPullUp();

  // ready to read data from sensor
  DS18B20_Reset();
  DS18B20_Check();
  DS18B20_WriteByte(0xCC);
  DS18B20_WriteByte(DS18B20_CMD_READSCRATCHPAD);

  
  // Read the first two bytes (LSB and MSB) from the scratchpad
  lsb = DS18B20_ReadByte();
  msb = DS18B20_ReadByte();


  temp_raw = ((msb << 8) | lsb);
  if (temp_raw & 0x8000)
  {
    temp_raw = (~temp_raw +1);
    *temperature = -(float) temp_raw / 16.0;
    return RC_SUCC;
  }
  
  *temperature = (float) temp_raw / 16.0;

  return RC_SUCC;
}


/**
 * @brief Sets the DS18B20 temperature sensor to a faster resolution.
 * 
 * This function adjusts the resolution of the DS18B20 temperature sensor to a faster setting.
 * The resolution determines the number of bits used to represent the temperature value, which
 * affects the accuracy and conversion time of the sensor. By setting a faster resolution, the
 * sensor will provide temperature readings more quickly, but with reduced accuracy.
 * 
 * @return The new resolution setting of the DS18B20 temperature sensor.
 */
uint32_t DS18B20_faster_resolution(void)
{
  DS18B20_Reset();
  DS18B20_Check();
  DS18B20_WriteByte(0xCC);
  DS18B20_WriteByte(0x4e);
  DS18B20_WriteByte(0x00);   // useless
  DS18B20_WriteByte(0x00);   // useless
  DS18B20_WriteByte(0x1f);

  return RC_SUCC;
}