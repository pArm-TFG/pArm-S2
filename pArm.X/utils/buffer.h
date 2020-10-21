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
 * File: buffer.h
 * Author: Javinator9889
 * Comments: Buffer handler and shortcuts for handling buffer_t
 * Revision history: v1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BUFFER_H
#define	BUFFER_H

#include <stdlib.h>
#include "types.h"

/**
 * Creates a buffer of the given size, and initializes it.
 * 
 * @param size - the buffer size.
 * @return a pointer to the new created buffer.
 */
buffer_t *BUFFER_create(size_t size);

/**
 * Updates the given buffer size. If the new size is lower than the one
 * that was before, any stored value is destroyed.
 * 
 * @param buffer - the buffer whose size will be updated.
 * @param size - the new buffer size. Set to '0' to destroy the buffer.
 */
void BUFFER_update_size(buffer_t *buffer, size_t size);

/**
 * Safely frees the buffer data from the memory. Caution: this method
 * does not free the space used by the buffer_t itself. Use {@link free(void*)}
 * instead.
 * 
 * @param buffer - the buffer to be cleared.
 */
void BUFFER_free(buffer_t *buffer);

#endif	/* BUFFER_H */

