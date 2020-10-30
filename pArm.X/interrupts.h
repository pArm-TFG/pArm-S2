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
 * File: interrupts.h
 * Author: Javinator9889
 * Comments: base interrupts handler with different handlers
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#include <xc.h>
#include <stdint.h>
#include "utils/types.h"

// UART RX initializer
void U1RX_Init(volatile order_t* order);
#ifdef LIMIT_SWITCH_ENABLED
// Limit switch interrupts initializer
void CN_Init(volatile uint_fast8_t *switch_map);
#endif

// Define Timer interrupts
/**
 * UART1 Transmission interrupt
 */
void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void);

/**
 * UART1 Reception interrupt
 */
void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void);

/**
 * UART1 Error interrupt
 */
void __attribute__((__interrupt__, no_auto_psv)) _U1ErrInterrupt(void);

/**
 * Peripheral change interrupt
 */
void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void);

#endif	/* INTERRUPTS_H */

