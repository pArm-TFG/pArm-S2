#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "printf/io.h"
#include "interrupts.h"
#include "motor/servo.h"
#include "utils/utils.h"
#include "utils/time.h"
#include "utils/uart.h"
#include "utils/types.h"

volatile int _ICNFLAG = 0; // Auxiliar Flag defined in interrupts.h
volatile time_t _ns = 0ULL;
static char uart_buffer[1024] = {0};
static uint16_t uart_chars = 0U;
static volatile order_t *urx_order = NULL;

void U1RX_Init(volatile order_t *order) {
    urx_order = order;
}

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0; // Clear TX Interrupt flag
}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;
    if (U1STAbits.FERR == 1)
        return;
    if (U1STAbits.URXDA == 1) {
        char received_val = U1RXREG;
        printf("%c", received_val);
        if (received_val == '\n' || received_val == '\r') {
            if (urx_order == NULL) {
                printf("[DEBUG]\tU1RX not initialized!\n");
                return;
            }
            if (urx_order->order_buffer != NULL) {
                free(urx_order->order_buffer);
            }
//            if (order->order_buffer == NULL) {
//                char ptr[1024] = {'\0'};
//                order->order_buffer = ptr;
//            }
            printf("\n");
            uart_buffer[uart_chars++] = '\0';
            printf("[DEBUG]\tUpdating uart buffer with length of %d chars...\n", uart_chars);
            printf("[DEBUG]\tBuffer: %s\n", uart_buffer);
            urx_order->order_buffer = (char *) malloc(uart_chars * sizeof(char));
            if (urx_order->order_buffer == NULL) {
                printf("[ERROR]\tFailed to allocate %dB for order!\n", (uart_chars * sizeof(char)));
                return;
            }
            strncpy(urx_order->order_buffer, uart_buffer, (uart_chars * sizeof(char)));
//            strcpy(urx_order->order_buffer, uart_buffer);
//            strncpy(urx_order, uart_buffer, (uart_chars * sizeof(char)));
//            strcpy(order->order_buffer, uart_buffer);
//            cstrncpy(uart_buffer, order->order_buffer, uart_chars);
//            for (uint16_t i = 0; i < uart_chars; ++i) {
//                order->order_buffer[i] = uart_buffer[i];
//            }
//            char buffer[MAX_ORDER_LENGTH] = {0};
//            strncpy(buffer, uart_buffer, uart_chars);
//            snprintf(urx_order->order_buffer, (size_t) (uart_chars * sizeof(char)), "%s", uart_buffer);
//            order->order_buffer = buffer;
            urx_order->order_chars = uart_chars;
//            for (int i = 0; i <= urx_order->order_chars; ++i) {
//                printf("%c", order->order_buffer[i]);
//            }
            printf("[DEBUG]\tReceived order: %s\n", urx_order->order_buffer);
            uart_chars = 0;
            urx_order->message_received = true;
//            }
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

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void) {
    limit_switch_map[0] = PORTAbits.RA0;
    limit_switch_map[1] = PORTAbits.RA1;
    limit_switch_map[2] = PORTBbits.RB0;
    limit_switch_map[3] = PORTBbits.RB1;
    _ICNFLAG = 1; // Notify the input change using the auxiliar flag
    _CNIF = 0; // Clear the interruption flag
}

void __attribute__((__interrupt__, no_auto_psv)) _U1ErrInterrupt(void) {
    if ((U1STAbits.OERR == 1)) {
        U1STAbits.OERR = 0;
    }

    IFS4bits.U1EIF = 0;
}
