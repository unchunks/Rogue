#pragma once

#include "../../Ivec2.h"
#include <deque>
#include <iostream>
#include <algorithm>

#include "Enum.h"
#include "Const.h"

namespace OSearch{
	class Node
	{
	public:
		Node()
		:parent(Ivec2(0, 0)), cost(9999)
		{
		}
		Ivec2 parent;
		int cost;
	};
	// 経路探査する
	bool Search(Ivec2 _vec);

	std::deque<Ivec2> OSearch(CELL_TYPE def_data[FLOOR_H][FLOOR_W], Ivec2 _start, Ivec2 _goal);

	extern std::deque<Ivec2> osearch_route;

	extern Node map_data[FLOOR_H][FLOOR_W];
	extern bool can_walk[FLOOR_H][FLOOR_W];

	extern Ivec2 start, goal;
}
