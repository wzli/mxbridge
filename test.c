#include "mxbridge.h"

#define STRUCT_MsgA(X) \
    X(int32_t, a, )    \
    X(int32_t, b, )
GEN_STRUCT(MsgA);

#define STRUCT_MsgB(X) \
    X(int32_t, a, )    \
    X(int32_t, b, )
GEN_STRUCT(MsgB);

#define BRIDGE_MESSAGES(X) \
    X(MsgA, 10)            \
    X(MsgB, 10)
GEN_BRIDGE_DECLARATION(BRIDGE_MESSAGES);
GEN_BRIDGE_IMPLEMENTATION(BRIDGE_MESSAGES);

int frame_handler(const void* frame, int len, void* context) {
    printf("got frame: len %d buf %p ctx %p\n", len, frame, context);
    bridge_receive_frame(frame, len, context);
    return 0;
}

int message_handler(const void* message, const void* src_ctx, void* dst_ctx) {
    char buf[100];
    MsgA_to_json(message, buf);
    printf("received message: %s src_ctx %p dst_ctx %p\n", buf, src_ctx, dst_ctx);
    return 0;
}

int main() {
    char buf[100];
    MsgA msg_a = {20, 40};
    MsgA_to_json(&msg_a, buf);
    printf("original message: %s\n", buf);

    message_subscribe(MsgA_subscribers, message_handler, NULL);
    BRIDGE_SUBSCRIBE_ENDPOINT(MsgA, frame_handler, NULL);

    message_publish(MsgA_subscribers, &msg_a, (void*) 1);

    return 0;
}
