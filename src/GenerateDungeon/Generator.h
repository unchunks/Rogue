#pragma once

#include <iomanip>
#include <vector>

#include "Area.h"
#include "Room.h"
#include "Player.h"
#include "Enemy.h"
#include "Enum.h"
#include "Const.h"

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
    CELL_TYPE (*getFloor())[FLOOR_W] { return floorTYPE; }

    int areaCount = 0;
    int randomNumber;

    CELL_TYPE buff[BUFF_FLOOR_H][BUFF_FLOOR_W];

protected:
    CELL_TYPE floorTYPE[FLOOR_H][FLOOR_W];
    std::vector<Area> areas = std::vector<Area>(1, Area(0, 0, FLOOR_W, FLOOR_H));
    std::vector<Room> rooms = std::vector<Room>(1, Room(ROOM_MARGIN, ROOM_MARGIN, FLOOR_W - 2*ROOM_MARGIN, FLOOR_H - 2*ROOM_MARGIN));
};