#include <string.h>

#include <libcd.h>
#include <libetc.h>

#include <dw/clock.h>
#include <dw/doo2.h>
#include <dw/eab.h>
#include <dw/fade.h>
#include <dw/file.h>
#include <dw/file_queue.h>
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
#include <dw/text.h>
#include <dw/tournament.h>
#include <dw/trigger.h>
#include <dw/trn.h>
#include <dw/trn2.h>
#include <dw/ui.h>
#include <dw/utils.h>

typedef struct {
	uint8_t data[8];
} SelectionBoxUVData;

typedef struct {
	int8_t data[8];
} SelectionBoxOffsetData;

typedef struct {
	char s[12];
} BoxLabel;

typedef struct {
	uint32_t usedRows;
	TextBoxData box[6];
} TextBoxTable;

extern uint8_t SCRIPT_SECTION;
extern uint8_t SCRIPT_MAP_CHANGE;
extern int32_t SCRIPT_HAS_CONTROL;
extern uint8_t MAIN_D_801BE6B5[];
extern uint8_t MAIN_D_801BE6B6[];
extern uint8_t *SCRIPT_HEADER_PTR;
extern uint8_t *SCRIPT_DATA_PTR;
extern char SCRIPT_FILE_PATH[];
extern int8_t MAIN_STATE;
extern int16_t SCRIPT_MAP_CHANGE_STATE;
extern uint8_t SCRIPT_MOVEMENTS[];
extern GsOT *ACTIVE_ORDERING_TABLE;
extern SelectionBoxUVData MAIN_D_8013460C;
extern SelectionBoxUVData MAIN_D_80134614;
extern SelectionBoxUVData MAIN_D_8013461C;
extern SelectionBoxUVData MAIN_D_80134624;
extern SelectionBoxOffsetData MAIN_D_8013462C;
extern SelectionBoxOffsetData MAIN_D_80134634;
extern SelectionBoxOffsetData MAIN_D_8013463C;
extern SelectionBoxOffsetData MAIN_D_80134644;
extern SelectionBoxOffsetData MAIN_D_8013464C;
extern SelectionBoxOffsetData MAIN_D_80134654;
extern int16_t NAMING_LABEL_LAYOUT[];
extern uint8_t ACTIVE_BGM_VARIANT;
extern uint8_t MAIN_D_80134F82;
extern uint16_t MAIN_D_801307A0[10];
extern char NAMING_PREVIEW_PREFIX[4];
extern uint8_t TRADE_WANTED_ITEMS[];
extern uint8_t TRADE_GIVEN_ITEMS[];
extern uint8_t JUKEBOX_TRACKS[];
extern uint8_t JUKEBOX_PLAYING;
extern uint8_t ITEM_KEEPER_SIDE;
extern int32_t CARD_PRICES[];
extern uint16_t MAIN_D_80134608[2];
extern uint8_t MAIN_D_80134F81;
extern uint32_t POLLED_INPUT;
extern RECT ITEM_MENU_POPUP_ORIGINS[];
extern char PATH_ETCHI_BOSS_EFE_TMD[];
extern char PATH_ETCHI_OP_TIM[];
extern uint8_t PREVIOUS_SCREEN;
extern uint8_t PREVIOUS_EXIT;
extern uint8_t CURRENT_EXIT;
extern uint8_t SCRIPT_DOOR_MOVEMENT[];
extern int8_t TALKED_TO_ENTITY;
extern uint8_t MAPHEAD_DATA[];
extern uint8_t SCRIPT_HEADER[];
extern uint8_t SCRIPT_DATA[];
extern ScriptState SCRIPT_STATE;
extern uint8_t TEXT_BUFFERS[];
extern char MAPHEAD_FILE_PATH[];
extern BoxLabel ITEM_KEEPER_HAND_LABEL;
extern BoxLabel ITEM_KEEPER_STORED_LABEL;
extern TextBoxTable TEXT_BOX_TABLE;
extern char TRADE_HEADER_LABELS[];
extern char **NAMING_CHAR_PAGES[];
extern char MAIN_D_801B1D26[];
extern char NAMING_TITLE[5];
extern char NAMING_BUTTON_LABELS[8];
extern int16_t ITEM_MENU_CURSOR_WIDTHS[];
extern uint8_t *CURRENT_SCRIPT_PTR;

