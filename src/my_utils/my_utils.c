#include <my_utils.h>
#include <stm32l0xx.h>
#include <stdio.h>
#include <inttypes.h>

void initial_interrupt()
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // enables the clock for the System Configuration 

    GPIOC->MODER &= ~GPIO_MODER_MODE13;  // GPIOC as input (PIN C 13)

    SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI13_PC; //configures the source of the external interrupt on line 13
    EXTI->IMR |= EXTI_IMR_IM13; //  enables the interrupt request from EXTI line 13 
    EXTI->FTSR |= EXTI_FTSR_TR13; // configures EXTI line 13 to trigger an interrupt on the falling edge of the signal


    NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn,1);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}


void TIM2_Int_Init()
{
    
    // RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable timer2
    
    TIM2->PSC  = 160000-1; // not sure how to calculate this exactly
    TIM2->ARR = 500-1; // auto reload register

    TIM2->DIER |= TIM_DIER_UIE; // DMAdier

    NVIC_ClearPendingIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn,1);
    NVIC_EnableIRQ(TIM2_IRQn); // enable NVIC interrupt for timer 2

    TIM2->CR1 |= TIM_CR1_CEN; // enable timer2

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