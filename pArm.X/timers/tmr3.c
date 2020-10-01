/*
 * File:   tmr3.c
 * Author: javinator9889
 *
 * Created on 25 de agosto, 2020
 */

#include <p33EP512GM604.h>
#include "tmr3.h"
#include "../utils/types.h"
#include "../motor/motor.h"
#include "../sync/barrier.h"

motor_t *TMR3_motor;
barrier_t *TMR3_barrier;
double64_t TMR3_count;

void TMR3_Initialize(motor_t *motor, barrier_t *barrier) {
    TMR3_motor = motor;
    TMR3_barrier = barrier;
    TMR3_count = .0F;
    
    //TMR3 0; 
    TMR3 = 0x00;
    //Period = 1 us; Frequency = 59904000 Hz; PR3 59903; 
    PR3 = 0x3B;
    //TCKPS 1:1; TON disabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T3CON = 0x0;

    IFS0bits.T3IF = 0;

    IEC0bits.T3IE = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    TMR3_count += 1.0016F;

    if (TMR3_count >= TMR3_motor->movement_duration) {
        TMR3_motor->movement_finished = true;
        BARRIER_arrive(TMR3_barrier);
        // If movement is clockwise then add the count to current angle_us
        // else, the count must be substracted
        TMR3_motor->angle_us += (TMR3_motor->clockwise * TMR3_count);
        TMR3_Stop();
    }
    IFS0bits.T3IF = 0;
}

void TMR3_Start(void) {
    /* Clear old value*/
    TMR3_count = .0F;
    
    /*Enable the interrupt*/
    IEC0bits.T3IE = 1;

    /* Start the Timer */
    T3CONbits.TON = 1;
}

void TMR3_Stop(void) {
    /* Stop the Timer */
    T3CONbits.TON = 0;

    /*Disable the interrupt*/
    IEC0bits.T3IE = 0;
}
