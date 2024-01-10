#include <uart.h>
#include <clocks.h>
#include "pmi.h"

int main(void)
{
    /* Call your initialisations here */
    clocks_init_pmi();
    uart_init_nucusb(115200);

    TIM6_init();
    initialize_gpio();


    while(1)
    {
        /* Call your routines here */
        
        
    }
}