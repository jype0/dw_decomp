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
char MAIN_D_801345CC[] = "Sign";

char MAIN_D_801345D4[4] = "Box";

char MAIN_D_801345D8[8] = "Betamon";

char MAIN_D_801345E0[8] = "Tanemon";

char MAIN_D_801345E8[] = "Palmon";

char MAIN_D_8013030C[] = "Coelamon";

int16_t MAIN_D_80130318[22] = {
	0x03e7, 0x03e7, 0x03e7, 0x03e7, 0x270f, 0x270f, 0x270f, 0x270f,
	0x0064, 0x0064, 0x03e7, 0x03e7, 0x03e7, 0x03e7, 0x270f, 0x270f,
	0x270f, 0x03e7, 0x03e7, 0x03e7, 0x0063, 0x000a,
};

Pow10Table MAIN_D_80130344 = {
	{
		0x00000001, 0x0000000a, 0x00000064, 0x000003e8,
		0x00002710, 0x000186a0,
	},
};

char *MAIN_D_8013035C[6] = {
	MAIN_D_801345CC,
	MAIN_D_801345D4,
	MAIN_D_801345D8,
	MAIN_D_8013030C,
	MAIN_D_801345E0,
	MAIN_D_801345E8,
};
// clang-format on

int32_t getSpeakerName(int32_t speakerId, uint8_t *buf)
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

	if ((uint32_t)speakerId < 0xc8) {
		speakerId = scriptIdToEntityId(speakerId) & 0xff;
		speakerId = ENTITY_TABLE[speakerId]->type & 0xff;
		goto digimon;
	}

	speakerId = (speakerId - 0xc8) & 0xff;
	strcpy((char *)buf, MAIN_D_8013035C[speakerId]);

	return strlen(MAIN_D_8013035C[speakerId]);

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
	int32_t hi;
	int32_t lo;

	divs = MAIN_D_80130344;
	base = 0x824f;
	started = 0;
	while (digits != 0) {
		c = value / divs.v[digits - 1];
		c = base + c;
		value = value % divs.v[digits - 1];
		if (digits != 1) {
			if (c == 0x824f) {
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
		hi = c >> 8;
		lo = c;
		*buf++ = hi;
		*buf++ = lo;
skip:
		digits--;
	}

	return buf;
}

int32_t scriptIdToEntityId(int32_t scriptId)
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
	uint32_t v1;
	uint32_t v2;
	uint32_t v3;

	pollNextScriptUByte(&statIdx);
	pollNextScriptUShort(&trigger);
	pollNextTwoScriptBytes(&op, &years);
	pollNextTwoScriptBytes(&days, &hours);
	pollNextScriptUByte(&minutes);

	MAIN_D_80134FDC = (uint8_t *)(MAIN_D_80134FDC + 1);
	v1 = readPStat(statIdx);
	v2 = readPStat((uint8_t)(statIdx + 1));
	v3 = readPStat((uint8_t)(statIdx + 2));
	now = dateToSeconds(v1, v2, v3, readPStat((uint8_t)(statIdx + 3)));

	if (scriptCompareValue(op, now, dateToSeconds(years, days, hours, minutes)) != 0) {
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

int16_t *getStatsPointer(int32_t stat)
{
	switch (stat) {
	case SCRIPT_STAT_OFFENSE:
		return &PARTNER_ENTITY.digimonEntity.stats.base.off;
	case SCRIPT_STAT_DEFENSE:
		return &PARTNER_ENTITY.digimonEntity.stats.base.def;
	case SCRIPT_STAT_SPEED:
		return &PARTNER_ENTITY.digimonEntity.stats.base.speed;
	case SCRIPT_STAT_BRAINS:
		return &PARTNER_ENTITY.digimonEntity.stats.base.brain;
	case SCRIPT_STAT_MAX_HP:
		return &PARTNER_ENTITY.digimonEntity.stats.base.hp;
	case SCRIPT_STAT_MAX_MP:
		return &PARTNER_ENTITY.digimonEntity.stats.base.mp;
	case SCRIPT_STAT_CURRENT_HP:
		return &PARTNER_ENTITY.digimonEntity.stats.current.currentHP;
	case SCRIPT_STAT_CURRENT_MP:
		return &PARTNER_ENTITY.digimonEntity.stats.current.currentMP;
	case SCRIPT_STAT_TIREDNESS:
		return &PARTNER_PARA.tiredness;
	case SCRIPT_STAT_HAPPINESS:
		return &PARTNER_PARA.happiness;
	case SCRIPT_STAT_DISCIPLINE:
		return &PARTNER_PARA.discipline;
	case SCRIPT_STAT_ENERGY:
		return &PARTNER_PARA.energyLevel;
	case SCRIPT_STAT_VIRUS:
		return &PARTNER_PARA.virusBar;
	case SCRIPT_STAT_LIFETIME:
		return &PARTNER_PARA.remainingLifetime;
	case SCRIPT_STAT_MERIT:
		return &MERIT;
	case SCRIPT_STAT_STARTED_BATTLES:
		return &MAIN_D_80134FC8;
	case SCRIPT_STAT_FLED_BATTLES:
		return &MAIN_D_80134FCA;
	case SCRIPT_STAT_TOURNAMENTS_WON:
		return &MAIN_D_80134FCC;
	case SCRIPT_STAT_TOURNAMENT_WINS:
		return &TOURNAMENTS_LOST;
	case SCRIPT_STAT_TOURNAMENTS_LOST:
		return &MAIN_D_80134FD0;
	case SCRIPT_STAT_WEIGHT:
		return &PARTNER_PARA.weight;
	case SCRIPT_STAT_TAMER_LEVEL:
		MAIN_D_80135002 = TAMER_ENTITY.tamerLevel;
		return &MAIN_D_80135002;
	case SCRIPT_STAT_LIVES:
		MAIN_D_80135004 = PARTNER_ENTITY.lives;
		return &MAIN_D_80135004;
	}
}

int32_t scriptCompareCard(void)
{
	uint16_t value;
	uint8_t cardId;
	uint8_t op;

	pollNextTwoScriptBytes(&cardId, &op);
	pollNextScriptUShort(&value);

	return scriptCompareValue(op, (uint8_t)getCardAmount(cardId), value);
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

	pollNextTwoScriptBytes(&itemId, &op);
	pollNextScriptUShort(&value);

	return scriptCompareValue(op, (uint8_t)getItemCount(itemId), value);
}

int16_t enforceStatsLimits(int32_t stat, int16_t value)
{
	int16_t cap;

	if (stat == SCRIPT_STAT_CURRENT_HP) {
		cap = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	} else if (stat == SCRIPT_STAT_CURRENT_MP) {
		cap = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	} else {
		cap = MAIN_D_80130318[stat];
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

	MAIN_D_80134FDC = (uint8_t *)(MAIN_D_80134FDC + 1);

	pollNextScriptUByte(&op);
	pollNextInt(&value);

	return scriptCompareValue(op, MONEY, value);
}
