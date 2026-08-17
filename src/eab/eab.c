#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/btl.h>
#include <dw/eab.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/math.h>
#include <dw/sound.h>
#include <dw/types.h>

#include "common.h"

typedef struct {
	int16_t frame;
	int16_t phase;
	Entity *entity;
} EabHudState;

typedef struct {
	int16_t timer;
	int16_t pad;
	Entity *entity;
} EabModelFX;

typedef struct {
	int32_t timer;
	SVECTOR pos;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t pad;
} EabParticle;

typedef struct {
	int16_t frame;
	int16_t phase;
	Entity *entity;
	VECTOR location;
} EabState;

extern VECTOR EAB_D_800616DC;
extern VECTOR EAB_D_800616EC;
extern VECTOR EAB_D_800616FC;
extern int32_t EAB_D_8006170C[9][4];
extern int8_t EAB_D_8006179C[18];
extern VECTOR EAB_D_800617B0;
extern VECTOR EAB_D_800617C0;
extern int16_t EAB_D_800617D0[18][4];
extern EabState EAB_D_800617E8;
extern char EAB_D_80061800[];
extern GsRVIEW2 EAB_D_800619E4;
extern int8_t EAB_D_80061A04[10];
extern int32_t EAB_D_80061A10[100][4];

extern char *MAIN_D_80134C28;
extern SVECTOR MAIN_D_80134C2C;
extern SVECTOR MAIN_D_80134C34;
extern RGB8 MAIN_D_80134C3C;
extern SVECTOR MAIN_D_80134C40;
extern uint8_t MAIN_D_80134C48;
extern EabHudState MAIN_D_801353F0;
extern int32_t ENTITY1_WIREFRAME_TOTAL;
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t *EFE_DATA_STACK;

int32_t addObject(int32_t objectId, int32_t instanceId, void *tick, void *render);
void removeObject(int32_t objectId, int32_t instanceId);
void setMapLayerEnabled(int32_t enabled);
int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t);
int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
void renderTMDModel(uint8_t *buffer, int32_t id, GsCOORDINATE2 *coord, GsCOORDINATE2 *super, VECTOR *trans, SVECTOR *rot, VECTOR *scale);
char *initializeFlashData(char *base);
void createFlash(void);

void EAB_setModelColor(int32_t *color);
void EAB_tickBuildup(void);
void EAB_renderBuildup(void);
void EAB_initializeRings(void);
int32_t EAB_addBuildupRing(Entity *entity);
void EAB_tickBuildupRing(int32_t id);
void EAB_renderBuildupRing(int32_t id);
int32_t EAB_addSpawnRing(Entity *entity);
void EAB_tickSpawnRing(int32_t id);
void EAB_renderSpawnRing(int32_t id);
void EAB_removeRings(void);
void EAB_renderSpawn(void);
void EAB_tickSpawn(int32_t instanceId);
void EAB_initializeParticles(void);
void EAB_renderFlash(VECTOR *color);
void EAB_setEntitiesVisible(int32_t enabled);
int32_t EAB_addParticle(VECTOR *position, RGB8 *color);
void EAB_renderBackdrop(VECTOR *color);
void EAB_calculateCameraOrbit(VECTOR *viewRef, VECTOR *viewPos, Entity *entity,
                              SVECTOR *rotation, int32_t distance, int32_t height);
void EAB_removeParticles(void);
void EAB_tickParticle(int32_t id);
void EAB_renderParticle(int32_t id);

static void *eab_functions[] = {
	EAB_tick,
	EAB_startBuildup,
	EAB_renderParticle,
	EAB_tickParticle,
	EAB_removeParticles,
	EAB_calculateCameraOrbit,
	EAB_renderBackdrop,
	EAB_addParticle,
	EAB_setEntitiesVisible,
	EAB_renderFlash,
	EAB_initializeParticles,
	EAB_tickSpawn,
	EAB_renderSpawn,
	EAB_removeRings,
	EAB_renderSpawnRing,
	EAB_tickSpawnRing,
	EAB_addSpawnRing,
	EAB_renderBuildupRing,
	EAB_tickBuildupRing,
	EAB_addBuildupRing,
	EAB_initializeRings,
	EAB_renderBuildup,
	EAB_tickBuildup,
	EAB_setModelColor,
};

