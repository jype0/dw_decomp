#include <stdlib.h>

#include <libgpu.h>

#include <dw/endi.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/main.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/version.h>
#include <dw/world_object.h>

int32_t lerp(int32_t a, int32_t b, int32_t c, int32_t d, int32_t t);
void renderFXParticle(SVECTOR *p, int32_t a, RGB8 *c);
void calculateBoneMatrix(Entity *entity, int32_t b, MATRIX *m);

static void ENDI_downloadClut(u_long clut);
static void ENDI_setClutStp(u_long clut);
static void ENDI_setModelSemiTrans(Entity *entity, int32_t arg1);
static void ENDI_updateEnding(int32_t objectId);
static void ENDI_renderEndingObject(int32_t objectId);
static void ENDI_startParticles(void);
static void ENDI_fadeClut(u_long *srcClut, Entity *entity, u_long *dstClut,
		   int32_t startFrame, int32_t endFrame, int32_t frame);
static int32_t ENDI_spawnParticle(Entity *entity, int16_t boneIndex);
static void ENDI_releaseParticles(int32_t index);
static void ENDI_getParticleBase(int32_t arg0, SVECTOR *dst);
static void ENDI_setParticleBase(int32_t arg0, SVECTOR *src);
static void ENDI_stopParticles(void);
static void ENDI_clearParticles(void);
static void ENDI_renderParticles(int32_t objectId);
static void ENDI_tickEndingParticles(int32_t objectId);

u_long *ENDI_FADE_CLUT_BUFFER = (u_long *)GENERAL_BUFFER;
u_long *ENDI_CLUT_BUFFER = (u_long *)(GENERAL_BUFFER + 0x304);
RGB8 ENDI_PARTICLE_COLOR = { 0x80, 0x80, 0x80 };

static void *const endi_functions[] = {
	ENDI_tickEnding,
#if VERSION_IS(EU)
	ENDI_tickEndingParticles,
	ENDI_renderParticles,
#else
	ENDI_renderParticles,
	ENDI_tickEndingParticles,
#endif
	ENDI_clearParticles,
	ENDI_stopParticles,
	ENDI_setParticleBase,
	ENDI_getParticleBase,
	ENDI_releaseParticles,
	ENDI_spawnParticle,
	ENDI_fadeClut,
	ENDI_startParticles,
#if !VERSION_IS(EU)
	ENDI_renderEndingObject,
	ENDI_updateEnding,
#endif
	ENDI_setModelSemiTrans,
	ENDI_setClutStp,
	ENDI_downloadClut,
#if VERSION_IS(EU)
	ENDI_updateEnding,
	ENDI_renderEndingObject,
#endif
};

static void ENDI_setClutStp(u_long clut)
{
	int32_t i;
	RECT rect;
	int16_t *ptr;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t stp = 0;

	ptr = (int16_t *)clut;
	for (i = 0; i < 384; ++i) {
		r = (*ptr >> 0) & 0x1f;
		g = (*ptr >> 5) & 0x1f;
		b = (*ptr >> 10) & 0x1f;

		if (((r == 0) && (g == 0)) && (b == 0)) {
			stp = 0;
		} else {
			stp = 1;
		}

		*ptr = r;
		*ptr += g << 5;
		*ptr += b << 10;
		*ptr++ += stp << 15;
	}

	setRECT(&rect, 0, 488, 16, 24);
	LoadImage(&rect, (u_long *)clut);

	DrawSync(0);
}

static void ENDI_downloadClut(u_long clut)
{
	RECT rect;

	setRECT(&rect, 0, 488, 16, 24);
	StoreImage(&rect, (u_long *)clut);

	DrawSync(0);
}

static void ENDI_setModelSemiTrans(Entity *entity, int32_t arg1)
{
	int32_t i;
	ModelComponent *comp;
	TMDModel *data;
	TMDModel *model;
	int32_t nobj;
	struct TMD_STRUCT *obj;
	struct TMD_STRUCT *object;
	int32_t primn;
	uint8_t *prim;
	int32_t j;
	TMD_P_TG3 *p;

	comp = getEntityModelComponent(entity->type, 2);
	data = comp->modelPtr;
	model = data;
	nobj = model->nobj;
	obj = model->obj;

	for (i = 0; i < nobj; i++) {
		object = &obj[i];
		primn = object->primn;
		prim = (uint8_t *)object->primtop;
		for (j = 0; j < primn; j++) {
			p = (TMD_P_TG3 *)prim;
			switch (p->cd) {
			case 0x34:
				p->cd |= 2;
			case 0x36:
				p->tpage = (p->tpage & 0xff9f) | 0x20;
				prim += 0x1c;
				break;
			case 0x3c:
				p->cd |= 2;
			case 0x3e:
				p->tpage = (p->tpage & 0xff9f) | 0x20;
				prim += 0x24;
				break;
			}
		}
	}
}

