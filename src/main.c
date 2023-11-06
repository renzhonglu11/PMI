#include <uart.h>
#include <clocks.h>
#include <stm32l0xx.h>
#include <i2c.h>
#include <mcp23017.h>

#define MCP_IN_ADDR 0X40
#define MCP_IN_ADDR_read 0X41
#define GPIOB_ADDR 0x13

int main(void)
{
    /* Call your initialisations here */
    clocks_init_pmi();
    uart_init_nucusb(115200);


    int32_t res = init_mcp23017();
   

    while(1)
    {
        /* Call your routines here */
        // GPIOB->BSRR = GPIO_BSRR_BS_8;
        // GPIOB->BSRR = GPIO_BSRR_BS_9;
        uint8_t *buf;
        *buf = 0x11;

        int32_t res = i2c_hw_tx_device(MCP_IN_ADDR, GPIOB_ADDR, &buf, 2);
        int a =1;
        systick_delay_ms(1000);
    }
}