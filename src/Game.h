#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "Scene/1_Start.h"
#include "Scene/2_Home.h"
#include "Scene/3_DungeonMenu.h"
#include "Scene/4_Dungeon.h"

enum SCENE
{
    START,
    HOME,
    DUNGEON_MENU,
    DUNGEON_AREA_DIVIDE,
    DUNGEON_RRA,
};

class Game
{
public:
    Game();
    ~Game();

    bool Init();
    void RunLoop();
    void Shutdown();

    TTF_Font *getFontN() const { return mFontN; }
    TTF_Font *getFontB() const { return mFontB; }
    Mix_Chunk *getClickEffect() const { return mClickEffect; }

    void setNowScene(const SCENE nextScene) { mNowScene = nextScene; }
    SCENE getNowScene() const { return mNowScene; }

private:
    void Input();
    void Update();
    void Output();

    SDL_Event event;
    TTF_Font *mFontN, *mFontB;
    Mix_Chunk* mClickEffect;
    Mix_Music* mMusic;

    bool mIsRunning;

    class Start *mStart;
    class Home *mHome;
    class DungeonMenu *mDungeonMenu;
    class Dungeon *mDungeon;
    SCENE mNowScene;
};
