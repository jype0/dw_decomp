#include <libetc.h>

#include <dw/btl.h>
#include <dw/clock.h>
#include <dw/fade.h>
#include <dw/file.h>
#include <dw/file_queue.h>
#include <dw/font.h>
#include <dw/line.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/mov.h>
#include <dw/params.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/std.h>
#include <dw/tournament.h>
#include <dw/ui.h>
#include <dw/utils.h>

#define shop_START		((char *)0x80080800)

typedef struct {
	uint8_t mapId;
	uint8_t mode;
	int16_t startId;
	int16_t count;
	uint16_t trigger;
} MapLightUpdateData;

void reinitializeAfterTournament(void);
void thunkReinitializeAfterTournament(void);
void startTournament(void);
void *allocateArray(uint32_t size);
void freeArray(uint32_t *array);
void setMapObjectsFlag(int16_t start, int16_t count, int32_t flag);
void updateMapLightState(void);
void setMapLayerEnabled(uint8_t enabled);
int32_t isInvisible(Entity* entity);
int32_t isTamerOnScreen(void);
void startMovie(int32_t movieId);
void playMovie(int16_t movieId, int32_t shouldPlay);
void initializeFramebuffer(void);
void initStringFT4(POLY_FT4* poly);
void convertValueToDigits(int16_t n, int32_t value, int32_t *outCount,
			  int32_t *digits);
void renderSelectionCursor(int32_t x, int32_t y, int16_t w, int16_t h,
			   int32_t layer);
void loadStackedTIMFile(char *path);
void renderString(uint8_t color, int16_t x, int16_t y, int16_t w, int16_t h,
		  uint8_t u, uint8_t v, int32_t layer, int32_t shadow);
void pauseFrame(void);
void renderItemSprite(uint8_t type, int16_t x, int16_t y, int32_t layer);
void setItemTexture(POLY_FT4 *p, uint8_t id);
uint8_t entityGetTechFromAnim(Entity *e, uint8_t anim);
void entityLookAtTile(Entity *entity, int8_t tileX, int8_t tileY);
void setEntityTextDigit(POLY_FT4* poly, int32_t x, int32_t y);
void removePauseBox(void);
void renderItemAmount(int32_t color, int16_t n, int16_t x, int16_t y,
		      int16_t value, int32_t layer);
void drawEntityText(int32_t color, int16_t n, int16_t x, int16_t y,
			int16_t value, int32_t layer);
int32_t hasMove(int32_t move);
void learnMove(int32_t move);
void unlearnMove(int32_t move);
void createPauseBox(void);
void handlePause(void);
void renderPauseBox(int32_t instanceId);
void setPosDataPolyFT4(POLY_FT4 *prim, int16_t posX, int16_t posY, int16_t width, int16_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int16_t xPos, int16_t yPos, int16_t width, int16_t height);
void drawEntityTextIcon(int16_t x, int16_t y, uint8_t u, int32_t otOffset);
int32_t STD_func_800579D8(uint8_t *arg);

extern MapLightUpdateData MAP_LIGHT_UPDATE_DATA[];
extern uint8_t ITEM_CLUT_DATA[];
extern uint8_t MAP_LAYER_ENABLED;
extern RGB8 TEXT_COLORS[];
extern uint32_t PAUSE_INPUT;
extern uint32_t PAUSE_INPUT_PREVIOUS;
extern uint8_t PAUSE_BOX_VISIBLE;
extern uint8_t PAUSE_STATE;
extern int32_t MAIN_D_80134E7C;
extern char btl_START[];
extern char dget_START[];
extern char doo2_START[];
extern char dooa_START[];
extern char eab_START[];
extern char endi_START[];
extern char evl_START[];
extern char fish_START[];
extern char kar_START[];
extern char mov_START[];
extern char murd_START[];
extern char std_START[];
extern char trn_START[];
extern char trn2_START[];
extern char vs_START[];

// clang-format off
void *REL_BIN_OFFSETS[16] = {
	btl_START,
	std_START,
	fish_START,
	evl_START,
	kar_START,
	vs_START,
	mov_START,
	doo2_START,
	dooa_START,
	trn_START,
	shop_START,
	dget_START,
	trn2_START,
	murd_START,
	endi_START,
	eab_START,
};

char MAIN_D_8012B9AC[12] = "BTL_REL.BIN";

char MAIN_D_8012B9B8[12] = "STD_REL.BIN";

