
#include <dw/clock.h>
#include <dw/evl.h>
#include <dw/font.h>
#include <dw/map_object.h>
#include <dw/move.h>

#include "common.h"

typedef struct {
	int16_t orderValue;
	int16_t x;
	int16_t y;
	int16_t animSprites[8];
	uint8_t animTimes[8];
	uint8_t timer;
	uint8_t pad;
	int8_t currentFrame;
	int8_t flag;
} LocalMapObjectInstance;

typedef struct {
	int16_t texX;
	int16_t texY;
	int16_t someX;
	int16_t someY;
	int16_t someZ;
	uint8_t width;
	uint8_t height;
	int8_t clut;
	int8_t transparency;
} LocalMapObject;

extern int32_t ACTIVE_FRAMEBUFFER;
void clearTextSubArea(RECT *rect);
extern char *STATUS_VIEW_LABELS[];
extern RECT MAIN_D_801342F0;
extern int8_t MENU_SUB_STATE;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern GsOT_TAG *FRAMEBUFFER0_ORIGIN;
extern GsOT_TAG *FRAMEBUFFER1_ORIGIN;
extern GsOT *FRAMEBUFFER_OT[2];
extern GsRVIEW2 GS_VIEWPOINT;
extern SVECTOR MAIN_D_80134238;
extern GsRVIEW2 MAIN_D_80123880;
extern int32_t VIEWPORT_DISTANCE;
extern int16_t MAP_OBJECT_INSTANCE_COUNT;
extern LocalMapObject LOCAL_MAP_OBJECTS[];
extern int32_t MAP_OBJECT_MOVE_TO_DATA[];
extern uint8_t EQUIPPED_MOVES[4];

void renderRectPolyFT4(int16_t posX, int16_t posY, int32_t width,
		       int32_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag);
void renderBox(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t r,
	       uint8_t g, uint8_t b, uint8_t flags, int32_t otz);
int32_t hasMove(int32_t move);
void sortArray(int16_t *arr, int8_t count);
void renderDigimonMoveBox(void);
void renderDigimonMovesSelected(int16_t panel);
int32_t drawMoveViewHelpStrings(void);
void removeTriangleMenu(void);
void removeUIBox1(void);
int32_t randomLimit(int32_t max);
void removeStaticUIBox(int16_t id);
void removeObject(int32_t objectId, int32_t arg1);

void renderDigiviceMedals(void);

static void renderDigimonMoveBox__garbage__(LocalMapObjectInstance *mapObjects,
					    uint8_t *data, int32_t mapId)
{
	LocalMapObjectInstance *obj;
	int16_t *src;
	int32_t i;
	int32_t j;
	int32_t k;
	int16_t count;

	obj = mapObjects;
	src = (int16_t *)data;
	count = *src++;
	for (i = 0; i < count; i++) {
		LOCAL_MAP_OBJECTS[i].texX = *src++;
		LOCAL_MAP_OBJECTS[i].texY = *src++;
		LOCAL_MAP_OBJECTS[i].width = *src++;
		LOCAL_MAP_OBJECTS[i].height = *src++;
		LOCAL_MAP_OBJECTS[i].someX = *src++;
		LOCAL_MAP_OBJECTS[i].someY = *src++;
		LOCAL_MAP_OBJECTS[i].someZ = *src++;
		LOCAL_MAP_OBJECTS[i].clut = *src++;
		LOCAL_MAP_OBJECTS[i].transparency = *src++;
	}
	MAP_OBJECT_INSTANCE_COUNT = *src++;
	for (k = 0; k < MAP_OBJECT_INSTANCE_COUNT; k++) {
		for (j = 0; j < 8; j++) {
			obj->animSprites[j] = *src++;
		}
		for (j = 0; j < 8; j++) {
			obj->animTimes[j] = *src++;
		}
		obj->x = *src++;
		obj->y = *src++;
		obj->flag = *src++;
		if (((mapId >= 0x58 && mapId < 0x61) ||
		     (mapId >= 0x84 && mapId < 0x88)) &&
		    k < 0x23) {
			if (k >= 0x14) {
				obj->x = randomLimit(320);
			}
			obj->y = randomLimit(240);
			obj->flag |= 0x80;
		}
		obj++;
	}
	for (k = 0; k < 10; k++) {
		MAP_OBJECT_MOVE_TO_DATA[k] = 0;
	}
}

