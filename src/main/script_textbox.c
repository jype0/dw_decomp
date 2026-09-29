#include <string.h>

#include <libgs.h>

#include <dw/font.h>
#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/item.h>
#include <dw/params.h>
#include <dw/script.h>
#include <dw/sjis.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/ui.h>

typedef struct {
	uint32_t usedRows;
	TextBoxData box[6];
} TextBoxTable;

extern TextBoxTable MAIN_D_801BE80C;
extern uint8_t TEXTBOX_OPEN_TIMER;
extern int32_t MAIN_D_80134F94;
extern uint16_t MAIN_D_801BE952[];
extern uint16_t MAIN_D_801BE954[];
extern uint16_t MAIN_D_801BE956[];
extern uint16_t MAIN_D_801BE950[];
extern int32_t MAIN_D_801BE948[];
extern int32_t MAIN_D_801BE94C[];
extern GsOT *ACTIVE_ORDERING_TABLE;
extern char *MOVE_NAMES[];
extern int8_t MAIN_D_80134F98;
extern int32_t CURRENT_SCRIPT_PTR;
extern char *BGM_TRACK_NAMES[];
extern char *TOURNAMENT_NAMES[];

void renderSelectionCursor(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t xPos, int32_t yPos, int32_t width, int32_t height);
void renderString(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8);
int32_t flipIdleTextboxPage(int32_t boxId);
void closeTextbox(int32_t boxId, RECT *target);
void renderCustomSizedTextbox(void);
int32_t isKeyDown(uint32_t key);
void setFreshDialogue(void);
int32_t isXPressedAfterDialogue(void);
void clearTextboxLineCount(int32_t boxId);
int32_t tickConfirmDialogue(void);
void renderDialogueSelectionCursor(int32_t x, int32_t y);
void tickCustomSizedTextbox(void);
void getVRAMModeCoords(int32_t mode, int32_t *outX, int32_t *outClut);
int32_t advanceTextbox(int32_t boxId);
void setupDialogueBox(uint8_t owner);
uint16_t showTextboxReady(uint8_t boxId, uint8_t speakerId);
#if defined(VERSION_JP)
int32_t drawTextboxStrings(int32_t boxId);
#else
int32_t drawTextboxStrings(int32_t boxId, int32_t flag);
#endif
int32_t tickSelectionDialogue(void);
int32_t tickBackgroundDialogue(void);
int32_t getSpeakerName(int32_t speakerId, uint8_t *buf);
uint8_t *intToStringSJIS(uint8_t *buf, int32_t value, uint8_t digits, int32_t flag);
void renderUIBox(int32_t boxId);
void createTextbox(int32_t boxId, int32_t flags, RECT *rect, RECT *origin, void *tick, void *render);

static void *script_textbox_functions[] = {
	showTextboxReady,
	scriptSetTextboxSize,
	showTextbox,
	scriptShowSelection,
	renderUIBox,
	renderScriptDialogueBox,
	tickScriptDialogueBox,
	setupBoxOrigin,
	setupDialogueBox,
	advanceTextbox,
	flipIdleTextboxPage,
	drawString2,
	registerTextbox,
	triggerBoxCloseFlag,
	createTextbox,
	closeBox,
	closeAllTextboxes,
	closeTextbox,
	getVRAMModeCoords,
	tickTextboxHandling,
	initializeTextbox,
	renderCustomSizedTextbox,
	tickCustomSizedTextbox,
	renderDialogueSelectionCursor,
	tickBackgroundDialogue,
	tickConfirmDialogue,
	tickSelectionDialogue,
	clearTextboxLineCount,
	drawTextboxStrings,
};

void renderScriptDialogueBox(void)
{
	int16_t rowPx;
	int32_t i;
	int16_t x;
	int16_t y;
	int16_t ySave;

	rowPx = TEXT_BOX_DATA[0].vramRow * 12;
	rowPx += (TEXT_BOX_DATA[0].backPage * 48);
	x = UI_BOX_DATA[0].finalPos.x + 5;
	y = UI_BOX_DATA[0].finalPos.y + 4;
	ySave = y;

	i = 0;
	while (i < 4) {
		renderString(0, x, y, 0xfc, 0xc, 0, rowPx, 6, 1);
		i++;
		rowPx += 0xc;
		y += 0xd;
	}

	if (ACTIVE_INSTRUCTION == SCRIPT_OP_SET_SELECTION) {
		renderDialogueSelectionCursor(x, ySave);
	} else if (ACTIVE_INSTRUCTION == SCRIPT_OP_CALL_ROUTINE && SCRIPT_STATE_3 == 2) {
		renderDialogueSelectionCursor(x, ySave);
	}

	if (MAIN_D_80134F94 != 0) {
		renderUIBox(0);
	}
}