void EAB_setModelColor(int32_t *color)
{
	int32_t *hdr;
	int32_t *rec;
	int32_t i;
	int32_t count;
	int32_t t;
	int32_t idx;

	char (*pr)[0x20];
	char (*pg)[0x20];
	char (*pb)[0x20];

	idx = 0;
	hdr = (int32_t *)((int32_t)((uint32_t)MAIN_D_80134C28 + 0xc) + (idx * 28));
	rec = (int32_t *)hdr[4];
	count = hdr[5];
	pr = (char (*)[0x20])((char *)rec + 0x14);
	pg = (char (*)[0x20])((char *)rec + 0x15);
	pb = (char (*)[0x20])((char *)rec + 0x16);
	for (i = 0; i < count; i++) {
		t = (*rec >> 24) & 0xff;
		if ((t == 0x2f) || (t == 0x2d)) {
			(*pr)[0] = (int16_t)color[0];
			(*pg)[0] = (int16_t)color[1];
			(*pb)[0] = (int16_t)color[2];
			rec = (int32_t *)((int32_t)rec + 0x20);
			pr++;
			pg++;
			pb++;
		}
	}
}

void EAB_tickBuildup(void)
{
	Entity *entity;

	entity = MAIN_D_801353F0.entity;
	MAIN_D_801353F0.frame += 1;
	MAIN_D_801353F0.frame %= 18;
	if ((MAIN_D_801353F0.frame % 6) == 0) {
		EAB_addBuildupRing(entity);
	}
}

void EAB_renderBuildup(void)
{
}

void EAB_initializeRings(void)
{
	int32_t i;

	for (i = 0; i < 18; i++) {
		EAB_D_800617D0[i][0] = -1;
	}
}

static void eab__garbage__(void)
{
	int32_t v0;
	int32_t v1;
	int32_t v2;
	int32_t v3;

	v0 = EAB_D_800617D0[0][0] + 0;
	v1 = EAB_D_800617D0[1][0] + 1;
	v2 = EAB_D_800617D0[2][0] + 2;
	v3 = EAB_D_800617D0[0][0] + 3;
	EAB_D_800617D0[0][0] = (int16_t)((v0 * v1) + v2);
	EAB_D_800617D0[1][0] = (int16_t)((v1 * v2) + v3);
	EAB_D_800617D0[2][0] = (int16_t)((v2 * v3) + v0);
	EAB_D_800617D0[0][0] = (int16_t)((v3 * v0) + v1);
}

int32_t EAB_addBuildupRing(Entity *entity)
{
	EabModelFX *fx;
	int32_t i;

	for (i = 0; i < 3; i++) {
		if (((EabModelFX *)EAB_D_800617D0)[i].timer < 0) {
			break;
		}
	}

	if (i == 3) {
		return -1;
	}

	fx = &((EabModelFX *)EAB_D_800617D0)[i];
	fx->timer = 0;
	fx->entity = entity;

	return addObject(0x60d, i, EAB_tickBuildupRing, EAB_renderBuildupRing);
}

void EAB_tickBuildupRing(int32_t id)
{
	int16_t *p;

	p = &EAB_D_800617D0[id][0];
	*p += 1;
	if (*p >= 0x12) {
		*p = -1;
		removeObject(0x60d, id);
	}
}

