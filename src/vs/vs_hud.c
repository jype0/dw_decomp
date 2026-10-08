#include <inline_n.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/anim.h>
#include <dw/battle.h>
#include <dw/combat.h>
#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/math.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/version.h>
#include <dw/vs.h>

#include "common.h"

#define VS_TMD_BUFFER	((uint8_t *)0x80038000)

extern int16_t BTL_FINISHER_CHARGEUP_POS[2];
extern int8_t GAME_STATE;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t VIEWPORT_DISTANCE;
extern uint8_t VS_COMMAND_MENU_LAYOUTS[][10];
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern char MAIN_D_801A8B98[];
#if VERSION_REGION_IS(NTSCJ)
extern int16_t VS_DISCIPLINE[2];
#endif

void VS_removeFinisherChargeup(void);
void VS_tickCommandMenu(uint8_t i);
int32_t readFile(char *path, uint8_t *buffer);
void GsSortBoxFill(GsBOXF *bp, GsOT *ot, unsigned short pri);
void VS_renderCommandMenu(uint8_t id);
void VS_renderTargetCursor(uint8_t id);
void VS_tickTargetCursor(uint8_t id);
void VS_removeTargetCursor(uint8_t index);
#if VERSION_REGION_IS(NTSCJ)
void VS___tickVSInput(int32_t player);
void VS__tickVSInput(int32_t instanceId);
void VS__tickDigimonP1(int32_t instanceId);
void VS__tickDigimonP2(int32_t instanceId);
#endif
void VS_shuffleBattleStartTextPieces(void);
void VS_renderBattleStartText(void);
void VS_renderBattleStartTextBurst(void);
void VS_renderNumber(int32_t a, int32_t digits, int16_t x, int16_t y, int16_t value, int32_t layer);
void VS_renderFighterHPBar(int16_t id);
void VS_renderHPBarFill(int16_t id);
void VS_renderHPBarDigits(int16_t i, int16_t id);
void VS_tickFighterStatusBars(void);
void VS_renderFighterStatusBars(int32_t id);
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
void VS_renderMoveName(int32_t i);
void VS_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, uint8_t index);
void damageTick(FighterData *fighter, Stats *stats);

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
#if VERSION_REGION_IS(NTSCJ)
	VS__tickDigimonP2,
	VS__tickDigimonP1,
	VS__tickVSInput,
	VS___tickVSInput,
	VS_removeTargetCursor,
	VS_renderTargetCursor,
	VS_tickTargetCursor,
#else
	VS_removeTargetCursor,
	VS_tickTargetCursor,
	VS_renderTargetCursor,
#endif
	VS_addTargetCursor,
	VS_removeCommandMenu,
	VS_renderCommandMenu,
	VS_tickCommandMenu,
	VS_addCommandMenu,
};

int16_t VS_COMMAND_MENU_TOP[2];
int16_t VS_COMMAND_MENU_BOTTOM[2];
uint8_t VS_COMMAND_MENU_BLINK[2];
uint8_t VS_COMMAND_MENU_TIMER[2];
uint8_t VS_COMMAND_MENU_LAYOUT[2];
int16_t VS_FINISHER_BAR_X[2];
int16_t VS_FINISHER_BAR_Y[2];
uint8_t VS_BATTLE_START_TEXT_TIMER;
int32_t VS_BATTLE_TEXT_FINISHED;
uint8_t MAIN_D_801352CC[2];
uint8_t VS_FINISHER_FULL_FRAMES[2];
uint8_t VS_FINISHER_PULSE_FRAME[2];
uint8_t VS_FINISHER_BRIGHTNESS[2];
uint8_t VS_FINISHER_SEGMENTS[2];
uint8_t VS_FINISHER_READY[2];
uint8_t MAIN_D_801352D8[2];
uint8_t MAIN_D_801352DA[2];
uint8_t *VS_DRAW_MODEL_BUFFER;
int16_t VS_WIN_LOSS_DRAW_TIMER;
uint8_t VS_WINNER_ID;

static void *vs_hud_sbss_order[] = {
	&VS_WINNER_ID,
	&VS_WIN_LOSS_DRAW_TIMER,
	&VS_DRAW_MODEL_BUFFER,
	&MAIN_D_801352DA,
	&MAIN_D_801352D8,
	&VS_FINISHER_READY,
	&VS_FINISHER_SEGMENTS,
	&VS_FINISHER_BRIGHTNESS,
	&VS_FINISHER_PULSE_FRAME,
	&VS_FINISHER_FULL_FRAMES,
	&MAIN_D_801352CC,
	&VS_BATTLE_TEXT_FINISHED,
	&VS_BATTLE_START_TEXT_TIMER,
	&VS_FINISHER_BAR_Y,
	&VS_FINISHER_BAR_X,
	&VS_COMMAND_MENU_LAYOUT,
	&VS_COMMAND_MENU_TIMER,
	&VS_COMMAND_MENU_BLINK,
	&VS_COMMAND_MENU_BOTTOM,
	&VS_COMMAND_MENU_TOP,
};

// clang-format off
MATRIX VS_BATTLE_START_TEXT_MATRIX = {
	{
		0x100a, 0x0000, 0x0000, 0x0000,
		0x08e4, 0xf299, 0x0000, 0x0d5e,
		0x08e9,
	},
	{ 0x00000000, 0xfffffffe, 0x000002d4 },
};

