#include <stdlib.h>

#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/efe.h>
#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/graphics.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

typedef struct {
	int16_t timer;
	int16_t primCount;
	int32_t centers;
	int32_t *model;
	int32_t vertices;
	int32_t primitives;
	int16_t centerCount;
	int16_t bone;
} EvlShardSet;

typedef struct {
	int16_t vx;
	int16_t vy;
	int16_t vz;
} EvlModelVertex;

extern int32_t MAIN_D_8013520C;
extern uint8_t *MAIN_D_80135210;
extern EvlModelVertex *MAIN_D_80135214;
extern int16_t MAIN_D_80135218[3];
extern int8_t HAS_USED_EVOITEM;
extern char *MAIN_D_80135208;
extern int8_t EVL_D_80068938[];
extern EvlParticle EVL_D_80068944[100];
extern int16_t EVL_D_80063F3C[];
extern VECTOR EVL_D_80064D40;
extern char EVL_D_80064D50[];
extern uint16_t EVL_D_80065094[];
extern u_long EVL_D_80065398[];
extern u_long EVL_D_8006569C[];
extern char EVL_D_80065FA0[];
extern char EVL_D_80066274[];
extern EfeFlashData EVL_D_800677B0[12];
extern int16_t EVL_D_80067990[42];
extern GsRVIEW2 GS_VIEWPOINT;
extern GsRVIEW2 EVL_D_800688E8;
extern int32_t VIEWPORT_DISTANCE;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern int32_t MAIN_D_801351E4;
extern int32_t MAIN_D_801351E8;
extern int32_t MAIN_D_801351EC;
extern int32_t MAIN_D_801351F8;
extern SVECTOR MAIN_D_801349F8;
extern SVECTOR MAIN_D_80134A00;

void initializeEvolvedPartner(int32_t type, int32_t posX, int32_t posY, int32_t posZ,
                              int32_t rotationX, int32_t rotationY, int32_t rotationZ);
