#include "4_Dungeon.h"

extern int WIN_W;
extern int WIN_H;

const int BUTTON_W = 500;
const int BUTTON_H = 50;

Game *dungeon_g;

Dungeon::Dungeon(Game *game)
{
    dungeon_g = game;
    mGame = game;
    LoadData();
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
}

void Dungeon::LoadData()
{
}

void Dungeon::PlayMusic()
{
}