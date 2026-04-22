#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/pstat.h>
#include <dw/tournament.h>
#include <dw/trigger.h>
#include <dw/types.h>
#include <dw/ui.h>

#include "common.h"

extern uint16_t ACTIVE_MAP_SCRIPT;
extern uint16_t SELECTION_MENU_STATE;
extern uint8_t ACTIVE_INSTRUCTION;
extern uint8_t SCRIPT_STATE_3;
extern uint16_t SCRIPT_STATE_4;
extern int16_t TOURNAMENTS_LOST;

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
int32_t readPStat(int32_t id);
int32_t getTamerState(void);
int32_t getPartnerState(void);
void callScriptSection(int32_t, int32_t, int32_t);
void updateMapLightState(void);
extern int32_t IS_SCRIPT_PAUSED;

void updateTournamentRegistration(void)
{
	uint8_t i;
	uint8_t value;
	int32_t flag;
	uint32_t minutes;
	uint8_t day;
	uint8_t partnerType;

	flag = 0;
	if (isTriggerSet(TRIGGER_TOURNAMENT_REGISTERED)) {
		minutes = minutesOfDay();
		i = PARTNER_ENTITY.digimonEntity.entity.type;
		partnerType = i;
		value = readPStat(PSTAT_TOURNAMENT_DIGIMON);
		if (partnerType != value) {
			unsetTrigger(TRIGGER_TOURNAMENT_REGISTERED);
			return;
		}

		day = DAY;
		value = readPStat(PSTAT_TOURNAMENT_DAY);
		if (value & 0x80) {
			return;
		}

		if (day != value) {
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

		if (getTamerState()) {
			return;
		}

		if (getPartnerState() != 1) {
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


int32_t minutesOfDay(void)
{
	return MINUTE + HOUR * 60;
}

void scriptStartTournament(void)
{
	if (SELECTION_MENU_STATE) {
	}

	if (SELECTION_MENU_STATE != 3) {
		if (SELECTION_MENU_STATE != 2) {
			if (SELECTION_MENU_STATE == 1) {
				return;
			}

			if (SELECTION_MENU_STATE != 0) {
				return;
			}

			SELECTION_MENU_STATE = 2;
			return;
		}

		closeBox(0);
		startTournament();
	}

	ACTIVE_INSTRUCTION = 0;
}

uint8_t *getCupDataJumpTable(uint32_t section, uint32_t id)
{
	uint8_t *script;
	uint8_t *sectionPtr;

	script = getScript(ACTIVE_MAP_SCRIPT);
	sectionPtr = getScriptSection(script, section);

	return getCupDataJumpTableEntry(sectionPtr, id);
}

INCLUDE_ASM("asm/main/nonmatchings/tournament", getCupDataJumpTableEntry);

int32_t checkTournamentMedalConditions(void)
{
	int32_t result;
	uint16_t i;
	int32_t trigger;

	i = 0;
	trigger = TRIGGER_D_RANK_CUP_WON;
	while (i < 6) {
		if (isTriggerSet(trigger) == 0) {
			goto loop2;
		}

		++i;
		++trigger;
	}

	result = 0;
	goto check_medal;

loop2:
	i = 0;
	trigger = TRIGGER_VERSION_1_CUP_WON;
	while (i < 5) {
		if (isTriggerSet(trigger) == 0) {
			goto loop3;
		}

		++i;
		++trigger;
	}

	result = 1;
	goto check_medal;

loop3:
	i = 0;
	trigger = TRIGGER_FIRE_CUP_WON;
	while (i < 7) {
		if (isTriggerSet(trigger) == 0) {
			goto loop4;
		}

		++i;
		++trigger;
	}

	result = 2;
	goto check_medal;

loop4:
	i = 0;
	trigger = TRIGGER_DINO_CUP_WON;
	while (i < 4) {
		if (isTriggerSet(trigger) == 0) {
			goto level_check;
		}

		++i;
		++trigger;
	}

	result = 3;
	goto check_medal;

level_check:
	if (TOURNAMENTS_LOST >= 100) {
		result = 4;
		goto check_medal;
	}

	i = 0;
	while (i < 0x42) {
		if (getCardAmount(i) == 0) {
			goto ret_neg1;
		}

		++i;
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

	if (SELECTION_MENU_STATE == 1) {
		return;

		if (!result) {
		}
	}

	if (SELECTION_MENU_STATE != 0) {
		return;
	}

	result = checkTournamentMedalConditions();
	if (result == -1) {
		ACTIVE_INSTRUCTION = 0;
		return;
	}

	unlockMedal(TOURNAMENT_MEDAL_IDS[result]);
	showMapHeadTextbox(result, 0xff, 0, 0x4db);
	SELECTION_MENU_STATE = 1;
	SCRIPT_STATE_4 = 0;
	SCRIPT_STATE_3 = 1;
}
