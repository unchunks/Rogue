#include "GenerateDungeon/Player.h"

Player::Player(int _x, int _y, int _maxHP, int _STR, int _VIT)
: Character(_x, _y, _maxHP, _STR, _VIT, ALIVE, DOWN, PLAYER, "　プレイヤー　"), level(1), exp(0)
{
}

bool Player::levelUp(int _exp)
{
    exp += _exp;
    if(exp >= level * PLAYER_EXP)
    {
        exp = 0;
        level++;
        maxHP += 10;
        nowHP += 10;
        STR += 2;
        VIT += 2;
        return true;
    }
    return false;
}

void Player::reset()
{
    nowHP = maxHP;
    mState = ALIVE;
    mDir = DOWN;
    level = 1;
    exp = 0;
}