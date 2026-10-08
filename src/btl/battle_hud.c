#include <string.h>

#include <inline_n.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/battle.h>
#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/font.h>
#include <dw/input.h>
#include <dw/item.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/script.h>
#include <dw/sjis.h>
#include <dw/swap.h>
#include <dw/version.h>
#include <dw/world_object.h>

extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t VIEWPORT_DISTANCE;
extern StatsGains STATS_GAINS;
extern char BTL_STR_LISTENS_TO[];
#if VERSION_REGION_IS(NTSCJ)
extern char BTL_STR_WHITE_WAIT[];
#else
extern char BTL_STR_DROPPED[];
#endif
extern char BTL_STR_WAS_INJURED[];
extern char BTL_STR_SET_TECHNIQUE[];
extern char BTL_STR_PUT_UP_WITH_IT[];
extern char BTL_STR_MOVE_AWAY_CHANGE_TARGET[];
extern char BTL_STR_KEEP_IT_DOWN[];
extern char BTL_STR_GO_ALL_THE_WAY[];
extern char BTL_STR_MP_CONSUMPTION_BONUS[];
#if VERSION_REGION_IS(NTSCJ)
extern char BTL_STR_MP_BONUS_PERCENT[];
#else
extern char BTL_STR_REDUCED_BY[];
#endif
extern char BTL_STR_LEARNED[];
extern MATRIX BTL_BATTLE_START_TEXT_MATRIX;

void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void BTL_tickDeathCountdown(void);
void BTL_initializeFinisherChargeup(void);
void BTL_renderFinisherChargeup(void);
void BTL_tickFinisherChargeup(void);
void BTL_renderDeathCountdown(void);
void BTL_initializeBattleEndText(uint16_t arg0, int16_t arg1, RECT *arg2);
void BTL_renderBattleStartText(void);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount, int32_t *buf);
void setEntityTextDigit(POLY_FT4 *poly, int32_t x, int32_t y);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t u, int32_t v, int32_t w, int32_t h);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t x, int32_t y, int32_t w, int32_t h);
void BTL_drawBattleEndText(int32_t a);
void BTL_renderBattleStartTextBurst(void);
void BTL_scrollBattleEndText(void);
void BTL_appendItemDroppedText(Entity *e);
void BTL_appendInjuredText(char *name);
void BTL_appendCommandLearnedText(void);
void BTL_appendMPBonusText(void);
void BTL_appendMoveLearnedText(int32_t move);
void BTL_tickBattleEndText(void);
void BTL_renderBattleEndText(int32_t n);
int32_t BTL_isEndBoxTextFinished(void);
void BTL_shuffleBattleStartTextPieces(void);
void BTL_renderNumber(int32_t a, int32_t digits, int16_t x, int16_t y, int16_t value, int32_t layer);
void BTL_renderFinisherReadyIcon(void);
void BTL_renderPartnerStatusBars(int16_t idx);
void BTL_tickPartnerStatusBars(void);
void BTL_renderFinisherGauge(int16_t idx);
void BTL_renderFinisherGaugeSegment(int16_t i, int16_t idx);
void damageTick(FighterData *fighter, Stats *stats);

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

#if !VERSION_REGION_IS(NTSCJ)
char BTL_STR_COMMAND_RUN[] = "Run";
char BTL_STR_COMMAND_ATTACK[] = "Attack";
char BTL_STR_COMMAND_CHANGE[] = "Change";
#endif
uint8_t BTL_COMMAND_LABEL_U[5] = { 0, 11, 25, 39, 50 };
uint8_t BTL_COMMAND_LABEL_W[8] = { 11, 14, 14, 11, 11, 0, 0, 0 };
uint8_t BTL_DEATH_COUNTDOWN_DIGIT_U[4] = { 0x50, 0x68, 0x58, 0x68 };
uint8_t BTL_DEATH_COUNTDOWN_DIGIT_V[4] = { 0xa8, 0x90, 0x90, 0x80 };
#if VERSION_REGION_IS(NTSCJ)
char BTL_STR_YELLOW[] = "C7";
char BTL_STR_DROPPED[] = "C1はRC7";
char BTL_STR_MP_BONUS_20[] = "２０";
char BTL_STR_MP_BONUS_15[] = "１５";
char BTL_STR_MP_BONUS_10[] = "１０";
char BTL_STR_MP_BONUS_5[] = "５";
#else
char BTL_STR_YELLOW[] = "#C7";
char BTL_STR_WHITE_WAIT[] = "#C1#W";
char BTL_STR_MP_BONUS_20[] = "20";
char BTL_STR_MP_BONUS_15[] = "15";
char BTL_STR_MP_BONUS_10[] = "10";
char BTL_STR_MP_BONUS_5[] = "5";
char BTL_STR_MP_BONUS_PERCENT[] = "%!#W";
#endif
uint8_t BTL_FINISHER_SEGMENT_U[8] = { 0, 6, 10, 18, 22, 30, 38, 43 };
uint8_t BTL_FINISHER_SEGMENT_W[8] = { 6, 4, 8, 4, 8, 8, 5, 5 };
uint8_t BTL_FINISHER_SEGMENT_X[8] = { 0, 6, 10, 18, 22, 30, 38, 43 };

