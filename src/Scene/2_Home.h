#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "../UI/Button.h"
#include "../Functions/Color.h"
#include "../GenerateDungeon/Const.h"

#include "../Game.h"

class Home
{
public:
    Home(class Game *game);

    void Input(SDL_Event event);
    void Update();
    void Output();

private:
    void LoadData();
    void PlayMusic();

    class Game *mGame;

    Button mWeaponButton;
    Button mArmorButton;
    Button mItemButton;
    Button mOtherButton;
    Button mHaveButton;
    Button mStorageButton;
    Button mSaveButton;
    Button mDungeonButton;
};
