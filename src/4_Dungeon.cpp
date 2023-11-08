#include "Scene/4_Dungeon.h"

#define DEBUG

extern SDL_Renderer *gRenderer;

extern std::mt19937 random_engine;
extern std::uniform_int_distribution<int> random_num;

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

	tileSet.resize( TOTAL_TILES , Tile(0, 0, NONE));
	mPlayerSpriteClips.resize( ANIMATION_FRAMES * static_cast<int>(NO_DIRECTION) , {0, 0, 0, 0} );
	mEnemySpriteClips.resize( static_cast<int>(ENEMY_TYPE_NUMBER), std::vector<SDL_Rect>( ANIMATION_FRAMES * static_cast<int>(NO_DIRECTION) , {0, 0, 0, 0} ) );
    if(!LoadData())
    {
		SDL_Log( "Failed to load media!\n" );
    }
//REVIEW: これ要るのか？
	player.mCharTexture.setW(player.mCharTexture.getWidth() * TILE_WIDTH / SPRITE_CHAR_WIDTH);
	player.mCharTexture.setH(player.mCharTexture.getHeight() * TILE_HEIGHT / SPRITE_CHAR_HEIGHT);
	camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
}

Dungeon::~Dungeon()
{
	//Deallocate tiles
	tileSet.clear();
	tileSet.shrink_to_fit();

	//Free loaded images
	player.mSpriteClips.clear();
	player.mSpriteClips.shrink_to_fit();
	mEnemySpriteClips.clear();
	mEnemySpriteClips.shrink_to_fit();

    // player.mCharTexture.free(); //別の場所で解放済み?
	// gTileTexture.free();

	enemies.clear();
	enemies.shrink_to_fit();
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
	if(!inDungeon || goNextFloor)
    {
        return;
    }

    // プレイヤーのアップデート
// TODO: キーを長押しすると敵を無視して連続で動いてしまう
    // glm::vec2 front;
	player.isMoved = false;

	// 前の行動が終わっていないときはその続きをする
	if(!player.onTileCenter())
	{
		player.isMoved = true;
	}
	else if(event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
			case SDLK_w: player.isMoved = true; player.setDir(UP); break;
			case SDLK_a: player.isMoved = true; player.setDir(LEFT); break;
			case SDLK_s: player.isMoved = true; player.setDir(DOWN); break;
			case SDLK_d: player.isMoved = true; player.setDir(RIGHT); break;
			// case SDLK_k:
			// 	player.isMoved = true;
			// 	front = getFrontPos(player.getPos(), player.getDir());
			// 	if(!isOtherPos(front))
			// 		break;
			// 	player.attack(whichEnemy(front));
			// break;
#ifdef DEBUG
			case SDLK_1: player.isMoved = false; player.mSpriteClips = mPlayerSpriteClips; break;
			case SDLK_2: player.isMoved = false; player.mSpriteClips = mEnemySpriteClips[0]; break;
			case SDLK_3: player.isMoved = false; player.mSpriteClips = mEnemySpriteClips[1]; break;
			case SDLK_t: 
				player.isMoved = false; 
				player.setPos(
					enemies.at(0).getPos().x, 
					enemies.at(0).getPos().y + TILE_HEIGHT*2
				); 
				break;
#endif
			case SDLK_q: player.isMoved = false; inDungeon = false; break;
			default: player.isMoved = false; break;
		}
	}

// 階段を登ったときの処理
    if(player.isMoved)
	{
		SDL_Log("player(%d, %d) == %s", 
			(int)player.getPos().x / TILE_HEIGHT, 
			(int)player.getPos().y / TILE_HEIGHT, 
			floor[((int)player.getPos().y / TILE_HEIGHT)+1][((int)player.getPos().x / TILE_WIDTH)+1] == STEP? "階段" : "床");
		goNextFloor = (floor[((int)player.getPos().y / TILE_HEIGHT)][((int)player.getPos().x / TILE_WIDTH)] == STEP);
	}
    if(goNextFloor) {
        return;
    }
}

