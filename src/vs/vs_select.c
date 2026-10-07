#include <string.h>

#include <libgpu.h>
#include <libgs.h>

#include <dw/btl.h>
#include <dw/entity.h>
#include <dw/fade.h>
#include <dw/file.h>
#include <dw/file_queue.h>
#include <dw/font.h>
#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/main.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/vs.h>
#include <dw/world_object.h>

typedef struct {
	int16_t clutX;
	int16_t clutY;
	uint8_t u;
	uint8_t v;
	uint8_t w;
	uint8_t h;
	uint8_t tpage;
	uint8_t x;
	uint8_t y;
} VsListPanel;

typedef struct {
	int16_t clut;
	uint8_t u;
	uint8_t v;
	uint8_t w;
	uint8_t h;
	uint8_t x;
	uint8_t y;
} VsUISprite;

#if defined(VERSION_JP)
extern VsSelectDigimonData VS__SELECT_DIGIMON_DATA[2];
#else
VsSelectDigimonData VS__SELECT_DIGIMON_DATA[2];
#endif

void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
                  int32_t f, int32_t g, int32_t h, int32_t i);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
                          int32_t *digits);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uvX, int32_t uvY,
                      int32_t width, int32_t height);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY,
                       int32_t width, int32_t height);
void loadStackedTIMFile(char *path);

void VS__loadTextures(void);
void VS__tickSelectDigimon(void);
int32_t VS__isAlreadySelected(int16_t player, int16_t value);
void VS__handleDigimonSelected(VsSelectDigimonData *state);
void VS__createPressStartToBeginBox(int16_t id);
void VS__removePressStartToBeginBox(int16_t id);
void VS__setPolyFT4White(POLY_FT4 *poly);
void VS__tickSelectDigimonPlayer(int32_t id);
void VS__renderSelectDigimonPlayer();
void VS__renderPressStartToBeginBox(int32_t id);
int32_t VS__isKeyPressedByAnyPlayer(uint32_t buttons);
int32_t VS__tickSelectMode(void);
void VS__tickSelectBox(int32_t id);
void VS__renderSelectModeBox(int32_t depth);
int32_t VS__tickSelectMap(void);
void VS__renderSelectMapBox(int32_t id);

static void *vs_select_functions[] = {
	VS__renderSelectMapBox,
	VS__tickSelectMap,
	VS__renderSelectModeBox,
	VS__tickSelectBox,
	VS__tickSelectMode,
	VS__isKeyPressedByAnyPlayer,
	VS__renderPressStartToBeginBox,
	VS__renderSelectDigimonPlayer,
	VS__tickSelectDigimonPlayer,
	VS__initialize,
	VS__setPolyFT4White,
	VS__removePressStartToBeginBox,
	VS__createPressStartToBeginBox,
	VS__handleDigimonSelected,
	VS__isAlreadySelected,
	VS__tickSelectDigimon,
	VS__loadTextures,
};

// clang-format off

int16_t SPRITE_POS_X[4] = {
	0x0036, 0x0036, 0x0050, 0x0050,
};

int16_t SPRITE_POS_Y[4] = {
	0x0020, 0x0055, 0x0020, 0x0055,
};

int16_t VS__NAME_POS_X[4] = {
	0x0009, 0x0009, 0x005c, 0x005c,
};

int16_t VS__NAME_POS_Y[4] = {
	0x0036, 0x0047, 0x0036, 0x0047,
};

uint8_t VS__STAT_NUMBER_OFFSETS[6] = {
	0x00, 0x0c, 0x18, 0x24, 0x30, 0x3c,
};

uint8_t VS__BATTLE_COUNTS[4] = {
	0x01, 0x03, 0x05, 0x00,
};

char VS__PATH_ETCDAT_ETCTIM_BIN[] = "\\ETCDAT\\ETCTIM.BIN";

char VS__PATH_ETCNA_TITLE2_TIM[] = "\\ETCNA\\TITLE2.TIM";

char VS__PATH_ETCDAT_SYSTEM_W_TIM[] = "\\ETCDAT\\SYSTEM_W.TIM";

char VS__PATH_STDDAT_TAISEN1_TIM[20] = "\\STDDAT\\TAISEN1.TIM";

char VS__PATH_STDDAT_TAISEN2_TIM[20] = "\\STDDAT\\TAISEN2.TIM";

char VS__PATH_STDDAT_16TAISEN_TIM[] = "\\STDDAT\\16TAISEN.TIM";

char VS__PATH_STDDAT_TAISEN_F_TIM[] = "\\STDDAT\\TAISEN_F.TIM";

char VS__PATH_STDDAT_TIME_TIM[] = "\\STDDAT\\TIME.TIM";

#if defined(VERSION_JP)
char VS__STR_PRESS_START[] = "スタートボタンで";

char VS__STR_TO_BEGIN[] = "決定して下さい。";
#else
char VS__STR_PRESS_START[12] = "Press Start";

char VS__STR_TO_BEGIN[] = "to begin.";
#endif

uint8_t DIGIMON_SPRITE_CLUT[68] = {
	0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x01,
	0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x03,
	0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x02, 0x03,
	0x04, 0x03, 0x00, 0x00,
};

int16_t STAT_LIMITS[6] = {
	0x270f, 0x270f, 0x03e7, 0x03e7, 0x03e7, 0x03e7,
};

uint8_t STAT_OFFSETS[12] = {
	0x08, 0x14, 0x20, 0x2c, 0x38, 0x44, 0x4a, 0x54,
	0x62, 0x6c, 0x7a, 0x84,
};

/* Not equipped */
char STR_SOUBINASHI[] = "ソウビナシ";

