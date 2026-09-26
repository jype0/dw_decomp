#include <libetc.h>

#include <dw/font.h>
#include <dw/graphics.h>
#include <dw/map.h>
#include <dw/model.h>
#include <dw/partner.h>

#include "common.h"

typedef struct {
	int8_t v[66];
} CardSprites;

extern CardSprites MAIN_D_80123E28;
extern int8_t MENU_STATE;
extern int8_t MEDAL_SELECTOR_INDEX;
extern int16_t MAIN_D_8012472C[];
extern int8_t SELECTED_CARD;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern GsOT *FRAMEBUFFER_OT[2];
extern GsOT_TAG *FRAMEBUFFER0_ORIGIN;
extern GsOT_TAG *FRAMEBUFFER1_ORIGIN;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsRVIEW2 MAIN_D_80123860;
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t VIEWPORT_DISTANCE;
extern GsF_LIGHT LIGHT_DATA[3];
void startAnimation(Entity *entity, int32_t animId);
void tickAnimation(Entity *entity);
extern char *STATUS_VIEW_LABELS[];
extern char MAIN_D_80124C24[];
extern char MAIN_D_80124C30[];
extern char MAIN_D_80124C54[];
extern RECT MAIN_D_801342E0;
extern int8_t MENU_SUB_STATE;

int32_t drawCardViewStrings(void);
int32_t getCardAmount(uint8_t cardId);
int32_t hasMedal(uint16_t medal);
int32_t drawMedalViewStrings(void);
void renderSeperatorLines(int16_t *lines, int32_t b, ...);
void renderDigiviceMedals(void);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
		  int32_t f, int32_t g, int32_t h, int32_t i);
void renderRectPolyFT4(int16_t posX, int16_t posY, int32_t width,
		       int32_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag);
void renderSpriteBox(int16_t x, int16_t y, int16_t w, int16_t h, int32_t c1,
		     int32_t c2, uint8_t r, uint8_t g, uint8_t b, int32_t a10);
void renderBoxBar(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t r,
		  uint8_t g, uint8_t b, uint8_t flags, int32_t otz);
void renderTrianglePrimitive(int32_t color, int32_t x0, int32_t y0, int32_t x1,
			     int32_t y1, int32_t x2, int32_t y2, int32_t otz,
			     int32_t flag);
void renderInsetBox(int16_t a, int16_t b, int16_t c, int16_t d, int32_t otz);
void renderMedalView(void);
void renderCardsView(void);
int32_t drawDigimonStatsStrings(void);
void renderDigimonStatsBar(int32_t a, int32_t b, int32_t c, int16_t d, int16_t e);
void renderDigimonStatusConditions(int32_t condition);
void renderDigiviceEntity(Entity *entity, int32_t entityId);

static void *overworld_status_boxes_functions[] = {
	renderDigiviceEntity,
	renderDigimonStatusConditions,
	renderDigimonStatsBar,
	drawDigimonStatsStrings,
	renderSpriteBox,
	renderBoxBar,
	renderInsetBox,
	renderCardsView,
	renderMedalView,
};

void renderMedalView(void)
{
	int32_t i;
	int32_t j;
	int32_t ty;
	int32_t tv;
	int32_t state;

	state = MENU_STATE;

	if (state == 1) {
		goto grid;
	}

	if (state == 2) {
		goto cursor;
	}

	if (state == 3) {
		goto opening;
	}

	if (state != 0) {
		return;
	}

	if (drawMedalViewStrings() == 1) {
		MENU_STATE = 1;
	}

	return;

opening:
	if (drawMedalViewStrings() == 1) {
		MENU_STATE = 2;
	}

cursor:
	renderRectPolyFT4(((MEDAL_SELECTOR_INDEX % 5) * 38) - 0x7e,
			  ((MEDAL_SELECTOR_INDEX / 5) * 24) - 0x3d, 0x18, 0x18, 0, 0xd0,
			  0x18, 0x7dc7, 1, 0);

grid:
	renderSeperatorLines(MAIN_D_8012472C, 0xe, 5);
	renderString(3, -0x36, -0x51, 0x6c, 0xc, 0, 0xc, 5, 1);

	for (i = 0; i < 0xf; i++) {
		if (hasMedal(i) != 0) {
			if (MEDAL_SELECTOR_INDEX == i) {
				if (MENU_STATE != 3) {
					renderString(0, -0x5d, 0x17, 0xac, 0xc, 0, 0x18, 5, 1);

					for (j = 0, tv = 0x24, ty = 0x27; j < 3; j++, ty += 0xd, tv += 0xc) {
						renderString(0, -0x7e, ty, 0xfc, 0xc, 0, tv, 5, 1);
					}
				}

				renderDigiviceMedals();
			}
		} else {
			renderRectPolyFT4(((i % 5) * 38) - 0x7c, ((i / 5) * 24) - 0x3d, 0xf,
					  0x18, 0xf0, 0xc8, 0x18, 0x7dc7, 5, 0);
		}

		renderRectPolyFT4(((i % 5) * 38) - 0x7a, ((i / 5) * 24) - 0x3d, 0xf, 0x18,
				  0xf0, 0xb0, 0x18, 0x7dc7, 5, 0);
	}

	renderInsetBox(0x3e, 0x8d, 0xb0, 0xe, 5);
	renderString(0xf, -0x23, 0x55, 0xc, 0xc, 0x6c, 0xc, 5, 1);
	renderString(0, -0x17, 0x55, 0x18, 0xc, 0x84, 0xc, 5, 1);
	renderString(0, 1, 0x55, 0x24, 0xc, 0x9c, 0xc, 5, 1);
	renderString(7, 0x2b, 0x55, 0xc, 0xc, 0x78, 0xc, 5, 1);
	renderString(0, 0x37, 0x55, 0x30, 0xc, 0xc0, 0xc, 5, 1);
}

