#include "wall.h"

Wall::Wall(int a, int b)
    : Cell(a, b){ }

int Wall::getType(){
    return 1;
}

char Wall::getSymbol(){
    return '0';
}