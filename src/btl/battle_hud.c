#include <stdio.h>
#include <stdlib.h>

#include <libcd.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <mwinline_n.h>

#include <dw/aabb.h>
#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/graphics.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/sound.h>

#include "common.h"

extern GsSPRITE BTL_D_80073E54;
extern GsSPRITE BTL_D_80073E78;
extern char BTL_END_BOX_TEXTBUFFER[];
extern int16_t MAIN_D_8013509C;
extern uint8_t *MAIN_D_801350A4;
extern uint16_t MAIN_D_801350A8;
extern uint16_t MAIN_D_801350AA;
extern uint16_t MAIN_D_801350AC;
extern int16_t BTL_D_80073E9C[];
extern char *BTL_D_80072E34[];
extern int16_t MAIN_D_80135090[2];
extern int32_t MAIN_D_801350C0;
extern int16_t BTL_D_80073014[][2];
extern int16_t BTL_D_80073016[][2];
extern int8_t MAIN_D_801350BC;
extern StatsGains STATS_GAINS;
extern char BTL_D_80072EF8[];
extern char BTL_D_80072F44[];
extern char BTL_D_80072F58[];
extern char BTL_D_80072F70[];
extern char BTL_D_80072F90[];
extern char BTL_D_80072FA4[];
extern char BTL_D_80072FBC[];
extern char BTL_D_80072FD8[];
extern char MAIN_D_8013474C[8];
extern char MAIN_D_80134750[8];
extern char MAIN_D_80134754[8];
extern char MAIN_D_80134758[8];
extern char MAIN_D_8013475C[8];
extern char ITEM_PARA[][32];
extern char MAIN_D_80134744[8];
extern char BTL_D_80072F18[];
extern char BTL_D_800742A0[];
extern char BTL_D_80072FE4[];
extern char MAIN_D_80134740[8];
extern char BTL_D_80072F2C[];
extern int16_t MAIN_D_8013509E;
extern int16_t MAIN_D_801350A0;
extern int16_t MAIN_D_801350A2;
extern uint8_t MAIN_D_80134764[8];
extern uint8_t MAIN_D_8013476C[8];
extern uint8_t MAIN_D_80134774[8];
extern int8_t MAIN_D_80135094;
extern uint8_t MAIN_D_801350C4;
extern uint8_t MAIN_D_801350C5;
extern uint8_t MAIN_D_801350C6;
extern uint8_t MAIN_D_801350C7;
extern uint8_t MAIN_D_801350C8;
extern uint8_t MAIN_D_801350C9;
extern uint8_t MAIN_D_801350CA;
extern uint8_t MAIN_D_801350CB;
extern int32_t BTL_D_80073290[12];

void removeObject(int32_t objectId, int32_t instanceId);
void addObject(int32_t objectId, int32_t instanceId, void *tick, void *render);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
int16_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void BTL_tickDeathCountdown(void);
void BTL_initializeFinisherChargeup(void);
void BTL_renderFinisherChargeup(void);
void BTL_tickFinisherChargeup(void);
void BTL_renderDeathCountdown(void);
void BTL_initializeBattleEndText(int16_t arg0, int16_t arg1, RECT *arg2);
void BTL_renderBattleStartText(void);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount, int32_t *buf);
void setEntityTextDigit(POLY_FT4 *poly, int32_t x, int32_t y);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t u, int32_t v, int32_t w, int32_t h);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t x, int32_t y, int32_t w, int32_t h);
int32_t convertAsciiToJis(uint8_t input);
int32_t swapShortBytes(int32_t input);
int32_t drawGlyph(uint16_t glyph, int32_t x, int32_t y);
void BTL_drawBattleEndText(int32_t a);
char *strcpy(char *dst, const char *src);
void swapByte(char *a, char *b);
char *strcat(char *dst, const char *src);
void BTL_renderBattleStartTextBurst(void);
void BTL_scrollBattleEndText(void);
void BTL_appendItemDroppedText(int32_t *p);
void BTL_appendInjuredText(char *name);
void BTL_appendCommandLearnedText(void);
void BTL_appendMPBonusText(void);
void BTL_appendMoveLearnedText(int32_t move);
void BTL_tickBattleEndText(void);
void BTL_renderBattleEndText(int32_t n);
int32_t BTL_isEndBoxTextFinished(void);
void BTL_shuffleBattleStartTextPieces(void);
void BTL_renderNumber(int32_t a, int32_t digits, int32_t x, int32_t y, int16_t value, int32_t layer);
void GsSortBoxFill(GsBOXF *bp, GsOT *ot, unsigned short pri);
void BTL_renderFinisherReadyIcon(void);
void BTL_renderPartnerStatusBars(int16_t idx);
void BTL_tickPartnerStatusBars(void);
void BTL_renderFinisherGauge(int32_t idx);
void BTL_renderFinisherGaugeSegment(int16_t i, int32_t idx);

