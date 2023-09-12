#pragma once

#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <cmath>

#include "../extlib/random/random.hpp"

#include "types.hpp"

namespace Random {
    using EffoRandom = effolkronium::random_static;
    int get_int(int lower, int higher, int seed=7);
}

namespace Tools {
    double dst(int x1, int y1, int x2, int y2);
    template<typename T>
    double dst(mVec2<T> p1, mVec2<T> p2);
}