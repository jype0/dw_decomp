#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/btl.h>
#include <dw/dooa.h>
#include <dw/entity.h>
#include <dw/file.h>
#include <dw/file_queue.h>
#include <dw/main.h>
#include <dw/model.h>
#include <dw/murd.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/utils.h>

#define MURD_ORDERING_TABLE_0	((GsOT_TAG *)0x8008c000)

typedef struct {
	int16_t timer;
	int16_t phase;
	Entity *entity;
	int16_t lives;
	int16_t pad;
} MurdScene;

extern GsF_LIGHT LIGHT_DATA[3];
extern int32_t ACTIVE_FRAMEBUFFER;
extern int32_t VIEWPORT_DISTANCE;

void setMapLayerEnabled(int32_t enabled);
int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t);
int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out);
int32_t getDistance(int32_t x, int32_t y, int32_t z);
void renderParticleFlash(int16_t *params);
void renderDropShadow(Entity *entity);

void MAIN_func_800D91EC(int32_t message, int32_t value);
void MAIN_func_800D91FC(int32_t value);
void MAIN_func_800D9248(void);
void MAIN_func_800D9B60(uint32_t *buffer);
void MAIN_func_800D9BA8(int32_t level, int16_t *src, int32_t unused);

void MURD_tickScene(int32_t instanceId);
void MURD_renderScene(void);
void MURD_initializeOrderingTables(void);
void MURD_storeDigimonTexture(uint16_t *buffer, Entity *entity);
int32_t MURD_isDigimonLargerThanIris(Entity *entity, int32_t start, int32_t end, int32_t t);
void MURD_setOtherEntitiesVisible(int32_t restore);
void MURD_createLivesBox(Entity *entity);
void MURD_animateLivesBoxOut(void);
void MURD_renderFullscreenFade(VECTOR *color);
void MURD_removeLivesBox(void);
void MURD_renderDigimon(Entity *entity, int32_t depth);
int32_t MURD_renderIris(Entity *entity, int32_t start, int32_t end, int32_t t);
void MURD_tickLivesBox(void);
void MURD_renderLivesBox(int32_t layer);

static void *murd_functions[] = {
	MURD_tick,
	MURD_renderLivesBox,
	MURD_tickLivesBox,
	MURD_renderIris,
	MURD_renderDigimon,
	MURD_removeLivesBox,
	MURD_renderFullscreenFade,
	MURD_animateLivesBoxOut,
	MURD_createLivesBox,
	MURD_setOtherEntitiesVisible,
	MURD_isDigimonLargerThanIris,
	MURD_storeDigimonTexture,
	MURD_initializeOrderingTables,
	MURD_renderScene,
	MURD_tickScene,
};

int8_t MURD_LOADING_COMPLETE = 1;
int8_t MURD_ENTITIES_VISIBLE = 1;
RECT MURD_LIVES_BOX_FINAL_POS = { -95, -50, 190, 44 };
RECT MURD_LIVES_BOX_START_POS = { -8, -6, 16, 12 };
RECT MURD_LIVES_BOX_TARGET_POS = { -8, -6, 16, 12 };

MurdLivesBox MURD_LIVES_BOX;

char MURD_LIFE_TIM_PATH[16] = "\\ETCHI\\LIFE.TIM";

VECTOR MURD_FLASH_RISE = { 0x80, 0x80, 0x80, 0 };
VECTOR MURD_FLASH_HOLD_1 = { 0xff, 0xff, 0xff, 0 };
VECTOR MURD_FLASH_HOLD_2 = { 0xff, 0xff, 0xff, 0 };
VECTOR MURD_FLASH_HOLD_3 = { 0xff, 0xff, 0xff, 0 };
VECTOR MURD_FLASH_FADE = { 0x80, 0x80, 0x80, 0 };

// clang-format off
GsSPRITE MURD_LIVES_BACKDROP = {
	0x50000000,			/* attribute */
	-85,				/* x */
	-40,				/* y */
	64,				/* w */
	24,				/* h */
	getTPage(0, 1, 576, 256),	/* tpage */
	48,				/* u */
	0,				/* v */
	128,				/* cx */
	488,				/* cy */
	0x80,				/* r */
	0x80,				/* g */
	0x80,				/* b */
	0,				/* mx */
	0,				/* my */
	0x1000,				/* scalex */
	0x1000,				/* scaley */
	0,				/* rotate */
};

