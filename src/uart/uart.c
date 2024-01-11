#include <uart.h>
#include <stm32l0xx.h>
#include <stdarg.h>
#include <stdio.h>

/**
 * @brief Initialises GPIOs for nucleo's serial-usb bridge. Pins are PA2
 * and PA3.
 */
static void gpio_init(void)
{
    /* Using IO PA2 and PA3 for USART2 in alternate function mode */

    /* Enable clock for GPIOA */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    /* PA2, PA3: Set port mode to 2 for "alternate function mode" */
    GPIOA->MODER |= (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);
    GPIOA->MODER &= ~(GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0);

    /* PA2, PA3: Choose alternate function 4 for USART2_TX/USART2_RX */
    GPIOA->AFR[0] |= ((4UL << GPIO_AFRL_AFSEL2_Pos) |
                      (4UL << GPIO_AFRL_AFSEL3_Pos));
    GPIOA->AFR[0] &= ~((11UL << GPIO_AFRL_AFSEL2_Pos) |
                       (11UL << GPIO_AFRL_AFSEL3_Pos));
}


/**
 * @brief Initialises USART2 as uart for use with nucleo's serial-usb bridge
 * @param[in] baudrate Baud rate to be used
 * @return See @ref PMI_RC_E
 */
int32_t uart_init_nucusb(uint32_t baudrate)
{
    /* Initialise GPIOs */
    gpio_init();

    /* Enable APB1-clock for USART2 */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Disable USART2, allow configuration */
    USART2->CR1 &= ~USART_CR1_UE;

    /* Set the baud rate */
    USART2->BRR |= SystemCoreClock / baudrate;

    /* Enable transmitter, enable receiver */
    USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE);

    /* Enable USART2, apply configuration */
    USART2->CR1 |= USART_CR1_UE;

    return RC_SUCC;
}

/**
 * @brief Transmits single character by polling
 * @param[in] c Character to transmit
 */
void uart_tx_char(char c)
{
    USART2->TDR = c;
    while(!(USART2->ISR & USART_ISR_TXE))
    {
    }
}

/**
 * @brief Receives a character by polling
 * @return Received character
 */
char uart_rx_char(void)
{
    /* Wait until character is received */
    while(!(USART2->ISR & USART_ISR_RXNE))
    {
    }

    /* Read character */
    return (char) USART2->RDR;
}


void uart_tx_int(uint16_t num)
{
    // Convert the integer to its ASCII representation
    char buffer[12]; // Assumes a 32-bit integer, so up to 10 digits plus a null terminator
    sprintf(buffer, "%d", num);

    // Transmit each character in the buffer
    for (int i = 0; buffer[i] != '\0'; ++i)
    {
        uart_tx_char(buffer[i]);
    }
}


void uart_tx_str(char *string)
{
    // int8_t i=0;
    // const char text[] = "Hello, World!\n";
    // while(buf[i]!='\0')

    while(*string!='\0')
    { 
        //int8_t char_int = buf[i];
        //uart_tx_char(char_int);
        //i++;
      
        uart_tx_char(*string);
        string++;
    }

}




int32_t uart_rx_str(char *buf, uint16_t size, uint16_t *len)
{

    int i=0;
 

    while(i<size-2 )
    {
        
      char tmp_char = uart_rx_char();

        if(tmp_char == '\n')
        {
            break;
        }

        buf[i] = tmp_char;

        i++;
        *len += 1;
    }
    
    buf[i] = '\0';
  

    return *len;
}


int32_t uart_rx_uint(uint16_t *val)
{   

    char input_txt;
    uint8_t i = 0;
    *val = 0;

    while(1){
      if(input_txt == '\n'){
        break;
      }

      input_txt = uart_rx_char();
        
      uint16_t input_int = atoi(&input_txt);
      for(int j = 0;j<i;j++)
      {
          input_int = input_int * 10;
      }

      *val = input_int + *val;
      i++;
    }


    return *val;
    
}