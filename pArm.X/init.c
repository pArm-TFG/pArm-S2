/*
 * Copyright (C) 2020 - present | pArm-S2 by Javinator9889
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/.
 *
 * Created by Javinator9889 on 2020 - pArm-S1.
 */

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
#include "system_types.h"


void init_clock(void) {
#ifndef CONFIG_SIMULATOR
    // FRCDIV FRC/1; PLLPRE 2; DOZE 1:8; PLLPOST 1:2; DOZEN disabled; ROI disabled; 
    CLKDIV = 0x3000;
    // TUN Center frequency; 
    OSCTUN = 0x00;
    // ROON disabled; ROSEL FOSC; RODIV 0; ROSSLP disabled; 
    REFOCON = 0x00;
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
    PLLFBD = 0x3F;
    // AD1MD enabled; PWMMD enabled; T3MD enabled; T4MD enabled; T1MD enabled; U2MD enabled; T2MD enabled; U1MD enabled; QEI1MD enabled; SPI2MD enabled; SPI1MD enabled; C2MD enabled; C1MD enabled; DCIMD enabled; T5MD enabled; I2C1MD enabled; 
    PMD1 = 0x00;
    // OC5MD enabled; OC6MD enabled; OC7MD enabled; OC8MD enabled; OC1MD enabled; IC2MD enabled; OC2MD enabled; IC1MD enabled; OC3MD enabled; OC4MD enabled; IC6MD enabled; IC7MD enabled; IC5MD enabled; IC8MD enabled; IC4MD enabled; IC3MD enabled; 
    PMD2 = 0x00;
    // ADC2MD enabled; PMPMD enabled; U3MD enabled; QEI2MD enabled; RTCCMD enabled; CMPMD enabled; T9MD enabled; T8MD enabled; CRCMD enabled; T7MD enabled; I2C2MD enabled; T6MD enabled; 
    PMD3 = 0x00;
    // U4MD enabled; CTMUMD enabled; REFOMD enabled; 
    PMD4 = 0x00;
    // PWM2MD enabled; PWM1MD enabled; PWM4MD enabled; SPI3MD enabled; PWM3MD enabled; PWM6MD enabled; PWM5MD enabled; 
    PMD6 = 0x00;
    // PTGMD enabled; DMA0MD enabled; 
    PMD7 = 0x00;
    // CF no clock failure; NOSC FRCPLL; CLKLOCK unlocked; OSWEN Switch is Complete; IOLOCK not-active; 
    __builtin_write_OSCCONH((uint8_t) (0x01));
    __builtin_write_OSCCONL((uint8_t) (0x01));
    
    // Wait for Clock switch to occur
    while (OSCCONbits.OSWEN != 0);
    // And wait for clock switching to happen
    // First, wait for clock switch to occur
    // and thenm wait the PLL to lock
    while (OSCCONbits.LOCK != 1);
#endif
}

void initUART(void) {
    // Unlock the Peripheral Pin Selector (PPS)
    // for allowing changes on TRIS ports without
    // affecting expected device behavior.
    // 0xBF is a shortcut for ~(1 << 6) == 191
#ifndef CONFIG_SIMULATOR
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock PPS
#endif
    TRISCbits.TRISC7 = 1; // RC7 set as input
    TRISCbits.TRISC6 = 0; // RC6 set as output

    RPOR6bits.RP54R = 0b00001; // RC6->UART1:U1TX
    RPINR18bits.U1RXR = 55;
    // Lock again the PPS as we are done
    // configuring the remappable ports.
    // 0x40 is a shortcut for (1 << 6) == 64
#ifndef CONFIG_SIMULATOR
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
#endif
    
    IEC0bits.U1TXIE = 0;
    IEC0bits.U1RXIE = 0;
    
    // Setup UART
    // Stop on idle
    U1MODEbits.USIDL = 1;
    // Disable IrDA
    U1MODEbits.IREN = 0;
    // Use only TX and RX pins
    // ignoring CTS, RTS and BCLK
    U1MODEbits.UEN = 0b00;
    // Do not wake-up with UART
    U1MODEbits.WAKE = 0;
    // Disable loopback mode
    U1MODEbits.LPBACK = 0;
    // Do not use automatic baudrate when receiving
    U1MODEbits.ABAUD = 0;
    // Disable polarity inversion. Idle state is '1'
    U1MODEbits.URXINV = 0;
    // Do not use high speed baudrate
    U1MODEbits.BRGH = 0;
    // 8 data bits without parity
    U1MODEbits.PDSEL = 0b00;
    // One stop bit
    U1MODEbits.STSEL = 0;
    
    // Calculate the baudrate using the following equation
    // UxBRG = ((FCY / Desired Baud rate) / 16) - 1
    // For 9600 bauds and FCY = 59.904E6, the obtained BRG is
    // -> 389, and the obtained baudrate is: 9600, with an error
    // of 0%
    U1BRG = 0x185; 

    // Interrupt after one RX character is received;
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN enabled; ADDEN disabled; 
    U1STA = 0x400;
    
    // Enable UART TX Interrupt
    IEC0bits.U1TXIE = 1;
    IEC0bits.U1RXIE = 1;
    IEC4bits.U1EIE = 1;
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;
    IFS4bits.U1EIF = 0;
    IPC2bits.U1RXIP = 0b110;

    //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
    LATCbits.LATC7 = 1;
    LATCbits.LATC6 = 1;
    U1MODEbits.UARTEN = 1; // enabling UART ON bit
    U1STAbits.UTXEN = 1;
    
    // Wait 105 uS (when baudrate is 9600) for a first
    // transmission bit to be sent and detected, so then
    // the UART can be used
    DELAY_105uS;
}

