#include "pmi.h"

int current_index = 0;
int trigger_index = -1;
uint16_t adc_threshold = 2048; // Mid-range threshold for a 12-bit ADC
uint16_t previous_adc_value = 4095;
enum PMI_BOOL_E edge_detected = FALSE;
enum PMI_BOOL_E first_reading_taken = TRUE;

volatile uint8_t data_ready = 0;
volatile uint8_t debounce_in_progress = 0;
volatile uint8_t pb2_pressed = 0;


uint32_t initial_interrupt(void);
uint32_t config_button(void);
uint32_t reset_TIM2_zoom_level();

uint32_t extract_samples(uint16_t *extracted_data)
{
  if (data_ready)
  {
    // we get the data, now lock this function
    return RC_SUCC;
  }

  uint8_t start_index = (trigger_index - PRE_TRIGGER_COUNT + BUFFER_SIZE) % BUFFER_SIZE;

  uint8_t end_index = (trigger_index + POST_TRIGGER_COUNT) % BUFFER_SIZE;

  // uint16_t extracted_data[BUFFER_SIZE]; // Array to hold the extracted data
  int extracted_index = 0;

  for (int i = 0; i < BUFFER_SIZE; i++)
  {
    extracted_data[extracted_index++] = adc_buffer[start_index];
    start_index = (start_index + 1) % BUFFER_SIZE;

    if (start_index == end_index)
    {
      break; // Stop when we reach the end index
    }
  }

  return RC_SUCC;
}

void TIM6_IRQHandler(void)
{
  if (TIM6->SR & TIM_SR_UIF)
  {

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

    // Check update interrupt flag
    TIM6->SR &= ~TIM_SR_UIF; // Clear update interrupt flag
  }
}

static uint8_t waiting_for_rise = 1;

void TIM2_IRQHandler(void)
{
  // TODO: take a deep look at the circular buffer

  if (TIM2->SR & TIM_SR_UIF)
  {

    TIM2->SR &= ~TIM_SR_UIF; // Clear the update interrupt flag

    if (ADC1->CR & ADC_CR_ADSTART)
    {
      ADC1->CR |= ADC_CR_ADSTP; // stop conversion
      while (ADC1->CR & ADC_CR_ADSTP)
        ;
    }

    ADC1->CR |= ADC_CR_ADSTART; // start conversion

    // Wait for end of conversion (EOC)
    while (!(ADC1->ISR & ADC_ISR_EOC))
      ;

    ADC1->ISR |= ADC_ISR_EOC; // clear EOC bit

    uint16_t adc_val = (uint16_t)(ADC1->DR & 0xFFFF); // get ADC value

    if (first_reading_taken)
    {
      previous_adc_value = adc_val;
      first_reading_taken = FALSE;
      return;
    }

    if (waiting_for_rise)
    {
      if (adc_val >= 3071) // 4095*3/4 = 3071.25
      {
        GPIOC->BSRR = GPIO_BSRR_BR_4; // Reset PC4 (set it to 0)

        GPIOC->BSRR = GPIO_BSRR_BR_8;
        waiting_for_rise = 0;
      }
    }
    else
    {
      if (adc_val <= 1024) // 4095/4 = 1023.75
      {

        GPIOC->BSRR = GPIO_BSRR_BS_4; // Set PC4 (set it to 1)
        GPIOC->BSRR = GPIO_BSRR_BS_8;
        waiting_for_rise = 1;
      }
    }


    if (edge_detected == FALSE && (previous_adc_value > adc_threshold) && (adc_val < adc_threshold))
    {
      edge_detected = TRUE;
      trigger_index = current_index;
    }

    adc_buffer[current_index] = adc_val;               // Store in buffer
    previous_adc_value = adc_val;                      // Update for next comparison
    current_index = (current_index + 1) % BUFFER_SIZE; // Increment and wrap index

    // Check if we have captured enough post-trigger samples
    if (edge_detected && ((current_index - trigger_index + BUFFER_SIZE) % BUFFER_SIZE) >= POST_TRIGGER_COUNT)
    {
      // reset all the things here
      edge_detected = FALSE;
      // Now, adc_buffer contains 120 samples before and after the trigger
      // Process the buffer here or signal that it's ready to be processed
      

      
      extract_samples(extracted_data); // we get the current finish cirular buffer
      

      // if (adc_buffer[current_index] == 0)
      // {
      //   return;
      // }

      graph_ready = 1; // inform main to draw the graph
    }

    // TIM2->CR1 &= ~TIM_CR1_CEN;
  }
}

