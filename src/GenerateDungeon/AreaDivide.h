#pragma once

#include "GenerateDungeon/Generator.h"

class AreaDivide : public Generator
{
public:
    void generate();

private:
    void divide(int ID);

};
