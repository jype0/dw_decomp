#include <libgpu.h>
#include <libgs.h>
#include <dw/ui.h>

#include "common.h"

void tickUIBox(int32_t instanceId);
void renderUIBoxStatic(int32_t instanceId);
void renderUIBoxAnimated(int32_t instanceId);
void renderUIBoxAnim(int32_t instanceId, int16_t frame);
void renderUIBoxBorder(RECT *rect, int32_t layer);

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

// clang-format off
uint8_t MAIN_D_80134330[4] = {
	0x78, 0x7c, 0x78, 0x7c,
};

uint8_t MAIN_D_80134334[4] = {
	0x10, 0x10, 0x14, 0x14,
};

char MOVE_NAME_BUG[4] = "Bug";

char MOVE_NAME_TREMAR[] = "Tremar";

char MOVE_NAME_WAR_CRY[8] = "War Cry";

char MOVE_NAME_COUNTER[8] = "Counter";

char MOVE_NAME_BUBBLE[] = "Bubble";
// clang-format on

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

void renderUIBoxStatic(int32_t instanceId)
{
	UIBoxData *data;
	int32_t id;
	RECT *rect;
	int32_t layer;
	int16_t h25;
	int16_t rowHeight;
	int16_t barTop;
	POLY_F4 *p;
	GsBOXF box;

	data = &UI_BOX_DATA[instanceId];
	id = id = instanceId;
	rect = &data->finalPos;
	if (data->render != NULL) {
		data->render(instanceId);
	}
	renderUIBoxBorder(rect, layer = 6 - instanceId);
	renderLinePrimitive(0x20202, rect->x + 3, rect->y + 3, rect->x + 3, rect->y + rect->h - 3, layer, 0);
	renderLinePrimitive(0x20202, rect->x + rect->w - 4, rect->y + 3, rect->x + rect->w - 4,
			    rect->y + rect->h - 3, layer, 0);
	if (data->features & 1) {
		renderLinePrimitive(0x20202, rect->x + 3, rect->y + 13, rect->x + rect->w - 3, rect->y + 13,
				    layer, 0);
		renderLinePrimitive(0xFAD990, rect->x + 3, rect->y + 14, rect->x + rect->w - 3, rect->y + 14,
				    layer, 0);
		renderLinePrimitive(0x20202, rect->x + 3, rect->y + 15, rect->x + rect->w - 3, rect->y + 15,
				    layer, 0);
	}
	if (data->features & 4) {
		renderTrianglePrimitive(0x20202, rect->x + rect->w - 13, rect->y + rect->h - 10,
					rect->x + rect->w - 13, rect->y + 13, rect->x + rect->w - 6, rect->y + 13,
					layer, 0);
		renderTrianglePrimitive(0xA08769, rect->x + rect->w - 6, rect->y + 14, rect->x + rect->w - 6,
					rect->y + rect->h - 10, rect->x + rect->w - 12, rect->y + rect->h - 10, layer,
					0);
		h25 = rect->h - 25;
		rowHeight = h25 * UI_BOX_DATA[instanceId].visibleRows / UI_BOX_DATA[instanceId].totalRows;
		barTop = rect->y + 14 +
			 (h25 - rowHeight) * UI_BOX_DATA[instanceId].rowOffset /
				 (UI_BOX_DATA[instanceId].totalRows - UI_BOX_DATA[instanceId].visibleRows);
		renderTrianglePrimitive(0x20202, rect->x + rect->w - 7, barTop, rect->x + rect->w - 7, barTop + rowHeight,
					rect->x + rect->w - 13, barTop + rowHeight, layer, 0);
		renderTrianglePrimitive(0xA08769, rect->x + rect->w - 12, barTop + rowHeight, rect->x + rect->w - 12, barTop,
					rect->x + rect->w - 6, barTop, layer, 0);
		p = (POLY_F4 *)GsGetWorkBase();
		SetPolyF4(p);
		setRGB0(p, 0x5B, 0x70, 0x80);
		setXYWH(p, rect->x + rect->w - 11, barTop + 1, 4, rowHeight - 1);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 6 - id, p++);
		SetPolyF4(p);
		setRGB0(p, 0x35, 0x4B, 0x5C);
		setXYWH(p, rect->x + rect->w - 12, rect->y + 14, 6, rect->h - 24);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 6 - id, p++);
		GsSetWorkBase((PACKET *)p);
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
	setWH(&box, rect->w - 8, rect->h - 3);
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

