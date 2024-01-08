#include <uart.h>
#include <clocks.h>


int main(void)
{
    /* Call your initialisations here */
    clocks_init_pmi();
    uart_init_nucusb(115200);

    while(1)
    {
        /* Call your routines here */
        
    }
}