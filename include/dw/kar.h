#ifndef KAR_H
#define KAR_H

#include <dw/types.h>

/* there might be some better place the two typedefs next? */

typedef enum Overlay
{
    NONE     = 0,
    BTL_REL  = 1,
    STD_REL  = 2,
    FISH_REL = 3,
    EVL_REL  = 4,
    KAR_REL  = 5,
    VS_REL   = 6,
    MOV_REL  = 7,
    DOO2_REL = 8,
    DOOA_REL = 9,
    TRN_REL  = 10,
    SHOP_REL = 11,
    DGET_REL = 12,
    TRN2_REL = 13,
    MURD_REL = 14,
    ENDI_REL = 15,
    EAB_REL  = 16,
    OFFSET   = -1,
} Overlay;

typedef void (*FileCallback)();

void loadDynamicLibrary(Overlay id, void* lib, bool isAsync, FileCallback callback, void* param); // TODO: move this prototype to the header of the file which defines the function
void checkCurlingMap(int32_t mapId);

#endif
