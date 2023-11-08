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
    Enemy(int _x, int _y, int _maxHP, int _STR, int _VIT);
    ~Enemy();
    void walk(std::vector<class Tile> _tiles, class Character _player, std::vector<class Enemy> _otherEnemies);
    void setGoal(CELL_TYPE dungeon[FLOOR_H][FLOOR_W], glm::vec2 _goal);

    void attack(class Character& _enemy) override;

    void routeClear() {route.clear();}

    int getRouteSize() {return route.size();}
    int getElapsedTurn() {return elapsedTurn;}
    ENEMY_TYPE getEnemyType() {return enemy_type;}

private:
    std::deque<glm::vec2> route;
    glm::vec2 goal;
    glm::vec2 nextPos;
    int elapsedTurn;
    ENEMY_TYPE enemy_type;
};
