#include <string.h>

#include <libetc.h>
#include <libgpu.h>

#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/font.h>
#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/pstat.h>
#include <dw/script.h>
#include <dw/tournament.h>
#include <dw/trigger.h>
#include <dw/ui.h>


extern uint16_t ACTIVE_MAP_SCRIPT;

extern uint16_t SCRIPT_STATE_2;

extern uint8_t ACTIVE_INSTRUCTION;
extern uint8_t SCRIPT_TEXTBOX_MODE;
extern uint16_t SCRIPT_NEXT_STATE_2;
extern uint8_t *SCRIPT_POINTER;
extern uint8_t ACTIVE_INSTRUCTION;


void createTextbox(int32_t, uint8_t, RECT *, RECT *, void *, void *);
void registerTextbox(int32_t, int32_t, int32_t, int32_t, int32_t);
void showMapHeadTextbox(int32_t, int32_t, int32_t, int32_t);
uint8_t *intToStringSJIS(uint8_t *, int32_t, int32_t, int32_t);
void terminateString(char *, int32_t);

void setTrigger(uint16_t trigger);
void unsetTrigger(uint16_t trigger);
void *allocateArray(uint32_t);
void freeArray(void *);

void setInputRepeatMask(uint32_t);

int32_t isTriggerSet(uint16_t trigger);
uint8_t* getScriptSection(uint8_t* ptr, int32_t section);

int32_t isXPressedAfterDialogue(void);
int32_t isKeyDown(int32_t);
void triggerBoxCloseFlag(int32_t);
void playSound(int16_t, int16_t);

void renderString(int32_t colorId,
		  int32_t posX, int32_t posY,
		  int16_t uvWidth, int16_t uvHeight,
		  uint32_t uvX, uint32_t uvY,
		  int32_t offset, int32_t hasShadow);

void renderVerticalLine(uint8_t boxId, int16_t x, int16_t y, int16_t h);
void renderHorizontalLine(uint8_t boxId, int16_t x, int16_t y, int16_t w);
void renderSelectionCursor(int32_t x, int32_t y, int16_t w, int16_t h, int32_t layer);

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

uint8_t *DGET_BUFFER;
uint8_t TOURNAMENT_SELECTED_COLUMN;
uint8_t TOURNAMENT_SELECTED_ROW;
int32_t MAIN_D_801353B0;

static void *dget_sbss_order[] = {
	&MAIN_D_801353B0,
	&TOURNAMENT_SELECTED_ROW,
	&TOURNAMENT_SELECTED_COLUMN,
	&DGET_BUFFER,
};

void fillEnabledTournamentTable(void)
{
	int32_t day;
	uint8_t *data;
	int32_t j;
	uint8_t *ptr;
	int32_t i;
	uint8_t tournament;

	day = DAY;
	for (j = 0; j < 5; j++, day++) {
		data = &TOURNAMENT_SCHEDULE[(day % 30) * 6];
		ptr = &DGET_BUFFER[j * 6];
		for (i = 0; i < 6; i++) {
			tournament = *data++;
			if (tournament == 0xff) {
				break;
			}
			if (isTournamentEnabled(tournament) != 0) {
				*ptr++ = tournament;
			}
		}
	}

	if (minutesOfDay() <= 600) {
		j = 0;
	} else {
		j = 1;
	}
	for (; j < 2; j++) {
		ptr = &DGET_BUFFER[j * 6];
		for (i = 0; i < 6; i++, ptr++) {
			tournament = *ptr;
			if (tournament == 0xff) {
				break;
			}
			if (tournamentCheckEligible(tournament) != 0) {
				*ptr |= 0x80;
			}
			if (tournamentCheckFair(tournament) != 0) {
				*ptr |= 0x40;
			}
		}
	}
}

