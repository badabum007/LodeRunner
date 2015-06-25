#ifndef Andar_H
#define Andar_H

#include "GridBitmap.h"
class Andar
{
    public:
        Andar();
        Andar(GridBitmap* superior, GridBitmap* inferior);

        GridBitmap* superior;
        GridBitmap* inferior;
    protected:
    private:

};

#endif // Andar_H
