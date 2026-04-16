#include <libgs.h>
#include <dw/ui.h>

#include "common.h"

void tickUIBox(int32_t instanceId);
void renderUIBoxStatic(int32_t instanceId);
void renderUIBoxAnimated(int32_t instanceId);
void renderUIBoxAnim(int32_t instanceId, int32_t frame);

void playSound(int32_t soundId, uint32_t flag);
void renderTrianglePrimitive(int32_t color, int32_t x0, int32_t y0,
			     int32_t x1, int32_t y1, int32_t x2, int32_t y2,
			     int32_t layer, int32_t blend);
void setUVDataPolyFT4(PACKET *prim, int32_t uvX, int32_t uvY,
		      int32_t uvWidth, int32_t uvHeight);
void setPosDataPolyFT4(PACKET *prim, int32_t posX, int32_t posY,
		       int32_t width, int32_t height);
void renderLinePrimitive(int32_t color, int32_t x0, int32_t y0,
			 int32_t x1, int32_t y1,
			 int32_t layer, int32_t blend);

extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint8_t MAIN_D_80134330[4];
extern uint8_t MAIN_D_80134334[4];

static void *ui_functions[] = {
	renderUIBoxAnim,
	renderUIBoxBorder,
	removeAnimatedUIBox,
	renderUIBoxAnimated,
	createAnimatedUIBox,
	removeStaticUIBox,
	renderUIBoxStatic,
	tickUIBox,
	createStaticUIBox,
	initializeUIBoxData,
};

void initializeUIBoxData(void)
{
	int32_t i;

	for (i = 0; i < 6; i++) {
		UI_BOX_DATA[i].frame = 0;
		UI_BOX_DATA[i].state = 0;
	}
}