VsListPanel MAIN_D_8012F590[12] = {
	{ 0x0000, 0x01e8, 0x00, 0x00, 0x96, 0xd7, 0x06, 0x00, 0x00 },
	{ 0x0000, 0x01ea, 0x96, 0x00, 0x44, 0x27, 0x06, 0x06, 0x1b },
	{ 0x0000, 0x01eb, 0x96, 0x27, 0x44, 0x27, 0x06, 0x06, 0x44 },
	{ 0x0000, 0x01ec, 0x96, 0x4e, 0x44, 0x27, 0x06, 0x4c, 0x1b },
	{ 0x0000, 0x01ed, 0x96, 0x75, 0x44, 0x27, 0x06, 0x4c, 0x44 },
	{ 0x0000, 0x01e8, 0x96, 0x9c, 0x14, 0x0a, 0x06, 0x18, 0x02 },
	{ 0x0000, 0x01e9, 0x00, 0x00, 0x96, 0xd7, 0x06, 0x00, 0x00 },
	{ 0x0000, 0x01ea, 0x96, 0x00, 0x44, 0x27, 0x06, 0x06, 0x1b },
	{ 0x0000, 0x01eb, 0x96, 0x27, 0x44, 0x27, 0x06, 0x06, 0x44 },
	{ 0x0000, 0x01ec, 0x96, 0x4e, 0x44, 0x27, 0x06, 0x4c, 0x1b },
	{ 0x0000, 0x01ed, 0x96, 0x75, 0x44, 0x27, 0x06, 0x4c, 0x44 },
	{ 0x0000, 0x01e9, 0xaa, 0x9c, 0x14, 0x0a, 0x06, 0x18, 0x02 },
};

VsListPanel VS__ROW_PANELS[4] = {
	{ 0x0040, 0x01e8, 0x00, 0x00, 0x29, 0x16, 0x05, 0x09, 0x1e },
	{ 0x0040, 0x01e8, 0x00, 0x00, 0x29, 0x16, 0x05, 0x09, 0x51 },
	{ 0x0040, 0x01e8, 0x00, 0x00, 0x29, 0x16, 0x05, 0x62, 0x1e },
	{ 0x0040, 0x01e8, 0x00, 0x00, 0x29, 0x16, 0x05, 0x62, 0x51 },
};

VsUISprite MODE_SELECT_SPRITES[8] = {
	{ 0x01e9, 0x78, 0x00, 0x7c, 0x0e, 0x1c, 0x07 },
	{ 0x01ee, 0x00, 0x88, 0x28, 0x30, 0x0e, 0x25 },
	{ 0x01ee, 0x50, 0x88, 0x28, 0x30, 0x46, 0x25 },
	{ 0x01ef, 0xa0, 0x88, 0x28, 0x30, 0x7e, 0x25 },
	{ 0x01f4, 0x00, 0xe8, 0x34, 0x17, 0x0a, 0x60 },
	{ 0x01f4, 0x34, 0xe8, 0x34, 0x17, 0x41, 0x60 },
	{ 0x01f4, 0x68, 0xe8, 0x34, 0x17, 0x7a, 0x60 },
	{ 0x01ee, 0x78, 0x4c, 0x34, 0x3c, 0x08, 0x1f },
};

VsUISprite MAP_SELECT_SPRITES[8] = {
	{ 0x01ea, 0x78, 0x0e, 0x7c, 0x0e, 0x1c, 0x07 },
	{ 0x01f0, 0x00, 0xb8, 0x28, 0x30, 0x0e, 0x25 },
	{ 0x01f1, 0x50, 0xb8, 0x28, 0x30, 0x46, 0x25 },
	{ 0x01f2, 0xa0, 0xb8, 0x28, 0x30, 0x7e, 0x25 },
	{ 0x01eb, 0x78, 0x34, 0x34, 0x18, 0x08, 0x60 },
	{ 0x01eb, 0x78, 0x1c, 0x34, 0x18, 0x40, 0x60 },
	{ 0x01eb, 0xac, 0x1c, 0x30, 0x18, 0x7a, 0x60 },
	{ 0x01ee, 0x78, 0x4c, 0x34, 0x3c, 0x08, 0x1f },
};
// clang-format on

int16_t VS__MODE_STATE;
int8_t VS__PRESS_START_BOX_CREATED[2];
int32_t VS__BOTH_SELECTED;
uint8_t VS__SELECT_BOX_DATA_SELECTION_COUNT;
uint8_t VS__SELECT_BOX_DATA_SELECTION;
uint8_t VS__SELECT_BOX_DATA_ANIM_FRAME;
uint8_t VS__SELECT_BOX_DATA_HAS_SELECTED;
uint8_t VS__SELECT_BOX_DATA_TIMER;

static void *vs_select_sbss_order[] = {
	&VS__SELECT_BOX_DATA_TIMER,
	&VS__SELECT_BOX_DATA_HAS_SELECTED,
	&VS__SELECT_BOX_DATA_ANIM_FRAME,
	&VS__SELECT_BOX_DATA_SELECTION,
	&VS__SELECT_BOX_DATA_SELECTION_COUNT,
	&VS__BOTH_SELECTED,
	VS__PRESS_START_BOX_CREATED,
	&VS__MODE_STATE,
};

