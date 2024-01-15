#include "pmi.h"

int current_index = 0;
int trigger_index = -1;
uint16_t adc_threshold = 2048;          // Mid-range threshold for a 12-bit ADC
uint16_t previous_adc_value = 4095;
enum PMI_BOOL_E edge_detected = FALSE;
enum PMI_BOOL_E first_reading_taken = TRUE;

uint8_t data_ready = 0;



uint32_t extract_samples(uint16_t *extracted_data)
{
  if(data_ready)
  {
    // we get the data, now lock this function
    return RC_SUCC;
  }
  
  uint8_t start_index = (trigger_index - PRE_TRIGGER_COUNT + BUFFER_SIZE) % BUFFER_SIZE;

 
 
  uint8_t end_index = (trigger_index + POST_TRIGGER_COUNT) % BUFFER_SIZE;
  
  
  uart_tx_str("start: ");
  uart_tx_int(start_index);
  uart_tx_str("\n");
  uart_tx_str("end: ");
  uart_tx_int(end_index);
  uart_tx_str("\n");

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

void TIM2_IRQHandler(void)
{
  // TODO: take a deep look at the circular buffer

  if (TIM2->SR & TIM_SR_UIF)
  {

    TIM2->SR &= ~TIM_SR_UIF; // Clear the update interrupt flag

    if (ADC1->CR & ADC_CR_ADSTART)
    {
      ADC1->CR |= ADC_CR_ADSTP; // stop conversion
      while (ADC1->CR & ADC_CR_ADSTP);
    }

    ADC1->CR |= ADC_CR_ADSTART; // start conversion

    // Wait for end of conversion (EOC)
    while (!(ADC1->ISR & ADC_ISR_EOC));
    
    ADC1->ISR |= ADC_ISR_EOC;    // clear EOC bit


    uint16_t adc_val = (uint16_t)(ADC1->DR & 0xFFFF);  // get ADC value

    

    if(first_reading_taken)
    {
      previous_adc_value = adc_val;
      first_reading_taken = FALSE;
      return;
    }



    // uart_tx_int(adc_val);
    // uart_tx_str("\n");

    // if(data_ready)
    // {
    //   return;
    // }

  

    if (edge_detected == FALSE && (previous_adc_value >= adc_threshold) && (adc_val < adc_threshold))
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
      extract_samples(extracted_data);     // we get the current finish cirular buffer
      graph_ready = 1;                     // inform main to draw the graph
      
      first_reading_taken = FALSE;  
      


    }

    // TIM2->CR1 &= ~TIM_CR1_CEN;
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

  TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt

  NVIC_ClearPendingIRQ(TIM2_IRQn);
  NVIC_SetPriority(TIM2_IRQn, 2);
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

uint32_t initialize_gpio()
{

  RCC->IOPENR |= RCC_IOPENR_GPIOCEN; // Enable GPIOC clock

  GPIOC->MODER &= ~(GPIO_MODER_MODE4); // Clear PC4 mode
  GPIOC->MODER |= GPIO_MODER_MODE4_0;  // Set PC4 as output

  GPIOC->MODER &= ~(GPIO_MODER_MODE8); // Clear PC5 mode
  GPIOC->MODER |= GPIO_MODER_MODE8_0;  // Set PC5 as output

  // GPIOC->MODER |= GPIO_MODER_MODE5;


  // @renzhonglu11 TODO:set PC6
}