void Dungeon::Update()
{
    if(!inDungeon)
    {
std::cout << "ダンジョン脱出\n";
		quit();
        dungeon_g->setNowScene(SCENE::HOME);
        return;
    }

    if(goNextFloor)
    {
std::cout << "次の階へ移動\n";
        InitDungeon();
        return;
    }

	if(player.isMoved)
	{
		std::vector<Character> otherCharacters;
		for(auto enemy : enemies)
		{
			otherCharacters.push_back(enemy);
		}
		player.move(tileSet, otherCharacters);
	}
	player.setCamera(camera);

// 敵のアップデート
    glm::vec2 front;
    if(player.isMoved)
	{
std::cout << "敵移動\n";
		for(auto& e : enemies)
		{
			switch(e.getState())
			{
				case SEARCH:
std::cout << "case SEARCH\n";
					if(abs(e.getPos().x - player.getPos().x)/TILE_WIDTH + abs(e.getPos().y - player.getPos().y)/TILE_HEIGHT <= ENEMY_FIND_RANGE) {
						e.setState(FOUND);
						e.routeClear();
					}
					if(e.getRouteSize() < 1 || (e.getElapsedTurn() > ENEMY_SEARCH_INTERVAL))
						updateEnemyRoute(e, RANDOM_POS);
					e.walk(tileSet, player, enemies);
				break;
				case FOUND:
std::cout << "case FOUND\n";
					if(abs(e.getPos().x - player.getPos().x)/TILE_WIDTH + abs(e.getPos().y - player.getPos().y)/TILE_HEIGHT > ENEMY_FIND_RANGE)
						e.setState(SEARCH);
					front = getFrontPos(e.getPos(), e.getDir());
					// 攻撃できるとき
					if(player.getPos() == front) {
						e.attack(player);
						break;
					}
					// 攻撃できないとき
					else if((e.getRouteSize() < 1) || (e.getElapsedTurn() > ENEMY_SEARCH_INTERVAL))
						updateEnemyRoute(e, PLAYER_POS);
					e.walk(tileSet, player, enemies);
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
    	}
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

void Dungeon::Output()
{
	if(!inDungeon || goNextFloor)
    {
        return;
    }

	//Render level
	for(auto tile : tileSet)
	{
		tile.render( camera );
	}
	player.render(camera);
	for(auto e : enemies)
	{
		e.render(camera);
	}
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
	else//Character.hにcharacter.pngを読み込ませる。その後enemySpriteClipsに座標情報を入力してそのポインタを各キャラクターに渡す.
	{
		//プレイヤースプライトクリップを設定する
		for(int sprite_dir = 0; sprite_dir < static_cast<int>(NO_DIRECTION); sprite_dir++)
		{
			for(int sprite_num = 0; sprite_num < ANIMATION_FRAMES; sprite_num++)
			{
				mPlayerSpriteClips.at( ( sprite_dir * ANIMATION_FRAMES ) + sprite_num ).x = SPRITE_CHAR_WIDTH * ( sprite_dir * ANIMATION_FRAMES + sprite_num );
				mPlayerSpriteClips.at( ( sprite_dir * ANIMATION_FRAMES ) + sprite_num ).y = SPRITE_CHAR_HEIGHT*0;
				mPlayerSpriteClips.at( ( sprite_dir * ANIMATION_FRAMES ) + sprite_num ).w = SPRITE_CHAR_WIDTH;
				mPlayerSpriteClips.at( ( sprite_dir * ANIMATION_FRAMES ) + sprite_num ).h = SPRITE_CHAR_HEIGHT;
			}
		}
		
		//敵スプライトクリップを設定する
		for(int enemy_num = 0; enemy_num < static_cast<int>(ENEMY_TYPE_NUMBER); enemy_num++)
		{
			for(int sprite_dir = 0; sprite_dir < static_cast<int>(NO_DIRECTION); sprite_dir++)
			{
				for(int sprite_num = 0; sprite_num < ANIMATION_FRAMES; sprite_num++)
				{
					mEnemySpriteClips.at( enemy_num ).at( ( sprite_dir * ANIMATION_FRAMES ) + sprite_num ).x = SPRITE_CHAR_WIDTH * ( sprite_dir * ANIMATION_FRAMES + sprite_num );
					mEnemySpriteClips.at( enemy_num ).at( ( sprite_dir * ANIMATION_FRAMES ) + sprite_num ).y = SPRITE_CHAR_HEIGHT * ( enemy_num + 1 );
					mEnemySpriteClips.at( enemy_num ).at( ( sprite_dir * ANIMATION_FRAMES ) + sprite_num ).w = SPRITE_CHAR_WIDTH;
					mEnemySpriteClips.at( enemy_num ).at( ( sprite_dir * ANIMATION_FRAMES ) + sprite_num ).h = SPRITE_CHAR_HEIGHT;
				}
			}
		}
	}

	player.mSpriteClips = mPlayerSpriteClips;

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
		areaDivide.generate();
		for(int y=0; y<FLOOR_H; y++)
		{
			for(int x=0; x<FLOOR_W; x++)
			{
				floor[y][x] = areaDivide.buff[y+1][x+1];
			}
		}
	break;
	case DUNGEON_RRA:
		rra.generate();
		for(int y=0; y<FLOOR_H; y++)
		{
			for(int x=0; x<FLOOR_W; x++)
			{
				floor[y][x] = rra.buff[y+1][x+1];
			}
		}
	break;
    default: break;
	}

	//Load tile map
	if( !setTiles() )
	{
		SDL_Log( "Failed to load tile set!\n" );
	}

	glm::vec2 pos;
std::cout << "プレイヤーを初期化\n";
	switch(dungeon_g->getNowScene())
	{
	case DUNGEON_AREA_DIVIDE: pos = getRandomPos(areaDivide.getRoomNum()); break;
	case DUNGEON_RRA:		  pos = getRandomPos(	    rra.getRoomNum()); break;
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
        e = Enemy( static_cast<ENEMY_TYPE>( random_num( random_engine ) % static_cast<int>( ENEMY_TYPE_NUMBER ) ) );
SDL_Log("%d :: ", (int)e.getType());
		e.mSpriteClips = mEnemySpriteClips.at( static_cast<int>( e.getType() ) );
SDL_Log("X: %d, Y: %d, W: %d, H: %d\n", e.mSpriteClips.at( 0 ).x, e.mSpriteClips.at( 0 ).y, e.mSpriteClips.at( 0 ).w, e.mSpriteClips.at( 0 ).h);

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
	if((floor[(int)_pos.y][(int)_pos.x] != FLOOR)
	&& (floor[(int)_pos.y][(int)_pos.x] != AISLE)
	&& (floor[(int)_pos.y][(int)_pos.x] != STEP))
		return false;
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

bool Dungeon::setTiles()
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
				tileSet.at(i) = Tile( x, y, static_cast<CELL_TYPE>(tileType) );
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

void Dungeon::updateEnemyRoute(Enemy& _enemy, GOAL_TYPE _goleType)
{
	glm::vec2 goal;
	switch(_goleType)
	{
		case RANDOM_POS: 
			switch(dungeon_g->getNowScene())
			{
			case DUNGEON_AREA_DIVIDE: goal =  areaDivide.getRandomFloorPos(); break;
			case DUNGEON_RRA: 		  goal =  rra.getRandomFloorPos();        break;
			default: break;
			}
			break;
		case PLAYER_POS: goal = player.getPos(); break;
	}
	_enemy.setGoal(floor, goal);
}