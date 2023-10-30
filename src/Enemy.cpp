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

Enemy::Enemy(int _x, int _y, int _maxHP, int _STR, int _VIT, const char* _icon)
: Character(_x, _y, _maxHP, _STR, _VIT, SEARCH, DOWN, ENEMY), elapsedTurn(0), icon(_icon)
{
}

Enemy::~Enemy()
{
    route.clear();
    route.shrink_to_fit();
}

// void Enemy::walk()
// {
//     nextPos = route[0];
//     route.pop_front();
//     moveTo(nextPos);
//     elapsedTurn++;
// }

void Enemy::setGoal(CELL_TYPE dungeon[FLOOR_H][FLOOR_W], glm::vec2 _goal)
{
    goal = _goal;
    std::cout << "GOAL ==> (" << goal.x << ", " << goal.y << ")\n";
    route.clear();
    route = AStar::AStar(dungeon, glm::vec2(mBox.x, mBox.y), goal);
    route.pop_front();
    elapsedTurn = 0;
}

void Enemy::attack(Character& _opponent) {
    elapsedTurn++;
    _opponent.receiveDamage(STR);
}
