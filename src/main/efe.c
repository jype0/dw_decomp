#include <stdlib.h>
#include <string.h>

#include <libcd.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/efe.h>
#include <dw/entity.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

typedef struct {
	int16_t state;
	int16_t unk2;
	int16_t unk4;
} CloudFXEntry;

typedef struct {
	int32_t red;
	int32_t green;
	int32_t blue;
} EfeColor;

typedef struct {
	int16_t state;
	int16_t timer;
	int16_t type;
	int16_t mode;
	int16_t endFrame;
	int16_t r;
	int16_t g;
	int16_t b;
	SVECTOR position;
	Entity *entity;
	int16_t spread[4][3];
} ParticleFX;

typedef struct {
	uint8_t width;
	uint8_t height;
	uint8_t baseU;
	uint8_t baseV;
	uint8_t spanU;
	uint8_t spanV;
} EntityParticleSprite;

typedef struct {
	int16_t timer;
	int16_t boneId;
	Entity *owner;
	int16_t unk8;
	int16_t unkA;
} EntityParticleFX;

typedef struct {
	int16_t v;
	int16_t pad1;
	int16_t pad2;
} EfeParticleField;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t depth;
	uint32_t size;
	int16_t w;
	int16_t h;
	int16_t tpage;
	uint8_t clut;
	int8_t attr;
	int16_t clut2;
	int8_t r;
	int8_t g;
	int8_t b;
	int8_t a;
} FlashParams;

uint8_t MAIN_D_80123370[54] = {
	0x78, 0x50, 0x80, 0x80, 0x0f, 0x0f, 0x78, 0x50,
	0xc0, 0x80, 0x0f, 0x0f, 0xc8, 0x50, 0x80, 0x90,
	0x1f, 0x07, 0xc8, 0x96, 0x80, 0x80, 0x0f, 0x0f,
	0xc8, 0x96, 0xc0, 0x80, 0x0f, 0x0f, 0xfa, 0x64,
	0x80, 0x90, 0x1f, 0x07, 0x64, 0x64, 0x80, 0x80,
	0x0f, 0x0f, 0x64, 0x64, 0xc0, 0x80, 0x0f, 0x0f,
	0xc8, 0x64, 0x80, 0x90, 0x1f, 0x07,
};

GsSPRITE CLOUD_FX_SPRITE = {
	0x50000000,
	0, 0,
	32, 32,
	0x3c,
	0, 128,
	192, 488,
	128, 128, 128,
	16, 16,
	0, 0,
	0,
};

int8_t MAIN_D_801233CC[16] = {
	0x00, 0x00, 0x20, 0x20, 0x20, 0x40, 0x40, 0x40,
	0x40, 0x40, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00,
};
int16_t MAIN_D_801233DC[18] = {
	-50, -54, -58, -62, -66, -70, -74, -78, -82,
	-86, -90, -93, -96, -99, -50, -50, -50, -994,
};
int16_t MAIN_D_80123400[14] = {
	0x2000, 0x2ee0, 0x3840, 0x4000, 0x4000, 0x5000, 0x5000,
	0x5000, 0x5000, 0x5400, 0x5800, 0x5c00, 0x6000, 0x6400,
};
uint8_t MAIN_D_8012341C[16] = {
	0x80, 0x80, 0x80, 0x80, 0x79, 0x73, 0x6c, 0x66,
	0x60, 0x59, 0x53, 0x4c, 0x46, 0x40, 0x00, 0x00,
};
char PATH_ETCHI_EFEDAT_EFE[16] = "ETCHI\\EFEDAT.EFE";
uint32_t MAIN_D_8012343C[4] = {0};

static void *efe_data_order[] = {
	MAIN_D_8012343C,
	PATH_ETCHI_EFEDAT_EFE,
	MAIN_D_8012341C,
	MAIN_D_80123400,
	MAIN_D_801233DC,
	MAIN_D_801233CC,
	&CLOUD_FX_SPRITE,
	MAIN_D_80123370,
};

void setShortWithStride();
void getDrawingOffsetCopy(int32_t *x, int32_t *y);
int32_t isTamerOnScreen(void);
int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t);
void renderParticleFlash(int16_t *params);
void renderFXParticle(SVECTOR *pos, int32_t size, uint8_t *color);
int32_t worldPosToScreenPos(int16_t *world, int16_t *screen);
void addFXPrim(POLY_FT4 *prim, int32_t x, int32_t y, int16_t width, int16_t height, int32_t depth);
void renderSprite(GsSPRITE *spr, int16_t x, int16_t y, int32_t depth,
                  int32_t sx, int32_t sy);

void initializeParticleFX();
void createParticleFX(uint8_t kind, int32_t count, SVECTOR *pos, Entity *entity, int32_t lifetime);
void tickParticleFX(int32_t id);
void renderParticleFX();
int32_t addEntityParticleFX(Entity *owner, int16_t timer);
void initializeEntityParticleFX();
void tickEntityParticleFX(int32_t id);
void renderEntityParticleFX(int32_t id);
void removeEntityParticleFX();
void initializeCloudFXData();
void removeAllCloudFX();
void createCloudFX(int16_t *pos);
void tickCloudFX();
void renderCloudFX(int32_t id);
void rotateVector(void);
char *initializeFlashData(char *base);
void createFlash(void);
void tickEFEFlash();
void renderEFEFlash(int32_t id);
int32_t setEFEFlashOffset(int32_t id, int16_t x, int16_t y);
void downloadSomeImage();
void modifySomeImage(int32_t dim);
void findEFEDATFile(void);
void initializeEFE();
void getEFEDATEntry();
void renderParticleFlash();