static void *overworld_moves_box_functions[] = {
	removeUIBox1,
	removeTriangleMenu,
	sortArray,
	renderDigiviceMedals,
	drawMoveViewHelpStrings,
	renderDigimonMoveBox,
	renderDigimonMovesSelected,
};

extern int16_t MAIN_D_80124064[];
extern int16_t MAIN_D_80124118[];
extern int16_t MAIN_D_80134D3A;
extern int16_t MAIN_D_80134D38;
extern char *MOVE_NAMES[];
void renderSeparatorLines(int16_t *lines, int8_t count, int32_t zIndex);
void drawString(char *str, int32_t x, int32_t y);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void renderNumber(int32_t color, int32_t x, int16_t y, int32_t n, int32_t value, int32_t layer);

void renderDigimonMovesSelected(int16_t panel)
{
	RECT rect;
	Move *move;
	int32_t i;
	uint8_t row;
	uint8_t col;
#if defined(VERSION_JP)
	uint8_t status;
#endif
	uint8_t moveId;

	if (panel == 0) {
		renderSeparatorLines(MAIN_D_80124064, 0x12, 4);
	} else {
		renderSeparatorLines(MAIN_D_80124118, 0x12, 4);
	}

	renderRectPolyFT4((panel * 0x9d) - 0x91, 1, 4, 4, 0x78, 0x90, 5, 0x7b06, 4, 0);
	renderRectPolyFT4((panel * 0x9d) - 0x10, 1, 4, 4, 0x7c, 0x90, 5, 0x7b06, 4, 0);
	renderRectPolyFT4((panel * 0xcb) - 0x91, 0x5c, 4, 4, 0x78, 0x94, 5, 0x7b06, 4, 0);
	renderRectPolyFT4((panel * 0xcb) - 0x3e, 0x5c, 4, 4, 0x7c, 0x94, 5, 0x7b06, 4, 0);
	if (panel == 0) {
		renderRectPolyFT4(-0x10, 0x15, 4, 4, 0x7c, 0x94, 5, 0x7b06, 4, 0);
	} else {
		renderRectPolyFT4(0xc, 0x15, 4, 4, 0x78, 0x94, 5, 0x7b06, 4, 0);
	}
	renderBox((panel * 0x9d) - 0x8f, 3, 0x80, 0x13, 0x32, 0x32, 0x80, 0, 4);
	renderBox((panel * 0xcb) - 0x8f, 0x18, 0x57, 0x45, 0x32, 0x32, 0x80, 0, 4);
	renderRectPolyFT4((panel * 0x9d) - 0x8c, -2, 0x25, 7, 0x11, 0xb0, 5, 0x7b06, 3, 0);
	renderRectPolyFT4((panel * 0xcb) - 0x8c, 0x1d, 0x17, 7, 0x5c, 0xa2, 5, 0x7b06, 3, 0);
	renderRectPolyFT4((panel * 0xcb) - 0x8c, 0x2e, 0xb, 7, 0x74, 0xa2, 5, 0x7b06, 3, 0);
	renderRectPolyFT4((panel * 0xcb) - 0x8c, 0x3f, 0x13, 7, 0x4c, 0xa9, 5, 0x7b06, 3, 0);
	renderRectPolyFT4((panel * 0xcb) - 0x8c, 0x50, 0x10, 7, 0, 0xb0, 5, 0x7b06, 3, 0);

	for (i = 0; i < 4; i++) {
		renderRectPolyFT4((panel * 0xcb) - 0x71, i * 0x11 + 0x1b, 0xa, 0xa, 0x8a, 0x8c, 5, 0x7b06, 3, 0);
	}

	row = (MAIN_D_80134D3A - 0x73) / 18;
	col = (MAIN_D_80134D38 - 0x6f) / 15;
	if (col == 1) {
		col = 5;
	} else if (col == 2) {
		col = 1;
	} else if (col == 3) {
		col = 4;
	} else if (col == 4) {
		col = 2;
	} else if (col == 5) {
		col = 3;
	}
	moveId = col * 8 + row;
	if (col == 6) {
		moveId++;
	}

	if (hasMove(moveId)) {
		move = &MOVE_DATA[moveId];
		setRECT(&rect, 0, 0x84, 0x78, 0xc);
		clearTextSubArea(&rect);
		drawString(MOVE_NAMES[moveId], 0, 0x84);
		renderString(0, (panel * 0x9d) - 0x8a, 9, 0x78, 0xc, 0, 0x84, 3, 1);
		renderNumber(0, (panel * 0xcb) - 0x64, 0x1b, 3, move->power, 3);
		renderNumber(0, (panel * 0xcb) - 0x64, 0x2c, 3, move->mpCost * 3, 3);
		renderString(0, (panel * 0xcb) - 0x64, 0x3e, 0x24, 0xc, (move->range - 1) * 0x24, 0x78, 3, 1);
#if defined(VERSION_JP)
		status = move->status;
		if (status == 1) {
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0xc, 0xc, 0x84, 0x6c, 3, 1);
		} else if (status == 4) {
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0x24, 0xc, 0xc0, 0x6c, 3, 1);
		} else if (status != 0) {
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0x18, 0xc, (status - 2) * 0x18 + 0x90, 0x6c, 3, 1);
		}
