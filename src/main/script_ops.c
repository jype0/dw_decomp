#include <string.h>

#include <dw/item.h>
#include <dw/pstat.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/trigger.h>
#include <dw/ui.h>

#include "common.h"

int32_t getItemCount(int32_t itemId);
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

void MAIN_func_80108EB4(ItemMenuBox *box, int32_t b);
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
extern uint8_t CARD_DATA[];
extern uint8_t MAIN_D_80130438[];
extern uint8_t MAIN_D_80130444[];
extern uint8_t MAIN_D_801303B8[];
extern uint8_t MAIN_D_80134F8F;
extern uint8_t MAIN_D_80134F90;
extern int32_t MAIN_D_8012FFC4[];
extern uint16_t MAIN_D_80134608[2];
extern uint8_t MAIN_D_80134F81;
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
	int32_t any;
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
	int32_t any;
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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", rollCard);

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
	rect.x = src->x;
	rect.y = src->y;
	rect.w = src->w;
	rect.h = src->h;
	createTextbox(1, 0xf1, &rect, &origin, MAIN_func_80107E6C,
		      MAIN_func_80108090);
	registerTextbox(1, 9, 6, 1, 0);
	MAIN_func_800FCC98(result, 1, 9);
	MAIN_func_800FCCFC(result, 9, 1);
	MAIN_D_8013500C = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_801072C4);

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_80107444);

void MAIN_func_80107660(void)
{
	RECT rect;
	RECT origin;
	int32_t boxId;

	boxId = readPStat(PSTAT_254);
	setupBoxOrigin(boxId, &origin);
	rect.x = -152;
	rect.y = -98;
	rect.w = 148;
	rect.h = 127;
	createTextbox(1, 0xe1, &rect, &origin, MAIN_func_80108334,
		      MAIN_func_80108604);
	registerTextbox(1, 9, 6, 1, 1);
	MAIN_func_800FCC98(MAIN_D_80134F68, 1, 0xa);
	MAIN_func_80108610(0);
	setupBoxOrigin(0xfd, &origin);
	rect.x = 0;
	rect.y = -98;
	rect.w = 148;
	rect.h = 127;
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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_801078F4);

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
	rect.x = -71;
	rect.y = -100;
	rect.w = 222;
	rect.h = 129;
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
	rect.x = -88;
	rect.y = -80;
	rect.w = 223;
	rect.h = 83;
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
			rect.x = src->x;
			rect.y = src->y;
			rect.w = src->w;
			rect.h = src->h;
			MAIN_func_800FD244(&rect);
		} else if (MAIN_D_80135011 == 6) {
			MAIN_func_80108230();
		} else {
			rect.x = src->x;
			rect.y = src->y;
			rect.w = src->w;
			rect.h = src->h;
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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_80108230);

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
			rect.x = 5;
			rect.y = 0x20;
			rect.w = 0x80;
			rect.h = 0x12;
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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_801086E0);

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_80108890);

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_80108A98);

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_80108C88);

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_80108DC0);

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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_801093E4);

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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", MAIN_func_801096E8);

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

	rect1.x = screenPos[0];
	rect1.y = screenPos[1];
	rect1.w = 10;
	rect1.h = 10;
	rect2.x = -130;
	rect2.y = -78;
	rect2.w = 262;
	rect2.h = 59;
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

	rect1.x = screenPos[0];
	rect1.y = screenPos[1];
	rect1.w = 10;
	rect1.h = 10;
	rect2.x = -145;
	rect2.y = -91;
	rect2.w = 290;
	rect2.h = 138;
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

	rect1.x = screenPos[0];
	rect1.y = screenPos[1];
	rect1.w = 10;
	rect1.h = 10;
	rect2.x = -145;
	rect2.y = 60;
	rect2.w = 149;
	rect2.h = 42;
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

INCLUDE_ASM("asm/main/nonmatchings/script_ops", renderSelectionBox);

INCLUDE_ASM("asm/main/nonmatchings/script_ops", renderNameDisplayBox);

INCLUDE_ASM("asm/main/nonmatchings/script_ops", renderNamingUnderscore);
