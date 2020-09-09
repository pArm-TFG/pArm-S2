/*
 * File:   tmr4.c
 * Author: javinator9889
 *
 * Created on 25 de agosto, 2020
 */

#include <p33EP512GM604.h>

#include "tmr4.h"
#include "../motor/motor.h"

motor_t *tmr4_motor;

void TMR4_Initialize(motor_t *motor) {
    tmr4_motor = motor;

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
    tmr4_motor->angle_us += 1.0016F;
    if (tmr4_motor->angle_us >= tmr4_motor->movement_duration) {
        tmr4_motor->movement_finished = true;
        TMR4_Stop();
    }
    IFS1bits.T4IF = 0;
}

void TMR4_Start(void) {
    /* Clear old value*/
    tmr4_motor->angle_us = .0F;

    /*Enable the interrupt*/
    IEC1bits.T4IE = 1;

    /* Start the Timer */
    T4CONbits.TON = 1;
}

void TMR4_Stop(void) {
    /* Stop the Timer */
    T4CONbits.TON = 0;

    /*Disable the interrupt*/
    IEC1bits.T4IE = 0;
}
