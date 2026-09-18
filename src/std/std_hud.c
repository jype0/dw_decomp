#include <stdlib.h>

#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <mwinline_n.h>

#include <dw/aabb.h>
#include <dw/anim.h>
#include <dw/battle.h>
#include <dw/clock.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/std.h>
#include <dw/types.h>

#include "common.h"

typedef struct {
	uint32_t *vertTop;
	int32_t numVert;
	uint32_t *normalTop;
	int32_t numNormal;
	uint32_t *primTop;
	int32_t numPrim;
	int32_t scale;
} StdTmdObject;

extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern int16_t MAIN_D_8013518C[2];
extern int32_t MAIN_D_80135194;
extern uint8_t MAIN_D_80135190;
extern int16_t MAIN_D_801351A4;
extern uint8_t MAIN_D_801351B4;
extern uint8_t MAIN_D_80134D64;
extern int8_t GAME_STATE;
extern uint8_t MAIN_D_80135198[2];
extern uint8_t MAIN_D_8013519A[2];
extern uint8_t MAIN_D_8013519C[2];
extern uint8_t MAIN_D_8013519E[2];
extern uint8_t MAIN_D_801351A0[2];
extern uint8_t MAIN_D_801351A2[2];
extern int16_t MAIN_D_8013518E;
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t ACTIVE_FRAMEBUFFER;
extern int32_t VIEWPORT_DISTANCE;
extern uint8_t *MAIN_D_801351A8;
extern uint8_t CURRENT_SCREEN;
extern int32_t MAIN_D_801351AC;
extern int32_t MAIN_D_801350F0;
extern uint8_t *MAIN_D_801351B0;

void STD_func_8006B1F0(uint32_t *tmd, int32_t vofs, int32_t nofs, int32_t objIdx);
void STD_func_8006B6F4(void);
void STD_func_8005A830(void);
void STD_renderBattleStartText(void);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount, int32_t *buf);
void setEntityTextDigit(POLY_FT4 *poly, int32_t x, int32_t y);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t u, int32_t v, int32_t w, int32_t h);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t x, int32_t y, int32_t w, int32_t h);
void swapByte(uint8_t *a, uint8_t *b);
void STD_renderBattleStartTextBurst(void);
void STD_tickFinisherChargeup(void);
void STD_renderFinisherChargeup(void);
void STD_removeFinisherChargeup(void);
void STD_shuffleBattleStartTextPieces(void);
void STD_initializeBattleStartText(void);
void STD_func_8006A044(void);
void STD_initializeBattleStartTextBurst(void);
void STD_func_8006A508(void);
int32_t STD_func_8006A514(void);
void STD_renderNumber(int32_t a, int32_t digits, int32_t x, int32_t y, int16_t value, int32_t layer);
void STD_func_8006B1E4(int32_t i);
int32_t STD_isVersusModelSceneFinished(void);
void STD_func_8006B6E8(void);
void STD_func_80069468(void);
void STD_func_8006AD68(uint8_t id);
void STD_func_8006BA18(void);
void STD_func_8006B468(void);
void STD_func_8006AD00(int32_t i);
void STD_func_80069134(int16_t tech);
void GsSortBoxFill(GsBOXF *bp, GsOT *ot, unsigned short pri);
void STD_func_8006A824(int32_t id);
void STD_renderVersusModelScene(void);
void STD_func_8006BE64(void);
int32_t readFile(char *path, uint8_t *buffer);
void STD_func_8006B2BC(void);
void STD_tickVersusModelScene(void);
void STD_renderHPBarDigits(int16_t i, int32_t id);
void STD_renderFighterHPBar(int32_t id);
void damageTick(FighterData *fighter, Stats *stats);
void STD_func_8006B1F0(uint32_t *tmd, int32_t vofs, int32_t nofs, int32_t objIdx);

static void *std_hud_functions[] = {
	STD_func_8006BE64,
	STD_func_8006BA18,
	STD_func_8006B6F4,
	STD_func_8006B6E8,
	STD_isVersusModelSceneFinished,
	STD_renderVersusModelScene,
	STD_tickVersusModelScene,
	STD_func_8006B468,
	STD_func_8006B2BC,
	STD_func_8006B1F0,
	STD_func_8006B1E4,
	STD_func_8006AD68,
	STD_func_8006AD00,
	STD_renderHPBarDigits,
	STD_func_8006A824,
	STD_renderFighterHPBar,
	STD_renderNumber,
	STD_func_8006A514,
	STD_func_8006A508,
	STD_renderBattleStartTextBurst,
	STD_initializeBattleStartTextBurst,
	STD_func_8006A044,
	STD_renderBattleStartText,
	STD_initializeBattleStartText,
	STD_shuffleBattleStartTextPieces,
	STD_tickNPCTournament,
	STD_tickPartnerTournament,
	STD_tickTamerTournament,
	STD_func_80069468,
	STD_removeFinisherChargeup,
	STD_renderFinisherChargeup,
	STD_tickFinisherChargeup,
	STD_func_80069134,
};

// clang-format off
MATRIX STD_D_8007A718 = {
	{
		0x100a, 0x0000, 0x0000, 0x0000,
		0x08e4, 0xf299, 0x0000, 0x0d5e,
		0x08e9,
	},
	{ 0x00000000, 0xfffffffe, 0x000002d4 },
};

