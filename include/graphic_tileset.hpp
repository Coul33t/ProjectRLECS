#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "types.hpp"

#include "map.hpp"

#include "tools.hpp"

struct GraphicTileset {
    // All int are hex values
    int base_index;

    mVec2<uint> size;

    std::map<std::string, int> general_mapping;
    std::map<std::string, int> ground_mapping;
    std::map<std::string, int> wall_mapping;

    std::vector<std::string> general_keys;
    std::vector<std::string> ground_keys;
    std::vector<std::string> wall_keys;

    GraphicTileset(int base_index=0x1000): base_index(base_index) {
        size.w = 0;
        size.h = 0;
    };

    int getTile(const std::string& name) const {
        if (general_mapping.find(name) != general_mapping.end())
            return general_mapping.at(name);

        else if (ground_mapping.find(name) != ground_mapping.end())
            return ground_mapping.at(name);

        else if (wall_mapping.find(name) != wall_mapping.end())
            return wall_mapping.at(name);

        else {
            std::cout << "ERROR: can't find key " << name << " in the tiles list." << std::endl;
            return 0;
        }
    }

    int getGround(const std::string& name) const {
        return ground_mapping.at(name);
    }

    int getWall(const std::string& name) const {
        return wall_mapping.at(name);
    }

    int getRandomGeneral() {
        auto idx = Random::get_int(0, general_keys.size() - 1);
        return general_mapping[general_keys[idx]];
    }

    int getRandomGround() {
        auto idx = Random::get_int(0, ground_keys.size() - 1);
        return ground_mapping[ground_keys[idx]];
    }

    int getRandomWall() {
        auto idx = Random::get_int(0, wall_keys.size() - 1);
        return wall_mapping[wall_keys[idx]];
    }
};

namespace onebitpack {
    void initMapping(GraphicTileset& gt);
    void setGraphicsToTiles(Map& map, GraphicTileset& gt);
}