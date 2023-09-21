// #ifndef __DUNGEON
// #define __DUNGEON

// #include <iostream>
// #include <random>
// #include <glm/glm.hpp>

// #include "AreaDivide.h"

// #include "Character.h"
// #include "Player.h"
// #include "Enemy.h"
// #include "Const.h"

// class Dungeon
// {
// public:
//     void init(int _randomNumber);
//     void run();
//     void quit();

//     void update();

//     bool isRunning = true;

// private:
//     bool goNextFloor = false;
//     char input = 'a';
//     AreaDivide areaDivide;

//     Player player = Player(0, 0, PLAYER_HP, PLAYER_STR, PLAYER_VIT);
//     std::vector<Enemy> enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(SLIME));
//     std::vector<Enemy> deadEnemies;

//     int isOtherPos(glm::vec2 _pos);
//     bool canGetOn(glm::vec2 _pos);
//     glm::vec2 getRandomPos(int _roomCount);
//     glm::vec2 getFrontPos(glm::vec2 _pos, DIRECTION _dir);
//     Enemy& whichEnemy(glm::vec2 _pos);
// };

// // TODO: 削除予定
// const char* get(DIRECTION _dir)
// {
//     switch(_dir)
//     {
//         case LEFT:  return "左";
//         case RIGHT: return "右";
//         case UP:    return "上";
//         case DOWN:  return "下";
//     }
//     return "空";
// }

// void Dungeon::init(int _randomNumber)
// {
//     isRunning = true;
//     goNextFloor = false;
//     enemies.clear();
//     enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(SLIME));

//     areaDivide.initFloor();
//     areaDivide.generate(_randomNumber);
//     areaDivide.fillSurround();
//     areaDivide.randomEraseDeadEnd();
//     areaDivide.identificationWallKind();

//     for(auto& e : enemies)
//     {
//         e = Enemy((ENEMY_TYPE)(rand() % ENEMY_TYPE_NUMBER));
//     }

//     glm::vec2 pos = getRandomPos(areaDivide.getRoomNum());
//     player.setPos(pos);
//     for(auto& e : enemies)
//     {
//         pos = getRandomPos(areaDivide.getRoomNum());
//         e.setPos(pos);
//     }
//     for(auto e : enemies)
//     {
//         std::cout << "(" << e.getPos().x << ", " << e.getPos().y << ")\n";
//     }
// }

// void Dungeon::run()
// {
//     while(isRunning && !goNextFloor)
//     {
//         areaDivide.output(player, enemies);
//         std::cout << "　　　　　  | HP |向き|\n" ;
//         std::cout << "プレイヤー: |" << std::setw(4) << player.getNowHP() << "| " << get(player.getDir()) << " |\n";
//         // for(auto e: enemies) {
//         // std::cout << "　　敵　　: |" << std::setw(4) << e.getNowHP() << "|\n";
//         // }
//         std::cin >> input;
//         if(input == 'q') isRunning = false;
//         update();
//     }
// }

// void Dungeon::quit()
// {
// }

// void Dungeon::update()
// {
//     glm::vec2 front;
// // プレイヤーのアップデート
//     switch(input)
//     {
//     case 'w': player.move(DIRECTION::UP);    break;
//     case 'a': player.move(DIRECTION::LEFT);  break;
//     case 's': player.move(DIRECTION::DOWN);  break;
//     case 'd': player.move(DIRECTION::RIGHT); break;
//     case 'k':
//         front = getFrontPos(player.getPos(), player.getDir());
//         if(!isOtherPos(front))
//             break;
//         player.attack(whichEnemy(front));
//     break;
//     }
//     if(!canGetOn(player.getPos()))
//         player.back();
// // TODO: 階段を登ったときの処理
//     if(areaDivide.buff[(int)player.getPos().y+1][(int)player.getPos().x+1] == STEP) {
//         goNextFloor = true;
//         return;
//     }

