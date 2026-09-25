#include <stdio.h>

#include <libetc.h>
#include <mwinline_n.h>

#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/main.h>
#include <dw/script.h>
#include <dw/std.h>
#include <dw/ui.h>
#include <dw/utils.h>

#define shop_START		((char *)0x80080800)

void damageTick(FighterData* fighter, Stats* stats);
void sortItemsById(uint8_t *data, int32_t count);
void initStringFT4(POLY_FT4* poly);
void renderNumber(int32_t color, int32_t x, int16_t y, int32_t n,
		  int32_t value, int32_t layer);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
			  int32_t *digits);
void pauseFrame(void);
void setEntityTextDigit(POLY_FT4* poly, int32_t x, int32_t y);
void setPosDataPolyFT4(POLY_FT4 *prim, int16_t posX, int16_t posY, int16_t width, int16_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int16_t xPos, int16_t yPos, int16_t width, int16_t height);

extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[];
extern PACKET GS_WORK_BASES[];
extern char DRAW_OFFSETS[];
extern char btl_START[];
extern char dget_START[];
extern char doo2_START[];
extern char dooa_START[];
extern char eab_START[];
extern char endi_START[];
extern char evl_START[];
extern char fish_START[];
extern char kar_START[];
extern char mov_START[];
extern char murd_START[];
extern char std_START[];
extern char trn_START[];
extern char trn2_START[];
extern char vs_START[];

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
char ZERO_PAD_FORMATS[32] = "%01d\0%02d\0%03d\0%04d\0%05d\0%06d";

void *OVERLAY_LOAD_ADDRESSES[16] = {
	btl_START,
	std_START,
	fish_START,
	evl_START,
	kar_START,
	vs_START,
	mov_START,
	doo2_START,
	dooa_START,
	trn_START,
	shop_START,
	dget_START,
	trn2_START,
	murd_START,
	endi_START,
	eab_START,
};

char OVERLAY_FILE_BTL_REL_BIN[12] = "BTL_REL.BIN";

char OVERLAY_FILE_STD_REL_BIN[12] = "STD_REL.BIN";

char OVERLAY_FILE_FISH_REL_BIN[] = "FISH_REL.BIN";

char OVERLAY_FILE_EVL_REL_BIN[12] = "EVL_REL.BIN";

char OVERLAY_FILE_KAR_REL_BIN[12] = "KAR_REL.BIN";

char OVERLAY_FILE_VS_REL_BIN[] = "VS_REL.BIN";

char OVERLAY_FILE_MOV_REL_BIN[12] = "MOV_REL.BIN";

char OVERLAY_FILE_DOO2_REL_BIN[] = "DOO2_REL.BIN";

char OVERLAY_FILE_DOOA_REL_BIN[] = "DOOA_REL.BIN";

char OVERLAY_FILE_TRN_REL_BIN[12] = "TRN_REL.BIN";

char OVERLAY_FILE_SHOP_REL_BIN[] = "SHOP_REL.BIN";

char OVERLAY_FILE_DGET_REL_BIN[] = "DGET_REL.BIN";

char OVERLAY_FILE_TRN2_REL_BIN[] = "TRN2_REL.BIN";

char OVERLAY_FILE_MURD_REL_BIN[] = "MURD_REL.BIN";

char OVERLAY_FILE_ENDI_REL_BIN[] = "ENDI_REL.BIN";

char OVERLAY_FILE_EAB_REL_BIN[12] = "EAB_REL.BIN";

char *OVERLAY_FILE_NAMES[16] = {
	OVERLAY_FILE_BTL_REL_BIN,
	OVERLAY_FILE_STD_REL_BIN,
	OVERLAY_FILE_FISH_REL_BIN,
	OVERLAY_FILE_EVL_REL_BIN,
	OVERLAY_FILE_KAR_REL_BIN,
	OVERLAY_FILE_VS_REL_BIN,
	OVERLAY_FILE_MOV_REL_BIN,
	OVERLAY_FILE_DOO2_REL_BIN,
	OVERLAY_FILE_DOOA_REL_BIN,
	OVERLAY_FILE_TRN_REL_BIN,
	OVERLAY_FILE_SHOP_REL_BIN,
	OVERLAY_FILE_DGET_REL_BIN,
	OVERLAY_FILE_TRN2_REL_BIN,
	OVERLAY_FILE_MURD_REL_BIN,
	OVERLAY_FILE_ENDI_REL_BIN,
	OVERLAY_FILE_EAB_REL_BIN,
};

uint16_t TOURNAMENT_MEDAL_IDS[6] = {
	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x000c,
};

uint8_t FLEE_BUBBLE_SCALE[10] = {
	0x64, 0x68, 0x6c, 0x70, 0x74, 0x78, 0x74, 0x70,
	0x6c, 0x68,
};

uint8_t BATTLE_MUSIC[48] = {
	0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x02, 0x02,
	0x02, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x02, 0x01, 0x02, 0x02, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x01, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
	0x02, 0x01, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00,
};

uint8_t FLEE_CHANCE_TABLE[12] = {
	0x3c, 0x46, 0x64, 0x50, 0x3c, 0x64, 0x5a, 0x64,
	0x1e, 0x00, 0x00, 0x00,
};

uint8_t CONCAVE_SCREENS[18] = {
	0xa6, 0xa7, 0xd2, 0xd4, 0xdb, 0xe2, 0xe3, 0xe4,
	0xf7, 0xf8, 0xf9, 0xfd, 0xfe, 0xa1, 0x84, 0x02,
	0x0d, 0x65,
};

char TEXT_PAUSE[] = "Pause";
// clang-format on

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

void setEntityTextDigit(POLY_FT4* poly, int32_t x, int32_t y)
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

void renderNumber(int32_t color, int32_t x, int16_t y, int32_t n,
		  int32_t value, int32_t layer)
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
		setUVDataPolyFT4(prim, buf[i] * 8, 0xf0, 8, 12);
		setPosDataPolyFT4(prim, x + (((n - 1) - i) * 8), y, 8, 12);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

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

	sprintf(buf, &ZERO_PAD_FORMATS[(j = n - 1) * 5], value);
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

void setUVDataPolyFT4(POLY_FT4 *p, int16_t u, int16_t v, int16_t w, int16_t h)
{
	setUVWH(p, u, v, w, h);
}

void setPosDataPolyFT4(POLY_FT4 *p, int16_t x, int16_t y, int16_t w, int16_t h)
{
	setXYWH(p, x, y, w, h);
}
