/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File: utils.h
 * Author: Javinator9889
 * Comments: Standard utils for using them along the project
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UTILS_H
#define	UTILS_H

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <libpic30.h>
#include "types.h"
#include "defs.h"

#define arrsize(array) (sizeof (array) / sizeof *(array))
#define foreach(idxtype, item, array) \
    idxtype* item; \
    size_t size = arrsize(array); \
    for (item = array; item < (array + size); ++item)
#define clockCyclesPerMicrosecond() ( FCY / 1000000UL )
#define constraint(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

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
inline long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline double64_t roundp(double64_t value)
{
    return floorl(value + .5F);
}

inline double64_t preciseMap(
    double64_t value,
    double64_t in_min,
    double64_t in_max,
    double64_t out_min,
    double64_t out_max)
{
    double64_t slope = 1.0F * (out_max - out_min) / (in_max - in_min);
    return out_min + roundp(slope * (value - in_min));
}

inline double64_t mapf(double64_t x, double64_t in_min, double64_t in_max, double64_t out_min, double64_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline uint8_t isnan(float value)
{
    return ((*((unsigned*) &value + 1) & 0x7F80U) == 0x7F80U);
}

inline void delay_ms(uint64_t ms) {
    __delay_ms(ms);
}

inline void delay_us(uint64_t us) {
    __delay_us(us);
}

#endif	/* UTILS_H */

