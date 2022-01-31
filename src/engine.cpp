#include "../include/engine.hpp"

Engine::Engine() {
    game_state = GameState::STARTUP;

    initRenderer(80, 50);
}

Engine::Engine(uint w, uint h) {
    game_state = GameState::STARTUP;
    
    initRenderer(w, h);
}

Engine::~Engine() {

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
                Stats* enemy_stats = m->target.mut(e).get_mut<Stats>();
                std::cout << e.get<Stats>()->name << " attacks the " << enemy_stats->name << " for " << e.get<Melee>()->dmg << " damage !" << std::endl;  
                enemy_stats->hp -= e.get<Melee>()->dmg;
                std::cout << enemy_stats->name << "hp: " << enemy_stats->hp << "/" << enemy_stats->max_hp << std::endl;
                
                if (enemy_stats->hp <= 0) {
                    m->target.mut(e).remove<Alive>();                       
                    m->target.mut(e).add<Dead>();                  
                }

                m->target = flecs::entity::null();
            }
    });

    // AI system
    ecs_world.system<Position, Velocity, Monster, BasicAI, Alive>()
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e, Position& p, Velocity& v, const Monster& m, const BasicAI& bai, Alive& a) {
            
    });
    
    // Move system
    ecs_world.system<Position, Velocity, Alive>()
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e, Position& p, Velocity& v, const Alive& a) {
            if (v.dx != 0 || v.dy != 0) {
                p.x += v.dx;
                p.y += v.dy;
                v.dx = 0;
                v.dy = 0;
            }
    });

}

void Engine::initRenderer(uint w, uint h) {
    renderer.initConsole(w, h);
}


void Engine::initMap(uint w, uint h) {
    map.setSize(w, h);
}

void Engine::populateMap() {
    const uint max_monster_per_room = 5;
    uint nb_monsters = 0;

    std::vector<Room*> rooms = map.getRooms();

    TCODRandom* rng = TCODRandom::getInstance();

    // Player coordinates
    const Position* pc = ecs_world.lookup("Player").get<Position>();

    const uint max_tries_per_monster = 100;
    uint current_tries = 0;

    for (auto it = rooms.begin(); it != rooms.end(); it++) {
        nb_monsters = rng->getInt(0, max_monster_per_room);
        
        for (size_t i = 0; i < nb_monsters; i++) {
            uint x, y;
            bool has_enemy = false;
            current_tries = 0;

            do {
                current_tries++;
                has_enemy = false;

                x = rng->getInt((*it)->x, (*it)->x + (*it)->w);
                y = rng->getInt((*it)->y, (*it)->y + (*it)->h);

                ecs_world.each([&has_enemy, &x, &y](const Position& p, const Monster& m) { // flecs::entity argument is optional
                    if (p.x == x && p.y == y) {
                        has_enemy = true;
                    }
                });

            } while((has_enemy || (x == pc->x && y == pc->y)) && current_tries < max_tries_per_monster);

            if (current_tries < max_tries_per_monster)
                EntFactories::createMonster(ecs_world, x, y, 'x');
        }
    }
}

bool Engine::isWalkable(uint x, uint y) {
    const Position* pp = ecs_world.lookup("Player").get<Position>();
    if (x == pp->x && y == pp->y)
        return false;

    if(!map.isWalkable(x, y))
        return false;

    bool walkable = true;

    // If the entity blocks path, is alive and position == next move, can't go there
    ecs_world.each([x, y, &walkable](flecs::entity e, const Position& p, const BlocksPath& bp, const Alive& a) {
        if (!e.has<Player>())
            if(p.x == x && p.y == y)
                walkable = false;
    });

    return walkable;
}

bool Engine::hasEnemy(uint x, uint y) {
    bool enemy_found = false;
    
    ecs_world.each([x, y, &enemy_found](const Position& p, const BlocksPath& bp, const Monster& m) {
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

bool Engine::move(int dx, int dy) {
    ecs_world.each([this, dx, dy](const Player& pl, const Position& p, Velocity& v, Melee& m) {
        if(isWalkable(p.x + dx, p.y + dy)) {
            v.dx = dx;
            v.dy = dy;
            return true;
        }

        else if (hasEnemy(p.x + dx, p.y + dy)) {
            m.target = getEnemyAt(p.x + dx, p.y + dy);
            return true;
        }
    });

    return false;
}

void Engine::run() {
    Renderable a, b; 
    a = std::move(b);

    initSystems();

    map.createBSPMap();

    uint x = 0;
    uint y = 0;
    mVec2<uint> center = map.getRoom(0)->getCenter();

    player = EntFactories::createPlayer(ecs_world, center.x, center.y);
    populateMap();

    map.computeFov(player);

    bool compute_fov = false;
    
    while (!TCODConsole::isWindowClosed()) {
        game_state = GameState::IDLE;
        
        TCOD_key_t key;
        TCOD_event_t ev = TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

        mVec2<uint> dir;

        switch(key.vk) {
            case TCODK_UP :
                dir.x = 0;
                dir.y = -1;
                break;

            case TCODK_DOWN :
                dir.x = 0;
                dir.y = 1;
                break;

            case TCODK_LEFT :
                dir.x = -1;
                dir.y = 0;
                break;
            
            case TCODK_RIGHT :
                dir.x = 1;
                dir.y = 0;
                break;

            default: break;
        }

        if (ev == TCOD_EVENT_KEY_PRESS) {
            game_state = GameState::TOOK_TURN;
            move(dir.x, dir.y);
        }

        if (game_state == GameState::TOOK_TURN) {
            map.computeFov(player);
            ecs_world.progress();
        }
            

        TCODConsole::root->clear();
        renderer.renderMap(map, false); // true for debug
        renderer.renderEntities(map, ecs_world);
        renderer.renderGUIs();
        TCODConsole::flush();       
    }
}