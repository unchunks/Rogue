#include "GenerateDungeon/RRA.h"

extern std::mt19937 random_engine;
extern std::uniform_int_distribution<int> random_num;

void RRA::generate()
{
    // SDL_Log("生成開始\n");
    initFloor();

    // SDL_Log("部屋生成\n");
    int total_area = 0;
    Area area;
    for (int i = 0; i < AREA_MAX; i++)
    {
        if (total_area > FLOOR_W * FLOOR_H / 2)
            break;
        int ax = random_num(random_engine) % FLOOR_W;
        int ay = random_num(random_engine) % FLOOR_H;
        int aw = random_num(random_engine) % AREA_SIZE_MIN + AREA_SIZE_MIN;
        int ah = random_num(random_engine) % AREA_SIZE_MIN + AREA_SIZE_MIN;
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

    Room centerRoom = rooms[0];
    for (auto room : rooms)
    {
        // SDL_Log("room(X: %d, Y: %d, W: %d, H: %d)\n", room.x, room.y, room.w, room.h);

        // SDL_Log("通路書き込み\n");
        int aisle_x1 = centerRoom.x + (random_num(random_engine) % centerRoom.w);
        int aisle_y1 = centerRoom.y + (random_num(random_engine) % centerRoom.h);
        int aisle_x2 = room.x + (random_num(random_engine) % room.w);
        int aisle_y2 = room.y + (random_num(random_engine) % room.h);
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

        // SDL_Log("部屋書き込み\n");
        for(int y = room.y; y < (room.y + room.h); y++) {
            for(int x = room.x; x < (room.x + room.w); x++) {
                floorTYPE[y][x] = FLOOR;
            }
        }
    }

    // SDL_Log("地形整形\n");

    outputMap_forDebug();
    
    fillSurround();
    randomEraseDeadEnd();
    identificationWallKind();

    outputMap();

    // SDL_Log("階段生成\n");
    int roomNum = random_num(random_engine) % areas.size();
    Room room = rooms[roomNum];
    Ivec2 data_pos = Ivec2(0, 0);
    while(floorTYPE[data_pos.y][data_pos.x] != FLOOR)
    {
        data_pos.x = room.x + random_num(random_engine)%room.w;
        data_pos.y = room.y + random_num(random_engine)%room.h;
    // SDL_Log("data_pos(X: %d, Y: %d), room(X: %d, Y: %d, W: %d, H: %d)\n", data_pos.x, data_pos.y, room.x, room.y, room.w, room.h);
    }
    floorTYPE[data_pos.y][data_pos.x] = STEP;
    // SDL_Log("階段(%d, %d)\n", data_pos.x, data_pos.y);

    // SDL_Log("dungeon.mapに書き出し\n");
    outputMap();

    // SDL_Log("生成終了\n");
}
