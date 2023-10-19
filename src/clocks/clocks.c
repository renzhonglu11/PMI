#include <clocks.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>

#ifndef DRV_CLOCKS_AHB_CLOCK
    #define DRV_CLOCKS_AHB_CLOCK    DRV_CLOCKS_AHB_CLOCK_HSI16_16MHZ
#endif


#if (DRV_CLOCKS_AHB_CLOCK == DRV_CLOCKS_AHB_CLOCK_PLLHSI16_32MHZ)
/**
 * @brief Use PLL at 32 MHz for system clock, based on HSI16 clock
 */
static int32_t clks_init(void)
{
    int rc;

    /* Set voltage scaling range to 1.8 V */
    MODIFY_REG(PWR->CR, PWR_CR_VOS, (PWR_CR_VOS_0));

    /* Set system clock prescaler to 0 */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);

    /* Enable HSI16 clock */
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY))
    {
        /* For robust implementation, add time-out management here */
    }

    /* Select HSI as system clock */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSI);
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    {
        /* For robust implementation, add time-out management here */
    }

    /* Disable the PLL */
    RCC->CR &= (uint32_t)(~RCC_CR_PLLON);

    /* Wait until PLLRDY is cleared */
    while((RCC->CR & RCC_CR_PLLRDY) != 0)
    {
        /* For robust implementation, add time-out management here */
    }

    /* Access control register
     * FLASH_ACR_LATENCY: One wait state is used to read a word in the NVM.
     *      Necessary for main clock frequencies higher than 24 MHz
     * FLASH_ACR_PRFTEN: prefetch is enabled. The memory interface stores the
     *      last address fetched and tries to read the next one when no other
     *      read or write operation is ongoing.
     * FLASH_ACR_PRE_READ: The pre-read is enabled. The memory interface stores
     *      the last address read as data and tries to read the next one when
     *      no other read or write or prefetch operation is ongoing.
     */
    FLASH->ACR |= (FLASH_ACR_LATENCY | FLASH_ACR_PRFTEN | FLASH_ACR_PRE_READ);

    /* Set the PLL multiplier to 4 and divider by 2 */
    MODIFY_REG(RCC->CFGR, (RCC_CFGR_PLLMUL | RCC_CFGR_PLLDIV ),
               (RCC_CFGR_PLLMUL4 | RCC_CFGR_PLLDIV2));

    /* Enable the PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait until PLLRDY is set */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
        /* For robust implementation, add time-out management here */
    }

    /* Select PLL as system clock */
    RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_PLL);

    /* Wait until the PLL is switched on */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
    {
        /* For robust implementation, add time-out management here */
    }

    /* Enable and start SysTick timer
     * SysTick timer is clocked by the system clock. So the SysTick timer
     * reload value is:
     *  reload = 32 MHz / 1000 = 32000
     * Note that in the clocktree in the STM32 reference manual SysTick is
     * clocked by AHB/8, but SysTick_Config sets SysTick to run with the
     * processor clock (see CortexM0+ programming manual, SysTick CLKSOURCE).
     */
    rc = SysTick_Config(32000);
    if (rc)
    {
        return RC_ERR;
    }

    /* Set SysTick to the highest interrupt priority */
    NVIC_SetPriority(SysTick_IRQn, 0);

    return RC_SUCC;
}
#endif


#if (DRV_CLOCKS_AHB_CLOCK == DRV_CLOCKS_AHB_CLOCK_HSI16_16MHZ)
/**
 * @brief Initialise HSI16 clock as 16 MHz main source clock
 */
static int32_t clks_init(void)
{
    int rc;

    /* Set voltage scaling range to 1.8 V */
    MODIFY_REG(PWR->CR, PWR_CR_VOS, (PWR_CR_VOS_0));

    /* Set system clock prescaler to 0 */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);

    /* Enable HSI16 clock */
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY))
    {
        /* For robust implementation, add time-out management here */
    }

    /* Select HSI as system clock */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSI);
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    {
        /* For robust implementation, add time-out management here */
    }

    /* Disable the PLL */
    RCC->CR &= (uint32_t)(~RCC_CR_PLLON);

    /* Wait until PLLRDY is cleared */
    while((RCC->CR & RCC_CR_PLLRDY) != 0)
    {
        /* For robust implementation, add time-out management here */
    }

    /* Access control register
     * FLASH_ACR_PRFTEN: prefetch is enabled. The memory interface stores the
     *      last address fetched and tries to read the next one when no other
     *      read or write operation is ongoing.
     * FLASH_ACR_PRE_READ: The pre-read is enabled. The memory interface stores
     *      the last address read as data and tries to read the next one when
     *      no other read or write or prefetch operation is ongoing.
     */
    FLASH->ACR |= (FLASH_ACR_PRFTEN | FLASH_ACR_PRE_READ);

    /* Enable and start SysTick timer
     * SysTick timer is clocked by the system clock. So the SysTick timer
     * reload value is:
     *  reload = 16 MHz / 1000 = 16000
     * Note that in the clocktree in the STM32 reference manual SysTick is
     * clocked by AHB/8, but SysTick_Config sets SysTick to run with the
     * processor clock (see CortexM0+ programming manual, SysTick CLKSOURCE).
     */
    rc = SysTick_Config(16000);
    if (rc)
    {
        return RC_ERR;
    }

    /* Set SysTick to the highest interrupt priority */
    NVIC_SetPriority(SysTick_IRQn, 0);

    return RC_SUCC;
}
#endif


/**
 * @brief Initialises all clocks
*/
int32_t clocks_init_pmi(void)
{
    int rc;

    /* Initialise clocks */
    rc = clks_init();
    if (rc)
    {
        return rc;
    }

    SystemCoreClockUpdate();

    return 0;
}
