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

class ExpandColor
{
public:
    ExpandColor(SDL_Color _color)
    {
        color = _color;
    }
    SDL_Color color;
    SDL_Color operator+(SDL_Color _addColor)
    {
        return
        SDL_Color{
            static_cast<Uint8>(( color.r + _addColor.r ) / 2),
            static_cast<Uint8>(( color.g + _addColor.g ) / 2),
            static_cast<Uint8>(( color.b + _addColor.b ) / 2),
            static_cast<Uint8>(( color.a + _addColor.a ) / 2)};
    }
};
