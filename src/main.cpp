/**
Laboratorio FCG - Pratica 4
Ultima modificacao: 07/05/2014 - Acompanha bibliotecas: 3DObject.cpp(.h), bitmap.c(.h), glm.cpp,
glmimg.cpp, Texture.cpp(Texture.h)

Implementa a movimentação simples de uma camera em primeira pessoa para um personagem que anda
sobre um plano. Utiliza a biblioteca bitmap para carregamento simples de texturas, glm + 3DObject
para modelos OBJ e glmimg + Texture para aplicacao de texturas TGA sobre os modelos carregados

Tarefas:

1 - Adicionar neblina à cena ao implementar a função enableFog() com os parâmetros de habilitação
de fog na OpenGL;
2 - Usar uma imagem no formato BMP como MapaBitmap pra posicionar objetos 3D na cena. Use a cor de cada
pixel para definir qual o modelo será colocado, e a posição do pixel para definir a posição do
modelo no espaço. Pixels pretos não representam nenhum modelo, a posição fica vazia na cena;
3 - Mudar as configurações da fonte de luz da cena, já implementada no código base dado, para que
seja uma spotlight;
4 - Adicionar uma segunda fonte de luz que fique girando ao redor do cenário.
*/



#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include <cmath>
#include <random>
#include <ctime>
#include <cstring>
#include <iostream>
#include <gl/glut.h>
#include <string>
#include <chrono>
#include <tuple>
#include <algorithm>
/// STL INCLUSIONS
#include <vector>
#include <map>
#include <list>
#include "../include/FileMapReader.h"
#include "../include/Personagem.h"
#include "../include/Bloco.h"
//openal (sound lib)
#include "../CodeBlocks/include/al/alut.h"

//bitmap class to load bitmaps for textures
#include "../include/bitmap.h"

//handle for the al.obj model
//#include "ModelAl.h"

//handle generic obj models
#include "../include/3DObject.h"
#include "../include/Point3D.h"
#include "../include/Camera.h"

#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "alut.lib")

#define PI 3.14159265

// sound stuff
#define NUM_BUFFERS 1
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1

#define SMOOTH 0
#define SMOOTH_MATERIAL 1
#define SMOOTH_MATERIAL_TEXTURE 2

#define NUM_INIMIGOS 5
#define MAX_FLOORS 20
#define DISTANCIA_ANDARES 12
#include "../include/IAController.h"



#define MAX_PLANSIZE 15
#define MIN_PLANSIZE 0

using std::vector;
using std::map;
using std::list;

using namespace std::chrono;


void mainInit();
void initSound();
void initTexture();
void initModel();
void initLight();
void enableFog();
void createGLUI();
void mainRender();
void mainCreateMenu();
void onMouseButton(int button, int state, int x, int y);
void onMouseMove(int x, int y);
void onKeyDown(unsigned char key, int x, int y);
void onKeyUp(unsigned char key, int x, int y);
void onGLUIEvent(int id);
void onWindowReshape(int x, int y);
void mainIdle();
int main(int argc, char **argv);
void setWindow();
void setViewport(GLint left, GLint right, GLint bottom, GLint top);
void updateState();
void renderFloor();
void updateCam();
void setTextureToOpengl();

/**
Screen dimensions
*/
int windowWidth = 1200;
int windowHeight = 960;

/**
Screen position
*/
int windowXPos = 100;
int windowYPos = 150;

int mainWindowId = 0;

double xOffset = -1.9;
double yOffset = -1.3;
int mouseLastX = 0;
int mouseLastY = 0;

float roty = 0.0f;
float rotx = 90.0f;

bool rpressed = false;
bool rightPressed = false;
bool leftPressed = false;
bool upPressed = false;
bool downPressed = false;

bool spacePressed = false;

float speedX = 0.0f;
float speedY = 0.0f;
float speedZ = 0.0f;

float posX = 5.0f;
float posY = 0.0f;
float posZ = 5.0f;

/*
variavel auxiliar pra dar variação na altura do ponto de vista ao AndarBitmap.
*/
float headPosAux = 0.0f;

float maxSpeed = 0.25f;

float planeSize = 8.0f;

// more sound stuff (position, speed and orientation of the listener)
ALfloat listenerPos[]={0.0,0.0,4.0};
ALfloat listenerVel[]={0.0,0.0,0.0};
ALfloat listenerOri[]={0.0,0.0,1.0,
						0.0,1.0,0.0};

// now the position and speed of the sound source
ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

// buffers for openal stuff
ALuint  buffer[NUM_BUFFERS];
ALuint  source[NUM_SOURCES];
ALuint  environment[NUM_ENVIRONMENTS];
ALsizei size,freq;
ALenum  format;
ALvoid  *data;



// parte de código extraído de "texture.c" por Michael Sweet (OpenGL SuperBible)
// texture buffers and stuff
int i;                       /* Looping var */
BITMAPINFO	*info;           /* Bitmap information */
GLubyte	    *bits;           /* Bitmap RGB pixels */
GLubyte     *ptr;            /* Pointer into bit buffer */
GLubyte	    *rgba;           /* RGBA pixel buffer */
GLubyte	    *rgbaptr;        /* Pointer into RGBA buffer */
GLubyte     temp;            /* Swapping variable */
GLenum      type;            /* Texture type */
GLuint      texture;         /* Texture object */



bool crouched = false;
bool running = false;
float gravity = 0.004;
float heightLimit = 0.2;
float posYOffset = 0.2;

Personagem* me;

float backgrundColor[4] = {0.0f,0.0f,0.0f,1.0f};

