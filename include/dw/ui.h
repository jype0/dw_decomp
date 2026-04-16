#ifndef DW_UI_H
#define DW_UI_H

#include <libgpu.h>
#include <dw/graphics.h>
#include <dw/types.h>
#include <dw/world_object.h>

typedef struct {
	RECT startPos;
	RECT finalPos;
	int16_t frame;
	int16_t state;
	int16_t rowOffset;
	int16_t visibleRows;
	int16_t totalRows;
	uint8_t features;
	uint8_t color;
	TickFunction tick;
	RenderFunction render;
} UIBoxData;

typedef struct {
	RECT rect1;
	RECT rect2;
	uint8_t unk_0x10;
	uint8_t unk_0x11;
	uint8_t unk_0x12;
	uint8_t unk_0x13;
	uint8_t unk_0x14;
	uint8_t unk_0x15;
	uint8_t unk_0x16;
	uint8_t unk_0x17;
	uint32_t unk_0x18;
	uint32_t unk_0x1c;
	uint32_t unk_0x20;
	uint32_t unk_0x24;
	uint32_t unk_0x28;
	TickFunction tick;
	RenderFunction render;
} TextBoxData;

extern RGB8 UI_BOX_COLORS[];
extern UIBoxData UI_BOX_DATA[6];
extern TextBoxData TEXT_BOX_DATA[6];

void initializeUIBoxData(void);
void createStaticUIBox(int32_t id, uint8_t color, uint8_t features, RECT *pos,
		       TickFunction tickFunc, RenderFunction renderFunc);
void removeStaticUIBox(int32_t id);
void createAnimatedUIBox(int32_t instanceId, uint8_t color, uint8_t features,
			 RECT *finalPos, RECT *startPos,
			 TickFunction tickFunc, RenderFunction renderFunc);
void removeAnimatedUIBox(int32_t id, RECT *target);
void renderUIBoxBorder(RECT *size, int32_t layer);

#endif
