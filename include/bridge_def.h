#pragma once
#include "mxbridge.h"

#define STRUCT_MsgA(X) \
    X(int32_t, a, )    \
    X(int32_t, b, )
GEN_STRUCT(MsgA)

#define STRUCT_MsgB(X) \
    X(float, x, )      \
    X(MsgA, y, )
GEN_STRUCT(MsgB)

#define BRIDGE_MESSAGES(X) \
    X(MsgA, 10)            \
    X(MsgB, 10)
GEN_BRIDGE_DECLARATION(BRIDGE_MESSAGES)
