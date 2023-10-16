#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "../UI/Button.h"
#include "../Functions/Color.h"
#include "../GenerateDungeon/Const.h"

#include "../Game.h"

class DungeonMenu
{
public:
    DungeonMenu(class Game *game);

    void Input(SDL_Event event);
    void Update();
    void Output();

private:
    void LoadData();
    void PlayMusic();

    class Game *mGame;

    Button mAreaDivide;
    Button mRRA;
};
