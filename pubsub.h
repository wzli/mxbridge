#pragma once

typedef int (*MessageHandler)(const void* message, void* context);

typedef struct {
    MessageHandler message_handler;
    void* context;
} MessageSubscriber;

#define LEN(SUB) *(int*) (SUB)

static inline int message_subscribe(
        MessageSubscriber* subscribers, MessageHandler handler, void* context) {
    subscribers[++(LEN(subscribers))] = (MessageSubscriber){handler, context};
    return LEN(subscribers);
}

static inline int message_unsubscribe(
        MessageSubscriber* subscribers, MessageHandler handler, void* context) {
    for (int i = 1; i <= LEN(subscribers); ++i) {
        if (subscribers[i].message_handler == handler && subscribers[i].context == context) {
            subscribers[i] = subscribers[LEN(subscribers)];
            return --LEN(subscribers);
        }
    }
    return -1;
}

static inline int message_publish(MessageSubscriber* subscribers, const void* message) {
    int aggregate = 0;
    for (int i = 1; i <= LEN(subscribers); ++i) {
        aggregate += subscribers[i].message_handler(message, subscribers[i].context);
    }
    return aggregate;
}

#undef LEN
