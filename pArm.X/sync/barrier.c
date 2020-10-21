/*
 * Copyright (C) 2020 - present | pArm-S2 by Javinator9889
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
    barrier_t *created_barrier = (barrier_t *) malloc(sizeof(barrier_t));
    BARRIER_clr(created_barrier);
    created_barrier->total = total;
    return created_barrier;
}

void BARRIER_arrive(volatile barrier_t *barrier) {
    mutex_acquire(&barrier->lock);
    if (++barrier->counter >= barrier->total)
        barrier->flag = true;
    mutex_release(&barrier->lock);
}

void BARRIER_set_total(volatile barrier_t *barrier, uint16_t p) {
    mutex_acquire(&barrier->lock);
    barrier->total = p;
    mutex_release(&barrier->lock);
}

void BARRIER_clr(volatile barrier_t *barrier) {
    barrier->counter = 0;
    barrier->flag = false;
    barrier->lock = UNLOCKED;
}

void BARRIER_set_done(volatile barrier_t *barrier) {
    mutex_acquire(&barrier->lock);
    barrier->flag = true;
    mutex_release(&barrier->lock);
}

bool BARRIER_all_done(volatile barrier_t *barrier) {
    return barrier->flag;
}
