#pragma once

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

#include "../GenerateDungeon/Const.h"
#include "../Functions/Util.h"

class Log
{
public:
    Log();
    ~Log();

    void addText(std::string _text);
    void render(SDL_Renderer *renderer);
    void reset();

private:
    static constexpr int LOG_SIDE_PADDIG = 10;
    static constexpr int LINE_NUM = 5;
    static constexpr int LINE_PADDING = 5;
    static constexpr int FONT_SIZE = 15;
    static constexpr SDL_Rect logRect = {
        0, 
        SCREEN_HEIGHT - (FONT_SIZE + LINE_PADDING) * LINE_NUM - LINE_PADDING, 
        SCREEN_WIDTH, 
        (FONT_SIZE + LINE_PADDING) * LINE_NUM + LINE_PADDING};
    TTF_Font *fontN;
    SDL_Rect textRects[LINE_NUM];
    std::vector<std::string> texts;
};