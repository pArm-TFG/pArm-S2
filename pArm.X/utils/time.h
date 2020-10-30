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
 * File: time.h
 * Author: Javinator9889
 * Comments: the time management library
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIME_H
#define	TIME_H

#include "types.h"

/**
 * Initializes the time counting machine.
 */
void TIME_init(void);

/**
 * Updates the milliseconds timestamp using the microseconds one.
 */
void TIME_updateMs(void);

/**
 * Obtains current time in milliseconds.
 * 
 * @return time_t - current time in milliseconds.
 */
time_t TIME_now(void);

/**
 * Obtains the current time in microseconds.
 * 
 * @return time_t - current time in microseconds.
 */
time_t TIME_now_us(void);

/**
 * Sets the current time with the given value, in microseconds.
 * 
 * @param value_us - the new timestamp.
 */
void TIME_set_time(time_t value_us);

/**
 * TMR1 interrupt handler - counts-up microseconds.
 */
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void);

/**
 * TMR2 interrupt handler - counts-up milliseconds.
 */
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void);

#endif	/* TIME_H */

