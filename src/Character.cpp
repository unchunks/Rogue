#include "GenerateDungeon/Character.h"

LTexture Character::mCharTexture = LTexture();

Character::Character()
    : mAnimFrame(0), receivingDamage(false)
{
    mSpriteClips.resize(ANIMATION_FRAMES * static_cast<int>(NO_DIRECTION), {0, 0, 0, 0});
}

Character::Character(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, CHAR_TYPE _type)
    : isMoved(false), nowHP(_maxHP), maxHP(_maxHP), STR(_STR), VIT(_VIT), mState(_state), mDir(_dir), mType(_type)
    , mBox({TILE_W * 2, TILE_H * 2, SPRITE_CHAR_WIDTH, SPRITE_CHAR_HEIGHT})
    , mAnimFrame(0), receivingDamage(false)
{
}

Character::~Character()
{
    mSpriteClips.clear();
    mSpriteClips.shrink_to_fit();
}

//REVIEW
bool Character::move(std::vector<Tile> _tiles, std::vector<Character> _otherCharacters)
{
    glm::vec2 front = getDataPos();
    switch (mDir)
    {
        case LEFT:  front.x--;  break;
        case RIGHT: front.x++;  break;
        case UP:    front.y--;  break;
        case DOWN:  front.y++;  break;
        default: break;
    }
    if(onTileCenter() && collided(_tiles, front, _otherCharacters))
    {
        SDL_Log("移動先に障害物あり");
        return true;
    }
    SDL_Log("移動先に障害物無し");
    // 次のマス目につくまで前フレームの移動を継続
    switch (mDir)
    {
    case LEFT:
        if(onTileCenter())
            SDL_Log("move: 左に移動");
        mBox.x -= CHAR_VEL;
        if(mBox.y % TILE_H != TILE_H / 4)
        {
            SDL_Log("上下位置調整");
            mBox.y += (TILE_H / 4) - (mBox.y % TILE_H);
        }
        break;
    case RIGHT:
        if(onTileCenter())
            SDL_Log("move: 右に移動");
        mBox.x += CHAR_VEL;
        if(mBox.y % TILE_H != TILE_H / 4)
        {
            SDL_Log("上下位置調整");
            mBox.y += (TILE_H / 4) - (mBox.y % TILE_H);
        }
        break;
    case UP:
        if(onTileCenter())
            SDL_Log("move: 上に移動");
        mBox.y -= CHAR_VEL;
        if(mBox.x % TILE_W != TILE_W / 4)
        {
            SDL_Log("左右位置調整");
            mBox.x += (TILE_W / 4) - (mBox.x % TILE_W);
        }
        break;
    case DOWN:
        if(onTileCenter())
            SDL_Log("move: 下に移動");
        mBox.y += CHAR_VEL;
        if(mBox.x % TILE_W != TILE_W / 4)
        {
            SDL_Log("左右位置調整");
            mBox.x += (TILE_W / 4) - (mBox.x % TILE_W);
        }
        break;
    case NO_DIRECTION:
        break;
    }
    return false;
    // SDL_Log("move: (x: %d, y: %d)\n", ( mBox.x + mBox.w / 2 ) % TILE_W, ( mBox.y + mBox.h / 2 ) % TILE_H);
}

//REVIEW
bool Character::moveTo(glm::vec2 _destination, std::vector<Tile> _tiles, std::vector<Character> _otherCharacters)
{
    // if(getDataPos() == _destination)
    // {
    //     setDataPos(_destination);
    //     return false;
    // }

    if(!onTileCenter())
    {
        return move(_tiles, _otherCharacters);
    }
    else if(_destination.x < getDataPos().x)
    {
        SDL_Log("moveTo: 行き先(%d, %d)、現在地（%d, %d)", (int)_destination.x, (int)_destination.y, (int)getDataPos().x, (int)getDataPos().y);
        mDir = LEFT;
        return move(_tiles, _otherCharacters);
    }
    else if(_destination.x > getDataPos().x)
    {
        SDL_Log("moveTo: 行き先(%d, %d)、現在地（%d, %d)", (int)_destination.x, (int)_destination.y, (int)getDataPos().x, (int)getDataPos().y);
        mDir = RIGHT;
        return move(_tiles, _otherCharacters);
    }
    else if(_destination.y < getDataPos().y)
    {
        SDL_Log("moveTo: 行き先(%d, %d)、現在地（%d, %d)", (int)_destination.x, (int)_destination.y, (int)getDataPos().x, (int)getDataPos().y);
        mDir = UP;
        return move(_tiles, _otherCharacters);
    }
    else if(_destination.y > getDataPos().y)
    {
        SDL_Log("moveTo: 行き先(%d, %d)、現在地（%d, %d)", (int)_destination.x, (int)_destination.y, (int)getDataPos().x, (int)getDataPos().y);
        mDir = DOWN;
        return move(_tiles, _otherCharacters);
    }
    return false;
}

