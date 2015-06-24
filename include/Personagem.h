#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <iostream>
#include <vector>
#include "../include/ObjEnum.h"
#include <random>
#include <chrono>
#include <algorithm>


class Personagem
{
    public:
        Personagem();
        Personagem(std::pair<int,int> matrixPosition, std::pair<float, float> screenPosition);
        Personagem(int x, int y, float sX, float sY);
        virtual ~Personagem();
        void move_me(ObjEnum *, int, int);
        void keep_moving(ObjEnum *matrix);
        std::pair<int,int> get_matrix_pos();
        std::pair<float,float> get_screen_pos();
        bool isValid();
        void set_valid(bool b);
        bool is_moving();
        void set_moving(bool b);


    protected:
        bool moving = false;
        bool valid = true;
        std::pair<float,float> targetPosition;
        std::pair<float,float> screenPosition;
        std::pair<int,int> matrixPosition;
    private:
};

#endif // PERSONAGEM_H
