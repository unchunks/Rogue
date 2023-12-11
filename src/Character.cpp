#include "GenerateDungeon/Character.h"

extern SDL_Renderer *gRenderer;

LTexture Character::mCharTexture = LTexture();

Character::Character()
    : mAnimFrame(0), receivingDamage(false), mName("キャラクター")
{
    sprile_clips.resize(ANIMATION_FRAMES * static_cast<int>(NO_DIRECTION), {0, 0, 0, 0});
}

Character::Character(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, CHAR_TYPE _type, std::string _name)
    : isMoved(false), nowHP(_maxHP), maxHP(_maxHP), STR(_STR), VIT(_VIT), mState(_state), mDir(_dir), mType(_type)
    , mBox({TILE_W * 2, TILE_H * 2, SPRITE_CHAR_WIDTH, SPRITE_CHAR_HEIGHT})
    , mAnimFrame(0), receivingDamage(false), mName(_name)
{
}

Character::~Character()
{
    sprile_clips.clear();
    sprile_clips.shrink_to_fit();
}

bool Character::move(std::vector<Tile> _tiles, std::vector<Character> _otherCharacters)
{
    Ivec2 front = getDataPos();
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
}

bool Character::moveTo(Ivec2 _destination, std::vector<Tile> _tiles, std::vector<Character> _otherCharacters)
{
   
    if(_destination.x < getDataPos().x)
    {
        mDir = LEFT;
    }
    else if(_destination.x > getDataPos().x)
    {
        mDir = RIGHT;
    }
    else if(_destination.y < getDataPos().y)
    {
        mDir = UP;
    }
    else if(_destination.y > getDataPos().y)
    {
        mDir = DOWN;
    }
    else if(onTileCenter())
    {
        return false;
    }

    SDL_Log("moveTo: 行き先(%d, %d)、現在地（%d, %d)", _destination.x, _destination.y, getDataPos().x, getDataPos().y);

    return move(_tiles, _otherCharacters);
}

DIRECTION Character::adjacent(Character _opponent)
{
    if(!onTileCenter())
        return NO_DIRECTION;

//FIX: プレイヤーが複数の敵を引数にしても正しく動くように
    if( (mBox.x == _opponent.mBox.x) && (mBox.y - TILE_H == _opponent.mBox.y) )
    {
        mDir = UP;

        SDL_Log("adjacent: キャラに隣接（上）");

        return mDir;
    }

    if( (mBox.x == _opponent.mBox.x) && (mBox.y + TILE_H == _opponent.mBox.y) )
    {
        mDir = DOWN;

        SDL_Log("adjacent: キャラに隣接（下）");

        return mDir;
    }

    if( (mBox.x - TILE_W == _opponent.mBox.x) && (mBox.y == _opponent.mBox.y) )
    {
        mDir = LEFT;

        SDL_Log("adjacent: キャラに隣接（左）");

        return mDir;
    }

    if( (mBox.x + TILE_W == _opponent.mBox.x) && (mBox.y == _opponent.mBox.y) )
    {
        mDir = RIGHT;

        SDL_Log("adjacent: キャラに隣接（右）");

        return mDir;
    }
    return NO_DIRECTION;
}

DIRECTION Character::adjacent(std::vector<Character> _opponents)
{
    for(auto oppo : _opponents)
    {
        if(adjacent(oppo) != NO_DIRECTION)
        {
            return adjacent(oppo);
        }
    }
    return NO_DIRECTION;
}

std::string Character::attack(Character &_opponent)
{
    if(!onTileCenter())
        return "";
    
    return mName + "の攻撃　" + _opponent.receiveDamage(STR);
}

std::string Character::receiveDamage(int _damage)
{
    receivingDamage = true;

    _damage -= VIT;
    if (_damage <= 0)
        _damage = 1;
    nowHP -= _damage;
    if (nowHP <= 0)
    {
        mState = DEAD;
    }
    std::string log = mName + "は" + std::to_string(_damage);
    if(mState == DEAD)
    {
        log += "ダメージを受けて倒れた";
    }
    else
    {
        log += "ダメージを受けた";
    }
    return log;
}

void Character::healed(int _heal_val)
{
    nowHP += _heal_val;
    if (nowHP > maxHP)
        nowHP = maxHP;
}

