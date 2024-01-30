#include "button.h"

/**
 * @brief A efficient timer interrupt solution for button debouncing.
 *        After the button is pressed, the timer 21 will start counting.
 *        After 35ms, the timer 21 will trigger an interrupt and check whether the button is *    *        still pressed.
 *        If the button is still pressed, the logic of the corresponding button will be executed.
 *        Otherwise, nothing will happen and the debounce_in_progress is reset. 
*/



volatile uint8_t pb2_pressed = 0;
volatile uint8_t debounce_in_progress = 0;


/**
 * @brief Interrupt handler for EXTI lines 2 and 3.
 * 
 * This function is called when the interrupt for EXTI lines 2 and 3 is triggered.
 * It checks if the pending bit for EXTI line 2 is set, indicating that the interrupt
 * was caused by PB2 (SW1) button press. If the pending bit is set and debounce is not
 * in progress, it sets the pb2_pressed flag, starts the debounce timer, and clears the
 * pending bit.
 */
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



/**
 * @brief Interrupt handler for SW2.
 *        This function is called when the EXTI line 1 interrupt occurs.
 *        It starts a debounce timer if the interrupt is triggered and start the timer 21
 *        
 * @retval None
 */
void EXTI0_1_IRQHandler(void)
{
  // PB1 for SW2
  if ((EXTI->PR & EXTI_PR_PIF1))
  {

    if (!debounce_in_progress)
    {
      // Start debounce timer
      debounce_in_progress = 1;     // debounce in progress
      TIM21->CNT = 0;               // Reset the timer counter
      TIM21->CR1 |= TIM_CR1_CEN;    // start timer 21
    }
  }
  EXTI->PR = EXTI_PR_PIF1;
}



/**
 * @brief Configures the button GPIO pins and initializes the button interrupt.
 * 
 * This function enables the GPIOB clock and sets the input mode for the button pins.
 * It also calls the initialize_btn_interrupt() function to initialize the button interrupt.
 * 
 * @return RC_SUCC if the configuration is successful.
 */
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


/**
 * @brief Initializes the button interrupt.
 * 
 * This function enables the clock for the System Configuration and configures the EXTI lines
 * and NVIC interrupts for button interrupts. It sets the falling trigger for EXTI lines 1 and 2,
 * clears any pending interrupts, sets the priority, and enables the interrupts.
 * 
 * @return RC_SUCC if the initialization is successful.
 */
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