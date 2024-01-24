#pragma once

#include <iostream>
#include "../../include/SDL2/SDL.h"
#include "../../include/SDL2/SDL_ttf.h"
#include "../../SDL2/SDL_mixer.h"

#include "../UI/Button.h"
#include "../Functions/Color.h"
#include "../GenerateDungeon/Const.h"
#include "../GenerateDungeon/Enum.h"

class Start
{
public:
    Start();
    ~Start();

    void Input(SDL_Event event);
    void Update();
    void Output();

private:
    void LoadData();
    void PlayMusic();

    Button mStartButton;
};