void buildScheduleLabels(void)
{
	uint8_t *str;
	int16_t i;
	int16_t day;
	RECT rect1;
	RECT rect2;
	uint8_t color;
#if !defined(VERSION_JP)
	int32_t dayOfMonth;
#endif

	color = 0xe1;
	setupBoxOrigin(readPStat(PSTAT_254), &rect2);

	setRECT(&rect1, -54, -98, 108, 20);
	createTextbox(1, color, &rect1, &rect2, 0, renderTournamentTextbox);
	registerTextbox(1, 8, 2, 0, 0);
	showMapHeadTextbox(1, 0xff, 1, 0x4d8);

	TEXTBOX_LINES_PTR[0x210] = 0xd;

	str = (uint8_t *)TEXTBOX_LINES_PTR + 0x240;
	*str++ = 0x01;
	*str++ = 0x01;
	day = DAY;
	for (i = 0; i < 5; ++i, ++day) {
#if defined(VERSION_JP)
		str = (uint8_t *)intToStringSJIS(str, day % 30 + 1, 2, 0);
		*str++ = 0x93;
		*str++ = 0xfa;
#else
		dayOfMonth = day % 30;
		str = (uint8_t *)intToStringSJIS(str, dayOfMonth + 1, 2, 0);
		if ((i == 0) && ((dayOfMonth + 1) < 10)) {
			*str++ = 0x81;
			*str++ = 0x40;
		}
#endif
	}

	terminateString((char *)str, 1);
}

void buildScheduleEntries(void)
{
	TextBoxData *textBox;
	RECT rect1;
	RECT rect2;
	int32_t col;
	int32_t gradeLen;
	uint8_t *textStart;
	uint8_t *entryPtr;
	char *grade;
	uint8_t color;
	uint8_t *textPtr;
	uint8_t entry;
	int32_t i;

	color = 0xe1;
	TOURNAMENT_SELECTED_COLUMN = TOURNAMENT_SELECTED_ROW = 0;

	setupBoxOrigin(readPStat(PSTAT_254), &rect2);

	setRECT(&rect1, -0x82, -0x3e, 0x104, 0x7c);

	createTextbox(2, color, &rect1, &rect2, tickTournamentSchedule,
		      renderTournamentSchedule);
	registerTextbox(2, 10, 6, 0, 1);

	textPtr = (uint8_t *)TEXTBOX_LINES_PTR + 0x280;
	for (i = 0; i < 6; ++i) {
		textStart = textPtr;
		entryPtr = (uint8_t *)(DGET_BUFFER + i);
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
#if defined(VERSION_JP)
				*textPtr++ = '\x81';
				*textPtr++ = '\x40';
#endif
			}
		}
		if (i != 5) {
			terminateString((char *)textPtr, 0);
		} else {
			terminateString((char *)textPtr, 1);
		}
		textPtr = textStart + 0x40;
	}

	textBox = &TEXTBOX_DATA[2];
	textBox->pageReady = 1;
	++textBox->writeCount;
}

extern void showTextboxReady(int32_t, int32_t);

GARBAGE_ARRAY(initTournamentInfo, DGET_BUFFER, 3, 9);

