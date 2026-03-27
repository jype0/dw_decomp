#include <string.h>
#include <libetc.h>
#include <libgpu.h>
#include <dw/clock.h>
#include <dw/dget.h>
#include <dw/entity.h>
#include <dw/pstat.h>
#include <dw/trigger.h>
#include <dw/ui.h>

#include "common.h"

#define TOURNAMENT_ARRAY_SIZE	30

extern uint8_t TOURNAMENT_DATA[];
extern char *TOURNAMENT_GRADES[];

extern uint8_t *TOURNAMENT_ARRAY;
extern uint8_t TOURNAMENT_SELECTED_COLUMN;
extern uint8_t TOURNAMENT_SELECTED_ROW;
extern char *TEXT_BUFFERS_PTR;

extern uint16_t LOADED_SCRIPT_ID;

extern uint16_t SELECTION_MENU_STATE;

extern int32_t MAIN_D_801353B0;
extern uint8_t MAIN_D_80134FE8;
extern uint8_t MAIN_D_80135008;
extern uint16_t MAIN_D_80135014;
extern uint8_t *MAIN_D_80134FDC;
extern uint8_t MAIN_D_80134FE8;

int32_t minutesOfDay(void);

uint8_t readPStat(uint32_t address);

void setupBoxOrigin(int32_t, RECT *);
void createTextbox(int32_t, int32_t, RECT *, RECT *, void *, void *);
void registerTextbox(int32_t, int32_t, int32_t, int32_t, int32_t);
void showMapHeadTextbox(int32_t, int32_t, int32_t, int32_t);
uint8_t *intToStringSJIS(uint8_t *, int32_t, int32_t, int32_t);
void terminateString(char *, int32_t);

void setTrigger(uint16_t trigger);
void unsetTrigger(uint16_t trigger);
void showMapheadSelection(int32_t, int32_t, int32_t, uint32_t *, int32_t);
void *allocateArray(uint32_t);
void freeArray(void *);

void setInputRepeatMask(uint32_t);

int32_t isTriggerSet(uint16_t trigger);
uint8_t* getScript(uint32_t scriptId);
uint8_t* getScriptSection(uint8_t* ptr, int32_t section);

uint8_t *getScriptJumpTableEntry(int32_t, int32_t);
uint8_t *readScriptJumpTableEntry(uint8_t *, int32_t);

int32_t isXPressedAfterDialogue(void);
int32_t isKeyDown(int32_t);
void triggerBoxCloseFlag(int32_t);
void playSound(int16_t, int16_t);
void writePStat(int32_t id, uint8_t value);

void drawString(const char *str, uint32_t x, uint32_t y);
void renderString(int32_t colorId,
		  int32_t posX, int32_t posY,
		  int16_t uvWidth, int16_t uvHeight,
		  uint32_t uvX, uint32_t uvY,
		  int32_t offset, int32_t hasShadow);

void renderVerticalLine(int32_t, int32_t, int32_t, int32_t);
void renderHorizontalLine(int32_t, int32_t, int32_t, int32_t);
void renderSelectionCursor(int16_t, int16_t, int16_t, int16_t, uint16_t);

void fillEnabledTournamentTable(void);
void buildScheduleLabels(void);
void buildScheduleEntries(void);
void initTournamentInfo(int32_t param_1);
int32_t tournamentCheckFair(uint8_t value);
int32_t isTournamentEnabled(uint8_t value);
int32_t tournamentCheckEligible(uint8_t value);
void renderTournamentTextbox(void);
void tickTournamentSchedule(void);
void renderTournamentSchedule(void);
void tickTournamentInfo(void);
void renderTournamentInfo(void);

static void *dget_functions[] = {
	initTournamentSchedule,
	renderTournamentInfo,
	tickTournamentInfo,
	renderTournamentSchedule,
	tickTournamentSchedule,
	renderTournamentTextbox,
	tournamentCheckEligible,
	isTournamentEnabled,
	tournamentCheckFair,
	initTournamentInfo,
	buildScheduleEntries,
	buildScheduleLabels,
	fillEnabledTournamentTable,
};

