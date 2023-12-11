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
      _test_adxl345();
      adxl345_get_data();

    }
}