int16_t BTL_FINISHER_CHARGEUP_POS[2];
int8_t BTL_COMMAND_MENU_ACTIVE;
int32_t BATTLE_END_TYPING;
RECT BATTLE_END_BOX;
uint8_t *BATTLE_END_CURSOR;
uint16_t BATTLE_END_PEN_X;
uint16_t BATTLE_END_PEN_Y;
uint16_t BATTLE_END_V;
int16_t BATTLE_END_ROWS_SHOWN;
int16_t BATTLE_END_BOX_LINE_COUNT;
int16_t BATTLE_END_ROWS_DRAWN;
int16_t BATTLE_END_VISIBLE_ROWS;
int16_t BATTLE_END_WAIT_FRAMES;
int16_t BATTLE_END_WAIT_TIMER;
uint8_t BTL_BATTLE_START_TEXT_TIMER[4];
int32_t BTL_BATTLE_TEXT_FINISHED;
uint8_t BTL_STATUS_BARS_STEP;
uint8_t BTL_FINISHER_FULL_FRAMES;
uint8_t BTL_FINISHER_PULSE_FRAME;
uint8_t BTL_FINISHER_BRIGHTNESS;
uint8_t BTL_FINISHER_SEGMENTS;
uint8_t BTL_FINISHER_READY;
uint8_t BTL_MP_BAR_STEP;
uint8_t BTL_HP_BAR_STEP;

static void *battle_hud_sbss_order[] = {
	&BTL_HP_BAR_STEP,
	&BTL_MP_BAR_STEP,
	&BTL_FINISHER_READY,
	&BTL_FINISHER_SEGMENTS,
	&BTL_FINISHER_BRIGHTNESS,
	&BTL_FINISHER_PULSE_FRAME,
	&BTL_FINISHER_FULL_FRAMES,
	&BTL_STATUS_BARS_STEP,
	&BTL_BATTLE_TEXT_FINISHED,
	BTL_BATTLE_START_TEXT_TIMER,
#if VERSION_REGION_IS(NTSCJ)
	&BATTLE_END_WAIT_TIMER,
	&BATTLE_END_WAIT_FRAMES,
	&BATTLE_END_VISIBLE_ROWS,
	&BATTLE_END_ROWS_DRAWN,
	&BATTLE_END_BOX_LINE_COUNT,
	&BATTLE_END_ROWS_SHOWN,
	&BATTLE_END_CURSOR,
	&BATTLE_END_V,
	&BATTLE_END_PEN_Y,
	&BATTLE_END_PEN_X,
#else
	&BATTLE_END_WAIT_TIMER,
	&BATTLE_END_WAIT_FRAMES,
	&BATTLE_END_VISIBLE_ROWS,
	&BATTLE_END_ROWS_DRAWN,
	&BATTLE_END_BOX_LINE_COUNT,
	&BATTLE_END_ROWS_SHOWN,
	&BATTLE_END_V,
	&BATTLE_END_PEN_Y,
	&BATTLE_END_PEN_X,
	&BATTLE_END_CURSOR,
#endif
	&BATTLE_END_BOX,
	&BATTLE_END_TYPING,
	&BTL_COMMAND_MENU_ACTIVE,
	&BTL_FINISHER_CHARGEUP_POS,
};

// clang-format off
#if VERSION_REGION_IS(NTSCJ)
extern char *BTL_COMMAND_NAMES[];
#else
char BTL_STR_COMMAND_YOUR_CALL[] = "Your Call";
char BTL_STR_COMMAND_MODERATE[] = "Moderate";
char BTL_STR_COMMAND_DISTANCE[] = "Distance";
char BTL_STR_COMMAND_DEFENSIVE[] = "Defensive";

char *BTL_COMMAND_NAMES[8] = {
	BTL_STR_COMMAND_RUN,
	BTL_STR_COMMAND_ATTACK,
	BTL_STR_COMMAND_YOUR_CALL,
	BTL_STR_COMMAND_MODERATE,
	BTL_STR_COMMAND_DISTANCE,
	BTL_STR_COMMAND_DEFENSIVE,
	BTL_STR_COMMAND_CHANGE,
	NULL,
};
#endif

int8_t BTL_SHOUT_HOP_OFFSETS[20] = {
	0, -8, -14, -20, -25, -30, -34, -36,
	-38, -39, -40, -39, -38, -36, -34, -30,
	-34, -36, -38, -39,
};

int8_t BTL_SHOUT_DROP_OFFSETS[20] = {
	0, 1, 2, 4, 6, 10, 15, 20,
	26, 32, 40, 36, 34, 32, 31, 30,
	31, 32, 34, 36,
};

uint8_t BTL_COMMAND_MENU_LAYOUTS[6][10] = {
	{ 0x00, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x03, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x04, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x03, 0xff },
};

int16_t BTL_COMMAND_MENU_SLIDE_Y[8] = {
	0xff88, 0xff96, 0xffa2, 0xffac, 0xffb3, 0xffb8, 0xffbb, 0xffbc,
};

int16_t BTL_COMMAND_MENU_SLIDE_X[8] = {
	0xff68, 0xff69, 0xff6c, 0xff71, 0xff78, 0xff82, 0xff8e, 0xff9c,
};

uint8_t BTL_COMMAND_ICON_UVS[8][2] = {
	{ 0x00, 0xc0 },
	{ 0x20, 0xc0 },
	{ 0x40, 0xc0 },
	{ 0x60, 0xc0 },
	{ 0x80, 0xc0 },
	{ 0xa0, 0xc0 },
	{ 0xc0, 0xc0 },
	{ 0x00, 0x00 },
};

uint8_t BTL_SPECIAL_ICON_UVS[8][2] = {
	{ 0x00, 0xd0 },
	{ 0x20, 0xd0 },
	{ 0x40, 0xd0 },
	{ 0x60, 0xd0 },
	{ 0x80, 0xd0 },
	{ 0xa0, 0xd0 },
	{ 0xc0, 0xd0 },
	{ 0x00, 0x00 },
};
// clang-format on

