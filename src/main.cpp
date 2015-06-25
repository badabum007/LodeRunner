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
#define MAX_FLOORS 10

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
int windowWidth = 600;
int windowHeight = 480;

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

bool rightPressed = false;
bool leftPressed = false;
bool upPressed = false;
bool downPressed = false;

bool spacePressed = false;

float speedX = 0.0f;
float speedY = 0.0f;
float speedZ = 0.0f;

float posX = 0.0f;
float posY = 0.0f;
float posZ = 2.0f;

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
bool jumping = false;
float jumpSpeed = 0.06;
float gravity = 0.004;
float heightLimit = 0.2;
float posYOffset = 0.2;

float backgrundColor[4] = {0.0f,0.0f,0.0f,1.0f};

Point3D posicaoJogador;
C3DObject blocoIndest, blocoDest, ouro, personagem, escada;

list<Personagem*> inimigos;
list<Bloco*> blocos;
map<std::tuple<int,int,int>, Bloco*> blocosMap;


Mapa* mapa;

ObjEnum matrizMapa[GRID_HEIGHT][GRID_WIDTH][MAX_FLOORS];


//CModelAl modelAL;

void setWindow() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)windowWidth/(GLfloat)windowHeight,0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posX,posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)),posZ,
		posX + sin(roty*PI/180),posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180),posZ -cos(roty*PI/180),
		0.0,1.0,0.0);
}

/**
Atualiza a posição e orientação da camera
*/
void updateCam() {

	gluLookAt(posX,posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)),posZ,
		posX + sin(roty*PI/180),posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180),posZ -cos(roty*PI/180),
		0.0,1.0,0.0);

	// atualiza a posição do listener e da origen do som, são as mesmas da camera, já que os passos vem de onde o personagem está
	listenerPos[0] = posX;
	listenerPos[1] = posY;
	listenerPos[2] = posZ;
	source0Pos[0] = posX;
	source0Pos[1] = posY;
	source0Pos[2] = posZ;

    GLfloat light_position1[] = {posX, posY, posZ, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);


}

