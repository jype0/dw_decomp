#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <mwinline_n.h>

#include <dw/anim.h>
#include <dw/battle.h>
#include <dw/combat.h>
#include <dw/math.h>
#include <dw/types.h>
#include <dw/vs.h>

#include "common.h"

extern int16_t MAIN_D_80135090[2];
extern uint8_t MAIN_D_801352C4;
extern int8_t GAME_STATE;
extern int16_t MAIN_D_801352E0;
extern uint8_t MAIN_D_801352E2;
extern int32_t MAIN_D_801352C8;
extern int16_t MAIN_D_801352AC[2];
extern int16_t MAIN_D_801352B0[2];
extern uint8_t MAIN_D_801352B4[2];
extern uint8_t MAIN_D_801352B6[2];
extern uint8_t MAIN_D_801352B8[2];
extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint8_t MAIN_D_801352CC[2];
extern uint8_t MAIN_D_801352CE[2];
extern uint8_t MAIN_D_801352D0[2];
extern uint8_t MAIN_D_801352D2[2];
extern uint8_t MAIN_D_801352D4[2];
extern uint8_t MAIN_D_801352D6[2];
extern uint8_t MAIN_D_801352D8[2];
extern uint8_t MAIN_D_801352DA[2];
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t VIEWPORT_DISTANCE;
extern uint8_t *MAIN_D_801352DC;
extern uint8_t VS_D_80070764[][10];
extern int16_t MAIN_D_801352BC[2];
extern int16_t MAIN_D_801352C0[2];
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern char MAIN_D_801A8B98[];

void VS_removeFinisherChargeup(void);
void VS_tickCommandMenu(uint8_t i);
int32_t readFile(char *path, uint8_t *buffer);
void GsSortBoxFill(GsBOXF *bp, GsOT *ot, unsigned short pri);
void VS_renderCommandMenu(uint8_t id);
void VS_renderTargetCursor(uint8_t id);
void VS_tickTargetCursor(uint8_t id);
void VS_removeTargetCursor(uint8_t index);
void VS_shuffleBattleStartTextPieces(void);
void VS_renderBattleStartText(void);
void VS_renderBattleStartTextBurst(void);
void VS_renderNumber(int32_t a, int32_t digits, int32_t x, int32_t y, int16_t value, int32_t layer);
void VS_renderFighterHPBar(int32_t id);
void VS_renderHPBarFill(int32_t id);
void VS_renderHPBarDigits(int16_t i, int32_t id);
void VS_tickFighterStatusBars(void);
void VS_renderFighterStatusBars(uint8_t id);
void VS_tickVersusModelScene(void);
void VS_renderVersusModelScene(void);
void VS_loadStageModels(void);
void VS_addResultModelScene(Entity *entity);
void VS_tickResultModelScene(int32_t won);
void VS_renderResultModelScene(void);
void VS_setVersusModelSceneTimer(int16_t value);
void setEntityTextDigit(POLY_FT4 *poly, int32_t x, int32_t y);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uPos, int32_t vPos, int32_t width, int32_t height);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount, int32_t *buf);
void swapByte(uint8_t *a, uint8_t *b);
void addObject(int32_t objectId, int32_t instanceId, void *tick, void *render);
void removeObject(int32_t objectId, int32_t instanceId);
int32_t VS_renderCommandMenu__garbage__(int32_t i);
void VS_renderMoveName(int32_t i);
void VS_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, uint8_t index);
void damageTick(FighterData *fighter, Stats *stats);
int32_t VS_addResultModelScene__garbage__(int32_t i);

static void *vs_hud_functions[] = {
	VS_setVersusModelSceneTimer,
	VS_removeResultModelScene,
	VS_renderResultModelScene,
	VS_tickResultModelScene,
	VS_addResultModelScene,
	VS_loadStageModels,
	VS_isVersusModelSceneFinished,
	VS_removeVersusModelScene,
	VS_renderVersusModelScene,
	VS_tickVersusModelScene,
	VS_addVersusModelScene,
	VS_loadVersusSceneModel,
	VS_removeFighterStatusBars,
	VS_renderFighterStatusBars,
	VS_tickFighterStatusBars,
	VS_addFighterStatusBars,
	VS_renderHPBarDigits,
	VS_renderHPBarFill,
	VS_renderFighterHPBar,
	VS_renderNumber,
	VS_isBattleStartTextFinished,
	VS_removeBattleStartTextBurst,
	VS_renderBattleStartTextBurst,
	VS_initializeBattleStartTextBurst,
	VS_removeBattleStartText,
	VS_renderBattleStartText,
	VS_initializeBattleStartText,
	VS_shuffleBattleStartTextPieces,
	VS_removeTargetCursor,
	VS_tickTargetCursor,
	VS_renderTargetCursor,
	VS_addTargetCursor,
	VS_removeCommandMenu,
	VS_renderCommandMenu,
	VS_tickCommandMenu,
	VS_addCommandMenu,
};

