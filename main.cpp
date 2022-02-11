#include <iostream>

#include "include/libtcod_no_warnings.hpp"

#include "include/engine.hpp"

int main() {
    std::cout << "Hello world!" << std::endl;
    
    Engine engine;
    engine.run();

    return 0;
}