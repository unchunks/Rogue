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

bool Dungeon::LoadData( Tile* tiles[] )
{
	//Loading success flag
	bool success = true;

	//プレイヤーのスプライトシートのテクスチャを読み込む
	if( !gPlayerTexture.loadFromFile( "player.png" ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}
	else
	{
		//REVIEW: スプライトクリップを設定する
		player.gSpriteClips[ static_cast<int>(LEFT) + 0 ].x = SPRITE_CHAR_WIDTH*0;
		player.gSpriteClips[ static_cast<int>(LEFT) + 0 ].y = SPRITE_CHAR_HEIGHT*0;
		player.gSpriteClips[ static_cast<int>(LEFT) + 0 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ static_cast<int>(LEFT) + 0 ].h = SPRITE_CHAR_HEIGHT;
		player.gSpriteClips[ static_cast<int>(LEFT) + 1 ].x = SPRITE_CHAR_WIDTH*1;
		player.gSpriteClips[ static_cast<int>(LEFT) + 1 ].y = SPRITE_CHAR_HEIGHT*0;
		player.gSpriteClips[ static_cast<int>(LEFT) + 1 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ static_cast<int>(LEFT) + 1 ].h = SPRITE_CHAR_HEIGHT;
		player.gSpriteClips[ static_cast<int>(LEFT) + 2 ].x = SPRITE_CHAR_WIDTH*2;
		player.gSpriteClips[ static_cast<int>(LEFT) + 2 ].y = SPRITE_CHAR_HEIGHT*0;
		player.gSpriteClips[ static_cast<int>(LEFT) + 2 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ static_cast<int>(LEFT) + 2 ].h = SPRITE_CHAR_HEIGHT;
		player.gSpriteClips[ static_cast<int>(LEFT) + 3 ].x = SPRITE_CHAR_WIDTH*3;
		player.gSpriteClips[ static_cast<int>(LEFT) + 3 ].y = SPRITE_CHAR_HEIGHT*0;
		player.gSpriteClips[ static_cast<int>(LEFT) + 3 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ static_cast<int>(LEFT) + 3 ].h = SPRITE_CHAR_HEIGHT;

		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 0 ].x = SPRITE_CHAR_WIDTH*0;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 0 ].y = SPRITE_CHAR_HEIGHT*1;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 0 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 0 ].h = SPRITE_CHAR_HEIGHT;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 1 ].x = SPRITE_CHAR_WIDTH*1;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 1 ].y = SPRITE_CHAR_HEIGHT*1;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 1 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 1 ].h = SPRITE_CHAR_HEIGHT;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 2 ].x = SPRITE_CHAR_WIDTH*2;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 2 ].y = SPRITE_CHAR_HEIGHT*1;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 2 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 2 ].h = SPRITE_CHAR_HEIGHT;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 3 ].x = SPRITE_CHAR_WIDTH*3;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 3 ].y = SPRITE_CHAR_HEIGHT*1;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 3 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 3 ].h = SPRITE_CHAR_HEIGHT;

		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 0 ].x = SPRITE_CHAR_WIDTH*0;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 0 ].y = SPRITE_CHAR_HEIGHT*2;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 0 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 0 ].h = SPRITE_CHAR_HEIGHT;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 1 ].x = SPRITE_CHAR_WIDTH*1;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 1 ].y = SPRITE_CHAR_HEIGHT*2;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 1 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 1 ].h = SPRITE_CHAR_HEIGHT;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 2 ].x = SPRITE_CHAR_WIDTH*2;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 2 ].y = SPRITE_CHAR_HEIGHT*2;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 2 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 2 ].h = SPRITE_CHAR_HEIGHT;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 3 ].x = SPRITE_CHAR_WIDTH*3;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 3 ].y = SPRITE_CHAR_HEIGHT*2;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 3 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 3 ].h = SPRITE_CHAR_HEIGHT;

		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 0 ].x = SPRITE_CHAR_WIDTH*0;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 0 ].y = SPRITE_CHAR_HEIGHT*3;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 0 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 0 ].h = SPRITE_CHAR_HEIGHT;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 1 ].x = SPRITE_CHAR_WIDTH*1;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 1 ].y = SPRITE_CHAR_HEIGHT*3;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 1 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 1 ].h = SPRITE_CHAR_HEIGHT;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 2 ].x = SPRITE_CHAR_WIDTH*2;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 2 ].y = SPRITE_CHAR_HEIGHT*3;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 2 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 2 ].h = SPRITE_CHAR_HEIGHT;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 3 ].x = SPRITE_CHAR_WIDTH*3;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 3 ].y = SPRITE_CHAR_HEIGHT*3;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 3 ].w = SPRITE_CHAR_WIDTH;
		player.gSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 3 ].h = SPRITE_CHAR_HEIGHT;
	}

	//Load tile texture
	if( !gTileTexture.loadFromFile( "dungeon_tiles.png" ) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}

	//Load tile map
	if( !setTiles( tiles ) )
	{
		printf( "Failed to load tile set!\n" );
		success = false;
	}

	return success;
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



bool Dungeon::checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}


