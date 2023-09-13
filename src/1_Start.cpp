#include "1_Start.h"

extern int WIN_W;
extern int WIN_H;

const int BUTTON_W = 500;
const int BUTTON_H = 50;

Game *start_g;
void startButton()
{
    Mix_PlayChannel(-1, start_g->getClickEffect(), 0);
    start_g->setNowScene(SCENE::HOME);
    SDL_Log("startButton\n");
}

Start::Start(Game *game)
{
    start_g = game;
    mGame = game;
    LoadData();

    mStartButton.onClick = &startButton;
    mStartButton.create(
        mGame->getRenderer(),
        (WIN_W - BUTTON_W) / 2,
        (WIN_H - BUTTON_H) / 3,
        BUTTON_W,
        BUTTON_H,
        "S : Game Start",
        mGame->getFont(),
        Color::SDL_blue,
        Color::SDL_white
    );
}

void Start::Input(SDL_Event event)
{
    switch(event.key.keysym.sym)
    {
    case SDLK_s:    mStartButton.onClick();  break;
    }
}

void Start::Update()
{

}

void Start::Output()
{
    mStartButton.Draw();
}

void Start::LoadData()
{

}

void Start::PlayMusic()
{

}