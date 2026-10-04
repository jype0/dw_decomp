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
#include <dw/model.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/vs.h>
#include <dw/world_object.h>

extern char MAIN_D_80124C24[];
extern char MAIN_D_80124C54[];
extern int16_t MAIN_D_80134F24;
extern int16_t MAIN_D_80134F26;
extern uint8_t MAIN_D_80134F28;
extern int16_t MAIN_D_80134F2A;
extern int16_t MAIN_D_80134F2C;
extern CameraChase MAIN_D_801352A4;
extern int32_t MAIN_D_80134F20;
extern int16_t MAIN_D_801B1C70[];
extern int16_t MAIN_D_801B1C72[];
extern int16_t MAIN_D_801B1C74[];
extern int16_t MAIN_D_801B1C76[];
extern int16_t MAIN_D_801B1C78[];
extern int16_t MAIN_D_801B1C7A[];
extern uint8_t MAIN_D_80135274;
extern uint8_t MAIN_D_80134F2E;

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
	VS__renderIntroName,
	VS__tickIntroName,
	VS__renderIntroStatNumber,
	VS__renderIntroStats,
	VS__tickIntroStats,
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
uint8_t MAIN_D_801344F8[4] = {
	0x40, 0x2c, 0x26, 0x20,
};

uint8_t MAIN_D_801344FC[4] = {
	0x10, 0x07, 0x03, 0x00,
};

char MAIN_D_80134500[] = "ＨＰ";

char MAIN_D_80134508[] = "ＭＰ";

int16_t MAIN_D_8012F42C[6] = {
	0x270f, 0x270f, 0x03e7, 0x03e7, 0x03e7, 0x03e7,
};

RGB8 MAIN_D_8012F438[10] = {
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

void VS__placePlayer1(int32_t stage)
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

void VS__placePlayer2(int32_t stage)
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
	int32_t y;
	char *text;

	clearTextArea();
	drawString(MAIN_D_80134500, 0, 0);
	drawString(MAIN_D_80134508, 0, 12);

	for (i = 2, y = 24, text = MAIN_D_80124C24;
	     i < 6;
	     ++i, text += 12, y += 12) {
		drawString(text, 0, y);
		DrawSync(0);
	}

	drawString(MAIN_D_80124C54, 0, 0xf0);
}

void VS__addIntroText(Entity *entity, int32_t id)
{
	int32_t len;

	MAIN_D_80134F24 = 4;
	MAIN_D_80134F26 = 0;

	switch (DIGIMON_DATA[entity->type].special[0]) {
	case 0:
		MAIN_D_80134F28 = 3;
		break;
	case 1:
		MAIN_D_80134F28 = 1;
		break;
	case 2:
		MAIN_D_80134F28 = 6;
		break;
	case 3:
		MAIN_D_80134F28 = 2;
		break;
	case 4:
		MAIN_D_80134F28 = 4;
		break;
	case 5:
		MAIN_D_80134F28 = 0;
		break;
	case 6:
		MAIN_D_80134F28 = 5;
		break;
	default:
		MAIN_D_80134F28 = 0;
		break;
	}

	len = strlen(DIGIMON_DATA[entity->type].name) / 2;
	if (entity->type == 0x4e || entity->type == 0x3c) {
		len = 10;
	}

	MAIN_D_80134F2A = -(len * 16);
	MAIN_D_80134F2C = 68;
	addObject(0x1ab, id, VS__tickIntroName, VS__renderIntroName);
}

void VS__setPostIntroPosition(Entity *entity)
{
	if (MAIN_D_801352A4.timer != -1) {
		entity->posData->location = VS_D_80071744;
		entity->anim.locX = VS_D_80071744.vx << 15;
		entity->anim.locY = VS_D_80071744.vy << 15;
		entity->anim.locZ = VS_D_80071744.vz << 15;
		startAnimation(entity, 0x21);
		MAIN_D_801352A4.timer = -1;
	}
}

void VS__removeIntroText(int32_t id)
{
	removeObject(0x1ab, id);
}

void VS__removeIntroStats(int32_t id)
{
	if (MAIN_D_80134F20 != 0) {
		MAIN_D_80134F20 = 0;
		removeObject(0x1a9, id);
	}
}