static void *battle_hud_functions[] = {
	BTL_removePartnerStatusBars,
	BTL_renderPartnerStatusBars,
	BTL_tickPartnerStatusBars,
	BTL_initializePartnerStatusBars,
	BTL_renderFinisherGaugeSegment,
	BTL_renderFinisherReadyIcon,
	BTL_renderFinisherGauge,
	BTL_renderNumber,
	BTL_isBattleStartTextFinished,
	BTL_removeBattleStartTextBurst,
	BTL_renderBattleStartTextBurst,
	BTL_initializeBattleStartTextBurst,
	BTL_removeBattleStartText,
	BTL_renderBattleStartText,
	BTL_initializeBattleStartText,
	BTL_shuffleBattleStartTextPieces,
	BTL_isEndBoxTextFinished,
	BTL_renderBattleEndText,
	BTL_tickBattleEndText,
	BTL_scrollBattleEndText,
	BTL_drawBattleEndText,
	BTL_appendMoveLearnedText,
	BTL_appendMPBonusText,
	BTL_appendCommandLearnedText,
	BTL_appendInjuredText,
	BTL_appendItemDroppedText,
	BTL_initializeBattleEndText,
	BTL_removeDeathCountdown,
	BTL_renderDeathCountdown,
	BTL_tickDeathCountdown,
	BTL_addDeathCountdown,
	BTL_initializeDeathCountdown,
	BTL_removeFinisherChargeup,
	BTL_renderFinisherChargeup,
	BTL_tickFinisherChargeup,
	BTL_initializeFinisherChargeup,
	BTL_drawHoveredCommandName,
};

void BTL_drawHoveredCommandName(void)
{
	RECT area;
	uint8_t cmd;
	int16_t tech;

	area.x = 0;
	area.y = 0xd8;
	area.w = 0x90;
	area.h = 0xc;
	clearTextSubArea(&area);
	cmd = COMBAT_DATA_PTR->player.availableCommands[0][COMBAT_DATA_PTR->player.hoveredCommand[0]];
	if ((cmd >= 8) && (cmd < 0xc)) {
		tech = entityGetTechFromAnim(ENTITY_TABLE[1], PARTNER_ENTITY.digimonEntity.stats.base.moves[cmd - 8]);
		drawString(MOVE_NAMES[tech], 0, 0xd8);
	} else {
		drawString(BTL_D_80072E34[cmd - 1], 0, 0xd8);
	}

	renderString(0, -0x8c, -0x42, 0x90, 0xc, 0, 0xd8, 7, 1);
}

void BTL_initializeFinisherChargeup(void)
{
	SVECTOR v;
	VECTOR *loc;
	int32_t d;
	int32_t c;

	COMBAT_DATA_PTR->player.finisherChargeup[0] = 0;
	COMBAT_DATA_PTR->player.remainingChargeupTime[0] = 0x50;
	COMBAT_DATA_PTR->fighter[0].finisherProgress = 0;
	if (COMBAT_DATA_PTR->player.hoveredCommand[0] == 0) {
		COMBAT_DATA_PTR->player.hoveredCommand[0] = COMBAT_DATA_PTR->player.numCommands[0] - 1;
	}

	COMBAT_DATA_PTR->player.currentCommand[0] = 3;
	COMBAT_DATA_PTR->player.bufferedCommand[0] = 3;
	GsSetLsMatrix(&GsWSMATRIX);
	loc = &ENTITY_TABLE[1]->posData->location;
	v.vx = loc->vx;
	v.vy = -DIGIMON_DATA[ENTITY_TABLE[1]->type].height - 0x64;
	v.vz = loc->vz;
	gte_ldv0(&v);
	gte_rtps();
	gte_stsxy((int32_t *)MAIN_D_80135090);
	c = MAIN_D_80135090[0];
	d = 0xb7 - DRAWING_OFFSET_X;
	MAIN_D_80135090[0] = c - d;
	c = MAIN_D_80135090[1];
	d = 0x8c - DRAWING_OFFSET_Y;
	MAIN_D_80135090[1] = c - d;
	if (MAIN_D_80135090[0] >= 0x65) {
		MAIN_D_80135090[0] = 0x64;
	}

	if (MAIN_D_80135090[0] < -0x64) {
		MAIN_D_80135090[0] = -0x64;
	}

	if (MAIN_D_80135090[1] >= 0x65) {
		MAIN_D_80135090[1] = 0x64;
	}

	if (MAIN_D_80135090[1] < -0x64) {
		MAIN_D_80135090[1] = -0x64;
	}

	addObject(0x19a, 0, BTL_tickFinisherChargeup, BTL_renderFinisherChargeup);
}

