/* 
 * File: buffer.c
 * Author: Javinator9889
 * Comments: Buffer handler and shortcuts for handling buffer_t
 * Revision history: v1.0
 */
#include <stdlib.h>
#include "buffer.h"
#include "types.h"

buffer_t *BUFFER_create(size_t size) {
    buffer_t *ptr = (buffer_t *) malloc(sizeof(buffer_t));
    ptr->buffer = NULL;
    BUFFER_update_size(ptr, size);
    return ptr;
}

void BUFFER_update_size(buffer_t *buffer, size_t size) {
    buffer->size = size;
    buffer->bsize = (size * sizeof(char));
    realloc(buffer->buffer, buffer->bsize);
}

inline void BUFFER_free(buffer_t *buffer) {
    return BUFFER_update_size(buffer, 0U);
}
