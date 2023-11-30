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
    void walkTo(glm::vec2 _destination, std::vector<class Tile> _tiles, class Character _player, std::vector<class Enemy> _otherEnemies);
    
    /// @brief 目的地を設定し、ルートを検索
    /// @param dungeon データ系のマップ情報
    /// @param _goal データ系の目的地の座標
    void setGoal(CELL_TYPE dungeon[FLOOR_H][FLOOR_W], glm::vec2 _goal);

    void attack(class Character& _opponent) override;
    bool find(class Character _opponent);
    bool mustUpdateRoute();
    bool changeState(class Character _opponent);

    void routeClear() {route.clear(); elapsedTurn = ENEMY_SEARCH_INTERVAL + 1;}

    int getRouteSize() {return static_cast<int>(route.size());}
    int getElapsedTurn() {return elapsedTurn;}
    ENEMY_TYPE getEnemyType() {return enemy_type;}

private:
    std::deque<glm::vec2> route;
    glm::vec2 goal;
    glm::vec2 nextPos;
    int elapsedTurn;
    ENEMY_TYPE enemy_type;
};
