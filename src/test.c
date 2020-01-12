#include "bridge_def.h"

static char buf[100];

int frame_handler(const void* frame, int len, void* context) {
    printf("loopback frame: id %d len %d buf %p ctx %p\n", *(uint16_t*) frame, len, frame, context);
    bridge_receive_frame(frame, len, context);
    return 0;
}

int message_a_handler(const void* message, const void* src_ctx, void* dst_ctx) {
    MsgA_to_json(message, buf);
    printf("received message a: %s src_ctx %p dst_ctx %p\n", buf, src_ctx, dst_ctx);
    return 0;
}

int message_b_handler(const void* message, const void* src_ctx, void* dst_ctx) {
    MsgB_to_json(message, buf);
    printf("received message b: %s src_ctx %p dst_ctx %p\n", buf, src_ctx, dst_ctx);
    return 0;
}

int main() {
    message_subscribe(MsgA_subscribers, message_a_handler, NULL);
    message_subscribe(MsgB_subscribers, message_b_handler, NULL);
    BRIDGE_SUBSCRIBE_ENDPOINT(MsgA, frame_handler, NULL);
    BRIDGE_SUBSCRIBE_ENDPOINT(MsgB, frame_handler, NULL);

    MsgA msg_a = {20, 40};
    MsgA_to_json(&msg_a, buf);
    printf("send original message a: %s\n", buf);
    message_publish(MsgA_subscribers, &msg_a, (void*) 1);

    MsgB msg_b = {-2.5, {34, -22}};
    MsgB_to_json(&msg_b, buf);
    printf("\nsend original message b: %s\n", buf);
    message_publish(MsgB_subscribers, &msg_b, (void*) 1);

    return 0;
}
