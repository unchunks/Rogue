#ifndef __CONST
#define __CONST

constexpr int FLOOR_W = 40;
constexpr int FLOOR_H = 40;
constexpr int BUFF_FLOOR_W = FLOOR_W + 2;
constexpr int BUFF_FLOOR_H = FLOOR_H + 2;
constexpr int AREA_MAX = 10;
constexpr int AREA_SIZE_MIN = 10;

constexpr int NUM_ENEMY = 3;
constexpr int ENEMY_FIND_RANGE = 10;
constexpr int ENEMY_SEARCH_INTERVAL = 10;

// プレイヤー
constexpr int PLAYER_HP = 100;
constexpr int PLAYER_STR = 20;
constexpr int PLAYER_VIT = 10;

// TODO: キャラを作ったらEnum.hに名前を追加
// スライム
constexpr int   SLIME_HP   = 50;
constexpr int   SLIME_STR  = 2;
constexpr int   SLIME_VIT  = 5;
constexpr char* SLIME_ICON = "Ω ";

// ゴブリン
constexpr int   GOBLIN_HP   = 80;
constexpr int   GOBLIN_STR  = 5;
constexpr int   GOBLIN_VIT  = 8;
constexpr char* GOBLIN_ICON = "ಪ ";

// ドラゴン
constexpr int   DRAGON_HP   = 1000;
constexpr int   DRAGON_STR  = 70;
constexpr int   DRAGON_VIT  = 30;
constexpr char* DRAGON_ICON = "ඣ ";

#endif __CONST