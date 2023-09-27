#include "Game.h"

#include "Scene/1_Start.h"
#include "Scene/2_Home.h"
#include "Scene/3_DungeonMenu.h"
#include "Scene/4_Dungeon.h"

constexpr double FPS = 1;
constexpr double SPF = 1000.0 / FPS;

extern const int WIN_W = 1024;
extern const int WIN_H = 512;

SDL_atomic_t frames;
/* 設定された間隔で平均フレームレートの計算と表示を行う */
Uint32 fps_timer_callback(Uint32 interval, void *data)
{
        const float f = SDL_AtomicGet(&frames);
        const float iv = interval * 0.001f;

        /* 注意: printfがスレッドセーフであるかは環境に依存する */
        printf("%.2f\tfps\n", f / iv);

        /* フレームカウンタをリセットする */
        SDL_AtomicSet(&frames, 0);

        return interval;
}

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
SDL_Log("TTF_OpenFont: %s\n", TTF_GetError());
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
    mHome = new Home(this);
    mDungeonMenu = new DungeonMenu(this);
    mDungeon = new Dungeon(this);

    return true;
}

void Game::RunLoop()
{
    int beforTime = SDL_GetTicks();
    int afterTime = SDL_GetTicks();
    int elapsedTime = 0;
// 別スレッドで1秒毎にフレームレートを表示
// SDL_AddTimer(1000, fps_timer_callback, NULL);
    while (mIsRunning)
    {
        beforTime = SDL_GetTicks();

        Input();
        Update();
        Output();

        /* フレーム数に1を加える */
        SDL_AtomicAdd(&frames, 1);
        afterTime = SDL_GetTicks();
        elapsedTime = afterTime - beforTime;
        if(SPF > elapsedTime)
            SDL_Delay(SPF - elapsedTime);
    }
}

void Game::Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // if (event.type != SDL_KEYDOWN) continue;
        if (event.type == SDL_QUIT) mIsRunning = false;
SDL_Log("%d\n", getNowScene());
        switch (getNowScene())
        {
            case START:         mStart->Input(event);       break;
            case HOME:          mHome->Input(event);        break;
            case DUNGEON_MENU:  mDungeonMenu->Input(event); break;
            case DUNGEON:       mDungeon->Input(event);     break;
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
        case HOME:          mHome->Update();         break;
        case DUNGEON_MENU:  mDungeonMenu->Update();  break;
        case DUNGEON:       mDungeon->Update();      break;
    }
}

void Game::Output()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    switch (getNowScene())
    {
        case START:         mStart->Output();        break;
        case HOME:          mHome->Output();         break;
        case DUNGEON_MENU:  mDungeonMenu->Output();  break;
        case DUNGEON:       mDungeon->Output();      break;
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