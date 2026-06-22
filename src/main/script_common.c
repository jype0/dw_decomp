#include <string.h>

#include <libgs.h>

#include <dw/file_queue.h>
#include <dw/item.h>
#include <dw/map.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/ui.h>
#include <dw/utils.h>

#include "common.h"

typedef struct {
	uint8_t b[11];
} ShopkeeperIdTable;

typedef struct {
	uint8_t b[8];
} BabyTypeTable;

typedef struct {
	uint8_t digimonId;
	uint8_t spriteId;
	uint8_t unk2;
	uint8_t unk3;
} CardData;

typedef struct {
	int16_t v[15];
} MenuTextLayout;

typedef struct {
	int16_t v[28];
} BoxUVTable;

typedef struct {
	uint32_t usedRows;
	TextBoxData box[6];
} TextBoxTable;

void worldPosToScreenPos2(int16_t *a, int16_t *b, int16_t *c);
void renderSelectionCursor(int32_t a0, int32_t a1, int32_t a2, int32_t a3,
			   int32_t a4);
int32_t MAIN_func_80106730(uint8_t op, uint32_t lhs, uint32_t rhs);
int32_t getItemCount(int32_t itemId);
void renderItemSprite(int32_t itemId, int32_t x, int32_t y, int32_t depth);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY,
		       int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t xPos, int32_t yPos,
		      int32_t width, int32_t height);
void renderString(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4,
		  int32_t a5, int32_t a6, int32_t a7, int32_t a8);
int32_t MAIN_func_80100E40(int32_t boxId);
int32_t convertAsciiToJis(int32_t input);
int32_t swapShortBytes(int32_t input);
int32_t isAsciiEncoded(const char *value);
int32_t drawGlyph(uint16_t glyph, int32_t col, int32_t a2);
int32_t hasMove(int32_t moveId);
void unlearnMove(int32_t moveId);
void loadMap(uint16_t mapId);

void renderMonochromonMoodBubble(int32_t instanceId);
void closeTextbox(int32_t boxId, RECT *target);
void MAIN_func_800FF0FC(ItemMenuBox *box, int32_t style);
void MAIN_func_800FBC58(void);
void MAIN_func_800FAB30(void);
void MAIN_func_800FADE0(void);
void MAIN_func_800FD534(ItemMenuBox *box, int32_t style);
int32_t MAIN_func_800FD61C(ItemMenuBox *box, RECT *origin, int32_t uiBoxId);
void MAIN_func_801000E4(void);
int32_t MAIN_func_800FA834(void);
int32_t MAIN_func_800FAA68(void);
int32_t MAIN_func_800FAFB0(void);
void MAIN_func_800FB070(void);
void MAIN_func_800FB0CC(void);
void MAIN_func_800FBC00(void);
uint8_t *doShopkeeperLine(int32_t idx);
uint8_t *resolveMapHeadEntry(int32_t section, int32_t idx);
void processInput(void);
int32_t isKeyDown(uint32_t key);
void MAIN_func_800FC08C(void);
int32_t isXPressedAfterDialogue(void);
void *allocateArray(uint32_t size);
void freeArray(uint32_t *array);
uint8_t getRecycleId(uint8_t value);
int32_t MAIN_func_800FCF88(ItemMenuBox *box);
void MAIN_func_800FD3DC(ItemMenuBox *box, int32_t startRow, int32_t style);
void MAIN_func_800FD428(ItemMenuBox *box, int32_t style);
void MAIN_func_800FD4E8(ItemMenuBox *box, int32_t startRow, int32_t style);
void MAIN_func_800FDF84(void);
int32_t MAIN_func_800FE650(uint8_t a);
void MAIN_func_800FF2A8(ItemMenuBox *box);
void MAIN_func_800FF310(ItemMenuBox *box);
uint8_t *MAIN_func_800FF444(uint8_t *data, int32_t index);
void terminateString(uint8_t *str, int32_t flag);
uint8_t *padWithSpaces(uint8_t *str, int32_t width, int32_t used);
void setDigimonRaised(int32_t digimonId);
int32_t hasDigimonRaised(int32_t digimonId);
void unlockMedal(uint16_t medal);
int32_t hasMedal(uint16_t medal);
void MAIN_func_800FF900(void);
void checkShopMap(int32_t mapId);
void checkArenaMap(int32_t mapId);
void MAIN_func_800FFBB8(int32_t boxId);
int32_t MAIN_func_800FFDF4(void);
void MAIN_func_800FFF80(int32_t x, int32_t y);
void MAIN_func_800FFFF0(void);
void getVRAMModeCoords(int32_t mode, int32_t *outX, int32_t *outClut);
int32_t advanceTextbox(int32_t boxId);
void setupDialogueBox(uint8_t owner);
int32_t MAIN_func_80101EF8(int32_t boxId, int32_t speakerId);
uint8_t *resolveMapHeadEntry(int32_t section, int32_t idx);
void *allocateArray(uint32_t size);
uint8_t *MAIN_func_800FF444(uint8_t *data, int32_t index);
uint8_t *padWithSpaces(uint8_t *str, int32_t width, int32_t used);
void MAIN_func_800FB700(void);
void MAIN_func_800FB8D4(void);
void MAIN_func_800FBAA0(void);
int32_t MAIN_func_800FCFB8(RECT *origin);
int32_t MAIN_func_800FD244(RECT *origin);
void MAIN_func_800FD7D8(int32_t boxId, int32_t idx, int16_t x, int16_t y);
void MAIN_func_800FD8D4(ItemMenuBox *box);
void MAIN_func_800FDC5C(ItemMenuBox *box, int16_t x1, int16_t y1, int16_t x2,
			int16_t y2, int32_t flag);
void MAIN_func_800FE150(int32_t boxId, int16_t x, int16_t y, int32_t w,
			int16_t h);
void MAIN_func_800FE258(int32_t spriteId, int16_t x, int16_t y,
			int32_t depth);
void MAIN_func_800FE704(ItemMenuBox *box, uint8_t row, int32_t isLast);
void MAIN_func_800FE9F0(ItemMenuBox *box, uint8_t row, int32_t isLast);
void MAIN_func_800FEC30(ItemMenuBox *box, uint8_t row, int32_t isLast);
void MAIN_func_800FED64(ItemMenuBox *box, uint8_t row, int32_t isLast);
void MAIN_func_800FEEF0(ItemMenuBox *box, uint8_t row, int32_t isLast);
void MAIN_func_800FF338(int32_t boxId, int16_t x, int16_t y, int32_t w,
			int16_t h);
int32_t MAIN_func_800FFA4C(int32_t boxId, int32_t flag);
int32_t MAIN_func_800FFC1C(void);
int32_t MAIN_func_800FFF24(void);
int32_t drawString2(uint8_t *str, int16_t x, int16_t y, int32_t flag);
int32_t getSpeakerName(int32_t speakerId, uint8_t *buf);
uint8_t *intToStringSJIS(uint8_t *buf, int32_t value, int32_t digits,
			 int32_t flag);
void renderHorizontalLine(int32_t boxId, int16_t x, int16_t y, int32_t w);
void renderSellItemBox(void);
void renderUIBox(int32_t boxId);
void renderVerticalLine(int32_t boxId, int16_t x, int16_t y, int32_t h);
void tickSellItemBox(void);
int32_t getSpeakerName(int32_t speakerId, uint8_t *buf);
uint8_t *intToStringSJIS(uint8_t *buf, int32_t value, int32_t digits,
			 int32_t flag);

extern ScriptCameraMovement MAIN_D_801BE6B4[];
extern TextBoxTable MAIN_D_801BE80C;
extern uint8_t TEXTBOX_OPEN_TIMER;
extern uint32_t MAIN_D_8013501C;
extern int32_t MAIN_D_80135028;
extern uint32_t MAIN_D_80134F64;
extern uint8_t *MAIN_D_801345B0;
extern int16_t MAIN_D_80134F60;
extern uint8_t MAIN_D_8012FDCE[];
extern int32_t MAIN_D_80135024;
extern int32_t MAIN_D_80135020;
extern uint16_t MAIN_D_80135016;
extern int32_t MAIN_D_80135018;
extern uint32_t POLLED_INPUT;
extern ShopkeeperIdTable MAIN_D_8012FE5C;
extern int32_t MAIN_D_80134F94;
extern uint16_t MAIN_D_801BE952[];
extern uint16_t MAIN_D_801BE954[];
extern uint16_t MAIN_D_801BE956[];
extern BabyTypeTable MAIN_D_801345B4;
extern int16_t MAIN_D_8012FE68[];
extern int32_t MAIN_D_80134F88;
extern int16_t MAIN_D_801345BC[2];
extern uint32_t POLLED_INPUT_PREVIOUS;
extern uint8_t INVENTORY_ITEM_TYPES[];
extern uint8_t INVENTORY_ITEM_AMOUNTS[];
extern uint16_t MAIN_D_801BE950[];
extern int32_t MAIN_D_801BE948[];
extern int32_t MAIN_D_801BE94C[];
extern uint8_t MAIN_D_80134F81;
extern uint8_t MAIN_D_80134F82;
extern RECT MAIN_D_801302BC[];
extern uint8_t MAIN_D_80134F80;
extern int32_t MAIN_D_80134F7C;
extern CardData CARD_DATA[];
extern uint8_t MAIN_D_8012FFD9[];
extern int32_t MAIN_D_8012FFC4[];
extern GsOT *ACTIVE_ORDERING_TABLE;
extern BoxUVTable MAIN_D_8012FE24;
extern MenuTextLayout MAIN_D_8012FDE4;
extern MenuTextLayout MAIN_D_8012FE04;
extern char *MOVE_NAMES[];
extern char *MAIN_D_8012FEC8[];
extern char *MAP_NAME_PTR[];
extern char *ITEM_DESC_PTR[];
extern int8_t MAIN_D_80134F98;
extern char *MAIN_D_801300E0[];
extern int16_t MAIN_D_80130318[];
extern int32_t MAIN_D_80134FA8;
extern char MAIN_D_801345F0[8];