static void ENDI_updateEnding(int32_t objectId)
{
	EndingState *state;
	Entity *entity;
	int32_t scale;
	int32_t peakFrame;
	SVECTOR base;

	state = &ENDI_STATE;
	entity = state->entity;
	state->frame++;
	switch (state->phase) {
	case 0:
		ENDI_fadeClut(ENDI_CLUT_BUFFER, entity, ENDI_FADE_CLUT_BUFFER, 0, 60, state->frame);
		ENDI_spawnParticle(entity, state->frame % (DIGIMON_DATA[entity->type].boneCount - 1) + 1);

		if (state->frame >= 60) {
			state->phase = 1;
		}
		break;
	case 1:
		ENDI_spawnParticle(entity, state->frame % (DIGIMON_DATA[entity->type].boneCount - 1) + 1);

		peakFrame = 78;
		if (state->frame <= peakFrame) {
			scale = lerp(0x1000, 0x999, 60, peakFrame, state->frame);
		} else {
			scale = lerp(0x999, 0x1800, peakFrame, 80, state->frame);
		}

		entity->posData->scale.vx = scale;
		entity->posData->scale.vy = scale;
		entity->posData->scale.vz = scale;

		if (state->frame >= 80) {
			playSound(8, 1);
			state->phase = 2;
			ENDI_releaseParticles(0);
			state->velocity = -450;
		}
		break;
	case 2:
		ENDI_getParticleBase(0, &base);
		base.vy += state->velocity;
		ENDI_setParticleBase(0, &base);

		state->velocity += 20;
		if (state->velocity >= 0) {
			state->phase = 3;
		}
		break;
	case 3:
	case 10:
		stopSound();
		ENDI_stopParticles();
		removeObject(0x812, objectId);
		state->frame = -1;
		break;
	}
}

static void ENDI_renderEndingObject(int32_t objectId)
{
	EndingState *state;

	state = &ENDI_STATE;
}

static void ENDI_startParticles(void)
{
	ENDI_clearParticles();
	ENDI_DATA.flag = 1;
	addObject(0x813, 0, ENDI_tickEndingParticles, ENDI_renderParticles);
}

static void ENDI_fadeClut(u_long *srcClut, Entity *entity, u_long *dstClut,
			  int32_t startFrame, int32_t endFrame, int32_t frame)
{
	RECT rect;
	int32_t i;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t stp;
	int16_t *src;
	int16_t *dst;

	src = (int16_t *)srcClut;
	dst = (int16_t *)dstClut;

	for (i = 0; i < 384; i++) {
		r = *src & 0x1f;
		g = (*src >> 5) & 0x1f;
		b = (*src >> 10) & 0x1f;
		stp = (*src++ >> 15) & 1;
		if (frame != startFrame) {
			stp = 1;
		}

		r = (int16_t)r * (endFrame - frame) / (endFrame - startFrame);
		g = (int16_t)g * (endFrame - frame) / (endFrame - startFrame);
		b = (int16_t)b * (endFrame - frame) / (endFrame - startFrame);

		if (((r == 0) && (g == 0)) && (b == 0)) {
			stp = 0;
		} else {
			stp = 1;
		}

		*dst = r;
		*dst += g << 5;
		*dst += b << 10;
		*dst++ += stp << 15;
	}

	setRECT(&rect, 0, 488, 16, 24);
	LoadImage(&rect, dstClut);

	DrawSync(0);
}

static int32_t ENDI_spawnParticle(Entity *entity, int16_t boneIndex)
{
	int32_t i;
	EndiParticle *p;

	for (i = 0; i < NUM_ENDI_PARTICLES; ++i) {
		if (ENDI_PARTICLES[i].active < 0) {
			break;
		}
	}

	p = &ENDI_PARTICLES[i];
	p->active = 0;
	p->boneIndex = boneIndex;
	p->entity = entity;
	p->pos.vx = (rand() % 140) - 70;
	p->pos.vy = (rand() % 80) - 40;
	p->pos.vz = (rand() % 80) - 40;

	return i;
}

