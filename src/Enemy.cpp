#include "GenerateDungeon/Enemy.h"

Enemy::Enemy(ENEMY_TYPE _enemy_type)
: Character(), elapsedTurn(0)
{
    enemy_type = _enemy_type;
    switch(enemy_type)
    {
        case DEKA:
            maxHP = DEKA_HP;
            STR   = DEKA_STR;
            VIT   = DEKA_VIT;
        break;

        case GURI:
            maxHP = GURI_HP;
            STR   = GURI_STR;
            VIT   = GURI_VIT;
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

Enemy::Enemy(int _x, int _y, int _maxHP, int _STR, int _VIT)
: Character(_x, _y, _maxHP, _STR, _VIT, SEARCH, DOWN, ENEMY), elapsedTurn(0)
{
}

Enemy::~Enemy()
{
    route.clear();
    route.shrink_to_fit();
}

void Enemy::walk(std::vector<class Tile> _tiles, Character _player, std::vector<Enemy> _otherEnemies)
{
    if(onTileCenter())
    {
        SDL_Log("walk: nextPos(%d, %d)", (int)nextPos.x, (int)nextPos.y);
        SDL_Log("walk: enemPos(%d, %d)", (int)mBox.x/TILE_W + 1, (int)mBox.y/TILE_H + 1);
        if(nextPos.x < mBox.x/TILE_W + 1) mDir = LEFT;
        else if(nextPos.x > mBox.x/TILE_W + 1) mDir = RIGHT;
        else if(nextPos.y < mBox.y/TILE_H + 1) mDir = UP;
        else if(nextPos.y > mBox.y/TILE_H + 1) mDir = DOWN;

        nextPos = route[0];
        route.pop_front();
        elapsedTurn++;
        SDL_Log("walk: elapsedTurn = %d, route size = %d\n", elapsedTurn, static_cast<int>(route.size()));
    }
    std::vector<Character> otherCharacters;
    otherCharacters.push_back(_player);
    for(auto enemy : _otherEnemies)
    {
        otherCharacters.push_back(enemy);
    }
    bool touched = move(_tiles, otherCharacters);
    if(touched)
    {
        elapsedTurn = ENEMY_SEARCH_INTERVAL + 1;
    }
}

void Enemy::setGoal(CELL_TYPE _dungeon[FLOOR_H][FLOOR_W], glm::vec2 _goal)
{
    if(_goal.x > FLOOR_W || _goal.y > FLOOR_H || _goal.x < 0 || _goal.y < 0)
    {
        return;
    }
    goal = _goal;
    route.clear();
    route = AStar::AStar(_dungeon, glm::vec2(mBox.x / TILE_W, mBox.y / TILE_H), goal);
    // 現在地をポップ
    route.pop_front();
    if(route.size() == 0)
    {
        return;
    }
    SDL_Log("setGoal: 更新後のルート一覧");
    for(auto vec2 : route)
    {
    SDL_Log("setGoal: (%d, %d)", (int)vec2.x, (int)vec2.y);
    }

    nextPos = route.at(0);
    route.pop_front();
    elapsedTurn = 0;
}

void Enemy::attack(Character& _opponent)
{
    if(!onTileCenter())
        return;

    elapsedTurn++;
    _opponent.receiveDamage(STR);
}

bool Enemy::find(Character _opponent)
{
    if(!onTileCenter())
        return false;

    return 
      ( abs(mBox.x - _opponent.getImagePos().x) / TILE_W 
      + abs(mBox.y - _opponent.getImagePos().y) / TILE_H 
            <= ENEMY_FIND_RANGE);
}

bool Enemy::mustUpdateRoute()
{
    if(!onTileCenter())
        return false;

    bool result = ((static_cast<int>(route.size()) < 1) || (elapsedTurn > ENEMY_SEARCH_INTERVAL));
    if( static_cast<int>(route.size()) < 1 )
    {
        SDL_Log("mustUpdateRoute: ルートサイズが少ない %d", static_cast<int>(route.size()));
    }
    if( elapsedTurn > ENEMY_SEARCH_INTERVAL )
    {
        SDL_Log("mustUpdateRoute: 経過ターン超過 %d", elapsedTurn);
    }
    if(result)
    {
        SDL_Log("mustUpdateRoute: 更新要求");
    }
    return result;
}

bool Enemy::changeState(Character _opponent)
{
    if(!onTileCenter())
        return false;

    if (find(_opponent) && mState != FOUND)
    {
        SDL_Log("プレイヤー発見");
        setState(FOUND);
        routeClear();
        return true;
    }
    else if(!find(_opponent) && mState != SEARCH)
    {
        SDL_Log("プレイヤー未発見");
        setState(SEARCH);
        routeClear();
        return false;
    }

    return false;
}