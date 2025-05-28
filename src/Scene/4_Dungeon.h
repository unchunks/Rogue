#pragma once

#include <iostream>
#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../../Ivec2.h"

#include "../UI/Log.h"
#include "../Functions/Color.h"
#include "../GenerateDungeon/Const.h"
#include "../GenerateDungeon/Enum.h"

#include "../GenerateDungeon/AreaDivide.h"
#include "../GenerateDungeon/RRA.h"

#include "../GenerateDungeon/Player.h"
#include "../GenerateDungeon/Enemy.h"
#include "../GenerateDungeon/Const.h"
#include "../GenerateDungeon/Tile.h"
#include "../GenerateDungeon/LTexture.h"

#include "../Game.h"

enum GOAL_TYPE
{
    RANDOM_POS,
    PLAYER_POS
};

class Dungeon
{
public:
    Dungeon();
    ~Dungeon();

    void Input(SDL_Event event);
    void Update();
    void Output();

// ダンジョン生成
    void InitDungeon();
    void quit();

    bool in_dungeon;

private:
    bool LoadData();
    void PlayMusic();

    //.mapファイルからタイルをセット
    bool setTiles();

    void updateEnemyRoute(Enemy& _enemy, GOAL_TYPE _goleType);

    /// @brief 座標に自身以外のキャラが居るか
    /// @param _data_pos データ系座標
    /// @return 座標にいるキャラの数
    int isOtherPos(Ivec2 _data_pos);

    /// @brief 引数の位置に移動できるか判定する
    /// @param _data_pos データ系座標
    /// @return 移動できるなら true
    bool canGetOn(Ivec2 _data_pos);

    /// @brief ランダムな座標を返す
    /// @param _roomCount 
    /// @return データ系座標
    Ivec2 getRandomDataPos(int _roomCount);

    /// @brief 座標にいる敵を返す
    /// @param _data_pos 
    /// @return 敵の参照を返す
    Enemy& whichEnemy(Ivec2 _data_pos);

    // ダンジョン生成フラグ
    bool go_next_floor;

    CELL_TYPE floor[FLOOR_H][FLOOR_W];

    AreaDivide area_divide;
    RRA rra;

    Player player = Player(0, 0, PLAYER_HP, PLAYER_STR, PLAYER_VIT);
    std::vector<Enemy> enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(DEKA, 0));

	std::vector<Tile> tileSet;

    SDL_Rect camera;
    std::vector<SDL_Rect> mPlayerSpriteClips;
    std::vector<std::vector<SDL_Rect>> enemy_sprite_clips;

    CHAR_TYPE nowTurn;

    Log log;

    int floor_num;
};

