#include <stm32l0xx.h>
#include <i2c.h>
#include <mcp23017.h>
#include <pmi_stddefs.h>
#include <ctype.h>
#include <utils.h>
#include <systick.h>




int8_t running = 1;
int8_t on_off_flag = 1;
int8_t direction_flag = 1;

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

  if (EXTI->PR & EXTI_PR_PIF2) // pending bit of EXTI line 2 is set
  {
    // systick_delay_ms(40);

    // toggle_test();

    // if(!running)
    // {
    //   gpio = 0x04;

    // }
    on_off_flag ^= 1;

    // gpio ^= 0xff;

    // uint8_t config_iodir3[] = {gpio};
    // turn_on_led(MCP_IN_ADDR, MCP_GPIOA_ADDR, config_iodir3, ARRAY_SIZE(config_iodir3));
    EXTI->PR = EXTI_PR_PIF2; // write one to corresponding pending bit to clear pending bit
  }
}

void EXTI0_1_IRQHandler(void)
{
  // PB1 for SW2
  // TODO: implment interrupt handler for SW2
  if(EXTI->PR & EXTI_PR_PIF1)
  {
    direction_flag ^= 1;
  }
  EXTI->PR = EXTI_PR_PIF1;

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
    // int cur_position = counter;

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
  uint8_t led1 = O;
  uint8_t led2 = O;


  led1 ^= 0xff;
  led2 ^= 0xff;
  uint8_t config_iodir1[] = {led1};
  uint8_t config_iodir2[] = {led2};


  turn_on_led(MCP_IN_ADDR, MCP_GPIOA_ADDR, config_iodir1, ARRAY_SIZE(config_iodir1));
  turn_on_led(MCP_IN_ADDR, MCP_GPIOB_ADDR, config_iodir2, ARRAY_SIZE(config_iodir2));

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


uint32_t state_machine()
{
  // 1. get the current state from state machine
  // 2.1. when get back to initial state, hold
  // 2.2. otherwise go to next state and hold for certain time according to the state machine

  FSM_t fsm;
  // initiailize fsm
  fsm.cur_state = MS_G;
  fsm.fsm_table = state_table; // array


  


  FSM_EventHandler(&fsm,fsm.cur_state);


  // set_led()

  // delay(t)

  return RC_SUCC;
}