static void ENDI_releaseParticles(int32_t index)
{
	MATRIX baseMatrix;
	MATRIX boneMatrix;
	SVECTOR vec;
	EndiParticle *p;
	int32_t i;

	p = &ENDI_PARTICLES[index];
	ENDI_DATA.flag = 0;

	calculateBoneMatrix(p->entity, 0, &baseMatrix);

	ENDI_DATA.base.vx = baseMatrix.t[1];
	ENDI_DATA.base.vy = baseMatrix.t[2];
	ENDI_DATA.base.vz = baseMatrix.t[3];

	for (i = 0; i < NUM_ENDI_PARTICLES; ++i) {
		p = &ENDI_PARTICLES[i];
		if (p->active >= 0) {
			calculateBoneMatrix(p->entity, p->boneIndex, &boneMatrix);

			vec.vx = (p->pos.vx * p->entity->posData->scale.vx) / 0x1000;
			vec.vy = (p->pos.vy * p->entity->posData->scale.vy) / 0x1000;
			vec.vz = (p->pos.vz * p->entity->posData->scale.vz) / 0x1000;
			ApplyMatrixSV(&boneMatrix, &vec, &vec);

			vec.vx += boneMatrix.t[0];
			vec.vy += boneMatrix.t[1];
			vec.vz += boneMatrix.t[2];

			p->pos.vx = vec.vx - ENDI_DATA.base.vx;
			p->pos.vy = vec.vy - ENDI_DATA.base.vy;
			p->pos.vz = vec.vz - ENDI_DATA.base.vz;
		}
	}
}

static void ENDI_getParticleBase(int32_t arg0, SVECTOR *dst)
{
	copyVector(dst, &ENDI_DATA.base);
}

static void ENDI_setParticleBase(int32_t arg0, SVECTOR *src)
{
	copyVector(&ENDI_DATA.base, src);
}

static void ENDI_stopParticles(void)
{
	removeObject(0x813, 0);
}

static void ENDI_clearParticles(void)
{
	int32_t i;

	for (i = 0; i < NUM_ENDI_PARTICLES; ++i) {
		ENDI_PARTICLES[i].active = -1;
	}
}

static void ENDI_tickEndingParticles(int32_t objectId)
{
}

static void ENDI_renderParticles(int32_t objectId)
{
	int32_t i;
	EndiParticle *p;
	RGB8 color;
	MATRIX boneMatrix;
	SVECTOR vec;
	int32_t size;

	for (i = 0; i < NUM_ENDI_PARTICLES; ++i) {
		p = &ENDI_PARTICLES[i];
		if (p->active >= 0) {
			color = ENDI_PARTICLE_COLOR;
			if (ENDI_DATA.flag != 0) {
				boneMatrix = p->entity->posData[p->boneIndex].posMatrix.workm;
				vec.vx = (p->pos.vx * p->entity->posData->scale.vx) / 0x1000;
				vec.vy = (p->pos.vy * p->entity->posData->scale.vy) / 0x1000;
				vec.vz = (p->pos.vz * p->entity->posData->scale.vz) / 0x1000;
				ApplyMatrixSV(&boneMatrix, &vec, &vec);
				vec.vx += boneMatrix.t[0];
				vec.vy += boneMatrix.t[1];
				vec.vz += boneMatrix.t[2];
				copyVector(&p->worldPos, &vec);
			} else {
				p->worldPos.vx = p->pos.vx + ENDI_DATA.base.vx;
				p->worldPos.vy = p->pos.vy + ENDI_DATA.base.vy;
				p->worldPos.vz = p->pos.vz + ENDI_DATA.base.vz;
			}

			color.r = rand() % 70 + 60;
			color.g = rand() % 70 + 60;
			color.b = rand() % 70 + 60;

			size = rand() % 15 + 15;
			renderFXParticle(&p->worldPos, size, &color);
		}
	}
}

int32_t ENDI_tickEnding(entity, isInitialized)
	Entity *entity;
	int16_t isInitialized;
{
	EndingState *state;
	int32_t instanceId;

	state = &ENDI_STATE;
	instanceId = 0;
	if (isInitialized != 0) {
		return state->frame;
	}

	state->frame = 0;
	state->phase = 0;
	state->entity = entity;
	ENDI_downloadClut((u_long)ENDI_CLUT_BUFFER);
	ENDI_setClutStp((u_long)ENDI_CLUT_BUFFER);
	ENDI_setModelSemiTrans(ENTITY_TABLE[0], 0x28);
	PLAYER_SHADOW_ENABLED = 0;
	addObject(0x812, instanceId, ENDI_updateEnding, ENDI_renderEndingObject);
	ENDI_startParticles();

	return playSound(8, 0);
}