void MAIN_func_800D9B60(int16_t *clut);
void MAIN_func_800D9E68(char *base);
char *initializeFlashData(char *base);
int32_t customRandom(int32_t a, int32_t b);
void MAIN_func_80092B60(POLY_FT4 *prim);
void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
int32_t add3DSpritePrim(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
void addTamerLevel(int32_t chance, int32_t amount);
void learnMove(int32_t moveId);
void EVL_brightenDigimonClut(int16_t *clut, Entity *entity, int16_t *dst, int32_t start,
                             int32_t end, int32_t t);
void setDigimonRaised(int32_t type);
int32_t lerp(int32_t start, int32_t end, int32_t tMin, int32_t tMax, int32_t t);
int32_t worldPosToScreenPos(SVECTOR *worldPos, DVECTOR *screenPos);
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
char *EVL_initShardSets(char *base);
void EVL_tickEvoSequence(int32_t instanceId);
void EVL_fadeClutBank0(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame);
void EVL_fadeClutBank1(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame);
void EVL_updateEvoCamera(Entity *entity, int32_t unused, int32_t frame);
int32_t EVL_buildShardSet(Entity *entity, int32_t objIndex, int32_t bone);
int32_t EVL_spawnSpark(void *owner, int32_t timer, int32_t param);
void EVL_calculateCameraVectors(VECTOR *viewRef, VECTOR *viewPos, Entity *entity, SVECTOR *rotation, int32_t distance, int32_t height);
void EVL_tickShardSet(int32_t id);
void EVL_renderShardSet(int32_t index);
void EVL_renderTriShard(EvlModelVertex *drift, int32_t unused1, int16_t speed, int16_t timer, ModelComponent *model);
void EVL_renderQuadShard(EvlModelVertex *drift, int32_t unused1, int16_t speed, int16_t timer, ModelComponent *model);
void EVL_renderSparkStreak(int32_t id);
void EVL_tickSpark(int32_t id);
void EVL_applyEvolution(Entity *entity, Stats *stats, PartnerPara *para, int32_t digimonId);
void EVL_scaleBaseStats(Stats *stats, int16_t pct, int32_t unused);
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

// clang-format off
int8_t EVL_D_80063EFC[44] = {
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x02, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x03, 0xff, 0xff, 0xff, 0x04, 0xff,
	0x05, 0xff, 0x06, 0x00,
};

int8_t EVL_D_80063F28[20] = {
	0x01, 0x02, 0x03, 0x04, 0x05, 0x04, 0x03, 0x02,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};
// clang-format on

void EVL_storeDigimonClut(uint16_t *buffer, Entity *entity)
{
	ModelComponent *model;
	RECT rect;

	model = getEntityModelComponent(entity->type, getEntityType(entity));
	setRECT(&rect, (model->clutPage & 0x3f) << 4, model->clutPage >> 6, 0x10, 0x18);
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
		EVL_D_80068F84[i].bone = -1;
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

	setRECT(&rect, 0, 488, 16, 24);
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

	setRECT(&rect, 32, 488, 48, 24);
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

void EVL_updateEvoCamera(Entity *entity, int32_t unused, int32_t frame)
{
	int32_t size;
	VECTOR viewRef;
	VECTOR viewPos;
	SVECTOR pos;
	DVECTOR screen;
	SVECTOR rot;
	SVECTOR rot2;
	int32_t t;

	if (frame >= 0x11c) {
		return;
	}

	if ((frame >= 0x20) && (frame < 0x61)) {
		pos.vx = entity->posData->location.vx;
		pos.vy = entity->posData->location.vy;
		pos.vz = entity->posData->location.vz;
		worldPosToScreenPos(&pos, &screen);
		MAIN_D_801351F8 = 0x638;
		rot = MAIN_D_801349F8;
		rot.vy = entity->posData->rotation.vy + 0x638;
		size = (DIGIMON_DATA[entity->type].height < DIGIMON_DATA[entity->type].radius) ? (int32_t)DIGIMON_DATA[entity->type].radius : (int32_t)DIGIMON_DATA[entity->type].height;
		size = (size * 5) + 0x4b0;
		EVL_calculateCameraVectors(&viewRef, &viewPos, entity, &rot, size, DIGIMON_DATA[entity->type].height);
		if (frame < 0x41) {
			GS_VIEWPOINT.vrx = lerp(EVL_D_800688E8.vrx, viewRef.vx, 0x20, 0x40, frame);
			GS_VIEWPOINT.vry = lerp(EVL_D_800688E8.vry, viewRef.vy, 0x20, 0x40, frame);
			GS_VIEWPOINT.vrz = lerp(EVL_D_800688E8.vrz, viewRef.vz, 0x20, 0x40, frame);
			GS_VIEWPOINT.rz = 0;
			DRAWING_OFFSET_X = lerp(MAIN_D_801351E4, 0xa0, 0x20, 0x40, frame);
			DRAWING_OFFSET_Y = lerp(MAIN_D_801351E8, 0x78, 0x20, 0x40, frame);
		} else if (frame < 0x61) {
			GS_VIEWPOINT.vpx = lerp(EVL_D_800688E8.vpx, viewPos.vx, 0x40, 0x60, frame);
			GS_VIEWPOINT.vpy = lerp(EVL_D_800688E8.vpy, viewPos.vy, 0x40, 0x60, frame);
			GS_VIEWPOINT.vpz = lerp(EVL_D_800688E8.vpz, viewPos.vz, 0x40, 0x60, frame);
			t = lerp(0, 0x14, 0, 0xc8, DIGIMON_DATA[entity->type].height);
			DRAWING_OFFSET_Y = lerp(0x78, t + 0x78, 0x40, 0x60, frame);
			VIEWPORT_DISTANCE = lerp(MAIN_D_801351EC, 0x3e8, 0x40, 0x60, frame);
		}
	}

	if (frame >= 0x61) {
		MAIN_D_801351F8 += 0x16;
		rot2 = MAIN_D_80134A00;
		rot2.vy = MAIN_D_801351F8 + entity->posData->rotation.vy;
		if (DIGIMON_DATA[entity->type].height < DIGIMON_DATA[entity->type].radius) {
			size = DIGIMON_DATA[entity->type].radius;
		} else {
			size = DIGIMON_DATA[entity->type].height;
		}
		size = (size * 5) + 0x4b0;
		EVL_calculateCameraVectors(&viewRef, &viewPos, entity, &rot2, size, DIGIMON_DATA[entity->type].height);
		GS_VIEWPOINT.vrx = viewRef.vx;
		GS_VIEWPOINT.vry = viewRef.vy;
		GS_VIEWPOINT.vrz = viewRef.vz;
		GS_VIEWPOINT.vpx = viewPos.vx;
		GS_VIEWPOINT.vpy = viewPos.vy;
		GS_VIEWPOINT.vpz = viewPos.vz;
	}
}

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
	copyVector(&e->pos, position);
	e->r = color->r;
	e->g = color->g;
	e->b = color->b;
	addObject(0x607, i, EVL_tickParticle, EVL_renderParticle);

	return i;
}

int32_t EVL_spawnSpark(void *owner, int32_t timer, int32_t param)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 16; i++) {
		if (EVL_D_80068F84[i].bone == -1) {
			break;
		}
	}

	if (i == 16) {
		return -1;
	}

	p = (int16_t *)&EVL_D_80068F84[i];
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
	setRGB0(prim, MAIN_D_80135218[0], MAIN_D_80135218[1], MAIN_D_80135218[2]);
	prim->tpage = model->pixelPage;
	prim->clut = tri->clut;
	setUV3(prim, tri->tu0, tri->tv0, tri->tu1, tri->tv1, tri->tu2, tri->tv2);

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