int16_t STD_D_8007A738[155][2] = {
	{ 0xff54, 0xffd0 },
	{ 0xff5c, 0xffd0 },
	{ 0xff64, 0xffd0 },
	{ 0xff6c, 0xffd0 },
	{ 0xff74, 0xffd0 },
	{ 0xff54, 0xffdc },
	{ 0xff5c, 0xffdc },
	{ 0xff74, 0xffdc },
	{ 0xff7c, 0xffdc },
	{ 0xff54, 0xffe8 },
	{ 0xff5c, 0xffe8 },
	{ 0xff74, 0xffe8 },
	{ 0xff7c, 0xffe8 },
	{ 0xff54, 0xfff4 },
	{ 0xff5c, 0xfff4 },
	{ 0xff64, 0xfff4 },
	{ 0xff6c, 0xfff4 },
	{ 0xff74, 0xfff4 },
	{ 0xff54, 0x0000 },
	{ 0xff5c, 0x0000 },
	{ 0xff74, 0x0000 },
	{ 0xff7c, 0x0000 },
	{ 0xff54, 0x000c },
	{ 0xff5c, 0x000c },
	{ 0xff74, 0x000c },
	{ 0xff7c, 0x000c },
	{ 0xff54, 0x0018 },
	{ 0xff5c, 0x0018 },
	{ 0xff74, 0x0018 },
	{ 0xff7c, 0x0018 },
	{ 0xff54, 0x0024 },
	{ 0xff5c, 0x0024 },
	{ 0xff64, 0x0024 },
	{ 0xff6c, 0x0024 },
	{ 0xff74, 0x0024 },
	{ 0xffa4, 0xffd0 },
	{ 0xffac, 0xffd0 },
	{ 0xffa4, 0xffdc },
	{ 0xffac, 0xffdc },
	{ 0xff9c, 0xffe8 },
	{ 0xffa4, 0xffe8 },
	{ 0xffac, 0xffe8 },
	{ 0xffb4, 0xffe8 },
	{ 0xff94, 0xfff4 },
	{ 0xff9c, 0xfff4 },
	{ 0xffb4, 0xfff4 },
	{ 0xffbc, 0xfff4 },
	{ 0xff94, 0x0000 },
	{ 0xff9c, 0x0000 },
	{ 0xffb4, 0x0000 },
	{ 0xffbc, 0x0000 },
	{ 0xff8c, 0x000c },
	{ 0xff94, 0x000c },
	{ 0xff9c, 0x000c },
	{ 0xffa4, 0x000c },
	{ 0xffac, 0x000c },
	{ 0xffb4, 0x000c },
	{ 0xffbc, 0x000c },
	{ 0xffc4, 0x000c },
	{ 0xff8c, 0x0018 },
	{ 0xff94, 0x0018 },
	{ 0xffbc, 0x0018 },
	{ 0xffc4, 0x0018 },
	{ 0xff8c, 0x0024 },
	{ 0xff94, 0x0024 },
	{ 0xffbc, 0x0024 },
	{ 0xffc4, 0x0024 },
	{ 0xffd4, 0xffd0 },
	{ 0xffdc, 0xffd0 },
	{ 0xffe4, 0xffd0 },
	{ 0xffec, 0xffd0 },
	{ 0xfff4, 0xffd0 },
	{ 0xfffc, 0xffd0 },
	{ 0xffe4, 0xffdc },
	{ 0xffec, 0xffdc },
	{ 0xffe4, 0xffe8 },
	{ 0xffec, 0xffe8 },
	{ 0xffe4, 0xfff4 },
	{ 0xffec, 0xfff4 },
	{ 0xffe4, 0x0000 },
	{ 0xffec, 0x0000 },
	{ 0xffe4, 0x000c },
	{ 0xffec, 0x000c },
	{ 0xffe4, 0x0018 },
	{ 0xffec, 0x0018 },
	{ 0xffe4, 0x0024 },
	{ 0xffec, 0x0024 },
	{ 0x0014, 0xffd0 },
	{ 0x001c, 0xffd0 },
	{ 0x0024, 0xffd0 },
	{ 0x002c, 0xffd0 },
	{ 0x0034, 0xffd0 },
	{ 0x003c, 0xffd0 },
	{ 0x0024, 0xffdc },
	{ 0x002c, 0xffdc },
	{ 0x0024, 0xffe8 },
	{ 0x002c, 0xffe8 },
	{ 0x0024, 0xfff4 },
	{ 0x002c, 0xfff4 },
	{ 0x0024, 0x0000 },
	{ 0x002c, 0x0000 },
	{ 0x0024, 0x000c },
	{ 0x002c, 0x000c },
	{ 0x0024, 0x0018 },
	{ 0x002c, 0x0018 },
	{ 0x0024, 0x0024 },
	{ 0x002c, 0x0024 },
	{ 0x004c, 0xffd0 },
	{ 0x0054, 0xffd0 },
	{ 0x004c, 0xffdc },
	{ 0x0054, 0xffdc },
	{ 0x004c, 0xffe8 },
	{ 0x0054, 0xffe8 },
	{ 0x004c, 0xfff4 },
	{ 0x0054, 0xfff4 },
	{ 0x004c, 0x0000 },
	{ 0x0054, 0x0000 },
	{ 0x004c, 0x000c },
	{ 0x0054, 0x000c },
	{ 0x004c, 0x0018 },
	{ 0x0054, 0x0018 },
	{ 0x004c, 0x0024 },
	{ 0x0054, 0x0024 },
	{ 0x005c, 0x0024 },
	{ 0x0064, 0x0024 },
	{ 0x006c, 0x0024 },
	{ 0x0074, 0x0024 },
	{ 0x0084, 0xffd0 },
	{ 0x008c, 0xffd0 },
	{ 0x0094, 0xffd0 },
	{ 0x009c, 0xffd0 },
	{ 0x00a4, 0xffd0 },
	{ 0x00ac, 0xffd0 },
	{ 0x0084, 0xffdc },
	{ 0x008c, 0xffdc },
	{ 0x0084, 0xffe8 },
	{ 0x008c, 0xffe8 },
	{ 0x0084, 0xfff4 },
	{ 0x008c, 0xfff4 },
	{ 0x0094, 0xfff4 },
	{ 0x009c, 0xfff4 },
	{ 0x00a4, 0xfff4 },
	{ 0x00ac, 0xfff4 },
	{ 0x0084, 0x0000 },
	{ 0x008c, 0x0000 },
	{ 0x0084, 0x000c },
	{ 0x008c, 0x000c },
	{ 0x0084, 0x0018 },
	{ 0x008c, 0x0018 },
	{ 0x0084, 0x0024 },
	{ 0x008c, 0x0024 },
	{ 0x0094, 0x0024 },
	{ 0x009c, 0x0024 },
	{ 0x00a4, 0x0024 },
	{ 0x00ac, 0x0024 },
};

int32_t STD_D_8007A9A4[12] = {
	0x00000020, 0x00000040, 0x00000060, 0x00000080,
	0x000000a0, 0x000000c0, 0x000000e0, 0x000000ff,
	0x000000e0, 0x000000c0, 0x000000a0, 0x00000080,
};

BarSprite STD_D_8007A9D4[6] = {
	{ 0x01ec, 0x80, 0xa8, 0x68, 0x08, 0x0000, 0x0000 },
	{ 0x01eb, 0x90, 0xb0, 0x0b, 0x0b, 0x0003, 0xfffe },
	{ 0x01eb, 0x80, 0xb0, 0x02, 0x02, 0x0012, 0x0003 },
	{ 0x01ec, 0x80, 0xa8, 0x68, 0x08, 0x0000, 0x0000 },
	{ 0x01eb, 0x9b, 0xb0, 0x0b, 0x0b, 0x0003, 0xfffe },
	{ 0x01eb, 0x84, 0xb0, 0x02, 0x02, 0x0012, 0x0003 },
};

