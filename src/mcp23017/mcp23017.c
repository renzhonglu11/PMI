#include <stm32l0xx.h>
#include <i2c.h>
#include <mcp23017.h>
#include <pmi_stddefs.h>
#include <ctype.h>
#include <utils.h>
#include <systick.h>
#include <bmp_go_49x56.h>
#include <bmp_stop_51x56.h>
#include <ili9341.h>


int8_t running = 1;
int8_t on_off_flag = 1;
int8_t direction_flag = 1;
uint8_t start_fsm = 0;
int8_t interrupt_lock = 0; // easy lock. No multi threads.

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

  // nop_30();
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

int32_t config_button(void)
{
  //  clock is already enable in gpio_init()

  GPIOB->MODER &= ~(GPIO_MODER_MODE2); // PB2 for SW1
  GPIOB->MODER &= ~(GPIO_MODER_MODE1); // PB1 for SW2

  // return 0 if GPIOB->MODER not set correctly
  if ((GPIOB->MODER & GPIO_MODER_MODE2) != 0)
  {
    return RC_ERR;
  }

  return RC_SUCC;
}

void toggle_test()
{
  running ^= 1;
}

void EXTI2_3_IRQHandler()
{
  // PB2 for SW1
  // uint8_t gpio = 0x00;

  if (EXTI->PR & EXTI_PR_PIF2 ) // pending bit of EXTI line 2 is set
  {

    // on_off_flag ^= 1;  // exercise 1, turn on leds
    if(!interrupt_lock)
    {
    start_fsm ^= 1;      //  start fsm

    }

    EXTI->PR = EXTI_PR_PIF2; // write one to corresponding pending bit to clear pending bit
  }
}

void EXTI0_1_IRQHandler(void)
{
  // PB1 for SW2
  // TODO: implment interrupt handler for SW2
  if(EXTI->PR & EXTI_PR_PIF1)
  {
    // direction_flag ^= 1;  // exercise 1, change the direction of led loop
    if(!interrupt_lock)
    {
    start_fsm ^= 1;      //  start fsm

    }

    EXTI->PR = EXTI_PR_PIF1;
  }

}

void _turn_off_leds()
{
  nop_30();
  uint8_t gpio = 0x00; // 0000 0100
  uint8_t gpio2 = 0x00;
  gpio ^= 0xff;
  gpio2 ^= 0xff;
  uint8_t config_iodir2[] = {gpio};
  uint8_t config_iodir3[] = {gpio2};
  int32_t b = turn_on_led(MCP_IN_ADDR, MCP_GPIOA_ADDR, config_iodir3, ARRAY_SIZE(config_iodir3));
  int32_t a = turn_on_led(MCP_IN_ADDR, MCP_GPIOB_ADDR, config_iodir2, ARRAY_SIZE(config_iodir2));
}

int32_t counter = 0;

int32_t led_loop()
{
  // using for loop to turn on and off each led
  // TODO: use interrupt to turn on and off each led
  // TODO: use interrupt to reverse the direction of the led loop
  
  if (on_off_flag)
  {
    LIGHT lights[] = {D3_D8, D2_D7, D1_D6, D1_D6, D4_D9, D5_D10, D2_D7, D4_D9, D5_D10};
    int8_t len_lights = ARRAY_SIZE(lights);

    int cur_position = counter % len_lights;


    int8_t cur_led = lights[cur_position];

    int8_t cur_gpio = 0;

    if (cur_position >= 3 && cur_position < 7)
    {
      cur_gpio = MCP_GPIOB_ADDR;
    }
    else
    {
      cur_gpio = MCP_GPIOA_ADDR;
    }

    cur_led ^= 0xff;
    uint8_t config_iodir4[] = {cur_led};

    turn_on_led(MCP_IN_ADDR, cur_gpio, config_iodir4, ARRAY_SIZE(config_iodir4));
    
    systick_delay_ms(50);

    _turn_off_leds();

    if(direction_flag)
    {
      counter++;
    }else{
      counter--;
    }

    // if(counter>len_lights-1)
    // {
    //   counter = 0;
    // }

    // systick_delay_ms(50);
    // nop_30();

    return RC_SUCC;
  }
  
  return 0;
}

