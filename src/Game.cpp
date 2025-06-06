#include "Game.h"

extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;
extern TTF_Font *gFontN, *gFontB;
extern Mix_Chunk* gClickEffect;
extern SCENE gNowScene;

SDL_atomic_t frames;

/* 設定された間隔で平均フレームレートの計算と表示を行う */
Uint32 fps_timer_callback(Uint32 interval, void *data)
{
        // const Uint32 f = SDL_AtomicGet(&frames);
        // const iv = interval * 0.001f;

        /* 注意: SDL_Logがスレッドセーフであるかは環境に依存する */
// printf("%.2f\tfps\n", f / iv);

        /* フレームカウンタをリセットする */
        SDL_AtomicSet(&frames, 0);

        return interval;
}

Game::Game()
:mIsRunning(true)
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

    gFontN = TTF_OpenFont(resource_path("JF-Dot-K14.ttf"), 30);
    if (!gFontN) {
        SDL_Log("TTF_OpenFont: %s\n", TTF_GetError());
        return false;
    }
    gFontB = TTF_OpenFont(resource_path("JF-Dot-K14B.ttf"), 30);
    if (!gFontB) {
        SDL_Log("TTF_OpenFont: %s\n", TTF_GetError());
        return false;
    }

    // Initialize SDL_Mix
    sdlResult = Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
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
        "Rogue",                               // Window title
        SDL_WINDOWPOS_UNDEFINED,               // Top left x-coordinate of window
        SDL_WINDOWPOS_UNDEFINED,               // Top left y-coordinate of window
        SCREEN_WIDTH,                          // Width of window
        SCREEN_HEIGHT,                         // Height of window
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

    gClickEffect = Mix_LoadWAV(resource_path("click.mp3"));
    if(gClickEffect == NULL) {
        SDL_Log("Failed to load sound effect : %s", Mix_GetError());
        return false;
    }

    mMusic = Mix_LoadMUS(resource_path("dungeon.ogg"));//Mix_LoadMUS("assets/BGM.mp3");
    if(mMusic == NULL) {
        SDL_Log("Failed to load BGM : %s", Mix_GetError());
        return false;
    }

    mStart = new Start();
    mHome = new Home();
    mDungeonMenu = new DungeonMenu();
    mDungeon = new Dungeon();
    mCongratulations = new Congratulations();
    mGameOver = new GameOver();

    Mix_PlayMusic(mMusic, -1);

    return true;
}

void Game::RunLoop()
{
    // SDL_AddTimer(1000, fps_timer_callback, NULL);
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
        // SDL_AtomicAdd(&frames, 1);

        // FPS調整
        afterTime = SDL_GetTicks();
        elapsedTime = afterTime - beforTime;
        if(SPF > elapsedTime)
            SDL_Delay(SPF - elapsedTime);
    }
}

void Game::Input()
{
    Button staticButton;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            mIsRunning = false;
        }
        if(event.type != SDL_KEYDOWN && event.type != SDL_KEYUP)
        {
            continue;
        }
        
        switch (gNowScene)
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
            
            case CONGRATULATIONS:
                mCongratulations->Input(event);
                break;
            
            case GAME_OVER:
                mGameOver->Input(event);
                break;
        }
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) mIsRunning = false;
}

//画面の切り替わり後はここから
void Game::Update()
{
    switch (gNowScene)
    {
        case START:         
            mStart->Update();
            break;
        case HOME:          
            mHome->Update();
            break;
        case DUNGEON_MENU:  
            mDungeonMenu->Update();
            break;
        case DUNGEON_AREA_DIVIDE:
        case DUNGEON_RRA:   
            mDungeon->Update();
            break;
        case CONGRATULATIONS:
            mCongratulations->Update();
            break;
        case GAME_OVER:
            mGameOver->Update();
            break;
    }
}

void Game::Output()
{
    SDL_SetRenderDrawColor(gRenderer, 50, 50, 50, 255);
    SDL_RenderClear(gRenderer);

    switch (gNowScene)
    {
        case START:
            mStart->Output();
            break;
        case HOME:
            mHome->Output();
            break;
        case DUNGEON_MENU:
            mDungeonMenu->Output();
            break;
        case DUNGEON_AREA_DIVIDE:
        case DUNGEON_RRA:
            mDungeon->Output();
            break;
        case CONGRATULATIONS:
            mCongratulations->Output();
            break;
        case GAME_OVER:
            mGameOver->Output();
            break;
    }

    SDL_RenderPresent(gRenderer);
}

void Game::Shutdown()
{
    Mix_HaltMusic();
    Mix_FreeMusic(mMusic);
    mMusic = NULL;
    Mix_FreeChunk(gClickEffect);
    gClickEffect = NULL;
    Mix_CloseAudio();

    
    delete mStart;
    mStart = nullptr;
    delete mHome;
    mHome = nullptr;
    delete mDungeonMenu;
    mDungeonMenu = nullptr;
    delete mDungeon;
    mDungeon = nullptr;
    delete mCongratulations;
    mCongratulations = nullptr;
    delete mGameOver;
    mGameOver = nullptr;

    // try {
    //     delete mStart;
    //     delete mHome;
    //     delete mDungeonMenu;
    //     delete mDungeon;
    //     delete mCongratulations;
    //     delete mGameOver;

    // } catch (const std::exception& e) {
    //     std::cout << "class" << std::endl;
    //     std::cout << e.what() << std::endl;
    // }

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    TTF_Quit();
    Mix_Quit();

    SDL_Quit();
}