int32_t STD_D_8007AA10[8] = {
	0x00001004, 0x00000000, 0x00001000, 0x00000000,
	0x00001004, 0x00000000, 0x00000000, 0x000003e8,
};

int16_t STD_D_8007AA30[8] = {
	0xfe90, 0xfdfb, 0xfd7b, 0xfd0e, 0xfcb6, 0xfc70, 0xfc21, 0xfc18,
};

int16_t STD_D_8007AA40[24] = {
	0xfd44, 0xfd4c, 0xfd64, 0xfd8c, 0xfdc4, 0xfe0c, 0xfe64, 0xfecc,
	0xff44, 0xffcc, 0x0064, 0x0043, 0x002c, 0x001f, 0x001c, 0x001f,
	0x002c, 0x0043, 0x0064, 0x0056, 0x0052, 0x0056, 0x0064, 0x0000,
};

int16_t STD_D_8007AA70[14] = {
	0x0005, 0x0014, 0x002f, 0x0053, 0x0082, 0x00bc, 0x0100, 0x014e,
	0x01a7, 0x020a, 0x0278, 0x02f0, 0x0372, 0x0400,
};

int16_t STD_D_8007AA8C[18] = {
	0x0000, 0x0001, 0x0005, 0x000b, 0x0014, 0x001f, 0x002d, 0x003d,
	0x0050, 0x0065, 0x007d, 0x0097, 0x00b4, 0x00d3, 0x00f5, 0x0119,
	0x0140, 0x0000,
};

char STD_D_8007AAB0[20] = "\\STDDAT\\DRAW.TMD";

char STD_D_8007AAC4[24] = "\\STDDAT\\WIN_LOSE.TMD";

char STD_D_8007AADC[20] = "\\STDDAT\\CHAMP.TMD";

char STD_D_8007AAF0[20] = "\\STDDAT\\B_DAI.TMD";

char STD_D_8007AB04[20] = "\\STDDAT\\T_DAI.TMD";
// clang-format on

void STD_func_80069134(int16_t tech)
{
	COMBAT_DATA_PTR->player.finisherChargeup[0] = 0;
	COMBAT_DATA_PTR->player.remainingChargeupTime[0] = 0x50;
	COMBAT_DATA_PTR->fighter[0].finisherProgress = 0;
	if (COMBAT_DATA_PTR->player.hoveredCommand[0] == 0) {
		COMBAT_DATA_PTR->player.hoveredCommand[0] = COMBAT_DATA_PTR->player.numCommands[0] - 1;
	}

	COMBAT_DATA_PTR->player.currentCommand[0] = 3;
	COMBAT_DATA_PTR->player.bufferedCommand[0] = 3;
	MAIN_D_8013518C[0] = -0x8c;
	MAIN_D_8013518E = -0x4a;
	addObject(0x19a, 0, (TickFunction)STD_tickFinisherChargeup, (RenderFunction)STD_renderFinisherChargeup);
}

void STD_tickFinisherChargeup(void)
{
	int32_t up;
	uint32_t charge;

	COMBAT_DATA_PTR->player.remainingChargeupTime[0]--;
	up = 0;
	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 4) != 0) {
		up = 1;
	}

	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 8) != 0) {
		up = 1;
	}

	charge = COMBAT_DATA_PTR->player.finisherChargeup[0];
	if ((charge < 0x50) && (up != 0)) {
		COMBAT_DATA_PTR->player.finisherChargeup[0] = charge + 2;
	}
}

void STD_renderFinisherChargeup(void)
{
	POLY_FT4 prim;
	int32_t i;
	int32_t x;
	int16_t bars;

	SetPolyFT4(&prim);
	prim.tpage = getTPage(0, 0, 960, 256);
	prim.clut = GetClut(0x110, 0x1f2);
	setRGB0(&prim, 0x80, 0x80, 0x80);
	setUVWH(&prim, 0x58, 0xe0, 46, 12);
	setXYWH(&prim, MAIN_D_8013518C[0], MAIN_D_8013518C[1], 0x2e, 0xc);
	GsSortPoly(&prim, ACTIVE_ORDERING_TABLE, 7);
	bars = COMBAT_DATA_PTR->player.finisherChargeup[0] / 8;
	setUVWH(&prim, 0x88, 0xe0, 4, 6);
	for (i = 0, x = 0; i < bars; i++, x += 4) {
		setXYWH(&prim, (int32_t)(MAIN_D_8013518C[0] + 3 + x), MAIN_D_8013518C[1] + 3, 4, 6);
		GsSortPoly(&prim, ACTIVE_ORDERING_TABLE, 7);
	}

	if (COMBAT_DATA_PTR->player.remainingChargeupTime[0] == 0) {
		STD_removeFinisherChargeup();
	}
}

void STD_removeFinisherChargeup(void)
{
	if (COMBAT_DATA_PTR->player.remainingChargeupTime[0] != -1) {
		removeObject(0x19a, 0);
		COMBAT_DATA_PTR->player.remainingChargeupTime[0] = -1;
	}
}

void STD_func_80069468(void)
{
	int32_t d;

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x8000) {
		playSound(0, 2);
		COMBAT_DATA_PTR->player.hoveredCommand[0]++;
		if (COMBAT_DATA_PTR->player.numCommands[0] - 1 < COMBAT_DATA_PTR->player.hoveredCommand[0]) {
			COMBAT_DATA_PTR->player.hoveredCommand[0] = 1;
		}
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x2000) {
		playSound(0, 2);
		COMBAT_DATA_PTR->player.hoveredCommand[0]--;
		if (COMBAT_DATA_PTR->player.hoveredCommand[0] <= 0) {
			COMBAT_DATA_PTR->player.hoveredCommand[0] = COMBAT_DATA_PTR->player.numCommands[0] - 1;
		}
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40) {
		playSound(0, 3);
		COMBAT_DATA_PTR->player.bufferedCommand[0] = COMBAT_DATA_PTR->player.availableCommands[0][COMBAT_DATA_PTR->player.hoveredCommand[0]];
		if ((d = PARTNER_PARA.discipline) < 0x46) {
			COMBAT_DATA_PTR->player.commandDelay[0] = 0xa0 - d / 10;
		} else {
			COMBAT_DATA_PTR->player.commandDelay[0] = (0xa - d / 10) * 10;
		}
		COMBAT_DATA_PTR->player.commandDelay[0] = 0;
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x80) {
		if (COMBAT_DATA_PTR->fighter[0].finisherProgress == COMBAT_DATA_PTR->fighter[0].finisherGoal) {
			COMBAT_DATA_PTR->player.bufferedCommand[0] = 0xb;
			playSound(0, 3);
			COMBAT_DATA_PTR->player.commandDelay[0] = 0;
			COMBAT_DATA_PTR->player.currentCommand[0] = 0xb;
		}
	}
}

