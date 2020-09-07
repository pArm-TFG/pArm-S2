/* 
 * File: rand.c
 * Author: Javinator9889
 * Comments: Generate random numbers using the elapsed time in ns.
 * Revision history: v1.0
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "time.h"
#include "rand.h"

time_t _ns = 0ULL;
bool _rand_init = false;

void RAND_init(void) {
    //TMR6 0; 
    TMR6 = 0x00;
    //Period = 0.0000000501 s; Frequency = 59904000 Hz; PR6 2; 
    PR6 = 0x02;
    //TCKPS 1:1; T32 16 Bit; TON enabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T6CON = 0x8000;

    IFS2bits.T6IF = false;
    IEC2bits.T6IE = true;
}

void RAND_init_seed(void) {
    srand(_ns);
    _rand_init = true;
}

void RAND_stop(void) {
    T6CONbits.TON = 0;
    _ns = 0ULL;
    _rand_init = false;
}

int_fast64_t RAND(int_fast64_t min, int_fast64_t max) {
    if (!_rand_init)
        return 0ULL;
    return (int_fast64_t) (min + ((int_fast64_t) rand()) /
            (((int_fast64_t) RAND_MAX) / (max - min + 1LL) + 1LL));
}

inline int RAND_random(void) {
    if (!_rand_init)
        return 0;
    return rand();
}

void __attribute__((interrupt, no_auto_psv)) _T6Interrupt() {
    _ns += (TIME_now_us() * 1000ULL) + 50ULL;
    IFS2bits.T6IF = false;
}
