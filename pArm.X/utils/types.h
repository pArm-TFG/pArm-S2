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
 * File: types.h
 * Author: Javinator9889
 * Comments: A header file that contains all custom data types used in this project.
 * Revision history: 1.0
 */

#ifndef TYPES_H
#define	TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <stddef.h>

// Time definitions
#ifndef time_t
typedef uint64_t time_t;
#define time_t time_t
#endif

// Double precision double type
#ifndef double64_t
#if DBL_MANT_DIG < LDBL_MANT_DIG
typedef long double double64_t;
#else
typedef double double64_t;
#endif
#define double64_t double64_t
#endif

// Position definitions
#ifndef point_t

typedef struct {
    double64_t x;
    double64_t y;
    double64_t z;
} point_t;
#define point_t point_t
#endif

// Angle definitions
#ifndef angle_t

typedef struct {
    double64_t theta0;
    double64_t theta1;
    double64_t theta2;
} angle_t;
#define angle_t angle_t
#endif

// Pointer to function definitions
#ifndef TMR_func

typedef void (*TMR_func)(void) ;
#define TMR_func TMR_func
#endif

// GCODE custom return type
#ifndef GCODE_ret_t

typedef struct {
    bool is_err;
    int_fast16_t code;
    void *gcode_ret_val;
} GCODE_ret;
#define GCODE_ret_t GCODE_ret
#endif

// order_t defs
#ifndef MAX_ORDER_LENGTH
#define MAX_ORDER_LENGTH 1024U
#endif


// buffer_t definition for arbitrary buffers
#ifndef buffer_t

typedef struct {
    size_t size;
    size_t bsize;
    char *buffer;
} buffer_t;
#define buffer_t buffer_t
#endif

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
     * @type buffer_t
     * @see interrupts.h#_U1RXInterrupt
     */
    buffer_t *order_buffer;
}
/**
 * Order container with all the required information for managing
 * the UART messages.
 */
order_t;
#define order_t order_t
#endif

#endif	/* TYPES_H */

