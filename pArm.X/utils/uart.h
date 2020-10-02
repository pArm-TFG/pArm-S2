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

#ifndef order_t
typedef struct {
    /**
     * Flag active when a new message is received through UART port.
     * It is updated at <pre>interrupts.h#_U1RXInterrupt</pre>.
     * 
     * @type bool
     * @see interrupts.h#_U1RXInterrupt
     */
    bool message_received;
    
    /**
     * Buffer which contains the order received by the UART. It has fixed
     * size so no extra space is used. This variable is updated at
     * <pre>interrupts.h#_U1RXInterrupt</pre>.
     * 
     * @type char[]
     * @see interrupts.h#_U1RXInterrupt
     */
    char *order_buffer;
    
    /**
     * A simple counter indicating how long is the <pre>order_buffer</pre>
     * string. This is updated in <pre>interrupts.h#_U1RXInterrupt</pre>.
     * 
     * @type uint16_t
     * @see interrupts.h#_U1RXInterrupt
     */
    uint16_t order_chars;
} 
/**
 * Order container with all the required information for managing
 * the UART messages.
 */
order_t;
#define order_t order_t
#endif

void putch(char data);
void _putchar(char character);

uint8_t getch(void);

#endif	/* UART_H */

