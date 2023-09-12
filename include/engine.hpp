#pragma once

#include <vector>

#include "libtcod_no_warnings.hpp"
#include "BearLibTerminal.h"

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
        void initMap(uint w = 50, uint h = 30);

        void populateMap();

        bool isWalkable(uint x, uint y);
        bool hasEntity(uint x, uint y);
        flecs::entity getEnemyAt(uint x, uint y);
        bool move(int dx, int dy, flecs::entity& e);
        bool move_to(int x, int y, flecs::entity& e);
        void attack(flecs::entity& origin, flecs::entity& target);

        void run();


        Map map;
        Renderer renderer;

        flecs::world ecs_world;
        flecs::entity player;

        GameState game_state;
};