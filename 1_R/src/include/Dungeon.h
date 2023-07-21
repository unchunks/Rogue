#pragma once

#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>

#include "Data.h"
#include "Game.h"
#include "GenerateDungeon/Map.h"
#include "GenerateDungeon/gen1.h"
#include "GenerateDungeon/gen2.h"
#include "../omits/Button.h"
#include "../omits/Color.h"

class Dungeon
{
public:
    Dungeon(class Game *game);
    ~Dungeon();

    void Input(SDL_KeyCode keyCode);
    void Draw();

    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    CELL_TYPE originalMap[mapH][mapW];
    CELL_TYPE changedMap[mapH][mapW];

private:
    void LoadMedia();
    int GenerateDungeon();
    void CreateActor(int roomCount);
    void PlayMusic();

    void Update();

    void UnloadData();
    void MoveScene();

    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    bool mIsRunning;
    bool mUpdatingActors;
    char input;

    class Player* mPlayer;

    class Game *mGame;

    Button mSelectButtons[2];
};