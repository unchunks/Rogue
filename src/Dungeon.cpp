#include <iostream>

#include "include/Dungeon.h"

using namespace std;

char input;

void selectGenerateType() {
    cout << "ダンジョンの生成方法を選択してください\n";
    cout << "1: 区域分割法\n";
    cout << "2: オリジナル\n";
    cin >> input;
    switch(input) {
        case '1':
            AreaDivision();
            break;
        case '2':
            Original();
            break;
        default:
            selectGenerateType();
    }
}

void start() {
    srand(time(NULL));
    randomNumber = rand();
    areas[0] = Area(0, 0, FLOOR_W, FLOOR_H);
    areaCount = 1;
    initFloor();

    selectGenerateType();

    fillSurround();
    randomEraseDeadEnd();
    identificationWallKind();

    switch(input) {
        case '1':
            arrangementPlayer(areaCount);
            arrangementEnemy(areaCount);
            break;
        case '2':
            arrangementPlayer(areas.size());
            arrangementEnemy(areas.size());
            break;
    }

    cout << areaCount << " : " << areas.size() << "\n";

    while(input != 'q') {
        // output();
        outputInWindows();
        cin >> input;
        playerMove(input);
        enemyMove();

        cin.clear();
        cin.ignore();
    }
}