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

enum GOAL_TYPE
{
    RANDOM_POS,
    PLAYER_POS
};

class Dungeon
{
public:
    Dungeon(class Game *game);
    ~Dungeon();

    void Input(SDL_Event event);
    void Update();
    void Output();

// ダンジョン生成
    void InitDungeon();
    void quit();

    bool inDungeon;

private:
    bool LoadData();
    void PlayMusic();

    //.mapファイルからタイルをセット
    bool setTiles();

    void updateEnemyRoute(Enemy& _enemy, GOAL_TYPE _goleType);

    class Game *mGame;


// ダンジョン生成
    bool goNextFloor;
    char input = 'a';

    AreaDivide areaDivide;
    RRA rra;

    Player player = Player(0, 0, PLAYER_HP, PLAYER_STR, PLAYER_VIT);
    std::vector<Enemy> enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(DEKA));
    std::vector<Enemy> deadEnemies;

    int isOtherPos(glm::vec2 _pos);
    bool canGetOn(glm::vec2 _pos);
    glm::vec2 getRandomPos(int _roomCount);
    glm::vec2 getFrontPos(glm::vec2 _pos, DIRECTION _dir);
    Enemy& whichEnemy(glm::vec2 _pos);

    //The level tiles
	std::vector<Tile> tileSet;

    SDL_Rect camera;
    std::vector<SDL_Rect> mPlayerSpriteClips;
    std::vector<std::vector<SDL_Rect>> mEnemySpriteClips;
};

