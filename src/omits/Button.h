#pragma once

#include <string>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>

class Button
{
public:
    void create(
        SDL_Window *window, SDL_Renderer *renderer, 
        int x, int y, int w, int h, std::string text, TTF_Font *font, 
        SDL_Color bgColor, SDL_Color textColor)
    {
        mRenderer = renderer;

        mButtonRect = {x, y, w, h};
        SDL_SetRenderDrawColor(mRenderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(mRenderer, &mButtonRect);

        SDL_Surface *surface = SDL_GetWindowSurface(window);
        surface = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);
        mTexture = SDL_CreateTextureFromSurface(mRenderer, surface);

        // 文字を描写したTextureのサイズを取得する
        int iw, ih;
        SDL_QueryTexture(mTexture, NULL, NULL, &iw, &ih);

        mTextRect = {0, 0, iw, ih};
    }
    void Draw()
    {
        // 基準点からの開始位置, 貼り付ける基準点
        SDL_RenderCopy(mRenderer, mTexture, &mTextRect, &mButtonRect);
    }

    // 関数へのポインタ
    void (*onClick)();

    // ラムダバージョン
    //std::function<void()> onClick;

private:
    SDL_Renderer *mRenderer;
    SDL_Texture *mTexture;
    SDL_Rect mButtonRect;
    SDL_Rect mTextRect;
};