void fillEnabledTournamentTable(void)
{
	int32_t day;
	int32_t tournamentOffset;
	uint8_t *tournamentPtr;
	int32_t tournamentIndex;
	int32_t timeOfDay;
	uint8_t *tournamentData;
	uint8_t tournament;
	int32_t i;

	day = DAY;
	tournamentIndex = 0;
	tournamentOffset = 0;

	goto next;

	while (1) {
		tournamentData = &TOURNAMENT_DATA[(day % 30) * 6];
		tournamentPtr = &TOURNAMENT_ARRAY[tournamentOffset];
		for (i = 0; i < 6; ++i) {
			tournament = *tournamentData++;
			if (tournament == 0xff) {
				break;
			}
			if (isTournamentEnabled(tournament) != 0) {
				*tournamentPtr++ = tournament;
			}
		}

		++tournamentIndex;
		tournamentOffset += 6;
		++day;
next:
		if (tournamentIndex > 4) {
			int32_t timeOfDay =
				((uint32_t)minutesOfDay() <= 600U) ? 0U : 1U;
			for (; timeOfDay < 2; ++timeOfDay) {
				tournamentPtr = &TOURNAMENT_ARRAY[timeOfDay * 6];
				for (i = 0; i < 6; ++i, ++tournamentPtr) {
					tournament = *tournamentPtr;
					if (tournament == 0xff) {
						break;
					}
					if (tournamentCheckEligible(tournament) != 0) {
						*tournamentPtr |= 0x80;
					}
					if (tournamentCheckFair(tournament) != 0) {
						*tournamentPtr |= 0x40;
					}
				}
			}
			return;
		}
	}
}

void buildScheduleLabels(void)
{
	int32_t stat;
	uint8_t *str;
	int32_t dayOfMonth;
	int16_t i;
	int16_t day;
	RECT rect1;
	RECT rect2;

	stat = readPStat(PSTAT_254);
	setupBoxOrigin(stat, &rect2);

	rect1.x = -54;
	rect1.y = -98;
	rect1.w = 108;
	rect1.h = 20;
	createTextbox(1, 0xe1, &rect1, &rect2, 0, renderTournamentTextbox);
	registerTextbox(1, 8, 2, 0, 0);
	showMapHeadTextbox(1, 0xff, 1, 0x4d8);

	TEXT_BUFFERS_PTR[0x210] = 0xd;

	str = (uint8_t *)TEXT_BUFFERS_PTR + 0x240;
	*str++ = 0x01;
	*str++ = 0x01;
	day = DAY;
	for (i = 0; i < 5; ++i, ++day) {
		dayOfMonth = day % 30;
		str = (uint8_t *)intToStringSJIS(str, dayOfMonth + 1, 2, 0);
		if ((i == 0) && ((dayOfMonth + 1) < 10)) {
			*str++ = 0x81;
			*str++ = 0x40;
		}
	}

	terminateString((char *)str, 1);
}

void buildScheduleEntries(void)
{
	TextBoxData *textBox;
	uint8_t entry;
	int32_t stat;
	int32_t gradeLen;
	uint8_t *textPtr;
	char *grade;
	int32_t col;
	uint8_t *entryPtr;
	int32_t i;
	uint8_t *textStart;
	RECT rect1;
	RECT rect2;

	TOURNAMENT_SELECTED_ROW = 0;
	TOURNAMENT_SELECTED_COLUMN = 0;

	stat = readPStat(PSTAT_254);
	setupBoxOrigin(stat, &rect2);

	rect1.x = -0x82;
	rect1.y = -0x3e;
	rect1.w = 0x104;
	rect1.h = 0x7c;

	createTextbox(2, 0xe1, &rect1, &rect2, tickTournamentSchedule,
		      renderTournamentSchedule);
	registerTextbox(2, 10, 6, 0, 1);

	textPtr = (uint8_t *)TEXT_BUFFERS_PTR + 0x280;
	for (i = 0; i < 6; ++i) {
		textStart = textPtr;
		entryPtr = (uint8_t *)(TOURNAMENT_ARRAY + i);
		for (col = 0; col < 5; ++col, entryPtr += 6) {
			*textPtr++ = '\x01';
			entry = *entryPtr;
			if (entry != 0xff) {
				if ((entry & 0xc0) == 0) {
					*textPtr++ = '\x03';
				} else if ((entry & 0x40) != 0) {
					*textPtr++ = '\a';
				} else {
					*textPtr++ = '\x01';
				}
				entry &= 0x3f;
				grade = TOURNAMENT_GRADES[entry];
				gradeLen = strlen(grade);
				if (gradeLen == 2) {
					*textPtr++ = '\x0f';
					*textPtr++ = '\0';
					strcpy((char *)textPtr, grade);
					textPtr += gradeLen;
					*textPtr++ = '\x0f';
					*textPtr++ = '\0';
				} else {
					strcpy(textPtr, grade);
					textPtr += gradeLen;
				}
			} else {
				*textPtr++ = '\x01';
				*textPtr++ = '\x81';
				*textPtr++ = '\x40';
			}
		}
		if (i != 5) {
			terminateString((char *)textPtr, 0);
		} else {
			terminateString((char *)textPtr, 1);
		}
		textPtr = textStart + 0x40;
	}

	textBox = &TEXT_BOX_DATA[2];
	textBox->unk_0x15 = 1;
	++textBox->unk_0x11;
}