// clang-format off
#if defined(VERSION_JP)
int32_t drawTextboxStrings(boxId)
#else
int32_t drawTextboxStrings(boxId, flag)
#endif
	uint8_t boxId;
#if !defined(VERSION_JP)
	int32_t flag;
#endif
// clang-format on
{
	TextBoxData *box;
	int32_t done;
	uint32_t x;
	int32_t clut;
	int16_t px;
	int16_t row;
	uint8_t *buf;

	box = &MAIN_D_801BE80C.box[boxId];
	if (box->writeCount == box->renderCount) {
		return 0;
	}

	getVRAMModeCoords(box->vramMode, (int32_t *)&x, &clut);
	px = x;
	row = box->vramRow + box->writeRow;
	buf = TEXT_BUFFERS_PTR + (row << 6);
	if (x != 0) {
		buf += 0x20;
	}
	row = row * 12;
	if (box->vramMode == 0) {
		x = 1;
	} else {
		x = 2;
	}

	while (x != 0) {
#if defined(VERSION_JP)
		done = drawString2(buf, px, row);
#else
		done = drawString2(buf, px, row, flag);
#endif
		box->writeRow++;
		if (done != 0) {
			box->writeCount = 1;
			box->renderCount = 1;
			box->flipCount = 0;
			box->registered = 1;
			break;
		}
		row += 12;
		buf += 0x40;
		x--;
	}

	return 1;
}

// clang-format off
void clearTextboxLineCount(boxId)
	uint8_t boxId;
// clang-format on
{
	TextBoxData *entry;
	int32_t rows;

	entry = &MAIN_D_801BE80C.box[boxId];

	rows = ((int32_t *)entry)[9];
	if (entry->doubleBuffered == 1) {
		rows <<= 1;
	}

	((int32_t *)entry)[9] = 0;
	entry->registered = 1;
	MAIN_D_801BE80C.usedRows -= rows;
}

int32_t tickSelectionDialogue(void)
{
	if (flipIdleTextboxPage(0) != 0) {
		return 0xffff;
	}

	if (isXPressedAfterDialogue() == 0) {
		return 0xffff;
	}

	if (isKeyDown(CONFIRM_BUTTON)) {
		advanceTextbox(0);
		playSound(0, 3);

		return MAIN_D_801BE952[0];
	}

	if (isKeyDown(CANCEL_BUTTON)) {
		if (isTriggerSet(0x31) == 0) {
			if (ACTIVE_INSTRUCTION != SCRIPT_OP_CALL_ROUTINE) {
				advanceTextbox(0);
				MAIN_D_80134FDC =
					(uint8_t *)MAIN_D_801BE94C[0];
				playSound(0, 4);

				return 0xffff;
			}

			advanceTextbox(0);
			MAIN_D_801BE952[0] = MAIN_D_801BE950[0] - 1;
			playSound(0, 4);

			return MAIN_D_801BE952[0];
		}

		playSound(0, 0xb);
	} else if (isKeyDown(0x1000)) {
		if (MAIN_D_801BE952[0] == 0) {
			MAIN_D_801BE952[0] = MAIN_D_801BE950[0] - 1;
		} else {
			MAIN_D_801BE952[0] -= 1;
		}

		playSound(0, 2);
	} else if (isKeyDown(0x4000)) {
		MAIN_D_801BE952[0] += 1;
		if (MAIN_D_801BE952[0] == MAIN_D_801BE950[0]) {
			MAIN_D_801BE952[0] = 0;
		}

		playSound(0, 2);
	}
	return 0xffff;
}

int32_t tickConfirmDialogue(void)
{
	if (flipIdleTextboxPage(0) != 0) {
		return 0;
	}

	if (MAIN_D_80134FE5 == 0) {
		MAIN_D_80134F94 = 1;
	}

	if (isXPressedAfterDialogue() == 0) {
		return 0;
	}

	switch (MAIN_D_80134FE5) {
	case 0:
		if (isKeyDown(CONFIRM_BUTTON) == 0) {
			goto ret0;
		}

		if (UI_BOX_DATA[0].state != 1) {
			goto ret0;
		}

		if (TEXT_BOX_DATA[0].doubleBuffered == 1) {
			advanceTextbox(0);
			playSound(0, 3);
			return 1;
		} else {
			playSound(0, 3);
			ACTIVE_INSTRUCTION = 0;
			return 1;
		}
	case 2:
		if (MAIN_D_80135010 != 0) {
			goto ret0;
		}
		/* fall through */
	case 1:
		if (TEXT_BOX_DATA[0].doubleBuffered == 1) {
			advanceTextbox(0);
			return 1;
		} else {
			ACTIVE_INSTRUCTION = 0;
			return 1;
		}
	}
ret0:
	return 0;
}

