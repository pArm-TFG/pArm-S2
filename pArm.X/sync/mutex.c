/* 
 * File: mutex.c
 * Author: Javinator9889
 * Comments: Ensures mutual exclusion when accessing a critical section.
 * Revision history: v1.0
 */
#include <stdbool.h>
#include <xc.h>
#include "mutex.h"

static unsigned char test_and_set(mut_t *lock_ptr) {
    mut_t old_value;
    // Disable interrupts
    __builtin_disable_interrupts();
    old_value = *lock_ptr;
    *lock_ptr = LOCKED;
    // Enable interrupts
    __builtin_enable_interrupts();
    return old_value;
}

void mutex_acquire(mut_t *lock) {
    while (test_and_set(lock) == LOCKED);
}

void mutex_release(mut_t *lock) {
    if (test_and_set(lock) == LOCKED)
        *lock = UNLOCKED;
}
