#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "../UI/Button.h"
#include "../Functions/Color.h"
#include "../GenerateDungeon/Const.h"

#include "../Game.h"

class Congratulations
{
public:
    Congratulations(class Game *game);
    ~Congratulations();

    void Input(SDL_Event event);
    void Update();
    void Output();

private:
    void LoadData();

    class Game *mGame;

    TTF_Font *fontB;
    SDL_Rect cong_back = {10, 10, SCREEN_WIDTH - 20, 200};
    SDL_Rect cong_text = {15, 15, SCREEN_WIDTH - 30, 190};
    Button to_home, to_dungeonmenu;
};