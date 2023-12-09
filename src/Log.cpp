#include "UI/Log.h"

constexpr SDL_Rect Log::logRect;

Log::Log()
{
    // なぜか指定したフォントより1大きくなる
    fontN = TTF_OpenFont("assets/JF-Dot-K14.ttf", FONT_SIZE - 1);
    texts.resize(0);
}

Log::~Log()
{
    TTF_CloseFont(fontN);
}

void Log::addText(std::string _text)
{
    SDL_Log("%s", _text.c_str());

    texts.push_back(_text);
    if (static_cast<int>(texts.size()) > LINE_NUM)
    {
        texts.erase(texts.begin());
    }
}

void Log::render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(renderer, &logRect);
    if(texts.empty())
        return;

    SDL_Rect rect = {
        LOG_SIDE_PADDIG, 
        SCREEN_HEIGHT - (FONT_SIZE + LINE_PADDING) * LINE_NUM - LINE_PADDING, 
        0, 
        0};
    for(auto text : texts)
    {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(fontN, text.c_str(), {255, 255, 255});
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect.y += LINE_PADDING;
        rect.w = surface->w;
        rect.h = surface->h;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        rect.y += rect.h;
    }
}