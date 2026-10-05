#include <libcd.h>
#include <libgpu.h>
#include <libgs.h>
#include <string.h>
#include <sys/types.h>
#include <dw/file.h>
#include <dw/file_table.h>
#include <dw/main.h>

#include "common.h"

static void empty_800a31f0(char *path)
{
}

int32_t lookupFileTable(FileLookup *lookup, char *path)
{
	char *filename;
	FileEntry *entry;

	entry = (FileEntry *)&FILE_OFFSET_TABLE;
	if (NULL == (filename = strrchr(path, '\\'))) {
		filename = path;
	} else {
		++filename;
	}

	for (; entry->size != 0; ++entry) {
		if (!strcmp(filename, entry->filename)) {
			lookup->size = entry->size;
			CdIntToPos(entry->sector, &lookup->pos);
			break;
		}
	}

	if (entry->size) {
		return 1;
	} else {
		empty_800a31f0(path);
		return 0;
	}
}

uint32_t lookupFileSize(char *path)
{
	FileLookup lookup;

	lookupFileTable(&lookup, path);

	return lookup.size;
}

int32_t loadTextureFile(char *path, uint32_t *outTPage, uint32_t *outClut)
{
	u_long *addr;
	GsIMAGE image;
	RECT rect;
	int32_t size;
	u_long *buf;

	size = lookupFileSize(path);
	addr = buf = (u_long *)TEXTURE_BUFFER;
	readFile(path, buf);

	GsGetTimInfo(buf + 1, &image);

	setRECT(&rect, image.px, image.py, image.pw, image.ph);
	LoadImage(&rect, image.pixel);

	*outTPage = GetTPage(image.pmode & 3, 0, image.px, image.py);

	if (((image.pmode >> 3) & 1) != 0) {
		setRECT(&rect, image.cx, image.cy, image.cw, image.ch);
		LoadImage(&rect, image.clut);

		*outClut = GetClut(image.cx, image.cy);
	}
}

int32_t readFile(char *path, void *buffer)
{
	FileLookup lookup;
	int32_t result;

	if (lookupFileTable(&lookup, path) != 0) {
		do {
			do {
				while (CdControl(CdlSetloc,
						 (u_char *)&lookup.pos,
						 NULL) == 0);
			} while (CdRead(((lookup.size + 0x7ff) & ~0x7ff) >> 11,
					(u_long *)buffer, CdlModeSpeed) == 0);
			while ((result = CdReadSync(0, NULL)) > 0);
		} while (result != 0);
	}
}

int32_t loadTIMFile(char *path, void *buffer)
{
	GsIMAGE image;
	RECT rect;

	readFile(path, buffer);

	GsGetTimInfo((u_long *)buffer + 1, &image);

	setRECT(&rect, image.px, image.py, image.pw, image.ph);
	LoadImage(&rect, image.pixel);

	if (((image.pmode >> 3) & 1) != 0) {
		setRECT(&rect, image.cx, image.cy, image.cw, image.ch);
		LoadImage(&rect, image.clut);
	}
}

int32_t loadStackedTIMEntry(char *path, void *buffer, int32_t offset,
			    int32_t sectors)
{
	GsIMAGE image;
	RECT rect;

	readFileSectors(path, buffer, offset, sectors);
	GsGetTimInfo((u_long *)buffer + 1, &image);

	setRECT(&rect, image.px, image.py, image.pw, image.ph);
	LoadImage(&rect, image.pixel);

	if (((image.pmode >> 3) & 1) != 0) {
		setRECT(&rect, image.cx, image.cy, image.cw, image.ch);
		LoadImage(&rect, image.clut);
	}
}

int32_t readFileSectors(char *path, void *buffer, int32_t offset,
			int32_t sectors)
{
	FileLookup lookup;
	int32_t result;

	if (lookupFileTable(&lookup, path) != 0) {
		CdIntToPos(offset + CdPosToInt(&lookup.pos), &lookup.pos);
		do {
			do {
				while (CdControl(CdlSetloc,
						 (u_char *)&lookup.pos,
						 NULL) == 0);
			} while (CdRead(sectors, buffer, CdlModeSpeed) == 0);
			while ((result = CdReadSync(0, NULL)) > 0);
		} while (result != 0);
	}
}
