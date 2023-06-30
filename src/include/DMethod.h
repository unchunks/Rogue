#pragma once

#include <iostream>

#include "DDefine.h"
#include "AStar.h"

/* 以下このファイル内から呼び出す関数 */

bool isPlayerPos(int x, int y) {
    if((x == player.getX()) && (y == player.getY())) return true;
    return false;
}

bool isEnemyPos(AI e, int x, int y) {
    if((e.getX() == x) && (e.getY() == y)) return true;
    return false;
}

bool isEnemiesPos(int x, int y) {
    for(AI e : enemies) {
        if((e.getX() == x) && (e.getY() == y)) return true;
    }
    return false;
}

bool isFound(AI e) {
    if(abs(e.getX() - player.getX()) <= ENEMY_SEARCH_RANGE 
        && abs(e.getY() - player.getY() <= ENEMY_SEARCH_RANGE))
        return true;
    return false;
}

bool canGetOn(int x, int y, ACTOR_TYPE type) {
    if((floorTYPE[y][x] != FLOOR) && (floorTYPE[y][x] != AISLE)) return false;
    if((type != PLAYER) && (isPlayerPos(x, y))) return false;
    int same = 0;
    for(AI e : enemies) {
        if(isEnemyPos(e, x, y)) same++;
    }
    if(same > 1) return false;
    return true;
}

bool canAttack(int x, int y) {
    if(isPlayerPos(x+1, y)
    || isPlayerPos(x-1, y)
    || isPlayerPos(x, y+1)
    || isPlayerPos(x, y-1)) {
        return true;
    }
    if(isEnemiesPos(x+1, y)
    || isEnemiesPos(x-1, y)
    || isEnemiesPos(x, y+1)
    || isEnemiesPos(x, y-1)) {
        return true;
    }
    return false;
}

char posToEnemyicon(int x, int y) {
    for(AI e : enemies) {
        if(e.getX() == x && e.getY() == y) {
            return e.getIcon();
        }
    }
    return 'E';
}

/* 以下このファイル外から呼び出す関数 */

// フロアの初期化
void initFloor() {
    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            floorTYPE[y][x] = WALL_ALL;
        }
    }
}

// 外周を壁で埋める
void fillSurround() {
    for(int y=0; y<FLOOR_H; y++) {
        floorTYPE[y][0] = floorTYPE[y][FLOOR_W-1] = WALL_ALL;
    }
    for(int x=0; x<FLOOR_W; x++) {
        floorTYPE[0][x] = floorTYPE[FLOOR_H-1][x] = WALL_ALL;
    }
}

// 行き止まりをなくす
void eraseDeadEnd() {
    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            if(floorTYPE[y][x] == WALL_ALL) continue;
            int n = 0;
            if(floorTYPE[y+1][x] == WALL_ALL) n++;
            if(floorTYPE[y-1][x] == WALL_ALL) n++;
            if(floorTYPE[y][x+1] == WALL_ALL) n++;
            if(floorTYPE[y][x-1] == WALL_ALL) n++;
            if(n >= 3) {
                floorTYPE[y][x] = WALL_ALL;
            }
        }
    }
    for(int y=FLOOR_H-1; y>=0; y--) {
        for(int x=FLOOR_W-1; x>=0; x--) {
            if(floorTYPE[y][x] == WALL_ALL) continue;
            int n = 0;
            if(floorTYPE[y+1][x] == WALL_ALL) n++;
            if(floorTYPE[y-1][x] == WALL_ALL) n++;
            if(floorTYPE[y][x+1] == WALL_ALL) n++;
            if(floorTYPE[y][x-1] == WALL_ALL) n++;
            if(n >= 3) {
                floorTYPE[y][x] = WALL_ALL;
            }
        }
    }
}

// 行き止まりをランダムでなくす
void randomEraseDeadEnd() {
    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            if(floorTYPE[y][x] == WALL_ALL) continue;
            int n = 0;
            if(floorTYPE[y+1][x] == WALL_ALL) n++;
            if(floorTYPE[y-1][x] == WALL_ALL) n++;
            if(floorTYPE[y][x+1] == WALL_ALL) n++;
            if(floorTYPE[y][x-1] == WALL_ALL) n++;
            if((n >= 3) && (rand()%5)) {
                floorTYPE[y][x] = WALL_ALL;
            }
        }
    }
    for(int y=FLOOR_H-1; y>=0; y--) {
        for(int x=FLOOR_W-1; x>=0; x--) {
            if(floorTYPE[y][x] == WALL_ALL) continue;
            int n = 0;
            if(floorTYPE[y+1][x] == WALL_ALL) n++;
            if(floorTYPE[y-1][x] == WALL_ALL) n++;
            if(floorTYPE[y][x+1] == WALL_ALL) n++;
            if(floorTYPE[y][x-1] == WALL_ALL) n++;
            if((n >= 3) && (rand()%5)) {
                floorTYPE[y][x] = WALL_ALL;
            }
        }
    }
}

