#pragma once

class Area
{
public:
    Area(){};
    Area(int _x, int _y, int _w, int _h)
        :x(_x), y(_y), w(_w), h(_h){};
    int x, y, w, h;

    void operator = (Area area)
    {
        x = area.x;
        y = area.y;
        w = area.w;
        h = area.h;
    }
};
