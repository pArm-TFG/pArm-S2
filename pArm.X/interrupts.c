#include "interrupts.h"
#include "utils/time.h"


void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void) {
    increment_us();
    // Clear Timer1 interrupt
    IFS0bits.T1IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {
    // Clear Timer2 interrupt
    IFS0bits.T2IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0; // Clear TX Interrupt flag
}

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void)
{
     _CNIF = 0; 
}  