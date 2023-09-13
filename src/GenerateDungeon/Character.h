#pragma once

#include <vector>
#include <cstdlib>
#include <glm/glm.hpp>

enum STATE {
    // Player
    ALIVE,
    // AI
    SEARCH,
    FOUND,
    ATTACK,
    ESCAPE,
    // both
    DEAD
};
enum DIRECTION {
    LEFT,
    RIGHT,
    UP,
    DOWN
};
enum CHAR_TYPE {
    PLAYER,
    ENEMY
};

class Character
{
public:
    Character(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, CHAR_TYPE _type);

    void move(DIRECTION dir);
    void moveTo(glm::vec2 v);
    void back();
    void attack(class Character& enemy);
    void receiveDamage(int damage);

    void setPos(int _x, int _y);
    void setState(STATE _state);
    void setDir(DIRECTION _dir);

    int getX() {return x;}
    int getY() {return y;}
    STATE getState() {return mState;}
    DIRECTION getDir() {return mDir;}
    CHAR_TYPE getType() {return mType;}

protected:
    STATE mState;
    DIRECTION mDir;
    CHAR_TYPE mType;
    int x, y;
    int maxHP, STR, VIT;
    int nowHP;
};

Character::Character(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, CHAR_TYPE _type)
:x(_x), y(_y), maxHP(_maxHP), nowHP(_maxHP), STR(_STR), VIT(_VIT), mState(_state), mDir(_dir), mType(_type)
{
}

void Character::move(DIRECTION dir) {
    switch (dir) {
        case LEFT:  x--; break;
        case RIGHT: x++; break;
        case UP:    y--; break;
        case DOWN:  y++; break;
    }
    mDir = dir;
}

void Character::moveTo(glm::vec2 v) {
    if(v.x > x) move(RIGHT);
    else if(v.x < x) move(LEFT);
    else if(v.y > y) move(DOWN);
    else move(UP);
}

void Character::back() {
    switch (mDir) {
        case LEFT:  x++; break;
        case RIGHT: x--; break;
        case UP:    y++; break;
        case DOWN:  y--; break;
    }
}

void Character::attack(Character& opponent) {
    opponent.receiveDamage(STR);
}

void Character::receiveDamage(int damage) {
    nowHP -= abs(damage - VIT);
    if(nowHP <= 0) {
        mState = DEAD;
    }
}

void Character::setPos(int _x, int _y) {
    x = _x;
    y = _y;
}

void Character::setState(STATE _state) {
    mState = _state;
}


void Character::setDir(DIRECTION _dir) {
    mDir = _dir;
}

