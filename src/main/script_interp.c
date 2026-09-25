#include <string.h>

#include <libcd.h>

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
extern int16_t MAIN_D_8013078C[];
extern uint8_t MAIN_D_80135007;
extern uint8_t MAIN_D_80134F82;
extern uint16_t MAIN_D_801307A0[10];
extern char MAIN_D_801345F4[4];
extern uint8_t MAIN_D_80130438[];
extern uint8_t MAIN_D_80130444[];
extern uint8_t MAIN_D_801303B8[];
extern uint8_t MAIN_D_80134F8F;
extern uint8_t MAIN_D_80134F90;
extern int32_t MAIN_D_8012FFC4[];
extern uint16_t MAIN_D_80134608[2];
extern uint8_t MAIN_D_80134F81;
extern uint32_t POLLED_INPUT;
extern RECT MAIN_D_801302BC[];
extern char MAIN_D_80130394[];
extern char MAIN_D_801303A8[];
extern uint8_t PREVIOUS_SCREEN;
extern uint8_t PREVIOUS_EXIT;
extern uint8_t CURRENT_EXIT;
extern uint8_t MAIN_D_801BE738[];
extern int8_t TALKED_TO_ENTITY;
extern uint8_t MAPHEAD_DATA[];
extern uint8_t SCRIPT_HEADER[];
extern uint8_t SCRIPT_DATA[];
extern ScriptState SCRIPT_STATE;
extern uint8_t TEXT_BUFFERS[];
extern char MAIN_D_80130374[];
extern BoxLabel MAIN_D_801307B4;
extern BoxLabel MAIN_D_801307C0;
extern TextBoxTable MAIN_D_801BE80C;
extern char MAIN_D_801307CC[];
extern char **MAIN_D_80130774[];
extern char MAIN_D_801B1D26[];
extern char MAIN_D_801345F8[5];
extern char MAIN_D_80134600[8];
extern int16_t MAIN_D_801302FC[];
extern uint8_t *CURRENT_SCRIPT_PTR;

