#include <stdio.h>
#include "pubsub.h"

int message_handler_1(const void* message, void* context) {
    const int* msg = message;
    printf("got msg %d ctx %p\n", *msg, context);
    return 1;
};

MessageSubscriber subscribers[10];
int main() {
    message_subscribe(subscribers, message_handler_1, (void*) 1);
    message_subscribe(subscribers, message_handler_1, (void*) 2);
    message_subscribe(subscribers, message_handler_1, (void*) 3);
    int msg = 5;
    message_publish(subscribers, &msg);
    message_unsubscribe(subscribers, message_handler_1, (void*) 2);
    msg = 55;
    message_publish(subscribers, &msg);
    return 0;
}
