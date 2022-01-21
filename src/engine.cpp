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
    // Player system
    ecs_world.system<Player>()
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e, Player& pl) {
            // Motion
            Velocity* v = e.get_mut<Velocity>();
            Position* p = e.get_mut<Position>();
            if (v->dx != 0 || v->dy != 0) {
                p->x += v->dx;
                p->y += v->dy;
                v->dx = 0;
                v->dy = 0;
            }

            // attack
            Melee* m = e.get_mut<Melee>();

            if (m->target != flecs::entity::null()) {
                std::cout << e.get<Stats>()->name << " attacks the " << m->target.get_ref<Stats>()->name << " !" << std::endl;
                m->target = flecs::entity::null();
            }
    });

    // AI system
    ecs_world.system<Position, Velocity, Monster, BasicAI>()
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e, Position& p, Velocity& v, const Monster& m, const BasicAI& bai) {
            
    });
    
    // Move system
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

void Engine::populateMap() {
    const uint max_monster_per_room = 5;
    uint nb_monsters = 0;

    std::vector<Room*> rooms = map->getRooms();

    TCODRandom* rng = TCODRandom::getInstance();

    for (auto it = rooms.begin(); it != rooms.end(); it++) {
        nb_monsters = rng->getInt(0, max_monster_per_room);
        for (size_t i = 0; i < nb_monsters; i++) {
            uint x = rng->getInt((*it)->x, (*it)->x + (*it)->w);
            uint y = rng->getInt((*it)->y, (*it)->y + (*it)->h);
            enemies.push_back(EntFactories::createMonster(ecs_world, x, y, 'x'));
        }
    }
}

bool Engine::isWalkable(uint x, uint y) {
    if(!map->isWalkable(x, y))
        return false;

    bool walkable = true;

    ecs_world.each([x, y, &walkable](flecs::entity e, const Position& p, const BlocksPath& bp) {
        if (!e.has<Player>())
            if(p.x == x && p.y == y)
                walkable = false;
    });

    return walkable;
}

bool Engine::hasEnemy(uint x, uint y) {
    bool enemy_found = false;
    
    ecs_world.each([x, y, &enemy_found](flecs::entity e, const Position& p, const BlocksPath& bp, const Monster& m) {
        if(p.x == x && p.y == y)
            enemy_found = true;
    });

    return enemy_found;
}

flecs::entity Engine::getEnemyAt(uint x, uint y) {
    flecs::entity enemy;
    
    ecs_world.each([this, x, y, &enemy](flecs::entity e, const Position& p, const BlocksPath& bp, const Monster& m) {
        if(p.x == x && p.y == y)
            enemy = e;
    });

    return enemy;
}

void Engine::move(int dx, int dy) {
    ecs_world.each([this, dx, dy](flecs::entity e, const Player, const Position& p, Velocity& v, Melee& m) {
        if(isWalkable(p.x + dx, p.y + dy)) {
            v.dx = dx;
            v.dy = dy;
        }

        else if (hasEnemy(p.x + dx, p.y + dy)) {
            m.target = getEnemyAt(p.x + dx, p.y + dy);
        }
    });
}

void Engine::run() {
    initSystems();

    map->createBSPMap();

    uint x = 0;
    uint y = 0;
    mVec2<uint> center = map->getRoom(0)->getCenter();

    player = EntFactories::createPlayer(ecs_world, center.x, center.y);
    populateMap();

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