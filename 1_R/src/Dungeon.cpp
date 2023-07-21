#include <iostream>

#include "include/Dungeon.h"

Game *dungeon_g;

Dungeon::Dungeon(class Game *game)
{
    mGame = game;
    dungeon_g = game;
    LoadMedia();
    int roomCount = GenerateDungeon();
    // CreateActor(roomCount);
    // PlayMusic();
}

Dungeon::~Dungeon()
{
    UnloadData();
    MoveScene();
}

void Dungeon::Input(SDL_KeyCode keyCode)
{

}

void Dungeon::Draw()
{

}

/**************************************/

void Dungeon::LoadMedia()
{

}

int Dungeon::GenerateDungeon()
{
    int result = 0;
    std::cout << "ダンジョン生成方法を選択してください\n";
    std::cout << "1：区域分割法\n";
    std::cout << "2：オリジナル\n";
    std::cin >> input;
    // CELL_TYPE originalMap[mapH][mapW];
    for(int y = 0; y < mapH; y++) {
        for(int x = 0; x < mapW; x++) {
            originalMap[y][x] = WALL_ALL;
        }
    }
    switch(input) {
        case '1': 
            result = gen1(originalMap);
            break;
        case '2': 
            result = gen2(originalMap);
            break;
        default:
            std::cout << "不適切な値が入力されました。プログラムを終了します。\n";
    }

// 外周を壁で埋める
    for(int y = 0; y < mapH; y++) {
        originalMap[y][0] = originalMap[y][mapW-1] = WALL_ALL;
    }
    for(int x = 0; x < mapW; x++) {
        originalMap[0][x] = originalMap[mapH-1][x] = WALL_ALL;
    }

// 行き止まりをランダムでなくす
    for(int y = 0; y < mapH; y++) {
        for(int x = 0; x < mapW; x++) {
            if(originalMap[y][x] == WALL_ALL) continue;
            int n = 0;
            if(originalMap[y+1][x] == WALL_ALL) n++;
            if(originalMap[y-1][x] == WALL_ALL) n++;
            if(originalMap[y][x+1] == WALL_ALL) n++;
            if(originalMap[y][x-1] == WALL_ALL) n++;
            if((n >= 3) && (rand()%5)) {
                originalMap[y][x] = WALL_ALL;
            }
        }
    }
    for(int y = mapH-1; y >= 0; y--) {
        for(int x = mapW-1; x >= 0; x--) {
            if(originalMap[y][x] == WALL_ALL) continue;
            int n = 0;
            if(originalMap[y+1][x] == WALL_ALL) n++;
            if(originalMap[y-1][x] == WALL_ALL) n++;
            if(originalMap[y][x+1] == WALL_ALL) n++;
            if(originalMap[y][x-1] == WALL_ALL) n++;
            if((n >= 3) && (rand()%5)) {
                originalMap[y][x] = WALL_ALL;
            }
        }
    }

// 壁の種類を特定
    CELL_TYPE buff[mapH+2][mapW+2];
    for(int y = 0; y < mapH+2; y++) {
        buff[y][0] = buff[y][mapW+1] = WALL_ALL;
    }
    for(int x = 0; x < mapW+2; x++) {
        buff[0][x] = buff[mapH+1][x] = WALL_ALL;
    }
    for(int y = 0; y < mapH; y++) {
        for(int x = 0; x < mapW; x++) {
            buff[y+1][x+1] = originalMap[y][x];
        }
    }

    for(int y = 0; y < mapH; y++) {
        for(int x = 0; x < mapW; x++) {
            if(buff[y+1][x+1] != WALL_ALL) continue;
            int connect[4] = {0, 0, 0, 0}; // 0:床と接している、1:壁と接している
            int connectWallNum = 0;
            if(buff[y+2][x+1] == WALL_ALL) { // 下
                connect[0] = 1;
                connectWallNum++;
            }
            if(buff[y][x+1] == WALL_ALL) { // 上
                connect[1] = 1;
                connectWallNum++;
            }
            if(buff[y+1][x+2] == WALL_ALL) { // 右
                connect[2] = 1;
                connectWallNum++;
            }
            if(buff[y+1][x] == WALL_ALL) { // 左
                connect[3] = 1;
                connectWallNum++;
            }

            if(connectWallNum == 4) continue;
            else if(connectWallNum == 3) {
                if(connect[0] == 0) originalMap[y][x] = WALL_BOTTOM;
                else if(connect[1] == 0) originalMap[y][x] = WALL_TOP;
                else if(connect[2] == 0) originalMap[y][x] = WALL_RIGHT;
                else if(connect[3] == 0) originalMap[y][x] = WALL_LEFT;
            } else if(connectWallNum == 2) {
                if((connect[0] == 0) && connect[1] == 0) originalMap[y][x] = WALL_SIDE_TB;
                else if((connect[0] == 0) && connect[2] == 0) originalMap[y][x] = WALL_RIGHT_BOTTOM;
                else if((connect[0] == 0) && connect[3] == 0) originalMap[y][x] = WALL_LEFT_BOTTOM;
                else if((connect[1] == 0) && connect[2] == 0) originalMap[y][x] = WALL_RIGHT_TOP;
                else if((connect[1] == 0) && connect[3] == 0) originalMap[y][x] = WALL_LEFT_TOP;
                else if((connect[2] == 0) && connect[3] == 0) originalMap[y][x] = WALL_SIDE_LR;
            } else if(connectWallNum == 1) {
                if(connect[0] == 1) originalMap[y][x] = WALL_END_BOTTOM;
                else if(connect[1] == 1) originalMap[y][x] = WALL_END_TOP;
                else if(connect[2] == 1) originalMap[y][x] = WALL_END_RIGHT;
                else if(connect[3] == 1) originalMap[y][x] = WALL_END_LEFT;
            } else {
                originalMap[y][x] = PILLAR;
            }
        }
    }
    for(int y = 0; y < mapH; y++) {
        for(int x = 0; x < mapW; x++) {
            changedMap[y][x] = originalMap[y][x];
        }
    }
    return result;
}

