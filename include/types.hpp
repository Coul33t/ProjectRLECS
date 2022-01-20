#pragma once

#include <cstdint>

typedef unsigned int uint;

template<typename T>
struct mVec2
{
    union {
        struct { T x, y;};
        struct { T w, h;};
    };
};