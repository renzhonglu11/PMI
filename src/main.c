#include <uart.h>
#include <clocks.h>
#include <pmi.h>

void draw_graph(uint16_t* buffer, int buffer_size) {

    // Clear the display or just the area where you will draw the graph
    ili9341_rect_fill(0, 0, ili9341_display_info_get().width, ili9341_display_info_get().height, ILI9341_COLOR_BLACK);


    struct display_info_s display_info = ili9341_display_info_get();

    // Determine scaling factors based on the display size and ADC range
    uint16_t x_scale = display_info.width / buffer_size;
    // uint16_t y_scale = 62 / 4096;  // Assuming 12-bit ADC
    float y_scale = 62.0 / 4096.0; // Now y_scale is a floating-point number


    // Plot the ADC values
    for (int i = 0; i < buffer_size; i++) {
        int x = i * x_scale;
        int y = 150 - (int)(buffer[i] * y_scale);
        ili9341_pixel_set(x, y, ILI9341_COLOR_WHITE);
    }

    // Mark the trigger point (t=0)
    // int trigger_x = trigger_index * x_scale;
    // ili9341_line_draw(trigger_x, 0, trigger_x, display_info.height, ILI9341_COLOR_RED);

    // Optionally, mark the t=RC point if calculated
}



uint8_t graph_ready =0;

int main(void)
{
    /* Call your initialisations here */
    clocks_init_pmi();
    uart_init_nucusb(115200);
    ili9341_init(ILI9341_ORIENTATION_0); // initialize the LCD
    // Clear the display or just the area where you will draw the graph
    ili9341_rect_fill(0, 0, ili9341_display_info_get().width, ili9341_display_info_get().height, ILI9341_COLOR_BLACK);

    adc_init();

    TIM6_init();
    TIM2_init();
    initialize_gpio();
    
    
   

    while(1)
    {
        /* Call your routines here */
        
        // TIM2->CR1 |= TIM_CR1_CEN;
        uart_tx_int(graph_ready);
        uart_tx_str("\n");
        if(graph_ready)
        {
          // draw the graph
          // Extract data from the circular buffer
          uint16_t extracted_data[BUFFER_SIZE];
          extract_samples(extracted_data);



          // Draw the graph on the display
          draw_graph(extracted_data, BUFFER_SIZE);
          uart_tx_str("drawing...");
          uart_tx_str("\n");
          graph_ready = 0;
          systick_delay_ms(300);
        }



    }
}