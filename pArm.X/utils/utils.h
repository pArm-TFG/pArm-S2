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
 * File: utils.h
 * Author: Javinator9889
 * Comments: Standard utils for using them along the project
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UTILS_H
#define	UTILS_H

#include "types.h"
#include "defs.h"

// Gets the size of an array
#define arrsize(array) (sizeof (array) / sizeof *(array))

// Iterates through an array
#define foreach(idxtype, item, array) \
    idxtype* item; \
    size_t size = arrsize(array); \
    for (item = array; item < (array + size); ++item)
#define clockCyclesPerMicrosecond() ( FCY / 1000000UL )
#define constraint(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

/**
 * Maps a given value in between the constraints.
 * 
 * @param x - the given value.
 * @param in_min - minimum possible input value.
 * @param in_max - maximum possible input value.
 * @param out_min - minimum possible output value.
 * @param out_max - maximum possible output value.
 * @return the mapped value.
 */
long map(long x, long in_min, long in_max, long out_min, long out_max);

/**
 * Rounds the given value with better precission as by default.
 * 
 * @param value - the value to be rounded.
 * @return the rounded value.
 */
double64_t roundp(double64_t value);

/**
 * Maps a given value in between the constraints.
 * 
 * @param value - the given value.
 * @param in_min - minimum possible input value.
 * @param in_max - maximum possible input value.
 * @param out_min - minimum possible output value.
 * @param out_max - maximum possible output value.
 * @return the mapped value.
 */
double64_t preciseMap(
    double64_t value,
    double64_t in_min,
    double64_t in_max,
    double64_t out_min,
    double64_t out_max);


/**
 * Maps a given value in between the constraints.
 * 
 * @param x - the given value.
 * @param in_min - minimum possible input value.
 * @param in_max - maximum possible input value.
 * @param out_min - minimum possible output value.
 * @param out_max - maximum possible output value.
 * @return the mapped value.
 */
double64_t mapf(double64_t x, double64_t in_min, double64_t in_max, double64_t out_min, double64_t out_max);

/**
 * Checks if the given value is NaN (Not a Number).
 * 
 * @param x - the input value.
 * @return true if is NaN, else false.
 */
bool __isnan(double64_t x);

/**
 * Delays the specified amount of time.
 * @param ms - the time to delay.
 */
void delay_ms(time_t ms);

/**
 * Delays the specified amount of time.
 * @param us - the time to delay.
 */
void delay_us(time_t us);

#endif	/* UTILS_H */

