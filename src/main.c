#include <uart.h>
#include <clocks.h>
#include <stm32l0xx.h>
#include <systick.h>



void initial_interrupt()
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI13_PC;
    EXTI->IMR |= EXTI_IMR_IM13;

    EXTI->FTSR |= EXTI_FTSR_TR13;

    NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn,1);

    NVIC_EnableIRQ(EXTI4_15_IRQn);
}


void EXTI4_15_IRQHandler(void)
{
    if(EXTI->PR&EXTI_PR_PIF13)
    {
        GPIOA->ODR ^= GPIO_BSRR_BS_5;
        EXTI->PR = EXTI_PR_PIF13;
    }
}


int main(void)
{
    /* Call your initialisations here */
    clocks_init_pmi();
    uart_init_nucusb(115200);

    RCC->IOPENR |= RCC_IOPENR_GPIOAEN; 
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // enable clock for SYSCFG
   

    GPIOA->MODER |= GPIO_MODER_MODE5_0;
    GPIOA->MODER &= ~(GPIO_MODER_MODE5_1); 


    GPIOC->MODER &= ~GPIO_MODER_MODE13;  // input

    // Enable Pull-Up
    // GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_0; // Set LSB => 0b…XXXX1
    // GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD13_1); // Reset MSB => 0b…XXX0X



    initial_interrupt();

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
        // char buf[10];
        // char *char_ptr = buf;
        // uint16_t a= 0;
        // uint16_t  *len_ptr = &a;

        // uart_rx_str(char_ptr, 10, len_ptr);
      
        
        // uart_tx_str(buf);

        // uint16_t my_int = 0;
        
        // uart_rx_uint(&my_int); // Die Reihenfolge ist umgekehrt.

        // uint16_t b = 10;

        // toggle_led_with_input();


        // if(!(GPIOC->IDR & GPIO_IDR_ID13))
        // {
        //    GPIOA->BSRR = GPIO_BSRR_BS_5;
        // }else{
        //     GPIOA->BRR = GPIO_BSRR_BS_5;
        // }
        

        // systick_delay_ms(1000);
    }
}