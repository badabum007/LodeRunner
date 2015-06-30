#include "../include/IAController.h"
#include "../include/Personagem.h"
#include <random>
#include <windows.h>
#define MIN 50
#define MAX 100
#define MAX_ITERACOES 50
#define MIN_ITERACOES 0


float RandomFloat(float min, float max)
{
    // this  function assumes max > min, you may want
    // more robust error checking for a non-debug build
    float random = ((float) rand()) / (float) RAND_MAX;

    // generate (in your case) a float between 0 and (4.5-.78)
    // then add .78, giving you a float between .78 and 4.5
    float range = max - min;
    return (random*range) + min;
}

int RandomInt(int min, int max)
{
    return min + (rand() % (int)(max - min + 1));
}

bool isValid(float x, float y)
{
    if(x > 15 || x <= 0 || y > 15 || y <= 0)
        return false;
    return true;
}

void moveInimigo(Personagem* principal, Personagem* inimigo)
{
    float speedX,speedZ;

    if(inimigo->iteracoes > MAX_ITERACOES)
    {
    //    std::cout << inimigo->iteracoes;
        int sinal = RandomInt(0,1);
        inimigo->speedX = RandomInt(MIN,MAX) * sinal;
        inimigo->speedZ = RandomInt(MIN,MAX) * (1-sinal);

         std::cout << (inimigo->speedX==0 ^ inimigo->speedZ == 0) << std::endl;
        sinal = RandomInt(0,1);
        if(sinal==0)
            sinal = -1;
        inimigo->speedX *= sinal;
        sinal = RandomInt(0,1);
        if(sinal==0)
            sinal = -1;
        inimigo->speedZ *= sinal;

        inimigo->iteracoes = RandomInt(MIN_ITERACOES,MAX_ITERACOES);
    }
    else
    {

        if(isValid(inimigo->posicao.x + (FATOR_TAMANHO_MAPA*5*inimigo->speedX/10000), inimigo->posicao.z + (FATOR_TAMANHO_MAPA*5*inimigo->speedZ/10000)))
            inimigo->caminha(inimigo->speedX/10000 , inimigo->speedZ/10000);
        inimigo->iteracoes += 1;
    }
}