void EVL_brightenDigimonClut(int16_t *clut, Entity *entity, int16_t *dst,
                             int32_t start, int32_t end, int32_t t)
{
	ModelComponent *model;
	RECT rect;
	int16_t redFactor;
	int16_t greenFactor;
	int16_t blueFactor;
	int16_t red;
	int16_t green;
	int16_t blue;
	int16_t stp;
	int32_t amount;
	int32_t i;
	int32_t color;
	int32_t channels;

	model = getEntityModelComponent(entity->type, getEntityType(entity));
	if (end < t) {
		t = end;
	}

	redFactor = rand() % 100;
	greenFactor = rand() % 100;
	blueFactor = rand() % 100;
	i = 0;

	while (i < 0x180) {
		red = *clut & 0x1f;
		color = *clut;
		channels = color;
		green = (channels >> 5) & 0x1f;
		blue = (channels >> 10) & 0x1f;
		stp = (*clut++ >> 15) & 1;
		if (red || green || blue) {
			if (t != start) {
				stp = 1;
			}

			color = ((0x1f - red) * (t - start)) / (end - start);
			red += (redFactor * color) / 100;
			amount = ((0x1f - green) * (t - start)) / (end - start);
			green += (greenFactor * amount) / 100;
			amount = ((0x1f - blue) * (t - start)) / (end - start);
			blue += (blueFactor * amount) / 100;
		}

		dst[i] = red;
		dst[i] += green << 5;
		dst[i] += blue << 10;
		dst[i] += stp << 15;
		i++;
	}

	setRECT(&rect, (model->clutPage & 0x3f) << 4,
	        model->clutPage >> 6, 0x10, 0x18);
	LoadImage(&rect, (u_long *)dst);
}

