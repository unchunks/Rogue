#include <stdio.h>
#include <memory.h>
#include <list>
#include <queue>
#include <vector>
#include <math.h>

#include "DDefine.h"

const int Infinity = 100000;

enum EraseResult
{
	NotFound,		
	Erased,			
	CouldntErased	
};

// Map
// struct Cell
// {
// 	Cell() :
// 		X(-1),
// 		Y(-1)
// 	{
// 	}

// 	Cell(int x, int y)
// 	{
// 		X = x;
// 		Y = y;
// 	}

// 	int X;		
// 	int Y;		
// };

struct Node
{
	Node() :
		Node(0, 0)
	{
	}

	Node(int x, int y) :
		Position(x, y),
		HeuristicCost(Infinity),
		TotalCost(0)
	{
		AdjacentNodes.clear();
	}

	vec2 Position;						
	std::vector<Node*> AdjacentNodes;	
	float HeuristicCost;				
	float TotalCost;					
};

int CostTable[FLOOR_H][FLOOR_W];

// グラフ
Node Map[FLOOR_H][FLOOR_W];

bool Less(Node* a, Node* b)
{
	if (a->TotalCost < b->TotalCost) 
	{
		return true;
	}

	return false;
}

// 範囲セルチェック
bool IsCellWithinTheRange(int x, int y)
{
	if (x >= 0 &&
		x < FLOOR_W &&
		y >= 0 &&
		y < FLOOR_H)
	{
		return true;
	}

	return false;
}

// ノード作成
void CreateBuffMap()
{
    for (int y = 0; y < FLOOR_H; y++)
	{
		for (int x = 0; x < FLOOR_W; x++){
            if((floorTYPE[y][x] == FLOOR) || (floorTYPE[y][x] == AISLE)) {
                CostTable[y][x] = 1;
            } else {
                CostTable[y][x] = 0;
            }
        }
    }
	for (int y = 0; y < FLOOR_H; y++)
	{
		for (int x = 0; x < FLOOR_W; x++)
		{
			Map[y][x].Position.x = x;
			Map[y][x].Position.y = y;

			vec2 adjacent_cells[] = 
			{
				vec2(x, y - 1),
				vec2(x - 1, y),
				vec2(x + 1, y),
				vec2(x, y + 1),
			};

			// 隣接ノード追加
			for (const vec2& cell : adjacent_cells)
			{
				if (IsCellWithinTheRange(cell.x, cell.y) == true &&
					CostTable[cell.y][cell.x] == 1)
				{
					Map[y][x].AdjacentNodes.push_back(&Map[cell.y][cell.x]);
				}
			}
		}
	}
}

// コスト初期化
void InitCost(int heuristic_cost, int total_cost)
{
	for (int y = 0; y < FLOOR_H; y++)
	{
		for (int x = 0; x < FLOOR_W; x++)
		{
			Map[y][x].HeuristicCost = heuristic_cost;
			Map[y][x].TotalCost = total_cost;
		}
	}
}

float CalculateHeuristic(const Node* node, const Node* Goal)
{
	float x = fabsf(Goal->Position.x - node->Position.x);
	float y = fabsf(Goal->Position.y - node->Position.y);

	return sqrtf(x * x + y * y);
}

bool IsEqualCell(const vec2& a, const vec2& b)
{
	if (a.x == b.x &&
		a.y == b.y)
	{
		return true;
	}

	return false;
}

EraseResult EraseNode(std::list<Node*>& list, Node* new_node, float new_cost)
{
	// クローズリストチェック
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		// ノードと同じセルがあるか
		if (IsEqualCell(new_node->Position, (*itr)->Position) == true)
		{
			// コスト比較
			if (new_cost < (*itr)->TotalCost)
			{
				list.erase(itr);
				return EraseResult::Erased;
			}
			else
			{
				return EraseResult::CouldntErased;
			}
		}
	}

	return EraseResult::NotFound;
}

