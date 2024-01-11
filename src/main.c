#include <uart.h>
#include <clocks.h>
#include "pmi.h"

int main(void)
{
    /* Call your initialisations here */
    clocks_init_pmi();
    uart_init_nucusb(115200);


    adc_init();

    TIM6_init();
    TIM2_init();
    initialize_gpio();
    
   

    while(1)
    {
        /* Call your routines here */
        
        TIM2->CR1 |= TIM_CR1_CEN;
        
        
    }
}