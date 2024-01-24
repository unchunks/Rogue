#include "GenerateDungeon/Enemy.h"

Enemy::Enemy(const Enemy& other)
:ID(other.ID)
{
    maxHP = other.maxHP;
    STR   = other.STR;
    VIT   = other.VIT;
    mName = other.mName;
    enemy_type = other.enemy_type;
    mBox.x = other.mBox.x;
    mBox.y = other.mBox.y;
    mState = other.mState;
    mDir = other.mDir;
    mType = other.mType;
    nowHP = other.nowHP;
}

Enemy::Enemy(ENEMY_TYPE _enemy_type, int _id)
: Character(), ID(_id), elapsedTurn(0)
{
    enemy_type = _enemy_type;
    switch(enemy_type)
    {
        case DEKA:
            maxHP = DEKA_HP;
            STR   = DEKA_STR;
            VIT   = DEKA_VIT;
            EXP   = DEKA_EXP;
            mName = "　　デ　カ　　";
            break;

        case GURI:
            maxHP = GURI_HP;
            STR   = GURI_STR;
            VIT   = GURI_VIT;
            EXP   = GURI_EXP;
            mName = "　　グ　リ　　";
            break;

        case JELYF:
            maxHP = JELYF_HP;
            STR   = JELYF_STR;
            VIT   = JELYF_VIT;
            EXP   = JELYF_EXP;
            mName = "　 ジェリフ 　";
            break;

        case YUMMY:
            maxHP = YUMMY_HP;
            STR   = YUMMY_STR;
            VIT   = YUMMY_VIT;
            EXP   = YUMMY_EXP;
            mName = "　　ヤミー　　";
            break;
        
        case CRYSTAL:
            maxHP = CRYSTAL_HP;
            STR   = CRYSTAL_STR;
            VIT   = CRYSTAL_VIT;
            EXP   = CRYSTAL_EXP;
            mName = "　クリスタル　";
            break;

        case GROSSPIDER:
            maxHP = GROSSPIDER_HP;
            STR   = GROSSPIDER_STR;
            VIT   = GROSSPIDER_VIT;
            EXP   = GROSSPIDER_EXP;
            mName = "グロスパイダー";
            break;

        case IRON:
            maxHP = IRON_HP;
            STR   = IRON_STR;
            VIT   = IRON_VIT;
            EXP   = IRON_EXP;
            mName = "　 アイアン　";
            break;
        
        case TATSU:
            maxHP = TATSU_HP;
            STR   = TATSU_STR;
            VIT   = TATSU_VIT;
            EXP   = TATSU_EXP;
            mName = "　　タ　ツ　　";
            break;
        
        case BALL:
            maxHP = BALL_HP;
            STR   = BALL_STR;
            VIT   = BALL_VIT;
            EXP   = BALL_EXP;
            mName = "　　ボール　　";
            break;

        case ENEMY_TYPE_NUMBER: break;
    }
    mBox.x = 0;
    mBox.y = 0;
    mState = SEARCH;
    mDir = DOWN;
    mType = ENEMY;
    nowHP = maxHP;
}

Enemy::Enemy(int _x, int _y, int _maxHP, int _STR, int _VIT, int _id)
: Character(), ID(_id), elapsedTurn(0)
{
    enemy_type = DEKA;
    maxHP = DEKA_HP;
    STR   = DEKA_STR;
    VIT   = DEKA_VIT;
    mName = "　オリジナル　";
    mBox.x = 0;
    mBox.y = 0;
    mState = SEARCH;
    mDir = DOWN;
    mType = ENEMY;
    nowHP = maxHP;
    EXP = DEKA_EXP;
}

Enemy::~Enemy()
{
    route.clear();
    route.shrink_to_fit();
}

