#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/btl.h>
#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/graphics.h>
#include <dw/model.h>
#include <dw/types.h>

#include "common.h"

typedef struct {
	int32_t timer;
	SVECTOR pos;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t pad;
} EvlParticle;

typedef struct {
	int16_t timer;
	int16_t pad;
	int32_t centers;
	int32_t *model;
	int32_t vertices;
	int32_t primitives;
	int16_t centerCount;
	int16_t pad2;
} EvlShardSet;

typedef struct {
	int16_t vx;
	int16_t vy;
	int16_t vz;
} EvlModelVertex;

typedef struct {
	int16_t bone;
	int16_t timer;
	Entity *entity;
} EvlSpark;

extern int32_t MAIN_D_8013520C;
extern uint8_t *MAIN_D_80135210;
extern EvlModelVertex *MAIN_D_80135214;
extern int16_t MAIN_D_80135218[3];
extern int8_t MAIN_D_801349F4;
extern int8_t HAS_USED_EVOITEM;
extern char *MAIN_D_80135208;
extern int8_t EVL_D_80068938[];
extern EvlParticle EVL_D_80068944[100];
extern int16_t EVL_D_80068F84[][4];
extern int16_t EVL_D_80063F3C[];
extern int32_t EVL_D_80064D40[];
extern int32_t EVL_D_80064D44[];
extern int32_t EVL_D_80064D48[];
extern char EVL_D_80064D50[];
extern uint16_t EVL_D_80065094[];
extern u_long EVL_D_80065398[];
extern u_long EVL_D_8006569C[];
extern char EVL_D_80065FA0[];
extern char EVL_D_80066274[];
extern char EVL_D_800677B0[];
extern int16_t EVL_D_80067992[];
extern int16_t EVL_D_80067994[];

