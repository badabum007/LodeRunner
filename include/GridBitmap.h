#ifndef GRIDBITMAP_H
#define GRIDBITMAP_H




#include "Color.h"
#include "vector"
#include "Color.h"
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
        GridBitmap(std::vector<std::vector<Color>> grid);


        std::vector<std::vector<Color>> grid;
    protected:
    private:
};

#endif // GRIDBITMAP_H
