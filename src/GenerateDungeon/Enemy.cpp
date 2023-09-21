#include "Enemy.h"

Enemy::Enemy(ENEMY_TYPE _enemy_type)
: Character()
{
    enemy_type = _enemy_type;
    switch(enemy_type)
    {
    case SLIME:
        maxHP = SLIME_HP;
        STR   = SLIME_STR;
        VIT   = SLIME_VIT;
        icon  = SLIME_ICON;
    break;
    case GOBLIN:
        maxHP = GOBLIN_HP;
        STR   = GOBLIN_STR;
        VIT   = GOBLIN_VIT;
        icon  = GOBLIN_ICON;
    break;
    }
    mPos = glm::vec2(0, 0);
    mState = SEARCH;
    mDir = DOWN;
    mType = ENEMY;
    nowHP = maxHP;
}

Enemy::Enemy(int _x, int _y, int _maxHP, int _STR, int _VIT, const char* _icon)
: Character(_x, _y, _maxHP, _STR, _VIT, SEARCH, DOWN, ENEMY), elapsedTurn(0), icon(_icon)
{
}

void Enemy::walk()
{
    nextPos = route[0];
    route.pop_front();
    moveTo(nextPos);
    elapsedTurn++;
}

void Enemy::setGoal(CELL_TYPE dungeon[FLOOR_H][FLOOR_W], glm::vec2 _goal)
{
    goal = _goal;
    std::cout << "GOAL ==> (" << goal.x << ", " << goal.y << ")\n";
    route.clear();
    route = AStar(dungeon, mPos, goal);
    route.pop_front();
    elapsedTurn = 0;
}

void Enemy::attack(Character& _opponent) {
    elapsedTurn++;
    _opponent.receiveDamage(STR);
}