#include <xc.h>
#include "uart.h"

void putch(char character) {
    while (!IFS0bits.U1TXIF);
    U1TXREG = character;
}

void _putchar(char character) {
    putch(character);
}