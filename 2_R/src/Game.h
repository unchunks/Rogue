#pragma once

#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Functions/Math.h"

class Game
{
public:
    enum SCENE
    {
        START,
        HOME,
        DUNGEON_MENU,
        DUNGEON,
    };

    Game();
    ~Game();

    bool Init();
    void RunLoop();
    void Shutdown();

    SDL_Window *getWindow() const { return mWindow; }
    SDL_Renderer *getRenderer() const { return mRenderer; }
    TTF_Font *getFont() const { return mFont; }
    Mix_Chunk *getClickEffect() const { return mClickEffect; }

    SCENE setNowScene(const SCENE nextScene) { mNowScene = nextScene; }
    SCENE getNowScene() const { return mNowScene; }

private:
    void Input();
    void Update();
    void Output();

    SDL_Window *mWindow;
    SDL_Surface *mSurface;
    SDL_Renderer *mRenderer;
    TTF_Font *mFont;
    Mix_Chunk* mClickEffect;

    bool mIsRunning;

    class Start *mStart;
    // class Home *mHome;
    // class DungeonMenu *mDungeonMenu;
    // class Dungeon *mDungeon;
    SCENE mNowScene;
};