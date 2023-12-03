#include "Scene/4_Dungeon.h"

#define DEBUG

extern SDL_Renderer *gRenderer;

extern std::mt19937 random_engine;
extern std::uniform_int_distribution<int> random_num;

Game *dungeon_g;

// マップ画像のテクスチャ
LTexture gTileTexture = LTexture();
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

int turn = 0;

Dungeon::Dungeon(Game *game)
    : inDungeon(true), goNextFloor(true), nowTurn(PLAYER)
{
    dungeon_g = game;
    mGame = game;

    tileSet.resize(TOTAL_TILES, Tile(0, 0, NONE));
    mPlayerSpriteClips.resize(ANIMATION_FRAMES * static_cast<int>(NO_DIRECTION), {0, 0, 0, 0});
    mEnemySpriteClips.resize(static_cast<int>(ENEMY_TYPE_NUMBER), std::vector<SDL_Rect>(ANIMATION_FRAMES * static_cast<int>(NO_DIRECTION), {0, 0, 0, 0}));
    if (!LoadData())
    {
        printf("Failed to load media!\n");
    }
    camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
}

Dungeon::~Dungeon()
{
    // Deallocate tiles
    tileSet.clear();
    tileSet.shrink_to_fit();

    // Free loaded images
    player.mSpriteClips.clear();
    player.mSpriteClips.shrink_to_fit();
    mEnemySpriteClips.clear();
    mEnemySpriteClips.shrink_to_fit();

    // player.mCharTexture.free(); //別の場所で解放済み?
    // gTileTexture.free();

    enemies.clear();
    enemies.shrink_to_fit();
}

void Dungeon::Input(SDL_Event event)
{
    if (!inDungeon || goNextFloor)
    {
        return;
    }

    // プレイヤーのアップデート
    if (!player.onTileCenter())
    {
        player.isMoved = true;
    }
    else if (event.type == SDL_KEYDOWN && nowTurn == PLAYER)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            turn++;
            player.isMoved = true;
            player.setDir(UP);
            break;
        case SDLK_a:
            turn++;
            player.isMoved = true;
            player.setDir(LEFT);
            break;
        case SDLK_s:
            turn++;
            player.isMoved = true;
            player.setDir(DOWN);
            break;
        case SDLK_d:
            turn++;
            player.isMoved = true;
            player.setDir(RIGHT);
            break;
        // case SDLK_k:
        // 	player.isMoved = true;
        // 	front = getFrontPos(player.getImagePos(), player.getDir());
        // 	if(!isOtherPos(front))
        // 		break;
        // 	player.attack(whichEnemy(front));
        // 	break;
        case SDLK_q:
            player.isMoved = false;
            inDungeon = false;
            goNextFloor = true;
            break;
        // NOTE: 以下デバッグ用
        case SDLK_1:    // 見た目をプレイヤーに変更
            player.isMoved = false;
            player.mSpriteClips = mPlayerSpriteClips;
            break;
        case SDLK_2:    // 見た目をデカに変更
            player.isMoved = false;
            player.mSpriteClips = mEnemySpriteClips[0];
            break;
        case SDLK_3:    // 見た目をグリに変更
            player.isMoved = false;
            player.mSpriteClips = mEnemySpriteClips[1];
            break;
        case SDLK_4:    // 見た目をジェリフに変更
            player.isMoved = false;
            player.mSpriteClips = mEnemySpriteClips[2];
            break;
        case SDLK_5:    // 見た目をヤミーに変更
            player.isMoved = false;
            player.mSpriteClips = mEnemySpriteClips[3];
            break;
        case SDLK_r:    // ランダムな位置にワープ
            player.isMoved = false;
            switch (dungeon_g->getNowScene())
            {
                case DUNGEON_AREA_DIVIDE:
                    player.setDataPos(getRandomPos(areaDivide.getRoomNum()));
                    break;
                case DUNGEON_RRA:
                    player.setDataPos(getRandomPos(rra.getRoomNum()));
                    break;
                default:
                    break;
            }
            break;
        case SDLK_t:    // 敵の近くにワープ
            player.isMoved = false;
            player.setDataPos(glm::vec2(
                enemies.at(0).getDataPos().x,
                enemies.at(0).getDataPos().y + 2));
            break;
        default:
            break;
        }
        if (player.isMoved)
            SDL_Log("Input: 現在%dターン目", turn);
    }
}

