#include "GenerateDungeon/Character.h"

LTexture Character::mCharTexture = LTexture();

Character::Character()
{
    mSpriteClips.resize(ANIMATION_FRAMES * static_cast<int>(NO_DIRECTION), {0, 0, 0, 0});
}

Character::Character(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, CHAR_TYPE _type)
    : isMoved(false), nowHP(_maxHP), maxHP(_maxHP), STR(_STR), VIT(_VIT), mState(_state), mDir(_dir), mType(_type), mBox({TILE_W * 2, TILE_H * 2, SPRITE_CHAR_WIDTH, SPRITE_CHAR_HEIGHT}), mAnimFrame(0)
{
}

Character::~Character()
{
    mSpriteClips.clear();
    mSpriteClips.shrink_to_fit();
}

bool Character::move(std::vector<Tile> _tiles, std::vector<Character> _otherCharacters)
{
    bool touched = false;
    // 次のマス目につくまで前フレームの移動を継続
    switch (mDir)
    {
    case LEFT:
        SDL_Log("move: 左に移動");
        mBox.x -= CHAR_VEL;
        // キャラクターが左右上下に行き過ぎた場合、または壁に触れた場合戻る
        if (collided(_tiles, _otherCharacters))
        {
            // mBox.x += CHAR_VEL;// + TILE_W / 4;
            mBox.x += (TILE_W / 4) - (mBox.x % TILE_W);
            touched = true;
        }
        if(mBox.y % TILE_H == TILE_H / 4)
        {
            mBox.y += (TILE_H / 4) - (mBox.y % TILE_H);
        }
        break;
    case RIGHT:
        SDL_Log("move: 右に移動");
        mBox.x += CHAR_VEL;
        if (collided(_tiles, _otherCharacters))
        {
            // mBox.x -= CHAR_VEL;// + TILE_W / 4;
            mBox.x += (TILE_W / 4) - (mBox.x % TILE_W);
            touched = true;
        }
        if(mBox.y % TILE_H == TILE_H / 4)
        {
            mBox.y += (TILE_H / 4) - (mBox.y % TILE_H);
        }
        break;
    case UP:
        SDL_Log("move: 上に移動");
        mBox.y -= CHAR_VEL;
        if (collided(_tiles, _otherCharacters))
        {
            // mBox.y += CHAR_VEL;// + TILE_H / 4;
            mBox.y += (TILE_H / 4) - (mBox.y % TILE_H);
            touched = true;
        }
        if(mBox.x % TILE_W == TILE_W / 4)
        {
            mBox.x += (TILE_W / 4) - (mBox.x % TILE_W);
        }
        break;
    case DOWN:
        SDL_Log("move: 下に移動");
        mBox.y += CHAR_VEL;
        if (collided(_tiles, _otherCharacters))
        {
            // mBox.y -= CHAR_VEL;// + TILE_H / 4;
            mBox.y += (TILE_H / 4) - (mBox.y % TILE_H);
            touched = true;
        }
        if(mBox.x % TILE_W == TILE_W / 4)
        {
            mBox.x += (TILE_W / 4) - (mBox.x % TILE_W);
        }
        break;
    case NO_DIRECTION:
        break;
    }
    return touched;
    // SDL_Log("move: (x: %d, y: %d)\n", ( mBox.x + mBox.w / 2 ) % TILE_W, ( mBox.y + mBox.h / 2 ) % TILE_H);
}

//FIX 斜めでも反応する
bool Character::adjacent(Character _opponent)
{
    Character oppo = _opponent;

    oppo.mBox.y -= TILE_H;
    if( touchChar(oppo) )
    {
        mDir = UP;
        SDL_Log("adjacent: キャラに隣接（上）");
        return true;
    }

    oppo.mBox = _opponent.mBox;
    oppo.mBox.y += TILE_H;
    if( touchChar(oppo) )
    {
        mDir = DOWN;
        SDL_Log("adjacent: キャラに隣接（下）");
        return true;
    }

    oppo.mBox = _opponent.mBox;
    oppo.mBox.x -= TILE_W;
    if( touchChar(oppo) )
    {
        mDir = LEFT;
        SDL_Log("adjacent: キャラに隣接（左）");
        return true;
    }

    oppo.mBox = _opponent.mBox;
    oppo.mBox.x += TILE_W;
    if( touchChar(oppo) )
    {
        mDir = RIGHT;
        SDL_Log("adjacent: キャラに隣接（右）");
        return true;
    }
    return false;
}

void Character::attack(Character &_opponent)
{
    _opponent.receiveDamage(STR);
}

void Character::receiveDamage(int _damage)
{
    _damage -= VIT;
    if (_damage < -50)
        return;
    if (_damage <= 0)
        _damage = 1;
    nowHP -= _damage;
    if (nowHP <= 0)
    {
        mState = DEAD;
    }
}