extern int16_t MAIN_D_80138AA4[];
extern int16_t MAIN_D_801389B4[];
extern u_long SOME_IMAGE_DATA[];
extern uint8_t MAIN_D_801387B8[];
extern EfeParticleField MAIN_D_80138888[];
extern EfeParticleField MAIN_D_8013888A[];
extern EfeParticleField MAIN_D_8013888C[];
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t FLASH_INSTANCE;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern int32_t VIEWPORT_DISTANCE;

static void *efe_functions[] = {
	renderParticleFlash,
	getEFEDATEntry,
	initializeEFE,
	findEFEDATFile,
	modifySomeImage,
	downloadSomeImage,
	setEFEFlashOffset,
	renderEFEFlash,
	tickEFEFlash,
	createFlash,
	initializeFlashData,
	rotateVector,
	renderCloudFX,
	tickCloudFX,
	createCloudFX,
	removeAllCloudFX,
	initializeCloudFXData,
	removeEntityParticleFX,
	renderEntityParticleFX,
	tickEntityParticleFX,
	initializeEntityParticleFX,
	addEntityParticleFX,
	renderParticleFX,
	tickParticleFX,
	createParticleFX,
	initializeParticleFX,
};

// clang-format off
int8_t MAIN_D_80134214[4] = {
	0x02, 0x03, 0x02, 0x00,
};

int8_t MAIN_D_80134218[4] = {
	0x19, 0x30, 0x14, 0x00,
};

uint8_t MAIN_D_8013421C[4] = {
	0x40, 0x00, 0x00, 0x00,
};

char EFE_FILE_VERSION_SUFFIX[] = ";1";
// clang-format on

static void createParticleFX__garbage__(void)
{
	int32_t t0;
	int32_t t1;
	int32_t t2;
	int32_t t3;

	t0 = MAIN_D_801387B8[0];
	t1 = MAIN_D_801387B8[1];
	t2 = MAIN_D_801387B8[2];
	t3 = MAIN_D_801387B8[3];
	MAIN_D_801387B8[0] = t0 + t1 + t2 + t3;
}

void initializeParticleFX(void)
{
	int32_t i;
	SVECTOR ang;
	SVECTOR vec;
	MATRIX m;

	setShortWithStride(MAIN_D_801387B8, -1, 4, 0x34);
	for (i = 0; i < 0x32; i++) {
		ang.vx = (((rand() & 0x7F) - 0x40) << 12) / 64;
		ang.vy = (((rand() & 0x7F) - 0x40) << 12) / 64;
		ang.vz = (((rand() & 0x7F) - 0x40) << 12) / 64;
		vec.vy = 0;
		vec.vx = 0;
		vec.vz = ((rand() % 150) + 0xFA) * 0x10;
		RotMatrixZYX(&ang, &m);
		ApplyMatrixSV(&m, &vec, &vec);
		MAIN_D_80138888[i].v = vec.vx;
		MAIN_D_8013888A[i].v = vec.vy;
		MAIN_D_8013888C[i].v = vec.vz;
	}
}

void createParticleFX(uint8_t kind, int32_t count, SVECTOR *pos, Entity *entity, int32_t lifetime)
{
	ParticleFX *fx;
	int32_t i;
	int16_t p0;
	int16_t p1;
	int16_t p2;
	int16_t p3;

	for (i = 0; i < 4; i++) {
		if (((ParticleFX *)MAIN_D_801387B8)[i].state == -1) {
			break;
		}
	}
	if (i == 4) {
		return;
	}
	fx = &((ParticleFX *)MAIN_D_801387B8)[i];
	fx->spread[0][0] = (rand() % 200) - 100;
	fx->spread[1][0] = (rand() % 200) - 100;
	fx->spread[2][0] = (rand() % 200) - 100;
	fx->spread[3][0] = (rand() % 170) + 250;
	fx->spread[0][1] = (rand() % 200) - 100;
	fx->spread[1][1] = (rand() % 200) - 100;
	fx->spread[2][1] = (rand() % 200) - 100;
	fx->spread[3][1] = (rand() % 170) + 250;
	switch (count) {
	case 0:
		fx->timer = 0x13;
		break;
	case 1:
		fx->timer = 0x16;
		fx->spread[0][2] = (rand() % 200) - 100;
		fx->spread[1][2] = (rand() % 200) - 100;
		fx->spread[2][2] = (rand() % 200) - 100;
		fx->spread[3][2] = (rand() % 200) + 300;
		break;
	case 2:
		fx->timer = 0xe;
		break;
	default:
		return;
	}
	switch (kind) {
	case 0:
		fx->r = 0xff;
		fx->g = 0x6e;
		fx->b = 0x46;
		break;
	case 1:
		fx->r = 0xff;
		fx->g = 0x6e;
		fx->b = 0x46;
		break;
	case 2:
		fx->r = 0xc8;
		fx->g = 0xc8;
		fx->b = 0x64;
		break;
	case 3:
		fx->r = 0xcc;
		fx->g = 0;
		fx->b = 0x88;
		break;
	case 4:
		fx->r = 0x32;
		fx->g = 0x32;
		fx->b = 0xff;
		break;
	case 5:
		fx->r = 0x96;
		fx->g = 0x96;
		fx->b = 0x96;
		break;
	case 6:
		fx->r = 0xff;
		fx->g = 0x6e;
		fx->b = 0x6e;
		break;
	default:
		return;
	}
	fx->state = 0;
	fx->type = kind;
	fx->mode = count;
	fx->position = *pos;
	fx->entity = entity;
	fx->endFrame = lifetime + 4;
	addObject(0x600, i, tickParticleFX, renderParticleFX);
}

