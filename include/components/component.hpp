#pragma once

#include "../libtcod_no_warnings.hpp"
#include "BearLibTerminal.h"

#include <vector>
#include <string>

#include "../../extlib/flecs/flecs.h"

#include "../types.hpp"
#include "../colours.hpp"

struct Position {
    uint x, y;

    Position(uint x=0, uint y=0): x(x), y(y) {};
};

struct Velocity {
    int dx, dy;

    Velocity(uint dx=0, uint dy=0): dx(dx), dy(dy) {};
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
    };

    Stats(const std::string& name, int hp): name(name), hp(hp) {
        max_hp = hp;
        if(hp < 0)
            is_alive = false;
    };

    Stats(const std::string& name, int hp, int max_hp): name(name), hp(hp), max_hp(max_hp) {};
};

struct Renderable {
    char glyph, glyph_dead;
    color_t colour, colour_dead;
    
    Renderable() {};

    Renderable(char glyph, char glyph_dead='%', color_t colour=red, color_t colour_dead=dark_red): 
        glyph(glyph), glyph_dead(glyph_dead), colour(colour), colour_dead(colour_dead) {};
};