void renderCardsView(void)
{
	CardSprites sprites;
	int32_t col;
	int32_t colX;
	int32_t idx;
	int32_t x1;
	int32_t x2;
	int32_t row;
	int32_t y1;
	int32_t y2;
	int8_t card;
	int32_t state;

	sprites = MAIN_D_80123E28;

	state = MENU_STATE;

	if (state == 1) {
		goto grid;
	}

	if ((state == 3) || (state == 2)) {
		goto highlight;
	}

	if (state != 0) {
		return;
	}

	if (drawCardViewStrings() == 1) {
		MENU_STATE = 1;
	}

	return;

highlight:
	renderRectPolyFT4(((SELECTED_CARD % 11) * 24) - 0x84,
			  ((SELECTED_CARD / 11) * 24) - 0x40, 0x18, 0x14, 0, 0xe8,
			  0x18, 0x7dc7, 5, 0);

grid:
	for (col = 0, colX = 0; col < 6; col++, colX += 0x18) {
		col = col;
		colX = colX;
		row = 0;
		idx = (col * 11) + row;
		x1 = -0x81;
		x2 = 0x1e;
		y1 = colX - 0x3e;
		y2 = colX + 0x39;

		for (; row < 11; row++, x2 += 0x18, x1 += 0x18, idx++) {
			card = idx;

			if ((int8_t)getCardAmount(card) > 0) {
				renderRectPolyFT4(x1, y1, 0x10, 0x10, (card % 8) * 32,
						  (card / 8) * 16, 0x18,
						  (sprites.v[card] * 64) + 0x7a07, 5, 0);
			}

			renderSpriteBox(x2, y2, 0x12, 0x12, 0xbebebe, 0x3c3c3c, 0x69, 0x69,
					0x69, 5);
		}
	}

	renderString(3, -0x24, -0x50, 0x48, 0xc, 0, 0xc, 5, 1);
	renderString(0xf, -0x23, 0x53, 0xc, 0xc, 0x48, 0xc, 5, 1);
	renderString(0, -0x17, 0x53, 0x3c, 0xc, 0x54, 0xc, 5, 1);
	renderString(7, 0x2b, 0x53, 0xc, 0xc, 0x90, 0xc, 5, 1);
	renderString(0, 0x37, 0x53, 0x3c, 0xc, 0x9c, 0xc, 5, 1);
}

static int32_t renderInsetBox__garbage__(int32_t seed)
{
	int32_t t0 = 1;
	int32_t t1 = 2;
	int32_t t2 = 3;
	int32_t t3 = 4;
	int32_t t4 = 5;
	int32_t t5 = 6;
	int32_t t6 = 7;
	int32_t t7 = 8;
	int32_t t8 = 9;
	int32_t t9 = 10;
	int32_t t10 = 11;
	int32_t t11 = 12;
	int32_t t12 = 13;
	int32_t t13 = 14;
	int32_t t14 = 15;
	int32_t t15 = 16;
	int32_t t16 = 17;
	int32_t t17 = 18;
	int32_t t18 = 19;
	int32_t t19 = 20;
	int32_t t20 = 21;
	int32_t t21 = 22;
	int32_t t22 = 23;
	int32_t t23 = 24;
	int32_t t24 = 25;
	int32_t t25 = 26;
	int32_t t26 = 27;
	int32_t t27 = 28;
	int32_t t28 = 29;
	int32_t t29 = 30;
	int32_t t30 = 31;
	int32_t t31 = 32;
	int32_t t32 = 33;

	return seed + t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8 + t9 + t10 + t11 + t12 + t13 + t14 + t15 + t16 + t17 + t18 + t19 + t20 + t21 + t22 + t23 + t24 + t25 + t26 + t27 + t28 + t29 + t30 + t31 + t32;
}

