#ifndef BUTTON_H
#define BUTTON_H

#include <stm32l0xx.h>
#include "pmi_stddefs.h"
#include "global_common.h"

uint32_t config_button(void);
uint32_t initialize_btn_interrupt(void);



#endif /* BUTTON_H */