#else
		switch (move->status) {
		case 1:
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0x1a, 0xc, 0x84, 0x6c, 3, 1);
			break;
		case 2:
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0x1c, 0xc, 0xa0, 0x6c, 3, 1);
			break;
		case 3:
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0x20, 0xc, 0xbc, 0x6c, 3, 1);
			break;
		case 4:
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0x1e, 0xc, 0xdb, 0x6c, 3, 1);
			break;
		}
#endif
	}
}

void renderDigimonMoveBox(void)
{
	int16_t moveId;
	int16_t moves[16];
	int32_t col;
	int32_t row;
	int32_t j;
	int16_t clut;
	uint8_t x;
	uint8_t special;
	int8_t blink;
	int32_t i;
	uint8_t y;

	for (i = 0; i < 3; i++) {
		if (EQUIPPED_MOVES[i] < 0x3a) {
			x = EQUIPPED_MOVES[i] % 8;
			if (EQUIPPED_MOVES[i] < 8) {
				y = 0;
			} else if (EQUIPPED_MOVES[i] < 0x10) {
				y = 2;
			} else if (EQUIPPED_MOVES[i] < 0x18) {
				y = 4;
			} else if (EQUIPPED_MOVES[i] < 0x20) {
				y = 5;
			} else if (EQUIPPED_MOVES[i] < 0x28) {
				y = 3;
			} else if (EQUIPPED_MOVES[i] < 0x30) {
				y = 1;
			} else {
				x = (EQUIPPED_MOVES[i] - 1) % 8;
				y = 6;
			}
			if (EQUIPPED_MOVES[i] == 0x30) {
				x = 4;
				y = 1;
			}
			if (EQUIPPED_MOVES[i] == 0x39) {
				x = 6;
				y = 6;
			}
			renderBox(x * 0x12 - 0x2a, y * 0xf - 7, 0xc, 0xc, 200, 0, 0x28, 1, 4);
		}
	}

	for (i = 0; i < 0x10; i++) {
		moves[i] = DIGIMON_DATA[ENTITY_TABLE[1]->type].moves[i];
	}
	sortArray(moves, 0x10);

	i = 0;
	for (row = 0; row < 7; row++) {
		for (col = 0; col < 8; col++) {
			moveId = row * 8 + col;
			if (moveId >= 0x30) {
				moveId++;
			}
			j = 0;
			if (moveId == 0x2c) {
				for (j = 0; j < 0x10; j++) {
					if (moves[j] == 0x2c || moves[j] == 0x30) {
						break;
					}
					if (j == 0xf) {
						renderBox(0x1e, 8, 0xc, 0xc, 0x68, 0x68, 0x68, 3, 4);
					}
				}
			}
			if (moveId == 0x37) {
				for (j = 0; j < 0x10; j++) {
					if (moves[j] == 0x37 || moves[j] == 0x39) {
						break;
					}
					if (j == 0xf) {
						renderBox(0x42, 0x53, 0xc, 0xc, 0x68, 0x68, 0x68, 3, 4);
					}
				}
			}
			if (moveId == moves[i]) {
				i++;
			} else if (moveId != 0x2c && moveId != 0x37 && moveId != 0x30 &&
				   moveId != 0x39) {
				if (moveId < 8) {
					y = 0;
				} else if (moveId < 0x10) {
					y = 2;
				} else if (moveId < 0x18) {
					y = 4;
				} else if (moveId < 0x20) {
					y = 5;
				} else if (moveId < 0x28) {
					y = 3;
				} else if (moveId < 0x30) {
					y = 1;
				} else {
					y = 6;
				}
				renderBox(col * 0x12 - 0x2a, y * 0xf - 7, 0xc, 0xc, 0x68, 0x68, 0x68, 3, 4);
			}
			if (hasMove(moveId) == 1) {
				special = MOVE_DATA[moveId].special;
				switch (special) {
				case 0:
					clut = 0x7a06;
					break;
				case 1:
					clut = 0x7a06;
					break;
				case 2:
					clut = 0x7a46;
					break;
				case 3:
					clut = 0x7a46;
					break;
				case 4:
					clut = 0x7a46;
					break;
				case 5:
					clut = 0x7a86;
					break;
				case 6:
					clut = 0x7a06;
				}
				if (PLAYTIME_FRAMES % 0x14 < 10) {
					blink = 0;
				} else {
					blink = 0xc;
				}
				renderRectPolyFT4(col * 0x12 - 0x2a, special * 0xf - 7, 0xc, 0xc,
						  blink + (special * 0x18 + 0x24), 0x80, 5, clut, 4, 0);
			}
		}
	}
}

