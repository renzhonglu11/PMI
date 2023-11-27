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

void USART2_IRQHandler(void)
{
    
}


int32_t init_uart_interrupt()
{
    USART2->CR1 |=  USART_CR1_RXNEIE;  // enable intterupt

    
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn,2);


}
