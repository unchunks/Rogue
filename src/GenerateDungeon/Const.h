#pragma once

constexpr int FPS = 30;
constexpr int SPF = 1000 / FPS;

constexpr int FLOOR_W = 40;
constexpr int FLOOR_H = 40;
constexpr int BUFF_FLOOR_W = FLOOR_W + 2;
constexpr int BUFF_FLOOR_H = FLOOR_H + 2;
constexpr int AREA_MAX = 10;
constexpr int AREA_SIZE_MIN = 10;

// 同時スポーンする敵の数
constexpr int NUM_ENEMY = 10;
// 敵の検知範囲
constexpr int ENEMY_FIND_RANGE = 3;
constexpr int ENEMY_SEARCH_INTERVAL = 10;

// スクリーンの寸法（幅）
constexpr int SCREEN_WIDTH = 996;
// スクリーンの寸法（高さ）
constexpr int SCREEN_HEIGHT = 560;

// Tile 定数
constexpr int TILE_W = 128;
constexpr int TILE_H = 128;
constexpr int TOTAL_TILES = 1600;
constexpr int TOTAL_TILE_SPRITES = 20;

// レベルの寸法
constexpr int LEVEL_WIDTH = TILE_W * FLOOR_W;
constexpr int LEVEL_HEIGHT = TILE_H * FLOOR_H;

// UI同士の間隔
constexpr int PADDING = 5;

// キャラクターの寸法
constexpr int SPRITE_CHAR_WIDTH = 64;
constexpr int SPRITE_CHAR_HEIGHT = 64;

// 軸方向の最大速度
constexpr int CHAR_VEL = ((TILE_W + TILE_H) / FPS);

// 歩行アニメーション
constexpr int ANIMATION_FRAMES = 4;
// スプライト毎秒
constexpr int ANIM_SPEED = 8;

// REVIEW: プレイヤー
constexpr int PLAYER_HP = 100;
constexpr int PLAYER_STR = 20;
constexpr int PLAYER_VIT = 20;

// デカ
constexpr int DEKA_HP   = 70;
constexpr int DEKA_STR  = 10;
constexpr int DEKA_VIT  = 20;

// グリ
constexpr int GURI_HP   = 30;
constexpr int GURI_STR  = 35;
constexpr int GURI_VIT  = 5;

// ジェリフ
constexpr int JELYF_HP   = 20;
constexpr int JELYF_STR  = 15;
constexpr int JELYF_VIT  = 5;

// ヤミー
constexpr int YUMMY_HP   = 100;
constexpr int YUMMY_STR  = 5;
constexpr int YUMMY_VIT  = 5;

// クリスタル
constexpr int CRYSTAL_HP   = 50;
constexpr int CRYSTAL_STR  = 1;
constexpr int CRYSTAL_VIT  = 30;

// グロスパイダー
constexpr int GROSSPIDER_HP   = 40;
constexpr int GROSSPIDER_STR  = 7;
constexpr int GROSSPIDER_VIT  = 7;

// アイアン
constexpr int IRON_HP   = 50;
constexpr int IRON_STR  = 10;
constexpr int IRON_VIT  = 10;

// タツ
constexpr int TATSU_HP   = 20;
constexpr int TATSU_STR  = 20;
constexpr int TATSU_VIT  = 20;
