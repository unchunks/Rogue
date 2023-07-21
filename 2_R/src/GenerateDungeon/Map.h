#pragma once

#include <vector>

const int mapW = 40;
const int mapH = 40;

const int ROOM_MARGIN = 2;

const int AREA_MAX = 10;
const int AREA_SIZE_MIN = 10;

enum CELL_TYPE {
    NONE,
//0辺床と接地（柱）
    PILLAR,
//1辺床と接地
    WALL_LEFT,
    WALL_RIGHT,
    WALL_TOP,
    WALL_BOTTOM,
//2辺床と接地
    WALL_LEFT_TOP,
    WALL_LEFT_BOTTOM,
    WALL_RIGHT_TOP,
    WALL_RIGHT_BOTTOM,
    WALL_SIDE_LR,
    WALL_SIDE_TB,
//3辺床と接地
    WALL_END_LEFT,
    WALL_END_RIGHT,
    WALL_END_TOP,
    WALL_END_BOTTOM,
//4辺床と接地
    WALL_ALL,
    FLOOR,
    AISLE,
    STEP,

    PLAYER,
    ENEMY,
    CHARACTER
};

class Room {
public:
    Room(int _x, int _y, int _w, int _h)
        :x(_x), y(_y), w(_w), h(_h){};
    int x, y, w, h;
};

class Area {
public:
    Area(int _x, int _y, int _w, int _h)
        :x(_x), y(_y), w(_w), h(_h), 
        room(x + ROOM_MARGIN, y + ROOM_MARGIN, w - ROOM_MARGIN*2, h - ROOM_MARGIN*2){};
    int x, y, w, h;
    Room room;
};