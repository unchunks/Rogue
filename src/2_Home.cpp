#include "Scene/2_Home.h"

extern SDL_Renderer *gRenderer;

const int TOP_BUTTON_W = 60;
const int TOP_BUTTON_H = 60;
const int SIDE_BUTTON_W = 200;
const int SIDE_BUTTON_H = 60;

Game *home_g;
void weaponButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    SDL_Log("weaponButton\n");
}
void armorButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    SDL_Log("armorButton\n");
}
void itemButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    SDL_Log("itemButton\n");
}
void otherButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    SDL_Log("otherButton\n");
}

void haveButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    SDL_Log("haveButton\n");
}
void storageButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    SDL_Log("storageButton\n");
}
void saveButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    SDL_Log("saveButton\n");
}
void dungeonButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    home_g->setNowScene(SCENE::DUNGEON_MENU);
    SDL_Log("dungeonButton\n");
}


Home::Home(Game *game)
{
    mGame = game;
    home_g = game;
    LoadData();

    mWeaponButton.onClick = &weaponButton;
    mArmorButton.onClick = &armorButton;
    mItemButton.onClick = &itemButton;
    mOtherButton.onClick = &otherButton;
    mHaveButton.onClick = &haveButton;
    mStorageButton.onClick = &storageButton;
    mSaveButton.onClick = &saveButton;
    mDungeonButton.onClick = &dungeonButton;
    //トップボタン
    mWeaponButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 4) - padding,
        padding,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "武",
        mGame->getFont(),
        Color::SDL_blue,
        Color::SDL_white);
    mArmorButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 3) - padding,
        padding,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "防",
        mGame->getFont(),
        Color::SDL_blue,
        Color::SDL_white);
    mItemButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 2) - padding,
        padding,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "ア",
        mGame->getFont(),
        Color::SDL_blue,
        Color::SDL_white);
    mOtherButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 1) - padding,
        padding,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "他",
        mGame->getFont(),
        Color::SDL_blue,
        Color::SDL_white);

    //サイドボタン
    mHaveButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - padding,
        SIDE_BUTTON_H * 0 + padding,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "もちもの",
        mGame->getFont(),
        Color::SDL_blue,
        Color::SDL_white);
    mStorageButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - padding,
        (SIDE_BUTTON_H + 10) * 1 + padding,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "　倉庫　",
        mGame->getFont(),
        Color::SDL_blue,
        Color::SDL_white);
    mSaveButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - padding,
        (SIDE_BUTTON_H + 10) * 2 + padding,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        " セーブ ",
        mGame->getFont(),
        Color::SDL_blue,
        Color::SDL_white);
    mDungeonButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - padding,
        (SIDE_BUTTON_H + 10) * 3 + padding,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "ﾀﾞﾝｼﾞｮﾝ",
        mGame->getFont(),
        Color::SDL_blue,
        Color::SDL_white);
}

void Home::LoadData()
{
}

void Home::Input(SDL_Event event)
{
    switch(event.key.keysym.sym)
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

void Home::Update()
{
}

void Home::Output()
{
    mWeaponButton.Draw();
    mArmorButton.Draw();
    mItemButton.Draw();
    mOtherButton.Draw();
    mHaveButton.Draw();
    mStorageButton.Draw();
    mSaveButton.Draw();
    mDungeonButton.Draw();
}

void Home::PlayMusic()
{
}