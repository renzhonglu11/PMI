#include <uart.h>
#include <clocks.h>
#include <mpi.h>
#include <adxl345.h>
#include <stdio.h>
#include <pmi_string.h>
#include <systick.h>
#include <my_utils.h>
#include <i2c.h>




int main(void)
{
  /* Call your initialisations here */
  clocks_init_pmi();
  // uart_init_nucusb(115200);
  uart_init_nucusb(115200);

  ili9341_init(ILI9341_ORIENTATION_90); // initilize the LCD screen

  adxl345_init();
  qmc5883l_init();


  qmc5883l_write_reg(ConfigReg1,0x12);  // set the mode to continuous mode



  while (1)
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

    // output_data();
    // systick_delay_ms(100);
 

    // _qmc5883l_internal_test();  // testing I2C
    uint8_t data_buf[1];
    char hexStr[10];

    
    qmc5883l_read_reg(ConfigReg1,data_buf,elements_of(data_buf));
    sprintf(hexStr, "0x%X", data_buf[0]);
    uart_tx_str("configReg1: ");
    uart_tx_str(hexStr);
    uart_tx_char('\n');

    systick_delay_ms(1000);

  }
}