void VS__loadTextures(void)
{
	RegisteredDigimon *entries[2];
	VsSelectDigimonData *state;
	uint8_t *p;
	int32_t i;
	int32_t j;

#if !defined(VERSION_JP)
	ENTITY_TABLE[0]->isOnScreen = 0;
#endif
	loadTIMFile(VS__PATH_ETCDAT_SYSTEM_W_TIM, GENERAL_BUFFER);
	loadTIMFile(VS__PATH_STDDAT_TAISEN1_TIM, GENERAL_BUFFER);
	loadTIMFile(VS__PATH_STDDAT_TAISEN2_TIM, GENERAL_BUFFER);
	loadTIMFile(VS__PATH_STDDAT_16TAISEN_TIM, GENERAL_BUFFER);
	loadTIMFile(VS__PATH_STDDAT_TAISEN_F_TIM, GENERAL_BUFFER);
	loadTIMFile(VS__PATH_STDDAT_TIME_TIM, GENERAL_BUFFER);

	entries[0] = VS_DIGIMON_P1_PTR;
	entries[1] = VS_DIGIMON_P2_PTR;

	for (i = 0; i < 5; ++i) {
		VS_BATTLE_SETUP.fighters[0][i] = 0xff;
		VS_BATTLE_SETUP.fighters[1][i] = 0xff;
	}

	for (i = 0; i < 2; ++i) {
		state = &VS__SELECT_DIGIMON_DATA[i];
		state->slotCount = 0;
		state->selectionState = 0;
		state->listPage = 0;
		state->unk1 = 0;
		state->unk2 = 0;
		state->detailScrollDir = 0;
		state->unk4 = 0;
		state->unk6 = 0;
		state->unk5 = 0;
		state->unk7 = 0;
		state->detailPage = 0;
		state->selectedMask = 0;
		state->selectedSlot = 0;
		state->unk9 = 0;
		p = &state->unk10[0];
		for (j = 0; j < 24; ++j) {
			*p = 0;
			++p;
		}

		for (j = 0; j < 40; ++j) {
			if (entries[i][j].digimonId != 0) {
				state->slotIds[state->slotCount] = j;
				++state->slotCount;
			}
		}

		state->listPageCount = ((state->slotCount % 4) == 0) ? state->slotCount / 4 : (state->slotCount / 4) + 1;
	}
}

void VS__tickSelectDigimon(void)
{
	int32_t i;

	VS__PRESS_START_BOX_CREATED[0] = -1;
	VS__PRESS_START_BOX_CREATED[1] = -1;

	clearTextArea();

	addObject(0x1a0, 0, (TickFunction)VS__tickSelectDigimonPlayer, VS__renderSelectDigimonPlayer);
	addObject(0x1a0, 1, (TickFunction)VS__tickSelectDigimonPlayer, VS__renderSelectDigimonPlayer);

	fadeFromBlack(10);

	for (i = 0; i < 11; ++i) {
		VS_tickFrame();
	}

	VS__BOTH_SELECTED = 0;
	while (VS__BOTH_SELECTED == 0) {
		VS_tickFrame();
	}

	i = 0;
	fadeToBlack(20);
	for (; i < 21; ++i) {
		VS_tickFrame();
	}

	removeObject(0x1a0, 0);
	removeObject(0x1a0, 1);
}

int32_t VS__isAlreadySelected(int16_t player, int16_t value)
{
	int32_t i;
	uint8_t *table;

	if (player == 0) {
		table = VS_BATTLE_SETUP.fighters[0];
	} else {
		table = VS_BATTLE_SETUP.fighters[1];
	}

	for (i = 0; i < VS_BATTLE_SETUP.battleCount; ++i) {
		if (value == table[i]) {
			playSound(0, 0xb);
			return 1;
		}
	}

	return 0;
}

void VS__handleDigimonSelected(VsSelectDigimonData *state)
{
	int32_t i;

	playSound(0, 3);

	state->unk7 = 0;
	state->unk6 = 0;
	state->selectedMask |= 1 << state->selectedSlot;

	for (i = 0; i < VS_BATTLE_SETUP.battleCount; ++i) {
		if ((state->selectedMask & (1 << i)) == 0) {
			break;
		}
	}

	if (i == VS_BATTLE_SETUP.battleCount) {
		state->lastSlot = state->selectedSlot;
	}

	if (state->selectedSlot < (VS_BATTLE_SETUP.battleCount - 1)) {
		++state->selectedSlot;
	}
}

void VS__createPressStartToBeginBox(int16_t id)
{
	RECT rect;

	if (VS__PRESS_START_BOX_CREATED[id] == -1) {
		drawString(VS__STR_PRESS_START, 0, 0);
		drawString(VS__STR_TO_BEGIN, 0, 12);

		setRECT(&rect, (id == 0) ? -132 : 22, 32, 108, 36);
		createStaticUIBox(id, 0, 2, &rect, 0, VS__renderPressStartToBeginBox);

		VS__PRESS_START_BOX_CREATED[id] = 1;
	}
}

void VS__removePressStartToBeginBox(int16_t id)
{
	if (VS__PRESS_START_BOX_CREATED[id] != -1) {
		removeStaticUIBox(id);
		VS__PRESS_START_BOX_CREATED[id] = -1;
	}
}

void VS__setPolyFT4White(POLY_FT4 *poly)
{
	SetPolyFT4(poly);
	setRGB0(poly, 0x80, 0x80, 0x80);
}

void VS__initialize(RegisteredDigimon *fightersP1, RegisteredDigimon *fightersP2)
{
	int32_t done;

	VS_DIGIMON_P1_PTR = fightersP1;
	VS_DIGIMON_P2_PTR = fightersP2;
	VS__loadTextures();

	VS__MODE_STATE = 0;
	done = 0;
	while (done == 0) {
		switch (VS__MODE_STATE) {
		case 0:
			VS__tickSelectMode();
			++VS__MODE_STATE;
			break;
		case 1:
			VS__tickSelectMap();
			++VS__MODE_STATE;
			break;
		case 2:
			VS__tickSelectDigimon();
			++VS__MODE_STATE;
			break;
		case 3:
			VS_initializeVS();
			done = 1;
			break;
		}
	}

	loadStackedTIMFile(VS__PATH_ETCDAT_ETCTIM_BIN);
	loadTIMFile(VS__PATH_ETCNA_TITLE2_TIM, GENERAL_BUFFER_PTR);
}

