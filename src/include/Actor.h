#pragma once

#include <vector>
#include <cstdlib>

struct vec2{
    vec2() :
		x(-1),
		y(-1)
	{
	}

	vec2(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

    int x;
    int y;
};

class Actor {
public:
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

    Actor(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, char _icon);
    void move(DIRECTION dir);
    void moveTo(vec2 v);
    void back();
    void attack(class Actor* enemy);
    void receiveDamage(int damage);

    void setPos(int _x, int _y);
    void setDir(DIRECTION _dir);

    int getX() {return x;}
    int getY() {return y;}
    STATE getState() {return mState;}
    DIRECTION getDir() {return mDir;}
    char getIcon() {return icon;}

protected:
    STATE mState;
    DIRECTION mDir;
    int x, y;
    int maxHP, STR, VIT;
    int nowHP;

    const char icon;
};

Actor::Actor(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, char _icon)
:x(_x), y(_y), maxHP(_maxHP), nowHP(_maxHP), STR(_STR), VIT(_VIT), mState(_state), mDir(_dir), icon(_icon)
{
}

void Actor::move(DIRECTION dir) {
    switch (dir) {
        case LEFT:  x--; break;
        case RIGHT: x++; break;
        case UP:    y--; break;
        case DOWN:  y++; break;
    }
    mDir = dir;
}

void Actor::moveTo(vec2 v) {
    if(v.x > x) move(RIGHT);
    else if(v.x < x) move(LEFT);
    else if(v.y > y) move(DOWN);
    else move(UP);
}

void Actor::back() {
    switch (mDir) {
        case LEFT:  x++; break;
        case RIGHT: x--; break;
        case UP:    y++; break;
        case DOWN:  y--; break;
    }
}

void Actor::attack(Actor* opponent) {
    opponent->receiveDamage(STR);
}

void Actor::receiveDamage(int damage) {
    nowHP -= abs(damage - VIT);
    if(nowHP <= 0) {
        mState = DEAD;
    }
}

void Actor::setPos(int _x, int _y) {
    x = _x;
    y = _y;
}

void Actor::setDir(DIRECTION _dir) {
    mDir = _dir;
}