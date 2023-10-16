#pragma once

#include <SDL.h>

#include "Enum.h"
#include "Const.h"

//TODO: タイルクラス
class Tile
{
    public:
		//座標とタイプを初期化
		Tile( int x, int y, CELL_TYPE tileType );

		//タイルを表示
		void render( SDL_Rect& camera );

		//タイプを取得
		CELL_TYPE getType();

		//Rectの衝突判定
		SDL_Rect getBox();

    private:
		//タイルの属性
		SDL_Rect mBox;

		//タイルのタイプ
		CELL_TYPE mType;
};

Tile::Tile( int x, int y, CELL_TYPE tileType )
{
	//オフセットを取得する
    mBox.x = x;
    mBox.y = y;

	//コリジョンボックスを設定する
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

	//タイルの種類を取得する
    mType = tileType;
}

void Tile::render( SDL_Rect& camera )
{
	//タイルが画面上にある場合
    if( checkCollision( camera, mBox ) )
    {
		//タイルを表示
        gTileTexture.render( mBox.x - camera.x, mBox.y - camera.y, &gTileClips[ mType ] );
    }
}

CELL_TYPE Tile::getType()
{
    return mType;
}

SDL_Rect Tile::getBox()
{
    return mBox;
}