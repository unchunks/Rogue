#pragma once

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../GenerateDungeon/Const.h"

class Log
{
public:
    Log();
    ~Log();

    void addText(std::string _text);
    void render(SDL_Renderer *renderer, TTF_Font *font);

private:
    static constexpr int LINE_SIZE = 3;
    static constexpr int LINE_PADDING = 3;
    static constexpr int TEXT_SIZE = 10;
    static constexpr SDL_Rect logRect = {0, SCREEN_HEIGHT - (TEXT_SIZE + LINE_PADDING) * LINE_SIZE, SCREEN_WIDTH, (TEXT_SIZE + LINE_PADDING) * LINE_SIZE};
    SDL_Rect textRects[LINE_SIZE];
    std::vector<std::string> texts = {"null", "null", "null"};
};