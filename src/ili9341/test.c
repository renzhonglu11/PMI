#include "ili_priv.h"
#include <systick.h>

/**
 * @brief Do a test and benchmark routine
 * @return Execution time in miliseconds
 */
uint32_t ili9341_test(void)
{
    uint8_t i;
    int8_t cnt = 12;
    uint32_t t1;
    uint16_t col;
    struct display_info_s dspi;

    t1 = systick_get();

    dspi = ili9341_display_info_get();

    ili9341_rect_fill(0, 0, dspi.width, dspi.height, ILI9341_COLOR_YELLOW);

    ili9341_rotation_set(ILI9341_ORIENTATION_0);
    for (i = 0; i < 13; i++)
    {
        ili9341_text_pos_set(i,i);
        ili9341_str_print("TEST!", ILI9341_COLOR_OLIVE, ILI9341_COLOR_YELLOW);
    }

    dspi = ili9341_display_info_get();
    ili9341_rect_fill(3, 3, dspi.width - 6, dspi.height - 6,
        ILI9341_COLOR_DARKGREY);

    while (cnt-- > 0)
    {
        for(i = 48; i < 127; i++)
        {
            ili9341_rotation_set(i % 4);
            ili9341_text_pos_set(i % 20, i % 20);
            ili9341_char_print(i, ILI9341_COLOR_GREEN, ILI9341_COLOR_DARKGREY);
        }
    }

    dspi = ili9341_display_info_get();
    col = ILI9341_COLOR_BLACK;
    for (i = 0; i < dspi.width; i++)
    {
        ili9341_line_draw(i, 40, dspi.width - i, dspi.height - 40,
            col);
        col += 0x1042;
    }

    return systick_get() - t1;
}

/**
 * @brief Prints an endless matrix like screen routine
 */
void ili9341_enterthematrix(void)
{
    uint8_t i;
    uint32_t cnt;
    uint8_t cnt_print;
    struct display_info_s dspi;

    dspi = ili9341_display_info_get();

    ili9341_rect_fill(0, 0, dspi.width, dspi.height, ILI9341_COLOR_BLACK);
    ili9341_rotation_set(ILI9341_ORIENTATION_0);
    cnt = 0;
    while (1)
    {
        cnt_print = 2;
        while (cnt_print--)
        {
            for(i = 48; i < 127; i += (cnt % 5))
            {
                if (!(cnt % 3) || !(cnt % 67) || !(cnt % 97))
                {
                    ili9341_char_print(i, ILI9341_COLOR_GREEN,
                        ILI9341_COLOR_DARKGREY);
                }
                else
                {
                    if (cnt % 53)
                    {
                        ili9341_char_print(' ', ILI9341_COLOR_GREEN,
                            ILI9341_COLOR_DARKGREY);
                    }

                    if (!(cnt % 29))
                    {
                        dspi = ili9341_display_info_get();
                        ili9341_rect_fill(dspi.x_p, dspi.y_p, dspi.font_width,
                            dspi.font_height, ILI9341_COLOR_BLACK);
                    }
                }
                cnt++;
            }
        }
    }
}