#include "include/DungeonMenu.h"

extern int WINDOW_W;
extern int WINDOW_H;

const int BUTTON_W = 250;
const int BUTTON_H = 60;

Game *dungeonMenu_g;
void dungeonMenuButton()
{
    Mix_PlayChannel(-1, mClickEffect, 0);
    // start_g->mNowScene = Game::scene::DUNGEON;
    SDL_Log("Clicked\n");
    SDL_Log("%d\n", dungeonMenu_g->mNowScene);
}

DungeonMenu::DungeonMenu(Game *game)
{
    mGame = game;
    dungeonMenu_g = game;
    LoadMedia();

    mSelectButtons[0].onClick = &dungeonMenuButton;
    mSelectButtons[1].onClick = &dungeonMenuButton;
    mSelectButtons[0].create(
        mGame->getRenderer(),
        (WINDOW_W / 2 - BUTTON_W) / 2,
        (WINDOW_H - BUTTON_H) / 2,
        BUTTON_W,
        BUTTON_H,
        "1 : 区域分割法",
        mGame->mFont,
        Color::SDL_blue,
        Color::SDL_white);
    mSelectButtons[1].create(
        mGame->getRenderer(),
        (WINDOW_W * 1.5 - BUTTON_W) / 2,
        (WINDOW_H - BUTTON_H) / 2,
        BUTTON_W,
        BUTTON_H,
        "2 : オリジナル",
        mGame->mFont,
        Color::SDL_blue,
        Color::SDL_white);
}

void DungeonMenu::LoadMedia()
{
}

void DungeonMenu::Draw()
{
    mSelectButtons[0].Draw();
    mSelectButtons[1].Draw();
}

void DungeonMenu::Input(SDL_KeyCode keyCode)
{
    SDL_Log("Input\n");
    mSelectButtons[0].onClick();
    // mSelectButtons[1].onClick();
}

void DungeonMenu::PlayMusic()
{
}

void DungeonMenu::MoveScene()
{
}