bool Dungeon::setTiles( Tile* tiles[] )
{
	//Success flag
	bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map( "dungeon.map" );

    //If the map couldn't be loaded
    if( map.fail() )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		//Initialize the tiles
		for( int i = 0; i < TOTAL_TILES; ++i )
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if( map.fail() )
			{
				//Stop loading map
				printf( "Error loading map: Unexpected end of file!\n" );
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
			{
				tiles[ i ] = new Tile( x, y, static_cast<CELL_TYPE>(tileType) );
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if( x >= LEVEL_WIDTH )
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}
		
		//Clip the sprite sheet
		if( tilesLoaded )
		{

			gTileClips[ static_cast<int>(NONE) ].x = 0;
			gTileClips[ static_cast<int>(NONE) ].y = 0;
			gTileClips[ static_cast<int>(NONE) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(NONE) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(PILLAR) ].x = TILE_WIDTH*2;
			gTileClips[ static_cast<int>(PILLAR) ].y = 0;
			gTileClips[ static_cast<int>(PILLAR) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(PILLAR) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_LEFT) ].x = 0;
			gTileClips[ static_cast<int>(WALL_LEFT) ].y = TILE_HEIGHT;
			gTileClips[ static_cast<int>(WALL_LEFT) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_LEFT) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_RIGHT) ].x = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_RIGHT) ].y = TILE_HEIGHT;
			gTileClips[ static_cast<int>(WALL_RIGHT) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_RIGHT) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_TOP) ].x = TILE_WIDTH*2;
			gTileClips[ static_cast<int>(WALL_TOP) ].y = TILE_HEIGHT;
			gTileClips[ static_cast<int>(WALL_TOP) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_TOP) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_BOTTOM) ].x = TILE_WIDTH*3;
			gTileClips[ static_cast<int>(WALL_BOTTOM) ].y = TILE_HEIGHT;
			gTileClips[ static_cast<int>(WALL_BOTTOM) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_BOTTOM) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_LEFT_TOP) ].x = 0;
			gTileClips[ static_cast<int>(WALL_LEFT_TOP) ].y = TILE_HEIGHT*2;
			gTileClips[ static_cast<int>(WALL_LEFT_TOP) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_LEFT_TOP) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_LEFT_BOTTOM) ].x = 0;
			gTileClips[ static_cast<int>(WALL_LEFT_BOTTOM) ].y = TILE_HEIGHT*3;
			gTileClips[ static_cast<int>(WALL_LEFT_BOTTOM) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_LEFT_BOTTOM) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_RIGHT_TOP) ].x = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_RIGHT_TOP) ].y = TILE_HEIGHT*2;
			gTileClips[ static_cast<int>(WALL_RIGHT_TOP) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_RIGHT_TOP) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_RIGHT_BOTTOM) ].x = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_RIGHT_BOTTOM) ].y = TILE_HEIGHT*3;
			gTileClips[ static_cast<int>(WALL_RIGHT_BOTTOM) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_RIGHT_BOTTOM) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_SIDE_LR) ].x = TILE_WIDTH*3;
			gTileClips[ static_cast<int>(WALL_SIDE_LR) ].y = 0;
			gTileClips[ static_cast<int>(WALL_SIDE_LR) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_SIDE_LR) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_SIDE_TB) ].x = TILE_WIDTH*4;
			gTileClips[ static_cast<int>(WALL_SIDE_TB) ].y = 0;
			gTileClips[ static_cast<int>(WALL_SIDE_TB) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_SIDE_TB) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_END_LEFT) ].x = TILE_WIDTH*3;
			gTileClips[ static_cast<int>(WALL_END_LEFT) ].y = TILE_HEIGHT*2;
			gTileClips[ static_cast<int>(WALL_END_LEFT) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_END_LEFT) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_END_RIGHT) ].x = TILE_WIDTH*2;
			gTileClips[ static_cast<int>(WALL_END_RIGHT) ].y = TILE_HEIGHT*2;
			gTileClips[ static_cast<int>(WALL_END_RIGHT) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_END_RIGHT) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_END_TOP) ].x = TILE_WIDTH*3;
			gTileClips[ static_cast<int>(WALL_END_TOP) ].y = TILE_HEIGHT*3;
			gTileClips[ static_cast<int>(WALL_END_TOP) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_END_TOP) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_END_BOTTOM) ].x = TILE_WIDTH*2;
			gTileClips[ static_cast<int>(WALL_END_BOTTOM) ].y = TILE_HEIGHT*3;
			gTileClips[ static_cast<int>(WALL_END_BOTTOM) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_END_BOTTOM) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(WALL_ALL) ].x = 0;
			gTileClips[ static_cast<int>(WALL_ALL) ].y = 0;
			gTileClips[ static_cast<int>(WALL_ALL) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(WALL_ALL) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(FLOOR) ].x = TILE_WIDTH;
			gTileClips[ static_cast<int>(FLOOR) ].y = 0;
			gTileClips[ static_cast<int>(FLOOR) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(FLOOR) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(AISLE) ].x = TILE_WIDTH;
			gTileClips[ static_cast<int>(AISLE) ].y = 0;
			gTileClips[ static_cast<int>(AISLE) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(AISLE) ].h = TILE_HEIGHT;

			gTileClips[ static_cast<int>(STEP) ].x = TILE_WIDTH*4;
			gTileClips[ static_cast<int>(STEP) ].y = TILE_HEIGHT;
			gTileClips[ static_cast<int>(STEP) ].w = TILE_WIDTH;
			gTileClips[ static_cast<int>(STEP) ].h = TILE_HEIGHT;
		}
	}

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}

bool Dungeon::touchesWall( SDL_Rect box, Tile* tiles[] )
{
    //Go through the tiles
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        //If the tile is a wall type tile
        if( ( tiles[ i ]->getType() != FLOOR ) && ( tiles[ i ]->getType() != AISLE ) && ( tiles[ i ]->getType() != STEP ) )
        {
            //If the collision box touches the wall tile
            if( checkCollision( box, tiles[ i ]->getBox() ) )
            {
                return true;
            }
        }
    }

    //If no wall tiles were touched
    return false;
}




