#include "../include/GridBitmap.h"
#include "../include/Color.h"

using namespace std;

GridBitmap::GridBitmap(vector<vector<Color>> grid)
{
    this->grid = grid;
}

GridBitmap::GridBitmap()
{
}
