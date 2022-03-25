#include "../include/tools.hpp"

namespace Random {
    int get_int(int min, int max, int seed) {
        if (min == max)
            return min;

        if (min > max)
            std::swap(min, max);

        return EffoRandom::get(min, max);
    }
}