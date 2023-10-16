#pragma once

constexpr int FLOOR_W = 40;
constexpr int FLOOR_H = 40;
constexpr int BUFF_FLOOR_W = FLOOR_W + 2;
constexpr int BUFF_FLOOR_H = FLOOR_H + 2;
constexpr int AREA_MAX = 10;
constexpr int AREA_SIZE_MIN = 10;

constexpr int NUM_ENEMY = 3;
constexpr int ENEMY_FIND_RANGE = 10;
constexpr int ENEMY_SEARCH_INTERVAL = 10;

//カメラレベルの寸法
constexpr int LEVEL_WIDTH = 5120;
constexpr int LEVEL_HEIGHT = 5120;

//Tile 定数
constexpr int TILE_WIDTH = 128;
constexpr int TILE_HEIGHT = 128;
constexpr int TOTAL_TILES = 1600;
constexpr int TOTAL_TILE_SPRITES = 20;

//キャラクターの寸法
constexpr int SPRITE_CHAR_WIDTH = 64;
constexpr int SPRITE_CHAR_HEIGHT = 64;

//軸方向の最大速度
constexpr int DOT_VEL = 8;

//位置調整の速度
constexpr int DOT_BACK_VEL = DOT_VEL / 2;

//キャラアニメーションのフレーム数
constexpr int ANIMATION_FRAMES = 4;
constexpr int ANIM_SPEED = 4; //毎秒

constexpr double FPS = 1;
constexpr double SPF = 1000.0 / FPS;

// プレイヤー
constexpr int PLAYER_HP = 100;
constexpr int PLAYER_STR = 20;
constexpr int PLAYER_VIT = 10;

// NOTE: キャラを作ったらEnum.hに名前を追加
// スライム
constexpr int   SLIME_HP   = 50;
constexpr int   SLIME_STR  = 2;
constexpr int   SLIME_VIT  = 5;
constexpr char* SLIME_ICON = (char*)"Ω ";

// ゴブリン
constexpr int   GOBLIN_HP   = 80;
constexpr int   GOBLIN_STR  = 5;
constexpr int   GOBLIN_VIT  = 8;
constexpr char* GOBLIN_ICON = (char*)"ಪ ";

// ドラゴン
constexpr int   DRAGON_HP   = 1000;
constexpr int   DRAGON_STR  = 70;
constexpr int   DRAGON_VIT  = 30;
constexpr char* DRAGON_ICON = (char*)"ඣ ";
