#ifndef FILEMAPREADER_H_INCLUDED
#define FILEMAPREADER_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include "GridBitmap.h"


class FileMapReader
{
    public:
        FileMapReader();
        static GridBitmap* generateMapBitmap(char* path);

};






#endif // FILEMAPREADER_H_INCLUDED
