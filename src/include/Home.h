#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Game.h"
#include "../omits/Button.h"
#include "../omits/Color.h"

class Home
{
public:
    Home(class Game *game);

    void LoadMedia();
    void Draw();
    void Input(SDL_KeyCode keyCode);

    void CreateButton();
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