void Character::setImagePos(Ivec2 _image_pos)
{

    SDL_Log("setImagePos: (%d, %d)", _image_pos.x / TILE_W, _image_pos.y / TILE_H);

    mBox.x = _image_pos.x;
    mBox.y = _image_pos.y;
}

void Character::setDataPos(Ivec2 _data_pos)
{

    SDL_Log("setDataPos: (%d, %d)", _data_pos.x, _data_pos.y);

    mBox.x = _data_pos.x * TILE_W + TILE_W / 4;
    mBox.y = _data_pos.y * TILE_H + TILE_H / 4;
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
// TODO: HPはキャラの上に表示するかも
    int HP_BAR_H = 20;
    SDL_Rect hp_bar_frame = {
        mBox.x, 
        mBox.y - HP_BAR_H, 
        SPRITE_CHAR_WIDTH, 
        HP_BAR_H};
    SDL_Rect hp_bar = {
        mBox.x, 
        mBox.y - HP_BAR_H, 
        SPRITE_CHAR_WIDTH * nowHP / maxHP, 
        HP_BAR_H};
    SDL_Log("x: %d, y: %d, w: %d, h: %d", hp_bar_frame.x, hp_bar_frame.y, hp_bar_frame.w, hp_bar_frame.h);
    // SDL_Log("HPのフレームを表示");
    SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 255);
    SDL_RenderFillRect(gRenderer, &hp_bar_frame);

    // SDL_Log("HPを表示");
    SDL_SetRenderDrawColor(gRenderer, 0, 239, 166, 255);
    SDL_RenderFillRect(gRenderer, &hp_bar);

// REVIEW: 常にアニメーションを再生するかどうか検討
    int c_sprite_num = (static_cast<int>(mDir) * ANIMATION_FRAMES);
    if(isMoved)
    {
        mAnimFrame++;
        if(mAnimFrame >= ANIMATION_FRAMES * FPS / ANIM_SPEED)
        {
            mAnimFrame = 0;
        }
        c_sprite_num += (mAnimFrame * ANIM_SPEED / FPS);
    }
    else
    {
        mAnimFrame = 0;
    }
    // キャラクターを表示
    mCharTexture.render(mBox.x - _camera.x, mBox.y - _camera.y, &sprile_clips[c_sprite_num], receivingDamage);
    receivingDamage = false;  
}

Ivec2 Character::getFrontDataPos()
{
    switch(mDir)
    {
        case LEFT:  return getDataPos() + Ivec2( -1,  0); break;
        case RIGHT: return getDataPos() + Ivec2(  1,  0); break;
        case UP:    return getDataPos() + Ivec2(  0, -1); break;
        case DOWN:  return getDataPos() + Ivec2(  0,  1); break;
        case NO_DIRECTION: break;
    }
    return getDataPos();
}

bool Character::onTileCenter()
{
    if ( ( ( mBox.x % TILE_W ) == ( TILE_W / 4 ) )
      && ( ( mBox.y % TILE_H )  == ( TILE_H / 4 ) ) )
    {
        return true;
    }
    return false;
}

bool Character::collided(std::vector<Tile> _tiles, Ivec2 _data_pos, std::vector<class Character> _otherCharacters)
{
    return (mapOver() || touchWall(_tiles, _data_pos) || touchChars(_otherCharacters, _data_pos));
}

bool Character::touchWall(std::vector<Tile> _tiles, Ivec2 _data_pos)
{
    for (auto _tile : _tiles)
    {
        // 壁でなければスキップ
        if ((_tile.getType() == FLOOR) || (_tile.getType() == AISLE) || (_tile.getType() == STEP))
        {
            continue;
        }
        // 壁ならtrueを返す
        if (_data_pos.x == (_tile.getBox().x / TILE_W)
         && _data_pos.y == (_tile.getBox().y / TILE_H))
        {

            SDL_Log("touchWall: 壁に接触");

            return true;
        }
    }

    return false;
}

bool Character::touchChars(std::vector<Character> _otherCharacters, Ivec2 _data_pos)
{
    for(auto _otherCharacter : _otherCharacters)
    {
        if( touchChar(_otherCharacter, _data_pos) )
        {
            return true;
        }
    }
    return false;
}

bool Character::touchChar(Character _otherCharacter, Ivec2 _data_pos)
{
    if( (_data_pos == _otherCharacter.getDataPos()) )
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