#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

/**
 * @brief Increments ticks by one
 */
void systick_inc(void);

/**
 * @brief Gets current value of ticks
 * @return Ticks since startup
 */
uint64_t systick_get(void);

/**
 * @brief Delay for a period of time
 * @param msec Delay in milliseconds
 */
void systick_delay_ms(uint32_t msec);

#endif /* SYSTICK_H */
