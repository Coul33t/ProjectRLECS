#include "../include/entities_factory.hpp"

namespace EntFactories {
    flecs::entity createPlayer(flecs::world& ecs_world, uint x, uint y) {
        flecs::entity player = ecs_world.entity("Player");
        player.set<Position>({x, y});
        player.add<Velocity>();
        player.set<Vision>(10);
        player.set<Stats>(20);
        player.set<Melee>(5);
        player.add<Player>();
        player.set<Renderable>('@');
        return player;
    }
}