Point3D posicaoJogador;
int andarNivel = 1;
TipoCamera atualCam = TipoCamera::PRIMEIRA_PESSOA;
Camera primeiraPessoaCam, terceiraPessoaCam, cimaCam;
C3DObject blocoIndest, blocoDest, ouro, personagem, escadaSobe, escadaDesce,inimigoO;

int ourosColetados=0, totalOuros=0;
list<Personagem*> inimigos;
list<Bloco*> blocos;
map<std::tuple<int,int,int>, Bloco*> blocosMap;
map<std::tuple<int,int,int>, Personagem*> inimigosMap;


Mapa* mapa;

ObjEnum matrizMapa[GRID_HEIGHT][GRID_WIDTH][MAX_FLOORS];


//CModelAl modelAL;

void setWindow() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)windowWidth/(GLfloat)windowHeight,0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(me->posicao.x,me->posicao.y + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)),me->posicao.z,
		me->posicao.x + sin(roty*PI/180),me->posicao.y + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180),me->posicao.z -cos(roty*PI/180),
		0.0,1.0,0.0);
}

/**
Atualiza a posição e orientação da camera
*/
void updateCam() {


    primeiraPessoaCam.set_eye(me->posicao.x,(GLfloat)( me->posicao.y + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180))), me->posicao.z);
    primeiraPessoaCam.set_center(me->posicao.x + sin(roty*PI/180),me->posicao.y + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180),me->posicao.z -cos(roty*PI/180));
    primeiraPessoaCam.set_upvector(0.0, 1.0, 0.0);
	// pengoCamera.callGluLookAt();


  /*  terceiraPessoaCam.set_eye(me->posicao + 5 * Point3D(sin(roty*PI/180), -cos(rotx*PI/180), cos(roty*PI/180)));
    terceiraPessoaCam.set_center(me->posicao);
    terceiraPessoaCam.set_upvector(0.0, 1.0, 0.0);*/


    terceiraPessoaCam.set_eye(me->posicao.x-25,(GLfloat)( me->posicao.y + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)))+5, me->posicao.z);
    terceiraPessoaCam.set_center(me->posicao.x + sin(roty*PI/180),me->posicao.y + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180),me->posicao.z -cos(roty*PI/180));
    terceiraPessoaCam.set_upvector(0.0, 1.0, 0.0);


	/*gluLookAt(me->posicao.x,me->posicao.y + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)),me->posicao.z,
		me->posicao.x + sin(roty*PI/180),me->posicao.y + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180),me->posicao.z -cos(roty*PI/180),
		0.0,1.0,0.0);*/
    switch (atualCam)
	{
        case TipoCamera::PRIMEIRA_PESSOA:
            primeiraPessoaCam.callGluLookAt();
            break;
        case TipoCamera::TERCEIRA_PESSOA:
            terceiraPessoaCam.callGluLookAt();
            break;
        case TipoCamera::CIMA:
            cimaCam.callGluLookAt();
            glDisable(GL_FOG);
            break;
	}


	// atualiza a posição do listener e da origen do som, são as mesmas da camera, já que os passos vem de onde o personagem está
	listenerPos[0] = me->posicao.x;
	listenerPos[1] = me->posicao.y;
	listenerPos[2] = me->posicao.z;
	source0Pos[0] = me->posicao.x;
	source0Pos[1] = me->posicao.y;
	source0Pos[2] = me->posicao.z;

    GLfloat light_position1[] = {me->posicao.x, me->posicao.y, me->posicao.z, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);


}

