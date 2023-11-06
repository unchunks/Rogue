#include "GenerateDungeon/Character.h"

LTexture Character::mCharTexture = LTexture();

Character::Character()
{
    mSpriteClips.resize(ANIMATION_FRAMES * static_cast<int>(NO_DIRECTION), {0, 0, 0, 0});
}

Character::Character(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, CHAR_TYPE _type)
    : nowHP(_maxHP), maxHP(_maxHP), STR(_STR), VIT(_VIT), mState(_state), mDir(_dir), mType(_type), mBox({TILE_WIDTH * 2, TILE_HEIGHT * 2, SPRITE_CHAR_WIDTH, SPRITE_CHAR_HEIGHT})
{
    mNowMoving = false;
}

Character::~Character()
{
    mSpriteClips.clear();
    mSpriteClips.shrink_to_fit();
}

/// @brief mNowMoving のとき向いている方向に移動。当たり判定も含む
/// @param _tiles 当たり判定用の全タイルの配列
void Character::move(std::vector<Tile> _tiles)
{
    // 移動中の場合、次のマス目につくまで前フレームの移動を継続
    if (!mNowMoving)
        return;

    switch (mMovingDir)
    {
    case LEFT:
        mDir = LEFT;
        mBox.x -= CHAR_VEL;
        // キャラクターが左右上下に行き過ぎた場合、または壁に触れた場合戻る
        if ((mBox.x < 0) || (mBox.x + SPRITE_CHAR_WIDTH > LEVEL_WIDTH) || touchesWall(mBox, _tiles))
        {
            mBox.x += CHAR_VEL + TILE_WIDTH / 4;
            mNowMoving = false;
        }
        break;
    case RIGHT:
        mDir = RIGHT;
        mBox.x += CHAR_VEL;
        if ((mBox.x < 0) || (mBox.x + SPRITE_CHAR_WIDTH > LEVEL_WIDTH) || touchesWall(mBox, _tiles))
        {
            mBox.x -= CHAR_VEL + TILE_WIDTH / 4;
            mNowMoving = false;
        }
        break;
    case UP:
        mDir = UP;
        mBox.y -= CHAR_VEL;
        if ((mBox.y < 0) || (mBox.y + SPRITE_CHAR_HEIGHT > LEVEL_HEIGHT) || touchesWall(mBox, _tiles))
        {
            mBox.y += CHAR_VEL + TILE_HEIGHT / 4;
            mNowMoving = false;
        }
        break;
    case DOWN:
        mDir = DOWN;
        mBox.y += CHAR_VEL;
        if ((mBox.y < 0) || (mBox.y + SPRITE_CHAR_HEIGHT > LEVEL_HEIGHT) || touchesWall(mBox, _tiles))
        {
            mBox.y -= CHAR_VEL + TILE_HEIGHT / 4;
            mNowMoving = false;
        }
        break;
    case NO_DIRECTION:
        break;
    }
    if (onTileCenter() && mNowMoving)
    {
        mNowMoving = false;
    }
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

void Character::render(SDL_Rect &_camera, int _anim_frame)
{
    int p_sprite_num = (static_cast<int>(mDir) * ANIMATION_FRAMES);
    // 移動中はアニメーション
    if (mNowMoving)
        p_sprite_num += (_anim_frame * ANIM_SPEED / FPS);
    // プレイヤーを表示
    mCharTexture.render(mBox.x - _camera.x, mBox.y - _camera.y, &mSpriteClips[p_sprite_num]);
}

// 以下private関数

bool Character::onTileCenter()
{
    if (((mBox.x + mBox.w / 2) % TILE_WIDTH == TILE_WIDTH / 2) &&
        ((mBox.y + mBox.h / 2) % TILE_HEIGHT == TILE_HEIGHT / 2))
        return true;
    return false;
}

/// @brief 矩形とタイルの衝突判定
/// @param box 移動している物体の当たり判定
/// @param _tiles 全タイルの配列
bool Character::touchesWall(SDL_Rect box, std::vector<Tile> _tiles)
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