extern void MAIN_func_80101EF8(int32_t, int32_t);

INCLUDE_ASM("asm/dget/nonmatchings/dget", initTournamentInfo);

int32_t tournamentCheckFair(uint8_t value)
{
	uint8_t *jumpTable;
	uint8_t *sectionPtr;
	uint8_t *typePtr;
	uint8_t type;

	jumpTable = getScriptJumpTableEntry(10, value);
	sectionPtr = readScriptJumpTableEntry(jumpTable, 2);
	typePtr = (uint8_t *)(sectionPtr + 2);

	while ((type = *typePtr++) < 0xfe) {
		if (type == (PARTNER_ENTITY.digimonEntity.entity.type & 0xffU)) {
			return 1;
		}
	}

	return 0;
}

int32_t isTournamentEnabled(uint8_t tournament)
{
	uint8_t triggerCount;
	uint8_t *triggerPtr;
	uint8_t minWins;
	uint8_t *scriptEntry;
	uint32_t reqType;
	uint8_t minTriggers;
	uint8_t *reqSection;
	uint8_t *entrySection;
	int32_t triggered;
	int32_t trigIdx;
	uint8_t trigger;

	scriptEntry = getScriptJumpTableEntry(10, tournament);
	reqSection = (uint8_t *)readScriptJumpTableEntry(scriptEntry, 1) + 2;
	entrySection = (uint8_t *)readScriptJumpTableEntry(scriptEntry, 4);
	minTriggers = reqSection[0];
	minWins = reqSection[1];
	reqType = (uint32_t)reqSection[2];
	triggerPtr = entrySection + 2;

	if (minTriggers == 0) {
		return 1;
	}

	triggerCount = 0;
	trigger = *triggerPtr;
	if (trigger < 0xfeu) {
		while ((trigger = *triggerPtr++) < 0xfe) {
			triggered = isTriggerSet(trigger + TRIGGER_OGRE_FORTRESS_OPENED);
			if (triggered != 0) {
				++triggerCount;
			}
		}
	} else {
		for (trigIdx = TRIGGER_OGRE_FORTRESS_OPENED;
		     trigIdx < TRIGGER_WARUSEADRAMON_BEATEN;
		     ++trigIdx) {
			triggered = isTriggerSet(trigIdx);
			if (triggered != 0) {
				++triggerCount;
			}
		}
	}

	if (triggerCount < minTriggers) {
		return 0;
	}

	switch (reqType) {
	case 0:
		return 1;
	case 1:
		return isTriggerSet(TRIGGER_GRADE_A_CUP_WON);
	case 2:
		scriptEntry = (uint8_t *)getScript(LOADED_SCRIPT_ID);
		triggerPtr = (uint8_t *)getScriptSection(scriptEntry, 0xb) + 2;
		triggerCount = 0;
		while ((trigger = *triggerPtr++) < 0xfe) {
			triggered = isTriggerSet(trigger + TRIGGER_OGRE_FORTRESS_OPENED);
			if (triggered != 0) {
				++triggerCount;
			}
		}
		if (minWins <= triggerCount) {
			return 1;
		}
		break;
	default:
		triggerPtr = readScriptJumpTableEntry(scriptEntry, 4) + 2;
		triggerCount = 0;
		while ((trigger = *triggerPtr++) <= reqType) {
			triggered = isTriggerSet(trigger + TRIGGER_OGRE_FORTRESS_OPENED);
			if (triggered != 0) {
				++triggerCount;
			}
		}
		if (minWins <= triggerCount) {
			return 1;
		}
		break;
	}

	return 0;
}

