#include <ds18b20.h>

#define DS18B20_CMD_CONVERTTEMP 0x44
#define DS18B20_CMD_READSCRATCHPAD 0xBE


void OneWire_Reset();
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
  OneWire_Reset();

  // return RC_SUCC;
  return DS18B20_Check();
}

void OneWire_Reset()
{

  uint8_t response;

  ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13);  // clear mode bits for pin 13
  ONE_WIRE_PORT->MODER |= (GPIO_MODER_MODE13_0); // set pin as output

  // pull the line low
  ONE_WIRE_PORT->BRR = OW_PIN;
  OW_DELAY_RST();
  // release the line
  ONE_WIRE_PORT->BSRR = OW_PIN;

  // set pin 13 as input mode
  // ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13); // clear mode bits for pin 13
  OW_DELAY_RST_WAIT();
  
  // response = (ONE_WIRE_PORT->IDR & OW_PIN) ? 0 : 1;

  // return response;
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
    DELAY2US();
    GPIOB->BSRR |= GPIO_BSRR_BS_13; // Release line
    OW_DELAY_WRITE();
  }
  else
  {
    // Write '0'
    GPIOB->BRR |= GPIO_BRR_BR_13; // Pull low
    OW_DELAY_WRITE();
    GPIOB->BSRR |= GPIO_BSRR_BS_13; // Release line
    DELAY2US();
  }

  // // Recovery time
  // for (int i = 0; i < 4; i++) { // 4 * 20 nops = ~80 cycles
  //     nop_20();
  // }
}

void OneWire_WriteByte(uint8_t byte)
{
  uint8_t byte_mask = 0x01;

  for (int i = 0; i < 8; i++)
  {
    DS18B20_WriteBit(byte & byte_mask); // Write the LSB
    // byte >>= 1; // Shift right to move the next bit into the LSB position
    byte_mask = byte_mask << 1;
  }
}

int OneWire_ReadBit(void)
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

  DELAY12US();

  bit = (GPIOB->IDR & OW_PIN) ? 1 : 0;

  OW_DELAY_READ(); // Complete the timeslot

  return bit;
}

uint8_t DS18B20_ReadByte(void)
{
  uint8_t byte = 0;

  for (int i = 0; i < 8; i++)
  {
    if (OneWire_ReadBit())
    {
      byte |= (1 << i);
    }
  }

  return byte;
}

uint8_t DS18B20_Check(void)
{
  uint8_t retry = 0;
  // set pin 13 as input mode
  ONE_WIRE_PORT->MODER &= ~(GPIO_MODER_MODE13); // clear mode bits for pin 13

  while(((ONE_WIRE_PORT->IDR & OW_PIN)==1)&&retry<200)
  {
    retry++;
    DELAY1US();
  }
  if(retry>=200)return 1;
  else retry=0;

  while(((ONE_WIRE_PORT->IDR & OW_PIN)!=1)&&retry<240)
  {
    retry++;
    DELAY1US();

  }
  if(retry>=240) return 1;
  return 0;
}


void DS185B20_Start()
{
  OneWire_Reset();
  DS18B20_Check();
  OneWire_WriteByte(0xCC);
  OneWire_WriteByte(0x44);
}


float DS18B20_Get_Temp(void)
{
  uint16_t temp_raw;
  float temperature;

  // Reset and check if the device is present


  // Send 'Skip ROM' command to address all devices
  DS185B20_Start();
  OneWire_Reset();
  DS18B20_Check();
  // Start temperature conversion
  // OneWire_WriteByte(DS18B20_CMD_CONVERTTEMP);

  // systick_delay_ms(800);

  // if (!OneWire_Reset())
  // {
  //   return -999.0; // Error or no device present
  // }

  // // Send 'Skip ROM' command again before reading scratchpad
  // OneWire_WriteByte(0xCC);

  // OneWire_WriteByte(DS18B20_CMD_READSCRATCHPAD);

  // // Read the first two bytes (LSB and MSB) from the scratchpad
  // uint8_t lsb = DS18B20_ReadByte();
  // uint8_t msb = DS18B20_ReadByte();

  // temp_raw = (msb << 8) | lsb;

  // // Convert raw value to Celsius
  // temperature = (float)temp_raw / 16.0;

  return temperature;
}