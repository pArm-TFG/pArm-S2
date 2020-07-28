#include "interrupts.h"

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void) {
    // Clear Timer1 interrupt
    IFS0bits.T1IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {
    // Clear Timer2 interrupt
    IFS0bits.T2IF = 0;
}
