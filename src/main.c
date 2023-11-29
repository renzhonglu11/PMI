#include <uart.h>
#include <clocks.h>
#include <stm32l0xx.h>
#include <i2c.h>
#include <mcp23017.h>
#include <utils.h>
#include <pmi_stddefs.h>
#include <ili9341.h>
#include <string.h>
#include <uart_irq.h>
#include <systick.h>
// #include <bmp_go_49x56.h>
// #include <bmp_stop_51x56.h>

void USART_SendByte(uint8_t byte) {
    while (!(USART2->ISR & USART_ISR_TXE)); // Wait for TXE flag
    USART2->TDR = byte; // Send the byte
}

void USART_SendString(char *string) {
    while (*string) {
        USART_SendByte(*string++);
    }
}


int main(void)
{
  /* Call your initialisations here */
  clocks_init_pmi();

  // uart_init_nucusb(115200);
  init_uart_interrupt(115200);


  init_mcp23017();
  ili9341_init(ILI9341_ORIENTATION_0);
  ili9341_enable(1);

  initial_interrupt();

  uint8_t gpio = 0x00;  // 0000 0100
  uint8_t gpio2 = 0x00;
  gpio ^= 0xff;
  gpio2 ^= 0xff;
  uint8_t config_iodir2[] = {gpio};
  uint8_t config_iodir3[] = {gpio2};

  // uint8_t config_iodir[] = { 0x00 };
  // write_mcp23017(MCP_IN_ADDR, MCP_IODIRB, config_iodir, sizeof(config_iodir)/sizeof(config_iodir[0]));
  // write_mcp23017(MCP_IN_ADDR, MCP_IODIRA, config_iodir, sizeof(config_iodir)/sizeof(config_iodir[0]));
  config_button();
  config_gpio(TRUE, 'A');
  config_gpio(TRUE, 'B');
  turn_on_led(MCP_IN_ADDR, MCP_GPIOA_ADDR, config_iodir3, ARRAY_SIZE(config_iodir3));
  turn_on_led(MCP_IN_ADDR, MCP_GPIOB_ADDR, config_iodir2, ARRAY_SIZE(config_iodir2));
  
  
  
  while (1)
  {

    // led_loop();

    // state_machine();


    // TODO: try to implement aufgabe 2c
    // e.g. state_machine();
    // 1. initial state of the state machine
    // 2. use interrupt to starte the state machine
    // 3. use state machine to control the led
    //  3.1. implement a function that can accept state machine as paramter to control led

    // state_machine();

    // 1. get the current state from state machine
    // 2.1. when get back to initial state, hold
    // 2.2. otherwise go to next state and hold for certain time according to the state machine
     


    // try to send data to uart
    
    while (!(USART2->ISR & USART_ISR_TXE)); // Wait for TXE flag

    USART_SendString("Gg\n");
    systick_delay_ms(2000);

  }
}