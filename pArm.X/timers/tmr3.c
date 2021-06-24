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
 * File:   tmr3.c
 * Author: javinator9889
 *
 * Created on 25 de agosto, 2020
 */

#include <p33EP512GM604.h>
#include "tmr3.h"
#include "../utils/defs.h"
#include "../utils/types.h"
#include "../motor/motor.h"
#include "../sync/barrier.h"
#include "../motor/servo.h"
#ifdef DEBUG_ENABLED
#include "../printf/io.h"
#endif

static motor_t *TMR3_motor;
static volatile barrier_t *TMR3_barrier;
static volatile int_fast32_t TMR3_count;
static volatile int_fast32_t duration;
static int_fast8_t amount;

void TMR3_Initialize(motor_t *motor, volatile barrier_t *barrier) {
    TMR3_motor = motor;
    TMR3_barrier = barrier;
    TMR3_count = .0F;
    
    //TMR3 0; 
    TMR3 = 0x00;
    //Period = 1 ms; Frequency = 59904000 Hz; PR3 59903;
    PR3 = TMR_VALUE;
    //TCKPS 1:1; TON disabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T3CON = 0x0;

    IFS0bits.T3IF = 0;

    IEC0bits.T3IE = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    uint16_t dc = SERVO_position(TMR3_motor->servoHandler);
    dc += amount;
    SERVO_write_value(TMR3_motor->servoHandler, dc);
    if (dc == TMR3_motor->target_dc_value) {
        TMR3_Stop();
    }
    IFS0bits.T3IF = 0;
}

void TMR3_Start(void) {
    // Setup the amount value to +1 if current value is
    // lower than expected value, or -1 otherwise. '0' if
    // equals
    uint16_t dc = SERVO_position(TMR3_motor->servoHandler);
    uint16_t target_dc = TMR3_motor->target_dc_value;
    amount = (dc < target_dc) ? 1 : ((dc > target_dc) ? -1 : 0);
    
#ifdef DEBUG_ENABLED
    printf("[DEBUG]\tMotor#3 duty cycle is %d - moving to %d with step of %d\n",
            dc, target_dc, amount);
#endif
    
    /*Enable the interrupt*/
    IEC0bits.T3IE = 1;
    
    IFS0bits.T3IF = 0;

    /* Start the Timer */
    T3CONbits.TON = 1;
}

void TMR3_Stop(void) {
#ifdef DEBUG_ENABLED
    printf("[DEBUG]\tMotor#3 has finished moving!\n");
#endif
    TMR3_motor->movement_finished = true;
    BARRIER_arrive(TMR3_barrier);
    /* Stop the Timer */
    T3CONbits.TON = 0;

    /*Disable the interrupt*/
    IEC0bits.T3IE = 0;
}
