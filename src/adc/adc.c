#include "adc.h"


/**
 * 开启PIN口和ADC1时钟，PIN为模拟输入
 * 复位ADC1和设置prescale
 * 初始化ADC1参数
 * enable ADC1和calibration
 * 获取ADC1值
*/


uint32_t adc_init()
{
  RCC->APB2ENR |= RCC_APB2ENR_ADCEN; // Enable ADC clock


  // ADC prescale?

  // ensure adc is disabled

  // clear clock mode first


  // set alignment to right (set ADSTART to 0 first)


  

  // enable adc

  /*
    calibration:
      1. enable calibration
      2. start calibration
      3. wait for calibration to finish
  */



  return RC_SUCC;
}


uint32_t get_adc()
{
  // set sample time (using largest sample time for now)
  // select adc channel for PC5



  // enable adc


  return RC_SUCC;
}