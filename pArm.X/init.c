/*
 * File:   init.c
 * Author: javinator9889
 *
 * Created on 3 de julio de 2020, 13:07
 */

#include <p33EP512GM604.h>
#include "init.h"
#include "utils/utils.h"
#include "utils/defs.h"


void initBoard(void) {
    // Disable watchdog timer
    RCONbits.SWDTEN = 0;

    // Setup de PLL for reaching 40 MHz with a 7.3728 clock.
    // Maximum speed is of 140 MHz as the maximum temperature
    // of 85 ºC implies 70 MIPS.
    //
    // For working at ~120 MHz:
    // F_osc = F_in * M / (N1 * N2)
    // F_cy = F_osc / 2
    // F_osc ~= 120 MHz -> F_osc = 7.3728 * 65 / (2 * 2) = 119.808 MHz
    // F_cy = F_osc / 2 = 59.904 MHz
    //
    // Then, setup the PLL's prescaler, postcaler and divisor
    PLLFBDbits.PLLDIV = 63; // M = PLLDIV + 2 -> PLLDIV = 65 - 2 = 63
    CLKDIVbits.PLLPOST = 0; // N2 = 2 * (PLLPOST + 1) -> PLLPOST = (N2 / 2) - 1 = 0
    CLKDIVbits.PLLPRE = 0; // N1 = PLLPRE + 2; -> PLLPRE = N1 - 2 = 0

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

void initUART(void) {
    // RX RP55
    RPINR18bits.U1RXR = 0b0110111;
    TRISCbits.TRISC7 = 1;

    // TX RP54
    RPOR6bits.RP54R = 0b000001;
    TRISCbits.TRISC6 = 0;

    // Setup UART
    U1MODEbits.STSEL = 0;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.ABAUD = 0;
    U1MODEbits.BRGH = 0;
    
    // Calculate the baudrate using the following equation
    // UxBRG = ((FCY / Desired Baud rate) / 16) - 1
    // For 9600 bauds and FCY = 59.904E6, the obtained BRG is
    // -> 389, and the obtained baudrate is: 9600, with an error
    // of 0%
    U1BRG = 389;

    U1STAbits.URXISEL0 = 0;
    U1STAbits.URXISEL1 = 0;
    
    IEC0bits.U1TXIE = 1;
    
    U1STAbits.UTXEN = 1;
    
    // Enable UART TX Interrupt
    IEC0bits.U1TXIE = 1;
    IEC0bits.U1RXIE = 1;

    U1MODEbits.UARTEN = 1;
    
    DELAY_105uS;
}

void initPWM(void) {
    TRISBbits.TRISB11 = 0; // PWM3L
    TRISBbits.TRISB13 = 0; // PMW2L
    TRISBbits.TRISB15 = 0; // PWM1L
    TRISBbits.TRISB14 = 0; // PWM1H

    PTCON2bits.PCLKDIV = 0b101; // Prescaler 1:32

    // Setup PWM period - the motors have a
    // minimum time in between pulses of 20ms,
    // so the frequency must be of 50 Hz.
    //
    // F_osc = 119.808 MHz
    // F_PWM = 50 Hz
    // PWM_Prescaler = 64
    // PTPER = F_osc / (F_PWM * PWM_Prescaler) --> PTPER = 119.808 MHz / (50 Hz * 32)
    // = 37440 = PTPER --> F_PWM = 50.000 Hz
    PTPER = 37440;

    // Initialize intependent time base to zero.
    // As we are using PWMxL, we only use 
    // SPHASEx ports. If using PWMxH, just change 
    // SPHASEx to PHASEx ones.
    SPHASE3 = 0;
    SPHASE2 = 0;
    SPHASE1 = 0;
    SPHASE4 = 0;

    // By default, set no duty cycle of programmed signals
    SDC3 = 0;
    SDC2 = 0;
    SDC1 = 0;
    PDC1 = 0;

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
    IOCON1bits.PENH = 1;
    // Disable high output as we are not using it
    IOCON4bits.PENH = 0;
    IOCON3bits.PENH = 0;
    IOCON2bits.PENH = 0;

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

void TMR1_Initialize(void) {
    T1CON = 0;
    T1CONbits.TCS = 0;
    T1CONbits.TCKPS0 = 0;
    T1CONbits.TCKPS1 = 0b00; // 1:64
    T1CONbits.TGATE = 0;
    IFS0bits.T1IF = 0;
    IPC0bits.T1IP = 6;
    IEC0bits.T1IE = 1;
    // Setup PR1 for interrupting each 1 us
    // PR1 = (FCY / PRESCALER) * 1E-6 -> 59 == 0x3B
    PR1 = 0x3B;
    T1CONbits.TON = 1;
}

void TMR2_Initialize(void) {
    //TMR2 0; 
    TMR2 = 0x00;
    //Period = 0.001 s; Frequency = 59904000 Hz; PR2 59903; 
    PR2 = 0xE9FF;
    //TCKPS 1:1; T32 16 Bit; TON enabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T2CON = 0x8000;

    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
}

void initDigitalPorts(void)
{
    //Digital Ports for micro-interruptors, set as input
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    
    //Input Change Notification Interrupt configuration
    CNENBbits.CNIEB1 = 1;
    _CNIP=7;       // priority (7 = highest)
    _CNIE = 1; // Enable CN interrupts
    _CNIF = 0; //
    
    //Digital Ports for LED lights, set as output.
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB8 = 0;
    
    //Set I/O ports to digital, clear the analogic enable bit.
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA1 = 0;
    ANSELBbits.ANSB0 = 0;
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB7 = 0;
    ANSELBbits.ANSB8 = 0;
}
