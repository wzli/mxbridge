#pragma once
#include "mxgen.h"
#include "pubsub.h"

typedef enum {
    BRIDGE_SUCCESS = 0,
    BRIDGE_SAME_SRC_DST = -1,
    BRIDGE_INVALID_FRAME_LENGTH = -2,
    BRIDGE_UNKNOWN_MESSAGE_ID = -3,
    BRIDGE_MESSAGE_LENGTH_MISMATCH = -4,
} BridgeError;

typedef int (*FrameHandler)(const void* frame, int len, void* context);

typedef struct {
    FrameHandler frame_handler;
    void* context;
} FrameEndpoint;

int bridge_receive_frame(const void* frame, int len, void* context);

#define MESSAGE_ID_SIZE 2

// consolidated declaration generation
#define GEN_BRIDGE_DECLARATION(BRIDGE_MESSAGES) \
    GEN_BRIDGE_MESSAGE_IDS(BRIDGE_MESSAGES)     \
    BRIDGE_MESSAGES(GEN_BRIDGE_SUBSCRIBER_DECL) \
    BRIDGE_MESSAGES(GEN_BRIDGE_SEND_DECL)

// consolidated implementation generation
#define GEN_BRIDGE_IMPLEMENTATION(BRIDGE_MESSAGES) \
    BRIDGE_MESSAGES(GEN_BRIDGE_SUBSCRIBER_IMPL)    \
    BRIDGE_MESSAGES(GEN_BRIDGE_SEND_IMPL)          \
    GEN_BRIDGE_RECEIVE_IMPL(BRIDGE_MESSAGES)

// subscribe frame endpoint to message broadcasts
#define BRIDGE_SUBSCRIBE_ENDPOINT(TYPE, HANDLER, CTX)                          \
    message_subscribe(TYPE##_subscribers, (MessageHandler) TYPE##_bridge_send, \
            &(FrameEndpoint){HANDLER, CTX});

// macros below are used internally

// generate message ids
#define GEN_BRIDGE_MESSAGE_IDS_FIELD(TYPE, MAX_SUBS) TYPE##_ID,
#define GEN_BRIDGE_MESSAGE_IDS(BRIDGE_MESSAGES) \
    typedef enum { BRIDGE_MESSAGES(GEN_BRIDGE_MESSAGE_IDS_FIELD) N_MESSAGE_IDS } MessageId;

// generate declaration of message subscriber queues
#define GEN_BRIDGE_SUBSCRIBER_DECL(TYPE, MAX_SUBS) \
    extern MessageSubscriber TYPE##_subscribers[(MAX_SUBS) + 1];

// generate declaration of send functions
#define GEN_BRIDGE_SEND_DECL(TYPE, MAX_SUBS) \
    int TYPE##_bridge_send(const TYPE* message, const void* src_ctx, FrameEndpoint* frame_endpoint);

// generate definition of message subscriber queues
#define GEN_BRIDGE_SUBSCRIBER_IMPL(TYPE, MAX_SUBS) \
    MessageSubscriber TYPE##_subscribers[(MAX_SUBS) + 1];

// generate implementation of send functions
#define GEN_BRIDGE_SEND_IMPL(TYPE, MAX_SUBS)                                           \
    int TYPE##_bridge_send(                                                            \
            const TYPE* message, const void* src_ctx, FrameEndpoint* frame_endpoint) { \
        FrameEndpoint* dst = frame_endpoint;                                           \
        if (src_ctx == dst->context) {                                                 \
            return BRIDGE_SAME_SRC_DST;                                                \
        }                                                                              \
        uint8_t buf[sizeof(TYPE) + MESSAGE_ID_SIZE];                                   \
        *(uint16_t*) buf = TYPE##_ID;                                                  \
        int len = TYPE##_serialize(message, buf + MESSAGE_ID_SIZE);                    \
        return dst->frame_handler(buf, len + MESSAGE_ID_SIZE, dst->context);           \
    }

// generate implementation of receive function
#define GEN_BRIDGE_RECEIVE_IMPL_FIELD(TYPE, MAX_SUBS)              \
    case TYPE##_ID: {                                              \
        TYPE msg;                                                  \
        len -= TYPE##_deserialize(&msg, frame + MESSAGE_ID_SIZE);  \
        if (len != MESSAGE_ID_SIZE) {                              \
            return BRIDGE_MESSAGE_LENGTH_MISMATCH;                 \
        }                                                          \
        return message_publish(TYPE##_subscribers, &msg, context); \
    }

#define GEN_BRIDGE_RECEIVE_IMPL(BRIDGE_MESSAGES)                          \
    int bridge_receive_frame(const void* frame, int len, void* context) { \
        if (len < MESSAGE_ID_SIZE) {                                      \
            return BRIDGE_INVALID_FRAME_LENGTH;                           \
        }                                                                 \
        switch (*(uint16_t*) frame) {                                     \
            BRIDGE_MESSAGES(GEN_BRIDGE_RECEIVE_IMPL_FIELD)                \
            default:                                                      \
                return BRIDGE_UNKNOWN_MESSAGE_ID;                         \
        }                                                                 \
        return BRIDGE_SUCCESS;                                            \
    }
