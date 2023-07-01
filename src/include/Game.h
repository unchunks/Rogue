#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Data.h"
#include "Start.h"
#include "Home.h"
#include "DungeonMenu.h"

struct Vector2
{
    float x;
    float y;
};

// Game class
class Game
{
public:
    Game();
    // Initialize the game
    bool Initialize();
    // Runs the game loop until the game is over
    void RunLoop();
    // Shutdown the game
    void Shutdown();

    SDL_Renderer *getRenderer() { return mRenderer; }
    SDL_Window *getWindow() { return mWindow; }
    TTF_Font *mFont;

    enum scene {
        START,
        HOME,
        DUNGEONMENU,
        DUNGEON
    };

    scene mNowScene;

private:
    // Helper functions for the game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // Window created by SDL
    SDL_Window *mWindow;

    SDL_Surface *mSurface;
    
    // Renderer for 2D drawing
    SDL_Renderer *mRenderer;
    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;

    class Start *mStart;
    class Home *mHome;
    class DungeonMenu *mDungeonMenu;
};