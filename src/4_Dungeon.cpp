#include "Scene/4_Dungeon.h"

extern SDL_Renderer *gRenderer;

extern std::mt19937 random_engine;
extern std::uniform_int_distribution<int> random_num;

Game *dungeon_g;

// マップ画像のテクスチャ
LTexture gTileTexture = LTexture();
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

int turn = 0;

Dungeon::Dungeon(Game *game)
    : in_dungeon(true), go_next_floor(true), nowTurn(PLAYER)
{
    dungeon_g = game;
    mGame = game;

    tileSet.resize(TOTAL_TILES, Tile(0, 0, NONE));
    mPlayerSpriteClips.resize(ANIMATION_FRAMES * static_cast<int>(NO_DIRECTION), {0, 0, 0, 0});
    enemy_sprite_clips.resize(static_cast<int>(ENEMY_TYPE_NUMBER), std::vector<SDL_Rect>(ANIMATION_FRAMES * static_cast<int>(NO_DIRECTION), {0, 0, 0, 0}));
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
    player.sprile_clips.clear();
    player.sprile_clips.shrink_to_fit();
    enemy_sprite_clips.clear();
    enemy_sprite_clips.shrink_to_fit();

    // player.mCharTexture.free(); //別の場所で解放済み?
    // gTileTexture.free();

    enemies.clear();
    enemies.shrink_to_fit();
}

void Dungeon::Input(SDL_Event event)
{
    if (!in_dungeon || go_next_floor)
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
        case SDLK_UP:
            turn++;
            player.isMoved = true;
            player.setDir(UP);
            break;
        case SDLK_a:
        case SDLK_LEFT:
            turn++;
            player.isMoved = true;
            player.setDir(LEFT);
            break;
        case SDLK_s:
        case SDLK_DOWN:
            turn++;
            player.isMoved = true;
            player.setDir(DOWN);
            break;
        case SDLK_d:
        case SDLK_RIGHT:
            turn++;
            player.isMoved = true;
            player.setDir(RIGHT);
            break;
        case SDLK_SPACE:
            turn++;
            player.healed(1);
            nowTurn = ENEMY;
            player.isMoved = false;
            for(auto &e : enemies)
            {
                e.isMoved = true;
            }
            SDL_Log("敵のターン===============================================================================");
        	break;
        case SDLK_q:
            player.isMoved = false;
            in_dungeon = false;
            go_next_floor = true;
            break;
        // NOTE: 以下デバッグ用
        case SDLK_1:    // 見た目をプレイヤーに変更
            player.isMoved = false;
            player.sprile_clips = mPlayerSpriteClips;
            break;
        case SDLK_2:    // 見た目をデカに変更
            player.isMoved = false;
            player.sprile_clips = enemy_sprite_clips[0];
            break;
        case SDLK_3:    // 見た目をグリに変更
            player.isMoved = false;
            player.sprile_clips = enemy_sprite_clips[1];
            break;
        case SDLK_4:    // 見た目をジェリフに変更
            player.isMoved = false;
            player.sprile_clips = enemy_sprite_clips[2];
            break;
        case SDLK_5:    // 見た目をヤミーに変更
            player.isMoved = false;
            player.sprile_clips = enemy_sprite_clips[3];
            break;
        case SDLK_t:    // ランダムな位置にワープ
            player.isMoved = false;
            switch (dungeon_g->getNowScene())
            {
                case DUNGEON_AREA_DIVIDE:
                    player.setDataPos(getRandomDataPos(area_divide.getRoomNum()));
                    break;
                case DUNGEON_RRA:
                    player.setDataPos(getRandomDataPos(rra.getRoomNum()));
                    break;
                default:
                    break;
            }
            break;
        case SDLK_e:    // 敵の近くにワープ
            player.isMoved = false;
            player.setDataPos(Ivec2(
                enemies.at(0).getDataPos().x,
                enemies.at(0).getDataPos().y + 2));
            break;
        default:
            break;
        }
        if(player.isMoved && turn % 3 == 0)
        {
            player.healed(1);
        }
    }
}

