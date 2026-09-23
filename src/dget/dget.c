#include <string.h>

#include <libetc.h>
#include <libgpu.h>

#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/font.h>
#include <dw/pstat.h>
#include <dw/tournament.h>
#include <dw/trigger.h>
#include <dw/ui.h>

extern char *TEXT_BUFFERS_PTR;

extern uint16_t ACTIVE_MAP_SCRIPT;

extern uint16_t SELECTION_MENU_STATE;

extern uint8_t ACTIVE_INSTRUCTION;
extern uint8_t SCRIPT_STATE_3;
extern uint16_t SCRIPT_STATE_4;
extern uint8_t *MAIN_D_80134FDC;
extern uint8_t ACTIVE_INSTRUCTION;

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

int32_t isXPressedAfterDialogue(void);
int32_t isKeyDown(int32_t);
void triggerBoxCloseFlag(int32_t);
void playSound(int16_t, int16_t);
void writePStat(int32_t id, uint8_t value);

void renderString(int32_t colorId,
		  int32_t posX, int32_t posY,
		  int16_t uvWidth, int16_t uvHeight,
		  uint32_t uvX, uint32_t uvY,
		  int32_t offset, int32_t hasShadow);

void renderVerticalLine(int32_t, int32_t, int32_t, int32_t);
void renderHorizontalLine(int32_t, int32_t, int32_t, int32_t);
void renderSelectionCursor(int16_t, int16_t, int16_t, int16_t, uint16_t);

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

uint8_t *TOURNAMENT_ARRAY;
uint8_t TOURNAMENT_SELECTED_COLUMN;
uint8_t TOURNAMENT_SELECTED_ROW;
int32_t MAIN_D_801353B0;

static void *dget_sbss_order[] = {
	&MAIN_D_801353B0,
	&TOURNAMENT_SELECTED_ROW,
	&TOURNAMENT_SELECTED_COLUMN,
	&TOURNAMENT_ARRAY,
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

	setRECT(&rect1, -54, -98, 108, 20);
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

	setRECT(&rect1, -0x82, -0x3e, 0x104, 0x7c);

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
	textBox->pageReady = 1;
	++textBox->writeCount;
}

extern void MAIN_func_80101EF8(int32_t, int32_t);

static void initTournamentInfo__garbage__(void)
{
	int32_t v0;
	int32_t v1;
	int32_t v2;
	int32_t v3;
	int32_t v4;
	int32_t v5;
	int32_t v6;
	int32_t v7;

	v0 = TOURNAMENT_ARRAY[0] + 0;
	v1 = TOURNAMENT_ARRAY[1] + 1;
	v2 = TOURNAMENT_ARRAY[2] + 2;
	v3 = TOURNAMENT_ARRAY[0] + 3;
	v4 = TOURNAMENT_ARRAY[1] + 4;
	v5 = TOURNAMENT_ARRAY[2] + 5;
	v6 = TOURNAMENT_ARRAY[0] + 6;
	v7 = TOURNAMENT_ARRAY[1] + 7;
	TOURNAMENT_ARRAY[0] = (uint8_t)((v0 * v1) + v2);
	TOURNAMENT_ARRAY[1] = (uint8_t)((v1 * v2) + v3);
	TOURNAMENT_ARRAY[2] = (uint8_t)((v2 * v3) + v4);
	TOURNAMENT_ARRAY[0] = (uint8_t)((v3 * v4) + v5);
	TOURNAMENT_ARRAY[1] = (uint8_t)((v4 * v5) + v6);
	TOURNAMENT_ARRAY[2] = (uint8_t)((v5 * v6) + v7);
	TOURNAMENT_ARRAY[0] = (uint8_t)((v6 * v7) + v0);
	TOURNAMENT_ARRAY[1] = (uint8_t)((v7 * v0) + v1);
}
void initTournamentInfo(int32_t arg)
{
	uint8_t *saved;
	uint8_t *jumpTable;
	uint8_t *entryPtr;
	uint8_t entry;
	int32_t boxId;
	int32_t slot;
	int32_t stat;
	int16_t yOff;
	int16_t x;
	int16_t y;
	RECT rect1;
	RECT rect2;

	MAIN_D_801353B0 = arg;
	if (arg != 0) {
		boxId = 0xe1;
		stat = readPStat(PSTAT_254);
		setupBoxOrigin(stat, &rect2);
		yOff = -0x4f;
		slot = 8;
	} else {
		x = UI_BOX_DATA[2].finalPos.x + 4;
		y = UI_BOX_DATA[2].finalPos.y + 3;
		x = x + TOURNAMENT_SELECTED_COLUMN * 51 + 3;
		y = y + TOURNAMENT_SELECTED_ROW * 16 + 0x16;
		setRECT(&rect2, x, y, 0x2a, 0xd);
		boxId = 0xc1;
		yOff = -0x31;
		slot = 0;

	}
	setRECT(&rect1, -0x7e, yOff, 0xfc, 0x63);
	createTextbox(3, boxId, &rect1, &rect2, tickTournamentInfo,
		      renderTournamentInfo);
	registerTextbox(3, slot, 7, 0, 0);

	saved = MAIN_D_80134FDC;
	entryPtr = TOURNAMENT_ARRAY + TOURNAMENT_SELECTED_COLUMN * 6;
	entry = entryPtr[TOURNAMENT_SELECTED_ROW];
	entry &= 0x3f;
	jumpTable = getCupDataJumpTable(10, entry);
	MAIN_D_80134FDC = getCupDataJumpTableEntry(jumpTable, 0) + 2;
	MAIN_func_80101EF8(3, 0xff);
	ACTIVE_INSTRUCTION = 0x64;
	MAIN_D_80134FDC = saved;
}

