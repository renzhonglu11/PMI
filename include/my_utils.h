#ifndef MY_UTILS_H
#define MY_UTILS_H

#include <stdint.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>
#include <pmi_string.h>
#include <adxl345.h>
#include <stdio.h>
#include <string.h>
#include <qmc5883l.h>
#include <ds18b20.h>


// uint32_t output_data(void);
void send_sensor_data_over_UART();


#endif /* MY_UTILS_H */