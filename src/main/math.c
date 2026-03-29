#include <dw/math.h>
#include <rand.h>

#pragma optimization_level 4

int32_t random(int32_t limit)
{
    return (limit * rand()) >> 15;
}

int32_t _sin(int32_t input)
{
    input &= 0x1FF;

    if ((0 <= input) && (input < 128))
        return SIN_LOOKUP_TABLE[input];
    else if ((input >= 128) && (input < 256))
        return SIN_LOOKUP_TABLE[256 - input];
    else if ((input >= 256) && (input < 384))
        return -SIN_LOOKUP_TABLE[input - 256];
    else if ((input >= 384) && (input < 512))
        return -SIN_LOOKUP_TABLE[512 - input];
} // NOLINT undefined behavior intentional

int32_t _cos(int32_t input)
{
    return _sin(input + 128);
}

int32_t _atan(int32_t dy, int32_t dx)
{
    int32_t generalDirection;
    int32_t offset;
    int32_t tmpVal;

    if (dy < 0) {
        if (dx < 0) generalDirection = 2;
        else generalDirection = 1;
    } else {
        if (dx < 0) generalDirection = 3;
        else generalDirection = 0;
    }

    dy = ABS(dy);
    dx = ABS(dx);

    if (dy >= dx)
    {
        int32_t tmpDx = dx; 

        tmpDx *= 512; 
        if (dy > 0)
            offset = tmpDx / dy;
        else
            offset = 0;
        if (offset > 511) offset = 511;
        tmpVal = ATAN_LOOKUP_TABLE[offset];
    }
    else
    {
        int32_t tmpDy = dy; 

        tmpDy *= 512; 
        if (dx > 0)
            offset = tmpDy / dx; 
        else
            offset = 0;
        if (offset > 511) offset = 511;
        tmpVal = 1023 - ATAN_LOOKUP_TABLE[offset];
    }

    if ((generalDirection == 1) || (generalDirection == 2)) tmpVal = 2048 - tmpVal;
    if (generalDirection >= 2) tmpVal = -tmpVal;

    return (tmpVal + 2048) & 0xFFF;
}
