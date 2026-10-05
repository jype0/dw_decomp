#include <string.h>

#include <libcd.h>

#include <dw/clock.h>
#include <dw/doo2.h>
#include <dw/eab.h>
#include <dw/fade.h>
#include <dw/file.h>
#include <dw/font.h>
#include <dw/item.h>
#include <dw/main.h>
#include <dw/map_object.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/pstat.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/sound_async.h>
#include <dw/tamer.h>
#include <dw/tournament.h>
#include <dw/trigger.h>
#include <dw/trn.h>
#include <dw/trn2.h>
#include <dw/ui.h>
#include <dw/utils.h>

extern uint8_t SCRIPT_SECTION;
extern uint8_t MAIN_D_80134FE9;
extern int32_t MAIN_D_80134FF0;
extern uint8_t *SCRIPT_OFFSET_PTR;
extern uint8_t *MAP_SCRIPT_PTR;
extern int8_t MAIN_STATE;
extern int16_t SCRIPT_MAP_CHANGE_STATE;
extern ScriptCameraMovement SCRIPT_MOVEMENT[22];
extern uint8_t ACTIVE_BGM_TRACK;
extern uint8_t PREVIOUS_SCREEN;
extern uint8_t PREVIOUS_EXIT;
extern uint8_t CURRENT_EXIT;
extern int8_t TALKED_TO_ENTITY;
extern uint8_t MAPHEAD_SCRIPT_BUFFER[];
extern uint8_t SCRIPT_OFFSET_TABLE[];
extern uint8_t MAP_SCRIPT_BUFFER[];
extern ScriptState SCRIPT_STATE;
extern uint8_t TEXTBOX_LINES_BUFFER[];
extern uint8_t *ACTIVE_SCRIPT;
extern int32_t LOAD_FILE_SECTION_STARTPOS;

