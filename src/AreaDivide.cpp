#include "GenerateDungeon/AreaDivide.h"

extern std::mt19937 random_engine;
extern std::uniform_int_distribution<int> random_num;

void AreaDivide::divide(int ID)
{
    if(areas.size() > AREA_MAX) {
        return;
    }
// エリア分割できるかの確認用変数
    int w = areas[ID].w;
    int h = areas[ID].h;
    Area newArea;
    int newID = areaCount + 1;
// エリアの長辺で分割
    if(areas[ID].w > areas[ID].h) {
        w /= (random_num(random_engine)%3) + 2;
        newArea = Area(
            areas[ID].x + w,
            areas[ID].y,
            areas[ID].w - w,
            areas[ID].h
        );
    } else {
        h /= (random_num(random_engine)%3) + 2;
        newArea = Area(
            areas[ID].x,
            areas[ID].y + h,
            areas[ID].w,
            areas[ID].h - h
        );
    }
// エリアの最小サイズに満たない場合はキャンセル
    if(w < AREA_SIZE_MIN || h < AREA_SIZE_MIN
        || newArea.w < AREA_SIZE_MIN || newArea.h < AREA_SIZE_MIN) {
        return;
    }
    areaCount++;
    areas[ID].w = w;
    areas[ID].h = h;
    areas.push_back(newArea);
    if((areas[ID].w > 20) || (areas[ID].h > 20) || (random_num(random_engine)%2))
        divide(ID);
    if((areas[newID].w > 20) || (areas[newID].h > 20) || !(random_num(random_engine)%2))
        divide(newID);
}

void AreaDivide::generate()
{
    printf("生成開始");
    initFloor();

    areas = std::vector<Area>(1, Area(0, 0, FLOOR_W, FLOOR_H));
    areaCount = 0;

    printf("エリア分割");
    divide(areaCount);
    if(areaCount < 5) divide(areaCount);

    printf("部屋生成");
// 各エリアに対する処理
    for(auto area : areas) {
        Room room = Room(
            area.x + ROOM_MARGIN,
            area.y + ROOM_MARGIN,
            area.w - 2*ROOM_MARGIN,
            area.h - 2*ROOM_MARGIN
        );
        rooms.push_back(room);

printf("通路書き込み\n");
        int aisle_num = random_num(random_engine);
        for(int y=area.y; y<area.y + area.h; y++) {
            floorTYPE[y][room.x + room.w + 1] = AISLE;
            floorTYPE[y][room.x + (aisle_num % (room.w - 2)) + 1] = AISLE;
        }
        for(int x=area.x; x<area.x + area.w; x++) {
            floorTYPE[room.y + room.h + 1][x] = AISLE;
            floorTYPE[room.y + (aisle_num % (room.h - 2)) + 1][x] = AISLE;
        }

printf("部屋書き込み\n");
        for(int y=room.y; y<room.y + room.h; y++) {
            for(int x=room.x; x<room.x + room.w; x++) {
                floorTYPE[y][x] = FLOOR;
            }
        }
    }

printf("地形整形\n");

    outputMap_forDebug();

    fillSurround();
    randomEraseDeadEnd();
    identificationWallKind();

printf("階段生成\n");
    int roomNum = random_num(random_engine) % areaCount;
    Room room = rooms[roomNum];
    Ivec2 pos = Ivec2(0, 0);
    while(floorTYPE[pos.y][pos.x] != FLOOR)
    {
        pos.x = room.x + random_num(random_engine)%room.w;
        pos.y = room.y + random_num(random_engine)%room.h;
    }
    floorTYPE[pos.y][pos.x] = STEP;
printf("階段(%d, %d)\n", pos.x, pos.y);

printf("dungeon.mapに書き出し\n");
    outputMap();

printf("生成終了\n");
}
