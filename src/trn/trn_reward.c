#include <string.h>

#include <dw/anim.h>
#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/font.h>
#include <dw/item.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/trn.h>
#include <dw/types.h>

extern int16_t STATS_GAINS[6];
extern char *MOVE_NAMES[];

void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void createCameraMovement(VECTOR *pos, int32_t speed);
void initializeDaytimeTransition(int32_t phase);
void tickConditionBoundaries(void);
void createCloudFX(int16_t *pos);
int32_t hasMove(int32_t moveId);
void learnMove(int32_t moveId);
void createMenuBox(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, void (*tick)(void), void (*render)(void));
void TRN_func_800888A0(int8_t arg);
int16_t TRN_calculateTrainingMultiplier(int32_t type, int32_t mode, int32_t flag);
void TRN_advanceTrainingTime(int16_t tiredGain, int16_t energyLoss, int16_t happyLoss);
void TRN_renderNewOrdersBox(void);
int32_t TRN_tryLearnMove(int32_t type);
void TRN_renderMoveLearnedBox(void);

static void *trn_reward_functions[] = {
	TRN_renderMoveLearnedBox,
	TRN_tryLearnMove,
	TRN_renderNewOrdersBox,
	TRN_advanceTrainingTime,
	TRN_calculateTrainingMultiplier,
	TRN_awardBrainsTrainingGains,
	TRN_awardMpTrainingGains,
	TRN_awardDefenseTrainingGains,
	TRN_awardSpeedTrainingGains,
	TRN_awardOffenseTrainingGains,
	TRN_statGainsAreZero,
	TRN_awardHpTrainingGains,
	TRN_createCloudFXLine,
	TRN_func_800888A0,
	TRN_startSlotSessionIfEnabled,
	TRN_saveTrainingStartTime,
};

// clang-format off
int8_t TRAINING_ANIM_IDS[67][2] = {
	{ 0x0, 0x0 }, { 0x2e, 0xa }, { 0x2e, 0xa }, { 0x33, 0xc },
	{ 0x2e, 0x14 }, { 0x35, 0x14 }, { 0x2e, 0xa }, { 0x2e, 0xa },
	{ 0x2e, 0xa }, { 0x35, 0xd }, { 0x2e, 0xa }, { 0x2e, 0x14 },
	{ 0x31, 0xa }, { 0x2e, 0x8 }, { 0x2e, 0xa }, { 0x2e, 0xa },
	{ 0x2e, 0xa }, { 0x2e, 0x14 }, { 0x2e, 0xa }, { 0x2e, 0xa },
	{ 0x2e, 0xe }, { 0x2e, 0xf }, { 0x2e, 0xa }, { 0x2e, 0xa },
	{ 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0x8 },
	{ 0x33, 0xa }, { 0x2e, 0x14 }, { 0x2e, 0x14 }, { 0x2e, 0xa },
	{ 0x2e, 0xe }, { 0x2e, 0x14 }, { 0x31, 0xa }, { 0x2e, 0xa },
	{ 0x35, 0x14 }, { 0x2e, 0x14 }, { 0x2e, 0xa }, { 0x2e, 0x14 },
	{ 0x2e, 0xd }, { 0x2f, 0x1e }, { 0x2e, 0xf }, { 0x2e, 0xf },
	{ 0x2e, 0xa }, { 0x35, 0x17 }, { 0x2e, 0xa }, { 0x33, 0xa },
	{ 0x2e, 0xa }, { 0x2e, 0xa }, { 0x34, 0xa }, { 0x2e, 0xa },
	{ 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0xa },
	{ 0x30, 0x1e }, { 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0x19 },
	{ 0x30, 0xa }, { 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0xa },
	{ 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0xa },
};

char TRN_TEXT_GIVE_IT_ALL_YOU_GOT[] = "Give it all you got!";

char TRN_TEXT_TAKE_IT_EASY[] = "Take it easy!";

char TRN_TEXT_GET_BACK[] = "Get back!";

char TRN_TEXT_CHANGE_TARGET[] = "Change target!";

