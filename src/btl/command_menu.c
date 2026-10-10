#include <inline_n.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/font.h>
#include <dw/item.h>
#include <dw/script.h>
#include <dw/version.h>
#include <dw/world_object.h>

extern char *BTL_COMMAND_NAMES[];

void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uPos, int32_t vPos, int32_t width, int32_t height);
void BTL_tickCommandMenu(void);
void BTL_renderCommandMenu(int32_t arg0);
void BTL_removeCommandMenu(void);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void BTL_initializeFinisherChargeup(void);
void BTL_tickFinisherChargeup(void);
void BTL_renderFinisherChargeup(void);
void BTL_tickDeathCountdown(void);
void BTL_renderDeathCountdown(void);

int16_t BTL_COMMAND_MENU_X;
int16_t BTL_COMMAND_MENU_Y;
uint8_t BTL_COMMAND_MENU_BLINK;
uint8_t BTL_COMMAND_MENU_TIMER;
uint8_t BTL_COMMAND_MENU_X_STEP;
uint8_t BTL_COMMAND_MENU_Y_STEP;
uint8_t BTL_COMMAND_MENU_LAYOUT;
int16_t BTL_FINISHER_CHARGEUP_POS[2];
int8_t BTL_COMMAND_MENU_ACTIVE;

static void *command_menu_sbss_order[] = {
	&BTL_COMMAND_MENU_ACTIVE,
	&BTL_FINISHER_CHARGEUP_POS,
#if VERSION_IS(EU)
	&BTL_COMMAND_MENU_X,
	&BTL_COMMAND_MENU_Y,
	&BTL_COMMAND_MENU_BLINK,
	&BTL_COMMAND_MENU_TIMER,
	&BTL_COMMAND_MENU_X_STEP,
	&BTL_COMMAND_MENU_Y_STEP,
	&BTL_COMMAND_MENU_LAYOUT,
#else
	&BTL_COMMAND_MENU_LAYOUT,
	&BTL_COMMAND_MENU_Y_STEP,
	&BTL_COMMAND_MENU_X_STEP,
	&BTL_COMMAND_MENU_TIMER,
	&BTL_COMMAND_MENU_BLINK,
	&BTL_COMMAND_MENU_Y,
	&BTL_COMMAND_MENU_X,
#endif
};

static void *command_menu_functions[] = {
	BTL_removeDeathCountdown,
#if VERSION_IS(EU)
	BTL_addDeathCountdown,
	BTL_tickDeathCountdown,
	BTL_renderDeathCountdown,
	BTL_initializeDeathCountdown,
	BTL_removeFinisherChargeup,
	BTL_initializeFinisherChargeup,
	BTL_tickFinisherChargeup,
	BTL_renderFinisherChargeup,
	BTL_drawHoveredCommandName,
	BTL_isCommandMenuClosed,
	BTL_removeCommandMenu,
	BTL_tickCommandMenu,
	BTL_renderCommandMenu,
#else
	BTL_renderDeathCountdown,
	BTL_tickDeathCountdown,
	BTL_addDeathCountdown,
	BTL_initializeDeathCountdown,
	BTL_removeFinisherChargeup,
	BTL_renderFinisherChargeup,
	BTL_tickFinisherChargeup,
	BTL_initializeFinisherChargeup,
	BTL_drawHoveredCommandName,
	BTL_isCommandMenuClosed,
	BTL_removeCommandMenu,
	BTL_renderCommandMenu,
	BTL_tickCommandMenu,
#endif
	BTL_initializeCommandMenu,
};

uint8_t BTL_COMMAND_LABEL_U[5] = { 0, 11, 25, 39, 50 };
uint8_t BTL_COMMAND_LABEL_W[8] = { 11, 14, 14, 11, 11, 0, 0, 0 };
uint8_t BTL_DEATH_COUNTDOWN_DIGIT_U[4] = { 0x50, 0x68, 0x58, 0x68 };
uint8_t BTL_DEATH_COUNTDOWN_DIGIT_V[4] = { 0xa8, 0x90, 0x90, 0x80 };

// clang-format off
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

