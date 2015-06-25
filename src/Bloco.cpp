#include "../include/Bloco.h"
#include "../include/ObjEnum.h"

Bloco::Bloco()
{
    //ctor
}

Bloco::Bloco(std::tuple<int, int, int> matrixPosition)
{
    this->matrixPosition = matrixPosition;
}

void Bloco::destroi()
{
    if(this->tipo != ObjEnum::BLOCODEST)
        return;
}

std::tuple<int,int,int> Bloco::getMatrixPosition()
{
    return this->matrixPosition;
}