int16_t VS_BATTLE_START_TEXT_POSITIONS[155][2] = {
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

int32_t VS_FINISHER_PULSE[12] = {
	0x00000020, 0x00000040, 0x00000060, 0x00000080,
	0x000000a0, 0x000000c0, 0x000000e0, 0x000000ff,
	0x000000e0, 0x000000c0, 0x000000a0, 0x00000080,
};

BarSprite VS_STATUS_BAR_SPRITES[6] = {
	{ 0x01ec, 0x80, 0xa8, 0x68, 0x08, 0x0000, 0x0000 },
	{ 0x01eb, 0x90, 0xb0, 0x0b, 0x0b, 0x0003, 0xfffe },
	{ 0x01eb, 0x80, 0xb0, 0x02, 0x02, 0x0012, 0x0003 },
	{ 0x01ec, 0x80, 0xa8, 0x68, 0x08, 0x0000, 0x0000 },
	{ 0x01eb, 0x9b, 0xb0, 0x0b, 0x0b, 0x0003, 0xfffe },
	{ 0x01eb, 0x84, 0xb0, 0x02, 0x02, 0x0012, 0x0003 },
};

MATRIX VS_MODEL_SCENE_MATRIX = {
	{
		{ 0x1004, 0x0000, 0x0000 },
		{ 0x0000, 0x1000, 0x0000 },
		{ 0x0000, 0x0000, 0x1004 },
	},
	{ 0x00000000, 0x00000000, 0x000003e8 },
};

int16_t VS_RESULT_X_CURVE[18] = {
	0x0000, 0x0001, 0x0005, 0x000b, 0x0014, 0x001f, 0x002d, 0x003d,
	0x0050, 0x0065, 0x007d, 0x0097, 0x00b4, 0x00d3, 0x00f5, 0x0119,
	0x0140, 0x0000,
};

uint8_t VS_RESULT_MODEL_INDICES[2][5] = {
	{ 0x00, 0x01, 0x04, 0x05, 0x06 },
	{ 0x02, 0x03, 0x04, 0x05, 0x06 },
};

char VS_PATH_STDDAT_DRAW_TMD[] = "\\STDDAT\\DRAW.TMD";

uint8_t VS_COMMAND_LABEL_U[5] = { 0, 11, 25, 39, 50 };
uint8_t VS_COMMAND_LABEL_W[5] = { 11, 14, 14, 11, 11 };
uint8_t VS_FINISHER_SEGMENT_U[8] = { 0, 6, 10, 18, 22, 30, 38, 43 };
uint8_t VS_FINISHER_SEGMENT_W[8] = { 6, 4, 8, 4, 8, 8, 5, 5 };
uint8_t VS_FINISHER_SEGMENT_X[8] = { 0, 6, 10, 18, 22, 30, 38, 43 };
char *VS_VERSUS_MODEL_PATH = VS_PATH_STDDAT_DRAW_TMD;
int16_t VS_VERSUS_MODEL_TARGET_X[4] = { -273, -86, 94, 272 };

char VS_PATH_STDDAT_1P2PWIN_TMD[20] = "\\STDDAT\\1P2PWIN.TMD";
// clang-format on

void VS_addCommandMenu(uint8_t index)
{
	VS_COMMAND_MENU_TOP[index] = 0x44;
	VS_COMMAND_MENU_BOTTOM[index] = VS_COMMAND_MENU_TOP[index] + 0x20;
	VS_COMMAND_MENU_BLINK[index] = 0;
	VS_COMMAND_MENU_TIMER[index] = 0;

	switch (COMBAT_DATA_PTR->player.numCommands[index]) {
	case 2:
		VS_COMMAND_MENU_LAYOUT[index] = 0;
		break;
	case 3:
		VS_COMMAND_MENU_LAYOUT[index] = 1;
		break;
	case 4:
		VS_COMMAND_MENU_LAYOUT[index] = 2;
		break;
	case 5:
		VS_COMMAND_MENU_LAYOUT[index] = 3;
		break;
	case 6:
		VS_COMMAND_MENU_LAYOUT[index] = 4;
		break;
	case 7:
		VS_COMMAND_MENU_LAYOUT[index] = 5;
		break;
	case 8:
		VS_COMMAND_MENU_LAYOUT[index] = 6;
		break;
	case 9:
		VS_COMMAND_MENU_LAYOUT[index] = 7;
		break;
	}

	MAIN_D_80134AC8[index] = 0;
	addObject(0x198, index, VS_tickCommandMenu, VS_renderCommandMenu);
}

void VS_tickCommandMenu(uint8_t i)
{
	VS_COMMAND_MENU_TIMER[i]++;
	if (GAME_STATE != 0) {
		if (GAME_STATE == 4) {
			if ((VS_COMMAND_MENU_TIMER[i] % 8) == 0) {
				VS_COMMAND_MENU_BLINK[i] = (VS_COMMAND_MENU_BLINK[i] + 1) & 1;
			}
		}
	}
}

GARBAGE(VS_renderCommandMenu, 17);

void VS_renderCommandMenu(uint8_t id)
{
	POLY_FT4 *prim;
	int32_t i;
	int16_t rowY;
	int16_t width;
	int16_t base;
	int16_t x;
	int16_t count;

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
		setClut(prim, 282, 497);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVDataPolyFT4(prim, 0x3d, 0xe0, 0x16, 0x16);
		if ((count % 2) == 0) {
			int32_t y;

			if ((COMBAT_DATA_PTR->player.hoveredCommand[id] % 2) == 0) {
				y = VS_COMMAND_MENU_TOP[id];
			} else {
				y = VS_COMMAND_MENU_TOP[id] + 0xa;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		} else {
			int32_t y;

			if ((COMBAT_DATA_PTR->player.hoveredCommand[id] % 2) == 1) {
				y = VS_COMMAND_MENU_TOP[id];
			} else {
				y = VS_COMMAND_MENU_TOP[id] + 0xa;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	for (i = 1; i < COMBAT_DATA_PTR->player.numCommands[id]; i++) {
		SetPolyFT4(prim);
		prim->tpage = getTPage(0, 0, 960, 256);
		setClut(prim, 272, 496);
		setRGB0(prim, 0x80, 0x80, 0x80);
		VS_setCommandIconUV((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]], prim, COMBAT_DATA_PTR->player.availableCommands[id][i]);
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
		x = (int16_t)base - (i * 0xe);
		if ((count % 2) == 0) {
			setXY4(prim, x, ((i % 2) == 0) ? VS_COMMAND_MENU_TOP[id] + 3 : VS_COMMAND_MENU_TOP[id] + 0xd, x + 0x10, ((i % 2) == 0) ? VS_COMMAND_MENU_TOP[id] + 3 : VS_COMMAND_MENU_TOP[id] + 0xd, x, ((i % 2) == 0) ? VS_COMMAND_MENU_BOTTOM[id] - 0xd : VS_COMMAND_MENU_BOTTOM[id] - 3, x + 0x10, ((i % 2) == 0) ? VS_COMMAND_MENU_BOTTOM[id] - 0xd : VS_COMMAND_MENU_BOTTOM[id] - 3);
		} else {
			setXY4(prim, x, ((i % 2) == 1) ? VS_COMMAND_MENU_TOP[id] + 3 : VS_COMMAND_MENU_TOP[id] + 0xd, x + 0x10, ((i % 2) == 1) ? VS_COMMAND_MENU_TOP[id] + 3 : VS_COMMAND_MENU_TOP[id] + 0xd, x, ((i % 2) == 1) ? VS_COMMAND_MENU_BOTTOM[id] - 0xd : VS_COMMAND_MENU_BOTTOM[id] - 3, x + 0x10, ((i % 2) == 1) ? VS_COMMAND_MENU_BOTTOM[id] - 0xd : VS_COMMAND_MENU_BOTTOM[id] - 3);
		}
		if ((i == COMBAT_DATA_PTR->player.hoveredCommand[id]) && (VS_COMMAND_MENU_BLINK[id] == 1)) {
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
		setTPage(prim, 0, 0, 960, 256);
		setClut(prim, 272, 497);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVWH(prim, VS_COMMAND_LABEL_U[VS_COMMAND_MENU_LAYOUTS[VS_COMMAND_MENU_LAYOUT[id]][i]], 0xe0, VS_COMMAND_LABEL_W[VS_COMMAND_MENU_LAYOUTS[VS_COMMAND_MENU_LAYOUT[id]][i]], 31);
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
		setXY4(prim, (rowY - 0x8f) + id * 0xa6, VS_COMMAND_MENU_TOP[id], ((rowY - 0x8f) + width) + id * 0xa6, VS_COMMAND_MENU_TOP[id], (rowY - 0x8f) + id * 0xa6, VS_COMMAND_MENU_BOTTOM[id], ((rowY - 0x8f) + width) + id * 0xa6, VS_COMMAND_MENU_BOTTOM[id]);
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
	COMBAT_DATA_PTR->player.bufferedCommand[id] = COMBAT_DATA_PTR->player.currentCommand[id] = 3;
	VS_FINISHER_BAR_X[id] = id * 0xea - 0x8c;
	VS_FINISHER_BAR_Y[id] = -0x4a;
	addObject(0x19a, id, VS_tickTargetCursor, VS_renderTargetCursor);
}

void VS_renderTargetCursor(uint8_t id)
{
	POLY_FT4 prim;
	int32_t i;
	int16_t bars;

	SetPolyFT4(&prim);
	prim.tpage = getTPage(0, 0, 960, 256);
	setClut(&prim, 272, 498);
	setRGB0(&prim, 0x80, 0x80, 0x80);
	setUVWH(&prim, 0x58, 0xe0, 46, 12);
	setXYWH(&prim, VS_FINISHER_BAR_X[id], VS_FINISHER_BAR_Y[id], 0x2e, 0xc);
	GsSortPoly(&prim, ACTIVE_ORDERING_TABLE, 7);
	bars = COMBAT_DATA_PTR->player.finisherChargeup[id] / 8;
	setUVWH(&prim, 0x88, 0xe0, 4, 6);
	for (i = 0; i < bars; i++) {
		setXYWH(&prim, (int32_t)(VS_FINISHER_BAR_X[id] + 3 + i * 4), VS_FINISHER_BAR_Y[id] + 3, 4, 6);
		GsSortPoly(&prim, ACTIVE_ORDERING_TABLE, 7);
	}

	if (COMBAT_DATA_PTR->player.remainingChargeupTime[id] == 0) {
		VS_removeTargetCursor(id);
	}
}

void VS_tickTargetCursor(uint8_t id)
{
	uint32_t input;
	uint32_t prev;
	int32_t up;

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
	if ((COMBAT_DATA_PTR->player.finisherChargeup[id] < 0x50) && (up != 0)) {
		COMBAT_DATA_PTR->player.finisherChargeup[id] += 2;
	}
	POLLED_INPUT = input;
	POLLED_INPUT_PREVIOUS = prev;
}

void VS_removeTargetCursor(uint8_t index)
{
	if (COMBAT_DATA_PTR->player.remainingChargeupTime[index] != -1) {
		removeObject(0x19a, index);
		COMBAT_DATA_PTR->player.remainingChargeupTime[index] = -1;
	}
}

#if VERSION_REGION_IS(NTSCJ)
void VS___tickVSInput(int32_t player)
{
	uint32_t input;
	uint32_t previous;

	input = POLLED_INPUT;
	previous = POLLED_INPUT_PREVIOUS;
	if (player == 1) {
		POLLED_INPUT = (POLLED_INPUT >> 16) & 0xffff;
		POLLED_INPUT_PREVIOUS = (POLLED_INPUT_PREVIOUS >> 16) & 0xffff;
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x8000) {
		playSound(0, 2);
		COMBAT_DATA_PTR->player.hoveredCommand[player]++;
		if (COMBAT_DATA_PTR->player.hoveredCommand[player] > COMBAT_DATA_PTR->player.numCommands[player] - 1) {
			COMBAT_DATA_PTR->player.hoveredCommand[player] = 1;
		}
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x2000) {
		playSound(0, 2);
		COMBAT_DATA_PTR->player.hoveredCommand[player]--;
		if (COMBAT_DATA_PTR->player.hoveredCommand[player] <= 0) {
			COMBAT_DATA_PTR->player.hoveredCommand[player] = COMBAT_DATA_PTR->player.numCommands[player] - 1;
		}
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON) {
		playSound(0, 3);
		COMBAT_DATA_PTR->player.bufferedCommand[player] = COMBAT_DATA_PTR->player.availableCommands[player][COMBAT_DATA_PTR->player.hoveredCommand[player]];
		if (VS_DISCIPLINE[player] < 0x46) {
			COMBAT_DATA_PTR->player.commandDelay[player] = 0xa0 - VS_DISCIPLINE[player] / 10;
		} else {
			COMBAT_DATA_PTR->player.commandDelay[player] = (0xa - VS_DISCIPLINE[player] / 10) * 10;
		}
		COMBAT_DATA_PTR->player.commandDelay[player] = 0;
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x80) {
		if (COMBAT_DATA_PTR->fighter[player].finisherProgress == COMBAT_DATA_PTR->fighter[player].finisherGoal) {
			COMBAT_DATA_PTR->player.bufferedCommand[player] = 0xb;
			playSound(0, 3);
			COMBAT_DATA_PTR->player.commandDelay[player] = 0;
			COMBAT_DATA_PTR->player.currentCommand[player] = 0xb;
		}
	}
	POLLED_INPUT = input;
	POLLED_INPUT_PREVIOUS = previous;
}

// clang-format off
void VS__tickVSInput(instanceId)
	int16_t instanceId;
// clang-format on
{
	if (GAME_STATE == 4) {
		VS___tickVSInput(instanceId);
	}
}

// clang-format off
void VS__tickDigimonP1(instanceId)
	int16_t instanceId;
// clang-format on
{
	tickAnimation(ENTITY_TABLE[instanceId]);
}

// clang-format off
void VS__tickDigimonP2(instanceId)
	int16_t instanceId;
// clang-format on
{
	tickAnimation(ENTITY_TABLE[instanceId]);
}
#endif

void VS_shuffleBattleStartTextPieces(void)
{
	int32_t i;
	int32_t r;

	for (i = 0; i < 0x9b; i++) {
		r = randomLimit(0x9b);
		swapByte(&VS_BATTLE_START_TEXT_PIECES[i][0x11], &VS_BATTLE_START_TEXT_PIECES[r][0x11]);
	}
}

void VS_initializeBattleStartText(void)
{
	uint8_t (*p)[20];
	int32_t sgn;
	int32_t i;
	int32_t r;
#if !VERSION_REGION_IS(NTSCJ)
	int32_t t;
#endif

	VS_BATTLE_START_TEXT_TIMER = 0;
	VS_BATTLE_TEXT_FINISHED = 0;
	p = VS_BATTLE_START_TEXT_PIECES;
	for (i = 0; i < 0x9b; i++, p++) {
		(*p)[0x11] = i;
		(*p)[0x12] = 0x18;
		(*p)[0x13] = randomLimit(3);
	}

	VS_shuffleBattleStartTextPieces();

	p = VS_BATTLE_START_TEXT_PIECES;
	for (i = 0; i < 0x9b; i++, p++) {
		if (randomLimit(2) == 1) {
			sgn = 1;
		} else {
			sgn = -1;
		}
		((int16_t *)*p)[5] = VS_BATTLE_START_TEXT_POSITIONS[i][1];
		((int8_t *)*p)[0x10] = -sgn * ((randomLimit(3) + 1) << 5);
		if ((0 <= i) && (i < 0x33)) {
			((int16_t *)*p)[4] = (sgn * 500) + randomLimit(100) - 50;
		} else if ((0x33 <= i) && (i < 0x65)) {
			((int16_t *)*p)[4] = (sgn * 600) + randomLimit(100) - 50;
		} else {
			((int16_t *)*p)[4] = (sgn * 700) + randomLimit(100) - 50;
		}
		r = randomLimit(5);
#if VERSION_REGION_IS(NTSCJ)
		((int16_t *)*p)[6] = (r + 8) * VS_BATTLE_START_TEXT_POSITIONS[i][0] / 8;
		((int16_t *)*p)[7] = (r + 8) * VS_BATTLE_START_TEXT_POSITIONS[i][1] / 8;
#else
		t = VS_BATTLE_START_TEXT_POSITIONS[i][0];
		((int16_t *)*p)[6] = (r + 8) * t / 8;
		t = VS_BATTLE_START_TEXT_POSITIONS[i][1];
		((int16_t *)*p)[7] = (r + 8) * t / 8;
#endif
		((int16_t *)*p)[0] = 0;
		((int16_t *)*p)[1] = 0;
		((int16_t *)*p)[2] = 0;
	}

	addObject(0x1a6, 0, NULL, VS_renderBattleStartText);
}

void VS_renderBattleStartText(void)
{
	POLY_F4 *shadow;
	uint8_t (*p)[20];
	POLY_FT4 *prim;
	int32_t i;
	int32_t n;
	GsOT_TAG *ot;
	int32_t otz;
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
	SVECTOR q0;
	SVECTOR q1;
	SVECTOR q2;
	SVECTOR q3;
	POLY_FT4 *ft;
	uint16_t clut;
#if VERSION_EQUAL_OR_NEWER(JP_TRIAL)
	MATRIX m;

	m = GsWSMATRIX;
#endif
	GsSetProjection(0x200);
	GsSetLsMatrix(&VS_BATTLE_START_TEXT_MATRIX);

	n = 0;
	for (i = 0; i < 0x9b; i++) {
		if (VS_BATTLE_START_TEXT_PIECES[i][0x12] != 0) {
			break;
		}
		n++;
	}

	if (n == 0x9b) {
		clut = GetClut(256, (VS_BATTLE_START_TEXT_TIMER++ % 6 / 2) + 488);
		VS_BATTLE_TEXT_FINISHED = 1;
	} else {
		clut = GetClut(256, 488);
	}

	p = VS_BATTLE_START_TEXT_PIECES;
	prim = (POLY_FT4 *)GsGetWorkBase();
	ot = ACTIVE_ORDERING_TABLE->org;
	for (i = 0; i < 0x9b; i++, p++) {
		if (((int16_t *)*p)[4] != VS_BATTLE_START_TEXT_POSITIONS[i][0]) {
			((int16_t *)*p)[4] += ((int8_t *)*p)[0x10];
			if (((int8_t *)*p)[0x10] > 0) {
				if (((int16_t *)*p)[4] > VS_BATTLE_START_TEXT_POSITIONS[i][0]) {
					((int16_t *)*p)[4] = VS_BATTLE_START_TEXT_POSITIONS[i][0];
				}
			} else {
				if (((int16_t *)*p)[4] < VS_BATTLE_START_TEXT_POSITIONS[i][0]) {
					((int16_t *)*p)[4] = VS_BATTLE_START_TEXT_POSITIONS[i][0];
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
		setEntityTextDigit(prim, 256, 488);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->clut = clut;
		gte_ldv3(&p0, &p1, &p2);
		gte_rtpt();
		gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
		gte_stszotz(&otz);
		gte_ldv0(&p3);
		gte_rtps();
		gte_stsxy(&prim->x3);
		setUVWH(prim, (((uint8_t *)*p)[0x13] * 8) + 0x80, 0x80, 8, 8);
		AddPrim(ot + 5, prim++);

		if (n != 0x9b) {
			SetPolyFT4(prim);
			setRGB0(prim, 0x80, 0x80, 0x80);
			setTPage(prim, 0, 0, 896, 384);
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
#if VERSION_IS(JP)
	GsSetRefView2(&GS_VIEWPOINT);
#else
	GsWSMATRIX = m;
#endif
}

void VS_removeBattleStartText(void)
{
	removeObject(0x1a6, 0);
}

void VS_initializeBattleStartTextBurst(void)
{
	VS_BATTLE_TEXT_FINISHED = 0;
	addObject(0x1a6, 0, NULL, VS_renderBattleStartTextBurst);
}

void VS_renderBattleStartTextBurst(void)
{
	POLY_F4 *shadow;
	uint8_t (*p)[20];
	POLY_FT4 *prim;
	int32_t i;
	int32_t j;
	int32_t otz;
	int32_t dead;
	GsOT_TAG *ot;
	SVECTOR corner[4];
	SVECTOR pts[4];
	SVECTOR out;
	MATRIX m;
	POLY_FT4 *ft;
	int16_t cx;
	int16_t cy;
#if VERSION_EQUAL_OR_NEWER(JP_TRIAL)
	MATRIX saved;

	saved = GsWSMATRIX;
#endif
	GsSetProjection(0x200);
	GsSetLsMatrix(&VS_BATTLE_START_TEXT_MATRIX);

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

	dead = 0;
	p = VS_BATTLE_START_TEXT_PIECES;
	ot = ACTIVE_ORDERING_TABLE->org;
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
		setEntityTextDigit(prim, 256, 488);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setClut(prim, 256, 488);
		gte_ldv3(&pts[0], &pts[1], &pts[2]);
		gte_rtpt();
		gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
		gte_stszotz(&otz);
		gte_ldv0(&pts[3]);
		gte_rtps();
		gte_stsxy(&prim->x3);
		setUVWH(prim, (((uint8_t *)*p)[0x13] * 8) + 0x80, 0x80, 8, 8);
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
		((int16_t *)*p)[4] += ((int16_t *)*p)[6];
		((int16_t *)*p)[5] += ((int16_t *)*p)[7];
	}

	GsSetWorkBase((PACKET *)prim);
	if (dead == 0x9b) {
		VS_BATTLE_TEXT_FINISHED = 1;
	}
	GsSetProjection(VIEWPORT_DISTANCE);
#if VERSION_IS(JP)
	GsSetRefView2(&GS_VIEWPOINT);
#else
	GsWSMATRIX = saved;
#endif
}

void VS_removeBattleStartTextBurst(void)
{
	removeObject(0x1a6, 0);
}

int32_t VS_isBattleStartTextFinished(void)
{
	return VS_BATTLE_TEXT_FINISHED;
}

void VS_renderNumber(int32_t a, int32_t digits, int16_t x, int16_t y, int16_t value, int32_t layer)
{
	POLY_FT4 *prim;
	int32_t i;
	int32_t count;
	int32_t buf[4];
#if !VERSION_REGION_IS(NTSCJ)
	uint32_t width;
#endif

	prim = (POLY_FT4 *)GsGetWorkBase();
#if !VERSION_REGION_IS(NTSCJ)
	width = digits;
#endif
	convertValueToDigits(digits, value, &count, buf);
	for (i = count - 1; i >= 0; i--) {
		setEntityTextDigit(prim, 256, 492);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVDataPolyFT4(prim, buf[i] * 7, 172, 7, 11);
#if VERSION_REGION_IS(NTSCJ)
		setPosDataPolyFT4(prim, x + ((digits - 1) - i) * 7, y, 7, 11);
#else
		setPosDataPolyFT4(prim, x + (((int32_t)width - 1) - i) * 7, y, 7, 11);
#endif
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}
	GsSetWorkBase((PACKET *)prim);
}

void VS_renderFighterHPBar(int16_t id)
{
	int32_t n;
	int32_t i;

	if (GAME_STATE != 4) {
		return;
	}

	n = (COMBAT_DATA_PTR->fighter[id].finisherProgress * 6) / COMBAT_DATA_PTR->fighter[id].finisherGoal;

	if (VS_FINISHER_SEGMENTS[id] != n) {
		VS_FINISHER_PULSE_FRAME[id] = 0;
		VS_FINISHER_SEGMENTS[id] = n;
	}

	VS_FINISHER_BRIGHTNESS[id] = VS_FINISHER_PULSE[VS_FINISHER_PULSE_FRAME[id]];
	if (VS_FINISHER_PULSE_FRAME[id] < 0xb) {
		VS_FINISHER_PULSE_FRAME[id]++;
	}

	if (n == 6) {
		if (VS_FINISHER_FULL_FRAMES[id] < 0xa) {
			VS_FINISHER_FULL_FRAMES[id]++;
		}
		if (VS_FINISHER_FULL_FRAMES[id] >= 3) {
			n++;
		}
		if (VS_FINISHER_FULL_FRAMES[id] >= 5) {
			n++;
			VS_renderHPBarFill(id);
			if (VS_FINISHER_FULL_FRAMES[id] == 0xa) {
				VS_FINISHER_READY[id] = 1;
				VS_FINISHER_PULSE_FRAME[id] %= 0xb;
			}
		}
	}

	for (i = 0; i < n; i++) {
		VS_renderHPBarDigits(i, id);
	}
}

void VS_renderHPBarFill(int16_t id)
{
	GsBOXF box;
	POLY_FT4 *prim;

	prim = (POLY_FT4 *)GsGetWorkBase();
	setEntityTextDigit(prim, 256, 0x1e2);
	if ((VS_FINISHER_READY[id] != 1) || (COMBAT_DATA_PTR->player.currentCommand[id] == 0xb)) {
		setRGB0(prim, 0x80, 0x80, 0x80);
	} else {
		setRGB0(prim, VS_FINISHER_BRIGHTNESS[id], VS_FINISHER_BRIGHTNESS[id], VS_FINISHER_BRIGHTNESS[id]);
	}

	setUVWH(prim, 0x80, 0x88, 37, 9);
	setXYWH(prim, (id == 0 ? -0x56 : 0x6e), -0x4b, 0x25, 9);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 8, prim++);
	GsSetWorkBase((PACKET *)prim);

	box.attribute = 0x40000000;
	if ((VS_FINISHER_READY[id] != 1) || (COMBAT_DATA_PTR->player.currentCommand[id] == 0xb)) {
		box.r = box.g = box.b = 0x80;
	} else {
		box.r = box.g = box.b = VS_FINISHER_BRIGHTNESS[id];
	}

	setWH(&box, 0x29, 0xb);
	box.x = (id == 0 ? -0x58 : 0x6c);
	box.y = -0x4c;
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 8);
}

void VS_renderHPBarDigits(int16_t i, int16_t id)
{
	POLY_FT4 *prim;
	int32_t n;

	n = (COMBAT_DATA_PTR->fighter[id].finisherProgress * 6) / COMBAT_DATA_PTR->fighter[id].finisherGoal;
	prim = (POLY_FT4 *)GsGetWorkBase();
	setEntityTextDigit(prim, 0x100, 0x1ec);

	if ((((n - 1) == i) || (VS_FINISHER_READY[id] == 1)) && (COMBAT_DATA_PTR->player.currentCommand[id] != 0xb)) {
		setRGB0(prim, VS_FINISHER_BRIGHTNESS[id], VS_FINISHER_BRIGHTNESS[id], VS_FINISHER_BRIGHTNESS[id]);
	} else {
		setRGB0(prim, 0x80, 0x80, 0x80);
	}

	setUVWH(prim, VS_FINISHER_SEGMENT_U[i], 0x9d, VS_FINISHER_SEGMENT_W[i], 15);
	setXYWH(prim, (id == 0 ? VS_FINISHER_SEGMENT_X[i] - 0x8d : VS_FINISHER_SEGMENT_X[i] + 0x37), -0x4f, VS_FINISHER_SEGMENT_W[i], 15);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 8, prim++);
	GsSetWorkBase((PACKET *)prim);
}

// clang-format off
void VS_addFighterStatusBars(id)
	int16_t id;
// clang-format on
{
	MAIN_D_801352CC[id] = 0;
	VS_FINISHER_FULL_FRAMES[id] = 0;
	VS_FINISHER_PULSE_FRAME[id] = 11;
	VS_FINISHER_BRIGHTNESS[id] = 0;
	VS_FINISHER_SEGMENTS[id] = 0;
	VS_FINISHER_READY[id] = 0;
	MAIN_D_801352DA[id] = MAIN_D_801352D8[id] = 0;
	addObject(0x19c, id, VS_tickFighterStatusBars, VS_renderFighterStatusBars);
}

void VS_tickFighterStatusBars(void)
{
}

// clang-format off
void VS_renderFighterStatusBars(id)
	int16_t id;
// clang-format on
{
	int32_t bar;
	int32_t k;
	int16_t *hpPtr;
	FighterData *fighter;
	POLY_FT4 *prim;
	BarSprite *p;
	int16_t *mpPtr;
	int16_t cur;
	int16_t fill;
	int16_t maxHp;
	int16_t maxMp;
	int16_t x0;
	int16_t y0;

	fighter = &COMBAT_DATA_PTR->fighter[id];
	hpPtr = &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]])->stats.current.currentHP;
	mpPtr = &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]])->stats.current.currentMP;
	maxHp = ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]])->stats.base.hp;
	maxMp = ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]])->stats.base.mp;

	if (*hpPtr == 0) {
		fighter->hpDamageBuffer = 0;
	}
	if (fighter->hpDamageBuffer != 0) {
		damageTick(fighter, &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]])->stats);
	}
	*mpPtr -= fighter->mpDamageBuffer;
	fighter->mpDamageBuffer = 0;
	if (*mpPtr < 0) {
		*mpPtr = 0;
	}

	for (bar = 0; bar < 2; bar++) {
		p = &VS_STATUS_BAR_SPRITES[bar * 3 + 2];
		if (bar == 0) {
			if (id == 0) {
				x0 = -0x8c;
			} else {
				x0 = 0x24;
			}
			y0 = -0x64;
			cur = *hpPtr;
			fill = *hpPtr * 50 / maxHp;
		} else {
			if (id == 0) {
				x0 = -0x8c;
			} else {
				x0 = 0x24;
			}
			y0 = -0x58;
			cur = *mpPtr;
			fill = *mpPtr * 50 / maxMp;
		}
		VS_renderNumber(0, 4, x0 + 0x49, y0 - 3, cur, 8);
		prim = (POLY_FT4 *)GsGetWorkBase();
		for (k = 0; k < 3; k++, p--) {
			SetPolyFT4(prim);
			prim->clut = GetClut(0x100, p->clut);
			prim->tpage = getTPage(0, 0, 896, 256);
			setRGB0(prim, 0x80, 0x80, 0x80);
			setUVWH(prim, p->u, p->v, p->w, p->h);
			setXY0(prim, x0 + p->x, y0 + p->y);
			prim->x1 = x0 + p->x + (k == 0 ? fill : p->w);
			prim->y1 = y0 + p->y;
			prim->x2 = x0 + p->x;
			prim->y2 = p->h + (y0 + p->y);
			prim->x3 = x0 + p->x + (k == 0 ? fill : p->w);
			prim->y3 = p->h + (y0 + p->y);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 8, prim++);
		}
		GsSetWorkBase((PACKET *)prim);
	}
	VS_renderFighterHPBar(id);
}

