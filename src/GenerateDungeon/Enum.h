#pragma once

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
    STEP
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

// TODO: キャラを作ったらConst.hでステータスを作成
enum ENEMY_TYPE {
    DEKA,
    ENEMY_TYPE_NUMBER
};
