#include "cingbuffer.h"



struct Sender {
    RingBuffer *rb;
};

struct Receiver {
    RingBuffer *rb;
};

struct Spsc {
    struct Sender sender;
    struct Receiver receiver;
};

struct Spsc Spsc_new() {
    struct Spsc o;
    RingBuffer* rb;
    rb = RingBuffeer_new(1024);


    struct Sender sender;
    struct Receiver receiver;
    sender.rb = rb;
    receiver.rb = rb;

    o.sender = sender;
    o.receiver = receiver;
    
    return o;
}

