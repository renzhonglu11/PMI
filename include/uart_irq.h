#ifndef UART_IRQ_H
#define UART_IRQ_H

#include <pmi_stddefs.h>
#include <stdint.h>

// Using volattile to avoid compiler optimization which may
// cause variable in inteerupt be optimized
extern volatile uint32_t uart_last_rx_time;
extern volatile uint8_t uart_timeout_flag;
extern const uint32_t UART_TIMEOUT;




uint32_t uart_tx_str(char* buf);
uint32_t uart_tx_int(uint32_t val);
int32_t init_uart_interrupt(uint32_t baudrate);
void TIM2_Init(void);
#endif /* UART_IRQ_H */