static void *script_common_functions[] = {
	scriptInstructionFBtoFF,
	tickScriptedMovements,
	readFileSection,
	enforceStatsLimits,
	MAIN_func_80102630,
	MAIN_func_801025E8,
	MAIN_func_80102564,
	MAIN_func_80102514,
	MAIN_func_801024CC,
	getStatsPointer,
	MAIN_func_801022FC,
	scriptCompareDate,
	scriptIdToEntityId,
	intToStringSJIS,
	getSpeakerName,
	MAIN_func_80101EF8,
	scriptSetTextboxSize,
	showTextbox,
	scriptShowSelection,
	renderUIBox,
	renderScriptDialogueBox,
	tickScriptDialogueBox,
	setupBoxOrigin,
	setupDialogueBox,
	advanceTextbox,
	MAIN_func_80100E40,
	drawString2,
	registerTextbox,
	triggerBoxCloseFlag,
	createTextbox,
	closeBox,
	MAIN_func_8010064C,
	closeTextbox,
	getVRAMModeCoords,
	MAIN_func_80100258,
	MAIN_func_8010020C,
	MAIN_func_801000E4,
	MAIN_func_800FFFF0,
	MAIN_func_800FFF80,
	MAIN_func_800FFF24,
	MAIN_func_800FFDF4,
	MAIN_func_800FFC1C,
	MAIN_func_800FFBB8,
	MAIN_func_800FFA4C,
	checkArenaMap,
	MAIN_func_800FF9AC,
	checkShopMap,
	MAIN_func_800FF900,
	createMonochromonMoodBubble,
	hasMedal,
	unlockMedal,
	hasDigimonRaised,
	setDigimonRaised,
	showMapheadSelection,
	setDialogueOwner,
	renderVerticalLine,
	padWithSpaces,
	terminateString,
	MAIN_func_800FF444,
	MAIN_func_800FF338,
	MAIN_func_800FF310,
	MAIN_func_800FF2A8,
	MAIN_func_800FF0FC,
	MAIN_func_800FEEF0,
	MAIN_func_800FED64,
	MAIN_func_800FEC30,
	MAIN_func_800FE9F0,
	MAIN_func_800FE704,
	MAIN_func_800FE650,
	lostAllLives,
	showMapHeadTextbox,
	MAIN_func_800FE258,
	MAIN_func_800FE150,
	renderHorizontalLine,
	MAIN_func_800FDFB4,
	MAIN_func_800FDF84,
	MAIN_func_800FDC5C,
	MAIN_func_800FD8D4,
	MAIN_func_800FD7D8,
	MAIN_func_800FD61C,
	MAIN_func_800FD534,
	MAIN_func_800FD4E8,
	MAIN_func_800FD428,
	MAIN_func_800FD3DC,
	MAIN_func_800FD244,
	MAIN_func_800FCFB8,
	MAIN_func_800FCF88,
	MAIN_func_800FCCFC,
	MAIN_func_800FCC98,
	MAIN_func_800FCC40,
	MAIN_func_800FCB3C,
	MAIN_func_800FCA3C,
	MAIN_func_800FCA14,
	MAIN_func_800FC968,
	destroyItemMenuBox,
	showShopkeeperTextbox,
	initializeItemMenuBox,
	MAIN_func_800FC508,
	isPartnerBaby,
	dailyPStatTrigger,
	getRecycleId,
	freeArray,
	allocateArray,
	handleItemLoss,
	setInputRepeatMask,
	isXPressedAfterDialogue,
	MAIN_func_800FC08C,
	isKeyDown,
	processInput,
	initialKeyInputs,
	renderMonochromonMoodBubble,
	resolveMapHeadEntry,
	doShopkeeperLine,
	MAIN_func_800FBC58,
	MAIN_func_800FBC00,
	MAIN_func_800FBAA0,
	MAIN_func_800FB8D4,
	MAIN_func_800FB700,
	renderSellItemBox,
	tickSellItemBox,
	MAIN_func_800FB0CC,
	MAIN_func_800FB070,
	MAIN_func_800FAFB0,
	MAIN_func_800FADE0,
	MAIN_func_800FAB30,
	MAIN_func_800FAA68,
	MAIN_func_800FA834,
};

INCLUDE_ASM("asm/main/nonmatchings/script_common", MAIN_func_800FA834);

int32_t MAIN_func_800FAA68(void)
{
	int32_t i;
	uint8_t *buf;
	int32_t result;
	int32_t type;
	int32_t amount;

	result = 0;
	buf = MAIN_D_80134F6C->buf;
	MAIN_D_80134F6C->itemCount = 0;

	for (i = 0; i < INVENTORY_SIZE[0]; i++) {
		type = INVENTORY_ITEM_TYPES[i];
		amount = INVENTORY_ITEM_AMOUNTS[i];

		if (type != 0xff) {
			if (ITEM_PARA[type].droppable != 0) {
				amount = (amount | 0x80) & 0xff;
				result = 1;
			}
		} else {
			type = 0xff;
		}

		MAIN_D_80134F6C->itemCount++;
		*buf++ = type;
		*buf++ = amount;
	}

	return result;
}

void MAIN_func_800FAB30(void)
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

	if (SELECTION_MENU_STATE != 1) {
		return;
	}

	if (isKeyDown(0x40)) {
		src = &MAIN_D_801302BC[MAIN_D_80135011];
		rect.x = src->x;
		rect.y = src->y;
		rect.w = src->w;
		rect.h = src->h;

		switch (MAIN_D_80135011) {
		default:
			MAIN_func_800FCFB8(&rect);
			break;
		case 7:
			MAIN_func_800FAFB0();
			break;
		case 5:
			MAIN_func_800FD244(&rect);
			break;
		}
	} else if (isKeyDown(0x10)) {
		if (MAIN_D_80135011 == 7) {
			SELECTION_MENU_STATE = 0xa;
		} else if (MAIN_D_80135011 == 5) {
			if (isTriggerSet(3) != 0) {
				writePStat(0xfe, 0xff);
				unsetTrigger(3);
				SELECTION_MENU_STATE = 4;
			}
		} else {
			SELECTION_MENU_STATE = 8;
		}

		playSound(0, 4);
	} else if (isKeyDown(0x1000)) {
		if (POLLED_INPUT & 8) {
			MAIN_func_800FD3DC(box, 9, 0);
		} else {
			MAIN_func_800FD428(box, 0);
		}
	} else if (isKeyDown(0x4000)) {
		if (POLLED_INPUT & 8) {
			MAIN_func_800FD4E8(box, 9, 0);
		} else {
			MAIN_func_800FD534(box, 0);
		}
	} else if (isKeyDown(0x800)) {
		src = &MAIN_D_801302BC[MAIN_D_80135011];
		rect.x = src->x;
		rect.y = src->y;
		rect.w = src->w;
		rect.h = src->h;
		MAIN_func_800FD61C(box, &rect, 1);
		playSound(0, 3);
	}
}

int32_t MAIN_func_800FAFB0(void)
{
	ItemMenuBox *box;
	int32_t idx;

	box = MAIN_func_800FCC40();
	idx = (box->topRow + box->cursor) * 2;
	MAIN_D_80134F78 = box->buf[idx];

	if (MAIN_D_80134F78 != 0xff && box->buf[idx + 1] != 0) {
		MAIN_D_8013500C = ITEM_PARA[MAIN_D_80134F78].meritValue;
		SELECTION_MENU_STATE = 0xb;
		SCRIPT_STATE_3 = 0;
		playSound(0, 3);
		return 1;
	}

	playSound(0, 0xb);

	return 0;
}

