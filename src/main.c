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
    
    send_sensor_data_over_UART();
    uart_tx_str("\n");
    
    systick_delay_ms(500);

  }
}