void initTournamentInfo(int32_t arg)
{
	RECT rect1;
	RECT rect2;
	uint8_t *saved;
	uint8_t *jumpTable;
	int16_t yOff;
	uint8_t color;
	uint8_t slot;
	int16_t x;
	int16_t y;
	uint8_t entry;

	MAIN_D_801353B0 = arg;
	if (arg != 0) {
		color = 0xe1;
		setupBoxOrigin(readPStat(PSTAT_254), &rect2);
		yOff = -0x4f;
		slot = 8;
	} else {
		color = 0xc1;
		x = UI_BOX_DATA[2].finalPos.x + 4;
		y = UI_BOX_DATA[2].finalPos.y + 3;
		x = x + TOURNAMENT_SELECTED_COLUMN * 51 + 3;
		y = y + TOURNAMENT_SELECTED_ROW * 16 + 0x16;
		setRECT(&rect2, x, y, 0x2a, 0xd);
		yOff = -0x31;
		slot = 0;
	}
	setRECT(&rect1, -0x7e, yOff, 0xfc, 0x63);
	createTextbox(3, color, &rect1, &rect2, tickTournamentInfo,
		      renderTournamentInfo);
	registerTextbox(3, slot, 7, 0, 0);

	entry = *(uint8_t *)(DGET_BUFFER +
			     (uint32_t)TOURNAMENT_SELECTED_COLUMN * 6 +
			     (uint32_t)TOURNAMENT_SELECTED_ROW);
	entry &= 0x3f;
	saved = SCRIPT_POINTER;
	jumpTable = getCupDataJumpTable(10, entry);
	SCRIPT_POINTER = getCupDataJumpTableEntry(jumpTable, 0) + 2;
	showTextboxReady(3, 0xff);
	ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
	SCRIPT_POINTER = saved;
}

int32_t tournamentCheckFair(uint8_t value)
{
	uint8_t *jumpTable;
	uint8_t *typePtr;
	uint8_t type;
	uint8_t partnerType;

	jumpTable = getCupDataJumpTable(10, value);
	typePtr = getCupDataJumpTableEntry(jumpTable, 2) + 2;
	partnerType = PARTNER_ENTITY.digimonEntity.entity.type;
	while ((type = *typePtr++) < 0xfe) {
		if (type == partnerType) {
			return 1;
		}
	}

	return 0;
}

