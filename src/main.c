#include <uart.h>
#include <clocks.h>
#include <pmi.h>

int main(void)
{

  clocks_init_pmi();
  uart_init_nucusb(115200);
  initialize_project();

  while (1)
  {

    systick_delay_ms(300);

    if (graph_ready)
    {
      /**
       * 1. extract data from adc_buffer
       * 2. draw the corresponding curve
       * 3. display the values of metrics
       * 4. reset the everything for the next display of sampled data
       */
      extract_samples(extracted_data);
      draw_graph(extracted_data, BUFFER_SIZE, ILI9341_COLOR_WHITE, ILI9341_COLOR_RED);
      displayValues(zoom_lvl, ILI9341_COLOR_WHITE);
      systick_delay_ms(700);

      reset_project();
    }
  }
}