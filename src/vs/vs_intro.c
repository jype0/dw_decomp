#include <string.h>

#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#include <dw/anim.h>
#include <dw/btl.h>
#include <dw/entity.h>
#include <dw/fade.h>
#include <dw/file_queue.h>
#include <dw/font.h>
#include <dw/graphics.h>
#include <dw/input.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/version.h>
#include <dw/vs.h>
#include <dw/world_object.h>

extern char MAIN_D_80124C54[];
extern CameraChase VS_INTRO_CAMERA_CHASE;
#if !VERSION_IS(US)
extern int16_t VS__INTRO_STATS_DATA[6];
#else
int16_t VS__INTRO_STATS_DATA[6];
#endif
extern uint8_t VS_MUSIC;

void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
                  int32_t f, int32_t g, int32_t h, int32_t i);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
                          int32_t *digits);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uvX, int32_t uvY,
                      int32_t width, int32_t height);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY,
                       int32_t width, int32_t height);
void SetPolyGT4(POLY_GT4 *prim);

void VS__placePlayer1(int32_t stage);
void VS__placePlayer2(int32_t stage);
void VS__drawStatLabelText(void);
void VS__addIntroText(Entity *entity, int32_t id);
void VS__setPostIntroPosition(Entity *entity);
void VS__removeIntroText(int32_t id);
void VS__removeIntroStats(int32_t id);
void VS__addIntroStats(Entity *entity, int32_t id);
void VS__renderIntroStatBar(int32_t stat, int32_t value);
void VS__renderIntroNameChar(int16_t x, int16_t y, int16_t size,
                             uint8_t character);
void VS__runIntro(int32_t stage);
void VS__tickIntroStats(int32_t id);
void VS__renderIntroStats(int32_t id);
void VS__renderIntroStatNumber(int32_t x, int32_t y, int32_t digits, int32_t value,
                               int32_t layer);
void VS__tickIntroName(int32_t id);
void VS__renderIntroName(int32_t id);

static void *vs_intro_functions[] = {
#if VERSION_IS(EU)
	VS__tickIntroName,
	VS__renderIntroName,
	VS__renderIntroStatNumber,
	VS__tickIntroStats,
	VS__renderIntroStats,
#else
	VS__renderIntroName,
	VS__tickIntroName,
	VS__renderIntroStatNumber,
	VS__renderIntroStats,
	VS__tickIntroStats,
#endif
	VS__runIntro,
	VS__renderIntroNameChar,
	VS__renderIntroStatBar,
	VS__addIntroStats,
	VS__removeIntroStats,
	VS__removeIntroText,
	VS__setPostIntroPosition,
	VS__addIntroText,
	VS__drawStatLabelText,
	VS__placePlayer2,
	VS__placePlayer1,
};

// clang-format off
uint8_t VS__INTRO_NAME_CHAR_SIZES[4] = {
	0x40, 0x2c, 0x26, 0x20,
};

uint8_t VS__INTRO_NAME_CHAR_OFFSETS[4] = {
	0x10, 0x07, 0x03, 0x00,
};

char VS__STR_HP[] = "ＨＰ";

char VS__STR_MP[] = "ＭＰ";

int16_t VS__STAT_BAR_LIMITS[6] = {
	0x270f, 0x270f, 0x03e7, 0x03e7, 0x03e7, 0x03e7,
};