void BTL_drawHoveredCommandName(void)
{
	RECT area;
	uint8_t cmd;
	int16_t tech;

	setRECT(&area, 0, 0xd8, 0x90, 0xc);
	clearTextSubArea(&area);
	cmd = COMBAT_DATA_PTR->player.availableCommands[0][COMBAT_DATA_PTR->player.hoveredCommand[0]];
	if ((cmd >= 8) && (cmd < 0xc)) {
		tech = entityGetTechFromAnim(ENTITY_TABLE[1], PARTNER_ENTITY.digimonEntity.stats.base.moves[cmd - 8]);
		drawString(MOVE_NAMES[tech], 0, 0xd8);
	} else {
		drawString(BTL_COMMAND_NAMES[cmd - 1], 0, 0xd8);
	}

	renderString(0, -0x8c, -0x42, 0x90, 0xc, 0, 0xd8, 7, 1);
}

void BTL_initializeFinisherChargeup(void)
{
	SVECTOR v;
	VECTOR *loc;

	COMBAT_DATA_PTR->player.finisherChargeup[0] = 0;
	COMBAT_DATA_PTR->player.remainingChargeupTime[0] = 0x50;
	COMBAT_DATA_PTR->fighter[0].finisherProgress = 0;
	if (COMBAT_DATA_PTR->player.hoveredCommand[0] == 0) {
		COMBAT_DATA_PTR->player.hoveredCommand[0] = COMBAT_DATA_PTR->player.numCommands[0] - 1;
	}

	COMBAT_DATA_PTR->player.bufferedCommand[0] = COMBAT_DATA_PTR->player.currentCommand[0] = 3;
	GsSetLsMatrix(&GsWSMATRIX);
	loc = &ENTITY_TABLE[1]->posData->location;
	v.vx = loc->vx;
	v.vy = -DIGIMON_DATA[ENTITY_TABLE[1]->type].height - 0x64;
	v.vz = loc->vz;
	gte_ldv0(&v);
	gte_rtps();
	gte_stsxy((int32_t *)BTL_FINISHER_CHARGEUP_POS);
	BTL_FINISHER_CHARGEUP_POS[0] = (int16_t)BTL_FINISHER_CHARGEUP_POS[0] - (0xb7 - DRAWING_OFFSET_X);
	BTL_FINISHER_CHARGEUP_POS[1] = (int16_t)BTL_FINISHER_CHARGEUP_POS[1] - (0x8c - DRAWING_OFFSET_Y);
	if (BTL_FINISHER_CHARGEUP_POS[0] >= 0x65) {
		BTL_FINISHER_CHARGEUP_POS[0] = 0x64;
	}

	if (BTL_FINISHER_CHARGEUP_POS[0] < -0x64) {
		BTL_FINISHER_CHARGEUP_POS[0] = -0x64;
	}

	if (BTL_FINISHER_CHARGEUP_POS[1] >= 0x65) {
		BTL_FINISHER_CHARGEUP_POS[1] = 0x64;
	}

	if (BTL_FINISHER_CHARGEUP_POS[1] < -0x64) {
		BTL_FINISHER_CHARGEUP_POS[1] = -0x64;
	}

	addObject(0x19a, 0, (TickFunction)BTL_tickFinisherChargeup, (RenderFunction)BTL_renderFinisherChargeup);
}

void BTL_tickFinisherChargeup(void)
{
	int32_t up;

	COMBAT_DATA_PTR->player.remainingChargeupTime[0]--;
	up = 0;
	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 4) != 0) {
		up = 1;
	}

	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 8) != 0) {
		up = 1;
	}

	if ((COMBAT_DATA_PTR->player.finisherChargeup[0] < 0x50) && (up != 0)) {
		COMBAT_DATA_PTR->player.finisherChargeup[0] += 2;
	}
}

