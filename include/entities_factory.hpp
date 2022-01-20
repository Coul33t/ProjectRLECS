#pragma once

#include "components/component.hpp"
#include "tags/tags.hpp"
#include "../extlib/flecs/flecs.h"

namespace EntFactories {
    flecs::entity createPlayer(flecs::world& ecs_world, uint x, uint y);
}