void EAB_renderBuildupRing(int32_t id)
{
	GsCOORDINATE2 coord;
	VECTOR trans;
	SVECTOR rot;
	VECTOR scale;
	int16_t *p;
	Entity *entity;

	p = &EAB_D_800617D0[id][0];
	entity = ((Entity **)p)[1];
	rot = MAIN_D_80134C2C;
	scale = EAB_D_800616EC;
	scale.vx = scale.vz = lerp(0x10b8, 0x614, 1, 0x12, p[0]);
	scale.vy = ((_sin(lerp(0, 0x80, 1, 0x12, p[0])) * 0xc3c) / 4096) + 0x15c;
	trans.vx = entity->posData->location.vx;
	trans.vy = entity->posData->location.vy;
	trans.vz = entity->posData->location.vz;
	renderTMDModel((uint8_t *)MAIN_D_80134C28, 0, &coord, NULL, &trans, &rot, &scale);
}

int32_t EAB_addSpawnRing(Entity *entity)
{
	EabModelFX *fx;
	int32_t i;

	for (i = 0; i < 3; i++) {
		if (((EabModelFX *)EAB_D_800617D0)[i].timer < 0) {
			break;
		}
	}

	if (i == 3) {
		return -1;
	}

	fx = &((EabModelFX *)EAB_D_800617D0)[i];
	fx->timer = 0;
	fx->entity = entity;

	return addObject(0x60d, i, EAB_tickSpawnRing, EAB_renderSpawnRing);
}

void EAB_tickSpawnRing(int32_t id)
{
	int16_t *p;

	p = &EAB_D_800617D0[id][0];
	*p += 1;
	if (*p >= 9) {
		*p = -1;
		removeObject(0x60d, id);
	}
}

void EAB_renderSpawnRing(int32_t id)
{
	GsCOORDINATE2 coord;
	VECTOR trans;
	SVECTOR rot;
	VECTOR scale;
	int16_t *p;
	Entity *entity;

	p = &EAB_D_800617D0[id][0];
	entity = ((Entity **)p)[1];
	rot = MAIN_D_80134C34;
	scale = EAB_D_800616FC;
	scale.vx = scale.vz = lerp(0x16cc, 0x10b8, 1, 0x12, p[0]);
	scale.vy = ((_sin(lerp(0, 0x80, 1, 0x12, p[0])) * 0x2b8) / 4096) + 0xae;
	trans.vx = entity->posData->location.vx;
	trans.vy = entity->posData->location.vy;
	trans.vz = entity->posData->location.vz;
	renderTMDModel((uint8_t *)MAIN_D_80134C28, 0, &coord, NULL, &trans, &rot, &scale);
}

void EAB_removeRings(void)
{
	int32_t i;

	for (i = 0; i < 3; i++) {
		if (EAB_D_800617D0[i][0] >= 0) {
			EAB_D_800617D0[i][0] = -1;
			removeObject(0x60d, i);
		}
	}
}

void EAB_renderSpawn(void)
{
}