bool Character::adjacent(Character _opponent)
{
    if(!onTileCenter())
        return false;
//REVIEW: プレイヤーが複数の敵を引数にしても正しく動くように
    if( (mBox.x == _opponent.mBox.x) && (mBox.y - TILE_H == _opponent.mBox.y) )
    {
        mDir = UP;
        SDL_Log("adjacent: キャラに隣接（上）");
        return true;
    }

    if( (mBox.x == _opponent.mBox.x) && (mBox.y + TILE_H == _opponent.mBox.y) )
    {
        mDir = DOWN;
        SDL_Log("adjacent: キャラに隣接（下）");
        return true;
    }

    if( (mBox.x - TILE_W == _opponent.mBox.x) && (mBox.y == _opponent.mBox.y) )
    {
        mDir = LEFT;
        SDL_Log("adjacent: キャラに隣接（左）");
        return true;
    }

    if( (mBox.x + TILE_W == _opponent.mBox.x) && (mBox.y == _opponent.mBox.y) )
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
    receivingDamage = true;

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

void Character::setImagePos(glm::vec2 _pos)
{
    SDL_Log("setImagePos: (%d, %d)", (int)_pos.x / TILE_W, (int)_pos.y / TILE_H);
    mBox.x = _pos.x;
    mBox.y = _pos.y;
}

void Character::setDataPos(glm::vec2 _pos)
{
    SDL_Log("setDataPos: (%d, %d)", (int)_pos.x, (int)_pos.y);
    mBox.x = _pos.x * TILE_W + TILE_W / 4;
    mBox.y = _pos.y * TILE_H + TILE_H / 4;
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
        receivingDamage = false;
    }
    if(mAnimFrame >= ANIMATION_FRAMES * FPS / ANIM_SPEED)
    {
        mAnimFrame = 0;
        receivingDamage = false;
    }
    // キャラクターを表示
    mCharTexture.render(mBox.x - _camera.x, mBox.y - _camera.y, &mSpriteClips[c_sprite_num], receivingDamage);    
}

// 以下private関数

bool Character::onTileCenter()
{
    // SDL_Log("タイル内座標(x: %d, y: %d)\n", (mBox.x % TILE_W), (mBox.y % TILE_H));
    if ( ( ( mBox.x % TILE_W ) == ( TILE_W / 4 ) )
      && ( ( mBox.y % TILE_H )  == ( TILE_H / 4 ) ) )
    {
        // SDL_Log("タイルの中央");
        // mBox.x = (mBox.x / TILE_W) * TILE_W + TILE_W / 4;
        // mBox.y = (mBox.y / TILE_H) * TILE_H + TILE_H / 4;
        return true;
    }
    return false;
}

bool Character::collided(std::vector<Tile> _tiles, glm::vec2 _pos, std::vector<class Character> _otherCharacters)
{
    return (mapOver() || touchWall(_tiles, _pos) || touchChars(_otherCharacters, _pos));
}

//TODO: 上と左の壁を通り抜ける
bool Character::touchWall(std::vector<Tile> _tiles, glm::vec2 _pos)
{
    for (auto _tile : _tiles)
    {
        // 壁でなければスキップ
        if ((_tile.getType() == FLOOR) || (_tile.getType() == AISLE) || (_tile.getType() == STEP))
        {
            continue;
        }
        // 壁ならtrueを返す
        if (_pos.x == (_tile.getBox().x / TILE_W)
         && _pos.y == (_tile.getBox().y / TILE_H))
        {
            SDL_Log("touchWall: 壁に接触");
            return true;
        }
    }

    return false;
}

bool Character::touchChars(std::vector<Character> _otherCharacters, glm::vec2 _pos)
{
    for(auto _otherCharacter : _otherCharacters)
    {
        if( touchChar(_otherCharacter, _pos) )
        {
            return true;
        }
    }
    return false;
}

bool Character::touchChar(Character  _otherCharacter, glm::vec2 _pos)
{
    if( (_pos == _otherCharacter.getDataPos()) )
        {
            SDL_Log("touchChar: キャラクターに接触");
            return true;
        }
    return false;
}

bool Character::mapOver()
{
    if( (getDataPos().x < 0) || (getDataPos().x >= FLOOR_W)
     || (getDataPos().y < 0) || (getDataPos().y >= FLOOR_H))
    {
        SDL_Log("mapOver: 範囲外");
        return true;
    }
    else
        return false;
}