static void tickParticleFX__garbage__(void)
{
	int32_t t0;
	int32_t t1;
	int32_t t2;
	int32_t t3;
	int32_t t4;
	int32_t t5;
	int32_t t6;
	int32_t t7;

	t0 = MAIN_D_801387B8[0];
	t1 = MAIN_D_801387B8[1];
	t2 = MAIN_D_801387B8[2];
	t3 = MAIN_D_801387B8[3];
	t4 = MAIN_D_801387B8[4];
	t5 = MAIN_D_801387B8[5];
	t6 = MAIN_D_801387B8[6];
	t7 = MAIN_D_801387B8[7];
	MAIN_D_801387B8[0] = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
}

void tickParticleFX(int32_t id)
{
	ParticleFX *fx;
	Entity *entity;
	PositionData *posData;
	SVECTOR pos;
	int16_t state;
	int32_t i;
	int32_t angle;
	int32_t dist;
	int16_t baseX;
	int16_t baseZ;
	int16_t radius;
	int16_t spread;

	fx = &((ParticleFX *)MAIN_D_801387B8)[id];
	entity = fx->entity;
	fx->state++;
	fx->endFrame--;
	if ((state = fx->state) == 10) {
		switch (fx->mode) {
		case 0:
			if (entity != NULL) {
				addEntityParticleFX(entity, 0xe);
				addEntityParticleFX(entity, 0xe);
				addEntityParticleFX(entity, 0xe);
			}
			break;
		case 1:
			addEntityParticleFX(entity, 0x16);
			addEntityParticleFX(entity, 0x16);
			addEntityParticleFX(entity, 0x16);
			break;
		case 2:
			break;
		}
		return;
	}
	if (state == 13) {
		switch (fx->mode) {
		case 0:
			if (entity != NULL) {
				addEntityParticleFX(entity, 0xe);
				addEntityParticleFX(entity, 0xe);
			}
			break;
		case 1:
			addEntityParticleFX(entity, 0x16);
			addEntityParticleFX(entity, 0xe);
			break;
		case 2:
			break;
		}
		return;
	}
	if ((fx->endFrame < 4) && (fx->endFrame >= 0) && (fx->mode == 1)) {
		posData = fx->entity->posData;
		baseX = posData->posMatrix.workm.t[0];
		baseZ = posData->posMatrix.workm.t[2];
		radius = DIGIMON_DATA[entity->type].radius;
		spread = radius >> 1;
		for (i = 0; i < fx->endFrame + 2; i++) {
			angle = rand();
			dist = radius + (rand() % spread);
			pos.vx = baseX + ((dist * _sin(angle)) >> 12);
			pos.vz = baseZ - ((dist * _cos(angle)) >> 12);
			createCloudFX((int16_t *)&pos);
		}
		return;
	}
	if ((state >= fx->timer) && (fx->endFrame < 0)) {
		fx->state = -1;
		removeObject(0x600, id);
	}
}

void renderParticleFX(int32_t id)
{
	ParticleFX *fx;
	ParticleFlashData flash;
	SVECTOR world;
	DVECTOR screen;
	uint8_t color[4];
	int32_t i;
	int32_t depth;
	int16_t flashCount;
	int16_t particleCount;
	int32_t timer;
	int32_t state;

	fx = &((ParticleFX *)MAIN_D_801387B8)[id];
	flashCount = MAIN_D_80134214[fx->mode];
	particleCount = MAIN_D_80134218[fx->mode];
	if (fx->state == 2) {
		for (i = 0; i < flashCount; i++) {
			world.vx = fx->position.vx + fx->spread[0][i];
			world.vy = fx->position.vy + fx->spread[1][i];
			world.vz = fx->position.vz + fx->spread[2][i];
			depth = worldPosToScreenPos((int16_t *)&world, (int16_t *)&screen);
			flash.screenPos.vx = screen.vx;
			flash.screenPos.vy = screen.vy;
			flash.sizeX = flash.sizeY = 0x40;
			flash.tpage = getTPage(1, 1, 832, 256);
			flash.uBase = 0x40;
			flash.vBase = 0xc0;
			flash.clut = getClut(0, 487);
			flash.color.r = fx->r;
			flash.color.g = fx->g;
			flash.color.b = fx->b;
			flash.colorScale = 0x80;
			flash.scale = (uint32_t)(fx->spread[3][i] * (VIEWPORT_DISTANCE * 8)) / (uint32_t)depth;
			flash.depth = depth >> 4;
			if ((flash.depth > 0x20) && (flash.depth < 0x1000)) {
				renderParticleFlash((int16_t *)&flash);
			}
		}
	}
	if (fx->state >= fx->timer) {
		return;
	}
	state = fx->state;
	timer = fx->timer;
	color[0] = lerp(fx->r, 0, 0, timer, state);
	color[1] = lerp(fx->g, 0, 0, timer, state);
	color[2] = lerp(fx->b, 0, 0, timer, state);
	state = lerp(0, 0x6e, 0, timer, state);
	state = _sin(state) >> 7;
	for (i = 0; i < particleCount; i++) {
		world.vx = fx->position.vx + ((state * MAIN_D_80138888[i].v) >> 9);
		world.vy = fx->position.vy + ((state * MAIN_D_8013888A[i].v) >> 9);
		world.vz = fx->position.vz + ((state * MAIN_D_8013888C[i].v) >> 9);
		renderFXParticle(&world, 0x28, color);
	}
}

