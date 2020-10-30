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
