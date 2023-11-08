#include "GenerateDungeon/Enemy.h"

Enemy::Enemy(ENEMY_TYPE _enemy_type)
: Character()
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
        nextPos = route[0];
        if(nextPos.x > mBox.x/TILE_WIDTH) mDir = LEFT;
        if(nextPos.x < mBox.x/TILE_WIDTH) mDir = RIGHT;
        if(nextPos.y > mBox.y/TILE_HEIGHT) mDir = UP;
        if(nextPos.y < mBox.y/TILE_HEIGHT) mDir = DOWN;
        route.pop_front();
        elapsedTurn++;
        SDL_Log("elapsedTurn: %d\n", elapsedTurn);
    }
    std::vector<Character> otherCharacters;
    otherCharacters.push_back(_player);
    for(auto enemy : _otherEnemies)
    {
        otherCharacters.push_back(enemy);
    }
    move(_tiles, otherCharacters);
}

/// @brief 目的地を設定し、ルートを検索
/// @param dungeon データ系のマップ情報
/// @param _goal データ系の目的地の座標
void Enemy::setGoal(CELL_TYPE _dungeon[FLOOR_H][FLOOR_W], glm::vec2 _goal)
{
    if(_goal.x > FLOOR_W || _goal.y > FLOOR_H || _goal.x < 0 || _goal.y < 0)
    {
        return;
    }
    goal = _goal;
    std::cout << "GOAL ==> (" << goal.x << ", " << goal.y << ")\n";
    route.clear();
    route = AStar::AStar(_dungeon, glm::vec2(mBox.x / TILE_WIDTH, mBox.y / TILE_HEIGHT), goal);
    // 現在地をポップ
    route.pop_front();
    elapsedTurn = 0;
}

void Enemy::attack(Character& _opponent) {
    elapsedTurn++;
    _opponent.receiveDamage(STR);
}
