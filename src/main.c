#include <uart.h>
#include <clocks.h>
#include <pmi.h>

uint8_t graph_ready = 0;

void get_metrics(float *averageValue, float *peakToPeakValue, float *timeSpan, float *capacitanceValue)
{

  *averageValue = 0;
  *peakToPeakValue = 0;
  *timeSpan = 0;
  *capacitanceValue = 0;
 
}

void displayValues(uint8_t zoomLevel) 
{
    // TODO: display all the values on the LCD

    float averageValue;
    float peakToPeakValue;
    // Implement the following functions as per your requirements
    float timeSpan;
    float rcTimeConstant ;
    float capacitanceValue;
    
    get_metrics(&averageValue, &peakToPeakValue, &timeSpan, &capacitanceValue);

    char displayString[30]; 
    char floatBuf[32];
    int yPos = 8; // Example Y position, adjust based on your graph position

    float2str(floatBuf, 30, averageValue, 2);
    sprintf(displayString, "Avg: %s", floatBuf);
    ili9341_text_pos_set(0, yPos++);
    ili9341_str_print(displayString, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    float2str(floatBuf, 30, peakToPeakValue, 2);
    sprintf(displayString, "P-P: %s", floatBuf);
    ili9341_text_pos_set(0, yPos++);
    ili9341_str_print(displayString, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    float2str(floatBuf, 30, rcTimeConstant, 2);
    sprintf(displayString, "RC: %s", floatBuf);
    ili9341_text_pos_set(0, yPos++);
    ili9341_str_print(displayString, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    float2str(floatBuf, 30, capacitanceValue, 2);
    sprintf(displayString, "C: %s", floatBuf);
    ili9341_text_pos_set(0, yPos++);
    ili9341_str_print(displayString, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    
    float2str(floatBuf, 30, timeSpan, 2);
    sprintf(displayString, "Span: %s", floatBuf);
    ili9341_text_pos_set(0, yPos++);
    ili9341_str_print(displayString, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    sprintf(displayString, "Zoom: %d", zoomLevel);
    ili9341_text_pos_set(0, yPos);
    ili9341_str_print(displayString, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}



int main(void)
{
  /* Call your initialisations here */
  clocks_init_pmi();
  uart_init_nucusb(115200);
  ili9341_init(ILI9341_ORIENTATION_0); // initialize the LCD
  // Clear the display or just the area where you will draw the graph
  ili9341_rect_fill(0, 0, ili9341_display_info_get().width, ili9341_display_info_get().height, BG_COLOR);

  adc_init();

  TIM6_init();
  TIM2_init();
  initialize_gpio();

  while (1)
  {
    /* Call your routines here */
    systick_delay_ms(300);

    if (graph_ready)
    {
      // draw the graph
      // Extract data from the circular buffer
      uint16_t extracted_data[BUFFER_SIZE];
      extract_samples(extracted_data);

    //   uint8_t trigger_index = 0;

    //   for (int i = 0; i < BUFFER_SIZE; i++) {
    //   if (i > 0 && extracted_data[i] < 2048 && extracted_data[i - 1] >= 2048) {
    //       // Falling edge detected at index i
    //       trigger_index = i;
    //       break;
    //   }
    // }




      // Draw the graph on the display
      // draw_graph(extracted_data, BUFFER_SIZE,ILI9341_COLOR_BLUE, trigger_index);
      draw_graph(extracted_data, BUFFER_SIZE,ILI9341_COLOR_BLUE);

      graph_ready = 0;
      displayValues(1);

      
      systick_delay_ms(2000);

      draw_graph(extracted_data, BUFFER_SIZE,ILI9341_COLOR_WHITE);
    }
  }
}