void Dungeon::CreateActor(int roomCount)
{
    // mPlayer = new Player(this);
    // int roomNum = rand() % roomCount;
    // Vector2 pos;
    // pos.x = areas[roomNum].room.x + rand()%areas[roomNum].room.w;
    // pos.y = areas[roomNum].room.y + rand()%areas[roomNum].room.h;

    // while(changedMap[(int)pos.y][(int)pos.x] != AISLE
    // && changedMap[(int)pos.y][(int)pos.x] != FLOOR) {
    //     pos.x = areas[roomNum].room.x + rand()%areas[roomNum].room.w;
    //     pos.y = areas[roomNum].room.y + rand()%areas[roomNum].room.h;
    // }

    // mPlayer->SetPosition(pos);
    // changedMap[(int)pos.y][(int)pos.x] = PLAYER;
}

void Dungeon::PlayMusic()
{

}

void Dungeon::UnloadData()
{
    while (!mActors.empty())
	{
		delete mActors.back();
	}
}

void Dungeon::MoveScene()
{
    
}


// void Dungeon::UpdateGame()
// {
    // // 敵アップデート
    // mUpdatingActors = true;
    // for(auto actor : mActors)
    // {
    //     actor->Update();
    // }
    // mUpdatingActors = false;
// 
    // for(auto pending : mPendingActors)
    // {
    //     mActors.emplace_back(pending);
    // }
    // mPendingActors.clear();
// 
    // // std::vector<Actor*> deadActors;
    // for(auto actor : mActors)
    // {
    //     if(actor->GetState() == Actor::Dead)
    //     {
    //         // deadActors.emplace_back(actor);
    //         delete actor;
    //     }
    // }
// 
    // for(auto actor : deadActors)
    // {
    //     delete actor:
    // }
// }