void VS__addIntroStats(Entity *entity, int32_t id)
{
	if (MAIN_D_80134F20 != 1) {
		MAIN_D_80134F20 = 1;
		MAIN_D_801B1C70[0] = -100;
		MAIN_D_801B1C72[0] = -100;
		MAIN_D_801B1C74[0] = -10;
		MAIN_D_801B1C76[0] = -10;
		MAIN_D_801B1C78[0] = -10;
		MAIN_D_801B1C7A[0] = -10;
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
	       value * 100 / MAIN_D_8012F42C[stat] - 50, stat * 16 - 26,
	       -50, stat * 16 - 18,
	       value * 100 / MAIN_D_8012F42C[stat] - 50, stat * 16 - 18);

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
	setRGB0(prim, MAIN_D_8012F438[MAIN_D_80134F28].r, MAIN_D_8012F438[MAIN_D_80134F28].g, MAIN_D_8012F438[MAIN_D_80134F28].b);
	setRGB1(prim, MAIN_D_8012F438[MAIN_D_80134F28].r, MAIN_D_8012F438[MAIN_D_80134F28].g, MAIN_D_8012F438[MAIN_D_80134F28].b);
	setRGB2(prim, MAIN_D_8012F438[MAIN_D_80134F28].r / 10, MAIN_D_8012F438[MAIN_D_80134F28].g / 10, MAIN_D_8012F438[MAIN_D_80134F28].b / 10);
	setRGB3(prim, MAIN_D_8012F438[MAIN_D_80134F28].r / 10, MAIN_D_8012F438[MAIN_D_80134F28].g / 10, MAIN_D_8012F438[MAIN_D_80134F28].b / 10);

	u = (character % 32) * 32;
	v = (character / 8) * 32;

	if (size < 64) {
		setUVWH(prim, u, v, (u != 0xe0 ? 32 : 31), (v != 0xe0 ? 32 : 31));
	} else {
		setUVWH(prim, u, v, 31, 31);
	}

	setXYWH(prim, x, y, size, size);

	GsSetWorkBase((PACKET *)prim);
}