void MAIN_func_800FB070(void)
{
	if (UI_BOX_DATA[3].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (isKeyDown(0x850) == 0) {
		return;
	}

	triggerBoxCloseFlag(3);
	playSound(0, 3);
}

void MAIN_func_800FB0CC(void)
{
	renderString(0, (int16_t)(UI_BOX_DATA[3].finalPos.x + 6),
		     (int16_t)(UI_BOX_DATA[3].finalPos.y + 5), 0xfc, 0xc, 0,
		     (int16_t)(TEXT_BOX_DATA[3].vramRow * 12), 3, 1);
}

void renderSellItemBox(void)
{
	MenuTextLayout layout;
	int32_t srcCol;
	int16_t *entry;
	CardData *card;
	int32_t i;
	int16_t rowPx;
	int16_t y;
	int16_t x;
	int16_t sx;
	int16_t sy;

	layout = MAIN_D_8012FDE4;
	rowPx = TEXT_BOX_DATA[3].vramRow * 12;
	x = UI_BOX_DATA[3].finalPos.x;
	y = UI_BOX_DATA[3].finalPos.y;
	renderHorizontalLine(3, 4, 0x17, 0x7a);
	renderHorizontalLine(3, 0xc, 0x3c, 0x6a);
	MAIN_func_800FE150(3, 0x55, 0x2a, 0x1a, 0xe);
	sx = x + 8;
	sy = y + 5;

	if (MAIN_D_80135011 < 3) {
		renderItemSprite(MAIN_D_80134F78, sx, sy, 3);
	} else {
		card = (CardData *)(MAIN_D_80134F78 * 4);
		card = (CardData *)((uint32_t)card + (uint32_t)CARD_DATA);
		sx += 2;
		sy += 2;
		MAIN_func_800FE258(card->spriteId, sx, sy, 3);
	}

	entry = layout.v;
	i = 0;
	srcCol = 0;
	while (i < 4) {
		renderString(0, entry[0] + x, entry[1] + y, entry[2] * 12,
			     0xc, srcCol * 12, rowPx, 3, 1);
		i++;
		srcCol = (int16_t)(srcCol + entry[2]);
		entry += 3;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/script_common", MAIN_func_800FADE0);

void MAIN_func_800FB700(void)
{
	uint8_t *out;
	char *name;
	uint32_t total;

	out = TEXT_BUFFERS_PTR + (MAIN_D_80135018 << 6);

	if (MAIN_D_80135011 < 3) {
		strcpy(out, ITEM_PARA[MAIN_D_80134F78].name);
		out += strlen(ITEM_PARA[MAIN_D_80134F78].name);
	} else {
		strcpy(out, name = DIGIMON_DATA[CARD_DATA[MAIN_D_80134F78].digimonId].name);
		out += strlen(name);
	}

	*out++ = 0x18;
	*out++ = 0;
	out = intToStringSJIS(out, MAIN_D_80134F7C, 5, 0);
	*out++ = 0x19;
	*out++ = 0;
	out = intToStringSJIS(out, MAIN_D_80134F81, 2, 0);
	*out++ = 0x1a;
	*out++ = 0;

	total = MAIN_D_80134F7C * MAIN_D_80134F81;
	if (total >= 0xf4240) {
		total = 0xf423f;
	}

	out = intToStringSJIS(out, total, 6, 0);
	*out++ = 0;
	*out = 0;
	TEXT_BOX_DATA[3].pageReady = 1;
	TEXT_BOX_DATA[3].writeCount++;
}

void MAIN_func_800FB8D4(void)
{
	ItemMenuBox *box;
	uint8_t amount;

	if (UI_BOX_DATA[3].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (isKeyDown(0x10)) {
		triggerBoxCloseFlag(3);
		playSound(0, 4);
		return;
	}

	if (isKeyDown(0x8000)) {
		MAIN_D_80134F81 = 0;
		playSound(0, 2);
		return;
	}

	if (isKeyDown(0x2000)) {
		MAIN_D_80134F81 = 1;
		playSound(0, 2);
		return;
	}

	if (isKeyDown(0x40)) {
		triggerBoxCloseFlag(3);
		if (MAIN_D_80134F81 == 0) {
			if (MAIN_D_80135011 != 5) {
				box = MAIN_func_800FCC40();
				amount = getCardAmount(MAIN_D_80134F78);
				amount = amount + 1u;
				setCardAmount(MAIN_D_80134F78, amount);
				MONEY -= MAIN_D_8012FFC4[MAIN_D_8012FFD9[MAIN_D_80134F78 * 4]];
				SCRIPT_STATE_PTR->smth[box->cursor] = 0xff;
				MAIN_D_80134F84 = 1;
				SELECTION_MENU_STATE = 4;
				MAIN_func_800FDFB4();
			} else {
				writePStat(0xfe, MAIN_D_80134F78);
				unsetTrigger(3);
				SELECTION_MENU_STATE = 4;
				playSound(0, 3);
			}
		} else {
			playSound(0, 4);
		}
	}
}

void MAIN_func_800FBAA0(void)
{
	MenuTextLayout layout;
	int16_t *entry;
	int16_t rowPx;
	int16_t x;
	int16_t y;
	int32_t i;

	layout = MAIN_D_8012FE04;
	entry = layout.v;
	rowPx = TEXT_BOX_DATA[3].vramRow * 12;
	x = UI_BOX_DATA[3].finalPos.x + 4;
	y = UI_BOX_DATA[3].finalPos.y + 3;
	i = 0;
	while (i < 3) {
		renderString(0, x + entry[2], y + entry[3], entry[4], 0xc,
			     entry[0], rowPx + entry[1], 3, 1);
		i++;
		entry += 5;
	}
	renderSelectionCursor(x + 8 + MAIN_D_80134F81 * 47, y + 0x12, 0x28,
			      0xe, 3);
}

void MAIN_func_800FBC00(void)
{
	int32_t line;
	uint8_t saved;

	if (MAIN_D_80134F84 != 0) {
		MAIN_D_80134F84 = 0;

		if (MAIN_D_80134F88 != 0) {
			line = 6;
		} else {
			line = 7;
		}

		saved = ACTIVE_INSTRUCTION;
		showShopkeeperTextbox(line, 0xff, 2);
		ACTIVE_INSTRUCTION = saved;
	}
}

uint8_t *doShopkeeperLine(int32_t idx)
{
	ShopkeeperIdTable shopkeeperIds;
	int32_t section;
	int32_t pstat;
	int32_t i;
	int32_t npcType;

	shopkeeperIds = MAIN_D_8012FE5C;
	pstat = readPStat(0xfe) & 0xff;

	if (pstat == 0xff) {
		section = 0x4ce;
	} else {
		pstat = scriptIdToEntityId(pstat) & 0xff;
		if (pstat != 0xff) {
			npcType =
				*(int32_t *)(&NPC_ENTITIES[pstat - 2]) & 0xff;
			for (i = 0; (uint32_t)i < 0xb; i++) {
				if (npcType == shopkeeperIds.b[i]) {
					section = (i + 0x4c4) & 0xffff;
					goto done;
				}
			}
		}
		section = 0x4c4;
	}
done:
	return resolveMapHeadEntry(section, idx);
}

uint8_t *resolveMapHeadEntry(int32_t section, int32_t idx)
{
	uint8_t *script;
	uint8_t *sectionPtr;
	uint8_t *offsetPtr;

	script = getScript(0);
	sectionPtr = getScriptSection(script, section);
	offsetPtr = sectionPtr + idx * 4;
	offsetPtr = (uint8_t *)(offsetPtr + 2);

	return script + *(uint16_t *)offsetPtr + 2;
}

INCLUDE_ASM("asm/main/nonmatchings/script_common", renderMonochromonMoodBubble);

void initialKeyInputs(void)
{
	MAIN_D_80135024 = 0;
	MAIN_D_80135020 = -1;
	MAIN_D_8013501C = 0;
	MAIN_D_80135016 = 0;
}

void processInput(void)
{
	uint32_t held;

	MAIN_D_80135020 |= ~POLLED_INPUT;
	held = POLLED_INPUT & MAIN_D_8013501C;
	if (held == 0) {
		MAIN_D_80135016 = 0;
	}

	if (MAIN_D_8013501C != 0) {
		if (held == (POLLED_INPUT_PREVIOUS & MAIN_D_8013501C)) {
			if (MAIN_D_80135016 == 8) {
				MAIN_D_80135016 = 6;
			} else if (MAIN_D_80135016 != 6) {
				held = 0;
			}

			MAIN_D_80135016 += 1;
		} else {
			MAIN_D_80135016 = 0;
			MAIN_D_80135020 |= MAIN_D_8013501C;
			held = 0;
		}
	}

	MAIN_D_80135024 = held | (POLLED_INPUT & MAIN_D_80135020);
}

int32_t isKeyDown(uint32_t key)
{
	if ((MAIN_D_80135024 & key) == 0) {
		return 0;
	}

	MAIN_D_80135020 &= ~key;

	return 1;
}

void MAIN_func_800FC08C(void)
{
	MAIN_D_80135028 = 1;
}

int32_t isXPressedAfterDialogue(void)
{
	if (MAIN_D_80135028 != 0) {
		if ((POLLED_INPUT & 0x40) != 0) {
			return 0;
		}

		MAIN_D_80135028 = 0;
	}
	return 1;
}

void setInputRepeatMask(uint32_t mask)
{
	MAIN_D_8013501C = mask;
}

void handleItemLoss(void)
{
	uint8_t *pool;
	uint8_t size;
	uint8_t k;
	int32_t count;
	int32_t lose;
	uint8_t n;
	uint8_t slot;
	uint8_t recycleId;
	int32_t idx;

	size = INVENTORY_SIZE[0];
	pool = allocateArray(size);
	k = 0;
	count = 0;
	while (k < size) {
		if (INVENTORY_ITEM_TYPES[k] != 0xff) {
			pool[count] = k;
			count += 1;
		}
		k++;
	}

	lose = count * 30 / 100;
	writePStat(0xc8, lose);
	if (lose == 0) {
		freeArray((uint32_t *)pool);
		return;
	}

	for (n = 0; n < lose; n++) {
		k = 0;
		count = 0;
		while (k < size) {
			if (INVENTORY_ITEM_TYPES[k] != 0xff) {
				pool[count] = k;
				count += 1;
			}
			k++;
		}

		slot = pool[random(count)];
		recycleId = INVENTORY_ITEM_TYPES[slot];
		recycleId = getRecycleId(recycleId);
		if (recycleId != 0xff) {
			SCRIPT_STATE_PTR->smth[recycleId + 6] +=
				INVENTORY_ITEM_AMOUNTS[slot];
			idx = recycleId + 6;
			if (SCRIPT_STATE_PTR->smth[idx] >= 0x64) {
				SCRIPT_STATE_PTR->smth[idx] = 0x63;
			}
		}

		removeItem(INVENTORY_ITEM_TYPES[slot], 0x63);
	}

	freeArray((uint32_t *)pool);
}

uint8_t getRecycleId(uint8_t value)
{
	uint8_t i;

	for (i = 0; i < 0x4e; i++) {
		if (value == MAIN_D_8012FE78[i]) {
			return i;
		}
	}

	return 0xff;
}

INCLUDE_ASM("asm/main/nonmatchings/script_common", dailyPStatTrigger);

int32_t isPartnerBaby(void)
{
	BabyTypeTable babyTypes;
	int32_t i;
	int32_t partnerType;

	babyTypes = MAIN_D_801345B4;
	partnerType = PARTNER_ENTITY.digimonEntity.entity.type & 0xff;

	for (i = 0; (uint32_t)i < 8; i++) {
		if (partnerType == babyTypes.b[i]) {
			return 1;
		}
	}

	return 0;
}

void tickSellItemBox(void)
{
	RECT origin;
	uint8_t owner;
	uint8_t qty;
	int32_t unitPrice;
	int32_t savings;
	int8_t q;

	if (UI_BOX_DATA[3].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (MAIN_D_80134F81 != MAIN_D_80134F80 && isKeyDown(0x1000)) {
		if (POLLED_INPUT & 0x20) {
			MAIN_D_80134F81 += 10;
			if (MAIN_D_80134F81 > MAIN_D_80134F80) {
				MAIN_D_80134F81 -= 10;
			}
		} else {
			MAIN_D_80134F81 += 1;
		}

		playSound(0, 2);
		MAIN_func_800FB700();

		return;
	}
	if (MAIN_D_80134F81 != 1 && isKeyDown(0x4000)) {
		if (POLLED_INPUT & 0x20) {
			q = MAIN_D_80134F81;
			q -= 10;
			if (q <= 0) {
				q += 10;
			}
			MAIN_D_80134F81 = q;
		} else {
			MAIN_D_80134F81 -= 1;
		}

		playSound(0, 2);
		MAIN_func_800FB700();

		return;
	}

	if (isKeyDown(0x10)) {
		triggerBoxCloseFlag(3);
		playSound(0, 4);
	} else if (isKeyDown(0x80)) {
		MAIN_D_80134F81 = MAIN_D_80134F80;
		playSound(0, 3);
	} else if (isKeyDown(0x40)) {
		if (MAIN_D_80135011 < 3) {
			if (MAIN_D_80135011 == 1) {
				MONEY += MAIN_D_80134F81 * MAIN_D_80134F7C;
				if (MONEY >= 0xf4240) {
					MONEY = 0xf423f;
				}

				removeItem(MAIN_D_80134F78, MAIN_D_80134F81);
				owner = readPStat(0xfe);
			} else {
				unitPrice = MAIN_D_80134F7C;
				savings = isPartnerBaby();
				if (savings != 0) {
					unitPrice = unitPrice * 90 / 100;
					savings = MAIN_D_80134F7C;
					savings -= unitPrice;
					MAIN_D_8013500C =
						MAIN_D_80134F81 * savings;
				}

				qty = MAIN_D_80134F81;
				MONEY -= qty * unitPrice;
				giveItem(MAIN_D_80134F78, qty);

				if (MAIN_D_80135011 == 2) {
					owner = getRecycleId(MAIN_D_80134F78);
					SCRIPT_STATE_PTR->smth[owner + 6] -=
						MAIN_D_80134F81;
				}

				owner = 0xfd;
			}
			SELECTION_MENU_STATE = 7;
		} else {
			MONEY += MAIN_D_80134F81 * MAIN_D_80134F7C;
			if (MONEY >= 0xf4240) {
				MONEY = 0xf423f;
			}

			owner = getCardAmount(MAIN_D_80134F78);
			owner = (uint32_t)owner - MAIN_D_80134F81;
			setCardAmount(MAIN_D_80134F78, owner);
			owner = readPStat(0xfe);
			SELECTION_MENU_STATE = 4;
		}

		setupBoxOrigin(owner, &origin);
		triggerBoxCloseFlag(3);
		closeTextbox(3, &origin);
		MAIN_D_80134F84 = 1;
		MAIN_func_800FDFB4();
	}

	MAIN_func_800FB700();
}

INCLUDE_ASM("asm/main/nonmatchings/script_common", MAIN_func_800FEEF0);

void MAIN_func_800FC508(void)
{
	int32_t npcId;
	int32_t i;
	int32_t trig;
	int32_t found;

	npcId = readPStat(0xfe) & 0xff;

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0x100, 6, 0xb2, 0x18,
				      6, 0x5a);
		initializeItemMenuBox(&MAIN_D_80134F6C,
				      INVENTORY_SIZE[0] << 1, 6, 0xd2, 0x18,
				      6, 0x5a);
		MAIN_D_80134F70 = 0;

		found = 0;
		for (i = 0, trig = 0x180; i < 0x80; i++, trig++) {
			if (isTriggerSet(trig)) {
				found = 1;
				break;
			}
		}

		MAIN_D_80134F74 = 0;
		if (found) {
			showShopkeeperTextbox(0, npcId, 0);
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 3;
			SCRIPT_STATE_3 = 1;
		} else {
			showShopkeeperTextbox(1, npcId, 0);
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 2;
			SCRIPT_STATE_3 = 1;
		}
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F6C);
		destroyItemMenuBox(&MAIN_D_80134F68);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		MAIN_func_800FC968(1);
		MAIN_func_800FCA14(2, 0xfd, 3, &MAIN_D_80134F70);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		MAIN_D_80135011 = 0;
		MAIN_func_800FA834();
		MAIN_func_800FCA3C();
		showShopkeeperTextbox(8, npcId, 0);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 5:
		setInputRepeatMask(0x5000);
		MAIN_D_80135011 = 1;
		MAIN_func_800FAA68();
		MAIN_func_800FCA3C();
		showShopkeeperTextbox(9, npcId, 0);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 6:
		triggerBoxCloseFlag(2);

		if (MAIN_D_80134F74) {
			showShopkeeperTextbox(4, npcId, 0);
		} else {
			showShopkeeperTextbox(5, npcId, 0);
		}

		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	case 7:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);

		if (MAIN_D_80135011 == 0 && isPartnerBaby()) {
			showShopkeeperTextbox(0xd, npcId, 0);
		} else {
			showShopkeeperTextbox(0xa, npcId, 0);
		}

		SCRIPT_STATE_4 = 3;
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 1;
		MAIN_D_80134F74 = 1;
		break;
	case 8:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showShopkeeperTextbox(0xb, npcId, 0);
		SELECTION_MENU_STATE = 9;
		break;
	case 9:
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	}
}

void initializeItemMenuBox(ItemMenuBox **box, int32_t bufSize, int32_t rows,
			   int32_t x, uint8_t y, uint8_t w, uint8_t h)
{
	*box = (ItemMenuBox *)allocateArray(0x20);
	(*box)->buf = allocateArray(bufSize);
	(*box)->isOpen = 0;
	(*box)->visibleRows = rows;
	(*box)->rect.x = x;
	(*box)->rect.y = y;
	(*box)->rect.w = w;
	(*box)->rect.h = h;
}

void showShopkeeperTextbox(int32_t idx, int32_t owner, int32_t boxId)
{
	showMapHeadTextbox(idx, owner, boxId, 0xff);
}

void destroyItemMenuBox(ItemMenuBox **box)
{
	freeArray((uint32_t *)(*box)->buf);
	freeArray((uint32_t *)*box);
	*box = 0;
}

void MAIN_func_800FC968(int32_t showBits)
{
	RECT rect;
	RECT origin;

	MAIN_D_80134F84 = 1;
	MAIN_D_80134F88 = showBits;
	if (UI_BOX_DATA[2].state == 1) {
		return;
	}

	setupBoxOrigin(0xfd, &origin);
	rect.x = -0x98;
	rect.y = -0x62;
	rect.w = 0x52;
	rect.h = 0x21;
	createTextbox(2, 0xe1, &rect, &origin, MAIN_func_800FBC00,
		      MAIN_func_800FBC58);
	registerTextbox(2, 8, 1, 0, 0);
	MAIN_func_800FBC00();
}

void MAIN_func_800FCA14(int32_t idx, int32_t owner, int32_t boxId,
			int32_t *outSelection)
{
	showMapheadSelection(idx, owner, boxId, outSelection, 0xff);
}

void MAIN_func_800FCA3C(void)
{
	RECT rect;
	RECT origin;
	int32_t boxId;
	ItemMenuBox *result;
	int16_t *dims;

	if (MAIN_D_80135011 == 1 || MAIN_D_80135011 == 5) {
		boxId = 0xfd;
	} else {
		boxId = readPStat(0xfe) & 0xff;
	}

	setupBoxOrigin(boxId, &origin);
	result = MAIN_func_800FCC40();
	dims = (int16_t *)((uint8_t *)MAIN_D_8013027C + MAIN_D_80135011 * 8);
	rect.x = dims[0];
	rect.y = dims[1];
	rect.w = dims[2];
	rect.h = dims[3];
	createTextbox(1, 0xf1, &rect, &origin, MAIN_func_800FAB30,
		      MAIN_func_800FADE0);
	registerTextbox(1, 9, 6, 1, 0);
	MAIN_func_800FCC98(result, 1, 9);
	MAIN_func_800FCCFC(result, 9, 0);
	MAIN_D_8013500C = 0;
}

void MAIN_func_800FCB3C(void)
{
	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F6C,
				      INVENTORY_SIZE[0] << 1, 6, 0x9a, 0x18,
				      6, 0x5a);

		if (MAIN_func_800FAA68()) {
			SELECTION_MENU_STATE = 3;
			SCRIPT_STATE_3 = 0;
		} else {
			setTrigger(3);
			writePStat(0xfe, 0xff);
			SELECTION_MENU_STATE = 2;
			SCRIPT_STATE_3 = 0;
		}
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F6C);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0x5000);
		MAIN_D_80135011 = 5;
		MAIN_func_800FCA3C();
		SELECTION_MENU_STATE = 1;
		break;
	case 4:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		SELECTION_MENU_STATE = 2;
		break;
	}
}

