#pragma once

#include <vector>
#include <iostream>
#include "libtcod_no_warnings.hpp"

#include "map.hpp"
#include "colours.hpp"
#include "gui.hpp"

#include "components/component.hpp"
#include "tags/tags.hpp"

const uint MAP_POSITION_X = 0;
const uint MAP_POSITION_Y = 0;
const uint MAP_SIZE_W = 50;
const uint MAP_SIZE_H = 30;

const uint MESSAGES_POSITION_X = 0;
const uint MESSAGES_POSITION_Y = 30;
const uint MESSAGES_SIZE_W = 50;
const uint MESSAGES_SIZE_H = 20;

const uint STATS_POSITION_X = 50;
const uint STATS_POSITION_Y = 0;
const uint STATS_SIZE_W = 30;
const uint STATS_SIZE_H = 30;

class Renderer {
    public:
        Renderer();
        Renderer(uint w, uint h);

        void initConsole(uint w, uint h);

        void renderTile(Map& map, uint x, uint y, bool debug);

        void renderMap(Map& map, bool debug=false);

        void renderEntities(Map& map, flecs::world& ecs_world);

        void renderGUIs();

        Gui gui_map, gui_messages, gui_stats;
};