// clang-format off
void renderDialogueSelectionCursor(x, y)
	int16_t x;
	int16_t y;
// clang-format on
{
	if (TEXT_BOX_DATA[0].idle == 1) {
		return;
	}

#if defined(VERSION_JP)
	renderSelectionCursor(x - 1, (long)y + MAIN_D_801BE954[0] + MAIN_D_801BE952[0] * 13,
	                      MAIN_D_801BE956[0], 0xd, 6);
#else
	renderSelectionCursor(x - 1,
	                      y + MAIN_D_801BE954[0] + MAIN_D_801BE952[0] * 13 - 2,
	                      MAIN_D_801BE956[0], 0xd, 6);
#endif
}

void tickCustomSizedTextbox(void)
{
	if (flipIdleTextboxPage(0) != 0) {
		return;
	}

	if (!isXPressedAfterDialogue()) {
		return;
	}

	if (isKeyDown(CONFIRM_BUTTON)) {
		if (UI_BOX_DATA[0].state != 1) {
			return;
		}

		if (advanceTextbox(0) != 0) {
			triggerBoxCloseFlag(0);
		}

		playSound(0, 3);

		return;
	}

	if (!isKeyDown(CANCEL_BUTTON)) {
		return;
	}

	if (UI_BOX_DATA[0].state == 1) {
		while (TEXT_BOX_DATA[0].pageReady == 0) {
			showTextbox(0, 0xff);
		}

		ACTIVE_INSTRUCTION = 0;
		TEXT_BOX_DATA[0].idle = 1;
		triggerBoxCloseFlag(0);
		playSound(0, 4);
	}
}

void renderCustomSizedTextbox(void)
{
	TextBoxData *box;
	int16_t rowPx;
	int16_t x;
	int16_t pagePx;
	int16_t y;
	int32_t i;

	box = MAIN_D_801BE80C.box;
	pagePx = box->vramRows * 12;
	rowPx = box->vramRow * 12;
	rowPx += (int16_t)(box->backPage * pagePx);
	x = UI_BOX_DATA[0].finalPos.x + 5;
	y = UI_BOX_DATA[0].finalPos.y + 4;

	for (i = 0; i < box->vramRows; i++, rowPx += 0xc, y += 0xd) {
		renderString(0, x, y, 0xfc, 0xc, 0, rowPx, 6, 1);
	}
}

void initializeTextbox(void)
{
	TextBoxData *box;
	int32_t i;

	for (i = 0, box = TEXT_BOX_DATA; i < 6; i++, box++) {
		box->flags = 0;
		box->vramRows = 0;
		box->registered = 1;
	}

	MAIN_D_801BE80C.usedRows = 0;
	MAIN_D_80134FFC = 0;
	MAIN_D_80135010 = 0;
	TEXTBOX_OPEN_TIMER = 0;
}

#if defined(VERSION_JP)
void tickTextboxHandling(void)
#else
void tickTextboxHandling(int32_t flag)
#endif
{
	RECT area;
	int32_t drew;
	int32_t x;
	int32_t clut;
	TextBoxData *box;
	int32_t i;
	uint8_t flags;
	uint8_t features;
	uint8_t color;

	drew = 0;
	if (ACTIVE_INSTRUCTION != 0xff) {
		for (i = 0, box = TEXT_BOX_DATA; i < 6; i++, box++) {
			if (box->vramRows != 0) {
				if (box->registered == 1 && box->writeCount != box->renderCount) {
					if (box->doubleBuffered == 1) {
						box->writeRow = (box->backPage ^ 1) * box->vramRows;
						getVRAMModeCoords(box->vramMode, &x, &clut);
						area.x = x;
						area.y = (box->vramRow + box->writeRow) * 12;
						area.w = clut;
						area.h = box->vramRows * 12;
						clearTextSubArea(&area);
						box->registered = 0;
					} else {
						box->writeRow = 0;
						box->registered = 0;
					}
				}
				if (drew == 0 && box->registered == 0) {
#if defined(VERSION_JP)
					drew = drawTextboxStrings(i & 0xff);
#else
					drew = drawTextboxStrings(i & 0xff, flag);
#endif
				}
				flags = box->flags;
				if ((int16_t)(flags & 0xf) != UI_BOX_DATA[i].state) {
					if ((flags & 0xf) == 1) {
						if (box->registered == 1 && UI_BOX_DATA[i].state == 0) {
							features = (flags >> 4) & 3;
							if (features == 0) {
								color = 1;
							} else {
								color = 0;
							}
							if ((flags & 0x80) == 0) {
								createStaticUIBox((int16_t)i, color, features, &box->rect, box->tick, box->render);
							} else {
								createAnimatedUIBox((int16_t)i, color, features, &box->rect, &box->origin, box->tick, box->render);
							}
						}
					} else if (UI_BOX_DATA[i].state == 1) {
						closeTextbox(i & 0xff, 0);
					}
				}
			}
		}
		if (MAIN_D_80134FFC != 0) {
			MAIN_D_80134FFC--;
		}
		if (MAIN_D_80135010 != 0) {
			MAIN_D_80135010--;
		}
		TEXTBOX_OPEN_TIMER++;
		MAIN_D_80134F94 = 0;
		return;
	}
	for (i = 0; i < 6; i++) {
		if (UI_BOX_DATA[i].state != 0) {
			break;
		}
		setTextColor(1);
		IS_SCRIPT_PAUSED = 1;
	}
}

