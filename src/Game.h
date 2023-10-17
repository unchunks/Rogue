#pragma once

#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

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

    TTF_Font *getFont() const { return mFont; }
    Mix_Chunk *getClickEffect() const { return mClickEffect; }

    void setNowScene(const SCENE nextScene) { mNowScene = nextScene; }
    SCENE getNowScene() const { return mNowScene; }

private:
    void Input();
    void Update();
    void Output();

    SDL_Event event;
    TTF_Font *mFont;
    Mix_Chunk* mClickEffect;

    bool mIsRunning;

    class Start *mStart;
    class Home *mHome;
    class DungeonMenu *mDungeonMenu;
    class Dungeon *mDungeon;
    SCENE mNowScene;
};
