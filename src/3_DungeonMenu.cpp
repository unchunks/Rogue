#include "3_DungeonMenu.h"

extern int WIN_W;
extern int WIN_H;

const int BUTTON_W = 250;
const int BUTTON_H = 60;

Game *dungeonMenu_g;
void areaDivide()
{
    Mix_PlayChannel(-1, dungeonMenu_g->getClickEffect(), 0);
    dungeonMenu_g->setNowScene(SCENE::DUNGEON);
    SDL_Log("areaDivide\n");
    SDL_Log("%d\n", dungeonMenu_g->setNowScene(SCENE::DUNGEON));
}
void RRA()
{
    Mix_PlayChannel(-1, dungeonMenu_g->getClickEffect(), 0);
    dungeonMenu_g->setNowScene(SCENE::DUNGEON);
    SDL_Log("RRA\n");
    SDL_Log("%d\n", dungeonMenu_g->setNowScene(SCENE::DUNGEON));
}

DungeonMenu::DungeonMenu(Game *game)
{
    mGame = game;
    dungeonMenu_g = game;
    LoadData();

    mAreaDivide.onClick = &areaDivide;
    mRRA.onClick = &RRA;
    mAreaDivide.create(
        mGame->getRenderer(),
        (WIN_W / 2 - BUTTON_W) / 2,
        (WIN_H - BUTTON_H) / 2,
        BUTTON_W,
        BUTTON_H,
        "1 : 区域分割法",
        mGame->getFont(),
        Color::SDL_blue,
        Color::SDL_white);
    mRRA.create(
        mGame->getRenderer(),
        (WIN_W * 1.5 - BUTTON_W) / 2,
        (WIN_H - BUTTON_H) / 2,
        BUTTON_W,
        BUTTON_H,
        "2 : RRA",
        mGame->getFont(),
        Color::SDL_blue,
        Color::SDL_white);
}

void DungeonMenu::LoadData()
{
}

void DungeonMenu::Input(SDL_Event event)
{
    switch(event.key.keysym.sym)
    {
    case SDLK_1:    mAreaDivide.onClick();  break;
    case SDLK_2:    mRRA.onClick();         break;
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
