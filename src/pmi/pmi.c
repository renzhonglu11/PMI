#include "pmi.h"


#define BUFFER_SIZE 240
uint16_t adc_buffer[BUFFER_SIZE];
uint16_t buffer_index = 0;
enum PMI_BOOL_E edge_detected = FALSE;
uint16_t adc_threshold = 2048;  // Mid-range threshold for 12-bit ADC



void TIM6_IRQHandler(void)
{
  if (TIM6->SR & TIM_SR_UIF)
  {
    // Check update interrupt flag
    TIM6->SR &= ~TIM_SR_UIF; // Clear update interrupt flag

    // GPIOC->ODR ^= GPIO_ODR_OD4; // Toggle PC4
    if (GPIOC->ODR & GPIO_ODR_OD4)
    {
      GPIOC->BSRR = GPIO_BSRR_BR_4; // Reset PC4 (set it to 0) if it is currently set
      GPIOC->BSRR = GPIO_BSRR_BR_8; // Reset PC8 (set it to 0) if it is currently set
    }
    else
    {
      GPIOC->BSRR = GPIO_BSRR_BS_4; // Set PC4 (set it to 1) if it is currently reset
      GPIOC->BSRR = GPIO_BSRR_BS_8; // Set PC8 (set it to 1) if it is currently reset
    }
  }
}


uint16_t adc_threshold = 2048;  // Mid-range threshold for 12-bit ADC
uint16_t previous_adc_value = 4095;  // Assume fully charged initially





void TIM2_IRQHandler(void)
{
  if (TIM2->SR & TIM_SR_UIF) 
  {

    TIM2->SR &= ~TIM_SR_UIF; // Clear the update interrupt flag
    
    if(ADC1->CR&ADC_CR_ADSTART)
    {
      ADC1->CR |= ADC_CR_ADSTP; // stop conversion
      while(ADC1->CR & ADC_CR_ADSTP);
    }

    ADC1->CR |= ADC_CR_ADSTART; // start conversion

    uint16_t adc_val = (uint16_t)(ADC1->DR& 0xFFFF);

    // uart_tx_int(adc_val);
    // uart_tx_str("\n");

    TIM2->CR1 &= ~TIM_CR1_CEN;
  }
}


uint32_t TIM2_init()
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable Timer 2 clock

  // ensure timer2 is in initial state
  RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;
  RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;

  /**
   * 1/1mhz = 1us (microsecond)
   * 16mhz/16 = 1mhz, easy prescaler
   * t = 1/1mhz*400 = 1ms, so ARR should be 400
  */
  // TIM2->PSC = 16 - 1;  
  TIM2->PSC = 16;              // Set prescaler to 16
  // TIM2->ARR = 6400-1;             
  TIM2->ARR = 400-1;              // Set auto-reload to 400           

  TIM2->DIER |= TIM_DIER_UIE;      // Enable update interrupt

  NVIC_ClearPendingIRQ(TIM2_IRQn);
  NVIC_SetPriority(TIM2_IRQn,1);
  NVIC_EnableIRQ(TIM2_IRQn);


  // TIM2->CR1 |= TIM_CR1_CEN;         // Start Timer 2


  return RC_SUCC;
}


uint32_t TIM6_init()
{

  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; // Enable Timer 6 clock
  
  // ensure timer6 is in initial state
  RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST;
  RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM6RST;

  /**
   * 1s = 1000ms, 1/1hz = 1s, 1/1kmhz = 1ms
   * 16mhz/16000 = 1khz, easy prescaler
   * t = 1/1khz*60 = 60ms, so ARR should be 60
  */


  TIM6->PSC = 16000 - 1;              // Set prescaler to 16000
  TIM6->ARR = 60 - 1;                 // Set auto-reload to 60 for 60ms
  TIM6->DIER |= TIM_DIER_UIE;         // Enable update interrupt

  NVIC_ClearPendingIRQ(TIM6_IRQn);
  NVIC_SetPriority(TIM6_IRQn, 2);
  NVIC_EnableIRQ(TIM6_IRQn);

  TIM6->CR1 |= TIM_CR1_CEN; // Start Timer 6

  return RC_SUCC;
}





uint32_t initialize_gpio()
{
  
  RCC->IOPENR |= RCC_IOPENR_GPIOCEN; // Enable GPIOC clock

  GPIOC->MODER &= ~(GPIO_MODER_MODE4); // Clear PC4 mode
  GPIOC->MODER |= GPIO_MODER_MODE4_0; // Set PC4 as output

  GPIOC->MODER &= ~(GPIO_MODER_MODE8); // Clear PC5 mode
  GPIOC->MODER |= GPIO_MODER_MODE8_0; // Set PC5 as output


  // GPIOC->MODER |= GPIO_MODER_MODE5;

}