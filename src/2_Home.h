#ifndef __HOME
#define __HOME

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "UI/Button.h"
#include "Functions/Color.h"

#include "Game.h"

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

#endif __HOME