// clang-format off
void VS__tickSelectDigimonPlayer(id)
	int16_t id;
// clang-format on
{
	VsSelectDigimonData *state;
	long idx;
	uint8_t *table;
	int32_t i;
	uint32_t input;
	uint32_t previous;
	int32_t count;
	uint8_t mask;

	if (id == 0) {
		table = VS_BATTLE_SETUP.fighters[0];
	} else {
		table = VS_BATTLE_SETUP.fighters[1];
	}

	if (id == 1) {
		input = POLLED_INPUT;
		previous = POLLED_INPUT_PREVIOUS;
		POLLED_INPUT = (POLLED_INPUT >> 16) & 0xffff;
		POLLED_INPUT_PREVIOUS = (POLLED_INPUT_PREVIOUS >> 16) & 0xffff;
	}

	state = &VS__SELECT_DIGIMON_DATA[id];
	++state->unk5;
	if ((state->unk5 % 8) == 0) {
		state->unk4 = (state->unk4 + 1) & 1;
	}

	switch (state->selectionState) {
	case 0:
		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x8000) {
			state->listPage--;
			if (state->listPage == 0xff) {
				state->listPage = state->listPageCount - 1;
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x2000) {
			++state->listPage;
			state->listPage %= state->listPageCount;
		}

		if (state->listPage == (state->listPageCount - 1)) {
			count = state->slotCount - ((state->listPageCount - 1) * 4);
		} else {
			count = 4;
		}

		idx = state->listPage * 4;

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x10) {
			if (VS__isAlreadySelected(id, state->slotIds[idx]) == 0) {
				table[state->selectedSlot] = state->slotIds[idx];
				VS__handleDigimonSelected(state);
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x40) {
			if ((VS__isAlreadySelected(id, state->slotIds[idx + 3]) == 0) && (count == 4)) {
				table[state->selectedSlot] = state->slotIds[idx + 3];
				VS__handleDigimonSelected(state);
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x80) {
			if ((VS__isAlreadySelected(id, state->slotIds[idx + 1]) == 0) && (count >= 2)) {
				table[state->selectedSlot] = state->slotIds[idx + 1];
				VS__handleDigimonSelected(state);
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x20) {
			if ((VS__isAlreadySelected(id, state->slotIds[idx + 2]) == 0) && (count >= 3)) {
				table[state->selectedSlot] = state->slotIds[idx + 2];
				VS__handleDigimonSelected(state);
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x1000) {
			playSound(0, 2);
			if (state->selectedSlot != 0) {
				state->unk7 = 0;
			}
			if (state->selectedSlot != 0) {
				state->selectedSlot--;
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x4000) {
			playSound(0, 2);
			if (state->selectedSlot != (VS_BATTLE_SETUP.battleCount - 1)) {
				state->unk7 = 0;
			}
			if (state->selectedSlot < (VS_BATTLE_SETUP.battleCount - 1)) {
				++state->selectedSlot;
			}
		}

		if (state->unk2 == (state->detailPage * 24)) {
			if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x4) {
				state->detailScrollDir = 0xff;
				if (state->detailPage != 0) {
					state->detailPage--;
				}
			}

			if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x8) {
				state->detailScrollDir = 1;
				if (state->detailPage < 5) {
					++state->detailPage;
				}
			}
		}

		if (state->unk2 != (state->detailPage * 24)) {
			state->unk2 += state->detailScrollDir * 4;
		}

		for (i = 0; i < VS_BATTLE_SETUP.battleCount; ++i) {
			if ((state->selectedMask & (1 << i)) == 0) {
				break;
			}
		}

		if (i == VS_BATTLE_SETUP.battleCount) {
			VS__createPressStartToBeginBox(id);
			++state->selectionState;
		}
		break;
	case 1:
		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x800) {
			playSound(0, 3);
			++state->selectionState;
			state->unk9 = 1;
			VS__removePressStartToBeginBox(id);
		} else if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x20) || (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x80) ||
		           (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x10) || (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x40) ||
		           (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x8000) ||
		           (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x2000) ||
		           (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x1000) ||
		           (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x4000)) {
			playSound(0, 4);
			state->selectionState--;
			VS__removePressStartToBeginBox(id);
			mask = 1 << state->lastSlot;
			state->selectedMask = state->selectedMask & ~mask;
			table[state->lastSlot] = 0xff;
		}
		break;
	case 2:
		if ((VS__SELECT_DIGIMON_DATA[0].unk9 == 1) && (VS__SELECT_DIGIMON_DATA[1].unk9 == 1)) {
			VS__BOTH_SELECTED = 1;
		}
		break;
	}

	if (id == 1) {
		POLLED_INPUT = input;
		POLLED_INPUT_PREVIOUS = previous;
	}
}

GARBAGE(VS__renderSelectDigimonPlayer, 33);

// clang-format off
void VS__renderSelectDigimonPlayer(id)
	int16_t id;
