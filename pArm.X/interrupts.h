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
void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _U1ErrInterrupt(void);

#endif	/* INTERRUPTS_H */

