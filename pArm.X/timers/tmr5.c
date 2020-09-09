/*
 * File:   tmr5.c
 * Author: javinator9889
 *
 * Created on 25 de agosto, 2020
 */

#include <p33EP512GM604.h>

#include "tmr5.h"
#include "../motor/motor.h"

motor_t *tmr5_motor;

void TMR5_Initialize(motor_t *motor) {
    tmr5_motor = motor;
    
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
    tmr5_motor->angle_us += 1.0016F;
    if (tmr5_motor->angle_us >= tmr5_motor->movement_duration) {
        tmr5_motor->movement_finished = true;
        TMR5_Stop();
    }
    IFS1bits.T5IF = 0;
}

void TMR5_Start(void) {
    /* Clear old value*/
    tmr5_motor->angle_us = .0F;

    /*Enable the interrupt*/
    IEC1bits.T5IE = 1;

    /* Start the Timer */
    T5CONbits.TON = 1;
}

void TMR5_Stop(void) {
    /* Stop the Timer */
    T5CONbits.TON = 0;

    /*Disable the interrupt*/
    IEC1bits.T5IE = 0;
}

