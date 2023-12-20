#ifndef ADXL345_H
#define ADXL345_H

#include <pmi_stddefs.h>
#include <stm32l0xx.h>
#include <stdio.h>
#include <uart.h>
#include <systick.h>
#include <mpi.h>

#define WRITE_BIT 0x80 // 1000 0000
#define MB_BIT 0x40    // 0100 0000

#define DEVID 0x00 // Device ID

// Power
#define POWER_CTL 0x2DU         // Power-saving features control
#define POWER_CTL_Wakeup 0x03U  // Wakeup (0b00000011)   1 Hz
#define POWER_CTL_Sleep 0x04U   // Sleep (0b00000100)
#define POWER_CTL_Measure 0x08U // Measure (0b00001000)

// Data
#define DATA_FORMAT 0x31  // data format
#define DATAX0 0x32 // X-Axis Data 0
#define DATAX1 0x33 // X-Axis Data 1
#define DATAY0 0x34 // Y-Axis Data 0
#define DATAY1 0x35 // Y-Axis Data 1
#define DATAZ0 0x36 // Z-Axis Data 0
#define DATAZ1 0x37 // Z-Axis Data 1
#define OFSX   0x1E
#define OFSY   0x1F
#define OFSZ   0x20


// FIFO
#define FIFO_CTL 0x38             // FIFO control
#define FIFO_CTL_MODE_Stream 0x80 // Stream mode (0b10000000)



// GPIO definition
#define ADXLCS_GPIO_Port GPIOA

typedef struct AccelerometerData
{
    int16_t x;
    int16_t y;
    int16_t z;
}accelerometerData;

// functions

int32_t adxl345_init(void);
int32_t _test_adxl345(void);
int32_t adxl345_acc_data(float *x, float *y, float *z);

#endif /* ADXL345_H */