void STD_tickTamerTournament(int32_t instanceId)
{
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x100) {
		MAIN_D_80134D64 = (MAIN_D_80134D64 + 1) & 1;
	}
	if (GAME_STATE == 4) {
		STD_func_80069468();
	}
}

void STD_tickPartnerTournament(int32_t i)
{
	tickAnimation(ENTITY_TABLE[i]);
}

void STD_tickNPCTournament(int32_t i)
{
	tickAnimation(ENTITY_TABLE[i]);
}

void STD_shuffleBattleStartTextPieces(void)
{
	int32_t i;

	for (i = 0; i < 0x9b; i++) {
		swapByte(&STD_D_8007BB94[i][0x11], &STD_D_8007BB94[random(0x9b)][0x11]);
	}
}

void STD_initializeBattleStartText(void)
{
	uint8_t (*p)[20];
	int32_t sgn;
	int32_t i;
	int32_t r;
	int32_t t;

	MAIN_D_80135190 = 0;
	MAIN_D_80135194 = 0;
	p = STD_D_8007BB94;
	for (i = 0; i < 0x9b; i++, p++) {
		(*p)[0x11] = i;
		(*p)[0x12] = 0x18;
		(*p)[0x13] = random(3);
	}

	STD_shuffleBattleStartTextPieces();

	p = STD_D_8007BB94;
	for (i = 0; i < 0x9b; i++, p++) {
		if (random(2) == 1) {
			sgn = 1;
		} else {
			sgn = -1;
		}
		((int16_t *)*p)[5] = STD_D_8007A738[i][1];
		(*p)[0x10] = -sgn * ((random(3) + 1) << 5);
		if ((0 <= i) && (i < 0x33)) {
			((int16_t *)*p)[4] = (sgn * 500) + random(100) - 50;
		} else if ((0x33 <= i) && (i < 0x65)) {
			((int16_t *)*p)[4] = (sgn * 600) + random(100) - 50;
		} else {
			((int16_t *)*p)[4] = (sgn * 700) + random(100) - 50;
		}
		r = random(5);
		t = STD_D_8007A738[i][0];
		((int16_t *)*p)[6] = (r + 8) * t / 8;
		t = STD_D_8007A738[i][1];
		((int16_t *)*p)[7] = (r + 8) * t / 8;
		((int16_t *)*p)[0] = 0;
		((int16_t *)*p)[1] = 0;
		((int16_t *)*p)[2] = 0;
	}

	addObject(0x1a6, 0, NULL, (RenderFunction)STD_renderBattleStartText);
}

