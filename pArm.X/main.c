/*
 * File:   main.c
 * Author: javinator9889
 *
 * Created on 11 de junio de 2020, 11:16
 */


volatile char receivedValue = 0;

#include "utils/defs.h"
#include "pragmas.h"
#include <xc.h>
#include <stdbool.h>
#include <stddef.h>
#ifdef USE_CUSTOM_PRINTF
#include "printf/printf.h"
#endif
#include "utils/uart.h"
#include "init.h"
#include "interrupts.h"
#include "utils/time.h"
#include "rsa/rsa.h"

rsa_t *RSA_key = NULL;


void setup(void);
void loop(void);

int main(void) {
    setup();
    while (true) {
        loop();
    }
    return 0;
}

inline void setup(void) {
    system_initialize();
    
}

inline void loop(void) {
    
}
