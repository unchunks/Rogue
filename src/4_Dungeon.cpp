#include "Scene/4_Dungeon.h"

extern SDL_Renderer *gRenderer;

Game *dungeon_g;

//マップ画像のテクスチャ
LTexture gTileTexture = LTexture();
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

//NOTE: Dungeonの関数

Dungeon::Dungeon(Game *game)
:inDungeon(true), goNextFloor(true)
{
    dungeon_g = game;
    mGame = game;
    if(!LoadData())
    {
		SDL_Log( "Failed to load media!\n" );
    }
	player.mCharTexture.setW(player.mCharTexture.getWidth() * TILE_WIDTH / SPRITE_CHAR_WIDTH);
	player.mCharTexture.setH(player.mCharTexture.getHeight() * TILE_HEIGHT / SPRITE_CHAR_HEIGHT);
	camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
}

// REVIEW: デバッグ用
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
// SDL_Log("Input\n");
    // プレイヤーのアップデート
// TODO: キーを長押しすると敵を無視して連続で動いてしまう
    // glm::vec2 front;
    playerMoved = false;

	if(player.mNowMoving)
	{
		return;
	}

	if(event.type == SDL_KEYDOWN)
	{
		player.mNowMoving = true;

		switch(event.key.keysym.sym)
		{
			case SDLK_w: playerMoved = true; player.mMovingDir = UP; break; //player.move(DIRECTION::UP);    break;
			case SDLK_a: playerMoved = true; player.mMovingDir = LEFT; break; //player.move(DIRECTION::LEFT);  break;
			case SDLK_s: playerMoved = true; player.mMovingDir = DOWN; break; //player.move(DIRECTION::DOWN);  break;
			case SDLK_d: playerMoved = true; player.mMovingDir = RIGHT; break; //player.move(DIRECTION::RIGHT); break;
			// case SDLK_k:
			// 	playerMoved = true;
			// 	front = getFrontPos(player.getPos(), player.getDir());
			// 	if(!isOtherPos(front))
			// 		break;
			// 	player.attack(whichEnemy(front));
			// break;
			case SDLK_q: inDungeon = false; break;
		}
	}
	else if(event.type == SDL_KEYUP)
	{
		player.mMovingDir = NO_DIRECTION;
	}

	// if(!canGetOn(player.getPos()))
	// {
	// 	player.back();
	// }

// 階段を登ったときの処理
    if(playerMoved) switch(dungeon_g->getNowScene())
	{
	case DUNGEON_AREA_DIVIDE: goNextFloor = (areaDivide.buff[(int)player.getPos().y+1][(int)player.getPos().x+1] == STEP); break;
	case DUNGEON_RRA: 		  goNextFloor = (	    rra.buff[(int)player.getPos().y+1][(int)player.getPos().x+1] == STEP); break;
    default: break;
    }
    if(goNextFloor) {
        return;
    }
}

void Dungeon::Update(int anim_frame)
{
// SDL_Log("Update\n");
    if(!inDungeon)
    {
std::cout << "ダンジョン脱出\n";
		quit(tileSet);
        dungeon_g->setNowScene(SCENE::HOME);
        return;
    }

    if(goNextFloor)
    {
        InitDungeon();
        return;
    }

	player.move(tileSet);
// std::cout << "(" << (int)player.getPos().x/TILE_WIDTH << ", " << (int)player.getPos().y/TILE_WIDTH << ")\n";
	player.setCamera(camera);

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
// std::cout << abs(e.getPos().x - player.getPos().x) + abs(e.getPos().y - player.getPos().y) << std::endl;
                if(abs(e.getPos().x - player.getPos().x) + abs(e.getPos().y - player.getPos().y) <= ENEMY_FIND_RANGE) {
                    e.setState(FOUND);
                    e.routeClear();
                }
// std::cout << e.getRouteSize() << " : " << e.getElapsedTurn() << std::endl;
                if((e.getRouteSize() < 1) || (e.getElapsedTurn() > ENEMY_SEARCH_INTERVAL))
				{
					switch(dungeon_g->getNowScene())
					{
					case DUNGEON_AREA_DIVIDE: e.setGoal(areaDivide.getFloor(), areaDivide.getRandomFloorPos()); break;
					case DUNGEON_RRA: 		  e.setGoal(	   rra.getFloor(), rra.getRandomFloorPos());        break;
                    default: break;
                    }
				}
                // e.walk();
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
				{
					switch(dungeon_g->getNowScene())
					{
					case DUNGEON_AREA_DIVIDE: e.setGoal(areaDivide.getFloor(), player.getPos()); break;
					case DUNGEON_RRA: 		  e.setGoal(	   rra.getFloor(), player.getPos()); break;
                    default: break;
					}
				}
                // e.walk();
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

        if(!canGetOn(e.getPos()));
            // e.back();
std::cout << "Enemy updated\n";
    }

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

