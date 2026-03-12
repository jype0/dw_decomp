#ifndef DW_FILE_TABLE_H
#define DW_FILE_TABLE_H

#include <dw/types.h>

typedef struct {
    int32_t sector;
    int32_t size;
    char *filename;
} FileEntry;

extern FileEntry FILE_TABLE[];

#endif
