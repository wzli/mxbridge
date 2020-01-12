# Minimal X-Macro Bridge
A minimal framework to provide publish/subscribe facilities and message bridging across embedded devices

## Use Case
- Originally aimed to eliminate boilder plate 
- Consolidate various callbacks into a publish subscribe framework

## Features

## Example Usage
1. Create header files defining message types and bridge messages, see [bridge_def.h](./include/bridge_def.h)
2. In any one compilation unit, call the GEN_BRIDGE_IMPLEMENTATION() macro, see [bridge_impl.c](./src/bridge_impl.c)
3. Use BRIDGE_SUBSCRIBE_ENDPOINT() to subscribe FrameEndpoints (where to send serialized outbound message frames)
4. Use message_subscribe() to subscribe MessageHandlers (where to receive deserialized inbound messages)
5. Call bridge_receive_frame() when a message frame is received, to deserialize and publish message to subscribed handlers.
6. Call message_publish(MsgType_subscribers, &msg, src_ctx) to publish message to all subscribed handlers.
7. If a FrameEndpoint is subscribed, it will deserialize the message and trigger its defined frame_handler.
8. See [loop back test example](./src/test.c).

## Demo
First install GCC, then to test:
```bash
make
./build/test
```
Output:
```json
send original message a: {"a":20, "b":40}
received message a: {"a":20, "b":40} src_ctx 0x1 dst_ctx (nil)
loopback frame: id 0 len 10 buf 0x7ffd7e1432ee ctx (nil)
received message a: {"a":20, "b":40} src_ctx (nil) dst_ctx (nil)

send original message b: {"x":-2.5, "y":{"a":34, "b":-22}}
received message b: {"x":-2.5, "y":{"a":34, "b":-22}} src_ctx 0x1 dst_ctx (nil)
loopback frame: id 1 len 14 buf 0x7ffd7e1432ea ctx (nil)
received message b: {"x":-2.5, "y":{"a":34, "b":-22}} src_ctx (nil) dst_ctx (nil)
```
