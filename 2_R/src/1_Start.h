#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "UI/Button.h"
#include "Functions/Color.h"

class Start
{
public:
    Start(class Game *game);

    void Input();
    void Update();
    void Output();

private:
    void LoadData();
    void PlayMusic();

    class Game *mGame;

    Button mStartButton;
};