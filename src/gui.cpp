#include "../include/gui.hpp"

Gui::Gui() {
    pos.x = 0;
    pos.y = 0;
    size.w = 0;
    size.h = 0;
    con = new TCODConsole(size.w, size.h);
    name = "GUI NAME NOT SET";
}

Gui::Gui(uint x, uint y, uint width, uint height, std::string name) {
    pos.x = x;
    pos.y = y;
    size.w = width;
    size.h = height;
    con = new TCODConsole(width, height);
    name = name;
}

Gui::~Gui() {
    delete con;
}

void Gui::setGuiAttributes(std::string name, uint x, uint y, uint w, uint h) {
    this->name = name;
    pos.x = x;
    pos.y = y;
    size.w = w;
    size.h = h;
}

void Gui::addBar(uint x, uint y, uint width, std::string name, int val, 
                 uint max_val, TCODColor& fg, TCODColor& bg, TCODColor& msg_fg) {
    bars.push_back(Bar(x, y, width, name, val, max_val, fg, bg, msg_fg));
}

void Gui::updateBarPos(std::string bar_name, uint new_x, uint new_y) {
    for (auto it = bars.begin(); it != bars.end(); it++) {
        if ((*it).name == bar_name) {
            (*it).x = new_x;
            (*it).y = new_y;
        }
    }
}

void Gui::updateBarVal(std::string bar_name, uint new_val, uint new_max_val) {
    for (auto it = bars.begin(); it != bars.end(); it++) {
        if ((*it).name == bar_name) {
            (*it).max_val = new_max_val;
            (*it).val = new_val;
        }
    }
}

void Gui::updateBarColours(std::string bar_name, TCODColor& new_fg, TCODColor& new_bg, TCODColor& new_msg_fg) {
    for (auto it = bars.begin(); it != bars.end(); it++) {
        if ((*it).name == bar_name) {
            (*it).fg = new_fg;
            (*it).bg = new_bg;
            (*it).msg_fg = new_msg_fg;
        }
    }
}

void Gui::renderBars() {
    for (auto it_bar = bars.begin(); it_bar != bars.end(); it_bar++) {
        con->setDefaultBackground((*it_bar).bg);
        con->rect((*it_bar).x, (*it_bar).y, (*it_bar).width, 1, false, TCOD_BKGND_SET);
        
        int fill_width = (int)((*it_bar).val / (*it_bar).max_val * (*it_bar).width);
        
        if (fill_width > 0) {
            con->setDefaultBackground((*it_bar).fg);
            con->rect((*it_bar).x, (*it_bar).y, fill_width, 1, false, TCOD_BKGND_SET);
        }

        con->setDefaultBackground((*it_bar).msg_fg);
        con->printEx((*it_bar).x + (*it_bar).width / 2, (*it_bar).y, TCOD_BKGND_NONE, TCOD_CENTER,
            "%s: %g/%g", (*it_bar).name, (*it_bar).val, (*it_bar).max_val);
    }
}

void Gui::render(bool debug) {
    con->clear();

    if(debug) {
        con->setDefaultBackground(red);
        con->rect(0, 0, size.w, size.h, false, TCOD_BKGND_SET);
    }

    else {
        renderBars();
    }

    TCODConsole::blit(con, 0, 0, size.w, size.h, 
                      TCODConsole::root, pos.x, pos.y);
}