// clang-format on
{
	POLY_FT4 *prim;
	VsSelectDigimonData *st;
	VsListPanel *panel;
	long i;
	long j;
	int32_t k;
	int32_t l;
	int32_t rowCount;
	int32_t halfLen;
	int16_t *stats;
	uint16_t *text;
	char buf[24];
	POLY_F4 *bar;
	RegisteredDigimon *fighters;
	RegisteredDigimon *fighter;
	int32_t count;
	int32_t digits[4];
	int16_t baseX;
	int16_t y;
	int16_t width;
	int16_t height;
	int16_t glyph;
	int16_t moveRow;
	int16_t moveId;
	int16_t dx;
	int16_t posY;
	uint16_t ch;
	uint8_t spriteU;
	uint8_t spriteV;
	uint8_t baseIdx;
	uint8_t type;

	baseX = id * 154 - 152;
	y = -109;
	if (id == 0) {
		fighters = VS_DIGIMON_P1_PTR;
	} else {
		fighters = VS_DIGIMON_P2_PTR;
	}
	st = &VS__SELECT_DIGIMON_DATA[(int16_t)id];

	if (st->listPage == (st->listPageCount - 1)) {
		rowCount = st->slotCount - ((st->listPageCount - 1) * 4);
	} else {
		rowCount = 4;
	}
	baseIdx = st->listPage * 4;

	bar = (POLY_F4 *)GsGetWorkBase();
	for (i = 0; i < rowCount; i++) {
		panel = &VS__ROW_PANELS[i];
		stats = &fighters[st->slotIds[baseIdx + i]].hp;
		for (j = 0; j < 6; j++, stats++) {
			width = *stats * 34 / STAT_LIMITS[j];
			if ((st->unk2 < (STAT_OFFSETS[j] + 2)) &&
			    (STAT_OFFSETS[j] <= (st->unk2 + 22))) {
				SetPolyF4(bar);
				setRGB0(bar, 0, 255, 255);
				setXY4(bar, baseX + panel->x + 7,
				       (y + panel->y) + (STAT_OFFSETS[j] - st->unk2),
				       width + (baseX + panel->x + 7),
				       (y + panel->y) + (STAT_OFFSETS[j] - st->unk2),
				       baseX + panel->x + 7,
				       (y + panel->y) + (STAT_OFFSETS[j] - st->unk2) + 2,
				       width + (baseX + panel->x + 7),
				       (y + panel->y) + (STAT_OFFSETS[j] - st->unk2) + 2);
				AddPrim(ACTIVE_ORDERING_TABLE->org + 10, bar++);
			}
		}
	}
	GsSetWorkBase((PACKET *)bar);

	prim = (POLY_FT4 *)GsGetWorkBase();
	if (st->listPageCount != 1) {
		dx = (baseX + 75) - (st->listPageCount * 11 / 2);
		for (i = 0; i < st->listPageCount; i++) {
			VS__setPolyFT4White(prim);
			prim->tpage = 6;
			prim->clut = GetClut(0, id + 0x1e8);
			setUVDataPolyFT4(prim, (i != st->listPage) ? 0xc8 : 0xbe, 0x9c, 10, 6);
			setPosDataPolyFT4(prim, dx + i * 11, y + 19, 10, 6);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
		}
		for (i = 0; i < 2; i++) {
			VS__setPolyFT4White(prim);
			prim->tpage = 6;
			prim->clut = GetClut(0, id + 0x1e8);
			setUVDataPolyFT4(prim, (i == 0) ? 0xd2 : 0xd6, 0x9c, 5, 8);
			setPosDataPolyFT4(prim, (baseX + 7) + i * 132, y + 18, 4, 7);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
		}
	}

	for (i = 0; i < VS_BATTLE_SETUP.battleCount; i++) {
		VS__setPolyFT4White(prim);
		prim->tpage = 6;
		prim->clut = GetClut(0, 0x1ef);
		if ((i == st->selectedSlot) && (st->unk9 == 0)) {
			setUVDataPolyFT4(prim, (st->unk6 * 12) + 0x96, 0xb2, 12, 12);
			if ((st->unk7 % 5) == 0) {
				st->unk6++;
			}
			st->unk7++;
			if (st->unk7 >= 5) {
				st->unk7 = 0;
			}
			st->unk6 = st->unk6 & 7;
		} else {
			setUVDataPolyFT4(prim, i * 12 + 0x96, 0xa6, 12, 12);
		}
		setPosDataPolyFT4(prim, baseX + 15, (y + 114) + i * 20, 12, 12);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
		if ((st->selectedMask & (1 << i)) != 0) {
			VS__setPolyFT4White(prim);
			prim->tpage = 6;
			prim->clut = GetClut(0, 0x1f1);
			setUVDataPolyFT4(prim, 0x98, 0xc8, 16, 16);
			setPosDataPolyFT4(prim, baseX + 33, (y + 112) + i * 20, 16, 16);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
			for (j = 0; j < 6; j++) {
				VS__setPolyFT4White(prim);
				prim->tpage = 7;
				prim->clut = GetClut(0x30, 0x1e8);
				setUVDataPolyFT4(prim, 0x70, 0x48, 8, 8);
				setPosDataPolyFT4(prim, (baseX + 0x39) + j * 8, (y + 116) + i * 20, 8, 8);
				AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
			}
		}
		VS__setPolyFT4White(prim);
		prim->tpage = 6;
		prim->clut = GetClut(0, id + 0x1e8);
		setUVDataPolyFT4(prim, 0, ((st->selectedMask & (1 << i)) != 0) ? 0xd8 : 0xec, 0x85,
		                 ((st->selectedMask & (1 << i)) != 0) ? 0x14 : 0x13);
		setPosDataPolyFT4(prim, baseX + 8, (y + 110) + i * 20, 0x85, 0x14);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
	}

	for (i = 0; i < rowCount; i++) {
		type = fighters[st->slotIds[baseIdx + i]].digimonId;
		VS__setPolyFT4White(prim);
		prim->tpage = 14;
		prim->clut = GetClut(0x120, DIGIMON_SPRITE_CLUT[type] + 0x1e0);
		if (type == 0x73) {
			spriteU = (st->unk4 * 16) + 0x3e0;
			spriteV = 0xe0;
		} else {
			spriteU = (((type - 3) % 32) * 32) + (st->unk4 * 16);
			spriteV = ((type - 3) / 8) * 32;
		}
		width = (spriteU != 0xf0) ? 16 : 15;
		height = (spriteV != 0xf0) ? 16 : 15;
		setUVDataPolyFT4(prim, spriteU, spriteV, width, height);
		setPosDataPolyFT4(prim, baseX + SPRITE_POS_X[i], y + SPRITE_POS_Y[i], width, height);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);

		text = (uint16_t *)fighters[st->slotIds[baseIdx + i]].name;
		halfLen = strlen((char *)text) / 2;
		for (j = 0; j < halfLen; j++) {
			VS__setPolyFT4White(prim);
			prim->tpage = 7;
			prim->clut = GetClut(0x30, 0x1e8);
			ch = *text++;
			ch = ((ch & 0xff) << 8) + ((ch & 0xff00) >> 8);
			for (k = 0; k < 216; k++) {
				if (ch == VS_FONT_CHARS[k]) {
					glyph = k;
					break;
				}
			}
			setUVDataPolyFT4(prim, (VS_FONT_GLYPHS[glyph] % 15) * 8, (VS_FONT_GLYPHS[glyph] / 15) * 8, 8, 8);
			setPosDataPolyFT4(prim, (baseX + VS__NAME_POS_X[i]) + j * 8, y + VS__NAME_POS_Y[i], 8, 8);
#if defined(VERSION_JP)
			AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
#endif
		}

		panel = &VS__ROW_PANELS[i];
		fighter = &fighters[st->slotIds[baseIdx + i]];
		for (j = 0; j < 3; j++) {
			if (fighter->moves[j] == 0xff) {
				strcpy(buf, STR_SOUBINASHI);
				text = (uint16_t *)buf;
			} else {
				moveId = DIGIMON_DATA[fighter->digimonId].moves[fighter->moves[j] - 0x2e];
				text = (uint16_t *)MOVE_NAMES[moveId];
			}
			halfLen = strlen((char *)text) / 2;
			for (k = 0; k < halfLen; k++) {
				ch = *text++;
				if (k < 5) {
					moveRow = j * 2 + 6;
				} else {
					moveRow = j * 2 + 7;
				}
				if ((st->unk2 <= (STAT_OFFSETS[moveRow] + 8)) && (STAT_OFFSETS[moveRow] <= (st->unk2 + 22))) {
					VS__setPolyFT4White(prim);
					prim->tpage = 7;
					prim->clut = GetClut(0x30, 0x1e8);
					ch = ((ch & 0xff) << 8) + ((ch & 0xff00) >> 8);
					glyph = 0;
					for (l = 0; l < 216; l++) {
						if (ch == VS_FONT_CHARS[l]) {
							glyph = l;
							break;
						}
					}
					if (st->unk2 > STAT_OFFSETS[moveRow]) {
						height = st->unk2 - STAT_OFFSETS[moveRow];
						setUVDataPolyFT4(prim, (VS_FONT_GLYPHS[glyph] % 15) * 8,
						                 height + (VS_FONT_GLYPHS[glyph] / 15) * 8, 8, 8 - height);
						height = 8 - height;
						posY = y + panel->y;
					} else if ((st->unk2 + 22) < (STAT_OFFSETS[moveRow] + 8)) {
						height = (STAT_OFFSETS[moveRow] + 8) - (st->unk2 + 22);
						setUVDataPolyFT4(prim, (VS_FONT_GLYPHS[glyph] % 15) * 8,
						                 (VS_FONT_GLYPHS[glyph] / 15) * 8, 8, 8 - height);
						height = 8 - height;
						posY = STAT_OFFSETS[moveRow] + (y + panel->y) - st->unk2;
					} else {
						height = 8;
						setUVDataPolyFT4(prim, (VS_FONT_GLYPHS[glyph] % 15) * 8,
						                 (VS_FONT_GLYPHS[glyph] / 15) * 8, 8, height);
						posY = STAT_OFFSETS[moveRow] + (y + panel->y) - st->unk2;
					}
					setPosDataPolyFT4(prim,
					                  (k < 5) ? ((baseX + panel->x) + 1) + k * 8
					                          : ((baseX + panel->x) + 1) + (k - 5) * 8,
					                  posY, 8, height);
#if defined(VERSION_JP)
					AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
#endif
				}
			}
		}

		stats = &fighters[st->slotIds[baseIdx + i]].hp;
		for (j = 0; j < 6; j++, stats++) {
			if ((st->unk2 <= (VS__STAT_NUMBER_OFFSETS[j] + 7)) && (VS__STAT_NUMBER_OFFSETS[j] <= (st->unk2 + 22))) {
				convertValueToDigits(4, *stats, &count, digits);
				for (k = count - 1; k >= 0; k--) {
					VS__setPolyFT4White(prim);
					prim->tpage = 6;
					prim->clut = GetClut(0, 0x1f1);
					if (st->unk2 > VS__STAT_NUMBER_OFFSETS[j]) {
						height = st->unk2 - VS__STAT_NUMBER_OFFSETS[j];
						setUVDataPolyFT4(prim, digits[k] * 5 + 0x98, height + 0xc0, 5, 7 - height);
						height = 7 - height;
						posY = y + panel->y;
					} else if ((st->unk2 + 22) < (VS__STAT_NUMBER_OFFSETS[j] + 7)) {
						height = (VS__STAT_NUMBER_OFFSETS[j] + 7) - (st->unk2 + 22);
						setUVDataPolyFT4(prim, digits[k] * 5 + 0x98, 0xc0, 5, 7 - height);
						height = 7 - height;
						posY = VS__STAT_NUMBER_OFFSETS[j] + (y + panel->y) - st->unk2;
					} else {
						height = 7;
						setUVDataPolyFT4(prim, digits[k] * 5 + 0x98, 0xc0, 5, height);
						posY = VS__STAT_NUMBER_OFFSETS[j] + (y + panel->y) - st->unk2;
					}
					setPosDataPolyFT4(prim, ((baseX + panel->x) + 22) + (3 - k) * 5, posY, 5, height);
					AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
				}
			}
		}

		VS__setPolyFT4White(prim);
		prim->tpage = panel->tpage;
		prim->clut = GetClut(panel->clutX, panel->clutY);
		setUVDataPolyFT4(prim, panel->u, panel->v + VS__SELECT_DIGIMON_DATA[(int16_t)id].unk2, panel->w, panel->h);
		setPosDataPolyFT4(prim, baseX + panel->x, y + panel->y, panel->w, panel->h);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
	}

	for (i = 5; i >= 0; i--) {
		panel = &MAIN_D_8012F590[id * 6 + i];
		VS__setPolyFT4White(prim);
		prim->tpage = panel->tpage;
		prim->clut = GetClut(panel->clutX, panel->clutY);
		setUVDataPolyFT4(prim, panel->u, panel->v, panel->w, panel->h);
		setPosDataPolyFT4(prim, baseX + panel->x, y + panel->y, panel->w, panel->h);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
	}
	GsSetWorkBase((PACKET *)prim);
}

