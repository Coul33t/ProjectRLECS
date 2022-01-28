#include "../include/gui.hpp"

Gui::Gui() {
    pos.x = 0;
    pos.y = 0;
    size.w = 0;
    size.h = 0;
    con = new TCODConsole(size.w, size.h);
    //debug_colour = TCODColor(255, 0, 0);
}

Gui::Gui(uint x, uint y, uint width, uint height) {
    pos.x = x;
    pos.y = y;
    size.w = width;
    size.h = height;
    con = new TCODConsole(width, height);
}

Gui::~Gui() {
    delete con;
}

void Gui::renderBar(uint x, uint y, uint width, std::string name, int val, 
                     uint max_val, TCODColor& colour, TCODColor& back_colour) {

}

void Gui::render(bool debug) {
    if(debug) {

    }
}