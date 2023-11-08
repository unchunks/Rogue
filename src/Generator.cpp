#include "GenerateDungeon/Generator.h"

std::random_device random_seed;
std::mt19937 random_engine(random_seed());
std::uniform_int_distribution<int> random_num(AREA_MAX, 100);

Generator::Generator()
{
}

Generator::~Generator()
{
    areas.clear();
    areas.shrink_to_fit();
    rooms.clear();
    rooms.shrink_to_fit();
}

void Generator::initFloor() {
    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            floorTYPE[y][x] = WALL_ALL;
        }
    }
}

void Generator::fillSurround() {
    for(int y=0; y<FLOOR_H; y++) {
        floorTYPE[y][0] = floorTYPE[y][FLOOR_W-1] = WALL_ALL;
    }
    for(int x=0; x<FLOOR_W; x++) {
        floorTYPE[0][x] = floorTYPE[FLOOR_H-1][x] = WALL_ALL;
    }
}

void Generator::eraseDeadEnd() {
    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            if(floorTYPE[y][x] == WALL_ALL) continue;
            int n = 0;
            if(floorTYPE[y+1][x] == WALL_ALL) n++;
            if(floorTYPE[y-1][x] == WALL_ALL) n++;
            if(floorTYPE[y][x+1] == WALL_ALL) n++;
            if(floorTYPE[y][x-1] == WALL_ALL) n++;
            if(n >= 3) {
                floorTYPE[y][x] = WALL_ALL;
            }
        }
    }
    for(int y=FLOOR_H-1; y>=0; y--) {
        for(int x=FLOOR_W-1; x>=0; x--) {
            if(floorTYPE[y][x] == WALL_ALL) continue;
            int n = 0;
            if(floorTYPE[y+1][x] == WALL_ALL) n++;
            if(floorTYPE[y-1][x] == WALL_ALL) n++;
            if(floorTYPE[y][x+1] == WALL_ALL) n++;
            if(floorTYPE[y][x-1] == WALL_ALL) n++;
            if(n >= 3) {
                floorTYPE[y][x] = WALL_ALL;
            }
        }
    }
}

void Generator::outputMap_forDebug() {
    bool charDrawn = false;

    std::ofstream writing_file;
    std::string filename = "debug.map";
    writing_file.open(filename, std::ios::out);
    std::string writing_text = "";

    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            charDrawn = false;
            if(charDrawn)
                continue;
            switch(floorTYPE[y][x]) {
                case NONE:      writing_text += "00 "; break; //std::cout << "~ ";
                case WALL_ALL:  writing_text += "16 "; break; //std::cout << "■ ";
                case FLOOR:     writing_text += "17 "; break; //std::cout << "  ";
                case AISLE:     writing_text += "18 "; break; //std::cout << "  ";
                default:        writing_text += "   ";
            }
        }
        writing_text += "\n";
    }
    writing_file << writing_text << std::endl;
    writing_file.close();
}

void Generator::randomEraseDeadEnd() {
    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            if(floorTYPE[y][x] == WALL_ALL) continue;
            int n = 0;
            if(floorTYPE[y+1][x] == WALL_ALL) n++;
            if(floorTYPE[y-1][x] == WALL_ALL) n++;
            if(floorTYPE[y][x+1] == WALL_ALL) n++;
            if(floorTYPE[y][x-1] == WALL_ALL) n++;
            if((n >= 3) && (random_num(random_engine)%5)) {
                floorTYPE[y][x] = WALL_ALL;
            }

            if(floorTYPE[(FLOOR_H-1)-y][(FLOOR_W-1)-x] == WALL_ALL) continue;
            n = 0;
            if(floorTYPE[(FLOOR_H-1)-y+1][(FLOOR_W-1)-x] == WALL_ALL) n++;
            if(floorTYPE[(FLOOR_H-1)-y-1][(FLOOR_W-1)-x] == WALL_ALL) n++;
            if(floorTYPE[(FLOOR_H-1)-y][(FLOOR_W-1)-x+1] == WALL_ALL) n++;
            if(floorTYPE[(FLOOR_H-1)-y][(FLOOR_W-1)-x-1] == WALL_ALL) n++;
            if((n >= 3) && (random_num(random_engine)%5)) {
                floorTYPE[(FLOOR_H-1)-y][(FLOOR_W-1)-x] = WALL_ALL;
            }
        }
    }
}

