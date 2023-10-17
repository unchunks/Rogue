#include "Game.h"

#include "Scene/1_Start.h"
#include "Scene/2_Home.h"
#include "Scene/3_DungeonMenu.h"
#include "Scene/4_Dungeon.h"

extern SDL_Window *gWindow = nullptr;
extern SDL_Surface *gSurface = nullptr;
extern SDL_Renderer *gRenderer = nullptr;

SDL_atomic_t frames;

//現在のアニメーション フレーム
int anim_frame = 0;

/* 設定された間隔で平均フレームレートの計算と表示を行う */
Uint32 fps_timer_callback(Uint32 interval, void *data)
{
        const float f = SDL_AtomicGet(&frames);
        const float iv = interval * 0.001f;

        /* 注意: printfがスレッドセーフであるかは環境に依存する */
// SDL_Log("%.2f\tfps\n", f / iv);

        /* フレームカウンタをリセットする */
        SDL_AtomicSet(&frames, 0);

        return interval;
}

Game::Game()
:mIsRunning(true)
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
    gWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)", // Window title
        100,                                   // Top left x-coordinate of window
        100,                                   // Top left y-coordinate of window
        SCREEN_WIDTH,                              // Width of window
        SCREEN_HEIGHT,                              // Height of window
        0                                      // Flags (0 for no flags set)
    );

    if (!gWindow)
    {
SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Create SDL renderer
    gRenderer = SDL_CreateRenderer(
        gWindow, // Window to create renderer for
        -1,      // Usually -1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!gRenderer)
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
    SDL_AddTimer(1000, fps_timer_callback, NULL);
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

        //次のフレームに移動
		anim_frame++;
		//サイクルアニメーション
		if( anim_frame >= ANIMATION_FRAMES / ANIM_SPEED * FPS )
			anim_frame = 0;

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
    while (SDL_PollEvent(&event))
    {
        // if (event.type != SDL_KEYDOWN) continue;
        if (event.type == SDL_QUIT) mIsRunning = false;
// SDL_Log("%d\n", getNowScene());
        switch (getNowScene())
        {
            case START:         mStart->Input(event);       break;
            case HOME:          mHome->Input(event);        break;
            case DUNGEON_MENU:  mDungeonMenu->Input(event); break;
            case DUNGEON_AREA_DIVIDE:
            case DUNGEON_RRA:   mDungeon->Input(event);     break;
        }
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) mIsRunning = false;
}

void Game::Update()
{
    switch (getNowScene())
    {
        case START:         mStart->Update();             break;
        case HOME:          mHome->Update();              break;
        case DUNGEON_MENU:  mDungeonMenu->Update();       break;
        case DUNGEON_AREA_DIVIDE:
        case DUNGEON_RRA:   mDungeon->Update(anim_frame); break;
    }
}

void Game::Output()
{
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    switch (getNowScene())
    {
        case START:         mStart->Output();             break;
        case HOME:          mHome->Output();              break;
        case DUNGEON_MENU:  mDungeonMenu->Output();       break;
        case DUNGEON_AREA_DIVIDE:
        case DUNGEON_RRA:   mDungeon->Output(anim_frame); break;
    }

    SDL_RenderPresent(gRenderer);
}

void Game::Shutdown()
{
    Mix_FreeChunk(mClickEffect);
    mClickEffect = NULL;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}