void getVRAMModeCoords(int32_t mode, int32_t *outX, int32_t *outClut)
{
	if (mode == 0) {
		*outX = 0;
		*outClut = 0xff;
	} else if (mode == 1) {
		*outX = 0;
		*outClut = 0x7f;
	} else {
		*outX = 0x80;
		*outClut = 0x7f;
	}
}

// clang-format off
void closeTextbox(boxId, target)
	int16_t boxId;
	RECT *target;
// clang-format on
{
	if (UI_BOX_DATA[boxId].state != 0 && UI_BOX_DATA[boxId].state != 3) {
		if ((MAIN_D_801BE80C.box[boxId].flags & 0x40) == 0) {
			removeStaticUIBox(boxId);
		} else {
			removeAnimatedUIBox(boxId, target);
		}

		clearTextboxLineCount(boxId);
	}
}

void closeAllTextboxes(void)
{
	int32_t i;
	uint8_t flags;

	for (i = 0; i < 6; i++) {
		flags = MAIN_D_801BE80C.box[i].flags;
		if ((flags & 0xf) != 0) {
			if (flags & 0x40) {
				ACTIVE_INSTRUCTION = 0xff;
				IS_SCRIPT_PAUSED = 0;
			}

			closeBox(i & 0xff);
		}
	}
}

// clang-format off
void createTextbox(boxId, flags, rect, origin, tick, render)
	uint8_t boxId;
	uint8_t flags;
	RECT *rect;
	RECT *origin;
	void *tick;
	void *render;
// clang-format on
{
	TextBoxData *entry;

	entry = &MAIN_D_801BE80C.box[boxId];

	if ((entry->flags & 0xf) == 1) {
		closeTextbox(boxId, 0);
	}

	entry->flags = flags;
	entry->rect = *rect;
	entry->origin = *origin;
	entry->tick = tick;
	entry->render = render;

	setFreshDialogue();
}

// clang-format off
void triggerBoxCloseFlag(boxId)
	uint8_t boxId;
// clang-format on
{
	TextBoxData *e = &MAIN_D_801BE80C.box[boxId];
	uint8_t val = e->flags;

	if ((val & 0xf) != 0) {
		val = val & 0xf0;
		e->flags = val;
	}
}

// clang-format off
void registerTextbox(boxId, row, rows, doubleBuffer, mode)
	uint8_t boxId;
	int32_t row;
	int32_t rows;
	uint8_t doubleBuffer;
	int32_t mode;
// clang-format on
{
	TextBoxData *entry;
	int32_t usedRows;
	RECT rect;
	int32_t vramX;
	int32_t vramW;

	entry = &MAIN_D_801BE80C.box[boxId];
	entry->vramRow = row;
	entry->vramRows = rows;
	entry->writeRow = 0;
	entry->vramMode = mode;
	entry->backPage = 0;
	entry->doubleBuffered = doubleBuffer;
	entry->writeCount = 0;
	entry->renderCount = 0;
	entry->flipCount = 0;
	entry->registered = 1;
	entry->pageReady = 0;
	entry->idle = 1;

	usedRows = entry->vramRows;
	if (entry->doubleBuffered == 1) {
		usedRows = usedRows << 1;
	}

	MAIN_D_801BE80C.usedRows += usedRows;
	setTextColor(1);
	getVRAMModeCoords(entry->vramMode, &vramX, &vramW);
	setRECT(&rect, vramX, entry->vramRow * 12, vramW, usedRows * 12);
	clearTextSubArea(&rect);
}

