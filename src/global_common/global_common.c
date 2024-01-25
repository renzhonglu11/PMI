#include "global_common.h"



uint8_t power_of_2(uint8_t exponent)
{
  uint8_t r = 1;

  for (uint8_t i = 1; i < exponent; i++)
  {
    r *= 2;
  }

  return r;
}