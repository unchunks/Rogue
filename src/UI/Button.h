#pragma once

#include <string>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "../Functions/Color.h"

class Button
{
public:
    void create(
        SDL_Renderer *renderer, 
        int x, int y, int w, int h, std::string text, TTF_Font *font, 
        SDL_Color buttonColor, SDL_Color textColor)
    {
        mRenderer = renderer;
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);

        mButtonColor = buttonColor;
        mButtonRect = {x, y+diffLevel*hierarchy, w-diffLevel*hierarchy*2, h-diffLevel*hierarchy*2};

        mTexture = SDL_CreateTextureFromSurface(mRenderer, surface);

        // 文字を描写したTextureのサイズを取得する
        // int iw, ih;
        // SDL_QueryTexture(mTexture, NULL, NULL, &iw, &ih);
        // mTextRect = {0, 0, iw, ih};

        SDL_FreeSurface(surface);
    }
    void Draw()
    {
        SDL_Color buffColor;
        buffColor.r = 100;
        buffColor.g = 100;
        buffColor.b = 100;
        buffColor.a = 255;
        SDL_Color diffColor;
        diffColor.r = (mButtonColor.r - buffColor.r) / (hierarchy+2);
        diffColor.g = (mButtonColor.g - buffColor.g) / (hierarchy+2);
        diffColor.b = (mButtonColor.b - buffColor.b) / (hierarchy+2);
        SDL_Rect buffRect = mButtonRect;

        for(int i=0; i<hierarchy; i++)
        {
            buffColor.r += diffColor.r;
            buffColor.g += diffColor.g;
            buffColor.b += diffColor.b;
            SDL_SetRenderDrawColor(mRenderer, buffColor.r, buffColor.g, buffColor.b, buffColor.a);
            SDL_RenderFillRect(mRenderer, &buffRect);
            buffRect.x += diffLevel;
            buffRect.y -= diffLevel;
        }
        buffRect.x += (padding-diffLevel);
        buffRect.y += (padding+diffLevel);
        buffRect.w -= padding*2;
        buffRect.h -= padding*2;
        SDL_SetRenderDrawColor(mRenderer, mButtonColor.r, mButtonColor.g, mButtonColor.b, mButtonColor.a);
        SDL_RenderFillRect(mRenderer, &buffRect);

        // 基準点からの開始位置, 貼り付ける基準点
        buffRect.x += padding;
        buffRect.y += padding;
        buffRect.w -= padding*2;
        buffRect.h -= padding*2;
        SDL_RenderCopy(mRenderer, mTexture, NULL, &buffRect);
    }
    void press()
    {
        hierarchy = 0;
    }
    void release()
    {
        hierarchy = 5;
    }

    // 関数へのポインタ
    void (*onClick)();

private:
    const int padding = 5;
    const int diffLevel = 1;
    int hierarchy = 5;

    SDL_Renderer *mRenderer;
    SDL_Texture *mTexture;
    SDL_Rect mButtonRect;
    SDL_Color mButtonColor;
};
