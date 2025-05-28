#pragma once

#include <SDL.h>

#include "Enum.h"
#include "Const.h"
#include "LTexture.h"

//タイルクラス
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

//ボックスの衝突検出
bool AinB( SDL_Rect a, SDL_Rect b );