int32_t addEntityParticleFX(Entity *owner, int16_t timer)
{
	EntityParticleFX *fx;
	int32_t i;
	int32_t r;

	for (i = 0; i < 20; i++) {
		if (((EntityParticleFX *)MAIN_D_801389B4)[i].timer < 0) {
			break;
		}
	}
	if (i == 20) {
		return -1;
	}
	fx = &((EntityParticleFX *)MAIN_D_801389B4)[i];
	fx->timer = timer;
	r = rand();
	fx->boneId = (r % (DIGIMON_DATA[owner->type].boneCount - 1)) + 1;
	fx->unk8 = 0;
	fx->owner = owner;
	addObject(0x502, i, tickEntityParticleFX, renderEntityParticleFX);
	return i;
}

void initializeEntityParticleFX(void)
{
	setShortWithStride(MAIN_D_801389B4, -1, 0x14, 0xC);
}

void tickEntityParticleFX(int32_t id)
{
	char *e;
	int32_t bc;

	e = (char *)&MAIN_D_801389B4[id * 6];
	if (*(int16_t *)e <= 0) {
		removeEntityParticleFX(id);
		return;
	}
	if ((*(int16_t *)e % 4) == 0) {
		bc = DIGIMON_DATA[**(int32_t **)(e + 4)].boneCount - 1;
		*(int16_t *)(e + 2) = (rand() % bc) + 1;
		*(int16_t *)(e + 8) = rand() % 9;
	}
	*(int16_t *)e = *(int16_t *)e - 1;
}

void renderEntityParticleFX(int32_t id)
{
	EntityParticleFX *fx;
	Entity *owner;
	POLY_FT4 *prim;
	EntityParticleSprite *sprite;
	PositionData *bone;
	DVECTOR screenPos;
	SVECTOR worldPos;
	int32_t depth;

	fx = &((EntityParticleFX *)MAIN_D_801389B4)[id];
	owner = fx->owner;
	prim = (POLY_FT4 *)GsGetWorkBase();
	sprite = &((EntityParticleSprite *)MAIN_D_80123370)[fx->unk8];
	bone = &owner->posData[fx->boneId];
	worldPos.vx = bone->posMatrix.workm.t[0];
	worldPos.vy = bone->posMatrix.workm.t[1];
	worldPos.vz = bone->posMatrix.workm.t[2];
	depth = worldPosToScreenPos((int16_t *)&worldPos, (int16_t *)&screenPos);
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	setSemiTrans(prim, 1);
	prim->r0 = prim->g0 = prim->b0 = 0x80;
	prim->tpage = getTPage(0, 1, 768, 256);
	prim->clut = getClut(192, 490);
	setUVWH(prim, sprite->baseU + (sprite->spanU + 1) * (fx->timer % 4), sprite->baseV, sprite->spanU, sprite->spanV);
	addFXPrim(prim, screenPos.vx, screenPos.vy, sprite->width, sprite->height, depth);
}

void removeEntityParticleFX(int32_t id)
{
	MAIN_D_801389B4[id * 6] = -1;
	removeObject(0x502, id);
}

void initializeCloudFXData(void)
{
	setShortWithStride(MAIN_D_80138AA4, -1, 0x3C, 6);
}

void removeAllCloudFX(void)
{
	int32_t i;

	for (i = 0; i < 0x3C; i++) {
		if (((CloudFXEntry *)MAIN_D_80138AA4)[i].state != -1) {
			((CloudFXEntry *)MAIN_D_80138AA4)[i].state = -1;
			removeObject(0x601, i);
		}
	}
}

void createCloudFX(int16_t *pos)
{
	CloudFXEntry *e;
	int32_t i;

	for (i = 0; i < 0x3C; i++) {
		if (((CloudFXEntry *)MAIN_D_80138AA4)[i].state < 0) {
			break;
		}
	}
	if (i != 0x3C) {
		e = &((CloudFXEntry *)MAIN_D_80138AA4)[i];
		e->state = 0;
		e->unk2 = pos[0];
		e->unk4 = pos[2];
		addObject(0x601, i, tickCloudFX, renderCloudFX);
	}
}

void tickCloudFX(int32_t id)
{
	int16_t *p;

	p = &MAIN_D_80138AA4[id * 3];
	*p += 1;
	if (*p >= 0xE) {
		*p = -1;
		removeObject(0x601, id);
	}
}

