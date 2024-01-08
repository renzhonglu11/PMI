#include <systick.h>

/* Ticks since startup. One tick should be equal to 1 millisecond */
volatile uint64_t tick;

/**
 * @brief Increments ticks by one
 */
void systick_inc(void)
{
    ++tick;
}

/**
 * @brief Gets current value of ticks
 * @return Ticks since startup
 */
uint64_t systick_get(void)
{
    return tick;
}

/**
 * @brief Delay for a period of time
 * @param msec Delay in milliseconds
 */
void systick_delay_ms(uint32_t msec)
{
    uint64_t t = tick + msec;

    while(tick < t)
    {
    }
}
