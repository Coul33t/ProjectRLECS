#pragma once

#include <iostream> 

#include "libtcod_no_warnings.hpp"

#include "types.hpp"

#include "../extlib/flecs/flecs.h"

#include "components/component.hpp"

static const uint ROOM_MAX_SIZE = 8;
static const uint ROOM_MIN_SIZE = 3;

struct Tile {
    bool walkable, explored, transparent;
    Tile(bool walkable=true, bool explored=false, bool transparent=false): 
        walkable(walkable), explored(explored), transparent(transparent) {}
};

struct Room {
    uint x, y, w, h;
    
    Room(uint x, uint y, uint w, uint h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    } 
};

class Map {
    public:
        Map();
        Map(uint w, uint h);

        ~Map();

        Room* getRoom(uint idx);
        const std::vector<Room*>& getRooms() const;

        void setSize(uint w, uint h);

        Tile* getTile(uint x, uint y);

        uint coordinates2dto1d(uint x, uint y);

        void setWall(uint x, uint y);
        void setFloor(uint x, uint y);

        bool isWalkable(uint x, uint y);
        bool isInFov(uint x, uint y);
        bool isExplored(uint x, uint y);

        void computeFov(uint x, uint y, uint vision_radius);
        void computeFov(flecs::entity player);
        
        void createEmptyMap();

        mVec2<uint16_t> size;
        std::vector<Tile*> map;
        std::vector<Room*> rooms;
        void dig(int x1, int y1, int x2, int y2);
        void createBSPMap();
        TCODMap* tcod_map;

    private:
        friend class BspListener;

};

class BspListener : public ITCODBspCallback {
    private :
        Map &map; // a map to dig
        int roomNum; // room number
        int lastx,lasty; // center of the last room
    public :
        BspListener(Map &map) : map(map), roomNum(0) {}
        bool visitNode(TCODBsp *node, void *userData) {
            if (node->isLeaf()) {    
                int x, y, w, h;
                // dig a room
                TCODRandom *rng = TCODRandom::getInstance();
                w=rng->getInt(ROOM_MIN_SIZE, node->w - 2);
                h=rng->getInt(ROOM_MIN_SIZE, node->h - 2);
                x=rng->getInt(node->x + 1, node->x + node->w - w - 1);
                y=rng->getInt(node->y + 1, node->y + node->h - h - 1);
                map.dig(x, y, x + w - 1, y + h - 1);

                if ( roomNum != 0 ) {
                    // dig a corridor from last room
                    map.dig(lastx,      lasty, x + w / 2, lasty);
                    map.dig(x + w / 2,  lasty, x + w / 2, y + h / 2);
                }

                lastx = x + w/ 2;
                lasty = y + h/ 2;
                roomNum++;
            }

            return true;
        }
};