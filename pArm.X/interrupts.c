#include "interrupts.h"
#include "utils/time.h"

volatile int __ICNFLAG = 0;
extern char receivedValue;

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
    IFS0bits.U1RXIF = 0;
    printf("!\n");
    if (U1STAbits.FERR == 1 )
        return;
    if (U1STAbits.OERR == 1) {
        U1STAbits.OERR = 0;
        return;
    }
    if (U1STAbits.URXDA == 1) {
        receivedValue = U1RXREG;
    }
}

/*void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void) {
    printf("RXI2 received");
    if (U2STAbits.FERR == 1);
    if (U2STAbits.OERR == 1)
        U2STAbits.OERR = 0;
    if (U2STAbits.URXDA == 1) {
        receivedValue = U2RXREG;
    }
    IFS1bits.U2RXIF = 0;
}*/

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void)
{
    __ICNFLAG = 1;
     _CNIF = 0; 
}  