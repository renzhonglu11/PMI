#ifndef MCP23017_H
#define MCP23017_H

#include <stdint.h>
#include <pmi_stddefs.h>

#define MCP_IN_ADDR 0x20
#define MCP_GPIOA_ADDR 0x12
#define MCP_GPIOB_ADDR 0x13
#define MCP_IODIRB (uint8_t)0x01
#define MCP_IODIRA (uint8_t)0x00

// the led of 0x04 is not used
typedef enum
{
    D1_D9= 0x01,
    D2_D7 = 0x02,
    D3_D8 = 0x04,
    D4_D6 = 0x08,
    D5_D10 = 0x10,
    
}LIGHT;

// TODO: define FSM_table

// TODO: define state machine



int32_t config_gpio(enum PMI_BOOL_E value, char gpio);
int32_t config_button(void);
int32_t initial_interrupt();
// int32_t init_gpio(enum PMI_BOOL_E use_oupt, char gpio);
int32_t init_mcp23017();
int32_t write_mcp23017(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t buf_size);
int32_t turn_on_led(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t buf_size);
int32_t led_loop();


#endif /* MCP23017_H */
