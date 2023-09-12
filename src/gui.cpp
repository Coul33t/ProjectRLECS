#include "../include/gui.hpp"

Gui::Gui() {
    pos.x = 0;
    pos.y = 0;
    size.w = 0;
    size.h = 0;
    //con = new TCODConsole(size.w, size.h);
    name = "GUI NAME NOT SET";
}

Gui::Gui(uint x, uint y, uint width, uint height, std::string name) {
    pos.x = x;
    pos.y = y;
    size.w = width;
    size.h = height;
    //con = new TCODConsole(width, height);
    name = name;
}

Gui::~Gui() = default;

void Gui::setGuiAttributes(std::string name, uint x, uint y, uint w, uint h) {
    this->name = name;
    pos.x = x;
    pos.y = y;
    size.w = w;
    size.h = h;
}

void Gui::addBar(uint x, uint y, uint width, std::string name, int val, 
                 uint max_val, color_t fg, color_t bg, color_t msg_fg) {
    bars.emplace_back(Bar(x, y, width, name, val, max_val, fg, bg, msg_fg));
}

void Gui::updateBarPos(std::string& bar_name, uint new_x, uint new_y) {
    for (Bar bar: bars) {
        if (bar.name == bar_name) {
            bar.x = new_x;
            bar.y = new_y;
        }
    }
}

void Gui::updateBarVal(std::string& bar_name, int new_val, uint new_max_val) {
    for (Bar bar: bars) {
        if (bar.name == bar_name) {
            bar.max_val = new_max_val;
            bar.val = new_val;
        }
    }
}

void Gui::updateBarColours(std::string& bar_name, color_t new_fg, color_t new_bg, color_t new_msg_fg) {
    for (Bar bar: bars) {
        if (bar.name == bar_name) {
            bar.fg = new_fg;
            bar.bg = new_bg;
            bar.msg_fg = new_msg_fg;
        }
    }
}

void Gui::renderChar(uint x, uint y, char chr, color_t fg, color_t bg) const {
    terminal_bkcolor(bg);
    terminal_color(fg);
    terminal_put_ext(x + this->pos.x, y + this->pos.y, 0, 0, chr);
}

void Gui::renderString(uint x, uint y, std::string str, color_t fg, color_t bg) const {
    terminal_bkcolor(bg);
    terminal_color(fg);
    terminal_print(x + this->pos.x, y + this->pos.y, str.c_str());
}


/*void Gui::renderBars() {
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
}*/

/*void Gui::render(bool debug) {
    con->clear();

    if(debug) {
        con->setDefaultBackground(red);
        for(size_t y = 0; y < size.h; y++) {
            for (size_t x = 0; x < size.w; x++) {
                con->putCharEx(x, y, 'X', blue, default_bg);
            }
        }
        con->setDefaultBackground(black);
        //con->rect(0, 0, size.w, size.h, true, TCOD_BKGND_SET);
    }

    else {
        renderBars();
    }



    TCODConsole::blit(this->con, 0, 0, size.w, size.h, 
                      TCODConsole::root, 0, 0);
}*/