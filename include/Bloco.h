#ifndef BLOCO_H
#define BLOCO_H

#include <iostream>
#include <vector>
#include "ObjEnum.h"



class Bloco
{
    public:
        Bloco();
        Bloco(std::pair<int,int> matrixPosition);
        void destroi();


    protected:
    private:
        std::pair<int,int> matrixPosition;
        ObjEnum tipo;
        bool isDestroying;
};

#endif // BLOCO_H