void unsetCameraFollowPlayer(void);
int32_t scriptTickChangeMap(int32_t param_1, int32_t param_2, int32_t param_3);
int32_t tickEntityWalkTo(uint8_t scriptId1, uint8_t scriptId2, int32_t targetX, int32_t targetZ, int8_t withCamera);
int32_t tickRemoveMist(void);
int32_t tickSaveMachine(void);
int32_t scriptTickMainMenu(void);
int32_t isTrainingComplete(void);
void setCameraFollowPlayer(void);
void setFoodTimer(int32_t type);
void setActiveAnim(int32_t state);
int32_t hasMove(int32_t moveId);
void learnMove(int32_t moveId);
void unloadDigimonModel(int32_t digimonType);
void loadNPCModel(int32_t modelId);
int32_t scriptCompareSignedValue(int32_t op, int32_t lhs, int32_t rhs);
void updateBGM(void);
void forceUpdateBGM(void);
void pollNextScriptTwoUShort(uint16_t *out1, uint16_t *out2);
void skipHours(int32_t hours);
void scriptSetDigimon(int32_t a0, int32_t a1, int32_t a2);
void scriptUnloadEntity(int32_t a0);
void resetEntityOrigin(int32_t a0);
void setMapObjectsFlag(int32_t start, int32_t count, int32_t flag);
void resetMapObjectAnimation(int32_t a0, int32_t a1);
void createMeramonShake(void);
void createNinjamonEffect(void);
void openSaveMachine(void);
void gameClearSave(void);
void spawnSpriteAtLocation(int16_t x, int16_t y, int16_t z, int16_t w, int32_t type);
void spawnSpriteAtEntity(int32_t entId, int32_t sprite, int32_t param);
void setImpassableRect(int16_t x, int16_t y, int8_t w, int8_t h);
void addEntityText(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
void setLoopCountToOne(int32_t a0);
void loadTrainingLibrary(int32_t a0);
int32_t loadTextureFile(char *path, uint32_t *outTPage, uint32_t *outClut);
void setMapHeadActive(void);
int32_t tickCameraMoveTo(int32_t x, int32_t y, uint8_t speed);
int32_t tickCameraMoveToEntity(uint32_t scriptId, uint8_t speed);
int32_t tickMoveObjectTo(uint8_t objectIndex, uint8_t moveIndex, int8_t steps,
                         int16_t targetX, int16_t targetY);
void initializeScripts(void);
void initializeLoadedNPCModels(void);
void runMapHeadScript(int32_t section);
void setupMap(int32_t param_1, int32_t param_2);
void loadMap(uint16_t mapId);
uint8_t *getScriptSection(uint8_t *script, int32_t section);
int32_t addFileReadRequestPath(char *path, int32_t buffer, uint8_t *isRunning,
                               void *callback, void *callbackParam);

static void *script_interp_text_order[] = {
	_hasMove,
	getTriggerOffset,
	pollNextScriptTwoUShort,
	scriptUnloadModel,
	forceUpdateBGM,
	updateBGM,
	playBGM,
	pollNextTwoScriptShorts,
	pollNextScriptShort,
	scriptLoadModel,
	secondsToDate,
	pollNextInt,
	dateToSeconds,
	setCardAmount,
	getCardAmount,
	scriptLearnMove,
	skipOneReadInteger,
	scriptCompareSignedValue,
	scriptCompareValue,
	pollNextTwoScriptBytes,
	pollNextScriptUShort,
	skipOnePollTwoScriptBytes,
	unsetTrigger,
	setTrigger,
	pollNextScriptUByte,
	pollOneUByteOneUShort,
	skipOneReadOneUShort,
	popScriptStack,
	resetBGM,
	pushScriptStack,
	skipOneReadTwoShort,
	writePStat,
	isTriggerSet,
	scriptPauseGame,
	readPStat,
	getScriptSection,
	getScript,
	tickScript,
	callScriptSection,
	runMapHeadScript,
	initializeLoadedNPCModels,
	initializeScripts,
	tickScriptedMovement,
	handleMusicOverride,
	scriptStartAnimation,
	scriptSetEntityWalking,
	consumeMapChangeShowName,
	scriptUpdateEnergyBoundaries,
	scriptIfInstruction,
	returnFromScriptFile,
	returnFromScriptFile,
	setMapHeadActive,
	scriptInstruction64to7E,
	scriptInstruction5Ato5F,
	scriptInstruction46to58,
	scriptInstruction28to3F,
	scriptInstruction10to27,
	scriptInstructionFBtoFF,
	tickScriptedMovements,
	readFileSection,
};

// clang-format off
char MAIN_D_80130374[] = "\\SCN\\MAPHEAD.SCN";
char MAIN_D_80130388[12] = "\\SCN\\DG.SCN";
char MAIN_D_80130394[20] = "\\ETCHI\\BOSS_EFE.TMD";
char MAIN_D_801303A8[] = "\\ETCHI\\OP.TIM";

char MAIN_D_801345F0[] = ";1";
// clang-format on

int32_t tickScript(void)
{
	int32_t ret;
	uint8_t op;

	if (IS_SCRIPT_PAUSED) {
		return 1;
	}

#if defined(VERSION_JP)
	tickTextboxHandling();
#else
	tickTextboxHandling(0);
#endif
	tickScriptedMovements();
	if (MAIN_D_80134FE9 == 0x4b) {
		if (scriptTickChangeMap((int16_t)SCRIPT_PARAM_1,
		                       (int16_t)SCRIPT_STATE_2,
		                       SCRIPT_MAP_CHANGE_SHOW_NAME)) {
			MAIN_D_80134FF0 = 0;
			scriptPauseGame(0xff);
			MAIN_D_80134FE9 = 0;
		}
	}

	switch (ACTIVE_INSTRUCTION) {
	case SCRIPT_OP_DELAY:
		if (DELAY_FRAMES == 0) {
			ACTIVE_INSTRUCTION = 0;
		}
		break;
	case SCRIPT_OP_CALL_ROUTINE:
		switch (SCRIPT_PARAM_1) {
		case 0:
			openDiscardItem();
			break;
		case 1:
			openMeritShop();
			break;
		case 8:
			if (!isTriggerSet(TRIGGER_3)) {
				openShop();
			} else {
				openRecycleShop();
			}
			break;
		case 14:
			if (!isTriggerSet(TRIGGER_3)) {
				if (!isTriggerSet(TRIGGER_4)) {
					rollCardPack();
				} else {
					openBuyCardMenu();
				}
			} else {
				openSellCardMenu();
			}
			break;
		case 11:
			tickItemKeeper();
			break;
		case 2:
			openMojyamonShop();
			break;
		case 9:
			openJukebox();
			break;
		case 10:
			tickBirdraTransport();
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
			if (DOO2_tickEggInput()) {
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
			if (tickSaveMachine()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case 54:
			if (scriptTickMainMenu()) {
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
		default:
			break;
		}
		break;
	case SCRIPT_OP_WAIT_FOR_ENTITY:
		if (WAIT_FOR_ENTITY_ID == 0x19) {
			if (getScriptSyncBit()) {
				ACTIVE_INSTRUCTION = 0;
			}
		} else if (WAIT_FOR_ENTITY_ID == 0x1a) {
			if (TRN_LOADING_COMPLETE == 0) {
				ACTIVE_INSTRUCTION = 0;
			}
		} else if (WAIT_FOR_ENTITY_ID == 0xff) {
			if (getScriptSyncBit()) {
				for (op = 0; op < 0x16; op++) {
					if (SCRIPT_MOVEMENT[op].type != 0xff) {
						goto found;
					}
				}
				ACTIVE_INSTRUCTION = 0;
found:;
			}
		} else {
			if (SCRIPT_MOVEMENT[WAIT_FOR_ENTITY_ID].type == 0xff) {
				ACTIVE_INSTRUCTION = 0;
			}
		}
		break;
	case SCRIPT_OP_SET_SELECTION:
	case SCRIPT_OP_SHOW_TEXTBOX:
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
		op = *SCRIPT_POINTER++;
		if (op >= 0xfb && op <= 0xff) {
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
			closeAllTextboxes();
			writePStat(0, MAIN_D_80134FE7);
			MAIN_D_80134FE9 = 0x4b;
			SCRIPT_MAP_CHANGE_STATE = 0;
			return IS_SCRIPT_PAUSED;
		}
	}

	if (IS_SCRIPT_PAUSED) {
		closeAllTextboxes();
		writePStat(0, MAIN_D_80134FE7);
		if (MAIN_D_80134FF0 == 1) {
			setMovementEnabled(-1, 0);
			setCameraFollowPlayer();
			startGameTime();
		}
	}

	return IS_SCRIPT_PAUSED;
}

uint8_t *getScript(uint16_t mapId)
{
	uint32_t *table;
	uint32_t offset;
	uint32_t size;

	if (mapId == 0) {
		return MAPHEAD_SCRIPT_PTR;
	}

	if (ACTIVE_MAP_SCRIPT == mapId) {
		return MAP_SCRIPT_PTR;
	}

	ACTIVE_MAP_SCRIPT = mapId;
	table = (uint32_t *)SCRIPT_OFFSET_PTR;
	offset = table[mapId];
	size = table[mapId + 1] - table[mapId];
	readFileSection(MAIN_D_80130388, MAP_SCRIPT_PTR, offset, size);

	return MAP_SCRIPT_PTR;
}

// clang-format off
uint8_t *getScriptSection(script, section)
	uint8_t *script;
	uint16_t section;
// clang-format on
{
	uint16_t *entry = (uint16_t *)&script[2];

	while (1) {
		if (entry[0] == section) {
			return &script[entry[1]];
		}

		if (entry[0] == 0xffff) {
			return 0;
		}

		entry += 2;
	}
}

// clang-format off
void scriptInstruction10to27(op)
	uint8_t op;
// clang-format on
{
	StackEntry entry;
	uint16_t shortArg;
	uint16_t offset;
	uint8_t pstat;
	uint8_t value;
	uint8_t unusedByte;
	int32_t newValue;

	switch (op) {
	case SCRIPT_OP_SET_SELECTION:
		MAIN_D_80135000 = 1;
		scriptShowSelection();
		longjmp(SCRIPT_JMP_BUF, 2);
	case SCRIPT_OP_UNUSED_11:
		break;
	case SCRIPT_OP_UNUSED_12:
		break;
	case SCRIPT_OP_JUMP_AND_LINK:
		skipOneReadOneUShort(&shortArg);
		entry.scriptPtr = SCRIPT_POINTER;
		entry.scriptId = ACTIVE_MAP_SCRIPT;
		entry.smth[0] = 1;
		pushScriptStack(&entry);
		SCRIPT_POINTER =
			(uint8_t *)((uint32_t)ACTIVE_SCRIPT + shortArg);
		break;
	case SCRIPT_OP_JUMP_TO_FILE_AND_LINK:
		skipOneReadTwoShort(&shortArg, &offset);
		entry.scriptPtr = SCRIPT_POINTER;
		entry.scriptId = ACTIVE_MAP_SCRIPT;
		entry.smth[0] = 1;
		pushScriptStack(&entry);
		ACTIVE_SCRIPT = getScript(shortArg);
		SCRIPT_POINTER = getScriptSection(
			(uint8_t *)(int32_t)ACTIVE_SCRIPT, offset);
		break;
	case SCRIPT_OP_JUMP_RETURN:
		SCRIPT_POINTER++;
		popScriptStack(&entry);
		ACTIVE_SCRIPT = getScript(entry.scriptId);
		SCRIPT_POINTER = (uint8_t *)entry.scriptPtr;
		break;
	case SCRIPT_OP_JUMP_TO:
		skipOneReadOneUShort(&shortArg);
		SCRIPT_POINTER =
			(uint8_t *)((uint32_t)ACTIVE_SCRIPT + shortArg);
		break;
	case SCRIPT_OP_JUMP_TO_FILE:
		skipOneReadTwoShort(&shortArg, &offset);
		ACTIVE_SCRIPT = getScript(shortArg);
		SCRIPT_POINTER = getScriptSection(
			(uint8_t *)(int32_t)ACTIVE_SCRIPT, offset);
		break;
	case SCRIPT_OP_SWITCH:
		pollOneUByteOneUShort(&pstat, &shortArg);
		value = readPStat(pstat);
		if (value >= shortArg) {
			value = shortArg - 1;
		}
		shortArg = *(uint16_t *)(SCRIPT_POINTER + (value << 1));
		SCRIPT_POINTER =
			(uint8_t *)((uint32_t)ACTIVE_SCRIPT + shortArg);
		break;
	case SCRIPT_OP_IF:
		SCRIPT_POINTER++;
		scriptIfInstruction();
		break;
	case SCRIPT_OP_SHOW_TEXTBOX:
		SCRIPT_POINTER++;
		if (MAIN_D_80135000 == 2) {
			showTextbox(0, 0xff);
		} else {
			showTextbox(0, CURRENT_DIALOGUE_OWNER);
		}
		longjmp(SCRIPT_JMP_BUF, 2);
	case SCRIPT_OP_SET_DIALOG_OWNER:
		pollNextScriptUByte(&pstat);
		MAIN_D_80135000 = 0;
		if (UI_BOX_DATA[0].state != 1) {
			CURRENT_DIALOGUE_OWNER = pstat - 1;
		}
		setDialogueOwner(pstat);
		break;
	case SCRIPT_OP_SET_TRIGGER:
		skipOneReadOneUShort(&shortArg);
		setTrigger(shortArg);
		break;
	case SCRIPT_OP_UNSET_TRIGGER:
		skipOneReadOneUShort(&shortArg);
		unsetTrigger(shortArg);
		break;
	case SCRIPT_OP_SET_PSTAT:
		skipOnePollTwoScriptBytes(&pstat, &value);
		writePStat(pstat, value);
		break;
	case SCRIPT_OP_ADD_TO_PSTAT:
		skipOnePollTwoScriptBytes(&pstat, &value);
		newValue = readPStat(pstat) + value;
		if (newValue >= 0x100) {
			newValue = 0xff;
		}
		writePStat(pstat, newValue);
		break;
	case SCRIPT_OP_REDUCE_PSTAT:
		skipOnePollTwoScriptBytes(&pstat, &value);
		newValue = readPStat(pstat) - value;
		if (newValue < 0) {
			newValue = 0;
		}
		writePStat(pstat, newValue);
		break;
	case SCRIPT_OP_STORE_MAP_ID:
		pollNextScriptUByte(&pstat);
		writePStat(pstat, CURRENT_SCREEN_ID);
		break;
	case SCRIPT_OP_STORE_DIGIMON_TYPE:
		pollNextScriptUByte(&pstat);
		value = PARTNER_ENTITY.digimonEntity.entity.type;
		writePStat(pstat, value);
		break;
	case SCRIPT_OP_SET_INVENTORY_SIZE:
		pollNextScriptUByte(&pstat);
		setInventorySize(pstat);
		break;
	case SCRIPT_OP_STORE_RANDOM:
		skipOnePollTwoScriptBytes(&pstat, &value);
		newValue = randomLimit(value + 1);
		writePStat(pstat, newValue);
		break;
	case SCRIPT_OP_STORE_DATE:
		pollNextScriptUByte(&pstat);
		writePStat(pstat + 0, YEAR);
		writePStat(pstat + 1, DAY);
		writePStat(pstat + 2, HOUR);
		writePStat(pstat + 3, MINUTE);
		break;
	case SCRIPT_OP_SET_TEXTBOX_SIZE:
		scriptPauseGame(0xff);
		MAIN_D_80135000 = 2;
		scriptSetTextboxSize();
		break;
	case SCRIPT_OP_FADEOUT_HUD:
		pollNextScriptUByte(&pstat);
		closeBox(pstat);
		longjmp(SCRIPT_JMP_BUF, 2);
	}

	longjmp(SCRIPT_JMP_BUF, 1);
}

void scriptUpdateEnergyBoundaries(int32_t a0, int32_t a1)
{
	if (PARTNER_PARA.energyLevel > RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyCap) {
		PARTNER_PARA.energyLevel = RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyCap;
	}
	if (PARTNER_PARA.condition & 4) {
		if (RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyThreshold <= PARTNER_PARA.energyLevel) {
			PARTNER_PARA.condition &= ~4;
			setFoodTimer(PARTNER_ENTITY.digimonEntity.entity.type);
			PARTNER_PARA.starvationTimer = 0;
		}
	}
}

void consumeMapChangeShowName(void)
{
	int32_t state = readPStat(6) & 0xff;

	switch (state) {
	case 0:
		SCRIPT_MAP_CHANGE_SHOW_NAME = (SCRIPT_SECTION_IS_EVENT != 0) ^ 1;
		break;
	case 1:
		SCRIPT_MAP_CHANGE_SHOW_NAME = 1;
		break;
	case 2:
		SCRIPT_MAP_CHANGE_SHOW_NAME = 0;
		break;
	}

	writePStat(6, 0);
}

void scriptSetEntityWalking(uint8_t actorId, int32_t animationId)
{
	if (actorId == 0xfd) {
		startAnimationTamer((int16_t)(animationId + 2));
		tamerSetState(0xa);
	} else if (actorId == 0xfc) {
		partnerStartAnimation((animationId + 1) << 1);
		partnerSetState(0xc);
	} else {
		scriptNPCStartAnimation(actorId, (animationId + 1) << 1);
		setActiveAnim(0xc);
	}
}

void scriptStartAnimation(uint8_t actorId, int32_t animationId)
{
	if (actorId == 0xfd) {
		startAnimationTamer(animationId);
		tamerSetState(0xa);
	} else if (actorId == 0xfc) {
		partnerStartAnimation(animationId);
		partnerSetState(0xc);
	} else {
		scriptNPCStartAnimation(actorId, animationId);
		setActiveAnim(0xc);
	}
}

void scriptIfInstruction(void)
{
	uint8_t condOp;
	uint8_t pstatValue;
	uint8_t comparand;
	uint16_t shortArg;
	int32_t cond;
	int32_t result;

	result = 0;
	for (;;) {
		pollNextScriptUByte(&condOp);
		SCRIPT_POINTER++;
		if (condOp == 0x19) {
			longjmp(SCRIPT_JMP_BUF, 1);
		}
		switch (condOp & 0x38) {
		case 0:
			pollNextScriptUShort(&shortArg);
			cond = isTriggerSet(shortArg);
			if ((condOp & 7) == 0) {
				cond = (cond == 1);
				break;
			} else {
				cond = (cond == 0);
				break;
			}
		case (1 << 3):
			pollNextTwoScriptBytes(&pstatValue, &comparand);
			pstatValue = readPStat(pstatValue);
			cond = scriptCompareValue(condOp, pstatValue,
			                          comparand);
			break;
		case (2 << 3):
			pollNextScriptUShort(&shortArg);
			if (result != 1) {
				continue;
			}
			SCRIPT_POINTER =
				(uint8_t *)((uint32_t)ACTIVE_SCRIPT +
			                    shortArg);
			longjmp(SCRIPT_JMP_BUF, 1);
		case (3 << 3):
			pollNextScriptUShort(&shortArg);
			if (result != 0) {
				continue;
			}

			SCRIPT_POINTER =
				(uint8_t *)((uint32_t)ACTIVE_SCRIPT +
			                    shortArg);
			longjmp(SCRIPT_JMP_BUF, 1);
		case (4 << 3):
			switch (condOp & 7) {
			case 0:
				cond = scriptCompareStat();
				break;
			case 1:
				cond = scriptCompareCard();
				break;
			case 2:
				cond = scriptCompareMove();
				break;
			case 3:
				cond = scriptCompareCondition();
				break;
			case 4:
				cond = scriptCompareItemCount();
				break;
			case 5:
				cond = scriptCompareMoney();
				break;
			}
			break;
		}

		if (condOp & 0x80) {
			int32_t combined = 0;
			if (result == 1 && cond == 1) {
				combined = 1;
			}
			result = combined;
		} else if (condOp & 0x40) {
			int32_t combined = 1;
			if (result != 1 && cond != 1) {
				combined = 0;
			}
			result = combined;
		} else {
			result = cond;
		}
	}
}

int32_t scriptCompareValue(uint8_t op, uint32_t lhs, uint32_t rhs)
{
	int32_t result;

	switch (op & 7) {
	case 0:
		result = lhs == rhs;
		break;
	case 1:
		result = lhs != rhs;
		break;
	case 2:
		result = lhs >= rhs;
		break;
	case 3:
		result = lhs <= rhs;
		break;
	case 4:
		result = lhs > rhs;
		break;
	case 5:
		result = lhs < rhs;
		break;
	}

	return result;
}

// clang-format off
int32_t scriptCompareSignedValue(op, lhs, rhs)
	uint8_t op;
	int32_t lhs;
	int32_t rhs;
// clang-format on
{
	int32_t result;

	switch (op & 7) {
	case 0:
		result = lhs == rhs;
		break;
	case 1:
		result = lhs != rhs;
		break;
	case 2:
		result = lhs >= rhs;
		break;
	case 3:
		result = lhs <= rhs;
		break;
	case 4:
		result = lhs > rhs;
		break;
	case 5:
		result = lhs < rhs;
		break;
	}

	return result;
}

static void scriptInstruction28to3F__garbage__(int32_t op)
{
	StackEntry entry;
	uint16_t shortArg;
	uint16_t offset;
	uint8_t pstat;
	uint8_t value;
	uint8_t unusedByte;
	int32_t newValue;

	switch (op) {
	case SCRIPT_OP_SET_SELECTION:
		MAIN_D_80135000 = 1;
		scriptShowSelection();
		longjmp(SCRIPT_JMP_BUF, 2);
		break;
	case SCRIPT_OP_JUMP_AND_LINK:
		skipOneReadOneUShort(&shortArg);
		entry.scriptPtr = SCRIPT_POINTER;
		entry.scriptId = ACTIVE_MAP_SCRIPT;
		entry.smth[0] = 1;
		pushScriptStack(&entry);
		SCRIPT_POINTER = (uint8_t *)((uint32_t)ACTIVE_SCRIPT + shortArg);
		break;
	}
}

// clang-format off
void scriptInstruction28to3F(op)
	uint8_t op;
// clang-format on
{
	int16_t *statPtr;
	int32_t intArg;
	int32_t result;
	uint16_t value;
	uint8_t byteArg1;
	uint8_t byteArg2;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint32_t sec;

	switch (op) {
	case SCRIPT_OP_GIVE_ITEM:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		result = giveItem(byteArg1, byteArg2);
		if (result != 0) {
			unsetTrigger(0);
		} else {
			setTrigger(0);
		}
		break;
	case SCRIPT_OP_REMOVE_ITEM:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		removeItem(byteArg1, byteArg2);
		break;
	case SCRIPT_OP_ADD_MONEY:
		skipOneReadInteger(&intArg);
		MONEY += intArg;
		if (MONEY >= 0xf4240) {
			MONEY = 0xf423f;
		}
		break;
	case SCRIPT_OP_REDUCE_MONEY:
		skipOneReadInteger(&intArg);
		MONEY -= intArg;
		if (MONEY < 0) {
			MONEY = 0;
		}
		break;
	case SCRIPT_OP_COMPARE_DATE:
		scriptCompareDate();
		break;
	case SCRIPT_OP_LEARN_MOVE:
		pollNextScriptUByte(&byteArg1);
		scriptLearnMove(byteArg1);
		break;
	case SCRIPT_OP_UNUSED_2E:
		pollNextScriptUByte(&byteArg1);
		break;
	case SCRIPT_OP_GIVE_CARD:
		pollNextScriptUByte(&byteArg1);
		byteArg2 = getCardAmount(byteArg1);
		if (byteArg2 < 9) {
			byteArg2++;
			setCardAmount(byteArg1, byteArg2);
		}
		break;
	case SCRIPT_OP_TAKE_CARD:
		pollNextScriptUByte(&byteArg1);
		byteArg2 = getCardAmount(byteArg1);
		if (byteArg2 != 0) {
			byteArg2--;
			setCardAmount(byteArg1, byteArg2);
		}
		break;
	case SCRIPT_OP_SET_MERIT:
		skipOneReadOneUShort(&value);
		MERIT = value;
		if (MERIT >= 0x2710) {
			MERIT = 0x270f;
		}
		break;
	case SCRIPT_OP_ADD_MERIT:
		skipOneReadOneUShort(&value);
		MERIT += value;
		if (MERIT >= 0x2710) {
			MERIT = 0x270f;
		}
		break;
	case SCRIPT_OP_REDUCE_MERIT:
		skipOneReadOneUShort(&value);
		MERIT = -value;
		if (MERIT < 0) {
			MERIT = 0;
		}
		break;
	case SCRIPT_OP_SET_STAT:
		pollOneUByteOneUShort(&byteArg1, &value);
		statPtr = getStatsPointer(byteArg1);
		*statPtr = enforceStatsLimits(byteArg1, value);
		scriptUpdateEnergyBoundaries(byteArg1, *statPtr);
		if (byteArg1 == SCRIPT_STAT_TAMER_LEVEL) {
			TAMER_ENTITY.tamerLevel = (uint8_t)TMP_TAMER_LEVEL;
		}
		if (byteArg1 == SCRIPT_STAT_LIVES) {
			PARTNER_ENTITY.lives = (uint8_t)TMP_LIVES;
		}
		break;
	case SCRIPT_OP_ADD_STAT:
		pollOneUByteOneUShort(&byteArg1, &value);
		statPtr = getStatsPointer(byteArg1);
		*statPtr += value;
		*statPtr = enforceStatsLimits(byteArg1, *statPtr);
		scriptUpdateEnergyBoundaries(byteArg1, *statPtr);
		if (byteArg1 == SCRIPT_STAT_TAMER_LEVEL) {
			TAMER_ENTITY.tamerLevel = (uint8_t)TMP_TAMER_LEVEL;
		}
		if (byteArg1 == SCRIPT_STAT_LIVES) {
			PARTNER_ENTITY.lives = (uint8_t)TMP_LIVES;
		}
		break;
	case SCRIPT_OP_REDUCE_STAT:
		pollOneUByteOneUShort(&byteArg1, &value);
		statPtr = getStatsPointer(byteArg1);
		intArg = *statPtr - (int16_t)value;
		if (byteArg1 != 9) {
			if (intArg < 0) {
				intArg = 0;
			}
		} else {
			if (intArg < -0x64) {
				intArg = -0x64;
			}
		}
		*statPtr = intArg;
		scriptUpdateEnergyBoundaries(byteArg1, *statPtr);
		if (byteArg1 == SCRIPT_STAT_TAMER_LEVEL) {
			TAMER_ENTITY.tamerLevel = (uint8_t)TMP_TAMER_LEVEL;
		}
		if (byteArg1 == SCRIPT_STAT_LIVES) {
			PARTNER_ENTITY.lives = (uint8_t)TMP_LIVES;
		}
		break;
	case SCRIPT_OP_ADVANCE_TO_DATE_AT:
		pollNextScriptUByte(&byteArg1);
		byteArg2 = readPStat(byteArg1 + 0);
		day = readPStat(byteArg1 + 1);
		hour = readPStat(byteArg1 + 2);
		minute = readPStat(byteArg1 + 3);
		sec = dateToSeconds(byteArg2, day, hour, minute);
		{
			uint32_t cur_sec = dateToSeconds(YEAR, DAY & 0xff,
			                                 HOUR & 0xff, MINUTE & 0xff);
			if (cur_sec < sec) {
				skipHours((sec - cur_sec) / 60);
			}
		}
		{
			int16_t prev_day = DAY;
			YEAR = (int32_t)byteArg2;
			DAY = (int32_t)day;
			HOUR = (int32_t)hour;
			MINUTE = (int32_t)minute;
			CURRENT_FRAME = HOUR * 1200 + MINUTE * 20;
			if (DAY != prev_day) {
				dailyPStatTrigger();
			}
		}
		break;
	case SCRIPT_OP_ADD_MINUTES_TO_DATE_AT:
	case SCRIPT_OP_ADD_MINUTES_TO_DATE_AT_2:
		pollNextScriptUByte(&byteArg1);
		pollNextInt(&intArg);
		byteArg2 = readPStat(byteArg1 + 0);
		day = readPStat(byteArg1 + 1);
		hour = readPStat(byteArg1 + 2);
		minute = readPStat(byteArg1 + 3);
		sec = dateToSeconds(byteArg2, day, hour, minute);
		if (op == 0x38) {
			intArg = sec + intArg;
		} else {
			intArg = sec - intArg;
			if (intArg < 0) {
				intArg = 0;
			}
		}
		secondsToDate(intArg, &byteArg2, &day, &hour, &minute);
		writePStat(byteArg1 + 0, byteArg2);
		writePStat(byteArg1 + 1, day);
		writePStat(byteArg1 + 2, hour);
		writePStat(byteArg1 + 3, minute);
		break;
	case SCRIPT_OP_UNUSED_3A:
	case SCRIPT_OP_UNUSED_3B:
	case SCRIPT_OP_UNUSED_3C:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		break;
	case SCRIPT_OP_UNUSED_3D:
	case SCRIPT_OP_UNUSED_3E:
		pollNextScriptUByte(&byteArg1);
		break;
	case SCRIPT_OP_STORE_DIGIMON_VALUE:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		byteArg1 = readPStat(byteArg1);
		byteArg1 = DIGIMON_DATA[byteArg1].type;
		writePStat(byteArg2, byteArg1);
		break;
	}

	longjmp(SCRIPT_JMP_BUF, 1);
}

// clang-format off
void scriptInstruction46to58(op)
	uint8_t op;
// clang-format on
{
	StackEntry entry;
	int16_t posX;
	int16_t posY;
	uint8_t byteArg1;
	uint8_t byteArg2;
	uint8_t byteArg3;
	uint8_t entityId;
	ScriptCameraMovement *b;

	switch (op) {
	case SCRIPT_OP_LOAD_DIGIMON:
		pollNextScriptUByte(&byteArg1);
		scriptLoadModel(byteArg1);
		break;
	case SCRIPT_OP_SET_DIGIMON:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		scriptSetDigimon(byteArg1, byteArg2, byteArg3);
		break;
	case SCRIPT_OP_UNLOAD_ENTITY:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		scriptUnloadEntity(byteArg1);
		break;
	case SCRIPT_OP_CALL_DIGIMON_ROUTINE:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		callDigimonRoutine(byteArg1);
		break;
	case SCRIPT_OP_WAIT_FOR_ENTITY:
		pollNextScriptUByte(&byteArg1);
		ACTIVE_INSTRUCTION = SCRIPT_OP_WAIT_FOR_ENTITY;
		if (byteArg1 == 0xff) {
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xc8) {
			byteArg1 = 0xa;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xca) {
			byteArg1 = 0xb;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xcb) {
			byteArg1 = 0xc;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xcc) {
			byteArg1 = 0xd;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xcd) {
			byteArg1 = 0xe;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xce) {
			byteArg1 = 0xf;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xcf) {
			byteArg1 = 0x10;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd0) {
			byteArg1 = 0x11;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd1) {
			byteArg1 = 0x12;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd2) {
			byteArg1 = 0x13;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd3) {
			byteArg1 = 0x14;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd4) {
			byteArg1 = 0x15;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xc9) {
			byteArg1 = 0x19;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd5) {
			byteArg1 = 0x19;
			SOME_SCRIPT_SYNC_BIT = 0;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd6) {
			byteArg1 = 0x1a;
			goto wait_for_entity_end;
		}
		byteArg1 = scriptIdToEntityId(byteArg1);
wait_for_entity_end:
		WAIT_FOR_ENTITY_ID = byteArg1;
		longjmp(SCRIPT_JMP_BUF, 2);
	case SCRIPT_OP_WARP_TO:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		SCRIPT_PARAM_1 = byteArg1;
		SCRIPT_STATE_2 = byteArg2;
		consumeMapChangeShowName();
		entry.smth[0] = 4;
		entry.smth[1] = byteArg3;
		pushScriptStack(&entry);
		longjmp(SCRIPT_JMP_BUF, 3);
	case SCRIPT_OP_ENTITY_LOOK_AT_ENTITY:
		scriptPauseGame(0xff);
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = &SCRIPT_MOVEMENT[entityId];
		b->type = 0;
		b->entityId = byteArg1;
		b->target = byteArg2;
		break;
	case SCRIPT_OP_ENTITY_SET_ROTATION:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextScriptShort(&posX);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = &SCRIPT_MOVEMENT[entityId];
		b->type = 1;
		b->entityId = byteArg1;
		b->posX = posX;
		break;
	case SCRIPT_OP_ENTITY_WALK_TO:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		scriptSetEntityWalking(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = &SCRIPT_MOVEMENT[entityId];
		b->type = 2;
		b->entityId = byteArg1;
		b->posX = posX;
		b->posY = posY;
		break;
	case SCRIPT_OP_MOVE_CAMERA_TO:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		b = &SCRIPT_MOVEMENT[10];
		b->type = 6;
		b->posX = posX;
		b->posY = posY;
		b->speed = (int32_t)byteArg1;
		break;
	case SCRIPT_OP_MOVE_CAMERA_TO_ENTITY:
		scriptPauseGame(0xff);
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		b = &SCRIPT_MOVEMENT[10];
		b->type = 7;
		b->entityId = byteArg1;
		b->speed = (int32_t)byteArg2;
		break;
	case SCRIPT_OP_ENTITY_WALK_TO_ENTITY:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		scriptSetEntityWalking(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = &SCRIPT_MOVEMENT[entityId];
		b->type = 3;
		b->entityId = byteArg1;
		b->target = byteArg3;
		break;
	case SCRIPT_OP_ENTITY_WALK_TO_WITH_CAMERA:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		scriptSetEntityWalking(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = &SCRIPT_MOVEMENT[entityId];
		b->type = 4;
		b->entityId = byteArg1;
		b->posX = posX;
		b->posY = posY;
		break;
	case SCRIPT_OP_ENTITY_WALK_TO_ENTITY_WITH_CAMERA:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		scriptSetEntityWalking(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = &SCRIPT_MOVEMENT[entityId];
		b->type = 5;
		b->entityId = byteArg1;
		b->target = byteArg3;
		break;
	case SCRIPT_OP_RESET_ENTITY_ORIGIN:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		resetEntityOrigin(byteArg1);
		break;
	case SCRIPT_OP_SET_TEXTBOX_ORIGIN:
		SCRIPT_POINTER++;
		pollNextTwoScriptShorts(&TEXTBOX_ORIGIN_X, &TEXTBOX_ORIGIN_Y);
		pollNextScriptShort(&TEXTBOX_ORIGIN_Z);
		break;
	case SCRIPT_OP_PLAY_ANIMATION:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		scriptStartAnimation(byteArg1, byteArg2);
		break;
	case SCRIPT_OP_SET_OBJ_VISIBILITY:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		setMapObjectsFlag(byteArg1, 1, byteArg2);
		break;
	case SCRIPT_OP_TELEPORT:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		SCRIPT_PARAM_1 = readPStat(byteArg1 + 0);
		SCRIPT_STATE_2 = readPStat(byteArg1 + 1);
		consumeMapChangeShowName();
		entry.smth[0] = 4;
		entry.smth[1] = 0xff;
		pushScriptStack(&entry);
		longjmp(SCRIPT_JMP_BUF, 3);
	}

	longjmp(SCRIPT_JMP_BUF, 1);
}

// clang-format off
void scriptInstruction5Ato5F(op)
	uint8_t op;
// clang-format on
{
	uint8_t byteArg1;
	uint8_t byteArg2;

	switch (op) {
	case SCRIPT_OP_PLAY_SOUND:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		playSound(byteArg1, byteArg2);
		break;
	case SCRIPT_OP_UNUSED_5B:
		pollNextScriptUByte(&byteArg1);
		break;
	case SCRIPT_OP_UNUSED_5C:
		pollNextScriptUByte(&byteArg1);
		break;
	case SCRIPT_OP_SET_BGM:
		pollNextScriptUByte(&byteArg1);
		playBGM(byteArg1);
		break;
	case SCRIPT_OP_STOP_BGM:
		pollNextScriptUByte(&byteArg1);
		resetBGM();
		break;
	case SCRIPT_OP_UNUSED_5F:
		pollNextScriptUByte(&byteArg1);
		break;
	}

	longjmp(SCRIPT_JMP_BUF, 1);
}

void handleMusicOverride(uint8_t *outFont, uint8_t *outVariant)
{
	uint8_t mode;

	mode = readPStat(PSTAT_245);
	switch (mode) {
	case 0:
		if (HOUR >= 6 && HOUR < 21) {
			*outVariant = 0;
		} else {
			*outVariant = 1;
		}
		break;
	case 1:
		*outVariant = 0;
		break;
	case 2:
		*outFont = 6;
		*outVariant = 2;
		break;
	case 3:
		*outFont = 0xb;
		*outVariant = 2;
		break;
	case 4:
		*outFont = 0xf;
		*outVariant = 2;
		break;
	case 5:
		*outFont = 0x15;
		*outVariant = 0;
		break;
	case 6:
		*outFont = 0x15;
		*outVariant = 1;
		break;
	case 7:
		*outFont = 0x1a;
		*outVariant = 0;
		break;
	case 8:
		*outFont = 0x1a;
		*outVariant = 1;
		break;
	case 9:
		*outFont = 0x1b;
		*outVariant = 0;
		break;
	case 0xa:
		*outFont = 0x1b;
		*outVariant = 1;
		break;
	}
}

// clang-format off
void scriptInstruction64to7E(op)
	uint8_t op;
// clang-format on
{
	StackEntry entry;
	int16_t posX;
	int16_t posY;
	int16_t posZ;
	int16_t posW;
	uint16_t triggerId;
	uint8_t byteArg1;
	uint8_t byteArg2;
	uint8_t byteArg3;
	uint8_t entityId;
	uint8_t padByte;
	ScriptCameraMovement *b;

	switch (op) {
	case SCRIPT_OP_CALL_ROUTINE:
		pollNextScriptUByte(&byteArg1);
		SCRIPT_PARAM_1 = byteArg1;
		switch (byteArg1) {
		case 0x16:
			if (checkTournamentMedalConditions() == -1) {
				break;
			}
			/* fall through */
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x06:
		case 0x08:
		case 0x09:
		case 0x0a:
		case 0x0b:
		case 0x0e:
		case 0x12:
		case 0x2f:
			ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
			SCRIPT_STATE_2 = 0;
			SCRIPT_TEXTBOX_MODE = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x07:
			byteArg1 = readPStat(0xfe);
			if ((CURRENT_SCREEN_ID == 0x6b) ||
			    (CURRENT_SCREEN_ID == 0x6c) ||
			    (CURRENT_SCREEN_ID == 0xa5) ||
			    (CURRENT_SCREEN_ID == 0x63)) {
				switch (byteArg1) {
				case 0:
					TRN2_setupHpTraining(CURRENT_SCREEN_ID);
					break;
				case 1:
					TRN2_setupOffenseTraining(CURRENT_SCREEN_ID);
					break;
				case 2:
					TRN2_setupSpeedTraining(CURRENT_SCREEN_ID);
					break;
				case 3:
					TRN2_setupDefenseTraining(CURRENT_SCREEN_ID);
					break;
				case 4:
					TRN2_setupMpTraining(CURRENT_SCREEN_ID);
					break;
				}
			} else {
				switch (byteArg1) {
				case 0:
					TRN_setupHpTraining(CURRENT_SCREEN_ID);
					break;
				case 1:
					TRN_setupOffenseTraining(CURRENT_SCREEN_ID);
					break;
				case 2:
					TRN_setupSpeedTraining(CURRENT_SCREEN_ID);
					break;
				case 3:
					TRN_setupDefenseTraining(CURRENT_SCREEN_ID);
					break;
				case 4:
					TRN_setupMpTraining(CURRENT_SCREEN_ID);
					break;
				case 5:
					TRN_setupBrainsTraining(CURRENT_SCREEN_ID);
					break;
				}
			}
			ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
			SCRIPT_TEXTBOX_MODE = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x11:
			break;
		case 0x0c:
			byteArg1 = readPStat(0xfe);
			if (byteArg1 != 0xff) {
				removeItem(byteArg1, 1);
			}
			break;
		case 0x0d:
			byteArg1 = readPStat(0xfe);
			if (byteArg1 != 0xff) {
				removeItem(byteArg1, 0x63);
			}
			break;
		case 0x0f:
			createShopBitsBox(1);
			break;
		case 0x10:
			triggerBoxCloseFlag(2);
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x13:
			byteArg1 = readPStat(0xfe);
			setDirtCartModel(byteArg1);
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x14:
			decreaseDirtPileSize();
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x15:
			byteArg1 = readPStat(0xf7);
			byteArg2 = readPStat(0xf8);
			resetMapObjectAnimation(byteArg1, byteArg2);
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x1d:
			DOO2_openEggBox();
			/* fall through */
		case 0x17:
		case 0x18:
		case 0x32:
			ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
			SCRIPT_TEXTBOX_MODE = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x19:
			MAIN_D_80134FE7 = readPStat(0xfe);
			break;
		case 0x1a:
			loadDirtCartModel();
			break;
		case 0x1b:
			loadDirtPileModel();
			break;
		case 0x1c:
			createMonochromonMoodBubble();
			break;
		case 0x1e:
			SHOP_VARIABLE = 0;
			break;
		case 0x1f:
			posX = readPStat(0xf4) + (readPStat(0xf3) << 8);
			SHOP_VARIABLE += posX;
			break;
		case 0x38:
			writePStat(0xf3, (SHOP_VARIABLE / 256) & 0xff);
			writePStat(0xf4, SHOP_VARIABLE & 0xff);
			break;
		case 0x20:
			initializeNamingBuffer(readPStat(0xfe));
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x21:
			setTrigger(0x25);
			setTrigger(0x26);
			setTrigger(0x27);
			if (DAY != 0x16) {
				byteArg1 = 0x95;
			} else {
				byteArg1 = 0x16;
			}
			writePStat(2, byteArg1);
			writePStat(3, 0x16);
			writePStat(4,
			           PARTNER_ENTITY.digimonEntity.entity.type);
			break;
		case 0x22:
			createMeramonShake();
			break;
		case 0x23:
			loadShopLibrary();
			break;
		case 0x24:
			readMapTFS(CURRENT_SCREEN_ID);
			break;
		case 0x27:
			loadTrainingLibrary(CURRENT_SCREEN_ID);
			break;
		case 0x30:
			openSaveMachine();
			/* fall through */
		case 0x25:
			ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
			SCRIPT_TEXTBOX_MODE = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x36:
			gameClearSave();
			ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
			SCRIPT_TEXTBOX_MODE = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x26:
			createNinjamonEffect();
			break;
		case 0x33:
			addFileReadRequestPath(MAIN_D_80130394,
			                       (int32_t)BOSS_EFE_TMD_BUFFER, 0, 0, 0);
			loadDynamicLibrary(EAB_REL, 0, 0, 0, 0);
			readVBALLSection(5, 0x73);
			loadMapSounds2(0x15);
			break;
		case 0x34:
			loadTextureFile(MAIN_D_801303A8, 0, 0);
			loadDynamicLibrary(ENDI_REL, 0, 1, 0, 0);
			loadMapSounds2(0x14);
			break;
		case 0x28:
			isSoundLoaded(0, 8);
			EAB_startBuildup(
				ENTITY_TABLE[scriptIdToEntityId(5)]);
			break;
		case 0x35:
			isSoundLoaded(0, 8);
			tamerSetState(0x10);
			SOME_SCRIPT_SYNC_BIT = 0;
			ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
			SCRIPT_TEXTBOX_MODE = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
			break;
		case 0x37:
			setLoopCountToOne(readPStat(0xfe));
			longjmp(SCRIPT_JMP_BUF, 2);
			break;
		case 0x29:
			spawnGearbox();
			break;
		case 0x2a:
			somethingToyTown((int8_t)readPStat(0xfe));
			break;
		case 0x2b:
			spawnToyTownBoxes();
			break;
		case 0x2c:
			openToyTownBox((int8_t)readPStat(0xfe));
			break;
		case 0x2d:
			fadeToWhite((int32_t)readPStat(0xfe));
			break;
		case 0x2e:
			fadeFromWhite((int32_t)readPStat(0xfe));
			break;
		case 0x31:
			spawnAngemonPedestal();
			break;
		}
		break;
	case SCRIPT_OP_REMOVE_CONDITION:
		pollNextScriptUByte(&byteArg1);
		PARTNER_PARA.condition &= ~byteArg1;
		break;
	case SCRIPT_OP_START_BATTLE:
		pollNextScriptUByte(&byteArg1);
		if (BATTLES_STARTED < 0x270f) {
			BATTLES_STARTED++;
		}
		byteArg1 = readPStat(0xfa);
		if (byteArg1 != 0) {
			for (byteArg1 = 0xfb; byteArg1 < 0xfe; byteArg1++) {
				byteArg2 = readPStat(byteArg1);
				if (byteArg2 != 0xff) {
					byteArg2 =
						scriptIdToEntityId(byteArg2);
					writePStat(byteArg1, byteArg2);
				}
			}
		}
		stopBGM();
		{
			int16_t outcome;

			outcome = startBattle(MAIN_D_80134F9C);
			writePStat(0xff, outcome);
			if (outcome == -1) {
				handleItemLoss();
				PARTNER_ENTITY.lives -= 1;
				if (PARTNER_ENTITY.lives == 0) {
					PARTNER_PARA.remainingLifetime = 0;
				}
				ACTIVE_SCRIPT = getScript(0);
				SCRIPT_POINTER = getScriptSection((uint8_t *)(int32_t)ACTIVE_SCRIPT, 0x4de);
				break;
			}
			if (outcome == 0) {
				if (BATTLES_FLED < 0x270f) {
					BATTLES_FLED++;
				}
				SCRIPT_PARAM_1 = PREVIOUS_SCREEN;
				SCRIPT_STATE_2 = PREVIOUS_EXIT;
				PREVIOUS_EXIT = CURRENT_EXIT;
				SCRIPT_MAP_CHANGE_SHOW_NAME = 0;
				entry.smth[0] = 4;
				entry.smth[1] = 0xff;
				pushScriptStack(&entry);
				longjmp(SCRIPT_JMP_BUF, 3);
			}
		}
		if (isTriggerSet(1) != 0) {
			setMovementEnabled(-1, 1);
			initializeTextbox();
			break;
		}

		b = &SCRIPT_MOVEMENT[10];
		b->type = 7;
		b->entityId = 0xfd;
		b->speed = 0xa;

		ACTIVE_INSTRUCTION = SCRIPT_OP_WAIT_FOR_ENTITY;
		WAIT_FOR_ENTITY_ID = 0xa;
		initializeTextbox();
		longjmp(SCRIPT_JMP_BUF, 2);
	case SCRIPT_OP_DELAY:
		skipOneReadOneUShort(&DELAY_FRAMES);
		ACTIVE_INSTRUCTION = SCRIPT_OP_DELAY;
		longjmp(SCRIPT_JMP_BUF, 2);
	case SCRIPT_OP_SET_TEXTBOX_MODE:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		TEXTBOX_CLOSE_MODE = byteArg1;
		if (byteArg1 != 2) {
			break;
		}
		AUTOCLOSE_FRAMES = byteArg2;
		break;
	case SCRIPT_OP_DEAL_DAMAGE:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		{
			int32_t damage;
			damage = (PARTNER_ENTITY.digimonEntity.stats.base.hp *
			          readPStat(byteArg1)) /
			         100;
			if (PARTNER_ENTITY.digimonEntity.stats.current
			                    .currentHP -
			            (int16_t)damage <
			    1) {
				damage = PARTNER_ENTITY.digimonEntity.stats
				                 .current.currentHP -
				         1;
			}
			PARTNER_ENTITY.digimonEntity.stats.current
				.currentHP -= damage;
			addEntityText((int32_t)ENTITY_TABLE[1], 0, 0, damage,
			              0);
		}
		break;
	case SCRIPT_OP_SET_AUTOTALK:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		byteArg1 = scriptIdToEntityId(byteArg1);
		if (byteArg1 == 0xff || byteArg1 < 2) {
			break;
		}
		NPC_ENTITIES[byteArg1 - 2].autotalk = byteArg2;
		break;
	case SCRIPT_OP_DATA:
		break;
	case SCRIPT_OP_ENTITY_MOVE_TO:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = &SCRIPT_MOVEMENT[entityId];
			b->type = 8;
			b->entityId = byteArg1;
			b->speed = byteArg2;
			b->posX = posX;
			b->posY = posY;
		}
		break;
	case SCRIPT_OP_ENTITY_MOVE_TO_ENTITY:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = &SCRIPT_MOVEMENT[entityId];
			b->type = 9;
			b->entityId = byteArg1;
			b->target = byteArg2;
			b->speed = byteArg3;
		}
		break;
	case SCRIPT_OP_ENTITY_MOVE_TO_WITH_CAMERA:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = &SCRIPT_MOVEMENT[entityId];
			b->type = 0xb;
			b->entityId = byteArg1;
			b->speed = byteArg2;
			b->posX = posX;
			b->posY = posY;
		}
		break;
	case SCRIPT_OP_ENTITY_MOVE_TO_ENTITY_WITH_CAMERA:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = &SCRIPT_MOVEMENT[entityId];
			b->type = 0xb;
			b->entityId = byteArg1;
			b->target = byteArg2;
			b->speed = byteArg3;
		}
		break;
	case SCRIPT_OP_ROTATE_3D_OBJECT:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		SCRIPT_MOVEMENT[11].type = 0xc;
		SCRIPT_MOVEMENT[11].entityId = byteArg1;
		SCRIPT_MOVEMENT[11].target = byteArg3;
		break;
	case SCRIPT_OP_MOVE_OBJECT_TO:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		pollNextTwoScriptBytes(&entityId, &padByte);
		pollNextTwoScriptShorts(&posX, &posY);
		byteArg1 += 0xc;
		b = &SCRIPT_MOVEMENT[byteArg1];
		b->type = 0xd;
		b->entityId = byteArg2;
		b->speed = byteArg3;
		b->target = entityId;
		b->targetX = posX;
		b->targetY = posY;
		break;
	case SCRIPT_OP_ENTITY_MOVE_TO_AXIS:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextScriptShort(&posX);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = &SCRIPT_MOVEMENT[entityId];
			b->type = 0xe;
			b->entityId = byteArg1;
			b->target = byteArg2;
			b->posX = posX;
			b->speed = byteArg3;
		}
		break;
	case SCRIPT_OP_ENTITY_MOVE_TO_AXIS_WITH_CAMERA:
		scriptPauseGame(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextScriptShort(&posX);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = &SCRIPT_MOVEMENT[entityId];
			b->type = 0xf;
			b->entityId = byteArg1;
			b->target = byteArg2;
			b->posX = posX;
			b->speed = byteArg3;
		}
		break;
	case SCRIPT_OP_SPAWN_ITEM:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		spawnItem(byteArg1, posX, posY);
		break;
	case SCRIPT_OP_SPAWN_CHEST:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptShorts(&posZ, &posW);
		pollNextScriptUShort(&triggerId);
		spawnChest(posX, posY, posZ, posW, byteArg1, triggerId);
		break;
	case SCRIPT_OP_SPAWN_BOULDER:
		pollNextScriptUByte(&byteArg1);
		spawnBoulder();
		break;
	case SCRIPT_OP_MOVE_BOULDER:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		moveBoulder(posX, posY);
		longjmp(SCRIPT_JMP_BUF, 2);
	case SCRIPT_OP_DESPAWN_BOULDER:
		pollNextScriptUByte(&byteArg1);
		removeObject(0xfb6, 0);
		break;
	case SCRIPT_OP_UNLOAD_DIGIMON:
		pollNextScriptUByte(&byteArg1);
		scriptUnloadModel(byteArg1);
		break;
	case SCRIPT_OP_COPY_PSTAT:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		byteArg1 = readPStat(byteArg1);
		writePStat(byteArg2, byteArg1);
		break;
	case SCRIPT_OP_SECTION_ON_EXIT:
		pollNextScriptUByte(&byteArg1);
		entry.smth[0] = 4;
		entry.smth[1] = byteArg1;
		pushScriptStack(&entry);
		writePStat(0, MAIN_D_80134FE7);
		break;
	case SCRIPT_OP_SET_RECT_IMPASSABLE:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		setImpassableRect(posX, posY, byteArg2,
		                  byteArg3);
		break;
	case SCRIPT_OP_SPAWN_SPRITE_AT_LOCATION:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptShorts(&posZ, &posW);
		spawnSpriteAtLocation(posX, posY, posZ, posW, byteArg1);
		break;
	case SCRIPT_OP_SPAWN_SPRITE_AT_ENTITY:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		spawnSpriteAtEntity(byteArg1, byteArg2, byteArg3);
		break;
	}

	longjmp(SCRIPT_JMP_BUF, 1);
}

void setMapHeadActive(void)
{
	uint16_t off;

	off = *(int16_t *)MAPHEAD_SCRIPT_PTR;
	ACTIVE_SCRIPT = MAPHEAD_SCRIPT_PTR;
	SCRIPT_POINTER = MAPHEAD_SCRIPT_PTR + off - 2;
}

void initializeScripts(void)
{
	MAPHEAD_SCRIPT_PTR = MAPHEAD_SCRIPT_BUFFER;
	SCRIPT_OFFSET_PTR = SCRIPT_OFFSET_TABLE;
	MAP_SCRIPT_PTR = MAP_SCRIPT_BUFFER;
	SCRIPT_STATE_PTR = &SCRIPT_STATE;
	TEXTBOX_LINES_PTR = TEXTBOX_LINES_BUFFER;

	readFile(MAIN_D_80130374, MAPHEAD_SCRIPT_PTR);
	readFileSection(MAIN_D_80130388, SCRIPT_OFFSET_PTR, 0, 0x2000);
	memset((void *)SCRIPT_STATE_PTR, 0, sizeof(*SCRIPT_STATE_PTR));

	MERIT = 0;
	SCRIPT_STACK_OFFSET = 0;
	BATTLES_STARTED = 0;
	BATTLES_FLED = 0;
	TOURNAMENTS_WON = 0;
	TOURNAMENT_WINS = 0;
	TOURNAMENTS_LOST = 0;
	CURRENT_SCRIPT_ID = 0xffff;
	ACTIVE_MAP_SCRIPT = 0xffff;
	TEXTBOX_ORIGIN_X = -0x270f;
	TEXTBOX_ORIGIN_Y = -0x270f;
	TEXTBOX_ORIGIN_Z = -0x270f;

	inputInit();
	dailyPStatTrigger();
	initializeLoadedNPCModels();
}

void initializeLoadedNPCModels(void)
{
	int32_t i;

	for (i = 0; i < 8; i++) {
		LOADED_DIGIMON_MODELS[i] = -1;
	}
}

// clang-format off
void runMapHeadScript(section)
	uint8_t section;
// clang-format on
{
	callScriptSection(0, section, 1);
	tickScript();
}

// clang-format off
void callScriptSection(scriptId, section, param)
	uint16_t scriptId;
	int32_t section;
	int32_t param;
// clang-format on
{
	int32_t i;

	ACTIVE_SCRIPT = getScript(scriptId);
	SCRIPT_POINTER =
		getScriptSection((uint8_t *)(int32_t)ACTIVE_SCRIPT,
	                         section);
	SCRIPT_SECTION_IS_EVENT = param;
	SCRIPT_SECTION = section;
	TEXTBOX_CLOSE_MODE = 0;
	CURRENT_DIALOGUE_OWNER = 0xfd;
	MAIN_D_80134FE7 = readPStat(0);
	ACTIVE_INSTRUCTION = 0;
	MAIN_D_80134FE9 = 0;
	MAIN_D_80134FEC = 0;
	MAIN_D_80134FF0 = 0;
	IS_SCRIPT_PAUSED = 0;
	SOME_SCRIPT_SYNC_BIT = 1;
	MAIN_D_80134F9C = TALKED_TO_ENTITY;
	for (i = 0; i < 0x16; i++) {
		SCRIPT_MOVEMENT[i].type = 0xff;
	}

	initializeTextbox();
}

void tickScriptedMovement(int32_t slot)
{
	ScriptCameraMovement *movement = &SCRIPT_MOVEMENT[slot];
	int32_t done;

	switch (movement->type) {
	case 0:
		done = tickLookAtEntity(movement->entityId, movement->target);
		break;
	case 1:
		done = tickEntitySetRotation(movement->entityId,
		                             movement->posX);
		break;
	case 2:
		done = tickEntityWalkTo((uint8_t)movement->entityId, 0xff,
		                        movement->posX, movement->posY, 0);
		break;
	case 3:
		done = tickEntityWalkTo((uint8_t)movement->entityId, movement->target,
		                        0, 0, 0);
		break;
	case 4:
		done = tickEntityWalkTo((uint8_t)movement->entityId, 0xff,
		                        movement->posX, movement->posY, 1);
		break;
	case 5:
		done = tickEntityWalkTo((uint8_t)movement->entityId, movement->target,
		                        0, 0, 1);
		break;
	case 6:
		done = tickCameraMoveTo(movement->posX,
		                        movement->posY,
		                        movement->speed);
		break;
	case 7:
		done = tickCameraMoveToEntity(movement->entityId,
		                              movement->speed);
		break;
	case 8:
		done = tickEntityMoveTo(movement->entityId, 0xff,
		                        movement->posX, movement->posY,
		                        movement->speed, 0);
		break;
	case 9:
		done = tickEntityMoveTo(movement->entityId, movement->target,
		                        0, 0, movement->speed, 0);
		break;
	case 10:
		done = tickEntityMoveTo(movement->entityId, 0xff,
		                        movement->posX, movement->posY,
		                        movement->speed, 1);
		break;
	case 0xb:
		done = tickEntityMoveTo(movement->entityId, movement->target,
		                        0, 0, movement->speed, 1);
		break;
	case 0xc:
		done = tickRotateDoor((int32_t)movement->entityId, movement->target);
		break;
	case 0xd:
		done = tickMoveObjectTo(movement->entityId, (slot & 0xff) - 0xc,
		                        movement->target, movement->targetX,
		                        movement->targetY);
		break;
	case 0xe:
		done = tickEntityMoveToAxis(movement->entityId,
		                            movement->posX,
		                            movement->target,
		                            movement->speed, 0);
		break;
	case 0xf:
		done = tickEntityMoveToAxis(movement->entityId,
		                            movement->posX,
		                            movement->target,
		                            movement->speed, 1);
		break;
	}

	if (done != 0) {
		if (slot < 10 && movement->type < 8) {
			if (slot == 0) {
				startAnimationTamer(0);
			} else if (slot == 1) {
				partnerStartAnimation(0);
			} else if (slot < 10) {
				scriptNPCStartAnimation(movement->entityId, 0);
			}
		}

		movement->type = 0xff;
	}
}

void pushScriptStack(StackEntry *entry)
{
	StackEntry *stackTop;

	stackTop = &SCRIPT_STATE_PTR->stack[SCRIPT_STACK_OFFSET];
	*stackTop = *entry;
	++SCRIPT_STACK_OFFSET;
}

void resetBGM(void)
{
	ACTIVE_BGM_FONT = 0xff;
	stopBGM();
}

int32_t popScriptStack(StackEntry *out)
{
	StackEntry *stackTop;
	StackEntry entry;

	if (SCRIPT_STACK_OFFSET != 0) {
		--SCRIPT_STACK_OFFSET;
		stackTop = &SCRIPT_STATE_PTR->stack[SCRIPT_STACK_OFFSET];
		entry = *stackTop;
	} else {
		entry.smth[0] = 0;
	}

	*out = entry;
}

void skipOneReadOneUShort(uint16_t *out)
{
	SCRIPT_POINTER++;
	pollNextScriptUShort(out);
}

void pollNextScriptUByte(uint8_t *out)
{
	*out = *SCRIPT_POINTER;
	SCRIPT_POINTER++;
}

void pollNextScriptUShort(uint16_t *out)
{
	*out = *(uint16_t *)SCRIPT_POINTER;
	SCRIPT_POINTER += 2;
}

void pollOneUByteOneUShort(uint8_t *outByte, uint16_t *outShort)
{
	pollNextScriptUByte(outByte);
	pollNextScriptUShort(outShort);
}

void skipOneReadTwoShort(uint16_t *out1, uint16_t *out2)
{
	SCRIPT_POINTER++;
	pollNextScriptUShort(out1);
	pollNextScriptUShort(out2);
}

void setTrigger(uint16_t trigger)
{
	uint8_t *ptr;
	uint8_t mask;
	uint8_t flags;

	getTriggerOffset(trigger, &ptr, &mask);
	flags = *ptr;
	*ptr = flags | mask;
}

void unsetTrigger(uint16_t trigger)
{
	uint8_t *ptr;
	uint8_t mask;
	uint8_t flags;

	getTriggerOffset(trigger, &ptr, &mask);
	flags = *ptr;
	*ptr = flags & ~mask;
}

void skipOnePollTwoScriptBytes(uint8_t *out1, uint8_t *out2)
{
	SCRIPT_POINTER++;
	pollNextScriptUByte(out1);
	pollNextScriptUByte(out2);
}

void pollNextTwoScriptBytes(uint8_t *out1, uint8_t *out2)
{
	pollNextScriptUByte(out1);
	pollNextScriptUByte(out2);
}

void skipOneReadInteger(int32_t *out)
{
	uint16_t lo;
	uint16_t hi;

	skipOneReadTwoShort(&lo, &hi);
	*out = lo + (hi << 16);
}

// clang-format off
void scriptLearnMove(moveId)
	uint8_t moveId;
// clang-format on
{
	learnMove((int16_t)(int32_t)moveId);
}

// clang-format off
uint8_t getCardAmount(cardId)
	int16_t cardId;
// clang-format on
{
	uint8_t *cardPtr;

	cardPtr = &SCRIPT_STATE_PTR->cards[cardId / 2];
	if ((cardId & 1) == 0) {
		return *cardPtr & 0xf;
	} else {
		return *cardPtr >> 4;
	}
}

// clang-format off
int32_t setCardAmount(cardId, value)
	int16_t cardId;
	int32_t value;
// clang-format on
{
	uint8_t *cardPtr;

	cardPtr = &SCRIPT_STATE_PTR->cards[cardId / 2];
	if ((cardId & 1) == 0) {
		*cardPtr = (*cardPtr & 0xf0) | value;
	} else {
		*cardPtr = (*cardPtr & 0xf) | (value << 4);
	}
}

// clang-format off
uint32_t dateToSeconds(years, days, hours, minutes)
	uint8_t years;
	uint8_t days;
	uint8_t hours;
	uint8_t minutes;
// clang-format on
{
	uint32_t total;

	total = minutes;
	total += hours * 60;
	total += days * 24 * 60;
	total += years * 30 * 24 * 60;

	return total;
}

void pollNextScriptTwoUShort(uint16_t *out1, uint16_t *out2)
{
	pollNextScriptUShort(out1);
	pollNextScriptUShort(out2);
}

void pollNextInt(int32_t *out)
{
	uint16_t lo;
	uint16_t hi;

	pollNextScriptTwoUShort(&lo, &hi);
	*out = lo + (hi << 16);
}

void secondsToDate(uint32_t totalMinutes, uint8_t *outYear,
                   uint8_t *outDay, uint8_t *outHour, uint8_t *outMinute)
{
	if (totalMinutes >= (255 * 43200)) {
		totalMinutes = 59 + (23 * 60) + (14 * 1440) + (25 * 43200);
	}

	*outMinute = totalMinutes % 60;
	totalMinutes /= 60;
	*outHour = totalMinutes % 24;
	totalMinutes /= 24;
	*outDay = totalMinutes % 30;
	*outYear = totalMinutes / 30;
}

void scriptLoadModel(int32_t modelId)
{
	int32_t i;

	for (i = 0; i < 8; i++) {
		if (LOADED_DIGIMON_MODELS[i] == -1) {
			LOADED_DIGIMON_MODELS[i] = modelId;
			loadNPCModel(modelId);
			return;
		}
	}
}

void pollNextScriptShort(int16_t *out)
{
	*out = *(int16_t *)SCRIPT_POINTER;
	SCRIPT_POINTER += 2;
}

void pollNextTwoScriptShorts(int16_t *out1, int16_t *out2)
{
	pollNextScriptShort(out1);
	pollNextScriptShort(out2);
}

// clang-format off
void playBGM(bgmId)
	uint8_t bgmId;
// clang-format on
{
	uint8_t font;
	uint8_t variant;

	font = bgmId;
	if (font == 0xff) {
		resetBGM();
		return;
	}

	handleMusicOverride(&font, &variant);
	ACTIVE_BGM_FONT = font;
	ACTIVE_BGM_TRACK = variant;
	stopBGM();
	playMusic(font, variant);
}

void updateBGM(void)
{
	uint8_t font;
	uint8_t variant;

	font = ACTIVE_BGM_FONT;
	if (font == 0xff) {
		resetBGM();
		return;
	}

	handleMusicOverride(&font, &variant);
	if (ACTIVE_BGM_FONT != font || ACTIVE_BGM_TRACK != variant) {
		ACTIVE_BGM_FONT = font;
		ACTIVE_BGM_TRACK = variant;
		stopBGM();
		playMusic(font, variant);
	}
}

void forceUpdateBGM(void)
{
	uint8_t font;
	uint8_t variant;

	font = ACTIVE_BGM_FONT;
	if (font == 0xff) {
		resetBGM();
		return;
	}

	handleMusicOverride(&font, &variant);
	ACTIVE_BGM_FONT = font;
	ACTIVE_BGM_TRACK = variant;
	stopBGM();
	playMusic(font, variant);
}

void scriptUnloadModel(int16_t modelId)
{
	int32_t i;

	for (i = 0; i < 8; i++) {
		if (LOADED_DIGIMON_MODELS[i] == modelId) {
			LOADED_DIGIMON_MODELS[i] = -1;
			unloadDigimonModel(modelId);
			return;
		}
	}
}

// clang-format off
void getTriggerOffset(trigger, outPtr, outMask)
	uint16_t trigger;
	uint8_t **outPtr;
	uint8_t *outMask;
// clang-format on
{
	uint8_t bit;

	bit = trigger % 8;
	*outPtr = &SCRIPT_STATE_PTR->triggers[trigger / 8];
	*outMask = 1;
	while (bit != 0) {
		*outMask <<= 1;
		bit--;
	}
}

// clang-format off
int32_t _hasMove(moveId)
	uint8_t moveId;
// clang-format on
{
	return hasMove((int16_t)(int32_t)moveId);
}

void returnFromScriptFile(void)
{
	StackEntry entry;
	uint8_t *script;
	uint8_t *section;
	uint8_t type;

	for (;;) {
		popScriptStack(&entry);
		type = entry.smth[0];
		if (type == 0) {
			break;
		}
		if (type == 3) {
			readMapTFS(CURRENT_SCREEN_ID);
			setupMap(CURRENT_SCREEN_ID, 0);
			MAIN_D_80134FEC = 0;
			script = getScript(CURRENT_SCRIPT_ID);
			section = getScriptSection(script, 0xfe);
			if (section != 0) {
				ACTIVE_SCRIPT = script;
				SCRIPT_POINTER = section;
				longjmp(SCRIPT_JMP_BUF, 1);
			}
		} else if (type == 4) {
			if (entry.smth[1] != 0xff) {
				ACTIVE_SCRIPT = getScript(CURRENT_SCRIPT_ID);
				SCRIPT_POINTER = getScriptSection(ACTIVE_SCRIPT, entry.smth[1]);
			} else {
				setMapHeadActive();
			}
			longjmp(SCRIPT_JMP_BUF, 2);
		}
	}
	IS_SCRIPT_PAUSED = 1;
	longjmp(SCRIPT_JMP_BUF, 2);
}

uint8_t readPStat(uint8_t index)
{
	return SCRIPT_STATE_PTR->pstats[index];
}

void scriptPauseGame(int32_t owner)
{
	int32_t entityId;

	if (MAIN_D_80134FF0 == 0) {
		MAIN_D_80134FF0 = 1;
		setMovementEnabled(0, 1);
		setMovementEnabled(1, 1);
		unsetCameraFollowPlayer();
		writePStat(0, 3);
		clearTextArea();
		stopGameTime();

		if (SCRIPT_SECTION_IS_EVENT != 0 && isTriggerSet(TRIGGER_44) == 0) {
			entityId = scriptIdToEntityId(SCRIPT_SECTION) & 0xff;
			if (entityId != 0xff) {
				SCRIPT_MOVEMENT[entityId].type = 0;
				SCRIPT_MOVEMENT[entityId].entityId = SCRIPT_SECTION;
				SCRIPT_MOVEMENT[entityId].target = 0xfd;
				SCRIPT_MOVEMENT[0].type = 0;
				SCRIPT_MOVEMENT[0].entityId = 0xfd;
				SCRIPT_MOVEMENT[0].target = SCRIPT_SECTION;
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
	uint8_t flags;

	getTriggerOffset(trigger, &ptr, &mask);
	flags = *ptr;

	return (flags & mask) != 0;
}

void writePStat(uint8_t index, uint8_t value)
{
	uint8_t *ptr;

	ptr = &SCRIPT_STATE_PTR->pstats[index];
	*ptr = value;
}

void readFileSection(char *filename, void *dest, uint32_t offset,
                     uint32_t size)
{
	CdlFILE file;
	char path[64];
	uint8_t mode;
	int32_t sector;

	mode = 0x80;

	if (LOAD_FILE_SECTION_STARTPOS == 0) {
		path[0] = '\\';
		strcpy(&path[1], filename);
		strcat(path, MAIN_D_801345F0);
		if (CdSearchFile(&file, path) == 0) {
			return;
		}

		while (CdControl(0xe, &mode, 0) == 0)
			;

		LOAD_FILE_SECTION_STARTPOS = CdPosToInt(&file.pos);
	} else {
		while (CdControl(0xe, &mode, 0) == 0)
			;
	}

	sector = LOAD_FILE_SECTION_STARTPOS + (offset >> 11);
	file.pos = *CdIntToPos(sector, &file.pos);

	while (CdControl(2, (u_char *)&file.pos, 0) == 0)
		;
	while (CdRead(size >> 11, dest, mode) == 0)
		;
	while (CdReadSync(0, 0) > 0)
		;
}

void tickScriptedMovements(void)
{
	int32_t i;

	for (i = 0; i < 0x16; i++) {
		if (SCRIPT_MOVEMENT[i].type != 0xff) {
			tickScriptedMovement(i);
		}
	}
}

// clang-format off
void scriptInstructionFBtoFF(op)
	uint8_t op;
// clang-format on
{
	StackEntry entry;

	switch (op) {
	case 0xff:
	case 0xfe:
		returnFromScriptFile();
		break;
	case 0xfb:
		skipOneReadTwoShort(&CURRENT_SCRIPT_ID, &CURRENT_SCREEN_ID);

		entry.smth[0] = 3;
		pushScriptStack(&entry);

		MAIN_D_80134FEC = 1;

		resetBGM();
		loadMap(CURRENT_SCREEN_ID);
		longjmp(SCRIPT_JMP_BUF, 1);
		break;
	}
}
