#include "include/Home.h"

extern int WINDOW_W;
extern int WINDOW_H;

const int TOP_BUTTON_W = WINDOW_W / 10;
const int TOP_BUTTON_H = WINDOW_H / 5;
const int SIDE_BUTTON_W = WINDOW_W / 5;
const int SIDE_BUTTON_H = WINDOW_H / 5;

Game *home_g;
void weaponButton()
{
    SDL_Log("weaponButton\n");
}
void armorButton()
{
    SDL_Log("armorButton\n");
}
void itemButton()
{
    SDL_Log("itemButton\n");
}
void otherButton()
{
    SDL_Log("otherButton\n");
}

void haveButton()
{
    SDL_Log("haveButton\n");
}
void storageButton()
{
    SDL_Log("storageButton\n");
}
void saveButton()
{
    SDL_Log("saveButton\n");
}
void dungeonButton()
{
    SDL_Log("dungeonButton\n");
}


Home::Home(Game *game)
{
    mGame = game;
    home_g = game;
    LoadMedia();

    mWeaponButton.onClick = &weaponButton;
/*
    mArmorButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        10 + (TOP_BUTTON_W * 1),
        0,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "mArmorButton",
        mGame->mFont,
        (SDL_Color){255, 0, 255, 255},
        (SDL_Color){255, 255, 255, 255});
    mArmorButton.onClick = &armorButton;

    mItemButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        10 + (TOP_BUTTON_W * 2),
        0,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "mItemButton",
        mGame->mFont,
        (SDL_Color){255, 0, 255, 255},
        (SDL_Color){255, 255, 255, 255});
    mItemButton.onClick = &itemButton;

    mOtherButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        10 + (TOP_BUTTON_W * 3),
        0,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "mOtherButton",
        mGame->mFont,
        (SDL_Color){255, 0, 255, 255},
        (SDL_Color){255, 255, 255, 255});
    mOtherButton.onClick = &otherButton;

    mHaveButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        WINDOW_W - SIDE_BUTTON_W,
        SIDE_BUTTON_H * 0,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "mHaveButton",
        mGame->mFont,
        (SDL_Color){255, 0, 255, 255},
        (SDL_Color){255, 255, 255, 255});
    mHaveButton.onClick = &haveButton;

    mStorageButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        WINDOW_W - SIDE_BUTTON_W,
        SIDE_BUTTON_H * 1,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "mStorageButton",
        mGame->mFont,
        (SDL_Color){255, 0, 255, 255},
        (SDL_Color){255, 255, 255, 255});
    mStorageButton.onClick = &storageButton;

    mSaveButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        WINDOW_W - SIDE_BUTTON_W,
        SIDE_BUTTON_H * 2,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "mSaveButton",
        mGame->mFont,
        (SDL_Color){255, 0, 255, 255},
        (SDL_Color){255, 255, 255, 255});
    mSaveButton.onClick = &saveButton;

    mDungeonButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        WINDOW_W - SIDE_BUTTON_W,
        SIDE_BUTTON_H * 3,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "mDungeonButton",
        mGame->mFont,
        (SDL_Color){255, 0, 255, 255},
        (SDL_Color){255, 255, 255, 255});
    mDungeonButton.onClick = &dungeonButton;*/
}

void Home::LoadMedia()
{
}

void Home::Draw()
{
    mWeaponButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        10 + (TOP_BUTTON_W * 0),
        0,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "mWeaponButton",
        mGame->mFont,
        (SDL_Color){255, 0, 255, 255},
        (SDL_Color){255, 255, 255, 255});
    mWeaponButton.Draw();
}

void Home::Input(SDL_KeyCode keyCode)
{
    SDL_Log("Input\n");
    switch(keyCode)
    {
        case SDLK_1:
            mWeaponButton.onClick();
            break;
        case SDLK_2:
            mArmorButton.onClick();
            break;
        case SDLK_3:
            mItemButton.onClick();
            break;
        case SDLK_4:
            mOtherButton.onClick();
            break;
        case SDLK_5:
            mHaveButton.onClick();
            break;
        case SDLK_6:
            mStorageButton.onClick();
            break;
        case SDLK_7:
            mSaveButton.onClick();
            break;
        case SDLK_8:
            mDungeonButton.onClick();
            break;
    }
}

void Home::PlayMusic()
{
}

void Home::MoveScene()
{
}