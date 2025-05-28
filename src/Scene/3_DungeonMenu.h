#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "../UI/Button.h"
#include "../Functions/Color.h"
#include "../GenerateDungeon/Const.h"
#include "../GenerateDungeon/Enum.h"

#include "../Game.h"

class DungeonMenu
{
public:
    DungeonMenu();
    ~DungeonMenu();

    void Input(SDL_Event event);
    void Update();
    void Output();

private:
    void LoadData();
    void PlayMusic();

    Button mAreaDivide;
    Button mRRA;
};
