#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

/* Options to set the AHB clock speed */
#define DRV_CLOCKS_AHB_CLOCK_HSI16_16MHZ        1
#define DRV_CLOCKS_AHB_CLOCK_PLLHSI16_32MHZ     2

/* Predefine HSI16 as AHB clock */
#define DRV_CLOCKS_AHB_CLOCK    DRV_CLOCKS_AHB_CLOCK_HSI16_16MHZ

/**
 * @brief Initialises all clocks
*/
int32_t clocks_init_pmi(void);

#endif /* CLOCK_H */