RGB8 VS__INTRO_NAME_COLORS[10] = {
	{ 0x80, 0x80, 0x80 },
	{ 0xc8, 0x64, 0x32 },
	{ 0x1e, 0xff, 0x1e },
	{ 0xd0, 0x1e, 0x50 },
	{ 0x1e, 0x80, 0x80 },
	{ 0xc8, 0xc8, 0x00 },
	{ 0x32, 0xb4, 0xc8 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
};

// clang-format on

int32_t VS__INTRO_STATS_ACTIVE;
int16_t VS__INTRO_DATA_FRAME_COUNT;
int16_t VS__INTRO_DATA_RENDERED_CHARACTERS;
uint8_t VS__INTRO_DATA_COLOR;
int16_t VS__INTRO_DATA_POS_X;
int16_t VS__INTRO_DATA_POS_Y;
uint8_t VS__INTRO_DATA_ANIM_FRAME;

static void *vs_intro_sbss_order[] = {
	&VS__INTRO_DATA_ANIM_FRAME,
#if VERSION_IS(EU)
	&VS__INTRO_DATA_FRAME_COUNT,
	&VS__INTRO_DATA_RENDERED_CHARACTERS,
	&VS__INTRO_DATA_COLOR,
	&VS__INTRO_DATA_POS_X,
	&VS__INTRO_DATA_POS_Y,
#else
	&VS__INTRO_DATA_POS_Y,
	&VS__INTRO_DATA_POS_X,
	&VS__INTRO_DATA_COLOR,
	&VS__INTRO_DATA_RENDERED_CHARACTERS,
	&VS__INTRO_DATA_FRAME_COUNT,
#endif
	&VS__INTRO_STATS_ACTIVE,
};

// clang-format off
void VS__placePlayer1(stage)
	int16_t stage;
// clang-format on
{
	int16_t startX;

	if (stage == 1) {
		startX = 1800;
	} else {
		startX = 2500;
	}

	setEntityPosition(1, startX, 0, 0);
	setEntityRotation(1, 0, 0x400, 0);
	startAnimation(ENTITY_TABLE[1], 33);
}

// clang-format off
void VS__placePlayer2(stage)
	int16_t stage;
// clang-format on
{
	int16_t startX;

	if (stage == 1) {
		startX = -1800;
	} else {
		startX = -2500;
	}

	setEntityPosition(2, startX, 0, 0);
	setEntityRotation(2, 0, 0xc00, 0);
	startAnimation(ENTITY_TABLE[2], 33);
}

void VS__drawStatLabelText(void)
{
	int32_t i;

	clearTextArea();
	drawString(VS__STR_HP, 0, 0);
	drawString(VS__STR_MP, 0, 12);

	for (i = 2; i < 6; i++) {
		drawString(MAIN_D_80124C0C[i], 0, i * 12);
		DrawSync(0);
	}

	drawString(MAIN_D_80124C54, 0, 0xf0);
}

// clang-format off
void VS__addIntroText(entity, id)
	Entity *entity;
	int16_t id;
// clang-format on
{
	int32_t len;

	VS__INTRO_DATA_FRAME_COUNT = 4;
	VS__INTRO_DATA_RENDERED_CHARACTERS = 0;

	switch (DIGIMON_DATA[entity->type].special[0]) {
	case 0:
		VS__INTRO_DATA_COLOR = 3;
		break;
	case 1:
		VS__INTRO_DATA_COLOR = 1;
		break;
	case 2:
		VS__INTRO_DATA_COLOR = 6;
		break;
	case 3:
		VS__INTRO_DATA_COLOR = 2;
		break;
	case 4:
		VS__INTRO_DATA_COLOR = 4;
		break;
	case 5:
		VS__INTRO_DATA_COLOR = 0;
		break;
	case 6:
		VS__INTRO_DATA_COLOR = 5;
		break;
	default:
		VS__INTRO_DATA_COLOR = 0;
		break;
	}

	len = strlen(DIGIMON_NAME(entity->type)) / 2;
	if (entity->type == 0x4e || entity->type == 0x3c) {
		len = 10;
	}
#if VERSION_IS(EU)
	if (len > 10) {
		len = 10;
	}
#endif

	VS__INTRO_DATA_POS_X = -(len * 16);
#if VERSION_IS(EU)
	VS__INTRO_DATA_POS_Y = 36;
#else
	VS__INTRO_DATA_POS_Y = 68;
#endif
	addObject(0x1ab, id, VS__tickIntroName, VS__renderIntroName);
}

void VS__setPostIntroPosition(Entity *entity)
{
	if (VS_INTRO_CAMERA_CHASE.timer != -1) {
		entity->posData->location = VS_INTRO_TARGET_POS;
		entity->anim.locX = VS_INTRO_TARGET_POS.vx << 15;
		entity->anim.locY = VS_INTRO_TARGET_POS.vy << 15;
		entity->anim.locZ = VS_INTRO_TARGET_POS.vz << 15;
		startAnimation(entity, 0x21);
		VS_INTRO_CAMERA_CHASE.timer = -1;
	}
}

// clang-format off
void VS__removeIntroText(id)
	int16_t id;
// clang-format on
{
	removeObject(0x1ab, id);
}

// clang-format off
void VS__removeIntroStats(id)
	int16_t id;
// clang-format on
{
	if (VS__INTRO_STATS_ACTIVE != 0) {
		VS__INTRO_STATS_ACTIVE = 0;
		removeObject(0x1a9, id);
	}
}

// clang-format off
void VS__addIntroStats(entity, id)
	Entity *entity;
	int16_t id;
// clang-format on
{
	if (VS__INTRO_STATS_ACTIVE != 1) {
		VS__INTRO_STATS_ACTIVE = 1;
		VS__INTRO_STATS_DATA[0] = -100;
		VS__INTRO_STATS_DATA[1] = -100;
		VS__INTRO_STATS_DATA[2] = -10;
		VS__INTRO_STATS_DATA[3] = -10;
		VS__INTRO_STATS_DATA[4] = -10;
		VS__INTRO_STATS_DATA[5] = -10;
		addObject(0x1a9, id, VS__tickIntroStats, VS__renderIntroStats);
	}
}

void VS__renderIntroStatBar(int32_t stat, int32_t value)
{
	POLY_F4 *prim;

	prim = (POLY_F4 *)GsGetWorkBase();

	SetPolyF4(prim);
	setRGB0(prim, 80, 200, 80);
	setXY4(prim,
	       -50, stat * 16 - 26,
	       value * 100 / VS__STAT_BAR_LIMITS[stat] - 50, stat * 16 - 26,
	       -50, stat * 16 - 18,
	       value * 100 / VS__STAT_BAR_LIMITS[stat] - 50, stat * 16 - 18);
#if !VERSION_IS(US)
	AddPrim(ACTIVE_ORDERING_TABLE->org + 3, prim++);
#endif

	GsSetWorkBase((PACKET *)prim);
}

void VS__renderIntroNameChar(int16_t x, int16_t y, int16_t size,
                             uint8_t character)
{
	POLY_GT4 *prim;
	uint8_t u;
	uint8_t v;

	prim = (POLY_GT4 *)GsGetWorkBase();

	SetPolyGT4(prim);
	prim->tpage = getTPage(0, 0, 768, 0);
	setClut(prim, 0, 480);
	setRGB0(prim, VS__INTRO_NAME_COLORS[VS__INTRO_DATA_COLOR].r, VS__INTRO_NAME_COLORS[VS__INTRO_DATA_COLOR].g, VS__INTRO_NAME_COLORS[VS__INTRO_DATA_COLOR].b);
	setRGB1(prim, VS__INTRO_NAME_COLORS[VS__INTRO_DATA_COLOR].r, VS__INTRO_NAME_COLORS[VS__INTRO_DATA_COLOR].g, VS__INTRO_NAME_COLORS[VS__INTRO_DATA_COLOR].b);
	setRGB2(prim, VS__INTRO_NAME_COLORS[VS__INTRO_DATA_COLOR].r / 10, VS__INTRO_NAME_COLORS[VS__INTRO_DATA_COLOR].g / 10, VS__INTRO_NAME_COLORS[VS__INTRO_DATA_COLOR].b / 10);
	setRGB3(prim, VS__INTRO_NAME_COLORS[VS__INTRO_DATA_COLOR].r / 10, VS__INTRO_NAME_COLORS[VS__INTRO_DATA_COLOR].g / 10, VS__INTRO_NAME_COLORS[VS__INTRO_DATA_COLOR].b / 10);

	u = (character % 32) * 32;
	v = (character / 8) * 32;

	if (size < 64) {
		setUVWH(prim, u, v, (u != 0xe0 ? 32 : 31), (v != 0xe0 ? 32 : 31));
	} else {
		setUVWH(prim, u, v, 31, 31);
	}

	setXYWH(prim, x, y, size, size);
#if !VERSION_IS(US)
	AddPrim(ACTIVE_ORDERING_TABLE->org + 3, prim++);
#endif

	GsSetWorkBase((PACKET *)prim);
}

// clang-format off
void VS__runIntro(stage)
#if !VERSION_IS(US)
	int16_t stage;
#else
	int32_t stage;
#endif
// clang-format on
{
	int32_t x;
	int32_t i;
	uint32_t pad;
	uint32_t prev;

	if (stage == 1) {
		x = 1300;
	} else {
		x = 2000;
	}

	VS__placePlayer1(stage);
	VS__placePlayer2(stage);
	VS__drawStatLabelText();
	VS_startCameraChase(ENTITY_TABLE[1], x, 0);
	VS__addIntroText(ENTITY_TABLE[1], 1);
	stopBGM();
	stopSound();
	playMusic(VS_MUSIC, 0);

	i = 0;
	fadeFromBlack(5);

	for (; i < 6; ++i) {
		VS_tickFrame();
	}

	while (ENTITY_TABLE[1]->anim.animFlag & 1) {
		pad = PadRead(1);
		VS_tickFrame();

		if ((pad & ~prev) & CONFIRM_BUTTON) {
			VS__setPostIntroPosition(ENTITY_TABLE[1]);
			prev = pad;
			break;
		}

		prev = pad;
	}

	VS__removeIntroText(1);
	VS__removeIntroStats(1);
	removeObject(0x1aa, 0);
	VS_startCameraChase(ENTITY_TABLE[2], -x, 1);
	VS__addIntroText(ENTITY_TABLE[2], 2);
	stopBGM();
	stopSound();
	playMusic(VS_MUSIC, 1);

	while (ENTITY_TABLE[2]->anim.animFlag & 1) {
		pad = PadRead(1);
		pad = (pad >> 16) & 0xffff;
		prev = (prev >> 16) & 0xffff;

		VS_tickFrame();

		if ((pad & ~prev) & CONFIRM_BUTTON) {
			prev = pad;
			VS__setPostIntroPosition(ENTITY_TABLE[2]);
			break;
		}

		prev = pad;
	}

	VS__removeIntroText(2);
	VS__removeIntroStats(2);
	removeObject(0x1aa, 0);
	stopBGM();
	stopSound();
}

// clang-format off
void VS__tickIntroStats(id)
	int16_t id;
// clang-format on
{
	Stats *stats;

	VS__INTRO_STATS_DATA[0] += 200;
	VS__INTRO_STATS_DATA[1] += 200;
	VS__INTRO_STATS_DATA[2] += 20;
	VS__INTRO_STATS_DATA[3] += 20;
	VS__INTRO_STATS_DATA[4] += 20;
	VS__INTRO_STATS_DATA[5] += 20;

	stats = &((DigimonEntity *)ENTITY_TABLE[id])->stats;
	if (stats->current.currentHP < VS__INTRO_STATS_DATA[0]) {
		VS__INTRO_STATS_DATA[0] = stats->current.currentHP;
	}

	if (stats->current.currentMP < VS__INTRO_STATS_DATA[1]) {
		VS__INTRO_STATS_DATA[1] = stats->current.currentMP;
	}

	if (stats->base.off < VS__INTRO_STATS_DATA[2]) {
		VS__INTRO_STATS_DATA[2] = stats->base.off;
	}

	if (stats->base.def < VS__INTRO_STATS_DATA[3]) {
		VS__INTRO_STATS_DATA[3] = stats->base.def;
	}

	if (stats->base.speed < VS__INTRO_STATS_DATA[4]) {
		VS__INTRO_STATS_DATA[4] = stats->base.speed;
	}

	if (stats->base.brain < VS__INTRO_STATS_DATA[5]) {
		VS__INTRO_STATS_DATA[5] = stats->base.brain;
	}
}

// clang-format off
void VS__renderIntroStats(id)
	int16_t id;
// clang-format on
{
	Stats *stats;
	int32_t i;

	for (i = 0; i < 6; ++i) {
		renderString(0, -100, i * 16 - 28, 48, 12, 0, i * 12, 0, 1);
		VS__renderIntroStatBar((int16_t)i, VS__INTRO_STATS_DATA[i]);
	}

	stats = &((DigimonEntity *)ENTITY_TABLE[id])->stats;
	if (VS__INTRO_STATS_DATA[0] != stats->current.currentHP ||
	    VS__INTRO_STATS_DATA[1] != stats->current.currentMP ||
	    VS__INTRO_STATS_DATA[2] != stats->base.off ||
	    VS__INTRO_STATS_DATA[3] != stats->base.def ||
	    VS__INTRO_STATS_DATA[4] != stats->base.speed ||
	    VS__INTRO_STATS_DATA[5] != stats->base.brain) {
		playSound(0, 0x16);
	} else {
		for (i = 0; i < 6; ++i) {
			VS__renderIntroStatNumber(52, (int16_t)(i * 16 - 28), 4,
			                          VS__INTRO_STATS_DATA[i], 3);
		}
	}
}

// clang-format off
void VS__renderIntroStatNumber(x, y, digits, value, layer)
	int16_t x;
	int16_t y;
	int16_t digits;
	int32_t value;
	int32_t layer;
// clang-format on
{
	POLY_FT4 *prim;
	int32_t i;
	int32_t count;
	int32_t buf[6];

	prim = (POLY_FT4 *)GsGetWorkBase();

	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 832, 0);
		setClut(prim, 16, 480);
		setUVDataPolyFT4(prim, buf[i] * 12, 32, 12, 12);
		setPosDataPolyFT4(prim, x + ((digits - 1) - i) * 12, y, 12, 12);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void VS__tickIntroName(int32_t id)
{
	int32_t len;

	++VS__INTRO_DATA_FRAME_COUNT;

	len = strlen(DIGIMON_NAME(ENTITY_TABLE[id]->type)) / 2;
	if (ENTITY_TABLE[id]->type == 0x4e || ENTITY_TABLE[id]->type == 0x3c) {
		len = 10;
	}

	if (len == VS__INTRO_DATA_RENDERED_CHARACTERS && VS__INTRO_DATA_ANIM_FRAME == 3) {
		if (VS_INTRO_CAMERA_CHASE.timer == 0) {
			startAnimation(ENTITY_TABLE[id], 0x23);
			VS_INTRO_CAMERA_CHASE.timer = 20;
		}

#if VERSION_IS(EU)
		if (VS__INTRO_DATA_POS_Y >= -103) {
#else
		if (VS__INTRO_DATA_POS_Y >= -71) {
#endif
			VS__INTRO_DATA_POS_Y -= 28;
		} else {
			VS__addIntroStats(ENTITY_TABLE[id], id);
		}
	}
}

// clang-format off
void VS__renderIntroName(id)
	int16_t id;
// clang-format on
{
	int32_t charCount;
	int32_t i;
	int32_t charIndex;
#if VERSION_IS(EU)
	uint16_t *glyph;
	uint16_t code;
#endif
	int16_t y;
	int16_t size;
	uint8_t character;
#if VERSION_IS(EU)
	int8_t secondRow;
#endif

	charCount = strlen(DIGIMON_NAME(ENTITY_TABLE[id]->type)) / 2;
	if (ENTITY_TABLE[id]->type == 0x4e || ENTITY_TABLE[id]->type == 0x3c) {
		charCount = 10;
	}

	if (VS__INTRO_DATA_FRAME_COUNT % 4 == 0) {
		if (VS__INTRO_DATA_RENDERED_CHARACTERS < charCount) {
			++VS__INTRO_DATA_RENDERED_CHARACTERS;
			VS__INTRO_DATA_ANIM_FRAME = 0;
		}
	} else if (VS__INTRO_DATA_ANIM_FRAME != 3) {
		++VS__INTRO_DATA_ANIM_FRAME;
	}

	charIndex = 0;
#if VERSION_IS(EU)
	secondRow = 0;
#endif
	for (i = 0; i < VS__INTRO_DATA_RENDERED_CHARACTERS; ++i) {
#if VERSION_IS(EU)
		glyph = &((uint16_t *)DIGIMON_NAME(ENTITY_TABLE[id]->type))[charIndex++];
		code = *glyph;
		code = (code << 8) | (code >> 8);
		if (code >= 0x8281) {
			character = code - 0x8281;
		} else {
			character = code - 0x8260;
		}

		if (charIndex == 11) {
			secondRow = 1;
#else
		character = VS__INTRO_DIGIMON_NAMES[ENTITY_TABLE[id]->type][charIndex++];
		if (character == 0x3d) {
			character = VS__INTRO_DIGIMON_NAMES[ENTITY_TABLE[id]->type][charIndex++];
#endif
		}

		if (i == VS__INTRO_DATA_RENDERED_CHARACTERS - 1) {
			y = VS__INTRO_DATA_POS_Y - VS__INTRO_NAME_CHAR_OFFSETS[VS__INTRO_DATA_ANIM_FRAME];
			size = VS__INTRO_NAME_CHAR_SIZES[VS__INTRO_DATA_ANIM_FRAME];
		} else {
			y = VS__INTRO_DATA_POS_Y;
			size = 32;
		}

#if VERSION_IS(EU)
		if (secondRow == 0) {
			VS__renderIntroNameChar((int16_t)VS__INTRO_DATA_POS_X + i * 32, y, size, character);
		} else {
			VS__renderIntroNameChar((int16_t)VS__INTRO_DATA_POS_X + i * 32 - 320, y + 32, size, character);
#else
		VS__renderIntroNameChar((int16_t)VS__INTRO_DATA_POS_X + i * 32, y, size, character);

		if (character == 0x1f || character == 0x25) {
			character = VS__INTRO_DIGIMON_NAMES[ENTITY_TABLE[id]->type][charIndex++];
			VS__renderIntroNameChar((int16_t)VS__INTRO_DATA_POS_X + i * 32, y, size, character);
#endif
		}
	}
}
