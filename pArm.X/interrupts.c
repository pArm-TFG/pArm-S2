#include "interrupts.h"
#include "utils/time.h"

 volatile int _ICNFLAG = 0; // Auxiliar Flag defined in interrupts.h

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void) {
    _now_us += 1ULL;
    // Clear Timer1 interrupt
    IFS0bits.T1IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {
    _now_ms += 1ULL;
    // Clear Timer2 interrupt
    IFS0bits.T2IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0; // Clear TX Interrupt flag
}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) {
    //    if (U1STAbits.URXDA == 1) printf("%c", U1RXREG);
    printf("RX message!\n");
    IFS0bits.U1RXIF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void)
{
    _ICNFLAG = 1; // Notify the input change using the auxiliar flag
     _CNIF = 0; // Clear the interruption flag
     
}  