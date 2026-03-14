#include "Scene/2_Home.h"

extern SDL_Renderer *gRenderer;
extern TTF_Font *gFontN;
extern Mix_Chunk* gClickEffect;
extern SCENE gNowScene;

const int SIDE_BUTTON_W = 250;
const int SIDE_BUTTON_H = 60;

const int SLEEP_NUM = 15;
const int SLEEP_SPEED = 3;
const int SLEEP_W = 256;
const int SLEEP_H = 256;

void dungeonButton()
{
    Mix_PlayChannel(-1, gClickEffect, 0);
    gNowScene = SCENE::DUNGEON_MENU;
    // SDL_Log("dungeonButton\n");
}

Home::Home()
    : mAnimFrame(0)
{
    LoadData();

    mDungeonButton.onClick = &dungeonButton;

    mDungeonButton.create(
        gRenderer,
        SCREEN_WIDTH - SIDE_BUTTON_W - PADDING,
        (SIDE_BUTTON_H + 10) * 3 + PADDING,
        SIDE_BUTTON_W,
        SIDE_BUTTON_H,
        "Enter:ダンジョン",
        gFontN,
        Color::SDL_blue,
        Color::SDL_white);
}

Home::~Home()
{
    mBornSleepTexture.free();
    mBornSleepSpriteClips.clear();
    mBornSleepSpriteClips.shrink_to_fit();
}

void Home::LoadData()
{
    mBornSleepSpriteClips.resize(SLEEP_NUM, {0, 0, 0, 0});
    if (!mBornSleepTexture.loadFromFile(resource_path("bone_sleeping.png")))
    {
        SDL_Log("Failed to load walking animation texture!\n");
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
        case SDLK_RETURN:
            mDungeonButton.press();
            break;
        }
        return;
    }
    switch (event.key.keysym.sym)
    {
    case SDLK_RETURN:
        if (mDungeonButton.isPressed)
        {
            mDungeonButton.release();
            mDungeonButton.onClick();
        }
        break;
    }
}

void Home::Update()
{
}

void Home::Output()
{
    mDungeonButton.Draw();

    mAnimFrame++;
    if (mAnimFrame >= SLEEP_NUM * FPS / SLEEP_SPEED)
    {
        mAnimFrame = 0;
    }
    int c_sprite_num = (mAnimFrame * SLEEP_SPEED / FPS);

    //レンダリングスペースを設定し、画面にレンダリング
	SDL_Rect renderQuad = { mBornSleepBox.x, mBornSleepBox.y, SLEEP_W, SLEEP_H };

	//画面にレンダリングする
	SDL_RenderCopy( gRenderer, mBornSleepTexture.getTexture(), &mBornSleepSpriteClips[c_sprite_num], &renderQuad);
}

void Home::PlayMusic()
{
}
