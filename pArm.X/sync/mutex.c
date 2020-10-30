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

/* 
 * File: mutex.c
 * Author: Javinator9889
 * Comments: Ensures mutual exclusion when accessing a critical section.
 * Revision history: v1.0
 */
#include <stdbool.h>
#include <xc.h>
#include "mutex.h"

static inline void mutex_switch(bool state) {
    IEC0bits.T3IE = state;
    IEC1bits.T4IE = state;
    IEC1bits.T5IE = state;
}

static unsigned char test_and_set(mut_t *lock_ptr) {
    mut_t old_value;
    // Disable interrupts
    mutex_switch(false);
    old_value = *lock_ptr;
    *lock_ptr = LOCKED;
    // Enable interrupts
    mutex_switch(true);
    return old_value;
}

void mutex_acquire(mut_t *lock) {
    while (test_and_set(lock) == LOCKED);
}

void mutex_release(mut_t *lock) {
    if (test_and_set(lock) == LOCKED)
        *lock = UNLOCKED;
}