/**
 * @brief Interrupt handler for TIM21.
 *
 * This function is called when the update interrupt flag of TIM21 is set.
 * It clears the interrupt flag, stops the timer, and processes button actions
 * based on the state of PB1 and PB2 buttons. It also updates the zoom level
 * based on the button actions.
 */
void TIM21_IRQHandler(void)
{
  if (TIM21->SR & TIM_SR_UIF)
  {
    // Clear update interrupt flag
    TIM21->SR &= ~TIM_SR_UIF;

    // Stop the timer
    TIM21->CR1 &= ~TIM_CR1_CEN;

    // Process button action now that debouncing is complete
    // Process button action for PB1
    if (!(GPIOB->IDR & GPIO_IDR_ID1))
    {
      if (zoom_lvl <= 1)
      {
        zoom_lvl = 1;
      }
      else
      {
        zoom_lvl -= 1; // Decrease zoom level
        reset_TIM2_zoom_level();
      }
    }

    // Process button action for PB2
    if (!(GPIOB->IDR & GPIO_IDR_ID2))
    {
      if (zoom_lvl >= 5)
      {
        zoom_lvl = 5;
      }
      else
      {
        zoom_lvl += 1; // Increase zoom level
        reset_TIM2_zoom_level();
      }
    }

    // Reset debounce flag
    debounce_in_progress = 0;
  }
}

// uint8_t power_of_2(uint8_t exponent)
// {
//   uint8_t r = 1;

//   for (uint8_t i = 1; i < exponent; i++)
//   {
//     r *= 2;
//   }

//   return r;
// }

uint32_t reset_TIM2_zoom_level()
{
  const uint32_t ARR_DEFAULT = 400;

  TIM2->ARR = ARR_DEFAULT / power_of_2(zoom_lvl) - 1;

  return RC_SUCC;
}

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

uint32_t initialize_gpio()
{

  RCC->IOPENR |= RCC_IOPENR_GPIOCEN; // Enable GPIOC clock
  config_button();

  GPIOC->MODER &= ~(GPIO_MODER_MODE4); // Clear PC4 mode
  GPIOC->MODER |= GPIO_MODER_MODE4_0;  // Set PC4 as output

  GPIOC->MODER &= ~(GPIO_MODER_MODE8); // Clear PC5 mode
  GPIOC->MODER |= GPIO_MODER_MODE8_0;  // Set PC5 as output

  // GPIOC->MODER |= GPIO_MODER_MODE5;

  GPIOC->MODER &= ~(GPIO_MODER_MODE6); // Clear PC6 mode
  GPIOC->MODER |= GPIO_MODER_MODE6_0;  // Set PC6 as output


  GPIOC->BSRR = GPIO_BSRR_BS_4;  // Set PC4 by default, so the capcitor is charging
  return RC_SUCC;
}

uint32_t config_button(void)
{
  //  enable GPIOB clock
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

  // set input mode
  GPIOB->MODER &= ~(GPIO_MODER_MODE2); // PB2 for SW1
  GPIOB->MODER &= ~(GPIO_MODER_MODE1); // PB1 for SW2

  initial_interrupt();

  return RC_SUCC;
}

uint32_t initial_interrupt(void)
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

uint32_t initialize_project()
{
  adc_init();
  TIM2_init();
  TIM21_init();
  initialize_gpio();

  ili9341_init(ILI9341_ORIENTATION_0); // initialize the LCD
  // Clear the LCD
  ili9341_rect_fill(0, 0, ili9341_display_info_get().width, ili9341_display_info_get().height, ILI9341_COLOR_BLACK);
  return RC_SUCC;
}