void Generator::identificationWallKind() {
    for(int y=0; y<FLOOR_H + 2; y++) {
        buff[y][0] = buff[y][FLOOR_W + 1] = WALL_ALL;
    }
    for(int x=0; x<FLOOR_W + 2; x++) {
        buff[0][x] = buff[FLOOR_H + 1][x] = WALL_ALL;
    }
    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            buff[y + 1][x + 1] = floorTYPE[y][x];
        }
    }

    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            if(buff[y + 1][x + 1] != WALL_ALL) continue;
            int connect[4] = {0, 0, 0, 0}; // 0:床と接している、1:壁と接している
            int connectWallNum = 0;
            if(buff[y + 2][x + 1] == WALL_ALL) { // 下
                connect[0] = 1;
                connectWallNum++;
            }
            if(buff[y][x + 1] == WALL_ALL) { // 上
                connect[1] = 1;
                connectWallNum++;
            }
            if(buff[y + 1][x + 2] == WALL_ALL) { // 右
                connect[2] = 1;
                connectWallNum++;
            }
            if(buff[y + 1][x] == WALL_ALL) { // 左
                connect[3] = 1;
                connectWallNum++;
            }

            if(connectWallNum == 4) continue;
            else if(connectWallNum == 3) {
                if(connect[0] == 0) floorTYPE[y][x] = WALL_BOTTOM;
                else if(connect[1] == 0) floorTYPE[y][x] = WALL_TOP;
                else if(connect[2] == 0) floorTYPE[y][x] = WALL_RIGHT;
                else if(connect[3] == 0) floorTYPE[y][x] = WALL_LEFT;
            } else if(connectWallNum == 2) {
                if((connect[0] == 0) && connect[1] == 0) floorTYPE[y][x] = WALL_SIDE_TB;
                else if((connect[0] == 0) && connect[2] == 0) floorTYPE[y][x] = WALL_RIGHT_BOTTOM;
                else if((connect[0] == 0) && connect[3] == 0) floorTYPE[y][x] = WALL_LEFT_BOTTOM;
                else if((connect[1] == 0) && connect[2] == 0) floorTYPE[y][x] = WALL_RIGHT_TOP;
                else if((connect[1] == 0) && connect[3] == 0) floorTYPE[y][x] = WALL_LEFT_TOP;
                else if((connect[2] == 0) && connect[3] == 0) floorTYPE[y][x] = WALL_SIDE_LR;
            } else if(connectWallNum == 1) {
                if(connect[0] == 1) floorTYPE[y][x] = WALL_END_BOTTOM;
                else if(connect[1] == 1) floorTYPE[y][x] = WALL_END_TOP;
                else if(connect[2] == 1) floorTYPE[y][x] = WALL_END_RIGHT;
                else if(connect[3] == 1) floorTYPE[y][x] = WALL_END_LEFT;
            } else {
                floorTYPE[y][x] = PILLAR;
            }
        }
    }
    // for(int y=0; y<FLOOR_H; y++)
    //     for(int x=0; x<FLOOR_W; x++)
    //         buff[y+1][x+1] = floorTYPE[y][x];
}