// オープンリストに追加
bool AddAdjacentNode(std::list<Node*>& open_list, std::list<Node*>& close_list, Node* adjacent_node, float cost)
{
	bool can_update = true;

	std::list<Node*> node_list[] =
	{
		close_list,
		open_list
	};

	for (std::list<Node*>& list : node_list)
	{
		// 既存のコストが高ければ更新
		if (EraseNode(list, adjacent_node, cost) == EraseResult::CouldntErased)
		{
			can_update = false;
		}
	}
	
	if (can_update == true)
	{
		open_list.push_back(adjacent_node);
		return true;
	}

	return false;
}

std::vector<vec2> AStar(vec2 start, vec2 goal)
{
	std::list<Node*> open_list;
	std::list<Node*> close_list;

	//const Node* start_node = &Map[start.Y][start.X];
	const Node* goal_node = &Map[goal.y][goal.x];

	// 更新ノード位置保存
	vec2 last_update_cells[FLOOR_H][FLOOR_W];

	// グラフ初期化
	InitCost(Infinity, 0);

	// スタートノード指定
	open_list.push_back(&Map[start.y][start.x]);

	int count = 0;

	// オープンリストがなくなるまで
	while (open_list.empty() == false)
	{
		count++;

		Node* search_node = (*open_list.begin());
		// 探索リストから除外
		open_list.erase(open_list.begin());

		// ゴールで終わり
		if (IsEqualCell(search_node->Position, goal) == true)
		{
			// クローズリストに最後のノード追加
			close_list.push_back(search_node);
			break;
		}

		for (Node* adjacent_node : search_node->AdjacentNodes)
		{
			// 未計算の場合
			if (adjacent_node->HeuristicCost == Infinity)
			{
				adjacent_node->HeuristicCost = CalculateHeuristic(adjacent_node, goal_node);
			}

			// ノード間コスト
			float edge_cost = CostTable[adjacent_node->Position.y][adjacent_node->Position.x];
			// 総コスト
			float node_cost = search_node->TotalCost;
			float total_cost = edge_cost + adjacent_node->HeuristicCost + node_cost;

			// ノード追加
			if (AddAdjacentNode(open_list, close_list, adjacent_node, total_cost) == true)
			{
				// 総コスト更新
				adjacent_node->TotalCost = total_cost;

				if (adjacent_node->Position.y == 0 && adjacent_node->Position.x == 2)
				{
					int xx = 0;
					xx = 100;
				}

				// 更新セル保存
				last_update_cells[adjacent_node->Position.y][adjacent_node->Position.x] = search_node->Position;
			}
		}

		bool is_add_close = true;

		// クローズリストチェック
		for (auto itr = close_list.begin(); itr != close_list.end(); itr++)
		{
			if (IsEqualCell(search_node->Position, (*itr)->Position) == true)
			{
				is_add_close = false;
				break;
			}
		}

		// ノード追加
		if (is_add_close == true)
		{
			// このノードの探索終了
			close_list.push_back(search_node);
		}

		// 昇順ソート
		open_list.sort(Less);
	}

	// 経路復元
	std::list<vec2> route_list;

	// ゴールから復元
	route_list.push_back(goal);
	while (route_list.empty() == false)
	{
		vec2 route = route_list.front();

		// スタート位置なら終了
		if (IsEqualCell(route, start) == true)
		{
			// 復元経路表示
			for (vec2& cell : route_list)
			{
				printf("x = %d y = %d\n", cell.x, cell.y);
			}
			break;
		}
		else
		{
			if (IsCellWithinTheRange(route.x, route.y) == true)
			{
				// 追加
				route_list.push_front(last_update_cells[route.y][route.x]);
			}
			else
			{
				printf("Not Found\n");
				break;
			}
		}
	}

	printf("探索回数 = %d\n", count);

	return std::vector<vec2> (++route_list.begin(), route_list.end());
}
