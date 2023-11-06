#include <stm32l0xx.h>
#include <i2c.h>


int32_t init_mcp23017()
{
    return i2c_hw_init();
}