void VS__renderPressStartToBeginBox(int32_t id)
{
	renderString(0,
	             UI_BOX_DATA[id].finalPos.x + 6,
	             UI_BOX_DATA[id].finalPos.y + 6,
	             96, 24, 0, 0, 6 - id, 1);
}

int32_t VS__isKeyPressedByAnyPlayer(uint32_t buttons)
{
	uint32_t input;
	uint32_t previous;

	if (buttons & (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS)) {
		return 1;
	}

	input = POLLED_INPUT;
	previous = POLLED_INPUT_PREVIOUS;
	POLLED_INPUT = (POLLED_INPUT >> 16) & 0xffff;
	POLLED_INPUT_PREVIOUS = (POLLED_INPUT_PREVIOUS >> 16) & 0xffff;

	if (buttons & (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS)) {
		POLLED_INPUT = input;
		POLLED_INPUT_PREVIOUS = previous;

		return 2;
	}

	POLLED_INPUT = input;
	POLLED_INPUT_PREVIOUS = previous;

	return 0;
}

int32_t VS__tickSelectMode(void)
{
	RECT rect;
	int32_t i;
	int32_t count1;
	int32_t count2;
	int32_t j;

	count2 = 0;
	count1 = 0;

	for (i = 0; i < 40; ++i) {
		if (VS_DIGIMON_P1_PTR[i].digimonId != 0) {
			++count1;
		}
		if (VS_DIGIMON_P2_PTR[i].digimonId != 0) {
			++count2;
		}
	}

	if (count1 >= 3 && count2 >= 3) {
		if (count1 >= 5 && count2 >= 5) {
			VS__SELECT_BOX_DATA_SELECTION_COUNT = 2;
		} else {
			VS__SELECT_BOX_DATA_SELECTION_COUNT = 1;
		}
	} else {
		VS__SELECT_BOX_DATA_SELECTION_COUNT = 0;
	}

	VS__SELECT_BOX_DATA_SELECTION = 0;
	VS__SELECT_BOX_DATA_ANIM_FRAME = 0;
	VS__SELECT_BOX_DATA_HAS_SELECTED = 0;
	VS__SELECT_BOX_DATA_TIMER = 0;
	setRECT(&rect, -90, -70, 180, 128);
	createStaticUIBox(0, 1, 0, &rect, VS__tickSelectBox,
	                  VS__renderSelectModeBox);

	fadeFromBlack(5);

	for (j = 0; j < 6; ++j) {
		VS_tickFrame();
	}

	while (VS__SELECT_BOX_DATA_HAS_SELECTED == 0) {
		VS_tickFrame();
	}

	fadeToBlack(5);

	for (j = 0; j < 6; ++j) {
		VS_tickFrame();
	}

	VS_BATTLE_SETUP.battleCount = VS__BATTLE_COUNTS[VS__SELECT_BOX_DATA_SELECTION];
	removeStaticUIBox(0);

	if (VS__SELECT_BOX_DATA_HAS_SELECTED == 1) {
		return 1;
	}

	return 0;
}

