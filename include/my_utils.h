#ifndef MYUTILS_H
#define MYUTILS_H
#include <stdio.h>

void initial_interrupt();
void TIM2_Int_Init();
void print_array(int16_t *data, uint8_t len);
int32_t max_value(int16_t *data, uint8_t len,int16_t *max);
int32_t int2string(char *buf, uint16_t size, int16_t number,uint16_t *len);




#endif /* MYUTILS_H */