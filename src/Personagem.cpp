#include "../include/Personagem.h"
#include "../include/ObjEnum.h"
//#include "../include/collision.h"


extern bool collides(float,float);

Personagem::Personagem()
{

}

Personagem::~Personagem()
{
    //dtor
}

Personagem::Personagem(Point3D posInicial)
{
    this->posicao = posInicial;

}

void Personagem::caminha(float speedX, float speedZ)
{
    this->posicao.x += FATOR_TAMANHO_MAPA*5*speedX;
    this->posicao.z += FATOR_TAMANHO_MAPA*5*speedZ;
}