// clang-format off
MATRIX VS_D_800707D4 = {
	{
		0x100a, 0x0000, 0x0000, 0x0000,
		0x08e4, 0xf299, 0x0000, 0x0d5e,
		0x08e9,
	},
	{ 0x00000000, 0xfffffffe, 0x000002d4 },
};

int16_t VS_D_800707F4[155][2] = {
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

int32_t VS_D_80070A60[12] = {
	0x00000020, 0x00000040, 0x00000060, 0x00000080,
	0x000000a0, 0x000000c0, 0x000000e0, 0x000000ff,
	0x000000e0, 0x000000c0, 0x000000a0, 0x00000080,
};

BarSprite VS_D_80070A90[2] = {
	{ 0x01ec, 0x80, 0xa8, 0x68, 0x08, 0x0000, 0x0000 },
	{ 0x01eb, 0x90, 0xb0, 0x0b, 0x0b, 0x0003, 0xfffe },
};

int16_t VS_D_80070AA4[20] = {
	0x01eb, 0xb080, 0x0202, 0x0012, 0x0003, 0x01ec, 0xa880, 0x0868,
	0x0000, 0x0000, 0x01eb, 0xb09b, 0x0b0b, 0x0003, 0xfffe, 0x01eb,
	0xb084, 0x0202, 0x0012, 0x0003,
};

int32_t VS_D_80070ACC[8] = {
	0x00001004, 0x00000000, 0x00001000, 0x00000000,
	0x00001004, 0x00000000, 0x00000000, 0x000003e8,
};

int16_t VS_D_80070AEC[18] = {
	0x0000, 0x0001, 0x0005, 0x000b, 0x0014, 0x001f, 0x002d, 0x003d,
	0x0050, 0x0065, 0x007d, 0x0097, 0x00b4, 0x00d3, 0x00f5, 0x0119,
	0x0140, 0x0000,
};

uint8_t VS_D_80070B10[2][5] = {
	{ 0x00, 0x01, 0x04, 0x05, 0x06 },
	{ 0x02, 0x03, 0x04, 0x05, 0x06 },
};

char VS_D_80070B1C[20] = "\\STDDAT\\DRAW.TMD";

char VS_D_80070B30[20] = "\\STDDAT\\1P2PWIN.TMD";
// clang-format on

void VS_addCommandMenu(uint8_t index)
{
	MAIN_D_801352AC[index] = 0x44;
	MAIN_D_801352B0[index] = MAIN_D_801352AC[index] + 0x20;
	MAIN_D_801352B4[index] = 0;
	MAIN_D_801352B6[index] = 0;

	switch (COMBAT_DATA_PTR->player.numCommands[index]) {
	case 2:
		MAIN_D_801352B8[index] = 0;
		break;
	case 3:
		MAIN_D_801352B8[index] = 1;
		break;
	case 4:
		MAIN_D_801352B8[index] = 2;
		break;
	case 5:
		MAIN_D_801352B8[index] = 3;
		break;
	case 6:
		MAIN_D_801352B8[index] = 4;
		break;
	case 7:
		MAIN_D_801352B8[index] = 5;
		break;
	case 8:
		MAIN_D_801352B8[index] = 6;
		break;
	case 9:
		MAIN_D_801352B8[index] = 7;
		break;
	}

	MAIN_D_80134AC8[index] = 0;
	addObject(0x198, index, VS_tickCommandMenu, VS_renderCommandMenu);
}

void VS_tickCommandMenu(uint8_t i)
{
	MAIN_D_801352B6[i]++;
	if (GAME_STATE != 0) {
		if (GAME_STATE == 4) {
			if ((MAIN_D_801352B6[i] % 8) == 0) {
				MAIN_D_801352B4[i] = (MAIN_D_801352B4[i] + 1) & 1;
			}
		}
	}
}

int32_t VS_renderCommandMenu__garbage__(int32_t i)
{
	int32_t v0;
	int32_t v1;
	int32_t v2;
	int32_t v3;
	int32_t v4;
	int32_t v5;
	int32_t v6;
	int32_t v7;
	int32_t v8;
	int32_t v9;
	int32_t v10;
	int32_t v11;
	int32_t v12;
	int32_t v13;
	int32_t v14;
	int32_t v15;
	int32_t v16;
	int32_t v17;

	v0 = i + 0;
	v1 = i + 1;
	v2 = i + 2;
	v3 = i + 3;
	v4 = i + 4;
	v5 = i + 5;
	v6 = i + 6;
	v7 = i + 7;
	v8 = i + 8;
	v9 = i + 9;
	v10 = i + 10;
	v11 = i + 11;
	v12 = i + 12;
	v13 = i + 13;
	v14 = i + 14;
	v15 = i + 15;
	v16 = i + 16;
	v17 = i + 17;
	return v0 + v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v10 + v11 + v12 + v13 + v14 + v15 + v16 + v17;
}

void VS_renderCommandMenu(uint8_t id)
{
	POLY_FT4 *prim;
	int32_t i;
	int32_t off;
	int16_t base;
	int16_t count;
	int16_t x;
	int32_t y;
	int16_t rowY;
	int16_t width;

	base = (id * 0xa6) - 0x8c + ((COMBAT_DATA_PTR->player.numCommands[id] - 1) * 0xe);
	if (GAME_STATE == 4) {
		VS_renderMoveName(id);
	}

	prim = (POLY_FT4 *)GsGetWorkBase();
	count = COMBAT_DATA_PTR->player.numCommands[id] - 1;

	if (GAME_STATE == 4) {
		x = base - (COMBAT_DATA_PTR->player.hoveredCommand[id] * 0xe);
		SetPolyFT4(prim);
		prim->tpage = getTPage(0, 0, 960, 256);
		prim->clut = GetClut(0x11a, 0x1f1);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVDataPolyFT4(prim, 0x3d, 0xe0, 0x16, 0x16);
		if ((count % 2) == 0) {
			if ((COMBAT_DATA_PTR->player.hoveredCommand[id] % 2) == 0) {
				y = MAIN_D_801352AC[id];
			} else {
				y = MAIN_D_801352AC[id] + 0xa;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		} else {
			if ((COMBAT_DATA_PTR->player.hoveredCommand[id] % 2) == 1) {
				y = MAIN_D_801352AC[id];
			} else {
				y = MAIN_D_801352AC[id] + 0xa;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	for (i = 1, off = 0xe; i < COMBAT_DATA_PTR->player.numCommands[id]; i++, off += 0xe) {
		SetPolyFT4(prim);
		prim->tpage = getTPage(0, 0, 960, 256);
		prim->clut = GetClut(0x110, 0x1f0);
		setRGB0(prim, 0x80, 0x80, 0x80);
		VS_setCommandIconUV((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]], prim, COMBAT_DATA_PTR->player.availableCommands[id][i]);
		x = base - off;
		if ((count % 2) == 0) {
			setXY4(prim, x, ((i % 2) == 0) ? MAIN_D_801352AC[id] + 3 : MAIN_D_801352AC[id] + 0xd, x + 0x10, ((i % 2) == 0) ? MAIN_D_801352AC[id] + 3 : MAIN_D_801352AC[id] + 0xd, x, ((i % 2) == 0) ? MAIN_D_801352B0[id] - 0xd : MAIN_D_801352B0[id] - 3, x + 0x10, ((i % 2) == 0) ? MAIN_D_801352B0[id] - 0xd : MAIN_D_801352B0[id] - 3);
		} else {
			setXY4(prim, x, ((i % 2) == 1) ? MAIN_D_801352AC[id] + 3 : MAIN_D_801352AC[id] + 0xd, x + 0x10, ((i % 2) == 1) ? MAIN_D_801352AC[id] + 3 : MAIN_D_801352AC[id] + 0xd, x, ((i % 2) == 1) ? MAIN_D_801352B0[id] - 0xd : MAIN_D_801352B0[id] - 3, x + 0x10, ((i % 2) == 1) ? MAIN_D_801352B0[id] - 0xd : MAIN_D_801352B0[id] - 3);
		}
		if ((i == COMBAT_DATA_PTR->player.hoveredCommand[id]) && (MAIN_D_801352B4[id] == 1)) {
			prim->u0 += 0x10;
			prim->u1 += 0x10;
			prim->u2 += 0x10;
			prim->u3 += 0x10;
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	for (i = 0; i < COMBAT_DATA_PTR->player.numCommands[id]; i++) {
		SetPolyFT4(prim);
		setSemiTrans(prim, 1);
		prim->tpage = GetTPage(0, 0, 0x3c0, 0x100);
		prim->clut = GetClut(0x110, 0x1f1);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVWH(prim, MAIN_D_80134AE8[VS_D_80070764[MAIN_D_801352B8[id]][i]], 0xe0, MAIN_D_80134AF0[VS_D_80070764[MAIN_D_801352B8[id]][i]], 31);
		if (i > 0) {
			rowY = ((i - 1) * 0xe) + 0xb;
		} else {
			rowY = 0;
		}
		if ((i == 0) || (i == (COMBAT_DATA_PTR->player.numCommands[id] - 1))) {
			width = 0xb;
		} else {
			width = 0xe;
		}
		setXY4(prim, (rowY - 0x8f) + id * 0xa6, MAIN_D_801352AC[id], ((rowY - 0x8f) + width) + id * 0xa6, MAIN_D_801352AC[id], (rowY - 0x8f) + id * 0xa6, MAIN_D_801352B0[id], ((rowY - 0x8f) + width) + id * 0xa6, MAIN_D_801352B0[id]);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void VS_removeCommandMenu(int32_t i)
{
	MAIN_D_80134AC8[i] = 0;
	removeObject(0x198, i);
}

// clang-format off
void VS_addTargetCursor(id, tech)
	int16_t id;
	int32_t tech;
// clang-format on
{
	COMBAT_DATA_PTR->player.finisherChargeup[id] = 0;
	COMBAT_DATA_PTR->player.remainingChargeupTime[id] = 0x50;
	COMBAT_DATA_PTR->fighter[id].finisherProgress = 0;
	if (COMBAT_DATA_PTR->player.hoveredCommand[id] == 0) {
		COMBAT_DATA_PTR->player.hoveredCommand[id] = COMBAT_DATA_PTR->player.numCommands[id] - 1;
	}
	COMBAT_DATA_PTR->player.currentCommand[id] = 3;
	COMBAT_DATA_PTR->player.bufferedCommand[id] = 3;
	MAIN_D_801352BC[id] = id * 0xea - 0x8c;
	MAIN_D_801352C0[id] = -0x4a;
	addObject(0x19a, id, VS_tickTargetCursor, VS_renderTargetCursor);
}

void VS_renderTargetCursor(uint8_t id)
{
	POLY_FT4 prim;
	int32_t i;
	int32_t x;
	int16_t bars;
	uint32_t n;
	int32_t px;

	SetPolyFT4(&prim);
	prim.tpage = getTPage(0, 0, 960, 256);
	prim.clut = GetClut(0x110, 0x1f2);
	setRGB0(&prim, 0x80, 0x80, 0x80);
	setUVWH(&prim, 0x58, 0xe0, 46, 12);
	setXYWH(&prim, MAIN_D_801352BC[id], MAIN_D_801352C0[id], 0x2e, 0xc);
	n = id;
	GsSortPoly(&prim, ACTIVE_ORDERING_TABLE, 7);
	bars = COMBAT_DATA_PTR->player.finisherChargeup[n] / 8;
	setUVWH(&prim, 0x88, 0xe0, 4, 6);
	for (i = 0, x = 0; i < bars; i++, x += 4) {
		px = MAIN_D_801352BC[n] + 3;
		setXYWH(&prim, px + x, MAIN_D_801352C0[n] + 3, 4, 6);
		GsSortPoly(&prim, ACTIVE_ORDERING_TABLE, 7);
	}

	if (COMBAT_DATA_PTR->player.remainingChargeupTime[n] == 0) {
		VS_removeTargetCursor(id);
	}
}

void VS_tickTargetCursor(uint8_t id)
{
	uint32_t input;
	uint32_t prev;
	int32_t up;
	uint32_t charge;

	input = POLLED_INPUT;
	prev = POLLED_INPUT_PREVIOUS;
	if (id == 1) {
		POLLED_INPUT = (uint16_t)(POLLED_INPUT >> 16);
		POLLED_INPUT_PREVIOUS = (uint16_t)(POLLED_INPUT_PREVIOUS >> 16);
	}
	COMBAT_DATA_PTR->player.remainingChargeupTime[id]--;
	up = 0;
	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 4) != 0) {
		up = 1;
	}
	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 8) != 0) {
		up = 1;
	}
	charge = COMBAT_DATA_PTR->player.finisherChargeup[id];
	if ((charge < 0x50) && (up != 0)) {
		COMBAT_DATA_PTR->player.finisherChargeup[id] = charge + 2;
	}
	POLLED_INPUT = input;
	POLLED_INPUT_PREVIOUS = prev;
}

void VS_removeTargetCursor(uint8_t index)
{
	if (((int8_t *)((uint32_t)index + (uint32_t)COMBAT_DATA_PTR))[0x66a] != -1) {
		removeObject(0x19a, index);
		((int8_t *)((uint32_t)index + (uint32_t)COMBAT_DATA_PTR))[0x66a] = -1;
	}
}

void VS_shuffleBattleStartTextPieces(void)
{
	int32_t i;

	for (i = 0; i < 0x9b; i++) {
		swapByte(&VS_D_80071B38[i][0x11], &VS_D_80071B38[random(0x9b)][0x11]);
	}
}

void VS_initializeBattleStartText(void)
{
	uint8_t (*p)[20];
	int32_t sgn;
	int32_t i;
	int32_t r;
	int32_t t;

	MAIN_D_801352C4 = 0;
	MAIN_D_801352C8 = 0;
	p = VS_D_80071B38;
	for (i = 0; i < 0x9b; i++, p++) {
		(*p)[0x11] = i;
		(*p)[0x12] = 0x18;
		(*p)[0x13] = random(3);
	}

	VS_shuffleBattleStartTextPieces();

	p = VS_D_80071B38;
	for (i = 0; i < 0x9b; i++, p++) {
		if (random(2) == 1) {
			sgn = 1;
		} else {
			sgn = -1;
		}
		((int16_t *)*p)[5] = VS_D_800707F4[i][1];
		(*p)[0x10] = -sgn * ((random(3) + 1) << 5);
		if ((0 <= i) && (i < 0x33)) {
			((int16_t *)*p)[4] = (sgn * 500) + random(100) - 50;
		} else if ((0x33 <= i) && (i < 0x65)) {
			((int16_t *)*p)[4] = (sgn * 600) + random(100) - 50;
		} else {
			((int16_t *)*p)[4] = (sgn * 700) + random(100) - 50;
		}
		r = random(5);
		t = VS_D_800707F4[i][0];
		((int16_t *)*p)[6] = (r + 8) * t / 8;
		t = VS_D_800707F4[i][1];
		((int16_t *)*p)[7] = (r + 8) * t / 8;
		((int16_t *)*p)[0] = 0;
		((int16_t *)*p)[1] = 0;
		((int16_t *)*p)[2] = 0;
	}

	addObject(0x1a6, 0, NULL, VS_renderBattleStartText);
}

void VS_renderBattleStartText(void)
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
	MATRIX m;

	m = GsWSMATRIX;
	GsSetProjection(0x200);
	GsSetLsMatrix(&VS_D_800707D4);

	n = 0;
	for (i = 0; i < 0x9b; i++) {
		if (VS_D_80071B38[i][0x12] != 0) {
			break;
		}
		n++;
	}

	if (n == 0x9b) {
		y = MAIN_D_801352C4++;
		clut = GetClut(0x100, y % 6 / 2 + 0x1e8);
		MAIN_D_801352C8 = 1;
	} else {
		clut = GetClut(0x100, 0x1e8);
	}

	p = VS_D_80071B38;
	prim = (POLY_FT4 *)GsGetWorkBase();
	ot = ACTIVE_ORDERING_TABLE->org;
	for (i = 0; i < 0x9b; i++, p++) {
		if (((int16_t *)*p)[4] != VS_D_800707F4[i][0]) {
			((int16_t *)*p)[4] += ((int8_t *)*p)[0x10];
			if (((int8_t *)*p)[0x10] > 0) {
				if (((int16_t *)*p)[4] > VS_D_800707F4[i][0]) {
					((int16_t *)*p)[4] = VS_D_800707F4[i][0];
				}
			} else {
				if (((int16_t *)*p)[4] < VS_D_800707F4[i][0]) {
					((int16_t *)*p)[4] = VS_D_800707F4[i][0];
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
	GsWSMATRIX = m;
}

void VS_removeBattleStartText(void)
{
	removeObject(0x1a6, 0);
}

void VS_initializeBattleStartTextBurst(void)
{
	MAIN_D_801352C8 = 0;
	addObject(0x1a6, 0, NULL, VS_renderBattleStartTextBurst);
}

void VS_renderBattleStartTextBurst(void)
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
	MATRIX saved;

	saved = GsWSMATRIX;
	GsSetProjection(0x200);
	GsSetLsMatrix(&VS_D_800707D4);

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
	p = VS_D_80071B38;
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
		MAIN_D_801352C8 = 1;
	}
	GsSetProjection(VIEWPORT_DISTANCE);
	GsWSMATRIX = saved;
}

void VS_removeBattleStartTextBurst(void)
{
	removeObject(0x1a6, 0);
}

int32_t VS_isBattleStartTextFinished(void)
{
	return MAIN_D_801352C8;
}

void VS_renderNumber(int32_t a, int32_t digits, int32_t x, int32_t y, int16_t value, int32_t layer)
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

void VS_renderFighterHPBar(int32_t id)
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
	n = (COMBAT_DATA_PTR->fighter[id].finisherProgress * 6) /
	    COMBAT_DATA_PTR->fighter[id].finisherGoal;

	if (MAIN_D_801352D4[k] != n) {
		MAIN_D_801352D0[k] = 0;
		MAIN_D_801352D4[k] = n;
	}

	MAIN_D_801352D2[k] = VS_D_80070A60[MAIN_D_801352D0[k]];
	if (MAIN_D_801352D0[k] < 0xb) {
		MAIN_D_801352D0[k]++;
	}

	if (n == 6) {
		if (MAIN_D_801352CE[k] < 0xa) {
			MAIN_D_801352CE[k]++;
		}
		v = MAIN_D_801352CE[k];
		v2 = v;
		if (v >= 3) {
			n++;
		}
		if (v2 >= 5) {
			n++;
			VS_renderHPBarFill(id);
			if (MAIN_D_801352CE[k] == 0xa) {
				MAIN_D_801352D6[k] = 1;
				MAIN_D_801352D0[k] = (uint32_t)MAIN_D_801352D0[k] % 0xb;
			}
		}
	}

	for (i = 0; i < n; i++) {
		VS_renderHPBarDigits((int16_t)i, id);
	}
}

void VS_renderHPBarFill(int32_t id)
{
	GsBOXF box;
	POLY_FT4 *prim;
	uint32_t n;

	prim = (POLY_FT4 *)GsGetWorkBase();
	setEntityTextDigit(prim, 256, 0x1e2);
	n = id;
	if ((MAIN_D_801352D6[id] != 1) || (((uint8_t *)COMBAT_DATA_PTR + n)[0x64e] == 0xb)) {
		setRGB0(prim, 0x80, 0x80, 0x80);
	} else {
		setRGB0(prim, MAIN_D_801352D2[n], MAIN_D_801352D2[n], MAIN_D_801352D2[n]);
	}

	setUVWH(prim, 0x80, 0x88, 37, 9);
	setXYWH(prim, (id == 0 ? -0x56 : 0x6e), -0x4b, 0x25, 9);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 8, prim++);
	GsSetWorkBase((PACKET *)prim);

	box.attribute = 0x40000000;
	if ((MAIN_D_801352D6[n] != 1) || (((uint8_t *)COMBAT_DATA_PTR + n)[0x64e] == 0xb)) {
		box.r = box.g = box.b = 0x80;
	} else {
		box.r = box.g = box.b = MAIN_D_801352D2[n];
	}

	setWH(&box, 0x29, 0xb);
	box.x = (id == 0 ? -0x58 : 0x6c);
	box.y = -0x4c;
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 8);
}

void VS_renderHPBarDigits(int16_t i, int32_t id)
{
	POLY_FT4 *prim;
	int32_t n;
	uint32_t k;

	k = id;
	n = (COMBAT_DATA_PTR->fighter[id].finisherProgress * 6) /
	    COMBAT_DATA_PTR->fighter[id].finisherGoal;
	prim = (POLY_FT4 *)GsGetWorkBase();
	setEntityTextDigit(prim, 0x100, 0x1ec);

	if ((((n - 1) == i) || (MAIN_D_801352D6[k] == 1)) &&
	    (((uint8_t *)COMBAT_DATA_PTR + k)[0x64e] != 0xb)) {
		setRGB0(prim, MAIN_D_801352D2[k], MAIN_D_801352D2[k], MAIN_D_801352D2[k]);
	} else {
		setRGB0(prim, 0x80, 0x80, 0x80);
	}

	setUVWH(prim, MAIN_D_80134AF8[i], 0x9d, MAIN_D_80134B00[i], 15);
	setXYWH(prim, (id == 0 ? MAIN_D_80134B08[i] - 0x8d : MAIN_D_80134B08[i] + 0x37), -0x4f, MAIN_D_80134B00[i], 15);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 8, prim++);
	GsSetWorkBase((PACKET *)prim);
}

// clang-format off
void VS_addFighterStatusBars(id)
	int16_t id;
// clang-format on
{
	MAIN_D_801352CC[id] = 0;
	MAIN_D_801352CE[id] = 0;
	MAIN_D_801352D0[id] = 11;
	MAIN_D_801352D2[id] = 0;
	MAIN_D_801352D4[id] = 0;
	MAIN_D_801352D6[id] = 0;
	MAIN_D_801352DA[id] = MAIN_D_801352D8[id] = 0;
	addObject(0x19c, id, VS_tickFighterStatusBars, VS_renderFighterStatusBars);
}

void VS_tickFighterStatusBars(void)
{
}

void VS_renderFighterStatusBars(uint8_t id)
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
		p = &VS_D_80070A90[bar * 3 + 2];
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
		VS_renderNumber(0, 4, nx, ny, cur, 8);
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
	VS_renderFighterHPBar(id);
}

