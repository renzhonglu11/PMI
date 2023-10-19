#ifndef I2C_H
#define I2C_H

#include <stdint.h>

/* Time in milliseconds to abort communication if no acknowledge is sent */
#define I2C_BUS_TIMEOUT_MS      10

/**
 * @brief Initialises hardware i2c for pmi purposes
 * @return See @ref PMI_RC_E
 */
int32_t i2c_hw_init(void);

/**
 * @brief Transmits an amount of data over i2c
 * @param addr 7 bit device address
 * @param *buf Data buffer
 * @param buf_size Size of buffer
 */
int32_t i2c_hw_tx(uint8_t addr, uint8_t *buf,  uint8_t buf_size);

/**
 * @brief Receives an amount of data over i2c
 * @param addr 7 bit device address
 * @param *buf Data buffer
 * @param buf_size: Size of buffer
 * @return See @ref PMI_RC_E
 */
int32_t i2c_hw_rx(uint8_t addr, uint8_t *buf, uint8_t buf_size);

/**
 * @brief Receives an amount of data over i2c
 * @param addr 7 bit device address
 * @param reg Register address to read from
 * @param *buf Data buffer
 * @param buf_size: Size of buffer
 * @return See @ref PMI_RC_E
 */
int32_t i2c_hw_rx_device(uint8_t addr, uint8_t reg, uint8_t *buf,
    uint8_t buf_size);

/**
 * @brief Transmits an amount of data over i2c
 * @param addr 7 bit device address
 * @param reg Register address to write to
 * @param *buf Data buffer
 * @param buf_size Size of buffer
 */
int32_t i2c_hw_tx_device(uint8_t addr, uint8_t reg, uint8_t *buf,
    uint8_t buf_size);

#endif /* I2C_H */
