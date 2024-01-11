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
  


    // Configure the ADC
    ADC1->CFGR1 &= ~ADC_CFGR1_RES;  // Set ADC resolution (e.g., 12-bit, modify as needed)
    // ADC1->CFGR1 &= ~ADC_CFGR1_EXTEN; // Clear the EXTEN bits first
    // ADC1->CFGR1 |= ADC_CFGR1_EXTEN_1; // Set EXTEN to 10 for trigger detection on falling edge
    // ADC1->CFGR1 |= ADC_CFGR1_EXTSEL_0; // Select the external trigger (e.g., TIM2_TRGO, modify as needed)

    // select clock mode
    ADC1->CFGR2 &= (~ADC_CFGR2_CKMODE); // clear mode bits 
    
    // select channel
    ADC1->CHSELR |= ADC_CHSELR_CHSEL15; // select channel 5 (PC5)

    // set alignment to right (set ADSTART to 0 first)
    
    // sampling time selection
    if((ADC1->CR&ADC_CR_ADSTART)!=0)
    {
      ADC1->CR |= ADC_CR_ADSTP; // stop conversion
    }


    // conversion time: sampling time + 12.5 ADC clock cycles
    // 239.5 + 12.5 = 252 ADC clock cycles
    // 252 /16mhz = 15.75 us
    

    ADC1->SMPR |= ADC_SMPR_SMP_0|ADC_SMPR_SMP_1|ADC_SMPR_SMP_2; // set sampling time to 239.5 ADC clock cycles
    


    ADC1->CR |= ADC_CR_ADEN; // enable ADC


    while(!(ADC1->ISR & ADC_ISR_ADRDY)); // wait until ADC is ready
    
      
    


    return RC_SUCC;





}


uint32_t get_adc()
{
  // set sample time (using largest sample time for now)
  // select adc channel for PC5



  // enable adc


  return RC_SUCC;
}