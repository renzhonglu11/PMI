#include "ili_priv.h"
#include <pmi_stddefs.h>

/* The used fonts are part of Adafruits GFX library.
 * See https://github.com/adafruit/Adafruit-GFX-Library
 */

#include "fonts/gfxfont.h"
#define PROGMEM     // Necessary to override Atmels megaAVR PROGMEM key
#include "fonts/FreeMonoBold12pt7b.h"
#include "fonts/FreeMonoBold9pt7b.h"
#include "fonts/FreeMono12pt7b.h"
#include "fonts/FreeMono9pt7b.h"

#include <systick.h>
#include <stdio.h>
#include <stdarg.h>

/* Helpler macros */
/* Returns absolute value */
#define abs(x) (((x) < 0) ? -(x) : (x))

/* Swaps bytes in 16 bit datatype */
#define swap_vals_int16(a, b)   \
do                              \
{                               \
    int16_t t = a;              \
    a = b;                      \
    b = t;                      \
} while (0)

/* Constant offset for y advance to decrease line distance */
#define Y_ADVANCE_OFFS      2

/* ILI9341 command reference */
#define TFTWIDTH        240
#define TFTHEIGHT       320

#define CMD_NOP         0x00
#define CMD_SWRESET     0x01
#define CMD_RDDID       0xD3
#define CMD_RDDST       0x09

#define CMD_SLPIN       0x10
#define CMD_SLPOUT      0x11
#define CMD_PTLON       0x12
#define CMD_NORON       0x13

#define CMD_RDMODE      0x0A
#define CMD_RDMADCTL    0x0B
#define CMD_RDPIXFMT    0x0C
#define CMD_RDIMGFMT    0x0D
#define CMD_RDSELFDIAG  0x0F

#define CMD_INVOFF      0x20
#define CMD_INVON       0x21
#define CMD_GAMMASET    0x26
#define CMD_DISPOFF     0x28
#define CMD_DISPON      0x29

#define CMD_CASET       0x2A
#define CMD_PASET       0x2B
#define CMD_RAMWR       0x2C
#define CMD_RAMRD       0x2E

#define CMD_PTLAR       0x30
#define CMD_MADCTL      0x36
#define CMD_PIXFMT      0x3A

#define CMD_FRMCTR1     0xB1
#define CMD_FRMCTR2     0xB2
#define CMD_FRMCTR3     0xB3
#define CMD_INVCTR      0xB4
#define CMD_DFUNCTR     0xB6

#define CMD_PWCTR1      0xC0
#define CMD_PWCTR2      0xC1
#define CMD_PWCTR3      0xC2
#define CMD_PWCTR4      0xC3
#define CMD_PWCTR5      0xC4
#define CMD_VMCTR1      0xC5
#define CMD_VMCTR2      0xC7

#define CMD_RDID1       0xDA
#define CMD_RDID2       0xDB
#define CMD_RDID3       0xDC
#define CMD_RDID4       0xDD

#define CMD_GMCTRP1     0xE0
#define CMD_GMCTRN1     0xE1

#define MADCTL_MY       0x80
#define MADCTL_MX       0x40
#define MADCTL_MV       0x20
#define MADCTL_ML       0x10
#define MADCTL_RGB      0x00
#define MADCTL_BGR      0x08
#define MADCTL_MH       0x04

#define CMD_PWCTR6      0xFC
#define _MW 0x2C    // Memory write
#define _SC 0x2A    // Set column
#define _SP 0x2B    // Set Page
#define TFTLCD_DELAY8 0x7F
#define TABLE_END 0xFF

/* Initial values */
static const uint8_t tbl_init_values[] = {
    0x26, 1, 0x01,              // Gamma curve select -> gamma curve 1
    0x34, 0,                    // Tearing effect line OFF
    0x36, 1, 0x08,              // Memory access control
    0x3A, 1, 0x55,              // Pixel format set -> 16 bit/pixel
    0xB0, 1, 0x00,              // RGB IF signal control -> dont use RGB IF
    0xB1, 2, 0x00, 0x1B,        // Frame rate control -> 70 Hz, full color mode
    0xB4, 1, 0x00,              // Inversion control -> No inversion
    0xB6, 3, 0x0A, 0xA0, 0x27,  // Display Function [0A 82 27 XX]    .kbv SS=1
    0xC0, 1, 0x2A,              // Power control 1 -> set GVDD level to 4.95V
    0xC5, 2, 0x3F, 0x3C,        // VCOM 1 [31 3C]
    0xC7, 1, 0xB5,              // VCOM 2 [C0]
    0xF6, 3, 0x00, 0x00, 0x00,
    TABLE_END,
};

