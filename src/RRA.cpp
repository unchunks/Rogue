#include "GenerateDungeon/RRA.h"

std::random_device mSeed;
std::mt19937 mEngine(mSeed());
std::uniform_int_distribution<int> mRandomNum(AREA_MAX, 100);

void RRA::generate()
{
    int total_area = 0;
    Area area;
    for (int i = 0; i < AREA_MAX; i++)
    {
        if (total_area > FLOOR_W * FLOOR_H / 2)
            break;
        int ax = mRandomNum(mEngine) % FLOOR_W;
        int ay = mRandomNum(mEngine) % FLOOR_H;
        int aw = mRandomNum(mEngine) % AREA_SIZE_MIN + AREA_SIZE_MIN;
        int ah = mRandomNum(mEngine) % AREA_SIZE_MIN + AREA_SIZE_MIN;
        area = Area(ax, ay, aw, ah);
        if (area.x + area.w >= FLOOR_W)
        {
            area.x = FLOOR_W - area.w - 1;
        }
        if (area.y + area.h >= FLOOR_H)
        {
            area.y = FLOOR_H - area.h - 1;
        }
        if (i == 0)
        {
            areas[0] = area;
            rooms[0] = Room(area);
        }
        else
        {
            areas.push_back(area);
            rooms.push_back(Room(area));
        }
        total_area += area.w * area.h;
    }

    for (auto room_itr = rooms.begin() + 1; room_itr < rooms.end(); room_itr++)
    {
        int aisle_x1 = (room_itr - 1)->x + (mRandomNum(mEngine) % (room_itr - 1)->w);
        int aisle_y1 = (room_itr - 1)->y + (mRandomNum(mEngine) % (room_itr - 1)->h);
        int aisle_x2 = room_itr->x + (mRandomNum(mEngine) % room_itr->w);
        int aisle_y2 = room_itr->y + (mRandomNum(mEngine) % room_itr->h);
        int middle_x = (aisle_x1 + aisle_x2) / 2;

        int dx = 1;
        if (aisle_x1 > aisle_x2)
            dx = -1;
        for (int x = aisle_x1; x != middle_x; x += dx)
        {
            floorTYPE[aisle_y1][x] = AISLE;
        }
        for (int x = middle_x; x != aisle_x2; x += dx)
        {
            floorTYPE[aisle_y2][x] = AISLE;
        }
        if (aisle_y1 > aisle_y2)
        {
            int work = aisle_y1;
            aisle_y1 = aisle_y2;
            aisle_y2 = work;
        }
        for (int y = aisle_y1; y <= aisle_y2; y++)
        {
            floorTYPE[y][middle_x] = AISLE;
        }
    }

    fillSurround();
    randomEraseDeadEnd();
    identificationWallKind();

    int roomNum = mRandomNum(mEngine) % areas.size();
    Room room = rooms[roomNum];
    glm::vec2 pos;
    while(floorTYPE[(int)pos.y][(int)pos.x] != FLOOR)
    {
        pos.x = room.x + mRandomNum(mEngine)%room.w;
        pos.y = room.y + mRandomNum(mEngine)%room.h;
    }
    floorTYPE[(int)pos.y][(int)pos.x] = STEP;
    std::cout << "階段(" << pos.x << ", " << pos.y << ")\n";
}