void Dungeon::Output(int anim_frame)
{
// SDL_Log("Output\n");
	//Render level
	for( int i = 0; i < TOTAL_TILES; ++i )
	{
		tileSet[ i ]->render( camera );
	}
	player.render(camera, anim_frame);

    // for(int y=0; y<FLOOR_H+2; y++) {
    //     for(int x=0; x<FLOOR_W+2; x++) {
    //         std::cout << static_cast<int>(areaDivide.buff[y][x]) << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << "　　　　　  | HP |向き|\n" ;
    // std::cout << "プレイヤー: |" << std::setw(4) << player.getNowHP() << "| " << get(player.getDir()) << " |\n";
    // for(auto e: enemies) {
    // std::cout << "　　敵　　: |" << std::setw(4) << e.getNowHP() << "|\n";
    // }
}

bool Dungeon::LoadData()
{
	//Loading success flag
	bool success = true;

	//TODO: 敵のテクスチャも読み込めるように
	//プレイヤーのスプライトシートのテクスチャを読み込む
	if( !player.mCharTexture.loadFromFile( "assets/character.png" ) )
	{
		SDL_Log( "Failed to load walking animation texture!\n" );
		success = false;
	}
	else
	{
		//REVIEW: スプライトクリップを設定する
		player.mSpriteClips[ static_cast<int>(LEFT) + 0 ].x = SPRITE_CHAR_WIDTH*0;
        player.mSpriteClips[ static_cast<int>(LEFT) + 0 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ static_cast<int>(LEFT) + 0 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ static_cast<int>(LEFT) + 0 ].h = SPRITE_CHAR_HEIGHT;
		player.mSpriteClips[ static_cast<int>(LEFT) + 1 ].x = SPRITE_CHAR_WIDTH*1;
		player.mSpriteClips[ static_cast<int>(LEFT) + 1 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ static_cast<int>(LEFT) + 1 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ static_cast<int>(LEFT) + 1 ].h = SPRITE_CHAR_HEIGHT;
		player.mSpriteClips[ static_cast<int>(LEFT) + 2 ].x = SPRITE_CHAR_WIDTH*2;
		player.mSpriteClips[ static_cast<int>(LEFT) + 2 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ static_cast<int>(LEFT) + 2 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ static_cast<int>(LEFT) + 2 ].h = SPRITE_CHAR_HEIGHT;
		player.mSpriteClips[ static_cast<int>(LEFT) + 3 ].x = SPRITE_CHAR_WIDTH*3;
		player.mSpriteClips[ static_cast<int>(LEFT) + 3 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ static_cast<int>(LEFT) + 3 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ static_cast<int>(LEFT) + 3 ].h = SPRITE_CHAR_HEIGHT;

		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 0 ].x = SPRITE_CHAR_WIDTH*4;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 0 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 0 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 0 ].h = SPRITE_CHAR_HEIGHT;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 1 ].x = SPRITE_CHAR_WIDTH*5;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 1 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 1 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 1 ].h = SPRITE_CHAR_HEIGHT;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 2 ].x = SPRITE_CHAR_WIDTH*6;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 2 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 2 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 2 ].h = SPRITE_CHAR_HEIGHT;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 3 ].x = SPRITE_CHAR_WIDTH*7;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 3 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 3 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ (static_cast<int>(RIGHT) * ANIMATION_FRAMES) + 3 ].h = SPRITE_CHAR_HEIGHT;

		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 0 ].x = SPRITE_CHAR_WIDTH*8;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 0 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 0 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 0 ].h = SPRITE_CHAR_HEIGHT;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 1 ].x = SPRITE_CHAR_WIDTH*9;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 1 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 1 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 1 ].h = SPRITE_CHAR_HEIGHT;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 2 ].x = SPRITE_CHAR_WIDTH*10;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 2 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 2 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 2 ].h = SPRITE_CHAR_HEIGHT;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 3 ].x = SPRITE_CHAR_WIDTH*11;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 3 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 3 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ (static_cast<int>(UP) * ANIMATION_FRAMES) + 3 ].h = SPRITE_CHAR_HEIGHT;

		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 0 ].x = SPRITE_CHAR_WIDTH*12;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 0 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 0 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 0 ].h = SPRITE_CHAR_HEIGHT;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 1 ].x = SPRITE_CHAR_WIDTH*13;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 1 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 1 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 1 ].h = SPRITE_CHAR_HEIGHT;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 2 ].x = SPRITE_CHAR_WIDTH*14;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 2 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 2 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 2 ].h = SPRITE_CHAR_HEIGHT;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 3 ].x = SPRITE_CHAR_WIDTH*15;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 3 ].y = SPRITE_CHAR_HEIGHT*0;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 3 ].w = SPRITE_CHAR_WIDTH;
		player.mSpriteClips[ (static_cast<int>(DOWN) * ANIMATION_FRAMES) + 3 ].h = SPRITE_CHAR_HEIGHT;
	}

	//Load tile texture
	if( !gTileTexture.loadFromFile( "assets/dungeon_tiles.png" ) )
	{
		SDL_Log( "Failed to load tile set texture!\n" );
		success = false;
	}

	return success;
}

void Dungeon::PlayMusic()
{
}

