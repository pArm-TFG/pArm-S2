/*
 * 2020 | pArm-S2 by Javinator9889
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/.
 *
 * Created by Javinator9889 on 2020 - pArm-S1.
 */

/* 
 * File: utils.c
 * Author: Javinator9889
 * Comments: Standard utils for using them along the project
 * Revision history: 1.0
 */
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <libpic30.h>
#include <stdbool.h>
#include "utils.h"
#include "types.h"
#include "defs.h"

/**
 * Maps a value in between the output range by the given input range
 * @param x the value to map.
 * @param in_min the minimum acceptable value.
 * @param in_max the maximum acceptable value.
 * @param out_min the minimum output value.
 * @param out_max the maximum output value.
 * @return the mapped 'x' value in between [out_min, out_max]
 * @see https://www.arduino.cc/reference/en/language/functions/math/map/
 */
inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline double64_t roundp(double64_t value) {
    return floorl(value + .5F);
}

inline double64_t preciseMap(
        double64_t value,
        double64_t in_min,
        double64_t in_max,
        double64_t out_min,
        double64_t out_max) {
    double64_t slope = 1.0F * (out_max - out_min) / (in_max - in_min);
    return out_min + roundp(slope * (value - in_min));
}

inline double64_t mapf(double64_t x, double64_t in_min, double64_t in_max, double64_t out_min, double64_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//#ifndef __isnan
bool __isnan(double64_t x) {
    int_fast64_t hx;
    EXTRACT_WORDS64(hx, x);
    hx &= UINT64_C(0x7FFFFFFFFFFFFFFF);
    hx = UINT64_C(0x7ff0000000000000) - hx;
    return (bool) (((uint64_t) hx) >> 63);
}
//#define isnan __isnan
//#endif

inline void delay_ms(time_t ms) {
    return delay_us(ms * 1000ULL);
}

inline void delay_us(time_t us) {
    __delay32(us * (FCY / 1000000ULL));
}

void cstrncpy(char source[], char dest[], uint16_t size) {
    for (int i = (size - 1); i >= 0; i--) dest[i] = source[i];
}