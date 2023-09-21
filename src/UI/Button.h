#ifndef __BUTTON
#define __BUTTON

#include <string>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "../Functions/Color.h"

const int padding = 3;
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

        mBottomOutlineRect = {x, y+h-3, w, 3};
        mRightOutlineRect = {x+w-3, y, 3, h-2};
        mButtonRect = {x, y, w-3, h-3};

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
        SDL_SetRenderDrawColor(mRenderer, mButtonColor.r/2.5, mButtonColor.g/2.5, mButtonColor.b/2.5, mButtonColor.a);
        SDL_RenderFillRect(mRenderer, &mBottomOutlineRect);

        SDL_SetRenderDrawColor(mRenderer, mButtonColor.r/3, mButtonColor.g/3, mButtonColor.b/3, mButtonColor.a);
        SDL_RenderFillRect(mRenderer, &mRightOutlineRect);

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
    SDL_Rect mBottomOutlineRect, mRightOutlineRect, mButtonRect, mTextRect;
    SDL_Color mButtonColor;
};

#endif __BUTTON