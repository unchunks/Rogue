#pragma once

constexpr int FPS = 30;
constexpr int SPF = 1000 / FPS;

constexpr int FLOOR_W = 40;
constexpr int FLOOR_H = 40;
constexpr int BUFF_FLOOR_W = FLOOR_W + 2;
constexpr int BUFF_FLOOR_H = FLOOR_H + 2;
constexpr int AREA_MAX = 10;
constexpr int AREA_SIZE_MIN = 10;

constexpr int NUM_ENEMY = 3;
constexpr int ENEMY_FIND_RANGE = 10;
constexpr int ENEMY_SEARCH_INTERVAL = 10;

//スクリーンの寸法
constexpr int SCREEN_WIDTH = 996;
constexpr int SCREEN_HEIGHT = 560;

//レベルの寸法
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
constexpr int CHAR_VEL = ((TILE_WIDTH + TILE_HEIGHT) / FPS);// - (TILE_WIDTH % ((TILE_WIDTH + TILE_HEIGHT) / FPS));

//歩行アニメーション
constexpr int ANIMATION_FRAMES = 4;
constexpr int ANIM_SPEED = 4; //スプライト毎秒

// プレイヤー
constexpr int PLAYER_HP = 100;
constexpr int PLAYER_STR = 20;
constexpr int PLAYER_VIT = 10;

// デカ
constexpr int   DEKA_HP   = 50;
constexpr int   DEKA_STR  = 2;
constexpr int   DEKA_VIT  = 5;

// グリ
constexpr int   GURI_HP   = 50;
constexpr int   GURI_STR  = 2;
constexpr int   GURI_VIT  = 5;

