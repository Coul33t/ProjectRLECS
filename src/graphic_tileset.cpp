//
// Created by couland-q on 25/03/2022.
//

#include "../include/graphic_tileset.hpp"

namespace onebitpack {
    void initMapping(GraphicTileset& gt) {
        gt.size.w = 49;
        gt.size.h = 22;
        gt.general_mapping["void"] = gt.base_index;
        gt.ground_mapping["ground1"] = gt.base_index + 2;
        gt.ground_mapping["ground2"] = gt.base_index + 3;
        gt.ground_mapping["ground3"] = gt.base_index + 4;
        gt.wall_mapping["wall"] = gt.base_index + (17 * gt.size.w + 10);

        for (auto it = gt.general_mapping.begin(); it != gt.general_mapping.end(); it++) {
            gt.general_keys.push_back((*it).first);
        }

        for (auto it = gt.ground_mapping.begin(); it != gt.ground_mapping.end(); it++) {
            gt.ground_keys.push_back((*it).first);
        }

        for (auto it = gt.wall_mapping.begin(); it != gt.wall_mapping.end(); it++) {
            gt.wall_keys.push_back((*it).first);
        }
    }

    void setGraphicsToTiles(Map& map, GraphicTileset& gt) {
        for (Tile tile: map.map) {
            if (tile.isWall())
                tile.graphic_idx = gt.getTile("wall");
            else
                tile.graphic_idx = gt.getRandomGround();
        }
    }
}