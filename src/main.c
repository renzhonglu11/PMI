#include <uart.h>
#include <clocks.h>
#include <mpi.h>
#include <adxl345.h>
#include <stdio.h>
#include <pmi_string.h>
#include <systick.h>
#include <my_utils.h>

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
      // float x =0.0;
      // float y =0.0;
      // float z =0.0;

      // int32_t success_get_data =  adxl345_acc_data(&x,&y,&z);
  


      // sprintf(StrBuff,"X=%.3f    m/s2   Y=%.3f  m/s2   Z=%.3f  m/s2 \r\n",data.x*3.9/1000*9.8,data.y*3.9/1000*9.8,data.z*3.9/1000*9.8);

      // char buffer[20];


      // float2str(buffer,elements_of(buffer),x,3);




      // uart_tx_str(buffer);
      // uart_tx_char('\n');

      output_data();
      systick_delay_ms(100);

    }
}