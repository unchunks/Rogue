#include <iostream>
#include "GenerateDungeon/AStar.h"

namespace AStar
{
	Ivec2 CheckMatrix[4] = {
        Ivec2(-1, 0),  // 左
        Ivec2(1, 0),   // 右
        Ivec2(0, -1),  // 上
        Ivec2(0, 1)    // 下
    };

	MAPCELL astar_map_data[FLOOR_H][FLOOR_W];
	Ivec2 start, goal;
	std::deque<Ivec2> astar_route;
}

int depth = 0;

// マンハッタン距離を求める
int AStar::GetDistance(int from_x, int from_y)
{
	int dis = abs(from_x - goal.x) + abs(from_y - goal.y);
	return dis;
}

// スタート位置から現在地までのコスト集計
int AStar::BackTrace(int x, int y)
{
	if(x == start.x && y == start.y) return 0;

	int parent_way = static_cast<int>(astar_map_data[y][x].parent);
	return BackTrace(
		x + CheckMatrix[parent_way].x,
		y + CheckMatrix[parent_way].y
	) + 1;
}

// A*で経路探査する
int AStar::Search()
{
    depth++;
#ifdef __DEBUG_
    std::cout << "現在の深さ: " << depth << std::endl;
#endif
	// 検索中のノード、X・Y座標、実コスト
	MAPCELL *n = NULL;
	int CX = 0;
	int CY = 0;
	int BackCost = 0;

	// コストが最小のオープンノードを探す
	int cost_min = 9999;
	for(int y = 0; y < FLOOR_H; y++){
		for(int x = 0; x < FLOOR_W; x++){
			if(astar_map_data[y][x].SearchStatus != SEARCH_OPEN)continue;	// オープンでなければスキップ
			int cost = GetDistance(x,y);
			if(cost > cost_min)continue;	// 今よりゴールに遠ければスキップ
			
			cost_min = cost;
			n = &astar_map_data[y][x];
			CX = x;
			CY = y;

		}
	}

	// オープンノードがなければ終了(ゴールが見つからない)
	if(n == NULL) return -9;

	// ノードをクローズする
	n->SearchStatus = SEARCH_CLOSE;

	BackCost = BackTrace(CX, CY);

	// 4方向の移動先のコスト計算
	for(int i = 0; i < 4; i++){
		// 計算中の座標
		int check_x = CX + CheckMatrix[i].x;
		int check_y = CY + CheckMatrix[i].y;

		// マップ外でスキップ
		if(check_x < 0) continue;
		if(check_y < 0) continue;
		if(check_x >= FLOOR_W ) continue;
		if(check_y >= FLOOR_H) continue;

		// 通れないところをよける
		if(astar_map_data[check_y][check_x].status == 2) continue;

		int estimate_cost = BackCost + GetDistance(check_x, check_y) + 1;
		MAPCELL *cell = &astar_map_data[check_y][check_x];

		if(astar_map_data[check_y][check_x].SearchStatus == SEARCH_NO_CHECK){	// 計算中の座標が未チェック
			cell->parent 	   = (DIRECTION)(int)(-1.33 * i*i*i + 6 * i*i + -5.67 * i + 1);
			cell->SearchStatus = SEARCH_OPEN;

		}else if(estimate_cost < cell->cost){	// 計算中の座標のコストが更新可能
			cell->parent       = (DIRECTION)(int)(-1.33 * i*i*i + 6 * i*i + -5.67 * i + 1);
			cell->cost         = estimate_cost;
			cell->SearchStatus = SEARCH_OPEN;
		}
	}

	// 見つかったら探索終了
	if(CX == goal.x && CY == goal.y){
		return -1;
	}

    depth--;
	return Search();
}

// ゴールから逆算
void AStar::TraceRoute(int x, int y)
{
	Ivec2 *parent_way = &CheckMatrix[astar_map_data[y][x].parent];
	astar_route.push_front(Ivec2(x, y));
	if((x == start.x) && (y == start.y))
		return;
	TraceRoute(x + parent_way->x, y + parent_way->y);
}


std::deque<Ivec2> AStar::AStar(CELL_TYPE def_data[FLOOR_H][FLOOR_W], Ivec2 _start, Ivec2 _goal)
{
	astar_route.clear();
	start = _start;
	goal = _goal;
	for(int y = 0; y < FLOOR_H; y++)
	{
		for(int x = 0; x < FLOOR_W; x++)
		{
			astar_map_data[y][x] = MAPCELL();
		}
	}
	// マップ変換
    for(int y = 0; y < FLOOR_H; y++){
		for(int x = 0; x < FLOOR_W; x++){
            if((def_data[y][x] == FLOOR) || (def_data[y][x] == AISLE) || (def_data[y][x] == STEP))
                astar_map_data[y][x].status = 0;	// 床
            else 
                astar_map_data[y][x].status = 2;	// 壁

			// if(x == goal.x && y == goal.y)
			// 	std::cout << "G ";
			// else if(x == start.x && y == start.y)
			// 	std::cout << "S ";
			// else
			// 	std::cout << ((astar_map_data[y][x].status==0)? " " : "W") << " ";
		}
		// std::cout << "\n";
	}
	// 開始位置をオープンに
    astar_map_data[start.y][start.x].SearchStatus = SEARCH_OPEN;
    Search();
#ifdef __DEBUG_
	std::cout << "トレース開始\n";
#endif
	TraceRoute(goal.x, goal.y);
#ifdef __DEBUG_
	std::cout << "トレース終了\n";
#endif
	return astar_route;
}