#if defined(VERSION_JP)
int32_t drawString2(uint8_t *str, int16_t x, int16_t y)
#else
int32_t drawString2(uint8_t *str, int16_t x, int16_t y, int32_t flag)
#endif
{
#if defined(VERSION_JP)
#ifdef __MWERKS__
	extern void drawGlyph();
#endif
#else
	int32_t y2;
#endif
	RECT rect;
	uint8_t ch;
	int16_t pos;
	int16_t rem;
#if !defined(VERSION_JP)
	uint16_t adv;
#endif
	uint16_t glyph;

	pos = 0;
	for (;;) {
		ch = *str++;
		switch (ch) {
		case 0:
			return 1;
		case 3:
			str += 3;
			break;
		case 2:
			str++;
			break;
		case 1:
			ch = *str++;
			setTextColor(ch);
			break;
		case 0xc:
			str++;
			rem = pos / 12 % 8;
			if (rem != 0) {
				rem = (8 - rem) * 12;
				setRECT(&rect, x + pos, y, rem, 0xc);
				clearTextSubArea(&rect);
				pos = pos + rem;
			}
			break;
#if !defined(VERSION_JP)
		case 0x16:
			str++;
			setRECT(&rect, x + pos - 6, y, 0x69, 0xc);
			clearTextSubArea(&rect);
			pos = 0x69;
			break;
		case 0x17:
			str++;
			setRECT(&rect, x + pos, y, 0x69, 0xc);
			clearTextSubArea(&rect);
			pos = 0x69;
			break;
		case 0x1b:
			str++;
			setRECT(&rect, x + pos, y, 0xa0, 0xc);
			clearTextSubArea(&rect);
			pos = 0xa0;
			break;
		case 0x1c:
			str++;
			setRECT(&rect, x + pos, y, 0x64, 0xc);
			clearTextSubArea(&rect);
			pos = 0x64;
			break;
		case 0x18:
			str++;
			setRECT(&rect, x + pos, y, 0x60, 0xc);
			clearTextSubArea(&rect);
			pos = 0x60;
			break;
		case 0x19:
			str++;
			setRECT(&rect, x + pos, y, 0x9c, 0xc);
			clearTextSubArea(&rect);
			pos = 0x9c;
			break;
		case 0x1a:
			str++;
			setRECT(&rect, x + pos, y, 0xb4, 0xc);
			clearTextSubArea(&rect);
			pos = 0xb4;
			break;
#endif
		case 0xe:
			str++;
			rem = pos / 12 % 0xb;
			if (rem != 0) {
				rem = (0xb - rem) * 12;
				setRECT(&rect, x + pos, y, rem, 0xc);
				clearTextSubArea(&rect);
				pos = pos + rem;
			}
			break;
		case 0xf:
			str++;
			setRECT(&rect, x + pos, y, 6, 0xc);
			clearTextSubArea(&rect);
			pos += 6;
			break;
		case 0xd:
			return 0;
		default:
#if defined(VERSION_JP)
			glyph = ch + (*str++ << 8);
#else
			if (isAsciiEncoded((char *)&ch) != 0) {
				glyph = swapShortBytes(convertAsciiToJis(ch));
			} else {
				glyph = ch + (*str++ << 8);
			}
#endif

			if (glyph == 0x4081) {
				setRECT(&rect, x + pos, y, 0xc, 0xc);
				clearTextSubArea(&rect);
			} else {
#if defined(VERSION_JP)
				drawGlyph(glyph, x + pos, y);
#else
				y2 = y;
				adv = drawGlyph(glyph, x + pos, y2);
				if (MAIN_D_80134F98 != 0) {
					adv = 0xc;
				}
#endif
			}

#if defined(VERSION_JP)
			pos += 0xc;
#else
			pos += adv;
#endif
			break;
		}
	}
}

// clang-format off
int32_t flipIdleTextboxPage(boxId)
	uint8_t boxId;
// clang-format on
{
	TextBoxData *entry;

	entry = &MAIN_D_801BE80C.box[boxId];

	if (entry->doubleBuffered == 0) {
		return 0;
	}

	if (entry->registered == 0) {
		return 1;
	}

	if (entry->writeCount != entry->renderCount) {
		return 1;
	}

	if (entry->idle == 0) {
		return 0;
	}

	if (entry->renderCount == 0) {
		return 1;
	}

	entry->idle = 0;
	entry->backPage ^= 1;
	entry->flipCount++;

	if (entry->pageReady == 0) {
		ACTIVE_INSTRUCTION = 0;
	}

	TEXTBOX_OPEN_TIMER = 0;

	return 1;
}