void initLight() {
    glEnable(GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	GLfloat light_ambient[] = { backgrundColor[0], backgrundColor[1], backgrundColor[2], backgrundColor[3] };
	GLfloat light_diffuse[] = { 100.0, 100.0, 100.0, 100.0 };
	GLfloat light_specular[] = { 10.0, 10.0, 10.0, 10.0 };
	GLfloat light_position1[] = {0.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);


}

void initInimigos()
{
    ObjEnum value = ObjEnum::INIMIGO;
    for(int a=0;a < mapa->andares.size();a++)
    {
        for(int h=0;h<2 + (rand() % (int)(4));h++)
        {
            int i = 0 + (rand() % (int)(15));
            int j = 0 + (rand() % (int)(15));
            Point3D ponto;
            ponto.x = i;
            ponto.y = DISTANCIA_ANDARES*a;
            ponto.z = j;
            Personagem* b = new Personagem(ponto);
            inimigos.push_back(b);
            std::cout << rand();


            inimigosMap.insert(std::make_pair(std::make_tuple(i,j,-2 + 2+ DISTANCIA_ANDARES*a),b));
        }


    }
}

void initBlocos()
{
    for(int a=0;a < mapa->andares.size();a++)
    {
        for(int k=0;k<2;k++)
        {
            // std::cout << "A: " << a << "  K: " << k << std::endl;
            for(int i=0; i<GRID_HEIGHT; i++)
            {
                for(int j=0; j<GRID_WIDTH; j++)
                {

                    matrizMapa[i][j][k + a] = mapa->andares[a].andares[k].grid[i][j];
                }
            }
        }
    }

    for(int a=0;a < mapa->andares.size();a++)
    {
        for(int k=0;k<2;k++)
        {
            for(int i=0; i<GRID_HEIGHT; i++)
            {
                for(int j=0; j<GRID_WIDTH; j++)
                {
                    Bloco* b = new Bloco(std::make_tuple(i,j,-2 + 2*k+ DISTANCIA_ANDARES*a));
                    blocos.push_back(b);
                    ObjEnum value = mapa->andares[a].andares[k].grid[i][j];
                    //std::cout << (int)mapa->andares[a].andares[k].grid[i][j] << " ";
                    if(value == ObjEnum::BLOCODEST)
                        b->tipo = ObjEnum::BLOCODEST;
                    else if(value == ObjEnum::BLOCOINDEST)
                        b->tipo = ObjEnum::BLOCOINDEST;
                    else if(value == ObjEnum::OURO)
                    {
                        totalOuros += 1;
                        b->tipo = ObjEnum::OURO;
                    }

                    else if(value == ObjEnum::ESCADASOBE)
                        b->tipo = ObjEnum::ESCADASOBE;
                    else if(value == ObjEnum::ESCADADESCE)
                        b->tipo = ObjEnum::ESCADADESCE;

                    blocosMap.insert(std::make_pair(std::make_tuple(i,j,-2 + 2*k+ DISTANCIA_ANDARES*a),b));

                }
            }
        }

    }

    initInimigos();
}

std::pair<int,int> separaAltura(int y)
{
    int aux = (int)y;
    int k = ((aux+2) % DISTANCIA_ANDARES) /2;
    int a = ((aux + 2) + 2*k) / DISTANCIA_ANDARES;
    return std::make_pair(k,a/FATOR_TAMANHO_MAPA);
}

void renderMapa()
{
    for(int i=0; i<GRID_HEIGHT;i++)
    {
        for(int j=0;j<GRID_WIDTH;j++)
        {
            glPushMatrix();
            glScalef(FATOR_TAMANHO_MAPA,FATOR_TAMANHO_MAPA,FATOR_TAMANHO_MAPA);
            glTranslatef(0.0 + 2*i,-4,0.0 + 2*j);
            blocoIndest.Draw(SMOOTH_MATERIAL_TEXTURE);
            glPopMatrix();
            glPopMatrix();
        }

    }

    int k=0;
    for(Bloco* bl : blocos)
    {

        //Bloco* b = new Bloco(std::make_tuple(i,j,k));

       // blocosMap.erase(std::make_pair(std::make_tuple(i,j,k),b));
        //blocosMap.insert(std::make_pair(std::make_tuple(i,j,k),b));


        glPushMatrix();
        glScalef(FATOR_TAMANHO_MAPA,FATOR_TAMANHO_MAPA,FATOR_TAMANHO_MAPA);
        std::tuple<int,int,int> pos = bl->getMatrixPosition();
        glTranslatef(0.0 + 2*std::get<1>(pos),std::get<2>(pos),0.0 + 2*std::get<0>(pos));


        if(bl->tipo == ObjEnum::BLOCOINDEST)
        {
            //b->tipo = ObjEnum::BLOCOINDEST;
            blocoIndest.Draw(SMOOTH_MATERIAL_TEXTURE);
           // matrizMapa[i][j][k + a] = ObjEnum::BLOCOINDEST;
        }

        if(bl->tipo == ObjEnum::BLOCODEST)
        {
            //b->tipo = ObjEnum::BLOCODEST;
           // matrizMapa[i][j][k + a] = ObjEnum::BLOCODEST;
            if(bl->destroyied == false)
                blocoDest.Draw(SMOOTH_MATERIAL_TEXTURE);
            else
                bl->reconstroi();

        }

        if(bl->tipo == ObjEnum::OURO && bl->coletado==false)
        {
            //b->tipo = ObjEnum::OURO;
          //  std::cout << bl->coletado;
          //  std::cout << "AAAAAAAAAAAAAAAA";
            ouro.Draw(SMOOTH_MATERIAL_TEXTURE);
           // matrizMapa[i][j][k + a] = ObjEnum::OURO;
        }
        if(bl->tipo == ObjEnum::ESCADASOBE)
        {
           // b->tipo = ObjEnum::ESCADASOBE;
            escadaSobe.Draw(SMOOTH_MATERIAL_TEXTURE);
           // matrizMapa[i][j][k + a] = ObjEnum::ESCADASOBE;
        }
        if(bl->tipo == ObjEnum::ESCADADESCE)
        {
           // b->tipo = ObjEnum::ESCADASOBE;
            escadaDesce.Draw(SMOOTH_MATERIAL_TEXTURE);
           // matrizMapa[i][j][k + a] = ObjEnum::ESCADASOBE;
        }
        if(bl->tipo == ObjEnum::VAZIO)
        {
            //b->tipo = ObjEnum::VAZIO;
           // matrizMapa[i][j][k + a] = ObjEnum::VAZIO;
        }
//        blocos.push_back(b);



        glPopMatrix();
    }

    for(Personagem* p : inimigos)
    {
        glPushMatrix();
        glScalef(FATOR_TAMANHO_MAPA,FATOR_TAMANHO_MAPA,FATOR_TAMANHO_MAPA);
        glTranslatef(0.0 + 2*p->posicao.x,p->posicao.y,0.0 + 2*p->posicao.z);

        inimigoO.Draw(SMOOTH_MATERIAL_TEXTURE);

        glPopMatrix();

    }



}

void setViewport(GLint left, GLint right, GLint bottom, GLint top) {
	glViewport(left, bottom, right - left, top - bottom);
}


/**
Initialize
*/
void mainInit() {

	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	setWindow();
	setViewport(0, windowWidth, 0, windowHeight);

	// habilita remocao de faces ocultas

	glFrontFace (GL_CCW);

	glEnable(GL_CULL_FACE);

	// habilita o z-buffer
	glEnable(GL_DEPTH_TEST);

    initSound();

    initTexture();


//    initMapa();
    renderMapa();

	initModel();

    initBlocos();

	initLight();

	enableFog();

	printf("w - Andar \n");
	printf("s - ir pra tras \n");
	printf("mouse - direcao \n");
	printf("r - correr \n");
	printf("c - abaixar \n");
	printf("espaco - pular \n");

}

void initModel() {
	printf("Loading models.. \n");
	blocoIndest.Init();
	blocoIndest.Load("../../models/indestrutivel.obj");
	blocoDest.Init();
	blocoDest.Load("../../models/destrutivel.obj");
	ouro.Init();
	ouro.Load("../../models/ouro.obj");
	escadaSobe.Init();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	escadaSobe.Load("../../models/escadaSobe.obj");
    escadaDesce.Init();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	escadaDesce.Load("../../models/escadaDesce.obj");
    personagem.Init();
    personagem.Load("../../models/Steve.obj");
    inimigoO.Init();
    inimigoO.Load("../../models/Steve.obj");
	printf("Models ok. \n \n \n");
}

/**
Initialize openal and check for errors
*/
void initSound() {

	printf("Initializing OpenAl \n");

	// Init openAL
	alutInit(0, NULL);

	alGetError(); // clear any error messages

    // Generate buffers, or else no sound will happen!
    alGenBuffers(NUM_BUFFERS, buffer);

    if(alGetError() != AL_NO_ERROR)
    {
        printf("- Error creating buffers !!\n");
        exit(1);
    }
    else
    {
        printf("init() - No errors yet.\n");
    }

	alutLoadWAVFile("Footsteps.wav",&format,&data,&size,&freq,false);
    alBufferData(buffer[0],format,data,size,freq);

	alGetError(); /* clear error */
    alGenSources(NUM_SOURCES, source);

    if(alGetError() != AL_NO_ERROR)
    {
        printf("- Error creating sources !!\n");
        exit(2);
    }
    else
    {
        printf("init - no errors after alGenSources\n");
    }

	listenerPos[0] = me->posicao.x;
	listenerPos[1] = me->posicao.y;
	listenerPos[2] = me->posicao.z;

	source0Pos[0] = me->posicao.x;
	source0Pos[1] = me->posicao.y;
	source0Pos[2] = me->posicao.z;

	alListenerfv(AL_POSITION,listenerPos);
    alListenerfv(AL_VELOCITY,listenerVel);
    alListenerfv(AL_ORIENTATION,listenerOri);

	alSourcef(source[0], AL_PITCH, 1.0f);
    alSourcef(source[0], AL_GAIN, 1.0f);
    alSourcefv(source[0], AL_POSITION, source0Pos);
    alSourcefv(source[0], AL_VELOCITY, source0Vel);
    alSourcei(source[0], AL_BUFFER,buffer[0]);
    alSourcei(source[0], AL_LOOPING, AL_TRUE);

	printf("Sound ok! \n\n");
}

/**
Initialize the texture using the library bitmap
*/
void initTexture(void)
{
    printf ("\nLoading texture..\n");
    // Load a texture object (256x256 true color)
    bits = LoadDIBitmap("tiledbronze.bmp", &info);
    if (bits == (GLubyte *)0) {
		printf ("Error loading texture!\n\n");
		return;
	}
    // Figure out the type of texture
    if (info->bmiHeader.biHeight == 1)
      type = GL_TEXTURE_1D;
    else
      type = GL_TEXTURE_2D;

    // Create and bind a texture object
    glGenTextures(1, &texture);
	glBindTexture(type, texture);

    // Create an RGBA image
    rgba = (GLubyte *)malloc(info->bmiHeader.biWidth * info->bmiHeader.biHeight * 4);

    i = info->bmiHeader.biWidth * info->bmiHeader.biHeight;
    for( rgbaptr = rgba, ptr = bits;  i > 0; i--, rgbaptr += 4, ptr += 3)
    {
            rgbaptr[0] = ptr[2];     // windows BMP = BGR
            rgbaptr[1] = ptr[1];
            rgbaptr[2] = ptr[0];
            rgbaptr[3] = (ptr[0] + ptr[1] + ptr[2]) / 3;
    }
/*
	// Set texture parameters
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(type, 0, 4, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
                  0, GL_RGBA, GL_UNSIGNED_BYTE, rgba );
*/
    glEnable(GL_BLEND);
    printf("Textura %d\n", texture);
	printf("Textures ok.\n\n", texture);

}

/**
Recovers the texture already initialized in initTexture(), setting it to opengl
*/
void setTextureToOpengl(void)
{
    // Create and bind a texture object
    glGenTextures(1, &texture);
	glBindTexture(type, texture);

	// Set texture parameters
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(type, 0, 4, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
                  0, GL_RGBA, GL_UNSIGNED_BYTE, rgba );
}

void enableFog(void)
{
}

int getDirection()
{
     int direction = round(std::abs(int(roty) % 360)/45.0);
    if (direction == 8)
        direction = 0;

    if (downPressed)
        direction = (4+direction) % 8;
    if (leftPressed)
        direction = (6+direction) % 8;
    if (rightPressed)
        direction = (2+direction) % 8;
    return direction;
}

bool hacolisao (float floatX, float floatZ, float Y)
{

    int direction = getDirection();
    int Zaux = (int)(std::round((floatZ - 0.5)));
    int Xaux =  + ((int) std::round((floatX - 0.5)));
    int Z =  Zaux  / (FATOR_TAMANHO_MAPA*2) ;
    int X = Xaux  / (FATOR_TAMANHO_MAPA*2);



  //  int Xaux =
 //   if (Z <= MIN_PLANSIZE || Z >= MAX_PLANSIZE || X <= MIN_PLANSIZE || X >= MAX_PLANSIZE) return true;



    bool helper = false;
    std::pair<int,int> par = separaAltura(Y);
   int k = par.first, a = par.second;
  //  std::cout << "k: " << k << "   Y: " << me->posicao.y << "  a: " << a << std::endl;
    std::cout << "DIRECAO: " << direction << std::endl;
 //   return false;



    switch (direction)
    {
    case 0:
        if(mapa->andares[a].andares[k].grid[Z-1][X] == ObjEnum::ESCADASOBE || mapa->andares[a].andares[k].grid[Z-1][X+1] == ObjEnum::ESCADASOBE)
        {
            me->subindoEscada = true;
            return true;
        }
        break;
    case 2:
        if(mapa->andares[a].andares[k].grid[Z][X+1] == ObjEnum::ESCADASOBE || mapa->andares[a].andares[k].grid[Z+1][X+1] == ObjEnum::ESCADASOBE)
        {
            me->subindoEscada = true;
            return true;
        }
        break;
    case 4:
        if(mapa->andares[a].andares[k].grid[Z+1][X] == ObjEnum::ESCADASOBE)
        {
            me->subindoEscada = true;
            return true;
        }
        break;
    case 6:
        if( mapa->andares[a].andares[k].grid[Z][X-1] == ObjEnum::ESCADASOBE || mapa->andares[a].andares[k].grid[Z+1][X-1] == ObjEnum::ESCADASOBE)
        {
            me->subindoEscada = true;
            return true;
        }
        break;
    default:
        break;
    }

    switch (direction)
    {
    case 0:
        if(mapa->andares[a].andares[k].grid[Z-1][X] == ObjEnum::ESCADADESCE || mapa->andares[a].andares[k].grid[Z-1][X+1] == ObjEnum::ESCADADESCE)
        {
            me->descendoEscada = true;
            return true;
        }
        break;
    case 2:
        if(mapa->andares[a].andares[k].grid[Z][X+1] == ObjEnum::ESCADADESCE || mapa->andares[a].andares[k].grid[Z+1][X+1] == ObjEnum::ESCADADESCE)
        {
            me->descendoEscada = true;
            return true;
        }
        break;
    case 4:
        if( mapa->andares[a].andares[k].grid[Z+1][X] == ObjEnum::ESCADADESCE)
        {
            me->descendoEscada = true;
            return true;
        }
        break;
    case 6:
        if( mapa->andares[a].andares[k].grid[Z][X-1] == ObjEnum::ESCADADESCE || mapa->andares[a].andares[k].grid[Z+1][X-1] == ObjEnum::ESCADADESCE)
        {
            me->descendoEscada = true;
            return true;
        }
        break;
    default:
        break;
    }

    Bloco* b;
    switch (direction)
    {
    case 0:
        if(mapa->andares[a].andares[k].grid[Z-1][X] == ObjEnum::OURO || mapa->andares[a].andares[k].grid[Z-1][X+1] == ObjEnum::OURO)
        {
            b = blocosMap[std::make_tuple(Z-1,X,-2+2*k+DISTANCIA_ANDARES*a)];
            b->tipo = ObjEnum::VAZIO;
            b = blocosMap[std::make_tuple(Z-1,X+1,-2+2*k+DISTANCIA_ANDARES*a)];
            b->tipo = ObjEnum::VAZIO;
            ourosColetados++;
            return false;
        }
        break;
    case 2:
        if(mapa->andares[a].andares[k].grid[Z][X+1] == ObjEnum::OURO || mapa->andares[a].andares[k].grid[Z+1][X+1] == ObjEnum::OURO)
        {

            b = blocosMap[std::make_tuple(Z,X+1,-2+2*k+DISTANCIA_ANDARES*a)];
            b->tipo = ObjEnum::VAZIO;
            b = blocosMap[std::make_tuple(Z+1,X+1,-2+2*k+DISTANCIA_ANDARES*a)];
            b->tipo = ObjEnum::VAZIO;
            ourosColetados++;
            return false;
        }
        break;
    case 4:
        if(mapa->andares[a].andares[k].grid[Z+1][X] == ObjEnum::OURO)
        {
            b = blocosMap[std::make_tuple(Z+1,X,-2+2*k+DISTANCIA_ANDARES*a)];
            b->tipo = ObjEnum::VAZIO;
            ourosColetados++;
            return false;
        }
        break;
    case 6:
        if( mapa->andares[a].andares[k].grid[Z][X-1] == ObjEnum::OURO || mapa->andares[a].andares[k].grid[Z+1][X-1] == ObjEnum::OURO)
        {
            b = blocosMap[std::make_tuple(Z,X-1,-2+2*k+DISTANCIA_ANDARES*a)];
            b->tipo = ObjEnum::VAZIO;
            b = blocosMap[std::make_tuple(Z+1,X-1,-2+2*k+DISTANCIA_ANDARES*a)];
            b->tipo = ObjEnum::VAZIO;
            ourosColetados++;
            return false;
        }
        break;
    default:
        break;
    }

    switch (direction)
    {
    case 0:

        return mapa->andares[a].andares[k].grid[Z-1][X] != ObjEnum::VAZIO || mapa->andares[a].andares[k].grid[Z-1][X+1] != ObjEnum::VAZIO;

        break;
    case 2:

        return mapa->andares[a].andares[k].grid[Z][X+1] != ObjEnum::VAZIO || mapa->andares[a].andares[k].grid[Z+1][X+1] != ObjEnum::VAZIO;

        break;
    case 4:

        return mapa->andares[a].andares[k].grid[Z+1][X] != ObjEnum::VAZIO;// || Zaux % (FATOR_TAMANHO_MAPA*2) == 0;

        break;
    case 6:

        return  mapa->andares[a].andares[k].grid[Z][X-1] != ObjEnum::VAZIO || mapa->andares[a].andares[k].grid[Z+1][X-1] != ObjEnum::VAZIO;

        break;
    }
    return false;
}

void renderFloor() {
	// set things up to render the floor with the texture
	glShadeModel(GL_SMOOTH);
	glEnable(type);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPushMatrix();

    glTranslatef(-(float)planeSize/2.0f, 0.0f, -(float)planeSize/2.0f);

	float textureScaleX = 10.0;
	float textureScaleY = 10.0;
    glColor4f(1.0f,1.0f,1.0f,1.0f);
    int xQuads = 40;
    int zQuads = 40;
    for (int i = 0; i < xQuads; i++) {
        for (int j = 0; j < zQuads; j++) {
            glBegin(GL_QUADS);
                glTexCoord2f(1.0f, 0.0f);   // coords for the texture
                glNormal3f(0.0f,1.0f,0.0f);
                glVertex3f(i * (float)planeSize/xQuads, 0.0f, (j+1) * (float)planeSize/zQuads);

                glTexCoord2f(0.0f, 0.0f);  // coords for the texture
                glNormal3f(0.0f,1.0f,0.0f);
                glVertex3f((i+1) * (float)planeSize/xQuads, 0.0f, (j+1) * (float)planeSize/zQuads);

                glTexCoord2f(0.0f, 1.0f);  // coords for the texture
                glNormal3f(0.0f,1.0f,0.0f);
                glVertex3f((i+1) * (float)planeSize/xQuads, 0.0f, j * (float)planeSize/zQuads);

                glTexCoord2f(1.0f, 1.0f);  // coords for the texture
                glNormal3f(0.0f,1.0f,0.0f);
                glVertex3f(i * (float)planeSize/xQuads, 0.0f, j * (float)planeSize/zQuads);

            glEnd();
        }
    }


	glDisable(type);


	glPopMatrix();
}

void writeTextAt(int x, int y, std::string text)
{
    double matrix[16]; // Save the matrices
    double mvmatrix[16];
    glMatrixMode(GL_PROJECTION); // change the current matrix to PROJECTION
    glGetDoublev(GL_PROJECTION_MATRIX, matrix); // get the values from PROJECTION matrix to local variable
    glLoadIdentity(); // reset PROJECTION matrix to identity matrix
    glOrtho(0, windowWidth, 0, windowHeight, 0.0, 5.0); // orthographic perspective
    glMatrixMode(GL_MODELVIEW); // change current matrix to MODELVIEW matrix again
    glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
    glLoadIdentity(); // reset it to identity matrix
    glPushMatrix(); // push current state of MODELVIEW matrix to stack
    glLoadIdentity(); // reset it again. (may not be required, but it my convention)
    glRasterPos2i(x, y); // raster position in 2D
    for(int i=0; i<text.size(); i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)text.at(i)); // generation of characters in our text with 9 by 15 GLU font
    }
    glPopMatrix(); // get MODELVIEW matrix value from stack
    glMatrixMode(GL_PROJECTION); // change current matrix mode to PROJECTION
    glLoadMatrixd(matrix); // reset
    glMatrixMode(GL_MODELVIEW); // change current matrix mode to MODELVIEW
    glLoadMatrixd(mvmatrix);
}

