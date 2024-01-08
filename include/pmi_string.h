#ifndef PMI_STRING_H
#define PMI_STRING_H

#include <pmi_stddefs.h>

/**
 * @brief Convert floats to strings
 * @param[out] *str Char array to store string
 * @param[in] str_len Maximum string length
 * @param[in] value Float value
 * @param[in] resolution Number of decimals
 * @return See @ref PMI_RC_E
 */
uint32_t float2str(char *str, uint16_t str_len, float value,
    uint32_t resolution);

#endif /* PMI_STRING_H */