void BTL_renderFinisherChargeup(void)
{
	POLY_FT4 prim;
	int32_t i;
	int16_t bars;

	SetPolyFT4(&prim);
	prim.tpage = getTPage(0, 0, 960, 256);
	setClut(&prim, 272, 498);
	setRGB0(&prim, 0x80, 0x80, 0x80);
	setUVWH(&prim, 0x58, 0xe0, 46, 12);
	setXYWH(&prim, BTL_FINISHER_CHARGEUP_POS[0], BTL_FINISHER_CHARGEUP_POS[1], 0x2e, 0xc);
	GsSortPoly(&prim, ACTIVE_ORDERING_TABLE, 7);
	bars = COMBAT_DATA_PTR->player.finisherChargeup[0] / 8;
	setUVWH(&prim, 0x88, 0xe0, 4, 6);
	for (i = 0; i < bars; i++) {
		setXYWH(&prim, (int32_t)(BTL_FINISHER_CHARGEUP_POS[0] + 3 + i * 4), BTL_FINISHER_CHARGEUP_POS[1] + 3, 4, 6);
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

	sp = &BTL_DEATH_COUNTDOWN_SPRITE;
	sp->attribute = 0;
	sp->tpage = getTPage(0, 0, 896, 256);
	sp->u = 0x32;
	sp->v = 0x80;
	sp->mx = 0x14;
	sp->my = 0x14;
	sp->cx = 0x100;
	sp->r = 0x80;
	sp->g = 0x80;
	sp->b = 0x80;
	BTL_DEATH_COUNTDOWN.data.sprite = *sp;
	sp = &BTL_DEATH_COUNTDOWN.data.sprite;
	do {
		sp->mx = 8;
		sp->my = 8;
		setWH(sp, 0x10, 0x10);
	} while (0);
	BTL_DEATH_COUNTDOWN.data.timer = -1;
}

void BTL_addDeathCountdown(Entity *entity)
{
	DVECTOR pos;
	GsSPRITE *sprite;
	GsSPRITE *shadow;
#if !VERSION_REGION_IS(NTSCJ)
	int16_t py;
#endif

#if VERSION_EQUAL_OR_NEWER(JP_TRIAL)
	if (BTL_DEATH_COUNTDOWN.data.timer != -1) {
		return;
	}
#endif
	BTL_DEATH_COUNTDOWN.data.timer = 0;
	BTL_DEATH_COUNTDOWN.data.step = 0;
	getEntityScreenPos(entity, 1, &pos);
	if (pos.vx >= 0x8d) {
		pos.vx = 0x8c;
	}
	if (pos.vx < -0x8c) {
		pos.vx = -0x8c;
	}
	if (pos.vy >= 0x65) {
		pos.vy = 0x64;
	}
	if (pos.vy < -0x64) {
		pos.vy = -0x64;
	}
#if VERSION_REGION_IS(NTSCJ)
	sprite = &BTL_DEATH_COUNTDOWN_SPRITE;
	shadow = &BTL_DEATH_COUNTDOWN.data.sprite;
	shadow->cy = sprite->cy = 0x1ed;
	shadow->rotate = sprite->rotate = 0;
	shadow->x = sprite->x = pos.vx;
	shadow->y = sprite->y = pos.vy;
#else
	shadow = &BTL_DEATH_COUNTDOWN.data.sprite;
	do {
		BTL_DEATH_COUNTDOWN_SPRITE.cy = 0x1ed;
		shadow->cy = 0x1ed;
		BTL_DEATH_COUNTDOWN_SPRITE.rotate = 0;
		shadow->rotate = 0;
		BTL_DEATH_COUNTDOWN_SPRITE.x = pos.vx;
		shadow->x = pos.vx;
		py = pos.vy;
		BTL_DEATH_COUNTDOWN_SPRITE.y = py;
		shadow->y = py;
	} while (0);
#endif
	addObject(0x197, 0, (TickFunction)BTL_tickDeathCountdown, (RenderFunction)BTL_renderDeathCountdown);
}

void BTL_tickDeathCountdown(void)
{
	GsSPRITE *base;
	GsSPRITE *spin;
	long frame;

	if (TAMER_ITEM.worldItem.type == 0xff) {
		base = &BTL_DEATH_COUNTDOWN_SPRITE;
		spin = &BTL_DEATH_COUNTDOWN.data.sprite;
		do {
			BTL_DEATH_COUNTDOWN.data.timer++;
			if ((BTL_DEATH_COUNTDOWN.data.timer % 31) == 0) {
				BTL_DEATH_COUNTDOWN.data.step = 0;
			}
			BTL_DEATH_COUNTDOWN.data.step++;
			frame = (BTL_DEATH_COUNTDOWN.data.timer + 0x1d) / 30;
			if (frame < 5) {
				spin->u = BTL_DEATH_COUNTDOWN_DIGIT_U[frame - 1];
				spin->v = BTL_DEATH_COUNTDOWN_DIGIT_V[frame - 1];
			} else {
				spin->u = 0x58;
				spin->v = 0x80;
			}
			if ((BTL_DEATH_COUNTDOWN.data.step > 0) && (BTL_DEATH_COUNTDOWN.data.step < 0x15)) {
				if (BTL_DEATH_COUNTDOWN.data.step == 1) {
					base->scalex = base->scaley = 0x1000;
					spin->rotate = base->rotate = 0;
				}
				if ((BTL_DEATH_COUNTDOWN.data.step >= 2) && (BTL_DEATH_COUNTDOWN.data.step < 5)) {
					spin->scaley = spin->scalex += 0x199;
				} else if ((BTL_DEATH_COUNTDOWN.data.step >= 5) && (BTL_DEATH_COUNTDOWN.data.step < 8)) {
					spin->scaley = spin->scalex -= 0x199;
				} else {
					spin->scalex = spin->scaley = 0x1000;
				}
				if ((BTL_DEATH_COUNTDOWN.data.step % 3) == 0) {
					base->cy++;
					if (base->cy >= 0x1f0) {
						base->cy = 0x1ed;
					}
				}
			} else {
				spin->scalex = spin->scaley = base->scaley = base->scalex = ((0x64 - ((BTL_DEATH_COUNTDOWN.data.step - 0x14) * 5)) << 0xc) / 100;
				base->rotate += 0x40000;
				spin->rotate = base->rotate;
			}
			if ((spin->scalex >= 0x2000) || (spin->scaley >= 0x2000)) {
				setWH(base, 0x27, 0x27);
			} else {
				setWH(base, 0x28, 0x28);
			}
		} while (0);
	}
}

void BTL_renderDeathCountdown(void)
{
	GsSortSprite(&BTL_DEATH_COUNTDOWN.data.sprite, ACTIVE_ORDERING_TABLE, 7);
	GsSortSprite(&BTL_DEATH_COUNTDOWN_SPRITE, ACTIVE_ORDERING_TABLE, 7);
	if (BTL_DEATH_COUNTDOWN.data.timer >= 0x96) {
		BTL_removeDeathCountdown();
	}
}

void BTL_removeDeathCountdown(void)
{
	if (BTL_DEATH_COUNTDOWN.data.timer != -1) {
		removeObject(0x197, 0);
		BTL_DEATH_COUNTDOWN.data.timer = -1;
	}
}

void BTL_initializeBattleEndText(uint16_t arg0, int16_t arg1, RECT *arg2)
{
	BATTLE_END_TYPING = 0;
	BATTLE_END_BOX = *arg2;
	BATTLE_END_PEN_X = 0;
	BATTLE_END_V = BATTLE_END_PEN_Y = arg0;
	BATTLE_END_CURSOR = (uint8_t *)BTL_END_BOX_TEXTBUFFER;
	BATTLE_END_ROWS_SHOWN = 0;
	BATTLE_END_BOX_LINE_COUNT = 0;
	BATTLE_END_ROWS_DRAWN = 0;
	BTL_END_BOX_TEXTBUFFER[0] = 0;
	BATTLE_END_VISIBLE_ROWS = arg1;
	BATTLE_END_WAIT_FRAMES = 0x3c;
	BATTLE_END_WAIT_TIMER = BATTLE_END_WAIT_FRAMES;
}

void BTL_appendItemDroppedText(Entity *e)
{
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_YELLOW);
	strcat(BTL_END_BOX_TEXTBUFFER, DIGIMON_DATA[e->type].name);
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_DROPPED);
	strcat(BTL_END_BOX_TEXTBUFFER,
	       ITEM_PARA[DIGIMON_DATA[e->type].dropItem].name);
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_WHITE_WAIT);
	BATTLE_END_BOX_LINE_COUNT += 2;
}

