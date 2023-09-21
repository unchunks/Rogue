#ifndef __AREA_DIVIDE
#define __AREA_DIVIDE

#include <random> 

#include "Generator.h"

class AreaDivide : public Generator
{
public:
    void generate(int _randomNumber);
private:
    void divide(int ID);
};

#endif __AREA_DIVIDE