GsSPRITE MURD_LIFE_FULL = {
	0x50000000,			/* attribute */
	-11,				/* x */
	-40,				/* y */
	24,				/* w */
	24,				/* h */
	getTPage(0, 1, 576, 256),	/* tpage */
	0,				/* u */
	0,				/* v */
	128,				/* cx */
	488,				/* cy */
	0x80,				/* r */
	0x80,				/* g */
	0x80,				/* b */
	0,				/* mx */
	0,				/* my */
	0x1000,				/* scalex */
	0x1000,				/* scaley */
	0,				/* rotate */
};

GsSPRITE MURD_LIFE_EMPTY = {
	0x50000000,			/* attribute */
	-11,				/* x */
	-40,				/* y */
	24,				/* w */
	24,				/* h */
	getTPage(0, 1, 576, 256),	/* tpage */
	24,				/* u */
	0,				/* v */
	128,				/* cx */
	488,				/* cy */
	0x80,				/* r */
	0x80,				/* g */
	0x80,				/* b */
	0,				/* mx */
	0,				/* my */
	0x1000,				/* scalex */
	0x1000,				/* scaley */
	0,				/* rotate */
};

MurdScene MURD_SCENE = {
	0,	/* timer */
	0,	/* phase */
	NULL,	/* entity */
	0,	/* lives */
	0,	/* pad */
};
// clang-format on

static void murd__garbage__(void)
{
	int32_t v0;
	int32_t v1;
	int32_t v2;
	int32_t v3;

	v0 = MURD_TEXTURE_BUFFER[0] + 0;
	v1 = MURD_TEXTURE_BUFFER[1] + 1;
	v2 = MURD_TEXTURE_BUFFER[2] + 2;
	v3 = MURD_TEXTURE_BUFFER[3] + 3;
	MURD_TEXTURE_BUFFER[0] = (v0 * v1) + v2;
	MURD_TEXTURE_BUFFER[1] = (v1 * v2) + v3;
	MURD_TEXTURE_BUFFER[2] = (v2 * v3) + v0;
	MURD_TEXTURE_BUFFER[3] = (v3 * v0) + v1;
}

