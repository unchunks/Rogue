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

//NOTE: 独立した関数

bool AinB( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( (bottomA > topB) && (topA < bottomB) && (rightA > leftB) && (leftA < rightB))
    {
        return true;
    }

    //If none of the sides from A are outside B
    return false;
}
