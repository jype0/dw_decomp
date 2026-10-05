#include <string.h>

#include <dw/item.h>
#include <dw/params.h>
#include <dw/script.h>
#include <dw/ui.h>

typedef struct {
	int32_t v[6];
} Pow10Table;

int32_t scriptCompareSignedValue(uint8_t op, uint32_t lhs, uint32_t rhs);
int32_t getSpeakerName(int32_t speakerId, uint8_t *buf);
uint8_t *intToStringSJIS(uint8_t *buf, int32_t value, uint8_t digits, int32_t flag);

static void *script_value_functions[] = {
	enforceStatsLimits,
	scriptCompareMoney,
	scriptCompareItemCount,
	scriptCompareCondition,
	scriptCompareMove,
	scriptCompareCard,
	getStatsPointer,
	scriptCompareStat,
	scriptCompareDate,
	scriptIdToEntityId,
	intToStringSJIS,
	getSpeakerName,
};

// clang-format off
#if defined(VERSION_JP)
char MAIN_D_801345CC[] = "かんばん";

char MAIN_D_801345D4[] = "はこ";

char MAIN_D_801345D8[] = "ベタモン";

char MAIN_D_8013030C[] = "シーラモン";

char MAIN_D_801345E0[] = "タネモン";

char MAIN_D_801345E8[] = "パルモン";
#else
char MAIN_D_801345CC[] = "Sign";

char MAIN_D_801345D4[4] = "Box";

char MAIN_D_801345D8[8] = "Betamon";

char MAIN_D_8013030C[] = "Coelamon";

char MAIN_D_801345E0[8] = "Tanemon";

char MAIN_D_801345E8[] = "Palmon";
#endif

int16_t STATS_LIMIT_ARRAY[22] = {
	0x03e7, 0x03e7, 0x03e7, 0x03e7, 0x270f, 0x270f, 0x270f, 0x270f,
	0x0064, 0x0064, 0x03e7, 0x03e7, 0x03e7, 0x03e7, 0x270f, 0x270f,
	0x270f, 0x03e7, 0x03e7, 0x03e7, 0x0063, 0x000a,
};

Pow10Table MULTIPLE_OF_10 = {
	{
		0x00000001, 0x0000000a, 0x00000064, 0x000003e8,
		0x00002710, 0x000186a0,
	},
};

char *SPECIAL_SPEAKERS[6] = {
	MAIN_D_801345CC,
	MAIN_D_801345D4,
	MAIN_D_801345D8,
	MAIN_D_8013030C,
	MAIN_D_801345E0,
	MAIN_D_801345E8,
};
// clang-format on

// clang-format off
int32_t getSpeakerName(speakerId, buf)
	uint8_t speakerId;
	uint8_t *buf;
// clang-format on
{
	if (speakerId == 0xff) {
		return 0;
	}

	if (speakerId == 0xfd) {
		speakerId = 0;
		goto digimon;
	}

	if (speakerId == 0xfc) {
		strcpy((char *)buf, PARTNER_ENTITY.name);

		return strlen(PARTNER_ENTITY.name);
	}

	if (speakerId < 0xc8) {
		speakerId = scriptIdToEntityId(speakerId);
		speakerId = ENTITY_TABLE[speakerId]->type;
		goto digimon;
	}

	speakerId = speakerId - 0xc8;
	strcpy((char *)buf, SPECIAL_SPEAKERS[speakerId]);

	return strlen(SPECIAL_SPEAKERS[speakerId]);

digimon:
	strcpy((char *)buf, DIGIMON_DATA[speakerId].name);

	return strlen(DIGIMON_DATA[speakerId].name);
}