void BTL_tickFinisherChargeup(void)
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

void BTL_renderFinisherChargeup(void)
{
	POLY_FT4 prim;
	int32_t i;
	int32_t x;
	int16_t bars;

	SetPolyFT4(&prim);
	prim.tpage = 0x1f;
	prim.clut = GetClut(0x110, 0x1f2);
	prim.r0 = 0x80;
	prim.g0 = 0x80;
	prim.b0 = 0x80;
	prim.u0 = 0x58;
	prim.v0 = 0xe0;
	prim.u1 = 0x86;
	prim.v1 = 0xe0;
	prim.u2 = 0x58;
	prim.v2 = 0xec;
	prim.u3 = 0x86;
	prim.v3 = 0xec;
	prim.x0 = MAIN_D_80135090[0];
	prim.y0 = MAIN_D_80135090[1];
	prim.x1 = MAIN_D_80135090[0] + 0x2e;
	prim.y1 = MAIN_D_80135090[1];
	prim.x2 = MAIN_D_80135090[0];
	prim.y2 = MAIN_D_80135090[1] + 0xc;
	prim.x3 = MAIN_D_80135090[0] + 0x2e;
	prim.y3 = MAIN_D_80135090[1] + 0xc;
	GsSortPoly(&prim, ACTIVE_ORDERING_TABLE, 7);
	bars = COMBAT_DATA_PTR->player.finisherChargeup[0] / 8;
	prim.u0 = 0x88;
	prim.v0 = 0xe0;
	prim.u1 = 0x8c;
	prim.v1 = 0xe0;
	prim.u2 = 0x88;
	prim.v2 = 0xe6;
	prim.u3 = 0x8c;
	prim.v3 = 0xe6;
	for (i = 0, x = 0; i < bars; i++, x += 4) {
		prim.x0 = MAIN_D_80135090[0] + 3 + x;
		prim.y0 = MAIN_D_80135090[1] + 3;
		prim.x1 = (int32_t)(MAIN_D_80135090[0] + 3 + x) + 4;
		prim.y1 = MAIN_D_80135090[1] + 3;
		prim.x2 = MAIN_D_80135090[0] + 3 + x;
		prim.y2 = MAIN_D_80135090[1] + 9;
		prim.x3 = (int32_t)(MAIN_D_80135090[0] + 3 + x) + 4;
		prim.y3 = MAIN_D_80135090[1] + 9;
		GsSortPoly(&prim, ACTIVE_ORDERING_TABLE, 7);
	}

	if (COMBAT_DATA_PTR->player.remainingChargeupTime[0] == 0) {
		BTL_removeFinisherChargeup();
	}
}

void BTL_removeFinisherChargeup(void)
{
	if (COMBAT_DATA_PTR->player.remainingChargeupTime[0] != -1) {
		removeObject(0x19a, 0);
		COMBAT_DATA_PTR->player.remainingChargeupTime[0] = -1;
	}
}

void BTL_initializeDeathCountdown(void)
{
	GsSPRITE *sp;

	BTL_D_80073E54.attribute = 0;
	BTL_D_80073E54.tpage = 0x1e;
	BTL_D_80073E54.u = 0x32;
	BTL_D_80073E54.v = 0x80;
	BTL_D_80073E54.mx = 0x14;
	BTL_D_80073E54.my = 0x14;
	BTL_D_80073E54.cx = 0x100;
	BTL_D_80073E54.r = 0x80;
	BTL_D_80073E54.g = 0x80;
	BTL_D_80073E54.b = 0x80;
	BTL_D_80073E78 = BTL_D_80073E54;
	sp = &BTL_D_80073E78;
	do {
		sp->mx = 8;
		sp->my = 8;
		sp->w = 0x10;
		sp->h = 0x10;
	} while (0);
	BTL_D_80073E9C[1] = -1;
}

