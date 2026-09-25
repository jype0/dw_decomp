#include <libgpu.h>
#include <libgs.h>

#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/font.h>
#include <dw/item.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/training.h>
#include <dw/trn2.h>
#include <dw/types.h>
#include <dw/ui.h>

extern int16_t STATS_GAINS[6];
extern int16_t INITIAL_COMBAT_STATS[][6];
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern char STAT_LABELS[];
extern char FULLWIDTH_DIGITS[];
extern GsOT *ACTIVE_ORDERING_TABLE;

void worldPosToScreenPos(TrainingSpot *item, SVECTOR *out);
void renderLinePrimitive(uint32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t order, uint32_t mode);
void renderNumber(int32_t a, int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void renderTrianglePrimitive(int32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t otz, int32_t flag);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t x, int32_t y, int32_t w, int32_t h);
void TRN2_tickPostTrainingStatsBox();
void setEntityTextDigit(POLY_FT4 *poly, int32_t x, int32_t y);
void setUVDataPolyFT4(POLY_FT4 *p, int32_t u, int32_t v, int32_t w, int32_t h);
void tickConditionBoundaries(void);
void TRN2_renderPostTrainingStatsBox(uint8_t depth);
int32_t TRN2_getTrainingSpotScreenPos(int32_t key, int16_t sub, SVECTOR *out);

static void *trn2_hud_functions[] = {
	TRN2_getTrainingSpotScreenPos,
	TRN2_renderSlotSession,
	TRN2_tickSlotSession,
	TRN2_closeUIBox,
	TRN2_renderPostTrainingStatsBox,
	TRN2_tickPostTrainingStatsBox,
	TRN2_createPostTrainingStatsBox,
	TRN2_applyBaseStats,
	TRN2_saveBaseStats,
	TRN2_advanceTrainingTime,
	TRN2_calculateTrainingMultiplier,
};

// clang-format off
int8_t TRN2_D_8008DAA8[3][13] = {
	{ 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x1, 0x3, 0x5, 0x2, 0x4, 0x6 },
	{ 0x6, 0x4, 0x2, 0x5, 0x3, 0x1, 0x2, 0x5, 0x3, 0x6, 0x1, 0x4, 0x7 },
	{ 0x7, 0x6, 0x5, 0x4, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x3, 0x2, 0x1 },
};