void Dungeon::InitDungeon()
{
std::cout << "ダンジョンを初期化\n";
    inDungeon = true;
    goNextFloor = false;

	switch(dungeon_g->getNowScene())
	{
	case DUNGEON_AREA_DIVIDE:
		//FIX: ここでエラーが発生することがある
		areaDivide.generate();
	break;
	case DUNGEON_RRA:
		rra.generate();
	break;
    default: break;
	}

	//Load tile map
	if( !setTiles(tileSet) )
	{
		SDL_Log( "Failed to load tile set!\n" );
	}

	glm::vec2 pos;
std::cout << "プレイヤーを初期化\n";
	switch(dungeon_g->getNowScene())
	{
	case DUNGEON_AREA_DIVIDE: pos = getRandomPos(areaDivide.getRoomNum()); 	 break;
	case DUNGEON_RRA:		  pos = getRandomPos(	   	  rra.getRoomNum()); break;
    default: break;
    }
std::cout << "初期スポーン地点(" << pos.x << ", " << pos.y << ")\n";
	pos.x = pos.x*TILE_WIDTH + TILE_WIDTH/4;
	pos.y = pos.y*TILE_HEIGHT + TILE_HEIGHT/4;
    player.setPos(pos);

std::cout << "敵を初期化\n";
    enemies.clear();
    enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(DEKA));
    for(auto& e : enemies)
    {
        e = Enemy((ENEMY_TYPE)(rand() % ENEMY_TYPE_NUMBER));
    }
    for(auto& e : enemies)
    {
		switch(dungeon_g->getNowScene())
		{
		case DUNGEON_AREA_DIVIDE: pos = getRandomPos(areaDivide.getRoomNum()); break;
		case DUNGEON_RRA: 		  pos = getRandomPos(		rra.getRoomNum()); break;
        default: break;
        }
		pos.x = pos.x*TILE_WIDTH + TILE_WIDTH/4;
		pos.y = pos.y*TILE_HEIGHT + TILE_HEIGHT/4;
        e.setPos(pos);
    }
    // for(auto e : enemies)
    // {
    //     std::cout << "(" << e.getPos().x << ", " << e.getPos().y << ")\n";
    // }
}

void Dungeon::quit(Tile* tiles[])
{
    inDungeon = true;
    goNextFloor = true;
    player = Player(0, 0, PLAYER_HP, PLAYER_STR, PLAYER_VIT);
    enemies = std::vector<Enemy>(NUM_ENEMY, Enemy(DEKA));
	//Deallocate tiles
	// for( int i = 0; i < TOTAL_TILES; ++i )
	// {
	// 	if( tiles[ i ] != NULL )
	// 	{
	// 		delete tiles[ i ];
	// 		tiles[ i ] = NULL;
	// 	}
	// }

	//Free loaded images
	// player.mCharTexture.free();
	// gTileTexture.free();
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
	switch(dungeon_g->getNowScene())
	{
	case DUNGEON_AREA_DIVIDE:
		if((areaDivide.buff[(int)_pos.y+1][(int)_pos.x+1] != FLOOR)
    	&& (areaDivide.buff[(int)_pos.y+1][(int)_pos.x+1] != AISLE)
    	&& (areaDivide.buff[(int)_pos.y+1][(int)_pos.x+1] != STEP))
        	return false;
	break;
	case DUNGEON_RRA:
		if((rra.buff[(int)_pos.y+1][(int)_pos.x+1] != FLOOR)
    	&& (rra.buff[(int)_pos.y+1][(int)_pos.x+1] != AISLE)
    	&& (rra.buff[(int)_pos.y+1][(int)_pos.x+1] != STEP))
        	return false;
	break;
    default: break;
	}
    if(isOtherPos(_pos)  > 1)
        return false;
    return true;
}

glm::vec2 Dungeon::getRandomPos(int _roomCount)
{
    int roomNum = rand() % _roomCount;
	Room room = Room();
	switch(dungeon_g->getNowScene())
	{
	case DUNGEON_AREA_DIVIDE:
		room = areaDivide.getRoom(roomNum);
	break;
	case DUNGEON_RRA:
		room = rra.getRoom(roomNum);
	break;
    default: break;
	}
    glm::vec2 pos;
    pos.x = room.x + rand() % room.w;
    pos.y = room.y + rand() % room.h;
    while(!canGetOn(pos)) {
        pos.x = room.x + rand() % room.w;
        pos.y = room.y + rand() % room.h;
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

//NOTE: 独立した関数

bool setTiles( Tile* tiles[] )
{
	//Success flag
	bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //TODO: mapをファイルから読み込みではなく配列から読み込むようにする
	//Open the map
    std::ifstream map( "dungeon.map" );
	std::string strMap;

	//マップが読み込めなかった場合
    if( map.fail() )
    {
		SDL_Log( "Unable to load map file!\n" );
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
				SDL_Log( "Error loading map: Unexpected end of file!\n" );
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
				SDL_Log( "Error loading map: Invalid tile type at %d!\n", i );
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
