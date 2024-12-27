# A Single File Lock-Free Ring Buffer Implementation In C
cingbuffer is a "copy-and-paste" ring buffer for c programs, it is lock free, and utilizes atomics for optimal performance.

The code is intentionally "minimalistic" and small, at under 100 LOC.
```c
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
```
