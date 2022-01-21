#pragma once

#include <vector>
#include <string>

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
    flecs::entity target;

    Melee(uint dmg=1, bool attack_on_move=true): 
        dmg(dmg), attack_on_move(attack_on_move) {};
};

struct Stats {
    int hp, max_hp;
    bool is_alive = true;
    std::string name;

    Stats() {
        hp = 10;
        max_hp = 10;
        name = "NO NAME ASSIGNED";
    };

    Stats(int hp): hp(hp) {
        if(hp < 0)
            is_alive = false;
        max_hp = hp;
        name = "NO NAMED ASSIGNED";
    };

    Stats(int hp, bool is_alive): 
        hp(hp), is_alive(is_alive) {
            max_hp = hp;
            name = "NO NAMED ASSIGNED"; 
        };

    Stats(const std::string& name): name(name) {
        hp = 10;
        max_hp = 10;
    }

    Stats(const std::string& name, int hp): name(name), hp(hp) {
        max_hp = hp;
        if(hp < 0)
            is_alive = false;
    }

    Stats(const std::string& name, int hp, int max_hp): name(name), hp(hp), max_hp(max_hp) {};
};

struct Renderable {
    char glyph;

    Renderable(char glyph='X'): glyph(glyph) {};
};