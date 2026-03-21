#ifndef DW_MAP_H
#define DW_MAP_H

#include <libgte.h>
#include <libgs.h>

typedef struct {
	int16_t posX1;
	int16_t posY1;
	int16_t posX2;
	int16_t posY2;
} ToiletData;

extern ToiletData TOILET_DATA[];

typedef struct
{
	char filename[10];
	int8_t num8bppImages;
	int8_t num4bppImages;
	int8_t flags;
	int8_t doorsId;
	int8_t toiletId;
	int8_t loadingName;
} MapEntry;

extern MapEntry MAP_ENTRIES[];

extern VECTOR TOILET_POS1;
extern VECTOR TOILET_POS2;

#endif
