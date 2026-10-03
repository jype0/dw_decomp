#include <dw/evl.h>
#include <dw/ui.h>

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

typedef struct {
	int16_t posX;
	int16_t posY;
	uint8_t u;
	uint8_t v;
	uint8_t clut;
	uint8_t pad;
} EvoChartEntry;

typedef struct {
	int16_t m[5];
} EvoClutTable;

typedef struct {
	int16_t x1;
	int16_t x2;
	int16_t x3;
	int16_t x4;
	int16_t y1;
	int16_t y2;
	int16_t y3;
	int16_t y4;
} Line4Points;

typedef struct {
	int16_t posX;
	int16_t posY;
	int16_t unk4;
	int16_t unk6;
} ChartSprite;

typedef struct {
	int16_t posX;
	int16_t posY;
	int16_t width;
	int16_t height;
} Inset;

extern int16_t MAP_OBJECT_INSTANCE_COUNT;
extern LocalMapObject LOCAL_MAP_OBJECTS[];
extern int32_t MAP_OBJECT_MOVE_TO_DATA[];
extern int8_t MENU_SUB_STATE;
extern EvoChartEntry MAIN_D_80124544[];
extern int16_t MAIN_D_80134D40;
extern Line4Points MAIN_D_80124944[];
extern Line4Points MAIN_D_80124984[];
extern ChartSprite MAIN_D_80124AA8[];
extern ChartSprite MAIN_D_80124AC8[];
extern Line4Points MAIN_D_801249D4[];
extern Line4Points MAIN_D_80124A34[];
extern ChartSprite MAIN_D_80124AF0[];
extern ChartSprite MAIN_D_80124B20[];
extern RGB8 MAIN_D_80124A84[];
extern RGB8 MAIN_D_80124A85[];
extern RGB8 MAIN_D_80124A86[];
extern Inset MAIN_D_80124B48[];
extern EvoClutTable MAIN_D_80123E6C;

int32_t drawEvoChartStrings(int32_t arg);
void renderRectPolyFT4(int16_t posX, int16_t posY, int32_t width,
		       int32_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag);
void renderBorderBox(int16_t x, int16_t y, int16_t w, int16_t h, int32_t c1,
		     int32_t c2, uint8_t r, uint8_t g, uint8_t b, int32_t a10);
void renderString();
int32_t hasDigimonRaised(int32_t id);
void drawLine3P(int32_t color, int32_t x0, int32_t y0, int32_t x1,
		int32_t y1, int32_t x2, int32_t y2, int32_t otz,
		int32_t flag);
void drawLine2P(int32_t color, int32_t x0, int32_t y0, int32_t x1,
		int32_t y1, int32_t otz, int32_t flag);
int32_t strlen(char *s);
void renderInsetBox(int16_t a, int16_t b, int16_t c, int16_t d, int32_t otz);
void renderEvoChartDetail(void);
int32_t randomLimit(int32_t max);

