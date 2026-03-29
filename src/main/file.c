#include <libcd.h>
#include <libgpu.h>
#include <libgs.h>
#include <string.h>
#include <sys/types.h>
#include <dw/file.h>
#include <dw/file_table.h>

#include "common.h"

#pragma optimization_level 4

/* TODO: Move to a header file */
#define TEXTURE_BUFFER		0x80088800

static void empty_800a31f0(const char *path)
{
}

int32_t lookupFileTable(FileLookup *lookup, const char *path)
{
	const char *filename;
	int32_t result;
	FileEntry *entry;

	entry = (FileEntry *)&FILE_TABLE;
	if ((filename = strrchr(path, '\\')) == NULL) {
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

	result = entry->size;
	if (result) {
		return 1;
	} else {
		empty_800a31f0(path);
		return 0;
	}
}

uint32_t lookupFileSize(const char *path)
{
	FileLookup lookup;

	lookupFileTable(&lookup, path);

	return lookup.size;
}

int32_t loadTextureFile(const char *path, uint32_t *outTPage,
			uint32_t *outClut)
{
	GsIMAGE image;
	RECT rect;
	int32_t result;
	uint32_t tpage;
	uint32_t clut;

	lookupFileSize(path);
	result = readFile(path, (void *)TEXTURE_BUFFER);

	GsGetTimInfo(&((u_long *)TEXTURE_BUFFER)[1], &image);

	rect.x = image.px;
	rect.y = image.py;
	rect.w = image.pw;
	rect.h = image.ph;
	result = LoadImage(&rect, image.pixel);

	result = tpage = GetTPage(image.pmode & 3, 0, image.px, image.py);
	*outTPage = tpage;

	if (((image.pmode >> 3) & 1) != 0) {
		rect.x = image.cx;
		rect.y = image.cy;
		rect.w = image.cw;
		rect.h = image.ch;
		result = LoadImage(&rect,image.clut);

		result = clut = GetClut(image.cx, image.cy);
		*outClut = clut;
	}

	return result;
}

int32_t readFile(const char *path, void *buffer)
{
	FileLookup lookup;
	uint32_t sector;
	int32_t result;
	int32_t err;

	result = err = lookupFileTable(&lookup, path);
	if (err != 0) {
		do {
			do {
				while (CdControl(CdlSetloc,
						 (u_char *)&lookup.pos,
						 NULL) == 0);
				sector = ((lookup.size + 0x7ff) &
					  ~0x7ff) >> 11;
			} while (CdRead(sector, (u_long *)buffer,
					CdlModeSpeed) == 0);
			while ((result = CdReadSync(0, NULL)) > 0);
		} while (result != 0);

		return result;
	}
} // NOLINT undefined behavior intentional

int32_t loadTIMFile(char *path, void *buffer)
{
	GsIMAGE image;
	RECT rect;
	int32_t result;

	result = readFile(path,(char *)buffer);

	GsGetTimInfo(&((u_long *)buffer)[1], &image);

	rect.x = image.px;
	rect.y = image.py;
	rect.w = image.pw;
	rect.h = image.ph;
	result = LoadImage(&rect, image.pixel);

	if (((image.pmode >> 3) & 1) != 0) {
		rect.x = image.cx;
		rect.y = image.cy;
		rect.w = image.cw;
		rect.h = image.ch;
		result = LoadImage(&rect,image.clut);
	}

	return result;
}

int32_t loadStackedTIMEntry(const char *path, void *buffer, int32_t offset,
			    int32_t sectors)
{
	GsIMAGE image;
	RECT rect;
	int32_t result;

	result = readFileSectors(path, buffer, offset, sectors);
	GsGetTimInfo(&((u_long *)buffer)[1], &image);

	rect.x = image.px;
	rect.y = image.py;
	rect.w = image.pw;
	rect.h = image.ph;
	result = LoadImage(&rect,image.pixel);

	if (((image.pmode >> 3) & 1) != 0) {
		rect.x = image.cx;
		rect.y = image.cy;
		rect.w = image.cw;
		rect.h = image.ch;
		result = LoadImage(&rect,image.clut);
	}

	return result;
}

int32_t readFileSectors(const char *path, void *buffer, int32_t offset,
			int32_t sectors)
{
	FileLookup lookup;
	int32_t sector;
	int32_t result;
	int32_t err;

	result = err = lookupFileTable(&lookup, path);
	if (err != 0) {
		sector = CdPosToInt(&lookup.pos);
		CdIntToPos(offset + sector, &lookup.pos);
		do {
			do {
				while (CdControl(CdlSetloc,
						 (u_char *)&lookup.pos,
						 NULL) == 0);
			} while (CdRead(sectors, buffer, CdlModeSpeed) == 0);
			while ((result = CdReadSync(0, NULL)) > 0);
		} while (result != 0);

		return result;
	}
} // NOLINT undefined behavior intentional