void renderCloudFX(int32_t id)
{
	CloudFXEntry *cloud;
	SVECTOR worldPos;
	DVECTOR screenPos;
	int32_t depth;

	cloud = &((CloudFXEntry *)MAIN_D_80138AA4)[id];
	worldPos.vx = cloud->unk2;
	worldPos.vy = MAIN_D_801233DC[cloud->state];
	worldPos.vz = cloud->unk4;
	depth = worldPosToScreenPos((int16_t *)&worldPos, (int16_t *)&screenPos);
	CLOUD_FX_SPRITE.u = MAIN_D_801233CC[cloud->state];
	CLOUD_FX_SPRITE.r = CLOUD_FX_SPRITE.g = CLOUD_FX_SPRITE.b = MAIN_D_8012341C[cloud->state];
	renderSprite(&CLOUD_FX_SPRITE, screenPos.vx, screenPos.vy, depth,
	             MAIN_D_80123400[cloud->state], MAIN_D_80123400[cloud->state]);
}

static void rotateVector__garbage__(void)
{
	int32_t v0;
	int32_t v1;
	int32_t v2;
	int32_t v3;
	int32_t v4;
	int32_t v5;
	int32_t v6;
	int32_t v7;
	int32_t v8;
	int32_t v9;
	int32_t v10;
	int32_t v11;
	int32_t v12;
	int32_t v13;
	int32_t v14;
	int32_t v15;
	int32_t v16;
	int32_t v17;
	int32_t v18;
	int32_t v19;

	v0 = MAIN_D_80138AA4[0] + 0;
	v1 = MAIN_D_80138AA4[1] + 1;
	v2 = MAIN_D_80138AA4[2] + 2;
	v3 = MAIN_D_80138AA4[0] + 3;
	v4 = MAIN_D_80138AA4[1] + 4;
	v5 = MAIN_D_80138AA4[2] + 5;
	v6 = MAIN_D_80138AA4[0] + 6;
	v7 = MAIN_D_80138AA4[1] + 7;
	v8 = MAIN_D_80138AA4[2] + 8;
	v9 = MAIN_D_80138AA4[0] + 9;
	v10 = MAIN_D_80138AA4[1] + 10;
	v11 = MAIN_D_80138AA4[2] + 11;
	v12 = MAIN_D_80138AA4[0] + 12;
	v13 = MAIN_D_80138AA4[1] + 13;
	v14 = MAIN_D_80138AA4[2] + 14;
	v15 = MAIN_D_80138AA4[0] + 15;
	v16 = MAIN_D_80138AA4[1] + 16;
	v17 = MAIN_D_80138AA4[2] + 17;
	v18 = MAIN_D_80138AA4[0] + 18;
	v19 = MAIN_D_80138AA4[1] + 19;
	MAIN_D_80138AA4[0] = (v0 * v1) + v2;
	MAIN_D_80138AA4[1] = (v1 * v2) + v3;
	MAIN_D_80138AA4[2] = (v2 * v3) + v4;
	MAIN_D_80138AA4[0] = (v3 * v4) + v5;
	MAIN_D_80138AA4[1] = (v4 * v5) + v6;
	MAIN_D_80138AA4[2] = (v5 * v6) + v7;
	MAIN_D_80138AA4[0] = (v6 * v7) + v8;
	MAIN_D_80138AA4[1] = (v7 * v8) + v9;
	MAIN_D_80138AA4[2] = (v8 * v9) + v10;
	MAIN_D_80138AA4[0] = (v9 * v10) + v11;
	MAIN_D_80138AA4[1] = (v10 * v11) + v12;
	MAIN_D_80138AA4[2] = (v11 * v12) + v13;
	MAIN_D_80138AA4[0] = (v12 * v13) + v14;
	MAIN_D_80138AA4[1] = (v13 * v14) + v15;
	MAIN_D_80138AA4[2] = (v14 * v15) + v16;
	MAIN_D_80138AA4[0] = (v15 * v16) + v17;
	MAIN_D_80138AA4[1] = (v16 * v17) + v18;
	MAIN_D_80138AA4[2] = (v17 * v18) + v19;
	MAIN_D_80138AA4[0] = (v18 * v19) + v0;
	MAIN_D_80138AA4[1] = (v19 * v0) + v1;
}

void rotateVector(void)
{
	MATRIX m;
	SVECTOR vec;
	SVECTOR rot;
	SVECTOR out;
	int32_t *vp;
	int32_t *rp;
	vp = EFE_POP1(int32_t *);
	rp = EFE_POP1(int32_t *);
	vec.vx = vp[0];
	vec.vy = vp[1];
	vec.vz = vp[2];
	rot.vx = rp[0];
	rot.vy = rp[1];
	rot.vz = rp[2];
	RotMatrixZYX(&rot, &m);
	ApplyMatrixSV(&m, &vec, &out);
	vp[0] = out.vx;
	vp[1] = out.vy;
	vp[2] = out.vz;
}

char *initializeFlashData(char *base)
{
	int32_t i;

	EFE_FLASH_DATA = (EfeFlashData *)base;
	for (i = 0; i < 12; i++) {
		EFE_FLASH_DATA[i].progress = -1;
	}
	return base + sizeof(EfeFlashData) * 12;
}

