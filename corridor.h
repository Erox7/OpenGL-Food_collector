#ifndef corridor_h
#define corridor_h

#include "cell.h"

class Corridor : public Cell{
 public:
	Corridor(int, int);
    CellType getType();
    char getSymbol();
};
#endif
