#include "GenerateDungeon/Character.h"

extern int WIN_W;
extern int WIN_H;

Character::Character()
{
}

Character::Character(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, CHAR_TYPE _type)
:mState(_state), mDir(_dir), mType(_type)
{
    mPos.x = _x;
    mPos.y = _y;
    maxHP = nowHP = _maxHP;
    STR = _STR;
    VIT = _VIT;

    //コリジョンボックスを初期化する
    mBox.x = TILE_WIDTH*2;
    mBox.y = TILE_HEIGHT*2;
	mBox.w = SPRITE_CHAR_WIDTH;
	mBox.h = SPRITE_CHAR_HEIGHT;

    //速度を初期化する
    mVelX = 0;
    mVelY = 0;
}

void Character::move(DIRECTION _dir) {
    switch (_dir) {
        case LEFT:  mPos.x--; break;
        case RIGHT: mPos.x++; break;
        case UP:    mPos.y--; break;
        case DOWN:  mPos.y++; break;
        default:    break;
    }
    mDir = _dir;
}

void Character::moveTo(glm::vec2 _pos) {
    if(_pos.x > mPos.x) move(RIGHT);
    else if(_pos.x < mPos.x) move(LEFT);
    else if(_pos.y > mPos.y) move(DOWN);
    else move(UP);
}

void Character::back() {
    switch (mDir) {
        case LEFT:  mPos.x++; break;
        case RIGHT: mPos.x--; break;
        case UP:    mPos.y++; break;
        case DOWN:  mPos.y--; break;
        default:    break;
    }
}

void Character::attack(Character& _opponent) {
    _opponent.receiveDamage(STR);
}

void Character::receiveDamage(int _damage) {
    _damage -= VIT;
    if(_damage < -50) return;
    if(_damage <= 0) _damage = 1;
    nowHP -= _damage;
    if(nowHP <= 0) {
        mState = DEAD;
    }
}

void Character::setPos(int _x, int _y) {
    mPos.x = _x;
    mPos.y = _y;
}

void Character::setPos(glm::vec2 _pos) {
    mPos.x = _pos.x;
    mPos.y = _pos.y;
}

void Character::setState(STATE _state) {
    mState = _state;
}


void Character::setDir(DIRECTION _dir) {
    mDir = _dir;
}
//TODO
void Character::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
		//速度を調整する
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: 	 mVelY -= DOT_VEL; mDir = UP; 	 break;
            case SDLK_DOWN:  mVelY += DOT_VEL; mDir = DOWN;  break;
            case SDLK_LEFT:  mVelX -= DOT_VEL; mDir = LEFT;  break;
            case SDLK_RIGHT: mVelX += DOT_VEL; mDir = RIGHT; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP:
            case SDLK_DOWN:  mVelY = 0; break;
            case SDLK_LEFT:
            case SDLK_RIGHT: mVelX = 0; break;
        }
    }
}

void Character::move( Tile *tiles[] )
{
    //Move the player left or right
    mBox.x += mVelX;

    //If the player went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + SPRITE_CHAR_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
    {
        //move back
        mBox.x -= mVelX;
    }
	// OPTIMIZE: プレイヤーが中心に来るように（X座標）
	if( mVelX == 0 ) {
		if(( mBox.x + SPRITE_CHAR_WIDTH / 2 ) % TILE_WIDTH  > TILE_WIDTH / 2 ) {
			mBox.x -= DOT_BACK_VEL;
		}
		else if(( mBox.x + SPRITE_CHAR_WIDTH / 2 ) % TILE_WIDTH  < TILE_WIDTH / 2 ) {
			mBox.x += DOT_BACK_VEL;
		}
	}

    //Move the player up or down
    mBox.y += mVelY;

    //If the player went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + SPRITE_CHAR_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
    {
        //move back
        mBox.y -= mVelY;
    }
	// OPTIMIZE: プレイヤーが中心に来るように（Y座標）
	if( mVelY == 0 ) {
		if( ( mBox.y + SPRITE_CHAR_HEIGHT / 2 ) % TILE_HEIGHT  > 64) {
			mBox.y -= DOT_BACK_VEL;
		}
		else if( ( mBox.y + SPRITE_CHAR_HEIGHT / 2 ) % TILE_HEIGHT  < 64) {
			mBox.y += DOT_BACK_VEL;
		}
	}
}

void Character::setCamera( SDL_Rect& camera )
{
	//Center the camera over the player
	camera.x = ( mBox.x + SPRITE_CHAR_WIDTH / 2 ) - WIN_W / 2;
	camera.y = ( mBox.y + SPRITE_CHAR_HEIGHT / 2 ) - WIN_H / 2;

	//Keep the camera in bounds
	if( camera.x < 0 )
	{ 
		camera.x = 0;
	}
	if( camera.y < 0 )
	{
		camera.y = 0;
	}
	if( camera.x > LEVEL_WIDTH - camera.w )
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if( camera.y > LEVEL_HEIGHT - camera.h )
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Character::render( SDL_Rect& camera )
{
	int p_sprite_num = ( static_cast<int>( mDir ) * ANIMATION_FRAMES );
	//止まっているときは動かない
	if( ( mVelX != 0 ) || ( mVelY != 0 ) )
		p_sprite_num += (anim_frame * ANIM_SPEED / FPS);
	//プレイヤーを表示
	gPlayerTexture.render( mBox.x - camera.x, mBox.y - camera.y, &gSpriteClips[ p_sprite_num ]);
}