void initPWM(void) {
    TRISBbits.TRISB11 = 0; // PWM3L
    TRISBbits.TRISB13 = 0; // PMW2L
    TRISBbits.TRISB15 = 0; // PWM1L
    TRISAbits.TRISA7 = 0;  // PWM4L

    PTCON2bits.PCLKDIV = 0b110; // Prescaler 1:32

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
    SDC4 = 0;

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
    IOCON6bits.PENH = 0;
    IOCON6bits.PENL = 0;
    IOCON6bits.OVRENH = 1;
    IOCON6bits.OVRENH = 1;

    // Set PWM configurations to zero by default
    PWMCON6 = 0;
    PWMCON5 = 0;
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
    TMR1 = 0x00;
    // Period = 1 us;
    // Frequency = 59904000 Hz;
    // PR1 = 59 == an interrupt ~= 1.0016 us
    PR1 = 0x3B;
    
    // TON enabled; ->      1
    // Empty bit ->         0
    // TSIDL disabled; ->   0

    // Empty bit ->         0
    // Empty bit ->         0
    // Empty bit ->         0
    // Empty bit ->         0

    // Empty bit ->         0
    // Empty bit ->         0
    // TGATE disabled; ->   0
    // TCKPS 1:1; ->        0

    // Empty bit ->         0
    // TSYNC enabled ->     0
    // TCS FOSC/2; ->       0
    // Empty bit ->         0
    // 1000 0000 0000 0000 == 0x8000
    T1CON = 0x8000;

    // Clear interrupt flag,
    IFS0bits.T1IF = 0;
    // set high priority
    IPC0bits.T1IP = 5;
    // and enable interrupts
    IEC0bits.T1IE = 1;
}

void TMR2_Initialize(void) {
    // Reset TMR2 to zero
    TMR2 = 0x00;
    
    // Period = 1 ms; 
    // Frequency = 59904000 Hz; 
    // PR2 59903 == an interrupt each millisecond.
    PR2 = 0xE9FF;
    // TON enabled; ->      1
    // Empty bit ->         0
    // TSIDL disabled; ->   0
    
    // Empty bit ->         0
    // Empty bit ->         0
    // Empty bit ->         0
    // Empty bit ->         0
    
    // Empty bit ->         0
    // Empty bit ->         0
    // TGATE disabled; ->   0
    // TCKPS 1:1; ->        0
    
    // T32 16 Bit; ->       0
    // Empty bit ->         0
    // TCS FOSC/2; ->       0
    // Empty bit ->         0
    // 1000 0000 0000 0000 == 0x8000
    T2CON = 0x8000;

    // Clear interrupt flag...
    IFS0bits.T2IF = 0;
    // set high priority
    IPC1bits.T2IP = 5;
    // and enable TMR2 interruptions
    IEC0bits.T2IE = 1;
}

void init_ports(void)
{   
    //Digital Ports for micro-interruptors, set as input
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    
    //Input Change Notification Interrupt configuration
    _CNIP = 5;       // priority (7 = highest)
    _CNIE = 1; // Enable CN interrupts
    _CNIF = 0; // Interrupt flag cleared
    CNENBbits.CNIEB0 = 1;
    CNENBbits.CNIEB1 = 1;
    CNENAbits.CNIEA0 = 1;
    CNENAbits.CNIEA1 = 1;

    
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

inline void system_initialize(void) {
    INTERRUPT_GlobalDisable();
    init_clock();
    init_ports();
    initPWM();
    initUART();
    SYSTEM_CORCONModeOperatingSet(CORCON_MODE_PORVALUES);
    INTERRUPT_GlobalEnable();
}
