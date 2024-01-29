#include "timer.h"


uint16_t previous_adc_value = 4095;
uint16_t adc_threshold = 2048; // Mid-range threshold for a 12-bit ADC
uint16_t current_index = 0;

enum PMI_BOOL_E edge_detected = FALSE;
enum PMI_BOOL_E first_reading_taken = TRUE;

uint32_t reset_TIM2_zoom_level();


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

    if (edge_detected == FALSE && (previous_adc_value >= adc_threshold) && (adc_val < adc_threshold))
    {
      if (current_index % BUFFER_SIZE < 120)   
      {
        edge_detected = FALSE;
      }else
      {
        edge_detected = TRUE;
      }
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
      graph_ready = 1; // inform main to draw the graph
      first_reading_taken = FALSE;
      current_index = 0;
      TIM2->CR1 &= ~TIM_CR1_CEN; // now we have enough sampled data, stop the timer
    }
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

uint32_t reset_TIM2_zoom_level()
{
  const uint32_t ARR_DEFAULT = 400;

  TIM2->ARR = ARR_DEFAULT / power_of_2(zoom_lvl) - 1;

  return RC_SUCC;
}