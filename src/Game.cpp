#include "include/Game.h"

extern const int WINDOW_W = 1024;
extern const int WINDOW_H = 512;
const int FPS = 1000 / 30;

Game::Game()
    : mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true)
    , mNowScene(START), mStart(new Start(this))
{
}

bool Game::Initialize()
{
    // Initialize SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Initialize SDL_TTF
    sdlResult = TTF_Init();
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL_TTF: %s", TTF_GetError());
        return false;
    }

    mFont = TTF_OpenFont("assets/JF-Dot-K14.ttf", 40);
    if (!mFont) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return false;
    }

    // Create an SDL Window
    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)", // Window title
        100,                                   // Top left x-coordinate of window
        100,                                   // Top left y-coordinate of window
        WINDOW_W,                              // Width of window
        WINDOW_H,                              // Height of window
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

    return true;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        //TODO: 連続で反応しないように
        if(event.type == SDL_QUIT) {
            mIsRunning = false;
        }
        if(mNowScene == START)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_s:
                    SDL_Log("S\n");
                    mStart->Input();
                    break;
            }
        }
        else if(mNowScene == HOME)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_1:
                    SDL_Log("1\n");
                    mHome->Input(SDLK_1);
                    break;
                case SDLK_2:
                    SDL_Log("2\n");
                    mHome->Input(SDLK_2);
                    break;
                case SDLK_3:
                    SDL_Log("3\n");
                    mHome->Input(SDLK_3);
                    break;
                case SDLK_4:
                    SDL_Log("4\n");
                    mHome->Input(SDLK_4);
                    break;
                case SDLK_5:
                    SDL_Log("5\n");
                    mHome->Input(SDLK_5);
                    break;
                case SDLK_6:
                    SDL_Log("6\n");
                    mHome->Input(SDLK_6);
                    break;
                case SDLK_7:
                    SDL_Log("7\n");
                    mHome->Input(SDLK_7);
                    break;
                case SDLK_8:
                    SDL_Log("8\n");
                    mHome->Input(SDLK_8);
                    break;
                
            }
        }
        
    }

    // Get state of keyboard
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    // If escape is pressed, also end loop
    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }
}

void Game::UpdateGame()
{
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + FPS))
        ;

    // Delta time is the difference in ticks from last frame
    // (converted to seconds)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    // Clamp maximum delta time value
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    // Update tick counts (for next frame)
    mTicksCount = SDL_GetTicks();
}

void Game::GenerateOutput()
{
    // Set draw color to blue
    SDL_SetRenderDrawColor(
        mRenderer,
        0,  // R
        0,  // G
        0,  // B
        255 // A
    );

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    // TODO: now testing
    switch(mNowScene)
    {
        case START:
            mStart->Draw();
            break;
        case HOME:
            mHome->Draw();
            break;
    }

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
    TTF_Quit();

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

SDL_Renderer *Game::getRenderer()
{
    return mRenderer;
}

SDL_Window *Game::getWindow()
{
    return mWindow;;
}