char MAIN_D_8012B9C4[] = "FISH_REL.BIN";

char MAIN_D_8012B9D4[12] = "EVL_REL.BIN";

char MAIN_D_8012B9E0[12] = "KAR_REL.BIN";

char MAIN_D_8012B9EC[] = "VS_REL.BIN";

char MAIN_D_8012B9F8[12] = "MOV_REL.BIN";

char MAIN_D_8012BA04[] = "DOO2_REL.BIN";

char MAIN_D_8012BA14[] = "DOOA_REL.BIN";

char MAIN_D_8012BA24[12] = "TRN_REL.BIN";

char MAIN_D_8012BA30[] = "SHOP_REL.BIN";

char MAIN_D_8012BA40[] = "DGET_REL.BIN";

char MAIN_D_8012BA50[] = "TRN2_REL.BIN";

char MAIN_D_8012BA60[] = "MURD_REL.BIN";

char MAIN_D_8012BA70[] = "ENDI_REL.BIN";

char MAIN_D_8012BA80[12] = "EAB_REL.BIN";

char *REL_BIN_FILES[16] = {
	MAIN_D_8012B9AC,
	MAIN_D_8012B9B8,
	MAIN_D_8012B9C4,
	MAIN_D_8012B9D4,
	MAIN_D_8012B9E0,
	MAIN_D_8012B9EC,
	MAIN_D_8012B9F8,
	MAIN_D_8012BA04,
	MAIN_D_8012BA14,
	MAIN_D_8012BA24,
	MAIN_D_8012BA30,
	MAIN_D_8012BA40,
	MAIN_D_8012BA50,
	MAIN_D_8012BA60,
	MAIN_D_8012BA70,
	MAIN_D_8012BA80,
};

#if defined(VERSION_JP)
char MAIN_D_80134430[] = "ポーズ";
#else
char MAIN_D_80134430[] = "Pause";
#endif
// clang-format on

