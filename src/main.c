#include <uart.h>
#include <clocks.h>
#include <stm32l0xx.h>
#include <systick.h>
#include <stdio.h>
#include <inttypes.h>
#include <my_utils.h>



void TIM3_PWM_Init(int16_t *arr, int16_t *incr)
{
    

    RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST; 
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;
    
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable timer2
    
    GPIOA->MODER |= GPIO_MODER_MODE5_1;
    GPIOA->MODER &= ~(GPIO_MODER_MODE5_0); 

    TIM2->PSC  = 1600-1;

    char input = uart_rx_char();
    if(input == '1')
    {
      *arr += *incr;
      TIM2->ARR = *arr - 1;
    }else if(input == '0')
    {
      *arr -= *incr;
      TIM2->ARR = *arr - 1;
    }else
    {
      TIM2->ARR = *arr - 1;
    }
    
    
    TIM2->CCR1 = 10; // compare register


    // set to 111, configure filter
    TIM2->CCMR1 = TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE;
  

    TIM2->CCER |= TIM_CCER_CC1E; // capture/compare 1 output enable
    TIM2->CR1 |= TIM_CR1_CEN; // time2 enable

    // set GPIOA to output
    // GPIOA->MODER |= GPIO_MODER_MODE5_0;
    // GPIOA->MODER &= ~(GPIO_MODER_MODE5_1); 

    GPIOA->AFR[0] = 5 << GPIO_AFRL_AFSEL5_Pos;
    
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
    if(EXTI->PR & EXTI_PR_PIF13) // pending bit of EXTI 13
    {
        EXTI->PR = EXTI_PR_PIF13; // write one to corresponding pending bit to clear pending bit
        GPIOA->ODR ^= GPIO_BSRR_BS_5; 
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

    // Enable Pull-Up
    // GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_0; // Set LSB => 0b…XXXX1
    // GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD13_1); // Reset MSB => 0b…XXX0X

    // initial_interrupt();
    TIM2_Int_Init();
    // TIM3_PWM_Init();

    

    const char text[] = "Hello, World!\n";
    int16_t data[] = {0, 123, 42, -399};

    int16_t arr_incr = 100;
    int16_t arr = 2;

    

    while(1)
    {   
        // TIM3_PWM_Init(&arr, &arr_incr);
        // toggle_led_with_input();
        char input = uart_rx_char();
        uart_tx_char(input);
        // if(input == '1')
        // {
                   
        //   if(arr + arr_incr>1000)
        //   {
        //     continue;
        //   }
        //   arr += arr_incr;
        //   // TIM3_PWM_Init(arr);
            
        // }else if(input == '0')
        // {
        //   if(arr -arr_incr<0){
        //     continue;
        //   }
        //   arr -= arr_incr;
        //   // TIM3_PWM_Init(arr);
        // }

        // if(input == '1')
        // {
        //   uart_tx_char(input);
        //   if(arr + arr_incr>1000)
        //   {
        //     continue;
        //   }
        //   arr += arr_incr;
        //   TIM3_PWM_Init(arr);
            
        // }else if(input == '0')
        // {
        //   uart_tx_char(input);
        //   if(arr -arr_incr<0){
        //     continue;
        //   }
        //   arr -= arr_incr;
        //   TIM3_PWM_Init(arr);
        // }
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