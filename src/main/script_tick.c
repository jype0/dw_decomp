#include <dw/clock.h>
#include <dw/map_object.h>
#include <dw/script.h>
#include <dw/tournament.h>
#include <dw/trigger.h>

void unsetCameraFollowPlayer(void);
void clearTextArea(void);
extern int32_t MAIN_func_800D8E64(int32_t param_1, int32_t param_2,
				  int32_t param_3);
extern int32_t MAIN_func_800DF7F8(void);
extern int32_t tickRemoveMist(void);
extern int32_t MAIN_func_801138B0(void);
extern int32_t MAIN_func_80113A20(void);
extern int32_t DOO2_func_80071934(void);
extern int32_t isTrainingComplete(void);
extern void setCameraFollowPlayer(void);
extern void MAIN_func_80100258(int32_t a0);
extern void lostAllLives(void);

extern uint8_t MAIN_D_80134FE4;
extern uint8_t MAIN_D_80134FE9;
extern int32_t MAIN_D_80134FF0;
extern uint8_t MAIN_D_801BE6B5[];
extern uint8_t MAIN_D_801BE6B6[];
extern uint8_t *SCRIPT_HEADER_PTR;
extern uint8_t *SCRIPT_DATA_PTR;
extern char MAIN_D_80130388[];
extern int8_t MAIN_STATE;
extern int16_t SCRIPT_MAP_CHANGE_STATE;

extern uint8_t MAIN_D_801BE6B4[];

static void *script_tick_functions[] = {
	writePStat,
	isTriggerSet,
	MAIN_func_801062F8,
	readPStat,
	getScriptSection,
	getScript,
	tickScript,
};

