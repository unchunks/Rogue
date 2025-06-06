#pragma once

#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../SDL2/SDL_mixer.h"

#include "Functions/Util.h"

#include "Scene/1_Start.h"
#include "Scene/2_Home.h"
#include "Scene/3_DungeonMenu.h"
#include "Scene/4_Dungeon.h"
#include "Scene/5_Congratulations.h"
#include "Scene/6_GameOver.h"

class Game
{
public:
    Game();
    ~Game();

    bool Init();
    void RunLoop();
    void Shutdown();

private:
    void Input();
    void Update();
    void Output();

    SDL_Event event;
    Mix_Music* mMusic;

    bool mIsRunning;

    class Start *mStart;
    class Home *mHome;
    class DungeonMenu *mDungeonMenu;
    class Dungeon *mDungeon;
    class Congratulations *mCongratulations;
    class GameOver *mGameOver;
};
