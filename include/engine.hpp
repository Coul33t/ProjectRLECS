#pragma once

#include <vector>

#include "libtcod_no_warnings.hpp"

#include "../extlib/flecs/flecs.h"

#include "map.hpp"
#include "renderer.hpp"

#include "entities_factory.hpp"


class Engine {
    public:
        Engine();
        Engine(uint w, uint h);

        ~Engine();

        void initSystems();
        void initRenderer(uint w = 80, uint h = 50);
        void initMap(uint w = 40, uint h = 20);
        void initPlayer();

        void move(int dx, int dy);

        void run();


        Map* map;
        Renderer* renderer;

        flecs::world ecs_world;
        flecs::entity player;
        std::vector<flecs::entity> enemies;
};