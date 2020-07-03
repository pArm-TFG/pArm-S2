/*
 * File:   init.c
 * Author: javinator9889
 *
 * Created on 3 de julio de 2020, 13:07
 */

#include <p33EP512GM604.h>

#include "init.h"

#define delayUs(x) _delay((unsigned long) (x * (CLK_SPEED / 2000000.0)))

void initUART(int port, int baudrate) {
    // RX RPI44
    RPINR18bits.U1RXR = 0b0101100;
    TRISBbits.TRISB12 = 1;
    
    // TX RP43
    RPOR4bits.RP43R = 0b000001;
    TRISBbits.TRISB11 = 0;
    
    // Setup UART
    U1MODEbits.USIDL = 1;
    U1MODEbits.IREN = 0;
    U1MODEbits.UEN = 0;
    U1MODEbits.WAKE = 0;
    U1MODEbits.LPBACK = 0;
    U1MODEbits.ABAUD = 0;
    U1MODEbits.URXINV = 0;
    U1MODEbits.BRGH = 0;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;
    
    U1STAbits.URXISEL = 0;
    U1BRG = (int) (((CLK_SPEED / 2) / (16 * baudrate)) - 1);
    
    IPC2bits.U1RXIP = 0;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 0;
    
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
    
    __delay32((int) ((1 / baudrate) * 1E6));
}
