#ifndef DW_GRAPHICS_H
#define DW_GRAPHICS_H

#include <libgs.h>

#include <dw/types.h>

typedef struct {
	uint8_t r, g, b;
} RGB8;

/* Hack to match overlay .bss */
typedef union {
	uint8_t raw[32];
	MATRIX m;
} MATRIXRaw;

/* Hack to match overlay .bss */
typedef union {
	uint8_t raw[80];
	GsCOORDINATE2 coord;
} GsCOORDINATE2Raw;

#endif
