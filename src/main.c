#include <pmi.h>
#include <ds18b20.h>

int main(void)
{
  clocks_init_pmi();                     // default clock frequence is 16 MHz
  uart_init_nucusb(115200);
  init_proj_2();                        // initilize all the bus systems and sensors in project 2  
  // DS18B20_faster_resolution();          // lowest resolution(+-0.5) but get the correct temperature faster


  while (1)
  {
    send_sensor_data_over_UART();
    uart_tx_str("\n");
    systick_delay_ms(300);

  }
}