void BTL_addDeathCountdown(Entity *entity)
{
	int16_t pos[2];
	GsSPRITE *sp;
	int16_t py;

	if (BTL_D_80073E9C[1] == -1) {
		BTL_D_80073E9C[1] = 0;
		BTL_D_80073E9C[0] = 0;
		getEntityScreenPos(entity, 1, pos);
		if (pos[0] >= 0x8d) {
			pos[0] = 0x8c;
		}
		if (pos[0] < -0x8c) {
			pos[0] = -0x8c;
		}
		if (pos[1] >= 0x65) {
			pos[1] = 0x64;
		}
		if (pos[1] < -0x64) {
			pos[1] = -0x64;
		}
		sp = &BTL_D_80073E78;
		do {
			BTL_D_80073E54.cy = 0x1ed;
			sp->cy = 0x1ed;
			BTL_D_80073E54.rotate = 0;
			sp->rotate = 0;
			BTL_D_80073E54.x = pos[0];
			sp->x = pos[0];
			py = pos[1];
			BTL_D_80073E54.y = py;
			sp->y = py;
		} while (0);
		addObject(0x197, 0, BTL_tickDeathCountdown, BTL_renderDeathCountdown);
	}
}

void BTL_tickDeathCountdown(void)
{
	GsSPRITE *base;
	GsSPRITE *spin;
	int32_t frame;
	int32_t scale;
	uint8_t vpos;

	if (TAMER_ITEM[2] == 0xff) {
		base = &BTL_D_80073E54;
		spin = &BTL_D_80073E78;
		do {
			BTL_D_80073E9C[1]++;
			if ((BTL_D_80073E9C[1] % 31) == 0) {
				BTL_D_80073E9C[0] = 0;
			}
			BTL_D_80073E9C[0]++;
			frame = (BTL_D_80073E9C[1] + 0x1d) / 30;
			if (frame < 5) {
				spin->u = (MAIN_D_80134730 + frame)[7];
				vpos = (MAIN_D_80134730 + frame)[0xb];
			} else {
				spin->u = 0x58;
				vpos = 0x80;
			}
			spin->v = vpos;
			if ((BTL_D_80073E9C[0] > 0) && (BTL_D_80073E9C[0] < 0x15)) {
				if (BTL_D_80073E9C[0] == 1) {
					base->scaley = 0x1000;
					base->scalex = 0x1000;
					base->rotate = 0;
					spin->rotate = 0;
				}
				if ((BTL_D_80073E9C[0] >= 2) && (BTL_D_80073E9C[0] < 5)) {
					scale = spin->scalex + 0x199;
					spin->scalex = scale;
					spin->scaley = scale;
				} else if ((BTL_D_80073E9C[0] >= 5) && (BTL_D_80073E9C[0] < 8)) {
					scale = spin->scalex - 0x199;
					spin->scalex = scale;
					spin->scaley = scale;
				} else {
					spin->scaley = 0x1000;
					spin->scalex = 0x1000;
				}
				if ((BTL_D_80073E9C[0] % 3) == 0) {
					base->cy = base->cy + 1;
					if (base->cy >= 0x1f0) {
						base->cy = 0x1ed;
					}
				}
			} else {
				scale = ((0x64 - ((BTL_D_80073E9C[0] - 0x14) * 5)) << 0xc) / 100;
				base->scalex = scale;
				base->scaley = scale;
				spin->scaley = scale;
				spin->scalex = scale;
				base->rotate = base->rotate + 0x40000;
				spin->rotate = base->rotate;
			}
			if ((spin->scalex >= 0x2000) || (spin->scaley >= 0x2000)) {
				base->w = 0x27;
				base->h = 0x27;
			} else {
				base->w = 0x28;
				base->h = 0x28;
			}
		} while (0);
	}
}

