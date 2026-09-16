#include <string.h>

#include <dw/item.h>
#include <dw/pstat.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/trigger.h>
#include <dw/ui.h>

#include "common.h"

typedef struct {
	uint8_t data[8];
} SelectionBoxUVData;

typedef struct {
	int8_t data[8];
} SelectionBoxOffsetData;

extern GsOT *ACTIVE_ORDERING_TABLE;
void MAIN_func_800FD7D8(int32_t boxId, int32_t idx, int16_t x, int16_t y);
void MAIN_func_800FD8D4(ItemMenuBox *box);
void MAIN_func_800FDC5C(ItemMenuBox *box, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int32_t flag);
void renderSelectionCursor(int32_t x, int32_t y, int16_t w, int16_t h, int32_t layer);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t x, int32_t y, int32_t w, int32_t h);
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
extern int16_t MAIN_D_8013078E[];
extern GsOT *ACTIVE_ORDERING_TABLE;
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
void renderNamingUnderscore(int32_t boxId, int16_t x, int16_t y, int32_t w);
void renderSelectionBox(void);

extern uint8_t MAIN_D_80135007;
extern uint8_t MAIN_D_80134F82;
extern uint16_t MAIN_D_801307A0[10];
extern char MAIN_D_801345F4[4];
extern uint8_t MAIN_D_8012FFD9[];
extern uint8_t MAIN_D_8012FFDA[];
extern uint8_t CARD_DATA[];
extern uint8_t MAIN_D_80130438[];
extern uint8_t MAIN_D_80130444[];
extern uint8_t MAIN_D_801303B8[];
extern uint8_t MAIN_D_80134F8F;
extern uint8_t MAIN_D_80134F90;
extern int32_t MAIN_D_8012FFC4[];
extern uint16_t MAIN_D_80134608[2];
extern uint8_t MAIN_D_80134F81;
void terminateNamingBuffer(void);
void namingDeleteLast(void);
extern uint32_t POLLED_INPUT;
extern int32_t MAIN_D_80130250[];
extern RECT MAIN_D_801302BC[];

static void *script_ops_functions[] = {
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
};

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
			if (MAIN_D_8012FFC4[MAIN_D_8012FFD9[id *
			    4]] <= MONEY &&
			    (uint32_t)getCardAmount(id) < 9) {
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
		if (MAIN_D_8012FFD9[cardId * 4] == 0) {
			line = 2;
		} else if (MAIN_D_8012FFD9[cardId * 4] == 1) {
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

	writePStat(PSTAT_249, (CARD_DATA)[cardId * 4]);
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
	int32_t off;

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
	for (i = 0, off = 0; i < 0x42; i++, off += 4) {
		if (rarity == MAIN_D_8012FFD9[off]) {
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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_80107444);

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
	setupBoxOrigin(0xfd, &origin);
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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_80108090);

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
			MAIN_D_8013500C = *(int16_t *)&MAIN_D_8012FFDA[idx];
			kind = MAIN_D_8012FFD9[MAIN_D_80134F78 * 4] + 7;
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
		value = 0xd;
		TEXT_BUFFERS_PTR[0x246] = value;
		TEXT_BUFFERS_PTR[0x3c6] = value;
		MAIN_func_800FCCFC(MAIN_D_80134F68, 0xa, 0);
	} else {
		showMapHeadTextbox(5, 0xff, 2, 0x4d5);
		TEXT_BOX_DATA[2].writeCount--;
		value = 0xd;
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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_80108C88);

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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_80108EB4);

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
					       MAIN_D_80134F68->cursor) * 2]);
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
					     MAIN_D_80134F68->cursor) * 2];
		if ((item & 0x80) != 0) {
			showMapHeadTextbox(6, readPStat(PSTAT_254), 0, 0x4d6);
			MAIN_D_8013500C = MAIN_D_80130250[(item & 0x7f) * 2];
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
					     MAIN_D_80134F68->cursor) * 2];
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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_801099E8);

void MAIN_func_80109BBC(void)
{
	showMapHeadTextbox(0xf, 0xff, 1, 0x4d6);

	--TEXT_BOX_DATA[1].writeCount;
	TEXT_BUFFERS_PTR[0x254] = 0xd;
	TEXT_BUFFERS_PTR[0x354] = 0xd;

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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", tickNamingBox);

INCLUDE_ASM("asm/main/nonmatchings/script_ops", renderNamingBox);

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_8010A79C);

void updateNamingPreview(void)
{
	uint8_t *out;
	int32_t len;

	out = TEXT_BUFFERS_PTR;
	*out++ = 1;
	*out++ = 1;

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
		baseY = (by + MAIN_D_8013078E[idx]) - 4;
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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", renderNameDisplayBox);

INCLUDE_ASM("asm/main/nonmatchings/script_ops", renderNamingUnderscore);