void BTL_initializeCommandMenu(void)
{
	BTL_COMMAND_MENU_ACTIVE = 1;
	BTL_COMMAND_MENU_X = -0x98;
	BTL_COMMAND_MENU_Y = -0x78;
	BTL_COMMAND_MENU_BLINK = 0;
	BTL_COMMAND_MENU_TIMER = 0;
	BTL_COMMAND_MENU_X_STEP = 0;
	BTL_COMMAND_MENU_Y_STEP = 0;

	switch (COMBAT_DATA_PTR->player.numCommands[0]) {
	case 3:
		BTL_COMMAND_MENU_LAYOUT = 0;
		break;
	case 4:
		BTL_COMMAND_MENU_LAYOUT = 1;
		break;
	case 5:
		BTL_COMMAND_MENU_LAYOUT = 2;
		break;
	case 7:
		BTL_COMMAND_MENU_LAYOUT = 3;
		break;
	case 8:
		BTL_COMMAND_MENU_LAYOUT = 4;
		break;
	case 9:
		BTL_COMMAND_MENU_LAYOUT = 5;
		break;
	}

	BTL_COMMAND_MENU_CLOSED_FRAMES = 0;
	addObject(0x198, 0, (TickFunction)BTL_tickCommandMenu, BTL_renderCommandMenu);
}

void BTL_tickCommandMenu(void)
{
	BTL_COMMAND_MENU_TIMER++;

	if (BTL_COMMAND_MENU_ACTIVE == 1) {
		if (BTL_COMMAND_MENU_Y_STEP < 7) {
			BTL_COMMAND_MENU_Y_STEP++;
		}
		if ((BTL_COMMAND_MENU_Y_STEP == 7) && (BTL_COMMAND_MENU_X_STEP < 7)) {
			BTL_COMMAND_MENU_X_STEP++;
		}
		BTL_COMMAND_MENU_Y = BTL_COMMAND_MENU_SLIDE_Y[BTL_COMMAND_MENU_Y_STEP];
		BTL_COMMAND_MENU_X = BTL_COMMAND_MENU_SLIDE_X[BTL_COMMAND_MENU_X_STEP];
		if ((GAME_STATE == 1) && ((BTL_COMMAND_MENU_TIMER % 8) == 0)) {
			BTL_COMMAND_MENU_BLINK = (BTL_COMMAND_MENU_BLINK + 1) & 1;
		}
	} else {
		if (BTL_COMMAND_MENU_X_STEP != 0) {
			BTL_COMMAND_MENU_X_STEP--;
		}
		if ((BTL_COMMAND_MENU_X_STEP == 0) && (BTL_COMMAND_MENU_Y_STEP != 0)) {
			BTL_COMMAND_MENU_Y_STEP--;
		}
		BTL_COMMAND_MENU_Y = BTL_COMMAND_MENU_SLIDE_Y[BTL_COMMAND_MENU_Y_STEP];
		BTL_COMMAND_MENU_X = BTL_COMMAND_MENU_SLIDE_X[BTL_COMMAND_MENU_X_STEP];
	}
}

