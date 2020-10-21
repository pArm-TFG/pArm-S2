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
 * File:   tmr4.c
 * Author: javinator9889
 *
 * Created on 25 de agosto, 2020
 */

#include <p33EP512GM604.h>
#include "tmr4.h"
#include "../utils/types.h"
#include "../motor/motor.h"
#include "../sync/barrier.h"

motor_t *TMR4_motor;
volatile barrier_t *TMR4_barrier;
static volatile int_fast32_t TMR4_count;
static volatile int_fast32_t duration;

void TMR4_Initialize(motor_t *motor, volatile barrier_t *barrier) {
    TMR4_motor = motor;
    TMR4_barrier = barrier;
    TMR4_count = .0F;

    //TMR4 0; 
    TMR4 = 0x00;
    //Period = 1 us; Frequency = 59904000 Hz; PR4 59903;
    PR4 = 0x3B;
    //TCKPS 1:1; T32 16 Bit; TON enabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T4CON = 0x0;

    IFS1bits.T4IF = 0;
    IEC1bits.T4IE = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void) {
    TMR4_count += 1L;
    
    if (TMR4_count >= duration)
        TMR4_Stop();

    IFS1bits.T4IF = 0;
}

void TMR4_Start(void) {
    /* Clear old value*/
    TMR4_count = .0F;

    /*Enable the interrupt*/
    IEC1bits.T4IE = 1;

    /* Start the Timer */
    T4CONbits.TON = 1;
}

void TMR4_Stop(void) {
    TMR4_motor->movement_finished = true;
    BARRIER_arrive(TMR4_barrier);
    // If movement is clockwise then add the count to current angle_us
    // else, the count must be substracted
    TMR4_motor->angle_us += (TMR4_motor->clockwise * TMR4_count);
    /* Stop the Timer */
    T4CONbits.TON = 0;

    /*Disable the interrupt*/
    IEC1bits.T4IE = 0;
}
