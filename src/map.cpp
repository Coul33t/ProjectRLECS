 #include "../include/map.hpp"

Map::Map() {
    size.w = 40;
    size.h = 20;
    
    map.reserve(size.w * size.h);
    for (size_t i = 0; i < size.w * size.h; i++)
        map.push_back(new Tile(false));

    tcod_map = new TCODMap(size.w, size.h);
}

Map::Map(uint w, uint h) {
    size.w = w;
    size.h = h;
    
    map.reserve(size.w * size.h);
    for (size_t i = 0; i < size.w * size.h; i++)
        map.push_back(new Tile(false));

    tcod_map = new TCODMap(size.w, size.h);
}

Map::~Map() {
    for (Tile* t: map)
        delete t;

    for (Room* r: rooms)
        delete r;
    
    delete tcod_map;
}

Tile* Map::getTile(uint x, uint y) {
    return map[coordinates2dto1d(x, y)];
}

Room* Map::getRoom(uint idx) {
    if (idx > rooms.size() - 1) {
        std::cout << "WARNING: idx is bigger than size of rooms vector. Returning last room instead." << std::endl;
        return rooms.back();
    }

    return rooms[idx];
}

const std::vector<Room*>& Map::getRooms() const {
    return rooms;
}

void Map::setSize(uint w, uint h) {
    map.clear();
    size.w = w;
    size.h = h;
    
    map.reserve(size.w * size.h);
    for (size_t i = 0; i < size.w * size.h; i++)
        map.push_back(new Tile(false));

    tcod_map = new TCODMap(size.w, size.h);
}

uint Map::coordinates2dto1d(uint x, uint y) {
    return (size.w * y + x);
}

void Map::setWall(uint x, uint y) {
    map[coordinates2dto1d(x, y)]->walkable = false;
    tcod_map->setProperties(x, y, false, false);
}

void Map::setFloor(uint x, uint y) {
    map[coordinates2dto1d(x, y)]->walkable = true;
    map[coordinates2dto1d(x, y)]->transparent = true;
    tcod_map->setProperties(x, y, true, true);
}

bool Map::isWalkable(uint x, uint y) {
    return map[coordinates2dto1d(x, y)]->walkable;
}

bool Map::isExplored(uint x, uint y) {
    return map[coordinates2dto1d(x, y)]->explored;
}

bool Map::isInFov(uint x, uint y) {
    if(tcod_map->isInFov(x, y)) {
        map[coordinates2dto1d(x, y)]->explored = true;
        return true;
    }
    
    return false;
}

void Map::computeFov(uint x, uint y, uint vision_radius) {
    tcod_map->computeFov(x, y, vision_radius, true);
}

void Map::computeFov(flecs::entity player) {
    tcod_map->computeFov(player.get<Position>()->x, 
                         player.get<Position>()->y, 
                         player.get<Vision>()->fov_radius, 
                         true);
}

void Map::createEmptyMap() {
    for (size_t y = 0; y < size.h; y++) {
        for (size_t x = 0; x < size.w; x++) {
            if (y == 0 || y == size.h - 1 || x == 0 || x == size.w - 1)
                setWall(x, y);
            else 
                setFloor(x, y);
        }
    }
}

void Map::dig(int x1, int y1, int x2, int y2) {
    if (x2 < x1)
        std::swap(x1, x2);

    if (y2 < y1)
        std::swap(y1, y2);

    if (x1 < 1) {
        std::cout << "WARNING: x1 is smaller than 1. Setting it to 1 to fit." << std::endl;
        x1 = 1;
    }

    if (y1 < 1) {
        std::cout << "WARNING: y1 is smaller than 1. Setting it to 1 to fit." << std::endl;
        y1 = 1;
    }

    if (x2 > size.w - 2) {
        std::cout << "WARNING: width is bigger than map size. Reducing size to fit." << std::endl;
        x2 = size.w - 2;
    }

    if (y2 > size.h - 2) {
        std::cout << "WARNING: height is bigger than map size. Reducing size to fit." << std::endl;
        y2 = size.h - 2;
    }

    for (int tilex = x1; tilex <= x2; tilex++) {
        for (int tiley = y1; tiley <= y2; tiley++) {
            setFloor(tilex, tiley);
        }
    }

    rooms.push_back(new Room(x1, y1, x2 - x1, y2 - y1));
}

void Map::createBSPMap() {
    TCODBsp bsp(0, 0, size.w, size.h);
    bsp.splitRecursive(NULL, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, NULL);
}