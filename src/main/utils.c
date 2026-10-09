#include <stdio.h>

#include <inline_n.h>
#include <libetc.h>

#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/main.h>
#include <dw/script.h>
#include <dw/std.h>
#include <dw/ui.h>
#include <dw/utils.h>
#include <dw/version.h>

#if !VERSION_IS(US)
#define DIGIT_WIDTH 12
#else
#define DIGIT_WIDTH 8
#endif

void damageTick(FighterData* fighter, Stats* stats);
void sortItemsById(uint8_t *data, long count);
void initStringFT4(POLY_FT4* poly);
#if !VERSION_IS(US)
void renderNumber(int32_t color, int16_t x, int16_t y, int16_t n,
		  int32_t value, int32_t layer);
#else
void renderNumber(int32_t color, int16_t x, int16_t y, int32_t n,
		  int32_t value, int32_t layer);
#endif
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
			  int32_t *digits);
void pauseFrame(void);
void setEntityTextDigit(POLY_FT4* poly, int32_t x, int32_t y);
void setPosDataPolyFT4(POLY_FT4 *prim, int16_t posX, int16_t posY, int16_t width, int16_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int16_t xPos, int16_t yPos, int16_t width, int16_t height);

extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[];
extern PACKET GS_WORK_BASES[2][0x14000];
extern DR_OFFSET DR_OFFSETS[2];

// clang-format off
RGB8 TEXT_COLORS[17] = {
	{ 0x80, 0x80, 0x80 },
	{ 0x19, 0x55, 0x80 },
	{ 0x69, 0xc2, 0xff },
	{ 0xff, 0x96, 0x46 },
	{ 0xc8, 0xb4, 0x32 },
	{ 0x1e, 0x80, 0x80 },
	{ 0xd0, 0x1e, 0x50 },
	{ 0x1e, 0xff, 0x1e },
	{ 0x50, 0x50, 0x50 },
	{ 0x6e, 0x6e, 0x6e },
	{ 0x46, 0x46, 0x46 },
	{ 0x00, 0x80, 0x00 },
	{ 0x80, 0x00, 0x80 },
	{ 0x40, 0x40, 0x40 },
	{ 0x70, 0x44, 0x2c },
	{ 0x48, 0x54, 0x7c },
	{ 0x7c, 0x4c, 0x68 },
};

/* six 5-byte formats: "%01d" to "%06d" */
char NUMBER_FORMATS[32] = "%01d\0%02d\0%03d\0%04d\0%05d\0%06d";
// clang-format on

void pauseFrame(void)
{
	ACTIVE_FRAMEBUFFER = GsGetActiveBuff();
	ACTIVE_ORDERING_TABLE = &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER];
	GsSetWorkBase(GS_WORK_BASES[ACTIVE_FRAMEBUFFER]);
	GsClearOt(0, 0, ACTIVE_ORDERING_TABLE);
	tickObjects();
	renderObjects();
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0x20,
		&DR_OFFSETS[ACTIVE_FRAMEBUFFER]);
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

#if VERSION_IS(JP)
	if (stats->current.currentHP < 0) {
#else
	if (stats->current.currentHP <= 0) {
#endif
		stats->current.currentHP = 0;
		fighter->hpDamageBuffer = 0;
	}
}

void sortItemsById(uint8_t *data, long count)
{
	int32_t i;
	int32_t j;
	int16_t minIdx;
	uint8_t minVal;

	for (i = 0; i < count; i++) {
		minVal = data[i];
		minIdx = i;

		for (j = i; j < count; j++) {
			if (minVal > data[j]) {
				minIdx = j;
				minVal = data[j];
			}
		}

		swapByte(&data[i], &data[minIdx]);
	}

	(void)j;
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

void getEntityScreenPos(e, boneId, out)
Entity *e;
int16_t boneId;
DVECTOR *out;
{
	MATRIX *w;
	SVECTOR v;

	GsSetLsMatrix(&GsWSMATRIX);
	w = &e->posData[boneId].posMatrix.workm;
	v.vx = w->t[0];
	v.vy = w->t[1];
	v.vz = w->t[2];
	gte_ldv0(&v);
	gte_rtps();
	gte_stsxy((long *)out);
	out->vx -= 0xA0 - DRAWING_OFFSET_X;
	out->vy -= 0x78 - DRAWING_OFFSET_Y;
}

void setEntityTextDigit(poly, x, y)
POLY_FT4 *poly;
int16_t x;
int16_t y;
{
	SetPolyFT4(poly);
	poly->tpage = getTPage(0, 0, 896, 256);
	setClut(poly, x, y);
}

void initStringFT4(POLY_FT4* poly)
{
	SetPolyFT4(poly);
	poly->tpage = getTPage(0, 0, 704, 256);
	setClut(poly, 0xD0, 0x1E8);
}

#if !VERSION_IS(US)
void renderNumber(int32_t color, int16_t x, int16_t y, int16_t n,
		  int32_t value, int32_t layer)
#else
void renderNumber(int32_t color, int16_t x, int16_t y, int32_t n,
		  int32_t value, int32_t layer)
#endif
{
	POLY_FT4 *prim;
	int32_t i;
	int32_t count;
	int32_t buf[6];

	prim = (POLY_FT4 *)GsGetWorkBase();

	convertValueToDigits(n, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		initStringFT4(prim);
		setRGB0(prim, TEXT_COLORS[color].r, TEXT_COLORS[color].g,
			TEXT_COLORS[color].g);
		setUVDataPolyFT4(prim, buf[i] * DIGIT_WIDTH, 0xf0, DIGIT_WIDTH, 12);
		setPosDataPolyFT4(prim, x + (((n - 1) - i) * DIGIT_WIDTH), y, DIGIT_WIDTH, 12);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

#if !VERSION_IS(US)
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
			  int32_t *digits)
{
	char buf[8];
	int32_t i;

	sprintf(buf, &NUMBER_FORMATS[(n - 1) * 5], value);
	for (i = 0; i < n; i++) {
		*(digits + n - 1 - i) = buf[i] - '0';
	}
	*outCount = n;
	for (i = n - 1; i >= 0; i--) {
		if (digits[i] != 0) {
			break;
		}
		if (i != 0) {
			(*outCount)--;
		}
	}
}
#else
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
			  int32_t *digits)
{
	char buf[8];
	int32_t i;
	int32_t j;
	long long nv;
	int32_t off;
	char *base;
	int32_t cnt;

	sprintf(buf, &NUMBER_FORMATS[(j = n - 1) * 5], value);
	i = 0;
	off = 0;
	n = cnt = n;
	base = (char *)(digits + n) - 4;
	for (; i < n; i++) {
		*(int32_t *)(base - off) = buf[i] - '0';
		off += 4;
	}
	nv = j;
	*outCount = cnt;
	for (i = nv; i >= 0; i--) {
		if (digits[i] != 0) {
			break;
		}
		if (i != 0) {
			(*outCount)--;
		}
	}
}
#endif

void setUVDataPolyFT4(POLY_FT4 *p, int16_t u, int16_t v, int16_t w, int16_t h)
{
	setUVWH(p, u, v, w, h);
}

void setPosDataPolyFT4(POLY_FT4 *p, int16_t x, int16_t y, int16_t w, int16_t h)
{
	setXYWH(p, x, y, w, h);
}
