#include "cingbuffer.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    RingBuffer* rb;
    rb = RingBuffeer_new(1024);

    RingBuffer_write(rb, "aa", 2);

    char* data = malloc(2 * sizeof(char));
     RingBuffer_read(rb, data, 2);
    printf("read: %s\n", data);
    RingBuffer_free(rb);
}