void BTL_renderCommandMenu(arg0)
int16_t arg0;
{
	POLY_FT4 *prim;
	int32_t i;
	int16_t x;
	int16_t rowY;
	int16_t width;
	int16_t base;
	int16_t count;

	base = ((COMBAT_DATA_PTR->player.numCommands[0] - 1) * 0xe) - 0x8c;
	if (GAME_STATE == 1) {
		BTL_drawHoveredCommandName();
	}

	prim = (POLY_FT4 *)GsGetWorkBase();
	count = COMBAT_DATA_PTR->player.numCommands[0] - 1;

	if (GAME_STATE == 1) {
		x = base - (COMBAT_DATA_PTR->player.hoveredCommand[0] * 0xe);
		SetPolyFT4(prim);
		prim->tpage = getTPage(0, 0, 960, 256);
		setClut(prim, 282, 497);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVDataPolyFT4(prim, 0x3d, 0xe0, 0x16, 0x16);
		if ((count % 2) == 0) {
			int32_t y;

			if ((COMBAT_DATA_PTR->player.hoveredCommand[0] % 2) == 0) {
				y = -0x64;
			} else {
				y = -0x5a;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		} else {
			int32_t y;

			if ((COMBAT_DATA_PTR->player.hoveredCommand[0] % 2) == 1) {
				y = -0x64;
			} else {
				y = -0x5a;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	for (i = 1; i < COMBAT_DATA_PTR->player.numCommands[0]; i++) {
		SetPolyFT4(prim);
		prim->tpage = getTPage(0, 0, 960, 256);
		setClut(prim, 272, 496);
		setRGB0(prim, 0x80, 0x80, 0x80);
		BTL_setCommandIconUV((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[arg0]], prim, COMBAT_DATA_PTR->player.availableCommands[0][i]);
		if (i > 0) {
			rowY = ((i - 1) * 0xe) + 0xb;
		} else {
			rowY = 0;
		}
		if ((i == 0) || (i == (COMBAT_DATA_PTR->player.numCommands[0] - 1))) {
			width = 0xb;
		} else {
			width = 0xe;
		}
		x = (int16_t)base - (i * 0xe);
		if ((count % 2) == 0) {
			setXY4(prim, x, ((i % 2) == 0) ? BTL_COMMAND_MENU_X + 3 : BTL_COMMAND_MENU_X + 0xd, x + 0x10, ((i % 2) == 0) ? BTL_COMMAND_MENU_X + 3 : BTL_COMMAND_MENU_X + 0xd, x, ((i % 2) == 0) ? BTL_COMMAND_MENU_Y - 0xd : BTL_COMMAND_MENU_Y - 3, x + 0x10, ((i % 2) == 0) ? BTL_COMMAND_MENU_Y - 0xd : BTL_COMMAND_MENU_Y - 3);
		} else {
			setXY4(prim, x, ((i % 2) == 1) ? BTL_COMMAND_MENU_X + 3 : BTL_COMMAND_MENU_X + 0xd, x + 0x10, ((i % 2) == 1) ? BTL_COMMAND_MENU_X + 3 : BTL_COMMAND_MENU_X + 0xd, x, ((i % 2) == 1) ? BTL_COMMAND_MENU_Y - 0xd : BTL_COMMAND_MENU_Y - 3, x + 0x10, ((i % 2) == 1) ? BTL_COMMAND_MENU_Y - 0xd : BTL_COMMAND_MENU_Y - 3);
		}
		if ((i == COMBAT_DATA_PTR->player.hoveredCommand[0]) && (BTL_COMMAND_MENU_BLINK == 1)) {
			prim->u0 += 0x10;
			prim->u1 += 0x10;
			prim->u2 += 0x10;
			prim->u3 += 0x10;
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	for (i = 0; i < COMBAT_DATA_PTR->player.numCommands[0]; i++) {
		SetPolyFT4(prim);
		setSemiTrans(prim, 1);
		setTPage(prim, 0, 0, 960, 256);
		setClut(prim, 272, 497);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVWH(prim, BTL_COMMAND_LABEL_U[BTL_COMMAND_MENU_LAYOUTS[BTL_COMMAND_MENU_LAYOUT][i]], 0xe0, BTL_COMMAND_LABEL_W[BTL_COMMAND_MENU_LAYOUTS[BTL_COMMAND_MENU_LAYOUT][i]], 31);
		if (i > 0) {
			rowY = ((i - 1) * 0xe) + 0xb;
		} else {
			rowY = 0;
		}
		if ((i == 0) || (i == (COMBAT_DATA_PTR->player.numCommands[0] - 1))) {
			width = 0xb;
		} else {
			width = 0xe;
		}
		setXY4(prim, rowY - 0x8f, BTL_COMMAND_MENU_X, (rowY - 0x8f) + width, BTL_COMMAND_MENU_X, rowY - 0x8f, BTL_COMMAND_MENU_Y, (rowY - 0x8f) + width, BTL_COMMAND_MENU_Y);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void BTL_removeCommandMenu(void)
{
	if (BTL_COMMAND_MENU_ACTIVE != -1) {
		BTL_COMMAND_MENU_CLOSED_FRAMES = 0;
		BTL_COMMAND_MENU_ACTIVE = -1;
		removeObject(0x198, 0);
	}
}

int32_t BTL_isCommandMenuClosed(void)
{
	if ((BTL_COMMAND_MENU_X_STEP == 0) && (BTL_COMMAND_MENU_Y_STEP == 0)) {
		BTL_removeCommandMenu();
		BTL_COMMAND_MENU_CLOSED_FRAMES++;
		if (BTL_COMMAND_MENU_CLOSED_FRAMES >= 4) {
			return 1;
		}
	}

	return 0;
}

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
	sprite = &BTL_DEATH_COUNTDOWN_SPRITE;
	shadow = (GsSPRITE *)&BTL_DEATH_COUNTDOWN;
	shadow->cy = sprite->cy = 0x1ed;
	shadow->rotate = sprite->rotate = 0;
	shadow->x = sprite->x = pos.vx;
	shadow->y = sprite->y = pos.vy;
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