int32_t drawMoveViewHelpStrings(void)
{
	RECT rect;

	rect = MAIN_D_801342F0;
	if (MENU_SUB_STATE == 0) {
		clearTextSubArea(&rect);
	}

	drawString(STATUS_VIEW_LABELS[MENU_SUB_STATE + 9], 0, MENU_SUB_STATE * 0xc + 0x18);
	MENU_SUB_STATE++;

	if (MENU_SUB_STATE == 8) {
		return 1;
	}

	return 0;
}

void renderDigiviceMedals(void)
{
	MATRIX m;

	m = GsWSMATRIX;
	FRAMEBUFFER_OT[0]->length = 9;
	FRAMEBUFFER_OT[0]->org = FRAMEBUFFER0_ORIGIN;
	FRAMEBUFFER_OT[1]->length = 9;
	FRAMEBUFFER_OT[1]->org = FRAMEBUFFER1_ORIGIN;

	GsSetProjection(0x400);
	GsSetRefView2(&MAIN_D_80123880);
	GsClearOt(0, 1, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER]);

	MAIN_D_80134238.vy += 0x64;
	RotMatrix(&MAIN_D_80134238, &MEDAL_COORDINATES.coord);
	MEDAL_COORDINATES.flg = 0;
	drawObject(&MEDAL_OBJECT, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], 5);
	GsSortOt(FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);

	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void sortArray(int16_t *arr, int8_t count)
{
	int32_t i;
	int32_t j;
	int16_t tmp;

	for (i = 0; i < count - 1; ++i) {
		for (j = i + 1; j < count; ++j) {
			if (arr[i] > arr[j]) {
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

void removeTriangleMenu(void)
{
	removeStaticUIBox(0);
	removeObject(0xfa4, 0);
}

void removeUIBox1(void)
{
	removeStaticUIBox(1);
}