void STD_renderBattleStartText(void)
{
	POLY_FT4 *ft;
	POLY_F4 *shadow;
	uint8_t (*p)[20];
	POLY_FT4 *prim;
	GsOT_TAG *ot;
	int32_t i;
	int32_t n;
	int32_t y;
	uint16_t clut;
	int32_t otz;
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
	SVECTOR q0;
	SVECTOR q1;
	SVECTOR q2;
	SVECTOR q3;

	GsSetProjection(0x200);
	GsSetLsMatrix(&STD_D_8007A718);

	n = 0;
	for (i = 0; i < 0x9b; i++) {
		if (STD_D_8007BB94[i][0x12] != 0) {
			break;
		}
		n++;
	}

	if (n == 0x9b) {
		y = MAIN_D_80135190++;
		clut = GetClut(0x100, y % 6 / 2 + 0x1e8);
		MAIN_D_80135194 = 1;
	} else {
		clut = GetClut(0x100, 0x1e8);
	}

	p = STD_D_8007BB94;
	prim = (POLY_FT4 *)GsGetWorkBase();
	ot = ACTIVE_ORDERING_TABLE->org;
	for (i = 0; i < 0x9b; i++, p++) {
		if (((int16_t *)*p)[4] != STD_D_8007A738[i][0]) {
			((int16_t *)*p)[4] += ((int8_t *)*p)[0x10];
			if (((int8_t *)*p)[0x10] > 0) {
				if (((int16_t *)*p)[4] > STD_D_8007A738[i][0]) {
					((int16_t *)*p)[4] = STD_D_8007A738[i][0];
				}
			} else {
				if (((int16_t *)*p)[4] < STD_D_8007A738[i][0]) {
					((int16_t *)*p)[4] = STD_D_8007A738[i][0];
				}
			}
		} else {
			if (((uint8_t *)*p)[0x12] != 0) {
				((uint8_t *)*p)[0x12] -= 4;
			}
		}

		p0.vx = ((int16_t *)*p)[4];
		p0.vy = ((int16_t *)*p)[5];
		p0.vz = 0;
		p1.vx = p0.vx + 8;
		p1.vy = p0.vy;
		p1.vz = 0;
		p2.vx = p0.vx;
		p2.vy = p0.vy + 0xc;
		p2.vz = 0;
		p3.vx = p0.vx + 8;
		p3.vy = p0.vy + 0xc;
		p3.vz = 0;

		ft = prim;
		setEntityTextDigit(prim, 0x100, 0x1e8);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->clut = clut;
		gte_ldv3(&p0, &p1, &p2);
		gte_rtpt();
		gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
		gte_stszotz(&otz);
		gte_ldv0(&p3);
		gte_rtps();
		gte_stsxy(&prim->x3);
		setUVWH(prim, ((uint8_t *)*p)[0x13] * 8 + 0x80, 0x80, 8, 8);
		AddPrim(ot + 5, prim++);

		if (n != 0x9b) {
			SetPolyFT4(prim);
			setRGB0(prim, 0x80, 0x80, 0x80);
			prim->tpage = GetTPage(0, 0, 0x380, 0x180);
			prim->clut = clut;
			SetSemiTrans(prim, 1);
			if (((int8_t *)*p)[0x10] > 0) {
				q0.vx = p0.vx - ((uint8_t *)*p)[0x12];
				q0.vy = p0.vy;
				q0.vz = p0.vz;
				q1 = p0;
				q2.vx = p2.vx - ((uint8_t *)*p)[0x12];
				q2.vy = p2.vy;
				q2.vz = p2.vz;
				q3 = p2;
				setUVWH(prim, 0x9e, 0x80, -24, 8);
			} else {
				q0 = p1;
				q1.vx = p1.vx + ((uint8_t *)*p)[0x12];
				q1.vy = p1.vy;
				q1.vz = p1.vz;
				q2 = p3;
				q3.vx = p3.vx + ((uint8_t *)*p)[0x12];
				q3.vy = p3.vy;
				q3.vz = p3.vz;
				setUVWH(prim, 0x86, 0x80, 24, 8);
			}
			gte_ldv3(&q0, &q1, &q2);
			gte_rtpt();
			gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
			gte_stszotz(&otz);
			gte_ldv0(&q3);
			gte_rtps();
			gte_stsxy(&prim->x3);
			AddPrim(ot + 5, prim++);
		}

		shadow = (POLY_F4 *)prim;
		SetPolyF4(shadow);
		setRGB0(shadow, 0, 0, 0);
		setXY4(shadow, ft->x0 + 2, ft->y0 + 2, ft->x1 + 2, ft->y1 + 2, ft->x2 + 2, ft->y2 + 2, ft->x3 + 2, ft->y3 + 2);
		AddPrim(ot + 6, shadow++);
		prim = (POLY_FT4 *)shadow;
	}

	GsSetWorkBase((PACKET *)prim);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_func_8006A044(void)
{
	removeObject(0x1a6, 0);
}

void STD_initializeBattleStartTextBurst(void)
{
	MAIN_D_80135194 = 0;
	addObject(0x1a6, 0, NULL, (RenderFunction)STD_renderBattleStartTextBurst);
}

void STD_renderBattleStartTextBurst(void)
{
	POLY_FT4 *ft;
	POLY_F4 *shadow;
	uint8_t (*p)[20];
	POLY_FT4 *prim;
	GsOT_TAG *ot;
	int32_t i;
	int32_t dead;
	int32_t j;
	int16_t cx;
	int16_t cy;
	int32_t otz;
	SVECTOR corner[4];
	SVECTOR pts[4];
	SVECTOR out;
	MATRIX m;

	GsSetProjection(0x200);
	GsSetLsMatrix(&STD_D_8007A718);

	corner[0].vx = -4;
	corner[0].vy = -6;
	corner[0].vz = 0;
	corner[1].vx = 4;
	corner[1].vy = -6;
	corner[1].vz = 0;
	corner[2].vx = -4;
	corner[2].vy = 6;
	corner[2].vz = 0;
	corner[3].vx = 4;
	corner[3].vy = 6;
	corner[3].vz = 0;

	ot = ACTIVE_ORDERING_TABLE->org;
	dead = 0;
	p = STD_D_8007BB94;
	prim = (POLY_FT4 *)GsGetWorkBase();
	for (i = 0; i < 0x9b; i++, p++) {
		if ((((int16_t *)*p)[6] == 0) && (((int16_t *)*p)[7] == 0)) {
			dead++;
			continue;
		}

		PushMatrix();
		RotMatrix((SVECTOR *)*p, &m);
		for (j = 0; j < 4; j++) {
			ApplyMatrixSV(&m, &corner[j], &out);
			pts[j].vx = out.vx + ((int16_t *)*p)[4] - 4;
			pts[j].vy = out.vy + ((int16_t *)*p)[5] - 6;
			pts[j].vz = out.vz;
		}

		switch (((uint8_t *)*p)[0x13]) {
		case 0:
			((int16_t *)*p)[0] += 0x100;
			((int16_t *)*p)[1] += 0x100;
			break;
		case 1:
			((int16_t *)*p)[1] += 0x100;
			((int16_t *)*p)[2] += 0x100;
			break;
		case 2:
			((int16_t *)*p)[0] += 0x100;
			((int16_t *)*p)[2] += 0x100;
			break;
		}

		PopMatrix();

		ft = prim;
		setEntityTextDigit(prim, 0x100, 0x1e8);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->clut = GetClut(0x100, 0x1e8);
		gte_ldv3(&pts[0], &pts[1], &pts[2]);
		gte_rtpt();
		gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
		gte_stszotz(&otz);
		gte_ldv0(&pts[3]);
		gte_rtps();
		gte_stsxy(&prim->x3);
		setUVWH(prim, ((uint8_t *)*p)[0x13] * 8 + 0x80, 0x80, 8, 8);
		AddPrim(ot + 5, prim++);
		shadow = (POLY_F4 *)prim;
		SetPolyF4(shadow);
		setRGB0(shadow, 0, 0, 0);
		setXY4(shadow, ft->x0 + 2, ft->y0 + 2, ft->x1 + 2, ft->y1 + 2, ft->x2 + 2, ft->y2 + 2, ft->x3 + 2, ft->y3 + 2);
		AddPrim(ot + 6, shadow++);
		prim = (POLY_FT4 *)shadow;

		cx = ft->x0;
		cy = ft->y0;
		if ((cx < -0xb4) || (cx >= 0xb5) || (cy < -0x8c) || (cy >= 0x8d)) {
			((int16_t *)*p)[6] = 0;
			((int16_t *)*p)[7] = 0;
		}
		((int16_t *)*p)[4] = ((int16_t *)*p)[4] + ((int16_t *)*p)[6];
		((int16_t *)*p)[5] = ((int16_t *)*p)[5] + ((int16_t *)*p)[7];
	}

	GsSetWorkBase((PACKET *)prim);
	if (dead == 0x9b) {
		MAIN_D_80135194 = 1;
	}
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_func_8006A508(void)
{
	removeObject(0x1a6, 0);
}

int32_t STD_func_8006A514(void)
{
	return MAIN_D_80135194;
}

void STD_renderNumber(int32_t a, int32_t digits, int32_t x, int32_t y, int16_t value, int32_t layer)
{
	POLY_FT4 *prim;
	int32_t i;
	uint32_t width;
	int32_t count;
	int32_t buf[4];

	prim = (POLY_FT4 *)GsGetWorkBase();

	width = digits;
	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		setEntityTextDigit(prim, 256, 492);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVDataPolyFT4(prim, buf[i] * 7, 172, 7, 11);
		setPosDataPolyFT4(prim, x + ((((int32_t)width - 1) - i) * 7), y, 7, 11);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void STD_renderFighterHPBar(int32_t id)
{
	int32_t n;
	int32_t i;
	uint8_t v;
	uint32_t k;
	uint32_t v2;

	if (GAME_STATE != 4) {
		return;
	}

	k = id;
	n = (COMBAT_DATA_PTR->fighter[id].finisherProgress * 6) / COMBAT_DATA_PTR->fighter[id].finisherGoal;

	if (MAIN_D_801351A0[k] != n) {
		MAIN_D_8013519C[k] = 0;
		MAIN_D_801351A0[k] = n;
	}

	MAIN_D_8013519E[k] = STD_D_8007A9A4[MAIN_D_8013519C[k]];
	if (MAIN_D_8013519C[k] < 0xb) {
		MAIN_D_8013519C[k]++;
	}

	if (n == 6) {
		if (MAIN_D_8013519A[k] < 0xa) {
			MAIN_D_8013519A[k]++;
		}
		v = MAIN_D_8013519A[k];
		v2 = v;
		if (v >= 3) {
			n++;
		}
		if (v2 >= 5) {
			n++;
			STD_func_8006A824(id);
			if (MAIN_D_8013519A[k] == 0xa) {
				MAIN_D_801351A2[k] = 1;
				MAIN_D_8013519C[k] = (uint32_t)MAIN_D_8013519C[k] % 0xb;
			}
		}
	}

	for (i = 0; i < n; i++) {
		STD_renderHPBarDigits((int16_t)i, id);
	}
}

void STD_func_8006A824(int32_t id)
{
	GsBOXF box;
	POLY_FT4 *prim;
	uint32_t n;

	prim = (POLY_FT4 *)GsGetWorkBase();
	setEntityTextDigit(prim, 256, 0x1e2);
	n = id;
	if ((MAIN_D_801351A2[id] != 1) || (((uint8_t *)COMBAT_DATA_PTR + n)[0x64e] == 0xb)) {
		setRGB0(prim, 0x80, 0x80, 0x80);
	} else {
		setRGB0(prim, MAIN_D_8013519E[n], MAIN_D_8013519E[n], MAIN_D_8013519E[n]);
	}

	setUVWH(prim, 0x80, 0x88, 37, 9);
	setXYWH(prim, (id == 0 ? -0x56 : 0x6e), -0x4b, 0x25, 9);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 8, prim++);
	GsSetWorkBase((PACKET *)prim);

	box.attribute = 0x40000000;
	if ((MAIN_D_801351A2[n] != 1) || (((uint8_t *)COMBAT_DATA_PTR + n)[0x64e] == 0xb)) {
		box.r = box.g = box.b = 0x80;
	} else {
		box.r = box.g = box.b = MAIN_D_8013519E[n];
	}

	setWH(&box, 0x29, 0xb);
	box.x = -0x58;
	box.y = -0x4c;
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 8);
}

void STD_renderHPBarDigits(int16_t i, int32_t id)
{
	POLY_FT4 *prim;
	int32_t n;
	uint32_t k;

	k = id;
	n = (COMBAT_DATA_PTR->fighter[id].finisherProgress * 6) / COMBAT_DATA_PTR->fighter[id].finisherGoal;
	prim = (POLY_FT4 *)GsGetWorkBase();
	setEntityTextDigit(prim, 0x100, 0x1ec);

	if ((((n - 1) == i) || (MAIN_D_801351A2[k] == 1)) && (((uint8_t *)COMBAT_DATA_PTR + k)[0x64e] != 0xb)) {
		setRGB0(prim, MAIN_D_8013519E[k], MAIN_D_8013519E[k], MAIN_D_8013519E[k]);
	} else {
		setRGB0(prim, 0x80, 0x80, 0x80);
	}

	setUVWH(prim, MAIN_D_801348C0[i], 0x9d, MAIN_D_801348C8[i], 15);
	setXYWH(prim, (id == 0 ? MAIN_D_801348D0[i] - 0x8d : MAIN_D_801348D0[i] + 0x37), -0x4f, MAIN_D_801348C8[i], 15);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 8, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void STD_func_8006AD00(int32_t id)
{
	uint32_t n;

	MAIN_D_80135198[id] = 0;
	n = id;
	MAIN_D_8013519A[n] = 0;
	MAIN_D_8013519C[n] = 0xb;
	MAIN_D_8013519E[n] = 0;
	MAIN_D_801351A0[n] = 0;
	MAIN_D_801351A2[n] = 0;
	addObject(0x19c, id, NULL, (RenderFunction)STD_func_8006AD68);
}

void STD_func_8006AD68(uint8_t id)
{
	POLY_FT4 *prim;
	BarSprite *p;
	FighterData *fighter;
	DigimonEntity *e;
	int16_t *hpPtr;
	int16_t *mpPtr;
	int32_t maxHp;
	int32_t maxMp;
	int32_t bar;
	int32_t k;
	int16_t x0;
	int16_t y0;
	int16_t fill;
	int16_t cur;
	int16_t nx;
	int16_t ny;
	int32_t w;
	int32_t t;

	p = (BarSprite *)COMBAT_DATA_PTR;
	fighter = &COMBAT_DATA_PTR->fighter[id];
	e = (DigimonEntity *)ENTITY_TABLE[((CombatData *)p)->player.entityIds[id]];
	hpPtr = &e->stats.current.currentHP;
	mpPtr = &((DigimonEntity *)ENTITY_TABLE[((CombatData *)p)->player.entityIds[id]])->stats.current.currentMP;
	maxHp = ((DigimonEntity *)ENTITY_TABLE[((CombatData *)p)->player.entityIds[id]])->stats.base.hp;
	maxMp = ((DigimonEntity *)ENTITY_TABLE[((CombatData *)p)->player.entityIds[id]])->stats.base.mp;

	if (*hpPtr == 0) {
		fighter->hpDamageBuffer = 0;
	}
	if (fighter->hpDamageBuffer != 0) {
		damageTick(fighter,
		           &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]])->stats);
	}
	*mpPtr = *mpPtr - fighter->mpDamageBuffer;
	fighter->mpDamageBuffer = 0;
	if (*mpPtr < 0) {
		*mpPtr = 0;
	}

	for (bar = 0; bar < 2; bar++) {
		p = &STD_D_8007A9D4[bar * 3 + 2];
		if (bar == 0) {
			if (id == 0) {
				x0 = -0x8c;
			} else {
				x0 = 0x24;
			}
			y0 = -0x64;
			cur = *hpPtr;
			w = cur;
			fill = w * 50 / maxHp;
		} else {
			if (id == 0) {
				x0 = -0x8c;
			} else {
				x0 = 0x24;
			}
			y0 = -0x58;
			cur = *mpPtr;
			w = cur;
			fill = w * 50 / maxMp;
		}
		nx = x0 + 0x49;
		ny = y0 - 3;
		STD_renderNumber(0, 4, nx, ny, cur, 8);
		prim = (POLY_FT4 *)GsGetWorkBase();
		for (k = 0; k < 3; k++, p--) {
			SetPolyFT4(prim);
			prim->clut = GetClut(0x100, p->clut);
			prim->tpage = getTPage(0, 0, 896, 256);
			setRGB0(prim, 0x80, 0x80, 0x80);
			setUVWH(prim, p->u, p->v, p->w, p->h);
			setXY0(prim, x0 + p->x, y0 + p->y);
			if (k == 0) {
				w = fill;
			} else {
				w = p->w;
			}
			t = x0 + p->x;
			prim->x1 = t + w;
			prim->y1 = y0 + p->y;
			prim->x2 = x0 + p->x;
			prim->y2 = p->h + (y0 + p->y);
			if (k == 0) {
				w = fill;
			} else {
				w = p->w;
			}
			t = x0 + p->x;
			prim->x3 = t + w;
			prim->y3 = p->h + (y0 + p->y);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 8, prim++);
		}
		GsSetWorkBase((PACKET *)prim);
	}
	if (id == 0) {
		STD_renderFighterHPBar(id);
	}
}

