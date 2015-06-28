#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

class Color
{

    public:
        Color();
        Color(uint8_t R, uint8_t G, uint8_t B);
        uint8_t R;
        uint8_t G;
        uint8_t B;

        bool operator==(Color* rhs) const
        {
            if(this->R == rhs->R && this->G == rhs->G && this->B == rhs->B)
                return true;
            return false;
        }


    protected:
    private:

};

static Color* BLOCOINDEST_COLOR = new Color(0,0,0); //preto

static Color* PRINCIPAL_COLOR = new Color(0,0,255); //azul

static Color* INIMIGO_COLOR = new Color(255,0,0); //vermelho

static Color* BLOCODEST_COLOR = new Color(128,128,128); //cinza

static Color* OURO_COLOR = new Color(255,255,0); //amarelo

static Color* ESCADASOBE_COLOR = new Color(0,255,0); //verde

static Color* ESCADADESCE_COLOR = new Color(0,128,0); //verde

static Color* VAZIO_COLOR = new Color(255,255,255); //branco

#endif // COLOR_H
