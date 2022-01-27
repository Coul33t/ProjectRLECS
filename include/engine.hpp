#pragma once

#include <vector>

#include "libtcod_no_warnings.hpp"

#include "../extlib/flecs/flecs.h"

#include "map.hpp"
#include "gui.hpp"
#include "renderer.hpp"

#include "entities_factory.hpp"

enum GameState {
       STARTUP,
       IDLE,
       TOOK_TURN,
       VICTORY,
       DEFEAT
};

class Engine {
    public:
        Engine();
        Engine(uint w, uint h);

        ~Engine();

        void initSystems();
        void initRenderer(uint w = 80, uint h = 50);
        void initMap(uint w = 40, uint h = 20);

        void populateMap();

        bool isWalkable(uint x, uint y);
        bool hasEnemy(uint x, uint y);
        flecs::entity getEnemyAt(uint x, uint y);
        bool move(int dx, int dy);

        void run();


        Map map;
        Renderer renderer;

        flecs::world ecs_world;
        flecs::entity player;

        GameState game_state;
};