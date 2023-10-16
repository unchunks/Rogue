#pragma once

#include <random> 

#include "Generator.h"

class AreaDivide : public Generator
{
public:
    void generate(int _randomNumber);
private:
    void divide(int ID);
};
