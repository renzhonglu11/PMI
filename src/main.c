#include <uart.h>
#include <clocks.h>
#include <stm32l0xx.h>

int main(void)
{
    /* Call your initialisations here */
    clocks_init_pmi();
    uart_init_nucusb(115200);

    while(1)
    {
      // get the data from the sensor adxl345

      // print it out to UART 

    }
}