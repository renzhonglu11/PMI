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



int main(void)
{
  /* Call your initialisations here */
  clocks_init_pmi();

  // uart_init_nucusb(115200);
  init_uart_interrupt(115200);
  TIM2_Init();          // timeout timer for exe 3

  init_mcp23017();
  ili9341_init(ILI9341_ORIENTATION_0);
  ili9341_enable(1);

  initial_interrupt();
  config_button();
  config_gpio(TRUE, 'A');
  config_gpio(TRUE, 'B');
  _turn_off_leds();


  

  while (1)
  {

    // led_loop();

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
    



    if (uart_timeout_flag) {

        
        uart_timeout_leds();
                
    }
    else{
      state_machine();
    }



  }
}