// 壁の種類を特定
void identificationWallKind() {
    for(int y=0; y<FLOOR_H + 2; y++) {
        buff[y][0] = buff[y][FLOOR_W + 1] = WALL_ALL;
    }
    for(int x=0; x<FLOOR_W + 2; x++) {
        buff[0][x] = buff[FLOOR_H + 1][x] = WALL_ALL;
    }
    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            buff[y + 1][x + 1] = floorTYPE[y][x];
        }
    }

    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            if(buff[y + 1][x + 1] != WALL_ALL) continue;
            int connect[4] = {0, 0, 0, 0}; // 0:床と接している、1:壁と接している
            int connectWallNum = 0;
            if(buff[y + 2][x + 1] == WALL_ALL) { // 下
                connect[0] = 1;
                connectWallNum++;
            }
            if(buff[y][x + 1] == WALL_ALL) { // 上
                connect[1] = 1;
                connectWallNum++;
            }
            if(buff[y + 1][x + 2] == WALL_ALL) { // 右
                connect[2] = 1;
                connectWallNum++;
            }
            if(buff[y + 1][x] == WALL_ALL) { // 左
                connect[3] = 1;
                connectWallNum++;
            }

            if(connectWallNum == 4) continue;
            else if(connectWallNum == 3) {
                if(connect[0] == 0) floorTYPE[y][x] = WALL_BOTTOM;
                else if(connect[1] == 0) floorTYPE[y][x] = WALL_TOP;
                else if(connect[2] == 0) floorTYPE[y][x] = WALL_RIGHT;
                else if(connect[3] == 0) floorTYPE[y][x] = WALL_LEFT;
            } else if(connectWallNum == 2) {
                if((connect[0] == 0) && connect[1] == 0) floorTYPE[y][x] = WALL_SIDE_TB;
                else if((connect[0] == 0) && connect[2] == 0) floorTYPE[y][x] = WALL_RIGHT_BOTTOM;
                else if((connect[0] == 0) && connect[3] == 0) floorTYPE[y][x] = WALL_LEFT_BOTTOM;
                else if((connect[1] == 0) && connect[2] == 0) floorTYPE[y][x] = WALL_RIGHT_TOP;
                else if((connect[1] == 0) && connect[3] == 0) floorTYPE[y][x] = WALL_LEFT_TOP;
                else if((connect[2] == 0) && connect[3] == 0) floorTYPE[y][x] = WALL_SIDE_LR;
            } else if(connectWallNum == 1) {
                if(connect[0] == 1) floorTYPE[y][x] = WALL_END_BOTTOM;
                else if(connect[1] == 1) floorTYPE[y][x] = WALL_END_TOP;
                else if(connect[2] == 1) floorTYPE[y][x] = WALL_END_RIGHT;
                else if(connect[3] == 1) floorTYPE[y][x] = WALL_END_LEFT;
            } else {
                floorTYPE[y][x] = PILLAR;
            }
        }
    }
}

void arrangementPlayer(int roomCount) {
    int roomNum = rand() % roomCount;
    int px = areas[roomNum].room.x + rand()%areas[roomNum].room.w;
    int py = areas[roomNum].room.y + rand()%areas[roomNum].room.h;

    while(!canGetOn(px, py, PLAYER)) {
        px = areas[roomNum].room.x + rand()%areas[roomNum].room.w;
        py = areas[roomNum].room.y + rand()%areas[roomNum].room.h;
    }

    player.setPos(px, py);
}

void arrangementEnemy(int roomCount) {
    for(auto e = enemies.begin(); e != enemies.end(); ++e) {
        // enemies.insert(e, Slime(0, 0));
        // enemies.erase(e+1);
        int roomNum = rand() % roomCount;
        e->setPos(
            areas[roomNum].room.x + rand()%areas[roomNum].room.w, 
            areas[roomNum].room.y + rand()%areas[roomNum].room.h
        );

        while(!canGetOn(e->getX(), e->getY(), ENEMY)) {
            e->setPos(
                areas[roomNum].room.x + rand()%areas[roomNum].room.w, 
                areas[roomNum].room.y + rand()%areas[roomNum].room.h
            );
        }

        e->setDir(static_cast<Actor::DIRECTION> (rand() % 4));
    }
}

#include <iomanip>

