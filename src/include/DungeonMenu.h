#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Data.h"
#include "Game.h"
#include "../omits/Button.h"
#include "../omits/Color.h"

class DungeonMenu
{
public:
    DungeonMenu(class Game *game);

    void LoadMedia();
    void Draw();
    void Input(SDL_KeyCode keyCode);
    void PlayMusic();
    void MoveScene();

private:
    class Game *mGame;

    Button mSelectButtons[2];
};