void MURD_tickScene(int32_t instanceId)
{
	MurdScene *scene = &MURD_SCENE;
	Entity *entity = scene->entity;

	scene->timer++;
	switch (scene->phase) {
	case 0:
		if (MURD_isDigimonLargerThanIris(scene->entity, 0, 0x23, scene->timer) != 0) {
			entity->isOnMap = 0;
		}
		if (scene->timer < 0x23) {
			break;
		}
		scene->phase = 1;
		MAIN_func_800D9B60((uint32_t *)MURD_PALETTE_BACKUP);
		MAIN_func_800D9BA8(0xff, MURD_PALETTE_BACKUP, 0);
		MURD_setOtherEntitiesVisible(0);
		MURD_createLivesBox(entity);
		stopBGM();
		LIGHT_DATA[0].vx = 0x1e;
		LIGHT_DATA[0].vy = 0x64;
		LIGHT_DATA[0].vz = 0x1e;
		LIGHT_DATA[0].r = 0x40;
		LIGHT_DATA[0].g = 0x40;
		LIGHT_DATA[0].b = 0x40;
		GsSetFlatLight(0, &LIGHT_DATA[0]);
		LIGHT_DATA[1].vx = -0x1e;
		LIGHT_DATA[1].vy = 0x64;
		LIGHT_DATA[1].vz = 0;
		LIGHT_DATA[1].r = 0x28;
		LIGHT_DATA[1].g = 0x28;
		LIGHT_DATA[1].b = 0x28;
		GsSetFlatLight(1, &LIGHT_DATA[1]);
		LIGHT_DATA[2].vx = 0;
		LIGHT_DATA[2].vy = 0x64;
		LIGHT_DATA[2].vz = -0x1e;
		LIGHT_DATA[2].r = 0x26;
		LIGHT_DATA[2].g = 0x26;
		LIGHT_DATA[2].b = 0x26;
		GsSetFlatLight(2, &LIGHT_DATA[2]);
		setMapLayerEnabled(0);
		break;
	case 1:
		entity->isOnMap = 1;
		/* fall through */
	case 2:
		if (scene->timer < 0x5f) {
			break;
		}
		if (scene->lives == 0) {
			scene->phase = 0x64;
			MURD_animateLivesBoxOut();
			break;
		}
		scene->phase = 3;
		break;
	case 3: {
		VECTOR color = MURD_FLASH_RISE;

		color.vx = lerp(0, 0xa0, 0x5f, 0x87, scene->timer);
		color.vy = color.vz = color.vx;
		MURD_renderFullscreenFade(&color);
		if (scene->timer < 0x87) {
			break;
		}
		scene->phase = 4;
		MURD_removeLivesBox();
		break;
	}
	case 4: {
		VECTOR color = MURD_FLASH_HOLD_1;

		MURD_renderFullscreenFade(&color);
		if (UI_BOX_DATA[3].state != 0) {
			break;
		}
		scene->phase = 5;
		break;
	}
	case 5: {
		VECTOR color = MURD_FLASH_HOLD_2;

		MURD_renderFullscreenFade(&color);
		MAIN_func_800D91FC(0);
		MAIN_func_800D9248();
		setFullState(0x13, 1);
		setPartnerState(0xb);
		ENTITY_TABLE[0]->isOnMap = 1;
		scene->phase = 6;
		break;
	}
	case 6: {
		VECTOR color = MURD_FLASH_HOLD_3;

		MURD_renderFullscreenFade(&color);
		if (scene->timer < 0x91) {
			break;
		}
		scene->phase = 7;
		setMapLayerEnabled(1);
		break;
	}
	case 7: {
		VECTOR color = MURD_FLASH_FADE;

		color.vx = lerp(0xa0, 0, 0x91, 0xb9, scene->timer);
		color.vy = color.vz = color.vx;
		MURD_renderFullscreenFade(&color);
		if (scene->timer < 0xb9) {
			break;
		}
		removeObject(0x60a, instanceId);
		scene->timer = -0xa;
		break;
	}
	case 0x64:
		if (UI_BOX_DATA[3].state != 0) {
			break;
		}
		scene->phase = 0x65;
		break;
	case 0x65:
		if (scene->lives == 0) {
			while (MURD_LOADING_COMPLETE != 0) {
				tickFileReadQueue(0);
			}
		}
		removeObject(0x60a, instanceId);
		scene->timer = -0xa;
		DOOA_tick((PartnerEntity *)ENTITY_TABLE[1], GENERAL_BUFFER_PTR + 0x4b000, 0);
		ENTITY_TABLE[1]->isOnMap = 0;
		break;
	}
}

void MURD_renderScene(void)
{
	MurdScene *scene = &MURD_SCENE;
	Entity *entity = scene->entity;

	if (scene->timer < 0x24) {
		if (entity->isOnMap == 0) {
			MURD_renderDigimon(scene->entity, 0x21);
		}
		MURD_renderIris(scene->entity, 0, 0x23, scene->timer);
	}
}

void MURD_initializeOrderingTables(void)
{
	GsOT_TAG *tags = MURD_ORDERING_TABLE_0;

	MURD_ORDERING_TABLES[0].length = 0xb;
	MURD_ORDERING_TABLES[0].org = tags;
	MURD_ORDERING_TABLES[1].length = 0xb;
	MURD_ORDERING_TABLES[1].org = tags + 0x800;
}

void MURD_storeDigimonTexture(uint16_t *buffer, Entity *entity)
{
	ModelComponent *model;
	RECT rect;

	model = getEntityModelComponent(entity->type, getEntityType(entity));
	setRECT(&rect, (model->clutPage & 0x3f) << 4, model->clutPage >> 6, 0x10, 0x18);
	StoreImage(&rect, (u_long *)buffer);

	DrawSync(0);
}

int32_t MURD_isDigimonLargerThanIris(Entity *entity, int32_t start, int32_t end, int32_t t)
{
	SVECTOR pos;
	DVECTOR screen;
	int32_t size;
	int32_t radius;
	int32_t z;

	pos.vx = entity->posData->location.vx;
	pos.vy = entity->posData->location.vy - (DIGIMON_DATA[entity->type].height / 2);
	pos.vz = entity->posData->location.vz;
	size = getDistance(DIGIMON_DATA[entity->type].radius * 2, DIGIMON_DATA[entity->type].height, DIGIMON_DATA[entity->type].radius * 2);
	radius = (lerp(0xc8, 0, start, end, t) << 12) / 128;

	z = worldPosToScreenPos(&pos, &screen);
	if (z <= 0) {
		return 1;
	}

	if (((int32_t)(((uint32_t)(size * VIEWPORT_DISTANCE) / (uint32_t)z) << 12) / 256) >= radius) {
		return 1;
	}

	return 0;
}

