#include "GenerateDungeon/Character.h"

LTexture Character::mCharTexture = LTexture();

Character::Character()
{
    mSpriteClips.resize(ANIMATION_FRAMES * static_cast<int>(NO_DIRECTION), {0, 0, 0, 0});
}

Character::Character(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, CHAR_TYPE _type)
    : isMoved(false), nowHP(_maxHP), maxHP(_maxHP), STR(_STR), VIT(_VIT), mState(_state), mDir(_dir), mType(_type), mBox({TILE_WIDTH * 2, TILE_HEIGHT * 2, SPRITE_CHAR_WIDTH, SPRITE_CHAR_HEIGHT}), mAnimFrame(0)
{
}

Character::~Character()
{
    mSpriteClips.clear();
    mSpriteClips.shrink_to_fit();
}

/// @brief 向いている方向に移動。当たり判定も含む
/// @param _tiles 当たり判定用の全タイルの配列
bool Character::move(std::vector<Tile> _tiles, std::vector<Character> _otherCharacters)
{
    bool touched = false;
    // 次のマス目につくまで前フレームの移動を継続
    switch (mDir)
    {
    case LEFT:
        mBox.x -= CHAR_VEL;
        // キャラクターが左右上下に行き過ぎた場合、または壁に触れた場合戻る
        if (mapOver() || touchWall(mBox, _tiles) || touchChar(_otherCharacters))
        {
            mBox.x += CHAR_VEL + TILE_WIDTH / 4;
            touched = true;
        }
        if(mBox.y % TILE_HEIGHT == TILE_HEIGHT / 4)
        {
            mBox.y += (TILE_HEIGHT / 4) - (mBox.y % TILE_HEIGHT);
        }
        break;
    case RIGHT:
        mBox.x += CHAR_VEL;
        if (mapOver() || touchWall(mBox, _tiles) || touchChar(_otherCharacters))
        {
            mBox.x -= CHAR_VEL + TILE_WIDTH / 4;
            touched = true;
        }
        if(mBox.y % TILE_HEIGHT == TILE_HEIGHT / 4)
        {
            mBox.y += (TILE_HEIGHT / 4) - (mBox.y % TILE_HEIGHT);
        }
        break;
    case UP:
        mBox.y -= CHAR_VEL;
        if (mapOver() || touchWall(mBox, _tiles) || touchChar(_otherCharacters))
        {
            mBox.y += CHAR_VEL + TILE_HEIGHT / 4;
            touched = true;
        }
        if(mBox.x % TILE_WIDTH == TILE_WIDTH / 4)
        {
            mBox.x += (TILE_WIDTH / 4) - (mBox.x % TILE_WIDTH);
        }
        break;
    case DOWN:
        mBox.y += CHAR_VEL;
        if (mapOver() || touchWall(mBox, _tiles) || touchChar(_otherCharacters))
        {
            mBox.y -= CHAR_VEL + TILE_HEIGHT / 4;
            touched = true;
        }
        if(mBox.x % TILE_WIDTH == TILE_WIDTH / 4)
        {
            mBox.x += (TILE_WIDTH / 4) - (mBox.x % TILE_WIDTH);
        }
        break;
    case NO_DIRECTION:
        break;
    }
    if(onTileCenter())
    {
        SDL_Log("移動完了");
    }
    return touched;
    // SDL_Log("(x: %d, y: %d)\n", ( mBox.x + mBox.w / 2 ) % TILE_WIDTH, ( mBox.y + mBox.h / 2 ) % TILE_HEIGHT);
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


/// @brief 座標テレポート
/// @param _x X座標(画像系の座標)
/// @param _y Y座標(画像系の座標)
void Character::setPos(int _x, int _y)
{
    mBox.x = _x;
    mBox.y = _y;
}

/// @brief 座標テレポート
/// @param _pos 座標(画像系の座標)
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
    // プレイヤーを表示
    mCharTexture.render(mBox.x - _camera.x, mBox.y - _camera.y, &mSpriteClips[c_sprite_num]);
}

// 以下private関数

bool Character::onTileCenter()
{
// SDL_Log("タイル内座標(x: %d, y: %d)\n", (mBox.x % TILE_WIDTH), (mBox.y % TILE_HEIGHT));
    if ((mBox.x % TILE_WIDTH == TILE_WIDTH / 4) &&
        (mBox.y % TILE_HEIGHT == TILE_HEIGHT / 4))
        return true;
    return false;
}

/// @brief 矩形とタイルの衝突判定
/// @param box 移動している物体の当たり判定
/// @param _tiles 全タイルの配列
bool Character::touchWall(SDL_Rect box, std::vector<Tile> _tiles)
{
    // Go through the tiles
    for (auto _tile : _tiles)
    {
        // If the tile is a wall type tile
        if ((_tile.getType() != FLOOR) && (_tile.getType() != AISLE) && (_tile.getType() != STEP))
        {
            // If the collision box touches the wall tile
            if (checkCollision(box, _tile.getBox()))
            {
                return true;
            }
        }
    }

    // If no wall tiles were touched
    return false;
}

bool Character::touchChar(std::vector<Character> _otherCharacters)
{
    for(auto _otherCharacter : _otherCharacters)
    {
        if( (mBox.x + mBox.w > _otherCharacter.mBox.x)
         && (mBox.x < _otherCharacter.mBox.x + _otherCharacter.mBox.w)
         && (mBox.y + mBox.h > _otherCharacter.mBox.y)
         && (mBox.y < _otherCharacter.mBox.y + _otherCharacter.mBox.h) )
        {
            return true;
        }
    }
    return false;
}

bool Character::mapOver()
{
    if( (mBox.x < 0) || (mBox.x + SPRITE_CHAR_WIDTH > LEVEL_WIDTH)
     || (mBox.y < 0) || (mBox.y + SPRITE_CHAR_HEIGHT > LEVEL_HEIGHT))
        return true;
    else
        return false;
}