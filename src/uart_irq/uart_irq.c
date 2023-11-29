#include <uart.h>
#include <stm32l0xx.h>
#include <stdarg.h>
#include <stdio.h>
#include <uart_irq.h>
#include <mcp23017.h>


volatile uint32_t uart_last_rx_time = 0;
volatile uint8_t uart_timeout_flag = 0;
const uint32_t UART_TIMEOUT = 3;


uint32_t uart_tx_str(char* buf)
{
    
    int i = 0;
    while (buf[i] != '\0')
    {
        // Read char from buffer
        uart_tx_char(buf[i]);
        i++;
    }

    return RC_SUCC;
}

uint32_t uart_tx_int(uint32_t val)
{
    char str[20];
    sprintf(str, "%ld", val);
    uart_tx_str(str);
    return RC_SUCC;
}

/// @brief interrupt handler for UART2
/// @param  
void USART2_IRQHandler(void)
{
  // RXNE is 0 when data is not yet shifted out of the shift register
  if (USART2->ISR & USART_ISR_RXNE)
  {                                      // Check if the RXNE flag is set
    uint8_t received_char = USART2->RDR; // Read the received data
    
    
    uart_last_rx_time = 0;                  // Reset the timeout flag
    uart_timeout_flag = 0;                  // reset the timeout flag
    



    // Handle the received data
    while ((USART2->ISR & USART_ISR_TXE) == 0); // Wait until transmit complete

    USART2->TDR = received_char;


    // TODO: after receive the character. FSM should be set to corresponding state
  }

  // Handle other USART2 interrupts like transmission complete, if needed
  // ...
}

/// @brief Timer interrupt for timeout counter
/// @param  
void TIM2_IRQHandler(void) 
{
    if (TIM2->SR & TIM_SR_UIF) { // Check if update interrupt flag is set
        TIM2->SR &= ~TIM_SR_UIF; // Clear the update interrupt flag

        uart_last_rx_time++;
        if (uart_last_rx_time >= UART_TIMEOUT) {
            // Set the timeout flag
            uart_timeout_flag = 1;
            

        }
    }
}

/// @brief Initialise TIMER2 (tick in every 1 second with timer frequency of 64MHZ)
/// @param  
void TIM2_Init(void) {
    // Enable clock for TIM2 and configure the timer
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 639999;
    TIM2->ARR = 500;
    TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt
    TIM2->CR1 |= TIM_CR1_CEN; // Start the timer

    NVIC_SetPriority(TIM2_IRQn,1);
    // Enable TIM2 interrupt in NVIC
    NVIC_EnableIRQ(TIM2_IRQn);
}






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

/// @brief initialise the UART interrupt
/// @param baudrate 
/// @return 
int32_t init_uart_interrupt(uint32_t baudrate)
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

  USART2->CR1 |= USART_CR1_RXNEIE; // enable Receive Data Register Not Empty to receive data using interrupt


  USART2->CR1 &= ~USART_CR1_M0; // 8 data bits
  USART2->CR1 &= ~USART_CR1_M1; 

  /* Enable USART2, apply configuration */
  USART2->CR1 |= USART_CR1_UE;

  NVIC_EnableIRQ(USART2_IRQn);
  NVIC_SetPriority(USART2_IRQn, 2);

  return RC_SUCC;
}