void BTL_renderDeathCountdown(void)
{
	GsSortSprite(&BTL_D_80073E78, ACTIVE_ORDERING_TABLE, 7);
	GsSortSprite(&BTL_D_80073E54, ACTIVE_ORDERING_TABLE, 7);
	if (BTL_D_80073E9C[1] >= 0x96) {
		BTL_removeDeathCountdown();
	}
}

void BTL_removeDeathCountdown(void)
{
	if (BTL_D_80073E9C[1] != -1) {
		removeObject(0x197, 0);
		BTL_D_80073E9C[1] = -1;
	}
}

void BTL_initializeBattleEndText(int16_t arg0, int16_t arg1, RECT *arg2)
{
	MAIN_D_80135098 = 0;
	*(RECT *)&MAIN_D_8013509C = *arg2;
	MAIN_D_801350A4 = (uint8_t *)BTL_END_BOX_TEXTBUFFER;
	MAIN_D_801350A8 = 0;
	MAIN_D_801350AA = arg0;
	MAIN_D_801350AC = arg0;
	MAIN_D_801350AE = 0;
	MAIN_D_801350B0 = 0;
	MAIN_D_801350B2 = 0;
	BTL_END_BOX_TEXTBUFFER[0] = 0;
	MAIN_D_801350B4 = arg1;
	MAIN_D_801350B6 = 0x3c;
	MAIN_D_801350B8 = 0x3c;
}

void BTL_appendItemDroppedText(int32_t *p)
{
	strcat(BTL_END_BOX_TEXTBUFFER, MAIN_D_80134740);
	strcat(BTL_END_BOX_TEXTBUFFER, DIGIMON_DATA[p[0]].name);
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072F18);
	strcat(BTL_END_BOX_TEXTBUFFER, ITEM_PARA[DIGIMON_DATA[p[0]].dropItem]);
	strcat(BTL_END_BOX_TEXTBUFFER, MAIN_D_80134744);
	MAIN_D_801350B0 += 2;
}

void BTL_appendInjuredText(char *name)
{
	strcat(BTL_END_BOX_TEXTBUFFER, MAIN_D_80134740);
	strcat(BTL_END_BOX_TEXTBUFFER, name);
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072F2C);
	MAIN_D_801350B0 += 2;
}

void BTL_appendCommandLearnedText(void)
{
	int16_t old;
	int16_t total;

	total = INITIAL_COMBAT_STATS[0].brains + STATS_GAINS.brains;
	old = INITIAL_COMBAT_STATS[0].brains;
	if (total < 0x64) {
		return;
	}

	if (total >= 0x1f4) {
		if (old < 0x1f4) {
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072F44);
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072EF8);
			MAIN_D_801350B0 += 2;
		}
	} else if (total >= 0x190) {
		if (old < 0x190) {
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072F58);
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072EF8);
			MAIN_D_801350B0 += 2;
		}
	} else if (total >= 0x12c) {
		if (old < 0x12c) {
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072F70);
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072EF8);
			MAIN_D_801350B0 += 3;
		}
	} else if (total >= 0xc8) {
		if (old < 0xc8) {
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072F90);
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072EF8);
			MAIN_D_801350B0 += 2;
		}
	} else {
		if (old < 0x64) {
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072FA4);
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072EF8);
			MAIN_D_801350B0 += 2;
		}
	}
}

void BTL_appendMPBonusText(void)
{
	char buf[8];
	int16_t old;
	int16_t total;

	total = INITIAL_COMBAT_STATS[0].brains + STATS_GAINS.brains;
	old = INITIAL_COMBAT_STATS[0].brains;
	if (total < 0x2bc) {
		return;
	}

	buf[0] = 0;
	if (total >= 0x3e7) {
		if (old < 0x3e7) {
			strcpy(buf, MAIN_D_8013474C);
		}
	} else if (total >= 0x384) {
		if (old < 0x384) {
			strcpy(buf, MAIN_D_80134750);
		}
	} else if (total >= 0x320) {
		if (old < 0x320) {
			strcpy(buf, MAIN_D_80134754);
		}
	} else {
		if (old < 0x2bc) {
			strcpy(buf, MAIN_D_80134758);
		}
	}

	if (buf[0] == 0) {
		return;
	}

	strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072FBC);
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072FD8);
	strcat(BTL_END_BOX_TEXTBUFFER, buf);
	strcat(BTL_END_BOX_TEXTBUFFER, MAIN_D_8013475C);
	MAIN_D_801350B0 += 4;
}

