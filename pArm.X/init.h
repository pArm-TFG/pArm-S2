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
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef INIT_H
#define	INIT_H

#include <xc.h>
#include "system_types.h"

/**
 * Initializes the PWM module with the required values at the specific ports.
 */
void initPWM(void);

/**
 * Initializes TMR1, which counts up every microsecond.
 */
void TMR1_Initialize(void);

/**
 * Initializes TMR2, which counts up every millisecond.
 */
void TMR2_Initialize(void);

/**
 * Initializes the peripheral ports.
 */
void init_ports(void);

/**
 * Initializes the UART communications.
 */
void initUART(void);

/**
 * Initializes the system clock.
 */
void init_clock(void);

/**
  @Summary
    Enables global interrupts of the dsPIC33EP512GM604

  @Description
    This routine enables the global interrupt bit for the dsPIC33EP512GM604

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        INTERRUPT_GlobalEnable ();
    }
    </code>

 */
inline static void INTERRUPT_GlobalEnable(void)
{
    __builtin_enable_interrupts();
}

inline static void INTERRUPT_GlobalDisable(void) {
    __builtin_disable_interrupts();
}

/**
 * Sets the CPU core control register operating mode to a value that is decided by the
 * SYSTEM_CORCON_MODES argument.
 * @param modeValue SYSTEM_CORCON_MODES initialization mode specifier
 * @example
 * <code>
 * SYSTEM_CORCONModeOperatingSet(CORCON_MODE_ENABLEALLSATNORMAL_ROUNDUNBIASED);
 * </code>
 */
inline static void SYSTEM_CORCONModeOperatingSet(SYSTEM_CORCON_MODES modeValue)
{
    CORCON = (CORCON & 0x00F2) | modeValue;
}

/**
 * Initializes the entire system. This function is a wrapper of all the other
 * functions declared in {@code init.h}.
 */
void system_initialize(void);

#endif	/* INIT_H */

