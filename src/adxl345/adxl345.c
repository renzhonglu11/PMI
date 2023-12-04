#include <adxl345.h>


int32_t adxl345_init(void)
{
  // 1. sensor should be in sleep mode first

  // 2. Configure sensor

  // 3. Set sensor to measurement mode




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