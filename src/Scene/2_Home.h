#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "UI/Button.h"
#include "Utils/Color.h"
#include "Utils/Util.h"
#include "GenerateDungeon/Const.h"
#include "System/LTexture.h"

#include "Core/Game.h"

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

    Button mDungeonButton;

    int mAnimFrame;
    SDL_Rect mBornSleepBox;
    LTexture mBornSleepTexture;
    std::vector<SDL_Rect> mBornSleepSpriteClips;
};