void BTL_appendMoveLearnedText(int32_t move)
{
	strcat(BTL_END_BOX_TEXTBUFFER, MAIN_D_80134740);
	strcat(BTL_END_BOX_TEXTBUFFER, MOVE_NAMES[move]);
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_D_80072FE4);
	MAIN_D_801350B0 += 2;
}

void BTL_drawBattleEndText(int32_t flag)
{
	uint8_t c;
	uint16_t w;

	while (flag) {
		if (*MAIN_D_801350A4 == '#') {
			MAIN_D_801350A4++;
			switch (*MAIN_D_801350A4) {
			case 'C':
				MAIN_D_801350A4++;
				setTextColor(*MAIN_D_801350A4);
				MAIN_D_801350A4++;
				break;
			case 'W':
				MAIN_D_801350B8 = MAIN_D_801350B6;
				/* fall through */
			case 'R':
				MAIN_D_801350A8 = 0;
				MAIN_D_801350A4++;
				MAIN_D_801350AA += 0xc;
				MAIN_D_801350AE++;
				MAIN_D_801350B2++;
				return;
			}
			continue;
		}
		c = *MAIN_D_801350A4;
		if (c == '0') {
			return;
		}
		w = drawGlyph((uint16_t)swapShortBytes((uint16_t)convertAsciiToJis(c)), MAIN_D_801350A8, MAIN_D_801350AA);
		MAIN_D_801350A8 += w;
		MAIN_D_801350A4++;
		return;
	}
}

void BTL_scrollBattleEndText(void)
{
	MAIN_D_801350AC += 0xc;
	MAIN_D_801350AE--;
}

void BTL_tickBattleEndText(void)
{
	if (MAIN_D_801350B8 != 0) {
		MAIN_D_801350B8 -= 1;
	}

	if (BTL_END_BOX_TEXTBUFFER[0] == 0) {
		if (((POLLED_INPUT == 0x40) || (POLLED_INPUT == 0x10)) && (POLLED_INPUT != POLLED_INPUT_PREVIOUS)) {
			MAIN_D_801350B8 = 0;
		}
		return;
	}

	if (MAIN_D_801350AE == MAIN_D_801350B4) {
		if (MAIN_D_801350B8 == 0) {
			BTL_scrollBattleEndText();
		}
	} else {
		BTL_drawBattleEndText(MAIN_D_80135098);
		if (MAIN_D_801350B8 != 0) {
			MAIN_D_80135098 = 0;
		}
	}

	if (((POLLED_INPUT == 0x40) || (POLLED_INPUT == 0x10)) && (POLLED_INPUT != POLLED_INPUT_PREVIOUS)) {
		if (MAIN_D_801350B8 != 0) {
			MAIN_D_801350B8 = 0;
		} else {
			MAIN_D_80135098 = 1;
		}
	}
}

void BTL_renderBattleEndText(int32_t n)
{
	renderString(0, MAIN_D_8013509C, MAIN_D_8013509E, MAIN_D_801350A0, MAIN_D_801350A2, 0, MAIN_D_801350AC, 6 - n, 0);
}

int32_t BTL_isEndBoxTextFinished(void)
{
	if ((MAIN_D_801350B8 == 0) && (MAIN_D_801350B0 == MAIN_D_801350B2)) {
		return 1;
	}

	return 0;
}

void BTL_shuffleBattleStartTextPieces(void)
{
	int32_t i;

	for (i = 0; i < 0x9b; i++) {
		swapByte(&((char (*)[20])BTL_D_800742A0)[i][0x11], &((char (*)[20])BTL_D_800742A0)[random(0x9b)][0x11]);
	}
}

