#include "../include/engine.hpp"

Engine::Engine() {
    renderer = new Renderer();
    map = new Map();

    initRenderer(80, 50);
}

Engine::Engine(uint w, uint h) {
    renderer = new Renderer();
    map = new Map();

    initRenderer(w, h);
}

Engine::~Engine() {
    delete map;
    delete renderer;
}

void Engine::initSystems() {
    ecs_world.system<Position, Velocity>()
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e, Position& p, Velocity& v) {
            if (v.dx != 0 || v.dy != 0) {
                p.x += v.dx;
                p.y += v.dy;
                v.dx = 0;
                v.dy = 0;
            }
    });
}

void Engine::initRenderer(uint w, uint h) {
    renderer->initConsole(w, h);
}


void Engine::initMap(uint w, uint h) {
    map->setSize(w, h);
}

void Engine::move(int dx, int dy) {
    ecs_world.each([this, dx, dy](flecs::entity e, const Player, const Position& p, Velocity& v) {
        if(map->isWalkable(p.x + dx, p.y + dy)) {
            v.dx = dx;
            v.dy = dy;
        }
    });
}

void Engine::run() {
    initSystems();

    map->createBSPMap();

    uint x = 0;
    uint y = 0;

    Room* room = map->getRoom(0);
    x = room->x + 1;
    y = room->y + 1;

    player = EntFactories::createPlayer(ecs_world, x, y);

    map->computeFov(player);

    bool compute_fov = false;
    
    while (!TCODConsole::isWindowClosed()) {
        TCOD_key_t key;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

        switch(key.vk) {
            case TCODK_UP :
                move(0, -1);
                compute_fov = true;
                break;

            case TCODK_DOWN :
                move(0, 1);
                compute_fov = true;
                break;

            case TCODK_LEFT :
                move(-1, 0);
                compute_fov = true;
                break;
            
            case TCODK_RIGHT :
                move(1, 0);
                compute_fov = true;
                break;

            default: break;
        }

        if (compute_fov) {
            map->computeFov(player);
            compute_fov = false;
        }

        ecs_world.progress();

        TCODConsole::root->clear();
        renderer->renderMap(map, false); // true for debug
        renderer->renderEntities(map, ecs_world);
        TCODConsole::flush();       
    }
}