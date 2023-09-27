#include "Scene/4_Dungeon.h"

extern int WIN_W;
extern int WIN_H;

Game *dungeon_g;

Dungeon::Dungeon(Game *game)
:inDungeon(true), goNextFloor(true)
{
    dungeon_g = game;
    mGame = game;
    LoadData();
}

// TODO: 削除予定
const char* get(DIRECTION _dir)
{
    switch(_dir)
    {
        case LEFT:  return "左";
        case RIGHT: return "右";
        case UP:    return "上";
        case DOWN:  return "下";
        default:    return "空";
    }
}

void Dungeon::Input(SDL_Event event)
{
std::cout << "Input\n";
    // プレイヤーのアップデート
    glm::vec2 front;
    playerMoved = false;
    switch(event.key.keysym.sym)
    {
        case SDLK_w: playerMoved = true; player.move(DIRECTION::UP);    break;
        case SDLK_a: playerMoved = true; player.move(DIRECTION::LEFT);  break;
        case SDLK_s: playerMoved = true; player.move(DIRECTION::DOWN);  break;
        case SDLK_d: playerMoved = true; player.move(DIRECTION::RIGHT); break;
        case SDLK_k:
            playerMoved = true;
            front = getFrontPos(player.getPos(), player.getDir());
            if(!isOtherPos(front))
                break;
            player.attack(whichEnemy(front));
        break;
        case SDLK_q: inDungeon = false; break;
    }
    if(!canGetOn(player.getPos()))
        player.back();
// 階段を登ったときの処理
    if(areaDivide.buff[(int)player.getPos().y+1][(int)player.getPos().x+1] == STEP) {
        goNextFloor = true;
        return;
    }
}

void Dungeon::Update()
{
std::cout << "Update\n";
    if(!inDungeon)
    {
std::cout << "ダンジョン脱出\n";
        dungeon_g->setNowScene(SCENE::HOME);
        return;
    }
    if(goNextFloor)
    {
std::cout << "ダンジョン初期化\n";
        InitDungeon();
        return;
    }
// 敵のアップデート
    glm::vec2 front;
    if(playerMoved) for(auto& e : enemies)
    {
        // std::cout << "残り距離　: " << e.getRouteSize() << std::endl;
        // std::cout << "経過ターン: " << e.getElapsedTurn() << std::endl;
std::cout << "Enemy updating\n";
        switch(e.getState())
        {
            case SEARCH:
std::cout << "case SEARCH\n";
std::cout << abs(e.getPos().x - player.getPos().x) + abs(e.getPos().y - player.getPos().y) << std::endl;
                if(abs(e.getPos().x - player.getPos().x) + abs(e.getPos().y - player.getPos().y) <= ENEMY_FIND_RANGE) {
                    e.setState(FOUND);
                    e.routeClear();
                }
std::cout << e.getRouteSize() << " : " << e.getElapsedTurn() << std::endl;
                if((e.getRouteSize() < 1) || (e.getElapsedTurn() > ENEMY_SEARCH_INTERVAL))
                    e.setGoal(areaDivide.getFloor(), getRandomPos(areaDivide.areaCount));
                e.walk();
            break;
            case FOUND:
std::cout << "case FOUND\n";
                if(abs(e.getPos().x - player.getPos().x) + abs(e.getPos().y - player.getPos().y) > ENEMY_FIND_RANGE)
                    e.setState(SEARCH);
                // 攻撃できるとき
                front = getFrontPos(e.getPos(), e.getDir());
                if(player.getPos() == front) {
                    e.attack(player);
                    break;
                }
                // 攻撃できないとき
                else if((e.getRouteSize() < 1) || (e.getElapsedTurn() > ENEMY_SEARCH_INTERVAL))
                    e.setGoal(areaDivide.getFloor(), player.getPos());
                e.walk();
            break;
            case ESCAPE:
std::cout << "case ESCAPE\n";
            break;
            case DEAD:
std::cout << "case DEAD\n";
                deadEnemies.push_back(e);
            break;
// その他
            case ALIVE: break;
        }

        if(!canGetOn(e.getPos()))
            e.back();
    }
    std::cout << "Enemy updated\n";
// 敵の死亡判定
    for(auto it = enemies.begin(); it < enemies.end(); it++)
        if(it->getState() == DEAD)
            enemies.erase(it);
// プレイヤーの死亡判定
    if(player.getState() == DEAD) {
        inDungeon = false;
        goNextFloor = true;
    }
}

