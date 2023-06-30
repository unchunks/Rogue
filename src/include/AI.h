#pragma once

#include "Actor.h"

class AI : public Actor {
public:
    AI(int _x, int _y, int _maxHP, int _STR, int _VIT, char _icon);
    void randomMove();
    void setState(STATE state);

    std::vector<vec2> toPlayer;
    int elapsedTurn;

private:

};

AI::AI(int _x, int _y, int _maxHP, int _STR, int _VIT, char _icon)
: Actor(_x, _y, _maxHP, _STR, _VIT, SEARCH, DOWN, _icon), elapsedTurn(15)
{
}

void AI::randomMove() {
    switch (rand() % 4)
    {
        case 0: move(LEFT); break;
        case 1: move(RIGHT);break;
        case 2: move(UP);   break;
        case 3: move(DOWN); break;
    }
}

void AI::setState(STATE state) {
    mState = state;
}