void EAB_tickSpawn(int32_t instanceId)
{
	VECTOR flashColor;
	VECTOR fadeColor;
	RGB8 particleColor;
	VECTOR overlayColor;
	VECTOR viewRef;
	VECTOR viewPos;
	SVECTOR rotation;
	Entity *entity;
	EabState *state;

	state = &EAB_D_800617E8;
	entity = state->entity;
	state->frame++;

	if ((entity->anim.animId == 0x33) &&
	    (entity->anim.animFrame == entity->anim.frameCount)) {
		startAnimation(entity, 0x33);
	}

	switch (state->phase) {
	case 0:
		if (state->frame >= 0x2d) {
			flashColor = EAB_D_800617B0;
			EAB_renderFlash(&flashColor);
		}
		if (state->frame < 0x32) {
			break;
		}
		state->phase = 1;
		entity->isOnMap = 1;
		startAnimation(entity, 0x21);
		EAB_setEntitiesVisible(0);
		setMapLayerEnabled(0);
		EAB_D_800619E4 = GS_VIEWPOINT;
		removeObject(0x60b, 0);
		stopSound();
		playSound2(8, 2);
		((DigimonEntity *)entity)->stats.current.vabId = 5;
		/* fall through */
	case 1:
		EAB_addSpawnRing(entity);
		if (state->frame == 0xd2) {
			startAnimation(entity, 0x33);
		}
		if ((state->frame >= 0x32) && (state->frame < 0x53)) {
			fadeColor.vx = lerp(0xff, 0, 0x32, 0x52, state->frame);
			fadeColor.vy = fadeColor.vz = fadeColor.vx;
			EAB_renderFlash(&fadeColor);
		}
		if (EAB_D_8006179C[state->frame % 18] != 0) {
			particleColor = MAIN_D_80134C3C;
			particleColor.g = lerp(0x32, 0xe6, 1, 5,
			                       EAB_D_8006179C[state->frame % 18]);
			particleColor.g = particleColor.g *
			                  _sin(lerp(0, 0x80, 0x32, 0x122, state->frame)) / 4096;
			EAB_addParticle(&entity->posData->location, &particleColor);
		}
		overlayColor = EAB_D_800617C0;
		EAB_renderBackdrop(&overlayColor);
		if ((state->frame >= 0x32) && (state->frame < 0x123)) {
			ENTITY1_WIREFRAME_TOTAL = lerp(0x80, 0, 0x32, 0x122, state->frame);
			ENTITY1_WIREFRAME_TOTAL = 0x10 - (_sin(ENTITY1_WIREFRAME_TOTAL) * 16 / 4096);
		}
		rotation = MAIN_D_80134C40;
		if ((state->frame >= 0x32) && (state->frame < 0x123)) {
			rotation.vy = lerp(0x71c, 0x171c, 0x32, 0x122, state->frame);
		} else {
			rotation.vy = 0x171c;
		}
		rotation.vy += entity->posData->rotation.vy;
		EAB_calculateCameraOrbit(&viewRef, &viewPos, entity, &rotation,
		                         lerp(0xdac, 0xbb8, 0x32, 0x140, state->frame),
		                         lerp(0x258, 0x32, 0x32, 0x140, state->frame));
		GS_VIEWPOINT.vrx = viewRef.vx;
		GS_VIEWPOINT.vry = viewRef.vy;
		GS_VIEWPOINT.vrz = viewRef.vz;
		GS_VIEWPOINT.vpx = viewPos.vx;
		GS_VIEWPOINT.vpy = viewPos.vy;
		GS_VIEWPOINT.vpz = viewPos.vz;
		if (state->frame == 0x13f) {
			GS_VIEWPOINT = EAB_D_800619E4;
		}
		if (state->frame < 0x140) {
			break;
		}
		state->phase = 2;
		EAB_removeParticles();
		setMapLayerEnabled(1);
		EAB_setEntitiesVisible(1);
		GS_VIEWPOINT = EAB_D_800619E4;
		stopSound();
		playSound(8, 3);
		break;
	case 2:
		if (state->frame < 0x14a) {
			EAB_addSpawnRing(entity);
		}
		if (state->frame < 0x154) {
			break;
		}
		EAB_removeRings();
		state->frame = -1;
		removeObject(0x60c, instanceId);
		break;
	}
}

void EAB_initializeParticles(void)
{
	int32_t i;

	for (i = 0; i < 100; i++) {
		EAB_D_80061A10[i][0] = -1;
	}
}

void EAB_renderFlash(VECTOR *color)
{
	POLY_FT4 *prim;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = 0xdd;
	prim->clut = 0x79c0;
	prim->x0 = -0xa0;
	prim->y0 = -0x78;
	prim->x1 = 0xa0;
	prim->y1 = -0x78;
	prim->x2 = -0xa0;
	prim->y2 = 0x78;
	prim->x3 = 0xa0;
	prim->y3 = 0x78;
	prim->u0 = 0;
	prim->v0 = 0x80;
	prim->u1 = 3;
	prim->v1 = 0x80;
	prim->u2 = 0;
	prim->v2 = 0x83;
	prim->u3 = 3;
	prim->v3 = 0x83;
	prim->r0 = color->vx;
	prim->g0 = color->vy;
	prim->b0 = color->vz;
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0xa, prim);
	prim++;
	GsSetWorkBase((PACKET *)prim);
}