// void Dungeon::GenerateOutput()
// {
// 
    // std::cout << "  ";
    // for(int x = 0; x < mapW; x++)
    //     std::cout << std::setw(2) << x;
    // std::cout << "\n";
    // for(int y = 0; y < mapH; y++) {
    //     std::cout << std::setw(2) << y;
    //     for(int x = 0; x < mapW; x++) {
    //         switch(changedMap[y][x]) {
    //             case NONE:              std::cout << "~ "; break;
    //             case PILLAR:            std::cout << "・"; break;
    //             case WALL_LEFT:         std::cout << "｜"; break;
    //             case WALL_RIGHT:        std::cout << "｜"; break;
    //             case WALL_TOP:          std::cout << "￣"; break;
    //             case WALL_BOTTOM:       std::cout << "＿"; break;
    //             case WALL_LEFT_TOP:     std::cout << "⎾ "; break;
    //             case WALL_LEFT_BOTTOM:  std::cout << "⎿ "; break;
    //             case WALL_RIGHT_TOP:    std::cout << "⏋ "; break;
    //             case WALL_RIGHT_BOTTOM: std::cout << "⏌ "; break;
    //             case WALL_SIDE_LR:      std::cout << "||"; break;
    //             case WALL_SIDE_TB:      std::cout << "二"; break;
    //             case WALL_END_LEFT:     std::cout << "］"; break;
    //             case WALL_END_RIGHT:    std::cout << "［"; break;
    //             case WALL_END_TOP:      std::cout << "Ｖ"; break;
    //             case WALL_END_BOTTOM:   std::cout << "Ａ"; break;
    //             case WALL_ALL:          std::cout << "■ "; break;    // Windowsではスーペースはいらない
    //             case FLOOR: case AISLE: std::cout << "  "; break;
    //             case STEP:              std::cout << "S "; break;
// 
    //             case PLAYER:            std::cout << "@ "; break;
    //             case ENEMY:             std::cout << "E "; break;
    //         }
    //     }
    //     std::cout << "\n";
    // }
// }

// void Dungeon::Quit()
// {
//     UnloadData();
// }

// void Dungeon::AddActor(Actor* actor)
// {
    // if(mUpdatingActors)
    // {
    //     mPendingActors.emplace_back(actor);
    // }
    // else
    // {
    //     mActors.emplace_back(actor);
    // }
// }

// void Dungeon::RemoveActor(Actor* actor)
// {
    // auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    // if(iter != mPendingActors.end())
    // {
    //     std::iter_swap(iter, mPendingActors.end() - 1);
    //     mPendingActors.pop_back();
    //     // mPendingActors.erase(iter);
    // }
// 
    // iter = std::find(mActors.begin(), mActors.end(), actor);
    // if(iter != mActors.end())
    // {
    //     std::iter_swap(iter, mActors.end() - 1);
    //     mActors.pop_back();
    //     // mActors.erase
    // }
// }


// void selectGenerateType() {
//     cout << "ダンジョンの生成方法を選択してください\n";
//     cout << "1: 区域分割法\n";
//     cout << "2: オリジナル\n";
//     cin >> input;
//     switch(input) {
//         case '1':
//             AreaDivision();
//             break;
//         case '2':
//             Original();
//             break;
//         default:
//             selectGenerateType();
//     }
// }

// void start() {
//     srand(time(NULL));
//     randomNumber = rand();
//     areas[0] = Area(0, 0, FLOOR_W, FLOOR_H);
//     areaCount = 1;
//     initFloor();
//     selectGenerateType();
//     fillSurround();
//     randomEraseDeadEnd();
//     identificationWallKind();
//     switch(input) {
//         case '1':
//             arrangementPlayer(areaCount);
//             arrangementEnemy(areaCount);
//             break;
//         case '2':
//             arrangementPlayer(areas.size());
//             arrangementEnemy(areas.size());
//             break;
//     }
//     cout << areaCount << " : " << areas.size() << "\n";
//     while(input != 'q') {
//         // output();
//         outputInWindows();
//         cin >> input;
//         playerMove(input);
//         enemyMove();
//         cin.clear();
//         cin.ignore();
//     }
// }