void VS__runIntro(int32_t stage)
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
	playMusic(MAIN_D_80135274, 0);

	i = 0;
	fadeFromBlack(5);

	for (; i < 6; ++i) {
		VS_tickFrame();
	}

	while (ENTITY_TABLE[1]->anim.animFlag & 1) {
		pad = PadRead(1);
		VS_tickFrame();

		if ((pad & ~prev) & 0x40) {
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
	playMusic(MAIN_D_80135274, 1);

	while (ENTITY_TABLE[2]->anim.animFlag & 1) {
		pad = PadRead(1);
		pad = (pad >> 16) & 0xffff;
		prev = (prev >> 16) & 0xffff;

		VS_tickFrame();

		if ((pad & ~prev) & 0x40) {
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

void VS__tickIntroStats(int32_t id)
{
	Stats *stats;

	MAIN_D_801B1C70[0] += 200;
	MAIN_D_801B1C72[0] += 200;
	MAIN_D_801B1C74[0] += 20;
	MAIN_D_801B1C76[0] += 20;
	MAIN_D_801B1C78[0] += 20;
	MAIN_D_801B1C7A[0] += 20;

	stats = &((DigimonEntity *)ENTITY_TABLE[id])->stats;
	if (stats->current.currentHP < MAIN_D_801B1C70[0]) {
		MAIN_D_801B1C70[0] = stats->current.currentHP;
	}

	if (stats->current.currentMP < MAIN_D_801B1C72[0]) {
		MAIN_D_801B1C72[0] = stats->current.currentMP;
	}

	if (stats->base.off < MAIN_D_801B1C74[0]) {
		MAIN_D_801B1C74[0] = stats->base.off;
	}

	if (stats->base.def < MAIN_D_801B1C76[0]) {
		MAIN_D_801B1C76[0] = stats->base.def;
	}

	if (stats->base.speed < MAIN_D_801B1C78[0]) {
		MAIN_D_801B1C78[0] = stats->base.speed;
	}

	if (stats->base.brain < MAIN_D_801B1C7A[0]) {
		MAIN_D_801B1C7A[0] = stats->base.brain;
	}
}

void VS__renderIntroStats(int32_t id)
{
	Stats *stats;
	int32_t i;

	for (i = 0; i < 6; ++i) {
		renderString(0, -100, i * 16 - 28, 48, 12, 0, i * 12, 0, 1);
		VS__renderIntroStatBar((int16_t)i, MAIN_D_801B1C70[i]);
	}

	stats = &((DigimonEntity *)ENTITY_TABLE[id])->stats;
	if (MAIN_D_801B1C70[0] != stats->current.currentHP ||
	    MAIN_D_801B1C72[0] != stats->current.currentMP ||
	    MAIN_D_801B1C74[0] != stats->base.off ||
	    MAIN_D_801B1C76[0] != stats->base.def ||
	    MAIN_D_801B1C78[0] != stats->base.speed ||
	    MAIN_D_801B1C7A[0] != stats->base.brain) {
		playSound(0, 0x16);
	} else {
		for (i = 0; i < 6; ++i) {
			VS__renderIntroStatNumber(52, (int16_t)(i * 16 - 28), 4,
			                          MAIN_D_801B1C70[i], 3);
		}
	}
}

void VS__renderIntroStatNumber(int32_t x, int32_t y, int32_t digits, int32_t value,
                               int32_t layer)
{
	POLY_FT4 *prim;
	int32_t i;
	uint32_t width;
	int32_t count;
	int32_t buf[6];

	prim = (POLY_FT4 *)GsGetWorkBase();

	width = digits;
	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 832, 0);
		setClut(prim, 16, 480);
		setUVDataPolyFT4(prim, buf[i] * 12, 32, 12, 12);
		setPosDataPolyFT4(prim,
		                  x + (((int32_t)width - 1) - i) * 12, y,
		                  12, 12);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void VS__tickIntroName(int32_t id)
{
	int32_t len;

	++MAIN_D_80134F24;

	len = strlen(DIGIMON_DATA[ENTITY_TABLE[id]->type].name) / 2;
	if (ENTITY_TABLE[id]->type == 0x4e || ENTITY_TABLE[id]->type == 0x3c) {
		len = 10;
	}

	if (len == MAIN_D_80134F26 && MAIN_D_80134F2E == 3) {
		if (MAIN_D_801352A4.timer == 0) {
			startAnimation(ENTITY_TABLE[id], 0x23);
			MAIN_D_801352A4.timer = 20;
		}

		if (MAIN_D_80134F2C >= -71) {
			MAIN_D_80134F2C -= 28;
		} else {
			VS__addIntroStats(ENTITY_TABLE[id], id);
		}
	}
}

void VS__renderIntroName(int32_t id)
{
	int32_t charCount;
	uint32_t entityIndex;
	int32_t charIndex;
	int32_t i;
	int16_t y;
	int16_t size;
	uint8_t character;

	entityIndex = id;
	charCount = strlen(DIGIMON_DATA[ENTITY_TABLE[id]->type].name) / 2;
	if (ENTITY_TABLE[id]->type == 0x4e || ENTITY_TABLE[id]->type == 0x3c) {
		charCount = 10;
	}

	if (MAIN_D_80134F24 % 4 == 0) {
		if (MAIN_D_80134F26 < charCount) {
			++MAIN_D_80134F26;
			MAIN_D_80134F2E = 0;
		}
	} else if (MAIN_D_80134F2E != 3) {
		++MAIN_D_80134F2E;
	}

	charIndex = 0;
	for (i = 0; i < MAIN_D_80134F26; ++i) {
		character = VS_D_8006FF20[ENTITY_TABLE[entityIndex]->type][charIndex++];
		if (character == 0x3d) {
			character = VS_D_8006FF20[ENTITY_TABLE[entityIndex]->type][charIndex++];
		}

		if (i == MAIN_D_80134F26 - 1) {
			y = MAIN_D_80134F2C - MAIN_D_801344FC[MAIN_D_80134F2E];
			size = MAIN_D_801344F8[MAIN_D_80134F2E];
		} else {
			size = 32;
			y = MAIN_D_80134F2C;
		}

		VS__renderIntroNameChar((int16_t)(MAIN_D_80134F2A + i * 32), y,
		                        size, character);

		if (character == 0x1f || character == 0x25) {
			character = VS_D_8006FF20[ENTITY_TABLE[entityIndex]->type][charIndex++];
			VS__renderIntroNameChar((int16_t)(MAIN_D_80134F2A + i * 32),
			                        y, size, character);
		}
	}
}
