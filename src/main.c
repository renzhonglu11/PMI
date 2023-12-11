#include <uart.h>
#include <clocks.h>
#include <mpi.h>
#include <adxl345.h>
#include <stdio.h>


int main(void)
{
    /* Call your initialisations here */
    clocks_init_pmi();
    // uart_init_nucusb(115200);
    uart_init_nucusb(115200);
 
    ili9341_init(ILI9341_ORIENTATION_90);  // initilize the LCD screen

    adxl345_init();

    
    char uart_buf[] = "SPI Test\r\n";
    uint8_t uart_buf_len;
    char spi_buf[20];
    uint8_t spi_buf_len;


    uint8_t device_id_addr[] = {DEVID,DEVID,DEVID};
    uint8_t device_id_addr_len;

    uart_buf_len = elements_of(uart_buf);
    spi_buf_len = elements_of(spi_buf);
    device_id_addr_len = elements_of(device_id_addr);

    



    while(1)
    {
      // get the data from the sensor adxl345

      // print it out to UART 
      // _test_adxl345();
 

      struct AccelerometerData data =  adxl345_get_data();
  


      // sprintf(StrBuff,"X=%.3f    m/s2   Y=%.3f  m/s2   Z=%.3f  m/s2 \r\n",data.x*3.9/1000*9.8,data.y*3.9/1000*9.8,data.z*3.9/1000*9.8);
      double my_num = data.x*3.9/1000*9.8;


      // Convert the double to fixed-point representation (multiply by a factor, e.g., 1000)
      int fixed_point = (int)(my_num * 1000);

      // Transmit the fixed-point value through UART
      char buffer[20];  // Adjust the size based on your needs
      snprintf(buffer, sizeof(buffer), "%d.%03d", fixed_point / 1000, fixed_point < 0 ? -fixed_point % 1000 : fixed_point % 1000);

   


      uart_tx_str_signed(buffer);

      uart_tx_char('\n');

    }
}