void STD_func_8006B1E4(int32_t i)
{
	removeObject(0x19c, i);
}

void STD_func_8006B1F0(uint32_t *tmd, int32_t vofs, int32_t nofs, int32_t objIdx)
{
	struct TMD_STRUCT *obj;
	uint32_t *p;
	uint32_t *hdr;
	uint8_t code;
	int32_t i;
	uint32_t len;
	int32_t n;
	uint32_t delta;
	int32_t off;

	delta = (nofs << 8) + vofs;
	off = objIdx * 28;
	hdr = (uint32_t *)((int32_t)tmd + 8);
	hdr++;
	obj = (struct TMD_STRUCT *)(off + (uint32_t)hdr);
	n = obj->primn;
	p = (uint32_t *)obj->primtop;
	for (i = 0; i < n; i++) {
		code = *p >> 24;
		if (!(code & 4)) {
			return;
		}
		len = ((*p & 0xff00) >> 8) + 1;
		p[1] += delta;
		p[2] += delta;
		p[3] += delta;
		if (code & 8) {
			p[4] += delta;
		}
		p += len;
	}
}

void STD_func_8006B2BC(void)
{
	int32_t i;

	MAIN_D_801351A4 = 0;
	MAIN_D_801351A8 = (uint8_t *)0x80038000;
	readFile(MAIN_D_801348D8, MAIN_D_801351A8);
	GsMapModelingData((u_long *)(MAIN_D_801351A8 + 4));

	for (i = 0; i < 4; i++) {
		GsLinkObject4((u_long)(MAIN_D_801351A8 + 0xc), &STD_D_8007C7B0[i].data.obj, i);
		GsInitCoordinate2(NULL, &STD_D_8007C7B0[i].data.posMatrix);
		STD_D_8007C7B0[i].data.obj.attribute = 0;
		STD_D_8007C7B0[i].data.obj.coord2 = &STD_D_8007C7B0[i].data.posMatrix;
	}

	for (i = 0; i < 4; i++) {
		STD_D_8007C7B0[i].data.scale.vx = 0x1000;
		STD_D_8007C7B0[i].data.scale.vy = 0x1000;
		STD_D_8007C7B0[i].data.scale.vz = 0x1000;
		STD_D_8007C7B0[i].data.rotation.vx = 0;
		STD_D_8007C7B0[i].data.rotation.vy = 0;
		STD_D_8007C7B0[i].data.rotation.vz = 0;
		STD_D_8007C7B0[i].data.location.vx = 0x3e8;
		STD_D_8007C7B0[i].data.location.vy = 0x78;
		STD_D_8007C7B0[i].data.location.vz = 0x280;
		setupModelMatrix(&STD_D_8007C7B0[i].data);
	}
}

