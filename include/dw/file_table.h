#ifndef DW_FILE_TABLE_H
#define DW_FILE_TABLE_H

#include <dw/types.h>

typedef struct FILE 
{
    int32_t sector;
    int32_t size;
    int8_t* name;
} FILE;

extern FILE FILE_TABLE[];

#endif