void output() {
    std::cout << "  ";
    for(int x=0; x<FLOOR_W; x++)
        std::cout << std::setw(2) << x;
    std::cout << "\n";
    for(int y=0; y<FLOOR_H; y++) {
        std::cout << std::setw(2) << y;
        for(int x=0; x<FLOOR_W; x++) {
            if(isPlayerPos(x, y)) {
                std::cout << player.getIcon() << " ";
                continue;
            }
            if(isEnemiesPos(x, y)) {
                std::cout << posToEnemyicon(x, y) << " ";
                continue;
            }
            switch(floorTYPE[y][x]) {
                case NONE:              std::cout << "~ "; break;
                case PILLAR:            std::cout << "・"; break;
                case WALL_LEFT:         std::cout << "| "; break;
                case WALL_RIGHT:        std::cout << " |"; break;
                case WALL_TOP:          std::cout << "￣"; break;
                case WALL_BOTTOM:       std::cout << "＿"; break;
                case WALL_LEFT_TOP:     std::cout << "┏ "; break;
                case WALL_LEFT_BOTTOM:  std::cout << "┗ "; break;
                case WALL_RIGHT_TOP:    std::cout << " ┓"; break;
                case WALL_RIGHT_BOTTOM: std::cout << " ┛"; break;
                case WALL_SIDE_LR:      std::cout << "||"; break;
                case WALL_SIDE_TB:      std::cout << "二"; break;
                case WALL_END_LEFT:     std::cout << "＞"; break;
                case WALL_END_RIGHT:    std::cout << "＜"; break;
                case WALL_END_TOP:      std::cout << "Ｖ"; break;
                case WALL_END_BOTTOM:   std::cout << "Ａ"; break;
                case WALL_ALL:          std::cout << "■ "; break;
                case FLOOR: case AISLE: std::cout << "  "; break;
                case STEP:              std::cout << "S "; break;
            }
        }
        std::cout << "\n";
    }
}

void outputInWindows() {
    for(int x=0; x<FLOOR_W; x++)
        std::cout << std::setw(2) << x;
    std::cout << "\n";
    for(int y=0; y<FLOOR_H; y++) {
        std::cout << std::setw(2) << y;
        for(int x=0; x<FLOOR_W; x++) {
            if(isPlayerPos(x, y)) {
                std::cout << player.getIcon() << " ";
                continue;
            }
            if(isEnemiesPos(x, y)) {
                std::cout << posToEnemyicon(x, y) << " ";
                continue;
            }
            switch(floorTYPE[y][x]) {
                case NONE:              std::cout << "~ "; break;
                case PILLAR:            std::cout << "・"; break;
                case WALL_LEFT:         std::cout << "｜"; break;
                case WALL_RIGHT:        std::cout << "｜"; break;
                case WALL_TOP:          std::cout << "￣"; break;
                case WALL_BOTTOM:       std::cout << "＿"; break;
                case WALL_LEFT_TOP:     std::cout << "⎾ "; break;
                case WALL_LEFT_BOTTOM:  std::cout << "⎿ "; break;
                case WALL_RIGHT_TOP:    std::cout << "⏋ "; break;
                case WALL_RIGHT_BOTTOM: std::cout << "⏌ "; break;
                case WALL_SIDE_LR:      std::cout << "||"; break;
                case WALL_SIDE_TB:      std::cout << "二"; break;
                case WALL_END_LEFT:     std::cout << "］"; break;
                case WALL_END_RIGHT:    std::cout << "［"; break;
                case WALL_END_TOP:      std::cout << "Ｖ"; break;
                case WALL_END_BOTTOM:   std::cout << "Ａ"; break;
                case WALL_ALL:          std::cout << "■"; break;
                case FLOOR: case AISLE: std::cout << "  "; break;
                case STEP:              std::cout << "S "; break;
            }
        }
        std::cout << "\n";
    }
}

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

void enemyMove() {
    for(AI &e : enemies) {
        switch(e.getState()) {
            case Actor::SEARCH:
                // e.setDir(static_cast<Actor::DIRECTION> (rand() % 4));
                e.move(e.getDir());
                if(isFound(e)) {
                    e.setState(Actor::FOUND);
                }
                while(true) {
                    if(canGetOn(e.getX(), e.getY(), ENEMY))
                        break;
                    else {
                        e.back();
                        e.setDir(static_cast<Actor::DIRECTION> (rand() % 4));
                    }
                }
                break;

            case Actor::FOUND:
                // プレイヤーへの経路探索
                if(e.elapsedTurn > 5) {
                    CreateBuffMap();
                    vec2 start = vec2(e.getX(), e.getY());
                    vec2 goal = vec2(player.getX(), player.getY());
                    e.toPlayer = AStar(start, goal);
                    e.elapsedTurn = 0;
                }
                e.moveTo(e.toPlayer[e.elapsedTurn]);
                e.elapsedTurn++;
                if(canAttack(player.getX(), player.getY())) {
                    e.setState(Actor::ATTACK);
                }
                if(!isFound(e)) {
                    e.setState(Actor::SEARCH);
                }
                break;

            case Actor::ATTACK:
                e.attack(&player);
                break;

            case Actor::ESCAPE:
                break;
        }
    }
}
