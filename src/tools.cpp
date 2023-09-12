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

namespace Tools {
    double dst(int x1, int y1, int x2, int y2) {
        if (pow(x2 - x1, 2) + pow(y2 - y1, 2) != 0)
            return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);

        return 0;
    }

    template<typename T>
    double dst(mVec2<T> p1, mVec2<T> p2) {
        if (pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) != 0)
            return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) * 1.0);

        return 0;
    }
}