void VS__tickSelectBox(int32_t id)
{
	uint8_t previous;

	if (VS__SELECT_BOX_DATA_HAS_SELECTED != 0) {
		return;
	}

	if (VS__isKeyPressedByAnyPlayer(CONFIRM_BUTTON) != 0) {
		playSound(0, 3);
		VS__SELECT_BOX_DATA_HAS_SELECTED = 1;
		return;
	}

	++VS__SELECT_BOX_DATA_TIMER;
	if (VS__SELECT_BOX_DATA_TIMER % 8 == 0) {
		VS__SELECT_BOX_DATA_ANIM_FRAME = (VS__SELECT_BOX_DATA_ANIM_FRAME + 1) & 1;
	}

	if (VS__isKeyPressedByAnyPlayer(0x8000) != 0) {
		previous = VS__SELECT_BOX_DATA_SELECTION--;
		if (id == 1) {
			if (VS__SELECT_BOX_DATA_SELECTION == 0xff) {
				VS__SELECT_BOX_DATA_SELECTION = 2;
			}

			playSound(0, 2);
			VS__SELECT_BOX_DATA_ANIM_FRAME = 0;
		} else {
			if (VS__SELECT_BOX_DATA_SELECTION == 0xff) {
				VS__SELECT_BOX_DATA_SELECTION = VS__SELECT_BOX_DATA_SELECTION_COUNT;
			}

			if (previous != VS__SELECT_BOX_DATA_SELECTION) {
				playSound(0, 2);
				VS__SELECT_BOX_DATA_ANIM_FRAME = 0;
			}
		}
	}

	if (VS__isKeyPressedByAnyPlayer(0x2000) != 0) {
		previous = VS__SELECT_BOX_DATA_SELECTION++;
		if (id == 1) {
			VS__SELECT_BOX_DATA_SELECTION %= 3;
			playSound(0, 2);
			VS__SELECT_BOX_DATA_ANIM_FRAME = 0;
		} else {
			VS__SELECT_BOX_DATA_SELECTION %= VS__SELECT_BOX_DATA_SELECTION_COUNT + 1;
			if (previous != VS__SELECT_BOX_DATA_SELECTION) {
				playSound(0, 2);
				VS__SELECT_BOX_DATA_ANIM_FRAME = 0;
			}
		}
	}
}

