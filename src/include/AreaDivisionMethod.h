#pragma once
// 区域分割法

#include "DMethod.h"

int areaCount = 0;
int randomNumber;

void devidArea(int areaID) {
    if(areaCount > AREA_MAX) {
        return;
    }

    int newAreaID = areaCount;
// エリア分割が失敗したと用に値を退避
    int w = areas[areaID].w;
    int h = areas[areaID].h;
// エリアの長辺で分割
    if(areas[areaID].w > areas[areaID].h) {
        areas[areaID].w /= 2;
        areas[newAreaID] = Area(
            areas[areaID].x + areas[areaID].w, 
            areas[areaID].y, 
            w - areas[areaID].w, 
            areas[areaID].h
        );
    } else {
        areas[areaID].h /= 2;
        areas[newAreaID] = Area(
            areas[areaID].x, 
            areas[areaID].y + areas[areaID].h, 
            areas[areaID].w, 
            h - areas[areaID].h
        );
    }
// エリアの最小サイズに満たない場合はキャンセル
    if(areas[areaID].w < AREA_SIZE_MIN || areas[areaID].h < AREA_SIZE_MIN
        || areas[newAreaID].w < AREA_SIZE_MIN || areas[newAreaID].h < AREA_SIZE_MIN) {
            areas[areaID].w = w;
            areas[areaID].h = h;
            return;
    }

    areaCount++;
    devidArea(randomNumber % areaCount);
}

void AreaDivision() {
    devidArea(0);

// 各エリアに対する処理
    for(int i=0; i<areaCount; i++) {
        randomNumber = rand();
// 部屋の領域作成
        areas[i].room = Room(
            areas[i].x + ROOM_MARGIN, 
            areas[i].y + ROOM_MARGIN, 
            areas[i].w - (ROOM_MARGIN * 2), 
            areas[i].h - (ROOM_MARGIN * 2)
        );
// 通路作成
        for(int y=areas[i].y; y<areas[i].y + areas[i].h; y++) {
            floorTYPE[y][areas[i].room.x + areas[i].room.w + 1] = AISLE;
            floorTYPE[y][areas[i].room.x + (randomNumber % (areas[i].room.w - 2)) + 1] = AISLE;
        }
        for(int x=areas[i].x; x<areas[i].x + areas[i].w; x++) {
            floorTYPE[areas[i].room.y + areas[i].room.h + 1][x] = AISLE;
            floorTYPE[areas[i].room.y + (randomNumber % (areas[i].room.h - 2)) + 1][x] = AISLE;
        }
// 部屋作成
        for(int y=areas[i].room.y; y<areas[i].room.y + areas[i].room.h; y++) {
            for(int x=areas[i].room.x; x<areas[i].room.x + areas[i].room.w; x++) {
                floorTYPE[y][x] = FLOOR;
            }
        }
    }
}
