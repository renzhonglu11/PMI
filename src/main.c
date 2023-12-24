#include <mpi.h>
#include <ds18b20.h>

int main(void)
{
  clocks_init_pmi();  // default clock frequence is 16 MHz

  uart_init_nucusb(115200);
  init_proj_2();
  ili9341_init(ILI9341_ORIENTATION_90); // initilize the LCD screen
  
  DS18B20_faster_resolution();


  while (1)
  {
    
    // _qmc5883l_internal_test();  // testing I2C

    // output_data(); // output all the data from sensors


    
    // if(OneWire_Reset())
    // {
    //   uart_tx_str("OneWire_Reset() is working\n");

    // }
    // else
    // {
    //   uart_tx_str("OneWire_Reset() is not working\n");

    // }
    float temp;
    char T_ow[30];
    DS18B20_Get_Temp(&temp);

    float2str(T_ow,elements_of(T_ow),temp,2);
    uart_tx_str(T_ow);
    uart_tx_str("\n");
    
    systick_delay_ms(300);

  }
}