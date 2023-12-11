#pragma once

#include <deque>

#include "../../Ivec2.h"

#include "Enum.h"
#include "Const.h"

namespace AStar{
	// マンハッタン距離を求める
	int GetDistance(int from_x, int from_y);

	// スタート位置から現在地までのコスト集計
	int BackTrace(int x, int y);

	// A*で経路探査する
	int Search();

	// ゴールから逆算
	void TraceRoute(int x, int y);

	std::deque<Ivec2> AStar(CELL_TYPE def_data[FLOOR_H][FLOOR_W], Ivec2 _start, Ivec2 _goal);

	// 4方向のベクトル設定
	extern Ivec2 CheckMatrix[4];

	extern MAPCELL astar_map_data[FLOOR_H][FLOOR_W];
	extern Ivec2 start, goal;
	extern std::deque<Ivec2> astar_route;
}