int32_t rand(void);
void MAIN_func_800D9B60(int16_t *clut);
void MAIN_func_800D9E68(char *base);
char *initializeFlashData(char *base);
int32_t customRandom(int32_t a, int32_t b);
int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t);
int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out);
void MAIN_func_80092B60(POLY_FT4 *prim);
void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
int32_t add3DSpritePrim(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
int32_t getEntityType(Entity *entity);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
void removeObject(int32_t objectId, int32_t instanceId);

void EVL_setScratchTop(int32_t size);
void EVL_resetParticles(void);
void EVL_resetSparks(void);
void EVL_storeClutBank1(u_long *pixels);
void EVL_releaseAllParticles(void);
void EVL_tickParticle(int32_t id);
void EVL_storeDigimonClut(uint16_t *buffer, Entity *entity);
void EVL_storeClutBank0(u_long *pixels);
void EVL_setOtherEntitiesVisible(int32_t restore);
int32_t EVL_spawnParticle(VECTOR *position, RGB8 *color);
void EVL_renderParticle(int32_t id);
void addObject(int32_t objectId, int32_t instanceId, void *tick, void *render);

char *EVL_initShardSets(char *base);
void EVL_tickEvoSequence(void);
void EVL_fadeClutBank0(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame);
void EVL_fadeClutBank1(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame);
void EVL_updateEvoCamera(void);
void EVL_brightenDigimonClut(void);
void EVL_buildShardSet(void);
int32_t EVL_spawnSpark(void *owner, int32_t timer, int32_t param);
void EVL_calculateCameraVectors(VECTOR *viewRef, VECTOR *viewPos, Entity *entity, SVECTOR *rotation, int32_t distance, int32_t height);
void EVL_tickShardSet(int32_t id);
void EVL_renderShardSet(int32_t index);
void EVL_renderTriShard(EvlModelVertex *drift, int32_t unused1, int16_t speed, int16_t timer, ModelComponent *model);
void EVL_renderQuadShard(EvlModelVertex *drift, int32_t unused1, int16_t speed, int16_t timer, ModelComponent *model);
void EVL_renderSparkStreak(int32_t id);
void EVL_tickSpark(int32_t id);
void EVL_applyEvolution(void);
void EVL_scaleBaseStats(Stats *stats, int16_t pct);
void EVL_clampBaseStats(void);
void EVL_renderEvoSequence(void);

static void *evl_functions[] = {
	EVL_clampBaseStats,
	EVL_scaleBaseStats,
	EVL_applyEvolution,
	EVL_initEvoSequence,
	EVL_tickSpark,
	EVL_renderSparkStreak,
	EVL_renderParticle,
	EVL_tickParticle,
	EVL_renderQuadShard,
	EVL_renderTriShard,
	EVL_renderShardSet,
	EVL_tickShardSet,
	EVL_releaseAllParticles,
	EVL_calculateCameraVectors,
	EVL_spawnSpark,
	EVL_buildShardSet,
	EVL_brightenDigimonClut,
	EVL_spawnParticle,
	EVL_updateEvoCamera,
	EVL_setOtherEntitiesVisible,
	EVL_fadeClutBank1,
	EVL_fadeClutBank0,
	EVL_renderEvoSequence,
	EVL_tickEvoSequence,
	EVL_resetSparks,
	EVL_resetParticles,
	EVL_setScratchTop,
	EVL_initShardSets,
	EVL_storeClutBank1,
	EVL_storeClutBank0,
	EVL_storeDigimonClut,
};

void EVL_storeDigimonClut(uint16_t *buffer, Entity *entity)
{
	ModelComponent *model;
	RECT rect;

	model = getEntityModelComponent(entity->type, getEntityType(entity));
	rect.x = (model->clutPage & 0x3f) << 4;
	rect.y = model->clutPage >> 6;
	rect.w = 0x10;
	rect.h = 0x18;
	StoreImage(&rect, (u_long *)buffer);

	DrawSync(0);
}

void EVL_storeClutBank0(u_long *pixels)
{
	RECT rect;

	setRECT(&rect, 0, 488, 16, 24);
	StoreImage(&rect, pixels);
	DrawSync(0);
}

void EVL_storeClutBank1(u_long *pixels)
{
	RECT rect;

	setRECT(&rect, 32, 488, 48, 24);
	StoreImage(&rect, pixels);
	DrawSync(0);
}

char *EVL_initShardSets(char *base)
{
	int32_t i;
	int32_t off;
	int32_t rem;

	rem = (int32_t)base & 3;
	if (rem != 0) {
		base = (char *)((int32_t)base + (4 - rem));
	}

	MAIN_D_80135208 = base;
	base = (char *)((int32_t)base + 0x2d0);
	for (i = 0, off = 0; i < 30; i++, off += 0x18) {
		*(int16_t *)(MAIN_D_80135208 + off) = -1;
	}

	return base;
}

void EVL_setScratchTop(int32_t size)
{
	int32_t rem;

	rem = size & 3;
	if (rem != 0) {
		size += 4 - rem;
	}

	MAIN_D_8013520C = size;
}

void EVL_resetParticles(void)
{
	int32_t i;

	for (i = 0; i < 0x64; i++) {
		EVL_D_80068944[i].timer = -1;
	}
}

void EVL_resetSparks(void)
{
	int32_t i;

	for (i = 0; i < 0x10; i++) {
		EVL_D_80068F84[i][0] = -1;
	}
}

INCLUDE_ASM("asm/evl/nonmatchings/evl", EVL_tickEvoSequence);

void EVL_renderEvoSequence(void)
{
}

void EVL_fadeClutBank0(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame)
{
	int32_t i;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t stp;
	int16_t *dst;
	int16_t *src;
	RECT rect;

	src = srcClut;
	dst = dstClut;
	rand();
	rand();
	rand();
	for (i = 0; i < 384; i++) {
		int32_t den;
		int32_t num;

		num = endFrame - frame;
		den = endFrame - startFrame;

		r = *src & 0x1f;
		g = (*src >> 5) & 0x1f;
		b = (*src >> 10) & 0x1f;
		stp = (*src++ >> 15) & 0x1;

		if (frame != startFrame) {
			stp = 1;
		}
		r = r * num / den;
		g = g * num / den;
		b = b * num / den;

		*dst = r;
		*dst += (int16_t)(g << 5);
		*dst += (int16_t)(b << 10);
		*dst++ += (int16_t)(stp << 15);
	}

	rect.x = 0;
	rect.y = 488;
	rect.w = 16;
	rect.h = 24;
	LoadImage(&rect, (u_long *)dstClut);
}

void EVL_fadeClutBank1(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame)
{
	int32_t i;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t stp;
	int16_t *dst;
	int16_t *src;
	RECT rect;

	src = srcClut;
	dst = dstClut;
	rand();
	rand();
	rand();
	for (i = 0; i < 1152; i++) {
		int32_t den;
		int32_t num;

		num = endFrame - frame;
		den = endFrame - startFrame;

		r = *src & 0x1f;
		g = (*src >> 5) & 0x1f;
		b = (*src >> 10) & 0x1f;
		stp = (*src++ >> 15) & 0x1;

		if (frame != startFrame) {
			stp = 1;
		}
		r = r * num / den;
		g = g * num / den;
		b = b * num / den;

		*dst = r;
		*dst += (int16_t)(g << 5);
		*dst += (int16_t)(b << 10);
		*dst++ += (int16_t)(stp << 15);
	}

	rect.x = 32;
	rect.y = 488;
	rect.w = 48;
	rect.h = 24;
	LoadImage(&rect, (u_long *)dstClut);
}

void EVL_setOtherEntitiesVisible(int32_t restore)
{
	int32_t i;

	MAIN_D_801349F4 = restore;
	if (restore == 0) {
		for (i = 0; i < ENTITY_MAX; i++) {
			if ((ENTITY_TABLE[i] != (Entity *)&PARTNER_ENTITY) && (ENTITY_TABLE[i]->isOnMap != 0)) {
				ENTITY_TABLE[i]->isOnMap = 0;
				EVL_D_80068938[i] = 1;
			} else {
				EVL_D_80068938[i] = 0;
			}
		}
	} else {
		for (i = 0; i < ENTITY_MAX; i++) {
			if (ENTITY_TABLE[i] != (Entity *)&PARTNER_ENTITY) {
				ENTITY_TABLE[i]->isOnMap = EVL_D_80068938[i];
			}
		}
	}
}

INCLUDE_ASM("asm/evl/nonmatchings/evl", EVL_updateEvoCamera);

int32_t EVL_spawnParticle(VECTOR *position, RGB8 *color)
{
	EvlParticle *e;
	int32_t i;

	e = EVL_D_80068944;
	for (i = 0; i < 100; i++) {
		if (e->timer < 0) {
			break;
		}
		e++;
	}

	if (i == 100) {
		return -1;
	}

	e->timer = 0;
	e->pos.vx = position->vx;
	e->pos.vy = position->vy;
	e->pos.vz = position->vz;
	e->r = color->r;
	e->g = color->g;
	e->b = color->b;
	addObject(0x607, i, EVL_tickParticle, EVL_renderParticle);

	return i;
}

INCLUDE_ASM("asm/evl/nonmatchings/evl", EVL_brightenDigimonClut);

INCLUDE_ASM("asm/evl/nonmatchings/evl", EVL_buildShardSet);

int32_t EVL_spawnSpark(void *owner, int32_t timer, int32_t param)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 16; i++) {
		if (EVL_D_80068F84[i][0] == -1) {
			break;
		}
	}

	if (i == 16) {
		return -1;
	}

	p = EVL_D_80068F84[i];
	p[0] = timer;
	*(int32_t *)&p[2] = (int32_t)owner;
	p[1] = param;
	addObject(0x605, i, EVL_tickSpark, EVL_renderSparkStreak);

	return i;
}