void renderInsetBox(int16_t a, int16_t b, int16_t c, int16_t d, int32_t otz)
{
	void renderBoxBar(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t r,
			  uint8_t g, uint8_t b, uint8_t flags, int32_t otz);
	int32_t x;
	int32_t y;
	int32_t y0;
	int32_t x0;
	int32_t cc;
	int32_t dd;

	y = b;
	x = a;
	x = (int16_t)(x - 0xa0);
	y = (int16_t)(y - 0x78);
	y = y0 = y;
	x = x0 = x;
	dd = d;
	cc = c;
	renderTrianglePrimitive(0x20202, x0, y0 + dd, x0, y0, x0 + cc, y0, otz, 0);
	renderTrianglePrimitive(0xa08769, x0, y0 + dd, x0 + cc, y0 + dd, x0 + cc, y0,
				otz, 0);
	renderBoxBar(x, y, cc, dd, 0x35, 0x4b, 0x5c, 0, otz);
}

void renderBoxBar(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t r,
		  uint8_t g, uint8_t b, uint8_t flags, int32_t otz)
{
	GsBOXF box;

	if ((flags & 1) != 0) {
		if ((flags & 2) != 0) {
			box.attribute = 0x60000000;
		} else {
			box.attribute = 0x40000000;
		}
	} else {
		box.attribute = 0;
	}

	box.x = x;
	box.y = y;
	setWH(&box, w, h);
	box.r = r;
	box.g = g;
	box.b = b;

	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, otz);

	if ((flags & 0x80) != 0) {
		renderTrianglePrimitive(0x20202,
					x - 1, y - 1,
					x + 0xd, y - 1,
					x + 0xd, y + 0xc,
					otz, 0);
		renderTrianglePrimitive(0x20202,
					x - 1, y - 1,
					x - 1, y + 0xc,
					x + 0xd,
					y + 0xc,
					otz, 0);
	}
}

void renderSpriteBox(int16_t x, int16_t y, int16_t w, int16_t h, int32_t c1,
		     int32_t c2, uint8_t r, uint8_t g, uint8_t b, int32_t a10)
{
	int32_t py;
	int32_t px;

	x = x - 0xa0;
	y = y - 0x78;
	py = y;
	px = x;
	renderTrianglePrimitive(c1, px, py + h, px, py, px + w, py, a10, 0);
	renderTrianglePrimitive(c2, px, py + h, px + w, py + h, px + w, py, a10, 0);
	renderBoxBar(x, y, w, h, r, g, b, 0, a10);
}

int32_t drawDigimonStatsStrings(void)
{
	RECT rect;

	rect = MAIN_D_801342E0;

	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect);
		drawString(MAIN_D_80124C54, 0, 0xf0);
		drawString(MAIN_D_80124C24, 0, 0x24);
		drawString(MAIN_D_80124C30, 0x24, 0x24);
		MENU_SUB_STATE = 1;
		break;
	case 1:
		drawString(DIGIMON_DATA[ENTITY_TABLE[1]->type].name, 0, 0x48);
		drawString(PARTNER_ENTITY.name, 0, 0x3c);
		MENU_SUB_STATE = 2;
		break;
	case 2:
		drawString(STATUS_VIEW_LABELS[1], 0, 0x30);
		MENU_SUB_STATE = 3;
		break;
	case 3:
		drawString(STATUS_VIEW_LABELS[2], 0, 0x18);

		return 1;
	}

	return 0;
}

void renderDigimonStatsBar(int32_t a, int32_t b, int32_t c, int16_t d, int16_t e)
{
	renderBoxBar(d, e, (uint8_t)(c * a / b), 2, 0x32, 0xc8, 0xc8, 0, 5);
}

extern uint16_t PLAYTIME_FRAMES;
typedef struct {
	int32_t v[6];
} ConditionMaskTable;
extern ConditionMaskTable MAIN_D_80123DD0;

