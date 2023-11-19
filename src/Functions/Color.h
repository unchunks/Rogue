#pragma once

#include <SDL.h>

namespace Color{
    constexpr SDL_Color
        SDL_black = {0, 0, 0, 255},
        SDL_gray = {100, 100, 100, 255},
        SDL_white = {255, 255, 255, 255},
        SDL_red = {255, 0, 0, 255},
        SDL_green = {0, 255, 0, 255},
        SDL_blue = {0, 0, 255, 255};
};
