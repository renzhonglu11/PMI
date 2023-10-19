#include <uart.h>
#include <clocks.h>
#include <stm32l0xx.h>
#include <systick.h>


int main(void)
{
    /* Call your initialisations here */
    clocks_init_pmi();
    uart_init_nucusb(115200);

    RCC->IOPENR |= RCC_IOPENR_GPIOAEN; 

    GPIOA->MODER |= GPIO_MODER_MODE5_0;
    GPIOA->MODER &= ~(GPIO_MODER_MODE5_1); 

    const char text[] = "Hello, World!\n";


    while(1)
    {
        /* Call your routines here */

        //GPIOA->ODR ^= GPIO_ODR_OD5;
        //systick_delay_ms(1000);

        //GPIOA->BSRR = GPIO_BSRR_BS_5;
        //systick_delay_ms(100);
        //GPIOA->BRR = GPIO_BSRR_BS_5;
        //systick_delay_ms(100);


        // uart_tx_str(text);
        char buf [10];
        uint16_t  len = 0;

        uart_rx_str(&buf, 10, &len);
      
        uint16_t int_arr[len];
        // uart_tx_str(buf);

        uint16_t a = 10;

        uart_rx_uint(&int_arr);

        systick_delay_ms(1000);
    }
}