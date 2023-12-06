#include "Scene/3_DungeonMenu.h"

extern SDL_Renderer *gRenderer;

const int BUTTON_W = 250;
const int BUTTON_H = 60;

Game *dungeonMenu_g;
void area_divide()
{
    Mix_PlayChannel(-1, dungeonMenu_g->getClickEffect(), 0);
    dungeonMenu_g->setNowScene(SCENE::DUNGEON_AREA_DIVIDE);
    printf("area_divide\n");
}
void RRA()
{
    Mix_PlayChannel(-1, dungeonMenu_g->getClickEffect(), 0);
    dungeonMenu_g->setNowScene(SCENE::DUNGEON_RRA);
    printf("RRA\n");
}

DungeonMenu::DungeonMenu(Game *game)
{
    mGame = game;
    dungeonMenu_g = game;
    LoadData();

    mAreaDivide.onClick = &area_divide;
    mRRA.onClick = &RRA;
    mAreaDivide.create(
        gRenderer,
        (SCREEN_WIDTH / 2 - BUTTON_W) / 2,
        (SCREEN_HEIGHT - BUTTON_H) / 2,
        BUTTON_W,
        BUTTON_H,
        "1 : 区域分割法",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mRRA.create(
        gRenderer,
        (SCREEN_WIDTH * 1.5 - BUTTON_W) / 2,
        (SCREEN_HEIGHT - BUTTON_H) / 2,
        BUTTON_W,
        BUTTON_H,
        "2 : 　 RRA 　",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
}

void DungeonMenu::LoadData()
{
}

void DungeonMenu::Input(SDL_Event event)
{
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_1:
            mAreaDivide.press();
            break;
        case SDLK_2:
            mRRA.press();
            break;
        }
    }
    else
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_1:
            if(mAreaDivide.isPressed)
            {
                mAreaDivide.release();
                mAreaDivide.onClick();
            }
            break;
        case SDLK_2:
            if(mRRA.isPressed)
            {
                mRRA.release();
                mRRA.onClick();
            }
            break;
        }
    }
}

void DungeonMenu::Update()
{

}

void DungeonMenu::Output()
{
    mAreaDivide.Draw();
    mRRA.Draw();
}

void DungeonMenu::PlayMusic()
{
}