void Dungeon::Update()
{
    if (!in_dungeon)
    {
        quit();
        dungeon_g->setNowScene(SCENE::HOME);
        return;
    }

    if (go_next_floor)
    {
        InitDungeon();
        return;
    }

    player.setCamera(camera);

    bool all_e_on_ceneter = true;

    // プレイヤーのアップデート
    if (nowTurn == PLAYER && player.isMoved)
    {
        if (player.onTileCenter())
        {
            SDL_Log("プレイヤーのターン=========================================================================");
        }
        if( (player.onTileCenter()) && ( isOtherPos( player.getFrontDataPos() ) >= 0 ) )
        {
            player.attack(whichEnemy(player.getFrontDataPos()));
        }
        else
        {
            std::vector<Character> otherCharacters;
            for (auto enemy : enemies)
            {
                otherCharacters.push_back(enemy);
            }
            player.move(tileSet, otherCharacters);
        }

        if (player.onTileCenter())
        {
            SDL_Log("(%d, %d)", player.getDataPos().x, player.getDataPos().y);
        }

        // 階段を登ったときの処理
        go_next_floor = ((player.onTileCenter()) && (floor[(player.getDataPos().y)][(player.getDataPos().x)] == STEP));
        if (go_next_floor)
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
        all_e_on_ceneter = true;
        std::vector<Enemy*> dead_enemies;
        for (auto &e : enemies)
        {
            SDL_Log("%sのターン-------------------------------------------------------------------------", e.getName().c_str());

            if(!e.isMoved)
                continue;
            
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
GOTO_FOUND:
                    // 隣り合っている場合は攻撃
                    if (e.adjacent(player) != NO_DIRECTION)
                    {
                        e.attack(player);
                        break;
                    }

                    if(e.onTileCenter())
                    {
                        updateEnemyRoute(e, PLAYER_POS);
                    }
                    
                    e.walk(tileSet, player, enemies);
                    break;

                case ESCAPE:
                    break;
    //REVIEW: 動作確認
                case DEAD:
                    SDL_Log("DEAD");
                    dead_enemies.push_back(&e);
                    break;

                default:
                    break;
            }

            // プレイヤーのターンに移行
            if (e.onTileCenter())
            {
                SDL_Log("中央に到着したので、isMovedをfalseに");
                e.isMoved = false;
            }
            else
            {
                all_e_on_ceneter = false;
            }

        }
        if(all_e_on_ceneter)
        {
            nowTurn = PLAYER;
            while(!dead_enemies.empty())
            {
                SDL_Log("死亡した敵を別の敵としてリスポーン");
                auto *e = dead_enemies.front();
                *e = Enemy(static_cast<ENEMY_TYPE>(random_num(random_engine) % static_cast<int>(ENEMY_TYPE_NUMBER)));
                e->sprile_clips = enemy_sprite_clips.at(static_cast<int>(e->getEnemyType()));
                Ivec2 data_pos;
                switch (dungeon_g->getNowScene())
                {
                    case DUNGEON_AREA_DIVIDE:
                        data_pos = getRandomDataPos(area_divide.getRoomNum());
                        break;
                    case DUNGEON_RRA:
                        data_pos = getRandomDataPos(rra.getRoomNum());
                        break;
                    default:
                        break;
                }
                e->setDataPos(data_pos);
                dead_enemies.erase(dead_enemies.begin());
            }
            std::cout << "\n\n";
            SDL_Log("Input: 現在%dターン目", turn);
            SDL_Log("プレイヤー HP: %d", player.getNowHP());
            SDL_Log("%s HP: %d", enemies.at(0).getName().c_str(), enemies.at(0).getNowHP());
        }
    }

    if (player.getState() == DEAD)
    {
        in_dungeon = false;
        go_next_floor = true;
    }
}

