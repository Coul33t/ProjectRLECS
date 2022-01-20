#pragma once

#include <vector>
#include <iostream>
#include "libtcod_no_warnings.hpp"

#include "map.hpp"
#include "colours.hpp"

#include "components/component.hpp"

class Renderer {
    public:
        Renderer();
        Renderer(uint w, uint h);

        void initConsole(uint w, uint h);

        void renderTile(Map* map, uint x, uint y, bool debug);

        void renderMap(Map* map, bool debug=false);

        void renderEntities(Map* map, flecs::world& ecs_world);
};
