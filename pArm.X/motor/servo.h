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
 * File: servo.h
 * Author: Javinator9889
 * Comments: Servo controller header file
 * Revision history: 1.0
 */

#ifndef SERVO_H
#define	SERVO_H

#include <xc.h>
#include "../utils/defs.h"

#define usToTicks(_us)      ( (clockCyclesPerMicrosecond() * _us) / PRESCALE )
#define ticksToUs(_ticks)   ( ((unsigned)_ticks * PRESCALE) / clockCyclesPerMicrosecond() )
#define MIN_PULSE_WIDTH     usToTicks(((unsigned long) MIN_PULSE_MS * 1000))
#define MAX_PULSE_WIDTH     usToTicks(((unsigned long) MAX_PULSE_MS * 1000))

// Servo definition
typedef struct {
    uint16_t *dutyCycleRegister;
} Servo;

void writeAngle(const Servo *servo, uint16_t angle);
void writeMilliseconds(const Servo *servo, double ms);
void writeValue(const Servo *servo, uint16_t dutyCycleValue);

#endif	/* SERVO_H */
