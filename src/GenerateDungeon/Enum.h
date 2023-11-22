#pragma once

enum CELL_TYPE {
    NONE = 0,
//0辺床と接地（柱）
    PILLAR = 1,
//1辺床と接地
    WALL_LEFT = 2,
    WALL_RIGHT = 3,
    WALL_TOP = 4,
    WALL_BOTTOM = 5,
//2辺床と接地
    WALL_LEFT_TOP = 6,
    WALL_LEFT_BOTTOM = 7,
    WALL_RIGHT_TOP = 8,
    WALL_RIGHT_BOTTOM = 9,
    WALL_SIDE_LR = 10,
    WALL_SIDE_TB = 11,
//3辺床と接地
    WALL_END_RIGHT = 12,
    WALL_END_LEFT = 13,
    WALL_END_BOTTOM = 14,
    WALL_END_TOP = 15,
//4辺床と接地
    WALL_ALL = 16,
    FLOOR = 17,
    AISLE = 18,
    STEP = 19,

    CELL_NUM = 20
};

enum DIRECTION {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NO_DIRECTION
};

enum CHAR_TYPE {
    PLAYER,
    ENEMY
};

enum ENEMY_TYPE {
    DEKA,
    GURI,
    ENEMY_TYPE_NUMBER
};
