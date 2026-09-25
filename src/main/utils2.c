#include <libetc.h>

#include <dw/btl.h>
#include <dw/clock.h>
#include <dw/fade.h>
#include <dw/file.h>
#include <dw/file_queue.h>
#include <dw/font.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/mov.h>
#include <dw/params.h>
#include <dw/pstat.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/std.h>
#include <dw/tournament.h>
#include <dw/trigger.h>
#include <dw/ui.h>
#include <dw/utils.h>

typedef struct {
	uint8_t mapId;
	uint8_t mode;
	int16_t startId;
	int16_t count;
	uint16_t trigger;
} MapLightUpdateData;

void MAIN_func_800D92EC(void);
void MAIN_thunk_func_800D92EC(void);
void startTournament(void);
void *allocateArray(uint32_t size);
void freeArray(uint32_t *array);
void setMapObjectsFlag(int16_t start, int16_t count, int32_t flag);
void updateMapLightState(void);
void setMapLayerEnabled(uint8_t enabled);
int32_t isInvisible(Entity* entity);
int32_t isTamerOnScreen(void);
void startMovie(int32_t movieId);
void playMovie(int32_t movieId, dw_bool shouldPlay);
void initializeFramebuffer(void);
void initStringFT4(POLY_FT4* poly);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
			  int32_t *digits);
void renderSelectionCursor(int32_t x, int32_t y, int16_t w, int16_t h,
			   int32_t layer);
void renderTrianglePrimitive();
void loadStackedTIMFile(char *path);
void renderString(int32_t color, int16_t x, int16_t y, int16_t w, int16_t h,
		  uint8_t u, uint8_t v, int32_t layer, int32_t shadow);
void pauseFrame(void);
void renderItemSprite(uint8_t type, int16_t x, int16_t y, int32_t layer);
void setItemTexture(POLY_FT4 *p, uint8_t id);
uint8_t entityGetTechFromAnim(Entity *e, uint8_t anim);
void entityLookAtTile(Entity *entity, int32_t tileX, int32_t tileY);
void setEntityTextDigit(POLY_FT4* poly, int32_t x, int32_t y);
void MAIN_func_800E642C(void);
void renderSmallNumber(int32_t color, int32_t n, int32_t x, int16_t y,
		       int16_t value, int32_t layer);
void renderEntityTextNumber(int32_t color, int32_t n, int32_t x, int16_t y,
			int16_t value, int32_t layer);
int32_t hasMove(int32_t move);
void learnMove(int32_t move);
void unlearnMove(int32_t move);
void createPauseBox(void);
void handlePause(void);
void renderPauseBox(int32_t instanceId);
void setPosDataPolyFT4(POLY_FT4 *prim, int16_t posX, int16_t posY, int16_t width, int16_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int16_t xPos, int16_t yPos, int16_t width, int16_t height);
void renderSmallIcon(int16_t x, int16_t y, int16_t u, int32_t otOffset);

extern MapLightUpdateData MAP_LIGHT_UPDATE_DATA[];
extern char *OVERLAY_FILE_NAMES[];
extern void *OVERLAY_LOAD_ADDRESSES[];
extern uint8_t MAIN_D_80127BDC[];
extern uint8_t MAP_LAYER_ENABLED;
extern RGB8 TEXT_COLORS[];
extern uint32_t MAIN_D_80134E70;
extern uint32_t MAIN_D_80134E74;
extern uint8_t MAIN_D_80134E78[2];
extern int32_t MAIN_D_80134E7C;
extern char TEXT_PAUSE;

void renderEntityTextNumber(int32_t color, int32_t n, int32_t x, int16_t y,
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

void renderSmallNumber(int32_t color, int32_t n, int32_t x, int16_t y,
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
	uint32_t right;
	GsBOXF box;

	renderTrianglePrimitive(0xb0b0b0, x, y + h - 1, x, y, x + w - 1, y, layer,
				0);
	right = x + w;
	renderTrianglePrimitive(0x121212, right, y, right, y + h, x, y + h, layer,
				0);
	box.attribute = 0x40000000;
	box.r = box.g = box.b = 0x80;
	setRECT(&box, x + 1, y + 1, w - 1, h - 1);
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, (uint16_t)layer);
}