void renderDigimonStatusConditions(int32_t condition)
{
	ConditionMaskTable masks;
	int32_t i;
	uint8_t bobY;
	int16_t clut;

	masks = MAIN_D_80123DD0;
	if (PLAYTIME_FRAMES % 16 < 3) {
		bobY = 0x82;
	} else if (PLAYTIME_FRAMES % 16 < 5) {
		bobY = 0x80;
	} else if (PLAYTIME_FRAMES % 16 < 7) {
		bobY = 0x7d;
	} else if (PLAYTIME_FRAMES % 16 < 9) {
		bobY = 0x79;
	} else if (PLAYTIME_FRAMES % 16 < 11) {
		bobY = 0x7d;
	} else if (PLAYTIME_FRAMES % 16 < 13) {
		bobY = 0x80;
	} else {
		bobY = 0x82;
	}
	for (i = 0; i < 6; i++) {
		if (condition & masks.v[i]) {
			clut = 0x7a06;
			if ((i == 0) || (i == 4)) {
				clut = 0x7a86;
			}
			if (i == 5) {
				clut = 0x7a46;
			}
			renderRectPolyFT4(i * 0xf - 0x8a, bobY - 0x78, 0xc, 0xc, i * 0xc + 0x30, 0x8c, 5, clut, 5, 0);
		}
	}

	if (condition & 8) {
		renderRectPolyFT4(-0x30, bobY - 0x78, 0xc, 0xc, 0x60, 0xc8, 5, 0x7a06, 5, 0);
	}
}

static int32_t renderDigiviceEntity__garbage__(int32_t seed)
{
	int32_t t0 = 1;
	int32_t t1 = 2;
	int32_t t2 = 3;
	int32_t t3 = 4;
	int32_t t4 = 5;
	int32_t t5 = 6;
	int32_t t6 = 7;
	int32_t t7 = 8;
	int32_t t8 = 9;
	int32_t t9 = 10;
	int32_t t10 = 11;
	int32_t t11 = 12;
	int32_t t12 = 13;
	int32_t t13 = 14;
	int32_t t14 = 15;
	int32_t t15 = 16;
	int32_t t16 = 17;
	int32_t t17 = 18;
	int32_t t18 = 19;
	int32_t t19 = 20;
	int32_t t20 = 21;
	int32_t t21 = 22;
	int32_t t22 = 23;
	int32_t t23 = 24;
	int32_t t24 = 25;
	int32_t t25 = 26;
	int32_t t26 = 27;
	int32_t t27 = 28;
	int32_t t28 = 29;
	int32_t t29 = 30;
	int32_t t30 = 31;
	int32_t t31 = 32;
	int32_t t32 = 33;
	int32_t t33 = 34;

	return seed + t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8 + t9 + t10 + t11 + t12 + t13 + t14 + t15 + t16 + t17 + t18 + t19 + t20 + t21 + t22 + t23 + t24 + t25 + t26 + t27 + t28 + t29 + t30 + t31 + t32 + t33;
}

void renderDigiviceEntity(entity, entityId)
Entity *entity;
int16_t entityId;
{
	MATRIX m;
	VECTOR savedPos;
	SVECTOR savedRot;
	GsF_LIGHT lights[3];
	PositionData *pos;
	int32_t count;
	int32_t anim;
	int32_t type;
	int32_t i;
	int32_t bone;

	FRAMEBUFFER_OT[0]->length = 9;
	FRAMEBUFFER_OT[0]->org = FRAMEBUFFER0_ORIGIN;
	FRAMEBUFFER_OT[1]->length = 9;
	FRAMEBUFFER_OT[1]->org = FRAMEBUFFER1_ORIGIN;

	GsSetProjection(0x200);
	GsSetRefView2(&MAIN_D_80123860);
	GsClearOt(0, 5, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER]);

	for (i = 0; i < 3; i++) {
		lights[i].vx = lights[i].vy = lights[i].vz = (i == 0) ? 100 : 0;
		lights[i].r = lights[i].g = lights[i].b = 0x80;
		GsSetFlatLight(i, &lights[i]);
	}

	pos = entity->posData;
	type = entity->type;
	anim = entity->anim.animId;
	count = DIGIMON_DATA[type].boneCount;
	savedPos = pos->location;
	savedRot = pos->rotation;
	setEntityPosition(entityId, RAISE_DATA[type].viewX,
			  RAISE_DATA[type].viewY,
			  RAISE_DATA[type].viewZ);
	setEntityRotation(entityId, 0, 0, 0);
	setupEntityMatrix(entityId);
	startAnimation(entity, 0);
	tickAnimation(entity);
	m = GsWSMATRIX;

	for (bone = 0; bone < count; pos++, bone++) {
		if (pos->obj.tmd != NULL) {
			GsGetLw(pos->obj.coord2, &m);
			GsSetLightMatrix(&m);
			GsGetLs(pos->obj.coord2, &m);
			GsSetLsMatrix(&m);
			GsSortObject4(&pos->obj, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], 5,
				      getScratchAddr(0));
		}
	}

	GsSortOt(FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);

	for (i = 0; i < 3; i++) {
		GsSetFlatLight(i, &LIGHT_DATA[i]);
	}

	setEntityPosition(entityId, savedPos.vx, savedPos.vy, savedPos.vz);
	setEntityRotation(entityId, savedRot.vx, savedRot.vy, savedRot.vz);
	setupEntityMatrix(entityId);
	startAnimation(entity, anim);
	tickAnimation(entity);
}
