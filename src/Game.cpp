#include "Game.h"

#include "Scene/1_Start.h"
#include "Scene/2_Home.h"
#include "Scene/3_DungeonMenu.h"
#include "Scene/4_Dungeon.h"

extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;

SDL_atomic_t frames;

/* 設定された間隔で平均フレームレートの計算と表示を行う */
Uint32 fps_timer_callback(Uint32 interval, void *data)
{
        // const Uint32 f = SDL_AtomicGet(&frames);
        // const iv = interval * 0.001f;

        /* 注意: printfがスレッドセーフであるかは環境に依存する */
// printf("%.2f\tfps\n", f / iv);

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
    Shutdown();
}

bool Game::Init()
{
    // Initialize SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if (sdlResult != 0)
    {
        printf("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Initialize SDL_IMG
    sdlResult = IMG_Init(IMG_INIT_PNG);
    if (!(sdlResult & IMG_INIT_PNG))
    {
        printf("Unable to initialize SDL_IMG: %s", IMG_GetError());
        return false;
    }

    // Initialize SDL_TTF
    sdlResult = TTF_Init();
    if (sdlResult != 0)
    {
        printf("Unable to initialize SDL_TTF: %s", TTF_GetError());
        return false;
    }

    mFontN = TTF_OpenFont("assets/JF-Dot-K14.ttf", 30);
    if (!mFontN) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return false;
    }
    mFontB = TTF_OpenFont("assets/JF-Dot-K14B.ttf", 30);
    if (!mFontB) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return false;
    }

    // Initialize SDL_Mix
    sdlResult = Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
    if (sdlResult < 0)
    {
        printf("Unable to initialize SDL_Mix: %s", Mix_GetError());
        return false;
    }
    
    sdlResult = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    if (sdlResult < 0)
    {
        printf("Unable to initialize SDL_mixer: %s", Mix_GetError());
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
printf("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Create SDL renderer
    gRenderer = SDL_CreateRenderer(
        gWindow, // Window to create renderer for
        -1,      // Usually -1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!gRenderer)
    {
printf("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    mClickEffect = Mix_LoadWAV("assets/click.mp3");
    if(mClickEffect == NULL) {
printf("Failed to load sound effect : %s", Mix_GetError());
        return false;
    }

    mMusic = Mix_LoadMUS("assets/dungeon.ogg");//Mix_LoadMUS("assets/BGM.mp3");
    if(mMusic == NULL) {
printf("Failed to load BGM : %s", Mix_GetError());
        return false;
    }

    mStart = new Start(this);
    mHome = new Home(this);
    mDungeonMenu = new DungeonMenu(this);
    mDungeon = new Dungeon(this);

    Mix_PlayMusic(mMusic, -1);

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

        // SDL_Log("Input");
        Input();
        // SDL_Log("Update");
        Update();
        // SDL_Log("Output");
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
    Button staticButton;
    SDL_PollEvent(&event);
    do
    {
        if(event.type != SDL_KEYDOWN && event.type != SDL_KEYUP)
        {
            continue;
        }
        if (event.type == SDL_QUIT)
        {
            mIsRunning = false;
        }
        switch (getNowScene())
        {
            case START:         
                mStart->Input(event);       
                break;

            case HOME:          
                mHome->Input(event);        
                break;

            case DUNGEON_MENU:  
                mDungeonMenu->Input(event); 
                break;

            case DUNGEON_AREA_DIVIDE:
            case DUNGEON_RRA:   
                mDungeon->Input(event);     
                break;
        }
    } while (SDL_PollEvent(&event));
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) mIsRunning = false;
}

//画面の切り替わり後はここから
void Game::Update()
{
    switch (getNowScene())
    {
        case START:         mStart->Update();             break;
        case HOME:          mHome->Update();              break;
        case DUNGEON_MENU:  mDungeonMenu->Update();       break;
        case DUNGEON_AREA_DIVIDE:
        case DUNGEON_RRA:   mDungeon->Update(); break;
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
        case DUNGEON_RRA:   mDungeon->Output(); break;
    }

    SDL_RenderPresent(gRenderer);
}

void Game::Shutdown()
{
    Mix_FreeChunk(mClickEffect);
    mClickEffect = NULL;

    Mix_HaltMusic();
    Mix_FreeMusic(mMusic);
    mMusic = NULL;
    Mix_CloseAudio();
    
    TTF_CloseFont(mFontN);
    TTF_CloseFont(mFontB);

    delete mStart;
    delete mHome;
    delete mDungeonMenu;
    delete mDungeon;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    TTF_Quit();
    Mix_Quit();

    SDL_Quit();
}