uint8_t *intToStringSJIS(uint8_t *buf, int32_t value, uint8_t digits, int32_t flag)
{
	Pow10Table divs;
	uint16_t base;
	int32_t started;
	uint16_t c;

	divs = MULTIPLE_OF_10;
	started = 0;
	base = 0x824f;
	while (digits != 0) {
		c = base + ((value / divs.v[digits - 1]) & 0xffff);
		value = value % divs.v[digits - 1];
		if (digits != 1) {
			if (c == base) {
				if (started == 0) {
					if (flag != 0) {
						goto skip;
					}
					c = 0x8140;
				}
			} else {
				started = 1;
			}
		}
		*buf++ = c >> 8;
		*buf++ = c & 0xff;
skip:
		digits--;
	}

	return buf;
}

uint8_t scriptIdToEntityId(int32_t scriptId)
{
	uint8_t i;

	if (scriptId == 0xfd) {
		return 0;
	}

	if (scriptId == 0xfc) {
		return 1;
	}

	for (i = 0; i < 8; i++) {
		if (ENTITY_TABLE[i + 2] != 0 && scriptId == NPC_ENTITIES[i].scriptId) {
			return (uint8_t)(i + 2);
		}
	}

	return 0xff;
}

void scriptCompareDate(void)
{
	uint16_t trigger;
	uint8_t statIdx;
	uint8_t op;
	uint8_t years;
	uint8_t days;
	uint8_t hours;
	uint8_t minutes;
	uint32_t now;
	uint32_t date;
	int32_t res;

	pollNextScriptUByte(&statIdx);
	pollNextScriptUShort(&trigger);
	pollNextTwoScriptBytes(&op, &years);
	pollNextTwoScriptBytes(&days, &hours);
	pollNextScriptUByte(&minutes);

	SCRIPT_POINTER = (uint8_t *)(SCRIPT_POINTER + 1);
	now = dateToSeconds(readPStat(statIdx + 0), readPStat(statIdx + 1), readPStat(statIdx + 2), readPStat(statIdx + 3));
	date = dateToSeconds(years, days, hours, minutes);
	res = scriptCompareValue(op, now, date);

	if (res != 0) {
		setTrigger(trigger);
	} else {
		unsetTrigger(trigger);
	}
}

int32_t scriptCompareStat(void)
{
	uint16_t u;
	int16_t s;
	uint8_t b1;
	uint8_t b2;
	uint16_t stat;
	int16_t stat2;

	pollNextTwoScriptBytes(&b1, &b2);

	if (b1 != 9) {
		stat = *getStatsPointer(b1);
		pollNextScriptUShort(&u);

		return scriptCompareValue(b2, stat, u);
	}

	stat2 = *getStatsPointer(b1);
	pollNextScriptShort(&s);

	return scriptCompareSignedValue(b2, stat2, s);
}

// clang-format off
int16_t *getStatsPointer(stat)
	uint8_t stat;