void MURD_setOtherEntitiesVisible(int32_t restore)
{
	int32_t i;

	MURD_ENTITIES_VISIBLE = restore;
	if (restore == 0) {
		for (i = 0; i < ENTITY_MAX; i++) {
			if ((ENTITY_TABLE[i] != (Entity *)&PARTNER_ENTITY) && (ENTITY_TABLE[i]->isOnMap != 0)) {
				ENTITY_TABLE[i]->isOnMap = 0;
				MURD_ENTITY_VISIBILITY[i] = 1;
			} else {
				MURD_ENTITY_VISIBILITY[i] = 0;
			}
		}
	} else {
		for (i = 0; i < ENTITY_MAX; i++) {
			if (ENTITY_TABLE[i] != (Entity *)&PARTNER_ENTITY) {
				ENTITY_TABLE[i]->isOnMap = MURD_ENTITY_VISIBILITY[i];
			}
		}
	}
}

void MURD_createLivesBox(Entity *entity)
{
	RECT start;
	SVECTOR pos;

	start = MURD_LIVES_BOX_START_POS;
	MURD_LIVES_BOX.frame = 0;
	MURD_LIVES_BOX.state = 0;
	MURD_LIVES_BOX.partner = (PartnerEntity *)entity;

	copyVector(&pos, &entity->posData->location);
	worldPosToScreenPos(&pos, (DVECTOR *)&start);

	start.x = start.x - (int16_t)(0xa8 - DRAWING_OFFSET_X);
	start.y = start.y - (int16_t)(0x7e - DRAWING_OFFSET_Y);
	createAnimatedUIBox(3, 0, 2, &MURD_LIVES_BOX_FINAL_POS, &start, (TickFunction)MURD_tickLivesBox, (RenderFunction)MURD_renderLivesBox);
}

void MURD_animateLivesBoxOut(void)
{
	RECT target;
	SVECTOR pos;

	target = MURD_LIVES_BOX_TARGET_POS;

	copyVector(&pos, &MURD_LIVES_BOX.partner->digimonEntity.entity.posData->location);
	worldPosToScreenPos(&pos, (DVECTOR *)&target);

	target.x = target.x - (int16_t)(0xa8 - DRAWING_OFFSET_X);
	target.y = target.y - (int16_t)(0x7e - DRAWING_OFFSET_Y);
	removeAnimatedUIBox(3, &target);
}

void MURD_renderFullscreenFade(VECTOR *color)
{
	POLY_FT4 *prim;
	int32_t layer = 0;

	prim = (POLY_FT4 *)GsGetWorkBase();

	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = getTPage(1, 2, 832, 256);
	prim->clut = getClut(0, 487);
	setXYWH(prim, -0xa0, -0x78, 320, 240);
	setUVWH(prim, 0, 0x80, 3, 3);
	setRGB0(prim, color->vx, color->vy, color->vz);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim);
	prim++;

	GsSetWorkBase((PACKET *)prim);
}

void MURD_removeLivesBox(void)
{
	removeStaticUIBox(3);
}

