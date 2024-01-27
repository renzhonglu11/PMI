#include <uart.h>
#include <clocks.h>
#include <pmi.h>
#include "ili9341.h"

const float DIVISOR_MV = 4.095;
uint8_t graph_ready = 0;
uint8_t rc_range = 0;
uint32_t p2p_val = 0;
uint8_t volatile zoom_lvl = 1;

int main(void)
{
  /* Call your initialisations here */
  clocks_init_pmi();
  uart_init_nucusb(115200);
  ili9341_init(ILI9341_ORIENTATION_0); // initialize the LCD
  // Clear the LCD
  ili9341_rect_fill(0, 0, ili9341_display_info_get().width, ili9341_display_info_get().height, ILI9341_COLOR_BLACK);

  adc_init();

  TIM6_init();
  TIM2_init();
  TIM21_init();
  initialize_gpio();

  while (1)
  {
    
    systick_delay_ms(300);

    if (graph_ready)
    {
      
      extract_samples(extracted_data);
      
      draw_graph(extracted_data, BUFFER_SIZE, ILI9341_COLOR_WHITE, ILI9341_COLOR_RED);
      displayValues(zoom_lvl);
      systick_delay_ms(700);
      draw_graph(extracted_data, BUFFER_SIZE, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLACK);


      graph_ready = 0;
      data_ready = 0; // unlock extract_samples function
      TIM2->CNT = 0; 
      TIM2->CR1 |= TIM_CR1_CEN; // Start Timer 2
    }
  }
}