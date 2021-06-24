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
 * File:   tmr4.c
 * Author: javinator9889
 *
 * Created on 25 de agosto, 2020
 */

#include <p33EP512GM604.h>
#include "tmr4.h"
#include "../utils/defs.h"
#include "../utils/types.h"
#include "../motor/motor.h"
#include "../sync/barrier.h"
#include "../motor/servo.h"

static motor_t *TMR4_motor;
static volatile barrier_t *TMR4_barrier;
static volatile int_fast32_t TMR4_count;
static volatile int_fast32_t duration;
static int_fast8_t amount;

void TMR4_Initialize(motor_t *motor, volatile barrier_t *barrier) {
    TMR4_motor = motor;
    TMR4_barrier = barrier;
    TMR4_count = .0F;

    //TMR4 0; 
    TMR4 = 0x00;
    //Period = 1 ms; Frequency = 59904000 Hz; PR4 59903;
    PR4 = TMR_VALUE;
    //TCKPS 1:1; T32 16 Bit; TON enabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T4CON = 0x0;

    IFS1bits.T4IF = 0;
    IEC1bits.T4IE = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void) {
    uint16_t dc = SERVO_position(TMR4_motor->servoHandler);
    dc += amount;
    SERVO_write_value(TMR4_motor->servoHandler, dc);
    if (dc == TMR4_motor->target_dc_value) {
        TMR4_Stop();
    }

    IFS1bits.T4IF = 0;
}

void TMR4_Start(void) {
    // Setup the amount value to +1 if current value is
    // lower than expected value, or -1 otherwise. '0' if
    // equals
    uint16_t dc = SERVO_position(TMR4_motor->servoHandler);
    uint16_t target_dc = TMR4_motor->target_dc_value;
    amount = (dc < target_dc) ? 1 : ((dc > target_dc) ? -1 : 0);
    
#ifdef DEBUG_ENABLED
    printf("[DEBUG]\tMotor#4 duty cycle is %d - moving to %d with step of %d\n",
            dc, target_dc, amount);
#endif

    /*Enable the interrupt*/
    IEC1bits.T4IE = 1;

    /* Start the Timer */
    T4CONbits.TON = 1;
}

void TMR4_Stop(void) {
#ifdef DEBUG_ENABLED
    printf("[DEBUG]\tMotor#4 has finished moving!\n");
#endif
    TMR4_motor->movement_finished = true;
    BARRIER_arrive(TMR4_barrier);
    /* Stop the Timer */
    T4CONbits.TON = 0;

    /*Disable the interrupt*/
    IEC1bits.T4IE = 0;
}
