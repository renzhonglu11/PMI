#include <pmi_string.h>
#include <pmi_stddefs.h>
#include <stdio.h>


/**
 * @brief Convert floats to strings
 * @param[out] *str Char array to store string
 * @param[in] str_len Maximum string length
 * @param[in] value Float value
 * @param[in] resolution Number of decimals
 * @return See @ref PMI_RC_E
 */
uint32_t float2str(char *str, uint16_t str_len, float value,
    uint32_t resolution)
{
    int32_t rc;
    int32_t integer;
    float dec = value;

    /* Check Null pointer and string length */
    if (!str || !str_len)
    {
        return RC_PARAM_INVALID;
    }

    /* Get integer */
    integer = (int32_t) value;

    /* Get decimals */
    dec -= (float) integer;
    if (value < 0)
    {
        /* snprintf will not plot the '-' sign if the integer part is 0,
         * hence add it here */
        if (integer == 0)
        {
            *str = '-';
            str++;
        }

        dec *= -1;
    }

    /* Clear string */
    str[0] = '\0';

    switch (resolution) {
        case 0:
            rc = snprintf(str, str_len, "%ld", integer);
            break;
        case 1:
            rc = snprintf(str, str_len, "%ld.%01lu", integer,
                (uint32_t) (dec * 10));
            break;

        case 2:
            rc = snprintf(str, str_len, "%ld.%02lu", integer,
                (uint32_t) (dec * 100));
            break;

        case 3:
            rc = snprintf(str, str_len, "%ld.%03lu", integer,
                (uint32_t) (dec * 1000));
            break;

        case 4:
            rc = snprintf(str, str_len, "%ld.%04lu", integer,
                (uint32_t) (dec * 10000));
            break;

        case 5:
            rc = snprintf(str, str_len, "%ld.%05lu", integer,
                (uint32_t) (dec * 100000));
            break;

        case 6:
            rc = snprintf(str, str_len, "%ld.%06lu", integer,
                (uint32_t) (dec * 1000000));
            break;

        default:
        case 7:
            rc = snprintf(str, str_len, "%ld.%07lu", integer,
                (uint32_t) (dec * 10000000));
            break;
    }

    /* See return value of snprintf */
    if (rc < 0)
    {
        return RC_BUFF_OVERFLOW;
    }

    return RC_SUCC;
}
