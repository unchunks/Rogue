#include "GenerateDungeon/Player.h"

Player::Player(int _x, int _y, int _maxHP, int _STR, int _VIT)
: Character(_x, _y, _maxHP, _STR, _VIT, ALIVE, DOWN, PLAYER, "　プレイヤー　")
{
}

void Player::reset()
{
    nowHP = maxHP;
    mState = ALIVE;
    mDir = DOWN;
}