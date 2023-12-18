#ifndef QMC5884L_H
#define QMC5884L_H

#include <stdint.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>
#include <adxl345.h>
#include <i2c.h>

#define TEST_QMC5884L 1


/*configurations*/
#define ConfigReg1 0x09

#define StandbyConversion 0x00
#define ContinuousConversion 0x01


/*data*/
#define MAG_DATA_REGISTER 0x00
#define MAG_X_LSB 0x00
#define MAG_X_MSB 0x01
#define MAG_Y_LSB 0x02
#define MAG_Y_MSB 0x03
#define MAG_Z_LSB 0x04
#define MAG_Z_MSB 0x05



int32_t qmc5883l_init(void);
void qmc5883l_read_reg(uint8_t reg_address, uint8_t *data, uint8_t numBytes);
void qmc5883l_write_reg(uint8_t reg, uint8_t data);



#if (TEST_QMC5884L == 1)
int32_t _qmc5883l_internal_test();
#endif

#endif /* QMC5884L_H */