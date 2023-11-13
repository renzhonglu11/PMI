#include <uart.h>
#include <clocks.h>
#include <stm32l0xx.h>
#include <i2c.h>
#include <mcp23017.h>
#include <utils.h>
#include <pmi_stddefs.h>

int main(void)
{
  /* Call your initialisations here */
  clocks_init_pmi();
  uart_init_nucusb(115200);
  int32_t init_mcp = init_mcp23017();
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
  int32_t b = turn_on_led(MCP_IN_ADDR, MCP_GPIOA_ADDR, config_iodir3, ARRAY_SIZE(config_iodir3));
  int32_t a = turn_on_led(MCP_IN_ADDR, MCP_GPIOB_ADDR, config_iodir2, ARRAY_SIZE(config_iodir2));
  
  while (1)
  {
    // int8_t a = D4_D9;
    // LIGHT a = D4_D9; 
    // // // uint8_t gpio5 = 0x01;
    // a ^= 0xff;
    // uint8_t config_iodir4[] = {a};


    // turn_on_led(MCP_IN_ADDR, MCP_GPIOA_ADDR, config_iodir4, ARRAY_SIZE(config_iodir4));

    // turn_on_led(MCP_IN_ADDR, MCP_GPIOB_ADDR, config_iodir4, ARRAY_SIZE(config_iodir4));
    led_loop();

  }
}