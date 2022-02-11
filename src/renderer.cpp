#include "../include/renderer.hpp"


Renderer::Renderer() {
    initConsole(80, 50);

    //TCODConsole::initRoot(80, 50, "my game", false);
    gui_stats.setGuiAttributes("Stats_Gui", STATS_POSITION_X, STATS_POSITION_Y, STATS_SIZE_W, STATS_SIZE_H);
    gui_messages.setGuiAttributes("Messages_Gui", MESSAGES_POSITION_X, MESSAGES_POSITION_Y, MESSAGES_SIZE_W, MESSAGES_SIZE_H);
}

Renderer::Renderer(uint w, uint h) {
    initConsole(w, h);
}

Renderer::~Renderer() {
    terminal_close();
}

void Renderer::initConsole(uint w, uint h) {
    terminal_open();
    std::string size = "window: size=" + std::to_string(w) + "x" + std::to_string(h);
    terminal_set(size.c_str());
    //terminal_set("title='Project RLECS'");
    terminal_set("font: default");
    terminal_set("input: filter={keyboard}");
    terminal_color("white");
}

void Renderer::setConsole(uint w, uint h) {
    std::string size = "window: size=" + std::to_string(w) + "x" + std::to_string(h);
    terminal_set(size.c_str());
}

void Renderer::renderChar(uint x, uint y, char chr, color_t fg, color_t bg) {
    terminal_bkcolor(bg);
    terminal_color(fg);
    terminal_put_ext(x, y, 0, 0, chr);
}

void Renderer::renderString(uint x, uint y, std::string str, color_t fg, color_t bg) {
    terminal_bkcolor(bg);
    terminal_color(fg);
    terminal_print(x, y, str.c_str());
}

void Renderer::renderTile(Map& map, uint x, uint y, bool debug) {
    if (debug) {
        if (map.isWalkable(x, y))
            renderChar(x, y, '.', light_ground, default_bg);
            //TCODConsole::root->putCharEx(x, y, '.', light_ground, default_bg);
        else
            renderChar(x, y, '#', light_wall, default_bg);
            //TCODConsole::root->putCharEx(x, y, '#', light_wall, default_bg);
    }

    else {
        if(map.isInFov(x, y)) {   
            if (map.isWalkable(x, y))
                renderChar(x, y, '.', light_ground, default_bg);
                //TCODConsole::root->putCharEx(x, y, '.', light_ground, default_bg);
            else
                renderChar(x, y, '#', light_wall, default_bg);
                //TCODConsole::root->putCharEx(x, y, '#', light_wall, default_bg);
        }

        else if(map.isExplored(x, y)) {
            if (map.isWalkable(x, y))
                renderChar(x, y, '.', dark_ground, default_bg);
                //TCODConsole::root->putCharEx(x, y, '.', dark_ground, default_bg);
            else
                renderChar(x, y, '.', dark_wall, default_bg);
                //TCODConsole::root->putCharEx(x, y, '#', dark_wall, default_bg);

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
    ecs_world.each([&map, this](flecs::entity e, const Position& p, const Renderable& r, const Dead& d) {
        if(map.isInFov(p.x, p.y)) {
            renderChar(p.x, p.y, r.glyph_dead, r.colour_dead, default_bg);
            //TCODConsole::root->putCharEx(p.x, p.y, r.glyph_dead, r.colour_dead, default_bg);
        }
          
    });

    // Draw the entities, except for the player
    ecs_world.each([&map, this](flecs::entity e, const Position& p, const Renderable& r, const Alive& a) {
        if(!e.has<Player>()) {
            if(map.isInFov(p.x, p.y)) {
                renderChar(p.x, p.y, r.glyph, r.colour, default_bg);
                //TCODConsole::root->putCharEx(p.x, p.y, r.glyph, r.colour, default_bg); 
            }
        }  
    });

    // TODO: maybe draw the non-living entities (furnitures, etc.) first, then the living ones?

    
    // Draw the player the last so it's always on the top of everything
    flecs::entity player = ecs_world.lookup("Player");
    const Position* p = player.get<Position>();
    const Renderable* r = player.get<Renderable>();
    renderChar(p->x, p->y, r->glyph, r->colour, default_bg);
    //TCODConsole::root->putCharEx(p->x, p->y, r->glyph, r->colour, default_bg);
}

void Renderer::renderStats() {
    // Render the bars
    for (auto it_bar = gui_stats.bars.begin(); it_bar != gui_stats.bars.end(); it_bar++) {
        std::string bg_bar = ((*it_bar).width, " ");
        renderString((*it_bar).x + gui_stats.pos.x, (*it_bar).y + gui_stats.pos.y, bg_bar, (*it_bar).bg, (*it_bar).bg);     
        
        int fill_width = (int)((*it_bar).val / (*it_bar).max_val * (*it_bar).width);

        std::string fg_bar = (fill_width, " ");
        renderString((*it_bar).x + gui_stats.pos.x, (*it_bar).y + gui_stats.pos.y, fg_bar, (*it_bar).bg, (*it_bar).bg);     
    }
}

void Renderer::renderMessages() {
    //renderString();
}

void Renderer::renderGUIs() {
    renderStats();
    renderMessages();
    /*TCODConsole::blit(gui_messages.con, 0, 0, gui_messages.size.w, gui_messages.size.h, 
                      TCODConsole::root, 0, 0);*/
}