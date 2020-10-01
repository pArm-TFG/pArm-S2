/* 
 * File: barrier.c
 * Author: Javinator9889
 * Comments: Synchronization mechanism using a barrier and mutex.
 * Revision history: v1.0
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mutex.h"
#include "barrier.h"

barrier_t *BARRIER_create(uint16_t total) {
    barrier_t *barrier = (barrier_t *) malloc(sizeof (barrier_t));
    barrier->counter = 0;
    barrier->total = total;
    barrier->flag = false;
    barrier->lock = UNLOCKED;
    return barrier;
}

void BARRIER_arrive(barrier_t *barrier) {
    mutex_acquire(&barrier->lock);
    if (++barrier->counter == barrier->total)
        barrier->flag = true;
    mutex_release(&barrier->lock);
}

void BARRIER_set_total(barrier_t *barrier, uint16_t p) {
    mutex_acquire(&barrier->lock);
    barrier->total = p;
    mutex_release(&barrier->lock);
}

void BARRIER_clr(barrier_t *barrier) {
    barrier->counter = 0;
    barrier->flag = false;
    barrier->lock = UNLOCKED;
}

bool BARRIER_all_done(barrier_t *barrier) {
    mutex_acquire(&barrier->lock);
    bool flag = barrier->flag;
    mutex_release(&barrier->lock);
    return flag;
}