void Character::setPos(int _x, int _y)
{
    mBox.x = _x;
    mBox.y = _y;
}

void Character::setPos(glm::vec2 _pos)
{
    mBox.x = _pos.x;
    mBox.y = _pos.y;
}

void Character::setState(STATE _state)
{
    mState = _state;
}

void Character::setDir(DIRECTION _dir)
{
    mDir = _dir;
}

void Character::setCamera(SDL_Rect &_camera)
{
    // Center the _camera over the player
    _camera.x = (mBox.x + SPRITE_CHAR_WIDTH / 2) - SCREEN_WIDTH / 2;
    _camera.y = (mBox.y + SPRITE_CHAR_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    // Keep the _camera in bounds
    if (_camera.x < 0)
    {
        _camera.x = 0;
    }
    if (_camera.y < 0)
    {
        _camera.y = 0;
    }
    if (_camera.x > LEVEL_WIDTH - _camera.w)
    {
        _camera.x = LEVEL_WIDTH - _camera.w;
    }
    if (_camera.y > LEVEL_HEIGHT - _camera.h)
    {
        _camera.y = LEVEL_HEIGHT - _camera.h;
    }
}

void Character::render(SDL_Rect &_camera)
{
    int c_sprite_num = (static_cast<int>(mDir) * ANIMATION_FRAMES);
    if(isMoved)
    {
        c_sprite_num += (mAnimFrame++ * ANIM_SPEED / FPS);
    }
    else
    {
        mAnimFrame = 0;
    }
    if(mAnimFrame >= ANIMATION_FRAMES * FPS / ANIM_SPEED)
        mAnimFrame = 0;
    // キャラクターを表示
    mCharTexture.render(mBox.x - _camera.x, mBox.y - _camera.y, &mSpriteClips[c_sprite_num]);
}

// 以下private関数

bool Character::onTileCenter()
{
// SDL_Log("タイル内座標(x: %d, y: %d)\n", (mBox.x % TILE_W), (mBox.y % TILE_H));
    if ( ( abs( ( mBox.x % TILE_W ) - ( TILE_W / 4 ) ) < 5 )
      && ( abs( ( mBox.y % TILE_H )  - ( TILE_H / 4 ) ) < 5) )
    {
        mBox.x = (mBox.x / TILE_W) * TILE_W + TILE_W / 4;
        mBox.y = (mBox.y / TILE_H) * TILE_H + TILE_H / 4;
        return true;
    }
    return false;
}

bool Character::collided(std::vector<Tile> _tiles, std::vector<class Character> _otherCharacters)
{
    return (mapOver() || touchWall(_tiles) || touchChars(_otherCharacters));
}

bool Character::touchWall(std::vector<Tile> _tiles)
{
    // Go through the tiles
    for (auto _tile : _tiles)
    {
        // If the tile is a wall type tile
        if ((_tile.getType() == FLOOR) || (_tile.getType() == AISLE) || (_tile.getType() == STEP))
        {
            continue;
        }
        // If the collision box touches the wall tile
        if (checkCollision(mBox, _tile.getBox()))
        {
            SDL_Log("touchWall: 壁に接触");
            return true;
        }
    }

    // If no wall tiles were touched
    return false;
}

bool Character::touchChars(std::vector<Character> _otherCharacters)
{
    for(auto _otherCharacter : _otherCharacters)
    {
        if( (mBox.x / TILE_W + 1 == _otherCharacter.mBox.x / TILE_W + 1)
         && (mBox.y / TILE_H + 1 == _otherCharacter.mBox.y/ TILE_H + 1) )
        {
            SDL_Log("touchChars: キャラクターに接触");
            return true;
        }
    }
    return false;
}

bool Character::touchChar(Character  _otherCharacter)
{
    if( (mBox.x / TILE_W + 1 == _otherCharacter.mBox.x / TILE_W + 1)
        && (mBox.y / TILE_H + 1 == _otherCharacter.mBox.y/ TILE_H + 1) )
    {
        SDL_Log("touchChar: キャラクターに接触");
        SDL_Log("touchChar: (%d, %d) == (%d, %d)?", mBox.x / TILE_W + 1, mBox.y / TILE_H + 1, _otherCharacter.mBox.x / TILE_W + 1, _otherCharacter.mBox.y / TILE_H + 1);
        return true;
    }
    return false;
}

bool Character::mapOver()
{
    if( (mBox.x < 0) || (mBox.x + SPRITE_CHAR_WIDTH > LEVEL_WIDTH)
     || (mBox.y < 0) || (mBox.y + SPRITE_CHAR_HEIGHT > LEVEL_HEIGHT))
    {
        SDL_Log("mapOver: 範囲外");
        return true;
    }
    else
        return false;
}