#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class HUD
{
public:
    HUD();
    ~HUD();

    void update();
    void render();
private:
    SDL_Rect p_status_rect, floor_num_rect;
};