int32_t EVL_buildShardSet(Entity *entity, int32_t objIndex, int32_t bone)
{
	SVECTOR tmp;
	MATRIX m1;
	MATRIX m2;
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
	ModelComponent *model;
	struct TMD_STRUCT *obj;
	EvlShardSet *entry;
	SVECTOR *va;
	SVECTOR *vb;
	SVECTOR *vc;
	SVECTOR *vd;
	int32_t out;
	int32_t outStart;
	SVECTOR *src;
	int32_t prim;
	int32_t slot;
	int32_t i;
	int32_t j;
	int16_t cx;
	int16_t cy;
	int16_t cz;

	out = MAIN_D_8013520C;
	model = getEntityModelComponent(entity->type, 3);
	obj = &((struct TMD_STRUCT *)((uint32_t)model->modelPtr + 12))[objIndex];

	for (slot = 0; slot < 30; slot++) {
		if (((EvlShardSet *)MAIN_D_80135208)[slot].timer < 0) {
			break;
		}
	}
	if (slot == 30) {
		return -1;
	}

	calculateBoneMatrix(entity, bone, &m1);
	src = (SVECTOR *)obj->vertop;
	outStart = out;
	calculateBoneMatrix(entity, bone, &m2);
	for (i = 0; (uint32_t)i < obj->vern; i++) {
		ApplyMatrixSV(&m2, src++, &tmp);
		((EvlModelVertex *)out)->vx = tmp.vx + m2.t[0];
		((EvlModelVertex *)out)->vy = tmp.vy + m2.t[1];
		((EvlModelVertex *)out)->vz = tmp.vz + m2.t[2];
		out += sizeof(EvlModelVertex);
	}

	entry = &((EvlShardSet *)MAIN_D_80135208)[slot];
	entry->timer = 0;
	entry->primCount = obj->primn;
	entry->centers = out;
	entry->model = (int32_t *)entity;
	entry->bone = bone;
	entry->vertices = outStart;
	entry->primitives = (int32_t)obj->primtop;
	entry->centerCount = obj->primn;
	addObject(0x604, slot, EVL_tickShardSet, EVL_renderShardSet);

	prim = (int32_t)obj->primtop;
	for (j = 0; (uint32_t)j < obj->primn; j++) {
		switch (((int8_t *)prim)[3]) {
		case 0x34:
		case 0x36:
			va = &((SVECTOR *)obj->nortop)[((TMD_P_TG3 *)prim)->n0];
			vb = &((SVECTOR *)obj->nortop)[((TMD_P_TG3 *)prim)->n1];
			vc = &((SVECTOR *)obj->nortop)[((TMD_P_TG3 *)prim)->n2];
			ApplyMatrixSV(&m1, va, &p0);
			ApplyMatrixSV(&m1, vb, &p1);
			ApplyMatrixSV(&m1, vc, &p2);
			cx = (p0.vx + p1.vx + p2.vx) / 3;
			cy = (p0.vy + p1.vy + p2.vy) / 3;
			cz = (p0.vz + p1.vz + p2.vz) / 3;
			((EvlModelVertex *)out)->vx = cx;
			((EvlModelVertex *)out)->vy = cy;
			((EvlModelVertex *)out)->vz = cz;
			out += sizeof(EvlModelVertex);
			prim += sizeof(TMD_P_TG3);
			break;
		case 0x3c:
		case 0x3e:
			va = &((SVECTOR *)obj->nortop)[((TMD_P_TG4 *)prim)->n0];
			vb = &((SVECTOR *)obj->nortop)[((TMD_P_TG4 *)prim)->n1];
			vc = &((SVECTOR *)obj->nortop)[((TMD_P_TG4 *)prim)->n2];
			vd = &((SVECTOR *)obj->nortop)[((TMD_P_TG4 *)prim)->n3];
			ApplyMatrixSV(&m1, va, &p0);
			ApplyMatrixSV(&m1, vb, &p1);
			ApplyMatrixSV(&m1, vc, &p2);
			ApplyMatrixSV(&m1, vd, &p3);
			cx = (p0.vx + p1.vx + p2.vx) / 3;
			cy = (p0.vy + p1.vy + p2.vy) / 3;
			cz = (p0.vz + p1.vz + p2.vz) / 3;
			((EvlModelVertex *)out)->vx = cx;
			((EvlModelVertex *)out)->vy = cy;
			((EvlModelVertex *)out)->vz = cz;
			out += sizeof(EvlModelVertex);
			prim += sizeof(TMD_P_TG4);
			break;
		}
	}
	(void)cx;
	(void)cy;
	(void)cz;

	MAIN_D_8013520C = out;
	return slot;
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
	setRGB0(prim, MAIN_D_80135218[0], MAIN_D_80135218[1], MAIN_D_80135218[2]);
	prim->tpage = model->pixelPage;
	prim->clut = tri->clut;
	setUV4(prim, tri->tu0, tri->tv0, tri->tu1, tri->tv1, tri->tu2, tri->tv2, tri->tu3,
	       tri->tv3);

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

	e = &EVL_D_80068F84[id];
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
	prim->tpage = getTPage(0, 1, 768, 256);
	prim->clut = getClut(192, 489);
	setUV3(prim, 0x5f, 0xa0, 0x5f, 0xa7, 0x30, 0xa0);
	setRGB0(prim, (rand() % 128) + 10, prim->r0, prim->r0);
	addScreenPolyFT3(prim, &c, &a, &b);
}

