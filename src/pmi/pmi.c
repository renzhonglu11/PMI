#include "pmi.h"

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
    }
    else
    {
      GPIOC->BSRR = GPIO_BSRR_BS_4; // Set PC4 (set it to 1) if it is currently reset
    }
  }
}

uint32_t TIM2_init()
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable Timer 2 clock

  // ensure timer2 is in initial state
  RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;
  RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;

  /**
   * 1/1mhz = 1us (microsecond)
   * 16mhz/16 = 1mhz, easy prescaler
   * t = 1/1mhz*400 = 1ms, so ARR should be 400
  */
  TIM2->PSC = 16 - 1;              // Set prescaler to 16
  TIM2->ARR = 400 - 1;             // Set auto-reload to 400

  TIM2->DIER |= TIM_DIER_UIE;      // Enable update interrupt

  NVIC_EnableIRQ(TIM2_IRQn);
  NVIC_SetPriority(TIM21_IRQn,1);
  NVIC_EnableIRQ(TIM2_IRQn);


  TIM2->CR1 |= TIM_CR1_CEN;         // Start Timer 2


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

  NVIC_EnableIRQ(TIM6_IRQn);
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


  GPIOC->MODER |= GPIO_MODER_MODE5;

}