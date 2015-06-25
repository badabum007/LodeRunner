#ifndef FILEMAPREADER_H_INCLUDED
#define FILEMAPREADER_H_INCLUDED

#include <string.h>
#include <string>
#include <stdlib.h>
#include "GridBitmap.h"
#include "Mapa.h"


class FileMapReader
{
    public:
        FileMapReader();
        static Mapa* generateMapBitmap(std::string path);

};






#endif // FILEMAPREADER_H_INCLUDED
