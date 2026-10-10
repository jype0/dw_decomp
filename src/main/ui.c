#include <libgpu.h>
#include <libgs.h>
#include <dw/entity.h>
#include <dw/ui.h>
#include <dw/version.h>
#include <text/main/ui.h>

#include "common.h"

void tickUIBox(int32_t instanceId);
void renderUIBoxStatic(int32_t instanceId);
void renderUIBoxAnimated(int32_t instanceId);
void renderUIBoxAnim(int32_t instanceId, int16_t frame);
void renderUIBoxBorder(RECT *rect, int32_t layer);

void playSound(int32_t soundId, uint32_t flag);
void drawLine3P(int32_t color, int32_t x0, int32_t y0,
		int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		int32_t layer, int32_t blend);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uvX, int32_t uvY,
		      int32_t uvWidth, int32_t uvHeight);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY,
		       int32_t width, int32_t height);
void drawLine2P(int32_t color, int32_t x0, int32_t y0,
		int32_t x1, int32_t y1,
		int32_t layer, int32_t blend);

extern GsOT *ACTIVE_ORDERING_TABLE;

static void *ui_functions[] = {
	renderUIBoxAnim,
	renderUIBoxBorder,
	removeAnimatedUIBox,
#if VERSION_IS(EU)
	createAnimatedUIBox,
	renderUIBoxAnimated,
	removeStaticUIBox,
	createStaticUIBox,
	tickUIBox,
	renderUIBoxStatic,
#else
	renderUIBoxAnimated,
	createAnimatedUIBox,
	removeStaticUIBox,
	renderUIBoxStatic,
	tickUIBox,
	createStaticUIBox,
#endif
	initializeUIBoxData,
};

// clang-format off
STAT_LABELS_STRINGS

uint8_t BOX_BORDER_CORNERS_U[4] = {
	0x78, 0x7c, 0x78, 0x7c,
};

uint8_t BOX_BORDER_CORNERS_V[4] = {
	0x10, 0x10, 0x14, 0x14,
};

RGB8 UI_BOX_COLORS[5] = {
	{ 0x00, 0x00, 0x00 },
	{ 0x2d, 0x38, 0x40 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
};

#if VERSION_IS(EU)
char *MAIN_D_80124C0C[6] = {
	STR_STAT_LABEL_HP,
	STR_STAT_LABEL_MP,
	STR_STAT_LABEL_OFFENSE,
	STR_STAT_LABEL_DEFENSE,
	STR_STAT_LABEL_SPEED,
	STR_STAT_LABEL_BRAINS,
};
#else
char MAIN_D_80124C0C[6][12] = {
	STAT_LABEL(STR_STAT_LABEL_HP)
	STAT_LABEL(STR_STAT_LABEL_MP)
	STAT_LABEL(STR_STAT_LABEL_OFFENSE)
	STAT_LABEL(STR_STAT_LABEL_DEFENSE)
	STAT_LABEL(STR_STAT_LABEL_SPEED)
	STAT_LABEL(STR_STAT_LABEL_BRAINS)
};
#endif

char MAIN_D_80124C54[] = {
	0x82, 0x4f, 0x82, 0x50, 0x82, 0x51, 0x82, 0x52,
	0x82, 0x53, 0x82, 0x54, 0x82, 0x55, 0x82, 0x56,
	0x82, 0x57, 0x82, 0x58, 0x00,
};
// clang-format on

UIBoxData UI_BOX_DATA[6];
StatsGains STATS_GAINS;

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

void renderUIBoxStatic(instanceId)
	int16_t instanceId;
{
	RECT *rect;
	POLY_F4 *p;
	int32_t color1;
	int32_t color2;
	UIBoxData *data;
	GsBOXF box;
	int16_t h25;
	int16_t rowHeight;
	int16_t barTop;

	data = &UI_BOX_DATA[instanceId];
	rect = &data->finalPos;
	if (data->render != NULL) {
		data->render(instanceId);
	}
	renderUIBoxBorder(rect, 6 - instanceId);
	color1 = 0x20202;
	drawLine2P(color1, rect->x + 3, rect->y + 3, rect->x + 3, rect->y + rect->h - 3, 6 - instanceId, 0);
	drawLine2P(color1, rect->x + rect->w - 4, rect->y + 3, rect->x + rect->w - 4,
		   rect->y + rect->h - 3, 6 - instanceId, 0);
	if (data->features & 1) {
		color2 = 0xFAD990;
		drawLine2P(color1, rect->x + 3, rect->y + 13, rect->x + rect->w - 3, rect->y + 13,
			   6 - instanceId, 0);
		drawLine2P(color2, rect->x + 3, rect->y + 14, rect->x + rect->w - 3, rect->y + 14,
			   6 - instanceId, 0);
		drawLine2P(color1, rect->x + 3, rect->y + 15, rect->x + rect->w - 3, rect->y + 15,
			   6 - instanceId, 0);
	}
	if (data->features & 4) {
		color2 = 0xA08769;
		drawLine3P(color1, rect->x + rect->w - 13, rect->y + rect->h - 10,
			   rect->x + rect->w - 13, rect->y + 13, rect->x + rect->w - 6, rect->y + 13,
			   6 - instanceId, 0);
		drawLine3P(color2, rect->x + rect->w - 6, rect->y + 14, rect->x + rect->w - 6,
			   rect->y + rect->h - 10, rect->x + rect->w - 12, rect->y + rect->h - 10, 6 - instanceId,
			   0);
		h25 = rect->h - 25;
		rowHeight = h25 * UI_BOX_DATA[instanceId].visibleRows / UI_BOX_DATA[instanceId].totalRows;
		barTop = rect->y + 14 +
			 (h25 - rowHeight) * UI_BOX_DATA[instanceId].rowOffset /
				 (UI_BOX_DATA[instanceId].totalRows - UI_BOX_DATA[instanceId].visibleRows);
		drawLine3P(color1, rect->x + rect->w - 7, barTop, rect->x + rect->w - 7, barTop + rowHeight,
			   rect->x + rect->w - 13, barTop + rowHeight, 6 - instanceId, 0);
		drawLine3P(color2, rect->x + rect->w - 12, barTop + rowHeight, rect->x + rect->w - 12, barTop,
			   rect->x + rect->w - 6, barTop, 6 - instanceId, 0);
		p = (POLY_F4 *)GsGetWorkBase();
		SetPolyF4(p);
		setRGB0(p, 0x5B, 0x70, 0x80);
		setXYWH(p, rect->x + rect->w - 11, barTop + 1, 4, rowHeight - 1);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 6 - instanceId, p++);
		SetPolyF4(p);
		setRGB0(p, 0x35, 0x4B, 0x5C);
		setXYWH(p, rect->x + rect->w - 12, rect->y + 14, 6, rect->h - 24);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 6 - instanceId, p++);
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
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 6 - instanceId);
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