/* Reset off values */
static const uint8_t tbl_reset_off[] = {
    0x01, 0,            // Soft Reset
    TFTLCD_DELAY8, 150, // .kbv powers up with ONLY reset, sleep out, disp. on
    0x28, 0,            // Display Off
    0x3A, 1, 0x55,      // Pixel format is 565 = 16 bit per pixel
    TABLE_END,
};

/* Wake up values */
static const uint8_t tbl_wake_on[] = {
    0x11, 0,            // Sleep out
    TFTLCD_DELAY8, 150,
    0x29, 0,            // Display on
    TABLE_END,
};

/**
 * @brief Common display infos
 */
static struct display_info_s dspi =
{
    .width = TFTWIDTH,
    .height = TFTHEIGHT
};

/**
 * @brief Get current display settings
 * @return Current infos
 */
struct display_info_s ili9341_display_info_get(void)
{
    return dspi;
}

/* Used font */
const GFXfont *font;

/**
 * @brief Sets the font
 * @param *fnt Font
 * @return See @ref PMI_RC_E
 */
static uint32_t font_set(const GFXfont *fnt)
{
    if (!fnt)
    {
        return RC_PARAM_INVALID;
    }
    font = fnt;

    dspi.font_height = font->yAdvance;
    dspi.font_width = font->glyph->xAdvance;

    return RC_SUCC;
}

/**
 * @brief Transmits display controller command
 * @param[in] cmd See ILI9341 command reference
 */
static void cmd_tx(uint8_t cmd)
{
    io_cs_set(IO_STATE_ACTIVATE);
    io_wr_set(IO_STATE_ACTIVATE);
    io_rs_set(IO_RS_CMD);

    io_dpins_set(cmd);

    io_wr_set(IO_STATE_RELEASE);
    io_rs_set(IO_RS_DATA);
    io_cs_set(IO_STATE_RELEASE);
}

/**
 * @brief Transmit pixel data
 * @param *data[in] Buffer to transmit
 * @param size[in] Buffer size
 */
static void data8_tx(uint8_t *data, uint8_t size)
{
    uint16_t i;

    io_cs_set(IO_STATE_ACTIVATE);

    for (i = 0; i < size; i++)
    {
        io_wr_set(IO_STATE_ACTIVATE);
        io_dpins_set(*data++);
        io_wr_set(IO_STATE_RELEASE);
    }

    io_cs_set(IO_STATE_RELEASE);
}

/**
 * @brief Transmit pixel data
 * @param *data[in] uint16_t buffer to transmit
 * @param size[in] Buffer size
 */
static void data16_tx(uint16_t *data, uint8_t size)
{
    uint16_t i;

    io_cs_set(1);

    for (i = 0; i < size; i++)
    {
        io_wr_set(IO_STATE_ACTIVATE);

        io_dpins_set(*data >> 8);

        io_wr_set(IO_STATE_RELEASE);
        io_wr_set(IO_STATE_ACTIVATE);

        io_dpins_set(*data & 0xFF);

        io_wr_set(IO_STATE_RELEASE);
    }

    io_cs_set(IO_STATE_RELEASE);
}

/**
 * @brief Transmits display controller command followed by data
 * @param cmd[in] Command
 * @param *data[in] Pointer to data array
 * @param size[in] Size of data array
 */
static void cmd_data_tx(uint8_t cmd, uint8_t *data, uint8_t size)
{
    io_cs_set(IO_STATE_ACTIVATE);
    io_wr_set(IO_STATE_ACTIVATE);
    io_rs_set(IO_STATE_ACTIVATE);

    io_dpins_set(cmd);

    io_wr_set(IO_STATE_RELEASE);
    io_rs_set(IO_STATE_RELEASE);

    data8_tx(data, size);

    io_cs_set(IO_STATE_RELEASE);
}

/**
 * @brief Sets coordinates to draw a rectangle
 * @note Afterwards color must be set
 * @param[in] x0 x coordinate of upper left corner of rectangle
 * @param[in] y0 y coordinate of upper left corner of rectangle
 * @param[in] x1 x coordinate of lower right corner of rectangle
 * @param[in] y1 y coordinate of lower right corner of rectangle
 */
static void rect_coords_set(uint16_t x0, uint16_t y0,
    uint16_t x1, uint16_t y1)
{
    uint8_t x_dat[] = {x0 >> 8, x0, x1 >> 8, x1};
    uint8_t y_dat[] = {y0 >> 8, y0, y1 >> 8, y1};

    cmd_data_tx(CMD_CASET, x_dat, sizeof(x_dat));
    cmd_data_tx(CMD_PASET, y_dat, sizeof(y_dat));
}

