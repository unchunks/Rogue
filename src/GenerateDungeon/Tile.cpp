#include "GenerateDungeon/Tile.h"

extern LTexture gTileTexture;
extern SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

Tile::Tile( int x, int y, CELL_TYPE tileType )
{
	//オフセットを取得する
    mBox.x = x;
    mBox.y = y;

	//コリジョンボックスを設定する
    mBox.w = TILE_W;
    mBox.h = TILE_H;

	//タイルの種類を取得する
    mType = tileType;
}

void Tile::render( SDL_Rect& camera )
{
	//タイルが画面上にある場合s
    if( AinB( mBox, camera) )
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