// clang-format on
{
	int16_t *ptr;

	switch (stat) {
	case SCRIPT_STAT_OFFENSE:
		ptr = &PARTNER_ENTITY.digimonEntity.stats.base.off;
		break;
	case SCRIPT_STAT_DEFENSE:
		ptr = &PARTNER_ENTITY.digimonEntity.stats.base.def;
		break;
	case SCRIPT_STAT_SPEED:
		ptr = &PARTNER_ENTITY.digimonEntity.stats.base.speed;
		break;
	case SCRIPT_STAT_BRAINS:
		ptr = &PARTNER_ENTITY.digimonEntity.stats.base.brain;
		break;
	case SCRIPT_STAT_MAX_HP:
		ptr = &PARTNER_ENTITY.digimonEntity.stats.base.hp;
		break;
	case SCRIPT_STAT_MAX_MP:
		ptr = &PARTNER_ENTITY.digimonEntity.stats.base.mp;
		break;
	case SCRIPT_STAT_CURRENT_HP:
		ptr = &PARTNER_ENTITY.digimonEntity.stats.current.currentHP;
		break;
	case SCRIPT_STAT_CURRENT_MP:
		ptr = &PARTNER_ENTITY.digimonEntity.stats.current.currentMP;
		break;
	case SCRIPT_STAT_TIREDNESS:
		ptr = &PARTNER_PARA.tiredness;
		break;
	case SCRIPT_STAT_HAPPINESS:
		ptr = &PARTNER_PARA.happiness;
		break;
	case SCRIPT_STAT_DISCIPLINE:
		ptr = &PARTNER_PARA.discipline;
		break;
	case SCRIPT_STAT_ENERGY:
		ptr = &PARTNER_PARA.energyLevel;
		break;
	case SCRIPT_STAT_VIRUS:
		ptr = &PARTNER_PARA.virusBar;
		break;
	case SCRIPT_STAT_LIFETIME:
		ptr = &PARTNER_PARA.remainingLifetime;
		break;
	case SCRIPT_STAT_MERIT:
		ptr = &MERIT;
		break;
	case SCRIPT_STAT_STARTED_BATTLES:
		ptr = &BATTLES_STARTED;
		break;
	case SCRIPT_STAT_FLED_BATTLES:
		ptr = &BATTLES_FLED;
		break;
	case SCRIPT_STAT_TOURNAMENTS_WON:
		ptr = &TOURNAMENTS_WON;
		break;
	case SCRIPT_STAT_TOURNAMENT_WINS:
		ptr = &TOURNAMENT_WINS;
		break;
	case SCRIPT_STAT_TOURNAMENTS_LOST:
		ptr = &TOURNAMENTS_LOST;
		break;
	case SCRIPT_STAT_WEIGHT:
		ptr = &PARTNER_PARA.weight;
		break;
	case SCRIPT_STAT_TAMER_LEVEL:
		TMP_TAMER_LEVEL = (int32_t)TAMER_ENTITY.tamerLevel;
		ptr = &TMP_TAMER_LEVEL;
		break;
	case SCRIPT_STAT_LIVES:
		TMP_LIVES = (int32_t)PARTNER_ENTITY.lives;
		ptr = &TMP_LIVES;
		break;
	}

	return ptr;
}

int32_t scriptCompareCard(void)
{
	uint16_t value;
	uint8_t cardId;
	uint8_t op;
	uint8_t amount;

	pollNextTwoScriptBytes(&cardId, &op);
	pollNextScriptUShort(&value);
	amount = getCardAmount(cardId);

	return scriptCompareValue(op, amount, value);
}

int32_t scriptCompareMove(void)
{
	uint8_t moveId;
	uint8_t negate;
	int32_t res;

	pollNextTwoScriptBytes(&moveId, &negate);
	res = _hasMove(moveId);
	if (negate == 0) {
		return res;
	}

	return (res != 0) ^ 1;
}

int32_t scriptCompareCondition(void)
{
	uint8_t mask;
	uint8_t negate;

	pollNextTwoScriptBytes(&mask, &negate);

	if (negate == 0) {
		if (PARTNER_PARA.condition & mask) {
			return 1;
		}

		return 0;
	} else {
		if ((PARTNER_PARA.condition & mask) == 0) {
			return 1;
		}

		return 0;
	}
}

int32_t scriptCompareItemCount(void)
{
	uint16_t value;
	uint8_t itemId;
	uint8_t op;
	uint8_t count;

	pollNextTwoScriptBytes(&itemId, &op);
	pollNextScriptUShort(&value);
	count = getItemCount(itemId);

	return scriptCompareValue(op, count, value);
}

int16_t enforceStatsLimits(uint8_t stat, int16_t value)
{
	int16_t cap;

	if (stat == SCRIPT_STAT_CURRENT_HP) {
		cap = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	} else if (stat == SCRIPT_STAT_CURRENT_MP) {
		cap = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	} else {
		cap = STATS_LIMIT_ARRAY[stat];
	}

	if (cap < value) {
		return cap;
	}

	return value;
}

int32_t scriptCompareMoney(void)
{
	int32_t value;
	uint8_t op;

	SCRIPT_POINTER = (uint8_t *)(SCRIPT_POINTER + 1);

	pollNextScriptUByte(&op);
	pollNextInt(&value);

	return scriptCompareValue(op, MONEY, value);
}
