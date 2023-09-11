#pragma once

#include <random> 

#include "Generator.h"

class AreaDivide : public Generator
{
public:
    void generate();
private:
    void divide(int ID);
};

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
        w /= (randomNumber%3) + 2;
        newArea = Area(
            areas[ID].x + w,
            areas[ID].y,
            areas[ID].w - w,
            areas[ID].h
        );
    } else {
        h /= (randomNumber%3) + 2;
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
        std::cout << "中断" << std::endl;
        return;
    }
    areaCount++;
    areas[ID].w = w;
    areas[ID].h = h;
    areas.push_back(newArea);
    std::cout << areaCount << "回目:\n";
    for(auto area : areas) {
        std::cout << "(" << area.x << ", " << area.y << ") " << area.w << " x " << area.h << std::endl;
    }
    if((areas[ID].w > 20) || (areas[ID].h > 20) || (randomNumber%2))
        divide(ID);
    if((areas[newID].w > 20) || (areas[newID].h > 20) || !(randomNumber%2))
        divide(newID);
}

void AreaDivide::generate()
{
    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<int> randNum(AREA_MAX, 100);
    randomNumber = randNum(engine);

    areas = std::vector<Area>(1, Area(0, 0, FLOOR_W, FLOOR_H));
    areaCount = 0;
    divide(areaCount);

// 各エリアに対する処理
    std::cout << "生成後" << std::endl;
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
        for(int y=area.y; y<area.y + area.h; y++) {
            floorTYPE[y][room.x + room.w + 1] = AISLE;
            floorTYPE[y][room.x + (randomNumber % (room.w - 2)) + 1] = AISLE;
        }
        for(int x=area.x; x<area.x + area.w; x++) {
            floorTYPE[room.y + room.h + 1][x] = AISLE;
            floorTYPE[room.y + (randomNumber % (room.h - 2)) + 1][x] = AISLE;
        }
// 部屋作成
        for(int y=room.y; y<room.y + room.h; y++) {
            for(int x=room.x; x<room.x + room.w; x++) {
                floorTYPE[y][x] = FLOOR;
            }
        }
        std::cout << "(" << area.x << ", " << area.y << "), " << area.w << ", " << area.h << " : ";
        std::cout << "(" << room.x << ", " << room.y << "), " << room.w << ", " << room.h;
        std::cout << std::endl;
    }
}