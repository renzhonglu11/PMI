#include "button.h"


volatile uint8_t pb2_pressed = 0;
volatile uint8_t debounce_in_progress = 0;


void EXTI2_3_IRQHandler()
{
  // PB2 for SW1
  if ((EXTI->PR & EXTI_PR_PIF2)) // pending bit of EXTI line 2 is set
  {

    if (!debounce_in_progress)
    {
      pb2_pressed = 1;
      // Start debounce timer
      debounce_in_progress = 1; // debounce in progress
      TIM21->CNT = 0;           // Reset the timer count
      TIM21->CR1 |= TIM_CR1_CEN;
    }
  }
  EXTI->PR = EXTI_PR_PIF2; // write one to corresponding pending bit to clear pending bit
}

void EXTI0_1_IRQHandler(void)
{
  // PB1 for SW2
  // TODO: implment interrupt handler for SW2

  if ((EXTI->PR & EXTI_PR_PIF1))
  {

    if (!debounce_in_progress)
    {
      // Start debounce timer
      debounce_in_progress = 1; // debounce in progress
      TIM21->CNT = 0;           // Reset the timer count
      TIM21->CR1 |= TIM_CR1_CEN;
    }
  }
  EXTI->PR = EXTI_PR_PIF1;
}



uint32_t config_button(void)
{
  //  enable GPIOB clock
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

  // set input mode
  GPIOB->MODER &= ~(GPIO_MODER_MODE2); // PB2 for SW1
  GPIOB->MODER &= ~(GPIO_MODER_MODE1); // PB1 for SW2

  initialize_btn_interrupt();

  return RC_SUCC;
}


uint32_t initialize_btn_interrupt(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // enables the clock for the System Configuration

  // EXTI0, EXTI1 and  EXTI2, EXTI3
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PB; // PB2 for SW1
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PB; // PB1 for SW2

  EXTI->IMR |= EXTI_IMR_IM1; //  enables the interrupt request from EXTI line 1
  EXTI->IMR |= EXTI_IMR_IM2; //  enables the interrupt request from EXTI line 2

  EXTI->FTSR |= EXTI_FTSR_TR1; //  selects the falling trigger for the EXTI line 1
  EXTI->FTSR |= EXTI_FTSR_TR2; // selects the falling trigger for the EXTI line 2

  NVIC_ClearPendingIRQ(EXTI0_1_IRQn); // PB1 in EXTI1
  NVIC_ClearPendingIRQ(EXTI2_3_IRQn); // PB2 in EXTI2
  NVIC_SetPriority(EXTI0_1_IRQn, 3);
  NVIC_SetPriority(EXTI2_3_IRQn, 3);
  NVIC_EnableIRQ(EXTI0_1_IRQn);
  NVIC_EnableIRQ(EXTI2_3_IRQn);

  return RC_SUCC;
}