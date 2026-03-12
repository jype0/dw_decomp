#ifndef DW_FILE_H
#define DW_FILE_H

#include <libcd.h>

#include <dw/types.h>

typedef struct {
	int32_t sector;
	int32_t size;
	char *filename;
} FileEntry;

typedef struct {
	CdlLOC pos;
	uint32_t size;
	uint32_t unk2;
	uint32_t unk3;
	uint32_t unk4;
	uint32_t unk5;
} FileLookup;

extern FileEntry FILE_ENTRIES[];

int32_t lookupFileTable(FileLookup *lookup, const char *path);
uint32_t lookupFileSize(const char *path);
int32_t loadTextureFile(const char *path, uint32_t *outTPage,
			uint32_t *outClut);
int32_t readFile(const char *path, void *buffer);
int32_t loadTIMFile(char *path, void *buffer);
int32_t loadStackedTIMEntry(const char *path, void *buffer, int offset,
			    int sectors);
int readFileSectors(const char *path, void *buffer, int offset, int sectors);

#endif
