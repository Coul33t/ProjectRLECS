#pragma once
#include <string>
#include "colours.hpp"
#include "types.hpp"
#include "libtcod_no_warnings.hpp"
class Gui {
    public:
        Gui();
        Gui(uint x, uint y, uint width, uint height);
        ~Gui();

        void renderBar(uint x, uint y, uint width, std::string name, int val, 
                       uint max_val, TCODColor& colour, TCODColor& back_colour);
        
        void render(bool debug=false);

        // No choice or else weird error about being protected
        TCODConsole* con;
        TCODColor debug_colour;
        mVec2<uint> pos, size;
};