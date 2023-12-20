#include <mpi.h>


int main(void)
{
  clocks_init_pmi();  // default clock frequence is 16 MHz

  uart_init_nucusb(115200);
  init_proj_2();
  
  ili9341_init(ILI9341_ORIENTATION_90); // initilize the LCD screen

  


  while (1)
  {
    
    // _qmc5883l_internal_test();  // testing I2C

    output_data(); // output all the data from sensors

    systick_delay_ms(3000);

  }
}