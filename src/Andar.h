#ifndef ANDAR_H
#define ANDAR_H

#include "GridBitmap.h"
class Andar
{
    public:
        Andar();
        Andar(GridBitmap superior, GridBitmap inferior);

        GridBitmap Getsuperior() { return superior; }
        GridBitmap Getinferior() { return inferior; }
    protected:
    private:
        GridBitmap superior;
        GridBitmap inferior;
};

#endif // ANDAR_H