void BTL_initializeBattleStartText(void)
{
	char (*p)[20];
	int32_t sgn;
	int32_t i;
	int32_t r;
	int32_t t;

	MAIN_D_801350BC = 0;
	MAIN_D_801350C0 = 0;
	p = (char (*)[20])BTL_D_800742A0;
	for (i = 0; i < 0x9b; i++, p++) {
		(*p)[0x11] = i;
		(*p)[0x12] = 0x18;
		(*p)[0x13] = random(3);
	}

	BTL_shuffleBattleStartTextPieces();

	p = (char (*)[20])BTL_D_800742A0;
	for (i = 0; i < 0x9b; i++, p++) {
		if (random(2) == 1) {
			sgn = 1;
		} else {
			sgn = -1;
		}
		((int16_t *)*p)[5] = BTL_D_80073016[i][0];
		(*p)[0x10] = -sgn * ((random(3) + 1) << 5);
		if ((0 <= i) && (i < 0x33)) {
			((int16_t *)*p)[4] = (sgn * 500) + random(100) - 50;
		} else if ((0x33 <= i) && (i < 0x65)) {
			((int16_t *)*p)[4] = (sgn * 600) + random(100) - 50;
		} else {
			((int16_t *)*p)[4] = (sgn * 700) + random(100) - 50;
		}
		r = random(5);
		t = BTL_D_80073014[i][0];
		((int16_t *)*p)[6] = (r + 8) * t / 8;
		t = BTL_D_80073016[i][0];
		((int16_t *)*p)[7] = (r + 8) * t / 8;
		((int16_t *)*p)[0] = 0;
		((int16_t *)*p)[1] = 0;
		((int16_t *)*p)[2] = 0;
	}

	addObject(0x1a6, 0, NULL, BTL_renderBattleStartText);
}

INCLUDE_ASM("asm/btl/nonmatchings/battle_hud", BTL_renderBattleStartText);

void BTL_removeBattleStartText(void)
{
	removeObject(0x1a6, 0);
}

void BTL_initializeBattleStartTextBurst(void)
{
	MAIN_D_801350C0 = 0;
	addObject(0x1a6, 0, NULL, BTL_renderBattleStartTextBurst);
}

INCLUDE_ASM("asm/btl/nonmatchings/battle_hud", BTL_renderBattleStartTextBurst);

void BTL_removeBattleStartTextBurst(void)
{
	removeObject(0x1a6, 0);
}

int32_t BTL_isBattleStartTextFinished(void)
{
	return MAIN_D_801350C0;
}

void BTL_renderNumber(int32_t a, int32_t digits, int32_t x, int32_t y, int16_t value, int32_t layer)
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

void BTL_renderFinisherGauge(int32_t idx)
{
	FighterData *f;
	int32_t hp;
	int32_t i;

	if (MAIN_D_801350C4 != 7) {
		return;
	}

	f = COMBAT_DATA_PTR->fighter;
	hp = f[idx].finisherProgress * 6 / f[idx].finisherGoal;

	if (MAIN_D_801350C8 != hp) {
		MAIN_D_801350C6 = 0;
		MAIN_D_801350C8 = hp;
	}

	MAIN_D_801350C7 = BTL_D_80073290[MAIN_D_801350C6];
	if (MAIN_D_801350C6 < 0xB) {
		MAIN_D_801350C6++;
	}

	if (hp == 6) {
		if (MAIN_D_801350C5 < 0xA) {
			MAIN_D_801350C5++;
		}

		if (MAIN_D_801350C5 >= 3) {
			hp++;
		}

		if (MAIN_D_801350C5 >= 5) {
			hp++;
			BTL_renderFinisherReadyIcon();

			if (MAIN_D_801350C5 == 0xA) {
				MAIN_D_801350C9 = 1;
				MAIN_D_801350C6 %= 0xB;
			}
		}
	}

	for (i = 0; i < hp; i++) {
		BTL_renderFinisherGaugeSegment(i, idx);
	}
}

void BTL_renderFinisherReadyIcon(void)
{
	GsBOXF box;
	POLY_FT4 *prim;

	prim = (POLY_FT4 *)GsGetWorkBase();
	setEntityTextDigit(prim, 256, 482);
	if ((MAIN_D_801350C9 != 1) || (((uint8_t *)COMBAT_DATA_PTR)[0x64e] == 0xb)) {
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
	} else {
		prim->r0 = MAIN_D_801350C7;
		prim->g0 = MAIN_D_801350C7;
		prim->b0 = MAIN_D_801350C7;
	}

	prim->u0 = 0x80;
	prim->v0 = 0x88;
	prim->u1 = 0xa5;
	prim->v1 = 0x88;
	prim->u2 = 0x80;
	prim->v2 = 0x91;
	prim->u3 = 0xa5;
	prim->v3 = 0x91;
	prim->x0 = 0x6e;
	prim->y0 = -0x4b;
	prim->x1 = 0x93;
	prim->y1 = -0x4b;
	prim->x2 = 0x6e;
	prim->y2 = -0x42;
	prim->x3 = 0x93;
	prim->y3 = -0x42;
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0xa, prim++);
	GsSetWorkBase((PACKET *)prim);

	box.attribute = 0x40000000;
	if ((MAIN_D_801350C9 != 1) || (((uint8_t *)COMBAT_DATA_PTR)[0x64e] == 0xb)) {
		box.r = box.g = box.b = 0x80;
	} else {
		box.r = box.g = box.b = MAIN_D_801350C7;
	}

	box.w = 0x29;
	box.h = 0xb;
	box.x = 0x6c;
	box.y = -0x4c;
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 0xa);
}

