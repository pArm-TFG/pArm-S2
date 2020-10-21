/*
 * File:   tmr5.c
 * Author: javinator9889
 *
 * Created on 25 de agosto, 2020
 */

#include <p33EP512GM604.h>
#include "tmr5.h"
#include "../utils/types.h"
#include "../motor/motor.h"
#include "../sync/barrier.h"

motor_t *TMR5_motor;
volatile barrier_t *TMR5_barrier;
static volatile int_fast32_t TMR5_count;
static volatile int_fast32_t duration;

void TMR5_Initialize(motor_t *motor, volatile barrier_t *barrier) {
    TMR5_motor = motor;
    TMR5_barrier = barrier;
    TMR5_count = .0F;
    
    //TMR5 0; 
    TMR5 = 0x00;
    //Period = 1 us; Frequency = 59904000 Hz; PR5 59903; 
    PR5 = 0x3B;
    //TCKPS 1:1; TON enabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T5CON = 0x0;

    IFS1bits.T5IF = 0;
    IEC1bits.T5IE = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void) {
    TMR5_count += 1L;

    if (TMR5_count >= duration)
        TMR5_Stop();

    IFS1bits.T5IF = 0;
}

void TMR5_Start(void) {
    /* Clear old value*/
    TMR5_count = .0F;

    /*Enable the interrupt*/
    IEC1bits.T5IE = 1;

    /* Start the Timer */
    T5CONbits.TON = 1;
}

void TMR5_Stop(void) {
    TMR5_motor->movement_finished = true;
    BARRIER_arrive(TMR5_barrier);
    // If movement is clockwise then add the count to current angle_us
    // else, the count must be substracted
    TMR5_motor->angle_us += (TMR5_motor->clockwise * TMR5_count);
    /* Stop the Timer */
    T5CONbits.TON = 0;

    /*Disable the interrupt*/
    IEC1bits.T5IE = 0;
}

