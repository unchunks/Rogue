#include "GenerateDungeon/AreaDivide.h"

std::random_device mSeed;
std::mt19937 mEngine(mSeed());
std::uniform_int_distribution<int> mRandomNum(AREA_MAX, 100);

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
        w /= (mRandomNum(mEngine)%3) + 2;
        newArea = Area(
            areas[ID].x + w,
            areas[ID].y,
            areas[ID].w - w,
            areas[ID].h
        );
    } else {
        h /= (mRandomNum(mEngine)%3) + 2;
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
    if((areas[ID].w > 20) || (areas[ID].h > 20) || (mRandomNum(mEngine)%2))
        divide(ID);
    if((areas[newID].w > 20) || (areas[newID].h > 20) || !(mRandomNum(mEngine)%2))
        divide(newID);
}

void AreaDivide::generate()
{
    areas = std::vector<Area>(1, Area(0, 0, FLOOR_W, FLOOR_H));
    areaCount = 0;
    divide(areaCount);
    if(areaCount < 5) divide(areaCount);
// std::cout << "1\n";
// 各エリアに対する処理
    for(auto area : areas) {
// 部屋生成
    Room room = Room(
        area.x + ROOM_MARGIN, 
        area.y + ROOM_MARGIN, 
        area.w - 2*ROOM_MARGIN, 
        area.h - 2*ROOM_MARGIN
    );
    rooms.push_back(room);
// 通路作成
// std::cout << "1_1\n";
        for(int y=area.y; y<area.y + area.h; y++) {
            floorTYPE[y][room.x + room.w + 1] = AISLE;
            floorTYPE[y][room.x + (mRandomNum(mEngine) % (room.w - 2)) + 1] = AISLE;
        }
// std::cout << "1_2\n";
        for(int x=area.x; x<area.x + area.w; x++) {
            floorTYPE[room.y + room.h + 1][x] = AISLE;
            floorTYPE[room.y + (mRandomNum(mEngine) % (room.h - 2)) + 1][x] = AISLE;
        }
// std::cout << "1_3\n";
// 部屋作成
        for(int y=room.y; y<room.y + room.h; y++) {
            for(int x=room.x; x<room.x + room.w; x++) {
                floorTYPE[y][x] = FLOOR;
            }
        }
    }

    fillSurround();
    randomEraseDeadEnd();
    identificationWallKind();

// std::cout << "2\n";
    int roomNum = mRandomNum(mEngine) % areaCount;
    Room room = rooms[roomNum];
    glm::vec2 pos;
    while(floorTYPE[(int)pos.y][(int)pos.x] != FLOOR)
    {
        pos.x = room.x + mRandomNum(mEngine)%room.w;
        pos.y = room.y + mRandomNum(mEngine)%room.h;
    }
// std::cout << "3\n";
    floorTYPE[(int)pos.y][(int)pos.x] = STEP;
    std::cout << "階段(" << pos.x << ", " << pos.y << ")\n";
}
