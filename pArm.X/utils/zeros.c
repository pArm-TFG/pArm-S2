/* 
 * File: zeros.c
 * Author: Javinator9889
 * Comments: Utils for counting both trailing and leading zeros.
 * Revision history: v1.0
 */
#include <stdint.h>
#include "zeros.h"

const unsigned char clz_8b[256] = {
    8,
    7,
    REPEAT_2x(6),
    REPEAT_4x(5),
    REPEAT_8x(4),
    REPEAT_16x(3),
    REPEAT_32x(2),
    REPEAT_64x(1),
    REPEAT_128x(0)
};

int_fast64_t clz(int_fast64_t x) {
    int_fast64_t base, ms_oct;

    if (x & 0xFFFFFFFF00000000U)
        if (x & 0xFFFF000000000000U)
            if (x & 0xFF00000000000000U)
                base = 0, ms_oct = x >> 56;
            else
                base = 8, ms_oct = x >> 48;
        else
            if (x & 0x0000FF0000000000U)
            base = 16, ms_oct = x >> 40;
        else
            base = 24, ms_oct = x >> 32;
    else
        if (x & 0x00000000FFFF0000U)
        if (x & 0x00000000FF000000U)
            base = 32, ms_oct = x >> 24;
        else
            base = 40, ms_oct = x >> 16;
    else
        if (x & 0x000000000000FF00U)
        base = 48, ms_oct = x >> 8;
    else
        base = 56, ms_oct = x >> 0;

    return base + clz_8b[ms_oct];
}

int_fast64_t ctz(int_fast64_t x) {
    x &= -x;
    return 64LL - (clz(x) + 1);
}