ItemMenuBox *MAIN_func_800FCC40(void)
{
	switch (MAIN_D_80135011) {
	case 0:
	case 2:
	case 3:
	case 4:
	case 6:
		return MAIN_D_80134F68;
	case 1:
	case 5:
	case 7:
		return MAIN_D_80134F6C;
	}

	return 0;
}

void MAIN_func_800FCC98(ItemMenuBox *box, int32_t boxId, int32_t startRow)
{
	int32_t i;

	if (box->isOpen == 0) {
		box->isOpen = 1;
		box->boxId = boxId;
		box->topRow = 0;
		box->cursor = 0;
		box->prevTopRow = 0;
		box->prevCursor = 0;
		for (i = 0; i < box->visibleRows; i++) {
			box->itemRow[i] = startRow + i;
		}
	}
}

void MAIN_func_800FCCFC(ItemMenuBox *box, int32_t startRow, int32_t style)
{
	TextBoxData *entry;
	uint8_t *p;
	int32_t rows;
	int32_t last;
	int32_t i;

	entry = &MAIN_D_801BE80C.box[box->boxId];
	rows = box->itemCount - box->topRow;
	if (box->visibleRows < rows) {
		rows = box->visibleRows;
	}

	if (rows != 0) {
		i = 0;
		last = startRow + rows - 1;
		while (i < rows) {
			if (box->itemRow[i] == last) {
				if (style == 0) {
					MAIN_func_800FE704(box, i, 1);
				} else if (style == 1) {
					MAIN_func_800FE9F0(box, i, 1);
				} else if (style == 2) {
					MAIN_func_800FEC30(box, i, 1);
				} else if (style == 3) {
					MAIN_func_800FED64(box, i, 1);
				} else {
					MAIN_func_800FEEF0(box, i, 1);
				}
			} else {
				if (style == 0) {
					MAIN_func_800FE704(box, i, 0);
				} else if (style == 1) {
					MAIN_func_800FE9F0(box, i, 0);
				} else if (style == 2) {
					MAIN_func_800FEC30(box, i, 0);
				} else if (style == 3) {
					MAIN_func_800FED64(box, i, 0);
				} else {
					MAIN_func_800FEEF0(box, i, 0);
				}
			}

			i++;
		}
	} else {
		p = TEXT_BUFFERS_PTR + (startRow << 6);
		if (entry->vramMode == 2) {
			p += 0x20;
		}

		if (entry->doubleBuffered == 1) {
			p = (uint8_t *)(p + (((entry->backPage ^ 1) * entry->vramRows) << 6));
		}

		*p++ = 0;
		*p = 0;
	}

	entry->pageReady = 1;
	entry->writeCount++;
}

int32_t MAIN_func_800FCF88(ItemMenuBox *box)
{
	if (box == 0) {
		return 1;
	}

	return MAIN_func_800FE650(box->boxId);
}

int32_t MAIN_func_800FCFB8(RECT *origin)
{
	ItemMenuBox *box;
	RECT rect;
	int32_t idx;
	int16_t boxY;
	uint8_t id;
	uint8_t amount;
	uint8_t room;

	box = MAIN_func_800FCC40();
	idx = (box->topRow + box->cursor) * 2;
	MAIN_D_80134F78 = box->buf[idx];
	if (MAIN_D_80134F78 == 0xff) {
		goto fail;
	}

	id = MAIN_D_80134F78;
	amount = box->buf[idx + 1];

	if (MAIN_D_80135011 < 3) {
		MAIN_D_80134F7C = ITEM_PARA[id].value;
		if (MAIN_D_80135011 == 1) {
			if ((amount & 0x80) == 0) {
				goto fail;
			}

			MAIN_D_80134F7C >>= 1;
			MAIN_D_80134F80 = amount & 0x7f;
		} else {
			if (MAIN_D_80135011 == 0) {
				if (amount == 0) {
					goto fail;
				}
				MAIN_D_80134F80 = 0x63;
			} else {
				if ((amount & 0x80) == 0) {
					goto fail;
				}
				MAIN_D_80134F80 = amount & 0x7f;
			}

			room = 0x63 - getItemCount(id);
			if (room < MAIN_D_80134F80) {
				MAIN_D_80134F80 = room;
			}

			idx = MAIN_D_80134F7C;
			if (MAIN_D_80134F80 * idx > MONEY) {
				MAIN_D_80134F80 = MONEY / MAIN_D_80134F7C;
			}
		}
	} else {
		MAIN_D_80134F7C =
			MAIN_D_8012FFC4[MAIN_D_8012FFD9[id * 4]] >> 1;
		MAIN_D_80134F80 = amount;
	}

	MAIN_D_80134F81 = 1;
	MAIN_D_80134F82 = 1;
	MAIN_func_800FDF84();
	boxY = UI_BOX_DATA[1].finalPos.y;
	origin->x += UI_BOX_DATA[1].finalPos.x;
	origin->y += (boxY + box->cursor * 18);
	rect.x = -0x41;
	rect.y = -0x2a;
	rect.w = 0x82;
	rect.h = 0x53;
	createTextbox(3, 0xc1, &rect, origin, tickSellItemBox,
		      renderSellItemBox);
	registerTextbox(3, MAIN_D_80135018, 1, 0, 0);
	MAIN_func_800FB700();
	playSound(0, 3);

	return 1;
fail:
	playSound(0, 0xb);

	return 0;
}

int32_t MAIN_func_800FD244(RECT *origin)
{
	ItemMenuBox *box;
	RECT rect;
	int32_t idx;
	int16_t boxY;
	uint8_t amount;

	box = MAIN_func_800FCC40();
	idx = (box->topRow + box->cursor) * 2;
	MAIN_D_80134F78 = box->buf[idx];
	if (MAIN_D_80134F78 == 0xff) {
		goto fail;
	}

	amount = box->buf[idx + 1];
	if (MAIN_D_80135011 == 5) {
		if ((amount & 0x80) == 0) {
			goto fail;
		}
	} else {
		if (amount == 0) {
			goto fail;
		}
	}

	MAIN_func_800FDF84();
	boxY = UI_BOX_DATA[1].finalPos.y;
	origin->x += UI_BOX_DATA[1].finalPos.x;
	origin->y += (boxY + box->cursor * 18);
	rect.x = -0x38;
	rect.y = -0x15;
	rect.w = 0x70;
	rect.h = 0x2a;
	createTextbox(3, 0xc1, &rect, origin, MAIN_func_800FB8D4,
		      MAIN_func_800FBAA0);
	registerTextbox(3, MAIN_D_80135018, 2, 0, 0);
	showMapHeadTextbox(0, 0xff, 3, 0x4d8);
	MAIN_D_80134F81 = 0;
	playSound(0, 3);

	return 1;
fail:
	playSound(0, 0xb);

	return 0;
}