void STD_func_8006B468(void)
{
	addObject(0x19d, 0, (TickFunction)STD_tickVersusModelScene, (RenderFunction)STD_renderVersusModelScene);
}

void STD_tickVersusModelScene(void)
{
	int32_t i;

	MAIN_D_801351A4++;
	for (i = 0; i < 4; i++) {
		if (i * 5 < MAIN_D_801351A4) {
			if (STD_D_8007C7B0[i].data.location.vx != MAIN_D_801348DC[i]) {
				STD_D_8007C7B0[i].data.location.vx -= 200;
				if (STD_D_8007C7B0[i].data.location.vx < MAIN_D_801348DC[i]) {
					STD_D_8007C7B0[i].data.location.vx = MAIN_D_801348DC[i];
				}
			}
		}
		setupModelMatrix(&STD_D_8007C7B0[i].data);
	}
}

void STD_renderVersusModelScene(void)
{
	MATRIX lw;
	MATRIX ls;
	int32_t i;

	GsSetProjection(0x200);
	GsWSMATRIX = *(MATRIX *)STD_D_8007AA10;
	GsClearOt(0, 4, &STD_D_8007B714[ACTIVE_FRAMEBUFFER]);

	for (i = 0; i < 4; i++) {
		GsGetLws(STD_D_8007C7B0[i].data.obj.coord2, &lw, &ls);
		GsSetLightMatrix(&lw);
		GsSetLsMatrix(&ls);
		GsSortObject4(&STD_D_8007C7B0[i].data.obj, &STD_D_8007B714[ACTIVE_FRAMEBUFFER], 9, getScratchAddr(0));
	}

	GsSortOt(&STD_D_8007B714[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

int32_t STD_isVersusModelSceneFinished(void)
{
	if (MAIN_D_801351A4 >= 0x51) {
		return 1;
	} else {
		return 0;
	}
}

void STD_func_8006B6E8(void)
{
	removeObject(0x19d, 0);
}

void STD_func_8006B6F4(void)
{
	int32_t i;
	int32_t v;
	uint32_t buf;

	for (i = 0x14; i < 0x29; i++) {
		STD_D_8007CC50[i] = ((i * 0x2000) / 40) & 0xfff;
		(&STD_D_8007CC78[2])[i] = (((i * 0x800) / 40) + 0x800) & 0xfff;
		if ((i >= 0x14) && (i < 0x25)) {
			(&STD_D_8007CCA4[2])[i] = STD_D_8007AA8C[0x24 - i] - 0xe6;
			v = STD_D_8007AA8C[0x24 - i];
			(&STD_D_8007CCD0[2])[i] = 0x118 - v;
		}
		if ((i >= 0x25) && (i < 0x29)) {
			(&STD_D_8007CCA4[2])[i] = (&STD_D_8007AA40[2])[i] - 0xe6;
			v = (&STD_D_8007AA40[2])[i];
			(&STD_D_8007CCD0[2])[i] = 0x118 - v;
		}
	}

	STD_D_8007B714[0].length = 5;
	STD_D_8007B714[0].org = STD_D_8007B73C;
	STD_D_8007B714[1].length = 5;
	STD_D_8007B714[1].org = STD_D_8007B7BC;
	buf = (uint32_t)STD_D_8007CD28;
	readFile(STD_D_8007AAC4, (void *)buf);
	GsMapModelingData((u_long *)(buf + 4));
	for (i = 0; i < 8; i++) {
		GsLinkObject4((u_long)(buf + 0xc), &STD_D_8007F528[i].data.obj, i);
		GsInitCoordinate2(NULL, &STD_D_8007F528[i].data.posMatrix);
		STD_D_8007F528[i].data.obj.attribute = 0;
		STD_D_8007F528[i].data.obj.coord2 = &STD_D_8007F528[i].data.posMatrix;
	}

	for (i = 4; i < 8; i++) {
		STD_D_8007F528[i].data.scale.vx = 0x1000;
		STD_D_8007F528[i].data.scale.vy = 0x1000;
		STD_D_8007F528[i].data.scale.vz = 0x1000;
		STD_D_8007F528[i].data.rotation.vx = 0;
		STD_D_8007F528[i].data.rotation.vy = 0;
		STD_D_8007F528[i].data.rotation.vz = 0;
		STD_D_8007F528[i].data.location.vx = 0;
		STD_D_8007F528[i].data.location.vy = -0x2bc;
		STD_D_8007F528[i].data.location.vz = 0xdc;
		setupModelMatrix(&STD_D_8007F528[i].data);
	}
}

void STD_func_8006BA18(void)
{
	int32_t i;
	char *buf;
	int32_t n;

	MAIN_D_801351AC = 0;
	n = 0;
	for (i = 1; i < 0x29; i++) {
		(STD_D_8007F968 - 1)[i] = n + (i * i * 5 << 9) / (n + 0x640);
	}

	for (i = 0; i < n + 0x27; i++) {
		STD_D_8007F9B8[i] = 0x1400 - STD_D_8007F968[0x26 - i];
	}

	buf = buf = STD_D_8007CD28;
	readFile(STD_D_8007AADC, (uint8_t *)buf);
	MAIN_D_801350F0 = 0;
	GsMapModelingData((u_long *)(buf + 4));

	for (i = 0; i < 9; i++) {
		STD_func_8006B1F0((uint32_t *)buf,
		                  ((8 - i) - (8 - i) * 4) * 8, 0x30, i);
	}

	for (i = 0; i < 9; i++) {
		GsLinkObject4((u_long)(buf + 0xc), &STD_D_8007C7B0[i].data.obj, i);
		GsInitCoordinate2(NULL, &STD_D_8007C7B0[i].data.posMatrix);
		STD_D_8007C7B0[i].data.obj.attribute = 0x80000000;
		STD_D_8007C7B0[i].data.obj.coord2 = &STD_D_8007C7B0[i].data.posMatrix;
	}

	for (i = 0; i < 9; i++) {
		STD_D_8007C7B0[i].data.scale.vx = 0x1000;
		STD_D_8007C7B0[i].data.scale.vy = 0x2000;
		STD_D_8007C7B0[i].data.scale.vz = 0x1000;
		STD_D_8007C7B0[i].data.rotation.vx = 0;
		STD_D_8007C7B0[i].data.rotation.vy = 0;
		STD_D_8007C7B0[i].data.rotation.vz = 0;
		switch (i) {
		case 5:
			STD_D_8007CAD0[0] = 0xbe;
			STD_D_8007CAD0[1] = 0xc8;
			STD_D_8007CAD0[2] = 0x7bc;
			break;
		case 6:
			STD_D_8007CB58[0] = 0x168;
			STD_D_8007CB58[1] = 0xc8;
			STD_D_8007CB58[2] = 0x7bc;
			break;
		case 7:
			STD_D_8007CBE0[0] = 0x230;
			STD_D_8007CBE0[1] = 0xc8;
			STD_D_8007CBE0[2] = 0x7bc;
			break;
		case 8:
			STD_D_8007CC68[0] = 0x2e4;
			STD_D_8007CC68[1] = 0xc8;
			STD_D_8007CC68[2] = 0x7bc;
			break;
		default:
			STD_D_8007C7B0[i].data.location.vx = i * 0xc8 - 0x2f8;
			STD_D_8007C7B0[i].data.location.vy = 0xc8;
			STD_D_8007C7B0[i].data.location.vz = 0x7bc;
			break;
		}
		setupModelMatrix(&STD_D_8007C7B0[i].data);
	}

	MAIN_D_801351B0 = (uint8_t *)0x80038000;
	if (CURRENT_SCREEN == 0x6a) {
		readFile(STD_D_8007AAF0, MAIN_D_801351B0);
	} else {
		readFile(STD_D_8007AB04, MAIN_D_801351B0);
	}
	GsMapModelingData((u_long *)(MAIN_D_801351B0 + 4));
	GsLinkObject4((u_long)(MAIN_D_801351B0 + 0xc), &STD_D_8007F528[0].data.obj, 0);
	GsInitCoordinate2(NULL, &STD_D_8007F528[0].data.posMatrix);
	STD_D_8007F528[0].data.obj.attribute = 0;
	STD_D_8007F528[0].data.obj.coord2 = &STD_D_8007F528[0].data.posMatrix;
	STD_D_8007F528[0].data.scale.vx = 0x1000;
	STD_D_8007F528[0].data.scale.vy = 0x1000;
	STD_D_8007F528[0].data.scale.vz = 0x1000;
	STD_D_8007F528[0].data.rotation.vx = 0;
	STD_D_8007F528[0].data.rotation.vy = 0;
	STD_D_8007F528[0].data.rotation.vz = 0;
	STD_D_8007F528[0].data.location.vx = 0;
	STD_D_8007F528[0].data.location.vy = -0x1e;
	STD_D_8007F528[0].data.location.vz = 0;
	setupModelMatrix(&STD_D_8007F528[0].data);
}

void STD_func_8006BE64(void)
{
	RECT rect;
	int32_t i;

	MAIN_D_801351B4 = 0;
	GAME_STATE = 5;
	startAnimation(ENTITY_TABLE[1], 0x21);
	ENTITY_TABLE[1]->posData->location.vx = 0;
	ENTITY_TABLE[1]->posData->location.vy = 0;
	ENTITY_TABLE[1]->posData->location.vz = 0;
	ENTITY_TABLE[1]->posData->location.vy = -0x1e;
	ENTITY_TABLE[1]->anim.locY = -0xf0000;
	ENTITY_TABLE[1]->anim.locX = 0;
	ENTITY_TABLE[1]->anim.locZ = 0;
	ENTITY_TABLE[1]->posData->rotation.vy = 0x200;
	STD_func_8005A830();
	rect.x = 0x378;
	rect.y = 0;
	rect.w = 6;
	MAIN_D_801351A4 = 0;
	rect.h = 0x30;

	for (i = 0; i < 9; i++) {
		STD_D_8007FA08[i] = 0;
		MoveImage(&rect, (i * 6) + 0x348, 0x30);
	}
}