void closeBox(int32_t boxId)
{
	triggerBoxCloseFlag(boxId);
	closeTextbox(boxId, 0);
}

int32_t tickBackgroundDialogue(void)
{
	TextBoxData *entry;

	entry = TEXT_BOX_DATA;

	if (flipIdleTextboxPage(0) != 0) {
		return 0;
	}

	if (entry->renderCount == entry->flipCount) {
		entry->idle = 1;
		entry->pageReady = 0;
		return 1;
	}

	return 0;
}

// clang-format off
int32_t advanceTextbox(boxId)
	uint8_t boxId;
// clang-format on
{
	TextBoxData *entry;

	entry = &MAIN_D_801BE80C.box[boxId];
	if (entry->registered == 0) {
		return 0;
	}

	if (entry->pageReady == 1) {
		if (entry->renderCount != entry->flipCount) {
			entry->backPage ^= 1;
		} else {
			entry->idle = 1;
			entry->pageReady = 0;
			ACTIVE_INSTRUCTION = 0;
			return 1;
		}
	} else {
		entry->backPage ^= 1;
		ACTIVE_INSTRUCTION = 0;
	}

	entry->flipCount++;
	TEXTBOX_OPEN_TIMER = 0;

	return 0;
}

void setupDialogueBox(uint8_t owner)
{
	RECT rect;
	RECT origin;
	uint8_t flags;

	flags = 0x21;
	if (setupBoxOrigin(owner, &origin) != 0) {
		flags |= 0x80;
	}

	setRECT(&rect, -0x82, 0x2a, 0x106, 0x3b);
	createTextbox(0, flags, &rect, &origin, tickScriptDialogueBox,
	              renderScriptDialogueBox);
	registerTextbox(0, 0, 4, 1, 0);
}

int32_t setupBoxOrigin(int32_t ownerId, RECT *origin)
{
	int16_t pos[2];

	if (ownerId == 0xff) {
		return 0;
	}

	if (MAIN_D_80134FD2 != -0x270f) {
		worldPosToScreenPos2(&MAIN_D_80134FD2, &MAIN_D_80134FD4,
		                     &MAIN_D_80134FD6);
		pos[0] = MAIN_D_80134FD2 - 5;
		pos[1] = MAIN_D_80134FD4 - 5;
		MAIN_D_80134FD2 = -0x270f;
	} else {
		int32_t entityId = scriptIdToEntityId(ownerId) & 0xff;
		if (entityId == 0xff) {
			return 0;
		}

		getEntityScreenPos(ENTITY_TABLE[entityId], 1, pos);
	}

	setRECT(origin, pos[0], pos[1], 0xa, 0xa);

	return 1;
}

void tickScriptDialogueBox(void)
{
	if (ACTIVE_INSTRUCTION == SCRIPT_OP_SET_SELECTION) {
		if (tickSelectionDialogue() != 0xffff) {
			MAIN_D_80134FDC =
				(uint8_t *)((uint32_t)MAIN_D_801BE948[0] +
			                    MAIN_D_801BE952[0] * 2);
			MAIN_D_80134FDC =
				(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR +
			                    *(uint16_t *)MAIN_D_80134FDC);
		}
	} else if (ACTIVE_INSTRUCTION == SCRIPT_OP_SHOW_TEXTBOX) {
		tickConfirmDialogue();
	} else if (ACTIVE_INSTRUCTION == SCRIPT_OP_CALL_ROUTINE) {
		switch (SCRIPT_STATE_3) {
		case 0:
			break;
		case 1:
			if (tickConfirmDialogue() == 1) {
				SELECTION_MENU_STATE = SCRIPT_STATE_4;
				ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
				SCRIPT_STATE_3 = 0;
			}
			break;
		case 2:
			if (tickSelectionDialogue() != 0xffff) {
				SELECTION_MENU_STATE = (long)SCRIPT_STATE_4 + MAIN_D_801BE952[0];
				ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
				SCRIPT_STATE_3 = 0;
			}
			break;
		case 3:
			if (tickBackgroundDialogue() == 1) {
				ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
				SCRIPT_STATE_3 = 0;
			}
			break;
		}
	}
}