void Dungeon::Update()
{
    if (!inDungeon)
    {
        quit();
        dungeon_g->setNowScene(SCENE::HOME);
        return;
    }

    if (goNextFloor)
    {
        InitDungeon();
        return;
    }

    player.setCamera(camera);

    // if (!player.isMoved)
    //     return;

    // プレイヤーのアップデート
    if (nowTurn == PLAYER && player.isMoved)
    {
        if (player.onTileCenter())
        {
            SDL_Log("プレイヤーのターン=========================================================================");
        }
        std::vector<Character> otherCharacters;
        for (auto enemy : enemies)
        {
            otherCharacters.push_back(enemy);
        }
        player.move(tileSet, otherCharacters);
        if (player.onTileCenter())
        {
            SDL_Log("(%d, %d)", (int)player.getDataPos().x, (int)player.getDataPos().y);
        }

        // 階段を登ったときの処理
        goNextFloor = (floor[((int)player.getDataPos().y)][((int)player.getDataPos().x)] == STEP);
        if (goNextFloor)
            return;

        // 敵のターンに移行
        if (player.onTileCenter())
        {
            nowTurn = ENEMY;
            player.isMoved = false;
            for(auto &e : enemies)
            {
                e.isMoved = true;
            }
            SDL_Log("敵のターン===============================================================================");
        }

    }// 敵のアップデート
    else if (nowTurn == ENEMY)
    {
        for (auto &e : enemies)
        {
            switch (e.getState())
            {
                case SEARCH:
                    SDL_Log("SEARCH");
                    // 発見確認
                    if(e.changeState(player))
                    {
                        updateEnemyRoute(e, PLAYER_POS);
                        goto GOTO_FOUND;
                    }

                    // ルートの情報が古くなったり、なくなったりした場合に更新
                    if (e.mustUpdateRoute())
                    {
                        SDL_Log("SEARCH: ルート更新");
                        updateEnemyRoute(e, RANDOM_POS);
                    }
        GOTO_SEARCH:
                    e.walk(tileSet, player, enemies);
                    break;

                case FOUND:
                    SDL_Log("FOUND");
                    // 移動前の発見確認
                    if(e.changeState(player))
                    {
                        updateEnemyRoute(e, RANDOM_POS);
                        goto GOTO_SEARCH;
                    }
                    
                    // 隣り合っている場合は攻撃
                    if (e.adjacent(player))
                    {
                        e.attack(player);
                        break;
                    }
        GOTO_FOUND:
                    // e.walk(tileSet, player, enemies);
                    e.walkTo(player.getImagePos(), tileSet, player, enemies);
                    break;

                case ESCAPE:
                    break;
    //REVIEW: 
                case DEAD:
                    deadEnemies.push_back(e);
                    break;

                default:
                    break;
            }

            // 敵のターンに移行
            if (e.onTileCenter())
            {
                nowTurn = PLAYER;
                // プレイヤーが次動けるようfalseに
                player.isMoved = false;
            }
        }
    }

    if (player.getState() == DEAD)
    {
        inDungeon = false;
        goNextFloor = true;
    }

    for(auto e = deadEnemies.begin(); e < deadEnemies.end(); e++)
    {
        *e = Enemy(static_cast<ENEMY_TYPE>(random_num(random_engine) % static_cast<int>(ENEMY_TYPE_NUMBER)));
        e->mSpriteClips = mEnemySpriteClips.at(static_cast<int>(e->getEnemyType()));
        glm::vec2 pos;
        SDL_Log("敵の位置を初期化");
        switch (dungeon_g->getNowScene())
        {
            case DUNGEON_AREA_DIVIDE:
                pos = getRandomPos(areaDivide.getRoomNum());
                break;
            case DUNGEON_RRA:
                pos = getRandomPos(rra.getRoomNum());
                break;
            default:
                break;
        }
        e->setDataPos(pos);
        deadEnemies.erase(e);
    }
}

void Dungeon::Output()
{
    if (!inDungeon || goNextFloor)
        return;

    for (auto tile : tileSet)
        tile.render(camera);

    player.render(camera);

    for (auto e : enemies)
        e.render(camera);
}

