#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

class Color
{
    public:
        Color();
        Color(uint8_t R, uint8_t G, uint8_t B);
        uint8_t GetR() { return R; }
        uint8_t GetG() { return G; }
        uint8_t GetB() { return B; }
    protected:
    private:
        uint8_t R;
        uint8_t G;
        uint8_t B;
};

#endif // COLOR_H
