#pragma once

#include <iostream>

#include "src/GenerateDungeon/Enum.h"

class Ivec2
{
public:
    Ivec2()
    {
        x = 0;
        y = 0;
    }
    Ivec2(const Ivec2& _vec)
    {
        x = _vec.x;
        y = _vec.y;
    }
    Ivec2(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    ~Ivec2()
    {
    }

    Ivec2 operator+(const Ivec2& _vec) const
    {
        return Ivec2(x + _vec.x, y + _vec.y);
    }
    Ivec2 operator-(const Ivec2& _vec) const
    {
        return Ivec2(x - _vec.x, y - _vec.y);
    }
    Ivec2 operator*(const Ivec2& _vec) const
    {
        return Ivec2(x * _vec.x, y * _vec.y);
    }
    Ivec2 operator/(const Ivec2& _vec) const
    {
        if (_vec.x == 0 || _vec.y == 0) {
            std::cerr << "Error: Division by zero." << std::endl;
        }
        return Ivec2(x / _vec.x, y / _vec.y);
    }

    bool operator==(const Ivec2& _vec) const
    {
        return (x == _vec.x) && (y == _vec.y);
    }
    bool operator!=(const Ivec2& _vec) const
    {
        return !(*this == _vec);
    }

    Ivec2 left()
    {
        return Ivec2(x - 1, y);
    }
    Ivec2 right()
    {
        return Ivec2(x + 1, y);
    }
    Ivec2 up()
    {
        return Ivec2(x, y - 1);
    }
    Ivec2 down()
    {
        return Ivec2(x, y + 1);
    }
    /// @brief 前方の座標を返す
    /// @param _dir 
    /// @return NO_DIRECTIONの場合は現在地を返す
    Ivec2 front(DIRECTION _dir)
    {
        switch(_dir)
        {
            case LEFT:  return Ivec2(x, y); break;
            case RIGHT: return Ivec2(x, y); break;
            case UP:    return Ivec2(x, y); break;
            case DOWN:  return Ivec2(x, y); break;
            case NO_DIRECTION:  return Ivec2(x, y); break;
        }
        return Ivec2(x, y);
    }
    
    int x, y;
private:
};

