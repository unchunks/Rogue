#pragma once

#include "Actor.h"

class Player : public Actor {
public:
    Player(int _x, int _y, int _maxHP, int _STR, int _VIT);
    void setState(STATE state);
};

Player::Player(int _x, int _y, int _maxHP, int _STR, int _VIT)
: Actor(_x, _y, _maxHP, _STR, _VIT, ALIVE, DOWN, '@')
{
}

void Player::setState(STATE state) {
    mState = state;
}