void tickUIBox(int32_t instanceId)
{
	if (((UI_BOX_DATA[instanceId].state == 1) ||
	     (UI_BOX_DATA[instanceId].state == 4)) &&
	    UI_BOX_DATA[instanceId].tick != NULL) {
		UI_BOX_DATA[instanceId].tick(instanceId);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/ui", renderUIBoxStatic);

static void renderUIBoxStatic__garbage__(int32_t instanceId)
{
	UIBoxData *data;
	RECT *rect;
	int32_t iVar2;
	int32_t layer;
	int32_t iVar5;
	int32_t iVar6;
	int32_t iVar9;
	int16_t sVar1;
	int16_t rowHeight;
	PACKET *p;
	GsBOXF box;

	data = &UI_BOX_DATA[instanceId];
	rect = &data->finalPos;
	if (data->render != NULL) {
		data->render(instanceId);
	}
	renderUIBoxBorder(rect, layer = 6 - instanceId);
	iVar2 = rect->y;
	iVar6 = rect->x + 3;
	renderLinePrimitive(0x20202, iVar6, iVar2 + 3, iVar6,
			    iVar2 + rect->h - 3, layer, 0);
	iVar2 = rect->y;
	iVar6 = rect->x + rect->w - 4;
	renderLinePrimitive(0x20202, iVar6, iVar2 + 3, iVar6,
			    iVar2 + rect->h - 3, layer, 0);
	if (data->features & 1) {
		iVar2 = rect->y + 13;
		renderLinePrimitive(0x20202, rect->x + 3, iVar2,
				    rect->x + rect->w - 3, iVar2, layer, 0);
		iVar2 = rect->y + 14;
		renderLinePrimitive(0xfad990, rect->x + 3, iVar2,
				    rect->x + rect->w - 3, iVar2, layer, 0);
		iVar2 = rect->y + 15;
		renderLinePrimitive(0x20202, rect->x + 3, iVar2,
				    rect->x + rect->w - 3, iVar2, layer, 0);
	}
	if (data->features & 4) {
		iVar6 = rect->y + 13;
		iVar2 = rect->x + rect->w;
		renderTrianglePrimitive(0x20202, iVar2 - 13,
					rect->y + rect->h - 10, iVar2 - 13, iVar6,
					iVar2 - 6, iVar6, layer, 0);
		iVar6 = rect->y + rect->h - 10;
		iVar2 = rect->x + rect->w;
		renderTrianglePrimitive(0xa08769, iVar2 - 6, rect->y + 14, iVar2 - 6,
					iVar6, iVar2 - 12, iVar6, layer, 0);
		iVar2 = (rect->h - 25) * 0x10000 >> 0x10;
		rowHeight = (int16_t)((iVar2 * (int16_t)UI_BOX_DATA[instanceId].visibleRows) /
				(int32_t)(int16_t)UI_BOX_DATA[instanceId].totalRows);
		iVar9 = (rect->y + 14 +
			((iVar2 - rowHeight) * (int32_t)(int16_t)UI_BOX_DATA[instanceId].rowOffset) /
			((int32_t)(int16_t)UI_BOX_DATA[instanceId].totalRows -
			(int32_t)(int16_t)UI_BOX_DATA[instanceId].visibleRows)) * 0x10000 >> 0x10;
		iVar5 = iVar9 + rowHeight;
		iVar2 = rect->x + rect->w;
		iVar6 = iVar2 - 7;
		renderTrianglePrimitive(0x20202, iVar6, iVar9, iVar6, iVar5,
					iVar2 - 13, iVar5, layer, 0);
		iVar2 = rect->x + rect->w;
		iVar6 = iVar2 - 12;
		renderTrianglePrimitive(0xa08769, iVar6, iVar5, iVar6, iVar9,
					iVar2 - 6, iVar9, layer, 0);
		p = GsGetWorkBase();
		SetPolyF4((POLY_F4 *)p);
		p[4] = 0x5b;
		p[5] = 0x70;
		p[6] = 0x80;
		*(int16_t *)(p + 8) = rect->x + rect->w - 11;
		sVar1 = (int16_t)(iVar9 + 1);
		*(int16_t *)(p + 10) = sVar1;
		*(int16_t *)(p + 12) = rect->x + rect->w - 7;
		*(int16_t *)(p + 14) = sVar1;
		*(int16_t *)(p + 16) = rect->x + rect->w - 11;
		sVar1 = sVar1 + rowHeight - 1;
		*(int16_t *)(p + 18) = sVar1;
		*(int16_t *)(p + 20) = rect->x + rect->w - 7;
		*(int16_t *)(p + 22) = sVar1;
		AddPrim(ACTIVE_ORDERING_TABLE->org + 6 - instanceId, p);
		SetPolyF4((POLY_F4 *)(p + 24));
		p[28] = 0x35;
		p[29] = 0x4b;
		p[30] = 0x5c;
		*(int16_t *)(p + 32) = rect->x + rect->w - 12;
		*(int16_t *)(p + 34) = rect->y + 14;
		*(int16_t *)(p + 36) = rect->x + rect->w - 6;
		*(int16_t *)(p + 38) = rect->y + 14;
		*(int16_t *)(p + 40) = rect->x + rect->w - 12;
		*(int16_t *)(p + 42) = rect->y + rect->h - 10;
		*(int16_t *)(p + 44) = rect->x + rect->w - 6;
		*(int16_t *)(p + 46) = rect->y + rect->h - 10;
		AddPrim(ACTIVE_ORDERING_TABLE->org + 6 - instanceId, p + 24);
		GsSetWorkBase(p + 48);
	}
	if (data->features & 2) {
		box.attribute = 0x40000000;
	} else {
		box.attribute = 0;
	}
	box.r = UI_BOX_COLORS[data->color].r;
	box.g = UI_BOX_COLORS[data->color].g;
	box.b = UI_BOX_COLORS[data->color].b;
	box.x = rect->x + 4;
	box.y = rect->y + 3;
	box.w = rect->w - 8;
	box.h = rect->h - 3;
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, (uint16_t)layer);
}

void createStaticUIBox(int32_t id, uint8_t color, uint8_t features, RECT *pos,
		       TickFunction tickFunc, RenderFunction renderFunc)
{
	UIBoxData *data;

	data = &UI_BOX_DATA[id];
	data->state = 1;
	data->color = color;
	data->features = features;
	data->render = renderFunc;
	data->tick = tickFunc;
	data->finalPos = *pos;
	addObject(0x1a4, id, tickUIBox, renderUIBoxStatic);
}

void removeStaticUIBox(int32_t id)
{
	UI_BOX_DATA[id].state = 0;
	UI_BOX_DATA[id].frame = 0;
	removeObject(0x1a4, id);
}

void createAnimatedUIBox(int32_t instanceId, uint8_t color, uint8_t features,
			 RECT *finalPos, RECT *startPos,
			 TickFunction tickFunc, RenderFunction renderFunc)
{
	UIBoxData *data;

	data = &UI_BOX_DATA[instanceId];
	playSound(0, 0);
	data->state = 2;
	data->color = color;
	data->features = features;
	data->startPos = *(RECT *)startPos;
	data->finalPos = *(RECT *)finalPos;
	data->render = renderFunc;
	data->tick = tickFunc;
	addObject(0x1a4, instanceId, tickUIBox, renderUIBoxAnimated);
}

void renderUIBoxAnimated(int32_t instanceId)
{
	UIBoxData *data;
	int16_t state;
	int16_t currentFrame;

	data = &UI_BOX_DATA[instanceId];
	state = data->state;
	if ((state == 2) || (data->state == 3)) {
		if (data->state == 2) {
			currentFrame = data->frame;
			data->frame = currentFrame + 1;
		} else {
			currentFrame = --data->frame;
		}
		renderUIBoxAnim(instanceId, currentFrame);
		if (data->frame > 4) {
			data->state = 1;
		}
		if (!data->frame) {
			removeStaticUIBox(instanceId);
		}
	} else {
		renderUIBoxStatic(instanceId);
	}
}

void removeAnimatedUIBox(int32_t id, RECT *target)
{
	if (target != NULL) {
		UI_BOX_DATA[id].startPos = *target;
	}
	UI_BOX_DATA[id].state = 3;
}

INCLUDE_ASM("asm/main/nonmatchings/ui", renderUIBoxBorder);

INCLUDE_ASM("asm/main/nonmatchings/ui", renderUIBoxAnim);
