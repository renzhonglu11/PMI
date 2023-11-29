#include <systick.h>
#include <stm32l0xx.h>
#include <uart.h>

/**
  * @brief This function handles the 'non maskable interrupt'.
  */
void NMI_Handler(void)
{
}


/**
  * @brief This function handles the 'hard fault interrupt'.
  * @note If you get here, something went really wrong!
  */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}


/**
  * @brief This function handles the 'system service call via SWI instruction'.
  */
void SVC_Handler(void)
{
}


/**
  * @brief This function handles the 'pendable request for system service'.
  */
void PendSV_Handler(void)
{
}


/**
  * @brief This function handles the 'system tick timer'.
  */
void SysTick_Handler(void)
{
    systick_inc();
}