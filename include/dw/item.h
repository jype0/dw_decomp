#ifndef DW_ITEM_H
#define DW_ITEM_H

#include <libgte.h>

#include <dw/types.h>

typedef struct
{
	char name[20];
	int32_t value;
	uint16_t meritValue;
	int16_t sortingValue;
	uint8_t itemColor;
	uint8_t droppable;
	uint16_t unk;
} Item;

typedef struct {
	SVECTOR spriteLocation;
	int32_t type;
} WorldItem;

typedef struct {
	WorldItem worldItem;
	int32_t time;
} TamerItem;

extern Item ITEM_PARA[];

#endif