char TRN_TEXT_HANG_IN_THERE[] = "Hang in there!";

char TRN_TEXT_TECHNIQUE_MASTERED[20] = "Technique mastered.";

char TRN_TEXT_NEW_ORDERS[12] = "New orders!";

uint8_t TRN_D_8008F14C[56] = {
	0x2, 0x5, 0x0, 0x4, 0x3, 0x1, 0x6, 0x7,
	0x2b, 0x2c, 0x2a, 0x2e, 0x2d, 0x29, 0x28, 0x2f,
	0xc, 0xa, 0xd, 0x9, 0xb, 0xe, 0x8, 0xf,
	0x25, 0x26, 0x20, 0x23, 0x22, 0x24, 0x27, 0x21,
	0x17, 0x13, 0x14, 0x15, 0x12, 0x10, 0x11, 0x16,
	0x18, 0x1a, 0x1f, 0x1b, 0x1e, 0x1c, 0x19, 0x1d,
	0x37, 0x31, 0x32, 0x35, 0x36, 0x33, 0x34, 0x38,
};

int8_t TRN_D_8008F184[8][3] = {
	{ 0x0, 0xf, 0xa },
	{ 0x19, 0xd, 0x8 },
	{ 0x16, 0xb, 0x7 },
	{ 0x14, 0x9, 0x5 },
	{ 0x12, 0x8, 0x2 },
	{ 0xf, 0x7, 0x0 },
	{ 0xc, 0x6, 0x0 },
	{ 0xa, 0x5, 0x0 },
};

char TRN_TEXT_WAS_MASTERED[] = "was mastered!";
// clang-format on

void TRN_saveTrainingStartTime(void)
{
	MAIN_D_8013538C = CURRENT_FRAME;
	MAIN_D_8013538E = HOUR;
	MAIN_D_80135390 = MINUTE;
}

void TRN_startSlotSessionIfEnabled(int16_t arg)
{
	if (MAIN_D_80135370 == 1) {
		while (TRN_startSlotSession(arg) != 0) {
		}
	}
}

void TRN_func_800888A0(int8_t arg)
{
	if (MAIN_D_80135370 == 1) {
		if (TRN_getSlotSessionResult() == -2) {
			startAnimation(ENTITY_TABLE[1], 4);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			MAIN_D_80135371 = arg;
		}
	}
}

void TRN_createCloudFXLine(int16_t a, int16_t b, int16_t x, int16_t z, int16_t dx, int16_t dz, int8_t n)
{
	int16_t pos[3];
	int16_t d;
	int32_t i;
	int32_t r;

	if ((a % b) == 0) {
		if (a >= 1200) {
			a = 1200;
		}
		d = (a / 120) * b;
		for (i = 0; i < n; i++) {
			r = random(100);
			pos[0] = (x + d) - r;
			pos[1] = 0;
			pos[2] = (z + d) - r;
			createCloudFX(pos);
			x += dx;
			z += dz;
		}
	}
}

void TRN_awardHpTrainingGains(int32_t a, int16_t b, int32_t c)
{
	int16_t v;
	int16_t x;
	int16_t y;
	int16_t flag;

	v = TRN_calculateTrainingMultiplier(a, 0, b);
	flag = PARTNER_PARA.trainBoostFlag;
	if (flag & 0x10) {
		x = ((v * PARTNER_PARA.trainBoostValue) * 80) / 100;
	} else {
		x = (v * 80) / 10;
	}

	if (flag & 0x1) {
		y = ((v * PARTNER_PARA.trainBoostValue) * 2) / 100;
	} else {
		y = (v * 2) / 10;
	}

	x = (x * c) / 10;
	y = (y * c) / 10;
	if (PARTNER_PARA.tiredness == 100) {
		x = 1;
		y = 1;
	}

	if (x >= 1000) {
		x = 999;
	}

	TRN_saveBaseStats();
	STATS_GAINS[0] = x;
	STATS_GAINS[1] = 0;
	STATS_GAINS[2] = y;
	STATS_GAINS[3] = 0;
	STATS_GAINS[4] = 0;
	STATS_GAINS[5] = 0;
	TRN_createPostTrainingStatsBox();
	TRN_advanceTrainingTime(10, 1, 1);
}

