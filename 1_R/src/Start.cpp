#include "include/Start.h"

extern int WINDOW_W;
extern int WINDOW_H;

const int S_BUTTON_W = 500;
const int S_BUTTON_H = 50;

Game *start_g;
void startButton()
{
    Mix_PlayChannel(-1, mClickEffect, 0);
    start_g->mNowScene = Game::scene::HOME;
    SDL_Log("Clicked\n");
    SDL_Log("%d\n", start_g->mNowScene);
}

Start::Start(Game *game)
{
    mGame = game;
    start_g = game;
    LoadMedia();
    
    mStartButton.onClick = &startButton;
    mStartButton.create(
        mGame->getRenderer(),
        (WINDOW_W - S_BUTTON_W) / 2,
        (WINDOW_H - S_BUTTON_H) / 3,
        S_BUTTON_W,
        S_BUTTON_H,
        "S : Game Start",
        mGame->mFont,
        Color::SDL_blue,
        Color::SDL_white);
}

void Start::LoadMedia()
{
    // mTexture_GameStart = loadTexture("assets/gameStart.png", mGame->getRenderer());
    // mTexture_GameLoad = loadTexture("assets/gameLoad.png", mGame->getRenderer());
}

void Start::Draw()
{
    mStartButton.Draw();
}

void Start::Input(SDL_KeyCode keyCode)
{
    SDL_Log("Input\n");
    switch(keyCode)
    {
    case SDLK_s:
        mStartButton.onClick();
        break;
    }
}

void Start::PlayMusic()
{
}

void Start::MoveScene()
{
}