#include "include/Home.h"

extern int WINDOW_W;
extern int WINDOW_H;

const int TOP_BUTTON_W = 60;
const int TOP_BUTTON_H = 60;
const int SIDE_BUTTON_W = 200;
const int SIDE_BUTTON_H = 60;

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
    mArmorButton.onClick = &armorButton;
    mItemButton.onClick = &itemButton;
    mOtherButton.onClick = &otherButton;
    mHaveButton.onClick = &haveButton;
    mStorageButton.onClick = &storageButton;
    mSaveButton.onClick = &saveButton;
    mDungeonButton.onClick = &dungeonButton;
}

void Home::LoadMedia()
{
}

void Home::Draw()
{
    CreateButton();

    mWeaponButton.Draw();
    mArmorButton.Draw();
    mItemButton.Draw();
    mOtherButton.Draw();
    mHaveButton.Draw();
    mStorageButton.Draw();
    mSaveButton.Draw();
    mDungeonButton.Draw();

}

void Home::CreateButton()
{
    //トップボタン
    mWeaponButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        WINDOW_W - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 4),
        0,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "武",
        mGame->mFont,
        Color::SDL_blue,
        Color::SDL_white);
    mArmorButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        WINDOW_W - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 3),
        0,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "防",
        mGame->mFont,
        Color::SDL_blue,
        Color::SDL_white);
    mItemButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        WINDOW_W - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 2),
        0,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "ア",
        mGame->mFont,
        Color::SDL_blue,
        Color::SDL_white);
    mOtherButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        WINDOW_W - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 1),
        0,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "他",
        mGame->mFont,
        Color::SDL_blue,
        Color::SDL_white);

    //サイドボタン
    mHaveButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        WINDOW_W - SIDE_BUTTON_W,
        SIDE_BUTTON_H * 0,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "もちもの",
        mGame->mFont,
        Color::SDL_blue,
        Color::SDL_white);
    mStorageButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        WINDOW_W - SIDE_BUTTON_W,
        (SIDE_BUTTON_H + 10) * 1,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "　倉庫　",
        mGame->mFont,
        Color::SDL_blue,
        Color::SDL_white);
    mSaveButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        WINDOW_W - SIDE_BUTTON_W,
        (SIDE_BUTTON_H + 10) * 2,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        " セーブ ",
        mGame->mFont,
        Color::SDL_blue,
        Color::SDL_white);
    mDungeonButton.create(
        mGame->getWindow(), mGame->getRenderer(),
        WINDOW_W - SIDE_BUTTON_W,
        (SIDE_BUTTON_H + 10) * 3,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "ﾀﾞﾝｼﾞｮﾝ",
        mGame->mFont,
        Color::SDL_blue,
        Color::SDL_white);
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