int32_t TRN_statGainsAreZero(void)
{
	if (STATS_GAINS[0] != 0) {
		goto no;
	}

	if (STATS_GAINS[1] != 0) {
		goto no;
	}

	if (STATS_GAINS[2] != 0) {
		goto no;
	}

	if (STATS_GAINS[3] != 0) {
		goto no;
	}

	if (STATS_GAINS[4] != 0) {
		goto no;
	}

	if (STATS_GAINS[5] != 0) {
		goto no;
	}

	return 1;
no:
	return 0;
}

void TRN_awardOffenseTrainingGains(int32_t a, int32_t b, int32_t c)
{
	int16_t off;
	int16_t hp;
	int16_t spd;
	int16_t energy;
	int16_t tired;
	int16_t happy;
	int16_t v;
	int16_t flag;

	hp = off = spd = 0;
	tired = energy = happy = 0;
	switch (b) {
	case 0:
		off = 8;
		spd = 2;
		tired = 9;
		energy = 1;
		happy = 1;
		break;
	case 6:
		hp = 10;
		off = 8;
		tired = 9;
		energy = 1;
		happy = 1;
		break;
	}

	v = TRN_calculateTrainingMultiplier(a, 2, b);
	flag = PARTNER_PARA.trainBoostFlag;
	if (flag & 0x10) {
		hp = (hp * (v * PARTNER_PARA.trainBoostValue)) / 100;
	} else {
		hp = (hp * v) / 10;
	}

	if (flag & 1) {
		off = (off * (v * PARTNER_PARA.trainBoostValue)) / 100;
	} else {
		off = (off * v) / 10;
	}

	if (flag & 4) {
		spd = (spd * (v * PARTNER_PARA.trainBoostValue)) / 100;
	} else {
		spd = (spd * v) / 10;
	}

	hp = (hp * c) / 10;
	off = (off * c) / 10;
	spd = (spd * c) / 10;
	if (PARTNER_PARA.tiredness == 100) {
		if (b == 0) {
			spd = 1;
		} else {
			hp = 1;
		}
		off = 1;
	}

	TRN_saveBaseStats();
	STATS_GAINS[0] = hp;
	STATS_GAINS[1] = 0;
	STATS_GAINS[2] = off;
	STATS_GAINS[3] = 0;
	STATS_GAINS[4] = spd;
	STATS_GAINS[5] = 0;
	TRN_createPostTrainingStatsBox();
	TRN_advanceTrainingTime(tired, energy, happy);
}

void TRN_awardSpeedTrainingGains(int32_t a, int32_t b, int32_t c)
{
	int16_t x;
	int16_t y;
	int16_t energy;
	int16_t tired;
	int16_t happy;
	int16_t v;
	int16_t flag;

	x = y = 0;
	tired = energy = happy = 0;
	switch (b) {
	case 0:
		x = 10;
		y = 8;
		tired = 8;
		energy = 1;
		happy = 1;
		break;
	case 6:
		x = 10;
		y = 8;
		tired = 8;
		energy = 1;
		happy = 1;
		break;
	}

	v = TRN_calculateTrainingMultiplier(a, 4, b);
	flag = PARTNER_PARA.trainBoostFlag;
	if (flag & 0x10) {
		x = (x * (v * PARTNER_PARA.trainBoostValue)) / 100;
	} else {
		x = (x * v) / 10;
	}

	if (flag & 4) {
		y = (y * (v * PARTNER_PARA.trainBoostValue)) / 100;
	} else {
		y = (y * v) / 10;
	}

	x = (x * c) / 10;
	y = (y * c) / 10;
	if (PARTNER_PARA.tiredness == 100) {
		x = 1;
		y = 1;
	}

	if (x >= 1000) {
		x = 999;
	}

	TRN_saveBaseStats();
	STATS_GAINS[0] = x;
	STATS_GAINS[1] = 0;
	STATS_GAINS[2] = 0;
	STATS_GAINS[3] = 0;
	STATS_GAINS[4] = y;
	STATS_GAINS[5] = 0;
	TRN_createPostTrainingStatsBox();
	TRN_advanceTrainingTime(tired, energy, happy);
}

