#ifndef __DUNGEON
#define __DUNGEON

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <glm/glm.hpp>

#include "UI/Button.h"
#include "Functions/Color.h"

#include "GenerateDungeon/AreaDivide.h"

#include "GenerateDungeon/Player.h"
#include "GenerateDungeon/Enemy.h"

#include "GenerateDungeon/Const.h"

#include "Game.h"

class Dungeon
{
public:
    Dungeon(class Game *game);

    void Input(SDL_Event event);
    void Update();
    void Output();

// ダンジョン生成
    // void init();
    // void run();
    // void quit();

    // void update();

    // bool inDungeon = true;

private:
    void LoadData();
    void PlayMusic();

    class Game *mGame;

// ダンジョン生成
    // bool goNextFloor = false;
    // char input = 'a';
    // AreaDivide areaDivide;

    // Player player = Player(0, 0, PLAYER_HP, PLAYER_STR, PLAYER_VIT);
    // std::vector<Enemy> enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(SLIME));
    // std::vector<Enemy> deadEnemies;

    // int isOtherPos(glm::vec2 _pos);
    // bool canGetOn(glm::vec2 _pos);
    // glm::vec2 getRandomPos(int _roomCount);
    // glm::vec2 getFrontPos(glm::vec2 _pos, DIRECTION _dir);
    // Enemy& whichEnemy(glm::vec2 _pos);
};

#endif __DUNGEON