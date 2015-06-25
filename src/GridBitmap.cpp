#include "../include/GridBitmap.h"
#include "../include/ObjEnum.h"
#include <stdio.h>
#include <string.h>

GridBitmap::GridBitmap(ObjEnum grid[GRID_HEIGHT][GRID_WIDTH])
{
    memcpy(this->grid, grid, GRID_HEIGHT * GRID_WIDTH * sizeof(ObjEnum));
}

GridBitmap::GridBitmap()
{
}
