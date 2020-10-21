/*
 * Copyright (C) 2020 - present | pArm-S2 by Javinator9889
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
 * File: defs,h
 * Author: Javinator9889
 * Comments: Includes common definitions that may be used in the entire project
 * Revision history: 1.0
 */

#ifndef DEFS_H
#define	DEFS_H
#include <stdint.h>

// CLK definitions
#define FOSC        119808000UL
#define FCY         59904000UL
#define CLK_SPEED   7.3728
#define FCLK_SPEED  73728000UL
#define PRESCALE    ((uint8_t) FOSC / FCLK_SPEED)

// Servo definitions
#define MIN_PULSE_MS    0.75F
#define MAX_PULSE_MS    2.25F

// UART delay
#define DELAY_105uS asm volatile ("REPEAT, #4201"); Nop(); // 105uS delay

// Mathematical constants
#define MATH_PI         3.141592653589793238463F
#define MATH_TRANS      57.29577951308232087679F
#define MATH_TRANS_I    0.017453292519943295769F
#define DEG_151         2.356194490192344928847F
#define DEG_135         2.356194490192344928847F
#define DEG_120         2.094395102393195492308F
#define DEG_60          1.047197551196597746154F
#define DEG_55          0.959931088596881267308F
#define DEG_45          0.785398163397448309616F

// Possible missing functions
#ifndef max
#define	max(a, b)	(((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define	min(a, b)	(((a) < (b)) ? (a) : (b))
#endif

/* A union which permits us to convert between a double and two 32 bit ints.  */
typedef union {
    double value;

    struct {
        uint32_t lsw;
        uint32_t msw;
    } parts;
    uint64_t word;
} ieee_double_shape_type;

/* Get all in one, efficient on 64-bit machines.  */
#ifndef EXTRACT_WORDS64
#define EXTRACT_WORDS64(i,d)                                        \
do {                                                                \
  ieee_double_shape_type gh_u;                                        \
  gh_u.value = (d);                                                \
  (i) = gh_u.word;                                                \
} while (0)
#endif

/* Unsigned.  */
#define UINT8_C(c)        c
#define UINT16_C(c)        c
#define UINT32_C(c)        c ## U
#if __WORDSIZE == 64
#define UINT64_C(c)        c ## UL
#else
#define UINT64_C(c)        c ## ULL
#endif

/* Define ALIASNAME as a weak alias for NAME.
   If weak aliases are not available, this defines a strong alias.  */
#define weak_alias(name, aliasname) _weak_alias (name, aliasname)
#define _weak_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((weak, alias (#name)));

#endif	/* DEFS_H */

