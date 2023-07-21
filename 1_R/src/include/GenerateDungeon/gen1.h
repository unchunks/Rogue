#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

#include "Map.h"

std::vector<Area> areas(AREA_MAX, Area(0, 0, mapW, mapH));
int areaCount = 0;

void divideArea(int areaID) {
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
    divideArea(rand() % areaCount);
}

int gen1(CELL_TYPE mapBuff[mapH][mapW]) {
    divideArea(0);

// 各エリアに対する処理
    for(int i = 0; i < areaCount; i++) {
        int seed = rand();
// 部屋の領域作成
        areas[i].room = Room(
            areas[i].x + ROOM_MARGIN, 
            areas[i].y + ROOM_MARGIN, 
            areas[i].w - (ROOM_MARGIN * 2), 
            areas[i].h - (ROOM_MARGIN * 2)
        );
        // areas[i] = Area(
        //     areas[i].x, 
        //     areas[i].y, 
        //     areas[i].w, 
        //     areas[i].h
        // );
// 通路作成
        for(int y = areas[i].y; y < areas[i].y + areas[i].h; y++) {
            mapBuff[y][areas[i].room.x + areas[i].room.w + 1] = AISLE;
            mapBuff[y][areas[i].room.x + (seed % (areas[i].room.w - 2)) + 1] = AISLE;
        }
        for(int x = areas[i].x; x < areas[i].x + areas[i].w; x++) {
            mapBuff[areas[i].room.y + areas[i].room.h + 1][x] = AISLE;
            mapBuff[areas[i].room.y + (seed % (areas[i].room.h - 2)) + 1][x] = AISLE;
        }
// 部屋作成
        for(int y = areas[i].room.y; y < areas[i].room.y + areas[i].room.h; y++) {
            for(int x = areas[i].room.x; x < areas[i].room.x + areas[i].room.w; x++) {
                mapBuff[y][x] = FLOOR;
            }
        }
    }
    return areaCount;
}

