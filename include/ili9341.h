#ifndef ILI9341_H
#define ILI9341_H
#include <pmi_stddefs.h>

/* ILI9341 pre-defined colors in RGB565 (16-bit) format
                                    RGB565  RGB888 (24-bit) */
#define ILI9341_COLOR_BLACK         0x0000  //   0,   0,   0
#define ILI9341_COLOR_NAVY          0x000F  //   0,   0, 123
#define ILI9341_COLOR_DARKGREEN     0x03E0  //   0, 125,   0
#define ILI9341_COLOR_DARKCYAN      0x03EF  //   0, 125, 123
#define ILI9341_COLOR_MAROON        0x7800  // 123,   0,   0
#define ILI9341_COLOR_PURPLE        0x780F  // 123,   0, 123
#define ILI9341_COLOR_OLIVE         0x7BE0  // 123, 125,   0
#define ILI9341_COLOR_LIGHTGREY     0xC618  // 198, 195, 198
#define ILI9341_COLOR_DARKGREY      0x7BEF  // 123, 125, 123
#define ILI9341_COLOR_BLUE          0x001F  //   0,   0, 255
#define ILI9341_COLOR_GREEN         0x07E0  //   0, 255,   0
#define ILI9341_COLOR_CYAN          0x07FF  //   0, 255, 255
#define ILI9341_COLOR_RED           0xF800  // 255,   0,   0
#define ILI9341_COLOR_MAGENTA       0xF81F  // 255,   0, 255
#define ILI9341_COLOR_YELLOW        0xFFE0  // 255, 255,   0
#define ILI9341_COLOR_WHITE         0xFFFF  // 255, 255, 255
#define ILI9341_COLOR_ORANGE        0xFD20  // 255, 165,   0
#define ILI9341_COLOR_GREENYELLOW   0xAFE5  // 173, 255,  41
#define ILI9341_COLOR_PINK          0xFC18  // 255, 130, 198

/**
 * @brief Display orientation
 */
enum ILI9341_ORIENTATION_E
{
    ILI9341_ORIENTATION_0,
    ILI9341_ORIENTATION_90,
    ILI9341_ORIENTATION_180,
    ILI9341_ORIENTATION_270
};

/**
 * @brief Structure holding common display infos
 */
struct display_info_s
{
    uint16_t width;
    uint16_t height;

    uint16_t x_p;
    uint16_t y_p;

    uint8_t orientation;
    uint8_t column;

    int16_t font_width;
    int16_t font_height;
};

/* ili9341.c */

/**
 * @brief Get current display settings
 * @return Current infos
 */
struct display_info_s ili9341_display_info_get(void);

/**
 * @brief Sets a pixel at given position
 * @param x[in] x coordinate
 * @param y[in] y coordinate
 * @param color[in] See @ref ILI9341_COLOR_x
 */
void ili9341_pixel_set(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Draws a filled rectangle
 * @param[in] x x coordinate of upper left corner of rectangle
 * @param[in] y y coordinate of upper left corner of rectangle
 * @param[in] width Width of rectangle
 * @param[in] height Height of rectangel
 * @param[in] color Color of the rectangle
 */
void ili9341_rect_fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
    uint16_t color);

/**
 * @brief Sets display orientation
 * @param[in] orientation Set orientation. See @ref ILI9341_ORIENT_E
 */
void ili9341_rotation_set(uint8_t orientation);

/**
 * @brief Applies inversion to all pixel data on screen
 * @param inversion[in] If non-zero, display is inverted.
 */
void ili9341_display_inversion(uint8_t inversion);

/**
 * @brief Sets text pointer. Has to be set before text is printed
 * @param column[in] Column
 * @param row[in] Row
 * @return See @ref PMI_RC_E, 0 if success
 */
uint32_t ili9341_text_pos_set(uint8_t column, uint8_t row);

/**
 * @brief Puts a character on the screen
 * @note Use @ref ili9341_text_pos_set to set text position before
 * @note Text position is not changed, characters are overwritten use
 *  @ref ili9341_char_print if needed
 * @param c[in] Character to be printed
 * @param color[in] Font color @ref ILI9341_COLOR_x
 * @param bg_color[in] Background color @ref ILI9341_COLOR_x
 */
void ili9341_char_put(char c, uint16_t color, uint16_t bg_color);

/**
 * @brief Prints char and sets virtual cursor to the next position
 * @param c[in] Character to be printed
 * @param color[in] Font color @ref ILI9341_COLOR_x
 * @param bg_color[in] Background color @ref ILI9341_COLOR_x
 */
void ili9341_char_print(char c, uint16_t color, uint16_t bg_color);

/**
 * @brief Prints a string
 * @param str[in] String
 * @param color[in] Font color @ref ILI9341_COLOR_x
 * @param bg_color[in] Background color @ref ILI9341_COLOR_x
 */
void ili9341_str_print(char *str, uint16_t color, uint16_t bg_color);

/**
 * @brief Clears a string printed by @ref ili9341_str_print
 * @note Set start of clearance with @ref ili9341_text_pos_set
 * @param num_chars[in] Number of chars to be cleared
 * @param color[in] Color with which should be overwritten
 */
void ili9341_str_clear(uint16_t num_chars, uint16_t color);

/**
 * @brief Draws a line
 * @param x0[in] x coordinate of point 1
 * @param y0[in] y coordinate of point 1
 * @param x1[in] x coordinate of point 2
 * @param y1[in] y coordinate of point 2
 * @param color[in] Color @ref ILI9341_COLOR_x
 */
void ili9341_line_draw(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
    uint16_t color);

/**
 * @brief Enables display
 * @param enable[in] Non-zero value enables display, 0 disables it
 */
void ili9341_enable(uint8_t enable);

/**
 * @brief Performs a hardware reset
 */
void ili9341_hw_reset(void);

/**
 * @brief Initialise display
 * @param rotation[in] See @ref ILI9341_ORIENTATION_E
 * @return See @ref PMI_RC_E
 */
int32_t ili9341_init(uint8_t rotation);

/* test.c */

/**
 * @brief Do a test and benchmark routine
 * @return Execution time in miliseconds
 */
uint32_t ili9341_test(void);

/**
 * @brief Prints an endless matrix like screen routine
 */
void ili9341_enterthematrix(void);

#endif /* ILI9341_H */