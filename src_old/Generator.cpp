#include "GenerateDungeon/Generator.h"

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

void Generator::randomEraseDeadEnd() {
    for(int y=0; y<FLOOR_H; y++) {
        for(int x=0; x<FLOOR_W; x++) {
            if(floorTYPE[y][x] == WALL_ALL) continue;
            int n = 0;
            if(floorTYPE[y+1][x] == WALL_ALL) n++;
            if(floorTYPE[y-1][x] == WALL_ALL) n++;
            if(floorTYPE[y][x+1] == WALL_ALL) n++;
            if(floorTYPE[y][x-1] == WALL_ALL) n++;
            if((n >= 3) && (rand()%5)) {
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
            if((n >= 3) && (rand()%5)) {
                floorTYPE[y][x] = WALL_ALL;
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

void Generator::output(Player player, std::vector<Enemy> enemies) {
    bool charDrawn = false;

    std::ofstream writing_file;
    std::string filename = "dungeon.map";
    writing_file.open(filename, std::ios::out);
    std::string writing_text;

    std::cout << "  ";
    for(int x=0; x<FLOOR_W; x++)
        std::cout << std::setw(2) << x;
    std::cout << "\n";
    for(int y=0; y<FLOOR_H; y++) {
        std::cout << std::setw(2) << y;
        for(int x=0; x<FLOOR_W; x++) {
            charDrawn = false;
            // SDLでは後ろから重ねて描画する
            if((player.getPos().x == x) && (player.getPos().y == y)) {
                std::cout << "@ ";
                charDrawn = true;
            }
            for(auto e : enemies) {
                if((e.getPos().x == x) && (e.getPos().y == y)) {
                    std::cout << e.getIcon();
                    charDrawn = true;
                }
            }
            if(charDrawn)
                continue;
            switch(floorTYPE[y][x]) {
                case NONE:              writing_text = "00"; std::cout << "~ "; break;
                case PILLAR:            writing_text = "01"; std::cout << "・"; break;
                case WALL_LEFT:         writing_text = "02"; std::cout << "| "; break;
                case WALL_RIGHT:        writing_text = "03"; std::cout << " |"; break;
                case WALL_TOP:          writing_text = "04"; std::cout << "￣"; break;
                case WALL_BOTTOM:       writing_text = "05"; std::cout << "＿"; break;
                case WALL_LEFT_TOP:     writing_text = "06"; std::cout << "┏ "; break;
                case WALL_LEFT_BOTTOM:  writing_text = "07"; std::cout << "┗ "; break;
                case WALL_RIGHT_TOP:    writing_text = "08"; std::cout << " ┓"; break;
                case WALL_RIGHT_BOTTOM: writing_text = "09"; std::cout << " ┛"; break;
                case WALL_SIDE_LR:      writing_text = "10"; std::cout << "||"; break;
                case WALL_SIDE_TB:      writing_text = "11"; std::cout << "二"; break;
                case WALL_END_LEFT:     writing_text = "12"; std::cout << "＞"; break;
                case WALL_END_RIGHT:    writing_text = "13"; std::cout << "＜"; break;
                case WALL_END_TOP:      writing_text = "14"; std::cout << "Ｖ"; break;
                case WALL_END_BOTTOM:   writing_text = "15"; std::cout << "Ａ"; break;
                case WALL_ALL:          writing_text = "16"; std::cout << "■ "; break;
                case FLOOR:             writing_text = "17"; std::cout << "  "; break;
                case AISLE:             writing_text = "18"; std::cout << "  "; break;
                case STEP:              writing_text = "19"; std::cout << "▃▅"; break;
            }
        }
        // if(isPlayerPos(x, y)) {
            //     std::cout << "@ ";
            //     continue;
            // }
            // if(isEnemiesPos(x, y)) {
            //     cout << "E ";
            //     continue;
            // }
        std::cout << "\n";
    }
    writing_file << writing_text << std::endl;
    writing_file.close();
}
