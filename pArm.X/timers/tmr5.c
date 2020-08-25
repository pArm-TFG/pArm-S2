/*
 * File:   tmr5.c
 * Author: javinator9889
 *
 * Created on 25 de agosto, 2020
 */

#include <p33EP512GM604.h>

#include "tmr5.h"
#include "../motor/motor.h"

Motor_t *tmr5_motor;

void TMR5_Initialize(Motor_t *motor) {
    tmr5_motor = motor;
    
    //TMR5 0; 
    TMR5 = 0x00;
    //Period = 0.001 s; Frequency = 59904000 Hz; PR5 59903; 
    PR5 = 0xE9FF;
    //TCKPS 1:1; TON enabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T5CON = 0x0;

    IFS1bits.T5IF = 0;
    IEC1bits.T5IE = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void) {
    tmr5_motor->ticks++;
    IFS1bits.T5IF = 0;
}

void TMR5_Start(void) {
    /* Clear old value*/
    tmr5_motor->ticks = 0ULL;

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

