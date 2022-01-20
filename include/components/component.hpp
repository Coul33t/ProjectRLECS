#pragma once

#include <vector>

#include "../../extlib/flecs/flecs.h"

#include "../types.hpp"

struct Position {
    uint x, y;
    
    Position() {
        x = 0;
        y = 0;
    };

    Position(uint x, uint y): x(x), y(y) {};
};

struct Velocity {
    int dx, dy;

    Velocity() {
        dx = 0;
        dy = 0;
    }

    Velocity(uint dx, uint dy): dx(dx), dy(dy) {};
};

struct Vision {
    uint fov_radius;

    Vision(uint fov_radius=5): 
        fov_radius(fov_radius) {};
};

struct Melee {
    uint dmg;
    bool attack_on_move = true;
    Melee(uint dmg=1, bool attack_on_move=true): 
        dmg(dmg), attack_on_move(attack_on_move) {};
};

struct Stats {
    int hp;
    bool is_alive = true;

    Stats() {
        hp = 10;
        is_alive = true;
    };

    Stats(int hp): hp(hp) {
        if(hp < 0)
            is_alive = false;
    };

    Stats(int hp, bool is_alive): 
        hp(hp), is_alive(is_alive) {};
};

struct Renderable {
    char glyph;

    Renderable(char glyph='X'): glyph(glyph) {};
};