void unsetCameraFollowPlayer(void);
int32_t MAIN_func_800D8E64(int32_t param_1, int32_t param_2, int32_t param_3);
int32_t tickRemoveMist(void);
int32_t tickSaveMachine(void);
int32_t tickGameClearSave(void);
int32_t isTrainingComplete(void);
void setCameraFollowPlayer(void);
void setFoodTimer(int32_t type);
void setActiveAnim(int32_t state);
void renderItemMenuHeader(uint8_t boxId, int32_t idx, int16_t x, int16_t y);
void renderItemMenuScrollbar(ItemMenuBox *box);
void renderItemMenuRows(ItemMenuBox *box, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int32_t flag);
void renderSelectionCursor(int32_t x, int32_t y, int16_t w, int16_t h, int32_t layer);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t x, int32_t y, int32_t w, int32_t h);
int32_t random(int32_t limit);
void *allocateArray(uint32_t size);
void freeArray(uint32_t *array);
uint8_t *padWithSpaces(uint8_t *str, int32_t width, int32_t used);
void terminateString(uint8_t *str, int32_t flag);
void drawTextRow(uint8_t *str, int16_t x, int16_t y, int32_t flag);
int32_t hasMove(int32_t moveId);
void learnMove(int32_t moveId);
void unloadDigimonModel(int32_t a0);
void itemMenuSelectFirst(ItemMenuBox *box);
int32_t isXPressedAfterDialogue(void);
int32_t isKeyDown(int32_t mask);
int32_t openQuantityBox(RECT *origin);
int32_t openItemConfirmBox(RECT *origin);
ItemMenuBox *getActiveItemMenu(void);
int32_t openItemDescriptionBox(ItemMenuBox *box, RECT *origin, int32_t uiBoxId);
void itemMenuJumpToFirst(ItemMenuBox *box, int32_t startRow, int32_t style);
void itemMenuJumpToLast(ItemMenuBox *box, int32_t startRow, int32_t style);
void itemMenuSelectLast(ItemMenuBox *box);
int32_t tickItemMenuPageFlip(ItemMenuBox *box);
void itemMenuCursorUp(ItemMenuBox *box, int32_t style);
void itemMenuCursorDown(ItemMenuBox *box, int32_t style);
void unloadDigimonModel(int32_t digimonType);
void loadNPCModel(int32_t modelId);
void renderItemKeeperBox(ItemMenuBox *box, int8_t flag);
void tickTradeMenu(void);
void renderTradeMenuBox(void);
void tickCardMenu(void);
void renderCardMenuBox(void);
void tickTransportMenu(void);
void renderTransportMenuBox(void);
void updateNamingPreview(void);
void itemKeeperFocusStored(int32_t itemId);
void itemKeeperFocusHand(int32_t a0);
void tickJukeboxMenu(void);
void renderJukeboxMenuBox(void);
void tickNamingBox(void);
void renderNamingBox(void);
void layoutNamingBox(void);
int32_t scriptCompareSigned(int32_t op, int32_t lhs, int32_t rhs);
void updateBGM(void);
void forceUpdateBGM(void);
void pollNextScriptTwoUShort(uint16_t *out1, uint16_t *out2);
void tickItemKeeperMenu(void);
void renderItemKeeperStoredBox(void);
void layoutItemKeeperList(int32_t boxIndex);
void renderItemKeeperHandBox(void);
int32_t itemKeeperMoveOne(void);
int32_t itemKeeperMoveTen(void);
int32_t itemKeeperMoveAll(void);
void layoutTradeList(void);
void namingDeleteLast(void);
void terminateNamingBuffer(void);
void namingSelectionLeft(int16_t col, int16_t row, int32_t specialIdx);
void namingSelectionRight(int16_t col, int16_t row, int32_t specialIdx);
int32_t pickMeritCard(void);
void namingSelectionUp(int16_t column, int16_t row);
void namingSelectionDown(int16_t column, int16_t row);
void renderLinePrimitive(uint32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t order, uint32_t mode);
void renderNamingUnderscore(uint8_t boxId, int16_t x, int16_t y, int32_t w);
void renderSelectionBox(void);
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
void setRectImpassible(int16_t x, int16_t y, int8_t w, int8_t h);
void addEntityText(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
void setLoopCountToOne(int32_t a0);
void MAIN_func_800D9360(int32_t a0);
int32_t loadTextureFile(char *path, uint32_t *outTPage, uint32_t *outClut);
void setMapHeadActive(void);
int32_t tickMoveCameraTo(int32_t x, int32_t y, uint8_t speed);
int32_t tickMoveCameraToEntity(uint32_t scriptId, uint8_t speed);
int32_t tickMoveObjectTo(uint32_t scriptId1, uint32_t scriptId2, int32_t angle, int32_t targetX, int32_t targetY);
void initializeScripts(void);
void initializeLoadedNPCModels(void);
void runMapHeadScript(int32_t section);
void renderRaisedFrame(uint8_t boxId, int16_t x, int16_t y, int32_t w, int16_t h);
void getVRAMModeCoords(int32_t mode, int32_t *outX, int32_t *outClut);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void GsSortBoxFill(GsBOXF *bp, GsOT *otp, u_short pri);
void renderHorizontalLine(uint8_t boxId, int16_t x, int16_t y, int32_t w);
void drawString(char *str, int32_t x, int32_t y);
int32_t flipMenuPage(uint8_t boxId);
void MAIN_func_800D634C(int32_t param_1, int32_t param_2);

/*
 * NAMING_CURSOR is row * 5 + column in the grid of characters, or one of the
 * buttons under it.
 */
#define NAMING_ON_BUTTON	0x8000
#define NAMING_BUTTON_BACK	0x8000
#define NAMING_BUTTON_OK	0x8001

static void *script_interp_text_order[] = {
	renderNamingUnderscore,
	renderNameDisplayBox,
	renderSelectionBox,
	namingSelectionDown,
	namingSelectionUp,
	namingSelectionRight,
	namingSelectionLeft,
	terminateNamingBuffer,
	namingDeleteLast,
	updateNamingPreview,
	layoutNamingBox,
	renderNamingBox,
	tickNamingBox,
	setupNameDisplayBox,
	setupNameSelectorBox,
	showNewgameSelection,
	showNewgameDialogue,
	setupNewGameDialogueBox,
	layoutTradeList,
	renderTradeMenuBox,
	tickTradeMenu,
	renderTransportMenuBox,
	tickTransportMenu,
	renderJukeboxMenuBox,
	tickJukeboxMenu,
	renderItemKeeperBox,
	itemKeeperFocusHand,
	itemKeeperFocusStored,
	itemKeeperMoveAll,
	itemKeeperMoveTen,
	itemKeeperMoveOne,
	renderItemKeeperHandBox,
	layoutItemKeeperList,
	renderItemKeeperStoredBox,
	tickItemKeeperMenu,
	pickMeritCard,
	renderCardMenuBox,
	tickCardMenu,
	markTradeDone,
	openTradeMenuBox,
	fillTradeLists,
	openTransportMenuBox,
	fillTransportList,
	openJukeboxMenuBox,
	fillJukeboxList,
	openItemKeeperBoxes,
	fillItemKeeperLists,
	fillMeritItemList,
	fillOwnedCardList,
	openCardMenuBox,
	fillCardShopList,
	showCardTextbox,
	rollCard,
	fillLostItemList,
	scriptHasMove,
	getTriggerOffset,
	pollNextScriptTwoUShort,
	scriptUnloadModel,
	forceUpdateBGM,
	updateBGM,
	playBGM,
	pollNextTwoScriptShorts,
	pollNextScriptShort,
	scriptLoadModel,
	minutesToDate,
	pollNextInt,
	dateToMinutes,
	setCardAmount,
	getCardAmount,
	scriptLearnMove,
	skipOneReadInteger,
	scriptCompareSigned,
	scriptCompareValues,
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
	beginScriptEvent,
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
	scriptStartWalkAnimation,
	MAIN_func_801053EC,
	scriptUpdateEnergyBoundaries,
	scriptConditionBlock,
	returnFromScriptFile,
	returnFromScriptFile,
	setMapHeadActive,
	scriptInstruction64to7E,
	scriptInstruction5Ato5F,
	scriptInstruction46to58,
	scriptInstruction28to3F,
	scriptInstruction10to27,
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

	tickTextboxes(0);
	tickScriptedMovements();
	if (SCRIPT_MAP_CHANGE == 0x4b) {
		if (MAIN_func_800D8E64((int16_t)MAIN_D_80134FF8,
		                       (int16_t)SELECTION_MENU_STATE,
		                       MAIN_D_80134FA0)) {
			SCRIPT_HAS_CONTROL = 0;
			beginScriptEvent(SPEAKER_NONE);
			SCRIPT_MAP_CHANGE = 0;
		}
	}

	switch (ACTIVE_INSTRUCTION) {
	case SCRIPT_OP_WAIT:
		if (SCRIPT_WAIT_TIMER == 0) {
			ACTIVE_INSTRUCTION = 0;
		}
		break;
	case SCRIPT_OP_BUILTIN:
		/* The routine is picked by MAIN_D_80134FF8. */
		switch (MAIN_D_80134FF8) {
		case 3:
		case 4:
		case 5:
		case SCRIPT_BUILTIN_TAKE_ONE_ITEM:
		case SCRIPT_BUILTIN_TAKE_ALL_OF_ITEM:
		case SCRIPT_BUILTIN_OPEN_MONEY_BOX:
		case SCRIPT_BUILTIN_CLOSE_MONEY_BOX:
		case 17:
		case SCRIPT_BUILTIN_SET_DIRT_CART:
		case SCRIPT_BUILTIN_SHRINK_DIRT_PILE:
		case SCRIPT_BUILTIN_RESET_MAP_OBJECT_ANIM:
		case SCRIPT_BUILTIN_SET_TIME_SPEED:
		case 26:
		case 27:
		case SCRIPT_BUILTIN_MOOD_BUBBLE:
		case SCRIPT_BUILTIN_CLEAR_PRICE:
		case SCRIPT_BUILTIN_PRICE_FROM_TEXT_ARGS:
		case 33:
		case 34:
		case SCRIPT_BUILTIN_LOAD_SHOP:
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
		case SCRIPT_BUILTIN_PRICE_TO_TEXT_ARGS:
			goto done;
		case SCRIPT_BUILTIN_PICK_ITEM:
			tickPickItemMenu();
			break;
		case SCRIPT_BUILTIN_MERIT_SHOP:
			tickMeritShop();
			break;
		case SCRIPT_BUILTIN_SHOP:
			if (!isTriggerSet(TRIGGER_3)) {
				tickItemShop();
			} else {
				tickLostItemShop();
			}
			break;
		case SCRIPT_BUILTIN_CARD_SHOP:
			if (!isTriggerSet(TRIGGER_3)) {
				if (!isTriggerSet(TRIGGER_4)) {
					rollCardPack();
				} else {
					tickCardShop();
				}
			} else {
				tickCardSellShop();
			}
			break;
		case SCRIPT_BUILTIN_ITEM_KEEPER:
			tickItemKeeper();
			break;
		case SCRIPT_BUILTIN_COLLECTOR_TRADE:
			tickCollectorTrade();
			break;
		case SCRIPT_BUILTIN_JUKEBOX:
			openJukebox();
			break;
		case SCRIPT_BUILTIN_TRANSPORT:
			tickTransport();
			break;
		case SCRIPT_BUILTIN_TRAINING:
			if (isTrainingComplete()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case SCRIPT_BUILTIN_TOURNAMENT_SCHEDULE:
			initTournamentSchedule();
			break;
		case SCRIPT_BUILTIN_TOURNAMENT:
			scriptStartTournament();
			break;
		case SCRIPT_BUILTIN_TOURNAMENT_MEDAL:
			scriptCheckTournamentMedal();
			break;
		case SCRIPT_BUILTIN_OPEN_CHEST_TRAY:
			if (tickOpenChestTray(readPStat(PSTAT_BUILTIN_ARG))) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case SCRIPT_BUILTIN_CLOSE_CHEST_TRAY:
			if (tickCloseChestTray(readPStat(PSTAT_BUILTIN_ARG))) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case SCRIPT_BUILTIN_EGG_BOX:
			if (DOO2_tickEggInput()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case SCRIPT_BUILTIN_REMOVE_MIST:
			if (tickRemoveMist()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case SCRIPT_BUILTIN_FORGET_MOVES:
			lostAllLives();
			break;
		case SCRIPT_BUILTIN_SAVE_MACHINE:
			if (tickSaveMachine()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case SCRIPT_BUILTIN_GAME_CLEAR_SAVE:
			if (tickGameClearSave()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case SCRIPT_BUILTIN_ANGEMON_PEDESTAL:
			if (moveAngemonPedestal()) {
				ACTIVE_INSTRUCTION = 0;
			}
			break;
		case SCRIPT_BUILTIN_NAMING:
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
	case SCRIPT_OP_WAIT_MOVEMENT:
		if ((entityId = SCRIPT_WAIT_MOVEMENT_SLOT) == 0x19) {
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
					if (SCRIPT_MOVEMENTS[offset] != 0xff) {
						goto found;
					}
				}
				ACTIVE_INSTRUCTION = 0;
found:;
			}
		} else {
			if (SCRIPT_MOVEMENTS[entityId * 0xc] == 0xff) {
				ACTIVE_INSTRUCTION = 0;
			}
		}
		break;
	case SCRIPT_OP_CHOICE:
	case SCRIPT_OP_TEXT:
	case 0xff:
		break;
	}

	if (SCRIPT_MAP_CHANGE) {
		return IS_SCRIPT_PAUSED;
	}

	if (ACTIVE_INSTRUCTION) {
		return IS_SCRIPT_PAUSED;
	}
setjmp_retry:
	ret = setjmp(SCRIPT_JMP_BUF);
	if (ret == 0) {
		op = *SCRIPT_PC++;
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
			closeAllTextboxes();
			writePStat(0, SCRIPT_SAVED_TIME_SPEED);
			SCRIPT_MAP_CHANGE = 0x4b;
			SCRIPT_MAP_CHANGE_STATE = 0;
			return IS_SCRIPT_PAUSED;
		}
	}

	if (IS_SCRIPT_PAUSED) {
		closeAllTextboxes();
		writePStat(0, SCRIPT_SAVED_TIME_SPEED);
		if (SCRIPT_HAS_CONTROL == 1) {
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
	readFileSection(SCRIPT_FILE_PATH, SCRIPT_DATA_PTR, table[mapId],
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

void scriptInstruction10to27(int32_t op)
{
	StackEntry entry;
	uint16_t shortArg;
	uint16_t offset;
	uint8_t pstat;
	uint8_t value;
	uint8_t unusedByte;
	int32_t newValue;

	switch (op) {
	case SCRIPT_OP_CHOICE:
		DIALOGUE_BOX_MODE = 1;
		scriptShowSelection();
		longjmp(SCRIPT_JMP_BUF, 2);
		break;
	case SCRIPT_OP_CALL:
		skipOneReadOneUShort(&shortArg);
		entry.scriptPtr = SCRIPT_PC;
		entry.scriptId = ACTIVE_MAP_SCRIPT;
		entry.smth[0] = 1;
		pushScriptStack(&entry);
		SCRIPT_PC =
			(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR + shortArg);
		break;
	case SCRIPT_OP_CALL_SCRIPT:
		skipOneReadTwoShort(&shortArg, &offset);
		entry.scriptPtr = SCRIPT_PC;
		entry.scriptId = ACTIVE_MAP_SCRIPT;
		entry.smth[0] = 1;
		pushScriptStack(&entry);
		CURRENT_SCRIPT_PTR = getScript(shortArg);
		SCRIPT_PC = getScriptSection(
			(uint8_t *)(int32_t)CURRENT_SCRIPT_PTR, offset);
		break;
	case SCRIPT_OP_RETURN:
		SCRIPT_PC++;
		popScriptStack(&entry);
		CURRENT_SCRIPT_PTR = getScript(entry.scriptId);
		SCRIPT_PC = (uint8_t *)entry.scriptPtr;
		break;
	case SCRIPT_OP_JUMP:
		skipOneReadOneUShort(&shortArg);
		SCRIPT_PC =
			(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR + shortArg);
		break;
	case SCRIPT_OP_JUMP_SCRIPT:
		skipOneReadTwoShort(&shortArg, &offset);
		CURRENT_SCRIPT_PTR = getScript(shortArg);
		SCRIPT_PC = getScriptSection(
			(uint8_t *)(int32_t)CURRENT_SCRIPT_PTR, offset);
		break;
	case SCRIPT_OP_SWITCH:
		pollOneUByteOneUShort(&pstat, &shortArg);
		value = readPStat(pstat);
		if (value >= shortArg) {
			value = (shortArg - 1) & 0xff;
		}
		shortArg = *(uint16_t *)(SCRIPT_PC + (value << 1));
		SCRIPT_PC =
			(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR + shortArg);
		break;
	case SCRIPT_OP_CONDITION:
		SCRIPT_PC++;
		scriptConditionBlock();
		break;
	case SCRIPT_OP_TEXT:
		SCRIPT_PC++;
		if (DIALOGUE_BOX_MODE == 2) {
			showTextbox(0, SPEAKER_NONE);
		} else {
			showTextbox(0, DIALOGUE_SPEAKER);
		}
		longjmp(SCRIPT_JMP_BUF, 2);
	case SCRIPT_OP_SPEAKER:
		pollNextScriptUByte(&pstat);
		DIALOGUE_BOX_MODE = 0;
		/* Reopen the box even if the speaker is the same. */
		if (UI_BOX_DATA[0].state != 1) {
			DIALOGUE_SPEAKER = pstat - 1;
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
	case SCRIPT_OP_ADD_PSTAT:
		skipOnePollTwoScriptBytes(&pstat, &value);
		newValue = readPStat(pstat) + value;
		if (newValue >= 0x100) {
			newValue = 0xff;
		}
		writePStat(pstat, newValue);
		break;
	case SCRIPT_OP_SUB_PSTAT:
		skipOnePollTwoScriptBytes(&pstat, &value);
		newValue = readPStat(pstat) - value;
		if (newValue < 0) {
			newValue = 0;
		}
		writePStat(pstat, newValue);
		break;
	case SCRIPT_OP_GET_MAP:
		pollNextScriptUByte(&pstat);
		writePStat(pstat, CURRENT_MAP_ID);
		break;
	case SCRIPT_OP_GET_PARTNER_TYPE:
		pollNextScriptUByte(&pstat);
		value = PARTNER_ENTITY.digimonEntity.entity.type;
		writePStat(pstat, value);
		break;
	case SCRIPT_OP_SET_INVENTORY_SIZE:
		pollNextScriptUByte(&pstat);
		setInventorySize(pstat);
		break;
	case SCRIPT_OP_RANDOM:
		skipOnePollTwoScriptBytes(&pstat, &value);
		writePStat(pstat, random(value + 1));
		break;
	case SCRIPT_OP_GET_DATE:
		pollNextScriptUByte(&pstat);
		writePStat(pstat, YEAR);
		writePStat((pstat + 1) & 0xff, DAY);
		writePStat((pstat + 2) & 0xff, HOUR);
		writePStat((pstat + 3) & 0xff, MINUTE);
		break;
	case SCRIPT_OP_SIZED_TEXTBOX:
		beginScriptEvent(SPEAKER_NONE);
		DIALOGUE_BOX_MODE = 2;
		scriptSetTextboxSize();
		break;
	case SCRIPT_OP_CLOSE_TEXTBOX:
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

void MAIN_func_801053EC(void)
{
	int32_t state = readPStat(6) & 0xff;

	switch (state) {
	case 0:
		MAIN_D_80134FA0 = (SCRIPT_FROM_TALK != 0) ^ 1;
		break;
	case 1:
		MAIN_D_80134FA0 = 1;
		break;
	case 2:
		MAIN_D_80134FA0 = 0;
		break;
	}

	writePStat(6, 0);
}

void scriptStartWalkAnimation(uint8_t actorId, int32_t animationId)
{
	if (actorId == 0xfd) {
		startAnimationTamer((int16_t)(animationId + 2));
		setTamerState(0xa);
	} else if (actorId == 0xfc) {
		startPartnerAnimation((animationId + 1) << 1);
		setPartnerState(0xc);
	} else {
		startNPCAnimation(actorId, (animationId + 1) << 1);
		setActiveAnim(0xc);
	}
}

void scriptStartAnimation(uint8_t actorId, int32_t animationId)
{
	if (actorId == 0xfd) {
		startAnimationTamer(animationId);
		setTamerState(0xa);
	} else if (actorId == 0xfc) {
		startPartnerAnimation(animationId);
		setPartnerState(0xc);
	} else {
		startNPCAnimation(actorId, animationId);
		setActiveAnim(0xc);
	}
}

/*
 * Script instruction 0x19: a list of two-byte entries and their arguments,
 * ended by another 0x19. Bits 3-5 of an entry pick what it does:
 * 0: test trigger u16 (bit 0 clear: set, set: not set)
 * 1: compare pstat u8 with u8 (bits 0-2: ==, !=, >=, <=, >, <)
 * 2: jump to u16 if the result so far is true
 * 3: jump to u16 if the result so far is false
 * 4: one of the scriptTest*() functions, which read their own arguments
 * Bit 7 ands the test with the result so far, bit 6 ors it. With no jump
 * taken, the script goes on after the block.
 */
void scriptConditionBlock(void)
{
	uint8_t condOp;
	uint32_t one;
	uint8_t pstatValue;
	uint8_t comparand;
	uint16_t shortArg;
	int32_t cond;
	int32_t result;

	result = 0;
	for (;;) {
		pollNextScriptUByte(&condOp);
		SCRIPT_PC++;
		if (condOp == 0x19) {
			longjmp(SCRIPT_JMP_BUF, 1);
		}
		switch (condOp & 0x38) {
		case 0:
			pollNextScriptUShort(&shortArg);
			cond = isTriggerSet(shortArg);
			if ((condOp & 7) == 0) {
				cond ^= (one = 1);
				cond = (cond == 0);
				break;
			} else {
				cond = (cond == 0);
				break;
			}
		case (1 << 3):
			pollNextTwoScriptBytes(&pstatValue, &comparand);
			pstatValue = readPStat(pstatValue);
			cond = scriptCompareValues(condOp, pstatValue,
			                           comparand);
			break;
		case (2 << 3):
			pollNextScriptUShort(&shortArg);
			if (result != 1) {
				continue;
			}
			SCRIPT_PC =
				(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR +
			                    shortArg);
			longjmp(SCRIPT_JMP_BUF, 1);
		case (3 << 3):
			pollNextScriptUShort(&shortArg);
			if (result != 0) {
				continue;
			}

			SCRIPT_PC =
				(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR +
			                    shortArg);
			longjmp(SCRIPT_JMP_BUF, 1);
		case (4 << 3):
			switch (condOp & 7) {
			case 0:
				cond = scriptTestStat();
				break;
			case 1:
				cond = scriptTestCardAmount();
				break;
			case 2:
				cond = scriptTestHasMove();
				break;
			case 3:
				cond = scriptTestPartnerCondition();
				break;
			case 4:
				cond = scriptTestItemCount();
				break;
			case 5:
				cond = scriptTestMoney();
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

int32_t scriptCompareValues(uint8_t op, uint32_t lhs, uint32_t rhs)
{
	switch (op & 7) {
	case 0:
		return lhs == rhs;
	case 1:
		return lhs != rhs;
	case 2:
		return lhs >= rhs;
	case 3:
		return lhs <= rhs;
	case 4:
		return lhs > rhs;
	case 5:
		return lhs < rhs;
	}
}

int32_t scriptCompareSigned(int32_t op, int32_t lhs, int32_t rhs)
{
	switch (op & 7) {
	case 0:
		return lhs == rhs;
	case 1:
		return lhs != rhs;
	case 2:
		return lhs >= rhs;
	case 3:
		return lhs <= rhs;
	case 4:
		return lhs > rhs;
	case 5:
		return lhs < rhs;
	}
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
	case SCRIPT_OP_CHOICE:
		DIALOGUE_BOX_MODE = 1;
		scriptShowSelection();
		longjmp(SCRIPT_JMP_BUF, 2);
		break;
	case SCRIPT_OP_CALL:
		skipOneReadOneUShort(&shortArg);
		entry.scriptPtr = SCRIPT_PC;
		entry.scriptId = ACTIVE_MAP_SCRIPT;
		entry.smth[0] = 1;
		pushScriptStack(&entry);
		SCRIPT_PC = (uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR + shortArg);
		break;
	}
}

void scriptInstruction28to3F(int32_t op)
{
	int16_t *statPtr;
	uint32_t value32;
	int32_t intArg;
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
		if (giveItem(byteArg1, byteArg2) != 0) {
			unsetTrigger(0);
		} else {
			setTrigger(0);
		}
		break;
	case SCRIPT_OP_TAKE_ITEM:
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
	case SCRIPT_OP_SUB_MONEY:
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
	case SCRIPT_OP_NOP_2E:
		pollNextScriptUByte(&byteArg1);
		break;
	case SCRIPT_OP_GIVE_CARD:
		pollNextScriptUByte(&byteArg1);
		byteArg2 = getCardAmount(byteArg1);
		if (byteArg2 < 9) {
			byteArg2 = byteArg2 + 1u;
			setCardAmount(byteArg1, byteArg2);
		}
		break;
	case SCRIPT_OP_TAKE_CARD:
		pollNextScriptUByte(&byteArg1);
		byteArg2 = getCardAmount(byteArg1);
		if (byteArg2 != 0) {
			byteArg2 = byteArg2 - 1u;
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
	case SCRIPT_OP_SUB_MERIT:
		skipOneReadOneUShort(&value);
		MERIT = -value;
		if (MERIT < 0) {
			MERIT = 0;
		}
		break;
	case SCRIPT_OP_SET_STAT:
		pollOneUByteOneUShort(&byteArg1, &value);
		statPtr = getStatsPointer(byteArg1);
		value32 = value;
		*statPtr = enforceStatsLimits(byteArg1, (int16_t)value32);
		scriptUpdateEnergyBoundaries(byteArg1, *statPtr);
		if (byteArg1 == SCRIPT_STAT_TAMER_LEVEL) {
			TAMER_ENTITY.tamerLevel = STAT_TAMER_LEVEL_VALUE;
		}
		if (byteArg1 == SCRIPT_STAT_LIVES) {
			PARTNER_ENTITY.lives = STAT_LIVES_VALUE;
		}
		break;
	case SCRIPT_OP_ADD_STAT:
		pollOneUByteOneUShort(&byteArg1, &value);
		statPtr = getStatsPointer(byteArg1);
		value32 = value;
		*statPtr += value32;
		*statPtr = enforceStatsLimits(byteArg1, *statPtr);
		scriptUpdateEnergyBoundaries(byteArg1, *statPtr);
		if (byteArg1 == SCRIPT_STAT_TAMER_LEVEL) {
			TAMER_ENTITY.tamerLevel = STAT_TAMER_LEVEL_VALUE;
		}
		if (byteArg1 == SCRIPT_STAT_LIVES) {
			PARTNER_ENTITY.lives = STAT_LIVES_VALUE;
		}
		break;
	case SCRIPT_OP_SUB_STAT:
		pollOneUByteOneUShort(&byteArg1, &value);
		statPtr = getStatsPointer(byteArg1);
		intArg = *statPtr - (int16_t)value;
		/* Only happiness goes below 0, down to -100. */
		if (byteArg1 != SCRIPT_STAT_HAPPINESS) {
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
			TAMER_ENTITY.tamerLevel = STAT_TAMER_LEVEL_VALUE;
		}
		if (byteArg1 == SCRIPT_STAT_LIVES) {
			PARTNER_ENTITY.lives = STAT_LIVES_VALUE;
		}
		break;
	case SCRIPT_OP_WAIT_UNTIL_DATE:
		pollNextScriptUByte(&byteArg1);
		byteArg2 = readPStat(byteArg1);
		day = readPStat((byteArg1 + 1) & 0xff);
		hour = readPStat((byteArg1 + 2) & 0xff);
		minute = readPStat((byteArg1 + 3) & 0xff);
		sec = dateToMinutes(byteArg2, day, hour, minute);
		{
			uint32_t cur_sec = dateToMinutes(YEAR, DAY & 0xff,
			                                 HOUR & 0xff, MINUTE & 0xff);
			if (cur_sec < sec) {
				skipHours((sec - cur_sec) / 60);
			}
		}
		YEAR = byteArg2;
		{
			int16_t prev_day = DAY;
			DAY = day;
			HOUR = hour;
			MINUTE = minute;
			CURRENT_FRAME = HOUR * 1200 + MINUTE * 20;
			if (DAY != prev_day) {
				dailyPStatTrigger();
			}
		}
		break;
	case SCRIPT_OP_ADD_TO_DATE:
	case SCRIPT_OP_SUB_FROM_DATE:
		pollNextScriptUByte(&byteArg1);
		pollNextInt(&intArg);
		byteArg2 = readPStat(byteArg1);
		day = readPStat((byteArg1 + 1) & 0xff);
		hour = readPStat((byteArg1 + 2) & 0xff);
		minute = readPStat((byteArg1 + 3) & 0xff);
		sec = dateToMinutes(byteArg2, day, hour, minute);
		if (op == 0x38) {
			intArg = sec + intArg;
		} else {
			intArg = sec - intArg;
			if (intArg < 0) {
				intArg = 0;
			}
		}
		minutesToDate(intArg, &byteArg2, &day, &hour, &minute);
		writePStat(byteArg1, byteArg2);
		writePStat((byteArg1 + 1) & 0xff, day);
		writePStat((byteArg1 + 2) & 0xff, hour);
		writePStat((byteArg1 + 3) & 0xff, minute);
		break;
	case SCRIPT_OP_NOP_3A:
	case SCRIPT_OP_NOP_3B:
	case SCRIPT_OP_NOP_3C:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		break;
	case SCRIPT_OP_NOP_3D:
	case SCRIPT_OP_NOP_3E:
		pollNextScriptUByte(&byteArg1);
		break;
	case SCRIPT_OP_GET_DIGIMON_TYPE:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		byteArg1 = readPStat(byteArg1);
		byteArg1 = DIGIMON_DATA[byteArg1].type;
		writePStat(byteArg2, byteArg1);
		break;
	}

	longjmp(SCRIPT_JMP_BUF, 1);
}

void scriptInstruction46to58(int32_t op)
{
	StackEntry entry;
	int16_t posX;
	int16_t posY;
	uint8_t byteArg1;
	uint8_t byteArg2;
	uint8_t byteArg3;
	uint8_t entityId;
	uint8_t *b;

	switch (op) {
	case SCRIPT_OP_LOAD_MODEL:
		pollNextScriptUByte(&byteArg1);
		scriptLoadModel(byteArg1);
		break;
	case SCRIPT_OP_SPAWN_DIGIMON:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		scriptSetDigimon(byteArg1, byteArg2, byteArg3);
		break;
	case SCRIPT_OP_REMOVE_DIGIMON:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		scriptUnloadEntity(byteArg1);
		break;
	case SCRIPT_OP_DIGIMON_ROUTINE:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		callDigimonRoutine(byteArg1);
		break;
	case SCRIPT_OP_WAIT_MOVEMENT:
		pollNextScriptUByte(&byteArg1);
		ACTIVE_INSTRUCTION = SCRIPT_OP_WAIT_MOVEMENT;
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
		SCRIPT_WAIT_MOVEMENT_SLOT = byteArg1;
		longjmp(SCRIPT_JMP_BUF, 2);
	case SCRIPT_OP_CHANGE_SCREEN:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_D_80134FF8 = byteArg1;
		SELECTION_MENU_STATE = byteArg2;
		MAIN_func_801053EC();
		entry.smth[0] = 4;
		entry.smth[1] = byteArg3;
		pushScriptStack(&entry);
		longjmp(SCRIPT_JMP_BUF, 3);
	case SCRIPT_OP_LOOK_AT:
		beginScriptEvent(SPEAKER_NONE);
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = SCRIPT_MOVEMENTS + entityId * 0xc;
		b[0] = 0;
		b[1] = byteArg1;
		b[2] = byteArg2;
		break;
	case SCRIPT_OP_SET_ROTATION:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextScriptShort(&posX);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = SCRIPT_MOVEMENTS + entityId * 0xc;
		b[0] = 1;
		b[1] = byteArg1;
		*(int16_t *)(b + 4) = posX;
		break;
	case SCRIPT_OP_WALK_TO:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		scriptStartWalkAnimation(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = SCRIPT_MOVEMENTS + entityId * 0xc;
		b[0] = 2;
		b[1] = byteArg1;
		*(int16_t *)(b + 4) = posX;
		*(int16_t *)(b + 6) = posY;
		break;
	case SCRIPT_OP_CAMERA_TO:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		b = (uint8_t *)&SCRIPT_CAMERA_MOVEMENT;
		b[0] = 6;
		*(int16_t *)(b + 4) = posX;
		*(int16_t *)(b + 6) = posY;
		b[3] = byteArg1;
		break;
	case SCRIPT_OP_CAMERA_TO_ENTITY:
		beginScriptEvent(SPEAKER_NONE);
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		b = (uint8_t *)&SCRIPT_CAMERA_MOVEMENT;
		b[0] = 7;
		b[1] = byteArg1;
		b[3] = byteArg2;
		break;
	case SCRIPT_OP_WALK_TO_ENTITY:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		scriptStartWalkAnimation(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = SCRIPT_MOVEMENTS + entityId * 0xc;
		b[0] = 3;
		b[1] = byteArg1;
		b[2] = byteArg3;
		break;
	case SCRIPT_OP_WALK_TO_CAMERA:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		scriptStartWalkAnimation(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = SCRIPT_MOVEMENTS + entityId * 0xc;
		b[0] = 4;
		b[1] = byteArg1;
		*(int16_t *)(b + 4) = posX;
		*(int16_t *)(b + 6) = posY;
		break;
	case SCRIPT_OP_WALK_TO_ENTITY_CAMERA:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		scriptStartWalkAnimation(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = SCRIPT_MOVEMENTS + entityId * 0xc;
		b[0] = 5;
		b[1] = byteArg1;
		b[2] = byteArg3;
		break;
	case SCRIPT_OP_RESET_ORIGIN:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		resetEntityOrigin(byteArg1);
		break;
	case SCRIPT_OP_TEXTBOX_ORIGIN:
		SCRIPT_PC++;
		pollNextTwoScriptShorts(&TEXTBOX_ORIGIN_X, &TEXTBOX_ORIGIN_Y);
		pollNextScriptShort(&TEXTBOX_ORIGIN_Z);
		break;
	case SCRIPT_OP_ANIMATE:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		scriptStartAnimation(byteArg1, byteArg2);
		break;
	case SCRIPT_OP_SET_MAP_OBJECT:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		setMapObjectsFlag(byteArg1, 1, byteArg2);
		break;
	case SCRIPT_OP_CHANGE_SCREEN_PSTAT:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		MAIN_D_80134FF8 = readPStat(byteArg1);
		SELECTION_MENU_STATE = readPStat((byteArg1 + 1) & 0xff);
		MAIN_func_801053EC();
		entry.smth[0] = 4;
		entry.smth[1] = 0xff;
		pushScriptStack(&entry);
		longjmp(SCRIPT_JMP_BUF, 3);
	}

	longjmp(SCRIPT_JMP_BUF, 1);
}

void scriptInstruction5Ato5F(int32_t op)
{
	uint8_t byteArg1;
	uint8_t byteArg2;

	switch (op) {
	case SCRIPT_OP_PLAY_SOUND:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		playSound(byteArg1, byteArg2);
		break;
	case SCRIPT_OP_NOP_5B:
		pollNextScriptUByte(&byteArg1);
		break;
	case SCRIPT_OP_NOP_5C:
		pollNextScriptUByte(&byteArg1);
		break;
	case SCRIPT_OP_PLAY_BGM:
		pollNextScriptUByte(&byteArg1);
		playBGM(byteArg1);
		break;
	case SCRIPT_OP_STOP_BGM:
		pollNextScriptUByte(&byteArg1);
		resetBGM();
		break;
	case SCRIPT_OP_NOP_5F:
		pollNextScriptUByte(&byteArg1);
		break;
	}

	longjmp(SCRIPT_JMP_BUF, 1);
}

void handleMusicOverride(uint8_t *outFont, uint8_t *outVariant)
{
	switch (readPStat(PSTAT_245) & 0xff) {
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

void scriptInstruction64to7E(int32_t op)
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
	uint8_t *b;

	switch (op) {
	case SCRIPT_OP_BUILTIN:
		pollNextScriptUByte(&byteArg1);
		MAIN_D_80134FF8 = byteArg1;
		switch (byteArg1) {
		case SCRIPT_BUILTIN_TOURNAMENT_MEDAL:
			if (checkTournamentMedalConditions() == -1) {
				break;
			}
			/* fall through */
		case SCRIPT_BUILTIN_PICK_ITEM:
		case SCRIPT_BUILTIN_MERIT_SHOP:
		case SCRIPT_BUILTIN_COLLECTOR_TRADE:
		case SCRIPT_BUILTIN_TOURNAMENT:
		case SCRIPT_BUILTIN_SHOP:
		case SCRIPT_BUILTIN_JUKEBOX:
		case SCRIPT_BUILTIN_TRANSPORT:
		case SCRIPT_BUILTIN_ITEM_KEEPER:
		case SCRIPT_BUILTIN_CARD_SHOP:
		case SCRIPT_BUILTIN_TOURNAMENT_SCHEDULE:
		case SCRIPT_BUILTIN_FORGET_MOVES:
			ACTIVE_INSTRUCTION = SCRIPT_OP_BUILTIN;
			SELECTION_MENU_STATE = 0;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case SCRIPT_BUILTIN_TRAINING:
			byteArg1 = readPStat(PSTAT_BUILTIN_ARG);
			if ((CURRENT_MAP_ID == 0x6b) ||
			    (CURRENT_MAP_ID == 0x6c) ||
			    (CURRENT_MAP_ID == 0xa5) ||
			    (CURRENT_MAP_ID == 0x63)) {
				switch (byteArg1) {
				case 0:
					TRN2_setupHpTraining(CURRENT_MAP_ID);
					break;
				case 1:
					TRN2_setupOffenseTraining(CURRENT_MAP_ID);
					break;
				case 2:
					TRN2_setupSpeedTraining(CURRENT_MAP_ID);
					break;
				case 3:
					TRN2_setupDefenseTraining(CURRENT_MAP_ID);
					break;
				case 4:
					TRN2_setupMpTraining(CURRENT_MAP_ID);
					break;
				}
			} else {
				switch (byteArg1) {
				case 0:
					TRN_setupHpTraining(CURRENT_MAP_ID);
					break;
				case 1:
					TRN_setupOffenseTraining(CURRENT_MAP_ID);
					break;
				case 2:
					TRN_setupSpeedTraining(CURRENT_MAP_ID);
					break;
				case 3:
					TRN_setupDefenseTraining(CURRENT_MAP_ID);
					break;
				case 4:
					TRN_setupMpTraining(CURRENT_MAP_ID);
					break;
				case 5:
					TRN_setupBrainsTraining(CURRENT_MAP_ID);
					break;
				}
			}
			ACTIVE_INSTRUCTION = SCRIPT_OP_BUILTIN;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
			break;
		case SCRIPT_BUILTIN_TAKE_ONE_ITEM:
			byteArg1 = readPStat(PSTAT_BUILTIN_ARG);
			if (byteArg1 != 0xff) {
				removeItem(byteArg1, 1);
			}
			break;
		case SCRIPT_BUILTIN_TAKE_ALL_OF_ITEM:
			byteArg1 = readPStat(PSTAT_BUILTIN_ARG);
			if (byteArg1 != 0xff) {
				removeItem(byteArg1, 0x63);
			}
			break;
		case SCRIPT_BUILTIN_OPEN_MONEY_BOX:
			openMoneyBox(1);
			break;
		case SCRIPT_BUILTIN_CLOSE_MONEY_BOX:
			triggerBoxCloseFlag(2);
			longjmp(SCRIPT_JMP_BUF, 2);
		case SCRIPT_BUILTIN_SET_DIRT_CART:
			byteArg1 = readPStat(PSTAT_BUILTIN_ARG);
			setDirtCartModel(byteArg1);
			longjmp(SCRIPT_JMP_BUF, 2);
		case SCRIPT_BUILTIN_SHRINK_DIRT_PILE:
			decreaseDirtPileSize();
			longjmp(SCRIPT_JMP_BUF, 2);
		case SCRIPT_BUILTIN_RESET_MAP_OBJECT_ANIM:
			byteArg1 = readPStat(0xf7);
			byteArg2 = readPStat(0xf8);
			resetMapObjectAnimation(byteArg1, byteArg2);
			longjmp(SCRIPT_JMP_BUF, 2);
		case SCRIPT_BUILTIN_EGG_BOX:
			DOO2_openEggBox();
			/* fall through */
		case SCRIPT_BUILTIN_OPEN_CHEST_TRAY:
		case SCRIPT_BUILTIN_CLOSE_CHEST_TRAY:
		case SCRIPT_BUILTIN_ANGEMON_PEDESTAL:
			ACTIVE_INSTRUCTION = SCRIPT_OP_BUILTIN;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case SCRIPT_BUILTIN_SET_TIME_SPEED:
			SCRIPT_SAVED_TIME_SPEED = readPStat(PSTAT_BUILTIN_ARG);
			break;
		case 0x1a:
			loadDirtCartModel();
			break;
		case 0x1b:
			loadDirtPileModel();
			break;
		case SCRIPT_BUILTIN_MOOD_BUBBLE:
			createMonochromonMoodBubble();
			break;
		case SCRIPT_BUILTIN_CLEAR_PRICE:
			SCRIPT_PRICE = 0;
			break;
		case SCRIPT_BUILTIN_PRICE_FROM_TEXT_ARGS: {
			int32_t hi = readPStat(PSTAT_TEXT_ARG_1) << 8;
			int32_t lo = readPStat(PSTAT_TEXT_ARG_2);
			posX = lo + hi;
			SCRIPT_PRICE += posX;
		} break;
		case SCRIPT_BUILTIN_PRICE_TO_TEXT_ARGS:
			writePStat(PSTAT_TEXT_ARG_1, (SCRIPT_PRICE / 256) & 0xff);
			writePStat(PSTAT_TEXT_ARG_2, SCRIPT_PRICE & 0xff);
			break;
		case SCRIPT_BUILTIN_NAMING:
			initializeNamingBuffer(readPStat(PSTAT_BUILTIN_ARG));
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
		case SCRIPT_BUILTIN_LOAD_SHOP:
			loadShopLibrary();
			break;
		case 0x24:
			readMapTFS(CURRENT_MAP_ID);
			break;
		case 0x27:
			MAIN_func_800D9360(CURRENT_MAP_ID);
			break;
		case SCRIPT_BUILTIN_SAVE_MACHINE:
			openSaveMachine();
			/* fall through */
		case SCRIPT_BUILTIN_REMOVE_MIST:
			ACTIVE_INSTRUCTION = SCRIPT_OP_BUILTIN;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case SCRIPT_BUILTIN_GAME_CLEAR_SAVE:
			gameClearSave();
			ACTIVE_INSTRUCTION = SCRIPT_OP_BUILTIN;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x26:
			createNinjamonEffect();
			break;
		case 0x33:
			addFileReadRequestPath(PATH_ETCHI_BOSS_EFE_TMD,
			                       BOSS_EFE_TMD_BUFFER, 0, 0,
			                       0);
			loadDynamicLibrary(EAB_REL, 0, 0, 0, 0);
			readVBALLSection(5, 0x73);
			loadMapSounds2(0x15);
			break;
		case 0x34:
			loadTextureFile(PATH_ETCHI_OP_TIM, 0, 0);
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
			setTamerState(0x10);
			SOME_SCRIPT_SYNC_BIT = 0;
			ACTIVE_INSTRUCTION = SCRIPT_OP_BUILTIN;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
			break;
		case 0x37:
			setLoopCountToOne(readPStat(PSTAT_BUILTIN_ARG));
			longjmp(SCRIPT_JMP_BUF, 2);
			break;
		case 0x29:
			spawnGearbox();
			break;
		case 0x2a: {
			int8_t pstat = readPStat(PSTAT_BUILTIN_ARG);
			somethingToyTown(pstat);
		} break;
		case 0x2b:
			spawnToyTownBoxes();
			break;
		case 0x2c: {
			int8_t value = readPStat(PSTAT_BUILTIN_ARG);
			openToyTownBox(value);
		} break;
		case 0x2d: {
			int16_t value = readPStat(PSTAT_BUILTIN_ARG);
			fadeToWhite(value);
		} break;
		case 0x2e: {
			int16_t value = readPStat(PSTAT_BUILTIN_ARG);
			fadeFromWhite(value);
		} break;
		case 0x31:
			spawnAngemonPedestal();
			break;
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x11:
			goto script_end;
		}
		break;
	case SCRIPT_OP_CURE_CONDITION:
		pollNextScriptUByte(&byteArg1);
		PARTNER_PARA.condition &= ~byteArg1;
		break;
	case SCRIPT_OP_BATTLE:
		pollNextScriptUByte(&byteArg1);
		if (BATTLES_FOUGHT < 0x270f) {
			BATTLES_FOUGHT++;
		}
		byteArg1 = readPStat(PSTAT_BATTLE_SET_ENEMIES);
		if (byteArg1 != 0) {
			for (byteArg1 = PSTAT_BATTLE_ENEMY_1; byteArg1 < PSTAT_BATTLE_ENEMY_3 + 1; byteArg1++) {
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

			outcome = startBattle(SCRIPT_TALKED_ENTITY);
			writePStat(PSTAT_RESULT, outcome);
			if (outcome == -1) {
				handleItemLoss();
				PARTNER_ENTITY.lives -= 1;
				if (PARTNER_ENTITY.lives == 0) {
					PARTNER_PARA.remainingLifetime = 0;
				}
				CURRENT_SCRIPT_PTR = getScript(0);
				SCRIPT_PC = getScriptSection((uint8_t *)(int32_t)CURRENT_SCRIPT_PTR, 0x4de);
				break;
			}
			if (outcome == 0) {
				if (BATTLES_WON < 0x270f) {
					BATTLES_WON++;
				}
				MAIN_D_80134FA0 = 0;
				MAIN_D_80134FF8 = PREVIOUS_SCREEN;
				SELECTION_MENU_STATE = PREVIOUS_EXIT;
				PREVIOUS_EXIT = CURRENT_EXIT;
				entry.smth[0] = 4;
				entry.smth[1] = 0xff;
				pushScriptStack(&entry);
				longjmp(SCRIPT_JMP_BUF, 3);
			}
		}
		if (isTriggerSet(1) != 0) {
			setMovementEnabled(-1, 1);
			resetTextboxes();
			break;
		}

		b = (uint8_t *)&SCRIPT_CAMERA_MOVEMENT;
		b[0] = 7;
		b[1] = 0xfd;
		b[3] = 0xa;

		ACTIVE_INSTRUCTION = SCRIPT_OP_WAIT_MOVEMENT;
		SCRIPT_WAIT_MOVEMENT_SLOT = 0xa;
		resetTextboxes();
		longjmp(SCRIPT_JMP_BUF, 2);
	case SCRIPT_OP_WAIT:
		skipOneReadOneUShort(&SCRIPT_WAIT_TIMER);
		ACTIVE_INSTRUCTION = SCRIPT_OP_WAIT;
		longjmp(SCRIPT_JMP_BUF, 2);
	case SCRIPT_OP_TEXT_ADVANCE:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		TEXT_ADVANCE_MODE = byteArg1;
		if (byteArg1 != 2) {
			break;
		}
		TEXT_AUTO_ADVANCE_TIMER = byteArg2;
		break;
	case SCRIPT_OP_HURT_PARTNER:
		beginScriptEvent(SPEAKER_NONE);
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
	case SCRIPT_OP_NOP_6B:
		break;
	case SCRIPT_OP_MOVE_TO:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = SCRIPT_MOVEMENTS + entityId * 0xc;
			b[0] = 8;
			b[1] = byteArg1;
			b[3] = byteArg2;
			*(int16_t *)(b + 4) = posX;
			*(int16_t *)(b + 6) = posY;
		}
		break;
	case SCRIPT_OP_MOVE_TO_ENTITY:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = SCRIPT_MOVEMENTS + entityId * 0xc;
			b[0] = 9;
			b[1] = byteArg1;
			b[2] = byteArg2;
			b[3] = byteArg3;
		}
		break;
	case SCRIPT_OP_MOVE_TO_CAMERA:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = SCRIPT_MOVEMENTS + entityId * 0xc;
			b[0] = 0xb;
			b[1] = byteArg1;
			b[3] = byteArg2;
			*(int16_t *)(b + 4) = posX;
			*(int16_t *)(b + 6) = posY;
		}
		break;
	case SCRIPT_OP_MOVE_TO_ENTITY_CAMERA:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = SCRIPT_MOVEMENTS + entityId * 0xc;
			b[0] = 0xb;
			b[1] = byteArg1;
			b[2] = byteArg2;
			b[3] = byteArg3;
		}
		break;
	case SCRIPT_OP_ROTATE_DOOR:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		SCRIPT_DOOR_MOVEMENT[0] = 0xc;
		SCRIPT_DOOR_MOVEMENT[1] = byteArg1;
		SCRIPT_DOOR_MOVEMENT[2] = byteArg3;
		break;
	case SCRIPT_OP_MOVE_OBJECT:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		pollNextTwoScriptBytes(&entityId, &padByte);
		pollNextTwoScriptShorts(&posX, &posY);
		byteArg1 += 0xc;
		b = SCRIPT_MOVEMENTS + byteArg1 * 0xc;
		b[0] = 0xd;
		b[1] = byteArg2;
		b[3] = byteArg3;
		b[2] = entityId;
		*(int16_t *)(b + 8) = posX;
		*(int16_t *)(b + 0xa) = posY;
		break;
	case SCRIPT_OP_MOVE_ALONG_AXIS:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextScriptShort(&posX);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = SCRIPT_MOVEMENTS + entityId * 0xc;
			b[0] = 0xe;
			b[1] = byteArg1;
			b[2] = byteArg2;
			*(int16_t *)(b + 4) = posX;
			b[3] = byteArg3;
		}
		break;
	case SCRIPT_OP_MOVE_ALONG_AXIS_CAMERA:
		beginScriptEvent(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextScriptShort(&posX);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = SCRIPT_MOVEMENTS + entityId * 0xc;
			b[0] = 0xf;
			b[1] = byteArg1;
			b[2] = byteArg2;
			*(int16_t *)(b + 4) = posX;
			b[3] = byteArg3;
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
	case SCRIPT_OP_REMOVE_BOULDER:
		pollNextScriptUByte(&byteArg1);
		removeObject(0xfb6, 0);
		break;
	case SCRIPT_OP_UNLOAD_MODEL:
		pollNextScriptUByte(&byteArg1);
		scriptUnloadModel(byteArg1);
		break;
	case SCRIPT_OP_COPY_PSTAT:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		byteArg1 = readPStat(byteArg1);
		writePStat(byteArg2, byteArg1);
		break;
	case SCRIPT_OP_RETURN_SECTION:
		pollNextScriptUByte(&byteArg1);
		entry.smth[0] = 4;
		entry.smth[1] = byteArg1;
		pushScriptStack(&entry);
		writePStat(0, SCRIPT_SAVED_TIME_SPEED);
		break;
	case SCRIPT_OP_SET_IMPASSABLE:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		setRectImpassible(posX, posY, byteArg2,
		                  byteArg3);
		break;
	case SCRIPT_OP_SPAWN_SPRITE:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptShorts(&posZ, &posW);
		spawnSpriteAtLocation(posX, posY, posZ, posW, byteArg1);
		break;
	case SCRIPT_OP_SPAWN_SPRITE_AT:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		spawnSpriteAtEntity(byteArg1, byteArg2, byteArg3);
		break;
	}
script_end:
	longjmp(SCRIPT_JMP_BUF, 1);
}

void setMapHeadActive(void)
{
	uint8_t *scriptPtr;
	int32_t off;

	scriptPtr = MAPHEAD_DATA_PTR;
	off = *(int16_t *)scriptPtr;
	CURRENT_SCRIPT_PTR = scriptPtr;
	SCRIPT_PC = scriptPtr + (off & 0xffff) - 2;
}

void initializeScripts(void)
{
	MAPHEAD_DATA_PTR = MAPHEAD_DATA;
	SCRIPT_HEADER_PTR = SCRIPT_HEADER;
	SCRIPT_DATA_PTR = SCRIPT_DATA;
	SCRIPT_STATE_PTR = &SCRIPT_STATE;
	TEXT_BUFFERS_PTR = TEXT_BUFFERS;

	readFile(MAPHEAD_FILE_PATH, MAPHEAD_DATA_PTR);
	readFileSection(SCRIPT_FILE_PATH, SCRIPT_HEADER_PTR, 0, 0x2000);
	memset((void *)SCRIPT_STATE_PTR, 0, sizeof(*SCRIPT_STATE_PTR));

	CURRENT_SCRIPT_ID = 0xffff;
	ACTIVE_MAP_SCRIPT = 0xffff;
	MERIT = 0;
	SCRIPT_STACK_DEPTH = 0;
	BATTLES_FOUGHT = 0;
	BATTLES_WON = 0;
	TOURNAMENT_TITLES = 0;
	TOURNAMENT_WINS = 0;
	TOURNAMENT_LOSSES = 0;
	TEXTBOX_ORIGIN_X = -0x270f;
	TEXTBOX_ORIGIN_Y = -0x270f;
	TEXTBOX_ORIGIN_Z = -0x270f;

	initialKeyInputs();
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

void runMapHeadScript(int32_t section)
{
	callScriptSection(0, section, 1);
	tickScript();
}

/*
 * Starts section of script scriptId: the section of an NPC's script id when
 * the player talks to it (param 1), or of a trigger tile or event (param 0).
 */
void callScriptSection(int32_t scriptId, int32_t section, int32_t param)
{
	int32_t i;

	CURRENT_SCRIPT_PTR = getScript(scriptId);
	SCRIPT_PC =
		getScriptSection((uint8_t *)(int32_t)CURRENT_SCRIPT_PTR,
	                         section);
	SCRIPT_FROM_TALK = param;
	SCRIPT_SECTION = section;
	TEXT_ADVANCE_MODE = 0;
	DIALOGUE_SPEAKER = SPEAKER_PLAYER;
	SCRIPT_SAVED_TIME_SPEED = readPStat(0);
	SOME_SCRIPT_SYNC_BIT = 1;
	ACTIVE_INSTRUCTION = 0;
	SCRIPT_MAP_CHANGE = 0;
	SCRIPT_WARPING = 0;
	SCRIPT_HAS_CONTROL = 0;
	IS_SCRIPT_PAUSED = 0;
	SCRIPT_TALKED_ENTITY = TALKED_TO_ENTITY;
	for (i = 0; i < 0x16; i++) {
		((ScriptCameraMovement *)SCRIPT_MOVEMENTS)[i].type = 0xff;
	}

	resetTextboxes();
}

void tickScriptedMovement(int32_t slot)
{
	ScriptCameraMovement *movement = &((ScriptCameraMovement *)SCRIPT_MOVEMENTS)[slot];
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
		done = tickMoveCameraTo(movement->posX,
		                        movement->posY,
		                        movement->speed);
		break;
	case 7:
		done = tickMoveCameraToEntity(movement->entityId,
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
	case 0xc: {
		int32_t instanceId;
		int32_t target;

		instanceId = movement->entityId;
		target = movement->target;

		done = tickRotateDoor(instanceId, target);
	} break;
	case 0xd:
		done = tickMoveObjectTo(movement->entityId,
		                        ((slot & 0xff) - 0xc) & 0xff,
		                        (int8_t)movement->target,
		                        movement->targetX, movement->targetY);
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
				startPartnerAnimation(0);
			} else if (slot < 10) {
				startNPCAnimation(movement->entityId, 0);
			}
		}

		movement->type = 0xff;
	}
}

void pushScriptStack(StackEntry *entry)
{
	StackEntry *stackTop;

	stackTop = &SCRIPT_STATE_PTR->stack[SCRIPT_STACK_DEPTH];
	*stackTop = *entry;
	++SCRIPT_STACK_DEPTH;
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

	if (SCRIPT_STACK_DEPTH != 0) {
		--SCRIPT_STACK_DEPTH;
		stackTop = &SCRIPT_STATE_PTR->stack[SCRIPT_STACK_DEPTH];
		entry = *stackTop;
	} else {
		entry.smth[0] = 0;
	}

	*out = entry;
}

void skipOneReadOneUShort(uint16_t *out)
{
	SCRIPT_PC++;
	pollNextScriptUShort(out);
}

void pollNextScriptUByte(uint8_t *out)
{
	*out = *SCRIPT_PC;
	SCRIPT_PC++;
}

void pollNextScriptUShort(uint16_t *out)
{
	*out = *(uint16_t *)SCRIPT_PC;
	SCRIPT_PC += 2;
}

void pollOneUByteOneUShort(uint8_t *outByte, uint16_t *outShort)
{
	pollNextScriptUByte(outByte);
	pollNextScriptUShort(outShort);
}

void skipOneReadTwoShort(uint16_t *out1, uint16_t *out2)
{
	SCRIPT_PC++;
	pollNextScriptUShort(out1);
	pollNextScriptUShort(out2);
}

void setTrigger(uint16_t trigger)
{
	uint8_t *ptr;
	uint8_t mask;

	getTriggerOffset(trigger, &ptr, &mask);
	*ptr |= mask;
}

void unsetTrigger(uint16_t trigger)
{
	uint8_t *ptr;
	uint8_t mask;

	getTriggerOffset(trigger, &ptr, &mask);
	*ptr = *ptr & ~mask;
}

void skipOnePollTwoScriptBytes(uint8_t *out1, uint8_t *out2)
{
	SCRIPT_PC++;
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

void scriptLearnMove(int32_t moveId)
{
	learnMove((int16_t)moveId);
}

/* Lists the six cards on sale today, see dailyPStatTrigger(). */
int32_t fillCardShopList(void)
{
	uint8_t *buf;
	uint8_t any;
	uint8_t i;
	int32_t id;

	buf = MAIN_D_80134F68->buf;
	any = 0;
	MAIN_D_80134F68->itemCount = 0;
	for (i = 0; i < 6; i++) {
		MAIN_D_80134F68->itemCount++;
		id = SCRIPT_STATE_PTR->smth[i];
		*buf++ = id;
		if (id != 0xff) {
			if ((CARD_PRICES[CARD_DATA[id].spriteId] <= MONEY) &&
			    ((uint32_t)getCardAmount(id) < 9)) {
				*buf++ = 1;
			} else {
				*buf++ = 0;
			}
			any = 1;
		} else {
			*buf++ = 0;
		}
	}

	return any;
}

void showCardTextbox(void)
{
	int32_t line;
	int32_t cardId;
	uint8_t amount;

	cardId = readPStat(PSTAT_SELECTED) & 0xff;
	amount = getCardAmount(cardId);
	if (amount == 0) {
		if (CARD_DATA[cardId].spriteId == 0) {
			line = 2;
		} else if (CARD_DATA[cardId].spriteId == 1) {
			line = 3;
		} else {
			line = 4;
		}
	} else if (amount < 6) {
		line = 5;
	} else {
		line = 6;
	}

	if (amount < 9) {
		amount = amount + 1u;
		setCardAmount(cardId, amount);
	}

	writePStat(PSTAT_SELECTED, CARD_DATA[cardId].digimonId);
	showMapHeadTextbox(line, SPEAKER_PLAYER, 0, MAPHEAD_TEXT_CARD_SHOP);
}

int32_t fillOwnedCardList(void)
{
	uint8_t *buf;
	uint8_t any;
	uint8_t id;
	uint8_t amount;

	buf = MAIN_D_80134F68->buf;
	any = 0;
	MAIN_D_80134F68->itemCount = 0;
	for (id = 1; id < 0x41; id++) {
		MAIN_D_80134F68->itemCount++;
		amount = getCardAmount(id);
		if (amount != 0) {
			*buf++ = id;
			*buf++ = amount;
			any = 1;
		} else {
			*buf++ = 0xff;
			*buf++ = 0;
		}
	}

	return any;
}

int32_t getCardAmount(int32_t cardId)
{
	uint8_t *cardPtr;
	int32_t origId;
	uint8_t amount;

	origId = cardId;
	cardPtr = &SCRIPT_STATE_PTR->cards[cardId /= 2];
	if ((origId & 1) == 0) {
		amount = *cardPtr & 0xf;
	} else {
		amount = *cardPtr >> 4;
	}

	return amount;
}

int32_t setCardAmount(int32_t cardId, int32_t value)
{
	uint8_t *cardPtr;
	int32_t origId;

	origId = cardId;
	cardPtr = &SCRIPT_STATE_PTR->cards[cardId /= 2];
	if ((origId & 1) == 0) {
		*cardPtr = (*cardPtr & 0xf0) | value;
	} else {
		*cardPtr = (*cardPtr & 0xf) | (value << 4);
	}
}

uint32_t dateToMinutes(uint32_t years, uint32_t days, uint32_t hours,
                       uint32_t minutes)
{
	minutes += hours * 60;
	minutes += days * 1440;
	minutes += years * 43200;

	return minutes;
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

void minutesToDate(uint32_t totalMinutes, uint8_t *outYear,
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
	*out = *(int16_t *)SCRIPT_PC;
	SCRIPT_PC += 2;
}

void pollNextTwoScriptShorts(int16_t *out1, int16_t *out2)
{
	pollNextScriptShort(out1);
	pollNextScriptShort(out2);
}

void playBGM(int16_t bgmId)
{
	uint8_t font;
	uint8_t variant;

	font = bgmId;
	if (bgmId == 0xff) {
		resetBGM();
		return;
	}

	handleMusicOverride(&font, &variant);
	ACTIVE_BGM_FONT = font;
	ACTIVE_BGM_VARIANT = variant;
	stopBGM();
	playMusic(font, variant);
}

void updateBGM(void)
{
	uint8_t font;
	uint8_t variant;

	font = ACTIVE_BGM_FONT;
	if (ACTIVE_BGM_FONT == 0xff) {
		resetBGM();
		return;
	}

	handleMusicOverride(&font, &variant);
	if (ACTIVE_BGM_FONT != font || ACTIVE_BGM_VARIANT != variant) {
		ACTIVE_BGM_FONT = font;
		ACTIVE_BGM_VARIANT = variant;
		stopBGM();
		playMusic(font, variant);
	}
}

void forceUpdateBGM(void)
{
	uint8_t font;
	uint8_t variant;

	font = ACTIVE_BGM_FONT;
	if (ACTIVE_BGM_FONT == 0xff) {
		resetBGM();
		return;
	}

	handleMusicOverride(&font, &variant);
	ACTIVE_BGM_FONT = font;
	ACTIVE_BGM_VARIANT = variant;
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

void getTriggerOffset(int32_t trigger, uint8_t **outPtr, uint8_t *outMask)
{
	uint8_t bit;
	int32_t origTrigger;

	origTrigger = trigger;
	bit = (trigger %= 8);
	*outPtr = &SCRIPT_STATE_PTR->triggers[origTrigger / 8];
	*outMask = 1;
	while (bit != 0) {
		*outMask <<= 1;
		bit--;
	}
}

int32_t scriptHasMove(int32_t moveId)
{
	return hasMove((int16_t)moveId);
}

int32_t fillLostItemList(void)
{
	uint8_t *buf;
	uint32_t i;
	int32_t idx;
	int32_t itemId;
	int32_t count;

	buf = MAIN_D_80134F68->buf;
	MAIN_D_80134F68->itemCount = 0;
	idx = 6;
	for (i = 0; i < 0x4e; i = (i + 1) & 0xff, idx++) {
		count = SCRIPT_STATE_PTR->smth[idx];
		if (count != 0) {
			MAIN_D_80134F68->itemCount++;
			itemId = LOST_ITEM_IDS[i];
			*buf++ = itemId;
			if (ITEM_PARA[itemId].value <= MONEY) {
				count = (count | 0x80) & 0xff;
			}
			*buf++ = count;
		}
	}

	if (MAIN_D_80134F68->itemCount != 0) {
		return 1;
	}

	return 0;
}

uint8_t rollCard(void)
{
	uint8_t *cards;
	uint8_t *p;
	uint8_t count;
	uint8_t i;
	uint32_t rarity;

	rarity = (uint8_t)random(100);
	if (rarity == 0) {
		rarity = 0;
	} else if (rarity < 5) {
		rarity = 1;
	} else if (rarity < 0x14) {
		rarity = 2;
	} else if (rarity < 0x32) {
		rarity = 3;
	} else {
		rarity = 4;
	}

	cards = (uint8_t *)allocateArray(0x42);
	p = cards;
	count = 0;
	for (i = 0; i < 0x42; i++) {
		if (rarity == CARD_DATA[i].spriteId) {
			*p++ = i;
			count++;
		}
	}

	rarity = (uint8_t)random(count);
	i = cards[rarity];
	freeArray((uint32_t *)cards);

	return i;
}

void openCardMenuBox(void)
{
	RECT rect;
	RECT origin;
	ItemMenuBox *result;
	int32_t boxId;
	RECT *src;

	if (ITEM_MENU_MODE == ITEM_MENU_SELL_CARD) {
		boxId = 0xfd;
	} else {
		boxId = readPStat(PSTAT_BUILTIN_ARG) & 0xff;
	}

	setupBoxOrigin(boxId, &origin);
	result = getActiveItemMenu();
	src = &((RECT *)ITEM_MENU_RECTS)[ITEM_MENU_MODE];
	setRECT(&rect, src->x, src->y, src->w, src->h);
	createTextbox(1, 0xf1, &rect, &origin, tickCardMenu,
	              renderCardMenuBox);
	registerTextbox(1, 9, 6, 1, 0);
	openItemMenu(result, 1, 9);
	layoutItemMenu(result, 9, 1);
	SCRIPT_PRICE = 0;
}

int32_t fillMeritItemList(void)
{
	uint8_t hasFreeSlot;
	uint8_t *out;
	uint8_t i;
	uint8_t item;
	uint8_t size;
	int32_t any;

	size = INVENTORY.size;
	out = MAIN_D_80134F6C->buf;
	any = 0;
	hasFreeSlot = 0;
	MAIN_D_80134F6C->itemCount = 0;

	for (i = 0; i < size; i++) {
		if (INVENTORY.types.array[i] == 0xff) {
			hasFreeSlot = 1;
			break;
		}
	}

	for (item = 0; item < 0x80; item++) {
		if (ITEM_PARA[item].meritValue == 0) {
			continue;
		}

		MAIN_D_80134F6C->itemCount++;
		*out++ = item;

		if (ITEM_PARA[item].meritValue > MERIT) {
			*out++ = 0;
			continue;
		}

		if (hasFreeSlot != 0) {
			*out++ = 1;
			any = 1;
			continue;
		}

		for (i = 0; i < size; i++) {
			if ((INVENTORY.types.array[i] == item) &&
			    (INVENTORY.amounts.array[i] != 0x63)) {
				*out++ = 1;
				any = 1;
				goto next;
			}
		}

		*out++ = 0;
next:;
	}

	return any;
}

void fillItemKeeperLists(void)
{
	uint8_t hasFreeSlot;
	uint8_t *counts;
	uint8_t *out;
	uint8_t item;
	uint8_t c;
	uint8_t type;
	uint8_t i;
	uint8_t size;
	uint8_t *p;

	counts = &SCRIPT_STATE_PTR->smth[0x54];
	out = MAIN_D_80134F68->buf;
	hasFreeSlot = 0;
	MAIN_D_80134F68->itemCount = 0;
	size = INVENTORY.size;

	for (i = 0; i < size; i++) {
		if (INVENTORY.types.array[i] == 0xff) {
			hasFreeSlot = 1;
			break;
		}
	}

	for (item = 0; item < 0x80; item++) {
		c = *counts++;
		if (c == 0) {
			continue;
		}

		MAIN_D_80134F68->itemCount++;
		*out++ = item;

		if (hasFreeSlot != 0) {
			*out++ = c | 0x80;
			continue;
		}

		for (i = 0; i < size; i++) {
			if ((INVENTORY.types.array[i] == item) &&
			    (INVENTORY.amounts.array[i] != 0x63)) {
				*out++ = c | 0x80;
				goto next;
			}
		}

		*out++ = c;
next:;
	}

	out = MAIN_D_80134F6C->buf;
	MAIN_D_80134F6C->itemCount = 0;

	for (item = 0; item < size; item++) {
		MAIN_D_80134F6C->itemCount++;
		if ((type = INVENTORY.types.array[item]) != 0xff) {
			c = INVENTORY.amounts.array[item];
			*out++ = type;
			p = (uint8_t *)((uint32_t)type + (uint32_t)SCRIPT_STATE_PTR);
			if (p[0x54] != 0x63) {
				*out++ = c | 0x80;
			} else {
				*out++ = c;
			}
		} else {
			*out++ = 0xff;
			*out++ = 0;
		}
	}
}

void openItemKeeperBoxes(void)
{
	RECT rect;
	RECT origin;
	int32_t boxId;

	boxId = readPStat(PSTAT_BUILTIN_ARG);
	setupBoxOrigin(boxId, &origin);
	setRECT(&rect, -152, -98, 148, 127);
	createTextbox(1, 0xe1, &rect, &origin, tickItemKeeperMenu,
	              renderItemKeeperStoredBox);
	registerTextbox(1, 9, 6, 1, 1);
	openItemMenu(MAIN_D_80134F68, 1, 0xa);
	layoutItemKeeperList(0);
	setupBoxOrigin(SPEAKER_PLAYER, &origin);
	setRECT(&rect, 0, -98, 148, 127);
	createTextbox(2, 0xe1, &rect, &origin, 0, renderItemKeeperHandBox);
	registerTextbox(2, 9, 6, 1, 2);
	openItemMenu(MAIN_D_80134F6C, 2, 0xa);
	layoutItemKeeperList(1);
}

/* Lists the tracks, the last one only after trigger 50, and finds the one playing. */
void fillJukeboxList(void)
{
	uint8_t *buf;
	int32_t unlocked;
	uint8_t i;
	int32_t offset;
	uint8_t track;
	uint8_t b;

	unlocked = isTriggerSet(TRIGGER_50);
	buf = MAIN_D_80134F68->buf;
	MAIN_D_80134F68->itemCount = 0;
	for (i = 0; i < 0x3f; i++) {
		if (unlocked != 0) {
			MAIN_D_80134F68->itemCount++;
			*buf++ = i;
			buf++;
		} else if (i != 0x3e && i != 0x3e) {
			MAIN_D_80134F68->itemCount++;
			*buf++ = i;
			buf++;
		}
	}

	for (i = 0, offset = 0;
	     i < MAIN_D_80134F68->itemCount;
	     i++, offset += 2) {
		track = MAIN_D_80134F68->buf[offset];
		b = JUKEBOX_TRACKS[track * 2];
		if (b == ACTIVE_BGM_FONT) {
			b = JUKEBOX_TRACKS[track * 2 + 1];
			if (b == ACTIVE_BGM_VARIANT) {
				writePStat(PSTAT_SELECTED, i);
				JUKEBOX_PLAYING = i;
				return;
			}
		}
	}

	writePStat(PSTAT_SELECTED, 0xff);
	JUKEBOX_PLAYING = 0xff;
	stopBGM();
}

void openJukeboxMenuBox(void)
{
	RECT rect;
	RECT origin;
	int32_t i;
	uint8_t item;
	ItemMenuBox *menu;
	int32_t off;
	int32_t scroll;
	ItemMenuBox *box;
	int32_t count;
	int32_t row;

	setupBoxOrigin(readPStat(PSTAT_BUILTIN_ARG), &origin);
	setRECT(&rect, -0x47, -0x62, 0xde, 0x81);
	createTextbox(1, 0xf1, &rect, &origin, tickJukeboxMenu, renderJukeboxMenuBox);
	registerTextbox(1, 9, 6, 1, 0);

	if (MAIN_D_80134F68->isOpen == 0) {
		MAIN_D_80134F68->isOpen = 1;
		MAIN_D_80134F68->boxId = 1;

		item = readPStat(PSTAT_SELECTED);
		box = MAIN_D_80134F68;
		menu = box;
		count = menu->itemCount;

		for (i = 0, off = 0; i < count; i++, off += 2) {
			if (item == box->buf[off]) {
				goto found;
			}
		}

		i = 0;

found:
		scroll = i - menu->visibleRows;

		if (scroll < 0) {
			menu->topRow = 0;
			MAIN_D_80134F68->cursor = i;
		} else {
			menu->topRow = scroll + 1;
			MAIN_D_80134F68->cursor = MAIN_D_80134F68->visibleRows - 1;
		}

		MAIN_D_80134F68->prevTopRow = MAIN_D_80134F68->topRow;
		MAIN_D_80134F68->prevCursor = MAIN_D_80134F68->cursor;

		for (i = 0, row = 9; i < MAIN_D_80134F68->visibleRows; i++, row++) {
			MAIN_D_80134F68->itemRow[i] = row;
		}
	}

	layoutItemMenu(MAIN_D_80134F68, 9, 2);
}

void fillTransportList(void)
{
	uint8_t *buf;
	uint8_t i;

	buf = MAIN_D_80134F68->buf;
	MAIN_D_80134F68->itemCount = 0;
	for (i = 0; i < 6; i++) {
		if (isTriggerSet(TRANSPORT_DESTINATIONS[i].trigger) != 0) {
			MAIN_D_80134F68->itemCount++;
			if (TRANSPORT_DESTINATIONS[i].cost <= MONEY) {
				*buf++ = i | 0x80;
				buf++;
			} else {
				*buf++ = i;
				buf++;
			}
		}
	}
}

void openTransportMenuBox(void)
{
	RECT rect;
	RECT origin;
	int32_t boxId;

	boxId = readPStat(PSTAT_BUILTIN_ARG);
	setupBoxOrigin(boxId, &origin);
	setRECT(&rect, -71, -100, 222, 129);
	createTextbox(1, 0xf1, &rect, &origin, tickTransportMenu,
	              renderTransportMenuBox);
	registerTextbox(1, 9, 6, 1, 0);
	openItemMenu(MAIN_D_80134F68, 1, 9);
	layoutItemMenu(MAIN_D_80134F68, 9, 3);
	SCRIPT_PRICE = 0;
}

/*
 * Lists the three trades of set pstat 249 of TRADE_WANTED_ITEMS. Here
 * ITEM_MENU_MODE holds the index of the first trade, not a mode.
 */
void fillTradeLists(void)
{
	uint8_t *buf;
	uint8_t *buf2;
	int32_t save;
	uint8_t i;
	uint8_t idx;
	int32_t item;

	ITEM_MENU_MODE = readPStat(PSTAT_SELECTED) * 3;
	idx = ITEM_MENU_MODE;
	buf = MAIN_D_80134F68->buf;
	MAIN_D_80134F68->itemCount = 3;
	buf2 = MAIN_D_80134F6C->buf;
	MAIN_D_80134F6C->itemCount = 3;
	i = 0;
	while (i < 3) {
		item = TRADE_WANTED_ITEMS[idx];
		save = idx;
		if (getItemCount(item) != 0) {
			*buf++ = item | 0x80;
			buf++;
		} else {
			*buf++ = item;
			buf++;
		}
		*buf2++ = TRADE_GIVEN_ITEMS[save];
		buf2++;
		i++;
		idx++;
	}
}

void openTradeMenuBox(void)
{
	RECT rect;
	RECT origin;
	int32_t boxId;

	boxId = readPStat(PSTAT_BUILTIN_ARG);
	setupBoxOrigin(boxId, &origin);
	setRECT(&rect, -88, -80, 223, 83);
	createTextbox(1, 0xe1, &rect, &origin, tickTradeMenu,
	              renderTradeMenuBox);
	registerTextbox(1, 9, 4, 1, 0);
	openItemMenu(MAIN_D_80134F68, 1, 0xa);
	layoutTradeList();
}

/* Sets the trigger of the selected trade, and trigger 14 once all are done. */
void markTradeDone(void)
{
	ItemMenuBox *box = MAIN_D_80134F68;
	int32_t trigger;

	trigger = box->topRow + box->cursor + ITEM_MENU_MODE + TRIGGER_TRADE_DONE;
	setTrigger(trigger);
	for (trigger = TRIGGER_TRADE_DONE; trigger < TRIGGER_ALL_TRADES_DONE; trigger++) {
		if (isTriggerSet(trigger) == 0) {
			return;
		}
	}

	setTrigger(TRIGGER_ALL_TRADES_DONE);
}

void tickCardMenu(void)
{
	ItemMenuBox *box;
	RECT rect;
	RECT *src;

	box = getActiveItemMenu();
	if (tickItemMenuPageFlip(box) != 0) {
		return;
	}

	if (UI_BOX_DATA[3].state != 0) {
		return;
	}

	if (UI_BOX_DATA[1].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (SCRIPT_PRICE != 0) {
		return;
	}

	if (isKeyDown(PADRdown)) {
		src = &ITEM_MENU_POPUP_ORIGINS[ITEM_MENU_MODE];
		if (ITEM_MENU_MODE == ITEM_MENU_BUY_CARD) {
			setRECT(&rect, src->x, src->y, src->w, src->h);
			openItemConfirmBox(&rect);
		} else if (ITEM_MENU_MODE == ITEM_MENU_MERIT_CARD) {
			pickMeritCard();
		} else {
			setRECT(&rect, src->x, src->y, src->w, src->h);
			openQuantityBox(&rect);
		}
	} else if (isKeyDown(PADRup)) {
		if (ITEM_MENU_MODE != ITEM_MENU_MERIT_CARD) {
			SELECTION_MENU_STATE = 4;
		} else {
			SELECTION_MENU_STATE = 0xa;
		}
		playSound(0, 4);
	} else if (isKeyDown(PADLup)) {
		if (POLLED_INPUT & PADR1) {
			itemMenuJumpToFirst(box, 9, 1);
		} else {
			itemMenuCursorUp(box, 1);
		}
	} else if (isKeyDown(PADLdown)) {
		if (POLLED_INPUT & PADR1) {
			itemMenuJumpToLast(box, 9, 1);
		} else {
			itemMenuCursorDown(box, 1);
		}
	}
}

void renderCardMenuBox(void)
{
	int16_t bx;
	int16_t by;
	int16_t cy;
	ItemMenuBox *box;

	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	renderItemMenuHeader(1, 0, bx + 8, by + 5);
	if (ITEM_MENU_MODE != ITEM_MENU_MERIT_CARD) {
		renderItemMenuHeader(1, 1, bx + 0x80, by + 5);
		if (ITEM_MENU_MODE == ITEM_MENU_SELL_CARD) {
			renderItemMenuHeader(1, 3, bx + 0xb6, by + 5);
		}
	} else {
		renderItemMenuHeader(1, 3, bx + 0x80, by + 5);
	}
	box = getActiveItemMenu();
	renderItemMenuScrollbar(box);
	cy = by + box->cursor * 0x12 + 0x11;
draw:
	renderSelectionCursor(bx + 5, cy, ITEM_MENU_CURSOR_WIDTHS[ITEM_MENU_MODE], 0x12, 5);
	renderItemMenuRows(box, bx + 0x1a, by + 0x13, bx + 8, by + 0x12, 1);
}

int32_t pickMeritCard(void)
{
	ItemMenuBox *box;
	int32_t off;
	int32_t idx;
	uint8_t kind;

	box = getActiveItemMenu();
	off = (box->topRow + box->cursor) * 2;
	SELECTED_ITEM = box->buf[off];

	if (SELECTED_ITEM != 0xff) {
		kind = box->buf[off + 1];
		if (kind != 0) {
			idx = SELECTED_ITEM * 4;
			SCRIPT_PRICE = *(int16_t *)((uint8_t *)&CARD_DATA[0].unk2 + idx);
			kind = ((uint8_t *)&CARD_DATA[0].spriteId)[SELECTED_ITEM * 4] + 7;
			showMapHeadTextbox(kind, readPStat(PSTAT_BUILTIN_ARG), 0, MAPHEAD_TEXT_MERIT_SHOP);
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 7;
			SCRIPT_STATE_3 = 1;
			playSound(0, 3);

			return 1;
		}
	}

	playSound(0, 0xb);

	return 0;
}

/*
 * Left and right pick the list (ITEM_KEEPER_SIDE), cross moves one item to
 * the other list, circle ten and square all of them.
 */
void tickItemKeeperMenu(void)
{
	ItemMenuBox *box;
	RECT rect;
	int32_t a;
	int32_t b;

	a = tickItemMenuPageFlip(MAIN_D_80134F68);
	b = tickItemMenuPageFlip(MAIN_D_80134F6C);
	if (a == 1) {
		return;
	}

	if (b == 1) {
		return;
	}

	if (UI_BOX_DATA[3].state != 0) {
		return;
	}

	if (UI_BOX_DATA[1].state != 1) {
		return;
	}

	if (UI_BOX_DATA[2].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (SELECTION_MENU_STATE != 1) {
		return;
	}

	if (ITEM_KEEPER_SIDE == 0) {
		box = MAIN_D_80134F68;
	} else {
		box = MAIN_D_80134F6C;
	}

	if (isKeyDown(PADRup)) {
		SELECTION_MENU_STATE = 6;
		playSound(0, 4);
	} else if (isKeyDown(PADLleft)) {
		ITEM_KEEPER_SIDE = 0;
		playSound(0, 2);
	} else if (isKeyDown(PADLright)) {
		ITEM_KEEPER_SIDE = 1;
		playSound(0, 2);
	} else if (box->itemCount != 0) {
		if (isKeyDown(PADRdown)) {
			itemKeeperMoveOne();
			playSound(0, 3);
		} else if (isKeyDown(PADRright)) {
			itemKeeperMoveTen();
			playSound(0, 3);
		} else if (isKeyDown(PADRleft)) {
			itemKeeperMoveAll();
			playSound(0, 3);
		} else if (isKeyDown(PADLup)) {
			if (POLLED_INPUT & PADR1) {
				itemMenuSelectFirst(box);
				layoutItemKeeperList(ITEM_KEEPER_SIDE);
			} else {
				itemMenuCursorUp(box, 0);
			}
		} else if (isKeyDown(PADLdown)) {
			if (POLLED_INPUT & PADR1) {
				itemMenuSelectLast(box);
				layoutItemKeeperList(ITEM_KEEPER_SIDE);
			} else {
				itemMenuCursorDown(box, 0);
			}
		} else if (isKeyDown(PADstart)) {
			setRECT(&rect, 5, 0x20, 0x80, 0x12);
			openItemDescriptionBox(box, &rect,
			                   (uint8_t)(ITEM_KEEPER_SIDE + 1));
			playSound(0, 3);
		}
	}
}

void renderItemKeeperStoredBox(void)
{
	renderItemKeeperBox(MAIN_D_80134F68, 0);
}

/* Lays out the title and the rows of the stored (0) or carried (1) items. */
void layoutItemKeeperList(int32_t boxIndex)
{
	int32_t value;

	if (boxIndex == 0) {
		/* "Items Stored" */
		showMapHeadTextbox(4, SPEAKER_NONE, 1, MAPHEAD_TEXT_ITEM_KEEPER);
		/* Keep the title on the first row and add the list after it. */
		TEXT_BOX_DATA[1].writeCount--;
		value = TEXT_NEWLINE;
		TEXT_BUFFERS_PTR[0x246] = value;
		TEXT_BUFFERS_PTR[0x3c6] = value;
		layoutItemMenu(MAIN_D_80134F68, 0xa, 0);
	} else {
		/* "Items in Hand" */
		showMapHeadTextbox(5, SPEAKER_NONE, 2, MAPHEAD_TEXT_ITEM_KEEPER);
		TEXT_BOX_DATA[2].writeCount--;
		value = TEXT_NEWLINE;
		TEXT_BUFFERS_PTR[0x266] = value;
		TEXT_BUFFERS_PTR[0x3e6] = value;
		layoutItemMenu(MAIN_D_80134F6C, 0xa, 0);
	}
}

void renderItemKeeperHandBox(void)
{
	renderItemKeeperBox(MAIN_D_80134F6C, 1);
}

int32_t itemKeeperMoveOne(void)
{
	ItemMenuBox *box;
	int32_t off;
	uint8_t *buf;
	uint8_t *b2;
	int32_t item;
	uint8_t *p;

	if (ITEM_KEEPER_SIDE == 0) {
		box = MAIN_D_80134F68;
		off = (box->topRow + box->cursor) * 2;
		buf = box->buf;
		item = buf[off];
		b2 = box->buf;

		if (item == 0xff) {
			return 0;
		}

		if ((b2[off + 1] & 0x80) == 0) {
			return 0;
		}

		if ((uint32_t)getItemCount(item) >= 0x63) {
			return 0;
		}

		p = (uint8_t *)((uint32_t)item + (uint32_t)SCRIPT_STATE_PTR);
		p[0x54] -= 1;
		giveItem(item, 1);
		fillItemKeeperLists();
		itemKeeperFocusStored(item);
		itemKeeperFocusHand(item);

		return 1;
	}

	box = MAIN_D_80134F6C;
	off = (box->topRow + box->cursor) * 2;
	buf = box->buf;
	item = buf[off];
	b2 = box->buf;

	if (item == 0xff) {
		return 0;
	}

	if ((b2[off + 1] & 0x80) == 0) {
		return 0;
	}

	if (getItemCount(item) == 0) {
		return 0;
	}

	removeItem(item, 1);
	p = (uint8_t *)((uint32_t)item + (uint32_t)SCRIPT_STATE_PTR);
	p[0x54] += 1;
	fillItemKeeperLists();
	itemKeeperFocusHand(item);
	itemKeeperFocusStored(item);

	return 1;
}

int32_t itemKeeperMoveTen(void)
{
	ItemMenuBox *box;
	uint8_t *buf;
	uint8_t *b2;
	int32_t off;
	int32_t item;
	int32_t item2;
	uint8_t flags;
	uint8_t *p;

	if (ITEM_KEEPER_SIDE == 0) {
		box = MAIN_D_80134F68;
		off = (box->topRow + box->cursor) * 2;
		buf = box->buf;
		item = buf[off];
		b2 = box->buf;

		if (item == 0xff) {
			return 0;
		}

		flags = b2[off + 1];

		if ((flags & 0x80) == 0) {
			return 0;
		}

		flags &= 0x7f;

		if (flags < 0xa) {
			return 0;
		}

		if (((uint32_t)getItemCount(item) & 0xff) >= 0x5a) {
			return 0;
		}

		p = (uint8_t *)((uint32_t)item + (uint32_t)SCRIPT_STATE_PTR);
		p[0x54] -= 0xa;
		giveItem(item, 0xa);
		fillItemKeeperLists();
		itemKeeperFocusStored(item);
		itemKeeperFocusHand(item);

		return 1;
	}

	box = MAIN_D_80134F6C;
	off = (box->topRow + box->cursor) * 2;
	buf = box->buf;
	item = buf[off];
	b2 = box->buf;

	if (item == 0xff) {
		return 0;
	}

	flags = b2[off + 1];

	if ((flags & 0x80) == 0) {
		return 0;
	}

	flags &= 0x7f;

	if (flags < 0xa) {
		return 0;
	}

	if (((uint8_t *)((uint32_t)item + (uint32_t)SCRIPT_STATE_PTR))[0x54] >= 0x5a) {
		return 0;
	}

	item2 = item;
	item = item2;
	removeItem(item, 0xa);
	p = (uint8_t *)((uint32_t)item2 + (uint32_t)SCRIPT_STATE_PTR);
	p[0x54] += 0xa;
	fillItemKeeperLists();
	itemKeeperFocusHand(item);
	itemKeeperFocusStored(item);

	return 1;
}

int32_t itemKeeperMoveAll(void)
{
	ItemMenuBox *box;
	uint8_t *p;
	int32_t off;
	uint8_t *buf;
	uint8_t *b2;
	int32_t item;
	int32_t item2;
	uint8_t amount;
	uint8_t cap;

	if (ITEM_KEEPER_SIDE == 0) {
		box = MAIN_D_80134F68;
		off = (box->topRow + box->cursor) * 2;
		buf = box->buf;
		item = buf[off];
		b2 = box->buf;

		if (item == 0xff) {
			return 0;
		}

		amount = b2[off + 1];

		if ((amount & 0x80) == 0) {
			return 0;
		}

		amount &= 0x7f;
		cap = 0x63 - getItemCount(item);

		if (cap < amount) {
			amount = cap;
		}

		p = (uint8_t *)((uint32_t)item + (uint32_t)SCRIPT_STATE_PTR);
		p[0x54] = (uint32_t)p[0x54] - amount;
		giveItem(item, amount);
		fillItemKeeperLists();
		itemKeeperFocusStored(item);
		itemKeeperFocusHand(item);

		return 1;
	}

	box = MAIN_D_80134F6C;
	off = (box->topRow + box->cursor) * 2;
	buf = box->buf;
	item = buf[off];
	b2 = box->buf;

	if (item == 0xff) {
		return 0;
	}

	amount = b2[off + 1];

	if ((amount & 0x80) == 0) {
		return 0;
	}

	amount &= 0x7f;
	cap = 0x63 - ((uint8_t *)((uint32_t)item + (uint32_t)SCRIPT_STATE_PTR))[0x54];
	item2 = item;
	item = item2;

	if (cap < amount) {
		amount = cap;
	}

	removeItem(item, amount);
	p = (uint8_t *)((uint32_t)item2 + (uint32_t)SCRIPT_STATE_PTR);
	p[0x54] = (uint32_t)p[0x54] + amount;
	fillItemKeeperLists();
	itemKeeperFocusHand(item);
	itemKeeperFocusStored(item);

	return 1;
}

/* Keeps the cursor of the stored list on itemId after moving it. */
void itemKeeperFocusStored(int32_t itemId)
{
	int32_t i;
	int32_t top;
	int32_t cursor;
	int32_t count;
	ItemMenuBox *box;
	int32_t visible;
	int32_t d;

	MAIN_D_80134F68->prevTopRow = MAIN_D_80134F68->topRow;
	MAIN_D_80134F68->prevCursor = MAIN_D_80134F68->cursor;
	visible = MAIN_D_80134F68->visibleRows;
	box = MAIN_D_80134F68;
	count = box->itemCount;
	top = box->topRow;
	cursor = box->cursor;

	for (i = 0; i < count * 2; i += 2) {
		if (MAIN_D_80134F68->buf[i] == itemId) {
			i >>= 1;
			goto found;
		}
	}

	if (count == 0) {
		box->topRow = 0;
		MAIN_D_80134F68->cursor = 0;
		goto end;
	}
	i = top + cursor;
	if (i >= count) {
		i = count - 1;
	}
	if ((top + visible >= count) && (top != 0)) {
		box->topRow--;
		goto end;
	}
found:
	if ((i >= top) && (i < top + visible)) {
		box->cursor = i - top;
		goto end;
	}
	d = i - visible;
	if (d < 0) {
		box->topRow = 0;
		MAIN_D_80134F68->cursor = i;
	} else {
		box->topRow = d + 1;
		MAIN_D_80134F68->cursor = visible - 1;
	}
end:
	layoutItemKeeperList(0);
}

/* Keeps the cursor of the carried list on item after moving it. */
void itemKeeperFocusHand(int32_t item)
{
	int32_t i;
	ItemMenuBox *box;
	ItemMenuBox *menu;
	int32_t limit;
	int32_t top;
	int32_t scroll;

	MAIN_D_80134F6C->prevTopRow = MAIN_D_80134F6C->topRow;
	MAIN_D_80134F6C->prevCursor = MAIN_D_80134F6C->cursor;

	box = MAIN_D_80134F6C;
	menu = box;
	limit = menu->itemCount * 2;
	for (i = 0; i < limit; i += 2) {
		if (item == box->buf[i]) {
			i = i >> 1;
			if (i >= menu->topRow) {
				top = menu->topRow;
				if (i < (top + menu->visibleRows)) {
					menu->cursor = i - top;
					goto done;
				}
			}

			scroll = i - menu->visibleRows;
			if (scroll < 0) {
				menu->topRow = 0;
				MAIN_D_80134F6C->cursor = i;
			} else {
				menu->topRow = scroll + 1;
				MAIN_D_80134F6C->cursor = MAIN_D_80134F6C->visibleRows - 1;
			}
			goto done;
		}
	}

done:
	layoutItemKeeperList(1);
}

void renderItemKeeperBox(ItemMenuBox *box, int8_t flag)
{
	GsBOXF rect;
	int32_t x;
	int32_t clut;
	BoxLabel label1;
	BoxLabel label2;
	int16_t y;
	uint8_t boxId;
	int16_t bx;
	int16_t by;
	int32_t color;
	TextBoxData *tbox;

	boxId = box->boxId;
	label1 = ITEM_KEEPER_HAND_LABEL;
	label2 = ITEM_KEEPER_STORED_LABEL;
	bx = UI_BOX_DATA[boxId].finalPos.x;
	by = UI_BOX_DATA[boxId].finalPos.y;
	renderRaisedFrame(boxId, 4, 0x15, 0x67, 0xb);
	renderItemMenuHeader(boxId, 0, bx + 8, by + 0x17);
	renderRaisedFrame(boxId, 0x6b, 0x15, 0x25, 0xb);
	renderItemMenuHeader(boxId, 3, bx + 0x6e, by + 0x17);
	renderItemMenuScrollbar(box);
	y = by + box->cursor * 0x12 + 0x21;
draw:
	renderSelectionCursor(bx + 5, y, 0x80, 0x12, 6 - boxId);
	tbox = &TEXT_BOX_TABLE.box[boxId];
	getVRAMModeCoords(tbox->vramMode, &x, &clut);
	y = 0x6c;
	y += tbox->backPage * tbox->vramRows * 12;
	if (flag != 0) {
		drawString(label1.s, x, y);
		color = 0x38;
	} else {
		drawString(label2.s, x, y);
		color = 0x3c;
	}
	renderString(0, bx + 0x2d, by + 7, color, 0xc, x, y, 6 - boxId, 1);
	if (ITEM_KEEPER_SIDE == boxId - 1) {
		rect.attribute = 0x40000000;
		rect.g = 0x80;
		rect.r = 0x80;
		rect.b = 0xff;
		rect.x = bx + 4;
		rect.y = by + 3;
		rect.w = 0x8c;
		rect.h = 0x12;
		GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, (uint16_t)(6 - boxId));
	}
	renderItemMenuRows(box, bx + 8, by + 0x24, 0, 0, 2);
}

void tickJukeboxMenu(void)
{
	int32_t cur;
	int32_t offset;

	if (tickItemMenuPageFlip(MAIN_D_80134F68) != 0) {
		return;
	}

	if (UI_BOX_DATA[0].state != 1) {
		return;
	}

	if (UI_BOX_DATA[1].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	cur = readPStat(PSTAT_SELECTED) & 0xff;
	if (cur != JUKEBOX_PLAYING) {
		JUKEBOX_PLAYING = cur;
		stopBGM();
		offset = cur * 2;
		playMusic(JUKEBOX_TRACKS[offset],
		          JUKEBOX_TRACKS[offset + 1]);
	}

	if (isKeyDown(PADRdown)) {
		writePStat(PSTAT_SELECTED,
		           MAIN_D_80134F68->buf[(MAIN_D_80134F68->topRow +
		                                 MAIN_D_80134F68->cursor) *
		                                2]);
		/* "<bgm>, right?" */
		showMapHeadTextbox(2, readPStat(PSTAT_BUILTIN_ARG), 0, MAPHEAD_TEXT_SERVICES);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		layoutItemMenu(MAIN_D_80134F68, 9, 2);
		playSound(0, 3);
	} else if (isKeyDown(PADRup)) {
		SELECTION_MENU_STATE = 4;
		playSound(0, 4);
	} else if (isKeyDown(PADLup)) {
		if (POLLED_INPUT & PADR1) {
			itemMenuJumpToFirst(MAIN_D_80134F68, 9, 2);
		} else {
			itemMenuCursorUp(MAIN_D_80134F68, 2);
		}
	} else if (isKeyDown(PADLdown)) {
		if (POLLED_INPUT & PADR1) {
			itemMenuJumpToLast(MAIN_D_80134F68, 9, 2);
		} else {
			itemMenuCursorDown(MAIN_D_80134F68, 2);
		}
	}
}

void renderJukeboxMenuBox(void)
{
	int16_t bx;
	int16_t by;
	int16_t cy;

	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	renderItemMenuHeader(1, 4, bx + 8, by + 5);
	renderItemMenuHeader(1, 5, bx + 0x26, by + 5);
	renderItemMenuScrollbar(MAIN_D_80134F68);
	cy = by + MAIN_D_80134F68->cursor * 0x12 + 0x11;
draw:
	renderSelectionCursor(bx + 5, cy, 0xca, 0x12, 5);
	renderItemMenuRows(MAIN_D_80134F68, bx + 8, by + 0x13, 0, 0, 2);
}

void tickTransportMenu(void)
{
	uint8_t item;

	if (tickItemMenuPageFlip(MAIN_D_80134F68) != 0) {
		return;
	}

	if (UI_BOX_DATA[0].state != 1) {
		return;
	}

	if (UI_BOX_DATA[1].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (SCRIPT_PRICE != 0) {
		return;
	}

	if (isKeyDown(PADRdown)) {
		item = MAIN_D_80134F68->buf[(MAIN_D_80134F68->topRow +
		                             MAIN_D_80134F68->cursor) *
		                            2];
		if ((item & 0x80) != 0) {
			/* "Are you sure? Once you go, / I can't take you back." */
			showMapHeadTextbox(6, readPStat(PSTAT_BUILTIN_ARG), 0, MAPHEAD_TEXT_SERVICES);
			SCRIPT_PRICE = TRANSPORT_DESTINATIONS[item & 0x7f].cost;
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 4;
			SCRIPT_STATE_3 = 1;
			playSound(0, 3);
		} else {
			playSound(0, 0xb);
		}
	} else if (isKeyDown(PADRup)) {
		SELECTION_MENU_STATE = 6;
		playSound(0, 4);
	} else if (isKeyDown(PADLup)) {
		if (POLLED_INPUT & PADR1) {
			itemMenuJumpToFirst(MAIN_D_80134F68, 9, 3);
		} else {
			itemMenuCursorUp(MAIN_D_80134F68, 3);
		}
	} else if (isKeyDown(PADLdown)) {
		if (POLLED_INPUT & PADR1) {
			itemMenuJumpToLast(MAIN_D_80134F68, 9, 3);
		} else {
			itemMenuCursorDown(MAIN_D_80134F68, 3);
		}
	}
}

void renderTransportMenuBox(void)
{
	int16_t bx;
	int16_t by;
	int16_t cy;

	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	renderItemMenuHeader(1, 6, bx + 8, by + 5);
	renderItemMenuHeader(1, 2, bx + 0xa2, by + 5);
	renderItemMenuScrollbar(MAIN_D_80134F68);
	cy = by + MAIN_D_80134F68->cursor * 0x12 + 0x11;
draw:
	renderSelectionCursor(bx + 5, cy, 0xca, 0x12, 5);
	renderItemMenuRows(MAIN_D_80134F68, bx + 8, by + 0x13, 0, 0, 2);
}

void tickTradeMenu(void)
{
	uint8_t item;

	if (tickItemMenuPageFlip(MAIN_D_80134F68) != 0) {
		return;
	}

	if (UI_BOX_DATA[0].state != 1) {
		return;
	}

	if (UI_BOX_DATA[1].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (readPStat(PSTAT_SELECTED) != 0xff) {
		return;
	}

	if (isKeyDown(PADRdown)) {
		item = MAIN_D_80134F68->buf[(MAIN_D_80134F68->topRow +
		                             MAIN_D_80134F68->cursor) *
		                            2];
		if ((item & 0x80) != 0) {
			writePStat(PSTAT_SELECTED, (item & 0x7f));
			/* "You'll really give me <item>?" */
			showMapHeadTextbox(0xb, readPStat(PSTAT_BUILTIN_ARG), 0, MAPHEAD_TEXT_SERVICES);
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 4;
			SCRIPT_STATE_3 = 1;
			playSound(0, 3);
		} else {
			playSound(0, 0xb);
		}
	} else if (isKeyDown(PADRup)) {
		SELECTION_MENU_STATE = 6;
		playSound(0, 4);
	} else if (isKeyDown(PADLup)) {
		if (POLLED_INPUT & PADR1) {
			itemMenuSelectFirst(MAIN_D_80134F68);
			layoutTradeList();
		} else {
			itemMenuCursorUp(MAIN_D_80134F68, 4);
		}
	} else if (isKeyDown(PADLdown)) {
		if (POLLED_INPUT & PADR1) {
			itemMenuSelectLast(MAIN_D_80134F68);
			layoutTradeList();
		} else {
			itemMenuCursorDown(MAIN_D_80134F68, 4);
		}
	}
}

void renderTradeMenuBox(void)
{
	uint32_t x;
	int32_t clut;
	int16_t bx;
	int16_t by;
	int16_t y;
	int32_t id;
	TextBoxData *box;

	bx = UI_BOX_DATA[1].finalPos.x;
	id = MAIN_D_80134F68->boxId;
	by = UI_BOX_DATA[1].finalPos.y;
	renderHorizontalLine(1, 4, 0x15, 0xd7);
	box = &TEXT_BOX_TABLE.box[id];
	getVRAMModeCoords(box->vramMode, (int32_t *)&x, &clut);
	y = 0x6c;
	y += box->backPage * box->vramRows * 12;
	drawString(TRADE_HEADER_LABELS, x, y);
	renderString(0, bx + 0x14, by + 5, 0x42, 0xc, x, y, 5, 1);
	renderString(0, bx + 0x7a, by + 5, 0x4e, 0xc, x + 0x42, y, 5, 1);
	y = by + MAIN_D_80134F68->cursor * 0x12 + 0x19;
draw:
	renderSelectionCursor(bx + 5, y, 0xd4, 0x12, 5);
	renderItemMenuRows(MAIN_D_80134F68, bx + 8, by + 0x1b, 0, 0, 2);
}

void layoutTradeList(void)
{
	/* "I want You have" */
	showMapHeadTextbox(0xf, SPEAKER_NONE, 1, MAPHEAD_TEXT_SERVICES);

	/* Keep the title on the first row and add the list after it. */
	--TEXT_BOX_DATA[1].writeCount;
	TEXT_BUFFERS_PTR[0x254] = TEXT_NEWLINE;
	TEXT_BUFFERS_PTR[0x354] = TEXT_NEWLINE;

	layoutItemMenu(MAIN_D_80134F68, 0xa, 4);
}

void setupNewGameDialogueBox(void)
{
	int32_t i;
	RECT rect2;
	RECT rect1;
	int16_t screenPos[2];

	TEXT_MONOSPACE = 0;
	for (i = 2; i < 10; i++) {
		Entity *ent = ENTITY_TABLE[i];
		if (ent->type == 0x75) {
			getEntityScreenPos(ent, 1, screenPos);
		}
	}

	setRECT(&rect1, screenPos[0], screenPos[1], 10, 10);
	setRECT(&rect2, -130, -78, 262, 59);
	createTextbox(0, 0x81, &rect2, &rect1, tickScriptDialogueBox,
	              renderScriptDialogueBox);
	registerTextbox(0, 0, 4, 1, 0);
}

void showNewgameDialogue(int32_t textId, int16_t nextState)
{
	showMapHeadTextbox(textId, SPEAKER_NARRATOR, 0, MAPHEAD_TEXT_SERVICES);

	SELECTION_MENU_STATE = 1;
	SCRIPT_STATE_4 = nextState;
	SCRIPT_STATE_3 = 1;
}

void showNewgameSelection(int32_t textId, int16_t nextState)
{
	int32_t sel = 0;

	showMapheadSelection(textId, SPEAKER_NARRATOR, 2, &sel, MAPHEAD_TEXT_SERVICES);

	SELECTION_MENU_STATE = 1;
	SCRIPT_STATE_4 = nextState;
	SCRIPT_STATE_3 = 2;
}

void setupNameSelectorBox(void)
{
	int32_t i;
	int32_t flags;
	RECT rect2;
	RECT rect1;
	int16_t screenPos[2];

	TEXT_MONOSPACE = 1;
	if ((NAMING_FLAGS & NAMING_FROM_SCRIPT) == 0) {
		flags = 0xc1;
	} else {
		flags = 0xe1;
	}

	for (i = 2; i < 10; i++) {
		Entity *ent = ENTITY_TABLE[i];
		if (ent->type == 0x75) {
			getEntityScreenPos(ent, 1, screenPos);
		}
	}

	setRECT(&rect1, screenPos[0], screenPos[1], 10, 10);
	setRECT(&rect2, -145, -91, 290, 138);
	createTextbox(1, flags, &rect2, &rect1, tickNamingBox,
	              renderNamingBox);
	registerTextbox(1, 1, 7, 1, 0);

	MAIN_D_80134F81 = 0;
	NAMING_CURSOR = 0;
	layoutNamingBox();
}

void setupNameDisplayBox(void)
{
	int32_t i;
	int32_t flags;
	RECT rect2;
	RECT rect1;
	int16_t screenPos[2];

	if ((NAMING_FLAGS & NAMING_FROM_SCRIPT) == 0) {
		flags = 0xc1;
	} else {
		flags = 0xe1;
	}

	for (i = 2; i < 10; i++) {
		Entity *ent = ENTITY_TABLE[i];
		if (ent->type == 0x75) {
			getEntityScreenPos(ent, 1, screenPos);
		}
	}

	setRECT(&rect1, screenPos[0], screenPos[1], 10, 10);
	setRECT(&rect2, -145, 60, 149, 42);
	createTextbox(2, flags, &rect2, &rect1, 0, renderNameDisplayBox);
	registerTextbox(2, 0, 1, 0, 0);

	MAIN_D_80134F82 = strlen(NAMING_BUFFER) >> 1;
	if (MAIN_D_80134F82 == 6) {
		--MAIN_D_80134F82;
	}

	updateNamingPreview();
}

/*
 * Square clears the name, triangle deletes a character, circle and start jump
 * to the buttons, and cross types the character under the cursor.
 */
void tickNamingBox(void)
{
	int16_t row;
	int16_t col;
	uint16_t special;
	char **rows;
	char *str;
	uint8_t hi;
	uint8_t lo;
	int32_t pos;
	int16_t idx;
	uint8_t n;

	if (flipMenuPage(1) != 0) {
		return;
	}
	if (UI_BOX_DATA[1].state != 1) {
		return;
	}
	if (UI_BOX_DATA[2].state != 1) {
		return;
	}
	if (isXPressedAfterDialogue() == 0) {
		return;
	}
	row = NAMING_CURSOR / 5;
	col = NAMING_CURSOR % 5;
	special = NAMING_CURSOR & 0x7fff;
	if (isKeyDown(PADRleft)) {
		NAMING_BUFFER[0] = 0;
		MAIN_D_80134F82 = 0;
		updateNamingPreview();
		playSound(0, 3);
		return;
	}
	if (isKeyDown(PADRdown)) {
		if ((NAMING_CURSOR & NAMING_ON_BUTTON) == 0) {
			if (row < 9) {
				rows = NAMING_CHAR_PAGES[MAIN_D_80134F81 * 2];
			} else {
				row -= 9;
				rows = NAMING_CHAR_PAGES[MAIN_D_80134F81 * 2 + 1];
			}
			str = rows[row];
			col *= 2;
			hi = str[col + 0];
			lo = str[col + 1];
			/* A name cannot start with a space. */
			if (hi == 0x81 && lo == 0x40 && MAIN_D_80134F82 == 0) {
				playSound(0, 0xb);
				return;
			}
			n = MAIN_D_80134F82;
			idx = n * 2;
			pos = idx;
			NAMING_BUFFER[idx] = hi;
			NAMING_BUFFER[pos + 1] = lo;
			NAMING_BUFFER[pos + 2] = 0;
			if (n != 5) {
				MAIN_D_80134F82++;
			}
			updateNamingPreview();
			if (MAIN_D_801B1D26[0] != 0 && MAIN_D_80134F82 == 5) {
				NAMING_CURSOR = NAMING_BUTTON_OK;
			}
			playSound(0, 3);
			return;
		}
		/* special is 0 for Back and 1 for OK. */
		if (special != 1) {
			if (special == 0) {
				namingDeleteLast();
			}
		} else {
			if (NAMING_BUFFER[0] == 0) {
				playSound(0, 0xb);
				return;
			}
			terminateNamingBuffer();
			playSound(0, 3);
			SELECTION_MENU_STATE = 0x15;
		}
	} else if (isKeyDown(PADRright)) {
		NAMING_CURSOR = NAMING_BUTTON_BACK;
		playSound(0, 2);
	} else if (isKeyDown(PADRup)) {
		namingDeleteLast();
	} else if (isKeyDown(PADstart)) {
		NAMING_CURSOR = NAMING_BUTTON_OK;
		playSound(0, 2);
	} else if (isKeyDown(PADLleft)) {
		namingSelectionLeft(col, row, (int16_t)special);
	} else if (isKeyDown(PADLright)) {
		namingSelectionRight(col, row, (int16_t)special);
	} else if (isKeyDown(PADLup)) {
		namingSelectionUp(col, row);
	} else if (isKeyDown(PADLdown)) {
		namingSelectionDown(col, row);
	}
}

void renderNamingBox(void)
{
	TextBoxData *box;
	int32_t k;
	int32_t j;
	int32_t i;
	int16_t ty;
	int16_t y;
	int16_t x;
	int16_t bx;
	int16_t by;
	int16_t texY;
	int16_t texX;
	int16_t *lbl;

	renderSelectionBox();
	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	texX = 0;
	box = &TEXT_BOX_DATA[1];
	texY = box->backPage * box->vramRows * 12;
	texY += box->vramRow * 12;
	lbl = NAMING_LABEL_LAYOUT;
	for (j = 0; j < 3; j++, lbl += 3) {
		renderString(0, bx + lbl[0], by + lbl[1], lbl[2], 12, texX, texY, 5, 1);
		texX += lbl[2];
	}
	if (NAMING_BUFFER[0] == 0) {
		ty = 0xd;
	} else {
		ty = 0;
	}
	renderString(ty, bx + lbl[0], by + lbl[1], lbl[2], 12, texX, texY, 5, 1);
	y = by + 6;
	ty = texY + 12;
	for (i = 0; i < 3; i++, ty += 12) {
		texX = 0;
		for (j = 0; j < 3; j++, y += 14) {
			x = bx + 0x4e;
			for (k = 0; k < 5; k++, x += 18, texX += 12) {
				renderString(0, x, y, 12, 12, texX, ty, 5, 1);
			}
		}
	}
	y = by + 6;
	for (i = 0; i < 3; i++, ty += 12) {
		texX = 0;
		for (j = 0; j < 3; j++, y += 14) {
			x = bx + 0xbe;
			for (k = 0; k < 5; k++, x += 18, texX += 12) {
				renderString(0, x, y, 12, 12, texX, ty, 5, 1);
			}
		}
	}
}

/* Lays out the title, the buttons and the current page of characters. */
void layoutNamingBox(void)
{
	TextBoxData *box;
	uint8_t *buf;
	char **table;
	int32_t j;
	int32_t row;
	int32_t page;
	uint8_t *line;

	box = &TEXT_BOX_DATA[1];
	buf = TEXT_BUFFERS_PTR + box->vramRow * 64;
	buf = (uint8_t *)((uint32_t)buf + (box->backPage ^ 1) * box->vramRows * 64);
	line = buf;
	*buf++ = TEXT_COLOR;
	*buf++ = TEXT_COLOR_YELLOW;
	strcpy(buf, NAMING_TITLE);
	buf += strlen(NAMING_TITLE);
	*buf++ = TEXT_COLOR;
	*buf++ = TEXT_COLOR_WHITE;
	strcpy(buf, NAMING_BUTTON_LABELS);
	buf += strlen(NAMING_BUTTON_LABELS);
	*buf++ = TEXT_NEWLINE;
	*buf++ = 0;
	line += 0x40;
	page = MAIN_D_80134F81 * 2;
	for (row = 0; row < 2; row++) {
		table = NAMING_CHAR_PAGES[page + row];
		for (j = 0; j < 9; j += 3) {
			buf = line + row * 0xc0 + (j / 3) * 64;
			strcpy(buf, table[j]);
			buf += strlen(table[j]);
			strcpy(buf, table[j + 1]);
			buf += strlen(table[j + 1]);
			strcpy(buf, table[j + 2]);
			buf += strlen(table[j + 2]);
			*buf++ = TEXT_NEWLINE;
			*buf++ = 0;
		}
	}
	buf -= 2;
	*buf = 0;
	box->pageReady = 1;
	box->writeCount++;
}

/* Draws the name typed so far, padded to six characters. */
void updateNamingPreview(void)
{
	uint8_t *out;
	int32_t len;

	out = TEXT_BUFFERS_PTR;
	*out++ = TEXT_COLOR;
	*out++ = TEXT_COLOR_WHITE;

	if ((NAMING_FLAGS & NAMING_PARTNER) == 0) {
		strcpy(out, NAMING_PREVIEW_PREFIX);
		out += strlen(NAMING_PREVIEW_PREFIX);
	} else {
		strcpy(out, NAMING_PREVIEW_PREFIX + 1);
		out += strlen(((int32_t)NAMING_PREVIEW_PREFIX + 1));
	}

	strcpy(out, NAMING_BUFFER);
	len = strlen(NAMING_BUFFER);
	out += len;
	out = padWithSpaces(out, 6, len);
	terminateString(out, 1);
	drawTextRow(TEXT_BUFFERS_PTR, 0, 0, 1);
}

void namingDeleteLast(void)
{
	if (NAMING_BUFFER[0] != 0) {
		if (MAIN_D_80134F82 == 0) {
			NAMING_BUFFER[0] = 0;
		} else {
			MAIN_D_80134F82--;
			NAMING_BUFFER[MAIN_D_80134F82 * 2] = 0;
		}

		updateNamingPreview();
		playSound(0, 3);
	}
}

void terminateNamingBuffer(void)
{
	int pos;
	int lowPos;
	int scan;

	pos = 0;
	scan = 2;
	while (pos < 0xc) {
		if (NAMING_BUFFER[scan] == 0) {
			break;
		}

		pos += 2;
		scan += 2;
	}

	lowPos = pos + 1;

	while (pos != 0) {
		uint8_t hiByte;
		uint8_t loByte;

		hiByte = NAMING_BUFFER[pos];
		if (hiByte != 0x81) {
			break;
		}

		loByte = NAMING_BUFFER[lowPos];
		if (loByte != 0x40) {
			break;
		}

		pos -= 2;
		lowPos -= 2;
	}

	NAMING_BUFFER[pos + 2] = 0;
}

void namingSelectionLeft(int16_t col, int16_t row, int32_t specialIdx)
{
	if ((NAMING_CURSOR & NAMING_ON_BUTTON) == 0) {
		if (row < 9) {
			if (col == 0) {
				NAMING_CURSOR = MAIN_D_801307A0[row];
			} else {
				--col;
				NAMING_CURSOR = col + (row * 5);
			}
		} else {
			if (col == 0) {
				row -= 9;
				col = 4;
			} else {
				--col;
			}

			NAMING_CURSOR = col + (row * 5);
		}
	} else {
		NAMING_CURSOR = MAIN_D_80134608[specialIdx] + 0x31;
	}

	playSound(0, 2);
}

void namingSelectionRight(int16_t col, int16_t row, int32_t specialIdx)
{
	if ((NAMING_CURSOR & NAMING_ON_BUTTON) == 0) {
		if (row < 9) {
			if (col == 4) {
				row += 9;
				col = 0;
			} else {
				++col;
			}

			NAMING_CURSOR = col + (row * 5);
		} else if (col == 4) {
			NAMING_CURSOR = MAIN_D_801307A0[row - 9];
		} else {
			++col;
			NAMING_CURSOR = col + (row * 5);
		}
	} else {
		NAMING_CURSOR = MAIN_D_80134608[specialIdx];
	}

	playSound(0, 2);
}

void namingSelectionUp(int16_t column, int16_t row)
{
	if ((NAMING_CURSOR & NAMING_ON_BUTTON) == 0) {
		if (row < 9) {
			if (row == 0) {
				row = 8;
			} else {
				--row;
			}
		} else if (row == 9) {
			row = 17;
		} else {
			--row;
		}

		NAMING_CURSOR = column + row * 5;
	} else if (NAMING_CURSOR == NAMING_BUTTON_BACK) {
		NAMING_CURSOR = NAMING_BUTTON_OK;
	} else {
		--NAMING_CURSOR;
	}

	playSound(0, 2);
}

void namingSelectionDown(int16_t column, int16_t row)
{
	if ((NAMING_CURSOR & NAMING_ON_BUTTON) == 0) {
		if (row < 9) {
			if (row == 8) {
				row = 0;
			} else {
				++row;
			}
		} else if (row == 0x11) {
			row = 9;
		} else {
			++row;
		}

		NAMING_CURSOR = column + row * 5;
	} else if (NAMING_CURSOR == NAMING_BUTTON_OK) {
		NAMING_CURSOR = NAMING_BUTTON_BACK;
	} else {
		++NAMING_CURSOR;
	}

	playSound(0, 2);
}

void renderSelectionBox(void)
{
	SelectionBoxUVData u0;
	SelectionBoxUVData u1;
	SelectionBoxUVData v0;
	SelectionBoxUVData v1;
	SelectionBoxOffsetData xOffset;
	SelectionBoxOffsetData yOffset;
	SelectionBoxOffsetData width;
	SelectionBoxOffsetData height;
	SelectionBoxOffsetData altXOffset;
	SelectionBoxOffsetData altWidth;
	POLY_FT4 *prim;
	GsOT_TAG *tag;
	uint16_t tpage;
	uint16_t clut;
	int16_t baseX;
	int16_t baseY;
	int32_t idx;
	int32_t sel;
	int16_t bx;
	int16_t by;
	int32_t i;

	tpage = GetTPage(0, 0, 896, 448);
	clut = GetClut(256, 508);
	u0 = MAIN_D_8013460C;
	u1 = MAIN_D_80134614;
	v0 = MAIN_D_8013461C;
	v1 = MAIN_D_80134624;
	xOffset = MAIN_D_8013462C;
	yOffset = MAIN_D_80134634;
	width = MAIN_D_8013463C;
	height = MAIN_D_80134644;
	altXOffset = MAIN_D_8013464C;
	altWidth = MAIN_D_80134654;

	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;

	if ((NAMING_CURSOR & NAMING_ON_BUTTON) == 0) {
		sel = NAMING_CURSOR;
		baseY = sel / 5;
		baseX = sel % 5;

		if (baseY < 9) {
			baseX = (bx + (baseX * 18)) + 0x4a;
			baseY = (by + (baseY * 14)) + 2;
		} else {
			baseX = (bx + (baseX * 18)) + 0xba;
			baseY = (by + ((baseY - 9) * 14)) + 2;
		}
	} else {
		sel = NAMING_CURSOR;
		idx = ((sel & 0x7fff) * 3) + 3;
		baseX = (bx + NAMING_LABEL_LAYOUT[idx]) - 4;
		baseY = (by + (&NAMING_LABEL_LAYOUT[1])[idx]) - 4;
	}

	tag = &ACTIVE_ORDERING_TABLE->org[5];

	for (i = 0; i < 8; i++) {
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		setUV4(prim, u0.data[i], v0.data[i], u1.data[i], v0.data[i], u0.data[i], v1.data[i], u1.data[i], v1.data[i]);

		if ((NAMING_CURSOR & NAMING_ON_BUTTON) == 0) {
			setPosDataPolyFT4(prim, baseX + xOffset.data[i],
			                  baseY + yOffset.data[i], width.data[i],
			                  height.data[i]);
		} else {
			setPosDataPolyFT4(prim, baseX + altXOffset.data[i],
			                  baseY + yOffset.data[i], altWidth.data[i],
			                  height.data[i]);
		}

		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = tpage;
		prim->clut = clut;
		AddPrim(tag, prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}
}

void renderNameDisplayBox(void)
{
	int16_t x;
	int16_t y;
	int16_t sx;
	int16_t y6;
	int16_t v;
	int16_t i;
	x = UI_BOX_DATA[2].finalPos.x;
	y = UI_BOX_DATA[2].finalPos.y;
	sx = x + 7;
	y6 = y + 6;
	if ((NAMING_FLAGS & NAMING_PARTNER) == 0) {
		v = 0x48;
	} else {
		v = 0x54;
	}
	renderString(0, sx, y6, v, 0xc, 0, 0, 4, 1);
	sx = x + 0x3a;
	y6 = y + 0x14;
	for (i = 0; i < 6; i++, sx += 0xe, v += 0xc) {
		renderString(0, sx, y6, 0xc, 0xc, v, 0, 4, 1);
	}
	renderNamingUnderscore(2, MAIN_D_80134F82 * 0xc + 4, 0x10, 0xc);
}

void renderNamingUnderscore(uint8_t boxId, int16_t x, int16_t y, int32_t w)
{
	int32_t yc;

	x = x + UI_BOX_DATA[boxId].finalPos.x;
	y = y + UI_BOX_DATA[boxId].finalPos.y;
	boxId = 6 - boxId;
	yc = y;

	renderLinePrimitive(0x20202, x, yc, (x + w) - 1, yc, boxId, 0);
	y++;
	renderLinePrimitive(0x10c0c0, x, y, (x + w) - 1, y, boxId, 0);
	y++;
	renderLinePrimitive(0x20202, x, y, (x + w) - 1, y, boxId, 0);
}

void returnFromScriptFile(void)
{
	StackEntry entry;
	uint8_t *script;
	uint8_t *section;
	int32_t type;

	for (;;) {
		popScriptStack(&entry);
		type = entry.smth[0];
		if (type == 0) {
			break;
		}
		if (type == 3) {
			readMapTFS(CURRENT_MAP_ID);
			MAIN_func_800D634C(CURRENT_MAP_ID, 0);
			SCRIPT_WARPING = 0;
			script = getScript(CURRENT_SCRIPT_ID);
			section = getScriptSection(script, 0xfe);
			if (section != 0) {
				CURRENT_SCRIPT_PTR = script;
				SCRIPT_PC = section;
				longjmp(SCRIPT_JMP_BUF, 1);
			}
		} else if (type == 4) {
			if (entry.smth[1] != 0xff) {
				CURRENT_SCRIPT_PTR = getScript(CURRENT_SCRIPT_ID);
				SCRIPT_PC = getScriptSection(CURRENT_SCRIPT_PTR, entry.smth[1]);
			} else {
				setMapHeadActive();
			}
			longjmp(SCRIPT_JMP_BUF, 2);
		}
	}
	IS_SCRIPT_PAUSED = 1;
	longjmp(SCRIPT_JMP_BUF, 2);
}

uint8_t readPStat(int32_t index)
{
	return SCRIPT_STATE_PTR->pstats[index];
}

/*
 * Called when a script starts talking or moving things: the first time, it
 * stops the player and the clock and turns the NPC that was talked to and the
 * player to face each other. It also stops owner, if it is an NPC.
 */
void beginScriptEvent(int32_t owner)
{
	int32_t entityId;
	uint8_t speaker;
	int32_t idx;

	if (SCRIPT_HAS_CONTROL == 0) {
		SCRIPT_HAS_CONTROL = 1;
		setMovementEnabled(0, 1);
		setMovementEnabled(1, 1);
		unsetCameraFollowPlayer();
		writePStat(0, 3);
		clearTextArea();
		stopGameTime();

		if (SCRIPT_FROM_TALK != 0 && isTriggerSet(TRIGGER_44) == 0) {
			entityId = scriptIdToEntityId(SCRIPT_SECTION) & 0xff;
			if (entityId != 0xff) {
				SCRIPT_MOVEMENTS[entityId * 0xc] = 0;
				idx = entityId * 0xc;
				speaker = SCRIPT_SECTION;
				MAIN_D_801BE6B5[idx] = speaker;
				MAIN_D_801BE6B6[idx] = 0xfd;
				SCRIPT_MOVEMENTS[0] = 0;
				MAIN_D_801BE6B5[0] = 0xfd;
				MAIN_D_801BE6B6[0] = speaker;
			}
		}
	}

	unsetTrigger(TRIGGER_44);

	if ((uint32_t)owner < SPEAKER_SPECIAL) {
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