void EVL_calculateCameraVectors(VECTOR *viewRef, VECTOR *viewPos, Entity *entity, SVECTOR *rotation, int32_t distance, int32_t height)
{
	MATRIX m1;
	MATRIX m2;
	VECTOR v;

	calculateBoneMatrix(entity, 0, &m1);
	viewRef->vx = m1.t[0];
	viewRef->vy = m1.t[1] - (height / 2);
	viewRef->vz = m1.t[2];
	RotMatrixZYX(rotation, &m2);
	v.vx = 0;
	v.vy = 0;
	v.vz = distance;
	ApplyMatrixLV(&m2, &v, viewPos);
	viewPos->vx += viewRef->vx;
	viewPos->vy += viewRef->vy - 0x190 - (height * 2);
	viewPos->vz += viewRef->vz;
}

void EVL_releaseAllParticles(void)
{
	int32_t i;
	int32_t *p;

	p = &EVL_D_80068944[0].timer;
	for (i = 0; i < 100; i++) {
		if (*p >= 0) {
			removeObject(0x607, i);
			*p = -1;
		}
		p = (int32_t *)((uint32_t)p + 0x10);
	}
}

void EVL_tickShardSet(int32_t id)
{
	int16_t *p;

	p = (int16_t *)(MAIN_D_80135208 + (id * 0x18));
	if (*p >= 0x1f) {
		removeObject(0x604, id);
		*p = -1;
	} else {
		*p += 1;
	}
}

