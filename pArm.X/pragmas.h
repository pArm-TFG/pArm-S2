/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File: pragmas.h
 * Author: Javinator9889
 * Comments: A collection of configurations used in dsPIC33EP
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PRAGMAS_H
#define	PRAGMAS_H

#ifndef CONFIG_SIMULATOR

// FICD
#pragma config ICS = PGD1    //ICD Communication Channel Select bits->Communicate on PGEC1 and PGED1
#pragma config JTAGEN = OFF    //JTAG Enable bit->JTAG is disabled

// FPOR
#pragma config BOREN = ON    //->BOR is enabled
#pragma config ALTI2C1 = OFF    //Alternate I2C1 pins->I2C1 mapped to SDA1/SCL1 pins
#pragma config ALTI2C2 = OFF    //Alternate I2C2 pins->I2C2 mapped to SDA2/SCL2 pins
#pragma config WDTWIN = WIN25    //Watchdog Window Select bits->WDT Window is 25% of WDT period

// FWDT
#pragma config WDTPOST = PS32768    //Watchdog Timer Postscaler bits->1:32768
#pragma config WDTPRE = PR128    //Watchdog Timer Prescaler bit->1:128
#pragma config PLLKEN = ON    //PLL Lock Enable bit->Clock switch to PLL source will wait until the PLL lock signal is valid.
#pragma config WINDIS = OFF    //Watchdog Timer Window Enable bit->Watchdog Timer in Non-Window mode
#pragma config FWDTEN = OFF    //Watchdog Timer Enable bit->Watchdog timer enabled/disabled by user software

// FOSC
#pragma config POSCMD = NONE    //Primary Oscillator Mode Select bits->Primary Oscillator disabled
#pragma config OSCIOFNC = OFF    //OSC2 Pin Function bit->OSC2 is clock output
#pragma config IOL1WAY = ON    //Peripheral pin select configuration->Allow only one reconfiguration
#pragma config FCKSM = CSECME    //Clock Switching Mode bits->Both Clock switching and Fail-safe Clock Monitor are enabled

// FOSCSEL
#pragma config FNOSC = FRC    //Oscillator Source Selection->FRC
#pragma config PWMLOCK = ON    //PWM Lock Enable bit->Certain PWM registers may only be written after key sequence
#pragma config IESO = OFF    //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FGS
#pragma config GWRP = OFF    //General Segment Write-Protect bit->General Segment may be written
#pragma config GCP = OFF    //General Segment Code-Protect bit->General Segment Code protect is Disabled

#endif /* CONFIG_SIMULATOR */

#endif	/* PRAGMAS_H */