void BTL_renderFinisherGaugeSegment(int16_t i, int32_t idx)
{
	struct EfeFighter {
		int32_t unk0[6];
		int16_t goal;
		int16_t progress;
		int8_t unk1C[0x14c];
	};
	POLY_FT4 *prim;
	int32_t hp;
	struct EfeFighter *f;

	f = (struct EfeFighter *)COMBAT_DATA_PTR;
	hp = f[idx].progress * 6 / f[idx].goal;
	prim = (POLY_FT4 *)GsGetWorkBase();
	setEntityTextDigit(prim, 256, 492);
	if ((((hp - 1) == i) || (MAIN_D_801350C9 == 1)) && (((uint8_t *)COMBAT_DATA_PTR)[0x64e] != 0xb)) {
		prim->r0 = MAIN_D_801350C7;
		prim->g0 = MAIN_D_801350C7;
		prim->b0 = MAIN_D_801350C7;
	} else {
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
	}

	prim->u0 = MAIN_D_80134764[i];
	prim->v0 = 0x9d;
	prim->u1 = MAIN_D_80134764[i] + MAIN_D_8013476C[i];
	prim->v1 = 0x9d;
	prim->u2 = MAIN_D_80134764[i];
	prim->v2 = 0xac;
	prim->u3 = MAIN_D_80134764[i] + MAIN_D_8013476C[i];
	prim->v3 = 0xac;
	prim->x0 = MAIN_D_80134774[i] + 0x37;
	prim->y0 = -0x4f;
	prim->x1 = MAIN_D_80134774[i] + 0x37 + MAIN_D_8013476C[i];
	prim->y1 = -0x4f;
	prim->x2 = MAIN_D_80134774[i] + 0x37;
	prim->y2 = -0x40;
	prim->x3 = MAIN_D_80134774[i] + 0x37 + MAIN_D_8013476C[i];
	prim->y3 = -0x40;
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0xa, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void BTL_initializePartnerStatusBars(void)
{
	MAIN_D_801350C4 = 0;
	MAIN_D_801350C5 = 0;
	MAIN_D_801350C6 = 0xb;
	MAIN_D_801350C7 = 0;
	MAIN_D_801350C8 = 0;
	MAIN_D_801350C9 = 0;
	MAIN_D_801350CA = 0;
	MAIN_D_801350CB = 0;
	addObject(0x19c, 0, BTL_tickPartnerStatusBars, BTL_renderPartnerStatusBars);
}

void BTL_tickPartnerStatusBars(void)
{
	if (MAIN_D_80135094 != 1) {
		if (MAIN_D_801350C4 != 0) {
			MAIN_D_801350C4--;
		}
		if (MAIN_D_801350CA != 0) {
			MAIN_D_801350CA--;
		}
		if (MAIN_D_801350C4 < 5) {
			if (MAIN_D_801350CB != 0) {
				MAIN_D_801350CB--;
			}
		}
		return;
	}

	if (MAIN_D_801350C4 < 7) {
		MAIN_D_801350C4++;
	}

	if (MAIN_D_801350C4 != 0) {
		if (MAIN_D_801350CB < 7) {
			MAIN_D_801350CB++;
		}
	}

	if (MAIN_D_801350C4 >= 3) {
		if (MAIN_D_801350CA < 7) {
			MAIN_D_801350CA++;
		}
	}
}

INCLUDE_ASM("asm/btl/nonmatchings/battle_hud", BTL_renderPartnerStatusBars);

void BTL_removePartnerStatusBars(void)
{
	removeObject(0x19c, 0);
}