void Generator::outputMap() {
    bool charDrawn = false;

    std::ofstream writing_file;
    std::string filename = "dungeon.map";
    writing_file.open(filename, std::ios::out);
    std::string writing_text = "";

    std::cout << "1\n";
    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            charDrawn = false;
            if(charDrawn)
                continue;
            switch(floorTYPE[y][x]) {
                case NONE:              buff[y+1][x+1] = NONE;              writing_text += "00 "; break; //std::cout << "~ ";
                case PILLAR:            buff[y+1][x+1] = PILLAR;            writing_text += "01 "; break; //std::cout << "・";
                case WALL_LEFT:         buff[y+1][x+1] = WALL_LEFT;         writing_text += "02 "; break; //std::cout << "| ";
                case WALL_RIGHT:        buff[y+1][x+1] = WALL_RIGHT;        writing_text += "03 "; break; //std::cout << " |";
                case WALL_TOP:          buff[y+1][x+1] = WALL_TOP;          writing_text += "04 "; break; //std::cout << "￣";
                case WALL_BOTTOM:       buff[y+1][x+1] = WALL_BOTTOM;       writing_text += "05 "; break; //std::cout << "＿";
                case WALL_LEFT_TOP:     buff[y+1][x+1] = WALL_LEFT_TOP;     writing_text += "06 "; break; //std::cout << "┏ ";
                case WALL_LEFT_BOTTOM:  buff[y+1][x+1] = WALL_LEFT_BOTTOM;  writing_text += "07 "; break; //std::cout << "┗ ";
                case WALL_RIGHT_TOP:    buff[y+1][x+1] = WALL_RIGHT_TOP;    writing_text += "08 "; break; //std::cout << " ┓";
                case WALL_RIGHT_BOTTOM: buff[y+1][x+1] = WALL_RIGHT_BOTTOM; writing_text += "09 "; break; //std::cout << " ┛";
                case WALL_SIDE_LR:      buff[y+1][x+1] = WALL_SIDE_LR;      writing_text += "10 "; break; //std::cout << "||";
                case WALL_SIDE_TB:      buff[y+1][x+1] = WALL_SIDE_TB;      writing_text += "11 "; break; //std::cout << "二";
                case WALL_END_LEFT:     buff[y+1][x+1] = WALL_END_LEFT;     writing_text += "12 "; break; //std::cout << "＞";
                case WALL_END_RIGHT:    buff[y+1][x+1] = WALL_END_RIGHT;    writing_text += "13 "; break; //std::cout << "＜";
                case WALL_END_TOP:      buff[y+1][x+1] = WALL_END_TOP;      writing_text += "14 "; break; //std::cout << "Ｖ";
                case WALL_END_BOTTOM:   buff[y+1][x+1] = WALL_END_BOTTOM;   writing_text += "15 "; break; //std::cout << "Ａ";
                case WALL_ALL:          buff[y+1][x+1] = WALL_ALL;          writing_text += "16 "; break; //std::cout << "■ ";
                case FLOOR:             buff[y+1][x+1] = FLOOR;             writing_text += "17 "; break; //std::cout << "  ";
                case AISLE:             buff[y+1][x+1] = AISLE;             writing_text += "18 "; break; //std::cout << "  ";
                case STEP:              buff[y+1][x+1] = STEP;              writing_text += "19 "; break; //std::cout << "▃▅";
            }
        }
        writing_text += "\n";
    }
    std::cout << "2\n";
    writing_file << writing_text << std::endl;
    writing_file.close();
}

/// @brief ランダムなデータ系座標を返す
glm::vec2 Generator::getRandomFloorPos()
{
    int roomNum = random_num(random_engine) % rooms.size();
	Room room = rooms[roomNum];
    glm::vec2 pos;
    pos.x = room.x + random_num(random_engine)%room.w;
    pos.y = room.y + random_num(random_engine)%room.h;
    while(floorTYPE[static_cast<int>(pos.y)][static_cast<int>(pos.x)] != FLOOR) {
        pos.x = room.x + random_num(random_engine)%room.w;
        pos.y = room.y + random_num(random_engine)%room.h;
    }
    return pos;
}