void fimJogo()
{
    std::string printMe ("GAME OVER");
    writeTextAt(10,10,printMe);//nao funciona essa bagaça
}

void moveInimigos()
{
    for(Personagem* p : inimigos)
    {
        moveInimigo(me,p);
        inimigoO.Draw(SMOOTH_MATERIAL_TEXTURE);
    }
}

void renderScene()
{
	glClearColor(backgrundColor[0],backgrundColor[1],backgrundColor[2],backgrundColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // limpar o depth buffer

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	updateCam();

    /*glPushMatrix();
    glScalef(0.3,0.3,0.3);
    glTranslatef(0.0,-0.9,0.0);
	blocoIndest.Draw(SMOOTH_MATERIAL_TEXTURE); // use SMOOTH for obj files, SMOOTH_MATERIAL for obj+mtl files and SMOOTH_MATERIAL_TEXTURE for obj+mtl+tga files
	glPopMatrix();*/

    // sets the bmp file already loaded to the OpenGL parameters
    setTextureToOpengl();
//    renderCube(new Point3D(0,0,0));

    renderMapa();
    glTranslatef(me->posicao.x, 1+me->posicao.y, me->posicao.z);
    glScaled(6,6,6);
    glRotated(90+roty,0,1,0);

    personagem.Draw(SMOOTH_MATERIAL_TEXTURE);

    moveInimigos();
	//renderFloor();

	//modelAL.Translate(0.0f,1.0f,0.0f);
	//modelAL.Draw();
}

/*void updateCamera()
{

    posicaoJogador.set_coords(posX, posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) , posZ);

    terceiraPessoaCam.set_eye(posicaoJogador + 5 * Point3D(sin(roty*PI/180), -cos(rotx*PI/180), cos(roty*PI/180)));
    terceiraPessoaCam.set_center(posicaoJogador);
    terceiraPessoaCam.set_upvector(0.0, 1.0, 0.0);
	// pengoCamera.callGluLookAt();

    primeiraPessoaCam.set_eye(posicaoJogador+ Point3D(0.0, 1.0, 0.0));
    primeiraPessoaCam.set_center((posicaoJogador + Point3D(0.0,1.0,0.0)) - 5 * Point3D(sin(roty*PI/180), -cos(rotx*PI/180), cos(roty*PI/180)));


	// atualiza a posição do listener e da origen do som, são as mesmas da camera, já que os passos vem de onde o personagem está
	listenerPos[0] = me->posicao.x;
	listenerPos[1] = me->posicao.y;
	listenerPos[2] = me->posicao.z;
	source0Pos[0] = me->posicao.x;
	source0Pos[1] = me->posicao.y;
	source0Pos[2] = me->posicao.z;
}*/


void updateState()
{

    int Zaux = (int)(std::round((me->posicao.z - 0.5)));
    int Xaux =  + ((int) std::round((me->posicao.x - 0.5)));
    int Z =  Zaux  / (FATOR_TAMANHO_MAPA*2) ;
    int X = Xaux  / (FATOR_TAMANHO_MAPA*2);
    std::pair<int,int> par = separaAltura(me->posicao.y);
    int k = par.first, a = par.second;





    if(me->subindoEscada)
    {
        me->posicao.y += 0.4;
        if(me->posicao.y >= (-2 + 2+ DISTANCIA_ANDARES*(me->andarAtual+1)) * FATOR_TAMANHO_MAPA)
        {
            me->subindoEscada = false;
            me->andarAtual++;
        }

        return;
    }
    if(me->descendoEscada)
    {
        me->posicao.y -= 0.4;
        if(me->posicao.y <= (-2 + 2+ DISTANCIA_ANDARES*(me->andarAtual-1)) * FATOR_TAMANHO_MAPA)
        {
            me->descendoEscada = false;
            me->andarAtual--;
        }

        return;
    }

    if(me->caindo && me->posicao.y >= -10)
    {
        me->posicao.y -= 0.7;
    }
    Bloco* b = blocosMap[std::make_tuple(Z,X,-2+DISTANCIA_ANDARES*a)];
    if(k==1 && ((mapa->andares[a].andares[0].grid[Z][X] == ObjEnum::VAZIO) || (mapa->andares[a].andares[k-1].grid[Z][X] == ObjEnum::BLOCODEST && b->destroyied == true)))
    {
        me->caindo = true;
        k=-1;
    }
    if(me->posicao.y <= (-2 + 2+ DISTANCIA_ANDARES*(me->andarAtual-1)) * FATOR_TAMANHO_MAPA)
    {
        me->andarAtual--;
        k = 1;
        if(mapa->andares[a].andares[0].grid[Z][X] != ObjEnum::VAZIO)
        {
            me->posicao.y = 48*a;
           me->caindo = false;
        }

    }


	if ((upPressed || downPressed))
    {

		if (running)
        {
			speedX = 0.05 * sin(roty*PI/180) * 2;
			speedZ = -0.05 * cos(roty*PI/180) * 2;
		} else
		{
			speedX = 0.05 * sin(roty*PI/180);
			speedZ = -0.05 * cos(roty*PI/180);
		}

		// efeito de "sobe e desce" ao AndarBitmap
		headPosAux += 8.5f;
		if (headPosAux > 180.0f)
			headPosAux = 0.0f;

        if (upPressed)
        {
            if (hacolisao(me->posicao.x + speedX, me->posicao.z + speedZ,me->posicao.y) == false)
                me->caminha(speedX, speedZ);
            else
            {
                if (hacolisao(me->posicao.x + speedX, me->posicao.z, me->posicao.y)==false)
                {
                    me->caminha(speedX, 0);
                }
                else
                {
                    if (hacolisao(me->posicao.x, me->posicao.z + speedZ, me->posicao.y)==false)
                    {
                        me->caminha(0, speedZ);
                    }
                }
            }
        } else
        {
            if (hacolisao(me->posicao.x - speedX, me->posicao.z - speedZ,me->posicao.y)==false)
            {
                me->caminha(-1*speedX, -1*speedZ);
            }
            else
            {
                if (hacolisao(me->posicao.x - speedX, me->posicao.z,me->posicao.y)==false)
                    me->caminha(-1*speedX, 0);
                else if (hacolisao(me->posicao.x, me->posicao.z - speedZ,me->posicao.y)==false)
                       me->caminha(0, -1*speedZ);;
            }
        }

	}
	else
	{
		// parou de AndarBitmap, para com o efeito de "sobe e desce"
		headPosAux = fmod(headPosAux, 90) - 1 * headPosAux / 90;
		headPosAux -= 4.0f;
		if (headPosAux < 0.0f)
		{
			headPosAux = 0.0f;
		}
	}

    if(spacePressed)
    {

        int direction = getDirection();
        std::pair<int,int> par = separaAltura(me->posicao.y);
        int k = par.first;
        int a = par.second;
        int Z = (int)(std::round((me->posicao.z - 0.5))) / (FATOR_TAMANHO_MAPA*2);
        int X =  + ((int) std::round((me->posicao.x - 0.5))) / (FATOR_TAMANHO_MAPA*2);

         Bloco* b;

        if(direction == 0)
            b = blocosMap[std::make_tuple(Z-3,X,-2 + 2*(k-1) + DISTANCIA_ANDARES*a)];
        if(direction == 2)
            b = blocosMap[std::make_tuple(Z,X+3,-2 + 2*(k-1) + DISTANCIA_ANDARES*a)];
        if(direction == 4)
            b = blocosMap[std::make_tuple(Z+3,X,-2 + 2*(k-1) + DISTANCIA_ANDARES*a)];
        if(direction == 6)
            b = blocosMap[std::make_tuple(Z,X-3,-2 + 2*(k-1) + DISTANCIA_ANDARES*a)];

        if(b != NULL)
        {
            b->destroi();
            spacePressed = false;
        }

    }
	if(leftPressed)
    {
        roty -= 90;
        leftPressed = false;
    }
    if(rightPressed)
    {
        roty += 90;
        rightPressed = false;
    }

	posY += speedY;
	if (posY < heightLimit) {
		posY = heightLimit;
		speedY = 0.0f;
	} else {
		speedY -= gravity;
	}

	if (crouched)
    {
		posYOffset -= 0.01;
		if (posYOffset < 0.1)
		{
			posYOffset = 0.1;
		}
	} else
	{
		posYOffset += 0.01;
		if (posYOffset > 0.2)
		{
			posYOffset = 0.2;
		}
	}

}



/**
Render scene
*/
void mainRender() {
	updateState();
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)windowWidth/(GLfloat)windowHeight, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_DEPTH_BUFFER_BIT);
	updateCam();



	renderScene();

  //  cimaCam.set_eye(me->posicao.getX(), cimaCam.get_eye().getY(), me->posicao.getZ());
   /* cimaCam.set_center(me->posicao.getX(), -1, me->posicao.getZ());
    cimaCam.callGluLookAt();*/
