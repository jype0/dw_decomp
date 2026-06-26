#ifndef DW_ITEM_H
#define DW_ITEM_H

#include <libgte.h>

#include <dw/types.h>

typedef struct {
	SVECTOR spriteLocation;
	int32_t type;
} WorldItem;

typedef struct {
	WorldItem worldItem;
	int32_t time;
} TamerItem;

#endif