void Dungeon::Output()
{
    if (!in_dungeon || go_next_floor)
        return;

    for (auto tile : tileSet)
        tile.render(camera);

    player.render(camera);

    for (auto &e : enemies)
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
                    enemy_sprite_clips.at(enemy_num).at((sprite_dir * ANIMATION_FRAMES) + sprite_num).x = SPRITE_CHAR_WIDTH * (sprite_dir * ANIMATION_FRAMES + sprite_num);
                    enemy_sprite_clips.at(enemy_num).at((sprite_dir * ANIMATION_FRAMES) + sprite_num).y = SPRITE_CHAR_HEIGHT * (enemy_num + 1);
                    enemy_sprite_clips.at(enemy_num).at((sprite_dir * ANIMATION_FRAMES) + sprite_num).w = SPRITE_CHAR_WIDTH;
                    enemy_sprite_clips.at(enemy_num).at((sprite_dir * ANIMATION_FRAMES) + sprite_num).h = SPRITE_CHAR_HEIGHT;
                }
            }
        }
    }

    player.sprile_clips = mPlayerSpriteClips;

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
    in_dungeon = true;
    go_next_floor = false;

    // 選択した方法でダンジョンを生成
    switch (dungeon_g->getNowScene())
    {
    case DUNGEON_AREA_DIVIDE:
        area_divide.generate();
        for (int y = 0; y < FLOOR_H; y++)
            for (int x = 0; x < FLOOR_W; x++)
                floor[y][x] = area_divide.buff[y + 1][x + 1];
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

    Ivec2 data_pos;
    SDL_Log("InitDungeon: プレイヤーを初期化");
    switch (dungeon_g->getNowScene())
    {
    case DUNGEON_AREA_DIVIDE:
        data_pos = getRandomDataPos(area_divide.getRoomNum());
        break;
    case DUNGEON_RRA:
        data_pos = getRandomDataPos(rra.getRoomNum());
        break;
    default:
        break;
    }
    SDL_Log("InitDungeon: 初期スポーン地点(%d, %d)", data_pos.x, data_pos.y);
    player.setDataPos(data_pos);

    SDL_Log("InitDungeon: 敵を初期化");
    enemies.clear();
    enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(DEKA));
    for (auto &e : enemies)
    {
        e = Enemy(static_cast<ENEMY_TYPE>(random_num(random_engine) % static_cast<int>(ENEMY_TYPE_NUMBER)));
        e.sprile_clips = enemy_sprite_clips.at(static_cast<int>(e.getEnemyType()));

        SDL_Log("InitDungeon: 敵の位置を初期化");
        switch (dungeon_g->getNowScene())
        {
        case DUNGEON_AREA_DIVIDE:
            data_pos = getRandomDataPos(area_divide.getRoomNum());
            break;
        case DUNGEON_RRA:
            data_pos = getRandomDataPos(rra.getRoomNum());
            break;
        default:
            break;
        }
        e.setDataPos(data_pos);
        SDL_Log("InitDungeon: 敵の位置(%d, %d)", e.getDataPos().x, e.getDataPos().y);
    }
}

void Dungeon::quit()
{
    in_dungeon = true;
    go_next_floor = true;
    player.reset();
    enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(DEKA));
}

int Dungeon::isOtherPos(Ivec2 _data_pos)
{
    int sameNum = -1;
    if (player.getDataPos() == _data_pos)
        sameNum++;
    for (auto e : enemies)
    {
        if (e.getDataPos() == _data_pos)
            sameNum++;
    }
    return sameNum;
}

bool Dungeon::canGetOn(Ivec2 _data_pos)
{
    if ((floor[_data_pos.y][_data_pos.x] != FLOOR)
     && (floor[_data_pos.y][_data_pos.x] != AISLE)
     && (floor[_data_pos.y][_data_pos.x] != STEP))
        return false;
    if (isOtherPos(_data_pos) > 0)
        return false;
    return true;
}

Ivec2 Dungeon::getRandomDataPos(int _roomCount)
{
    int roomNum = rand() % _roomCount;
    Room room = Room();
    switch (dungeon_g->getNowScene())
    {
    case DUNGEON_AREA_DIVIDE:
        room = area_divide.getRoom(roomNum);
        break;
    case DUNGEON_RRA:
        room = rra.getRoom(roomNum);
        break;
    default:
        break;
    }
    Ivec2 data_pos;
    data_pos.x = room.x + rand() % room.w;
    data_pos.y = room.y + rand() % room.h;
    while (!canGetOn(data_pos))
    {
        data_pos.x = room.x + rand() % room.w;
        data_pos.y = room.y + rand() % room.h;
    }
    return data_pos;
}

Enemy &Dungeon::whichEnemy(Ivec2 _data_pos)
{
    for (auto &e : enemies)
        if ((e.getDataPos().x == _data_pos.x) && (e.getDataPos().y == _data_pos.y))
            return e;

    throw std::runtime_error("Enemy not found");
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
    Ivec2 goal(0, 0);
    switch (_goalType)
    {
    case RANDOM_POS:
        while (!canGetOn(goal))
        {
            switch (dungeon_g->getNowScene())
            {
            case DUNGEON_AREA_DIVIDE:
                goal = area_divide.getRandomFloorDataPos();
                break;
            case DUNGEON_RRA:
                goal = rra.getRandomFloorDataPos();
                break;
            default:
                break;
            }

            SDL_Log("updateEnemyRoute: RANDOM_POS = GOAL(%d, %d)", goal.x, goal.y);

        }
        break;
    case PLAYER_POS:
        goal = player.getDataPos();

        SDL_Log("updateEnemyRoute: PLAYER_POS = GOAL(%d, %d)", goal.x, goal.y);

        break;
    }
    _enemy.setGoal(floor, goal);
}
