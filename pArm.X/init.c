/*
 * File:   init.c
 * Author: javinator9889
 *
 * Created on 3 de julio de 2020, 13:07
 */

#include <p33EP512GM604.h>
#include "init.h"


void initBoard(void) {
    // Disable watchdog timer
    RCONbits.SWDTEN = 0;
    
    // Setup de PLL for reaching 40 MHz with a 7.3728 clock.
    // Maximum speed is of 140 MHz as the maximum temperature
    // of 85 ºC implies 70 MIPS.
    //
    // For working at ~40 MHz:
    // F_osc = F_in * M / (N1 * N2)
    // F_cy = F_osc / 2
    // F_osc ~= 80 MHz -> F_osc = 7.3728 * 217 / (5 * 4) = 79.9949 MHz
    // F_cy = F_osc / 2 = 39.9974 MHz
    //
    // Then, setup the PLL's prescaler, postcaler and divisor
    PLLFBDbits.PLLDIV = 215;    // M = PLLDIV + 2 -> PLLDIV = 217 - 2 = 215
    CLKDIVbits.PLLPOST = 1;     // N2 = 2 * (PLLPOST + 1) -> PLLPOST = (N2 / 2) - 1 = 1
    CLKDIVbits.PLLPRE = 3;      // N1 = PLLPRE + 2; -> PLLPRE = N1 - 2 = 3
    
    // Notify clock to use PLL
    // Start clock switching to primary
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(0x01);
    
    // And wait for clock switching to happen
    // First, wait for clock switch to occur
    // and thenm wait the PLL to lock
    while (OSCCONbits.COSC != 0b011);
    while (OSCCONbits.LOCK != 1);
}

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

void initPWM(void) {
    TRISBbits.TRISB11 = 0; // PWM3L
    TRISBbits.TRISB13 = 0; // PMW2L
    TRISBbits.TRISB15 = 0; // PWM1L
    TRISAbits.TRISA7 = 0; // PMW4L
    
    PTCON2bits.PCLKDIV = 0b101;  // Prescaler 1:32
    
    // Setup PWM period - the motors have a
    // minimum time in between pulses of 20ms,
    // so the frequency must be of 50 Hz.
    //
    // F_osc = 79.9949
    // F_PWM = 50 Hz
    // PWM_Prescaler = 32
    // PTPER = F_osc / (F_PWM * PWM_Prescaler) --> PTPER = 79.9949 MHz / (50 Hz * 32)
    // = 49996.8 ~= 49996 = PTPER --> F_PWM = 50.00080006... Hz
    PTPER = 49996;
    
    // Initialize intependent time base to zero.
    // As we are using PWMxL, we only use 
    // SPHASEx ports. If using PWMxH, just change 
    // SPHASEx to PHASEx ones.
    SPHASE3 = 0;
    SPHASE2 = 0;
    SPHASE1 = 0;
    SPHASE4 = 0;
    
    // By default, set no duty cycle of programmed signals
    SDC4 = 0;
    SDC3 = 0;
    SDC2 = 0;
    SDC1 = 0;
    
    // Disable Dead Time values
    ALTDTR4 = 0;
    ALTDTR3 = 0;
    ALTDTR2 = 0;
    ALTDTR1 = 0;
    
    DTR4 = 0;
    DTR3 = 0;
    DTR2 = 0;
    DTR1 = 0;
    
    // True independent work mode, so then both PWMxH and
    // PWMxL can be used independently
    IOCON4bits.PMOD = 0b11;
    IOCON3bits.PMOD = 0b11;
    IOCON2bits.PMOD = 0b11;
    IOCON1bits.PMOD = 0b11;
    
    // Disable PWM fault input
    FCLCON4bits.FLTMOD = 0b11;
    FCLCON3bits.FLTMOD = 0b11;
    FCLCON2bits.FLTMOD = 0b11;
    FCLCON1bits.FLTMOD = 0b11;
    
    // Do not swap LOW/HIGH values
    IOCON4bits.SWAP = 0;
    IOCON3bits.SWAP = 0;
    IOCON2bits.SWAP = 0;
    IOCON1bits.SWAP = 0;
    
    // Set pins as PWM ones
    IOCON4bits.PENL = 1;
    IOCON3bits.PENL = 1;
    IOCON2bits.PENL = 1;
    IOCON1bits.PENL = 1;
    // Disable high output as we are not using it
    IOCON4bits.PENH = 0;
    IOCON3bits.PENH = 0;
    IOCON2bits.PENH = 0;
    IOCON1bits.PENH = 0;

    // Set PWM configurations to zero by default
    PWMCON4 = 0;
    PWMCON3 = 0;
    PWMCON2 = 0;
    PWMCON1 = 0;
    
    // Disable dead time in-between output switches
    PWMCON4bits.DTC = 0b10;
    PWMCON3bits.DTC = 0b10;
    PWMCON2bits.DTC = 0b10;
    PWMCON1bits.DTC = 0b10;
    
    // and enable the PWM module
    PTCONbits.PTEN = 1;
}
