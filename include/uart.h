#ifndef UART_H
#define UART_H

#include <pmi_stddefs.h>

/**
 * @brief Initialises USART2 as uart for use with nucleo's serial-usb bridge
 * @param[in] baudrate Baud rate to be used
 * @return See @ref PMI_RC_E
 */
int32_t uart_init_nucusb(uint32_t baudrate);

/**
 * @brief Transmits single character by polling
 * @param[in] c Charakter to transmit
 */
void uart_tx_char(char c);

/**
 * @brief Receives a character by polling
 * @return Received character
 */
char uart_rx_char(void);

#endif /* UART_H */
