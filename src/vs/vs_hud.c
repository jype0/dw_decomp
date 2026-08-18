#include <libetc.h>
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
extern uint8_t MAIN_D_801352CC[2];
extern uint8_t MAIN_D_801352CE[2];
extern uint8_t MAIN_D_801352D0[2];
extern uint8_t MAIN_D_801352D2[2];
extern uint8_t MAIN_D_801352D4[2];
extern uint8_t MAIN_D_801352D6[2];
extern uint8_t MAIN_D_801352D8[2];
extern uint8_t MAIN_D_801352DA[2];
extern int32_t VS_D_80070A60[];

extern int32_t ACTIVE_FRAMEBUFFER;
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t VIEWPORT_DISTANCE;
extern int32_t VS_D_80070ACC[];
extern GsOT VS_D_80071764[];
extern PositionData VS_D_80072754[4];

extern int16_t MAIN_D_80134B14[4];
void setupModelMatrix(PositionData *posData);

extern uint8_t MAIN_D_80134AF8[8];
extern uint8_t MAIN_D_80134B00[8];
extern uint8_t MAIN_D_80134B08[8];

extern char *MAIN_D_80134B10;
extern uint8_t *MAIN_D_801352DC;
int32_t readFile(char *path, uint8_t *buffer);

void GsSortBoxFill(GsBOXF *bp, GsOT *ot, unsigned short pri);

void VS_renderCommandMenu(void);
void VS_renderTargetCursor(void);
void VS_tickTargetCursor(void);
void VS_removeTargetCursor(uint8_t index);
void VS_shuffleBattleStartTextPieces(void);
void VS_renderBattleStartText(void);
void VS_renderBattleStartTextBurst(void);
void VS_renderNumber(int32_t a, int32_t digits, int32_t x, int32_t y, int16_t value, int32_t layer);
void VS_renderFighterHPBar(int32_t id);
void VS_renderHPBarFill(int32_t id);
void VS_renderHPBarDigits(int16_t i, int32_t id);
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
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
	} else {
		prim->r0 = MAIN_D_801352D2[n];
		prim->g0 = MAIN_D_801352D2[n];
		prim->b0 = MAIN_D_801352D2[n];
	}

	prim->u0 = 0x80;
	prim->v0 = 0x88;
	prim->u1 = 0xa5;
	prim->v1 = 0x88;
	prim->u2 = 0x80;
	prim->v2 = 0x91;
	prim->u3 = 0xa5;
	prim->v3 = 0x91;
	prim->x0 = (id == 0 ? -0x56 : 0x6e);
	prim->y0 = -0x4b;
	prim->x1 = (id == 0 ? -0x56 : 0x6e) + 0x25;
	prim->y1 = -0x4b;
	prim->x2 = (id == 0 ? -0x56 : 0x6e);
	prim->y2 = -0x42;
	prim->x3 = (id == 0 ? -0x56 : 0x6e) + 0x25;
	prim->y3 = -0x42;
	AddPrim(ACTIVE_ORDERING_TABLE->org + 8, prim++);
	GsSetWorkBase((PACKET *)prim);

	box.attribute = 0x40000000;
	if ((MAIN_D_801352D6[n] != 1) || (((uint8_t *)COMBAT_DATA_PTR + n)[0x64e] == 0xb)) {
		box.r = box.g = box.b = 0x80;
	} else {
		box.r = box.g = box.b = MAIN_D_801352D2[n];
	}

	box.w = 0x29;
	box.h = 0xb;
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
		prim->r0 = MAIN_D_801352D2[k];
		prim->g0 = MAIN_D_801352D2[k];
		prim->b0 = MAIN_D_801352D2[k];
	} else {
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
	}

	prim->u0 = MAIN_D_80134AF8[i];
	prim->v0 = 0x9d;
	prim->u1 = MAIN_D_80134AF8[i] + MAIN_D_80134B00[i];
	prim->v1 = 0x9d;
	prim->u2 = MAIN_D_80134AF8[i];
	prim->v2 = 0xac;
	prim->u3 = MAIN_D_80134AF8[i] + MAIN_D_80134B00[i];
	prim->v3 = 0xac;
	prim->x0 = (id == 0 ? MAIN_D_80134B08[i] - 0x8d : MAIN_D_80134B08[i] + 0x37);
	prim->y0 = -0x4f;
	prim->x1 = MAIN_D_80134B00[i] + ((id == 0 ? MAIN_D_80134B08[i] - 0x8d : MAIN_D_80134B08[i] + 0x37));
	prim->y1 = -0x4f;
	prim->x2 = (id == 0 ? MAIN_D_80134B08[i] - 0x8d : MAIN_D_80134B08[i] + 0x37);
	prim->y2 = -0x40;
	prim->x3 = MAIN_D_80134B00[i] + ((id == 0 ? MAIN_D_80134B08[i] - 0x8d : MAIN_D_80134B08[i] + 0x37));
	prim->y3 = -0x40;
	AddPrim(ACTIVE_ORDERING_TABLE->org + 8, prim++);
	GsSetWorkBase((PACKET *)prim);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_addFighterStatusBars);

void VS_tickFighterStatusBars(void)
{
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_hud", VS_renderFighterStatusBars);

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
		GsLinkObject4((u_long)(MAIN_D_801352DC + 0xc), &VS_D_80072754[i].obj, i);
		GsInitCoordinate2(NULL, &VS_D_80072754[i].posMatrix);
		VS_D_80072754[i].obj.attribute = 0;
		VS_D_80072754[i].obj.coord2 = &VS_D_80072754[i].posMatrix;
	}

	for (i = 0; i < 4; i++) {
		VS_D_80072754[i].scale.vx = 0x1000;
		VS_D_80072754[i].scale.vy = 0x1000;
		VS_D_80072754[i].scale.vz = 0x1000;
		VS_D_80072754[i].rotation.vx = 0;
		VS_D_80072754[i].rotation.vy = 0;
		VS_D_80072754[i].rotation.vz = 0;
		VS_D_80072754[i].location.vx = 0x3e8;
		VS_D_80072754[i].location.vy = 0x78;
		VS_D_80072754[i].location.vz = 0x280;
		setupModelMatrix(&VS_D_80072754[i]);
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
			if (VS_D_80072754[i].location.vx != MAIN_D_80134B14[i]) {
				VS_D_80072754[i].location.vx -= 200;
				if (VS_D_80072754[i].location.vx < MAIN_D_80134B14[i]) {
					VS_D_80072754[i].location.vx = MAIN_D_80134B14[i];
				}
			}
		}
		setupModelMatrix(&VS_D_80072754[i]);
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
		GsGetLws(VS_D_80072754[i].obj.coord2, &lw, &ls);
		GsSetLightMatrix(&lw);
		GsSetLsMatrix(&ls);
		GsSortObject4(&VS_D_80072754[i].obj, &VS_D_80071764[ACTIVE_FRAMEBUFFER], 9,
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