int32_t tournamentCheckFair(uint8_t value)
{
	uint8_t *jumpTable;
	uint8_t *sectionPtr;
	uint8_t *typePtr;
	uint8_t type;

	jumpTable = getCupDataJumpTable(10, value);
	sectionPtr = getCupDataJumpTableEntry(jumpTable, 2);
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

	scriptEntry = getCupDataJumpTable(10, tournament);
	reqSection = (uint8_t *)getCupDataJumpTableEntry(scriptEntry, 1) + 2;
	entrySection = (uint8_t *)getCupDataJumpTableEntry(scriptEntry, 4);
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
		scriptEntry = (uint8_t *)getScript(ACTIVE_MAP_SCRIPT);
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
		triggerPtr = getCupDataJumpTableEntry(scriptEntry, 4) + 2;
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

	jumpTable = getCupDataJumpTable(10, tournament);
	sectionPtr = getCupDataJumpTableEntry(jumpTable, 3);
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

	uvY = TEXT_BOX_DATA[1].vramRow * 12;
	posX = UI_BOX_DATA[1].finalPos.x + 6;
	posY = UI_BOX_DATA[1].finalPos.y + 3;

	drawString("Tournament", 0, (uint32_t)uvY);
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

void renderTournamentSchedule(void)
{
	int16_t x;
	int16_t y;
	int16_t sx;
	int16_t sy;
	int16_t textOff;
	int16_t cellOff;
	int16_t cellY;
	int32_t gridX;
	int32_t i;
	int32_t row;

	x = UI_BOX_DATA[2].finalPos.x + 4;
	y = UI_BOX_DATA[2].finalPos.y + 3;
	i = 0;
	sx = 0x34;
	while (i < 5) {
		renderVerticalLine(2, (uint32_t)sx, 2, 0x13);
		renderVerticalLine(2, sx, 0x16, 0x64);
		i++;
		sx += 0x33;
	}
	renderHorizontalLine(2, 3, 0x14, 0xfe);

	sx = x + TOURNAMENT_SELECTED_COLUMN * 51 + 3;
	sy = y + TOURNAMENT_SELECTED_ROW * 16 + 0x16;
	renderSelectionCursor(sx, sy, 0x2a, 0xd, 4);

	sx = x + 0xc;
	sy = y + 2;
	textOff = 0x6c;
	renderString(0, sx, sy, 0x10, 0xc, 0, textOff, 4, 1);
	renderString(0, sx + 0x33, sy, 0x10, 0xc, 0x10, textOff, 4, 1);
	renderString(0, sx + 0x66, sy, 0x10, 0xc, 0x20, textOff, 4, 1);
	renderString(0, sx + 0x99, sy, 0x10, 0xc, 0x30, textOff, 4, 1);
	renderString(0, sx + 0xcc, sy, 0x10, 0xc, 0x40, textOff, 4, 1);

	textOff += 0xc;
	sy = y + 0x18;
	row = 0;
	gridX = x + 0x14;
	for (; row < 6; row++, sy += 0x10, textOff += 0xc) {
		sx = gridX;
		cellY = sy;
		i = 0;
		cellOff = 0;
		for (; i < 5; i++, sx += 0x33, cellOff += 0xc) {

			renderString(0, sx, cellY, 0xc, 0xc, cellOff, textOff, 4, 1);
		}
	}
}

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

	uvY = TEXT_BOX_DATA[3].vramRow * 12;
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
			ACTIVE_INSTRUCTION = 0;
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
		ACTIVE_INSTRUCTION = 0;
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
			SCRIPT_STATE_4 = 4;
			SCRIPT_STATE_3 = 1;
		}
		break;
	case 4:
		showMapheadSelection(4, 0xfd, 2, &selectionResult, 0x4d8);
		SELECTION_MENU_STATE = 2;
		SCRIPT_STATE_4 = 5;
		SCRIPT_STATE_3 = 2;
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
