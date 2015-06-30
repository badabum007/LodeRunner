#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <iostream>
#include <vector>
#include "../include/ObjEnum.h"
#include "Point3D.h"
#include <random>
#include <chrono>
#include <algorithm>

#define FATOR_TAMANHO_MAPA 4

class Personagem
{
    public:
        Personagem();
        Personagem(Point3D);
        virtual ~Personagem();
        void caminha(float, float);
        void keep_moving(ObjEnum *matrix);
        bool is_moving();
        bool subindoEscada = false;
        int andarAtual = 0;
        bool descendoEscada = false;
        bool moving = false;
        bool valid = true;
        bool caindo = false;
        float speedX = 0.0f;
        float speedZ = 0.0f;
        int iteracoes=5000000;

        std::pair<float,float> targetPosition;
        Point3D posicao;

    protected:

    private:
};

#endif // PERSONAGEM_H