void renderUIBoxBorder(RECT *rect, int32_t layer)
{
	POLY_FT4 *p;
	int32_t i;
	int16_t x;
	int16_t y;
	int16_t x1;
	int16_t y1;

	p = (POLY_FT4 *)GsGetWorkBase();
	for (i = 0; i < 4; i++) {
		SetPolyFT4(p);
		p->tpage = getTPage(0, 0, 320, 0);
		setClut(p, 0x60, 0x1EC);
		setRGB0(p, 0x80, 0x80, 0x80);
		setUVDataPolyFT4((PACKET *)p, MAIN_D_80134330[i], MAIN_D_80134334[i] + 0x80, 4, 4);
		setPosDataPolyFT4((PACKET *)p, (i % 2 == 0) ? rect->x : rect->x + rect->w - 4,
				  (i < 2) ? rect->y : rect->y + rect->h - 4, 4, 4);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, p++);
	}
	GsSetWorkBase((PACKET *)p);
	x = rect->x + 4;
	x1 = rect->x + rect->w - 4;
	y = rect->y;
	renderLinePrimitive(0x20202, x, y, x1, y, layer, 0);
	y++;
	renderLinePrimitive(0xFAD990, x, y, x1, y, layer, 0);
	y++;
	renderLinePrimitive(0x20202, x, y, x1, y, layer, 0);
	y = rect->y + rect->h - 3;
	renderLinePrimitive(0x20202, x, y, x1, y, layer, 0);
	y++;
	renderLinePrimitive(0xFAD990, x, y, x1, y, layer, 0);
	y++;
	renderLinePrimitive(0x20202, x, y, x1, y, layer, 0);
	x = rect->x;
	y = rect->y + 4;
	y1 = rect->y + rect->h - 3;
	renderLinePrimitive(0x20202, x, y, x, y1, layer, 0);
	x++;
	renderLinePrimitive(0xC59F4A, x, y, x, y1, layer, 0);
	x++;
	renderLinePrimitive(0xFAD990, x, y, x, y1, layer, 0);
	x = rect->x + rect->w - 3;
	renderLinePrimitive(0xFAD990, x, y, x, y1, layer, 0);
	x++;
	renderLinePrimitive(0xC59F4A, x, y, x, y1, layer, 0);
	x++;
	renderLinePrimitive(0x20202, x, y, x, y1, layer, 0);
}

void renderUIBoxAnim(int32_t instanceId, int16_t frame)
{
	RECT *start;
	RECT *final;
	int32_t layer;
	int16_t x1;
	int16_t y1;
	int16_t x2;
	int16_t y2;

	if (UI_BOX_DATA[instanceId].state == 3 && UI_BOX_DATA[instanceId].frame == 4) {
		playSound(0, 1);
	}
	start = &UI_BOX_DATA[instanceId].startPos;
	final = final = &UI_BOX_DATA[instanceId].finalPos;
	x1 = start->x + frame * (final->x - start->x) / 4;
	y1 = start->y + frame * (final->y - start->y) / 4;
	x2 = (start->x + start->w) + frame * ((final->x + final->w) - (start->x + start->w)) / 4;
	y2 = (start->y + start->h) + frame * ((final->y + final->h) - (start->y + start->h)) / 4;
	renderTrianglePrimitive(0x808080, x1, y1, x2, y1, x2, y2, layer = 6 - instanceId, 0);
	renderTrianglePrimitive(0x808080, x2, y2, x1, y2, x1, y1, layer, 0);
}
