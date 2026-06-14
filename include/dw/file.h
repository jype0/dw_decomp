#ifndef DW_FILE_H
#define DW_FILE_H

#include <libcd.h>

#include <dw/types.h>

typedef struct {
	CdlLOC pos;
	uint32_t size;
	uint32_t unk2;
	uint32_t unk3;
	uint32_t unk4;
	uint32_t unk5;
} FileLookup;

int32_t lookupFileTable(FileLookup *lookup, char *path);
uint32_t lookupFileSize(char *path);
int32_t loadTextureFile(char *path, uint32_t *outTPage, uint32_t *outClut);
int32_t readFile(char *path, void *buffer);
int32_t loadTIMFile(char *path, void *buffer);
int32_t loadStackedTIMEntry(char *path, void *buffer, int32_t offset,
			    int32_t sectors);
int32_t readFileSectors(char *path, void *buffer, int32_t offset,
			int32_t sectors);

#endif