int32_t tickScript(void)
{
	int32_t ret;
	int32_t offset;
	int32_t entityId;
	uint32_t op;
	uint8_t i;

	if (IS_SCRIPT_PAUSED) {
		return 1;
	}

	MAIN_func_80100258(0);
	tickScriptedMovements();
	if (MAIN_D_80134FE9 == 0x4b) {
		if (MAIN_func_800D8E64((int16_t)MAIN_D_80134FF8,
				       (int16_t)SELECTION_MENU_STATE,
				       MAIN_D_80134FA0)) {
			MAIN_D_80134FF0 = 0;
			MAIN_func_801062F8(0xff);
			MAIN_D_80134FE9 = 0;
		}
	}

	switch (ACTIVE_INSTRUCTION) {
	case 0x67:
		if (MAIN_D_80134FFC == 0) {
			ACTIVE_INSTRUCTION = 0;
		}
		break;
	case 0x64:
		switch (MAIN_D_80134FF8) {
		case 3:
		case 4:
		case 5:
		case 12:
		case 13:
		case 15:
		case 16:
		case 17:
		case 19:
		case 20:
		case 21:
		case 25:
		case 26:
		case 27:
		case 28:
		case 30:
		case 31:
		case 33:
		case 34:
		case 35:
		case 36:
		case 38:
		case 39:
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
		case 46:
		case 49:
		case 51:
		case 52:
		case 55:
		case 56:
			goto done;
		case 0:
			MAIN_func_800FCB3C();
			break;
		case 1:
			MAIN_func_8010BC10();
			break;
		case 8:
			if (!isTriggerSet(TRIGGER_3)) {
				MAIN_func_800FC508();
			} else {
				MAIN_func_8010B648();
			}
			break;
		case 14:
			if (!isTriggerSet(TRIGGER_3)) {
				if (!isTriggerSet(TRIGGER_4)) {
					rollCardPack();
				} else {
					MAIN_func_8010B9D8();
				}
			} else {
				MAIN_func_8010BB0C();
			}
			break;
		case 11:
			MAIN_func_8010BF68();
			break;
		case 2:
			MAIN_func_8010C4B0();
			break;
		case 9:
			openJukebox();
			break;
		case 10:
			MAIN_func_8010C28C();
			break;
		case 7:
			if (isTrainingComplete()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case 18:
			initTournamentSchedule();
			break;
		case 6:
			scriptStartTournament();
			break;
		case 22:
			scriptCheckTournamentMedal();
			break;
		case 23:
			if (tickOpenChestTray(readPStat(0xfe))) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case 24:
			if (tickCloseChestTray(readPStat(0xfe))) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case 29:
			if (DOO2_func_80071934()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case 37:
			if (tickRemoveMist()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case 47:
			lostAllLives();
			break;
		case 48:
			if (MAIN_func_801138B0()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case 54:
			if (MAIN_func_80113A20()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case 50:
			if (moveAngemonPedestal()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case 32:
			if (newGameStateMachine()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case 53:
			if (SOME_SCRIPT_SYNC_BIT == 1) {
				ACTIVE_INSTRUCTION = 0;
				MAIN_STATE = 3;
				longjmp(SCRIPT_JMP_BUF, 2);
			}
			/* fall through */
		done:
		default:
			break;
		}
		break;
	case 0x4a:
		if ((entityId = MAIN_D_80134FA4) == 0x19) {
			if (MAIN_func_800DF7F8()) {
				ACTIVE_INSTRUCTION = 0;
			}
		} else if (entityId == 0x1a) {
			if (TRN_LOADING_COMPLETE == 0) {
				ACTIVE_INSTRUCTION = 0;
			}
		} else if (entityId == 0xff) {
			if (MAIN_func_800DF7F8()) {
				for (i = 0, offset = 0;
				     i < 0x16;
				     ++i, offset += 0xc) {
					if (MAIN_D_801BE6B4[offset] != 0xff) {
						goto found;
					}
				}
				ACTIVE_INSTRUCTION = 0;
			found:;
			}
		} else {
			if (MAIN_D_801BE6B4[entityId * 0xc] == 0xff) {
				ACTIVE_INSTRUCTION = 0;
			}
		}
		break;
	case 0x10:
	case 0x1a:
	case 0xff:
		break;
	}

	if (MAIN_D_80134FE9) {
		return IS_SCRIPT_PAUSED;
	}

	if (ACTIVE_INSTRUCTION) {
		return IS_SCRIPT_PAUSED;
	}
setjmp_retry:
	ret = setjmp(SCRIPT_JMP_BUF);
	if (ret == 0) {
		op = *MAIN_D_80134FDC++;
		if (op >= 0xfb && op < 0x100) {
			scriptInstructionFBtoFF(op);
		} else if (op >= 0x10 && op < 0x28) {
			scriptInstruction10to27(op);
		} else if (op >= 0x28 && op < 0x40) {
			scriptInstruction28to3F(op);
		} else if (op >= 0x46 && op < 0x59) {
			scriptInstruction46to58(op);
		} else if (op >= 0x5a && op < 0x60) {
			scriptInstruction5Ato5F(op);
		} else if (op >= 0x64 && op < 0x7f) {
			scriptInstruction64to7E(op);
		} else {
			IS_SCRIPT_PAUSED = 1;
			longjmp(SCRIPT_JMP_BUF, 2);
		}
	} else {
		if (ret == 1) {
			goto setjmp_retry;
		}

		if (ret == 3) {
			MAIN_func_8010064C();
			writePStat(0, MAIN_D_80134FE7);
			MAIN_D_80134FE9 = 0x4b;
			SCRIPT_MAP_CHANGE_STATE = 0;
			return IS_SCRIPT_PAUSED;
		}
	}

	if (IS_SCRIPT_PAUSED) {
		MAIN_func_8010064C();
		writePStat(0, MAIN_D_80134FE7);
		if (MAIN_D_80134FF0 == 1) {
			setMovementEnabled(-1, 0);
			setCameraFollowPlayer();
			startGameTime();
		}
	}

	return IS_SCRIPT_PAUSED;
}

uint8_t *getScript(int32_t mapId)
{
	uint32_t *table;

	if (mapId == 0) {
		return MAPHEAD_DATA_PTR;
	}

	if (ACTIVE_MAP_SCRIPT == mapId) {
		return SCRIPT_DATA_PTR;
	}

	ACTIVE_MAP_SCRIPT = mapId;
	table = (uint32_t *)SCRIPT_HEADER_PTR;
	readFileSection(MAIN_D_80130388, SCRIPT_DATA_PTR, table[mapId],
			table[mapId + 1] - table[mapId]);

	return SCRIPT_DATA_PTR;
}

uint8_t *getScriptSection(uint8_t *script, int32_t section)
{
	uint16_t *entry = (uint16_t *)&script[2];

	while (1) {
		if (entry[0] == section) {
			return &script[entry[1]];
		}

		if (entry[0] == 0xffff) {
			return 0;
		}

		entry = &entry[2];
	}
}

uint8_t readPStat(int32_t index)
{
	return SCRIPT_STATE_PTR->pstats[index];
}

void MAIN_func_801062F8(int32_t owner)
{
	int32_t entityId;
	uint8_t speaker;
	int32_t idx;

	if (MAIN_D_80134FF0 == 0) {
		MAIN_D_80134FF0 = 1;
		setMovementEnabled(0, 1);
		setMovementEnabled(1, 1);
		unsetCameraFollowPlayer();
		writePStat(0, 3);
		clearTextArea();
		stopGameTime();

		if (MAIN_D_80134FE0 != 0 && isTriggerSet(TRIGGER_44) == 0) {
			entityId = scriptIdToEntityId(MAIN_D_80134FE4) & 0xff;
			if (entityId != 0xff) {
				MAIN_D_801BE6B4[entityId * 0xc] = 0;
				idx = entityId * 0xc;
				speaker = MAIN_D_80134FE4;
				MAIN_D_801BE6B5[idx] = speaker;
				MAIN_D_801BE6B6[idx] = 0xfd;
				MAIN_D_801BE6B4[0] = 0;
				MAIN_D_801BE6B5[0] = 0xfd;
				MAIN_D_801BE6B6[0] = speaker;
			}
		}
	}

	unsetTrigger(TRIGGER_44);

	if ((uint32_t)owner < 0xc8) {
		owner = scriptIdToEntityId(owner) & 0xff;
		if (owner != 0xff) {
			setMovementEnabled(owner, 1);
		}
	}
}

int32_t isTriggerSet(uint16_t trigger)
{
	uint8_t *ptr;
	uint8_t mask;

	getTriggerOffset(trigger, &ptr, &mask);

	return (*ptr & mask) != 0;
}

void writePStat(int32_t index, uint8_t value)
{
	uint8_t *ptr;

	ptr = &SCRIPT_STATE_PTR->pstats[index];
	*ptr = value;
}