bool Dungeon::LoadData()
{
    // Loading success flag
    bool success = true;

    // プレイヤーのスプライトシートのテクスチャを読み込む
    if (!player.mCharTexture.loadFromFile("assets/character.png"))
    {
        printf("Failed to load walking animation texture!\n");
        success = false;
    }
    else
    {
        // プレイヤースプライトクリップを設定する
        for (int sprite_dir = 0; sprite_dir < static_cast<int>(NO_DIRECTION); sprite_dir++)
        {
            for (int sprite_num = 0; sprite_num < ANIMATION_FRAMES; sprite_num++)
            {
                mPlayerSpriteClips.at((sprite_dir * ANIMATION_FRAMES) + sprite_num).x = SPRITE_CHAR_WIDTH * (sprite_dir * ANIMATION_FRAMES + sprite_num);
                mPlayerSpriteClips.at((sprite_dir * ANIMATION_FRAMES) + sprite_num).y = SPRITE_CHAR_HEIGHT * 0;
                mPlayerSpriteClips.at((sprite_dir * ANIMATION_FRAMES) + sprite_num).w = SPRITE_CHAR_WIDTH;
                mPlayerSpriteClips.at((sprite_dir * ANIMATION_FRAMES) + sprite_num).h = SPRITE_CHAR_HEIGHT;
            }
        }

        // 敵スプライトクリップを設定する
        for (int enemy_num = 0; enemy_num < static_cast<int>(ENEMY_TYPE_NUMBER); enemy_num++)
        {
            for (int sprite_dir = 0; sprite_dir < static_cast<int>(NO_DIRECTION); sprite_dir++)
            {
                for (int sprite_num = 0; sprite_num < ANIMATION_FRAMES; sprite_num++)
                {
                    mEnemySpriteClips.at(enemy_num).at((sprite_dir * ANIMATION_FRAMES) + sprite_num).x = SPRITE_CHAR_WIDTH * (sprite_dir * ANIMATION_FRAMES + sprite_num);
                    mEnemySpriteClips.at(enemy_num).at((sprite_dir * ANIMATION_FRAMES) + sprite_num).y = SPRITE_CHAR_HEIGHT * (enemy_num + 1);
                    mEnemySpriteClips.at(enemy_num).at((sprite_dir * ANIMATION_FRAMES) + sprite_num).w = SPRITE_CHAR_WIDTH;
                    mEnemySpriteClips.at(enemy_num).at((sprite_dir * ANIMATION_FRAMES) + sprite_num).h = SPRITE_CHAR_HEIGHT;
                }
            }
        }
    }

    player.mSpriteClips = mPlayerSpriteClips;

    // Load tile texture
    if (!gTileTexture.loadFromFile("assets/dungeon_tiles.png"))
    {
        printf("Failed to load tile set texture!\n");
        success = false;
    }

    return success;
}

void Dungeon::PlayMusic()
{
}

void Dungeon::InitDungeon()
{
    SDL_Log("InitDungeon: ダンジョンを初期化\n");
    inDungeon = true;
    goNextFloor = false;

    // 選択した方法でダンジョンを生成
    switch (dungeon_g->getNowScene())
    {
    case DUNGEON_AREA_DIVIDE:
        areaDivide.generate();
        for (int y = 0; y < FLOOR_H; y++)
            for (int x = 0; x < FLOOR_W; x++)
                floor[y][x] = areaDivide.buff[y + 1][x + 1];
        break;
    case DUNGEON_RRA:
        rra.generate();
        for (int y = 0; y < FLOOR_H; y++)
            for (int x = 0; x < FLOOR_W; x++)
                floor[y][x] = rra.buff[y + 1][x + 1];
        break;
    default:
        break;
    }

    // タイルマップを読み込み
    if (!setTiles())
        SDL_Log("InitDungeon: Failed to load tile set!");

    glm::vec2 pos;
    SDL_Log("InitDungeon: プレイヤーを初期化");
    switch (dungeon_g->getNowScene())
    {
    case DUNGEON_AREA_DIVIDE:
        pos = getRandomPos(areaDivide.getRoomNum());
        break;
    case DUNGEON_RRA:
        pos = getRandomPos(rra.getRoomNum());
        break;
    default:
        break;
    }
    SDL_Log("InitDungeon: 初期スポーン地点(%d, %d)", (int)pos.x, (int)pos.y);
    player.setDataPos(pos);

    SDL_Log("InitDungeon: 敵を初期化");
    enemies.clear();
    enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(DEKA));
    for (auto &e : enemies)
    {
        e = Enemy(static_cast<ENEMY_TYPE>(random_num(random_engine) % static_cast<int>(ENEMY_TYPE_NUMBER)));
        e.mSpriteClips = mEnemySpriteClips.at(static_cast<int>(e.getEnemyType()));

        SDL_Log("InitDungeon: 敵の位置を初期化");
        switch (dungeon_g->getNowScene())
        {
        case DUNGEON_AREA_DIVIDE:
            pos = getRandomPos(areaDivide.getRoomNum());
            break;
        case DUNGEON_RRA:
            pos = getRandomPos(rra.getRoomNum());
            break;
        default:
            break;
        }
        e.setDataPos(pos);
        SDL_Log("InitDungeon: 敵の位置(%d, %d)", (int)e.getDataPos().x, (int)e.getDataPos().y);
    }
}

void Dungeon::quit()
{
    inDungeon = true;
    goNextFloor = true;
    player.reset();
    enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(DEKA));
}

int Dungeon::isOtherPos(glm::vec2 _pos)
{
    int sameNum = 0;
    if (player.getImagePos() == _pos)
        sameNum++;
    for (auto e : enemies)
    {
        if (e.getImagePos() == _pos)
            sameNum++;
    }
    return sameNum;
}