void EVL_renderShardSet(int32_t index)
{
	ModelComponent *model;
	EvlShardSet *entry;
	int32_t shards;
	int32_t count;
	int32_t code;

	entry = &((EvlShardSet *)MAIN_D_80135208)[index];
	shards = entry->centers;
	model = getEntityModelComponent(entry->model[0], 3);
	count = entry->centerCount;
	MAIN_D_80135210 = (uint8_t *)entry->primitives;
	MAIN_D_80135214 = (EvlModelVertex *)entry->vertices;
	MAIN_D_80135218[0] = ((((31 - entry->timer) * 74) / 30) + 54);
	MAIN_D_80135218[1] = MAIN_D_80135218[0];
	MAIN_D_80135218[2] = MAIN_D_80135218[0];

	while (count-- > 0) {
		if (((code = ((int8_t *)MAIN_D_80135210)[3]) == 0x34) || (code == 0x36)) {
			EVL_renderTriShard((EvlModelVertex *)shards, 0, 60, entry->timer, model);
			shards += 6;
			MAIN_D_80135210 += 0x1c;
		} else if ((code == 0x3c) || (code == 0x3e)) {
			EVL_renderQuadShard((EvlModelVertex *)shards, 0, 60, entry->timer, model);
			shards += 6;
			MAIN_D_80135210 += 0x24;
		}
	}
}

void EVL_renderTriShard(EvlModelVertex *drift, int32_t unused1, int16_t speed, int16_t timer, ModelComponent *model)
{
	SVECTOR a;
	SVECTOR b;
	SVECTOR c;
	POLY_FT4 *prim;
	TMD_P_TG3 *tri;
	EvlModelVertex *v;
	int16_t dx;
	int16_t dy;
	int16_t dz;

	tri = (TMD_P_TG3 *)MAIN_D_80135210;
	prim = (POLY_FT4 *)GsGetWorkBase();
	MAIN_func_80092B60(prim);
	SetSemiTrans(prim, 1);
	prim->r0 = MAIN_D_80135218[0];
	prim->g0 = MAIN_D_80135218[1];
	prim->b0 = MAIN_D_80135218[2];
	prim->tpage = model->pixelPage;
	prim->clut = tri->clut;
	prim->u0 = tri->tu0;
	prim->v0 = tri->tv0;
	prim->u1 = tri->tu1;
	prim->v1 = tri->tv1;
	prim->u2 = tri->tu2;
	prim->v2 = tri->tv2;

	dx = drift->vx * timer / speed;
	dy = drift->vy * timer / speed;
	dz = drift->vz * timer / speed;

	v = &MAIN_D_80135214[tri->v0];
	a.vx = v->vx + dx;
	a.vy = v->vy + dy;
	a.vz = v->vz + dz;
	v = &MAIN_D_80135214[tri->v1];
	b.vx = v->vx + dx;
	b.vy = v->vy + dy;
	b.vz = v->vz + dz;
	v = &MAIN_D_80135214[tri->v2];
	c.vx = v->vx + dx;
	c.vy = v->vy + dy;
	c.vz = v->vz + dz;
	setSemiTrans(prim, 1);
	addScreenPolyFT3(prim, &a, &b, &c);
}

