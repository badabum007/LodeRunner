#ifndef GRIDBITMAP_H
#define GRIDBITMAP_H




#include "Color.h"
#include <vector>
#include "ObjEnum.h"

#define GRID_HEIGHT 16
#define GRID_WIDTH 16



/*enum class objColor
{
    Principal,
    Inimigo,
    BlocoDestrutivel,
    BlocoIndestrutivel,
    Ouro,
    Escada,
    Vazio

};*/

class GridBitmap
{
public:
        GridBitmap();
        GridBitmap(ObjEnum grid[GRID_HEIGHT][GRID_WIDTH]);


        ObjEnum grid[GRID_HEIGHT][GRID_WIDTH] ;
    protected:
    private:
};

#endif // GRIDBITMAP_H