int32_t isTournamentEnabled(uint8_t tournament)
{
	uint8_t trigger;
	int32_t trigIdx;
	uint8_t *scriptEntry;
	uint8_t *reqSection;
	uint8_t reqType;
	uint8_t minTriggers;
	uint8_t minWins;
	uint8_t triggerCount;
	uint8_t *triggerPtr;

	scriptEntry = getCupDataJumpTable(10, tournament);
	reqSection = getCupDataJumpTableEntry(scriptEntry, 1) + 2;
	triggerPtr = getCupDataJumpTableEntry(scriptEntry, 4) + 2;
	minTriggers = reqSection[0];
	minWins = reqSection[1];
	reqType = reqSection[2];

	if (minTriggers == 0) {
		return 1;
	}

	triggerCount = 0;
	trigger = *triggerPtr;
	if (trigger < 0xfe) {
		while ((trigger = *triggerPtr++) < 0xfe) {
			if (isTriggerSet(trigger + TRIGGER_OGRE_FORTRESS_OPENED)) {
				triggerCount++;
			}
		}
	} else {
		for (trigIdx = TRIGGER_OGRE_FORTRESS_OPENED;
		     trigIdx < TRIGGER_WARUSEADRAMON_BEATEN; trigIdx++) {
			if (isTriggerSet(trigIdx)) {
				triggerCount++;
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
		scriptEntry = getScript(ACTIVE_MAP_SCRIPT);
		triggerPtr = getScriptSection(scriptEntry, 0xb) + 2;
		triggerCount = 0;
		while ((trigger = *triggerPtr++) < 0xfe) {
			if (isTriggerSet(trigger + TRIGGER_OGRE_FORTRESS_OPENED)) {
				triggerCount++;
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
			if (isTriggerSet(trigger + TRIGGER_OGRE_FORTRESS_OPENED)) {
				triggerCount++;
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
	uint8_t *jumpTable;
	uint8_t *typePtr;
	uint8_t type;
	uint8_t partnerType;

	jumpTable = getCupDataJumpTable(10, tournament);
	typePtr = getCupDataJumpTableEntry(jumpTable, 3) + 2;
	partnerType = PARTNER_ENTITY.digimonEntity.entity.type;
	while ((type = *typePtr++) < 0xfe) {
		if (type == partnerType) {
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

	uvY = TEXTBOX_DATA[1].vramRow * 12;
	posX = UI_BOX_DATA[1].finalPos.x + 6;
	posY = UI_BOX_DATA[1].finalPos.y + 3;

#if defined(VERSION_JP)
	renderString(0, posX, posY, 0x60, 0xc, 0, uvY, 5, 1);
#else
	drawString("Tournament", 0, (uint32_t)uvY);
	renderString(0, posX, posY, 0x54, 0xc, 0, uvY, 5, 1);
#endif
}

void tickTournamentSchedule(void)
{
	uint8_t entry;

	if ((UI_BOX_DATA[3].state != 0) ||
	    (UI_BOX_DATA[2].state != 1) ||
	    (isXPressedAfterDialogue() == 0)) {
		return;
	}

	if (isKeyDown(CANCEL_BUTTON) != 0) {
		SCRIPT_STATE_2 = 3;
		playSound(0, 4);
		return;
	}

	if (isKeyDown(CONFIRM_BUTTON) != 0) {
		entry = *(uint8_t *)(DGET_BUFFER +
				     (uint32_t)TOURNAMENT_SELECTED_COLUMN * 6 +
				     (uint32_t)TOURNAMENT_SELECTED_ROW);
		if ((entry != 0xff) && ((entry & 0xc0) != 0)) {
			writePStat(PSTAT_TOURNAMENT_ID, entry & 0x3f);
			SCRIPT_STATE_2 = 3;
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
		entry = *(uint8_t *)(DGET_BUFFER +
				     (uint32_t)TOURNAMENT_SELECTED_COLUMN * 6 +
				     (uint32_t)TOURNAMENT_SELECTED_ROW);
		if (entry != 0xff) {
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
	int32_t row;
	int16_t textOff;
	int16_t x;
	int16_t y;
	int16_t cellOff;
	int16_t sy;
	int16_t sx;
	int32_t i;

	x = UI_BOX_DATA[2].finalPos.x + 4;
	y = UI_BOX_DATA[2].finalPos.y + 3;
	for (i = 0, sx = 0x34; i < 5; i++, sx += 0x33) {
		renderVerticalLine(2, sx, 2, 0x13);
		renderVerticalLine(2, sx, 0x16, 0x64);
	}
	renderHorizontalLine(2, 3, 0x14, 0xfe);

	sx = x + TOURNAMENT_SELECTED_COLUMN * 51 + 3;
	sy = y + TOURNAMENT_SELECTED_ROW * 16 + 0x16;
	renderSelectionCursor(sx, sy, 0x2a, 0xd, 4);

#if defined(VERSION_JP)
	cellOff = 0;
	textOff = 0x6c;
	sx = x + 6;
	sy = y + 2;
	for (i = 0; i < 5; i++, sx += 0x33, cellOff += 0x24) {
		renderString(0, sx, sy, 0x24, 0xc, cellOff, textOff, 4, 1);
	}
#else
	sx = x + 0xc;
	sy = y + 2;
	textOff = 0x6c;
	renderString(0, sx, sy, 0x10, 0xc, 0, textOff, 4, 1);
	renderString(0, sx + 0x33, sy, 0x10, 0xc, 0x10, textOff, 4, 1);
	renderString(0, sx + 0x66, sy, 0x10, 0xc, 0x20, textOff, 4, 1);
	renderString(0, sx + 0x99, sy, 0x10, 0xc, 0x30, textOff, 4, 1);
	renderString(0, sx + 0xcc, sy, 0x10, 0xc, 0x40, textOff, 4, 1);
#endif

	textOff += 0xc;
#if defined(VERSION_JP)
	sy = y + 0x16;
#else
	sy = y + 0x18;
#endif
	for (row = 0; row < 6; row++, sy += 0x10, textOff += 0xc) {
#if defined(VERSION_JP)
		sx = x + 0xc;
		for (i = 0, cellOff = 0; i < 5; i++, sx += 0x33, cellOff += 0x18) {
			renderString(0, sx, sy, 0x18, 0xc, cellOff, textOff, 4, 1);
		}
#else
		sx = x + 0x14;
		for (i = 0, cellOff = 0; i < 5; i++, sx += 0x33, cellOff += 0xc) {
			renderString(0, sx, sy, 0xc, 0xc, cellOff, textOff, 4, 1);
		}
#endif
	}
}

void tickTournamentInfo(void)
{
	if ((MAIN_D_801353B0 == 0) &&
	    (UI_BOX_DATA[3].state == 1) &&
	    (isXPressedAfterDialogue() != 0) &&
	    (isKeyDown(PADstart | CONFIRM_BUTTON | CANCEL_BUTTON) != 0)) {
		triggerBoxCloseFlag(3);
		if (MAIN_D_801353B0 == 0) {
			playSound(0, 3);
		}
	}
}

void renderTournamentInfo(void)
{
	int32_t i;
	int16_t uvY;
	int16_t posX;
	int16_t posY;

	uvY = TEXTBOX_DATA[3].vramRow * 12;
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
	uint32_t selectionResult;
	uint8_t value;

	selectionResult = 0;

	switch (SCRIPT_STATE_2) {
	case 0:
		if (isTriggerSet(TRIGGER_TOURNAMENT_REGISTERED) != 0) {
			ACTIVE_INSTRUCTION = 0;
		} else {
			DGET_BUFFER = allocateArray(TOURNAMENT_ARRAY_SIZE);
			memset(DGET_BUFFER, 0xff, TOURNAMENT_ARRAY_SIZE);
			writePStat(PSTAT_TOURNAMENT_ID, 0xff);
			fillEnabledTournamentTable();
			triggerBoxCloseFlag(0);
			setInputRepeatMask(0xf000);
			buildScheduleLabels();
			buildScheduleEntries();
			SCRIPT_STATE_2 = 2;
		}
		break;
	case 1:
		freeArray(DGET_BUFFER);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 2:
		break;
	case 3:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(2);
		triggerBoxCloseFlag(1);

		value = readPStat(PSTAT_TOURNAMENT_ID);
		if (value == 0xff) {
			SCRIPT_STATE_2 = 1;
		} else {
			initTournamentInfo(1);
			if (tournamentCheckFair(value) != 0) {
				showMapHeadTextbox(3, readPStat(PSTAT_254), 0, 0x4d8);
				setTrigger(TRIGGER_TOURNAMENT_OVERLEVELED);
			} else {
				showMapHeadTextbox(2, readPStat(PSTAT_254), 0, 0x4d8);
				unsetTrigger(TRIGGER_TOURNAMENT_OVERLEVELED);
			}
			SCRIPT_STATE_2 = 2;
			SCRIPT_NEXT_STATE_2 = 4;
			SCRIPT_TEXTBOX_MODE = 1;
		}
		break;
	case 4:
		showMapheadSelection(4, 0xfd, 2, (int32_t *)&selectionResult, 0x4d8);
		SCRIPT_STATE_2 = 2;
		SCRIPT_NEXT_STATE_2 = 5;
		SCRIPT_TEXTBOX_MODE = 2;
		break;
	case 5:
		triggerBoxCloseFlag(3);
		setTrigger(TRIGGER_TOURNAMENT_REGISTERED);
		setTrigger(TRIGGER_38);
		setTrigger(TRIGGER_39);
		value = DAY;
		if (TOURNAMENT_SELECTED_COLUMN != 0) {
			value = (value + 1) | 0x80;
		}
		writePStat(PSTAT_TOURNAMENT_DAY, value);
		writePStat(PSTAT_TOURNAMENT_DIGIMON,
			   PARTNER_ENTITY.digimonEntity.entity.type);
		SCRIPT_STATE_2 = 1;
		break;
	case 6:
		triggerBoxCloseFlag(3);
		SCRIPT_STATE_2 = 1;
	default:
		break;
	}
}
