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
 * File: mutex.h
 * Author: Javinator9889
 * Comments: Ensures mutual exclusion when accessing a critical section.
 * Revision history: v1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MUTEX_H
#define	MUTEX_H

#include <stdbool.h>
#define LOCKED      1
#define UNLOCKED    0

#ifndef mut_t
typedef volatile unsigned char mut_t;
#define mut_t mut_t
#endif

/**
 * With the given lock, enter the mutual exclusion zone.
 * 
 * @param lock - the mutex lock.
 */
void mutex_acquire(mut_t *lock);

/**
 * With the given lock, exit the mutual exclusion zone.
 * 
 * @param lock - the mutex lock.
 */
void mutex_release(mut_t *lock);

#endif	/* MUTEX_H */