/**
 * @brief Sets a pixel at given position
 * @param x[in] x coordinate
 * @param y[in] y coordinate
 * @param color[in] See @ref ILI9341_COLOR_x
 */
void ili9341_pixel_set(uint16_t x, uint16_t y, uint16_t color)
{
    rect_coords_set(x, y, x, y);

    cmd_tx(CMD_RAMWR);

    data16_tx(&color, 1);
}

/**
 * @brief Draws a filled rectangle
 * @param[in] x x coordinate of upper left corner of rectangle
 * @param[in] y y coordinate of upper left corner of rectangle
 * @param[in] width Width of rectangle
 * @param[in] height Height of rectangle
 * @param[in] color Color of the rectangle
 */
void ili9341_rect_fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
    uint16_t color)
{
    uint16_t h_cnt;
    rect_coords_set(x, y, x + width - 1, y + height - 1);

    cmd_tx(CMD_RAMWR);

    while(width-- > 0)
    {
        h_cnt = height;
        while (h_cnt-- > 0)
        {
            data16_tx(&color, 1);
        }
    }
}

/**
 * @brief Transmits command table
 * @param[in] table Table of commands
 */
static void cmd_table_do(const uint8_t *table)
{
    const uint8_t *p = table;
    uint8_t cmd;
    uint8_t size;

    while(1)
    {
        /* Read first table entry */
        cmd = *p++;
        if(cmd == TABLE_END)
        {
            return;
        }
        else if (cmd == TFTLCD_DELAY8)
        {
            p++;
        }
        else
        {
            /**
             * If first entry was neither TABLE_END nor TFTLCDDELAY
             *  it is a display command with N parameters.
            **/
            size = *p++;
            cmd_data_tx(cmd, (uint8_t *) p, size);
            /**
             * Increment pointer by N so it skips the
             *  next N parameters and points to the next command
             **/
            p = p + size;
        }
    }
}

/**
 * @brief Sets display orientation
 * @param[in] orientation Set orientation. See @ref ILI9341_ORIENT_E
 */
void ili9341_rotation_set(uint8_t orientation)
{
    uint8_t madctl_reg;

    dspi.x_p = 0;
    dspi.y_p = 0;

    dspi.orientation = orientation;
    switch (dspi.orientation)
    {
        case ILI9341_ORIENTATION_0:
            madctl_reg = (MADCTL_BGR);
            dspi.width  = TFTWIDTH;
            dspi.height = TFTHEIGHT;
            break;

        case ILI9341_ORIENTATION_90:
            madctl_reg = (MADCTL_MV | MADCTL_MX | MADCTL_BGR);
            dspi.width  = TFTHEIGHT;
            dspi.height = TFTWIDTH;
            break;

        case ILI9341_ORIENTATION_180:
            madctl_reg = (MADCTL_MY | MADCTL_MX | MADCTL_BGR);
            dspi.width  = TFTWIDTH;
            dspi.height = TFTHEIGHT;
            break;

        case ILI9341_ORIENTATION_270:
            madctl_reg = (MADCTL_MY | MADCTL_MV | MADCTL_BGR);
            dspi.width  = TFTHEIGHT;
            dspi.height = TFTWIDTH;
            break;

        default:
            return;
    }

    cmd_data_tx(CMD_MADCTL, &madctl_reg, sizeof(madctl_reg));
}

/**
 * @brief Applies inversion to all pixel data on screen
 * @param inversion[in] If non-zero, display is inverted.
 */
void ili9341_display_inversion(uint8_t inversion)
{
    uint8_t data = 0;

    if(inversion)
    {
        cmd_data_tx(CMD_INVON, &data, sizeof(data));
        return;
    }

    cmd_data_tx(CMD_INVOFF, &data, sizeof(data));
}

/**
 * @brief Enables display
 * @param enable[in] Non-zero value enables display, 0 disables it
 */
void ili9341_enable(uint8_t enable)
{
    if(enable)
    {
        cmd_tx(CMD_DISPON);
        return;
    }

    cmd_tx(CMD_DISPOFF);
}

/**
 * @brief Sets column pointer
 * @return RC_BUFF_OVERFLOW When an overflow occured. Cursor gets set to 0. See
 *  @ref PMI_RC_E
 * @return RC_SUCC When no overflow occured. See @ref PMI_RC_E
 */
static uint32_t column_set(uint8_t column)
{
    dspi.column = column;

    dspi.x_p = column * font->glyph->xAdvance;

    if(dspi.x_p >= dspi.width)
    {
        dspi.x_p = 0;
        /* Page overflow */
        return RC_BUFF_OVERFLOW;
    }

    return RC_SUCC;
}

