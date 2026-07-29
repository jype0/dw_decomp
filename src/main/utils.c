#include <libcd.h>

#include <dw/clock.h>
#include <dw/combat.h>
#include <dw/main.h>
#include <dw/params.h>
#include <dw/entity.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/ui.h>
#include <dw/utils.h>

#include <mwinline_n.h>

#include "common.h"

void MAIN_func_800D92EC(int32_t a, int32_t b, int32_t c, int32_t d);
void MAIN_thunk_func_800D92EC(int32_t a, int32_t b, int32_t c, int32_t d);
extern char MAIN_D_801345C4[];
extern uint16_t MAIN_D_80134FFE;
void setMapObjectsFlag(int16_t a, int16_t b, int32_t flag);
typedef struct {
	uint8_t map;
	uint8_t lightOn;
	int16_t a;
	int16_t b;
	uint16_t trigger;
} MapLightEntry;

void updateMapLightState(void);


void damageTick(FighterData* fighter, Stats* stats);
void sortItemsById(uint8_t *data, int32_t count);
void setMapLayerEnabled(uint8_t enabled);
int32_t isInvisible(Entity* entity);
int32_t isTamerOnScreen(void);
void startMovie(int32_t movieId);
void playMovie(int32_t movieId, dw_bool shouldPlay);
void initializeFramebuffer(void);
void initStringFT4(POLY_FT4* poly);
int32_t sprintf(char *dst, char *fmt, ...);
extern char MAIN_D_8012B94C[];
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
			  int32_t *digits);
void renderSelectionCursor(int32_t x, int32_t y, int32_t w, int32_t h,
			   int32_t layer);
void renderTrianglePrimitive();
void loadStackedTIMFile(char *path);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
		  int32_t f, int32_t g, int32_t h, int32_t i);
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
long VSync(long mode);
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[];
extern PACKET GS_WORK_BASES[];
extern char DRAW_OFFSETS[];
int32_t GsGetActiveBuff(void);
void tickObjects(void);
void renderObjects(void);
void pauseFrame(void);
void renderItemSprite(int32_t type, int16_t x, int16_t y, int32_t layer);
extern char *MAIN_D_8012BA8C[];
extern void *MAIN_D_8012B96C[];
void readFile(char *path, void *dest);
void addFileReadRequestPath();
extern uint8_t MAIN_D_80127BDC[];
void setItemTexture(POLY_FT4 *p, int32_t id);
void entityLookAtLocation(int32_t *loc);
uint8_t entityGetTechFromAnim(Entity *e, int32_t anim);
void entityLookAtTile(int32_t tileX, int32_t tileY);
void MAIN_func_800E53B4(POLY_FT4* poly, int32_t x, int32_t y);
void MAIN_func_800E642C(void);
int32_t hasMove(int32_t move);
void learnMove(int32_t move);
void MOV_func_80010620(int32_t movieId);
void createPauseBox(void);
void renderPauseBox(int32_t instanceId);
void drawString(char *text, int32_t color, int32_t pos);
void setPosDataPolyFT4(POLY_FT4 *prim, int16_t posX, int16_t posY, int16_t width, int16_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int16_t xPos, int16_t yPos, int16_t width, int16_t height);
void MAIN_func_800E60E0(int16_t x, int16_t y, int16_t u, int32_t otOffset);

extern GsOT *ACTIVE_ORDERING_TABLE;

extern uint8_t MAP_LAYER_ENABLED;
extern int32_t MAIN_D_80134E7C;
extern char MAIN_D_80134430;

void pauseFrame(void)
{
	ACTIVE_FRAMEBUFFER = GsGetActiveBuff();
	ACTIVE_ORDERING_TABLE = &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER];
	GsSetWorkBase(&GS_WORK_BASES[ACTIVE_FRAMEBUFFER * 0x14000]);
	GsClearOt(0, 0, ACTIVE_ORDERING_TABLE);
	tickObjects();
	renderObjects();
	AddPrim((char *)ACTIVE_ORDERING_TABLE->org + 0x80,
		&DRAW_OFFSETS[ACTIVE_FRAMEBUFFER * 0xC]);
	DrawSync(0);
	VSync(3);
	GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
	GsSwapDispBuff();
	GsSortClear(0, 0, 0, ACTIVE_ORDERING_TABLE);
	GsDrawOt(ACTIVE_ORDERING_TABLE);
}

void damageTick(FighterData* fighter, Stats* stats)
{
	if (fighter->hpDamageBuffer > 999) {
		stats->current.currentHP -= 900;
		fighter->hpDamageBuffer -= 900;
	}

	if (fighter->hpDamageBuffer > 99) {
		stats->current.currentHP -= 80;
		fighter->hpDamageBuffer -= 80;
	}

	if (fighter->hpDamageBuffer > 9) {
		stats->current.currentHP -= 6;
		fighter->hpDamageBuffer -= 6;
	}

	if (fighter->hpDamageBuffer > 0) {
		stats->current.currentHP -= 1;
		fighter->hpDamageBuffer -= 1;
	}

	if (stats->current.currentHP <= 0) {
		stats->current.currentHP = 0;
		fighter->hpDamageBuffer = 0;
	}
}

