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
	RECT origin;
	RECT rect;
	uint8_t flags;
	uint8_t writeCount;
	uint8_t renderCount;
	uint8_t flipCount;
	uint8_t registered;
	uint8_t pageReady;
	uint8_t idle;
	uint8_t doubleBuffered;
	uint32_t backPage;
	uint32_t vramMode;
	uint32_t vramRow;
	uint32_t vramRows;
	uint32_t writeRow;
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
