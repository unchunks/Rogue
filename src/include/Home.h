#pragma once

#include <SDL.h>

#include "Game.h"
#include "../omits/Button.h"

class Home
{
public:
    Home(class Game *game);

    void LoadMedia();
    void Draw();
    void Input(SDL_KeyCode keyCode);
    void PlayMusic();
    void MoveScene();

private:
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