void MAIN_func_800FD3DC(ItemMenuBox *box, int32_t startRow, int32_t style)
{
	MAIN_func_800FF310(box);
	MAIN_func_800FCCFC(box, startRow, style);
}

void MAIN_func_800FD428(ItemMenuBox *box, int32_t style)
{
	int32_t j;
	int32_t k;
	uint8_t last;

	if (box->topRow + box->cursor != 0) {
		if (box->cursor == 0) {
			box->topRow -= 1;
			j = box->visibleRows - 1;
			last = box->itemRow[j];

			k = j;
			while (k != 0) {
				box->itemRow[k] = box->itemRow[k - 1];
				k--;
			}

			box->itemRow[0] = last;
			MAIN_func_800FF0FC(box, style);
		} else {
			box->cursor -= 1;
		}
		playSound(0, 2);
	} else {
		playSound(0, 0xb);
	}
}

void MAIN_func_800FD4E8(ItemMenuBox *box, int32_t startRow, int32_t style)
{
	MAIN_func_800FF2A8(box);
	MAIN_func_800FCCFC(box, startRow, style);
}

void MAIN_func_800FD534(ItemMenuBox *box, int32_t style)
{
	int32_t k;
	uint8_t first;
	int32_t d;

	box->cursor += 1;

	if (box->topRow + box->cursor < box->itemCount) {
		if (box->cursor == box->visibleRows) {
			box->topRow += 1;
			box->cursor -= 1;
			k = 1;
			first = box->itemRow[0];
			d = 0;

			while (k < box->visibleRows) {
				box->itemRow[d] = box->itemRow[k];
				k++;
				d++;
			}

			box->itemRow[box->visibleRows - 1] = first;
			MAIN_func_800FF0FC(box, style);
		}
		playSound(0, 2);
	} else {
		box->cursor -= 1;
		playSound(0, 0xb);
	}
}

int32_t MAIN_func_800FD61C(ItemMenuBox *box, RECT *origin, int32_t uiBoxId)
{
	RECT rect;
	uint8_t *out;
	uint8_t item;
	int16_t boxY;

	item = box->buf[(box->topRow + box->cursor) * 2];
	if (item == 0xff) {
		return 0;
	}

	MAIN_func_800FDF84();
	boxY = UI_BOX_DATA[uiBoxId].finalPos.y;
	origin->x += UI_BOX_DATA[uiBoxId].finalPos.x;
	origin->y += (boxY + box->cursor * 18);
	rect.x = -0x84;
	rect.y = -0xb;
	rect.w = 0x108;
	rect.h = 0x16;
	createTextbox(3, 0xc1, &rect, origin, MAIN_func_800FB070,
		      MAIN_func_800FB0CC);
	registerTextbox(3, MAIN_D_80135018, 1, 0, 0);
	strcpy((out = TEXT_BUFFERS_PTR + (MAIN_D_80135018 << 6), out),
	       ITEM_DESC_PTR[item]);
	out += strlen(ITEM_DESC_PTR[item]);
	*out++ = 0;
	*out = 0;
	TEXT_BOX_DATA[3].pageReady = 1;
	TEXT_BOX_DATA[3].writeCount++;

	return 1;
}

void MAIN_func_800FD7D8(int32_t boxId, int32_t idx, int16_t x, int16_t y)
{
	BoxUVTable uvs;
	POLY_FT4 poly;
	int16_t *e;

	uvs = MAIN_D_8012FE24;
	e = &uvs.v[idx * 4];
	SetPolyFT4(&poly);
	poly.tpage = 5;
	poly.clut = GetClut(0x60, 0x1ec);
	poly.r0 = 0x80;
	poly.g0 = 0x80;
	poly.b0 = 0x80;
	setUVDataPolyFT4(&poly, e[0], e[1], e[2], e[3]);
	setPosDataPolyFT4(&poly, x, y, e[2], e[3]);

	GsSortPoly(&poly, ACTIVE_ORDERING_TABLE, (6 - boxId));
}

INCLUDE_ASM("asm/main/nonmatchings/script_common", MAIN_func_800FD8D4);

INCLUDE_ASM("asm/main/nonmatchings/script_common", MAIN_func_800FDC5C);

void MAIN_func_800FDF84(void)
{
	uint8_t *box = (uint8_t *)TEXT_BOX_DATA;

	MAIN_D_80135018 = *(int32_t *)(box + 0x20);
	MAIN_D_80135018 = (*(int32_t *)(box + 0x18) ^ 1) * *(int32_t *)(box + 0x24);
}

INCLUDE_ASM("asm/main/nonmatchings/script_common", MAIN_func_800FDFB4);

INCLUDE_ASM("asm/main/nonmatchings/script_common", renderHorizontalLine);

INCLUDE_ASM("asm/main/nonmatchings/script_common", MAIN_func_800FE150);

INCLUDE_ASM("asm/main/nonmatchings/script_common", MAIN_func_800FE258);

void showMapHeadTextbox(int32_t idx, int32_t owner, int32_t boxId,
			int32_t section)
{
	uint8_t *savedCursor;

	if (boxId == 0 && owner != 0xfe) {
		setDialogueOwner(owner);
	}

	savedCursor = MAIN_D_80134FDC;

	if (section == 0xff) {
		MAIN_D_80134FDC = doShopkeeperLine(idx);
	} else {
		MAIN_D_80134FDC = resolveMapHeadEntry(section, idx);
	}

	if (owner == 0xfe) {
		owner = 0xff;
	}

	MAIN_func_80101EF8(boxId, owner);
	ACTIVE_INSTRUCTION = 0x64;
	MAIN_D_80134FDC = savedCursor;
}

void lostAllLives(void)
{
	uint8_t *pool;
	int16_t i;
	int16_t count;
	int16_t pick;
	int32_t j;

	switch (SELECTION_MENU_STATE) {
	case 0:
		break;
	case 1:
		return;
	case 2:
		goto state2;
	case 3:
		goto state3;
	default:
		return;
	}

	i = 0;
	count = 0;
	while (i < 0x3a) {
		if (hasMove(i) != 0) {
			count += 1;
		}
		i++;
	}

	count = (10 - TAMER_ENTITY.tamerLevel) * 2 * (count * 100) / 100;
	if (count % 100 >= 0x32) {
		count += 100;
	}

	count = count / 100;
	if (count == 0) {
		ACTIVE_INSTRUCTION = 0;
		return;
	}

	writePStat(0xf4, count);
	SELECTION_MENU_STATE = 2;

	return;
state2:
	pool = allocateArray(0x3a);
	j = 0;
	i = 0;
	while (i < 0x3a) {
		if (hasMove(i) != 0) {
			pool[j] = i;
			j += 1;
		}
		i++;
	}

	pick = random(j);
	pick = pool[pick];
	unlearnMove(pick);
	writePStat(0xf3, pick);
	freeArray((uint32_t *)pool);
	showMapHeadTextbox(7, 0xff, 0, 0x4d8);
	SELECTION_MENU_STATE = 1;
	SCRIPT_STATE_4 = 3;
	SCRIPT_STATE_3 = 1;

	return;
state3:
	pick = readPStat(0xf4);
	pick -= 1;
	if (pick == 0) {
		closeBox(0);
		ACTIVE_INSTRUCTION = 0;
		return;
	}

	writePStat(0xf4, pick);
	SELECTION_MENU_STATE = 2;
}

int32_t MAIN_func_800FE650(uint8_t a)
{
	TextBoxData *entry;

	entry = &MAIN_D_801BE80C.box[a];

	if (entry->doubleBuffered == 0) {
		return 0;
	}

	if (entry->registered == 0) {
		return 1;
	}

	if (entry->writeCount != entry->renderCount) {
		return 1;
	}

	if (entry->pageReady == 0) {
		return 0;
	}

	entry->backPage ^= 1;
	entry->flipCount++;
	entry->pageReady = 0;
}

void MAIN_func_800FE704(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	char *name;
	uint8_t type;
	uint8_t amount;
	int32_t value;
	int32_t sum;
	int32_t idx;
	int32_t i;

	out = MAIN_func_800FF444((uint8_t *)box, row);
	sum = box->topRow + row;
	i = sum * 2;
	idx = i;
	type = box->buf[idx];
	amount = box->buf[idx + 1];

	if (type != 0xff) {
		*out++ = 1;
		if (MAIN_D_80135011 == 0 || MAIN_D_80135011 == 7) {
			if (amount != 0) {
				*out++ = 1;
			} else {
				*out++ = 3;
			}
		} else {
			if ((amount & 0x80) != 0) {
				*out++ = 1;
			} else {
				*out++ = 3;
			}
		}

		strcpy(out, name = ITEM_PARA[type].name);
		out += strlen(name);
		*out++ = 0xf;
		*out++ = 0;

		if (MAIN_D_80135011 != 5) {
			*out++ = 0x16;
			*out++ = 0;
			if (MAIN_D_80135011 != 7) {
				value = ITEM_PARA[type].value;
				if (MAIN_D_80135011 == 1) {
					if ((amount & 0x80) != 0) {
						value >>= 1;
					} else {
						for (i = 0; i < 4; i++) {
							*out++ = 0x81;
							*out++ = 0x7c;
						}
						*out++ = 0xf;
						*out++ = 0;
						goto amountPart;
					}
				}
			} else {
				value = ITEM_PARA[type].meritValue;
			}

			out = intToStringSJIS(out, value, 4, 0);
			*out++ = 0xf;
			*out++ = 0;
		}
amountPart:
		if (MAIN_D_80135011 == 1) {
			out = intToStringSJIS(out, amount & 0x7f, 2, 0);
		}
		if ((MAIN_D_80135011 != 0) && (MAIN_D_80135011 != 7) &&
		    (MAIN_D_80135011 != 1)) {
			*out++ = 0x1c;
			*out++ = 0;
			out = intToStringSJIS(out, amount & 0x7f, 2, 0);
		}
	}

	terminateString(out, isLast);
}

void MAIN_func_800FE9F0(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	char *name;
	uint8_t type;
	uint8_t amount;
	int32_t idx;
	int32_t value;

	out = MAIN_func_800FF444((uint8_t *)box, row);
	idx = (box->topRow + row) * 2;
	type = box->buf[idx];
	amount = box->buf[idx + 1];

	if (type != 0xff) {
		*out++ = 1;
		if (MAIN_D_80135011 == 3) {
			if (amount != 0) {
				if (getCardAmount(type) == 0) {
					*out++ = 7;
				} else {
					*out++ = 1;
				}
			} else {
				*out++ = 3;
			}
		} else {
			*out++ = 1;
		}

		strcpy(out, name = DIGIMON_DATA[((uint8_t *)CARD_DATA)[type * 4]].name);
		out += strlen(name);
		*out++ = 0x17;
		*out++ = 0;
		*out++ = 0xf;
		*out++ = 0;
		value = MAIN_D_8012FFC4[MAIN_D_8012FFD9[type * 4]];

		if (MAIN_D_80135011 == 3) {
			out = intToStringSJIS(out, value, 4, 0);
		} else {
			if (MAIN_D_80135011 != 6) {
				out = intToStringSJIS(out, value >> 1, 4, 0);
				*out++ = 0xf;
				*out++ = 0;
			}
			out = intToStringSJIS(out, amount, 2, 0);
		}
	}

	terminateString(out, isLast);
}