int32_t tournamentCheckEligible(uint8_t tournament)
{
	uint8_t type;
	uint8_t *jumpTable;
	uint8_t *sectionPtr;
	uint8_t *typePtr;

	jumpTable = getScriptJumpTableEntry(10, tournament);
	sectionPtr = readScriptJumpTableEntry(jumpTable, 3);
	typePtr = (uint8_t *)(sectionPtr + 2);

	while ((type = *typePtr++) < 0xfe) {
		if (type == (PARTNER_ENTITY.digimonEntity.entity.type & 0xffU)) {
			return 1;
		}
	}

	return 0;
}

void renderTournamentTextbox(void)
{
	int16_t uvY;
	int16_t posX;
	int16_t posY;

	uvY = TEXT_BOX_DATA[1].unk_0x20 * 12;
	posX = UI_BOX_DATA[1].finalPos.x + 6;
	posY = UI_BOX_DATA[1].finalPos.y + 3;

	drawString("Tournament", 0, uvY);
	renderString(0, posX, posY, 0x54, 0xc, 0, uvY, 5, 1);
}

void tickTournamentSchedule(void)
{
	uint8_t entry;

	if ((UI_BOX_DATA[3].state != 0) ||
	    (UI_BOX_DATA[2].state != 1) ||
	    (isXPressedAfterDialogue() == 0)) {
		return;
	}

	if (isKeyDown(PADRup) != 0) {
		SELECTION_MENU_STATE = 3;
		playSound(0, 4);
		return;
	}

	if (isKeyDown(PADRdown) != 0) {
		entry = *(uint8_t *)(TOURNAMENT_ARRAY +
				     (uint32_t)TOURNAMENT_SELECTED_COLUMN * 6 +
				     (uint32_t)TOURNAMENT_SELECTED_ROW);
		if ((entry != 0xff) && ((entry & 0xc0) != 0)) {
			writePStat(PSTAT_TOURNAMENT_ID, entry & 0x3f);
			SELECTION_MENU_STATE = 3;
			playSound(0, 3);
		} else {
			playSound(0, 0xb);
		}
		return;
	}

	if (isKeyDown(PADLleft) != 0) {
		if (TOURNAMENT_SELECTED_COLUMN == 0) {
			TOURNAMENT_SELECTED_COLUMN = 4;
		}
		else {
			--TOURNAMENT_SELECTED_COLUMN;
		}
		playSound(0, 2);
		return;
	}

	if (isKeyDown(PADLright) != 0) {
		if (TOURNAMENT_SELECTED_COLUMN == 4) {
			TOURNAMENT_SELECTED_COLUMN = 0;
		}
		else {
			++TOURNAMENT_SELECTED_COLUMN;
		}
		playSound(0, 2);
		return;
	}

	if (isKeyDown(PADLup) != 0) {
		if (TOURNAMENT_SELECTED_ROW == 0) {
			TOURNAMENT_SELECTED_ROW = 5;
		}
		else {
			--TOURNAMENT_SELECTED_ROW;
		}
		playSound(0, 2);
		return;
	}

	if (isKeyDown(PADLdown) != 0) {
		if (TOURNAMENT_SELECTED_ROW == 5) {
			TOURNAMENT_SELECTED_ROW = 0;
		} else {
			++TOURNAMENT_SELECTED_ROW;
		}
		playSound(0, 2);
		return;
	}

	if (isKeyDown(PADstart) != 0) {
		if (*(uint8_t *)(TOURNAMENT_ARRAY +
				 (uint32_t)TOURNAMENT_SELECTED_COLUMN * 6 +
				 (uint32_t)TOURNAMENT_SELECTED_ROW) != 0xff) {
			initTournamentInfo(0);
			playSound(0, 3);
		} else {
			playSound(0, 0xb);
		}
		return;
	}
}

