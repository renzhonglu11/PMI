#include <uart.h>
#include <clocks.h>
#include <stm32l0xx.h>
#include <systick.h>
#include <stdio.h>
#include <inttypes.h>

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

void TIM3_PWM_Init()
{
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable timer2


    TIM2->PSC  = 16000-1;
    TIM2->ARR = 500-1; 

    // set to 111
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_0;
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_1;
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_2; 

    TIM2->CCER |= TIM_CCER_CC1E; // enable capture


    // set GPIOA to output
    GPIOA->MODER |= GPIO_MODER_MODE5_0;
    GPIOA->MODER &= ~(GPIO_MODER_MODE5_1); 

    TIM2-> |= TIM_CR1_ARPE; // auto reload enable
    TIM2->CR1 |= TIM_CR1_CEN; // time2 enable
    
}


void TIM2_Int_Init()
{
    // RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable timer2
    
    TIM2->PSC  = 16000-1;
    TIM2->ARR = 500-1; // auto reload register

    TIM2->DIER |= TIM_DIER_UIE; // DMA

    NVIC_ClearPendingIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn,1);
    NVIC_EnableIRQ(TIM2_IRQn); // enable NVIC interrupt for timer 2

    TIM2->CR1 |= TIM_CR1_CEN;

}

void TIM2_IRQHandler( void )
{
    if(TIM2->SR & TIM_SR_UIF)
    {
        GPIOA->ODR ^= GPIO_BSRR_BS_5;
        
        TIM2->SR &= ~TIM_SR_UIF;
    }
}


void EXTI4_15_IRQHandler(void)
{
    if(EXTI->PR&EXTI_PR_PIF13)
    {
        GPIOA->ODR ^= GPIO_BSRR_BS_5;
        
        EXTI->PR = EXTI_PR_PIF13;
    }
}


int32_t max_value(int16_t *data, uint8_t len,int16_t *max)
{
    if(len == 0)
    {
        return 0; // len darf nicht 0 sein.
    }

    *max = data[0];

    for(int i = 0;i<len;i++)
    {
        if(data[i] > *max)
        {
            *max = data[i];
        }
    }

    return *max; // using the value from template as success code
}


int16_t *max_value_ptr(int16_t *data, uint8_t len)
{
    if(len == 0)
    {
        return 0; // len darf nicht 0 sein.
    }

    int16_t *max = &data[0]; // Pointer auf das erste Element.

    for(int i = 0;i<len;i++)
    {
        if(data[i] > *max)
        {
            max = &data[i];
        }
    }

    return max;
}


void print_array(int16_t *data, uint8_t len)
{
    for(int i = 0;i<len;i++)
    {
        char charArray[20];
        sprintf(charArray, "%" PRId16, data[i]);
        
        uart_tx_str(charArray);
        if(i == len - 1)
        {
            uart_tx_str("\n");
            continue;
        }
        uart_tx_str(", ");
        
    }
}


int32_t int2string(char *buf, uint16_t size, int16_t number,uint16_t *len)
{
    int i = 0;
    int is_neg = 0;

    if(number<0)
    {
      is_neg = 1;
      number = -number;
    }

    do {
        buf[i++] = '0' + (number % 10);
        number /= 10;
    } while (number > 0);

    if (is_neg) {
        buf[i++] = '-';
    }

    if(i>size)
    {
        return -1; // buffer overflow
    }

    buf[i] = '\0'; // end string


    // reverse string
    int length = i;
    for(int j=0;j<length/2;j++)
    {
        char tmp = buf[j];
        buf[j] = buf[length-j-1];
        buf[length-j-1] = tmp;
    }


    *len = length;

    return length;
}




int main(void)
{
    /* Call your initialisations here */
    clocks_init_pmi();
    uart_init_nucusb(115200);

    RCC->IOPENR |= RCC_IOPENR_GPIOAEN; 
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // enable clock for SYSCFG
   
    
    // GPIOA->MODER |= GPIO_MODER_MODE5_0;
    // GPIOA->MODER &= ~(GPIO_MODER_MODE5_1); 




    GPIOC->MODER &= ~GPIO_MODER_MODE13;  // input

    // Enable Pull-Up
    // GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_0; // Set LSB => 0b…XXXX1
    // GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD13_1); // Reset MSB => 0b…XXX0X
    // initial_interrupt();
    TIM2_Int_Init();

    const char text[] = "Hello, World!\n";
    int16_t data[] = {0, 123, 42, -399};



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

        // int16_t my_max = 0;
        // int16_t *max_ptr;
        // max_value(data,4,&my_max);
        // max_ptr = max_value_ptr(data,4);
        

        // *max_ptr = 1337; // change the corresponding value in data array

        // uint16_t max_len = 0;
        // char max_buf[20];
        // int2string(max_buf, 10, *max_ptr, &max_len);
        // uart_tx_str(max_buf);
        // uart_tx_str("\n");
         
        // int data_len = sizeof(data) / sizeof(data[0]);
        // print_array(data,data_len);
        
        // for(int i = 0;i<data_len;i++)
        // {
        //     char charArray[20];
        //     sprintf(charArray, "%" PRId16, data[i]);
            
        //     uart_tx_str(charArray);
        //     if(i == data_len - 1)
        //     {
        //         uart_tx_str("\n");
        //         continue;
        //     }
        //     uart_tx_str(", ");
            
        // }
        
        // GPIOA->BRR = GPIO_BSRR_BS_5; // LED OFF


        // systick_delay_ms(1000);
    }
}