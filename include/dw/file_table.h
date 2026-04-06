#ifndef DW_FILE_TABLE_H
#define DW_FILE_TABLE_H

#include <dw/types.h>
extern int16_t ATAN_LOOKUP_TABLE[];
extern int16_t SIN_LOOKUP_TABLE[];

typedef struct {
    int32_t sector;
    int32_t size;
    char *filename;
} FileEntry;

extern FileEntry FILE_TABLE[];

#endif
