#include "UI/Log.h"

constexpr SDL_Rect Log::logRect;

Log::Log()
{
}

Log::~Log()
{
}

void Log::addText(std::string _text)
{
    // FIX
    SDL_Log("%s", _text.c_str());
    if(std::equal(texts.at(0).begin(), texts.at(0).end(), "null"))
    {
        SDL_Log("1行目に追加");
        texts.at(0) = _text;
    }
    else if(std::equal(texts.at(1).begin(), texts.at(1).end(), "null"))
    {
        SDL_Log("2行目に追加");
        texts.at(1) = _text;
    }
    else
    {
        SDL_Log("3行目に追加");
        texts.at(0) = texts.at(1);
        texts.at(1) = texts.at(2);
        texts.at(2) = _text;
    }
}

void Log::render(SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_Log("ログのレンダリング開始");
    // int y = 0;
    int line = 0;
    SDL_Log("背景のレンダリング");
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(renderer, &logRect);
    SDL_Log("文章のレンダリング");
    for(auto text : texts)
    {
        // textRects = {0, SCREEN_H - (TEXT_SIZE + LINE_PADDING) * (LINE_SIZE - line), TEXT_SIZE};
        SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), {255, 255, 255});
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {0, SCREEN_HEIGHT - (TEXT_SIZE + LINE_PADDING) * (LINE_SIZE - line), surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        // y += surface->h;
        SDL_FreeSurface(surface);
        line++;
    }
}