void drawEntityText(int32_t color, int16_t n, int16_t x, int16_t y,
			int16_t value, int32_t layer)
{
	POLY_FT4 *prim;
	int32_t i;
	int32_t count;
	int32_t buf[4];

	prim = (POLY_FT4 *)GsGetWorkBase();

	convertValueToDigits(n, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		setEntityTextDigit(prim, 256, 484);
		setRGB0(prim, TEXT_COLORS[color].r, TEXT_COLORS[color].g,
			TEXT_COLORS[color].b);
		setUVDataPolyFT4(prim, buf[i] * 8, 0xb7, 8, 8);
		setPosDataPolyFT4(prim, x + (((n - 1) - i) * 7), y, 8, 8);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void renderItemAmount(int32_t color, int16_t n, int16_t x, int16_t y,
		      int16_t value, int32_t layer)
{
	POLY_FT4 *prim;
	int32_t i;
	int32_t count;
	int32_t buf[4];

	prim = (POLY_FT4 *)GsGetWorkBase();

	convertValueToDigits(n, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		setEntityTextDigit(prim, 256, 484);
		setRGB0(prim, TEXT_COLORS[color].r, TEXT_COLORS[color].g,
			TEXT_COLORS[color].b);
		setUVDataPolyFT4(prim, buf[i] * 6 + 0x89, 0x9c, 6, 10);
		setPosDataPolyFT4(prim, x + (((n - 1) - i) * 6), y, 6,
				  10);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void renderSelectionCursor(int32_t x, int32_t y, int16_t w, int16_t h,
			   int32_t layer)
{
	GsBOXF box;

	drawLine3P(0xb0b0b0, x, y + h - 1, x, y, x + w - 1, y, layer,
		   0);
	drawLine3P(0x121212, x + w, y, x + w, y + h, x, y + h, layer,
		   0);
	box.attribute = 0x40000000;
	box.r = box.g = box.b = 0x80;
	setRECT(&box, x + 1, y + 1, w - 1, h - 1);
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, (uint16_t)layer);
}

void renderString(uint8_t color, int16_t x, int16_t y, int16_t w, int16_t h,
		  uint8_t u, uint8_t v, int32_t layer, int32_t shadow)
{
	POLY_FT4 *prim;
#if !defined(VERSION_JP)
	GsOT *ot;
#endif

	prim = (POLY_FT4 *)GsGetWorkBase();
	initStringFT4(prim);
	setRGB0(prim, TEXT_COLORS[color].r, TEXT_COLORS[color].g,
		TEXT_COLORS[color].b);
	setUVDataPolyFT4(prim, u, v, w, h);
	setPosDataPolyFT4(prim, x, y, w, h);
#if defined(VERSION_JP)
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
#else
	AddPrim((ot = ACTIVE_ORDERING_TABLE)->org + layer, prim++);
#endif
	if (shadow != 0) {
		initStringFT4(prim);
		setRGB0(prim, 0, 0, 0);
		setUVDataPolyFT4(prim, u, v, w, h);
		setPosDataPolyFT4(prim, x + 1, y + 1, w, h);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}
	GsSetWorkBase((PACKET *)prim);
}

void renderItemSprite(uint8_t type, int16_t x, int16_t y, int32_t layer)
{
	POLY_FT4 *prim;
	int16_t width;
	int16_t height;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->tpage = getTPage(0, 0, 320, 0);
	setItemTexture(prim, type);

	width = prim->u1 - prim->u0;
	height = prim->v2 - prim->v0;
	setRGB0(prim, 0x80, 0x80, 0x80);
	setPosDataPolyFT4(prim, x, y, width, height);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void setItemTexture(POLY_FT4 *prim, uint8_t type)
{
	uint8_t u;
	uint8_t v;
	uint8_t w;
	uint8_t h;

	u = (type % 16) * 16;
	v = (type / 16) * 16;
	if (u == 0xf0) {
		w = 15;
	} else {
		w = 16;
	}
	if (v == 0xf0) {
		h = 15;
	} else {
		h = 16;
	}
	setUVDataPolyFT4(prim, u, v, w, h);
	setClut(prim, 0xe0, ITEM_CLUT_DATA[type] + 0x1e8);
}

int32_t hasMove(int32_t move)
{
	if (PARTNER_ENTITY.learnedMoves[move / 32] & (1 << (move % 32))) {
		return 1;
	}

	return 0;
}

void learnMove(int32_t move)
{
	if (move == 0x2C || move == 0x30) {
		PARTNER_ENTITY.learnedMoves[1] |= 0x1000;
		PARTNER_ENTITY.learnedMoves[1] |= 0x10000;
	} else if (move == 0x37 || move == 0x39) {
		PARTNER_ENTITY.learnedMoves[1] |= 0x800000;
		PARTNER_ENTITY.learnedMoves[1] |= 0x2000000;
	} else {
		PARTNER_ENTITY.learnedMoves[move / 32] |= 1 << (move % 32);
	}
}

uint8_t entityGetTechFromAnim(Entity *e, uint8_t anim)
{
	if (anim == 0xff) {
		return 0xff;
	}
	if ((e->type == 0x3cL) && (anim == 0x3c)) {
		return 0x70;
	}
	return DIGIMON_DATA[e->type].moves[anim - 0x2e];
}

void entityLookAtTile(Entity *entity, int8_t tileX, int8_t tileY)
{
	VECTOR loc;

	loc.vx = (tileX - 50) * 100 + 50;
	loc.vy = 0;
	loc.vz = (50 - tileY) * 100 - 50;
	entityLookAtLocation(entity, &loc);
}

void drawEntityTextIcon(int16_t x, int16_t y, uint8_t u, int32_t otOffset)
{
	POLY_FT4 *prim;

	prim = (POLY_FT4 *)GsGetWorkBase();
	setEntityTextDigit(prim, 0x100, 0x1E6);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setUVDataPolyFT4(prim, u, 0xB8, 8, 8);
	setPosDataPolyFT4(prim, x, y, 8, 8);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[otOffset], prim++);
	GsSetWorkBase((PACKET *)prim);
}

int32_t isInvisible(Entity* entity)
{
	if (entity == NULL || entity->isOnScreen == 0 || entity->isOnMap == 0) {
		return 1;
	}

	return 0;
}

void loadDynamicLibrary(Overlay lib, uint8_t *isComplete, uint8_t isAsync,
			FileCallback callback, void *param)
{
	uint8_t *nv;

	if (!isAsync) {
		readFile(REL_BIN_FILES[lib - 1], (nv = REL_BIN_OFFSETS[lib - 1]));
	} else {
		addFileReadRequestPath(REL_BIN_FILES[lib - 1],
				       (nv = REL_BIN_OFFSETS[lib - 1]), isComplete,
				       (FileRequestCallback)callback, param);
	}
}

void startMovie(int32_t movieId)
{
	uint8_t isComplete;

	CdInit();
	ResetGraph(0);
	SetGraphDebug(0);
	loadDynamicLibrary(MOV_REL, &isComplete, 0, NULL, NULL);
	MOV_playMovie(movieId);
	ResetGraph(3);
}

void handlePause(void)
{
	if (PAUSE_BOX_VISIBLE != 0) {
		removePauseBox();
		PAUSE_BOX_VISIBLE = 0;
	}
	if ((readPStat(0) == 3 || IS_GAMETIME_RUNNING == 0) && GAME_STATE == 0) {
		return;
	}
	if (FADE_PROTECTION == 1) {
		return;
	}
	PAUSE_INPUT = PadRead(1);
	if ((PAUSE_INPUT & 0x800) && !(PAUSE_INPUT_PREVIOUS & 0x800)) {
		PAUSE_STATE = (PAUSE_STATE + 1) & 1;
	}
	PAUSE_INPUT_PREVIOUS = PAUSE_INPUT;
	if (PAUSE_STATE != 0) {
		createPauseBox();
		pauseFrame();
		pauseFrame();
		PAUSE_BOX_VISIBLE++;
	}
	while (PAUSE_STATE != 0) {
		PAUSE_INPUT = PadRead(1);
		if ((PAUSE_INPUT & 0x800) && !(PAUSE_INPUT_PREVIOUS & 0x800)) {
			PAUSE_STATE = (PAUSE_STATE + 1) & 1;
		}
		PAUSE_INPUT_PREVIOUS = PAUSE_INPUT;
	}
}

void removePauseBox(void)
{
	if (MAIN_D_80134E7C != 0) {
		removeStaticUIBox(5);
		MAIN_D_80134E7C = 0;
		playSound(0, 3);
	}
}

void createPauseBox(void)
{
	RECT pos;

	if (MAIN_D_80134E7C != 1) {
		drawString(MAIN_D_80134430, 0x78, 0xF0);
#if defined(VERSION_JP)
		setRECT(&pos, -0x1A, -0xE, 0x30, 0x18);
#else
		setRECT(&pos, -0x1A, -0xE, 0x38, 0x18);
#endif
		createStaticUIBox(5, 1, 0, &pos, NULL, renderPauseBox);
		MAIN_D_80134E7C = 1;
		playSound(0, 3);
	}
}

void renderPauseBox(instanceId)
	int16_t instanceId;
{
	GsBOXF box;
	RECT *pos;

	box.attribute = 0x40000000;
	box.r = box.g = box.b = 0;
	setRECT(&box, -0xA0, -0x78, 0x140, 0xF0);
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 7 - instanceId);
	pos = &UI_BOX_DATA[5].finalPos;
#if defined(VERSION_JP)
	renderString(0, pos->x + 6, pos->y + 6, 0x24, 0xC, 0x78, 0xF0, 0, 1);
#else
	renderString(0, pos->x + 6, pos->y + 6, 0x2A, 0xC, 0x78, 0xF0, 0, 1);
#endif
}

void thunkReinitializeAfterTournament(void)
{
	reinitializeAfterTournament();
}

void setMapLayerEnabled(uint8_t enabled)
{
	MAP_LAYER_ENABLED = enabled;
}

void loadStackedTIMFile(char *path)
{
	GsIMAGE img;
	uint8_t *p;

	p = GENERAL_BUFFER;
	readFile(path, p);
	while (*(int32_t *)p == 0x10) {
		p = (uint8_t *)(p + 4);
		GsGetTimInfo((u_long *)p, &img);
		p += ((img.pw * img.ph) / 2 + 4) * 4;
		LoadImage((RECT *)&img.px, img.pixel);
		if ((img.pmode >> 3) & 1) {
			LoadImage((RECT *)&img.cx, img.clut);
			p += ((img.cw * img.ch) / 2 + 3) * 4;
		}
	}
}

void playMovie(int16_t movieId, int32_t shouldPlay)
{
	if (shouldPlay) {
		startMovie(movieId);
		initializeFramebuffer();
	}
}

void unlearnMove(int32_t move)
{
	int32_t i = 0;
	uint16_t mask;

	for (i = 0; i < 4; i++) {
		if (move == entityGetTechFromAnim(ENTITY_TABLE[1],
						  PARTNER_ENTITY.digimonEntity.stats.base.moves[i])) {
			PARTNER_ENTITY.digimonEntity.stats.base.moves[i] = 0xff;
			break;
		}
	}
	if (move == 0x2c || move == 0x30) {
		mask = 0x1000;
	} else if (move == 0x37 || move == 0x39) {
		mask = 0;
	} else {
		mask = 1 << (move % 32);
	}
	mask = mask ^ 0xffff;
	PARTNER_ENTITY.learnedMoves[move / 32] &= mask;
}

#if !defined(VERSION_JP)
int32_t isTamerOnScreen(void)
{
	if (ENTITY_TABLE[0]->isOnScreen == 1) {
		return 1;
	}

	return 0;
}
#endif

void updateMapLightState(void)
{
	int32_t i;
	MapLightUpdateData *p;

	p = MAP_LIGHT_UPDATE_DATA;
	for (i = 0; i <= 0; i++, p++) {
		if ((p->mapId == (CURRENT_SCREEN_ID & 0xFF)) &&
		    ((p->trigger == 0xFFFF) || (isTriggerSet(p->trigger) != 0))) {
			if ((HOUR >= 7) && (HOUR < 0x13)) {
				if (p->mode == 0) {
					setMapObjectsFlag(p->startId, p->count, 0);
				} else {
					setMapObjectsFlag(p->startId, p->count, 1);
				}
			} else if (p->mode == 1) {
				setMapObjectsFlag(p->startId, p->count, 0);
			} else {
				setMapObjectsFlag(p->startId, p->count, 1);
			}
		}
	}
}

void startTournament(void)
{
	uint8_t *p;
	uint8_t *pool;
	uint8_t *jumpTable;
	uint8_t *w;
	struct {
		uint8_t cup;
		uint8_t opponents[7];
	} t;
	int16_t result;
	int16_t expected;
	uint8_t id;
	uint8_t isComplete;
	uint8_t i;
	int32_t count;

	id = readPStat(3);
	t.cup = id;
	pool = (uint8_t *)allocateArray(0x70);
	w = pool;
	jumpTable = getCupDataJumpTable(10, id);
	p = getCupDataJumpTableEntry(jumpTable, 4) + 2;
	count = 0;
	i = *p;
	if (i < 0xfe) {
		if (id != 0x16) {
			while ((i = *p++) < 0xfe) {
				if (isTriggerSet(i + 200) != 0) {
					*w++ = i;
					count++;
				}
			}
		} else {
			while ((i = *p++) < 0xfe) {
				*w++ = i;
				count++;
			}
		}
	} else {
		for (i = 0; i < 0x70; i++) {
			if (isTriggerSet(i + 200) != 0) {
				*w++ = i;
				count++;
			}
		}
	}
	p = t.opponents;
	if (count == 7) {
		for (i = 0; i < 7; i++) {
			*p++ = pool[i];
		}
	} else if (count < 7) {
		for (i = 0; i < count; i++) {
			*p++ = pool[i];
		}
		for (i = count; i < 7; i++) {
			*p++ = pool[randomLimit(count)];
		}
	} else {
		id = PARTNER_ENTITY.digimonEntity.entity.type;
		for (i = 0; i < count; i++) {
			if (id == pool[i]) {
				pool[i] = 0xff;
				count--;
				break;
			}
		}
		for (i = 0; i < 7; i++) {
			id = (uint8_t)randomLimit(count) + 1;
			w = pool;
			while (id != 0) {
				if (*w++ != 0xff) {
					id--;
				}
			}
			w--;
			*p++ = *w;
			*w = 0xff;
			count--;
		}
	}
	freeArray((uint32_t *)pool);
	stopBGM();
	loadDynamicLibrary(STD_REL, &isComplete, 0, NULL, NULL);
	result = STD_func_800579D8(&t.cup);
	thunkReinitializeAfterTournament();
	unsetTrigger(0x25);
	id = readPStat(3);
	if (id != 5) {
		expected = 3;
	} else {
		expected = 2;
	}
	if (result == expected) {
		TOURNAMENTS_WON++;
		TOURNAMENTS_WON = enforceStatsLimits(0x11, TOURNAMENTS_WON);
		setTrigger(id + 15);
	} else {
		TOURNAMENTS_LOST++;
	}
	TOURNAMENT_WINS += result;
	TOURNAMENT_WINS = enforceStatsLimits(0x12, TOURNAMENT_WINS);
	TOURNAMENTS_LOST = enforceStatsLimits(0x13, TOURNAMENTS_LOST);
	writePStat(0xff, result);
}
