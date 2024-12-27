#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "cingbuffer.h"


RingBuffer* RingBuffeer_new(size_t size) {
    if (size == 0 || (size & (size - 1)) != 0) {
        return NULL;
    }

    RingBuffer* self = malloc(sizeof(RingBuffer) + size);
    if (self == NULL) {
        return NULL;
    }

    self->size = size;
    atomic_init(&self->wix, 0);
    atomic_init(&self->rix, 0);
    self->mask = size - 1;

    return self;
}

void RingBuffer_free(RingBuffer* self) {
    free(self);
}

size_t RingBuffer_write(RingBuffer* self, const void* data, size_t len) {
    size_t wix = atomic_load_explicit(&self->wix, memory_order_relaxed);
    size_t rix = atomic_load_explicit(&self->rix, memory_order_acquire);
    
    size_t available = self->size - (wix - rix);
    if (len > available) {
        len = available;
    }
    
    if (len == 0) {
        return 0;
    }

    size_t write_pos = wix & self->mask;
    size_t write_size = self->size - write_pos;
    if (write_size > len) {
        write_size = len;
    }

    memcpy(&self->buffer[write_pos], data, write_size);

    if (write_size < len) {
        memcpy(self->buffer, (unsigned char*)data + write_size, len - write_size);
    }

    atomic_store_explicit(&self->wix, wix + len, memory_order_release);
    
    return len;
}

size_t RingBuffer_read(RingBuffer* self, void* data, size_t len) {
    size_t rix = atomic_load_explicit(&self->rix, memory_order_relaxed);
    size_t wix = atomic_load_explicit(&self->wix, memory_order_acquire);
    
    size_t available = wix - rix;
    if (len > available) {
        len = available;
    }
    
    if (len == 0) {
        return 0;
    }

    size_t read_pos = rix & self->mask;
    size_t read_size = self->size - read_pos;
    if (read_size > len) {
        read_size = len;
    }

    memcpy(data, &self->buffer[read_pos], read_size);

    if (read_size < len) {
        memcpy((unsigned char*)data + read_size, self->buffer, len - read_size);
    }

    atomic_store_explicit(&self->rix, rix + len, memory_order_release);
    
    return len;
}

size_t RingBuffer_available_read(RingBuffer* self) {
    return atomic_load_explicit(&self->wix, memory_order_acquire) - 
           atomic_load_explicit(&self->rix, memory_order_relaxed);
}

size_t RingBuffer_available_write(RingBuffer* self) {
    return self->size - (atomic_load_explicit(&self->wix, memory_order_relaxed) - 
                      atomic_load_explicit(&self->rix, memory_order_acquire));
}