void unsetCameraFollowPlayer(void);
int32_t MAIN_func_800D8E64(int32_t param_1, int32_t param_2, int32_t param_3);
int32_t tickRemoveMist(void);
int32_t MAIN_func_801138B0(void);
int32_t MAIN_func_80113A20(void);
int32_t isTrainingComplete(void);
void setCameraFollowPlayer(void);
void setFoodTimer(int32_t type);
void setActiveAnim(int32_t state);
void MAIN_func_800FD7D8(uint8_t boxId, int32_t idx, int16_t x, int16_t y);
void MAIN_func_800FD8D4(ItemMenuBox *box);
void MAIN_func_800FDC5C(ItemMenuBox *box, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int32_t flag);
void renderSelectionCursor(int32_t x, int32_t y, int16_t w, int16_t h, int32_t layer);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t x, int32_t y, int32_t w, int32_t h);
int32_t random(int32_t limit);
void *allocateArray(uint32_t size);
void freeArray(uint32_t *array);
uint8_t *padWithSpaces(uint8_t *str, int32_t width, int32_t used);
void terminateString(uint8_t *str, int32_t flag);
void drawString2(uint8_t *str, int16_t x, int16_t y, int32_t flag);
int32_t hasMove(int32_t moveId);
void learnMove(int32_t moveId);
void unloadDigimonModel(int32_t a0);
void MAIN_func_800FF310(ItemMenuBox *box);
int32_t isXPressedAfterDialogue(void);
int32_t isKeyDown(int32_t mask);
int32_t MAIN_func_800FCFB8(RECT *origin);
int32_t MAIN_func_800FD244(RECT *origin);
ItemMenuBox *MAIN_func_800FCC40(void);
int32_t MAIN_func_800FD61C(ItemMenuBox *box, RECT *origin, int32_t uiBoxId);
void MAIN_func_800FD3DC(ItemMenuBox *box, int32_t startRow, int32_t style);
void MAIN_func_800FD4E8(ItemMenuBox *box, int32_t startRow, int32_t style);
void MAIN_func_800FF2A8(ItemMenuBox *box);
int32_t MAIN_func_800FCF88(ItemMenuBox *box);
void MAIN_func_800FD428(ItemMenuBox *box, int32_t style);
void MAIN_func_800FD534(ItemMenuBox *box, int32_t style);
void unloadDigimonModel(int32_t digimonType);
void loadNPCModel(int32_t modelId);
void MAIN_func_80108EB4(ItemMenuBox *box, int8_t flag);
void MAIN_func_801097F4(void);
void MAIN_func_801099E8(void);
void MAIN_func_80107E6C(void);
void MAIN_func_80108090(void);
void MAIN_func_801094F0(void);
void MAIN_func_801096E8(void);
void updateNamingPreview(void);
void MAIN_func_80108C88(int32_t itemId);
void MAIN_func_80108DC0(int32_t a0);
void MAIN_func_801091DC(void);
void MAIN_func_801093E4(void);
void tickNamingBox(void);
void renderNamingBox(void);
void MAIN_func_8010A79C(void);
int32_t MAIN_func_80106730(int32_t op, int32_t lhs, int32_t rhs);
void updateBGM(void);
void forceUpdateBGM(void);
void pollNextScriptTwoUShort(uint16_t *out1, uint16_t *out2);
void MAIN_func_80108334(void);
void MAIN_func_80108604(void);
void MAIN_func_80108610(int32_t boxIndex);
void MAIN_func_801086D4(void);
int32_t MAIN_func_801086E0(void);
int32_t MAIN_func_80108890(void);
int32_t MAIN_func_80108A98(void);
void MAIN_func_80109BBC(void);
void namingDeleteLast(void);
void terminateNamingBuffer(void);
void namingSelectionLeft(int16_t col, int16_t row, int32_t specialIdx);
void namingSelectionRight(int16_t col, int16_t row, int32_t specialIdx);
int32_t MAIN_func_80108230(void);
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
void MAIN_func_800FF338(uint8_t boxId, int16_t x, int16_t y, int32_t w, int16_t h);
void getVRAMModeCoords(int32_t mode, int32_t *outX, int32_t *outClut);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void GsSortBoxFill(GsBOXF *bp, GsOT *otp, u_short pri);
void renderHorizontalLine(uint8_t boxId, int16_t x, int16_t y, int32_t w);
void drawString(char *str, int32_t x, int32_t y);
int32_t MAIN_func_800FE650(uint8_t boxId);
void MAIN_func_800D634C(int32_t param_1, int32_t param_2);

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
	MAIN_func_8010A79C,
	renderNamingBox,
	tickNamingBox,
	setupNameDisplayBox,
	setupNameSelectorBox,
	showNewgameSelection,
	showNewgameDialogue,
	setupNewGameDialogueBox,
	MAIN_func_80109BBC,
	MAIN_func_801099E8,
	MAIN_func_801097F4,
	MAIN_func_801096E8,
	MAIN_func_801094F0,
	MAIN_func_801093E4,
	MAIN_func_801091DC,
	MAIN_func_80108EB4,
	MAIN_func_80108DC0,
	MAIN_func_80108C88,
	MAIN_func_80108A98,
	MAIN_func_80108890,
	MAIN_func_801086E0,
	MAIN_func_801086D4,
	MAIN_func_80108610,
	MAIN_func_80108604,
	MAIN_func_80108334,
	MAIN_func_80108230,
	MAIN_func_80108090,
	MAIN_func_80107E6C,
	MAIN_func_80107DFC,
	MAIN_func_80107D54,
	MAIN_func_80107C4C,
	MAIN_func_80107B98,
	MAIN_func_80107AB8,
	MAIN_func_801078F4,
	MAIN_func_80107784,
	MAIN_func_80107660,
	MAIN_func_80107444,
	MAIN_func_801072C4,
	MAIN_func_80107200,
	MAIN_func_80107110,
	MAIN_func_80107000,
	showCardTextbox,
	rollCard,
	MAIN_func_80106D28,
	MAIN_func_80106D1C,
	getTriggerOffset,
	pollNextScriptTwoUShort,
	scriptUnloadModel,
	forceUpdateBGM,
	updateBGM,
	playBGM,
	pollNextTwoScriptShorts,
	pollNextScriptShort,
	scriptLoadModel,
	MAIN_func_8010692C,
	pollNextInt,
	dateToSeconds,
	setCardAmount,
	getCardAmount,
	scriptLearnMove,
	skipOneReadInteger,
	MAIN_func_80106730,
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
	MAIN_func_801062F8,
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
	MAIN_func_80105464,
	MAIN_func_801053EC,
	scriptUpdateEnergyBoundaries,
	MAIN_func_801050C0,
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

	MAIN_func_80100258(0);
	tickScriptedMovements();
	if (MAIN_D_80134FE9 == 0x4b) {
		if (MAIN_func_800D8E64((int16_t)MAIN_D_80134FF8,
		                       (int16_t)SELECTION_MENU_STATE,
		                       MAIN_D_80134FA0)) {
			MAIN_D_80134FF0 = 0;
			MAIN_func_801062F8(SPEAKER_NONE);
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
	case 0x10:
		MAIN_D_80135000 = 1;
		scriptShowSelection();
		longjmp(SCRIPT_JMP_BUF, 2);
		break;
	case 0x13:
		skipOneReadOneUShort(&shortArg);
		entry.scriptPtr = MAIN_D_80134FDC;
		entry.scriptId = ACTIVE_MAP_SCRIPT;
		entry.smth[0] = 1;
		pushScriptStack(&entry);
		MAIN_D_80134FDC =
			(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR + shortArg);
		break;
	case 0x14:
		skipOneReadTwoShort(&shortArg, &offset);
		entry.scriptPtr = MAIN_D_80134FDC;
		entry.scriptId = ACTIVE_MAP_SCRIPT;
		entry.smth[0] = 1;
		pushScriptStack(&entry);
		CURRENT_SCRIPT_PTR = getScript(shortArg);
		MAIN_D_80134FDC = getScriptSection(
			(uint8_t *)(int32_t)CURRENT_SCRIPT_PTR, offset);
		break;
	case 0x15:
		MAIN_D_80134FDC++;
		popScriptStack(&entry);
		CURRENT_SCRIPT_PTR = getScript(entry.scriptId);
		MAIN_D_80134FDC = (uint8_t *)entry.scriptPtr;
		break;
	case 0x16:
		skipOneReadOneUShort(&shortArg);
		MAIN_D_80134FDC =
			(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR + shortArg);
		break;
	case 0x17:
		skipOneReadTwoShort(&shortArg, &offset);
		CURRENT_SCRIPT_PTR = getScript(shortArg);
		MAIN_D_80134FDC = getScriptSection(
			(uint8_t *)(int32_t)CURRENT_SCRIPT_PTR, offset);
		break;
	case 0x18:
		pollOneUByteOneUShort(&pstat, &shortArg);
		value = readPStat(pstat);
		if (value >= shortArg) {
			value = (shortArg - 1) & 0xff;
		}
		shortArg = *(uint16_t *)(MAIN_D_80134FDC + (value << 1));
		MAIN_D_80134FDC =
			(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR + shortArg);
		break;
	case 0x19:
		MAIN_D_80134FDC++;
		MAIN_func_801050C0();
		break;
	case 0x1a:
		MAIN_D_80134FDC++;
		if (MAIN_D_80135000 == 2) {
			showTextbox(0, SPEAKER_NONE);
		} else {
			showTextbox(0, MAIN_D_80134FE6);
		}
		longjmp(SCRIPT_JMP_BUF, 2);
	case 0x1b:
		pollNextScriptUByte(&pstat);
		MAIN_D_80135000 = 0;
		if (UI_BOX_DATA[0].state != 1) {
			MAIN_D_80134FE6 = pstat - 1;
		}
		setDialogueOwner(pstat);
		break;
	case 0x1c:
		skipOneReadOneUShort(&shortArg);
		setTrigger(shortArg);
		break;
	case 0x1d:
		skipOneReadOneUShort(&shortArg);
		unsetTrigger(shortArg);
		break;
	case 0x1e:
		skipOnePollTwoScriptBytes(&pstat, &value);
		writePStat(pstat, value);
		break;
	case 0x1f:
		skipOnePollTwoScriptBytes(&pstat, &value);
		newValue = readPStat(pstat) + value;
		if (newValue >= 0x100) {
			newValue = 0xff;
		}
		writePStat(pstat, newValue);
		break;
	case 0x20:
		skipOnePollTwoScriptBytes(&pstat, &value);
		newValue = readPStat(pstat) - value;
		if (newValue < 0) {
			newValue = 0;
		}
		writePStat(pstat, newValue);
		break;
	case 0x21:
		pollNextScriptUByte(&pstat);
		writePStat(pstat, CURRENT_MAP_ID);
		break;
	case 0x22:
		pollNextScriptUByte(&pstat);
		value = PARTNER_ENTITY.digimonEntity.entity.type;
		writePStat(pstat, value);
		break;
	case 0x23:
		pollNextScriptUByte(&pstat);
		setInventorySize(pstat);
		break;
	case 0x24:
		skipOnePollTwoScriptBytes(&pstat, &value);
		writePStat(pstat, random(value + 1));
		break;
	case 0x25:
		pollNextScriptUByte(&pstat);
		writePStat(pstat, YEAR);
		writePStat((pstat + 1) & 0xff, DAY);
		writePStat((pstat + 2) & 0xff, HOUR);
		writePStat((pstat + 3) & 0xff, MINUTE);
		break;
	case 0x26:
		MAIN_func_801062F8(SPEAKER_NONE);
		MAIN_D_80135000 = 2;
		scriptSetTextboxSize();
		break;
	case 0x27:
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
		MAIN_D_80134FA0 = (MAIN_D_80134FE0 != 0) ^ 1;
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

void MAIN_func_80105464(uint8_t actorId, int32_t animationId)
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

void MAIN_func_801050C0(void)
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
		MAIN_D_80134FDC++;
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
			MAIN_D_80134FDC =
				(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR +
			                    shortArg);
			longjmp(SCRIPT_JMP_BUF, 1);
		case (3 << 3):
			pollNextScriptUShort(&shortArg);
			if (result != 0) {
				continue;
			}

			MAIN_D_80134FDC =
				(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR +
			                    shortArg);
			longjmp(SCRIPT_JMP_BUF, 1);
		case (4 << 3):
			switch (condOp & 7) {
			case 0:
				cond = MAIN_func_801022FC();
				break;
			case 1:
				cond = MAIN_func_801024CC();
				break;
			case 2:
				cond = MAIN_func_80102514();
				break;
			case 3:
				cond = MAIN_func_80102564();
				break;
			case 4:
				cond = MAIN_func_801025E8();
				break;
			case 5:
				cond = MAIN_func_80102630();
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

int32_t MAIN_func_80106730(int32_t op, int32_t lhs, int32_t rhs)
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
	case 0x10:
		MAIN_D_80135000 = 1;
		scriptShowSelection();
		longjmp(SCRIPT_JMP_BUF, 2);
		break;
	case 0x13:
		skipOneReadOneUShort(&shortArg);
		entry.scriptPtr = MAIN_D_80134FDC;
		entry.scriptId = ACTIVE_MAP_SCRIPT;
		entry.smth[0] = 1;
		pushScriptStack(&entry);
		MAIN_D_80134FDC = (uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR + shortArg);
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
	case 0x28:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		if (giveItem(byteArg1, byteArg2) != 0) {
			unsetTrigger(0);
		} else {
			setTrigger(0);
		}
		break;
	case 0x29:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		removeItem(byteArg1, byteArg2);
		break;
	case 0x2a:
		skipOneReadInteger(&intArg);
		MONEY += intArg;
		if (MONEY >= 0xf4240) {
			MONEY = 0xf423f;
		}
		break;
	case 0x2b:
		skipOneReadInteger(&intArg);
		MONEY -= intArg;
		if (MONEY < 0) {
			MONEY = 0;
		}
		break;
	case 0x2c:
		scriptCompareDate();
		break;
	case 0x2d:
		pollNextScriptUByte(&byteArg1);
		scriptLearnMove(byteArg1);
		break;
	case 0x2e:
		pollNextScriptUByte(&byteArg1);
		break;
	case 0x2f:
		pollNextScriptUByte(&byteArg1);
		byteArg2 = getCardAmount(byteArg1);
		if (byteArg2 < 9) {
			byteArg2 = byteArg2 + 1u;
			setCardAmount(byteArg1, byteArg2);
		}
		break;
	case 0x30:
		pollNextScriptUByte(&byteArg1);
		byteArg2 = getCardAmount(byteArg1);
		if (byteArg2 != 0) {
			byteArg2 = byteArg2 - 1u;
			setCardAmount(byteArg1, byteArg2);
		}
		break;
	case 0x31:
		skipOneReadOneUShort(&value);
		MERIT = value;
		if (MERIT >= 0x2710) {
			MERIT = 0x270f;
		}
		break;
	case 0x32:
		skipOneReadOneUShort(&value);
		MERIT += value;
		if (MERIT >= 0x2710) {
			MERIT = 0x270f;
		}
		break;
	case 0x33:
		skipOneReadOneUShort(&value);
		MERIT = -value;
		if (MERIT < 0) {
			MERIT = 0;
		}
		break;
	case 0x34:
		pollOneUByteOneUShort(&byteArg1, &value);
		statPtr = getStatsPointer(byteArg1);
		value32 = value;
		*statPtr = enforceStatsLimits(byteArg1, (int16_t)value32);
		scriptUpdateEnergyBoundaries(byteArg1, *statPtr);
		if (byteArg1 == 0x15) {
			TAMER_ENTITY.tamerLevel = MAIN_D_80135002;
		}
		if (byteArg1 == 0x16) {
			PARTNER_ENTITY.lives = MAIN_D_80135004;
		}
		break;
	case 0x35:
		pollOneUByteOneUShort(&byteArg1, &value);
		statPtr = getStatsPointer(byteArg1);
		value32 = value;
		*statPtr += value32;
		*statPtr = enforceStatsLimits(byteArg1, *statPtr);
		scriptUpdateEnergyBoundaries(byteArg1, *statPtr);
		if (byteArg1 == 0x15) {
			TAMER_ENTITY.tamerLevel = MAIN_D_80135002;
		}
		if (byteArg1 == 0x16) {
			PARTNER_ENTITY.lives = MAIN_D_80135004;
		}
		break;
	case 0x36:
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
		if (byteArg1 == 0x15) {
			TAMER_ENTITY.tamerLevel = MAIN_D_80135002;
		}
		if (byteArg1 == 0x16) {
			PARTNER_ENTITY.lives = MAIN_D_80135004;
		}
		break;
	case 0x37:
		pollNextScriptUByte(&byteArg1);
		byteArg2 = readPStat(byteArg1);
		day = readPStat((byteArg1 + 1) & 0xff);
		hour = readPStat((byteArg1 + 2) & 0xff);
		minute = readPStat((byteArg1 + 3) & 0xff);
		sec = dateToSeconds(byteArg2, day, hour, minute);
		{
			uint32_t cur_sec = dateToSeconds(YEAR, DAY & 0xff,
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
	case 0x38:
	case 0x39:
		pollNextScriptUByte(&byteArg1);
		pollNextInt(&intArg);
		byteArg2 = readPStat(byteArg1);
		day = readPStat((byteArg1 + 1) & 0xff);
		hour = readPStat((byteArg1 + 2) & 0xff);
		minute = readPStat((byteArg1 + 3) & 0xff);
		sec = dateToSeconds(byteArg2, day, hour, minute);
		if (op == 0x38) {
			intArg = sec + intArg;
		} else {
			intArg = sec - intArg;
			if (intArg < 0) {
				intArg = 0;
			}
		}
		MAIN_func_8010692C(intArg, &byteArg2, &day, &hour, &minute);
		writePStat(byteArg1, byteArg2);
		writePStat((byteArg1 + 1) & 0xff, day);
		writePStat((byteArg1 + 2) & 0xff, hour);
		writePStat((byteArg1 + 3) & 0xff, minute);
		break;
	case 0x3a:
	case 0x3b:
	case 0x3c:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		break;
	case 0x3d:
	case 0x3e:
		pollNextScriptUByte(&byteArg1);
		break;
	case 0x3f:
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
	case 0x46:
		pollNextScriptUByte(&byteArg1);
		scriptLoadModel(byteArg1);
		break;
	case 0x47:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		scriptSetDigimon(byteArg1, byteArg2, byteArg3);
		break;
	case 0x48:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		scriptUnloadEntity(byteArg1);
		break;
	case 0x49:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		callDigimonRoutine(byteArg1);
		break;
	case 0x4a:
		pollNextScriptUByte(&byteArg1);
		ACTIVE_INSTRUCTION = 0x4a;
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
		MAIN_D_80134FA4 = byteArg1;
		longjmp(SCRIPT_JMP_BUF, 2);
	case 0x4b:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_D_80134FF8 = byteArg1;
		SELECTION_MENU_STATE = byteArg2;
		MAIN_func_801053EC();
		entry.smth[0] = 4;
		entry.smth[1] = byteArg3;
		pushScriptStack(&entry);
		longjmp(SCRIPT_JMP_BUF, 3);
	case 0x4c:
		MAIN_func_801062F8(SPEAKER_NONE);
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = MAIN_D_801BE6B4 + entityId * 0xc;
		b[0] = 0;
		b[1] = byteArg1;
		b[2] = byteArg2;
		break;
	case 0x4d:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextScriptShort(&posX);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = MAIN_D_801BE6B4 + entityId * 0xc;
		b[0] = 1;
		b[1] = byteArg1;
		*(int16_t *)(b + 4) = posX;
		break;
	case 0x4e:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_func_80105464(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = MAIN_D_801BE6B4 + entityId * 0xc;
		b[0] = 2;
		b[1] = byteArg1;
		*(int16_t *)(b + 4) = posX;
		*(int16_t *)(b + 6) = posY;
		break;
	case 0x4f:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		b = (uint8_t *)&MAIN_D_801BE72C;
		b[0] = 6;
		*(int16_t *)(b + 4) = posX;
		*(int16_t *)(b + 6) = posY;
		b[3] = byteArg1;
		break;
	case 0x50:
		MAIN_func_801062F8(SPEAKER_NONE);
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		b = (uint8_t *)&MAIN_D_801BE72C;
		b[0] = 7;
		b[1] = byteArg1;
		b[3] = byteArg2;
		break;
	case 0x51:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_func_80105464(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = MAIN_D_801BE6B4 + entityId * 0xc;
		b[0] = 3;
		b[1] = byteArg1;
		b[2] = byteArg3;
		break;
	case 0x52:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_func_80105464(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = MAIN_D_801BE6B4 + entityId * 0xc;
		b[0] = 4;
		b[1] = byteArg1;
		*(int16_t *)(b + 4) = posX;
		*(int16_t *)(b + 6) = posY;
		break;
	case 0x53:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_func_80105464(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) {
			break;
		}
		b = MAIN_D_801BE6B4 + entityId * 0xc;
		b[0] = 5;
		b[1] = byteArg1;
		b[2] = byteArg3;
		break;
	case 0x54:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		resetEntityOrigin(byteArg1);
		break;
	case 0x55:
		MAIN_D_80134FDC++;
		pollNextTwoScriptShorts(&MAIN_D_80134FD2, &MAIN_D_80134FD4);
		pollNextScriptShort(&MAIN_D_80134FD6);
		break;
	case 0x56:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		scriptStartAnimation(byteArg1, byteArg2);
		break;
	case 0x57:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		setMapObjectsFlag(byteArg1, 1, byteArg2);
		break;
	case 0x58:
		MAIN_func_801062F8(SPEAKER_NONE);
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
	case 0x5a:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		playSound(byteArg1, byteArg2);
		break;
	case 0x5b:
		pollNextScriptUByte(&byteArg1);
		break;
	case 0x5c:
		pollNextScriptUByte(&byteArg1);
		break;
	case 0x5d:
		pollNextScriptUByte(&byteArg1);
		playBGM(byteArg1);
		break;
	case 0x5e:
		pollNextScriptUByte(&byteArg1);
		resetBGM();
		break;
	case 0x5f:
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
	case 0x64:
		pollNextScriptUByte(&byteArg1);
		MAIN_D_80134FF8 = byteArg1;
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
			ACTIVE_INSTRUCTION = 0x64;
			SELECTION_MENU_STATE = 0;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x07:
			byteArg1 = readPStat(0xfe);
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
			ACTIVE_INSTRUCTION = 0x64;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
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
			MAIN_func_800FC968(1);
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
			ACTIVE_INSTRUCTION = 0x64;
			SCRIPT_STATE_3 = 0;
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
			MAIN_D_8013500C = 0;
			break;
		case 0x1f: {
			int32_t hi = readPStat(0xf3) << 8;
			int32_t lo = readPStat(0xf4);
			posX = lo + hi;
			MAIN_D_8013500C += posX;
		} break;
		case 0x38:
			writePStat(0xf3, (MAIN_D_8013500C / 256) & 0xff);
			writePStat(0xf4, MAIN_D_8013500C & 0xff);
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
			MAIN_func_800FF9AC();
			break;
		case 0x24:
			readMapTFS(CURRENT_MAP_ID);
			break;
		case 0x27:
			MAIN_func_800D9360(CURRENT_MAP_ID);
			break;
		case 0x30:
			openSaveMachine();
			/* fall through */
		case 0x25:
			ACTIVE_INSTRUCTION = 0x64;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x36:
			gameClearSave();
			ACTIVE_INSTRUCTION = 0x64;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x26:
			createNinjamonEffect();
			break;
		case 0x33:
			addFileReadRequestPath(MAIN_D_80130394,
			                       BOSS_EFE_TMD_BUFFER, 0, 0,
			                       0);
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
			setTamerState(0x10);
			SOME_SCRIPT_SYNC_BIT = 0;
			ACTIVE_INSTRUCTION = 0x64;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
			break;
		case 0x37:
			setLoopCountToOne(readPStat(0xfe));
			longjmp(SCRIPT_JMP_BUF, 2);
			break;
		case 0x29:
			spawnGearbox();
			break;
		case 0x2a: {
			int8_t pstat = readPStat(0xfe);
			somethingToyTown(pstat);
		} break;
		case 0x2b:
			spawnToyTownBoxes();
			break;
		case 0x2c: {
			int8_t value = readPStat(0xfe);
			openToyTownBox(value);
		} break;
		case 0x2d: {
			int16_t value = readPStat(0xfe);
			fadeToWhite(value);
		} break;
		case 0x2e: {
			int16_t value = readPStat(0xfe);
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
	case 0x65:
		pollNextScriptUByte(&byteArg1);
		PARTNER_PARA.condition &= ~byteArg1;
		break;
	case 0x66:
		pollNextScriptUByte(&byteArg1);
		if (MAIN_D_80134FC8 < 0x270f) {
			MAIN_D_80134FC8++;
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
				CURRENT_SCRIPT_PTR = getScript(0);
				MAIN_D_80134FDC = getScriptSection((uint8_t *)(int32_t)CURRENT_SCRIPT_PTR, 0x4de);
				break;
			}
			if (outcome == 0) {
				if (MAIN_D_80134FCA < 0x270f) {
					MAIN_D_80134FCA++;
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
			MAIN_func_8010020C();
			break;
		}

		b = (uint8_t *)&MAIN_D_801BE72C;
		b[0] = 7;
		b[1] = 0xfd;
		b[3] = 0xa;

		ACTIVE_INSTRUCTION = 0x4a;
		MAIN_D_80134FA4 = 0xa;
		MAIN_func_8010020C();
		longjmp(SCRIPT_JMP_BUF, 2);
	case 0x67:
		skipOneReadOneUShort(&MAIN_D_80134FFC);
		ACTIVE_INSTRUCTION = 0x67;
		longjmp(SCRIPT_JMP_BUF, 2);
	case 0x68:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		MAIN_D_80134FE5 = byteArg1;
		if (byteArg1 != 2) {
			break;
		}
		MAIN_D_80135010 = byteArg2;
		break;
	case 0x69:
		MAIN_func_801062F8(SPEAKER_NONE);
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
	case 0x6a:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		byteArg1 = scriptIdToEntityId(byteArg1);
		if (byteArg1 == 0xff || byteArg1 < 2) {
			break;
		}
		NPC_ENTITIES[byteArg1 - 2].autotalk = byteArg2;
		break;
	case 0x6b:
		break;
	case 0x6c:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = MAIN_D_801BE6B4 + entityId * 0xc;
			b[0] = 8;
			b[1] = byteArg1;
			b[3] = byteArg2;
			*(int16_t *)(b + 4) = posX;
			*(int16_t *)(b + 6) = posY;
		}
		break;
	case 0x6d:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = MAIN_D_801BE6B4 + entityId * 0xc;
			b[0] = 9;
			b[1] = byteArg1;
			b[2] = byteArg2;
			b[3] = byteArg3;
		}
		break;
	case 0x6e:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = MAIN_D_801BE6B4 + entityId * 0xc;
			b[0] = 0xb;
			b[1] = byteArg1;
			b[3] = byteArg2;
			*(int16_t *)(b + 4) = posX;
			*(int16_t *)(b + 6) = posY;
		}
		break;
	case 0x6f:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = MAIN_D_801BE6B4 + entityId * 0xc;
			b[0] = 0xb;
			b[1] = byteArg1;
			b[2] = byteArg2;
			b[3] = byteArg3;
		}
		break;
	case 0x70:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_D_801BE738[0] = 0xc;
		MAIN_D_801BE738[1] = byteArg1;
		MAIN_D_801BE738[2] = byteArg3;
		break;
	case 0x71:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		pollNextTwoScriptBytes(&entityId, &padByte);
		pollNextTwoScriptShorts(&posX, &posY);
		byteArg1 += 0xc;
		b = MAIN_D_801BE6B4 + byteArg1 * 0xc;
		b[0] = 0xd;
		b[1] = byteArg2;
		b[3] = byteArg3;
		b[2] = entityId;
		*(int16_t *)(b + 8) = posX;
		*(int16_t *)(b + 0xa) = posY;
		break;
	case 0x72:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextScriptShort(&posX);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = MAIN_D_801BE6B4 + entityId * 0xc;
			b[0] = 0xe;
			b[1] = byteArg1;
			b[2] = byteArg2;
			*(int16_t *)(b + 4) = posX;
			b[3] = byteArg3;
		}
		break;
	case 0x73:
		MAIN_func_801062F8(SPEAKER_NONE);
		pollNextScriptUByte(&byteArg1);
		pollNextScriptShort(&posX);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = MAIN_D_801BE6B4 + entityId * 0xc;
			b[0] = 0xf;
			b[1] = byteArg1;
			b[2] = byteArg2;
			*(int16_t *)(b + 4) = posX;
			b[3] = byteArg3;
		}
		break;
	case 0x74:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		spawnItem(byteArg1, posX, posY);
		break;
	case 0x75:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptShorts(&posZ, &posW);
		pollNextScriptUShort(&triggerId);
		spawnChest(posX, posY, posZ, posW, byteArg1, triggerId);
		break;
	case 0x76:
		pollNextScriptUByte(&byteArg1);
		spawnBoulder();
		break;
	case 0x77:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		moveBoulder(posX, posY);
		longjmp(SCRIPT_JMP_BUF, 2);
	case 0x78:
		pollNextScriptUByte(&byteArg1);
		removeObject(0xfb6, 0);
		break;
	case 0x79:
		pollNextScriptUByte(&byteArg1);
		scriptUnloadModel(byteArg1);
		break;
	case 0x7a:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		byteArg1 = readPStat(byteArg1);
		writePStat(byteArg2, byteArg1);
		break;
	case 0x7b:
		pollNextScriptUByte(&byteArg1);
		entry.smth[0] = 4;
		entry.smth[1] = byteArg1;
		pushScriptStack(&entry);
		writePStat(0, MAIN_D_80134FE7);
		break;
	case 0x7c:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		setRectImpassible(posX, posY, byteArg2,
		                  byteArg3);
		break;
	case 0x7d:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptShorts(&posZ, &posW);
		spawnSpriteAtLocation(posX, posY, posZ, posW, byteArg1);
		break;
	case 0x7e:
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
	MAIN_D_80134FDC = scriptPtr + (off & 0xffff) - 2;
}

void initializeScripts(void)
{
	MAPHEAD_DATA_PTR = MAPHEAD_DATA;
	SCRIPT_HEADER_PTR = SCRIPT_HEADER;
	SCRIPT_DATA_PTR = SCRIPT_DATA;
	SCRIPT_STATE_PTR = &SCRIPT_STATE;
	TEXT_BUFFERS_PTR = TEXT_BUFFERS;

	readFile(MAIN_D_80130374, MAPHEAD_DATA_PTR);
	readFileSection(MAIN_D_80130388, SCRIPT_HEADER_PTR, 0, 0x2000);
	memset((void *)SCRIPT_STATE_PTR, 0, sizeof(*SCRIPT_STATE_PTR));

	CURRENT_SCRIPT_ID = 0xffff;
	ACTIVE_MAP_SCRIPT = 0xffff;
	MERIT = 0;
	MAIN_D_80134FC6 = 0;
	MAIN_D_80134FC8 = 0;
	MAIN_D_80134FCA = 0;
	MAIN_D_80134FCC = 0;
	TOURNAMENTS_LOST = 0;
	MAIN_D_80134FD0 = 0;
	MAIN_D_80134FD2 = -0x270f;
	MAIN_D_80134FD4 = -0x270f;
	MAIN_D_80134FD6 = -0x270f;

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

void callScriptSection(int32_t scriptId, int32_t section, int32_t param)
{
	int32_t i;

	CURRENT_SCRIPT_PTR = getScript(scriptId);
	MAIN_D_80134FDC =
		getScriptSection((uint8_t *)(int32_t)CURRENT_SCRIPT_PTR,
	                         section);
	MAIN_D_80134FE0 = param;
	MAIN_D_80134FE4 = section;
	MAIN_D_80134FE5 = 0;
	MAIN_D_80134FE6 = SPEAKER_PLAYER;
	MAIN_D_80134FE7 = readPStat(0);
	SOME_SCRIPT_SYNC_BIT = 1;
	ACTIVE_INSTRUCTION = 0;
	MAIN_D_80134FE9 = 0;
	MAIN_D_80134FEC = 0;
	MAIN_D_80134FF0 = 0;
	IS_SCRIPT_PAUSED = 0;
	MAIN_D_80134F9C = TALKED_TO_ENTITY;
	for (i = 0; i < 0x16; i++) {
		((ScriptCameraMovement *)MAIN_D_801BE6B4)[i].type = 0xff;
	}

	MAIN_func_8010020C();
}

void tickScriptedMovement(int32_t slot)
{
	ScriptCameraMovement *movement = &((ScriptCameraMovement *)MAIN_D_801BE6B4)[slot];
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

	stackTop = &SCRIPT_STATE_PTR->stack[MAIN_D_80134FC6];
	*stackTop = *entry;
	++MAIN_D_80134FC6;
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

	if (MAIN_D_80134FC6 != 0) {
		--MAIN_D_80134FC6;
		stackTop = &SCRIPT_STATE_PTR->stack[MAIN_D_80134FC6];
		entry = *stackTop;
	} else {
		entry.smth[0] = 0;
	}

	*out = entry;
}

void skipOneReadOneUShort(uint16_t *out)
{
	MAIN_D_80134FDC++;
	pollNextScriptUShort(out);
}

void pollNextScriptUByte(uint8_t *out)
{
	*out = *MAIN_D_80134FDC;
	MAIN_D_80134FDC++;
}

void pollNextScriptUShort(uint16_t *out)
{
	*out = *(uint16_t *)MAIN_D_80134FDC;
	MAIN_D_80134FDC += 2;
}

void pollOneUByteOneUShort(uint8_t *outByte, uint16_t *outShort)
{
	pollNextScriptUByte(outByte);
	pollNextScriptUShort(outShort);
}

void skipOneReadTwoShort(uint16_t *out1, uint16_t *out2)
{
	MAIN_D_80134FDC++;
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
	MAIN_D_80134FDC++;
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

int32_t MAIN_func_80107000(void)
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
			if ((MAIN_D_8012FFC4[CARD_DATA[id].spriteId] <= MONEY) &&
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

	cardId = readPStat(PSTAT_249) & 0xff;
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

	writePStat(PSTAT_249, CARD_DATA[cardId].digimonId);
	showMapHeadTextbox(line, 0xfd, 0, 0x4d3);
}

int32_t MAIN_func_80107200(void)
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

uint32_t dateToSeconds(uint32_t years, uint32_t days, uint32_t hours,
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

void MAIN_func_8010692C(uint32_t totalMinutes, uint8_t *outYear,
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
	*out = *(int16_t *)MAIN_D_80134FDC;
	MAIN_D_80134FDC += 2;
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
	MAIN_D_80135007 = variant;
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
	if (ACTIVE_BGM_FONT != font || MAIN_D_80135007 != variant) {
		ACTIVE_BGM_FONT = font;
		MAIN_D_80135007 = variant;
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
	MAIN_D_80135007 = variant;
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

int32_t MAIN_func_80106D1C(int32_t moveId)
{
	return hasMove((int16_t)moveId);
}

int32_t MAIN_func_80106D28(void)
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
			itemId = MAIN_D_8012FE78[i];
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

void MAIN_func_80107110(void)
{
	RECT rect;
	RECT origin;
	ItemMenuBox *result;
	int32_t boxId;
	RECT *src;

	if (MAIN_D_80135011 == 4) {
		boxId = 0xfd;
	} else {
		boxId = readPStat(PSTAT_254) & 0xff;
	}

	setupBoxOrigin(boxId, &origin);
	result = MAIN_func_800FCC40();
	src = &((RECT *)MAIN_D_8013027C)[MAIN_D_80135011];
	setRECT(&rect, src->x, src->y, src->w, src->h);
	createTextbox(1, 0xf1, &rect, &origin, MAIN_func_80107E6C,
	              MAIN_func_80108090);
	registerTextbox(1, 9, 6, 1, 0);
	MAIN_func_800FCC98(result, 1, 9);
	MAIN_func_800FCCFC(result, 9, 1);
	MAIN_D_8013500C = 0;
}

int32_t MAIN_func_801072C4(void)
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

void MAIN_func_80107444(void)
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

void MAIN_func_80107660(void)
{
	RECT rect;
	RECT origin;
	int32_t boxId;

	boxId = readPStat(PSTAT_254);
	setupBoxOrigin(boxId, &origin);
	setRECT(&rect, -152, -98, 148, 127);
	createTextbox(1, 0xe1, &rect, &origin, MAIN_func_80108334,
	              MAIN_func_80108604);
	registerTextbox(1, 9, 6, 1, 1);
	MAIN_func_800FCC98(MAIN_D_80134F68, 1, 0xa);
	MAIN_func_80108610(0);
	setupBoxOrigin(SPEAKER_PLAYER, &origin);
	setRECT(&rect, 0, -98, 148, 127);
	createTextbox(2, 0xe1, &rect, &origin, 0, MAIN_func_801086D4);
	registerTextbox(2, 9, 6, 1, 2);
	MAIN_func_800FCC98(MAIN_D_80134F6C, 2, 0xa);
	MAIN_func_80108610(1);
}

void MAIN_func_80107784(void)
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
		b = MAIN_D_801303B8[track * 2];
		if (b == ACTIVE_BGM_FONT) {
			b = MAIN_D_801303B8[track * 2 + 1];
			if (b == MAIN_D_80135007) {
				writePStat(PSTAT_249, i);
				MAIN_D_80134F8F = i;
				return;
			}
		}
	}

	writePStat(PSTAT_249, 0xff);
	MAIN_D_80134F8F = 0xff;
	stopBGM();
}

void MAIN_func_801078F4(void)
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

	setupBoxOrigin(readPStat(0xfe), &origin);
	setRECT(&rect, -0x47, -0x62, 0xde, 0x81);
	createTextbox(1, 0xf1, &rect, &origin, MAIN_func_801091DC, MAIN_func_801093E4);
	registerTextbox(1, 9, 6, 1, 0);

	if (MAIN_D_80134F68->isOpen == 0) {
		MAIN_D_80134F68->isOpen = 1;
		MAIN_D_80134F68->boxId = 1;

		item = readPStat(0xf9);
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

	MAIN_func_800FCCFC(MAIN_D_80134F68, 9, 2);
}

void MAIN_func_80107AB8(void)
{
	uint8_t *buf;
	uint8_t i;

	buf = MAIN_D_80134F68->buf;
	MAIN_D_80134F68->itemCount = 0;
	for (i = 0; i < 6; i++) {
		if (isTriggerSet(MAIN_D_8013024C[i].trigger) != 0) {
			MAIN_D_80134F68->itemCount++;
			if (MAIN_D_8013024C[i].cost <= MONEY) {
				*buf++ = i | 0x80;
				buf++;
			} else {
				*buf++ = i;
				buf++;
			}
		}
	}
}

void MAIN_func_80107B98(void)
{
	RECT rect;
	RECT origin;
	int32_t boxId;

	boxId = readPStat(PSTAT_254);
	setupBoxOrigin(boxId, &origin);
	setRECT(&rect, -71, -100, 222, 129);
	createTextbox(1, 0xf1, &rect, &origin, MAIN_func_801094F0,
	              MAIN_func_801096E8);
	registerTextbox(1, 9, 6, 1, 0);
	MAIN_func_800FCC98(MAIN_D_80134F68, 1, 9);
	MAIN_func_800FCCFC(MAIN_D_80134F68, 9, 3);
	MAIN_D_8013500C = 0;
}

void MAIN_func_80107C4C(void)
{
	uint8_t *buf;
	uint8_t *buf2;
	int32_t save;
	uint8_t i;
	uint8_t idx;
	int32_t item;

	MAIN_D_80135011 = readPStat(PSTAT_249) * 3;
	idx = MAIN_D_80135011;
	buf = MAIN_D_80134F68->buf;
	MAIN_D_80134F68->itemCount = 3;
	buf2 = MAIN_D_80134F6C->buf;
	MAIN_D_80134F6C->itemCount = 3;
	i = 0;
	while (i < 3) {
		item = MAIN_D_80130438[idx];
		save = idx;
		if (getItemCount(item) != 0) {
			*buf++ = item | 0x80;
			buf++;
		} else {
			*buf++ = item;
			buf++;
		}
		*buf2++ = MAIN_D_80130444[save];
		buf2++;
		i++;
		idx++;
	}
}

void MAIN_func_80107D54(void)
{
	RECT rect;
	RECT origin;
	int32_t boxId;

	boxId = readPStat(PSTAT_254);
	setupBoxOrigin(boxId, &origin);
	setRECT(&rect, -88, -80, 223, 83);
	createTextbox(1, 0xe1, &rect, &origin, MAIN_func_801097F4,
	              MAIN_func_801099E8);
	registerTextbox(1, 9, 4, 1, 0);
	MAIN_func_800FCC98(MAIN_D_80134F68, 1, 0xa);
	MAIN_func_80109BBC();
}

void MAIN_func_80107DFC(void)
{
	ItemMenuBox *box = MAIN_D_80134F68;
	int32_t trigger;

	trigger = box->topRow + box->cursor + MAIN_D_80135011 + 5;
	setTrigger(trigger);
	for (trigger = 5; trigger < 0xe; trigger++) {
		if (isTriggerSet(trigger) == 0) {
			return;
		}
	}

	setTrigger(0xe);
}

void MAIN_func_80107E6C(void)
{
	ItemMenuBox *box;
	RECT rect;
	RECT *src;

	box = MAIN_func_800FCC40();
	if (MAIN_func_800FCF88(box) != 0) {
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

	if (MAIN_D_8013500C != 0) {
		return;
	}

	if (isKeyDown(0x40)) {
		src = &MAIN_D_801302BC[MAIN_D_80135011];
		if (MAIN_D_80135011 == 3) {
			setRECT(&rect, src->x, src->y, src->w, src->h);
			MAIN_func_800FD244(&rect);
		} else if (MAIN_D_80135011 == 6) {
			MAIN_func_80108230();
		} else {
			setRECT(&rect, src->x, src->y, src->w, src->h);
			MAIN_func_800FCFB8(&rect);
		}
	} else if (isKeyDown(0x10)) {
		if (MAIN_D_80135011 != 6) {
			SELECTION_MENU_STATE = 4;
		} else {
			SELECTION_MENU_STATE = 0xa;
		}
		playSound(0, 4);
	} else if (isKeyDown(0x1000)) {
		if (POLLED_INPUT & 8) {
			MAIN_func_800FD3DC(box, 9, 1);
		} else {
			MAIN_func_800FD428(box, 1);
		}
	} else if (isKeyDown(0x4000)) {
		if (POLLED_INPUT & 8) {
			MAIN_func_800FD4E8(box, 9, 1);
		} else {
			MAIN_func_800FD534(box, 1);
		}
	}
}

void MAIN_func_80108090(void)
{
	int16_t bx;
	int16_t by;
	int16_t cy;
	ItemMenuBox *box;

	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	MAIN_func_800FD7D8(1, 0, bx + 8, by + 5);
	if (MAIN_D_80135011 != 6) {
		MAIN_func_800FD7D8(1, 1, bx + 0x80, by + 5);
		if (MAIN_D_80135011 == 4) {
			MAIN_func_800FD7D8(1, 3, bx + 0xb6, by + 5);
		}
	} else {
		MAIN_func_800FD7D8(1, 3, bx + 0x80, by + 5);
	}
	box = MAIN_func_800FCC40();
	MAIN_func_800FD8D4(box);
	cy = by + box->cursor * 0x12 + 0x11;
draw:
	renderSelectionCursor(bx + 5, cy, MAIN_D_801302FC[MAIN_D_80135011], 0x12, 5);
	MAIN_func_800FDC5C(box, bx + 0x1a, by + 0x13, bx + 8, by + 0x12, 1);
}

int32_t MAIN_func_80108230(void)
{
	ItemMenuBox *box;
	int32_t off;
	int32_t idx;
	uint8_t kind;

	box = MAIN_func_800FCC40();
	off = (box->topRow + box->cursor) * 2;
	MAIN_D_80134F78 = box->buf[off];

	if (MAIN_D_80134F78 != 0xff) {
		kind = box->buf[off + 1];
		if (kind != 0) {
			idx = MAIN_D_80134F78 * 4;
			MAIN_D_8013500C = *(int16_t *)((uint8_t *)&CARD_DATA[0].unk2 + idx);
			kind = ((uint8_t *)&CARD_DATA[0].spriteId)[MAIN_D_80134F78 * 4] + 7;
			showMapHeadTextbox(kind, readPStat(0xfe), 0, 0x4d4);
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

void MAIN_func_80108334(void)
{
	ItemMenuBox *box;
	RECT rect;
	int32_t a;
	int32_t b;

	a = MAIN_func_800FCF88(MAIN_D_80134F68);
	b = MAIN_func_800FCF88(MAIN_D_80134F6C);
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

	if (MAIN_D_80134F90 == 0) {
		box = MAIN_D_80134F68;
	} else {
		box = MAIN_D_80134F6C;
	}

	if (isKeyDown(0x10)) {
		SELECTION_MENU_STATE = 6;
		playSound(0, 4);
	} else if (isKeyDown(0x8000)) {
		MAIN_D_80134F90 = 0;
		playSound(0, 2);
	} else if (isKeyDown(0x2000)) {
		MAIN_D_80134F90 = 1;
		playSound(0, 2);
	} else if (box->itemCount != 0) {
		if (isKeyDown(0x40)) {
			MAIN_func_801086E0();
			playSound(0, 3);
		} else if (isKeyDown(0x20)) {
			MAIN_func_80108890();
			playSound(0, 3);
		} else if (isKeyDown(0x80)) {
			MAIN_func_80108A98();
			playSound(0, 3);
		} else if (isKeyDown(0x1000)) {
			if (POLLED_INPUT & 8) {
				MAIN_func_800FF310(box);
				MAIN_func_80108610(MAIN_D_80134F90);
			} else {
				MAIN_func_800FD428(box, 0);
			}
		} else if (isKeyDown(0x4000)) {
			if (POLLED_INPUT & 8) {
				MAIN_func_800FF2A8(box);
				MAIN_func_80108610(MAIN_D_80134F90);
			} else {
				MAIN_func_800FD534(box, 0);
			}
		} else if (isKeyDown(0x800)) {
			setRECT(&rect, 5, 0x20, 0x80, 0x12);
			MAIN_func_800FD61C(box, &rect,
			                   (uint8_t)(MAIN_D_80134F90 + 1));
			playSound(0, 3);
		}
	}
}

void MAIN_func_80108604(void)
{
	MAIN_func_80108EB4(MAIN_D_80134F68, 0);
}

void MAIN_func_80108610(int32_t boxIndex)
{
	int32_t value;

	if (boxIndex == 0) {
		showMapHeadTextbox(4, 0xff, 1, 0x4d5);
		TEXT_BOX_DATA[1].writeCount--;
		value = TEXT_NEWLINE;
		TEXT_BUFFERS_PTR[0x246] = value;
		TEXT_BUFFERS_PTR[0x3c6] = value;
		MAIN_func_800FCCFC(MAIN_D_80134F68, 0xa, 0);
	} else {
		showMapHeadTextbox(5, 0xff, 2, 0x4d5);
		TEXT_BOX_DATA[2].writeCount--;
		value = TEXT_NEWLINE;
		TEXT_BUFFERS_PTR[0x266] = value;
		TEXT_BUFFERS_PTR[0x3e6] = value;
		MAIN_func_800FCCFC(MAIN_D_80134F6C, 0xa, 0);
	}
}

void MAIN_func_801086D4(void)
{
	MAIN_func_80108EB4(MAIN_D_80134F6C, 1);
}

int32_t MAIN_func_801086E0(void)
{
	ItemMenuBox *box;
	int32_t off;
	uint8_t *buf;
	uint8_t *b2;
	int32_t item;
	uint8_t *p;

	if (MAIN_D_80134F90 == 0) {
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
		MAIN_func_80107444();
		MAIN_func_80108C88(item);
		MAIN_func_80108DC0(item);

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
	MAIN_func_80107444();
	MAIN_func_80108DC0(item);
	MAIN_func_80108C88(item);

	return 1;
}

int32_t MAIN_func_80108890(void)
{
	ItemMenuBox *box;
	uint8_t *buf;
	uint8_t *b2;
	int32_t off;
	int32_t item;
	int32_t item2;
	uint8_t flags;
	uint8_t *p;

	if (MAIN_D_80134F90 == 0) {
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
		MAIN_func_80107444();
		MAIN_func_80108C88(item);
		MAIN_func_80108DC0(item);

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
	MAIN_func_80107444();
	MAIN_func_80108DC0(item);
	MAIN_func_80108C88(item);

	return 1;
}

int32_t MAIN_func_80108A98(void)
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

	if (MAIN_D_80134F90 == 0) {
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
		MAIN_func_80107444();
		MAIN_func_80108C88(item);
		MAIN_func_80108DC0(item);

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
	MAIN_func_80107444();
	MAIN_func_80108DC0(item);
	MAIN_func_80108C88(item);

	return 1;
}

void MAIN_func_80108C88(int32_t itemId)
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
	MAIN_func_80108610(0);
}

void MAIN_func_80108DC0(int32_t item)
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
	MAIN_func_80108610(1);
}

void MAIN_func_80108EB4(ItemMenuBox *box, int8_t flag)
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
	label1 = MAIN_D_801307B4;
	label2 = MAIN_D_801307C0;
	bx = UI_BOX_DATA[boxId].finalPos.x;
	by = UI_BOX_DATA[boxId].finalPos.y;
	MAIN_func_800FF338(boxId, 4, 0x15, 0x67, 0xb);
	MAIN_func_800FD7D8(boxId, 0, bx + 8, by + 0x17);
	MAIN_func_800FF338(boxId, 0x6b, 0x15, 0x25, 0xb);
	MAIN_func_800FD7D8(boxId, 3, bx + 0x6e, by + 0x17);
	MAIN_func_800FD8D4(box);
	y = by + box->cursor * 0x12 + 0x21;
draw:
	renderSelectionCursor(bx + 5, y, 0x80, 0x12, 6 - boxId);
	tbox = &MAIN_D_801BE80C.box[boxId];
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
	if (MAIN_D_80134F90 == boxId - 1) {
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
	MAIN_func_800FDC5C(box, bx + 8, by + 0x24, 0, 0, 2);
}

void MAIN_func_801091DC(void)
{
	int32_t cur;
	int32_t offset;

	if (MAIN_func_800FCF88(MAIN_D_80134F68) != 0) {
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

	cur = readPStat(PSTAT_249) & 0xff;
	if (cur != MAIN_D_80134F8F) {
		MAIN_D_80134F8F = cur;
		stopBGM();
		offset = cur * 2;
		playMusic(MAIN_D_801303B8[offset],
		          MAIN_D_801303B8[offset + 1]);
	}

	if (isKeyDown(0x40)) {
		writePStat(PSTAT_249,
		           MAIN_D_80134F68->buf[(MAIN_D_80134F68->topRow +
		                                 MAIN_D_80134F68->cursor) *
		                                2]);
		showMapHeadTextbox(2, readPStat(PSTAT_254), 0, 0x4d6);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		MAIN_func_800FCCFC(MAIN_D_80134F68, 9, 2);
		playSound(0, 3);
	} else if (isKeyDown(0x10)) {
		SELECTION_MENU_STATE = 4;
		playSound(0, 4);
	} else if (isKeyDown(0x1000)) {
		if (POLLED_INPUT & 8) {
			MAIN_func_800FD3DC(MAIN_D_80134F68, 9, 2);
		} else {
			MAIN_func_800FD428(MAIN_D_80134F68, 2);
		}
	} else if (isKeyDown(0x4000)) {
		if (POLLED_INPUT & 8) {
			MAIN_func_800FD4E8(MAIN_D_80134F68, 9, 2);
		} else {
			MAIN_func_800FD534(MAIN_D_80134F68, 2);
		}
	}
}

void MAIN_func_801093E4(void)
{
	int16_t bx;
	int16_t by;
	int16_t cy;

	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	MAIN_func_800FD7D8(1, 4, bx + 8, by + 5);
	MAIN_func_800FD7D8(1, 5, bx + 0x26, by + 5);
	MAIN_func_800FD8D4(MAIN_D_80134F68);
	cy = by + MAIN_D_80134F68->cursor * 0x12 + 0x11;
draw:
	renderSelectionCursor(bx + 5, cy, 0xca, 0x12, 5);
	MAIN_func_800FDC5C(MAIN_D_80134F68, bx + 8, by + 0x13, 0, 0, 2);
}

void MAIN_func_801094F0(void)
{
	uint8_t item;

	if (MAIN_func_800FCF88(MAIN_D_80134F68) != 0) {
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

	if (MAIN_D_8013500C != 0) {
		return;
	}

	if (isKeyDown(0x40)) {
		item = MAIN_D_80134F68->buf[(MAIN_D_80134F68->topRow +
		                             MAIN_D_80134F68->cursor) *
		                            2];
		if ((item & 0x80) != 0) {
			showMapHeadTextbox(6, readPStat(PSTAT_254), 0, 0x4d6);
			MAIN_D_8013500C = MAIN_D_8013024C[item & 0x7f].cost;
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 4;
			SCRIPT_STATE_3 = 1;
			playSound(0, 3);
		} else {
			playSound(0, 0xb);
		}
	} else if (isKeyDown(0x10)) {
		SELECTION_MENU_STATE = 6;
		playSound(0, 4);
	} else if (isKeyDown(0x1000)) {
		if (POLLED_INPUT & 8) {
			MAIN_func_800FD3DC(MAIN_D_80134F68, 9, 3);
		} else {
			MAIN_func_800FD428(MAIN_D_80134F68, 3);
		}
	} else if (isKeyDown(0x4000)) {
		if (POLLED_INPUT & 8) {
			MAIN_func_800FD4E8(MAIN_D_80134F68, 9, 3);
		} else {
			MAIN_func_800FD534(MAIN_D_80134F68, 3);
		}
	}
}

void MAIN_func_801096E8(void)
{
	int16_t bx;
	int16_t by;
	int16_t cy;

	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	MAIN_func_800FD7D8(1, 6, bx + 8, by + 5);
	MAIN_func_800FD7D8(1, 2, bx + 0xa2, by + 5);
	MAIN_func_800FD8D4(MAIN_D_80134F68);
	cy = by + MAIN_D_80134F68->cursor * 0x12 + 0x11;
draw:
	renderSelectionCursor(bx + 5, cy, 0xca, 0x12, 5);
	MAIN_func_800FDC5C(MAIN_D_80134F68, bx + 8, by + 0x13, 0, 0, 2);
}

void MAIN_func_801097F4(void)
{
	uint8_t item;

	if (MAIN_func_800FCF88(MAIN_D_80134F68) != 0) {
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

	if (readPStat(PSTAT_249) != 0xff) {
		return;
	}

	if (isKeyDown(0x40)) {
		item = MAIN_D_80134F68->buf[(MAIN_D_80134F68->topRow +
		                             MAIN_D_80134F68->cursor) *
		                            2];
		if ((item & 0x80) != 0) {
			writePStat(PSTAT_249, (item & 0x7f));
			showMapHeadTextbox(0xb, readPStat(PSTAT_254), 0, 0x4d6);
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 4;
			SCRIPT_STATE_3 = 1;
			playSound(0, 3);
		} else {
			playSound(0, 0xb);
		}
	} else if (isKeyDown(0x10)) {
		SELECTION_MENU_STATE = 6;
		playSound(0, 4);
	} else if (isKeyDown(0x1000)) {
		if (POLLED_INPUT & 8) {
			MAIN_func_800FF310(MAIN_D_80134F68);
			MAIN_func_80109BBC();
		} else {
			MAIN_func_800FD428(MAIN_D_80134F68, 4);
		}
	} else if (isKeyDown(0x4000)) {
		if (POLLED_INPUT & 8) {
			MAIN_func_800FF2A8(MAIN_D_80134F68);
			MAIN_func_80109BBC();
		} else {
			MAIN_func_800FD534(MAIN_D_80134F68, 4);
		}
	}
}

void MAIN_func_801099E8(void)
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
	box = &MAIN_D_801BE80C.box[id];
	getVRAMModeCoords(box->vramMode, (int32_t *)&x, &clut);
	y = 0x6c;
	y += box->backPage * box->vramRows * 12;
	drawString(MAIN_D_801307CC, x, y);
	renderString(0, bx + 0x14, by + 5, 0x42, 0xc, x, y, 5, 1);
	renderString(0, bx + 0x7a, by + 5, 0x4e, 0xc, x + 0x42, y, 5, 1);
	y = by + MAIN_D_80134F68->cursor * 0x12 + 0x19;
draw:
	renderSelectionCursor(bx + 5, y, 0xd4, 0x12, 5);
	MAIN_func_800FDC5C(MAIN_D_80134F68, bx + 8, by + 0x1b, 0, 0, 2);
}

void MAIN_func_80109BBC(void)
{
	showMapHeadTextbox(0xf, 0xff, 1, 0x4d6);

	--TEXT_BOX_DATA[1].writeCount;
	TEXT_BUFFERS_PTR[0x254] = TEXT_NEWLINE;
	TEXT_BUFFERS_PTR[0x354] = TEXT_NEWLINE;

	MAIN_func_800FCCFC(MAIN_D_80134F68, 0xa, 4);
}

void setupNewGameDialogueBox(void)
{
	int32_t i;
	RECT rect2;
	RECT rect1;
	int16_t screenPos[2];

	MAIN_D_80134F98 = 0;
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
	showMapHeadTextbox(textId, 0xfe, 0, 0x4d6);

	SELECTION_MENU_STATE = 1;
	SCRIPT_STATE_4 = nextState;
	SCRIPT_STATE_3 = 1;
}

void showNewgameSelection(int32_t textId, int16_t nextState)
{
	int32_t sel = 0;

	showMapheadSelection(textId, 0xfe, 2, &sel, 0x4d6);

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

	MAIN_D_80134F98 = 1;
	if ((MAIN_D_80134F8E & 2) == 0) {
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
	MAIN_D_80134F8C = 0;
	MAIN_func_8010A79C();
}

void setupNameDisplayBox(void)
{
	int32_t i;
	int32_t flags;
	RECT rect2;
	RECT rect1;
	int16_t screenPos[2];

	if ((MAIN_D_80134F8E & 2) == 0) {
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

	MAIN_D_80134F82 = strlen(MAIN_D_801B1D1C) >> 1;
	if (MAIN_D_80134F82 == 6) {
		--MAIN_D_80134F82;
	}

	updateNamingPreview();
}

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

	if (MAIN_func_800FE650(1) != 0) {
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
	row = MAIN_D_80134F8C / 5;
	col = MAIN_D_80134F8C % 5;
	special = MAIN_D_80134F8C & 0x7fff;
	if (isKeyDown(0x80)) {
		MAIN_D_801B1D1C[0] = 0;
		MAIN_D_80134F82 = 0;
		updateNamingPreview();
		playSound(0, 3);
		return;
	}
	if (isKeyDown(0x40)) {
		if ((MAIN_D_80134F8C & 0x8000) == 0) {
			if (row < 9) {
				rows = MAIN_D_80130774[MAIN_D_80134F81 * 2];
			} else {
				row -= 9;
				rows = MAIN_D_80130774[MAIN_D_80134F81 * 2 + 1];
			}
			str = rows[row];
			col *= 2;
			hi = str[col + 0];
			lo = str[col + 1];
			if (hi == 0x81 && lo == 0x40 && MAIN_D_80134F82 == 0) {
				playSound(0, 0xb);
				return;
			}
			n = MAIN_D_80134F82;
			idx = n * 2;
			pos = idx;
			MAIN_D_801B1D1C[idx] = hi;
			MAIN_D_801B1D1C[pos + 1] = lo;
			MAIN_D_801B1D1C[pos + 2] = 0;
			if (n != 5) {
				MAIN_D_80134F82++;
			}
			updateNamingPreview();
			if (MAIN_D_801B1D26[0] != 0 && MAIN_D_80134F82 == 5) {
				MAIN_D_80134F8C = 0x8001;
			}
			playSound(0, 3);
			return;
		}
		if (special != 1) {
			if (special == 0) {
				namingDeleteLast();
			}
		} else {
			if (MAIN_D_801B1D1C[0] == 0) {
				playSound(0, 0xb);
				return;
			}
			terminateNamingBuffer();
			playSound(0, 3);
			SELECTION_MENU_STATE = 0x15;
		}
	} else if (isKeyDown(0x20)) {
		MAIN_D_80134F8C = 0x8000;
		playSound(0, 2);
	} else if (isKeyDown(0x10)) {
		namingDeleteLast();
	} else if (isKeyDown(0x800)) {
		MAIN_D_80134F8C = 0x8001;
		playSound(0, 2);
	} else if (isKeyDown(0x8000)) {
		namingSelectionLeft(col, row, (int16_t)special);
	} else if (isKeyDown(0x2000)) {
		namingSelectionRight(col, row, (int16_t)special);
	} else if (isKeyDown(0x1000)) {
		namingSelectionUp(col, row);
	} else if (isKeyDown(0x4000)) {
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
	lbl = MAIN_D_8013078C;
	for (j = 0; j < 3; j++, lbl += 3) {
		renderString(0, bx + lbl[0], by + lbl[1], lbl[2], 12, texX, texY, 5, 1);
		texX += lbl[2];
	}
	if (MAIN_D_801B1D1C[0] == 0) {
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

void MAIN_func_8010A79C(void)
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
	strcpy(buf, MAIN_D_801345F8);
	buf += strlen(MAIN_D_801345F8);
	*buf++ = TEXT_COLOR;
	*buf++ = TEXT_COLOR_WHITE;
	strcpy(buf, MAIN_D_80134600);
	buf += strlen(MAIN_D_80134600);
	*buf++ = TEXT_NEWLINE;
	*buf++ = 0;
	line += 0x40;
	page = MAIN_D_80134F81 * 2;
	for (row = 0; row < 2; row++) {
		table = MAIN_D_80130774[page + row];
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

void updateNamingPreview(void)
{
	uint8_t *out;
	int32_t len;

	out = TEXT_BUFFERS_PTR;
	*out++ = TEXT_COLOR;
	*out++ = TEXT_COLOR_WHITE;

	if ((MAIN_D_80134F8E & 1) == 0) {
		strcpy(out, MAIN_D_801345F4);
		out += strlen(MAIN_D_801345F4);
	} else {
		strcpy(out, MAIN_D_801345F4 + 1);
		out += strlen(((int32_t)MAIN_D_801345F4 + 1));
	}

	strcpy(out, MAIN_D_801B1D1C);
	len = strlen(MAIN_D_801B1D1C);
	out += len;
	out = padWithSpaces(out, 6, len);
	terminateString(out, 1);
	drawString2(TEXT_BUFFERS_PTR, 0, 0, 1);
}

void namingDeleteLast(void)
{
	if (MAIN_D_801B1D1C[0] != 0) {
		if (MAIN_D_80134F82 == 0) {
			MAIN_D_801B1D1C[0] = 0;
		} else {
			MAIN_D_80134F82--;
			MAIN_D_801B1D1C[MAIN_D_80134F82 * 2] = 0;
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
		if (MAIN_D_801B1D1C[scan] == 0) {
			break;
		}

		pos += 2;
		scan += 2;
	}

	lowPos = pos + 1;

	while (pos != 0) {
		uint8_t hiByte;
		uint8_t loByte;

		hiByte = MAIN_D_801B1D1C[pos];
		if (hiByte != 0x81) {
			break;
		}

		loByte = MAIN_D_801B1D1C[lowPos];
		if (loByte != 0x40) {
			break;
		}

		pos -= 2;
		lowPos -= 2;
	}

	MAIN_D_801B1D1C[pos + 2] = 0;
}

void namingSelectionLeft(int16_t col, int16_t row, int32_t specialIdx)
{
	if ((MAIN_D_80134F8C & 0x8000) == 0) {
		if (row < 9) {
			if (col == 0) {
				MAIN_D_80134F8C = MAIN_D_801307A0[row];
			} else {
				--col;
				MAIN_D_80134F8C = col + (row * 5);
			}
		} else {
			if (col == 0) {
				row -= 9;
				col = 4;
			} else {
				--col;
			}

			MAIN_D_80134F8C = col + (row * 5);
		}
	} else {
		MAIN_D_80134F8C = MAIN_D_80134608[specialIdx] + 0x31;
	}

	playSound(0, 2);
}

void namingSelectionRight(int16_t col, int16_t row, int32_t specialIdx)
{
	if ((MAIN_D_80134F8C & 0x8000) == 0) {
		if (row < 9) {
			if (col == 4) {
				row += 9;
				col = 0;
			} else {
				++col;
			}

			MAIN_D_80134F8C = col + (row * 5);
		} else if (col == 4) {
			MAIN_D_80134F8C = MAIN_D_801307A0[row - 9];
		} else {
			++col;
			MAIN_D_80134F8C = col + (row * 5);
		}
	} else {
		MAIN_D_80134F8C = MAIN_D_80134608[specialIdx];
	}

	playSound(0, 2);
}

void namingSelectionUp(int16_t column, int16_t row)
{
	if ((MAIN_D_80134F8C & 0x8000) == 0) {
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

		MAIN_D_80134F8C = column + row * 5;
	} else if (MAIN_D_80134F8C == 0x8000) {
		MAIN_D_80134F8C = 0x8001;
	} else {
		--MAIN_D_80134F8C;
	}

	playSound(0, 2);
}

void namingSelectionDown(int16_t column, int16_t row)
{
	if ((MAIN_D_80134F8C & 0x8000) == 0) {
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

		MAIN_D_80134F8C = column + row * 5;
	} else if (MAIN_D_80134F8C == 0x8001) {
		MAIN_D_80134F8C = 0x8000;
	} else {
		++MAIN_D_80134F8C;
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

	if ((MAIN_D_80134F8C & 0x8000) == 0) {
		sel = MAIN_D_80134F8C;
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
		sel = MAIN_D_80134F8C;
		idx = ((sel & 0x7fff) * 3) + 3;
		baseX = (bx + MAIN_D_8013078C[idx]) - 4;
		baseY = (by + (&MAIN_D_8013078C[1])[idx]) - 4;
	}

	tag = &ACTIVE_ORDERING_TABLE->org[5];

	for (i = 0; i < 8; i++) {
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		setUV4(prim, u0.data[i], v0.data[i], u1.data[i], v0.data[i], u0.data[i], v1.data[i], u1.data[i], v1.data[i]);

		if ((MAIN_D_80134F8C & 0x8000) == 0) {
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
	if ((MAIN_D_80134F8E & 1) == 0) {
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
			MAIN_D_80134FEC = 0;
			script = getScript(CURRENT_SCRIPT_ID);
			section = getScriptSection(script, 0xfe);
			if (section != 0) {
				CURRENT_SCRIPT_PTR = script;
				MAIN_D_80134FDC = section;
				longjmp(SCRIPT_JMP_BUF, 1);
			}
		} else if (type == 4) {
			if (entry.smth[1] != 0xff) {
				CURRENT_SCRIPT_PTR = getScript(CURRENT_SCRIPT_ID);
				MAIN_D_80134FDC = getScriptSection(CURRENT_SCRIPT_PTR, entry.smth[1]);
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
