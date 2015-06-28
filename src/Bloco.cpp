#include "../include/Bloco.h"
#include "../include/ObjEnum.h"
#include <ctime>
#include <iostream>

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
   switch(this->tipo)
    {
        case ObjEnum::BLOCODEST:
            this->destroyied = true;
            time(&(this->tempoInicioDest));
            this->reconstroi();
            break;
        default: break;
    }



}

void Bloco::reconstroi()
{
    time_t now;
    time(&now);

    //std::cout << difftime(now, this->tempoInicioDest) << std::endl;
    if(difftime(now, this->tempoInicioDest) > TEMPO_BLOCO_DESTRUIDO)
    {
        //std::cout << "spwanando";
        this->destroyied = false;
    }

}

std::tuple<int,int,int> Bloco::getMatrixPosition()
{
    return this->matrixPosition;
}
