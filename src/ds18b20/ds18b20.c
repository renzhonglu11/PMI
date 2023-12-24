#include <ds18b20.h>


void DS18B20_Reset();
uint8_t DS18B20_Check(void);


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

  // return RC_SUCC;
  return DS18B20_Check();
}

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

void DS18B20_WriteByte(uint8_t byte)
{
  uint8_t byte_mask = 0x01;

  for (int i = 0; i < 8; i++)
  {
    DS18B20_WriteBit(byte & byte_mask); // Write the LSB
    byte_mask = byte_mask << 1;
  }
}

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

uint8_t DS18B20_ReadByte(void)
{
  
  uint8_t i = 8, byte = 0;
	while (i--) {
		byte >>= 1;
		byte |= (DS18B20_ReadBit() << 7);
	}
	
	return byte;
}


uint8_t DS18B20_Check(void)
{
  uint8_t retry = 0;
  // set pin 13 as input mode
  ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13); // clear mode bits for pin 13

  while (((ONE_WIRE_PORT->IDR & OW_PIN) == 1) && retry < 220)
  {
    retry++;
    DELAY1US();
  }
  if (retry >= 220)
    return 1;
  else
    retry = 0;

  while (((ONE_WIRE_PORT->IDR & OW_PIN) != 1) && retry < 255)
  {
    retry++;
    DELAY1US();
  }
  if (retry >= 255)
    return 1;
  return 0;
}


void EnableStrongPullUp(void) {
    ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13); // Clear mode bits for PB13
    ONE_WIRE_PORT->MODER |= GPIO_MODER_MODE13_0; // Set PB13 as General Purpose Output Mode
    ONE_WIRE_PORT->OTYPER &= ~(GPIO_OTYPER_OT_13); // Set to Push-Pull
    ONE_WIRE_PORT->BSRR = OW_PIN; // Drive the line high
}

void DisableStrongPullUp(void) {
    ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13); // Clear mode bits for PB13
    ONE_WIRE_PORT->MODER |= GPIO_MODER_MODE13_0; // Set PB13 as Output
    ONE_WIRE_PORT->OTYPER |= GPIO_OTYPER_OT_13; // Set back to Open-Drain
    ONE_WIRE_PORT->PUPDR |= GPIO_PUPDR_PUPD13_0; // Enable Pull-Up
}


void DS185B20_Start()
{
  DS18B20_Reset();
  DS18B20_Check();
  DS18B20_WriteByte(0xCC);
  DS18B20_WriteByte(DS18B20_CMD_CONVERTTEMP);
  
}




uint32_t DS18B20_Get_Temp(float * temperature)
{
  uint16_t temp_raw = 0;  
  uint8_t lsb, msb;
  uint8_t temp;

  // Start temperature conversion
  DS185B20_Start();
  
  // WE NEED STRONG PULL UP TO ENSURE SUFFICIENT SUPPLY CURRENT!! (ds18b20 page 7)
  EnableStrongPullUp();
  systick_delay_ms(110);  // after changing to faster resolution, delay is now shorter now.
  DisableStrongPullUp();

  // ready to read data from sensor
  DS18B20_Reset();
  DS18B20_Check();
  DS18B20_WriteByte(0xCC);
  DS18B20_WriteByte(DS18B20_CMD_READSCRATCHPAD);

  
  // Read the first two bytes (LSB and MSB) from the scratchpad
  lsb = DS18B20_ReadByte();
  msb = DS18B20_ReadByte();


  temp_raw = (uint16_t)((msb << 8) | lsb);
  *temperature = (float) temp_raw / 16.0;

  return RC_SUCC;
}


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