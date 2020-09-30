#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "interrupts.h"
#include "utils/time.h"
#include "motor/servo.h"
#include "utils/time.h"

volatile int _ICNFLAG = 0; // Auxiliar Flag defined in interrupts.h
volatile time_t _ns = 0ULL;
static char uart_buffer[1024] = {0};
static uint16_t uart_chars = 0U;
extern bool message_received;
extern char *order_buffer;
extern uint16_t order_chars;

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void) {
    _now_us += 1ULL;
    // Clear Timer1 interrupt
    IFS0bits.T1IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {
    _now_ms += 1ULL;
    // Clear Timer2 interrupt
    IFS0bits.T2IF = 0;
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
        if (received_val == '\n') {
            if (order_buffer != NULL) {
                free(order_buffer);
            }
            uart_buffer[uart_chars] = '\0';
            order_buffer = (char *) malloc(sizeof(char) * uart_chars);
            strncpy(order_buffer, uart_buffer, uart_chars);
            order_chars = uart_chars;
            uart_chars = 0;
            message_received = true;
        } else {
            uart_buffer[uart_chars++] = received_val;
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

void __attribute__((interrupt, no_auto_psv)) _T6Interrupt() {
    _ns += (TIME_now_us() * 1000ULL) + 50ULL;
    IFS2bits.T6IF = false;
}