void VS_removeFighterStatusBars(int32_t i)
{
	removeObject(0x19c, i);
}

void VS_loadVersusSceneModel(void)
{
	int32_t i;

	MAIN_D_801352DC = (uint8_t *)0x80038000;
	readFile(MAIN_D_80134B10, MAIN_D_801352DC);
	GsMapModelingData((u_long *)(MAIN_D_801352DC + 4));

	for (i = 0; i < 4; i++) {
		GsLinkObject4((u_long)(MAIN_D_801352DC + 0xc), &VS_D_80072754[i].data.obj, i);
		GsInitCoordinate2(NULL, &VS_D_80072754[i].data.posMatrix);
		VS_D_80072754[i].data.obj.attribute = 0;
		VS_D_80072754[i].data.obj.coord2 = &VS_D_80072754[i].data.posMatrix;
	}

	for (i = 0; i < 4; i++) {
		VS_D_80072754[i].data.scale.vx = 0x1000;
		VS_D_80072754[i].data.scale.vy = 0x1000;
		VS_D_80072754[i].data.scale.vz = 0x1000;
		VS_D_80072754[i].data.rotation.vx = 0;
		VS_D_80072754[i].data.rotation.vy = 0;
		VS_D_80072754[i].data.rotation.vz = 0;
		VS_D_80072754[i].data.location.vx = 0x3e8;
		VS_D_80072754[i].data.location.vy = 0x78;
		VS_D_80072754[i].data.location.vz = 0x280;
		setupModelMatrix(&VS_D_80072754[i].data);
	}
}

