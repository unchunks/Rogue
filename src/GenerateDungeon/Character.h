#ifndef __CHARACTER
#define __CHARACTER

#include <vector>
#include <cstdlib>
#include <glm/glm.hpp>

#include "Enum.h"

enum STATE {
    // Player
    ALIVE,
    // AI
    SEARCH,
    FOUND,
    ESCAPE,
    // both
    DEAD
};

class Character
{
public:
    Character();
    Character(int _x, int _y, int _maxHP, int _STR, int _VIT, STATE _state, DIRECTION _dir, CHAR_TYPE _type);

    void move(DIRECTION _dir);
    void moveTo(glm::vec2 _pos);
    void back();
    virtual void attack(class Character& _enemy);
    void receiveDamage(int _damage);

    void setPos(int _x, int _y);
    void setPos(glm::vec2 _pos);
    void setState(STATE _state);
    void setDir(DIRECTION _dir);

    int getNowHP() {return nowHP;}
    STATE getState() {return mState;}
    DIRECTION getDir() {return mDir;}
    CHAR_TYPE getType() {return mType;}
    glm::vec2 getPos() {return mPos;}

protected:
    int maxHP, STR, VIT;
    int nowHP;
    STATE mState;
    DIRECTION mDir;
    CHAR_TYPE mType;
    glm::vec2 mPos;
};

#endif __CHARACTER