// // 敵のアップデート
//     for(auto& e : enemies)
//     {
//         // std::cout << "残り距離　: " << e.getRouteSize() << std::endl;
//         // std::cout << "経過ターン: " << e.getElapsedTurn() << std::endl;
//         switch(e.getState())
//         {
//         case SEARCH:
//             if(abs(e.getPos().x - player.getPos().x) + abs(e.getPos().y - player.getPos().y) <= ENEMY_FIND_RANGE) {
//                 e.setState(FOUND);
//                 e.routeClear();
//             }
//             if((e.getRouteSize() < 1) || (e.getElapsedTurn() > ENEMY_SEARCH_INTERVAL))
//                 e.setGoal(areaDivide.getFloor(), getRandomPos(areaDivide.areaCount));
//             e.walk();
//         break;

//         case FOUND:
//             if(abs(e.getPos().x - player.getPos().x) + abs(e.getPos().y - player.getPos().y) > ENEMY_FIND_RANGE)
//                 e.setState(SEARCH);
//             // 攻撃できるとき
//             front = getFrontPos(e.getPos(), e.getDir());
//             if(player.getPos() == front) {
//                 e.attack(player);
//                 break;
//             }
//             // 攻撃できないとき
//             else if((e.getRouteSize() < 1) || (e.getElapsedTurn() > ENEMY_SEARCH_INTERVAL))
//                 e.setGoal(areaDivide.getFloor(), player.getPos());
//             e.walk();
//         break;

//         case ESCAPE:
//         break;
//         case DEAD:
//             deadEnemies.push_back(e);
//         break;
//         }

//         if(!canGetOn(e.getPos()))
//             e.back();
//     }
//     for(auto it = enemies.begin(); it < enemies.end(); it++)
//         if(it->getState() == DEAD)
//             enemies.erase(it);
//     if(player.getState() == DEAD)
//         isRunning = false;
// }


// int Dungeon::isOtherPos(glm::vec2 _pos)
// {
//     int sameNum = 0;
//     if(player.getPos() == _pos)
//             sameNum++;
//     for(auto e : enemies)
//     {
//         if(e.getPos() == _pos)
//             sameNum++;
//     }
//     return sameNum;
// }

// bool Dungeon::canGetOn(glm::vec2 _pos) 
// {
//     if((areaDivide.buff[(int)_pos.y+1][(int)_pos.x+1] != FLOOR) 
//     && (areaDivide.buff[(int)_pos.y+1][(int)_pos.x+1] != AISLE)
//     && (areaDivide.buff[(int)_pos.y+1][(int)_pos.x+1] != STEP)) 
//         return false;
//     if(isOtherPos(_pos)  > 1)
//         return false;
//     return true;
// }

// glm::vec2 Dungeon::getRandomPos(int _roomCount)
// {
//     int roomNum = rand() % _roomCount;
//     Room room = areaDivide.getRoom(roomNum);
//     glm::vec2 pos;
//     pos.x = room.x + rand()%room.w;
//     pos.y = room.y + rand()%room.h;
//     while(!canGetOn(pos)) {
//         pos.x = room.x + rand()%room.w;
//         pos.y = room.y + rand()%room.h;
//     }
//     return pos;
// }

// Enemy& Dungeon::whichEnemy(glm::vec2 _pos)
// {
//     for(auto& e : enemies)
//     {
//         if((e.getPos().x == _pos.x) && (e.getPos().y == _pos.y))
//             return e;
//     }
//     Enemy enemy(0, 0, 0, 0, 0, "  ");
//     return enemy;
// }

// glm::vec2 Dungeon::getFrontPos(glm::vec2 _pos, DIRECTION _dir)
// {
//     glm::vec2 front(0, 0);
//     switch(_dir)
//     {
//     case LEFT:  front.x = -1; break;
//     case RIGHT: front.x = 1;  break;
//     case UP:    front.y = -1; break;
//     case DOWN:  front.y = 1;  break;
//     }
//     return _pos + front;
// }

// #endif __DUNGEON