void MURD_renderDigimon(Entity *entity, int32_t depth)
{
	MATRIX m;
	PositionData *pos;
	int32_t count;
	int32_t i;
	int32_t bone;

	for (i = 0; i < ENTITY_MAX; i++) {
		if (ENTITY_TABLE[i] == entity) {
			break;
		}
	}

	if (i == ENTITY_MAX) {
		return;
	}

	GsClearOt(0, 2, &MURD_ORDERING_TABLES[ACTIVE_FRAMEBUFFER]);

	MURD_ORDERING_TABLES[ACTIVE_FRAMEBUFFER].point = depth;
	count = DIGIMON_DATA[entity->type].boneCount;
	pos = entity->posData;
	m = GsWSMATRIX;

	for (bone = 0; bone < count; pos++, bone++) {
		if (pos->obj.tmd != NULL) {
			GsGetLw(pos->obj.coord2, &m);
			GsSetLightMatrix(&m);
			GsGetLs(pos->obj.coord2, &m);
			GsSetLsMatrix(&m);
			GsSortObject4(&pos->obj, &MURD_ORDERING_TABLES[ACTIVE_FRAMEBUFFER], 3, getScratchAddr(0));
		}
	}

	GsSortOt(&MURD_ORDERING_TABLES[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	renderDropShadow(entity);
}

int32_t MURD_renderIris(Entity *entity, int32_t start, int32_t end, int32_t t)
{
	SVECTOR pos;
	DVECTOR screen;
	int16_t flash[14];
	POLY_FT4 *prim;
	int32_t size;
	int32_t radius;
	int32_t z;
	int32_t visible;
	int32_t c = 0x80;
	int32_t left;
	int32_t right;
	int32_t cx;
	int32_t top;
	int32_t bottom;
	int32_t sl;
	int32_t sr;
	int32_t st;
	int32_t st2;
	int32_t st3;
	int32_t sb;
	int32_t thickness;
	int32_t lx;
	int32_t ly;
	int32_t lw;
	int32_t rx;
	int32_t ry;
	int32_t rw;
	int32_t ty;
	int32_t tw;
	int32_t th;
	int32_t by;
	int32_t bw;
	int32_t bh;

	pos.vx = entity->posData->location.vx;
	pos.vy = entity->posData->location.vy - (DIGIMON_DATA[entity->type].height / 2);
	pos.vz = entity->posData->location.vz;
	size = getDistance(DIGIMON_DATA[entity->type].radius * 2, DIGIMON_DATA[entity->type].height, DIGIMON_DATA[entity->type].radius * 2);
	radius = (lerp(0xc8, 0, start, end, t) << 12) / 128;
	z = worldPosToScreenPos(&pos, &screen);
	if (z <= 0) {
		return 1;
	}

	if (((int32_t)(((uint32_t)(size * VIEWPORT_DISTANCE) / (uint32_t)z) << 12) / 256) >= radius) {
		visible = 1;
	} else {
		visible = 0;
	}

	flash[0] = screen.vx;
	flash[1] = screen.vy;
	flash[7] = 0x40;
	flash[6] = 0x40;
	flash[8] = getTPage(1, 2, 832, 256);
	((uint8_t *)flash)[0x12] = 0;
	((uint8_t *)flash)[0x13] = c;
	flash[10] = getClut(0, 487);
	((uint8_t *)flash)[0x16] = c;
	((uint8_t *)flash)[0x17] = c;
	((uint8_t *)flash)[0x18] = c;
	((uint8_t *)flash)[0x19] = c;
	*(int32_t *)&flash[4] = radius;
	flash[2] = 0x22;
	renderParticleFlash(flash);
	prim = (POLY_FT4 *)GsGetWorkBase();
	right = (radius << 8) / 4096;
	top = screen.vy - right;
	cx = screen.vx;
	left = cx - right;
	bottom = screen.vy + right;
	right = right + cx;
	sl = left - (0xa0 - DRAWING_OFFSET_X);
	sr = right - (0xa0 - DRAWING_OFFSET_X);
	st = top - (0x78 - DRAWING_OFFSET_Y);
	sb = bottom - (0x78 - DRAWING_OFFSET_Y);
	st2 = st;
	st3 = st;
	thickness = lerp(4, 1, 0x1860, 0, radius);

	lx = left - (sl + 0xa0);
	ly = top - (st + 0x78);
	lw = (sl + 0xa0) + thickness;
	if (lw > 0) {
		SetPolyFT4(prim);
		SetSemiTrans(prim, 2);
		prim->r0 = prim->g0 = prim->b0 = 0x80;
		prim->tpage = getTPage(1, 2, 832, 256);
		prim->clut = getClut(0, 487);
		setUVWH(prim, 0, 0x80, 3, 3);
		setXYWH(prim, lx, ly, lw, 0xf0);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x22, prim++);
	}

	ry = top - (st2 + 0x78);
	rw = (0xa0 - sr) + thickness;
	rx = right - thickness;
	if (rw > 0) {
		SetPolyFT4(prim);
		prim->r0 = prim->g0 = prim->b0 = 0x80;
		SetSemiTrans(prim, 2);
		prim->tpage = getTPage(1, 2, 832, 256);
		prim->clut = getClut(0, 487);
		setUVWH(prim, 0, 0x80, 3, 3);
		setXYWH(prim, rx, ry, rw, 0xf0);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x22, prim++);
	}

	ty = top - (st3 + 0x78);
	tw = right - left;
	if (tw > 0) {
		th = (st3 + 0x78) + thickness;
		if (th > 0) {
			SetPolyFT4(prim);
			prim->r0 = prim->g0 = prim->b0 = 0x80;
			SetSemiTrans(prim, 2);
			prim->tpage = getTPage(1, 2, 832, 256);
			prim->clut = getClut(0, 487);
			setUVWH(prim, 0, 0x80, 3, 3);
			setXYWH(prim, left, ty, tw, th);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 0x22, prim++);
		}
	}

	by = bottom - thickness;
	bw = (int32_t)right - left;
	if (bw > 0) {
		bh = (0x78 - sb) + thickness;
		if (bh > 0) {
			SetPolyFT4(prim);
			prim->r0 = prim->g0 = prim->b0 = 0x80;
			SetSemiTrans(prim, 2);
			prim->tpage = getTPage(1, 2, 832, 256);
			prim->clut = getClut(0, 487);
			setUVWH(prim, 0, 0x80, 3, 3);
			setXYWH(prim, left, by, bw, bh);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 0x22, prim++);
		}
	}

	GsSetWorkBase((PACKET *)prim);

	return visible;
}

