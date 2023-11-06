#pragma once

#include <string>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "../Functions/Color.h"

const int padding = 6;

// const Mix_Chunk* mClickEffect = Mix_LoadWAV("../../assets/click.mp3");

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

        mTopOutlineRect  = {x, y, w, padding};
        mBottomOutlineRect = {x, y+h-padding, w, padding};
        mLeftOutlineRect  = {x, y, padding, h};
        mRightOutlineRect = {x+w-padding, y, padding, h};
        mButtonRect = {x+padding, y+padding, w-padding*2, h-padding*2};

        mTexture = SDL_CreateTextureFromSurface(mRenderer, surface);

        // 文字を描写したTextureのサイズを取得する
        int iw, ih;
        SDL_QueryTexture(mTexture, NULL, NULL, &iw, &ih);
        mTextRect = {0, 0, iw, ih};
        mTextRect = mButtonRect;
        mTextRect.x += padding;
        mTextRect.y += padding;
        mTextRect.w -= padding*2;
        mTextRect.h -= padding*2;

        SDL_FreeSurface(surface);
    }
    void Draw()
    {
        ExpandColor exchange = ExpandColor(mButtonColor);
        ExpandColor outColor = exchange + SDL_Color{100, 100, 100, 255};
        SDL_SetRenderDrawColor(mRenderer, outColor.color.r, outColor.color.g, outColor.color.b, outColor.color.a);
        SDL_RenderFillRect(mRenderer, &mBottomOutlineRect);
        SDL_RenderFillRect(mRenderer, &mRightOutlineRect);
        
        outColor = exchange + SDL_Color{180, 180, 180, 255};
        SDL_SetRenderDrawColor(mRenderer, outColor.color.r, outColor.color.g, outColor.color.b, outColor.color.a);
        SDL_RenderFillRect(mRenderer, &mTopOutlineRect);
        SDL_RenderFillRect(mRenderer, &mLeftOutlineRect);

        SDL_SetRenderDrawColor(mRenderer, mButtonColor.r, mButtonColor.g, mButtonColor.b, mButtonColor.a);
        SDL_RenderFillRect(mRenderer, &mButtonRect);

        // 基準点からの開始位置, 貼り付ける基準点
        SDL_RenderCopy(mRenderer, mTexture, NULL, &mTextRect);
    }

    // 関数へのポインタ
    void (*onClick)();

private:
    SDL_Renderer *mRenderer;
    SDL_Texture *mTexture;
    SDL_Rect mTopOutlineRect, mBottomOutlineRect, mLeftOutlineRect, mRightOutlineRect, mButtonRect, mTextRect;
    SDL_Color mButtonColor;
};
