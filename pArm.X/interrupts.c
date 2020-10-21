/*
 * 2020 | pArm-S2 by Javinator9889
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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "printf/io.h"
#include "interrupts.h"
#include "motor/servo.h"
#include "utils/utils.h"
#include "utils/time.h"
#include "utils/uart.h"
#include "utils/types.h"
#include "utils/buffer.h"

volatile time_t _ns = 0ULL;
static char uart_buffer[1024] = {0};
static uint16_t uart_chars = 0U;
static volatile order_t *urx_order = NULL;
#ifdef LIMIT_SWITCH_ENABLED
static volatile uint_fast8_t *limit_switch_map = NULL;
#endif

void U1RX_Init(volatile order_t *order) {
    urx_order = order;
}

#ifdef LIMIT_SWITCH_ENABLED
void CN_Init(volatile uint_fast8_t *switch_map) {
    limit_switch_map = switch_map;
}
#endif

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void) {
    PORTBbits.RB6 = U1TXREG;
    IFS0bits.U1TXIF = 0; // Clear TX Interrupt flag
}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;
    if (U1STAbits.FERR == 1)
        return;
    if (U1STAbits.URXDA == 1) {
        char received_val = U1RXREG;
        PORTBbits.RB5 = received_val;
#ifdef CLI_MODE
        printf("%c", received_val);
#endif
        if (received_val == '\n' || received_val == '\r') {
            if (urx_order == NULL) {
#ifdef DEBUG_ENABLED
                printf("[DEBUG]\tU1RX not initialized!\n");
#endif
                return;
            }
#ifdef CLI_MODE
            printf("\n");
#endif
            uart_buffer[uart_chars++] = '\0';
#ifdef DEBUG_ENABLED
            printf("[DEBUG]\tNew order received: '%s'\n", uart_buffer);
#endif
            if (urx_order->order_buffer == NULL) {
                urx_order->order_buffer = BUFFER_create(uart_chars);
            }
            if (urx_order->order_buffer->size != uart_chars) {
                BUFFER_update_size(urx_order->order_buffer, uart_chars);
            }
            if (urx_order->order_buffer->buffer == NULL) {
#ifdef DEBUG_ENABLED
                printf("[ERROR]\tFailed to allocate %dB for order!\n",
                        (uart_chars * sizeof (char)));
#endif
                BUFFER_free(urx_order->order_buffer);
                uart_chars = 0U;
                return;
            }
            strncpy(urx_order->order_buffer->buffer,
                    uart_buffer, 
                    urx_order->order_buffer->size);
            uart_chars = 0U;
            urx_order->message_received = true;
        } else {
            uart_buffer[uart_chars++] = received_val;
            if (uart_chars >= 1024) {
                // UART buffer overflow...
                // Release memory and ignore instruction
                uart_chars = 0;
                printf("J11\n");
            }
        }
    }
}

void __attribute__((__interrupt__, no_auto_psv)) _U1ErrInterrupt(void) {
    if ((U1STAbits.OERR == 1)) {
        U1STAbits.OERR = 0;
    }

    IFS4bits.U1EIF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void) {
#ifdef LIMIT_SWITCH_ENABLED
    if (limit_switch_map != NULL) {
        limit_switch_map[0] = PORTAbits.RA0;
        limit_switch_map[1] = PORTAbits.RA1;
        limit_switch_map[2] = PORTBbits.RB0;
        limit_switch_map[3] = PORTBbits.RB1;
    }
#endif
    _CNIF = 0; // Clear the interruption flag
}
