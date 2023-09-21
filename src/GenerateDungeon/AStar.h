#ifndef __ASTAR
#define __ASTAR

#include <glm/glm.hpp>
#include <deque>

#include "Enum.h"
#include "Const.h"

enum SEARCH_STATUS{
	SEARCH_NO_CHECK	= 0,
	SEARCH_OPEN		= 1,
	SEARCH_CLOSE	= 2,
};

typedef struct {
	int x;
	int y;
} POINT;

class MAPCELL {
public:
	MAPCELL()
	:status(2), cost(0), SearchStatus(SEARCH_NO_CHECK), parent(NO_DIRECTION)
	{}
	int status;					// 2:壁 0:床 1:ルート
	int cost;
	SEARCH_STATUS SearchStatus;	// 0:未調査 1:オープン 2:クローズ
	DIRECTION parent;			// 親の向き
};

namespace AStar{
	// マンハッタン距離を求める
	int GetDistance(int from_x, int from_y);

	// スタート位置から現在地までのコスト集計
	int BackTrace(int x, int y);

	// A*で経路探査する
	int Search(int count);

	// ゴールから逆算
	void TraceRoute(int x, int y);

	std::deque<glm::vec2> AStar(CELL_TYPE def_data[FLOOR_H][FLOOR_W], glm::vec2 _start, glm::vec2 _goal);
}
#endif __ASTAR