void createFlash(void)
{
	EfeFlashData *data;
	EfeColor *colorMax;
	EfeColor *colorMin;
	VECTOR *worldPos;
	int32_t scaleMax;
	int32_t scaleMin;
	int32_t tMax;
	int32_t fixedDepth;
	int32_t mode;
	int32_t absMode;
	int32_t i;
	int32_t offsetX;
	int32_t offsetY;

	colorMax = EFE_POP1(EfeColor *);
	colorMin = EFE_POP1(EfeColor *);
	scaleMax = EFE_POP1(int32_t);
	scaleMin = EFE_POP1(int32_t);
	tMax = EFE_POP1(int32_t);
	fixedDepth = EFE_POP1(int32_t);
	worldPos = EFE_POP1(VECTOR *);
	absMode = mode = EFE_POP1(int32_t);
	if (absMode < 0) {
		absMode = -absMode;
	}
	FLASH_INSTANCE = -1;
	for (i = 0; i < 12; i++) {
		if (EFE_FLASH_DATA[i].progress < 0) {
			break;
		}
	}
	if (i == 12) {
		return;
	}
	data = &EFE_FLASH_DATA[i];
	FLASH_INSTANCE = i;
	if ((absMode == 0x20) || (absMode == 1) || (absMode == 0)) {
		addObject(0x602, i, tickEFEFlash, renderEFEFlash);
		copyVector(&data->worldPos, worldPos);
		data->progress = 0;
		data->tMax = tMax;
		data->mode = mode;
		data->fixedDepth = fixedDepth;
		data->scaleMin = scaleMin;
		data->scaleMax = scaleMax;
		data->redMin = colorMin->red;
		data->greenMin = colorMin->green;
		data->blueMin = colorMin->blue;
		data->redMax = colorMax->red;
		data->greenMax = colorMax->green;
		data->blueMax = colorMax->blue;
		if (mode >= 0) {
			data->offsetX = 0;
			data->offsetY = 0;
		} else {
			if (isTamerOnScreen() == 1) {
				getDrawingOffsetCopy(&offsetX, &offsetY);
			} else {
				offsetX = DRAWING_OFFSET_X;
				offsetY = DRAWING_OFFSET_Y;
			}
			data->offsetX = 160 - offsetX;
			data->offsetY = 120 - offsetY;
		}
	}
}

void tickEFEFlash(int32_t id)
{
	EfeFlashData *data;

	data = &EFE_FLASH_DATA[id];
	data->progress++;
	if (data->progress >= data->tMax) {
		data->progress = -1;
		removeObject(0x602, id);
	}
}

void renderEFEFlash(int32_t id)
{
	EfeFlashData *data;
	ParticleFlashData flash;
	int32_t depth;
	int32_t absMode;
	int32_t red;
	int32_t green;
	int32_t blue;
	int32_t factor;

	data = &EFE_FLASH_DATA[id];
	absMode = data->mode;
	if (absMode < 0) {
		absMode = -absMode;
	}
	if (data->progress < 0) {
		return;
	}
	depth = worldPosToScreenPos((int16_t *)&data->worldPos, (int16_t *)&flash.screenPos);
	if (data->mode >= 0) {
		flash.screenPos.vx += (int16_t)(data->offsetX * VIEWPORT_DISTANCE / depth);
		flash.screenPos.vy += (int16_t)(data->offsetY * VIEWPORT_DISTANCE / depth);
	} else {
		flash.screenPos.vx = flash.screenPos.vx + data->offsetX;
		flash.screenPos.vy = flash.screenPos.vy + data->offsetY;
	}
	flash.sizeX = flash.sizeY = 0x40;
	switch (absMode) {
	case 0:
		flash.tpage = getTPage(1, 1, 832, 256);
		break;
	case 1:
		flash.tpage = getTPage(1, 1, 832, 256);
		break;
	case 0x20:
		flash.tpage = getTPage(1, 2, 832, 256);
		break;
	}
	flash.uBase = MAIN_D_8013421C[absMode & 3];
	flash.vBase = 0xc0;
	flash.clut = getClut(0, 487);
	red = lerp(data->redMin, data->redMax, 0, data->tMax, data->progress);
	green = lerp(data->greenMin, data->greenMax, 0, data->tMax, data->progress);
	blue = lerp(data->blueMin, data->blueMax, 0, data->tMax, data->progress);
	factor = _sin(lerp(0x80, 0x14, 0, data->tMax, data->progress));
	flash.color.r = (red * factor) >> 12;
	flash.color.g = (green * factor) >> 12;
	flash.color.b = (blue * factor) >> 12;
	flash.colorScale = 0x80;
	flash.scale = (uint32_t)(VIEWPORT_DISTANCE * lerp(data->scaleMin, data->scaleMax, 0, data->tMax, data->progress) * 10) / (uint32_t)depth;
	flash.scale += ((_sin(lerp(0, 0x1eb, 0, 0x17, data->progress)) * 300) >> 12) + 1;
	if (flash.scale >= 0x8000) {
		return;
	}
	if (data->fixedDepth <= 0) {
		flash.depth = depth >> 4;
	} else {
		flash.depth = data->fixedDepth;
	}
	if ((flash.depth > 0x20) && (flash.depth < 0x1000)) {
		renderParticleFlash((int16_t *)&flash);
	}
}

int32_t setEFEFlashOffset(int32_t id, int16_t x, int16_t y)
{
	EfeFlashData *data;

	data = &EFE_FLASH_DATA[id];
	data->offsetX = x;
	data->offsetY = y;
}

void downloadSomeImage(void)
{
	RECT r;

	setRECT(&r, 0x200, 0xF8, 0x100, 7);
	StoreImage(&r, SOME_IMAGE_DATA);
	DrawSync(0);
}

