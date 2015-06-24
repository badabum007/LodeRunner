#include "../include/AndarBitmap.h"

AndarBitmap::AndarBitmap()
{
}

AndarBitmap::AndarBitmap(GridBitmap* sup, GridBitmap* inf)
{
    this->superior = sup;
    this->inferior = inf;
}
