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
 * File: buffer.c
 * Author: Javinator9889
 * Comments: Buffer handler and shortcuts for handling buffer_t
 * Revision history: v1.0
 */
#include <stdlib.h>
#ifdef DEBUG_ENABLED
#include "../printf/io.h"
#endif
#include "buffer.h"
#include "types.h"

buffer_t *BUFFER_create(size_t size) {
    buffer_t *ptr = (buffer_t *) malloc(sizeof (buffer_t));
    if (ptr == NULL) {
#ifdef DEBUG_ENABLED
        printf("[ERROR]\tFailed to allocate struct buffer_t\n");
#endif
        return NULL;
    }
    ptr->buffer = NULL;
    BUFFER_update_size(ptr, size);
    return ptr;
}

void BUFFER_update_size(buffer_t *buffer, size_t size) {
    if (buffer->size != size) {
        buffer->size = size;
        buffer->bsize = (size * sizeof (char));
        buffer->buffer = (char *) realloc(buffer->buffer, buffer->bsize);
#ifdef DEBUG_ENABLED
        if (buffer->buffer == NULL && size != 0U) {
            printf("[ERROR]\tFailed to re-allocate %dB for new buffer!\n", buffer->bsize);
        }
#endif
    }
}

inline void BUFFER_free(buffer_t *buffer) {
    return BUFFER_update_size(buffer, 0U);
}
