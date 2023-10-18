#include "GenerateDungeon/RRA.h"

extern std::mt19937 random_engine;
extern std::uniform_int_distribution<int> random_num;

void RRA::generate()
{
SDL_Log("生成開始\n");
    initFloor();

SDL_Log("部屋生成\n");
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

SDL_Log("rooms[%d](X: %d, Y: %d, W: %d, H: %d)\n", i, rooms[i].x, rooms[i].y, rooms[i].w, rooms[i].h);
    }

    for (auto room_itr = rooms.begin() + 1; room_itr < rooms.end(); room_itr++)
    {
SDL_Log("通路書き込み\n");
        int aisle_x1 = (room_itr - 1)->x + (random_num(random_engine) % (room_itr - 1)->w);
        int aisle_y1 = (room_itr - 1)->y + (random_num(random_engine) % (room_itr - 1)->h);
        int aisle_x2 = room_itr->x + (random_num(random_engine) % room_itr->w);
        int aisle_y2 = room_itr->y + (random_num(random_engine) % room_itr->h);
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

SDL_Log("部屋書き込み\n");
        for(int y=room_itr->y; y<room_itr->y + room_itr->h; y++) {
            for(int x=room_itr->x; x<room_itr->x + room_itr->w; x++) {
                floorTYPE[y][x] = FLOOR;
            }
        }
    }

SDL_Log("地形整形\n");
    fillSurround();
    randomEraseDeadEnd();
    identificationWallKind();

    outputMap();

SDL_Log("階段生成\n");
    int roomNum = random_num(random_engine) % areas.size();
    Room room = rooms[roomNum];
    glm::vec2 pos = glm::vec2(0.0f, 0.0f);
    while(floorTYPE[(int)pos.y][(int)pos.x] != FLOOR)
    {
        pos.x = room.x + random_num(random_engine)%room.w;
        pos.y = room.y + random_num(random_engine)%room.h;
SDL_Log("pos(X: %.0f, Y: %.0f), room(X: %d, Y: %d, W: %d, H: %d)\n", pos.x, pos.y, room.x, room.y, room.w, room.h);
    }
    floorTYPE[(int)pos.y][(int)pos.x] = STEP;
SDL_Log("階段(%.0f, %.0f)\n", pos.x, pos.y);

SDL_Log("dungeon.mapに書き出し\n");
    outputMap();

SDL_Log("生成終了\n");
}
