#pragma once

#include <iostream>
#include <deque>

#include "AStar.h"
#include "Character.h"
#include "Const.h"
#include "Enum.h"

class Enemy : public Character
{
public:
    Enemy(ENEMY_TYPE _enemy_type);
    Enemy(int _x, int _y, int _maxHP, int _STR, int _VIT, const char* _icon);
    void walk();
    void setGoal(CELL_TYPE dungeon[FLOOR_H][FLOOR_W], glm::vec2 _goal);

    void attack(class Character& _enemy) override;

    void routeClear() {route.clear();}

    int getRouteSize() {return route.size();}
    int getElapsedTurn() {return elapsedTurn;}
    const char* getIcon() {return icon;}

private:
    std::deque<glm::vec2> route;
    glm::vec2 goal;
    glm::vec2 nextPos;
    int elapsedTurn;
    const char* icon;
    ENEMY_TYPE enemy_type;
};
