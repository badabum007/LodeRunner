#ifndef OBJETO_H
#define OBJETO_H

#include <iostream>
#include <vector>

class Objeto
{
    public:
        Objeto();
        virtual ~Objeto();
    protected:
        std::pair<int,int> matrixPosition;
        bool breakWhenStop = true;
        bool isValid = true;
        bool hitFlag = false;
    private:
};

#endif // OBJETO_H
