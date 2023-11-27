#include <uart.h>
#include <stm32l0xx.h>
#include <stdarg.h>
#include <stdio.h>

#include <uart_irq.h>

uint32_t uart_rx_str(char *buf, uint16_t size, uint16_t *len, char end)
{
    // uart receives string
    
    uint32_t i = 0;
    char c = uart_rx_char();
    while (c != end && i < size - 1)
    {
        *(buf + i) = c;
        i++;
        c = uart_rx_char();
    }
    *(buf + i) = '\0';
    *len = i;
    return RC_SUCC;
}
uint32_t uart_tx_str(char *buf)
{ // Iterate over text array
    int i = 0;
    while (buf[i] != '\0')
    { // Read char from buffer
        uart_tx_char(buf[i]);
        i++;
    }
    return RC_SUCC;
}
uint32_t uart_rx_int(uint32_t *val)
{
    uint16_t size = 20;
    uint16_t len = 0;
    char *str = (char *)malloc(size * sizeof(char));
    uart_rx_str(str, size, &len, '\r');
    uart_tx_str(str);
    *val = (uint32_t)strtol(str, (char **)NULL, 10);
    free(str);
    return RC_SUCC;
}
uint32_t uart_tx_int(uint32_t val)
{
    char str[20];
    sprintf(str, "%ld", val);
    uart_tx_str(str);
    return RC_SUCC;
}
uint32_t uart_tx_float(float val, uint8_t resolution)
{
    char str[20];
    float2str(str, 20, val, resolution);
    uart_tx_str(str);
    return RC_SUCC;
}
