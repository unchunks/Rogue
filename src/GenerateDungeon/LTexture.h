#pragma once

#include <string>
#include "../../include/SDL2/SDL_image.h"

//テクスチャーラッパークラス
class LTexture
{
public:
    //変数初期化
    LTexture();

    //メモリの割り当て解除
    ~LTexture();

    //指定パスから画像をロード
    bool loadFromFile( std::string path );
    
    #if defined(SDL_TTF_MAJOR_VERSION)
    //フォント指定の文字列から画像を作成
    bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
    #endif

    //テクスチャの割り当て解除
    void free();

    //色設定
    void setColor( Uint8 red, Uint8 green, Uint8 blue );

    //フィルター設定？
    void setBlendMode( SDL_BlendMode blending );

    //透過率設定
    void setAlpha( Uint8 alpha );

    void setW( int width );
    void setH( int height );
    
    //座標からテクスチャをレンダリング
    void render( int x, int y, SDL_Rect* clip = NULL, bool filter = false, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

    //画像の寸法取得
    int getWidth();
    int getHeight();
    SDL_Texture* getTexture(){return mTexture;}

private:
    //実際のハードウェアテクスチャ
    SDL_Texture* mTexture;

    //画像の寸法
    int mWidth;
    int mHeight;
};