void EVL_renderQuadShard(EvlModelVertex *drift, int32_t unused1, int16_t speed, int16_t timer, ModelComponent *model)
{
	SVECTOR a;
	SVECTOR b;
	SVECTOR c;
	SVECTOR d;
	POLY_FT4 *prim;
	TMD_P_TG4 *tri;
	EvlModelVertex *v;
	int16_t dx;
	int16_t dy;
	int16_t dz;

	tri = (TMD_P_TG4 *)MAIN_D_80135210;
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->r0 = MAIN_D_80135218[0];
	prim->g0 = MAIN_D_80135218[1];
	prim->b0 = MAIN_D_80135218[2];
	prim->tpage = model->pixelPage;
	prim->clut = tri->clut;
	prim->u0 = tri->tu0;
	prim->v0 = tri->tv0;
	prim->u1 = tri->tu1;
	prim->v1 = tri->tv1;
	prim->u2 = tri->tu2;
	prim->v2 = tri->tv2;
	prim->u3 = tri->tu3;
	prim->v3 = tri->tv3;

	dx = drift->vx * timer / speed;
	dy = drift->vy * timer / speed;
	dz = drift->vz * timer / speed;

	v = &MAIN_D_80135214[tri->v0];
	a.vx = v->vx + dx;
	a.vy = v->vy + dy;
	a.vz = v->vz + dz;
	v = &MAIN_D_80135214[tri->v1];
	b.vx = v->vx + dx;
	b.vy = v->vy + dy;
	b.vz = v->vz + dz;
	v = &MAIN_D_80135214[tri->v2];
	c.vx = v->vx + dx;
	c.vy = v->vy + dy;
	c.vz = v->vz + dz;
	v = &MAIN_D_80135214[tri->v3];
	d.vx = v->vx + dx;
	d.vy = v->vy + dy;
	d.vz = v->vz + dz;
	setSemiTrans(prim, 1);
	add3DSpritePrim(prim, &a, &b, &c, &d);
}

void EVL_tickParticle(int32_t id)
{
	int32_t *p;

	p = &EVL_D_80068944[id].timer;
	if (*p >= 0x56) {
		removeObject(0x607, id);
		*p = -1;
	} else {
		*p += 1;
	}
}

INCLUDE_ASM("asm/evl/nonmatchings/evl", EVL_renderParticle);