void sortItemsById(uint8_t *data, int32_t count)
{
	int32_t i;
	int32_t j;
	int16_t minIdx;
	uint8_t minVal;
	uint8_t *dst;

	for (i = 0, dst = data; i < count; ++i, ++dst) {
		minVal = data[i];
		minIdx = i;

		for (j = i; j < count; ++j) {
			if (data[j] < minVal) {
				minIdx = j;
				minVal = data[j];
			}
		}

		swapByte(dst, &data[minIdx]);
	}
}

void swapByte(uint8_t *a, uint8_t *b)
{
	uint8_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void swapShort(int16_t *a, int16_t *b)
{
	int16_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void swapInt(int32_t *a, int32_t *b)
{
	int32_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void getEntityScreenPos(Entity *e, int32_t boneId, int16_t *out)
{
	MATRIX *w;
	SVECTOR v;
	int16_t ox;

	GsSetLsMatrix(&GsWSMATRIX);
	w = &e->posData[boneId].posMatrix.workm;
	v.vx = w->t[0];
	v.vy = w->t[1];
	v.vz = w->t[2];
	gte_ldv0(&v);
	gte_rtps();
	gte_stsxy((long *)out);
	ox = 0xA0 - DRAWING_OFFSET_X;
	out[0] = out[0] - ox;
	out[1] -= 0x78 - DRAWING_OFFSET_Y;
}

void MAIN_func_800E53B4(POLY_FT4* poly, int32_t x, int32_t y)
{
	SetPolyFT4(poly);
	poly->tpage = 0x1E;
	poly->clut = GetClut(x, y);
}

void initStringFT4(POLY_FT4* poly)
{
	SetPolyFT4(poly);
	poly->tpage = 0x1B;
	poly->clut = GetClut(0xD0, 0x1E8);
}

INCLUDE_ASM("asm/main/nonmatchings/utils", renderNumber);

INCLUDE_ASM("asm/main/nonmatchings/utils", convertValueToDigits);

void setUVDataPolyFT4(POLY_FT4 *p, int16_t u, int16_t v, int16_t w, int16_t h)
{
	p->u0 = u;
	p->v0 = v;
	p->u1 = u + w;
	p->v1 = v;
	p->u2 = u;
	p->v2 = v + h;
	p->u3 = u + w;
	p->v3 = v + h;
}

void setPosDataPolyFT4(POLY_FT4 *p, int16_t x, int16_t y, int16_t w, int16_t h)
{
	p->x0 = x;
	p->y0 = y;
	p->x1 = x + w;
	p->y1 = y;
	p->x2 = x;
	p->y2 = y + h;
	p->x3 = x + w;
	p->y3 = y + h;
}

INCLUDE_ASM("asm/main/nonmatchings/utils", MAIN_func_800E5724);

INCLUDE_ASM("asm/main/nonmatchings/utils", MAIN_func_800E58A4);

INCLUDE_ASM("asm/main/nonmatchings/utils", renderSelectionCursor);

INCLUDE_ASM("asm/main/nonmatchings/utils", renderString);

INCLUDE_ASM("asm/main/nonmatchings/utils", renderItemSprite);

INCLUDE_ASM("asm/main/nonmatchings/utils", setItemTexture);

int32_t hasMove(int32_t move)
{
	if (PARTNER_ENTITY.learnedMoves[move / 32] & (1 << (move % 32))) {
		return 1;
	}

	return 0;
}

void learnMove(int32_t move)
{
	if (move == 0x2C || move == 0x30) {
		PARTNER_ENTITY.learnedMoves[1] |= 0x1000;
		PARTNER_ENTITY.learnedMoves[1] |= 0x10000;
	} else if (move == 0x37 || move == 0x39) {
		PARTNER_ENTITY.learnedMoves[1] |= 0x800000;
		PARTNER_ENTITY.learnedMoves[1] |= 0x2000000;
	} else {
		PARTNER_ENTITY.learnedMoves[move / 32] |= 1 << (move % 32);
	}
}

uint8_t entityGetTechFromAnim(Entity *e, int32_t anim)
{
  uint8_t new_var4;
  int32_t *new_var;
  int new_var3;
  int new_var2;
  if (anim == 0xFF)
  {
    return 0xFF;
  }
  if ((e->type == 0x3C) && (anim == 0x3C))
  {
    return 0x70;
  }
  new_var = (int32_t *) e;
  new_var2 = 0x23;
  e = e;
  new_var3 = 0x2E;
  new_var4 = ((uint8_t *) (((char *) DIGIMON_DATA) + new_var2))[((new_var[0] * 0x34) & 0xFFFFFFFFFFFFFFFF) + (anim - new_var3)];
  return new_var4;
}

INCLUDE_ASM("asm/main/nonmatchings/utils", entityLookAtTile);

void MAIN_func_800E60E0(int16_t x, int16_t y, int16_t u, int32_t otOffset)
{
	POLY_FT4 *prim;

	prim = (POLY_FT4 *)GsGetWorkBase();
	MAIN_func_800E53B4(prim, 0x100, 0x1E6);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setUVDataPolyFT4(prim, u, 0xB8, 8, 8);
	setPosDataPolyFT4(prim, x, y, 8, 8);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[otOffset], prim++);
	GsSetWorkBase((PACKET *)prim);
}

int32_t isInvisible(Entity* entity)
{
	if (entity == NULL || entity->isOnScreen == 0 || entity->isOnMap == 0) {
		return 1;
	}

	return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/utils", loadDynamicLibrary);

void startMovie(int32_t movieId)
{
	uint8_t isComplete;

	CdInit();
	ResetGraph(0);
	SetGraphDebug(0);
	loadDynamicLibrary(MOV_REL, &isComplete, 0, NULL, NULL);
	MOV_func_80010620(movieId);
	ResetGraph(3);
}

INCLUDE_ASM("asm/main/nonmatchings/utils", handlePause);

void MAIN_func_800E642C(void)
{
	if (MAIN_D_80134E7C != 0) {
		removeStaticUIBox(5);
		MAIN_D_80134E7C = 0;
		playSound(0, 3);
	}
}

void createPauseBox(void)
{
	RECT pos;

	if (MAIN_D_80134E7C != 1) {
		drawString(&MAIN_D_80134430, 0x78, 0xF0);
		pos.x = -0x1A;
		pos.y = -0xE;
		pos.w = 0x38;
		pos.h = 0x18;
		createStaticUIBox(5, 1, 0, &pos, NULL, renderPauseBox);
		MAIN_D_80134E7C = 1;
		playSound(0, 3);
	}
}

void renderPauseBox(int32_t instanceId)
{
  GsBOXF box;
  RECT *new_var;
  box.attribute = 0x40000000;
  box.b = 0;
  box.g = 0;
  box.r = 0;
  box.x = -0xA0;
  box.y = -0x78;
  box.w = 0x140;
  box.h = 0xF0;
  GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 7 - instanceId);
  new_var = &UI_BOX_DATA[5].finalPos;
  renderString(0, (int16_t) ((*new_var).x + 6), (int16_t) ((*new_var).y + 6), 0x2A, 0xC, 0x78, 0xF0, 0, 1);
}

void MAIN_thunk_func_800D92EC(int32_t a, int32_t b, int32_t c, int32_t d)
{
	MAIN_func_800D92EC(a, b, c, d);
}

void setMapLayerEnabled(uint8_t enabled)
{
	MAP_LAYER_ENABLED = enabled;
}

void loadStackedTIMFile(char *path)
{
	GsIMAGE img;
	int32_t *p;

	p = (int32_t *)GENERAL_BUFFER;
	readFile(path, GENERAL_BUFFER);
	while (*p == 0x10) {
		p = (int32_t *)((char *)p + 4);
		GsGetTimInfo((u_long *)p, &img);
		p = &p[((img.pw * img.ph) / 2) + 4];
		LoadImage((RECT *)&img.px, img.pixel);
		if ((img.pmode >> 3) & 1) {
			LoadImage((RECT *)&img.cx, img.clut);
			p = &p[((img.cw * img.ch) / 2) + 3];
		}
	}
}

void playMovie(int32_t movieId, dw_bool shouldPlay)
{
	if (shouldPlay) {
		startMovie(movieId);
		initializeFramebuffer();
	}
}

INCLUDE_ASM("asm/main/nonmatchings/utils", unlearnMove);

int32_t isTamerOnScreen(void)
{
	if (ENTITY_TABLE[0]->isOnScreen == 1) {
		return 1;
	}

	return 0;
}

void updateMapLightState(void)
{
	int32_t i;
	MapLightEntry *p;

	p = (MapLightEntry *)MAIN_D_801345C4;
	for (i = 0; i <= 0; i++, p++) {
		if ((p->map == (MAIN_D_80134FFE & 0xFF)) &&
		    ((p->trigger == 0xFFFF) || (isTriggerSet(p->trigger) != 0))) {
			if ((HOUR >= 7) && (HOUR < 0x13)) {
				if (p->lightOn == 0) {
					setMapObjectsFlag(p->a, p->b, 0);
				} else {
					setMapObjectsFlag(p->a, p->b, 1);
				}
			} else if (p->lightOn == 1) {
				setMapObjectsFlag(p->a, p->b, 0);
			} else {
				setMapObjectsFlag(p->a, p->b, 1);
			}
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/utils", startTournament);
