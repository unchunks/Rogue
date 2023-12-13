#include "Scene/1_Start.h"

extern SDL_Renderer *gRenderer;
extern TTF_Font *gFontB;
extern Mix_Chunk* gClickEffect;
extern SCENE gNowScene;

const int BUTTON_W = 500;
const int BUTTON_H = 80;

void startButton()
{
    Mix_PlayChannel(-1, gClickEffect, 0);
    gNowScene = SCENE::HOME;
    printf("startButton\n");
}

Start::Start()
{
    SDL_Log("Start constructor");
    LoadData();

    mStartButton.onClick = &startButton;
    mStartButton.create(
        gRenderer,
        (SCREEN_WIDTH - BUTTON_W) / 2,
        (SCREEN_HEIGHT - BUTTON_H) / 2,
        BUTTON_W,
        BUTTON_H,
        "S : Game Start",
        gFontB,
        Color::SDL_blue,
        Color::SDL_white
    );
}

Start::~Start()
{
}

void Start::LoadData()
{
}

void Start::Input(SDL_Event event)
{
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_s:
                mStartButton.press();
                break;
        }
        return;
    }
    switch(event.key.keysym.sym)
    {
        case SDLK_s:
            if(mStartButton.isPressed)
            {
                mStartButton.release();
                mStartButton.onClick();
            }
            break;
    }
}

void Start::Update()
{

}

void Start::Output()
{
    mStartButton.Draw();
}

void Start::PlayMusic()
{

}

