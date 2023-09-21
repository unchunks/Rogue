#ifndef __ROOM
#define __ROOM

const int ROOM_MARGIN = 2;

class Room 
{
public:
    Room(){};
    Room(int _x, int _y, int _w, int _h)
        :x(_x), y(_y), w(_w), h(_h){};
    int x, y, w, h;
};

#endif __ROOM