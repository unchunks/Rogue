#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "../UI/Button.h"
#include "../Functions/Color.h"
#include "../Functions/Util.h"
#include "../GenerateDungeon/Const.h"
#include "../GenerateDungeon/LTexture.h"

#include "../Game.h"

class Home
{
public:
    Home();
    ~Home();

    void Input(SDL_Event event);
    void Update();
    void Output();

private:
    void LoadData();
    void PlayMusic();

    Button mWeaponButton;
    Button mArmorButton;
    Button mItemButton;
    Button mOtherButton;
    Button mHaveButton;
    Button mStorageButton;
    Button mSaveButton;
    Button mDungeonButton;

    int mAnimFrame;
    SDL_Rect mBornSleepBox;
    LTexture mBornSleepTexture;
    std::vector<SDL_Rect> mBornSleepSpriteClips;
};
