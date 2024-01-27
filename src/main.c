#include <uart.h>
#include <clocks.h>
#include <pmi.h>

const float DIVISOR_MV = 4.095;
volatile uint8_t graph_ready = 0;
uint8_t rc_range = 0;
uint32_t p2p_val = 0;
volatile uint8_t zoom_lvl = 1;
uint16_t final_time_period = 0;

int main(void)
{
  /* Call your initialisations here */
  clocks_init_pmi();
  uart_init_nucusb(115200);
  initialize_project();   // initial the whole project


  while (1)
  {

    systick_delay_ms(300);

    if (graph_ready)
    {

      extract_samples(extracted_data);
      draw_graph(extracted_data, BUFFER_SIZE, ILI9341_COLOR_WHITE, ILI9341_COLOR_RED);
      displayValues(zoom_lvl, ILI9341_COLOR_WHITE);
      systick_delay_ms(700);
      draw_graph(extracted_data, BUFFER_SIZE, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLACK);

      graph_ready = 0;
      data_ready = 0;           // unlock extract_samples function
      TIM2->CR1 |= TIM_CR1_CEN; // Start Timer 2
    }
  }
}