void renderString(int32_t color, int16_t x, int16_t y, int16_t w, int16_t h,
		  uint8_t u, uint8_t v, int32_t layer, int32_t shadow)
{
	POLY_FT4 *prim;
	GsOT *ot;

	prim = (POLY_FT4 *)GsGetWorkBase();
	initStringFT4(prim);
	setRGB0(prim, TEXT_COLORS[color].r, TEXT_COLORS[color].g,
		TEXT_COLORS[color].b);
	setUVDataPolyFT4(prim, u, v, w, h);
	setPosDataPolyFT4(prim, x, y, w, h);
	AddPrim((ot = ACTIVE_ORDERING_TABLE)->org + layer, prim++);
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
	setClut(prim, 0xe0, MAIN_D_80127BDC[type] + 0x1e8);
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
	uint8_t new_var4;
	int32_t *new_var;
	int new_var3;
	int new_var2;
	if (anim == 0xFF)
	{
		return 0xFF;
	}
	if ((e->type == 0x3C) && (anim == 0x3C))
	{
		return 0x70;
	}
	new_var = (int32_t *) e;
	new_var2 = 0x23;
	e = e;
	new_var3 = 0x2E;
	new_var4 = ((uint8_t *) (((char *) DIGIMON_DATA) + new_var2))[((new_var[0] * 0x34) & 0xFFFFFFFFFFFFFFFF) + (anim - new_var3)];
	return new_var4;
}

void entityLookAtTile(Entity *entity, int32_t tileX, int32_t tileY)
{
	VECTOR loc;

	loc.vx = (tileX - 50) * 100 + 50;
	loc.vy = 0;
	loc.vz = (50 - tileY) * 100 - 50;
	entityLookAtLocation(entity, &loc);
}

void renderSmallIcon(int16_t x, int16_t y, int16_t u, int32_t otOffset)
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

void loadDynamicLibrary(Overlay lib, uint8_t *isComplete, dw_bool isAsync,
			FileCallback callback, void *param)
{
	uint8_t *nv;

	if (!isAsync) {
		readFile(OVERLAY_FILE_NAMES[lib - 1], (nv = OVERLAY_LOAD_ADDRESSES[lib - 1]));
	} else {
		addFileReadRequestPath(OVERLAY_FILE_NAMES[lib - 1],
				       (nv = OVERLAY_LOAD_ADDRESSES[lib - 1]), isComplete,
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
	uint32_t pad;

	if (MAIN_D_80134E78[0] != 0) {
		MAIN_func_800E642C();
		MAIN_D_80134E78[0] = 0;
	}
	if ((readPStat(0) == 3 || IS_GAMETIME_RUNNING == 0) && GAME_STATE == 0) {
		return;
	}
	if (FADE_PROTECTION == 1) {
		return;
	}
	MAIN_D_80134E70 = PadRead(1);
	pad = MAIN_D_80134E70;
	if ((pad & 0x800) && !(MAIN_D_80134E74 & 0x800)) {
		MAIN_D_80134E78[1] = (MAIN_D_80134E78[1] + 1) & 1;
	}
	MAIN_D_80134E74 = MAIN_D_80134E70;
	if (MAIN_D_80134E78[1] != 0) {
		createPauseBox();
		pauseFrame();
		pauseFrame();
		MAIN_D_80134E78[0]++;
	}
	while (MAIN_D_80134E78[1] != 0) {
		MAIN_D_80134E70 = PadRead(1);
		pad = MAIN_D_80134E70;
		if ((pad & 0x800) && !(MAIN_D_80134E74 & 0x800)) {
			MAIN_D_80134E78[1] = (MAIN_D_80134E78[1] + 1) & 1;
		}
		MAIN_D_80134E74 = MAIN_D_80134E70;
	}
}

void MAIN_func_800E642C(void)
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
		drawString(&TEXT_PAUSE, 0x78, 0xF0);
		setRECT(&pos, -0x1A, -0xE, 0x38, 0x18);
		createStaticUIBox(5, 1, 0, &pos, NULL, renderPauseBox);
		MAIN_D_80134E7C = 1;
		playSound(0, 3);
	}
}

void renderPauseBox(int32_t instanceId)
{
	GsBOXF box;
	RECT *new_var;
	box.attribute = 0x40000000;
	box.b = 0;
	box.g = 0;
	box.r = 0;
	setRECT(&box, -0xA0, -0x78, 0x140, 0xF0);
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 7 - instanceId);
	new_var = &UI_BOX_DATA[5].finalPos;
	renderString(0, (int16_t) ((*new_var).x + 6), (int16_t) ((*new_var).y + 6), 0x2A, 0xC, 0x78, 0xF0, 0, 1);
}

