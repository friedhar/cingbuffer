#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t size;                 
    atomic_size_t wix;           
    atomic_size_t rix;           
    size_t mask;                 
    unsigned char buffer[];      
} RingBuffer;


RingBuffer* RingBuffeer_new(size_t size);
void RingBuffer_free(RingBuffer* self);
size_t RingBuffer_write(RingBuffer* self, const void* data, size_t len);
size_t RingBuffer_read(RingBuffer* self, void* data, size_t len);
size_t RingBuffer_available_read(RingBuffer* self);
size_t RingBuffer_available_write(RingBuffer* self);