// clang-format off
void VS__renderSelectModeBox(depth)
	int16_t depth;
// clang-format on
{
	POLY_FT4 *prim;
	VsUISprite *sprite;
	int32_t i;

	prim = (POLY_FT4 *)GsGetWorkBase();
	sprite = MODE_SELECT_SPRITES;

	for (i = 0; i < 8; ++sprite, ++i) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->clut = GetClut(48, sprite->clut);

		if (i > 0 && i < 4 && VS__SELECT_BOX_DATA_SELECTION != i - 1) {
			setRGB0(prim, 0x40, 0x40, 0x40);
		}

		if (i == 4 && VS__SELECT_BOX_DATA_SELECTION != 0) {
			setClut(prim, 48, 501);
		}

		if (i == 5 && VS__SELECT_BOX_DATA_SELECTION != 1) {
			setClut(prim, 48, 501);
		}

		if (i == 6 && VS__SELECT_BOX_DATA_SELECTION != 2) {
			setClut(prim, 48, 501);
		}

		prim->tpage = getTPage(0, 0, 448, 0);

		if (i == VS__SELECT_BOX_DATA_SELECTION + 1 && i > 0 && i < 4) {
			setUVDataPolyFT4(prim,
			                 sprite->u + VS__SELECT_BOX_DATA_ANIM_FRAME * 40,
			                 sprite->v, sprite->w, sprite->h);

			if (i == 3 && VS__SELECT_BOX_DATA_ANIM_FRAME == 1) {
				setClut(prim, 48, 495);
			}
		} else {
			setUVDataPolyFT4(prim, sprite->u, sprite->v, sprite->w,
			                 sprite->h);
		}

		setPosDataPolyFT4(prim, (i != 7) ? sprite->x - 90 : (sprite->x - 90) + VS__SELECT_BOX_DATA_SELECTION * 56, sprite->y - 70,
		                  sprite->w, sprite->h);

		AddPrim((ACTIVE_ORDERING_TABLE->org + 6) - depth, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

int32_t VS__tickSelectMap(void)
{
	RECT rect;
	int32_t i;

	VS__SELECT_BOX_DATA_SELECTION = 0;
	VS__SELECT_BOX_DATA_ANIM_FRAME = 0;
	VS__SELECT_BOX_DATA_HAS_SELECTED = 0;
	VS__SELECT_BOX_DATA_TIMER = 0;
	setRECT(&rect, -90, -70, 180, 128);
	createStaticUIBox(1, 1, 0, &rect, VS__tickSelectBox,
	                  VS__renderSelectMapBox);

	fadeFromBlack(5);

	for (i = 0; i < 6; ++i) {
		VS_tickFrame();
	}

	while (VS__SELECT_BOX_DATA_HAS_SELECTED == 0) {
		VS_tickFrame();
	}

	fadeToBlack(5);

	for (i = 0; i < 6; ++i) {
		VS_tickFrame();
	}

	VS_BATTLE_SETUP.stage = VS__SELECT_BOX_DATA_SELECTION;
	removeStaticUIBox(1);

	if (VS__SELECT_BOX_DATA_HAS_SELECTED == 1) {
		return 1;
	}

	return 0;
}

// clang-format off
void VS__renderSelectMapBox(id)
	int16_t id;
// clang-format on
{
	POLY_FT4 *prim;
	VsUISprite *sprite;
	int32_t i;
	int32_t posX;

	prim = (POLY_FT4 *)GsGetWorkBase();

	if (id == 0) {
		sprite = MODE_SELECT_SPRITES;
	} else {
		sprite = MAP_SELECT_SPRITES;
	}

	for (i = 0; i < 8; ++i) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->clut = GetClut(48, sprite->clut);
		setUVDataPolyFT4(prim, sprite->u, sprite->v, sprite->w,
		                 sprite->h);

		if (i == 1 && VS__SELECT_BOX_DATA_SELECTION != 0) {
			setUVDataPolyFT4(prim, sprite->u + 40, sprite->v,
			                 sprite->w, sprite->h);
			setClut(prim, 48, 499);
		}

		if (i == 2 && VS__SELECT_BOX_DATA_SELECTION != 1) {
			setUVDataPolyFT4(prim, sprite->u + 40, sprite->v,
			                 sprite->w, sprite->h);
			setClut(prim, 48, 499);
		}

		if (i == 3 && VS__SELECT_BOX_DATA_SELECTION != 2) {
			setUVDataPolyFT4(prim, sprite->u + 40, sprite->v,
			                 sprite->w, sprite->h);
			setClut(prim, 48, 499);
		}

		if (i >= 4 && i < 7 && VS__SELECT_BOX_DATA_SELECTION != i - 4) {
			setClut(prim, 48, 492);
		}

		prim->tpage = getTPage(0, 0, 448, 0);
		if (i != 7) {
			posX = sprite->x - 90;
		} else {
			posX = sprite->x - 90 + VS__SELECT_BOX_DATA_SELECTION * 56;
		}

		setPosDataPolyFT4(prim, posX, sprite->y - 70, sprite->w,
		                  sprite->h);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 6 - id, prim++);
		++sprite;
	}

	GsSetWorkBase((PACKET *)prim);
}