void EAB_setEntitiesVisible(int32_t enabled)
{
	int32_t i;

	MAIN_D_80134C48 = enabled;
	if (enabled == 0) {
		for (i = 0; i < ENTITY_MAX; i++) {
			if ((i != 2) && (ENTITY_TABLE[i]->isOnMap != 0)) {
				ENTITY_TABLE[i]->isOnMap = 0;
				EAB_D_80061A04[i] = 1;
			} else {
				EAB_D_80061A04[i] = 0;
			}
		}
	} else {
		for (i = 0; i < ENTITY_MAX; i++) {
			if (i != 2) {
				ENTITY_TABLE[i]->isOnMap = EAB_D_80061A04[i];
			}
		}
	}
}

int32_t EAB_addParticle(VECTOR *position, RGB8 *color)
{
	EabParticle *e;
	int32_t i;

	e = (EabParticle *)EAB_D_80061A10;
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
	addObject(0x607, i, EAB_tickParticle, EAB_renderParticle);

	return i;
}

void EAB_renderBackdrop(VECTOR *color)
{
	POLY_FT4 *prim;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = 0xdd;
	prim->clut = 0x79c0;
	prim->x0 = -DRAWING_OFFSET_X;
	prim->y0 = -DRAWING_OFFSET_Y;
	prim->x1 = 0x140 - DRAWING_OFFSET_X;
	prim->y1 = -DRAWING_OFFSET_Y;
	prim->x2 = -DRAWING_OFFSET_X;
	prim->y2 = 0xf0 - DRAWING_OFFSET_Y;
	prim->x3 = 0x140 - DRAWING_OFFSET_X;
	prim->y3 = 0xf0 - DRAWING_OFFSET_Y;
	prim->u0 = 0;
	prim->v0 = 0x80;
	prim->u1 = 3;
	prim->v1 = 0x80;
	prim->u2 = 0;
	prim->v2 = 0x83;
	prim->u3 = 3;
	prim->v3 = 0x83;
	prim->r0 = color->vx;
	prim->g0 = color->vy;
	prim->b0 = color->vz;
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0xfa0, prim);
	prim++;
	GsSetWorkBase((PACKET *)prim);
}

void EAB_calculateCameraOrbit(VECTOR *viewRef, VECTOR *viewPos, Entity *entity,
                              SVECTOR *rotation, int32_t distance, int32_t height)
{
	MATRIX m1;
	MATRIX m2;
	VECTOR v;

	calculateBoneMatrix(entity, 0, &m1);
	viewRef->vx = m1.t[0];
	viewRef->vy = m1.t[1] - 0x64;
	viewRef->vz = m1.t[2];
	RotMatrixZYX(rotation, &m2);
	v.vx = 0;
	v.vy = 0;
	v.vz = distance;
	ApplyMatrixLV(&m2, &v, viewPos);
	viewPos->vx += viewRef->vx;
	viewPos->vy += viewRef->vy - height;
	viewPos->vz += viewRef->vz;
}

void EAB_removeParticles(void)
{
	int32_t i;
	int32_t *p;

	p = &EAB_D_80061A10[0][0];
	for (i = 0; i < 100; i++) {
		if (*p >= 0) {
			removeObject(0x607, i);
			*p = -1;
		}
		p = (int32_t *)((uint32_t)p + 0x10);
	}
}

void EAB_tickParticle(int32_t id)
{
	int32_t *p;

	p = &EAB_D_80061A10[id][0];
	if (*p >= 0x56) {
		removeObject(0x607, id);
		*p = -1;
	} else {
		*p += 1;
	}
}

