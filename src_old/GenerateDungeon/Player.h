#pragma once

#include "Character.h"

class Player : public Character
{
public:
    Player(int _x, int _y, int _maxHP, int _STR, int _VIT);
};
