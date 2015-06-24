#ifndef FILEMAPREADER_H_INCLUDED
#define FILEMAPREADER_H_INCLUDED

#include <string.h>
#include <string>
#include <stdlib.h>
#include "GridBitmap.h"
#include "MapaBitmap.h"


class FileMapReader
{
    public:
        FileMapReader();
        static MapaBitmap generateMapBitmap(std::string path);

};






#endif // FILEMAPREADER_H_INCLUDED