void MAIN_func_800FEC30(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	uint8_t type;
	int32_t len;

	out = MAIN_func_800FF444((uint8_t *)box, row);
	type = box->buf[(box->topRow + row) * 2];
	*out++ = 1;

	if (type == readPStat(0xf9)) {
		*out++ = 6;
	} else {
		*out++ = 1;
	}

	out = intToStringSJIS(out, type + 1, 2, 0);
	*out++ = 0xf;
	*out++ = 0;
	strcpy(out, MAIN_D_8012FEC8[type]);
	len = strlen(MAIN_D_8012FEC8[type]);
	out += len;
	out = padWithSpaces(out, 0xc, len);
	terminateString(out, isLast);
}

void MAIN_func_800FED64(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	uint8_t raw;
	uint8_t nameId;
	int32_t len;

	out = MAIN_func_800FF444((uint8_t *)box, row);
	raw = box->buf[(box->topRow + row) * 2];
	*out++ = 1;

	if ((raw & 0x80) != 0) {
		*out++ = 1;
	} else {
		*out++ = 3;
	}

	raw &= 0x7f;
	nameId = MAP_ENTRIES[MAIN_D_8013024C[raw].mapId].loadingName;
	strcpy(out, MAP_NAME_PTR[nameId]);
	len = strlen(MAP_NAME_PTR[nameId]);
	out += len;
	out = padWithSpaces(out, 0xc, len);
	*out++ = 0xf;
	*out++ = 0;
	*out++ = 0x1b;
	*out++ = 0;
	out = intToStringSJIS(out, MAIN_D_8013024C[raw].cost, 4, 0);
	terminateString(out, isLast);
}

void MAIN_func_800FF0FC(ItemMenuBox *box, int32_t style)
{
	TextBoxData *entry;
	uint8_t *p;
	int16_t row;
	int32_t outX;
	int32_t outClut;

	entry = &MAIN_D_801BE80C.box[box->boxId];
	((int32_t *)entry)[6] ^= 1;

	row = box->cursor;

	if (style == 0) {
		MAIN_func_800FE704(box, row, 1);
	} else if (style == 1) {
		MAIN_func_800FE9F0(box, row, 1);
	} else if (style == 2) {
		MAIN_func_800FEC30(box, row, 1);
	} else if (style == 3) {
		MAIN_func_800FED64(box, row, 1);
	} else {
		MAIN_func_800FEEF0(box, row, 1);
	}

	((int32_t *)entry)[6] ^= 1;

	row = box->itemRow[row];
	if (entry->doubleBuffered == 1) {
		row = row + (int16_t)(((int32_t *)entry)[6] *
				      ((int32_t *)entry)[9]);
	}

	getVRAMModeCoords(((int32_t *)entry)[7], &outX, &outClut);

	p = TEXT_BUFFERS_PTR + (row << 6);
	if (outX != 0) {
		p += 0x20;
	}

	drawString2(p, outX, row * 12, 1);
}

INCLUDE_ASM("asm/main/nonmatchings/script_common", MAIN_func_800FF338);

void MAIN_func_800FF2A8(ItemMenuBox *data)
{
	int32_t diff;

	data->prevTopRow = data->topRow;
	data->prevCursor = data->cursor;
	diff = data->itemCount - data->visibleRows;

	if (diff <= 0) {
		data->topRow = 0;
		data->cursor = data->itemCount - 1;
	} else {
		data->topRow = diff;
		data->cursor = data->visibleRows - 1;
	}

	playSound(0, 2);
}

void MAIN_func_800FF310(ItemMenuBox *data)
{
	data->prevTopRow = data->topRow;
	data->prevCursor = data->cursor;
	data->topRow = 0;
	data->cursor = 0;
	playSound(0, 2);
}

uint8_t *MAIN_func_800FF444(uint8_t *data, int32_t index)
{
	TextBoxData *entry;
	uint8_t *row;

	entry = &MAIN_D_801BE80C.box[data[0xc]];
	row = TEXT_BUFFERS_PTR + (data + index)[0x18] * 0x40;

	if (((int32_t *)entry)[7] == 2) {
		row += 0x20;
	}

	if (entry->doubleBuffered == 1) {
		row = (uint8_t *)(row +
				(((((int32_t *)entry)[6] ^ 1) *
				  ((int32_t *)entry)[9]) << 6));
	}

	return row;
}

void terminateString(uint8_t *str, int32_t flag)
{
	*str++ = 1;
	*str++ = 1;

	if (flag != 0) {
		*str++ = 0;
		*str = 0;
	} else {
		*str++ = 0xd;
		*str = 0;
	}
}

uint8_t *padWithSpaces(uint8_t *str, int32_t width, int32_t used)
{
	used = width - (used >> 1);
	while (used != 0) {
		*str++ = 0x81;
		*str++ = 0x40;
		used--;
	}

	return str;
}

INCLUDE_ASM("asm/main/nonmatchings/script_common", renderVerticalLine);

void setDialogueOwner(int32_t owner)
{
	if (MAIN_D_80134FE6 != owner) {
		MAIN_D_80134FE6 = owner;
		MAIN_func_801062F8(owner);
		setupDialogueBox(MAIN_D_80134FE6);
	}
}

void showMapheadSelection(int32_t idx, int32_t owner, int32_t x,
			  int32_t *outSel, uint16_t section)
{
	uint8_t *saved;
	uint16_t rows;

	if (owner != 0xfe) {
		setDialogueOwner(owner);
	} else {
		MAIN_D_80134FE6 = 0xff;
		owner = 0xfd;
	}

	saved = MAIN_D_80134FDC;

	if (section == 0xff) {
		MAIN_D_80134FDC = doShopkeeperLine(idx);
	} else {
		MAIN_D_80134FDC = resolveMapHeadEntry(section, idx);
	}

	MAIN_D_801BE950[0] = x;

	if (*outSel == 0) {
		*outSel = 1;
		MAIN_D_801BE952[0] = 0;
	}

	MAIN_D_801BE956[0] = MAIN_func_80101EF8(0, MAIN_D_80134FE6);
	MAIN_D_801BE956[0] = MAIN_D_801BE956[0] * 12 + 2;

	if (owner != 0xff) {
		MAIN_D_801BE954[0] = 0xd;
	} else {
		MAIN_D_801BE954[0] = 0;
	}

	ACTIVE_INSTRUCTION = 0x64;
	MAIN_D_80134FDC = saved;
}

void renderScriptDialogueBox(void)
{
	int16_t rowPx;
	int16_t x;
	int16_t y;
	int16_t ySave;
	int32_t i;

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

	if (ACTIVE_INSTRUCTION == 0x10) {
		MAIN_func_800FFF80(x, ySave);
	} else if (ACTIVE_INSTRUCTION == 0x64 && SCRIPT_STATE_3 == 2) {
		MAIN_func_800FFF80(x, ySave);
	}

	if (MAIN_D_80134F94 != 0) {
		renderUIBox(0);
	}
}

void setDigimonRaised(int32_t digimonId)
{
	if ((uint32_t)digimonId < 0x3f) {
		setTrigger(digimonId + 0x200);
	}
}

int32_t hasDigimonRaised(int32_t digimonId)
{
	return isTriggerSet(digimonId + 0x200);
}

void unlockMedal(uint16_t medal)
{
	if (medal < 0x13) {
		setTrigger(medal + 0x16c);
	}
}

int32_t hasMedal(uint16_t medal)
{
	return isTriggerSet(medal + 0x16c);
}

void createMonochromonMoodBubble(void)
{
	int32_t a;
	int32_t b;

	a = readPStat(0xf7) & 0xff;
	b = readPStat(0xf8) & 0xff;

	if ((uint32_t)b < 5) {
		if (scriptIdToEntityId(a) != 0xff) {
			MAIN_D_80134F60 = 0x1e;
			MAIN_D_8012FDCE[0] = b * 32;
			addObject(0x1b1, 0, 0, renderMonochromonMoodBubble);
		}
	} else {
		removeObject(0x1b1, 0);
	}
}

void MAIN_func_800FF900(void)
{
	callScriptSection(0, 0x4e4, 0);
}

void checkShopMap(int32_t mapId)
{
	uint8_t local;
	int32_t i;

	if (isTriggerSet(0x29) == 1) {
		unsetTrigger(0x29);
		return;
	}
	for (i = 0; i < 8; i++) {
		if (mapId == MAIN_D_8012FE68[i]) {
			loadDynamicLibrary(SHOP_REL, &local, 0, 0, 0);
			return;
		}
	}
}

void MAIN_func_800FF9AC(void)
{
	loadDynamicLibrary(SHOP_REL, (uint8_t *)&TRN_LOADING_COMPLETE, 1, 0, 0);
}