TrainingSpot TRN2_D_8008DAD0[7] = {
	{ 0x3f2, 0x0, -0x3cf, 0x0 },
	{ -0x1fa, 0x0, -0x791, 0x2 },
	{ -0x315, 0x0, -0x36b, 0x3 },
	{ 0x598, 0x0, 0x4fc, 0x1 },
	{ 0xc, 0x0, 0x1bd, 0x4 },
	{ -0x6f3, 0x0, 0x824, 0x5 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

TrainingSpot TRN2_D_8008DB08[3] = {
	{ -0x33d, 0x0, 0x1003, 0x2 },
	{ 0x2fe, 0x0, 0x6a, 0x4 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

TrainingSpot TRN2_D_8008DB20[2] = {
	{ 0x59a, 0x0, -0x11b, 0x1 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

TrainingSpot TRN2_D_8008DB30[3] = {
	{ -0x52b, 0x0, 0x540, 0x2 },
	{ -0x228, 0x0, 0x6be, 0x0 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

TrainingSpot TRN2_D_8008DB48[3] = {
	{ 0x2ee, 0x0, 0x7f8, 0x4 },
	{ -0xcc, 0x0, 0x962, 0x3 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

TrainingSpot TRN2_D_8008DB60[3] = {
	{ 0x5e9, 0x0, 0x5b, 0x2 },
	{ 0x0, 0x0, 0x3e8, 0x0 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

TrainingSpot TRN2_D_8008DB78[2] = {
	{ -0x7e1, 0x0, 0x562, 0x5 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

MapTrainingSpots TRN2_TRAINING_SPOTS[8] = {
	{ 0x70, TRN2_D_8008DAD0 },
	{ 0x4e, TRN2_D_8008DB08 },
	{ 0xa5, TRN2_D_8008DB20 },
	{ 0x6b, TRN2_D_8008DB30 },
	{ 0x6c, TRN2_D_8008DB48 },
	{ 0x63, TRN2_D_8008DB60 },
	{ 0x77, TRN2_D_8008DB78 },
	{ -0x1, NULL },
};
// clang-format on

int16_t TRN2_calculateTrainingMultiplier(int32_t type, int32_t mode)
{
	uint32_t num;
	uint32_t den;
	uint8_t tt;

	tt = RAISE_DATA[type].trainingType;
	num = 0;
	switch (mode) {
	case 0:
		if ((tt == 0) || (tt == 1)) {
			num = 10;
		} else if (tt == 2) {
			num = 9;
		} else if (tt == 3) {
			num = 11;
		} else {
			num = 8;
		}
		break;
	case 1:
		if ((tt == 0) || (tt == 3)) {
			num = 10;
		} else if (tt == 1) {
			num = 9;
		} else if (tt == 2) {
			num = 11;
		} else {
			num = 8;
		}
		break;
	case 2:
		if ((tt == 0) || (tt == 2)) {
			num = 10;
		} else if (tt == 1) {
			num = 11;
		} else if (tt == 3) {
			num = 9;
		} else {
			num = 8;
		}
		break;
	case 3:
		if ((tt == 0) || (tt == 1)) {
			num = 10;
		} else if (tt == 2) {
			num = 9;
		} else if (tt == 3) {
			num = 11;
		} else {
			num = 8;
		}
		break;
	case 4:
		if ((tt == 0) || (tt == 2)) {
			num = 10;
		} else if (tt == 1) {
			num = 11;
		} else if (tt == 3) {
			num = 9;
		} else {
			num = 8;
		}
		break;
	case 5:
		if ((tt == 0) || (tt == 3)) {
			num = 10;
		} else if (tt == 1) {
			num = 9;
		} else if (tt == 2) {
			num = 11;
		} else {
			num = 8;
		}
		break;
	}

	den = 10;
	if ((DIGIMON_DATA[type].level == 1) || (DIGIMON_DATA[type].level == 2)) {
		num = num * 9;
	} else if (DIGIMON_DATA[type].level == 3) {
		num = num * 10;
	} else if (DIGIMON_DATA[type].level == 4) {
		num = num * 11;
	} else {
		num = num * 12;
	}

	den = den * 10;
	if (getItemCount(0x21) > 0) {
		num = num * 11;
		den = den * 10;
	}

	return (num * 10) / den;
}

void TRN2_advanceTrainingTime(int16_t tiredGain, int16_t energyLoss, int16_t happyLoss)
{
	PARTNER_PARA.tiredness += tiredGain;
	PARTNER_PARA.energyLevel -= energyLoss;
	PARTNER_PARA.happiness -= happyLoss;
	if (PARTNER_PARA.tiredness >= 0x46) {
		PARTNER_PARA.happiness -= 3;
		PARTNER_PARA.discipline -= 3;
	}

	CURRENT_FRAME = MAIN_D_801353CE + 0x4b0;
	HOUR = MAIN_D_801353D0 + 1;
	if (HOUR >= 0x18) {
		PARTNER_PARA.age++;
		DAY++;
		dailyPStatTrigger();
		HOUR -= 0x18;
		CURRENT_FRAME -= 0x7080;
		if (DAY >= 0x1f) {
			DAY = 0;
			YEAR++;
			if (YEAR >= 0x64) {
				YEAR = 0;
			}
		}
	}

	updateMinuteHand(HOUR, MINUTE);
	if ((HOUR % 4) == 0) {
		if (PARTNER_PARA.happiness < 0x50) {
			PARTNER_PARA.remainingLifetime = PARTNER_PARA.remainingLifetime - (((PARTNER_PARA.happiness - 0x50) / -50) + 1);
		}
	}

	if (PARTNER_PARA.condition & 1) {
		PARTNER_PARA.sicknessCounter++;
		PARTNER_PARA.missedSleepHours++;
	}

	if (PARTNER_PARA.condition & 4) {
		PARTNER_PARA.starvationTimer -= 0xc;
		if (PARTNER_PARA.starvationTimer <= 0) {
			if (PARTNER_PARA.energyLevel < RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyThreshold) {
				PARTNER_PARA.careMistakes++;
			}
		}
	} else {
		PARTNER_PARA.foodLevel -= 0x3d;
	}

	PARTNER_PARA.energyLevel -= RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyUsage;
	if (PARTNER_PARA.condition & 8) {
		PARTNER_PARA.poopingTimer -= 0x4b0;
	} else {
		PARTNER_PARA.poopLevel -= 6;
	}

	PARTNER_PARA.remainingLifetime--;
	PARTNER_PARA.evoTimer++;
	if (PARTNER_PARA.remainingLifetime < 0) {
		PARTNER_PARA.remainingLifetime = 0;
	}

	if (PARTNER_PARA.condition & 0x20) {
		PARTNER_PARA.injuryTimer++;
	}

	if (PARTNER_PARA.condition & 0x40) {
		PARTNER_PARA.sicknessTimer++;
	}

	tickConditionBoundaries();
}

void TRN2_saveBaseStats(void)
{
	INITIAL_COMBAT_STATS[0][0] = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	INITIAL_COMBAT_STATS[0][1] = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	INITIAL_COMBAT_STATS[0][2] = PARTNER_ENTITY.digimonEntity.stats.base.off;
	INITIAL_COMBAT_STATS[0][3] = PARTNER_ENTITY.digimonEntity.stats.base.def;
	INITIAL_COMBAT_STATS[0][4] = PARTNER_ENTITY.digimonEntity.stats.base.speed;
	INITIAL_COMBAT_STATS[0][5] = PARTNER_ENTITY.digimonEntity.stats.base.brain;
}

void TRN2_applyBaseStats(void)
{
	PARTNER_ENTITY.digimonEntity.stats.base.hp = INITIAL_COMBAT_STATS[0][0];
	PARTNER_ENTITY.digimonEntity.stats.base.mp = INITIAL_COMBAT_STATS[0][1];
	PARTNER_ENTITY.digimonEntity.stats.base.off = INITIAL_COMBAT_STATS[0][2];
	PARTNER_ENTITY.digimonEntity.stats.base.def = INITIAL_COMBAT_STATS[0][3];
	PARTNER_ENTITY.digimonEntity.stats.base.speed = INITIAL_COMBAT_STATS[0][4];
	PARTNER_ENTITY.digimonEntity.stats.base.brain = INITIAL_COMBAT_STATS[0][5];
}

void TRN2_createPostTrainingStatsBox(void)
{
	int32_t i;
	int32_t row;
	int32_t y;
	int16_t screenPos[2];
	RECT finalPos;
	RECT startPos;

	MAIN_D_801353D4 = 0;
	clearTextArea();

	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] == 0) {
			MAIN_D_801353D8[i] = 0;
		} else {
			MAIN_D_801353D8[i] = 1;
		}
	}

	for (i = 0; i < 4; i++) {
		y = i * 12;
		row = i * 2;

		if (i < 3) {
			drawString(&STAT_LABELS[row * 12], 0, y * 2);
			drawString(&STAT_LABELS[(row + 1) * 12], 0, (row + 1) * 12);
		}
		if (i == 3) {
			drawString(TRN2_TEXT_MONEY_JP, 0, 84);
			drawString(FULLWIDTH_DIGITS, 0, 240);
		}
		DrawSync(0);
	}

	MAIN_D_801353DE = 100;

	setRECT(&finalPos, -94, -78, 188, 96);

	getEntityScreenPos(ENTITY_TABLE[1], 1, screenPos);

	setRECT(&startPos, screenPos[0] - 5, screenPos[1] - 5, 10, 10);
	createAnimatedUIBox(1, 0, 2, &finalPos, &startPos, TRN2_tickPostTrainingStatsBox, (RenderFunction)TRN2_renderPostTrainingStatsBox);
}

void TRN2_tickPostTrainingStatsBox(void)
{
	int32_t i;
	int16_t gain;

	if (MAIN_D_801353DE > 0) {
		MAIN_D_801353DE--;
	}

	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] != 0) {
			break;
		}
	}

	if ((POLLED_INPUT == 0x40) || (POLLED_INPUT == 0x10)) {
		if (!(POLLED_INPUT_PREVIOUS & 0x40) && !(POLLED_INPUT_PREVIOUS & 0x10)) {
			MAIN_D_801353D4 = 1;
		}
	}

	if (MAIN_D_801353D4 != 1) {
		return;
	}

	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] != 0) {
			gain = STATS_GAINS[i];
			INITIAL_COMBAT_STATS[0][i] += gain;

			if (gain > 0) {
				if (i < 2) {
					if (INITIAL_COMBAT_STATS[0][i] >= 10000) {
						INITIAL_COMBAT_STATS[0][i] = 9999;
					}
				} else {
					if (INITIAL_COMBAT_STATS[0][i] >= 1000) {
						INITIAL_COMBAT_STATS[0][i] = 999;
					}
				}
			}
			STATS_GAINS[i] = 0;
		}
	}

	MAIN_D_801353DE = 0;
}

void TRN2_renderPostTrainingStatsBox(uint8_t depth)
{
	RECT *box = &UI_BOX_DATA[1].finalPos;
	GsBOXF rect;
	int32_t i;
	int32_t first;
	int16_t y;
	int32_t hasStatGain;

	first = 1;
	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] == 0) {
			continue;
		}
		if ((MAIN_D_801353DE == 0) && (STATS_GAINS[i] > 0) && (first == 1)) {
			playSound(0, 0x16);
			STATS_GAINS[i] = STATS_GAINS[i] - 1;
			INITIAL_COMBAT_STATS[0][i] += 1;

			if (i < 2) {
				if (INITIAL_COMBAT_STATS[0][i] >= 10000) {
					INITIAL_COMBAT_STATS[0][i] = 9999;
				}
			} else {
				if (INITIAL_COMBAT_STATS[0][i] >= 1000) {
					INITIAL_COMBAT_STATS[0][i] = 999;
				}
			}
			first = 0;
		}
		if (STATS_GAINS[i] != 0) {
			POLY_FT4 *prim = (POLY_FT4 *)GsGetWorkBase();
			setEntityTextDigit(prim, 256, 491);
			setRGB0(prim, 0x80, 0x80, 0x80);
			setUVDataPolyFT4(prim, 96, 180, 12, 12);
			setPosDataPolyFT4(prim, box->x + 130, (box->y + 9) + (i * 13), 12, 12);
			AddPrim((ACTIVE_ORDERING_TABLE->org + 6) - depth, prim++);
			GsSetWorkBase((PACKET *)prim);
		}
		renderNumber(5, box->x + 142, (box->y + 9) + (i * 13), 3, STATS_GAINS[i], 6 - depth);
	}

	for (i = 0; i < 6; i++) {
		renderNumber(0, box->x + 68, (box->y + 9) + (i * 13), 4, INITIAL_COMBAT_STATS[0][i], 6 - depth);
	}

	renderLinePrimitive(0xfad990, box->x + 122, box->y + 2, box->x + 122, (box->y + box->h) - 3, 6 - depth, 0);
	renderLinePrimitive(0x20202, box->x + 123, box->y + 2, box->x + 123, (box->y + box->h) - 3, 6 - depth, 0);

	for (i = 0; i < 6; i++) {
		renderString(4, box->x + 10, (box->y + 9) + (i * 13), 48, 12, 0, i * 12, 6 - depth, 0);
	}

	rect.attribute = 0;
	rect.x = box->x + 66;
	rect.h = 2;

	for (i = 0; i < 6; i++) {
		y = (box->y + 20) + (i * 13);
		rect.y = y - 2;

		if (i < 2) {
			rect.w = (INITIAL_COMBAT_STATS[0][i] * 50) / 9999;
		} else {
			rect.w = (INITIAL_COMBAT_STATS[0][i] * 50) / 999;
		}
		hasStatGain = MAIN_D_801353D8[i];
		if (hasStatGain != 1) {
			if (hasStatGain == 0) {
				rect.r = rect.g = rect.b = 0x78;
				GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, (uint16_t)(6 - depth));
				rect.r = rect.g = rect.b = 0x28;
			}
		} else {
			rect.r = 0x69;
			rect.g = 0xc2;
			rect.b = 0xff;
			GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, (uint16_t)(6 - depth));
			rect.r = 0;
			rect.g = 0x5a;
			rect.b = 0x96;
		}
		rect.w = 50;

		renderTrianglePrimitive(0x20202, box->x + 64, y, box->x + 64, y - 3, box->x + 117, y - 3, 6 - depth, 0);
		renderTrianglePrimitive(0x666666, box->x + 117, y - 2, box->x + 117, y, box->x + 65, y, 6 - depth, 0);
		GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, (uint16_t)(6 - depth));
	}
}