void Enemy::walk(std::vector<class Tile> _tiles, class Character& _player, const std::vector<class Enemy>& _otherEnemies)
{
    std::vector<Character> otherCharacters;
    otherCharacters.push_back(static_cast<Character>(_player));
    for(auto enemy : _otherEnemies)
    {
        if(enemy.getDataPos() == getDataPos())
            continue;

        otherCharacters.push_back(static_cast<Character>(enemy));
    }
    // if(onTileCenter())
    // {
    //     SDL_Log("walk: nextPos(%d, %d)", nextPos.x, nextPos.y);
    //     SDL_Log("walk: enemPos(%d, %d)", getDataPos().x, getDataPos().y);
    // }

    // SDL_Log("タイル内座標(x: %d, y: %d)\n", (mBox.x % TILE_W), (mBox.y % TILE_H));

    bool touched = moveTo(nextPos, _tiles, otherCharacters);
    if(touched)
    {
        elapsedTurn = ENEMY_SEARCH_INTERVAL + 1;
    }

    if(onTileCenter() && mState == SEARCH)
    {
        nextPos = route[0];
        route.pop_front();
        elapsedTurn++;

        // SDL_Log("walk: %2d %s elapsedTurn = %3d, route size = %3d\n", ID, mName.c_str(), elapsedTurn, static_cast<int>(route.size()));

    }
}

void Enemy::walkTo(Ivec2 _destination, std::vector<class Tile> _tiles, class Character& _player, const std::vector<class Enemy>& _otherEnemies)
{
    _destination.x = _destination.x / TILE_W;
    _destination.y = _destination.y / TILE_H;

    nextPos = _destination;

    walk(_tiles, _player, _otherEnemies);
}

void Enemy::setGoal(CELL_TYPE _dungeon[FLOOR_H][FLOOR_W], Ivec2 _goal)
{
    if(_goal.x > FLOOR_W || _goal.y > FLOOR_H || _goal.x < 0 || _goal.y < 0)
    {
        SDL_Log("Goal position error");
        return;
    }
    goal = _goal;
    route.clear();

    // SDL_Log("setGoal: ルート探索");

    route = AStar::AStar(_dungeon, getDataPos(), goal);
    // 現在地をポップ

    // SDL_Log("現在地としてポップ(%d, %d)", route.at(0).x, route.at(0).y);

    route.pop_front();
    if(route.size() == 0)
    {
        return;
    }

    // SDL_Log("setGoal: 更新後のルート一覧");
    // for(auto vec2 : route)
    // {
    //     SDL_Log("setGoal: (%d, %d)", vec2.x, vec2.y);
    // }

    nextPos = route.at(0);
    route.pop_front();
    elapsedTurn = 0;
}

std::string Enemy::attack(Character& _opponent)
{
    if(!onTileCenter())
        return "";
    
    elapsedTurn++;
    return mName + "の攻撃　" + _opponent.receiveDamage(STR);
}

bool Enemy::find(Character _opponent)
{
    if(!onTileCenter())
        return false;

    return ( getDataPos().manhattan(_opponent.getDataPos()) <= ENEMY_FIND_RANGE );
}

bool Enemy::mustUpdateRoute()
{
    if(!onTileCenter())
        return false;

    bool result = ((static_cast<int>(route.size()) < 1) || (elapsedTurn > ENEMY_SEARCH_INTERVAL));

    // if( static_cast<int>(route.size()) < 1 )
    // {
    //     SDL_Log("mustUpdateRoute: ルートサイズが少ない %d", static_cast<int>(route.size()));
    // }
    // if( elapsedTurn > ENEMY_SEARCH_INTERVAL )
    // {
    //     SDL_Log("mustUpdateRoute: 経過ターン超過 %d", elapsedTurn);
    // }

    if(result)
    {

        // SDL_Log("mustUpdateRoute: 更新要求");

        routeClear();
    }
    return result;
}

bool Enemy::changeState(Character _opponent)
{
    if(!onTileCenter())
        return false;

    if(nowHP <= 0)
    {
        setState(DEAD);
        routeClear();
        return true;
    }
    else if (find(_opponent) && mState != FOUND)
    {

        // SDL_Log("プレイヤー発見");

        setState(FOUND);
        routeClear();
        return true;
    }
    else if(!find(_opponent) && mState != SEARCH)
    {

        // SDL_Log("プレイヤー未発見");

        setState(SEARCH);
        routeClear();
        return false;
    }

    return false;
}