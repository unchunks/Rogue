#include "Scene/2_Home.h"

extern SDL_Renderer *gRenderer;

const int TOP_BUTTON_W = 120;
const int TOP_BUTTON_H = 60;
const int SIDE_BUTTON_W = 250;
const int SIDE_BUTTON_H = 60;

const int SLEEP_NUM = 15;
const int SLEEP_SPEED = 3;
const int SLEEP_W = 256;
const int SLEEP_H = 256;

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
    : mAnimFrame(0)
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
    // トップボタン
    mWeaponButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 4) - PADDING,
        PADDING,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "1:武",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mArmorButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 3) - PADDING,
        PADDING,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "2:防",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mItemButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 2) - PADDING,
        PADDING,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "3:ア",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mOtherButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - ((TOP_BUTTON_W + 10) * 1) - PADDING,
        PADDING,
        TOP_BUTTON_W,
        TOP_BUTTON_H,
        "4:他",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);

    // サイドボタン
    mHaveButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - PADDING,
        SIDE_BUTTON_H * 0 + PADDING,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "5:もちもの",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mStorageButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - PADDING,
        (SIDE_BUTTON_H + 10) * 1 + PADDING,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "6:　倉庫　",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mSaveButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - PADDING,
        (SIDE_BUTTON_H + 10) * 2 + PADDING,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "7: セーブ ",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
    mDungeonButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - PADDING,
        (SIDE_BUTTON_H + 10) * 3 + PADDING,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "8:ダンジョン",
        mGame->getFontN(),
        Color::SDL_blue,
        Color::SDL_white);
}

// TODO
void Home::LoadData()
{
    mBornSleepSpriteClips.resize(SLEEP_NUM, {0, 0, 0, 0});
    if (!mBornSleepTexture.loadFromFile("assets/bone_sleeping.png"))
    {
        printf("Failed to load walking animation texture!\n");
        return;
    }
    for (int sprite_num = 0; sprite_num < SLEEP_NUM; sprite_num++)
    {
        mBornSleepSpriteClips.at(sprite_num).x = SPRITE_CHAR_WIDTH * sprite_num;
        mBornSleepSpriteClips.at(sprite_num).y = SPRITE_CHAR_HEIGHT * 0;
        mBornSleepSpriteClips.at(sprite_num).w = SPRITE_CHAR_WIDTH;
        mBornSleepSpriteClips.at(sprite_num).h = SPRITE_CHAR_HEIGHT;
    }
    mBornSleepBox = {10, SCREEN_HEIGHT - SLEEP_H, SLEEP_W, SLEEP_H};
}

void Home::Input(SDL_Event event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
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
        switch (event.key.keysym.sym)
        {
        case SDLK_1:
            if (mWeaponButton.isPressed)
            {
                mWeaponButton.release();
                mWeaponButton.onClick();
            }
            break;
        case SDLK_2:
            if (mArmorButton.isPressed)
            {
                mArmorButton.release();
                mArmorButton.onClick();
            }
            break;
        case SDLK_3:
            if (mItemButton.isPressed)
            {
                mItemButton.release();
                mItemButton.onClick();
            }
            break;
        case SDLK_4:
            if (mOtherButton.isPressed)
            {
                mOtherButton.release();
                mOtherButton.onClick();
            }
            break;
        case SDLK_5:
            if (mHaveButton.isPressed)
            {
                mHaveButton.release();
                mHaveButton.onClick();
            }
            break;
        case SDLK_6:
            if (mStorageButton.isPressed)
            {
                mStorageButton.release();
                mStorageButton.onClick();
            }
            break;
        case SDLK_7:
            if (mSaveButton.isPressed)
            {
                mSaveButton.release();
                mSaveButton.onClick();
            }
            break;
        case SDLK_8:
            if (mDungeonButton.isPressed)
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

    mAnimFrame++;
    if (mAnimFrame >= SLEEP_NUM * FPS / SLEEP_SPEED)
    {
        mAnimFrame = 0;
    }
    int c_sprite_num = (mAnimFrame * SLEEP_SPEED / FPS);
    // キャラクターを表示
    // mBornSleepTexture.render(mBornSleepBox.x, mBornSleepBox.y, &mBornSleepSpriteClips[c_sprite_num]);

    //レンダリングスペースを設定し、画面にレンダリング
	SDL_Rect renderQuad = { mBornSleepBox.x, mBornSleepBox.y, SLEEP_W, SLEEP_H };

	//画面にレンダリングする
	SDL_RenderCopy( gRenderer, mBornSleepTexture.getTexture(), &mBornSleepSpriteClips[c_sprite_num], &renderQuad);
}

void Home::PlayMusic()
{
}