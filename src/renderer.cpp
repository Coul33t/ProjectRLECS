#include "../include/renderer.hpp"


Renderer::Renderer() {
    TCODConsole::initRoot(80, 50, "my game", false);
}

Renderer::Renderer(uint w, uint h) {
    TCODConsole::initRoot(w, h, "my game", false);
}

void Renderer::initConsole(uint w, uint h) {
    TCODConsole::initRoot(w, h, "my game", false);
}

void Renderer::renderTile(Map* map, uint x, uint y, bool debug) {
    if (debug) {
        if (map->isWalkable(x, y))
            TCODConsole::root->putCharEx(x, y, '.', light_ground, default_bg);
        else
            TCODConsole::root->putCharEx(x, y, '#', light_wall, default_bg);
    }

    else {
        if(map->isInFov(x, y)) {   
            if (map->isWalkable(x, y))
                TCODConsole::root->putCharEx(x, y, '.', light_ground, default_bg);
            else
                TCODConsole::root->putCharEx(x, y, '#', light_wall, default_bg);
        }

        else if(map->isExplored(x, y)) {
            if (map->isWalkable(x, y))
                TCODConsole::root->putCharEx(x, y, '.', dark_ground, default_bg);
            else
                TCODConsole::root->putCharEx(x, y, '#', dark_wall, default_bg);

        }
    }
}

void Renderer::renderMap(Map* map, bool debug) {
    for(size_t y = 0; y < map->size.h; y++) {
        for (size_t x = 0; x < map->size.w; x++) {
            renderTile(map, x, y, debug);
        }
    }
}

void Renderer::renderEntities(Map* map, flecs::world& ecs_world) {
    ecs_world.each([map](flecs::entity e, const Position& p, const Renderable& r) {
        if(e.has<Player>())
            TCODConsole::root->putCharEx(p.x, p.y, r.glyph, green, default_bg);
        else
            if(map->isInFov(p.x, p.y))
                TCODConsole::root->putCharEx(p.x, p.y, r.glyph, red, default_bg);
        
    });
}