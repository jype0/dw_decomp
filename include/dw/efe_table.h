#ifndef DW_EFE_TABLE_H
#define DW_EFE_TABLE_H

#include <dw/types.h>
#include <libcd.h>

typedef struct {
	CdlLOC pos;
	uint32_t size;
} EFEX_FileLookup;

#define MAX_EFEX_FILES 128
#define NUM_EFEX_FILES 3


extern EFEX_FileLookup EFEX_TABLE[MAX_EFEX_FILES];
extern char* EFEX_PATH_ARRAY[NUM_EFEX_FILES];

void fillEFEXTable(void);

#endif