void renderUIBox(int32_t boxId)
{
	POLY_FT4 poly;
	int16_t x;
	int16_t y;
	int16_t u;

	x = UI_BOX_DATA[boxId].finalPos.x + 0xef;
	y = UI_BOX_DATA[boxId].finalPos.y + 0x2b;
	u = TEXTBOX_OPEN_TIMER / 4 % 3 * 16 + 0x4c;
	SetPolyFT4(&poly);
	SetSemiTrans(&poly, 1);
	poly.tpage = GetTPage(0, 1, 320, 128);
	poly.clut = GetClut(96, 500);
	setRGB0(&poly, 0x80, 0x80, 0x80);
	setUVDataPolyFT4(&poly, u, 0xb7, 0x10, 0xc);
	setPosDataPolyFT4(&poly, x, y, 0x10, 0xc);

	GsSortPoly(&poly, ACTIVE_ORDERING_TABLE, (6 - boxId));
}

void scriptShowSelection(void)
{
	uint8_t optionCount;
#if !defined(VERSION_JP)
	uint16_t height;
#endif

	pollNextScriptUByte(&optionCount);

	MAIN_D_801BE950[0] = optionCount;
	MAIN_D_801BE952[0] = 0;
	MAIN_D_801BE948[0] = (int32_t)MAIN_D_80134FDC;
	MAIN_D_80134FDC = MAIN_D_80134FDC + (optionCount + 1) * 2;
	MAIN_D_801BE956[0] = showTextbox(0, MAIN_D_80134FE6);
	MAIN_D_801BE956[0] = MAIN_D_801BE956[0] * 12 + 2;
#if !defined(VERSION_JP)
	height = MAIN_D_801BE956[0];

	if (height > 0xf0) {
		MAIN_D_801BE956[0] = 0xf0;
	}
#endif

	MAIN_D_80134FDC += 2;
	MAIN_D_801BE94C[0] = (int32_t)MAIN_D_80134FDC;

	if (MAIN_D_80134FE6 != 0xff) {
		MAIN_D_801BE954[0] = 0xd;
	} else {
		MAIN_D_801BE954[0] = 0;
	}

	ACTIVE_INSTRUCTION = SCRIPT_OP_SET_SELECTION;
}

GARBAGE(showTextbox, 1);

