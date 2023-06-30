#pragma once

#include <SDL.h>
// #include <SDL_image.h>
#include <SDL_ttf.h>

#include "Data.h"
#include "Game.h"
#include "../omits/Button.h"
#include "../omits/Color.h"

class Start
{
public:
    Start(class Game *game);

    void LoadMedia();
    void Draw();
    void Input();
    void PlayMusic();
    void MoveScene();

private:
    class Game *mGame;

    Button mStartButton;
    Button mLoadButton;

    // SDL_Texture *mTexture_GameStart;
    // SDL_Texture *mTexture_GameLoad;
};