INCLUDE_ASM("asm/dget/nonmatchings/dget", renderTournamentSchedule);

void tickTournamentInfo(void)
{
	if ((MAIN_D_801353B0 == 0) &&
	    (UI_BOX_DATA[3].state == 1) &&
	    (isXPressedAfterDialogue() != 0) &&
	    (isKeyDown(0x850) != 0)) {
		triggerBoxCloseFlag(3);
		if (MAIN_D_801353B0 == 0) {
			playSound(0, 3);
		}
	}
}

void renderTournamentInfo(void)
{
	int16_t uvY;
	int16_t posX;
	int16_t posY;
	int32_t i;

	uvY = TEXT_BOX_DATA[3].unk_0x20 * 12;
	posX = UI_BOX_DATA[3].finalPos.x + 6;
	posY = UI_BOX_DATA[3].finalPos.y + 3;

	i = 0;
	while (i < 7) {
		renderString(0, posX, posY, 0xf0, 0xc, 0, uvY, 3, 1);
		i++;
		posY += 13;
		uvY += 12;
	}
}

void initTournamentSchedule(void)
{
	uint8_t value;
	int32_t triggered;
	uint32_t stat;
	uint32_t selectionResult;

	selectionResult = 0;

	switch (SELECTION_MENU_STATE) {
	case 0:
		triggered = isTriggerSet(TRIGGER_TOURNAMENT_REGISTERED);
		if (triggered != 0) {
			MAIN_D_80134FE8 = 0;
		} else {
			TOURNAMENT_ARRAY = allocateArray(TOURNAMENT_ARRAY_SIZE);
			memset(TOURNAMENT_ARRAY, 0xff, TOURNAMENT_ARRAY_SIZE);
			writePStat(PSTAT_TOURNAMENT_ID, 0xff);
			fillEnabledTournamentTable();
			triggerBoxCloseFlag(0);
			setInputRepeatMask(0xf000);
			buildScheduleLabels();
			buildScheduleEntries();
			SELECTION_MENU_STATE = 2;
		}
		break;
	case 1:
		freeArray(TOURNAMENT_ARRAY);
		MAIN_D_80134FE8 = 0;
		break;
	case 3:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(2);
		triggerBoxCloseFlag(1);

		value = readPStat(PSTAT_TOURNAMENT_ID);
		if (value == 0xff) {
			SELECTION_MENU_STATE = 1;
		}
		else {
			initTournamentInfo(1);
			triggered = tournamentCheckFair(value);
			if (triggered != 0) {
				stat = readPStat(PSTAT_254);
				showMapHeadTextbox(3, stat, 0, 0x4d8);
				setTrigger(TRIGGER_TOURNAMENT_OVERLEVELED);
			} else {
				stat = readPStat(PSTAT_254);
				showMapHeadTextbox(2, stat, 0, 0x4d8);
				unsetTrigger(TRIGGER_TOURNAMENT_OVERLEVELED);
			}
			SELECTION_MENU_STATE = 2;
			MAIN_D_80135014 = 4;
			MAIN_D_80135008 = 1;
		}
		break;
	case 4:
		showMapheadSelection(4, 0xfd, 2, &selectionResult, 0x4d8);
		SELECTION_MENU_STATE = 2;
		MAIN_D_80135014 = 5;
		MAIN_D_80135008 = 2;
		break;
	case 5:
		triggerBoxCloseFlag(3);
		setTrigger(TRIGGER_TOURNAMENT_REGISTERED);
		setTrigger(TRIGGER_38);
		setTrigger(TRIGGER_39);
		value = DAY;
		if (TOURNAMENT_SELECTED_COLUMN != '\0') {
			value = (value + 1) | 0x80;
		}
		writePStat(PSTAT_TOURNAMENT_DAY, value);
		writePStat(PSTAT_TOURNAMENT_DIGIMON,
			   PARTNER_ENTITY.digimonEntity.entity.type);
		SELECTION_MENU_STATE = 1;
		break;
	case 6:
		triggerBoxCloseFlag(3);
		SELECTION_MENU_STATE = 1;
	default:
		break;
	}
}
