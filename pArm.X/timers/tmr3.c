/*
 * File:   tmr3.c
 * Author: javinator9889
 *
 * Created on 25 de agosto, 2020
 */

#include <p33EP512GM604.h>

#include "tmr3.h"
#include "../motor/motor.h"

motor_t *tmr3_motor;

void TMR3_Initialize(motor_t *motor) {
    tmr3_motor = motor;
    
    //TMR3 0; 
    TMR3 = 0x00;
    //Period = 0.001 s; Frequency = 59904000 Hz; PR3 59903; 
    PR3 = 0xE9FF;
    //TCKPS 1:1; TON disabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T3CON = 0x0;

    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    tmr3_motor->ticks++;
    IFS0bits.T3IF = 0;
}

void TMR3_Start(void) {
    /* Clear old value*/
    tmr3_motor->ticks = 0ULL;
    
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
