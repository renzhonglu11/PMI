#include <adxl345.h>

int32_t adxl345_init(void)
{
  spi_init_adxl345();

  while(_test_adxl345())
  {
    uart_tx_str("ADXL345 init fails!\n");
    systick_delay_ms(1000);  // wait for 1 sec
  }
  
  /*
    send: (we dont care the read data here!!) 
      1. send the address of the register to be read
      2. send the data to be written
  */


  // sensor configuration
  uint8_t power_sleep_buf[] = {POWER_CTL, POWER_CTL_Sleep};
  spi_txrx(power_sleep_buf, elements_of(power_sleep_buf));

  // Configure sensor
  uint8_t config_buf[] = {
      FIFO_CTL, FIFO_CTL_MODE_Stream // Enable stream mode in FIFO
                                     // SPI in 4-wire mode by default
  };
  spi_txrx(config_buf, elements_of(config_buf));

  // Power on sensor and start measuring
  // Enable measurement mode last
  uint8_t power_measure_buf[] = {POWER_CTL, POWER_CTL_Measure};
  spi_txrx(power_measure_buf, elements_of(power_measure_buf));

  return RC_SUCC;
}

int32_t adxl345_read_xyz(float *x, float *y, float *z)
{
  // need to take a deep look at the datasheet of adxl345

  // 1. Read data from sensor (x, y, z)

  // 2. Convert data to 16-bit signed integers

  // 3. Return data

  return RC_SUCC;
}


int32_t _test_adxl345(void)
{

  uint8_t buf[] = {(DEVID | WRITE_BIT) & ~MB_BIT};   
  char hexStr[10];

  spi_txrx(buf, 1);
  sprintf(hexStr, "0x%X", buf[0]);

  uart_tx_str("next:");
  uart_tx_char('\n');
  uart_tx_str(hexStr);
  uart_tx_char('\n');
  uart_tx_str("-----------:");
  uart_tx_char('\n');

  return RC_SUCC;
}

/// @brief get the datas from sensor
/// @param x 
/// @param y 
/// @param z 
/// @return RC_SUCC
int32_t adxl345_get_data(float *x, float *y, float *z)
{
  // 1. read x, y, z from the corresponding resgister (multiple bytes will be read)
  

  // 2. convert the data to 16-bit signed integers



  return RC_SUCC;
  
}
