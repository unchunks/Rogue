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
    printf("weaponButton\n");
}
void armorButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    printf("armorButton\n");
}
void itemButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    printf("itemButton\n");
}
void otherButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    printf("otherButton\n");
}

void haveButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    printf("haveButton\n");
}
void storageButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    printf("storageButton\n");
}
void saveButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    printf("saveButton\n");
}
void dungeonButton()
{
    Mix_PlayChannel(-1, home_g->getClickEffect(), 0);
    home_g->setNowScene(SCENE::DUNGEON_MENU);
    printf("dungeonButton\n");
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
        SCREEN_WIDTH - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 4) - PADDING,
        PADDING,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "武",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mArmorButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 3) - PADDING,
        PADDING,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "防",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mItemButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 2) - PADDING,
        PADDING,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "ア",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mOtherButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 1) - PADDING,
        PADDING,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "他",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);

    //サイドボタン
    mHaveButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - PADDING,
        SIDE_BUTTON_H * 0 + PADDING,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "もちもの",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mStorageButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - PADDING,
        (SIDE_BUTTON_H + 10) * 1 + PADDING,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "　倉庫　",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mSaveButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - PADDING,
        (SIDE_BUTTON_H + 10) * 2 + PADDING,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        " セーブ ",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mDungeonButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - PADDING,
        (SIDE_BUTTON_H + 10) * 3 + PADDING,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "ﾀﾞﾝｼﾞｮﾝ",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
}

void Home::LoadData()
{
}

void Home::Input(SDL_Event event)
{
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_1:
                mWeaponButton.press();
                break;
            case SDLK_2:
                mArmorButton.press();
                break;
            case SDLK_3:
                mItemButton.press();
                break;
            case SDLK_4:
                mOtherButton.press();
                break;
            case SDLK_5:
                mHaveButton.press();
                break;
            case SDLK_6:
                mStorageButton.press();
                break;
            case SDLK_7:
                mSaveButton.press();
                break;
            case SDLK_8:
                mDungeonButton.press();
                break;
        }
    }
    else
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_1:
                if(mWeaponButton.isPressed)
                {
                    mWeaponButton.release();
                    mWeaponButton.onClick();
                }
                break;
            case SDLK_2:
                if(mArmorButton.isPressed)
                {
                    mArmorButton.release();
                    mArmorButton.onClick();
                }
                break;
            case SDLK_3:
                if(mItemButton.isPressed)
                {
                    mItemButton.release();
                    mItemButton.onClick();
                }
                break;
            case SDLK_4:
                if(mOtherButton.isPressed)
                {
                    mOtherButton.release();
                    mOtherButton.onClick();
                }
                break;
            case SDLK_5:
                if(mHaveButton.isPressed)
                {
                    mHaveButton.release();
                    mHaveButton.onClick();
                }
                break;
            case SDLK_6:
                if(mStorageButton.isPressed)
                {
                    mStorageButton.release();
                    mStorageButton.onClick();
                }
                break;
            case SDLK_7:
                if(mSaveButton.isPressed)
                {
                    mSaveButton.release();
                    mSaveButton.onClick();
                }
                break;
            case SDLK_8:
                if(mDungeonButton.isPressed)
                {
                    mDungeonButton.release();
                    mDungeonButton.onClick();
                }
                break;
        }
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