void EVL_renderSparkStreak(int32_t id)
{
	MATRIX m;
	SVECTOR a;
	SVECTOR b;
	SVECTOR c;
	POLY_FT4 *prim;
	EvlSpark *e;
	int32_t lenSq;
	int32_t scale;

	e = &((EvlSpark *)EVL_D_80068F84)[id];
	calculateBoneMatrix(e->entity, e->bone, &m);
	a.vx = m.t[0];
	a.vy = m.t[1];
	a.vz = m.t[2];
	calculateBoneMatrix(e->entity, 1, &m);
	b.vx = a.vx - m.t[0];
	b.vy = a.vy - m.t[1];
	b.vz = a.vz - m.t[2];
	a.vx = m.t[0];
	a.vy = m.t[1];
	a.vz = m.t[2];
	lenSq = ((b.vx * b.vx) + (b.vy * b.vy)) + (b.vz * b.vz);
	scale = customRandom(0x190, 0x1f4);
	scale = scale * scale;
	b.vx = a.vx + ((b.vx * scale) / lenSq);
	b.vy = a.vy + ((b.vy * scale) / lenSq);
	b.vz = a.vz + ((b.vz * scale) / lenSq);
	c.vx = b.vx + customRandom(-0x50, 0x50);
	c.vy = b.vy + customRandom(-0x50, 0x50);
	c.vz = b.vz + customRandom(-0x50, 0x50);
	prim = (POLY_FT4 *)GsGetWorkBase();
	MAIN_func_80092B60(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = 0x3c;
	prim->clut = 0x7a4c;
	prim->u0 = 0x5f;
	prim->v0 = 0xa0;
	prim->u1 = 0x5f;
	prim->v1 = 0xa7;
	prim->u2 = 0x30;
	prim->v2 = 0xa0;
	prim->r0 = (rand() % 128) + 10;
	prim->g0 = prim->r0;
	prim->b0 = prim->r0;
	addScreenPolyFT3(prim, &c, &a, &b);
}

void EVL_tickSpark(int32_t id)
{
	int16_t *p;

	p = EVL_D_80068F84[id];
	if (p[1] < 0) {
		removeObject(0x605, id);
		p[0] = -1;
	} else {
		p[1] -= 1;
	}
}

void EVL_initEvoSequence(void)
{
	int16_t order[80];
	PartnerEntity *partner;
	int32_t i;
	int32_t j;
	int32_t bestVal;
	int32_t best;

	partner = EVO_SEQUENCE_DATA.partner;
	EVO_SEQUENCE_DATA.timer = 0;
	EVO_SEQUENCE_DATA.unk_0x8 = 0;
	EVO_SEQUENCE_DATA.state = 0;
	EVL_D_80064D40[0] = partner->digimonEntity.entity.posData->location.vx;
	EVL_D_80064D44[0] = partner->digimonEntity.entity.posData->location.vy;
	EVL_D_80064D48[0] = partner->digimonEntity.entity.posData->location.vz;
	MAIN_func_800D9B60(EVL_D_80063F3C);
	MAIN_func_800D9E68(EVL_D_80064D50);
	EVL_storeDigimonClut(EVL_D_80065094, (Entity *)partner);
	EVL_storeClutBank0(EVL_D_80065398);
	EVL_storeClutBank1(EVL_D_8006569C);
	EVL_initShardSets(EVL_D_80065FA0);
	EVL_setScratchTop((int32_t)EVL_D_80066274);
	initializeFlashData(EVL_D_800677B0);
	EVL_resetParticles();
	EVL_resetSparks();

	for (i = 0; i < 40; i++) {
		EVL_D_80067994[i] = -1;
	}

	EVL_D_80067992[DIGIMON_DATA[partner->digimonEntity.entity.type].boneCount] = -2;

	for (i = 1; i < DIGIMON_DATA[partner->digimonEntity.entity.type].boneCount; i++) {
		order[i] = rand() & 0xfff;
	}

	for (i = 1; i < DIGIMON_DATA[partner->digimonEntity.entity.type].boneCount; i++) {
		best = 1;
		bestVal = order[1];
		for (j = 2; j < DIGIMON_DATA[partner->digimonEntity.entity.type].boneCount; j++) {
			if (order[j] < bestVal) {
				best = j;
				bestVal = order[j];
			}
		}
		EVL_D_80067992[i] = best;
		order[best] = 0x1000;
	}

	addObject(0x80a, 0, EVL_tickEvoSequence, EVL_renderEvoSequence);
}

INCLUDE_ASM("asm/evl/nonmatchings/evl", EVL_applyEvolution);

void EVL_scaleBaseStats(Stats *stats, int16_t pct)
{
	if (HAS_USED_EVOITEM != 0) {
		return;
	}

	if (PARTNER_ENTITY.digimonEntity.entity.type != 0x27) {
		stats->base.hp = stats->base.hp * pct / 10;
		stats->base.mp = stats->base.mp * pct / 10;
		stats->base.off = stats->base.off * pct / 10;
		stats->base.def = stats->base.def * pct / 10;
		stats->base.speed = stats->base.speed * pct / 10;
		stats->base.brain = stats->base.brain * pct / 10;
	} else {
		stats->base.hp = PARTNER_PARA.sukaBackupHP + ((stats->base.hp - (PARTNER_PARA.sukaBackupHP / 2)) / 2);
		stats->base.mp = PARTNER_PARA.sukaBackupMP + ((stats->base.mp - (PARTNER_PARA.sukaBackupMP / 2)) / 2);
		stats->base.off = PARTNER_PARA.sukaBackupOff + ((stats->base.off - (PARTNER_PARA.sukaBackupOff / 2)) / 2);
		stats->base.def = PARTNER_PARA.sukaBackupDef + ((stats->base.def - (PARTNER_PARA.sukaBackupDef / 2)) / 2);
		stats->base.speed = PARTNER_PARA.sukaBackupSpeed + ((stats->base.speed - (PARTNER_PARA.sukaBackupSpeed / 2)) / 2);
		stats->base.brain = PARTNER_PARA.sukaBackupBrain + ((stats->base.brain - (PARTNER_PARA.sukaBackupBrain / 2)) / 2);
		PARTNER_PARA.virusBar = 0;
	}

	EVL_clampBaseStats();
	if (stats->current.currentHP > stats->base.hp) {
		stats->current.currentHP = stats->base.hp;
	}

	if (stats->current.currentMP > stats->base.mp) {
		stats->current.currentMP = stats->base.mp;
	}
}

void EVL_clampBaseStats(void)
{
	BaseStats *stats;

	stats = &PARTNER_ENTITY.digimonEntity.stats.base;
	if (stats->hp >= 10000) {
		stats->hp = 9999;
	}

	if (stats->mp >= 10000) {
		stats->mp = 9999;
	}

	if (stats->off >= 1000) {
		stats->off = 999;
	}

	if (stats->def >= 1000) {
		stats->def = 999;
	}

	if (stats->speed >= 1000) {
		stats->speed = 999;
	}

	if (stats->brain >= 1000) {
		stats->brain = 999;
	}
}
