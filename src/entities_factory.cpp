#include "../include/entities_factory.hpp"

namespace EntFactories {
    flecs::entity createPlayer(flecs::world& ecs_world, uint x, uint y) {
        flecs::entity player = ecs_world.entity("Player");
        // components
        int x_int = static_cast<int>(x);
        int y_int = static_cast<int>(y);
        player.set<Position>({x_int, y_int});
        player.add<Velocity>();
        player.set<Vision>(10);
        player.set<Stats>({"Exodius", 20, 20});
        player.set<Melee>(5);
        player.set<Renderable>({'@', '%', Colours::green, Colours::dark_green});
        player.add<Target>();

        // tags
        player.add<Player>();
        player.add<BlocksPath>();
        player.add<Alive>();
        return player;
    }

    flecs::entity createMonster(flecs::world& ecs_world, uint x, uint y, char glyph) {
        flecs::entity monster = ecs_world.entity();
        // components
        int x_int = static_cast<int>(x);
        int y_int = static_cast<int>(y);
        monster.set<Position>({x_int, y_int});
        monster.add<Velocity>();
        monster.set<Vision>(5);
        monster.set<Stats>({"Generic monster", 5, 5});
        monster.set<Melee>(1);
        monster.set<Renderable>({glyph, '%', Colours::red, Colours::dark_red});
        monster.add<Target>();
        
        // tags
        monster.add<Monster>();
        monster.add<BlocksPath>();
        monster.add<Alive>();
        monster.add<BasicAI>();
        return monster;
    }
}
