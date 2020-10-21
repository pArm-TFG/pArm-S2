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
 * File: barrier.h
 * Author: Javinator9889
 * Comments: Synchronization mechanism using a barrier and mutex.
 * Revision history: v1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BARRIER_H
#define	BARRIER_H

#include <stdint.h>
#include <stdbool.h>
#include "mutex.h"

#ifndef barrier_t
typedef struct {
    /**
     * Barrier internal counter - contains the amount of process that has
     * reached the barrier.
     */
    uint16_t counter;
    
    /**
     * Barrier internal value for knowing how many processes have to reach
     * the barrier before setting the {@code flag} to {@code true}.
     */
    uint16_t total;
    
    /**
     * Barrier flag indicating whether all expected processes have reached
     * the barrier. True when {@code counter} equals {@code total}.
     */
    bool flag;
    
    /**
     * Barrier internal lock for ensuring mutual exclusion during barrier
     * value set.
     */
    mut_t lock;
}
/**
 * Custom structure containing all the required information for handling
 * barriers. Must be used with specific methods defined in {@code barrier.h}.
 */
barrier_t;
#define barrier_t barrier_t
#endif

/**
 * Creates a new barrier ready to be used.
 * 
 * @param total how many processes will define the barrier. Sets the amount to
 * wait for.
 * @return a pointer to the new created barrier.
 */
barrier_t *BARRIER_create(uint16_t total);

/**
 * Method for indicating that a new process has reached the barrier.
 * 
 * @param barrier the reached barrier.
 */
void BARRIER_arrive(volatile barrier_t *barrier);

/**
 * Update the amount of process that the barrier will have.
 * 
 * @param barrier the barrier to be updated.
 * @param p the new amount of processes.
 */
void BARRIER_set_total(volatile barrier_t *barrier, uint16_t p);

/**
 * Safely clears the barrier structure.
 * 
 * @param barrier the barrier to be cleared.
 */
void BARRIER_clr(volatile barrier_t *barrier);

/**
 * Forcly updates the barrier setting its {@code flag} to true.
 * 
 * @param barrier the barrier to be updated.
 */
void BARRIER_set_done(volatile barrier_t *barrier);

/**
 * Checks if all the processes have reached the barrier.
 * 
 * @param barrier the barrier in which to check.
 * @return the flag value.
 */
bool BARRIER_all_done(volatile barrier_t *barrier);

#endif	/* BARRIER_H */

