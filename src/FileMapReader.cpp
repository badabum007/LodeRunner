#include "FileMapReader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


FileMapReader::FileMapReader()
{
}

char intToStr(int a)
{
    switch(a)
    {
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return '10';
        default: return -1;

    }

}


GridBitmap* generateMapBitmap(char* path)
{
   FILE* arq;
    char pathaux[200];
    int andar = 1;
    int grid = 0;
    int i, j;
    char aux;

    do
    {
        strcat(pathaux, path);
        aux = intToStr(andar);
        strcat(pathaux, &aux);
        aux = intToStr(grid);
        strcat(pathaux, &aux);
        strcat(pathaux, ".bmp");
        arq = fopen(pathaux, "rb");
        for(i=0;i<GRID_HEIGHT;i++)
        {
            for(j-0;j<GRID_WIDTH;j++)
            {


            }
        }

    }while(arq != NULL);

    return NULL;

}