static void renderEvoChartDetail__garbage__(LocalMapObjectInstance *mapObjects,
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

void renderEvoChartDetail(void)
{
	Line4Points *fromLines;
	Line4Points *toLines;
	int32_t j;
	ChartSprite *fromSprites;
	ChartSprite *toSprites;
	uint32_t color1;
	uint32_t color2;
	int32_t id;
	int32_t len;
	EvoClutTable clut;
	int32_t i;
	int8_t count1;
	int8_t count2;
	int8_t fromCount;
	int8_t toCount;

	clut = MAIN_D_80123E6C;
	MENU_SUB_STATE = 2;
	drawEvoChartStrings((int8_t)MAIN_D_80134D40);
	count1 = (count2 = 0);
	for (i = 0; i < 5; i++) {
		if (EVO_PATHS_DATA[MAIN_D_80134D40 - 1].from[i] != -1) {
			count1++;
		}
	}
	for (i = 0; i < 6; i++) {
		if (EVO_PATHS_DATA[MAIN_D_80134D40 - 1].to[i] != -1) {
			count2++;
		}
	}

	if ((count1 % 2) == 0) {
		fromLines = MAIN_D_80124944;
		fromSprites = MAIN_D_80124AA8;
		fromCount = 4;
	} else {
		fromLines = MAIN_D_80124984;
		fromSprites = MAIN_D_80124AC8;
		fromCount = 5;
	}
	if ((count2 % 2) == 0) {
		toLines = MAIN_D_801249D4;
		toSprites = MAIN_D_80124AF0;
		toCount = 6;
	} else {
		toLines = MAIN_D_80124A34;
		toSprites = MAIN_D_80124B20;
		toCount = 5;
	}

	for (j = 0; j < fromCount; j++) {
		if (EVO_PATHS_DATA[MAIN_D_80134D40 - 1].from[j] > 0) {
			drawLine3P(0x65db,
				   fromLines->x1, fromLines->y1 - 1,
				   fromLines->x2, fromLines->y2 - 1,
				   fromLines->x3, fromLines->y3 - 1,
				   4, 0);
			drawLine2P(0x65db,
				   fromLines->x3, fromLines->y3 - 1,
				   fromLines->x4, fromLines->y4 - 1,
				   4, 0);
			drawLine3P(0x794e3,
				   fromLines->x1, fromLines->y1,
				   fromLines->x2, fromLines->y2,
				   fromLines->x3, fromLines->y3,
				   4, 0);
			drawLine2P(0x794e3,
				   fromLines->x3, fromLines->y3,
				   fromLines->x4, fromLines->y4,
				   4, 0);
			drawLine3P(0x65db,
				   fromLines->x1, fromLines->y1 + 1,
				   fromLines->x2, fromLines->y2 + 1,
				   fromLines->x3, fromLines->y3 + 1,
				   4, 0);
			drawLine2P(0x65db,
				   fromLines->x3, fromLines->y3 + 1,
				   fromLines->x4, fromLines->y4 + 1,
				   4, 0);
		}
		fromLines++;
	}

	for (j = 0; j < toCount; j++) {
		if (EVO_PATHS_DATA[MAIN_D_80134D40 - 1].to[j] > 0) {
			color1 = (MAIN_D_80124A84[j * 2].r & 0xff) |
				 ((MAIN_D_80124A84[j * 2].g & 0xff) << 8) |
				 ((MAIN_D_80124A84[j * 2].b & 0xff) << 16);
			color2 = (MAIN_D_80124A84[(j * 2) + 1].r & 0xff) |
				 ((MAIN_D_80124A84[(j * 2) + 1].g & 0xff) << 8) |
				 ((MAIN_D_80124A84[(j * 2) + 1].b & 0xff) << 16);
			drawLine3P(color2,
				   toLines->x1, toLines->y1 - 1,
				   toLines->x2, toLines->y2 - 1,
				   toLines->x3, toLines->y3 - 1,
				   4, 0);
			drawLine2P(color2,
				   toLines->x3, toLines->y3 - 1,
				   toLines->x4, toLines->y4 - 1,
				   4, 0);
			drawLine3P(color1,
				   toLines->x1, toLines->y1,
				   toLines->x2, toLines->y2,
				   toLines->x3, toLines->y3,
				   4, 0);
			drawLine2P(color1,
				   toLines->x3, toLines->y3,
				   toLines->x4, toLines->y4,
				   4, 0);
			drawLine3P(color2,
				   toLines->x1, toLines->y1 + 1,
				   toLines->x2, toLines->y2 + 1,
				   toLines->x3, toLines->y3 + 1,
				   4, 0);
			drawLine2P(color2,
				   toLines->x3, toLines->y3 + 1,
				   toLines->x4, toLines->y4 + 1,
				   4, 0);
		}
		toLines++;
	}

	renderRectPolyFT4(-8, -0x14, 0x10, 0x10,
			  MAIN_D_80124544[MAIN_D_80134D40 - 1].u,
			  MAIN_D_80124544[MAIN_D_80134D40 - 1].v, 0x18,
			  clut.m[MAIN_D_80124544[MAIN_D_80134D40 - 1].clut],
			  4, 0);
	renderBorderBox(0x97, 99, 0x12, 0x12, 0xbebebe, 0x3c3c3c, 0x87, 0x87,
			0x87, 4);

	for (j = 0; j < fromCount; j++) {
		id = EVO_PATHS_DATA[MAIN_D_80134D40 - 1].from[j];
		if (id > 0) {
			if (hasDigimonRaised(id & 0xffff) == 1) {
				renderRectPolyFT4(fromSprites->posX, fromSprites->posY,
						  0x10, 0x10,
						  MAIN_D_80124544[id - 1].u,
						  MAIN_D_80124544[id - 1].v, 0x18,
						  clut.m[MAIN_D_80124544[id - 1].clut],
						  4, 0);
			}
			renderBorderBox(fromSprites->posX + 0x9f,
					fromSprites->posY + 0x77, 0x12, 0x12,
					0xbebebe, 0x3c3c3c, 0x87, 0x87, 0x87, 4);
		}
		fromSprites++;
	}

	for (j = 0; j < toCount; j++) {
		id = EVO_PATHS_DATA[MAIN_D_80134D40 - 1].to[j];
		if (id > 0) {
			if (hasDigimonRaised(id & 0xffff) == 1) {
				renderRectPolyFT4(toSprites->posX, toSprites->posY,
						  0x10, 0x10,
						  MAIN_D_80124544[id - 1].u,
						  MAIN_D_80124544[id - 1].v, 0x18,
						  clut.m[MAIN_D_80124544[id - 1].clut],
						  4, 0);
			}
			renderBorderBox(toSprites->posX + 0x9f,
					toSprites->posY + 0x77, 0x12, 0x12,
					0xbebebe, 0x3c3c3c, 0x87, 0x87, 0x87, 4);
		}
		toSprites++;
	}

#if defined(VERSION_JP)
	renderString(3, -0x19, -0x4f, 0x30, 0xc, 0, 0x18, 4);
	renderString(0, -0x56, 0x3a, 0x24, 0xc,
		     ((DIGIMON_DATA[MAIN_D_80134D40].level - 1) * 36) + 0x30, 0x18,
		     4);
#else
	renderString(3, -0x14, -0x4f, 0x24, 0xc, 0, 0x18, 4);
	switch (DIGIMON_DATA[MAIN_D_80134D40].level) {
	case 1:
		renderString(0, -0x5b, 0x3a, 0x2e, 0xc, 0x24, 0x18, 4);
		break;
	case 2:
		renderString(0, -0x63, 0x3a, 0x3e, 0xc, 0x52, 0x18, 4);
		break;
	case 3:
		renderString(0, -0x5b, 0x3a, 0x2f, 0xc, 0, 0x24, 4);
		break;
	case 4:
		renderString(0, -0x64, 0x3a, 0x41, 0xc, 0x2f, 0x24, 4);
		break;
	case 5:
		renderString(0, -0x64, 0x3a, 0x41, 0xc, 0, 0x3c, 4);
	}
#endif
	len = strlen(DIGIMON_DATA[MAIN_D_80134D40].name) / 2;
	renderString(0, -0x5c - ((len - 4) * 6), 0x4d, 0x78, 0xc, 0, 0x30, 4);
	for (i = 0; i < 6; i++) {
		renderInsetBox(MAIN_D_80124B48[i].posX, MAIN_D_80124B48[i].posY,
			       MAIN_D_80124B48[i].width,
			       MAIN_D_80124B48[i].height, 4);
	}
}
