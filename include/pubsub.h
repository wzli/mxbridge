#pragma once

typedef int (*MessageHandler)(const void* message, const void* src_ctx, void* dst_ctx);

typedef struct {
    MessageHandler message_handler;
    void* context;
} MessageSubscriber;

#define LEN(SUB) *(int*) (SUB)

static inline int message_subscribe(
        MessageSubscriber* subscribers, MessageHandler handler, void* context) {
    subscribers[++LEN(subscribers)] = (MessageSubscriber){handler, context};
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

static inline int message_publish(
        MessageSubscriber* subscribers, const void* message, const void* src_ctx) {
    int aggregate = 0;
    for (int i = 1; i <= LEN(subscribers); ++i) {
        aggregate += subscribers[i].message_handler(message, src_ctx, subscribers[i].context);
    }
    return aggregate;
}

#undef LEN