void BTL_appendInjuredText(char *name)
{
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_YELLOW);
	strcat(BTL_END_BOX_TEXTBUFFER, name);
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_WAS_INJURED);
	BATTLE_END_BOX_LINE_COUNT += 2;
}

void BTL_appendCommandLearnedText(void)
{
	int16_t old;
	int16_t total;

	total = INITIAL_COMBAT_STATS[0].brains + STATS_GAINS.brains;
	if (total < 0x64) {
		return;
	}
	old = INITIAL_COMBAT_STATS[0].brains;

	if (total >= 0x1f4) {
		if (old < 0x1f4) {
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_SET_TECHNIQUE);
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_LISTENS_TO);
			BATTLE_END_BOX_LINE_COUNT += 2;
		}
	} else if (total >= 0x190) {
		if (old < 0x190) {
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_PUT_UP_WITH_IT);
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_LISTENS_TO);
			BATTLE_END_BOX_LINE_COUNT += 2;
		}
	} else if (total >= 0x12c) {
		if (old < 0x12c) {
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_MOVE_AWAY_CHANGE_TARGET);
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_LISTENS_TO);
			BATTLE_END_BOX_LINE_COUNT += 3;
		}
	} else if (total >= 0xc8) {
		if (old < 0xc8) {
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_KEEP_IT_DOWN);
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_LISTENS_TO);
			BATTLE_END_BOX_LINE_COUNT += 2;
		}
	} else {
		if (old < 0x64) {
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_GO_ALL_THE_WAY);
			strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_LISTENS_TO);
			BATTLE_END_BOX_LINE_COUNT += 2;
		}
	}
}

void BTL_appendMPBonusText(void)
{
	char buf[8];
	int16_t old;
	int16_t total;

	total = INITIAL_COMBAT_STATS[0].brains + STATS_GAINS.brains;
	if (total < 0x2bc) {
		return;
	}

	buf[0] = 0;
	old = INITIAL_COMBAT_STATS[0].brains;
	if (total >= 0x3e7) {
		if (old < 0x3e7) {
			strcpy(buf, BTL_STR_MP_BONUS_20);
		}
	} else if (total >= 0x384) {
		if (old < 0x384) {
			strcpy(buf, BTL_STR_MP_BONUS_15);
		}
	} else if (total >= 0x320) {
		if (old < 0x320) {
			strcpy(buf, BTL_STR_MP_BONUS_10);
		}
	} else {
		if (old < 0x2bc) {
			strcpy(buf, BTL_STR_MP_BONUS_5);
		}
	}

	if (buf[0] == 0) {
		return;
	}

	strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_MP_CONSUMPTION_BONUS);
#if !VERSION_REGION_IS(NTSCJ)
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_REDUCED_BY);
#endif
	strcat(BTL_END_BOX_TEXTBUFFER, buf);
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_MP_BONUS_PERCENT);
	BATTLE_END_BOX_LINE_COUNT += 4;
}

// clang-format off
void BTL_appendMoveLearnedText(move)
	int16_t move;
// clang-format on
{
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_YELLOW);
	strcat(BTL_END_BOX_TEXTBUFFER, MOVE_NAMES[move]);
	strcat(BTL_END_BOX_TEXTBUFFER, BTL_STR_LEARNED);
	BATTLE_END_BOX_LINE_COUNT += 2;
}

