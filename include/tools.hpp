#pragma once

#include <iostream>
#include <algorithm>
#include <stdlib.h>

#include "../extlib/random/random.hpp"

namespace Random {
    using EffoRandom = effolkronium::random_static;
    int get_int(int lower, int higher, int seed=7);
}