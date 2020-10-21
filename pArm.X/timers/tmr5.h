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
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMER5_H
#define	TIMER5_H

#include "../motor/motor.h"
#include "../sync/barrier.h"

/**
 * Motor managed by TMR5.
 */
extern motor_t *TMR5_motor;

/**
 * Barrier used for synchronizing motors.
 */
extern volatile barrier_t *TMR5_barrier;

/**
 * Initializes the TMR5 to manage specified motor position in the given
 * barrier.
 * 
 * @param motor - the motor to manage.
 * @param barrier - the barrier that handles all the motors.
 */
void TMR5_Initialize(motor_t *motor, volatile barrier_t *barrier);

/**
 * TMR5 interrupt handler for managing the motor status.
 */
void __attribute__ ((interrupt, no_auto_psv)) _T5Interrupt(void);

/**
 * Starts the TMR5 that handles the specified motor.
 */
void TMR5_Start(void);

/**
 * Stops the TMR5 that handles the specified motor.
 */
void TMR5_Stop(void);

#endif	/* TIMER3_H */
