#include <libgpu.h>
#include <libgs.h>

#include <dw/combat.h>
#include <dw/math.h>
#include <dw/types.h>
#include <dw/vs.h>

extern int8_t MAIN_D_801352C4;
extern int16_t VS_D_800707F4[][2];
extern int16_t VS_D_800707F6[][2];
extern char VS_D_80071B38[];
extern int8_t GAME_STATE;
extern int16_t MAIN_D_801352E0;
extern uint8_t MAIN_D_801352E2;
extern int32_t MAIN_D_801352C8;
extern int16_t MAIN_D_801352AC[2];
extern int16_t MAIN_D_801352B0[2];
extern uint8_t MAIN_D_801352B4[2];
extern uint8_t MAIN_D_801352B6[2];
extern int8_t MAIN_D_801352B8[2];
extern int8_t MAIN_D_80134AC8[2];
extern GsOT *ACTIVE_ORDERING_TABLE;

void VS_tickCommandMenu(uint8_t i);
void VS_renderCommandMenu(void);
void VS_renderTargetCursor(void);
void VS_tickTargetCursor(void);
void VS_removeTargetCursor(uint8_t index);
void VS_shuffleBattleStartTextPieces(void);
void VS_renderBattleStartText(void);
void VS_renderBattleStartTextBurst(void);
void VS_renderNumber(int32_t a, int32_t digits, int32_t x, int32_t y, int16_t value, int32_t layer);
void VS_renderFighterHPBar(void);
void VS_renderHPBarFill(void);
void VS_renderHPBarDigits(void);
void VS_tickFighterStatusBars(void);
void VS_renderFighterStatusBars(void);
void VS_tickVersusModelScene(void);
void VS_renderVersusModelScene(void);
void VS_loadStageModels(void);
void VS_addResultModelScene(void);
void VS_tickResultModelScene(void);
void VS_renderResultModelScene(void);
void VS_setVersusModelSceneTimer(int16_t value);
void setEntityTextDigit(POLY_FT4 *poly, int32_t x, int32_t y);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uPos, int32_t vPos, int32_t width, int32_t height);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount, int32_t *buf);
void swapByte(char *a, char *b);
void addObject(int32_t objectId, int32_t instanceId, void *tick, void *render);
void removeObject(int32_t objectId, int32_t instanceId);

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

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_renderCommandMenu);

void VS_removeCommandMenu(int32_t i)
{
	MAIN_D_80134AC8[i] = 0;
	removeObject(0x198, i);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_addTargetCursor);

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_renderTargetCursor);

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_tickTargetCursor);

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
		swapByte(&((char (*)[20])VS_D_80071B38)[i][0x11], &((char (*)[20])VS_D_80071B38)[random(0x9b)][0x11]);
	}
}

void VS_initializeBattleStartText(void)
{
	char (*p)[20];
	int32_t sgn;
	int32_t i;
	int32_t r;
	int32_t t;

	MAIN_D_801352C4 = 0;
	MAIN_D_801352C8 = 0;
	p = (char (*)[20])VS_D_80071B38;
	for (i = 0; i < 0x9b; i++, p++) {
		(*p)[0x11] = i;
		(*p)[0x12] = 0x18;
		(*p)[0x13] = random(3);
	}

	VS_shuffleBattleStartTextPieces();

	p = (char (*)[20])VS_D_80071B38;
	for (i = 0; i < 0x9b; i++, p++) {
		if (random(2) == 1) {
			sgn = 1;
		} else {
			sgn = -1;
		}
		((int16_t *)*p)[5] = VS_D_800707F6[i][0];
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
		t = VS_D_800707F6[i][0];
		((int16_t *)*p)[7] = (r + 8) * t / 8;
		((int16_t *)*p)[0] = 0;
		((int16_t *)*p)[1] = 0;
		((int16_t *)*p)[2] = 0;
	}

	addObject(0x1a6, 0, NULL, VS_renderBattleStartText);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_renderBattleStartText);

void VS_removeBattleStartText(void)
{
	removeObject(0x1a6, 0);
}

void VS_initializeBattleStartTextBurst(void)
{
	MAIN_D_801352C8 = 0;
	addObject(0x1a6, 0, NULL, VS_renderBattleStartTextBurst);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_renderBattleStartTextBurst);

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
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
		setUVDataPolyFT4(prim, buf[i] * 7, 172, 7, 11);
		setPosDataPolyFT4(prim, x + ((((int32_t)width - 1) - i) * 7), y, 7, 11);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_renderFighterHPBar);

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_renderHPBarFill);

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_renderHPBarDigits);

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_addFighterStatusBars);

void VS_tickFighterStatusBars(void)
{
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_renderFighterStatusBars);

void VS_removeFighterStatusBars(int32_t i)
{
	removeObject(0x19c, i);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_loadVersusSceneModel);

void VS_addVersusModelScene(void)
{
	MAIN_D_801352E0 = 0;
	addObject(0x19d, 0, VS_tickVersusModelScene, VS_renderVersusModelScene);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_tickVersusModelScene);

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_renderVersusModelScene);

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

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_loadStageModels);

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_addResultModelScene);

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_tickResultModelScene);

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_renderResultModelScene);

void VS_removeResultModelScene(void)
{
	removeObject(0x19d, MAIN_D_801352E2);
}

void VS_setVersusModelSceneTimer(int16_t value)
{
	MAIN_D_801352E0 = value;
}
