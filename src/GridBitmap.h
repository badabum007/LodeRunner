#ifndef GRIDBITMAP_H
#define GRIDBITMAP_H

using namespace std;


#include "Color.h"
#include "vector"
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
        GridBitmap(vector<GridBitmap> grid);


        vector<GridBitmap> grid;
    protected:
    private:
};

#endif // GRIDBITMAP_H
