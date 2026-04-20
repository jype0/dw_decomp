#ifndef UTILS_H
#define UTILS_H

#include <dw/types.h>

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

void swapByte(uint8_t *a, uint8_t *b);
void swapShort(int16_t *a, int16_t *b);
void swapInt(int32_t *a, int32_t *b);

void loadDynamicLibrary(Overlay lib, uint8_t* isComplete, dw_bool isAsync, FileCallback callback, void* param); 

#endif
