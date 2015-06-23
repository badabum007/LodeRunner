#ifndef ANDAR_H
#define ANDAR_H

#include "GridBitmap.h"
class Andar
{
    public:
        Andar();
        Andar(GridBitmap superior, GridBitmap inferior);

        GridBitmap superior;
        GridBitmap inferior;
    protected:
    private:

};

#endif // ANDAR_H