/**
 * @brief Increase row pointer
 * @return RC_BUFF_OVERFLOW When an overflow occured. Cursor gets set to 0. See
 *  @ref PMI_RC_E
 * @return RC_SUCC When no overflow occured. See @ref PMI_RC_E
 */
static int32_t row_inc(void)
{
    dspi.y_p += font->yAdvance - Y_ADVANCE_OFFS;

    if(dspi.y_p >= dspi.height)
    {
        dspi.y_p = font->yAdvance - Y_ADVANCE_OFFS;
        /* Page overflow */
        return RC_BUFF_OVERFLOW;
    }

    return RC_SUCC;
}

/**
 * @brief Set row pointer
 * @return RC_SUCC When no overflow occured. See @ref PMI_RC_E
 */
static uint32_t row_set(uint8_t row)
{
    dspi.y_p = (font->yAdvance - Y_ADVANCE_OFFS) * (row + 1);

    if(dspi.y_p >= dspi.height)
    {
        dspi.y_p = font->yAdvance - Y_ADVANCE_OFFS;
        /* Page overflow */
        return RC_BUFF_OVERFLOW;
    }

    return RC_SUCC;
}

/**
 * @brief Sets text pointer. Has to be set before text is printed
 * @param column[in] Column
 * @param row[in] Row
 * @return See @ref PMI_RC_E, 0 if success
 */
uint32_t ili9341_text_pos_set(uint8_t column, uint8_t row)
{
    uint32_t rc;

    rc = column_set(column);
    if (rc)
    {
        return rc;
    }

    rc = row_set(row);

    return rc;
}

/**
 * @brief Puts a character on the screen
 * @note Use @ref ili9341_text_pos_set to set text position before
 * @note Text position is not changed, characters are overwritten use
 *  @ref ili9341_char_print if needed
 * @param c[in] Character to be printed
 * @param color[in] Font color @ref ILI9341_COLOR_x
 * @param bg_color[in] Background color @ref ILI9341_COLOR_x
 */
void ili9341_char_put(char c, uint16_t color, uint16_t bg_color)
{
    GFXglyph *glyph;
    int8_t xo, yo;
    uint8_t xx, yy, bits, bit;
    uint16_t b_off;
    uint8_t idx = (uint8_t) c;

    /* Determinte character offset */
    idx -= font->first;

    /* Initialise helper */
    glyph = &(font->glyph[idx]);

    /* Check bitmap size, skip if nothing has to be printed */
    if((glyph->width > 0) && (glyph->height > 0))
    {
        xo = glyph->xOffset;
        yo = glyph->yOffset;
        bits = 0;
        bit = 0;

        /* Check if char fits in current line, otherwise go to next row */
        if((dspi.x_p + xo + glyph->width) > dspi.width)
        {
            dspi.x_p = 0;
            row_inc();
        }

        /* Catch bug where address windows first coordinate is lower than
         * zero */
        if((int32_t) dspi.x_p + xo < 0)
        {
            xo = 0;
        }

        rect_coords_set(dspi.x_p + xo, dspi.y_p + yo,
            dspi.x_p + xo + glyph->width - 1,
            dspi.y_p + yo + glyph->height - 1);

        cmd_tx(CMD_RAMWR);

        b_off = glyph->bitmapOffset;
        for (yy = 0; yy < glyph->height; yy++)
        {
            for (xx = 0; xx < glyph->width; xx++)
            {
                if (!(bit++ & 0x07))
                {
                    bits = (font->bitmap[b_off++]);
                }

                if (bits & 0x80)
                {
                    data16_tx(&color, 1);
                }
                else
                {
                    data16_tx(&bg_color, 1);
                }

                bits <<= 1;
            }
        }
    }
    dspi.x_p += glyph->xAdvance;
}

/**
 * @brief Prints char and sets virtual cursor to the next position
 * @param c[in] Character to be printed
 * @param color[in] Font color @ref ILI9341_COLOR_x
 * @param bg_color[in] Background color @ref ILI9341_COLOR_x
 */
void ili9341_char_print(char c, uint16_t color, uint16_t bg_color)
{
    /* Catch non-printable and special characters */
    switch(c)
    {
        case '\r':
            /* Carriage return */
            column_set(0);
            break;
        case '\n':
            /* Line feed */
            row_inc();
            break;
        default:
            /* Determine if the char is printable with current font */
            if(c >= font->first && c <= font->last)
            {
                /* Check if cursor is too low */
                if(dspi.y_p < (font->yAdvance) - Y_ADVANCE_OFFS)
                {
                    dspi.y_p = (font->yAdvance) - Y_ADVANCE_OFFS;
                }

                ili9341_char_put(c, color, bg_color);
            }

            break;
    }
}