void EVL_tickSpark(int32_t id)
{
	int16_t *p;

	p = (int16_t *)&EVL_D_80068F84[id];
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
	copyVector(&EVL_D_80064D40, &partner->digimonEntity.entity.posData->location);
	MAIN_func_800D9B60(EVL_D_80063F3C);
	MAIN_func_800D9E68(EVL_D_80064D50);
	EVL_storeDigimonClut(EVL_D_80065094, (Entity *)partner);
	EVL_storeClutBank0(EVL_D_80065398);
	EVL_storeClutBank1(EVL_D_8006569C);
	EVL_initShardSets(EVL_D_80065FA0);
	EVL_setScratchTop((int32_t)EVL_D_80066274);
	initializeFlashData((char *)EVL_D_800677B0);
	EVL_resetParticles();
	EVL_resetSparks();

	for (i = 0; i < 40; i++) {
		(EVL_D_80067990 + 2)[i] = -1;
	}

	(EVL_D_80067990 + 1)[DIGIMON_DATA[partner->digimonEntity.entity.type].boneCount] = -2;

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
		(EVL_D_80067990 + 1)[i] = best;
		order[best] = 0x1000;
	}

	addObject(0x80a, 0, EVL_tickEvoSequence, (RenderFunction)EVL_renderEvoSequence);
}

// clang-format off
void EVL_applyEvolution(entity, stats, para, digimonId)
	Entity *entity;
	Stats *stats;
	PartnerPara *para;
	int16_t digimonId;
