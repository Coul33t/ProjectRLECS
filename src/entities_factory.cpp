#include "../include/entities_factory.hpp"

namespace EntFactories {
    flecs::entity createPlayer(flecs::world& ecs_world, uint x, uint y) {
        flecs::entity player = ecs_world.entity("Player");
        // components
        player.set<Position>({x, y});
        player.add<Velocity>();
        player.set<Vision>(10);
        player.set<Stats>({"Exodius", 20, 20});
        player.set<Melee>(5);
        player.set<Renderable>({'@', '%'});

        // tags
        player.add<Player>();
        player.add<BlocksPath>();
        return player;
    }

    flecs::entity createMonster(flecs::world& ecs_world, uint x, uint y, char glyph) {
        flecs::entity monster = ecs_world.entity();
        // components
        monster.set<Position>({x, y});
        monster.add<Velocity>();
        monster.set<Vision>(5);
        monster.set<Stats>({"Generic monster", 5, 5});
        monster.set<Melee>(1);
        monster.set<Renderable>(glyph);
        
        // tags
        monster.add<Monster>();
        monster.add<BlocksPath>();
        monster.add<BasicAI>();
        return monster;
    }
}
