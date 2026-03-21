#ifndef DW_UI_H
#define DW_UI_H

#include <libgpu.h>
#include <dw/types.h>
#include <dw/world_object.h>

typedef struct {
	RECT startPos;
	RECT finalPos;
	uint16_t frame;
	int16_t state;
	uint16_t rowOffset;
	uint16_t visibleRows;
	uint16_t totalRows;
	uint8_t features;
	uint8_t color;
	TickFunction tick;
	RenderFunction render;
} UIBoxData;

extern UIBoxData UI_BOX_DATA[6];

#endif