void TRN_awardDefenseTrainingGains(int32_t a, int16_t b, int32_t c)
{
	int16_t v;
	int16_t x;
	int16_t y;
	int16_t flag;

	v = TRN_calculateTrainingMultiplier(a, 3, b);
	flag = PARTNER_PARA.trainBoostFlag;
	if (flag & 2) {
		x = ((v * PARTNER_PARA.trainBoostValue) * 8) / 100;
	} else {
		x = (v * 8) / 10;
	}

	if (flag & 8) {
		y = ((v * PARTNER_PARA.trainBoostValue) * 2) / 100;
	} else {
		y = (v * 2) / 10;
	}

	x = (x * c) / 10;
	y = (y * c) / 10;
	if (PARTNER_PARA.tiredness == 100) {
		x = 1;
		y = 1;
	}

	TRN_saveBaseStats();
	STATS_GAINS[0] = 0;
	STATS_GAINS[1] = 0;
	STATS_GAINS[2] = 0;
	STATS_GAINS[3] = x;
	STATS_GAINS[4] = 0;
	STATS_GAINS[5] = y;
	TRN_createPostTrainingStatsBox();
	TRN_advanceTrainingTime(9, 1, 1);
}

void TRN_awardMpTrainingGains(int32_t a, int16_t b, int32_t c)
{
	int16_t v;
	int16_t x;
	int16_t y;
	int16_t flag;

	v = TRN_calculateTrainingMultiplier(a, 1, b);
	flag = PARTNER_PARA.trainBoostFlag;
	if (flag & 0x20) {
		x = ((v * PARTNER_PARA.trainBoostValue) * 80) / 100;
	} else {
		x = (v * 80) / 10;
	}

	if (flag & 0x2) {
		y = ((v * PARTNER_PARA.trainBoostValue) * 2) / 100;
	} else {
		y = (v * 2) / 10;
	}

	x = (x * c) / 10;
	y = (y * c) / 10;
	if (PARTNER_PARA.tiredness == 100) {
		x = 1;
		y = 1;
	}

	if (x >= 1000) {
		x = 999;
	}

	TRN_saveBaseStats();
	STATS_GAINS[0] = 0;
	STATS_GAINS[1] = x;
	STATS_GAINS[2] = 0;
	STATS_GAINS[3] = y;
	STATS_GAINS[4] = 0;
	STATS_GAINS[5] = 0;
	TRN_createPostTrainingStatsBox();
	TRN_advanceTrainingTime(7, 1, 1);
}

