#include "../include/renderer.hpp"


Renderer::Renderer() {
    TCODConsole::initRoot(80, 50, "my game", false);
}

Renderer::Renderer(uint w, uint h) {
    TCODConsole::initRoot(w, h, "my game", false);
    position_map.x = MAP_POSITION_X;
    position_map.y = MAP_POSITION_Y;
}

void Renderer::initConsole(uint w, uint h) {
    TCODConsole::initRoot(w, h, "my game", false);
    position_map.x = MAP_POSITION_X;
    position_map.y = MAP_POSITION_Y;
}

void Renderer::renderTile(Map& map, uint x, uint y, bool debug) {
    if (debug) {
        if (map.isWalkable(x, y))
            TCODConsole::root->putCharEx(x, y, '.', light_ground, default_bg);
        else
            TCODConsole::root->putCharEx(x, y, '#', light_wall, default_bg);
    }

    else {
        if(map.isInFov(x, y)) {   
            if (map.isWalkable(x, y))
                TCODConsole::root->putCharEx(x, y, '.', light_ground, default_bg);
            else
                TCODConsole::root->putCharEx(x, y, '#', light_wall, default_bg);
        }

        else if(map.isExplored(x, y)) {
            if (map.isWalkable(x, y))
                TCODConsole::root->putCharEx(x, y, '.', dark_ground, default_bg);
            else
                TCODConsole::root->putCharEx(x, y, '#', dark_wall, default_bg);

        }
    }
}

void Renderer::renderMap(Map& map, bool debug) {
    for(size_t y = 0; y < map.size.h; y++) {
        for (size_t x = 0; x < map.size.w; x++) {
            renderTile(map, x, y, debug);
        }
    }
}

void Renderer::renderEntities(Map& map, flecs::world& ecs_world) {
    // Draw the dead entities first (so they are under everything else)
    ecs_world.each([&map](flecs::entity e, const Position& p, const Renderable& r, const Dead& d) {
        if(map.isInFov(p.x, p.y)) {
            TCODConsole::root->putCharEx(p.x, p.y, r.glyph_dead, r.colour_dead, default_bg);
        }
          
    });

    // Draw the entities, except for the player
    ecs_world.each([&map](flecs::entity e, const Position& p, const Renderable& r, const Alive& a) {
        if(!e.has<Player>()) {
            if(map.isInFov(p.x, p.y)) {
                TCODConsole::root->putCharEx(p.x, p.y, r.glyph, r.colour, default_bg); 
            }
        }  
    });

    // TODO: maybe draw the non-living entities (furnitures, etc.) first, then the living ones?

    
    // Draw the player the last so it's always on the top of everything
    flecs::entity player = ecs_world.lookup("Player");
    const Position* p = player.get<Position>();
    const Renderable* r = player.get<Renderable>();
    TCODConsole::root->putCharEx(p->x, p->y, r->glyph, r->colour, default_bg);
}

void renderGUIs() {
    gui_messages.render(true);
    gui_stats.render(true);
    
    TCODConsole::blit(gui_messages.con, 0, 0, 
                      gui_messages.size.w, gui_messages.size.h, TCODConsole::root, 
                      gui_messages.pos.x, gui_messages.pos.y);

    TCODConsole::blit(gui_stats.con, 0, 0, 
                      gui_stats.size.w, gui_stats.size.h, TCODConsole::root, 
                      gui_stats.pos.x, gui_stats.pos.y);
}