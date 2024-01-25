#include "timer.h"


uint32_t TIM2_init()
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable Timer 2 clock

  // ensure timer2 is in initial state
  RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;
  RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;

  /**
   * 1/1mhz = 1us (microsecond)
   * 16mhz/16 = 1mhz, easy prescaler
   * t = 1/1mhz*400 = 400 us, so ARR should be 400
   */
  // TIM2->PSC = 16 - 1;
  TIM2->PSC = 16; // Set prescaler to 16
  // TIM2->ARR = 6400-1;
  TIM2->ARR = 400 - 1; // Set auto-reload to 400

  TIM2->CR1 |= TIM_CR1_ARPE; // Enable auto-reload preload


  TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt

  NVIC_ClearPendingIRQ(TIM2_IRQn);
  NVIC_SetPriority(TIM2_IRQn, 1);
  NVIC_EnableIRQ(TIM2_IRQn);

  TIM2->CR1 |= TIM_CR1_CEN; // Start Timer 2
  // TIM2->CR1 |= TIM_CR1_CEN;

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

  TIM6->PSC = 16000 - 1;      // Set prescaler to 16000
  TIM6->ARR = 60 - 1;         // Set auto-reload to 60 for 60ms
  TIM6->DIER |= TIM_DIER_UIE; // Enable update interrupt

  NVIC_ClearPendingIRQ(TIM6_IRQn);
  NVIC_SetPriority(TIM6_IRQn, 1);
  NVIC_EnableIRQ(TIM6_IRQn);

  TIM6->CR1 |= TIM_CR1_CEN; // Start Timer 6

  return RC_SUCC;
}


uint32_t TIM21_init()
{
  RCC->APB2ENR |= RCC_APB2ENR_TIM21EN; // Enable Timer 2 clock

  // ensure timer2 is in initial state
  RCC->APB2RSTR |= RCC_APB2RSTR_TIM21RST;
  RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM21RST;

  /**
   * 1s = 1000ms, 1/1hz = 1s, 1/1kmhz = 1ms
   * 16mhz/16000 = 1khz, easy prescaler
   * t = 1/1khz*35 = 35ms, so ARR should be 35
   */

  TIM21->PSC = 16000 - 1;      // Set prescaler to 16000
  TIM21->ARR = 35 - 1;         // Set auto-reload to 60 for 60ms
  TIM21->DIER |= TIM_DIER_UIE; // Enable update interrupt

  NVIC_ClearPendingIRQ(TIM21_IRQn);
  NVIC_SetPriority(TIM21_IRQn, 2);
  NVIC_EnableIRQ(TIM21_IRQn);



  return RC_SUCC;
}