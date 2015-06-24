#ifndef AndarBitmap_H
#define AndarBitmap_H

#include "GridBitmap.h"
class AndarBitmap
{
    public:
        AndarBitmap();
        AndarBitmap(GridBitmap* superior, GridBitmap* inferior);

        GridBitmap* superior;
        GridBitmap* inferior;
    protected:
    private:

};

#endif // AndarBitmap_H
