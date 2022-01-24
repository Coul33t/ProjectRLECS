#pragma once

#include "components/component.hpp"
#include "tags/tags.hpp"
#include "../extlib/flecs/flecs.h"
#include "colours.hpp"

namespace EntFactories {
    flecs::entity createPlayer(flecs::world& ecs_world, uint x, uint y);
    flecs::entity createMonster(flecs::world& ecs_world, uint x, uint y, char glyph);
}