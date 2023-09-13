#include "4_Dungeon.h"

extern int WIN_W;
extern int WIN_H;

const int BUTTON_W = 500;
const int BUTTON_H = 50;

Game *dungeon_g;
// void startButton()
// {
//     Mix_PlayChannel(-1, dungeon_g->getClickEffect(), 0);
//     dungeon_g->setNowScene(SCENE::HOME);
//     SDL_Log("startButton\n");
// }

Dungeon::Dungeon(Game *game)
{
    dungeon_g = game;
    mGame = game;
    LoadData();

    // mDungeonButton.onClick = &startButton;
    // mDungeonButton.create(
    //     mGame->getRenderer(),
    //     (WIN_W - BUTTON_W) / 2,
    //     (WIN_H - BUTTON_H) / 3,
    //     BUTTON_W,
    //     BUTTON_H,
    //     "S : Game Dungeon",
    //     mGame->getFont(),
    //     Color::SDL_blue,
    //     Color::SDL_white
    // );
}

void Dungeon::Input(SDL_Event event)
{
    switch(event.key.keysym.sym)
    {
    // case SDLK_s:    mDungeonButton.onClick();  break;
    }
}

void Dungeon::Update()
{

}

void Dungeon::Output()
{
    // mDungeonButton.Draw();
}

void Dungeon::LoadData()
{

}

void Dungeon::PlayMusic()
{

}