int32_t initial_interrupt()
{
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // enables the clock for the System Configuration

  // EXTI0, EXTI1 and  EXTI2, EXTI3
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PB; // PB2 for SW1
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PB; // PB1 for SW2

  EXTI->IMR |= EXTI_IMR_IM1; //  enables the interrupt request from EXTI line 1
  EXTI->IMR |= EXTI_IMR_IM2; //  enables the interrupt request from EXTI line 2

  EXTI->FTSR |= EXTI_FTSR_TR1; //  selects the falling trigger for the EXTI line 1
  EXTI->FTSR |= EXTI_FTSR_TR2; // selects the falling trigger for the EXTI line 2

  NVIC_ClearPendingIRQ(EXTI0_1_IRQn); // PB1 in EXTI1
  NVIC_ClearPendingIRQ(EXTI2_3_IRQn); // PB2 in EXTI2
  NVIC_SetPriority(EXTI0_1_IRQn, 1);
  NVIC_SetPriority(EXTI2_3_IRQn, 2);
  NVIC_EnableIRQ(EXTI0_1_IRQn);
  NVIC_EnableIRQ(EXTI2_3_IRQn);

  return RC_SUCC;
}


uint32_t FSM_EventHandler(FSM_t* fsm_t,int8_t event)
{
  // TODO: implement state machine handler
  
  // control led according to the given state machine
  uint8_t led1 = fsm_t->fsm_table[event].HS;
  uint8_t led2 = fsm_t->fsm_table[event].NS;


  led1 ^= 0xff;
  led2 ^= 0xff;
  uint8_t config_iodir1[] = {led1};
  uint8_t config_iodir2[] = {led2};


  turn_on_led(MCP_IN_ADDR, MCP_GPIOA_ADDR, config_iodir1, ARRAY_SIZE(config_iodir1));
  turn_on_led(MCP_IN_ADDR, MCP_GPIOB_ADDR, config_iodir2, ARRAY_SIZE(config_iodir2));


  // draw pciture to led
  int8_t x0 = 20;
  int8_t y0 = 20;
  int8_t width = 49;
  int8_t height = 56;
  int8_t x1 = 20;
  int8_t y1 = (20+56+20);
  if(fsm_t->fsm_table[event].HS == g)
  { 
    ili9341_draw_bmp_h(x1, y1, width, height, (uint8_t*)stop_51x56,  ILI9341_COLOR_BLACK,  ILI9341_COLOR_BLACK);
    ili9341_draw_bmp_h(x0, y0, width, height, (uint8_t*)go_49x56,  ILI9341_COLOR_GREEN,  ILI9341_COLOR_BLACK);

  }else if (fsm_t->fsm_table[event].NS != O || fsm_t->fsm_table[event].NS != n || fsm_t->fsm_table[event].NS != g)
  {
    ili9341_draw_bmp_h(x0, y0, width, height, (uint8_t*)go_49x56,  ILI9341_COLOR_BLACK,  ILI9341_COLOR_BLACK);
    ili9341_draw_bmp_h(x1, y1, width, height, (uint8_t*)stop_51x56, ILI9341_COLOR_RED,  ILI9341_COLOR_BLACK);
    
  }
  

  return RC_SUCC;
}



const ampel_state_t state_table[9] = {

    // main street side street   condition ?
    // |         |       |   time to wait
    // |         |       |   |
    // |         |       |   |   next state     Name
    //----------------------------------------------------------------------
    {G, g, 1, 3, MS_Y},  // MS_G
    {Y, g, 0, 1, MS_R1}, // MS_Y
    {R, R, 0, 2, SS_D},  // MS_R1
    {R, D, 0, 1, SS_G},  // SS_D
    {g, G, 0, 10, SS_Y}, // SS_G
    {R, Y, 0, 1, MS_R2}, // SS_Y
    {R, R, 0, 1, MS_D},  // MS_R2
    {D, R, 0, 1, TL_G},  // MS_D
    {G, R, 0, 1, MS_G},  // MS_G
};




FSM_t fsm = {
  .cur_state = MS_G,
  .fsm_table = state_table,
};

  // // initiailize fsm
uint32_t state_machine()
{ 
  // current implementation. Not sure there is better way to refactor the codes ！！！
  // 1. get the current state from state machine
  // 2.1. when get back to initial state, hold
  // 2.2. otherwise go to next state and hold for certain time according to the state machine


  // TODO： implement interrupt to start the state machine
  ampel_state_t cur_row = fsm.fsm_table[fsm.cur_state];
  
  FSM_EventHandler(&fsm , fsm.cur_state);


  // condition == 1 means back to initial state and hold
  if(!start_fsm && cur_row.condition)
  {
    
    return RC_SUCC;
  }

  if(cur_row.condition){
    start_fsm ^=1;         // set back to initial state
    interrupt_lock ^=1;    // set lock
  }


  uint8_t next_state = fsm.fsm_table[fsm.cur_state].next_state;

  systick_delay_ms(cur_row.time2wait * 1000);

  fsm.cur_state = next_state;    // go to the next state

  if(fsm.fsm_table[fsm.cur_state].condition==1)
  {
    
    interrupt_lock ^=1; // reset lock
  }
  return RC_SUCC;
}