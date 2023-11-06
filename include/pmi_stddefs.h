/**
 * @brief Collection of useful macros and definitions
 */

#ifndef PMI_STDDEFS_H
#define PMI_STDDEFS_H

#include <stdint.h>
#include <stddef.h>


#if 0
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})
#else
#define container_of(ptr, type, member)     \
        ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
#endif


/** Possible return values for functions */
enum PMI_RC_E
{
    RC_SUCC,

    RC_ERR,
    RC_PARAM_INVALID,
    RC_TIMEOUT,
    RC_CONF_INVALID,

    RC_N_AVAILABLE,
    RC_BUFF_OVERFLOW
};

/** Just bool */
enum PMI_BOOL_E
{
    FALSE,
    TRUE
};

/**
 * @brief Converts an index to bitmask
 */
#define bit2idx(_x)          (1UL << (_x))

/**
 * @brief Returns number of elements in an array
 */
#define elements_of(_x)      (sizeof(_x) / sizeof(_x[0]))

/* No operation. Delays code execution for given clock cycles */
#define nop_1() asm("nop")
#define nop_2() nop_1(); nop_1()
#define nop_3() nop_1(); nop_2()
#define nop_5() nop_3(); nop_2()
#define nop_10() nop_5(); nop_5()
#define nop_20() nop_10(); nop_10()
#define nop_30() nop_10(); nop_20()
#define nop_50() nop_20(); nop_30()

#endif /* PMI_STDDEFS_H */
