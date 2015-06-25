#include "../include/Bloco.h"
#include "../include/ObjEnum.h"

Bloco::Bloco()
{
    //ctor
}

Bloco::Bloco(std::pair<int, int> matrixPosition)
{
    this->matrixPosition = matrixPosition;
}

void Bloco::destroi()
{
    if(this->tipo != ObjEnum::BLOCODEST)
        return;
}