// clang-format off
void VS_removeFighterStatusBars(i)
	int16_t i;
// clang-format on
{
	removeObject(0x19c, i);
}

void VS_loadVersusSceneModel(void)
{
	int32_t i;

	VS_DRAW_MODEL_BUFFER = VS_TMD_BUFFER;
	readFile(VS_VERSUS_MODEL_PATH, VS_DRAW_MODEL_BUFFER);
	GsMapModelingData((u_long *)(VS_DRAW_MODEL_BUFFER + 4));

	for (i = 0; i < 4; i++) {
		GsLinkObject4((u_long)(VS_DRAW_MODEL_BUFFER + 0xc), &VS_VERSUS_MODEL_OBJECTS[i].data.obj, i);
		GsInitCoordinate2(NULL, &VS_VERSUS_MODEL_OBJECTS[i].data.posMatrix);
		VS_VERSUS_MODEL_OBJECTS[i].data.obj.attribute = 0;
		VS_VERSUS_MODEL_OBJECTS[i].data.obj.coord2 = &VS_VERSUS_MODEL_OBJECTS[i].data.posMatrix;
	}

	for (i = 0; i < 4; i++) {
		VS_VERSUS_MODEL_OBJECTS[i].data.scale.vx = 0x1000;
		VS_VERSUS_MODEL_OBJECTS[i].data.scale.vy = 0x1000;
		VS_VERSUS_MODEL_OBJECTS[i].data.scale.vz = 0x1000;
		VS_VERSUS_MODEL_OBJECTS[i].data.rotation.vx = 0;
		VS_VERSUS_MODEL_OBJECTS[i].data.rotation.vy = 0;
		VS_VERSUS_MODEL_OBJECTS[i].data.rotation.vz = 0;
		VS_VERSUS_MODEL_OBJECTS[i].data.location.vx = 0x3e8;
		VS_VERSUS_MODEL_OBJECTS[i].data.location.vy = 0x78;
		VS_VERSUS_MODEL_OBJECTS[i].data.location.vz = 0x280;
		setupModelMatrix(&VS_VERSUS_MODEL_OBJECTS[i].data);
	}
}

