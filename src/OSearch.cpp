#include "GenerateDungeon/OSearch.h"

namespace OSearch
{
    std::deque<Ivec2> osearch_route = std::deque<Ivec2>();

	Node map_data[FLOOR_H][FLOOR_W];
    bool can_walk[FLOOR_H][FLOOR_W];

    Ivec2 start, goal;
}

std::deque<Ivec2> OSearch::OSearch(CELL_TYPE def_data[FLOOR_H][FLOOR_W], Ivec2 _start, Ivec2 _goal)
{
    if(_start == _goal)
        return osearch_route;
    
    start = _start;
    goal  = _goal;

	// マップ変換
    for(int y = 0; y < FLOOR_H; y++)
    {
		for(int x = 0; x < FLOOR_W; x++)
        {
            if(x == start.x && y == start.y)
            {
                map_data[y][x].cost = 0;
                map_data[y][x].parent = start;
                can_walk[y][x] = true;
            }
            else if((def_data[y][x] == FLOOR) || (def_data[y][x] == AISLE) || (def_data[y][x] == STEP))
            {
                can_walk[y][x] = true;
            }
            else 
            {
                can_walk[y][x] = false;
            }
		}
	}

    std::cout << "探索前\n";
    for(int y = 0; y < FLOOR_H; y++)
    {
		for(int x = 0; x < FLOOR_W; x++)
        {
            if(x == start.x && y == start.y)
            {
                std::cout << "S";
            }
            else if(x == goal.x && y == goal.y)
            {
                std::cout << "G";
            }
            else if(can_walk[y][x])
            {
                std::cout << " ";
            }
            else
            {
                std::cout << "W";
            }
        }
        std::cout << std::endl;
    }

    Search(_start);

    std::cout << "探索後\n";
    for(int y = 0; y < FLOOR_H; y++)
    {
		for(int x = 0; x < FLOOR_W; x++)
        {
            if(x == start.x && y == start.y)
            {
                std::cout << "S";
            }
            else if(x == goal.x && y == goal.y)
            {
                std::cout << "G";
            }
            else if(std::find(osearch_route.begin(), osearch_route.end(), Ivec2(x, y)) != osearch_route.end())
            {
                std::cout << "*";
            }
            else if(can_walk[y][x])
            {
                std::cout << "W";
            }
        }
        std::cout << std::endl;
    }
	
	return osearch_route;
}

bool OSearch::Search(Ivec2 _vec)
{
    // ゴールだったらtrueを返す
    if(_vec == goal)
    {
        return true;
    }

    // 移動可能（床）かつ親でないなら
    if(can_walk[_vec.left().y][_vec.left().x] && _vec.left() != map_data[_vec.y][_vec.x].parent)
    {
        // 最小コストを更新
        if(map_data[_vec.left().y][_vec.left().x].cost > map_data[_vec.y][_vec.x].cost + 1)
        {
            map_data[_vec.left().y][_vec.left().x].cost = map_data[_vec.y][_vec.x].cost + 1;
            // 親を設定
            map_data[_vec.left().y][_vec.left().x].parent = _vec;
        }
        // 再帰してゴールが見つかったらルートに追加
        if(Search(_vec.left()))
        {
            osearch_route.push_back(Ivec2(_vec.left()));
            return true;
        }
    }
    if(can_walk[_vec.right().y][_vec.right().x] && _vec.right() != map_data[_vec.y][_vec.x].parent)
    {
        if(map_data[_vec.right().y][_vec.right().x].cost > map_data[_vec.y][_vec.x].cost + 1)
        {
            map_data[_vec.right().y][_vec.right().x].cost = map_data[_vec.y][_vec.x].cost + 1;
            map_data[_vec.right().y][_vec.right().x].parent = _vec;
        }
        if(Search(_vec.right()))
        {
            osearch_route.push_back(Ivec2(_vec.right()));
            return true;
        }
    }
    if(can_walk[_vec.up().y][_vec.up().x] && _vec.up() != map_data[_vec.y][_vec.x].parent)
    {
        if(map_data[_vec.up().y][_vec.up().x].cost > map_data[_vec.y][_vec.x].cost + 1)
        {
            map_data[_vec.up().y][_vec.up().x].cost = map_data[_vec.y][_vec.x].cost + 1;
            map_data[_vec.up().y][_vec.up().x].parent = _vec;
        }
        if(Search(_vec.up()))
        {
            osearch_route.push_back(Ivec2(_vec.up()));
            return true;
        }
    }
    if(can_walk[_vec.down().y][_vec.down().x] && _vec.down() != map_data[_vec.y][_vec.x].parent)
    {
        if(map_data[_vec.down().y][_vec.down().x].cost > map_data[_vec.y][_vec.x].cost + 1)
        {
            map_data[_vec.down().y][_vec.down().x].cost = map_data[_vec.y][_vec.x].cost + 1;
            map_data[_vec.down().y][_vec.down().x].parent = _vec;
        }
        if(Search(_vec.down()))
        {
            osearch_route.push_back(Ivec2(_vec.down()));
            return true;
        }
    }
    
    return false;
}

