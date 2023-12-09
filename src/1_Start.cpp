#include "Scene/1_Start.h"

extern SDL_Renderer *gRenderer;

const int BUTTON_W = 500;
const int BUTTON_H = 80;

Game *start_g;
void startButton()
{
    Mix_PlayChannel(-1, start_g->getClickEffect(), 0);
    start_g->setNowScene(SCENE::HOME);
    printf("startButton\n");
}

Start::Start(Game *game)
{
    start_g = game;
    mGame = game;
    LoadData();

    mStartButton.onClick = &startButton;
    mStartButton.create(
        gRenderer,
        (SCREEN_WIDTH - BUTTON_W) / 2,
        (SCREEN_HEIGHT - BUTTON_H) / 2,
        BUTTON_W,
        BUTTON_H,
        "S : Game Start",
        mGame->getFontB(),
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
        case SDLK_s:    mStartButton.press();  break;
        }
    }
    else
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_s:
            if(mStartButton.isPressed)
            {
                mStartButton.release(); mStartButton.onClick();
            }
            break;
        }
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