void BTL_drawBattleEndText(int32_t flag)
{
#if VERSION_REGION_IS(NTSCJ)
#ifdef __MWERKS__
	extern void drawGlyph(uint16_t codepoint, int32_t x, int32_t y);
#endif
	uint16_t glyph;

	do {
		switch (*BATTLE_END_CURSOR) {
		case 'C':
			BATTLE_END_CURSOR++;
			setTextColor(*BATTLE_END_CURSOR);
			BATTLE_END_CURSOR++;
			break;
		case 'W':
			BATTLE_END_WAIT_TIMER = BATTLE_END_WAIT_FRAMES;
			/* fall through */
		case 'R':
			BATTLE_END_CURSOR++;
			BATTLE_END_PEN_X = 0;
			BATTLE_END_PEN_Y += 0xc;
			BATTLE_END_ROWS_SHOWN++;
			BATTLE_END_ROWS_DRAWN++;
			return;
		case '0':
			return;
		default:
			glyph = BATTLE_END_CURSOR[0] | (BATTLE_END_CURSOR[1] << 8);
			drawGlyph(glyph, BATTLE_END_PEN_X, BATTLE_END_PEN_Y);
			BATTLE_END_PEN_X += 0xc;
			BATTLE_END_CURSOR += 2;
			break;
		}
	} while (flag);
#else
	uint8_t c;
	uint16_t glyph;
	uint16_t w;

	while (flag) {
		if (*BATTLE_END_CURSOR == '#') {
			BATTLE_END_CURSOR++;
			switch (*BATTLE_END_CURSOR) {
			case 'C':
				BATTLE_END_CURSOR++;
				setTextColor(*BATTLE_END_CURSOR);
				BATTLE_END_CURSOR++;
				break;
			case 'W':
				BATTLE_END_WAIT_TIMER = BATTLE_END_WAIT_FRAMES;
				/* fall through */
			case 'R':
				BATTLE_END_PEN_X = 0;
				BATTLE_END_CURSOR++;
				BATTLE_END_PEN_Y += 0xc;
				BATTLE_END_ROWS_SHOWN++;
				BATTLE_END_ROWS_DRAWN++;
				return;
			}
			continue;
		}
		c = *BATTLE_END_CURSOR;
		if (c == '0') {
			return;
		}
		glyph = swapShortBytes(convertAsciiToJis(c));
		w = drawGlyph(glyph, BATTLE_END_PEN_X, BATTLE_END_PEN_Y);
		BATTLE_END_PEN_X += w;
		BATTLE_END_CURSOR++;
		return;
	}
#endif
}

void BTL_scrollBattleEndText(void)
{
	BATTLE_END_V += 0xc;
	BATTLE_END_ROWS_SHOWN--;
}

void BTL_tickBattleEndText(void)
{
	if (BATTLE_END_WAIT_TIMER != 0) {
		BATTLE_END_WAIT_TIMER -= 1;
	}

	if (BTL_END_BOX_TEXTBUFFER[0] == 0) {
		if (((POLLED_INPUT == CONFIRM_BUTTON) || (POLLED_INPUT == CANCEL_BUTTON)) && (POLLED_INPUT != POLLED_INPUT_PREVIOUS)) {
			BATTLE_END_WAIT_TIMER = 0;
		}
		return;
	}

	if (BATTLE_END_ROWS_SHOWN == BATTLE_END_VISIBLE_ROWS) {
		if (BATTLE_END_WAIT_TIMER == 0) {
			BTL_scrollBattleEndText();
		}
	} else {
		BTL_drawBattleEndText(BATTLE_END_TYPING);
		if (BATTLE_END_WAIT_TIMER != 0) {
			BATTLE_END_TYPING = 0;
		}
	}

	if (((POLLED_INPUT == CONFIRM_BUTTON) || (POLLED_INPUT == CANCEL_BUTTON)) && (POLLED_INPUT != POLLED_INPUT_PREVIOUS)) {
		if (BATTLE_END_WAIT_TIMER != 0) {
			BATTLE_END_WAIT_TIMER = 0;
		} else {
			BATTLE_END_TYPING = 1;
		}
	}
}

// clang-format off
void BTL_renderBattleEndText(n)
	int16_t n;
// clang-format on
{
	renderString(0, BATTLE_END_BOX.x, BATTLE_END_BOX.y, BATTLE_END_BOX.w, BATTLE_END_BOX.h, 0, BATTLE_END_V, 6 - n, 0);
}

int32_t BTL_isEndBoxTextFinished(void)
{
	if ((BATTLE_END_WAIT_TIMER == 0) && (BATTLE_END_BOX_LINE_COUNT == BATTLE_END_ROWS_DRAWN)) {
		return 1;
	}

	return 0;
}

void BTL_shuffleBattleStartTextPieces(void)
{
	int32_t i;
	int32_t r;

	for (i = 0; i < 0x9b; i++) {
		r = randomLimit(0x9b);
		swapByte(&BTL_BATTLE_START_TEXT_PIECES[i][0x11], &BTL_BATTLE_START_TEXT_PIECES[r][0x11]);
	}
}

void BTL_initializeBattleStartText(void)
{
	uint8_t (*p)[20];
	int32_t sgn;
	int32_t i;
	int32_t r;
#if !VERSION_REGION_IS(NTSCJ)
	int32_t t;
#endif

	BTL_BATTLE_START_TEXT_TIMER[0] = 0;
	BTL_BATTLE_TEXT_FINISHED = 0;
	p = BTL_BATTLE_START_TEXT_PIECES;
	for (i = 0; i < 0x9b; i++, p++) {
		(*p)[0x11] = i;
		(*p)[0x12] = 0x18;
		(*p)[0x13] = randomLimit(3);
	}

	BTL_shuffleBattleStartTextPieces();

	p = BTL_BATTLE_START_TEXT_PIECES;
	for (i = 0; i < 0x9b; i++, p++) {
		if (randomLimit(2) == 1) {
			sgn = 1;
		} else {
			sgn = -1;
		}
		((int16_t *)*p)[5] = BTL_BATTLE_START_TEXT_POSITIONS[i][1];
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
		((int16_t *)*p)[6] = (r + 8) * BTL_BATTLE_START_TEXT_POSITIONS[i][0] / 8;
		((int16_t *)*p)[7] = (r + 8) * BTL_BATTLE_START_TEXT_POSITIONS[i][1] / 8;
#else
		t = BTL_BATTLE_START_TEXT_POSITIONS[i][0];
		((int16_t *)*p)[6] = (r + 8) * t / 8;
		t = BTL_BATTLE_START_TEXT_POSITIONS[i][1];
		((int16_t *)*p)[7] = (r + 8) * t / 8;
#endif
		((int16_t *)*p)[0] = 0;
		((int16_t *)*p)[1] = 0;
		((int16_t *)*p)[2] = 0;
	}

	addObject(0x1a6, 0, NULL, (RenderFunction)BTL_renderBattleStartText);
}

