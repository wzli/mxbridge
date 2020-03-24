#pragma once
#include "mxbridge.h"

#define TYPEDEF_MsgA(X, _) \
    X(int32_t, a, )        \
    X(int32_t, b, )
MXGEN(struct, MsgA)

#define TYPEDEF_MsgB(X, _) \
    X(float, x, )          \
    X(MsgA, y, )
MXGEN(struct, MsgB)

#define BRIDGE_MESSAGES(X) \
    X(MsgA, 10)            \
    X(MsgB, 10)
GEN_BRIDGE_DECLARATION(BRIDGE_MESSAGES)
