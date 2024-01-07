#include <pmi.h>



/**
 * @brief Initializes project 2.
 *
 * This function initializes project 2 by performing necessary setup and configuration.
 *
 * @return The status of the initialization process.
 */
uint32_t init_proj_2(void)
{
  adxl345_init();
  qmc5883l_init();
  while(init_ds18b20()==1)
  {
    uart_tx_str("ds18b20 init fails!\n");
    systick_delay_ms(1000);  // wait for 1 sec
  }
  ili9341_init(ILI9341_ORIENTATION_90); // initilize the LCD screen

  return RC_SUCC;
}