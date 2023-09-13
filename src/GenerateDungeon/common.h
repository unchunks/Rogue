#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "AStar.h"

using namespace std;


/* 以下このファイル内から呼び出す関数 */

// bool isEnemyPos(AI e, int x, int y) {
//     if((e.getX() == x) && (e.getY() == y)) return true;
//     return false;
// }

// bool isEnemiesPos(int x, int y) {
//     for(AI e : enemies) {
//         if((e.getX() == x) && (e.getY() == y)) return true;
//     }
//     return false;
// }

// bool isFound(AI e) {
//     if(abs(e.getX() - player.getX()) <= ENEMY_SEARCH_RANGE 
//         && abs(e.getY() - player.getY() <= ENEMY_SEARCH_RANGE))
//         return true;
//     return false;
// }

// bool canAttack(int x, int y) {
//     if(isPlayerPos(x+1, y)
//     || isPlayerPos(x-1, y)
//     || isPlayerPos(x, y+1)
//     || isPlayerPos(x, y-1)) {
//         return true;
//     }
//     if(isEnemiesPos(x+1, y)
//     || isEnemiesPos(x-1, y)
//     || isEnemiesPos(x, y+1)
//     || isEnemiesPos(x, y-1)) {
//         return true;
//     }
//     return false;
// }

/* 以下このファイル外から呼び出す関数 */

// void arrangementEnemy(int roomCount) {
//     for(AI &e : enemies) {
//         int roomNum = rand() % roomCount;
//         e.setPos(
//             areas[roomNum].room.x + rand()%areas[roomNum].room.w, 
//             areas[roomNum].room.y + rand()%areas[roomNum].room.h
//         );
//
//         while(!canGetOn(e.getX(), e.getY(), ENEMY)) {
//             e.setPos(
//                 areas[roomNum].room.x + rand()%areas[roomNum].room.w, 
//                 areas[roomNum].room.y + rand()%areas[roomNum].room.h
//             );
//         }
//
//         e.setDir(static_cast<Actor::DIRECTION> (rand() % 4));
//     }
// }

void playerMove(char input) {
    switch(input) {
        case 'w': player.move(Actor::UP);   break;
        case 'a': player.move(Actor::LEFT); break;
        case 's': player.move(Actor::DOWN); break;
        case 'd': player.move(Actor::RIGHT);break;
    }
    if(!canGetOn(player.getX(), player.getY(), PLAYER)) {
        player.back();
    }
}

// void enemyMove() {
//     for(AI &e : enemies) {
//         switch(e.getState()) {
//             case Actor::SEARCH:
//                 // e.setDir(static_cast<Actor::DIRECTION> (rand() % 4));
//                 e.move(e.getDir());
//                 if(isFound(e)) {
//                     e.setState(Actor::FOUND);
//                 }
//                 while(true) {
//                     if(canGetOn(e.getX(), e.getY(), ENEMY))
//                         break;
//                     else {
//                         e.back();
//                         e.setDir(static_cast<Actor::DIRECTION> (rand() % 4));
//                     }
//                 }
//                 break;
//
//             case Actor::FOUND:
//                 // プレイヤーへの経路探索
//                 if(e.elapsedTurn > 5) {
//                     CreateBuffMap();
//                     vec2 start = vec2(e.getX(), e.getY());
//                     vec2 goal = vec2(player.getX(), player.getY());
//                     e.toPlayer = AStar(start, goal);
//                     e.elapsedTurn = 0;
//                 }
//                 e.moveTo(e.toPlayer[e.elapsedTurn]);
//                 e.elapsedTurn++;
//                 if(canAttack(player.getX(), player.getY())) {
//                     e.setState(Actor::ATTACK);
//                 }
//                 if(!isFound(e)) {
//                     e.setState(Actor::SEARCH);
//                 }
//                 break;
//
//             case Actor::ATTACK:
//                 e.attack(&player);
//                 break;
//
//             case Actor::ESCAPE:
//                 break;
//         }
//     }
// }
