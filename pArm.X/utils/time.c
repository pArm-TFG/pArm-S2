#include "time.h"
#include "types.h"
#include "../init.h"

static time_t _now_us = 0ULL;
static time_t _now_ms = 0ULL;

inline void TIME_updateMs(void) {
    _now_ms = (time_t) (_now_us / 1000ULL);
}

void TIME_init(void) {
    TIME_set_time(0ULL);
    TMR1_Initialize();
    TMR2_Initialize();
}

time_t TIME_now(void) {
    return _now_ms;
}

time_t TIME_now_us(void) {
    return _now_us;
}

inline void TIME_increment_us(void) {
    _now_us += 1ULL;
    TIME_updateMs();
}

void TIME_set_time(time_t value_us) {
    _now_us = value_us;
    TIME_updateMs();
}

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