void TRN_awardBrainsTrainingGains(int32_t a, int32_t b, int32_t c)
{
	int16_t brains;
	int16_t mp;
	int16_t base;
	int16_t energy;
	int16_t tired;
	int16_t happy;
	int16_t v;
	int16_t flag;
	int32_t y;
	int32_t shown;

	base = PARTNER_ENTITY.digimonEntity.stats.base.brain;
	MAIN_D_80135388 = 0;
	mp = brains = 0;
	tired = energy = happy = 0;
	switch (b) {
	case 0:
		mp = 10;
		brains = 8;
		tired = 6;
		energy = 1;
		happy = 1;
		break;
	case 9:
		brains = 15;
		tired = 6;
		energy = 1;
		happy = 1;
		break;
	}

	v = TRN_calculateTrainingMultiplier(a, 1, b);
	flag = PARTNER_PARA.trainBoostFlag;
	if (flag & 0x20) {
		mp = (mp * (v * PARTNER_PARA.trainBoostValue)) / 100;
	} else {
		mp = (mp * v) / 10;
	}

	if (flag & 8) {
		brains = (brains * (v * PARTNER_PARA.trainBoostValue)) / 100;
	} else {
		brains = (brains * v) / 10;
	}

	mp = (mp * c) / 10;
	brains = (brains * c) / 10;
	if (PARTNER_PARA.tiredness == 100) {
		if (b == 0) {
			mp = 1;
		}
		brains = 1;
	}

	if (mp >= 1000) {
		mp = 999;
	}

	if (brains >= 1000) {
		brains = 999;
	}

	TRN_saveBaseStats();
	STATS_GAINS[0] = 0;
	STATS_GAINS[1] = mp;
	STATS_GAINS[2] = 0;
	STATS_GAINS[3] = 0;
	STATS_GAINS[4] = 0;
	STATS_GAINS[5] = brains;
	TRN_createPostTrainingStatsBox();
	TRN_advanceTrainingTime(tired, energy, happy);
	brains = base + brains;
	shown = 0;
	y = 0x84;
	if ((base < 0x64) && (brains >= 0x64)) {
		setTextColor(7);
		drawString(TRN_TEXT_GIVE_IT_ALL_YOU_GOT, 0, 0x78);
		shown = 1;
	}

	if ((base < 0xc8) && (brains >= 0xc8)) {
		setTextColor(7);
		drawString(TRN_TEXT_TAKE_IT_EASY, 0, 0x78);
		shown = 1;
	}

	if ((base < 0x12c) && (brains >= 0x12c)) {
		setTextColor(7);
		drawString(TRN_TEXT_GET_BACK, 0, 0x78);
		drawString(TRN_TEXT_CHANGE_TARGET, 0, 0x84);
		shown = 1;
		y = 0x94;
	}

	if ((base < 0x190) && (brains >= 0x190)) {
		setTextColor(7);
		drawString(TRN_TEXT_HANG_IN_THERE, 0, 0x78);
		shown = 1;
	}

	if ((base < 0x1f4) && (brains >= 0x1f4)) {
		setTextColor(7);
		drawString(TRN_TEXT_TECHNIQUE_MASTERED, 0, 0x78);
		shown = 1;
	}

	if (shown == 1) {
		setTextColor(1);
		drawString(TRN_TEXT_NEW_ORDERS, 0, y);
		createMenuBox(2, -0x58, 0x12, 0xb0, 0x30, 2, 0, TRN_renderNewOrdersBox);
		MAIN_D_80135388 = 1;
	} else if (((base < 0x96) && (brains >= 0x96)) || ((base < 0xfa) && (brains >= 0xfa)) || ((base < 0x15e) && (brains >= 0x15e)) || ((base < 0x1c2) && (brains >= 0x1c2)) || ((base < 0x226) && (brains >= 0x226)) || ((base < 0x28a) && (brains >= 0x28a)) || ((base < 0x2ee) && (brains >= 0x2ee)) || ((base < 0x320) && (brains >= 0x320)) || ((base < 0x352) && (brains >= 0x352)) || ((base < 0x384) && (brains >= 0x384)) || ((base < 0x3b6) && (brains >= 0x3b6)) || (brains == 999)) {
		TRN_tryLearnMove(a);
	}
}

int16_t TRN_calculateTrainingMultiplier(int32_t type, int32_t mode, int32_t flag)
{
	uint32_t num;
	uint32_t den;
	uint32_t counterIndex;
	uint8_t tt;

	tt = RAISE_DATA[type].trainingType;
	num = 0;
	counterIndex = mode;
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
	if (flag == 0) {
		if ((mode == 0) || (mode == 3) || (mode == 4)) {
			if (isTriggerSet(0xdb) == 1) {
				num = num * 6;
				den = den * 5;
			}
		}
		if ((mode == 2) || (mode == 1) || (mode == 5)) {
			if (isTriggerSet(0xfb) == 1) {
				num = num * 6;
				den = den * 5;
			}
		}
		if ((mode == 0) || (mode == 1) || (mode == 2)) {
			if ((&PARTNER_PARA.upgradeHPcounter)[mode] >= 10) {
				num = num * 6;
				den = den * 5;
			}
		}
		if ((mode == 3) || (mode == 4) || (mode == 5)) {
			if ((&PARTNER_PARA.upgradeDefenseCounter)[(int32_t)counterIndex - 3] >= 10) {
				num = num * 6;
				den = den * 5;
			}
		}
	}

	if (getItemCount(0x21) > 0) {
		num = num * 11;
		den = den * 10;
	}

	return (num * 10) / den;
}

