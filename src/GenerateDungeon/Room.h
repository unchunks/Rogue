#pragma once

#include "Area.h"
#include "Const.h"

const int ROOM_MARGIN = 2;

class Room
{
public:
    Room()
    {
        x = y = 0;
        w = h = 0;
    };
    Room(int _x, int _y, int _w, int _h)
        :x(_x), y(_y), w(_w), h(_h){};
    Room(Area area)
    {
        x = area.x + ROOM_MARGIN;
        y = area.y + ROOM_MARGIN;
        w = area.w - 2*ROOM_MARGIN;
        h = area.h - 2*ROOM_MARGIN;
    }

    void operator = (Room room)
    {
        x = room.x;
        y = room.y;
        w = room.w;
        h = room.h;
    }

    int x, y, w, h;
};