void removeStaticUIBox(int16_t id)
{
	UI_BOX_DATA[id].state = 0;
	UI_BOX_DATA[id].frame = 0;
	removeObject(0x1a4, id);
}

void createAnimatedUIBox(int16_t instanceId, uint8_t color, uint8_t features,
			 RECT *finalPos, RECT *startPos,
			 TickFunction tickFunc, RenderFunction renderFunc)
{
	UIBoxData *data;

	data = &UI_BOX_DATA[instanceId];
	playSound(0, 0);
	data->state = 2;
	data->color = color;
	data->features = features;
	data->startPos = *startPos;
	data->finalPos = *finalPos;
	data->render = renderFunc;
	data->tick = tickFunc;
	addObject(0x1a4, instanceId, tickUIBox, renderUIBoxAnimated);
}

void renderUIBoxAnimated(instanceId)
	int16_t instanceId;
{
	UIBoxData *data;
	int16_t currentFrame;

	data = &UI_BOX_DATA[instanceId];
	if ((data->state == 2) || (data->state == 3)) {
		if (data->state == 2) {
			currentFrame = data->frame++;
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

void removeAnimatedUIBox(int16_t id, RECT *target)
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
	int32_t black;
	int32_t gold;
	int32_t brown;
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
		setUVDataPolyFT4(p, BOX_BORDER_CORNERS_U[i], BOX_BORDER_CORNERS_V[i] + 0x80, 4, 4);
		setPosDataPolyFT4(p, (i % 2 == 0) ? rect->x : rect->x + rect->w - 4,
				  (i < 2) ? rect->y : rect->y + rect->h - 4, 4, 4);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, p++);
	}
	GsSetWorkBase((PACKET *)p);
	black = 0x20202;
	gold = 0xFAD990;
	brown = 0xC59F4A;
	x = rect->x + 4;
	x1 = rect->x + rect->w - 4;
	y = rect->y;
	drawLine2P(black, x, y, x1, y, layer, 0);
	y++;
	drawLine2P(gold, x, y, x1, y, layer, 0);
	y++;
	drawLine2P(black, x, y, x1, y, layer, 0);
	y = rect->y + rect->h - 3;
	drawLine2P(black, x, y, x1, y, layer, 0);
	y++;
	drawLine2P(gold, x, y, x1, y, layer, 0);
	y++;
	drawLine2P(black, x, y, x1, y, layer, 0);
	x = rect->x;
	y = rect->y + 4;
	y1 = rect->y + rect->h - 3;
	drawLine2P(black, x, y, x, y1, layer, 0);
	x++;
	drawLine2P(brown, x, y, x, y1, layer, 0);
	x++;
	drawLine2P(gold, x, y, x, y1, layer, 0);
	x = rect->x + rect->w - 3;
	drawLine2P(gold, x, y, x, y1, layer, 0);
	x++;
	drawLine2P(brown, x, y, x, y1, layer, 0);
	x++;
	drawLine2P(black, x, y, x, y1, layer, 0);
}

void renderUIBoxAnim(int32_t instanceId, int16_t frame)
{
	RECT *start;
	RECT *final;
	int16_t x0;
	int16_t y0;
	int16_t x1;
	int16_t y1;
	int16_t x2;
	int16_t y2;

	if (UI_BOX_DATA[instanceId].state == 3 && UI_BOX_DATA[instanceId].frame == 4) {
		playSound(0, 1);
	}
	start = &UI_BOX_DATA[instanceId].startPos;
	final = &UI_BOX_DATA[instanceId].finalPos;
	x0 = start->x + frame * (final->x - start->x) / 4;
	y0 = start->y + frame * (final->y - start->y) / 4;
	x1 = (start->x + start->w) + frame * ((final->x + final->w) - (start->x + start->w)) / 4;
	y1 = y0;
	x2 = x1;
	y2 = (start->y + start->h) + frame * ((final->y + final->h) - (start->y + start->h)) / 4;
	drawLine3P(0x808080, x0, y0, x1, y1, x2, y2, 6 - instanceId, 0);
	drawLine3P(0x808080, x2, y2, x0, y2, x0, y0, 6 - instanceId, 0);
}