void MAIN_thunk_func_800D92EC(void)
{
	MAIN_func_800D92EC();
}

void setMapLayerEnabled(uint8_t enabled)
{
	MAP_LAYER_ENABLED = enabled;
}

void loadStackedTIMFile(char *path)
{
	GsIMAGE img;
	int32_t *p;

	p = (int32_t *)GENERAL_BUFFER;
	readFile(path, GENERAL_BUFFER);
	while (*p == 0x10) {
		p = (int32_t *)((char *)p + 4);
		GsGetTimInfo((u_long *)p, &img);
		p = &p[((img.pw * img.ph) / 2) + 4];
		LoadImage((RECT *)&img.px, img.pixel);
		if ((img.pmode >> 3) & 1) {
			LoadImage((RECT *)&img.cx, img.clut);
			p = &p[((img.cw * img.ch) / 2) + 3];
		}
	}
}

void playMovie(int32_t movieId, dw_bool shouldPlay)
{
	if (shouldPlay) {
		startMovie(movieId);
		initializeFramebuffer();
	}
}

void unlearnMove(int32_t move)
{
	int32_t i;
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

int32_t isTamerOnScreen(void)
{
	if (ENTITY_TABLE[0]->isOnScreen == 1) {
		return 1;
	}

	return 0;
}

void updateMapLightState(void)
{
	int32_t i;
	MapLightUpdateData *p;

	p = MAP_LIGHT_UPDATE_DATA;
	for (i = 0; i <= 0; i++, p++) {
		if ((p->mapId == (CURRENT_MAP_ID & 0xFF)) &&
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
	struct {
		uint8_t cup;
		uint8_t opponents[10];
		uint8_t isComplete;
	} t;
	uint8_t c;
	uint8_t *w;
	int32_t count;
	uint8_t id;
	uint8_t *p;
	uint8_t *pool;
	uint8_t i;
	int16_t result;
	int16_t expected;
	int32_t trig;

	id = readPStat(PSTAT_TOURNAMENT_ID);
	t.cup = id;
	pool = (uint8_t *)allocateArray(0x70);
	w = pool;
	p = getCupDataJumpTableEntry(getCupDataJumpTable(10, id), 4) + 2;
	count = 0;
	c = *p;
	if (c < 0xfe) {
		if (id != 0x16) {
			while ((c = *p++) < 0xfe) {
				if (isTriggerSet(c + TRIGGER_DIGIMON_MET) != 0) {
					*w++ = c;
					count++;
				}
			}
		} else {
			while ((c = *p++) < 0xfe) {
				*w++ = c;
				count++;
			}
		}
	} else {
		i = 0;
		trig = 200;
		while (i < 0x70) {
			if (isTriggerSet(trig) != 0) {
				*w++ = i;
				count++;
			}
			i++;
			trig++;
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
			*p++ = pool[random(count)];
		}
	} else {
		id = PARTNER_ENTITY.digimonEntity.entity.type;
		for (i = 0; i < count; i++) {
			if (pool[i] == id) {
				pool[i] = 0xff;
				count--;
				break;
			}
		}
		for (i = 0; i < 7; i++) {
			id = (uint8_t)random(count) + 1;
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
	loadDynamicLibrary(STD_REL, &t.isComplete, 0, NULL, NULL);
	result = STD_func_800579D8(&t.cup);
	MAIN_thunk_func_800D92EC();
	unsetTrigger(TRIGGER_TOURNAMENT_REGISTERED);
	id = readPStat(PSTAT_TOURNAMENT_ID);
	if (id != 5) {
		expected = 3;
	} else {
		expected = 2;
	}
	if (result == expected) {
		TOURNAMENT_TITLES++;
		TOURNAMENT_TITLES = enforceStatsLimits(SCRIPT_STAT_TOURNAMENT_TITLES, TOURNAMENT_TITLES);
		setTrigger(id + TRIGGER_CUP_WON);
	} else {
		TOURNAMENT_LOSSES++;
	}
	TOURNAMENT_WINS += result;
	TOURNAMENT_WINS = enforceStatsLimits(SCRIPT_STAT_TOURNAMENT_WINS, TOURNAMENT_WINS);
	TOURNAMENT_LOSSES = enforceStatsLimits(SCRIPT_STAT_TOURNAMENT_LOSSES, TOURNAMENT_LOSSES);
	writePStat(PSTAT_RESULT, result);
}