void checkArenaMap(int32_t mapId)
{
	uint8_t local;
	int32_t i;

	for (i = 0; i < 2; i++) {
		if (mapId == MAIN_D_801345BC[i]) {
			loadDynamicLibrary(DGET_REL, &local, 0, 0, 0);
			return;
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/script_common", MAIN_func_800FFA4C);

void MAIN_func_800FFBB8(int32_t boxId)
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

int32_t MAIN_func_800FFC1C(void)
{
	if (MAIN_func_80100E40(0) != 0) {
		return 0xffff;
	}

	if (isXPressedAfterDialogue() == 0) {
		return 0xffff;
	}

	if (isKeyDown(0x40)) {
		advanceTextbox(0);
		playSound(0, 3);

		return MAIN_D_801BE952[0];
	}

	if (isKeyDown(0x10)) {
		if (isTriggerSet(0x31) == 0) {
			if (ACTIVE_INSTRUCTION != 0x64) {
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

		return 0xffff;
	}

	if (isKeyDown(0x1000)) {
		if (MAIN_D_801BE952[0] == 0) {
			MAIN_D_801BE952[0] = MAIN_D_801BE950[0] - 1;
		} else {
			MAIN_D_801BE952[0] -= 1;
		}

		playSound(0, 2);

		return 0xffff;
	}

	if (isKeyDown(0x4000)) {
		MAIN_D_801BE952[0] += 1;
		if (MAIN_D_801BE952[0] == MAIN_D_801BE950[0]) {
			MAIN_D_801BE952[0] = 0;
		}

		playSound(0, 2);
	}
	return 0xffff;
}

int32_t MAIN_func_800FFDF4(void)
{
	if (MAIN_func_80100E40(0) != 0) {
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
		if (isKeyDown(0x40) == 0) {
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

void MAIN_func_800FFF80(int32_t x, int32_t y)
{
	if (TEXT_BOX_DATA[0].idle == 1) {
		return;
	}

	renderSelectionCursor(x - 1,
			      y + MAIN_D_801BE954[0] + MAIN_D_801BE952[0] * 13 - 2,
			      MAIN_D_801BE956[0], 0xd, 6);
}

void MAIN_func_800FFFF0(void)
{
	if (MAIN_func_80100E40(0) != 0) {
		return;
	}

	if (!isXPressedAfterDialogue()) {
		return;
	}

	if (isKeyDown(0x40)) {
		if (UI_BOX_DATA[0].state != 1) {
			return;
		}

		if (advanceTextbox(0) != 0) {
			triggerBoxCloseFlag(0);
		}

		playSound(0, 3);

		return;
	}

	if (!isKeyDown(0x10)) {
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

INCLUDE_ASM("asm/main/nonmatchings/script_common", MAIN_func_801000E4);

void MAIN_func_8010020C(void)
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

INCLUDE_ASM("asm/main/nonmatchings/script_common", MAIN_func_80100258);

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
void closeTextbox(int32_t boxId, RECT *target)
{
	uint32_t b;

	b = boxId;

	if (UI_BOX_DATA[boxId].state != 0 && UI_BOX_DATA[boxId].state != 3) {
		if ((TEXT_BOX_DATA[b].flags & 0x40) == 0) {
			removeStaticUIBox(boxId);
		} else {
			removeAnimatedUIBox(boxId, target);
		}

		MAIN_func_800FFBB8(boxId);
	}
}

void MAIN_func_8010064C(void)
{
	int32_t i;
	uint8_t flags;

	for (i = 0; i < 6; i++) {
		flags = TEXT_BOX_DATA[i].flags;
		if ((flags & 0xf) != 0) {
			if (flags & 0x40) {
				ACTIVE_INSTRUCTION = 0xff;
				IS_SCRIPT_PAUSED = 0;
			}

			closeBox(i & 0xff);
		}
	}
}

void createTextbox(int32_t boxId, int32_t flags, RECT *rect, RECT *origin,
		   void *tick, void *render)
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

	MAIN_func_800FC08C();
}

void triggerBoxCloseFlag(int32_t boxId)
{
	TextBoxData *e = &MAIN_D_801BE80C.box[boxId];
	uint8_t val = e->flags;
	uint32_t v;

	if ((val & 0xf) != 0) {
		v = val & 0xf0;
		v &= 0xff;
		e->flags = v;
	}
}

void registerTextbox(int32_t boxId, int32_t row, int32_t rows,
		     int32_t doubleBuffer, int32_t mode)
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
	rect.x = vramX;
	rect.y = entry->vramRow * 12;
	rect.w = vramW;
	rect.h = usedRows * 12;
	clearTextSubArea(&rect);
}

int32_t drawString2(uint8_t *str, int16_t x, int16_t y, int32_t flag)
{
	int32_t y2;
	RECT rect;
	uint8_t ch;
	int16_t pos;
	int16_t rem;
	uint16_t adv;
	int32_t save;
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
			save = pos;
			if (rem != 0) {
				rem = (8 - rem) * 12;
				rect.x = x + save;
				rect.y = y;
				rect.w = rem;
				rect.h = 0xc;
				clearTextSubArea(&rect);
				pos = save + rem;
			}
			break;
		case 0x16:
			str++;
			rect.x = x + pos - 6;
			rect.y = y;
			rect.w = 0x69;
			rect.h = 0xc;
			clearTextSubArea(&rect);
			pos = 0x69;
			break;
		case 0x17:
			str++;
			rect.x = x + pos;
			rect.y = y;
			rect.w = 0x69;
			rect.h = 0xc;
			clearTextSubArea(&rect);
			pos = 0x69;
			break;
		case 0x1b:
			str++;
			rect.x = x + pos;
			rect.y = y;
			rect.w = 0xa0;
			rect.h = 0xc;
			clearTextSubArea(&rect);
			pos = 0xa0;
			break;
		case 0x1c:
			str++;
			rect.x = x + pos;
			rect.y = y;
			rect.w = 0x64;
			rect.h = 0xc;
			clearTextSubArea(&rect);
			pos = 0x64;
			break;
		case 0x18:
			str++;
			rect.x = x + pos;
			rect.y = y;
			rect.w = 0x60;
			rect.h = 0xc;
			clearTextSubArea(&rect);
			pos = 0x60;
			break;
		case 0x19:
			str++;
			rect.x = x + pos;
			rect.y = y;
			rect.w = 0x9c;
			rect.h = 0xc;
			clearTextSubArea(&rect);
			pos = 0x9c;
			break;
		case 0x1a:
			str++;
			rect.x = x + pos;
			rect.y = y;
			rect.w = 0xb4;
			rect.h = 0xc;
			clearTextSubArea(&rect);
			pos = 0xb4;
			break;
		case 0xe:
			str++;
			rem = pos / 12 % 0xb;
			save = pos;
			if (rem != 0) {
				rem = (0xb - rem) * 12;
				rect.x = x + save;
				rect.y = y;
				rect.w = rem;
				rect.h = 0xc;
				clearTextSubArea(&rect);
				pos = save + rem;
			}
			break;
		case 0xf:
			str++;
			rect.x = x + pos;
			rect.y = y;
			rect.w = 6;
			rect.h = 0xc;
			clearTextSubArea(&rect);
			pos += 6;
			break;
		case 0xd:
			return 0;
		default:
			if (isAsciiEncoded((char *)&ch) != 0) {
				glyph = swapShortBytes((uint16_t)convertAsciiToJis(ch));
			} else {
				glyph = ch + (*str++ << 8);
			}

			if (glyph == 0x4081) {
				rect.x = x + pos;
				rect.y = y;
				rect.w = 0xc;
				rect.h = 0xc;
				clearTextSubArea(&rect);
			} else {
				y2 = y;
				adv = drawGlyph(glyph, x + pos, y2);
				if (MAIN_D_80134F98 != 0) {
					adv = 0xc;
				}
			}

			pos += adv;
			break;
		}
	}
}

int32_t MAIN_func_80100E40(int32_t boxId)
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

int32_t MAIN_func_800FFF24(void)
{
	TextBoxData *entry;

	entry = TEXT_BOX_DATA;

	if (MAIN_func_80100E40(0) != 0) {
		return 0;
	}

	if (entry->renderCount == entry->flipCount) {
		entry->idle = 1;
		entry->pageReady = 0;
		return 1;
	}

	return 0;
}
int32_t advanceTextbox(int32_t boxId)
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

	rect.x = -0x82;
	rect.y = 0x2a;
	rect.w = 0x106;
	rect.h = 0x3b;
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

	origin->x = pos[0];
	origin->y = pos[1];
	origin->w = 0xa;
	origin->h = 0xa;

	return 1;
}

void tickScriptDialogueBox(void)
{
	int32_t sel;

	if (ACTIVE_INSTRUCTION == 0x10) {
		sel = MAIN_func_800FFC1C();
		if (sel != 0xffff) {
			MAIN_D_80134FDC =
				(uint8_t *)((uint32_t)MAIN_D_801BE948[0] +
					    MAIN_D_801BE952[0] * 2);
			MAIN_D_80134FDC =
				(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR +
					    *(uint16_t *)MAIN_D_80134FDC);
		}
	} else if (ACTIVE_INSTRUCTION == 0x1a) {
		MAIN_func_800FFDF4();
	} else if (ACTIVE_INSTRUCTION == 0x64) {
		switch (SCRIPT_STATE_3) {
		case 0:
			break;
		case 1:
			if (MAIN_func_800FFDF4() == 1) {
				SCRIPT_STATE_3 = 0;
				SELECTION_MENU_STATE = SCRIPT_STATE_4;
				ACTIVE_INSTRUCTION = 0x64;
			}
			break;
		case 2:
			sel = MAIN_func_800FFC1C();
			if (sel != 0xffff) {
				SCRIPT_STATE_3 = 0;
				sel = MAIN_D_801BE952[0];
				SELECTION_MENU_STATE =
					SCRIPT_STATE_4 + (uint16_t)sel;
				ACTIVE_INSTRUCTION = 0x64;
			}
			break;
		case 3:
			if (MAIN_func_800FFF24() == 1) {
				ACTIVE_INSTRUCTION = 0x64;
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
	poly.tpage = GetTPage(0, 1, 0x140, 0x80);
	poly.clut = GetClut(0x60, 0x1f4);
	poly.r0 = 0x80;
	poly.g0 = 0x80;
	poly.b0 = 0x80;
	setUVDataPolyFT4(&poly, u, 0xb7, 0x10, 0xc);
	setPosDataPolyFT4(&poly, x, y, 0x10, 0xc);

	GsSortPoly(&poly, ACTIVE_ORDERING_TABLE, (6 - boxId));
}

void scriptShowSelection(void)
{
	uint8_t optionCount;
	uint16_t height;

	pollNextScriptUByte(&optionCount);

	MAIN_D_801BE950[0] = optionCount;
	MAIN_D_801BE952[0] = 0;
	MAIN_D_801BE948[0] = (int32_t)MAIN_D_80134FDC;
	MAIN_D_80134FDC += (optionCount + 1) * 2;
	MAIN_D_801BE956[0] = showTextbox(0, MAIN_D_80134FE6);
	MAIN_D_801BE956[0] = MAIN_D_801BE956[0] * 12 + 2;
	height = MAIN_D_801BE956[0];

	if (height > 0xf0) {
		MAIN_D_801BE956[0] = 0xf0;
	}

	MAIN_D_80134FDC += 2;
	MAIN_D_801BE94C[0] = (int32_t)MAIN_D_80134FDC;

	if (MAIN_D_80134FE6 != 0xff) {
		MAIN_D_801BE954[0] = 0xd;
	} else {
		MAIN_D_801BE954[0] = 0;
	}

	ACTIVE_INSTRUCTION = 0x10;
}

static void showTextbox__garbage__(void)
{
	uint8_t moveSlot;
	uint8_t objectId;
	uint8_t speed;
	uint8_t targetId;
	uint8_t pad;
	int16_t posX;
	int16_t posY;
	ScriptCameraMovement *slot;

	MAIN_func_801062F8(0xff);
	pollNextScriptUByte(&moveSlot);
	pollNextTwoScriptBytes(&objectId, &speed);
	pollNextTwoScriptBytes(&targetId, &pad);
	pollNextTwoScriptShorts(&posX, &posY);
	moveSlot += 0xc;
	slot = &MAIN_D_801BE6B4[moveSlot];
	slot->type = 0xd;
	slot->entityId = objectId;
	slot->speed = speed;
	slot->target = targetId;
	slot->targetX = posX;
	slot->targetY = posY;
}

uint32_t showTextbox(int32_t boxId, uint32_t speakerId)
{
	TextBoxData *entry;
	uint8_t *base;
	uint8_t *out;
	uint32_t col;
	uint16_t maxCol;
	uint32_t row;
	uint32_t rowOffset;
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
		out += getSpeakerName(speakerId, out);
		*out++ = 1;
		*out++ = 1;
		*out++ = 0xd;
		*out = 0;
		row++;
		out = base + (row << 6);
	}
	rowOffset = row << 6;
top:
	{
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
				col = (col + ((lines >> 1) &
					      0xffff)) & 0xffff;
				goto top;
			case 6:
				MAIN_D_80134FDC++;
				lines = getSpeakerName(0xfc, out);
				out += lines;
				col = (col + ((lines >> 1) &
					      0xffff)) & 0xffff;
				goto top;
			case 7:
				ctrl = *MAIN_D_80134FDC++;
				{
					ctrl = readPStat(ctrl);
					strcpy(out,
					       (const char *)
						       &DIGIMON_DATA[ctrl]);
					lines = strlen(
						(const char *)
							&DIGIMON_DATA[ctrl]);
					out += lines;
					col = (col + ((lines >> 1) &
						      0xffff)) & 0xffff;
				}
				goto top;
			case 8:
				ctrl = *MAIN_D_80134FDC++;
				{
					ctrl = readPStat(ctrl);
					strcpy(out, MOVE_NAMES[ctrl]);
					lines = strlen(MOVE_NAMES[ctrl]);
					out += lines;
					col = (col + ((lines >> 1) &
						      0xffff)) & 0xffff;
				}
				goto top;
			case 9:
				ctrl = *MAIN_D_80134FDC++;
				{
					ctrl = readPStat(ctrl);
					strcpy(out,
					       (const char *)
						       &ITEM_PARA[ctrl]);
					lines = strlen(
						(const char *)
							&ITEM_PARA[ctrl]);
					out += lines;
					col = (col + ((lines >> 1) &
						      0xffff)) & 0xffff;
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
					strcpy(out,
					       MAIN_D_8012FEC8[ctrl]);
					lines = strlen(MAIN_D_8012FEC8[ctrl]);
					out += lines;
					col = (col + ((lines >> 1) & 0xffff)) & 0xffff;
				}
				goto top;
			case 18:
				ctrl = *MAIN_D_80134FDC++;
				{
					ctrl = readPStat(ctrl);
					strcpy(out,
					       MAIN_D_801300E0[ctrl]);
					lines = strlen(MAIN_D_801300E0[ctrl]);
					out += lines;
					col = (col + ((lines >> 1) & 0xffff)) & 0xffff;
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
				if (maxCol < col) {
					maxCol = col;
				}
				col = 0;
				ctrl = *MAIN_D_80134FDC;
				if (ctrl == 0) {
					MAIN_D_80134FDC += 2;
					out -= 2;
					*out = ctrl;
					ctrl = *MAIN_D_80134FDC;
					if (ctrl == 0x1a) {
						goto done;
					}
					entry->pageReady = 1;
					goto done;
				}
				rowOffset += 0x40;
				row++;
				lines = rowOffset;
				out = base + lines;
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
				col = (col + 1) & 0xffff;
				goto top;
			}
		}
	}
done:
	entry->writeCount++;
	ACTIVE_INSTRUCTION = 0x1a;

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
	rect1.x = (0x140 - width) / 2 - 0xa0;
	rect1.y = (0xf0 - height) / 2 - 0x78;
	rect1.w = width;
	rect1.h = height;
	createTextbox(0, boxFlags, &rect1, &origin, MAIN_func_800FFFF0,
		      MAIN_func_801000E4);
	registerTextbox(0, 0, rows, 1, 0);
}

int32_t MAIN_func_80101EF8(int32_t boxId, int32_t speakerId)
{
	TEXT_BOX_DATA[boxId].pageReady = 1;

	return showTextbox(boxId, speakerId);
}

INCLUDE_ASM("asm/main/nonmatchings/script_common", getSpeakerName);

INCLUDE_ASM("asm/main/nonmatchings/script_common", intToStringSJIS);

int32_t scriptIdToEntityId(int32_t scriptId)
{
	uint8_t i;

	if (scriptId == 0xfd) {
		return 0;
	}

	if (scriptId == 0xfc) {
		return 1;
	}

	for (i = 0; i < 8; i++) {
		if (ENTITY_TABLE[i + 2] != 0
		    && scriptId == NPC_ENTITIES[i].scriptId) {
			return (uint8_t)(i + 2);
		}
	}

	return 0xff;
}

void scriptCompareDate(void)
{
	uint16_t trigger;
	uint8_t statIdx;
	uint8_t op;
	uint8_t years;
	uint8_t days;
	uint8_t hours;
	uint8_t minutes;
	uint32_t now;
	uint32_t v1;
	uint32_t v2;
	uint32_t v3;

	pollNextScriptUByte(&statIdx);
	pollNextScriptUShort(&trigger);
	pollNextTwoScriptBytes(&op, &years);
	pollNextTwoScriptBytes(&days, &hours);
	pollNextScriptUByte(&minutes);

	MAIN_D_80134FDC = (uint8_t *)(MAIN_D_80134FDC + 1);
	v1 = readPStat(statIdx);
	v2 = readPStat((uint8_t)(statIdx + 1));
	v3 = readPStat((uint8_t)(statIdx + 2));
	now = dateToSeconds(v1, v2, v3, readPStat((uint8_t)(statIdx + 3)));

	if (scriptCompareValues(op, now, dateToSeconds(years, days, hours,
						       minutes)) != 0) {
		setTrigger(trigger);
	} else {
		unsetTrigger(trigger);
	}
}

int32_t MAIN_func_801022FC(void)
{
	uint16_t u;
	int16_t s;
	uint8_t b1;
	uint8_t b2;
	uint16_t stat;
	int16_t stat2;

	pollNextTwoScriptBytes(&b1, &b2);

	if (b1 != 9) {
		stat = *getStatsPointer(b1);
		pollNextScriptUShort(&u);

		return scriptCompareValues(b2, stat, u);
	}

	stat2 = *getStatsPointer(b1);
	pollNextScriptShort(&s);

	return MAIN_func_80106730(b2, stat2, s);
}

int16_t *getStatsPointer(int32_t stat)
{
	switch (stat) {
	case 0:
		return &PARTNER_ENTITY.digimonEntity.stats.base.off;
	case 1:
		return &PARTNER_ENTITY.digimonEntity.stats.base.def;
	case 2:
		return &PARTNER_ENTITY.digimonEntity.stats.base.speed;
	case 3:
		return &PARTNER_ENTITY.digimonEntity.stats.base.brain;
	case 4:
		return &PARTNER_ENTITY.digimonEntity.stats.base.hp;
	case 5:
		return &PARTNER_ENTITY.digimonEntity.stats.base.mp;
	case 6:
		return &PARTNER_ENTITY.digimonEntity.stats.current.currentHP;
	case 7:
		return &PARTNER_ENTITY.digimonEntity.stats.current.currentMP;
	case 8:
		return &PARTNER_PARA.tiredness;
	case 9:
		return &PARTNER_PARA.happiness;
	case 0xa:
		return &PARTNER_PARA.discipline;
	case 0xb:
		return &PARTNER_PARA.energyLevel;
	case 0xc:
		return &PARTNER_PARA.virusBar;
	case 0xd:
		return &PARTNER_PARA.remainingLifetime;
	case 0xe:
		return &MERIT;
	case 0xf:
		return &MAIN_D_80134FC8;
	case 0x10:
		return &MAIN_D_80134FCA;
	case 0x11:
		return &MAIN_D_80134FCC;
	case 0x12:
		return &TOURNAMENTS_LOST;
	case 0x13:
		return &MAIN_D_80134FD0;
	case 0x14:
		return &PARTNER_PARA.weight;
	case 0x15:
		MAIN_D_80135002 = TAMER_ENTITY.tamerLevel;
		return &MAIN_D_80135002;
	case 0x16:
		MAIN_D_80135004 = PARTNER_ENTITY.lives;
		return &MAIN_D_80135004;
	}
}

void *allocateArray(uint32_t size)
{
	uint32_t oldTop;

	oldTop = MAIN_D_80134F64;
	size = ((size + 3) >> 2) << 2;
	*(uint32_t *)(MAIN_D_801345B0 + oldTop) = size;
	MAIN_D_80134F64 += (size + 4);

	return MAIN_D_801345B0 + (oldTop + 4);
}

void freeArray(uint32_t *array)
{
	MAIN_D_80134F64 -= array[-1] + 4;
}

void MAIN_func_800FBC58(void)
{
	int16_t rowPx;
	int16_t x;
	int16_t y;

	rowPx = TEXT_BOX_DATA[2].vramRow * 12;
	x = UI_BOX_DATA[2].finalPos.x + 6;
	y = UI_BOX_DATA[2].finalPos.y + 4;

	renderString(0, x + 0x10, y, 0x30, 0xc, 0, rowPx, 4, 1);
	renderString(0, x + 0x10, y + 0xd, 0x3c, 0xc, 0x30, rowPx, 4, 1);
}

int32_t MAIN_func_801024CC(void)
{
	uint16_t value;
	uint8_t cardId;
	uint8_t op;

	pollNextTwoScriptBytes(&cardId, &op);
	pollNextScriptUShort(&value);

	return scriptCompareValues(op, (uint8_t)getCardAmount(cardId), value);
}

int32_t MAIN_func_80102514(void)
{
	uint8_t moveId;
	uint8_t negate;
	int32_t res;

	pollNextTwoScriptBytes(&moveId, &negate);
	res = MAIN_func_80106D1C(moveId);
	if (negate == 0) {
		return res;
	}

	return (res != 0) ^ 1;
}

int32_t MAIN_func_80102564(void)
{
	uint8_t mask;
	uint8_t negate;

	pollNextTwoScriptBytes(&mask, &negate);

	if (negate == 0) {
		if (PARTNER_PARA.condition & mask) {
			return 1;
		}

		return 0;
	} else {
		if ((PARTNER_PARA.condition & mask) == 0) {
			return 1;
		}

		return 0;
	}
}

int32_t MAIN_func_801025E8(void)
{
	uint16_t value;
	uint8_t itemId;
	uint8_t op;

	pollNextTwoScriptBytes(&itemId, &op);
	pollNextScriptUShort(&value);

	return scriptCompareValues(op, (uint8_t)getItemCount(itemId), value);
}

void readFileSection(char *filename, void *dest, uint32_t offset,
		     uint32_t size)
{
	CdlFILE file;
	char path[64];
	uint8_t mode;

	mode = 0x80;

	if (MAIN_D_80134FA8 == 0) {
		path[0] = '\\';
		strcpy(&path[1], filename);
		strcat(path, MAIN_D_801345F0);
		if (CdSearchFile(&file, path) == 0) {
			return;
		}

		while (CdControl(0xe, &mode, 0) == 0);

		MAIN_D_80134FA8 = CdPosToInt(&file.pos);
	} else {
		while (CdControl(0xe, &mode, 0) == 0);
	}

	file.pos = *CdIntToPos(MAIN_D_80134FA8 + (offset >> 11), &file.pos);

	while (CdControl(2, (u_char *)&file.pos, 0) == 0);
	while (CdRead(size >> 11, dest, mode) == 0);
	while (CdReadSync(0, 0) > 0);
}

void tickScriptedMovements(void)
{
	int32_t i;

	for (i = 0; i < 0x16; i++) {
		if (MAIN_D_801BE6B4[i].type != 0xff) {
			tickScriptedMovement(i);
		}
	}
}

void scriptInstructionFBtoFF(int32_t op)
{
	StackEntry entry;

	if (op != 0xfb) {
		if (op == 0xfe || op == 0xff) {
			returnFromScriptFile();
		}
		return;
	}

	skipOneReadTwoShort(&CURRENT_SCRIPT_ID, &MAIN_D_80134FFE);

	entry.smth[0] = 3;
	pushScriptStack(&entry);

	MAIN_D_80134FEC = 1;

	resetBGM();
	loadMap(MAIN_D_80134FFE);
	longjmp(SCRIPT_JMP_BUF, 1);
}

int32_t enforceStatsLimits(int32_t stat, int32_t value)
{
	int32_t cap;

	if (stat == 6) {
		cap = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	} else if (stat == 7) {
		cap = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	} else {
		cap = MAIN_D_80130318[stat];
	}

	if (cap < value) {
		return cap;
	}

	return value;
}

int32_t MAIN_func_80102630(void)
{
	int32_t value;
	uint8_t op;

	MAIN_D_80134FDC = (uint8_t *)(MAIN_D_80134FDC + 1);

	pollNextScriptUByte(&op);
	pollNextInt(&value);

	return scriptCompareValues(op, MONEY, value);
}
