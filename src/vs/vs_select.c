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

extern char *MAIN_D_8013526C;
extern char *MAIN_D_80135270;
extern uint8_t MAIN_D_801B1C7C[];
extern int8_t MAIN_D_80134F52[2];
extern int32_t MAIN_D_80134F54;
extern int16_t MAIN_D_80134F50;
extern uint8_t MAIN_D_801B1CB2[];
extern uint8_t MAIN_D_801B1D02[];
extern uint8_t MAIN_D_80134F58;
extern uint8_t MAIN_D_80134F59;
extern uint8_t MAIN_D_80134F5A;
extern uint8_t MAIN_D_80134F5B;
extern uint8_t MAIN_D_80134F5C;

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
int32_t VS__isAlreadySelected(uint8_t player, int32_t value);
void VS__handleDigimonSelected(uint8_t *state);
void VS__createPressStartToBeginBox(uint8_t id);
void VS__removePressStartToBeginBox(uint8_t id);
void VS__setPolyFT4White(POLY_FT4 *poly);
void VS__initialize(char *namesP1, char *namesP2);
void VS__tickSelectDigimonPlayer(uint8_t id);
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

int16_t MAIN_D_80134528[4] = {
	0x0036, 0x0036, 0x0050, 0x0050,
};

int16_t MAIN_D_80134530[4] = {
	0x0020, 0x0055, 0x0020, 0x0055,
};

int16_t MAIN_D_80134538[4] = {
	0x0009, 0x0009, 0x005c, 0x005c,
};

int16_t MAIN_D_80134540[4] = {
	0x0036, 0x0047, 0x0036, 0x0047,
};

uint8_t MAIN_D_80134548[6] = {
	0x00, 0x0c, 0x18, 0x24, 0x30, 0x3c,
};

uint8_t MAIN_D_80134550[4] = {
	0x01, 0x03, 0x05, 0x00,
};

char MAIN_D_8012F464[] = "\\ETCDAT\\ETCTIM.BIN";

char MAIN_D_8012F478[] = "\\ETCNA\\TITLE2.TIM";

char MAIN_D_8012F48C[] = "\\ETCDAT\\SYSTEM_W.TIM";

char MAIN_D_8012F4A4[20] = "\\STDDAT\\TAISEN1.TIM";

char MAIN_D_8012F4B8[20] = "\\STDDAT\\TAISEN2.TIM";

char MAIN_D_8012F4CC[] = "\\STDDAT\\16TAISEN.TIM";

char MAIN_D_8012F4E4[] = "\\STDDAT\\TAISEN_F.TIM";

char MAIN_D_8012F4FC[] = "\\STDDAT\\TIME.TIM";

char MAIN_D_8012F510[12] = "Press Start";

char MAIN_D_8012F51C[] = "to begin.";