bool Dungeon::canGetOn(glm::vec2 _pos)
{
    if ((floor[(int)_pos.y][(int)_pos.x] != FLOOR) && (floor[(int)_pos.y][(int)_pos.x] != AISLE) && (floor[(int)_pos.y][(int)_pos.x] != STEP))
        return false;
    if (isOtherPos(_pos) > 1)
        return false;
    return true;
}

glm::vec2 Dungeon::getRandomPos(int _roomCount)
{
    int roomNum = rand() % _roomCount;
    Room room = Room();
    switch (dungeon_g->getNowScene())
    {
    case DUNGEON_AREA_DIVIDE:
        room = areaDivide.getRoom(roomNum);
        break;
    case DUNGEON_RRA:
        room = rra.getRoom(roomNum);
        break;
    default:
        break;
    }
    glm::vec2 pos;
    pos.x = room.x + rand() % room.w;
    pos.y = room.y + rand() % room.h;
    while (!canGetOn(pos))
    {
        pos.x = room.x + rand() % room.w;
        pos.y = room.y + rand() % room.h;
    }
    return pos;
}

Enemy &Dungeon::whichEnemy(glm::vec2 _pos)
{
    for (auto &e : enemies)
        if ((e.getImagePos().x == _pos.x) && (e.getImagePos().y == _pos.y))
            return e;

    throw std::runtime_error("Enemy not found");
}

glm::vec2 Dungeon::getFrontPos(glm::vec2 _pos, DIRECTION _dir)
{
    glm::vec2 front(0, 0);
    switch (_dir)
    {
    case LEFT:
        front.x = -1;
        break;
    case RIGHT:
        front.x = 1;
        break;
    case UP:
        front.y = -1;
        break;
    case DOWN:
        front.y = 1;
        break;
    default:
        break;
    }
    return _pos + front;
}

bool Dungeon::setTiles()
{
    // Success flag
    bool tilesLoaded = true;

    // The tile offsets
    int x = 0, y = 0;

    // Open the map
    std::ifstream map("dungeon.map");
    std::string strMap;

    // マップが読み込めなかった場合
    if (map.fail())
    {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    }
    else
    {
        // Initialize the tiles
        for (int i = 0; i < TOTAL_TILES; ++i)
        {
            // Determines what kind of tile will be made
            int tileType = -1;

            // Read tile from map file
            map >> tileType;

            // If the was a problem in reading the map
            if (map.fail())
            {
                // Stop loading map
                printf("Error loading map: Unexpected end of file!\n");
                tilesLoaded = false;
                break;
            }

            // If the number is a valid tile number
            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
            {
                tileSet.at(i) = Tile(x, y, static_cast<CELL_TYPE>(tileType));
            }
            // If we don't recognize the tile type
            else
            {
                // Stop loading map
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }

            // Move to next tile spot
            x += TILE_W;

            // If we've gone too far
            if (x >= LEVEL_WIDTH)
            {
                // Move back
                x = 0;

                // Move to the next row
                y += TILE_H;
            }
        }

        // FIX: Clip the sprite sheet
        int mapTilesetW = 5;
        int mapTilesetH = 4;
        if (tilesLoaded)
            for (y = 0; y < mapTilesetH; y++)
                for (x = 0; x < mapTilesetW; x++)
                {
                    gTileClips[y * mapTilesetW + x].x = x * TILE_W;
                    gTileClips[y * mapTilesetW + x].y = y * TILE_H;
                    gTileClips[y * mapTilesetW + x].w = TILE_W;
                    gTileClips[y * mapTilesetW + x].h = TILE_H;
                }
    }

    // Close the file
    map.close();

    // If the map was loaded fine
    return tilesLoaded;
}

void Dungeon::updateEnemyRoute(Enemy &_enemy, GOAL_TYPE _goalType)
{
    _enemy.routeClear();
    glm::vec2 goal(0, 0);
    switch (_goalType)
    {
    case RANDOM_POS:
        while (!canGetOn(goal))
        {
            switch (dungeon_g->getNowScene())
            {
            case DUNGEON_AREA_DIVIDE:
                goal = areaDivide.getRandomFloorPos();
                break;
            case DUNGEON_RRA:
                goal = rra.getRandomFloorPos();
                break;
            default:
                break;
            }
            SDL_Log("updateEnemyRoute: RANDOM_POS = GOAL(%d, %d)", (int)goal.x, (int)goal.y);
        }
        break;
    case PLAYER_POS:
        goal = player.getImagePos();
        goal.x /= TILE_W;
        goal.x++;
        goal.y /= TILE_H;
        goal.y++;
        SDL_Log("updateEnemyRoute: PLAYER_POS = GOAL(%d, %d)", (int)goal.x, (int)goal.y);
        break;
    }
    _enemy.setGoal(floor, goal);
}