void VS_addVersusModelScene(void)
{
	VS_WIN_LOSS_DRAW_TIMER = 0;
	addObject(0x19d, 0, VS_tickVersusModelScene, VS_renderVersusModelScene);
}

void VS_tickVersusModelScene(void)
{
	int32_t i;

	VS_WIN_LOSS_DRAW_TIMER++;
	for (i = 0; i < 4; i++) {
		if (VS_WIN_LOSS_DRAW_TIMER > i * 5) {
			if (VS_VERSUS_MODEL_OBJECTS[i].data.location.vx != VS_VERSUS_MODEL_TARGET_X[i]) {
				VS_VERSUS_MODEL_OBJECTS[i].data.location.vx -= 200;
				if (VS_VERSUS_MODEL_OBJECTS[i].data.location.vx < VS_VERSUS_MODEL_TARGET_X[i]) {
					VS_VERSUS_MODEL_OBJECTS[i].data.location.vx = VS_VERSUS_MODEL_TARGET_X[i];
				}
			}
		}
		setupModelMatrix(&VS_VERSUS_MODEL_OBJECTS[i].data);
	}
}

void VS_renderVersusModelScene(void)
{
	MATRIX lw;
	MATRIX ls;
	int32_t i;

	GsSetProjection(0x200);
	GsWSMATRIX = VS_MODEL_SCENE_MATRIX;
	GsClearOt(0, 4, &VS_MODEL_SCENE_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);

	for (i = 0; i < 4; i++) {
		GsGetLws(VS_VERSUS_MODEL_OBJECTS[i].data.obj.coord2, &lw, &ls);
		GsSetLightMatrix(&lw);
		GsSetLsMatrix(&ls);
		GsSortObject4(&VS_VERSUS_MODEL_OBJECTS[i].data.obj, &VS_MODEL_SCENE_ORDERING_TABLE[ACTIVE_FRAMEBUFFER], 9,
		              getScratchAddr(0));
	}

	GsSortOt(&VS_MODEL_SCENE_ORDERING_TABLE[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void VS_removeVersusModelScene(void)
{
	removeObject(0x19d, 0);
}

int32_t VS_isVersusModelSceneFinished(void)
{
	if (VS_WIN_LOSS_DRAW_TIMER >= 0x51) {
		return 1;
	} else {
		return 0;
	}
}

void VS_loadStageModels(void)
{
	long i;
	uint8_t *buf;

	for (i = 0x14; i < 0x29; i++) {
		VS_RESULT_ROTATION_X[i - 0x14] = ((i * 0x2000) / 40) & 0xfff;
		VS_RESULT_ROTATION_Y[i - 0x14] = (((i * 0x800) / 40) + 0x800) & 0xfff;
		if ((i >= 0x14) && (i < 0x25)) {
			VS_D_800729CC[i - 0x14] = VS_RESULT_X_CURVE[0x24 - i] - 0x134;
			VS_D_800729F8[i - 0x14] = VS_RESULT_X_CURVE[0x24 - i] - 0x140;
			VS_RESULT_LAST_X[i - 0x14] = 0x111 - VS_RESULT_X_CURVE[0x24 - i];
		}
		if ((i >= 0x25) && (i < 0x29)) {
			VS_D_800729CC[i - 0x14] = VS_RESULT_X_CURVE[i - 0x24] - 0x134;
			VS_D_800729F8[i - 0x14] = VS_RESULT_X_CURVE[i - 0x24] - 0x140;
			VS_RESULT_LAST_X[i - 0x14] = 0x111 - VS_RESULT_X_CURVE[i - 0x24];
		}
	}

	VS_MODEL_SCENE_ORDERING_TABLE[0].length = 5;
	VS_MODEL_SCENE_ORDERING_TABLE[0].org = VS_MODEL_SCENE_OT_TAGS_0;
	VS_MODEL_SCENE_ORDERING_TABLE[1].length = 5;
	VS_MODEL_SCENE_ORDERING_TABLE[1].org = VS_MODEL_SCENE_OT_TAGS_1;
	buf = (uint8_t *)MAIN_D_801A8B98;
	readFile(VS_PATH_STDDAT_1P2PWIN_TMD, buf);
	GsMapModelingData((u_long *)(buf + 4));
	for (i = 0; i < 7; i++) {
		GsLinkObject4((u_long)(buf + 0xc), &VS_RESULT_MODEL_OBJECTS[i].data.obj, i);
		GsInitCoordinate2(NULL, &VS_RESULT_MODEL_OBJECTS[i].data.posMatrix);
		VS_RESULT_MODEL_OBJECTS[i].data.obj.attribute = 0;
		VS_RESULT_MODEL_OBJECTS[i].data.obj.coord2 = &VS_RESULT_MODEL_OBJECTS[i].data.posMatrix;
	}
}

void VS_addResultModelScene(Entity *entity)
{
	int32_t i;

	VS_WIN_LOSS_DRAW_TIMER = 0;
	if (entity == ENTITY_TABLE[1]) {
		VS_WINNER_ID = 0;
	} else {
		VS_WINNER_ID = 1;
	}
	VS_RESULT_MODEL_OBJECTS[VS_WINNER_ID * 2].data.location.vx = -0x148;
	VS_RESULT_MODEL_OBJECTS[VS_WINNER_ID * 2].data.location.vy = 0;
	VS_RESULT_MODEL_OBJECTS[VS_WINNER_ID * 2].data.location.vz = -0x4d0;
	VS_RESULT_MODEL_OBJECTS[VS_WINNER_ID * 2 + 1].data.location.vx = -0x9b;
	VS_RESULT_MODEL_OBJECTS[VS_WINNER_ID * 2 + 1].data.location.vy = 0;
	VS_RESULT_MODEL_OBJECTS[VS_WINNER_ID * 2 + 1].data.location.vz = -0x4d0;
	VS_RESULT_MODEL_OBJECTS[VS_WINNER_ID * 2].data.location.vx = -0x154;
	VS_RESULT_MODEL_OBJECTS[VS_WINNER_ID * 2].data.location.vy = 0;
	VS_RESULT_MODEL_OBJECTS[VS_WINNER_ID * 2].data.location.vz = -0x4d0;
	VS_RESULT_MODEL_OBJECTS[VS_WINNER_ID * 2 + 1].data.location.vx = -0x9b;
	VS_RESULT_MODEL_OBJECTS[VS_WINNER_ID * 2 + 1].data.location.vy = 0;
	VS_RESULT_MODEL_OBJECTS[VS_WINNER_ID * 2 + 1].data.location.vz = -0x4d0;
	VS_RESULT_MODEL_OBJECTS[4].data.location.vx = 0;
	VS_RESULT_MODEL_OBJECTS[4].data.location.vy = 0;
	VS_RESULT_MODEL_OBJECTS[4].data.location.vz = -0x4d0;
	VS_RESULT_MODEL_OBJECTS[5].data.location.vx = 0x84;
	VS_RESULT_MODEL_OBJECTS[5].data.location.vy = 0;
	VS_RESULT_MODEL_OBJECTS[5].data.location.vz = -0x4d0;
	VS_RESULT_MODEL_OBJECTS[6].data.location.vx = 0xfd;
	VS_RESULT_MODEL_OBJECTS[6].data.location.vy = 0;
	VS_RESULT_MODEL_OBJECTS[6].data.location.vz = -0x4d0;
	for (i = 0; i < 5; i++) {
		VS_RESULT_MODEL_STEPS[i] = 0;
		VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.scale.vx = 0x1000;
		VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.scale.vy = 0x1000;
		VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.scale.vz = 0x1000;
		VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.rotation.vx = 0;
		VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.rotation.vy = 0;
		VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.rotation.vz = 0;
		setupModelMatrix(&VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data);
	}
	addObject(0x19d, VS_WINNER_ID, VS_tickResultModelScene, VS_renderResultModelScene);
}

// clang-format off
void VS_tickResultModelScene(won)
	int16_t won;
// clang-format on
{
	int32_t i;

	if (VS_WIN_LOSS_DRAW_TIMER < 0xa0) {
		VS_WIN_LOSS_DRAW_TIMER++;
	}
	for (i = 0; i < 5; i++) {
		if (VS_WIN_LOSS_DRAW_TIMER > i * 6 + 0x3c) {
			if (VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.location.vz < 0xdc) {
				VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.location.vz += 0x42;
			}
			if (VS_RESULT_MODEL_STEPS[i] < 0x14) {
				VS_RESULT_MODEL_STEPS[i]++;
			}
			VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.rotation.vx = VS_RESULT_ROTATION_X[VS_RESULT_MODEL_STEPS[i]];
			VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.rotation.vy = VS_RESULT_ROTATION_Y[VS_RESULT_MODEL_STEPS[i]];
			if (i == 0) {
				if (won == 0) {
					VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.location.vx = VS_D_800729CC[VS_RESULT_MODEL_STEPS[i]];
				} else {
					VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.location.vx = VS_D_800729F8[VS_RESULT_MODEL_STEPS[i]];
				}
			}
			if (i == 4) {
				VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.location.vx = VS_RESULT_LAST_X[VS_RESULT_MODEL_STEPS[4]];
			}
		}
		setupModelMatrix(&VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data);
	}
}

void VS_renderResultModelScene(void)
{
	MATRIX lw;
	MATRIX ls;
	int32_t i;

	GsSetProjection(0x15e);
	GsWSMATRIX = VS_MODEL_SCENE_MATRIX;
	GsClearOt(0, 4, &VS_MODEL_SCENE_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
	for (i = 4; i >= 0; i--) {
		GsGetLws(VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.obj.coord2, &lw, &ls);
		GsSetLightMatrix(&lw);
		GsSetLsMatrix(&ls);
		GsSortObject4(&VS_RESULT_MODEL_OBJECTS[VS_RESULT_MODEL_INDICES[VS_WINNER_ID][i]].data.obj, &VS_MODEL_SCENE_ORDERING_TABLE[ACTIVE_FRAMEBUFFER], 9, getScratchAddr(0));
	}
	GsSortOt(&VS_MODEL_SCENE_ORDERING_TABLE[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void VS_removeResultModelScene(void)
{
	removeObject(0x19d, VS_WINNER_ID);
}

void VS_setVersusModelSceneTimer(int16_t value)
{
	VS_WIN_LOSS_DRAW_TIMER = value;
}
