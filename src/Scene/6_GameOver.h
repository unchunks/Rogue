#pragma once

#include <iostream>
#include "../../include/SDL2/SDL.h"
#include "../../include/SDL2/SDL_ttf.h"
#include "../../SDL2/SDL_mixer.h"

#include "../UI/Button.h"
#include "../Functions/Color.h"
#include "../GenerateDungeon/Const.h"
#include "../GenerateDungeon/Enum.h"

#include "../Game.h"

class GameOver
{
public:
    GameOver();
    ~GameOver();

    void Input(SDL_Event event);
    void Update();
    void Output();

private:
    void LoadData();

    SDL_Rect over_back = {10, 10, SCREEN_WIDTH - 20, 200};
    SDL_Rect over_text = {15, 15, SCREEN_WIDTH - 30, 190};
    Button to_home, to_dungeonmenu;
};