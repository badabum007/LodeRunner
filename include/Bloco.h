#ifndef BLOCO_H
#define BLOCO_H

#include <iostream>
#include <vector>
#include <tuple>
#include "ObjEnum.h"



class Bloco
{
    public:
        Bloco();
        Bloco(std::tuple<int,int,int> matrixPosition);
        void destroi();
        std::tuple<int,int,int> matrixPosition;
        std::tuple<int,int,int> getMatrixPosition();
        ObjEnum tipo;
        bool isDestroying = false;
        bool destroyied = false;

    protected:
    private:

};

#endif // BLOCO_H