// clang-format on
{
	EvoStatsGains *gains;
	int32_t newId;
	int16_t targetLevel;
	int32_t oldType;
	long currentType;
	int32_t moveBase;
	uint8_t special;
	uint8_t *moves;
	uint8_t *movePtr;
	uint8_t candidates[16];
	uint8_t best;
	int32_t move;
	int32_t i;
	int32_t count;
	PositionData *position;
	int16_t x, y, z;
	int16_t rx, ry, rz;
	int16_t oldLevel;

	gains = &EVO_GAINS_DATA[digimonId];
	currentType = PARTNER_ENTITY.digimonEntity.entity.type;
	oldLevel = DIGIMON_DATA[currentType].level;
	newId = gains->targetDigimon;
	targetLevel = DIGIMON_DATA[digimonId].level;
	if (digimonId == 0x0b || digimonId == 0x27 || digimonId == 0x35 || digimonId == 6 ||
	    currentType == 0x27 || targetLevel < 3) {
		EVL_scaleBaseStats(stats, (int8_t)gains->brains, digimonId);
		newId = gains->targetDigimon;
	} else {
		if (HAS_USED_EVOITEM == 0) {
			if (stats->base.hp >= gains->hp) {
				stats->base.hp += (int16_t)(gains->hp / 10);
			} else {
				stats->base.hp = (gains->hp + stats->base.hp) / 2;
			}

			if (stats->base.mp >= gains->mp) {
				stats->base.mp += (int16_t)(gains->mp / 10);
			} else {
				stats->base.mp = (gains->mp + stats->base.mp) / 2;
			}

			if (stats->base.off >= gains->offense) {
				stats->base.off += (int16_t)(gains->offense / 10);
			} else {
				stats->base.off = (gains->offense + stats->base.off) / 2;
			}

			if (stats->base.def >= gains->defense) {
				stats->base.def += (int16_t)(gains->defense / 10);
			} else {
				stats->base.def = (gains->defense + stats->base.def) / 2;
			}

			if (stats->base.speed >= gains->speed) {
				stats->base.speed += (int16_t)(gains->speed / 10);
			} else {
				stats->base.speed = (gains->speed + stats->base.speed) / 2;
			}

			if (stats->base.brain >= gains->brains) {
				stats->base.brain += (int16_t)(gains->brains / 10);
			} else {
				stats->base.brain = (gains->brains + stats->base.brain) / 2;
			}

			EVL_clampBaseStats();
			if (IS_SCRIPT_PAUSED == 1) {
				if (DIGIMON_DATA[newId].level == 4) {
					addTamerLevel(20, 1);
				}
				if (DIGIMON_DATA[newId].level == 5) {
					addTamerLevel(100, 1);
				}
			}
		}
		newId = gains->targetDigimon;
	}
	para->weight = RAISE_DATA[newId].defaultWeight;
	para->careMistakes = 0;
	para->battles = 0;
	if (DIGIMON_DATA[newId].level == 5 && HAS_USED_EVOITEM == 0) {
		para->remainingLifetime += 96;
	}
	special = DIGIMON_DATA[newId].special[0];
	if (special == 0) {
		moveBase = 0;
	} else if (special == 1) {
		moveBase = 40;
	} else if (special == 2) {
		moveBase = 8;
	} else if (special == 3) {
		moveBase = 32;
	} else if (special == 4) {
		moveBase = 16;
	} else if (special == 5) {
		moveBase = 24;
	} else if (special == 6) {
		moveBase = 49;
	}
	movePtr = DIGIMON_DATA[newId].moves;
	moves = movePtr;
	count = 0;
	for (i = 0; i < 16; movePtr++, i++) {
		if ((uint32_t)*movePtr >= (uint32_t)moveBase && *movePtr <= moveBase + 8) {
			candidates[count] = *movePtr;
			count++;
		}
	}
	candidates[count] = 0xff;
	best = candidates[0];
	for (count = 0; candidates[count] != 0xff; count++) {
		move = candidates[count];
		if (MOVE_DATA[best].power > MOVE_DATA[move].power && MOVE_DATA[move].power != 0) {
			best = move;
		}
	}
	learnMove(best);
	for (movePtr = moves, i = 0; i < 16; i++) {
		if (*movePtr++ == best) {
			break;
		}
	}
	PARTNER_ENTITY.digimonEntity.stats.base.moves[0] = i + 0x2e;
	PARTNER_ENTITY.digimonEntity.stats.base.moves[1] = 0xff;
	PARTNER_ENTITY.digimonEntity.stats.base.moves[2] = 0xff;
	if (DIGIMON_DATA[newId].level < 3) {
		PARTNER_ENTITY.digimonEntity.stats.base.moves[0] = 0x2e;
	}
	for (i = 15; i >= 0; i--) {
		if (DIGIMON_DATA[newId].moves[i] == 0xff) {
			continue;
		}
		if (DIGIMON_DATA[newId].moves[i] < 0x3a) {
			continue;
		}
		if (DIGIMON_DATA[newId].moves[i] >= 0x71) {
			continue;
		}
		PARTNER_ENTITY.digimonEntity.stats.base.moves[3] = i + 0x2e;
		break;
	}
	if (i < 0) {
		PARTNER_ENTITY.digimonEntity.stats.base.moves[3] = 0xff;
	}
	position = entity->posData;
	x = position->location.vx;
	y = position->location.vy;
	z = position->location.vz;
	rx = position->rotation.vx;
	ry = position->rotation.vy;
	rz = position->rotation.vz;
	oldType = PARTNER_ENTITY.digimonEntity.entity.type;
	removeEntity(oldType, 1);
	ENTITY_TABLE[1] = NULL;
	thunkUnloadModel(oldType, 3);
	initializeEvolvedPartner(newId, x, y, z, rx, ry, rz);
	setDigimonRaised((uint16_t)newId);
	if (oldLevel != DIGIMON_DATA[newId].level) {
		para->evoTimer = 0;
	}
}

void EVL_scaleBaseStats(Stats *stats, int16_t pct, int32_t unused)
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
