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


  // using clock that is derived from APB clock
  // clear clock mode first

  // select clock mode
  ADC1->CFGR2 &= (~ADC_CFGR2_CKMODE); // clear mode bits 
  ADC1->CFGR2 |= ADC_CFGR2_CKMODE_0;
  

  // set alignment to right (set ADSTART to 0 first)
  

  // enable adc



  /*
    calibration:
      1. Ensure that ADEN = 0 and DMAEN = 0.
      2. Set ADCAL = 1.
      3. Wait until ADCAL = 0 (or until EOCAL = 1). This can be handled by interrupt if the 
      interrupt is enabled by setting the EOCALIE bit in the ADC_IER register. The ADCAL 
      bit can remain set for some time even after EOCAL has been set. As a result, the 
      software must wait for ADCAL = 0 after EOCAL = 1 to be able to set ADEN = 1 for next 
      ADC conversions.
      4. The calibration factor can be read from bits 6:0 of ADC_DR or ADC_CALFACT 
      registers.
  */

  // ensure ADEN to be 0 before calibration
  if((ADC1->CR & ADC_CR_ADEN)!=0)
  {
    ADC1->CR |= ADC_CR_ADDIS;    // disable ADC
  }

  ADC1->CR |= ADC_CR_ADCAL;    // start calibration

  while ((ADC1->ISR & ADC_ISR_EOCAL)==0)
  {
    
  }

  ADC1->ISR |= ADC_ISR_EOCAL;  // write 1 to clear bit 
  


  return RC_SUCC;
}


uint32_t get_adc()
{
  // set sample time (using largest sample time for now)
  // select adc channel for PC5



  // enable adc


  return RC_SUCC;
}