/**
 * @brief Prints a string
 * @param str[in] String
 * @param color[in] Font color @ref ILI9341_COLOR_x
 * @param bg_color[in] Background color @ref ILI9341_COLOR_x
 */
void ili9341_str_print(char *str, uint16_t color, uint16_t bg_color)
{
    while(*str)
    {
        ili9341_char_print(*str, color, bg_color);
        str++;
    }
}

/**
 * @brief Clears a string printed by @ref ili9341_str_print
 * @note Set start of clearance with @ref ili9341_text_pos_set
 * @param num_chars[in] Number of chars to be cleared
 * @param color[in] Color with which should be overwritten
 */
void ili9341_str_clear(uint16_t num_chars, uint16_t color)
{
    ili9341_rect_fill(dspi.x_p, dspi.y_p - dspi.font_height + Y_ADVANCE_OFFS,
        dspi.font_width * num_chars, dspi.font_height, color);
}

/**
 * @brief Draws a line
 * @param x0[in] x coordinate of point 1
 * @param y0[in] y coordinate of point 1
 * @param x1[in] x coordinate of point 2
 * @param y1[in] y coordinate of point 2
 * @param color[in] Color @ref ILI9341_COLOR_x
 */
void ili9341_line_draw(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
    uint16_t color)
{
    int16_t steep;
    int16_t dx, dy;
    int16_t err;
    int16_t ystep;

    steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
        swap_vals_int16(x0, y0);
        swap_vals_int16(x1, y1);
    }


    if (x0 > x1)
    {
        swap_vals_int16(x0, x1);
        swap_vals_int16(y0, y1);
    }

    dx = x1 - x0;
    dy = abs(y1 - y0);

    err = dx / 2;

    if (y0 < y1)
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    for (; x0 <= x1; x0++)
    {
        if (steep)
        {
            ili9341_pixel_set(y0, x0, color);
        }
        else
        {
            ili9341_pixel_set(x0, y0, color);
        }

        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

/**
 * @brief Applies common initialisation routines
 */
static void configure_std(void)
{
    cmd_table_do(tbl_reset_off);
    cmd_table_do(tbl_init_values);
    cmd_table_do(tbl_wake_on);

    /* Increment y pointer so first line gets printed correctly */
    dspi.y_p = font->yAdvance - Y_ADVANCE_OFFS;
}

/**
 * @brief Performs a hardware reset
 */
void ili9341_hw_reset(void)
{
    io_cs_set(IO_STATE_RELEASE);
    io_rd_set(IO_RS_DATA);
    io_wr_set(IO_STATE_RELEASE);
    io_rst_set(IO_STATE_ACTIVATE);

    systick_delay_ms(1);

    io_rst_set(IO_STATE_RELEASE);

    systick_delay_ms(2);

    io_rst_set(IO_STATE_ACTIVATE);

    systick_delay_ms(2);

    io_cs_set(IO_STATE_RELEASE);
}

/**
 * @brief Initialise display
 * @param rotation[in] See @ref ILI9341_ORIENTATION_E
 * @return See @ref PMI_RC_E
 */
int32_t ili9341_init(uint8_t rotation)
{
    int rc;

    rc = font_set(&FreeMonoBold12pt7b);
    if (rc)
    {
        return rc;
    }

    io_gpio_init();
    ili9341_hw_reset();
    configure_std();
    systick_delay_ms(40);

    switch (rotation)
    {
        case ILI9341_ORIENTATION_0:
            ili9341_rotation_set(ILI9341_ORIENTATION_0);
            break;

        case ILI9341_ORIENTATION_90:
            ili9341_rotation_set(ILI9341_ORIENTATION_90);
            break;

        case ILI9341_ORIENTATION_180:
            ili9341_rotation_set(ILI9341_ORIENTATION_180);
            break;

        case ILI9341_ORIENTATION_270:
            ili9341_rotation_set(ILI9341_ORIENTATION_270);
            break;

        default:
            break;
    }

    ili9341_rect_fill(0, 0, dspi.width, dspi.height, ILI9341_COLOR_BLACK);

    return RC_SUCC;
}


// uint32_t calculate_string_width(const char* str) 
// {
//     int width = 0;
//     while (*str) {
//         char c = *str++;
//         if (c >= font->first && c <= font->last) {
//             GFXglyph *glyph = &font->glyph[c - font->first];
//             width += glyph->xAdvance;
//         }
//     }
//     return width;
// }