void initLight() {
    glEnable(GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	GLfloat light_ambient[] = { backgrundColor[0], backgrundColor[1], backgrundColor[2], backgrundColor[3] };
	GLfloat light_diffuse[] = { 10.0, 10.0, 10.0, 10.0 };
	GLfloat light_specular[] = { 10.0, 10.0, 10.0, 10.0 };
	GLfloat light_position1[] = {0.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);

}


void renderMapa()
{

    for(int i=0; i<GRID_HEIGHT;i++)
    {
        for(int j=0;j<GRID_WIDTH;j++)
        {
            glPushMatrix();
            glScalef(0.3,0.3,0.3);
            glTranslatef(0.0 + 2*i,-4,0.0 + 2*j);
            blocoIndest.Draw(SMOOTH_MATERIAL_TEXTURE);
            glPopMatrix();
            glPopMatrix();
        }

    }

    int a =0;


    for(Andar andar : mapa->andares)
    {
        for(int k=0;k<2;k++)
        {
            for(int i=0; i<andar.andares[k].grid.size(); i++)
            {
                for(int j=0; j<andar.andares[k].grid[i].size(); j++)
                {
                    std::vector<ObjEnum> linha;
                    Bloco* b = new Bloco(std::make_tuple(i,j,k));
                    blocos.push_back(b);
                    blocosMap.insert(std::make_pair(std::make_tuple(i,j,k),b));

                    glPushMatrix();
                    glScalef(0.3,0.3,0.3);
                    glTranslatef(0.0 + 2*i,-2 + 2*k + 12*a,0.0 + 2*j);

                    if(andar.andares[k].grid[i][j] == ObjEnum::BLOCOINDEST)
                    {
                        blocoIndest.Draw(SMOOTH_MATERIAL_TEXTURE);
                        matrizMapa[i][j][k] = ObjEnum::BLOCOINDEST;
                    }

                    if(andar.andares[k].grid[i][j] == ObjEnum::BLOCODEST)
                    {
                        blocoDest.Draw(SMOOTH_MATERIAL_TEXTURE);
                        matrizMapa[i][j][k] = ObjEnum::BLOCODEST;
                    }

                    if(andar.andares[k].grid[i][j] == ObjEnum::OURO)
                    {
                        ouro.Draw(SMOOTH_MATERIAL_TEXTURE);
                        matrizMapa[i][j][k] = ObjEnum::OURO;
                    }
                    if(andar.andares[k].grid[i][j] == ObjEnum::ESCADA)
                    {
                        escada.Draw(SMOOTH_MATERIAL_TEXTURE);
                        matrizMapa[i][j][k] = ObjEnum::ESCADA;
                    }
                    if(andar.andares[k].grid[i][j] == ObjEnum::VAZIO)
                    {

                        matrizMapa[i][j][k] = ObjEnum::VAZIO;
                    }



                    glPopMatrix();
                }
            }
        }
        a++;



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


  //  initMapa();

	initModel();



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
	escada.Init();
	escada.Load("../../models/escada.obj");


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

	listenerPos[0] = posX;
	listenerPos[1] = posY;
	listenerPos[2] = posZ;

	source0Pos[0] = posX;
	source0Pos[1] = posY;
	source0Pos[2] = posZ;

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

bool hacolisao (float floatX, float floatZ, int Y){
    Y = 2;
    int direction = round(std::abs(int(roty) % 360)/45.0);
    if (direction == 8)
        direction = 0;

    if (downPressed)
        direction = (4+direction) % 8;
    if (leftPressed)
        direction = (6+direction) % 8;
    if (rightPressed)
        direction = (2+direction) % 8;
    int Z = (int)std::round(posicaoJogador.getZ() - 0.5) + 12;
    int X = (int)std::round(posicaoJogador.getX() - 0.5) + 12;
    Z = (int)(std::round(floatZ - 0.5)) + 12;
    X = + ((int) std::round(floatX - 0.5)+12);
    if (Z <= 0 || Z >= planeSize - 1 || X <= 0 || X >= planeSize - 1) return true;

    bool helper = false;
    /*for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            int l = i + x;
            int k = j + z;
            if (l < 0 || l >= 24 || k < 0 || k >= 0 || (i == 0 && j == 0)) continue;
            if (sceneMatrix[l*24+k] == ENEMY)
            {
                pengoDead = true;
                return false;
            }
        }
    }*/

    switch (direction)
    {
    case 0:
        Z--;
        return  matrizMapa[X-1][Z][Y] != ObjEnum::VAZIO ||
                matrizMapa[X][Z][Y]!= ObjEnum::VAZIO ||
                matrizMapa[X+1][Z][Y] != ObjEnum::VAZIO;
    case 1:
        return  matrizMapa[X+1][Z-1][Y] != ObjEnum::VAZIO ||
                matrizMapa[X][Z-1][Y] != ObjEnum::VAZIO ||
                matrizMapa[X+1][Z][Y]!= ObjEnum::VAZIO;
        break;
    case 2:
        X++;
        return  matrizMapa[X][Z-1][Y] != ObjEnum::VAZIO ||
                matrizMapa[X][Z][Y] != ObjEnum::VAZIO ||
                matrizMapa[X][Z+1][Y] != ObjEnum::VAZIO;
        break;
    case 3:
        return  matrizMapa[X+1][Z][Y] != ObjEnum::VAZIO ||
                matrizMapa[X+1][Z+1][Y] != ObjEnum::VAZIO ||
                matrizMapa[X][Z+1][Y] != ObjEnum::VAZIO;
        break;
    case 4:
        Z++;
        return matrizMapa[X+1][Z][Y] != ObjEnum::VAZIO ||
                matrizMapa[X][Z][Y]!= ObjEnum::VAZIO ||
                matrizMapa[X-1][Z][Y]!= ObjEnum::VAZIO;
        break;
    case 5:
        return matrizMapa[X][Z+1][Y] != ObjEnum::VAZIO ||
                matrizMapa[X-1][Z+1][Y] != ObjEnum::VAZIO ||
                matrizMapa[X-1][Z][Y] != ObjEnum::VAZIO;
        break;
    case 6:
        X--;
        return  matrizMapa[X][Z-1][Y] != ObjEnum::VAZIO ||
                matrizMapa[X][Z][Y] != ObjEnum::VAZIO ||
                matrizMapa[X][Z+1][Y] != ObjEnum::VAZIO;
        break;
    case 7:
        return matrizMapa[X-1][Z][Y] != ObjEnum::VAZIO ||
                matrizMapa[X-1][Z-1][Y]!= ObjEnum::VAZIO ||
                matrizMapa[X][Z-1][Y] != ObjEnum::VAZIO;
        break;

    }
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

void renderScene() {
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
	//renderFloor();

	//modelAL.Translate(0.0f,1.0f,0.0f);
	//modelAL.Draw();
}

void updateCamera()
{

    posicaoJogador.set_coords(posX, posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) , posZ);

    posicaoJogador.set_eye(pengoPosition + 5 * Point3D(sin(roty*PI/180), -cos(rotx*PI/180), cos(roty*PI/180)));
    posicaoJogador.set_center(pengoPosition);
    posicaoJogador.set_upvector(0.0, 1.0, 0.0);
	// pengoCamera.callGluLookAt();

    fpCamera.set_eye(pengoPosition+ Point3D(0.0, 1.0, 0.0));
    fpCamera.set_center((pengoPosition + Point3D(0.0,1.0,0.0)) - 5 * Point3D(sin(roty*PI/180), -cos(rotx*PI/180), cos(roty*PI/180)));


	// atualiza a posição do listener e da origen do som, são as mesmas da camera, já que os passos vem de onde o personagem está
	listenerPos[0] = posX;
	listenerPos[1] = posY;
	listenerPos[2] = posZ;
	source0Pos[0] = posX;
	source0Pos[1] = posY;
	source0Pos[2] = posZ;
}


void updateState()
{

	if (upPressed || downPressed)
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
            if (hacolisao(posX + speedX, posZ + speedZ,posicaoJogador.y)==false)
            {
                posX += speedX;
                posZ += speedZ;
            }
            else
            {
                if (hacolisao(posX + speedX, posZ, posicaoJogador.y)==false)
                {
                    posX += speedX;
                }
                else
                {
                    if (hacolisao(posX, posZ + speedZ, posicaoJogador.y)==false)
                    {
                        posZ += speedZ;
                    }
                }
            }
        } else
        {


            if (hacolisao(posX - speedX, posZ - speedZ,posicaoJogador.y)==false)
            {
                posX -= speedX;
                posZ -= speedZ;
            }
            else
            {
                if (hacolisao(posX - speedX, posZ,posicaoJogador.y)==false)
                    posX -= speedX;
                else if (hacolisao(posX, posZ - speedZ,posicaoJogador.y)==false)
                        posZ -= speedZ;
            }
        }

	} else
	{
		// parou de AndarBitmap, para com o efeito de "sobe e desce"
		headPosAux = fmod(headPosAux, 90) - 1 * headPosAux / 90;
		headPosAux -= 4.0f;
		if (headPosAux < 0.0f) {
			headPosAux = 0.0f;
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
		jumping = false;
	} else {
		speedY -= gravity;

	}

	if (crouched) {
		posYOffset -= 0.01;
		if (posYOffset < 0.1) {
			posYOffset = 0.1;
		}
	} else {
		posYOffset += 0.01;
		if (posYOffset > 0.2) {
			posYOffset = 0.2;
		}
	}

}

/**
Render scene
*/
void mainRender() {
	updateState();
	renderScene();
	updateCamera();
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
void onMousePassiveMove(int x, int y) {

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

/**
Key press event handler
*/
void onKeyDown(unsigned char key, int x, int y) {
	//printf("%d \n", key);
	switch (key) {
		case 32: //space
			if (!spacePressed && !jumping) {
				jumping = true;
				speedY = jumpSpeed;
			}
			spacePressed = true;
			break;
		case 119: //w
			if (!upPressed) {
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
		case 100: //d
			rightPressed = true;
			break;
		case 99: //c
			crouched = true;
			break;
		case 114: //r
			running = true;
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
	switch (key) {
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
		case 97: //a
			leftPressed = false;
			break;
		case 100: //d
			rightPressed = false;
			break;
		case 99: //c
			crouched = false;
			break;
		case 114: //r
			running = false;
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
    posicaoJogador.y = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(windowWidth,windowHeight);
	glutInitWindowPosition(windowXPos,windowYPos);

	/**
	Store main window id so that glui can send it redisplay events
	*/
	mainWindowId = glutCreateWindow("FPS");

	glutDisplayFunc(mainRender);

	glutReshapeFunc(onWindowReshape);

	/**
	Register mouse events handlers
	*/
	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMove);
	glutPassiveMotionFunc(onMousePassiveMove);

	/**
	Register keyboard events handlers
	*/
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);


    mapa = FileMapReader::generateMapBitmap("C:/Users/Usuario/Desktop/mapas");
	mainInit();


	glutMainLoop();

    return 0;
}
