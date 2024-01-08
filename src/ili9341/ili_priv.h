#ifndef ILI_PRIV_H
#define ILI_PRIV_H

#include <ili9341.h>
#include <stm32l0xx.h>

/**
 * @brief Enable clock for all necessary ports and set every pin to output and
 *  the highest output speed configuration
 */
void io_gpio_init(void);

/**
 * @brief Signal states
 */
enum IO_STATE_E
{
    IO_STATE_RELEASE,
    IO_STATE_ACTIVATE
};

/**
 * @brief Setter for reset pin
 * @param state[in] Use @ref IO_STATE_E
 */
void io_rst_set(uint8_t state);

/**
 * @brief Setter for read control pin
 * @param state[in] Use @ref IO_STATE_E
 */
void io_rd_set(uint8_t state);

/**
 * @brief Setter for write control pin
 * @param state[in] Use @ref IO_STATE_E
 */
void io_wr_set(uint8_t state);

/**
 * @brief Setter for chip select pin
 * @param state[in] If non-zero, chip select is activated. Use @ref PMI_BOOL_E
 */
void io_cs_set(uint8_t state);

/**
 * @brief Set rs datapin state
 * @note Use with @ref io_rs_set
 */
enum IO_RS_E
{
    IO_RS_DATA,
    IO_RS_CMD
};

/**
 * @brief Setter for read or command pin
 * @param cmd[in] If non-zero, pin is set to command. Use @ref IO_RS_E
 */
void io_rs_set(uint8_t cmd);

/**
 * @brief Set all data pins to output
 */
void io_dpins_as_output(void);

/**
 * @brief Set all data pins to input
 */
void io_dpins_as_input(void);

/**
 * @brief Applies byte on data pins
 * @note Set pins to output first. See @ref io_dpins_as_output
 * @param b[in] Data byte, LSB corresponds to D0, MSB corresponds to D7
 */
void io_dpins_set(uint8_t b);

/**
 * @brief Gets current state on parallel bus
 * @note Pins have to be set as inputs by @ref ili9341_set_dir_read
 * @return Data mask where MSB corresponds to D0 and LSB corresponds to D7
 */
uint8_t io_dpins_get(void);

#endif /* ILI_PRIV_H */
