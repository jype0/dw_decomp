#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/partner.h>
#include <dw/pstat.h>
#include <dw/tamer.h>
#include <dw/tournament.h>
#include <dw/trigger.h>
#include <dw/types.h>
#include <dw/ui.h>

#include "common.h"

extern uint16_t ACTIVE_MAP_SCRIPT;
extern uint16_t SCRIPT_STATE_2;
extern uint8_t ACTIVE_INSTRUCTION;
extern uint8_t SCRIPT_TEXTBOX_MODE;
extern uint16_t SCRIPT_NEXT_STATE_2;
extern int16_t TOURNAMENT_WINS;

uint8_t *getScript(uint32_t scriptId);
uint8_t *getScriptSection(uint8_t *ptr, int32_t section);

void closeBox(int32_t id);
void startTournament(void);
void unlockMedal(uint16_t medalId);
void showMapHeadTextbox(int32_t, int32_t, int32_t, int32_t);
int32_t isTriggerSet(uint16_t trigger);
int32_t hasMedal(uint16_t medal);
int32_t getCardAmount(uint8_t cardId);
void unsetTrigger(uint16_t trigger);
uint8_t readPStat(int32_t id);
void callScriptSection(int32_t, int32_t, int32_t);
void updateMapLightState(void);
extern int32_t IS_SCRIPT_PAUSED;

// clang-format off
uint16_t TOURNAMENT_MEDAL_IDS[6] = {
	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x000c,
};
// clang-format on

void updateTournamentRegistration(void)
{
	uint8_t i;
	uint8_t value;
	int32_t flag;
	uint32_t minutes;
	uint8_t partnerType;

	flag = 0;
	if (isTriggerSet(TRIGGER_TOURNAMENT_REGISTERED)) {
		minutes = minutesOfDay();
		partnerType = PARTNER_ENTITY.digimonEntity.entity.type;
		value = readPStat(PSTAT_TOURNAMENT_DIGIMON);
		if (partnerType != value) {
			unsetTrigger(TRIGGER_TOURNAMENT_REGISTERED);
			return;
		}

		i = DAY;
		value = readPStat(PSTAT_TOURNAMENT_DAY);
		if (value & 0x80) {
			return;
		}

		if (i != value) {
			flag = 1;
		}

		if (minutes < 0x2D0) {
			return;
		}

		if (minutes >= 0x565) {
			flag = 1;
		}

		if (!IS_SCRIPT_PAUSED) {
			return;
		}

		if (tamerGetState()) {
			return;
		}

		if (partnerGetState() != 1) {
			return;
		}

		for (i = 0; i < 6; ++i) {
			if (UI_BOX_DATA[i].state != 0) {
				return;
			}
		}

		if (flag) {
			callScriptSection(0, 0x4DA, 1);
			unsetTrigger(TRIGGER_TOURNAMENT_REGISTERED);
			return;
		}

		if (minutes < 0x4B0) {
			if (!isTriggerSet(TRIGGER_38)) {
				return;
			}

			unsetTrigger(TRIGGER_38);
		} else {
			if (!isTriggerSet(TRIGGER_39)) {
				return;
			}

			unsetTrigger(TRIGGER_39);
		}

		callScriptSection(0, 0x4D9, 1);
	}

	updateMapLightState();
}


uint32_t minutesOfDay(void)
{
	return MINUTE + HOUR * 60;
}

void scriptStartTournament(void)
{
	switch (SCRIPT_STATE_2) {
	case 0:
		SCRIPT_STATE_2 = 2;
		break;
	case 1:
		break;
	case 2:
		closeBox(0);
		startTournament();
		/* fall through */
	case 3:
		ACTIVE_INSTRUCTION = 0;
		break;
	}
}

uint8_t *getCupDataJumpTable(uint8_t section, uint8_t id)
{
	uint8_t *script;
	uint8_t *sectionPtr;

	script = getScript(ACTIVE_MAP_SCRIPT);
	sectionPtr = getScriptSection(script, section);

	return getCupDataJumpTableEntry(sectionPtr, id);
}

uint8_t *getCupDataJumpTableEntry(uint8_t *scriptPtr, uint8_t id)
{
	uint8_t *script;

	script = getScript(ACTIVE_MAP_SCRIPT);
	scriptPtr = scriptPtr + id * 4 + 2;
	scriptPtr = script + *(uint16_t *)scriptPtr;

	return scriptPtr;
}

int32_t checkTournamentMedalConditions(void)
{
	int32_t result;
	uint16_t i;

	for (i = 0; i < 6; i++) {
		if (isTriggerSet(TRIGGER_D_RANK_CUP_WON + i) == 0) {
			goto loop2;
		}
	}

	result = 0;
	goto check_medal;

loop2:
	for (i = 0; i < 5; i++) {
		if (isTriggerSet(TRIGGER_VERSION_1_CUP_WON + i) == 0) {
			goto loop3;
		}
	}

	result = 1;
	goto check_medal;

loop3:
	for (i = 0; i < 7; i++) {
		if (isTriggerSet(TRIGGER_FIRE_CUP_WON + i) == 0) {
			goto loop4;
		}
	}

	result = 2;
	goto check_medal;

loop4:
	for (i = 0; i < 4; i++) {
		if (isTriggerSet(TRIGGER_DINO_CUP_WON + i) == 0) {
			goto level_check;
		}
	}

	result = 3;
	goto check_medal;

level_check:
	if (TOURNAMENT_WINS >= 100) {
		result = 4;
		goto check_medal;
	}

	for (i = 0; i < 0x42; i++) {
		if (getCardAmount(i) == 0) {
			goto ret_neg1;
		}
	}

	if (isTriggerSet(TRIGGER_BEATEN_GAME_ONCE) == 0) {
		goto ret_neg1;
	}

	result = 5;
	goto check_medal;

ret_neg1:
	return -1;

check_medal:
	if (hasMedal(TOURNAMENT_MEDAL_IDS[result]) != 0) {
		return -1;
	}

	return result;
}

void scriptCheckTournamentMedal(void)
{
	int32_t result;

	switch (SCRIPT_STATE_2) {
	case 0:
		result = checkTournamentMedalConditions();
		if (result == -1) {
			ACTIVE_INSTRUCTION = 0;
			return;
		}

		unlockMedal(TOURNAMENT_MEDAL_IDS[result]);
		showMapHeadTextbox(result, 0xff, 0, 0x4db);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 0;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	case 1:
		break;
	}
}
