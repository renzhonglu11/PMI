#include "graph_utils.h"



void ili9341_draw_thick_line_horizontal(int16_t x, int16_t y, uint16_t color, uint8_t thickness);

uint32_t draw_graph(uint16_t buffer[], int buffer_size, uint16_t color) 
{

    // Clear the display or just the area where you will draw the graph
    ili9341_rect_fill(0, 0, ili9341_display_info_get().width, 200, BG_COLOR);


    struct display_info_s display_info = ili9341_display_info_get();

    // Determine scaling factors based on the display size and ADC range
    uint16_t x_scale = display_info.width / buffer_size;
    // uint16_t x_scale = display_info.width / 120;
    // uint16_t y_scale = 62 / 4096;  // Assuming 12-bit ADC
    float y_scale = 62.0 / 4096.0; // Now y_scale is a floating-point number

    // int start_index = triger_index;
    // int end_index = (triger_index + 120) % buffer_size;

    // for (int i = 0; i < buffer_size; i++)
    // {
    //   if (i > 120)
    //   {
    //     break; // Stop when we reach the end index
    //   }
    //   // buffer[start_index];
    //   int16_t x = i * x_scale;
    //   int16_t y = 140 - (int16_t)(buffer[start_index]*y_scale);   // set 140 as the offset first
    //   ili9341_draw_thick_line_horizontal(x, y, color, 2);
    //   start_index = (start_index + 1) % buffer_size;

    // }



    // Plot the ADC values
    for (uint8_t i = 0; i < buffer_size; i++) {
        int16_t x = i * x_scale;
        int16_t y = 140 - (int16_t)(buffer[i] * y_scale);   // set 140 as the offset first
        // ili9341_pixel_set(x, y, ILI9341_COLOR_BLUE);
        ili9341_draw_thick_line_horizontal(x, y, color, 2);

    }

    // Mark the trigger point (t=0)
    // int trigger_x = trigger_index * x_scale;
    // ili9341_line_draw(trigger_x, 0, trigger_x, display_info.height, ILI9341_COLOR_RED);

    // Optionally, mark the t=RC point if calculated
    
    
    // systick_delay_ms(300);
    return RC_SUCC;
}


void ili9341_draw_thick_line_horizontal(int16_t x, int16_t y, uint16_t color, uint8_t thickness) 
{
    for (int i = 0; i < thickness; i++) {
        ili9341_pixel_set(x, y + i, color);
    }
}