#include "Game.h"

#include "1_Start.h"
#include "2_Home.h"
#include "3_DungeonMemu.h"
#include "4_Dungeon.h"

extern const int WIN_W = 1024;
extern const int WIN_H = 512;

Game::Game()
:mWindow(nullptr)
,mRenderer(NULL)
,mIsRunning(true)
,mNowScene(START)
{

}

Game::~Game()
{
    
}

bool Game::Init()
{
    // Initialize SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Initialize SDL_IMG
    sdlResult = IMG_Init(IMG_INIT_PNG);
    if (!(sdlResult & IMG_INIT_PNG))
    {
        SDL_Log("Unable to initialize SDL_IMG: %s", IMG_GetError());
        return false;
    }

    // Initialize SDL_TTF
    sdlResult = TTF_Init();
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL_TTF: %s", TTF_GetError());
        return false;
    }

    mFont = TTF_OpenFont("assets/JF-Dot-K14.ttf", 30);
    if (!mFont) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return false;
    }

    // Initialize SDL_Mix
    sdlResult = Mix_Init(MIX_INIT_MP3);
    if (sdlResult < 0)
    {
        SDL_Log("Unable to initialize SDL_Mix: %s", Mix_GetError());
        return false;
    }
    sdlResult = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    if (sdlResult < 0)
    {
        SDL_Log("Unable to initialize SDL_mixer: %s", Mix_GetError());
        return false;
    }


    // Create an SDL Window
    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)", // Window title
        100,                                   // Top left x-coordinate of window
        100,                                   // Top left y-coordinate of window
        WIN_W,                              // Width of window
        WIN_H,                              // Height of window
        0                                      // Flags (0 for no flags set)
    );

    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Create SDL renderer
    mRenderer = SDL_CreateRenderer(
        mWindow, // Window to create renderer for
        -1,      // Usually -1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    mClickEffect = Mix_LoadWAV("assets/click.mp3");
    if(mClickEffect == NULL) {
        SDL_Log("Failed to load sound effect : %s", Mix_GetError());
        return false;
    }

    mStart = new Start(this);
    // mHome = new Home(this);
    // mDungeonMenu = new DungeonMenu(this);
    // mDungeon = new Dungeon(this);

    return true;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        Input();
        Update();
        Output();
    }
}

void Game::Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type != SDL_KEYDOWN) continue;
        if (event.type == SDL_QUIT) mIsRunning = false;
        switch (getNowScene())
        {
        case START:         mStart->Input();        break;
        // case HOME:          mHome->Input();         break;
        // case DUNGEON_MENU:  mDungeonMenu->Input();  break;
        // case DUNGEON:       mDungeon->Input();      break;
        }
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) mIsRunning = false;
}

void Game::Update()
{
    switch (getNowScene())
    {
    case START:         mStart->Update();        break;
    // case HOME:          mHome->Update();         break;
    // case DUNGEON_MENU:  mDungeonMenu->Update();  break;
    // case DUNGEON:       mDungeon->Update();      break;
    }
}

void Game::Output()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    switch (getNowScene())
    {
    case START:         mStart->Output();        break;
    // case HOME:          mHome->Output();         break;
    // case DUNGEON_MENU:  mDungeonMenu->Output();  break;
    // case DUNGEON:       mDungeon->Output();      break;
    }

    SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
    Mix_FreeChunk(mClickEffect);
    mClickEffect = NULL;

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    mWindow = NULL;
    mRenderer = NULL;
    
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}