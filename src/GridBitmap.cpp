#include "../include/GridBitmap.h"
#include "../include/Color.h"


GridBitmap::GridBitmap(std::vector<std::vector<Color>> grid)
{
    this->grid = grid;
}

GridBitmap::GridBitmap()
{
}
