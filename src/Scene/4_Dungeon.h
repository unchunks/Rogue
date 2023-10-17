#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <glm/glm.hpp>

#include "../UI/Button.h"
#include "../Functions/Color.h"
#include "../GenerateDungeon/Const.h"

#include "../GenerateDungeon/AreaDivide.h"
#include "../GenerateDungeon/RRA.h"

#include "../GenerateDungeon/Player.h"
#include "../GenerateDungeon/Enemy.h"
#include "../GenerateDungeon/Const.h"
#include "../GenerateDungeon/Tile.h"
#include "../GenerateDungeon/LTexture.h"

#include "../Game.h"

class Dungeon
{
public:
    Dungeon(class Game *game);

    void Input(SDL_Event event);
    void Update(int anim_frame);
    void Output(int anim_frame);

// ダンジョン生成
    void InitDungeon();
    void quit(Tile* tiles[]);

    bool inDungeon;

// TODO: ダンジョンを画像で表示

private:
    bool LoadData();
    void PlayMusic();

    class Game *mGame;

    bool playerMoved = false;

// ダンジョン生成
    bool goNextFloor;
    char input = 'a';

    AreaDivide areaDivide;
    RRA rra;

    Player player = Player(0, 0, PLAYER_HP, PLAYER_STR, PLAYER_VIT);
    std::vector<Enemy> enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(SLIME));
    std::vector<Enemy> deadEnemies;

    int isOtherPos(glm::vec2 _pos);
    bool canGetOn(glm::vec2 _pos);
    glm::vec2 getRandomPos(int _roomCount);
    glm::vec2 getFrontPos(glm::vec2 _pos, DIRECTION _dir);
    Enemy& whichEnemy(glm::vec2 _pos);

    //The level tiles
	Tile* tileSet[ TOTAL_TILES ];

    SDL_Rect camera;
};

//ボックスとタイルの衝突判定
bool touchesWall( SDL_Rect box, Tile* tiles[] );

//.mapファイルからタイルをセット
bool setTiles( Tile *tiles[] );

