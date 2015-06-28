#ifndef BLOCO_H
#define BLOCO_H

#include <iostream>
#include <vector>
#include <tuple>
#include <ctime>
#include "ObjEnum.h"

#define TEMPO_BLOCO_DESTRUIDO 4000


class Bloco
{
    public:
        Bloco();
        Bloco(std::tuple<int,int,int> matrixPosition);
        void destroi();
        void reconstroi();
        std::tuple<int,int,int> matrixPosition;
        std::tuple<int,int,int> getMatrixPosition();
        ObjEnum tipo;
        bool isDestroying = false;
        bool destroyied = false;
        time_t tempoInicioDest;

    protected:
    private:

};

#endif // BLOCO_H