void Dungeon::Output()
{
std::cout << "Output\n";
    areaDivide.output(player, enemies);
    // for(int y=0; y<FLOOR_H+2; y++) {
    //     for(int x=0; x<FLOOR_W+2; x++) {
    //         std::cout << static_cast<int>(areaDivide.buff[y][x]) << " ";
    //     }
    //     std::cout << std::endl;
    // }
    std::cout << "　　　　　  | HP |向き|\n" ;
    std::cout << "プレイヤー: |" << std::setw(4) << player.getNowHP() << "| " << get(player.getDir()) << " |\n";
    // for(auto e: enemies) {
    // std::cout << "　　敵　　: |" << std::setw(4) << e.getNowHP() << "|\n";
    // }
}

void Dungeon::LoadData()
{
}

void Dungeon::PlayMusic()
{
}

// ダンジョン生成

void Dungeon::InitDungeon()
{
    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<int> randNum(AREA_MAX, 100);

    inDungeon = true;
    goNextFloor = false;
    enemies.clear();
    enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(SLIME));

    areaDivide.initFloor();
    areaDivide.generate(randNum(engine));
    areaDivide.fillSurround();
    areaDivide.randomEraseDeadEnd();
    areaDivide.identificationWallKind();

    for(auto& e : enemies)
    {
        e = Enemy((ENEMY_TYPE)(rand() % ENEMY_TYPE_NUMBER));
    }

    glm::vec2 pos = getRandomPos(areaDivide.getRoomNum());
    player.setPos(pos);
    for(auto& e : enemies)
    {
        pos = getRandomPos(areaDivide.getRoomNum());
        e.setPos(pos);
    }
    for(auto e : enemies)
    {
        std::cout << "(" << e.getPos().x << ", " << e.getPos().y << ")\n";
    }
}

void Dungeon::quit()
{
}

int Dungeon::isOtherPos(glm::vec2 _pos)
{
    int sameNum = 0;
    if(player.getPos() == _pos)
            sameNum++;
    for(auto e : enemies)
    {
        if(e.getPos() == _pos)
            sameNum++;
    }
    return sameNum;
}

bool Dungeon::canGetOn(glm::vec2 _pos) 
{
    if((areaDivide.buff[(int)_pos.y+1][(int)_pos.x+1] != FLOOR) 
    && (areaDivide.buff[(int)_pos.y+1][(int)_pos.x+1] != AISLE)
    && (areaDivide.buff[(int)_pos.y+1][(int)_pos.x+1] != STEP)) 
        return false;
    if(isOtherPos(_pos)  > 1)
        return false;
    return true;
}

glm::vec2 Dungeon::getRandomPos(int _roomCount)
{
    int roomNum = rand() % _roomCount;
    Room room = areaDivide.getRoom(roomNum);
    glm::vec2 pos;
    pos.x = room.x + rand()%room.w;
    pos.y = room.y + rand()%room.h;
    while(!canGetOn(pos)) {
        pos.x = room.x + rand()%room.w;
        pos.y = room.y + rand()%room.h;
    }
    return pos;
}

Enemy& Dungeon::whichEnemy(glm::vec2 _pos)
{
    for(auto& e : enemies)
    {
        if((e.getPos().x == _pos.x) && (e.getPos().y == _pos.y))
            return e;
    }
    throw std::runtime_error("Enemy not found");
}

glm::vec2 Dungeon::getFrontPos(glm::vec2 _pos, DIRECTION _dir)
{
    glm::vec2 front(0, 0);
    switch(_dir)
    {
        case LEFT:  front.x = -1; break;
        case RIGHT: front.x = 1;  break;
        case UP:    front.y = -1; break;
        case DOWN:  front.y = 1;  break;
        default: break;
    }
    return _pos + front;
}
