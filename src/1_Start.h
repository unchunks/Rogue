#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "UI/Button.h"
#include "Functions/Color.h"

#include "Game.h"


class Start
{
public:
    Start(class Game *game);

    void Input(SDL_Event event);
    void Update();
    void Output();

private:
    void LoadData();
    void PlayMusic();

    class Game *mGame;

    Button mStartButton;
};