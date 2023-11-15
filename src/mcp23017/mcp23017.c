#include <stm32l0xx.h>
#include <i2c.h>
#include <mcp23017.h>
#include <pmi_stddefs.h>
#include <ctype.h>
#include <utils.h>
#include <systick.h>

int8_t running = 1;


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
  uint8_t gpio = 0x00;

  if(EXTI->PR & EXTI_PR_PIF2) // pending bit of EXTI line 2 is set
    {
        systick_delay_ms(40);

        toggle_test();
        
        if(!running)
        {
          gpio = 0x04;
        }

        gpio ^= 0xff;
        
        uint8_t config_iodir3[] = {gpio};
        turn_on_led(MCP_IN_ADDR, MCP_GPIOA_ADDR, config_iodir3, ARRAY_SIZE(config_iodir3));
        EXTI->PR = EXTI_PR_PIF2; // write one to corresponding pending bit to clear pending bit
        
    }
}

void EXTI0_1_IRQHandler(void)
{
  // PB1 for SW2
  // TODO: implment interrupt handler for SW2
  
}

void _turn_off_leds()
{
  
  uint8_t gpio = 0x00;  // 0000 0100
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

  LIGHT lights[] = {D3_D8, D2_D7, D1_D9,D1_D9, D4_D6, D5_D10, D2_D7, D4_D6, D5_D10};
  int8_t len_lights =  ARRAY_SIZE(lights); 
  
  int cur_position = counter % len_lights; 

  int8_t cur_led = lights[cur_position];
  
  int8_t cur_gpio = 0;

  if(cur_position >=3 && cur_position <7)
  {
    cur_gpio = MCP_GPIOB_ADDR;
   

  }else
  {
    cur_gpio = MCP_GPIOA_ADDR;
  
  }
  
  
  cur_led ^= 0xff;
  uint8_t config_iodir4[] = {cur_led};


  turn_on_led(MCP_IN_ADDR, cur_gpio, config_iodir4, ARRAY_SIZE(config_iodir4)); 
  systick_delay_ms(50);
  _turn_off_leds();
  counter++;
  systick_delay_ms(50);


}



int32_t initial_interrupt()
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // enables the clock for the System Configuration 

    // EXTI0, EXTI1 and  EXTI2, EXTI3 
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PB; // PB2 for SW1
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PB; // PB1 for SW2
    

    EXTI->IMR |= EXTI_IMR_IM1;  //  enables the interrupt request from EXTI line 1
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