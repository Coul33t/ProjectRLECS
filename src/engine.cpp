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
        .each([this](flecs::entity e, Player& pl) {
            // Motion
            auto v = e.get_mut<Velocity>();
            auto p = e.get_mut<Position>();
            if (v->dx != 0 || v->dy != 0) {
                p->x += v->dx;
                p->y += v->dy;
                v->dx = 0;
                v->dy = 0;
            }

            // attack
            auto t = e.get_mut<Target>();

            if (t->target != flecs::entity::null()) {
                auto target_e = t->target.mut(e);
                attack(e, target_e);
            }
    });

    // AI system
    ecs_world.system<Position, Velocity, Monster, BasicAI, Alive, Target>()
        .kind(flecs::OnUpdate)
        .each([this](flecs::entity e, Position& p, Velocity& v, const Monster& m, const BasicAI& bai, Alive& a, Target& t) {
            
            /*if(t.target != flecs::entity::null()) {

            }

            auto player = ecs_world.lookup("Player");
            if(map.isInFov(p.x, p.y)) {
                move(player.get<Position>()->x, player.get<Position>()->y, e);
            }*/
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
    renderer.setConsole(w, h);
    onebitpack::initMapping(renderer.gt);
    onebitpack::setGraphicsToTiles(map, renderer.gt);
}


void Engine::initMap(uint w, uint h) {
    map.setSize(w, h);
}

void Engine::populateMap() {
    const uint max_monster_per_room = 5;
    uint nb_monsters = 0;

    std::vector<Room> rooms = map.getRooms();

    TCODRandom* rng = TCODRandom::getInstance();

    // Player coordinates
    auto pc = ecs_world.lookup("Player").get<Position>();

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

                x = rng->getInt((*it).x, (*it).x + (*it).w);
                y = rng->getInt((*it).y, (*it).y + (*it).h);

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
    auto pp = ecs_world.lookup("Player").get<Position>();
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
    
    ecs_world.each([this, x, y, &enemy](const flecs::entity e, const Position& p, const BlocksPath& bp, const Monster& m) {
        if(p.x == x && p.y == y) {
            enemy = e;
        }
    });

    return enemy;
}

bool Engine::move(int dx, int dy, flecs::entity& e) {
    bool has_moved = false;

    auto p = e.get_mut<Position>();
    auto v = e.get_mut<Velocity>();
    auto t = e.get_mut<Target>();

    if(isWalkable(p->x + dx, p->y + dy)) {
        v->dx = dx;
        v->dy = dy;
        has_moved = true;
    }

    else if (hasEnemy(p->x + dx, p->y + dy)) {
        t->target = getEnemyAt(p->x + dx, p->y + dy);
        has_moved = true;
    }
    
    return has_moved;
}

void Engine::attack(flecs::entity& origin, flecs::entity& target) {
    auto enemy_stats = target.get_mut<Stats>();
    auto origin_melee = origin.get_mut<Melee>();

    std::cout << origin.get<Stats>()->name << " attacks the " << enemy_stats->name << 
                 " for " << origin_melee->dmg << " damage !" << std::endl;  

    enemy_stats->hp -= origin_melee->dmg;

    std::cout << enemy_stats->name << "hp: " << enemy_stats->hp << "/" << enemy_stats->max_hp << std::endl;

    if (enemy_stats->hp <= 0) {
        target.remove<Alive>();                       
        target.add<Dead>();                  
    }

    origin.get_mut<Target>()->target = flecs::entity::null();
}

void Engine::run() {
    initMap();
    initSystems();

    map.createBSPMap();

    uint x = 0;
    uint y = 0;
    mVec2<uint> center = map.getRoom(0).getCenter();

    player = EntFactories::createPlayer(ecs_world, center.x, center.y);
    populateMap();

    map.computeFov(player);

    renderer.initPlayerBars(ecs_world);

    bool compute_fov = false;

    terminal_refresh();

    int key;
    bool key_pressed = false;
    bool has_moved = false;
    
    while (terminal_peek() != TK_CLOSE) {
        game_state = GameState::IDLE;

        key = 0; //TODO: check if 0 is ok
        
        if (terminal_has_input()) {
            key = terminal_read();
            key_pressed = true;
        }

        mVec2<uint> dir;
        dir.x = 0;
        dir.y = 0;

        switch(key) {
            case TK_UP :
                dir.x = 0;
                dir.y = -1;
                break;

            case TK_DOWN :
                dir.x = 0;
                dir.y = 1;
                break;

            case TK_LEFT :
                dir.x = -1;
                dir.y = 0;
                break;
            
            case TK_RIGHT :
                dir.x = 1;
                dir.y = 0;
                break;

            case TK_P:
                renderer.render_ascii = !renderer.render_ascii;
                break;

            default: break;
        }

        if(!(dir.x == 0 && dir.y == 0))
            has_moved = move(dir.x, dir.y, player);

        if (has_moved) {
            game_state = GameState::TOOK_TURN;
            dir.x = 0;
            dir.y = 0;
            key_pressed = false;
            has_moved = false;
        }

        if (game_state == GameState::TOOK_TURN) {
            ecs_world.progress();
            map.computeFov(player);
        }
        
        renderer.renderMap(map, false); // true for debug
        renderer.renderEntities(map, ecs_world);
        renderer.renderGUIs(ecs_world);

        terminal_refresh();
    }
}