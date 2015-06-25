#include "../include/Andar.h"

Andar::Andar()
{
}

Andar::Andar(GridBitmap* sup, GridBitmap* inf)
{
    this->superior = sup;
    this->inferior = inf;
}
