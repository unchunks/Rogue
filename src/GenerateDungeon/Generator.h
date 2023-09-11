#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

#include "Area.h"
#include "Room.h"
#include "Player.h"
#include "Enemy.h"

const int FLOOR_W = 40;
const int FLOOR_H = 40;
const int AREA_MAX = 10;
const int AREA_SIZE_MIN = 10;

enum CELL_TYPE {
    NONE,
//0辺床と接地（柱）
    PILLAR,
//1辺床と接地
    WALL_LEFT,
    WALL_RIGHT,
    WALL_TOP,
    WALL_BOTTOM,
//2辺床と接地
    WALL_LEFT_TOP,
    WALL_LEFT_BOTTOM,
    WALL_RIGHT_TOP,
    WALL_RIGHT_BOTTOM,
    WALL_SIDE_LR,
    WALL_SIDE_TB,
//3辺床と接地
    WALL_END_LEFT,
    WALL_END_RIGHT,
    WALL_END_TOP,
    WALL_END_BOTTOM,
//4辺床と接地
    WALL_ALL,
    FLOOR,
    AISLE,
    STEP
};

class Generator
{
public:
    void initFloor();
    void fillSurround();
    void eraseDeadEnd();
    void randomEraseDeadEnd();
    void identificationWallKind();
    void output(Player player, std::vector<Enemy> enemies);

    Area getArea(int id) { return areas[id]; }
    Room getRoom(int id) { return rooms[id]; }
    int getAreaNum() { return areas.size(); }
    int getRoomNum() { return rooms.size(); }

    int areaCount = 0;
    int randomNumber;

    CELL_TYPE buff[FLOOR_H + 2][FLOOR_W + 2];

protected:
    CELL_TYPE floorTYPE[FLOOR_H][FLOOR_W];
    std::vector<Area> areas = std::vector<Area>(1, Area(0, 0, FLOOR_W, FLOOR_H));
    std::vector<Room> rooms = std::vector<Room>(1, Room(ROOM_MARGIN, ROOM_MARGIN, FLOOR_W - 2*ROOM_MARGIN, FLOOR_H - 2*ROOM_MARGIN));
};

void Generator::initFloor() {
    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            floorTYPE[y][x] = WALL_ALL;
        }
    }
}

void Generator::fillSurround() {
    for(int y=0; y<FLOOR_H; y++) {
        floorTYPE[y][0] = floorTYPE[y][FLOOR_W-1] = WALL_ALL;
    }
    for(int x=0; x<FLOOR_W; x++) {
        floorTYPE[0][x] = floorTYPE[FLOOR_H-1][x] = WALL_ALL;
    }
}

void Generator::eraseDeadEnd() {
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

void Generator::randomEraseDeadEnd() {
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

void Generator::identificationWallKind() {
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

void Generator::output(Player player, std::vector<Enemy> enemies) {
    std::cout << "  ";
    for(int x=0; x<FLOOR_W; x++)
        std::cout << std::setw(2) << x;
    std::cout << "\n";
    for(int y=0; y<FLOOR_H; y++) {
        std::cout << std::setw(2) << y;
        for(int x=0; x<FLOOR_W; x++) {
            // SDLでは後ろから重ねて描画する
            if((player.getX() == x) && (player.getY() == y)) {
                std::cout << "@ ";
                continue;
            }
            // for(auto c : enemies) {
            //     if((c.getX() == x) && (c.getY() == y)) {
            //         std::cout << "E ";
            //         continue;
            //     }
            // }
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
        // if(isPlayerPos(x, y)) {
            //     std::cout << "@ ";
            //     continue;
            // }
            // if(isEnemiesPos(x, y)) {
            //     cout << "E ";
            //     continue;
            // }
        std::cout << "\n";
    }
}

