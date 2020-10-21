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
#ifndef TIMER3_H
#define	TIMER3_H

#include "../motor/motor.h"
#include "../sync/barrier.h"

extern motor_t *TMR3_motor;
extern volatile barrier_t *TMR3_barrier;

void TMR3_Initialize(motor_t *motor, volatile barrier_t *barrier);
void __attribute__ ((interrupt, no_auto_psv)) _T3Interrupt(void);
void TMR3_Start(void);
void TMR3_Stop(void);

#endif	/* TIMER3_H */
