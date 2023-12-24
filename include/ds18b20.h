#ifndef DS18B20_H
#define DS18B20_H

#include <stdint.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>
#include <pmi_string.h>
#include <stdio.h>
#include <systick.h>


#define ONE_WIRE_PORT GPIOB
#define GPIO_PIN_13 (1U << 13) // User-defined macro for GPIO pin 13


#define OW_PIN GPIO_PIN_13
#define DS18B20_CMD_CONVERTTEMP 0x44
#define DS18B20_CMD_READSCRATCHPAD 0xBE



#define nop_100() nop_50(); nop_50()
#define nop_200() nop_100(); nop_100()
#define nop_400() nop_200(); nop_200()
#define nop_500() nop_400(); nop_100()
#define nop_600() nop_500(); nop_100()
#define nop_1000() nop_500(); nop_500()
#define nop_2000() nop_1000(); nop_1000()
#define nop_4000() nop_2000(); nop_2000()
#define nop_5000() nop_4000(); nop_1000()


/**
 * DELAY MATTERS!!!
 * 1/16MHZ = 62.5
 * 750 microseconds/62.5 nanoseconds = 7680 nops
 * 15 microseconds/62.5 nanoseconds = 240 nops
 * 60 microseconds/62.5 nanoseconds = 960 nops
 * 50 microseconds/62.5 nanoseconds = 800 nops
 * 1 microseconds/62.5 nanoseconds = 16 nops
 * 2 microseconds/62.5 nanoseconds = 32 nops
 * 5 microseconds/62.5 nanoseconds = 80 nops
 * 12 microseconds/62.5 nanoseconds = 192 nops
*/

#define OW_DELAY_RST()        nop_5000(); nop_5000(); nop_2000();nop_10()
#define OW_DELAY_RST_WAIT()   nop_200();nop_20();nop_20();nop_10()
#define OW_DELAY_WRITE()      nop_500();nop_400();nop_50();nop_10();nop_10()
#define OW_DELAY_READ()       nop_400();nop_100(); nop_200(); nop_50(); nop_30(); nop_5()
#define DELAY1US()            nop_10();nop_5(); nop_1()
#define DELAY2US()            nop_20();nop_10();nop_1()
#define DELAY5US()            nop_50();nop_20();nop_10()
#define DELAY12US()           nop_50();nop_50();nop_50();nop_20();nop_20()


uint8_t init_ds18b20();
uint32_t DS18B20_Get_Temp(float * temperature);
uint32_t DS18B20_faster_resolution(void);

#endif /* DS18B20_H */