#include "pmi.h"

/**
 * @brief Initializes the GPIO pins for PMI functionality.
 *
 * This function enables the GPIOC clock and configures the necessary pins for PMI.
 * PC4, PC5, and PC6 are set as output pins.
 * PC4 is set high by default to charge the capacitor.
 *
 * @return RC_SUCC if the initialization is successful.
 */
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

  GPIOC->BSRR = GPIO_BSRR_BS_4; // Set PC4 by default, so the capcitor is charging
  return RC_SUCC;
}

/**
 * @brief Initializes the project by initializing various components and clearing the LCD.
 *
 * This function initializes the ADC, TIM2, TIM21, GPIO, and the ILI9341 LCD display.
 * It sets the orientation of the LCD to ILI9341_ORIENTATION_0 and fills the entire screen with black color.
 *
 * @return RC_SUCC indicating successful initialization.
 */
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

uint32_t reset_project()
{
  GPIOC->BSRR = GPIO_BSRR_BS_6; // Set PC4 (set it to 1) if it is currently reset
  draw_graph(extracted_data, BUFFER_SIZE, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLACK);
  graph_ready = 0;
  data_ready = 0;           // unlock extract_samples function
  TIM2->CR1 |= TIM_CR1_CEN; // Start Timer 2

  return RC_SUCC;
}