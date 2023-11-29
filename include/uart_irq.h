#ifndef UART_IRQ_H
#define UART_IRQ_H

#include <pmi_stddefs.h>

int32_t init_uart_interrupt(uint32_t baudrate);
uint32_t uart_tx_str(char* buf);

#endif /* UART_IRQ_H */