#if 0
void EAB_renderParticle(int32_t id)
{
	SVECTOR corners[4];
	DVECTOR screen[4];
	int32_t depth[4];
	EabParticle *e;
	LINE_F2 *prim;
	int32_t size;
	int32_t d;
	int32_t i;
	int32_t j;

	e = &((EabParticle *)EAB_D_80061A10)[id];
	size = lerp(8, 0x9c4, 0, 0x56, e->timer);
	corners[0].vx = e->pos.vx + size;
	corners[0].vz = e->pos.vz + size;
	corners[1].vx = e->pos.vx + size;
	corners[1].vz = e->pos.vz - size;
	corners[2].vx = e->pos.vx - size;
	corners[2].vz = e->pos.vz - size;
	corners[3].vx = e->pos.vx - size;
	corners[3].vz = e->pos.vz + size;
	corners[0].vy = corners[1].vy = corners[2].vy = corners[3].vy = e->pos.vy;

	for (i = 0; i < 4; i++) {
		depth[i] = worldPosToScreenPos(&corners[i], &screen[i]) >> 4;
	}

	prim = (LINE_F2 *)GsGetWorkBase();
	for (i = 0; i < 4; i++) {
		d = depth[i];
		if ((d > 0x20) && (d < 0x1000)) {
			j = (i + 1) % 4;
			d = depth[j];
			if ((d > 0x20) && (d < 0x1000)) {
				SetLineF2(prim);
				prim->r0 = lerp(e->r, 0, 0, 0x56, e->timer);
				prim->g0 = lerp(e->g, 0, 0, 0x56, e->timer);
				prim->b0 = lerp(e->b, 0, 0, 0x56, e->timer);
				prim->x0 = screen[i].vx;
				prim->y0 = screen[i].vy;
				prim->x1 = screen[j].vx;
				prim->y1 = screen[j].vy;
				AddPrim(ACTIVE_ORDERING_TABLE->org + 0xf9f, prim++);
			}
		}
	}

	GsSetWorkBase((PACKET *)prim);
}
#else
INCLUDE_ASM("asm/eab/nonmatchings/eab", EAB_renderParticle);
#endif

void EAB_startBuildup(Entity *entity)
{
	VECTOR color;

	GsMapModelingData((unsigned long *)(MAIN_D_80134C28 + 4));
	color = EAB_D_800616DC;
	EAB_setModelColor((int32_t *)&color);
	MAIN_D_801353F0.frame = 0;
	MAIN_D_801353F0.phase = 0;
	MAIN_D_801353F0.entity = entity;
	addObject(0x60b, 0, EAB_tickBuildup, EAB_renderBuildup);
	EAB_initializeRings();
	entity->isOnMap = 0;
	playSound2(8, 0);
}

int32_t EAB_tick(Entity *entity, int32_t isInitialized)
{
	VECTOR colorEnd;
	VECTOR colorStart;
	EabState *state;
	int32_t i;

	state = &EAB_D_800617E8;
	if (isInitialized != 0) {
		return state->frame;
	}

	state->frame = 0;
	state->phase = 0;
	state->entity = entity;
	state->location = entity->posData->location;
	addObject(0x60c, 0, EAB_tickSpawn, EAB_renderSpawn);
	EAB_initializeParticles();
	initializeFlashData(EAB_D_80061800);
	for (i = 0; i < 9; i++) {
		*EFE_DATA_STACK++ = 0x20;
		*EFE_DATA_STACK++ = (int32_t)EAB_D_8006170C[i];
		*EFE_DATA_STACK++ = 0x22;
		*EFE_DATA_STACK++ = 0x32;
		*EFE_DATA_STACK++ = 0;
		*EFE_DATA_STACK++ = EAB_D_8006170C[i][3];
		colorEnd.vx = colorEnd.vy = colorEnd.vz = 0xff;
		colorStart.vx = colorStart.vy = colorStart.vz = 0x14;
		*EFE_DATA_STACK++ = (int32_t)&colorStart;
		*EFE_DATA_STACK++ = (int32_t)&colorEnd;
		createFlash();
	}
	playSound(8, 1);

	return 0;
}