void BTL_renderBattleStartText(void)
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

	GsSetProjection(0x200);
	GsSetLsMatrix(&BTL_BATTLE_START_TEXT_MATRIX);

	n = 0;
	for (i = 0; i < 0x9b; i++) {
		if (BTL_BATTLE_START_TEXT_PIECES[i][0x12] != 0) {
			break;
		}
		n++;
	}

	if (n == 0x9b) {
		clut = GetClut(256, (BTL_BATTLE_START_TEXT_TIMER[0]++ % 6 / 2) + 488);
		BTL_BATTLE_TEXT_FINISHED = 1;
	} else {
		clut = GetClut(256, 488);
	}

	p = BTL_BATTLE_START_TEXT_PIECES;
	prim = (POLY_FT4 *)GsGetWorkBase();
	ot = ACTIVE_ORDERING_TABLE->org;
	for (i = 0; i < 0x9b; i++, p++) {
		if (((int16_t *)*p)[4] != BTL_BATTLE_START_TEXT_POSITIONS[i][0]) {
			((int16_t *)*p)[4] += ((int8_t *)*p)[0x10];
			if (((int8_t *)*p)[0x10] > 0) {
				if (((int16_t *)*p)[4] > BTL_BATTLE_START_TEXT_POSITIONS[i][0]) {
					((int16_t *)*p)[4] = BTL_BATTLE_START_TEXT_POSITIONS[i][0];
				}
			} else {
				if (((int16_t *)*p)[4] < BTL_BATTLE_START_TEXT_POSITIONS[i][0]) {
					((int16_t *)*p)[4] = BTL_BATTLE_START_TEXT_POSITIONS[i][0];
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
	GsSetRefView2(&GS_VIEWPOINT);
}

void BTL_removeBattleStartText(void)
{
	removeObject(0x1a6, 0);
}

void BTL_initializeBattleStartTextBurst(void)
{
	BTL_BATTLE_TEXT_FINISHED = 0;
	addObject(0x1a6, 0, NULL, (RenderFunction)BTL_renderBattleStartTextBurst);
}

void BTL_renderBattleStartTextBurst(void)
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

	GsSetProjection(0x200);
	GsSetLsMatrix(&BTL_BATTLE_START_TEXT_MATRIX);

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
	p = BTL_BATTLE_START_TEXT_PIECES;
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
		BTL_BATTLE_TEXT_FINISHED = 1;
	}
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void BTL_removeBattleStartTextBurst(void)
{
	removeObject(0x1a6, 0);
}

int32_t BTL_isBattleStartTextFinished(void)
{
	return BTL_BATTLE_TEXT_FINISHED;
}

void BTL_renderNumber(int32_t a, int32_t digits, int16_t x, int16_t y, int16_t value, int32_t layer)
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

void BTL_renderFinisherGauge(int16_t idx)
{
	int32_t hp;
	int32_t i;

	if (BTL_STATUS_BARS_STEP != 7) {
		return;
	}

	hp = COMBAT_DATA_PTR->fighter[idx].finisherProgress * 6 / COMBAT_DATA_PTR->fighter[idx].finisherGoal;

	if (BTL_FINISHER_SEGMENTS != hp) {
		BTL_FINISHER_PULSE_FRAME = 0;
		BTL_FINISHER_SEGMENTS = hp;
	}

	BTL_FINISHER_BRIGHTNESS = BTL_FINISHER_PULSE[BTL_FINISHER_PULSE_FRAME];
	if (BTL_FINISHER_PULSE_FRAME < 0xb) {
		BTL_FINISHER_PULSE_FRAME++;
	}

	if (hp == 6) {
		if (BTL_FINISHER_FULL_FRAMES < 0xa) {
			BTL_FINISHER_FULL_FRAMES++;
		}

		if (BTL_FINISHER_FULL_FRAMES >= 3) {
			hp++;
		}

		if (BTL_FINISHER_FULL_FRAMES >= 5) {
			hp++;
			BTL_renderFinisherReadyIcon();

			if (BTL_FINISHER_FULL_FRAMES == 0xa) {
				BTL_FINISHER_READY = 1;
				BTL_FINISHER_PULSE_FRAME %= 0xb;
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
	if ((BTL_FINISHER_READY != 1) || (COMBAT_DATA_PTR->player.currentCommand[0] == 0xb)) {
		setRGB0(prim, 0x80, 0x80, 0x80);
	} else {
		setRGB0(prim, BTL_FINISHER_BRIGHTNESS, BTL_FINISHER_BRIGHTNESS, BTL_FINISHER_BRIGHTNESS);
	}

	setUVWH(prim, 0x80, 0x88, 37, 9);
	setXYWH(prim, 0x6e, -0x4b, 37, 9);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0xa, prim++);
	GsSetWorkBase((PACKET *)prim);

	box.attribute = 0x40000000;
	if ((BTL_FINISHER_READY != 1) || (COMBAT_DATA_PTR->player.currentCommand[0] == 0xb)) {
		box.r = box.g = box.b = 0x80;
	} else {
		box.r = box.g = box.b = BTL_FINISHER_BRIGHTNESS;
	}

	setWH(&box, 0x29, 0xb);
	box.x = 0x6c;
	box.y = -0x4c;
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 0xa);
}

void BTL_renderFinisherGaugeSegment(int16_t i, int16_t idx)
{
	POLY_FT4 *prim;
	int32_t hp;

	hp = COMBAT_DATA_PTR->fighter[idx].finisherProgress * 6 / COMBAT_DATA_PTR->fighter[idx].finisherGoal;
	prim = (POLY_FT4 *)GsGetWorkBase();
	setEntityTextDigit(prim, 256, 492);
	if ((((hp - 1) == i) || (BTL_FINISHER_READY == 1)) && (COMBAT_DATA_PTR->player.currentCommand[0] != 0xb)) {
		setRGB0(prim, BTL_FINISHER_BRIGHTNESS, BTL_FINISHER_BRIGHTNESS, BTL_FINISHER_BRIGHTNESS);
	} else {
		setRGB0(prim, 0x80, 0x80, 0x80);
	}

	setUVWH(prim, BTL_FINISHER_SEGMENT_U[i], 0x9d, BTL_FINISHER_SEGMENT_W[i], 15);
	setXYWH(prim, BTL_FINISHER_SEGMENT_X[i] + 0x37, -0x4f, BTL_FINISHER_SEGMENT_W[i], 15);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0xa, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void BTL_initializePartnerStatusBars(void)
{
	BTL_STATUS_BARS_STEP = 0;
	BTL_FINISHER_FULL_FRAMES = 0;
	BTL_FINISHER_PULSE_FRAME = 0xb;
	BTL_FINISHER_BRIGHTNESS = 0;
	BTL_FINISHER_SEGMENTS = 0;
	BTL_FINISHER_READY = 0;
	BTL_HP_BAR_STEP = BTL_MP_BAR_STEP = 0;
	addObject(0x19c, 0, (TickFunction)BTL_tickPartnerStatusBars, (RenderFunction)BTL_renderPartnerStatusBars);
}

void BTL_tickPartnerStatusBars(void)
{
	if (BTL_COMMAND_MENU_ACTIVE != 1) {
		if (BTL_STATUS_BARS_STEP != 0) {
			BTL_STATUS_BARS_STEP--;
		}
		if (BTL_MP_BAR_STEP != 0) {
			BTL_MP_BAR_STEP--;
		}
		if (BTL_STATUS_BARS_STEP < 5) {
			if (BTL_HP_BAR_STEP != 0) {
				BTL_HP_BAR_STEP--;
			}
		}
		return;
	}

	if (BTL_STATUS_BARS_STEP < 7) {
		BTL_STATUS_BARS_STEP++;
	}

	if (BTL_STATUS_BARS_STEP != 0) {
		if (BTL_HP_BAR_STEP < 7) {
			BTL_HP_BAR_STEP++;
		}
	}

	if (BTL_STATUS_BARS_STEP >= 3) {
		if (BTL_MP_BAR_STEP < 7) {
			BTL_MP_BAR_STEP++;
		}
	}
}

void BTL_renderPartnerStatusBars(int16_t idx)
{
	POLY_FT4 *prim;
	const BarSprite *p;
	FighterData *fighter;
	int32_t bar;
	int32_t k;
	int16_t *hpPtr;
	int16_t *mpPtr;
	int16_t cur;
	int16_t fill;
	int16_t maxHp;
	int16_t maxMp;
	int16_t x0;
	int16_t y0;

	fighter = &COMBAT_DATA_PTR->fighter[idx];
	hpPtr = &PARTNER_ENTITY.digimonEntity.stats.current.currentHP;
	mpPtr = &PARTNER_ENTITY.digimonEntity.stats.current.currentMP;
	maxHp = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	maxMp = PARTNER_ENTITY.digimonEntity.stats.base.mp;

	if (*hpPtr == 0) {
		fighter->hpDamageBuffer = 0;
	}
	if (fighter->hpDamageBuffer != 0) {
		damageTick(fighter, &PARTNER_ENTITY.digimonEntity.stats);
	}
	*mpPtr -= fighter->mpDamageBuffer;
	fighter->mpDamageBuffer = 0;
	if (*mpPtr < 0) {
		*mpPtr = 0;
	}

	for (bar = 0; bar < 2; bar++) {
		p = &BTL_STATUS_BAR_SPRITES[(bar * 3) + 2];
		if (bar == 0) {
			x0 = BTL_STATUS_BAR_X[BTL_HP_BAR_STEP];
			y0 = -0x64;
			cur = PARTNER_ENTITY.digimonEntity.stats.current.currentHP;
			fill = *hpPtr * 50 / maxHp;
		} else {
			x0 = BTL_STATUS_BAR_X[BTL_MP_BAR_STEP];
			y0 = -0x58;
			cur = PARTNER_ENTITY.digimonEntity.stats.current.currentMP;
			fill = *mpPtr * 50 / maxMp;
		}
		BTL_renderNumber(0, 4, x0 + 0x49, y0 - 3, cur, 0xa);
		prim = (POLY_FT4 *)GsGetWorkBase();
		for (k = 0; k < 3; k++, p--) {
			SetPolyFT4(prim);
			setClut(prim, 256, p->clut);
			prim->tpage = getTPage(0, 0, 896, 256);
			setRGB0(prim, 0x80, 0x80, 0x80);
			setUVWH(prim, p->u, p->v, p->w, p->h);
			setXYWH(prim, x0 + p->x, y0 + p->y, (k == 0) ? fill : p->w, p->h);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
		}
		GsSetWorkBase((PACKET *)prim);
	}
	BTL_renderFinisherGauge(idx);
}

void BTL_removePartnerStatusBars(void)
{
	removeObject(0x19c, 0);
}
