#pragma once
#include <iostream>
#include <string>
#include "colours.hpp"
#include "types.hpp"
#include "libtcod_no_warnings.hpp"

struct ConTile {
    char chr;
    color_t bg, fg;

    ConTile() {
        chr = ' ';
        bg = Colours::black;
        fg = Colours::white;
    }
};

struct Message {
    uint x, y;
    std::string content;
    color_t fg, bg;
    
    Message() {
        x = 0;
        y = 0;
        content = "MESSAGE CONTENT NOT SET";
        fg = color_from_argb(255, 255, 255, 255);
        bg = color_from_argb(255,   0,   0,   0);
    };

    Message(uint x, uint y, std::string content): x(x), y(y), content(content) {
        fg = color_from_argb(255, 255, 255, 255);
        bg = color_from_argb(255,   0,   0,   0);
    };

    Message(uint x, uint y, std::string content, color_t fg, color_t bg):
        x(x), y(y), content(content), fg(fg), bg(bg) {};
};

struct Bar {
    uint x, y, width;
    std::string name;
    int val;
    uint max_val;
    color_t fg, bg, msg_fg;

    Bar() {
        x = 0;
        y = 0;
        width = 5;
        name = "NO NAME SET";
        val = 5;
        max_val = 10;
        fg =     color_from_argb(255, 255, 255, 255);
        bg =     color_from_argb(255,   0,   0,   0);
        msg_fg = color_from_argb(255, 125, 125, 125);
    }

    Bar(uint x, uint y, uint width, std::string name, int val, 
        uint max_val, color_t fg, color_t bg, color_t msg_fg):
            x(x), y(y), width(width), name(name), val(val),
            max_val(max_val), fg(fg), bg(bg), msg_fg(msg_fg) {};
};


class Gui {
    public:
        Gui();
        Gui(uint x, uint y, uint width, uint height, std::string name);
        ~Gui();

        void setGuiAttributes(std::string name, uint x, uint y, uint w, uint h);

        void addBar(uint x, uint y, uint width, std::string name, int val, 
                    uint max_val, color_t fg, color_t bg, color_t msg_fg);

        void updateBarPos(std::string bar_name, uint new_x, uint new_y);
        void updateBarVal(std::string bar_name, uint new_val, uint new_max_val);
        void updateBarColours(std::string bar_name, color_t new_fg, color_t new_bg, color_t new_msg_fg);

        void renderChar(uint x, uint y, char chr,color_t fg, color_t bg);
        void renderString(uint x, uint y, std::string str, color_t fg, color_t bg);

        /*void renderBars();
        
        void render(bool debug=false);*/

        std::string name;
        mVec2<uint> pos, size;
        std::vector<Bar> bars;
        std::vector<Message> messages;
};