void VS_addVersusModelScene(void)
{
	MAIN_D_801352E0 = 0;
	addObject(0x19d, 0, VS_tickVersusModelScene, VS_renderVersusModelScene);
}

void VS_tickVersusModelScene(void)
{
	int32_t i;

	MAIN_D_801352E0++;
	for (i = 0; i < 4; i++) {
		if (i * 5 < MAIN_D_801352E0) {
			if (VS_D_80072754[i].data.location.vx != MAIN_D_80134B14[i]) {
				VS_D_80072754[i].data.location.vx -= 200;
				if (VS_D_80072754[i].data.location.vx < MAIN_D_80134B14[i]) {
					VS_D_80072754[i].data.location.vx = MAIN_D_80134B14[i];
				}
			}
		}
		setupModelMatrix(&VS_D_80072754[i].data);
	}
}

void VS_renderVersusModelScene(void)
{
	MATRIX lw;
	MATRIX ls;
	int32_t i;

	GsSetProjection(0x200);
	GsWSMATRIX = *(MATRIX *)VS_D_80070ACC;
	GsClearOt(0, 4, &VS_D_80071764[ACTIVE_FRAMEBUFFER]);

	for (i = 0; i < 4; i++) {
		GsGetLws(VS_D_80072754[i].data.obj.coord2, &lw, &ls);
		GsSetLightMatrix(&lw);
		GsSetLsMatrix(&ls);
		GsSortObject4(&VS_D_80072754[i].data.obj, &VS_D_80071764[ACTIVE_FRAMEBUFFER], 9,
		              getScratchAddr(0));
	}

	GsSortOt(&VS_D_80071764[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void VS_removeVersusModelScene(void)
{
	removeObject(0x19d, 0);
}

int32_t VS_isVersusModelSceneFinished(void)
{
	if (MAIN_D_801352E0 >= 0x51) {
		return 1;
	} else {
		return 0;
	}
}

void VS_loadStageModels(void)
{
	int32_t i;
	int32_t v;
	uint32_t buf;

	for (i = 0x14; i < 0x29; i++) {
		(&VS_D_80072974[-0x14])[i] = ((i * 0x2000) / 40) & 0xfff;
		(&VS_D_800729A0[-0x14])[i] = (((i * 0x800) / 40) + 0x800) & 0xfff;
		if ((i >= 0x14) && (i < 0x25)) {
			(&VS_D_800729CC[-0x14])[i] = VS_D_80070AEC[0x24 - i] - 0x134;
			v = VS_D_80070AEC[0x24 - i];
			(&VS_D_800729F8[-0x14])[i] = v - 0x140;
			(&VS_D_80072A24[-0x14])[i] = 0x111 - v;
		}
		if ((i >= 0x25) && (i < 0x29)) {
			(&VS_D_800729CC[-0x14])[i] = VS_D_80070AA4[i] - 0x134;
			v = VS_D_80070AA4[i];
			(&VS_D_800729F8[-0x14])[i] = v - 0x140;
			(&VS_D_80072A24[-0x14])[i] = 0x111 - v;
		}
	}

	VS_D_80071764[0].length = 5;
	VS_D_80071764[0].org = VS_D_8007178C;
	VS_D_80071764[1].length = 5;
	VS_D_80071764[1].org = VS_D_8007180C;
	buf = (uint32_t)MAIN_D_801A8B98;
	readFile(VS_D_80070B30, (void *)buf);
	GsMapModelingData((u_long *)(buf + 4));
	for (i = 0; i < 7; i++) {
		GsLinkObject4((u_long)(buf + 0xc), &VS_D_80072A50[i].data.obj, i);
		GsInitCoordinate2(NULL, &VS_D_80072A50[i].data.posMatrix);
		VS_D_80072A50[i].data.obj.attribute = 0;
		VS_D_80072A50[i].data.obj.coord2 = &VS_D_80072A50[i].data.posMatrix;
	}
}

int32_t VS_addResultModelScene__garbage__(int32_t i)
{
	int32_t v0;
	int32_t v1;
	int32_t v2;
	int32_t v3;
	int32_t v4;
	int32_t v5;
	int32_t v6;
	int32_t v7;
	int32_t v8;
	int32_t v9;
	int32_t v10;
	int32_t v11;
	int32_t v12;
	int32_t v13;
	int32_t v14;
	int32_t v15;
	int32_t v16;
	int32_t v17;
	int32_t v18;
	int32_t v19;
	int32_t v20;
	int32_t v21;
	int32_t v22;
	int32_t v23;

	v0 = i + 0;
	v1 = i + 1;
	v2 = i + 2;
	v3 = i + 3;
	v4 = i + 4;
	v5 = i + 5;
	v6 = i + 6;
	v7 = i + 7;
	v8 = i + 8;
	v9 = i + 9;
	v10 = i + 10;
	v11 = i + 11;
	v12 = i + 12;
	v13 = i + 13;
	v14 = i + 14;
	v15 = i + 15;
	v16 = i + 16;
	v17 = i + 17;
	v18 = i + 18;
	v19 = i + 19;
	v20 = i + 20;
	v21 = i + 21;
	v22 = i + 22;
	v23 = i + 23;
	return v0 + v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v10 + v11 + v12 + v13 + v14 + v15 + v16 + v17 + v18 + v19 + v20 + v21 + v22 + v23;
}

void VS_addResultModelScene(Entity *entity)
{
	int32_t i;
	int32_t k;

	MAIN_D_801352E0 = 0;
	if (entity == ENTITY_TABLE[1]) {
		MAIN_D_801352E2 = 0;
	} else {
		MAIN_D_801352E2 = 1;
	}
	VS_D_80072A50[MAIN_D_801352E2 * 2].data.location.vx = -0x148;
	VS_D_80072A50[MAIN_D_801352E2 * 2].data.location.vy = 0;
	VS_D_80072A50[MAIN_D_801352E2 * 2].data.location.vz = -0x4d0;
	VS_D_80072A50[MAIN_D_801352E2 * 2 + 1].data.location.vx = -0x9b;
	VS_D_80072A50[MAIN_D_801352E2 * 2 + 1].data.location.vy = 0;
	VS_D_80072A50[MAIN_D_801352E2 * 2 + 1].data.location.vz = -0x4d0;
	VS_D_80072A50[MAIN_D_801352E2 * 2].data.location.vx = -0x154;
	VS_D_80072A50[MAIN_D_801352E2 * 2].data.location.vy = 0;
	VS_D_80072A50[MAIN_D_801352E2 * 2].data.location.vz = -0x4d0;
	VS_D_80072A50[MAIN_D_801352E2 * 2 + 1].data.location.vx = -0x9b;
	VS_D_80072A50[MAIN_D_801352E2 * 2 + 1].data.location.vy = 0;
	VS_D_80072A50[MAIN_D_801352E2 * 2 + 1].data.location.vz = -0x4d0;
	VS_D_80072A50[4].data.location.vx = 0;
	VS_D_80072A50[4].data.location.vy = 0;
	VS_D_80072A50[4].data.location.vz = -0x4d0;
	VS_D_80072A50[5].data.location.vx = 0x84;
	VS_D_80072A50[5].data.location.vy = 0;
	VS_D_80072A50[5].data.location.vz = -0x4d0;
	VS_D_80072A50[6].data.location.vx = 0xfd;
	VS_D_80072A50[6].data.location.vy = 0;
	VS_D_80072A50[6].data.location.vz = -0x4d0;
	for (i = 0; i < 5; i++) {
		VS_D_80072E08[i] = 0;
		k = VS_D_80070B10[MAIN_D_801352E2][i];
		VS_D_80072A50[k].data.scale.vx = 0x1000;
		VS_D_80072A50[k].data.scale.vy = 0x1000;
		VS_D_80072A50[k].data.scale.vz = 0x1000;
		VS_D_80072A50[k].data.rotation.vx = 0;
		VS_D_80072A50[k].data.rotation.vy = 0;
		VS_D_80072A50[k].data.rotation.vz = 0;
		setupModelMatrix(&VS_D_80072A50[k].data);
	}
	addObject(0x19d, MAIN_D_801352E2, VS_tickResultModelScene, VS_renderResultModelScene);
}

void VS_tickResultModelScene(int32_t won)
{
	int32_t i;
	int32_t t;
	int32_t k;

	if (MAIN_D_801352E0 < 0xa0) {
		MAIN_D_801352E0++;
	}
	for (i = 0, t = 0x3c; i < 5; i++, t += 6) {
		if (t < MAIN_D_801352E0) {
			k = VS_D_80070B10[MAIN_D_801352E2][i];
			if (VS_D_80072A50[k].data.location.vz < 0xdc) {
				VS_D_80072A50[k].data.location.vz += 0x42;
			}
			if (VS_D_80072E08[i] < 0x14) {
				VS_D_80072E08[i]++;
			}
			VS_D_80072A50[k].data.rotation.vx = VS_D_80072974[VS_D_80072E08[i]];
			VS_D_80072A50[k].data.rotation.vy = VS_D_800729A0[VS_D_80072E08[i]];
			if (i == 0) {
				if (won == 0) {
					VS_D_80072A50[k].data.location.vx = VS_D_800729CC[VS_D_80072E08[i]];
				} else {
					VS_D_80072A50[k].data.location.vx = VS_D_800729F8[VS_D_80072E08[i]];
				}
			}
			if (i == 4) {
				VS_D_80072A50[k].data.location.vx = VS_D_80072A24[VS_D_80072E08[4]];
			}
		}
		setupModelMatrix(&VS_D_80072A50[VS_D_80070B10[MAIN_D_801352E2][i]].data);
	}
}

void VS_renderResultModelScene(void)
{
	MATRIX lw;
	MATRIX ls;
	int32_t i;

	GsSetProjection(0x15e);
	GsWSMATRIX = *(MATRIX *)VS_D_80070ACC;
	GsClearOt(0, 4, &VS_D_80071764[ACTIVE_FRAMEBUFFER]);
	for (i = 4; i >= 0; i--) {
		GsGetLws(VS_D_80072A50[VS_D_80070B10[MAIN_D_801352E2][i]].data.obj.coord2, &lw, &ls);
		GsSetLightMatrix(&lw);
		GsSetLsMatrix(&ls);
		GsSortObject4(&VS_D_80072A50[VS_D_80070B10[MAIN_D_801352E2][i]].data.obj, &VS_D_80071764[ACTIVE_FRAMEBUFFER], 9, getScratchAddr(0));
	}
	GsSortOt(&VS_D_80071764[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void VS_removeResultModelScene(void)
{
	removeObject(0x19d, MAIN_D_801352E2);
}

void VS_setVersusModelSceneTimer(int16_t value)
{
	MAIN_D_801352E0 = value;
}