void MURD_tickLivesBox(void)
{
	MurdLivesBox *box = &MURD_LIVES_BOX;

	box->frame++;

	switch (box->state) {
	case 0:
		if (box->frame >= 0xa) {
			box->state = 1;
		}
		break;
	case 1:
		if (box->frame >= 0x1e) {
			box->state = 2;
		}
		break;
	case 2:
		break;
	}
}

void MURD_renderLivesBox(int32_t layer)
{
	int32_t depth = 6 - layer;
	PartnerEntity *partner = MURD_LIVES_BOX.partner;
	MurdLivesBox *box = &MURD_LIVES_BOX;
	int32_t offset = 0;
	int32_t i;
	int32_t x;

	GsSortSprite(&MURD_LIVES_BACKDROP, ACTIVE_ORDERING_TABLE, depth);

	for (i = 0, x = -0xb; i < (partner->lives + offset); i++, x += 0x22) {
		MURD_LIFE_FULL.x = x;
		GsSortSprite(&MURD_LIFE_FULL, ACTIVE_ORDERING_TABLE, depth);
	}

	MURD_LIFE_FULL.x = (i * 0x22) - 0xb;
	MURD_LIFE_EMPTY.x = (i * 0x22) - 0xb;

	switch (box->state) {
	case 0:
		GsSortSprite(&MURD_LIFE_FULL, ACTIVE_ORDERING_TABLE, depth);
		break;
	case 1:
		if (((box->frame / 2) & 1) == 0) {
			GsSortSprite(&MURD_LIFE_FULL, ACTIVE_ORDERING_TABLE, depth);
		} else {
			GsSortSprite(&MURD_LIFE_EMPTY, ACTIVE_ORDERING_TABLE, depth);
		}
		break;
	case 2:
		GsSortSprite(&MURD_LIFE_EMPTY, ACTIVE_ORDERING_TABLE, depth);
		break;
	}

	for (i = partner->lives + 1, x = (i * 0x22) - 0xb; i < 3; i++, x += 0x22) {
		MURD_LIFE_EMPTY.x = x;
		GsSortSprite(&MURD_LIFE_EMPTY, ACTIVE_ORDERING_TABLE, depth);
	}
}

int32_t MURD_tick(PartnerEntity *partner, int32_t isInitialized)
{
	MurdScene *scene = &MURD_SCENE;
	int32_t message;

	if (isInitialized != 0) {
		return scene->timer;
	}

	addObject(0x60a, 0, MURD_tickScene, (RenderFunction)MURD_renderScene);

	scene->timer = 0;
	scene->phase = 0;
	scene->entity = (Entity *)partner;
	scene->lives = partner->lives;

	loadTextureFile(MURD_LIFE_TIM_PATH, NULL, NULL);

	if (scene->lives != 0) {
		if ((isTriggerSet(0xdc) == 1) || (isTriggerSet(0xd6) == 1) || (readPStat(1) >= 0x32)) {
			message = 0xcd;
		} else {
			message = 0xda;
		}
		MAIN_func_800D91EC(message, 1);
	} else {
		loadDynamicLibrary(DOOA_REL, NULL, 0, NULL, NULL);
		MURD_LOADING_COMPLETE = 0;
	}

	MURD_initializeOrderingTables();
	MURD_storeDigimonTexture(MURD_TEXTURE_BUFFER, (Entity *)partner);

	return 0;
}