void TRN2_closeUIBox(int32_t id)
{
	if (UI_BOX_DATA[id].state != 0) {
		removeAnimatedUIBox(id, NULL);
	}
}

void TRN2_tickSlotSession(void)
{
	int16_t *p = MAIN_D_801353E0;

	switch (p[1]) {
	case 0:
		TRN2_createSlotMachineBox(p[2]);
		p[1] = 1;
		break;
	case 1:
		p[3] = TRN2_getSlotMachineResult();
		if (p[3] != -1) {
			p[1] = 2;
		}
		break;
	case 2:
		if (UI_BOX_DATA[3].state == 0) {
			p[0] = p[3];
			removeObject(0xfdc, 0);
		}
		break;
	}
}

void TRN2_renderSlotSession(void)
{
}

int32_t TRN2_getTrainingSpotScreenPos(int32_t key, int16_t sub, SVECTOR *out)
{
	int32_t i;
	int32_t id;
	TrainingSpot *p;

	i = 0;
	for (;;) {
		id = TRN2_TRAINING_SPOTS[i].mapId;
		if (id == key) {
			break;
		}
		if (id == -1) {
			return -1;
		}
		i++;
	}

	p = TRN2_TRAINING_SPOTS[i].spots;
	for (;;) {
		if (p->stat == sub) {
			break;
		}
		if (p->stat == -1) {
			return -1;
		}
		p++;
	}

	worldPosToScreenPos(p, out);
	return 0;
}