uint16_t showTextbox(uint8_t boxId, uint8_t speakerId)
{
	TextBoxData *entry;
	uint8_t *base;
	uint8_t *out;
	uint16_t col;
	uint16_t maxCol;
	uint32_t row;
	int32_t lines;
	uint8_t ctrl;

	entry = &MAIN_D_801BE80C.box[boxId];
	base = TEXT_BUFFERS_PTR + (entry->vramRow << 6);
	if (entry->vramMode == 2) {
		base += 0x20;
	}
	if (entry->doubleBuffered == 1) {
		base = (uint8_t *)(base + (((entry->backPage ^ 1) * entry->vramRows) << 6));
	}
	out = base;
	col = maxCol = 0;
	row = 0;
	if (speakerId != 0xff) {
		*out++ = 1;
		if (speakerId == 0xfd) {
			*out++ = 6;
		} else if (speakerId == 0xfc) {
			*out++ = 0xa;
		} else if (speakerId >= 0xc8) {
			*out++ = 5;
		} else {
			*out++ = 7;
		}
		lines = getSpeakerName(speakerId, out);
		out += lines;
		*out++ = 1;
		*out++ = 1;
		*out++ = 0xd;
		*out = 0;
		row++;
		out = base + (row << 6);
	}
top: {
	ctrl = *MAIN_D_80134FDC++;
	{
		switch (ctrl) {
		case 3:
			*out++ = ctrl;
			*out++ = *MAIN_D_80134FDC++;
			*out++ = *MAIN_D_80134FDC++;
			*out++ = *MAIN_D_80134FDC++;
			goto top;
		case 4:
			ctrl = *MAIN_D_80134FDC++;
			ctrl = readPStat(ctrl);
			out = intToStringSJIS(out, ctrl, 3, 1);
			goto top;
		case 5:
			MAIN_D_80134FDC++;
			lines = getSpeakerName(0xfd, out);
			out += lines;
			col += (uint16_t)(lines >> 1);
			goto top;
		case 6:
			MAIN_D_80134FDC++;
			lines = getSpeakerName(0xfc, out);
			out += lines;
			col += (uint16_t)(lines >> 1);
			goto top;
		case 7:
			ctrl = *MAIN_D_80134FDC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out,
				       (const char *)&DIGIMON_DATA[ctrl]);
				lines = strlen(
					(const char *)&DIGIMON_DATA[ctrl]);
				out += lines;
				col += (uint16_t)(lines >> 1);
			}
			goto top;
		case 8:
			ctrl = *MAIN_D_80134FDC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out, MOVE_NAMES[ctrl]);
				lines = strlen(MOVE_NAMES[ctrl]);
				out += lines;
				col += (uint16_t)(lines >> 1);
			}
			goto top;
		case 9:
			ctrl = *MAIN_D_80134FDC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out,
				       (const char *)&ITEM_PARA[ctrl]);
				lines = strlen(
					(const char *)&ITEM_PARA[ctrl]);
				out += lines;
				col += (uint16_t)(lines >> 1);
			}
			goto top;
		case 10:
			MAIN_D_80134FDC++;
			out = intToStringSJIS(out, MONEY, 6, 0);
			goto top;
		case 11:
			MAIN_D_80134FDC++;
			out = intToStringSJIS(out, MERIT, 4, 0);
			goto top;
		case 16:
			MAIN_D_80134FDC++;
			out = intToStringSJIS(out, MAIN_D_8013500C, 5, 1);
			goto top;
		case 19:
			MAIN_D_80134FDC++;
			out = intToStringSJIS(out, MAIN_D_80134FCC, 3, 1);
			goto top;
		case 20:
			MAIN_D_80134FDC++;
			out = intToStringSJIS(out, TOURNAMENTS_LOST, 3, 1);
			goto top;
		case 21:
			MAIN_D_80134FDC++;
			out = intToStringSJIS(out, MAIN_D_80134FD0, 3, 1);
			goto top;
		case 17:
			ctrl = *MAIN_D_80134FDC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out, BGM_TRACK_NAMES[ctrl]);
				lines = strlen(BGM_TRACK_NAMES[ctrl]);
				out += lines;
				col += (uint16_t)(lines >> 1);
			}
			goto top;
		case 18:
			ctrl = *MAIN_D_80134FDC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out, TOURNAMENT_NAMES[ctrl]);
				lines = strlen(TOURNAMENT_NAMES[ctrl]);
				out += lines;
				col += (uint16_t)(lines >> 1);
			}
			goto top;
		case 12:
			*out++ = ctrl;
			*out++ = *MAIN_D_80134FDC++;
			goto top;
		case 14:
			*out++ = ctrl;
			*out++ = *MAIN_D_80134FDC++;
			goto top;
		case 13:
			*out++ = ctrl;
			*out++ = *MAIN_D_80134FDC++;
			if (col > maxCol) {
				maxCol = col;
			}
			col = 0;
			ctrl = *MAIN_D_80134FDC;
			if (ctrl == 0) {
				MAIN_D_80134FDC += 2;
				out -= 2;
				*out = ctrl;
				ctrl = *MAIN_D_80134FDC;
				if (ctrl == SCRIPT_OP_SHOW_TEXTBOX) {
					goto done;
				}
				entry->pageReady = 1;
				goto done;
			}
			row++;
			out = base + (row << 6);
			goto top;
		case 1:
		case 2:
			*out++ = ctrl;
			*out++ = *MAIN_D_80134FDC++;
			goto top;
		case 15:
		default:
			*out++ = ctrl;
			*out++ = *MAIN_D_80134FDC++;
			col++;
			goto top;
		}
	}
}
done:
	entry->writeCount++;
	ACTIVE_INSTRUCTION = SCRIPT_OP_SHOW_TEXTBOX;

	return maxCol;
}

void scriptSetTextboxSize(void)
{
	uint8_t originId;
	uint8_t cols;
	uint8_t rows;
	uint8_t boxFlags;
	RECT rect1;
	RECT origin;
	uint16_t width;
	uint16_t height;

	pollNextScriptUByte(&originId);
	pollNextTwoScriptBytes(&cols, &rows);

	boxFlags = 0x21;
	if (setupBoxOrigin(originId, &origin) != 0) {
		boxFlags |= 0x80;
	}

	width = cols * 12 + 10;
	height = rows * 13 + 7;
	setRECT(&rect1, (0x140 - width) / 2 - 0xa0, (0xf0 - height) / 2 - 0x78, width, height);
	createTextbox(0, boxFlags, &rect1, &origin, tickCustomSizedTextbox,
	              renderCustomSizedTextbox);
	registerTextbox(0, 0, rows, 1, 0);
}

uint16_t showTextboxReady(uint8_t boxId, uint8_t speakerId)
{
	MAIN_D_801BE80C.box[boxId].pageReady = 1;

	return showTextbox(boxId, speakerId);
}