//	updateCamera();
   // updateCam();
	glFlush();
	glutPostRedisplay();
	Sleep(30);
}

/**
Handles events from the mouse right button menu
*/
void mainHandleMouseRightButtonMenuEvent(int option) {
	switch (option) {
		case 1 :
			exit(0);
			break;
		default:
			break;
	}
}

/**
Create mouse button menu
*/
void mainCreateMenu() {
	glutCreateMenu(mainHandleMouseRightButtonMenuEvent);
	glutAddMenuEntry("Quit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/**
Mouse button event handler
*/
void onMouseButton(int button, int state, int x, int y) {
	//printf("onMouseButton button: %d \n", button);
	glutPostRedisplay();
}

/**
Mouse move while button pressed event handler
*/
void onMouseMove(int x, int y) {

	/*mouseLastX = x;
	mouseLastY = y;*/

	glutPostRedisplay();
}

/**
Mouse move with no button pressed event handler
*/
void onMousePassiveMove(int x, int y)
{

	roty += (x - mouseLastX);

	rotx -= (y - mouseLastY);

	if (rotx < -128.0) {
		rotx = -128.0;
	}

	if (rotx > -45.0) {
		rotx = -45.0;
	}

	mouseLastX = x;
	mouseLastY = y;

	//glutPostRedisplay();
}

TipoCamera alteraCamera(TipoCamera curCamera)
{
    switch (curCamera)
    {
        case TipoCamera::PRIMEIRA_PESSOA:
            return TipoCamera::TERCEIRA_PESSOA;
        case TipoCamera::TERCEIRA_PESSOA:
            return TipoCamera::CIMA;
        case TipoCamera::CIMA:
            return TipoCamera::PRIMEIRA_PESSOA;
        default:
            return TipoCamera::PRIMEIRA_PESSOA;
    }


}

/**
Key press event handler
*/
void onKeyDown(unsigned char key, int x, int y)
{
	//printf("%d \n", key);
	switch (key)
	{
		case 32: //space
			if (!spacePressed)
            {

			}
			spacePressed = true;
			break;
		case 119: //w
			if (!upPressed)
            {
				alSourcePlay(source[0]);
			}
			upPressed = true;
			break;
		case 115: //s
			downPressed = true;
			break;
		case 97: //a
			leftPressed = true;
			break;
        case 'r':
            rpressed = true;
            break;
		case 100: //d
			rightPressed = true;
			break;
		case 99: //c
			crouched = true;
			break;
		default:
			break;
	}

	//glutPostRedisplay();
}

/**
Key release event handler
*/
void onKeyUp(unsigned char key, int x, int y) {
	switch (key)
	{
		case 32: //space
			spacePressed = false;
			break;
		case 119: //w
			if (upPressed) {
				alSourceStop(source[0]);
			}
			upPressed = false;
			break;
		case 115: //s
			downPressed = false;
			break;
        case 'r':
            rpressed = false;
            break;
		case 97: //a
			leftPressed = false;
			break;
		case 100: //d
			rightPressed = false;
			break;
		case 99: //c
			crouched = false;
			break;
		case 'v': //r
			atualCam = alteraCamera(atualCam);
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
	}

	//glutPostRedisplay();
}

void onWindowReshape(int x, int y) {
	windowWidth = x;
	windowHeight = y;
	setWindow();
	setViewport(0, windowWidth, 0, windowHeight);
}

/**
Glut idle funtion
*/
void mainIdle() {
	/**
	Set the active window before send an glutPostRedisplay call
	so it wont be accidently sent to the glui window
	*/

	glutSetWindow(mainWindowId);
	glutPostRedisplay();
}



int main(int argc, char **argv)
{
    fimJogo();
    srand(time(0));
    Point3D ponto;
    ponto.x = 0; ponto.y = 0; ponto.z = 1;
    me = new Personagem(ponto);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(windowWidth,windowHeight);
	glutInitWindowPosition(windowXPos,windowYPos);


    cimaCam.set_eye(0.0, 44.0, 0.0);
    cimaCam.set_center(0.0, -1.0, 0.0);
    cimaCam.set_upvector(1.0, 0.0, 0.0);

	/**
	Store main window id so that glui can send it redisplay events
	*/
	mainWindowId = glutCreateWindow("FPS");

	glutDisplayFunc(mainRender);

	glutReshapeFunc(onWindowReshape);

	/**
	Register mouse events handlers
	*/
	/*glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMove);
	glutPassiveMotionFunc(onMousePassiveMove);*/

	/**
	Register keyboard events handlers
	*/
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);

    time_t now;
	time(&now);

    mapa = FileMapReader::generateMapBitmap("C:/Users/Usuario/Desktop/mapas");
	mainInit();


	glutMainLoop();

    return 0;
}