void TRN_advanceTrainingTime(int16_t tiredGain, int16_t energyLoss, int16_t happyLoss)
{
	PARTNER_PARA.tiredness += tiredGain;
	PARTNER_PARA.energyLevel -= energyLoss;
	PARTNER_PARA.happiness -= happyLoss;
	if (PARTNER_PARA.tiredness >= 0x46) {
		PARTNER_PARA.happiness -= 3;
		PARTNER_PARA.discipline -= 3;
	}

	CURRENT_FRAME = MAIN_D_8013538C + 0x4b0;
	HOUR = MAIN_D_8013538E + 1;
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

	if (HOUR == 0x10) {
		initializeDaytimeTransition(0);
	} else if (HOUR == 0x14) {
		initializeDaytimeTransition(1);
	} else if (HOUR == 6) {
		initializeDaytimeTransition(2);
	}

	if (PARTNER_PARA.condition & 1) {
		PARTNER_PARA.sicknessCounter++;
		PARTNER_PARA.missedSleepHours++;
	}

	if (PARTNER_PARA.condition & 4) {
		PARTNER_PARA.starvationTimer -= 0x78;
		if (PARTNER_PARA.starvationTimer <= 0) {
			if (PARTNER_PARA.energyLevel < RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyThreshold) {
				PARTNER_PARA.careMistakes++;
			}
		}
	} else {
		PARTNER_PARA.foodLevel -= 0x3c;
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

void TRN_renderNewOrdersBox(void)
{
	renderString(0, -78, 24, 240, 48, 0, 120, 4, 0);
}

int32_t TRN_tryLearnMove(int32_t type)
{
	uint8_t special[3];
	int8_t moveId[3];
	int8_t weight[3];
	int32_t n;
	int32_t i;
	int32_t j;
	int32_t k;
	int8_t best;
	int8_t move;

	for (n = 0; n < 3; n++) {
		special[n] = DIGIMON_DATA[type].special[n];
	}

	weight[0] = moveId[0] = weight[1] = moveId[1] = weight[2] = moveId[2] = -1;
	for (i = 0; i < 3; i++) {
		if (special[i] == 0xff) {
			continue;
		}
		for (k = 0; k < 8; k++) {
			if (hasMove(TRN_D_8008F14C[k + special[i] * 8]) == 1) {
				continue;
			}
			for (j = 0; j < 16; j++) {
				if (TRN_D_8008F14C[k + special[i] * 8] == DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].moves[j]) {
					moveId[i] = TRN_D_8008F14C[k + special[i] * 8];
					weight[i] = TRN_D_8008F184[k][i];
				}
			}
			if ((moveId[i] != -1) && (weight[i] != -1)) {
				break;
			}
		}
	}

	best = weight[0];
	move = moveId[0];
	if (weight[0] < weight[1]) {
		best = weight[1];
		move = moveId[1];
	}

	if (best < weight[2]) {
		best = weight[2];
		move = moveId[2];
	}

	if (random(100) < best) {
		learnMove(move);
		drawString(MOVE_NAMES[move], 0, 0x78);
		MAIN_D_80135392 = strlen(MOVE_NAMES[move]);
		drawString(TRN_TEXT_WAS_MASTERED, 0, 0x84);
		createMenuBox(2, -0x58, 0x12, 0xb0, 0x23, 2, 0, TRN_renderMoveLearnedBox);
		MAIN_D_80135388 = 1;
		return 1;
	}

	return 0;
}

void TRN_renderMoveLearnedBox(void)
{
	renderString(3, -78, 24, (MAIN_D_80135392 / 2) * 12, 12, 0, 120, 4, 0);
	renderString(0, ((MAIN_D_80135392 / 2) * 12) - 78, 24, 12, 12, 0, 132, 4, 0);
	renderString(0, -78, 36, 48, 12, 12, 132, 4, 0);
}