uint8_t MAIN_D_8012F528[68] = {
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

int16_t MAIN_D_8012F56C[6] = {
	0x270f, 0x270f, 0x03e7, 0x03e7, 0x03e7, 0x03e7,
};

uint8_t MAIN_D_8012F578[12] = {
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

VsListPanel MAIN_D_8012F620[4] = {
	{ 0x0040, 0x01e8, 0x00, 0x00, 0x29, 0x16, 0x05, 0x09, 0x1e },
	{ 0x0040, 0x01e8, 0x00, 0x00, 0x29, 0x16, 0x05, 0x09, 0x51 },
	{ 0x0040, 0x01e8, 0x00, 0x00, 0x29, 0x16, 0x05, 0x62, 0x1e },
	{ 0x0040, 0x01e8, 0x00, 0x00, 0x29, 0x16, 0x05, 0x62, 0x51 },
};

VsUISprite MAIN_D_8012F650[8] = {
	{ 0x01e9, 0x78, 0x00, 0x7c, 0x0e, 0x1c, 0x07 },
	{ 0x01ee, 0x00, 0x88, 0x28, 0x30, 0x0e, 0x25 },
	{ 0x01ee, 0x50, 0x88, 0x28, 0x30, 0x46, 0x25 },
	{ 0x01ef, 0xa0, 0x88, 0x28, 0x30, 0x7e, 0x25 },
	{ 0x01f4, 0x00, 0xe8, 0x34, 0x17, 0x0a, 0x60 },
	{ 0x01f4, 0x34, 0xe8, 0x34, 0x17, 0x41, 0x60 },
	{ 0x01f4, 0x68, 0xe8, 0x34, 0x17, 0x7a, 0x60 },
	{ 0x01ee, 0x78, 0x4c, 0x34, 0x3c, 0x08, 0x1f },
};

VsUISprite MAIN_D_8012F690[8] = {
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

void VS__loadTextures(void)
{
	uint8_t *entries[2];
	uint8_t *state;
	uint8_t *p;
	int32_t i;
	int32_t j;
	int32_t rows;

	ENTITY_TABLE[0]->isOnScreen = 0;
	loadTIMFile(MAIN_D_8012F48C, GENERAL_BUFFER);
	loadTIMFile(MAIN_D_8012F4A4, GENERAL_BUFFER);
	loadTIMFile(MAIN_D_8012F4B8, GENERAL_BUFFER);
	loadTIMFile(MAIN_D_8012F4CC, GENERAL_BUFFER);
	loadTIMFile(MAIN_D_8012F4E4, GENERAL_BUFFER);
	loadTIMFile(MAIN_D_8012F4FC, GENERAL_BUFFER);

	entries[0] = (uint8_t *)MAIN_D_8013526C;
	entries[1] = (uint8_t *)MAIN_D_80135270;

	for (i = 0; i < 5; ++i) {
		VS_D_800716A8[i] = 0xff;
		(&VS_D_800716A8[5])[i] = 0xff;
	}

	for (i = 0; i < 2; ++i) {
		state = &MAIN_D_801B1C7C[i * 0x50];
		state[0x2b] = 0;
		state[0x28] = 0;
		state[0x29] = 0;
		state[0x2d] = 0;
		state[0x2e] = 0;
		state[0x2f] = 0;
		state[0x30] = 0;
		state[0x32] = 0;
		state[0x31] = 0;
		state[0x33] = 0;
		state[0x2c] = 0;
		state[0x35] = 0;
		state[0x34] = 0;
		state[0x36] = 0;
		p = &state[0x37];
		for (j = 0; j < 24; ++j) {
			*p = 0;
			++p;
		}

		for (j = 0; j < 40; ++j) {
			if ((entries[i] + j * 64)[0x1c] != 0) {
				state[state[0x2b]] = j;
				++state[0x2b];
			}
		}

		if (state[0x2b] % 4 == 0) {
			state[0x2a] = state[0x2b] / 4;
		} else {
			rows = state[0x2b] / 4;
			++rows;
			state[0x2a] = rows;
		}
	}
}

void VS__tickSelectDigimon(void)
{
	int32_t i;

	MAIN_D_80134F52[0] = -1;
	MAIN_D_80134F52[1] = -1;

	clearTextArea();

	addObject(0x1a0, 0, (TickFunction)VS__tickSelectDigimonPlayer, VS__renderSelectDigimonPlayer);
	addObject(0x1a0, 1, (TickFunction)VS__tickSelectDigimonPlayer, VS__renderSelectDigimonPlayer);

	fadeFromBlack(10);

	for (i = 0; i < 11; ++i) {
		VS_tickFrame();
	}

	MAIN_D_80134F54 = 0;
	while (MAIN_D_80134F54 == 0) {
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

int32_t VS__isAlreadySelected(uint8_t player, int32_t value)
{
	int32_t i;
	uint8_t *table;

	if (player == 0) {
		table = VS_D_800716A8;
	} else {
		table = (&VS_D_800716A8[5]);
	}

	for (i = 0; i < VS_D_800716A8[11]; ++i) {
		if (value == table[i]) {
			playSound(0, 0xb);
			return 1;
		}
	}

	return 0;
}

void VS__handleDigimonSelected(uint8_t *state)
{
	int32_t i;
	int32_t count;

	playSound(0, 3);

	state[0x33] = 0;
	state[0x32] = 0;
	state[0x35] |= 1 << state[0x34];

	count = VS_D_800716A8[11];
	for (i = 0; i < count; ++i) {
		if ((state[0x35] & (1 << i)) == 0) {
			break;
		}
	}

	if (i == VS_D_800716A8[11]) {
		state[0x4f] = state[0x34];
	}

	if (state[0x34] < VS_D_800716A8[11] - 1) {
		++state[0x34];
	}
}

void VS__createPressStartToBeginBox(uint8_t id)
{
	RECT rect;

	if (MAIN_D_80134F52[id] == -1) {
		drawString(MAIN_D_8012F510, 0, 0);
		drawString(MAIN_D_8012F51C, 0, 12);

		setRECT(&rect, (id == 0) ? -132 : 22, 32, 108, 36);
		createStaticUIBox(id, 0, 2, &rect, 0, VS__renderPressStartToBeginBox);

		MAIN_D_80134F52[id] = 1;
	}
}

void VS__removePressStartToBeginBox(uint8_t id)
{
	if (MAIN_D_80134F52[id] != -1) {
		removeStaticUIBox(id);
		MAIN_D_80134F52[id] = -1;
	}
}

void VS__setPolyFT4White(POLY_FT4 *poly)
{
	SetPolyFT4(poly);
	setRGB0(poly, 0x80, 0x80, 0x80);
}

void VS__initialize(char *namesP1, char *namesP2)
{
	int32_t done;

	MAIN_D_8013526C = namesP1;
	MAIN_D_80135270 = namesP2;
	VS__loadTextures();

	MAIN_D_80134F50 = 0;
	done = 0;
	while (done == 0) {
		switch (MAIN_D_80134F50) {
		case 0:
			VS__tickSelectMode();
			++MAIN_D_80134F50;
			break;
		case 1:
			VS__tickSelectMap();
			++MAIN_D_80134F50;
			break;
		case 2:
			VS__tickSelectDigimon();
			++MAIN_D_80134F50;
			break;
		case 3:
			VS_initializeVS();
			done = 1;
			break;
		}
	}

	loadStackedTIMFile(MAIN_D_8012F464);
	loadTIMFile(MAIN_D_8012F478, GENERAL_BUFFER_PTR);
}

void VS__tickSelectDigimonPlayer(uint8_t id)
{
	uint8_t *state;
	uint8_t *table;
	uint32_t input;
	uint32_t previous;
	uint32_t pressed;
	int32_t idx;
	int32_t count;
	int32_t last;
	int32_t row;
	int32_t i;
	int32_t total;

	if (id == 0) {
		table = VS_D_800716A8;
	} else {
		table = (&VS_D_800716A8[5]);
	}

	if (id == 1) {
		input = POLLED_INPUT;
		previous = POLLED_INPUT_PREVIOUS;
		POLLED_INPUT = (input >> 16) & 0xffff;
		POLLED_INPUT_PREVIOUS = (previous >> 16) & 0xffff;
	}

	state = &MAIN_D_801B1C7C[id * 0x50];
	++state[0x31];
	if (state[0x31] % 8 == 0) {
		state[0x30] = (state[0x30] + 1) & 1;
	}

	switch (state[0x28]) {
	case 0:
		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x8000) {
			state[0x29]--;
			if (state[0x29] == 0xff) {
				state[0x29] = state[0x2a] - 1;
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x2000) {
			++state[0x29];
			state[0x29] %= state[0x2a];
		}

		if ((row = state[0x29]) == (last = state[0x2a] - 1)) {
			count = state[0x2b] - last * 4;
		} else {
			count = 4;
		}

		idx = row * 4;

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x10) {
			if (VS__isAlreadySelected(id, state[idx]) == 0) {
				table[state[0x34]] = state[idx];
				VS__handleDigimonSelected(state);
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x40) {
			if (VS__isAlreadySelected(id, state[idx + 3]) == 0 &&
			    count == 4) {
				table[state[0x34]] = (state + idx)[3];
				VS__handleDigimonSelected(state);
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x80) {
			if (VS__isAlreadySelected(id, state[idx + 1]) == 0 &&
			    count >= 2) {
				table[state[0x34]] = (state + idx)[1];
				VS__handleDigimonSelected(state);
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x20) {
			if (VS__isAlreadySelected(id, state[idx + 2]) == 0 &&
			    count >= 3) {
				table[state[0x34]] = (state + idx)[2];
				VS__handleDigimonSelected(state);
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x1000) {
			playSound(0, 2);
			if (state[0x34] != 0) {
				state[0x33] = 0;
			}
			if (state[0x34] != 0) {
				state[0x34]--;
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x4000) {
			playSound(0, 2);
			if (state[0x34] != VS_D_800716A8[11] - 1) {
				state[0x33] = 0;
			}
			if (state[0x34] < VS_D_800716A8[11] - 1) {
				++state[0x34];
			}
		}

		if (state[0x2e] == state[0x2c] * 24) {
			if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x4) {
				state[0x2f] = 0xff;
				if (state[0x2c] != 0) {
					state[0x2c]--;
				}
			}

			if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x8) {
				state[0x2f] = 1;
				if (state[0x2c] < 5) {
					++state[0x2c];
				}
			}
		}

		if (state[0x2e] != state[0x2c] * 24) {
			state[0x2e] += state[0x2f] * 4;
		}

		total = VS_D_800716A8[11];
		for (i = 0; i < total; ++i) {
			if ((state[0x35] & (1 << i)) == 0) {
				break;
			}
		}

		if (i == VS_D_800716A8[11]) {
			VS__createPressStartToBeginBox(id);
			++state[0x28];
		}
		break;
	case 1:
		if ((pressed = POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x800) {
			playSound(0, 3);
			++state[0x28];
			state[0x36] = 1;
			VS__removePressStartToBeginBox(id);
		} else if ((pressed & 0x20) || (pressed & 0x80) ||
		           (pressed & 0x10) || (pressed & 0x40) ||
		           (pressed & 0x8000) || (pressed & 0x2000) ||
		           (pressed & 0x1000) || (pressed & 0x4000)) {
			playSound(0, 4);
			state[0x28]--;
			VS__removePressStartToBeginBox(id);
			state[0x35] = state[0x35] & ~(uint8_t)(1 << state[0x4f]);
			table[state[0x4f]] = 0xff;
		}
		break;
	case 2:
		if (MAIN_D_801B1CB2[0] == 1 && MAIN_D_801B1D02[0] == 1) {
			MAIN_D_80134F54 = 1;
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
	uint8_t baseIdx;
	int16_t glyph;
	int32_t clutId;
	int32_t i;
	int32_t j;
	int32_t rowCount;
	int32_t halfLen;
	int16_t *stats;
	uint16_t *text;
	char *names;
	uint8_t *rec;
	int32_t uvX;
	int32_t cursorY;
	int32_t cursorX;
	int32_t iconX;
	int16_t glyphTop;
	int16_t rowX;
	int16_t y;
	int16_t y4;
	int16_t rowX4;
	int16_t baseX;
	POLY_F4 *bar;
	POLY_FT4 *prim;
	VsListPanel *panel;
	uint8_t *st;
	int32_t type;
	int32_t spriteU;
	int32_t spriteV;
	int32_t ch;
	int32_t rowY;
	int32_t iconY;
	int32_t glyphY;
	int32_t barY;
	int32_t dx;
	int32_t mx;
	int16_t value;
	int16_t width;
	int32_t swidth;
	int16_t height;
	int16_t posY;
	int16_t moveRow;
	int32_t l;
	int32_t k;
	char buf[24];
	int32_t count;
	int32_t digits[4];

	baseX = (int16_t)(id * 154 - 152);
	if (id == 0) {
		names = MAIN_D_8013526C;
	} else {
		names = MAIN_D_80135270;
	}
	clutId = id;
	st = &MAIN_D_801B1C7C[(int16_t)id * 0x50];

	if (st[0x29] == st[0x2a] - 1) {
		rowCount = st[0x2b] - (st[0x2a] - 1) * 4;
	} else {
		rowCount = 4;
	}
	baseIdx = st[0x29] * 4;

	bar = (POLY_F4 *)GsGetWorkBase();
	for (i = 0; i < rowCount; i++) {
		panel = &MAIN_D_8012F620[i];
		stats = (int16_t *)(names + (st + baseIdx)[i] * 64);
		y = -109;
		for (j = 0; j < 6; j++, stats++) {
			width = (int16_t)(*stats * 34 / MAIN_D_8012F56C[j]);
			if (st[0x2e] < MAIN_D_8012F578[j] + 2 &&
			    st[0x2e] + 22 >= MAIN_D_8012F578[j]) {
				SetPolyF4(bar);
				setRGB0(bar, 0, 255, 255);
				setXY4(bar,
				       baseX + panel->x + 7,
				       (y + panel->y) + (MAIN_D_8012F578[j] - (uint8_t)st[0x2e]),
				       width + (baseX + panel->x + 7),
				       (y + panel->y) + (MAIN_D_8012F578[j] - (uint8_t)st[0x2e]),
				       baseX + panel->x + 7,
				       (y + panel->y) + (MAIN_D_8012F578[j] - (uint8_t)st[0x2e]) + 2,
				       width + (baseX + panel->x + 7),
				       (y + panel->y) + (MAIN_D_8012F578[j] - (uint8_t)st[0x2e]) + 2);
				AddPrim(ACTIVE_ORDERING_TABLE->org + 10, bar++);
			}
		}
	}
	GsSetWorkBase((PACKET *)bar);

	prim = (POLY_FT4 *)GsGetWorkBase();
	if (st[0x2a] != 1) {
		dx = (int16_t)((baseX + 75) - st[0x2a] * 11 / 2);
		y = -109;
		for (i = 0; i < st[0x2a]; i++) {
			VS__setPolyFT4White(prim);
			prim->tpage = 6;
			prim->clut = GetClut(0, clutId + 0x1e8);
			setUVDataPolyFT4(prim, (i != st[0x29]) ? 0xc8 : 0xbe,
			                 0x9c, 10, 6);
			setPosDataPolyFT4(prim, dx + i * 11, y + 19, 10, 6);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
		}
		for (i = 0; i < 2; i++) {
			VS__setPolyFT4White(prim);
			prim->tpage = 6;
			prim->clut = GetClut(0, clutId + 0x1e8);
			setUVDataPolyFT4(prim, (i == 0) ? 0xd2 : 0xd6, 0x9c, 5,
			                 8);
			setPosDataPolyFT4(prim, (baseX + 7) + i * 132,
			                  y + 18, 4, 7);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
		}
	}

	uvX = 0x96;
	cursorY = 5;
	cursorX = baseX + 15;
	iconX = baseX + 33;
	glyphTop = 7;
	i = 0;
	rowX = baseX;
	iconY = 3;
	glyphY = 0;
	barY = 1;
	for (; i < VS_D_800716A8[11];
	     i++, barY += 20, cursorY += 20, glyphY += 20, uvX += 12,
	     iconY += 20) {
		VS__setPolyFT4White(prim);
		prim->tpage = 6;
		prim->clut = GetClut(0, 0x1ef);
		if (i == st[0x34] && st[0x36] == 0) {
			setUVDataPolyFT4(prim, st[0x32] * 12 + 0x96, 0xb2, 12,
			                 12);
			if (st[0x33] % 5 == 0) {
				st[0x32]++;
			}
			st[0x33]++;
			if (st[0x33] >= 5) {
				st[0x33] = 0;
			}
			st[0x32] &= 7;
		} else {
			setUVDataPolyFT4(prim, uvX, 0xa6, 12, 12);
		}
		setPosDataPolyFT4(prim, cursorX, cursorY, 12, 12);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
		if ((st[0x35] & (1 << i)) != 0) {
			VS__setPolyFT4White(prim);
			prim->tpage = 6;
			prim->clut = GetClut(0, 0x1f1);
			setUVDataPolyFT4(prim, 0x98, 0xc8, 16, 16);
			setPosDataPolyFT4(prim, iconX, iconY, 16, 16);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
			rowY = glyphTop + glyphY;
			for (j = 0; j < 6; j++) {
				VS__setPolyFT4White(prim);
				prim->tpage = 7;
				prim->clut = GetClut(0x30, 0x1e8);
				setUVDataPolyFT4(prim, 0x70, 0x48, 8, 8);
				setPosDataPolyFT4(prim, (rowX + 0x39) + j * 8,
				                  rowY, 8, 8);
				AddPrim(ACTIVE_ORDERING_TABLE->org + 10,
				        prim++);
			}
		}
		VS__setPolyFT4White(prim);
		prim->tpage = 6;
		prim->clut = GetClut(0, clutId + 0x1e8);
		setUVDataPolyFT4(prim, 0,
		                 ((st[0x35] & (1 << i)) != 0) ? 0xd8 : 0xec, 0x85,
		                 ((st[0x35] & (1 << i)) != 0) ? 0x14 : 0x13);
		setPosDataPolyFT4(prim, baseX + 8, barY, 0x85, 0x14);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
	}

	y4 = -109;
	rowX4 = baseX;
	for (i = 0; i < rowCount; i++) {
		panel = &MAIN_D_8012F620[i];
		type = ((uint8_t *)names + (st + baseIdx)[i] * 64)[0x1c];
		VS__setPolyFT4White(prim);
		prim->tpage = 14;
		prim->clut = GetClut(0x120, MAIN_D_8012F528[type] + 0x1e0);
		if (type == 0x73) {
			spriteU = (uint8_t)(st[0x30] * 16 + 0x3e0);
			spriteV = 0xe0;
		} else {
			spriteU = (uint8_t)(((type - 3) % 32) * 32 +
			                    st[0x30] * 16);
			spriteV = (uint8_t)(((type - 3) / 8) * 32);
		}
		width = (int16_t)((spriteU != 0xf0) ? 16 : 15);
		height = (spriteV != 0xf0) ? 16 : 15;
		setUVDataPolyFT4(prim, spriteU, spriteV, width, height);
		setPosDataPolyFT4(prim, rowX4 + MAIN_D_80134528[i],
		                  y4 + MAIN_D_80134530[i], width, height);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);

		text = (uint16_t *)(names + (st + baseIdx)[i] * 64 + 14);
		halfLen = strlen((char *)text) / 2;
		y = -109;
		for (j = 0; j < halfLen; j++) {
			VS__setPolyFT4White(prim);
			prim->tpage = 7;
			prim->clut = GetClut(0x30, 0x1e8);
			ch = *text++;
			ch = (uint16_t)(((ch & 0xff) << 8) +
			                ((ch & 0xff00) >> 8));
			for (k = 0; k < 216; k++) {
				if (ch == VS_D_8006FBC0[k]) {
					glyph = k;
					break;
				}
			}
			setUVDataPolyFT4(prim, (VS_D_8006FD70[glyph] % 15) * 8,
			                 (VS_D_8006FD70[glyph] / 15) * 8, 8, 8);
			setPosDataPolyFT4(prim,
			                  (baseX + MAIN_D_80134538[i]) + j * 8,
			                  y + MAIN_D_80134540[i], 8, 8);
		}

		rec = (uint8_t *)(names + (st + baseIdx)[i] * 64);
		for (j = 0; j < 3; j++) {
			if ((rec + j)[0x1d] == 0xff) {
				strcpy(buf, STR_SOUBINASHI);
				text = (uint16_t *)buf;
			} else {
				text = (uint16_t *)MOVE_NAMES[DIGIMON_DATA[rec[0x1c]]
				                                      .moves[(rec + j)[0x1d] -
				                                             0x2e]];
			}
			halfLen = strlen((char *)text) / 2;
			y = -109;
			for (k = 0; k < halfLen; k++) {
				ch = *text++;
				if (k < 5) {
					moveRow = j * 2 + 6;
				} else {
					moveRow = j * 2 + 7;
				}
				if (st[0x2e] <= MAIN_D_8012F578[moveRow] + 8 &&
				    MAIN_D_8012F578[moveRow] <= st[0x2e] + 22) {
					VS__setPolyFT4White(prim);
					prim->tpage = 7;
					prim->clut = GetClut(0x30, 0x1e8);
					ch = (uint16_t)(((ch & 0xff) << 8) +
					                ((ch & 0xff00) >> 8));
					glyph = 0;
					for (l = 0; l < 216; l++) {
						if (ch == VS_D_8006FBC0[l]) {
							glyph = l;
							break;
						}
					}
					if (st[0x2e] > MAIN_D_8012F578[moveRow]) {
						height = st[0x2e] -
						         MAIN_D_8012F578[moveRow];
						value = VS_D_8006FD70[glyph];
						setUVDataPolyFT4(
							prim,
							(value % 15) * 8,
							height + (value / 15) * 8,
							8, 8 - height);
						height = 8 - height;
						posY = y + panel->y;
					} else if (st[0x2e] + 22 <
					           MAIN_D_8012F578[moveRow] + 8) {
						height = (MAIN_D_8012F578[moveRow] +
						          8) -
						         (st[0x2e] + 22);
						setUVDataPolyFT4(
							prim,
							(VS_D_8006FD70[glyph] % 15) * 8,
							(VS_D_8006FD70[glyph] / 15) * 8, 8,
							8 - height);
						height = 8 - height;
						posY = MAIN_D_8012F578[moveRow] +
						       (y + panel->y) -
						       st[0x2e];
					} else {
						height = 8;
						setUVDataPolyFT4(
							prim,
							(VS_D_8006FD70[glyph] % 15) * 8,
							(VS_D_8006FD70[glyph] / 15) * 8, 8, 8);
						posY = MAIN_D_8012F578[moveRow] +
						       (y + panel->y) -
						       st[0x2e];
					}
					if (k < 5) {
						mx = ((baseX + panel->x) + 1) +
						     k * 8;
					} else {
						mx = ((baseX + panel->x) + 1) +
						     (k - 5) * 8;
					}
					setPosDataPolyFT4(prim, mx, posY, 8,
					                  height);
				}
			}
		}

		stats = (int16_t *)(names + (st + baseIdx)[i] * 64);
		for (j = 0; j < 6; j++, stats++) {
			if (st[0x2e] <= MAIN_D_80134548[j] + 7 &&
			    MAIN_D_80134548[j] <= st[0x2e] + 22) {
				convertValueToDigits(4, *stats, &count, digits);
				y = -109;
				for (k = count - 1; k >= 0; k--) {
					VS__setPolyFT4White(prim);
					prim->tpage = 6;
					prim->clut = GetClut(0, 0x1f1);
					if (st[0x2e] > MAIN_D_80134548[j]) {
						height = st[0x2e] -
						         MAIN_D_80134548[j];
						setUVDataPolyFT4(
							prim,
							digits[k] * 5 + 0x98,
							height + 0xc0, 5,
							7 - height);
						height = 7 - height;
						posY = y + panel->y;
					} else if (st[0x2e] + 22 <
					           MAIN_D_80134548[j] + 7) {
						height = (MAIN_D_80134548[j] +
						          7) -
						         (st[0x2e] + 22);
						setUVDataPolyFT4(
							prim,
							digits[k] * 5 + 0x98,
							0xc0, 5, 7 - height);
						height = 7 - height;
						posY = MAIN_D_80134548[j] +
						       (y + panel->y) -
						       st[0x2e];
					} else {
						swidth = 7;
						height = swidth;
						setUVDataPolyFT4(
							prim,
							digits[k] * 5 + 0x98,
							0xc0, 5, 7);
						posY = MAIN_D_80134548[j] +
						       (y + panel->y) -
						       st[0x2e];
					}
					setPosDataPolyFT4(
						prim,
						((baseX + panel->x) + 22) +
							(3 - k) * 5,
						posY, 5, height);
					AddPrim(ACTIVE_ORDERING_TABLE->org + 10,
					        prim++);
				}
			}
		}

		VS__setPolyFT4White(prim);
		prim->tpage = panel->tpage;
		prim->clut = GetClut(panel->clutX, panel->clutY);
		setUVDataPolyFT4(prim, panel->u,
		                 panel->v +
		                         (MAIN_D_801B1C7C + (int16_t)id * 0x50)[0x2e],
		                 panel->w, panel->h);
		setPosDataPolyFT4(prim, rowX4 + panel->x, y4 + panel->y,
		                  panel->w, panel->h);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 10, prim++);
	}

	y = -109;
	for (i = 5; i >= 0; i--) {
		panel = &MAIN_D_8012F590[id * 6 + i];
		VS__setPolyFT4White(prim);
		prim->tpage = panel->tpage;
		prim->clut = GetClut(panel->clutX, panel->clutY);
		setUVDataPolyFT4(prim, panel->u, panel->v, panel->w,
		                 panel->h);
		setPosDataPolyFT4(prim, baseX + panel->x, y + panel->y,
		                  panel->w, panel->h);
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

	if (buttons & ((input = POLLED_INPUT) & ~(previous = POLLED_INPUT_PREVIOUS))) {
		return 1;
	}

	POLLED_INPUT = (input >> 16) & 0xffff;
	POLLED_INPUT_PREVIOUS = (previous >> 16) & 0xffff;

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
		if (((uint8_t (*)[64])MAIN_D_8013526C)[i][0x1c] != 0) {
			++count1;
		}
		if (((uint8_t (*)[64])MAIN_D_80135270)[i][0x1c] != 0) {
			++count2;
		}
	}

	if (count1 >= 3 && count2 >= 3) {
		if (count1 >= 5 && count2 >= 5) {
			MAIN_D_80134F58 = 2;
		} else {
			MAIN_D_80134F58 = 1;
		}
	} else {
		MAIN_D_80134F58 = 0;
	}

	setRECT(&rect, -90, -70, 180, 128);
	MAIN_D_80134F59 = 0;
	MAIN_D_80134F5A = 0;
	MAIN_D_80134F5B = 0;
	MAIN_D_80134F5C = 0;
	createStaticUIBox(0, 1, 0, &rect, VS__tickSelectBox,
	                  VS__renderSelectModeBox);

	fadeFromBlack(5);

	for (j = 0; j < 6; ++j) {
		VS_tickFrame();
	}

	while (MAIN_D_80134F5B == 0) {
		VS_tickFrame();
	}

	fadeToBlack(5);

	for (j = 0; j < 6; ++j) {
		VS_tickFrame();
	}

	VS_D_800716A8[11] = MAIN_D_80134550[MAIN_D_80134F59];
	removeStaticUIBox(0);

	if (MAIN_D_80134F5B == 1) {
		return 1;
	}

	return 0;
}

void VS__tickSelectBox(int32_t id)
{
	uint8_t previous;

	if (MAIN_D_80134F5B != 0) {
		return;
	}

	if (VS__isKeyPressedByAnyPlayer(0x40) != 0) {
		playSound(0, 3);
		MAIN_D_80134F5B = 1;
		return;
	}

	++MAIN_D_80134F5C;
	if (MAIN_D_80134F5C % 8 == 0) {
		MAIN_D_80134F5A = (MAIN_D_80134F5A + 1) & 1;
	}

	if (VS__isKeyPressedByAnyPlayer(0x8000) != 0) {
		previous = MAIN_D_80134F59--;
		if (id == 1) {
			if (MAIN_D_80134F59 == 0xff) {
				MAIN_D_80134F59 = 2;
			}

			playSound(0, 2);
			MAIN_D_80134F5A = 0;
		} else {
			if (MAIN_D_80134F59 == 0xff) {
				MAIN_D_80134F59 = MAIN_D_80134F58;
			}

			if (previous != MAIN_D_80134F59) {
				playSound(0, 2);
				MAIN_D_80134F5A = 0;
			}
		}
	}

	if (VS__isKeyPressedByAnyPlayer(0x2000) != 0) {
		previous = MAIN_D_80134F59++;
		if (id == 1) {
			MAIN_D_80134F59 %= 3;
			playSound(0, 2);
			MAIN_D_80134F5A = 0;
		} else {
			MAIN_D_80134F59 %= MAIN_D_80134F58 + 1;
			if (previous != MAIN_D_80134F59) {
				playSound(0, 2);
				MAIN_D_80134F5A = 0;
			}
		}
	}
}

void VS__renderSelectModeBox(int32_t depth)
{
	POLY_FT4 *prim;
	VsUISprite *sprite;
	int32_t i;
	int32_t j;
	int32_t x;

	prim = (POLY_FT4 *)GsGetWorkBase();
	sprite = MAIN_D_8012F650;

	for (i = 0, j = -1; i < 8; ++sprite, ++i, ++j) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->clut = GetClut(48, sprite->clut);

		if (i > 0 && i < 4 && MAIN_D_80134F59 != j) {
			setRGB0(prim, 0x40, 0x40, 0x40);
		}

		if (i == 4 && MAIN_D_80134F59 != 0) {
			setClut(prim, 48, 501);
		}

		if (i == 5 && MAIN_D_80134F59 != 1) {
			setClut(prim, 48, 501);
		}

		if (i == 6 && MAIN_D_80134F59 != 2) {
			setClut(prim, 48, 501);
		}

		prim->tpage = getTPage(0, 0, 448, 0);

		if (i == MAIN_D_80134F59 + 1 && i > 0 && i < 4) {
			setUVDataPolyFT4(prim,
			                 sprite->u + MAIN_D_80134F5A * 40,
			                 sprite->v, sprite->w, sprite->h);

			if (i == 3 && MAIN_D_80134F5A == 1) {
				setClut(prim, 48, 495);
			}
		} else {
			setUVDataPolyFT4(prim, sprite->u, sprite->v, sprite->w,
			                 sprite->h);
		}

		if (i != 7) {
			x = sprite->x - 90;
		} else {
			x = (sprite->x - 90) + MAIN_D_80134F59 * 56;
		}

		setPosDataPolyFT4(prim, x, sprite->y - 70, sprite->w,
		                  sprite->h);

		AddPrim((ACTIVE_ORDERING_TABLE->org + 6) - depth, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

int32_t VS__tickSelectMap(void)
{
	RECT rect;
	int32_t i;

	setRECT(&rect, -90, -70, 180, 128);
	MAIN_D_80134F59 = 0;
	MAIN_D_80134F5A = 0;
	MAIN_D_80134F5B = 0;
	MAIN_D_80134F5C = 0;
	createStaticUIBox(1, 1, 0, &rect, VS__tickSelectBox,
	                  VS__renderSelectMapBox);

	fadeFromBlack(5);

	for (i = 0; i < 6; ++i) {
		VS_tickFrame();
	}

	while (MAIN_D_80134F5B == 0) {
		VS_tickFrame();
	}

	fadeToBlack(5);

	for (i = 0; i < 6; ++i) {
		VS_tickFrame();
	}

	VS_D_800716A8[10] = MAIN_D_80134F59;
	removeStaticUIBox(1);

	if (MAIN_D_80134F5B == 1) {
		return 1;
	}

	return 0;
}

void VS__renderSelectMapBox(int32_t id)
{
	POLY_FT4 *prim;
	VsUISprite *sprite;
	int32_t i;
	int32_t posX;

	prim = (POLY_FT4 *)GsGetWorkBase();

	if (id == 0) {
		sprite = MAIN_D_8012F650;
	} else {
		sprite = MAIN_D_8012F690;
	}

	for (i = 0; i < 8; ++i) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->clut = GetClut(48, sprite->clut);
		setUVDataPolyFT4(prim, sprite->u, sprite->v, sprite->w,
		                 sprite->h);

		if (i == 1 && MAIN_D_80134F59 != 0) {
			setUVDataPolyFT4(prim, sprite->u + 40, sprite->v,
			                 sprite->w, sprite->h);
			setClut(prim, 48, 499);
		}

		if (i == 2 && MAIN_D_80134F59 != 1) {
			setUVDataPolyFT4(prim, sprite->u + 40, sprite->v,
			                 sprite->w, sprite->h);
			setClut(prim, 48, 499);
		}

		if (i == 3 && MAIN_D_80134F59 != 2) {
			setUVDataPolyFT4(prim, sprite->u + 40, sprite->v,
			                 sprite->w, sprite->h);
			setClut(prim, 48, 499);
		}

		if (i >= 4 && i < 7 && MAIN_D_80134F59 != i - 4) {
			setClut(prim, 48, 492);
		}

		prim->tpage = getTPage(0, 0, 448, 0);
		if (i != 7) {
			posX = sprite->x - 90;
		} else {
			posX = sprite->x - 90 + MAIN_D_80134F59 * 56;
		}

		setPosDataPolyFT4(prim, posX, sprite->y - 70, sprite->w,
		                  sprite->h);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 6 - id, prim++);
		++sprite;
	}

	GsSetWorkBase((PACKET *)prim);
}
