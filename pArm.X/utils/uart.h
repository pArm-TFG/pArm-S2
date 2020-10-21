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
 * File: uart.h
 * Author: Javinator9889
 * Comments: UART general I/O file handler
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_H
#define	UART_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Writes the given character through the UART1.
 * @param character - the char to be written.
 */
void putch(char character);

#ifdef USE_CUSTOM_PRINTF
/**
 * Wrapper for {@link putch}.
 */
void _putchar(char character);
#endif

#endif	/* UART_H */

