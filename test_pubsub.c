#include <stdio.h>
#include "pubsub.h"

int message_handler(const void* message, const void* src_ctx, void* dst_ctx) {
    const int* msg = message;
    printf("got msg %d src_ctx %p dst_ctx %p\n", *msg, src_ctx, dst_ctx);
    return 1;
};

MessageSubscriber subscribers[10];
int main() {
    message_subscribe(subscribers, message_handler, (void*) 1);
    message_subscribe(subscribers, message_handler, (void*) 2);
    message_subscribe(subscribers, message_handler, (void*) 3);
    int msg = 5;
    message_publish(subscribers, &msg, (void*) 0xFF);
    message_unsubscribe(subscribers, message_handler, (void*) 2);
    msg = 55;
    message_publish(subscribers, &msg, (void*) 0xF);
    return 0;
}