void modifySomeImage(int32_t dim)
{
	int16_t buffer[0x700];
	RECT rect;
	int16_t *src;
	int16_t *dst;
	int32_t i;

	src = (int16_t *)SOME_IMAGE_DATA;
	dst = buffer;
	for (i = 0; i < 0x700; i++) {
		int16_t pixel;
		int16_t red;
		int16_t green;
		int16_t blue;
		int16_t stp;

		pixel = *src;
		red = pixel & 0x1f;
		green = (pixel >> 5) & 0x1f;
		blue = (pixel >> 10) & 0x1f;
		stp = ((int16_t)pixel >> 15) & 1;
		do {
		} while (0);
		*dst = red * (255 - dim) / 255;
		src++;
		*dst += (green * (255 - dim) / 255) << 5;
		*dst += (blue * (255 - dim) / 255) << 10;
		*dst++ += stp << 15;
	}
	setRECT(&rect, 512, 248, 256, 7);
	LoadImage(&rect, (u_long *)buffer);
	DrawSync(0);
}

void findEFEDATFile(void)
{
	char name[0x40];
	CdlFILE file;
	uint8_t mode;
	int32_t i;

	i = 0;
	while (CdReadSync(1, 0) != 0) {
	}
	mode = 0x80;
	name[0] = 0x5C;
	strcpy(&name[1], PATH_ETCHI_EFEDAT_EFE);
	strcat(name, EFE_FILE_VERSION_SUFFIX);
	while ((int32_t)CdSearchFile(&file, name) == -1) {
	}
	CdControl(0xE, &mode, 0);
	MAIN_D_8012343C[i] = CdPosToInt(&file.pos);
}

void initializeEFE(void)
{
	setShortWithStride(EFE_LOADED_MOVE_DATA, -1, 0x11, 2);
	EFE_DATA_STACK = EFE_SCRIPT_MEM1_DATA;
	findEFEDATFile();
}

void getEFEDATEntry(int32_t id)
{
	CdlLOC loc;

	id -= 0x100;
	CdIntToPos(MAIN_D_8012343C[0] + (id * 0xA), &loc);
}

