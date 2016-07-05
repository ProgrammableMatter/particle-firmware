/**
 * @author Raoul Rubien 2016
 *
 */

#pragma once

#include <stdint.h>

/**
 * non static inline delay loop to be used in non static inline functions
 */
inline void __delay_loop_2(uint16_t __count) {
    __asm__ volatile (
    "1: sbiw %0,1" "\n\t"
            "brne 1b"
    : "=w" (__count)
    : "0" (__count)
    );
}

extern inline void __delay_loop_2(uint16_t __count);

#define DELAY_MS_196 \
    for (int ctr = 0; ctr < 6; ++ctr) { \
        __delay_loop_2(UINT16_MAX);\
    }

#define DELAY_MS_1 \
        __delay_loop_2(2000)

#define DELAY_MS_10 \
        __delay_loop_2(20000)

#define DELAY_US_15 \
        __delay_loop_2(30)

#define DELAY_US_5 \
        __delay_loop_2(10)

#define DELAY_US_150 \
        __delay_loop_2(150); \
        __delay_loop_2(150)

