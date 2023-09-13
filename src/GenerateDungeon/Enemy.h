#pragma once

#include "Character.h"

class Enemy : public Character {
public:
    Enemy(int _x, int _y, int _maxHP, int _STR, int _VIT);
    void randomMove();
    void setState(STATE state);

    std::vector<glm::vec2> toPlayer;
    int elapsedTurn;

private:

};

Enemy::Enemy(int _x, int _y, int _maxHP, int _STR, int _VIT)
: Character(_x, _y, _maxHP, _STR, _VIT, SEARCH, DOWN, ENEMY), elapsedTurn(15)
{
}

void Enemy::randomMove() {
    switch (rand() % 4)
    {
        case 0: move(LEFT); break;
        case 1: move(RIGHT);break;
        case 2: move(UP);   break;
        case 3: move(DOWN); break;
    }
}

void Enemy::setState(STATE state) {
    mState = state;
}