void renderParticleFlash(ParticleFlashData *params)
{
	POLY_FT4 *prim;
	int32_t sx;
	int32_t sy;
	GsOT_TAG *ot;
	int16_t r;
	int16_t g;
	int16_t b;
	int32_t rows;
	int32_t cellW;
	int32_t cellH;
	int32_t vStep;
	int32_t cols;
	int32_t uStep;
	int32_t h1;
	int32_t w1;
	int32_t i;
	int32_t j;
	int32_t px;
	int32_t py;

	if ((params->depth < 0x21) || (params->depth >= 0x1000)) {
		return;
	}
	ot = ACTIVE_ORDERING_TABLE->org;
	r = (params->color.r * params->colorScale) >> 7;
	g = (params->color.g * params->colorScale) >> 7;
	b = (params->color.b * params->colorScale) >> 7;
	w1 = (uint8_t)(params->sizeX - 1);
	h1 = (uint8_t)(params->sizeY - 1);
	if (abs(params->screenPos.vx) >= 0x400) {
		return;
	}
	if (abs(params->screenPos.vy) >= 0x200) {
		return;
	}
	sx = (params->scale * w1) >> 10;
	if (sx <= 0) {
		return;
	}
	sx += 1;
	sy = (params->scale * h1) >> 10;
	if (sy <= 0) {
		return;
	}
	sy += 1;
	if ((sx < 0x400) && (sy < 0x200)) {
		cols = 0;
	} else {
		cols = ((sx + 1) / 1024) + 1;
		if (cols != 0) {
			cols = (cols & ~3u) + 3;
		}
		if ((cols & 1) == 0) {
			cols++;
		}
	}
	rows = ((sy + 1) / 512) + 1;
	cellW = (sx + 1) / (cols + 1);
	cellH = (sy + 1) / (rows + 1);
	uStep = (w1 + 1) / (cols + 1);
	vStep = (h1 + 1) / (rows + 1);
	prim = (POLY_FT4 *)GsGetWorkBase();

	for (j = 0; j <= rows; j++) {
		py = params->screenPos.vy - sy + cellH * j;
		if ((py + cellH < -DRAWING_OFFSET_Y) || (py > -DRAWING_OFFSET_Y + 0xf0)) {
			continue;
		}
		for (i = 0; i <= cols; i++) {
			px = params->screenPos.vx - sx + cellW * i;
			if ((px + cellW < -DRAWING_OFFSET_X) || (px > -DRAWING_OFFSET_X + 0x140)) {
				continue;
			}
			SetPolyFT4(prim);
			SetSemiTrans(prim, 1);
			prim->tpage = params->tpage;
			prim->clut = params->clut;
			prim->x0 = px;
			prim->y0 = py;
			prim->x1 = px + cellW;
			prim->y1 = py;
			prim->x2 = px;
			prim->y2 = py + cellH;
			prim->x3 = px + cellW;
			prim->y3 = py + cellH;
			if (i == cols) {
				prim->x1 = prim->x3 = params->screenPos.vx;
			}
			if (j == rows) {
				prim->y2 = prim->y3 = params->screenPos.vy;
			}
			prim->r0 = r;
			prim->g0 = g;
			prim->b0 = b;
			prim->u0 = prim->u2 = params->uBase + uStep * i;
			prim->u1 = prim->u3 = params->uBase + uStep * (i + 1) - 1;
			prim->v0 = prim->v1 = params->vBase + vStep * j;
			prim->v2 = prim->v3 = params->vBase + vStep * (j + 1) - 1;
			AddPrim(&ot[params->depth], prim);
			prim++;
		}
	}

	for (j = 0; j <= rows; j++) {
		py = params->screenPos.vy - sy + cellH * j;
		if ((py + cellH < -DRAWING_OFFSET_Y) || (py > -DRAWING_OFFSET_Y + 0xf0)) {
			continue;
		}
		for (i = 0; i <= cols; i++) {
			px = params->screenPos.vx + sx - cellW * i;
			if ((px < -DRAWING_OFFSET_X) || (-DRAWING_OFFSET_X + 0x140 < px - cellW)) {
				continue;
			}
			SetPolyFT4(prim);
			SetSemiTrans(prim, 1);
			prim->tpage = params->tpage;
			prim->clut = params->clut;
			prim->x0 = px;
			prim->y0 = py;
			prim->x1 = px + (-cellW);
			prim->y1 = py;
			prim->x2 = px;
			prim->y2 = py + cellH;
			prim->x3 = px + (-cellW);
			prim->y3 = py + cellH;
			if (i == cols) {
				prim->x1 = prim->x3 = params->screenPos.vx;
			}
			if (j == rows) {
				prim->y2 = prim->y3 = params->screenPos.vy;
			}
			prim->r0 = r;
			prim->g0 = g;
			prim->b0 = b;
			prim->u0 = prim->u2 = params->uBase + uStep * i;
			prim->u1 = prim->u3 = params->uBase + uStep * (i + 1) - 1;
			prim->v0 = prim->v1 = params->vBase + vStep * j;
			prim->v2 = prim->v3 = params->vBase + vStep * (j + 1) - 1;
			AddPrim(&ot[params->depth], prim);
			prim++;
		}
	}

	for (j = 0; j <= rows; j++) {
		py = params->screenPos.vy + sy - cellH * j;
		if ((py < -DRAWING_OFFSET_Y) || (-DRAWING_OFFSET_Y + 0xf0 < py - cellH)) {
			continue;
		}
		for (i = 0; i <= cols; i++) {
			px = params->screenPos.vx - sx + cellW * i;
			if ((px + cellW < -DRAWING_OFFSET_X) || (px > -DRAWING_OFFSET_X + 0x140)) {
				continue;
			}
			SetPolyFT4(prim);
			SetSemiTrans(prim, 1);
			prim->tpage = params->tpage;
			prim->clut = params->clut;
			prim->x0 = px;
			prim->y0 = py;
			prim->x1 = px + cellW;
			prim->y1 = py;
			prim->x2 = px;
			prim->y2 = py + (-cellH);
			prim->x3 = px + cellW;
			prim->y3 = py + (-cellH);
			if (i == cols) {
				prim->x1 = prim->x3 = params->screenPos.vx;
			}
			if (j == rows) {
				prim->y2 = prim->y3 = params->screenPos.vy;
			}
			prim->r0 = r;
			prim->g0 = g;
			prim->b0 = b;
			prim->u0 = prim->u2 = params->uBase + uStep * i;
			prim->u1 = prim->u3 = params->uBase + uStep * (i + 1) - 1;
			prim->v0 = prim->v1 = params->vBase + vStep * j;
			prim->v2 = prim->v3 = params->vBase + vStep * (j + 1) - 1;
			AddPrim(&ot[params->depth], prim);
			prim++;
		}
	}

	for (j = 0; j <= rows; j++) {
		py = params->screenPos.vy + sy - cellH * j;
		if ((py < -DRAWING_OFFSET_Y) || (-DRAWING_OFFSET_Y + 0xf0 < py - cellH)) {
			continue;
		}
		for (i = 0; i <= cols; i++) {
			px = params->screenPos.vx + sx - cellW * i;
			if ((px < -DRAWING_OFFSET_X) || (-DRAWING_OFFSET_X + 0x140 < px - cellW)) {
				continue;
			}
			SetPolyFT4(prim);
			SetSemiTrans(prim, 1);
			prim->tpage = params->tpage;
			prim->clut = params->clut;
			prim->x0 = px;
			prim->y0 = py;
			prim->x1 = px + (-cellW);
			prim->y1 = py;
			prim->x2 = px;
			prim->y2 = py + (-cellH);
			prim->x3 = px + (-cellW);
			prim->y3 = py + (-cellH);
			if (i == cols) {
				prim->x1 = prim->x3 = params->screenPos.vx;
			}
			if (j == rows) {
				prim->y2 = prim->y3 = params->screenPos.vy;
			}
			prim->r0 = r;
			prim->g0 = g;
			prim->b0 = b;
			prim->u0 = prim->u2 = params->uBase + uStep * i;
			prim->u1 = prim->u3 = params->uBase + uStep * (i + 1) - 1;
			prim->v0 = prim->v1 = params->vBase + vStep * j;
			prim->v2 = prim->v3 = params->vBase + vStep * (j + 1) - 1;
			AddPrim(&ot[params->depth], prim);
			prim++;
		}
	}

	GsSetWorkBase((PACKET *)prim);
}
