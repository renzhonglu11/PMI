#include <stm32l0xx.h>
#include <i2c.h>
#include <mcp23017.h>
#include <pmi_stddefs.h>
#include <ctype.h>

int32_t init_mcp23017()
{
  return i2c_hw_init();
}

int32_t write_mcp23017(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t buf_size)
{
  return i2c_hw_tx_device(addr, reg, buf, buf_size);
}

/// @brief Turn on led
/// @param addr Address of the device 
/// @param reg  Register address to write to
/// @param buf  Value to write to the register
/// @param buf_size Size of the buffer
/// @return RC_SUCC if successful, RC_ERR otherwise
int32_t turn_on_led(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t buf_size)
{
  write_mcp23017(addr, reg, buf, buf_size);

  return RC_SUCC;
}

/// @brief Configure gpio
/// @param  value Whether to use output or input
/// @param gpio gpio to configure
/// @return Rc_SUCC if successful, RC_ERR otherwise
int32_t config_gpio(enum PMI_BOOL_E value, char gpio)
{
  uint8_t config_iodir[] = {0x00};
  uint8_t iodir = 0;

  if (tolower(gpio) == 'a')
  {
    iodir = MCP_IODIRA;
  }
  else if (tolower(gpio) == 'b')
  {
    iodir = MCP_IODIRB;
  }
  if (value == TRUE)
  {
    config_iodir[0] = 0x00;
  }
  else
  {
    config_iodir[0] = 0xff;
  }

  return write_mcp23017(MCP_IN_ADDR, iodir, config_iodir, sizeof(config_iodir) / sizeof(config_iodir[0]));
}
