#include "../include/FileMapReader.h"
#include "../include/bitmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream>

#include "../include/Mapa.h"
#include "../include/Andar.h"


FileMapReader::FileMapReader()
{
}

std::string to_string(int value)
    {
      //create an output string stream
      std::ostringstream os ;

      //throw the value into the string stream
      os << value ;

      //convert the string stream into a string and return
      return os.str() ;
    }

GridBitmap* ReadBMP(std::string filename)
{
    int i;
    GridBitmap* grid = new GridBitmap();
    const char* str  = filename.c_str();
    FILE* f = fopen(str, "rb");

    if(f == NULL)
        throw std::exception();

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

   /* cout << endl;
    cout << "  Name: " << filename << endl;
    cout << " Width: " << width << endl;
    cout << "Height: " << height << endl;*/

    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    for(int i = 0; i < height; i++)
    {
        std::vector<ObjEnum> linha;
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;
            Color cor = Color((uint8_t) data[j], (uint8_t) data[j+1], (uint8_t) data[j+2]);

            if(cor == PRINCIPAL_COLOR)
                linha.push_back(ObjEnum::PRINCIPAL);
            if(cor == INIMIGO_COLOR)
                linha.push_back(ObjEnum::INIMIGO);
            if(cor == BLOCOINDEST_COLOR)
                linha.push_back(ObjEnum::BLOCOINDEST);
            if(cor == BLOCODEST_COLOR)
                linha.push_back(ObjEnum::BLOCODEST);
            if(cor == OURO_COLOR)
                linha.push_back(ObjEnum::OURO);
            if(cor == ESCADA_COLOR)
                linha.push_back(ObjEnum::ESCADA);
            if(cor == VAZIO_COLOR)
                linha.push_back(ObjEnum::VAZIO);


             std::cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< std::endl;
        }
        grid->grid.push_back(linha);
    }

    fclose(f);
    return grid;
}

Mapa FileMapReader::generateMapBitmap(std::string path)
{
    int AndarBitmapInt=1;
    int nivel=0;
    std::string pathAux;
    Mapa MapaBitmap;


    try
    {
        while(true)
        {
             Andar AndarBitmap;
            for(nivel=0;nivel<2;nivel++)
            {
                //pathAux = "C:/Users/Usuario/Desktop/MapaBitmaps/10.bmp";
                pathAux = path + "/" + to_string(AndarBitmapInt) + to_string(nivel) + ".bmp";
                GridBitmap* data = ReadBMP(pathAux);
                if(nivel == 0)
                    AndarBitmap.inferior = data;
                else
                    AndarBitmap.superior = data;
            }
            AndarBitmapInt++;
            MapaBitmap.andares.push_back(AndarBitmap);
        }

    }catch(const std::exception& e)
    {

    }

    return MapaBitmap;


}


