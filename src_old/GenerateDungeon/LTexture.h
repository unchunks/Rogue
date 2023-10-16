#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

#include <Const.h>

//TODO: テクスチャーラッパークラス
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
    void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

    //画像の寸法取得
    int getWidth();
    int getHeight();

private:
    //実際のハードウェアテクスチャ
    SDL_Texture* mTexture;

    //画像の寸法
    int mWidth;
    int mHeight;
};

LTexture::LTexture()
{
	//初期化
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//割り当て解除
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//既存のテクスチャを取り除く
	free();

	//返り値のテクスチャ
	SDL_Texture* newTexture = NULL;

	//指定パスから画像を読み込み
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//画像のキーカラー
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//表層ピクセルからテクスチャを作成
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//画像の寸法取得
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//読み込んだSurfaceを開放
		SDL_FreeSurface( loadedSurface );
	}

	//成功したか返す
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//既存のテクスチャを取り除く
	free();

	//テキストをレンダリング
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//表層ピクセルからテクスチャを作成
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//画像の寸法取得
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Surfaceを開放
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//成功したか返す
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//テクスチャがあるときは開放
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//テクスチャRGBを調整する
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//ブレンド関数の設定
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//テクスチャアルファを調整する
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::setW( int width )
{
	mWidth = width;
}

void LTexture::setH( int height )
{
	mHeight = height;
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//レンダリングスペースを設定し、画面にレンダリング
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//クリップのレンダリング寸法を設定する
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//画面にレンダリングする
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}