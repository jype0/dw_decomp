#include <stdlib.h>

#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/anim.h>
#include <dw/btl.h>
#include <dw/efe.h>
#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/file_queue.h>
#include <dw/graphics.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/rng.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/sound_async.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

#define EVL_MMD_BUFFER	0x80020000

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

extern int8_t HAS_USED_EVOITEM;
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
extern EfeFlashBuffer EVL_D_800677B0;
extern int16_t EVL_D_80067994[40];
extern GsRVIEW2 GS_VIEWPOINT;
extern GsRVIEW2 EVL_D_800688E8;
extern int32_t VIEWPORT_DISTANCE;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern SVECTOR MAIN_D_801349F8;
extern SVECTOR MAIN_D_80134A00;
extern GsRVIEW2 EVL_D_80068918;
extern int16_t EVL_D_800679E4[];
extern RGB8 MAIN_D_801349E8;
extern SVECTOR MAIN_D_801349EC;
extern uint8_t CURRENT_SCREEN;
extern int32_t MAIN_D_801349E4;
extern VECTOR EVL_D_80068908;
int32_t getMapSoundId(int32_t mapId);
void EFECreateFlash(void);
void forceUpdateBGM(void);
void fadeoutCLUT1(int32_t level, int16_t *src, int32_t unused);
void fadeoutCLUT2(int32_t fade, char *src, int32_t unused);
void setMapLayerEnabled(int32_t enabled);

void initializeEvolvedPartner(int32_t type, int32_t posX, int32_t posY, int32_t posZ,
                              int32_t rotationX, int32_t rotationY, int32_t rotationZ);
void downloadCLUT1(int16_t *clut);
void downloadCLUT2(char *base);
char *initializeFlashData(char *base);
void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
int32_t addScreenPolyFT4(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
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
void EVL_storeClutBank1(int32_t buffer);
void EVL_releaseAllParticles(void);
void EVL_tickParticle(int32_t id);
void EVL_storeDigimonClut(int32_t buffer, Entity *entity);
void EVL_storeClutBank0(int32_t buffer);
void EVL_setOtherEntitiesVisible(int32_t restore);
int32_t EVL_spawnParticle(VECTOR *position, RGB8 *color);
void EVL_renderParticle(int32_t id);
int32_t EVL_initShardSets(int32_t base);
void EVL_tickEvoSequence(int32_t instanceId);
void EVL_fadeClutBank0(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame);
void EVL_fadeClutBank1(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame);
void EVL_updateEvoCamera(Entity *entity, int32_t unused, int32_t frame);
int32_t EVL_buildShardSet(Entity *entity, int32_t objIndex, int32_t bone);
int32_t EVL_spawnSpark(Entity *entity, int32_t bone, int32_t timer);
void EVL_calculateCameraVectors(VECTOR *viewRef, VECTOR *viewPos, Entity *entity, SVECTOR *rotation, int32_t distance, int32_t height);
void EVL_tickShardSet(int32_t id);
void EVL_renderShardSet(int32_t index);
void EVL_renderTriShard(EvlModelVertex *drift, int32_t unused1, int16_t speed, int16_t timer, ModelComponent *model);
void EVL_renderQuadShard(EvlModelVertex *drift, int32_t unused1, int16_t speed, int16_t timer, ModelComponent *model);
void EVL_renderSparkStreak(int32_t id);
void EVL_tickSpark(int32_t id);
void EVL_applyEvolution(Entity *entity, Stats *stats, PartnerPara *para, int16_t digimonId);
void EVL_scaleBaseStats(Stats *stats, int16_t pct, int16_t unused);
void EVL_clampBaseStats(void);
void EVL_renderEvoSequence(void);

static void *evl_functions[] = {
	EVL_clampBaseStats,
	EVL_scaleBaseStats,
	EVL_applyEvolution,
	EVL_initEvoSequence,
#if defined(VERSION_JP)
	EVL_renderSparkStreak,
	EVL_tickSpark,
#else
	EVL_tickSpark,
	EVL_renderSparkStreak,
#endif
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

int32_t MAIN_D_801349E4 = EVL_MMD_BUFFER;
RGB8 MAIN_D_801349E8 = { 0x0a, 0xff, 0x0a };
SVECTOR MAIN_D_801349EC = { 0 };
int8_t MAIN_D_801349F4 = 1;
SVECTOR MAIN_D_801349F8 = { 0 };
SVECTOR MAIN_D_80134A00 = { 0 };

int32_t MAIN_D_801351E4;
int32_t MAIN_D_801351E8;
int32_t MAIN_D_801351EC;
SVECTOR MAIN_D_801351F0;
int32_t MAIN_D_801351F8;
int32_t MAIN_D_801351FC;
int32_t MAIN_D_80135200;
int32_t MAIN_D_80135204;
EvlShardSet *MAIN_D_80135208;
int32_t MAIN_D_8013520C;
uint8_t *MAIN_D_80135210;
EvlModelVertex *MAIN_D_80135214;
int16_t MAIN_D_80135218[3];

static void *evl_sbss_order[] = {
	&MAIN_D_80135218,
	&MAIN_D_80135214,
	&MAIN_D_80135210,
	&MAIN_D_8013520C,
	&MAIN_D_80135208,
	&MAIN_D_80135204,
	&MAIN_D_80135200,
	&MAIN_D_801351FC,
	&MAIN_D_801351F8,
	&MAIN_D_801351F0,
	&MAIN_D_801351EC,
	&MAIN_D_801351E8,
	&MAIN_D_801351E4,
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

int8_t EVL_D_80063F28[18] = {
	0x01, 0x02, 0x03, 0x04, 0x05, 0x04, 0x03, 0x02,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00,
};
// clang-format on

void EVL_storeDigimonClut(int32_t buffer, Entity *entity)
{
	ModelComponent *model;
	TMDModel *tmd;
	RECT rect;

	model = getEntityModelComponent(entity->type, getEntityType(entity));
	tmd = model->modelPtr;
	setRECT(&rect, (model->clutPage & 0x3f) << 4, model->clutPage >> 6, 0x10, 0x18);
	StoreImage(&rect, (u_long *)buffer);

	DrawSync(0);
}

void EVL_storeClutBank0(int32_t buffer)
{
	RECT rect;

	setRECT(&rect, 0, 488, 16, 24);
	StoreImage(&rect, (u_long *)buffer);
	DrawSync(0);
}

void EVL_storeClutBank1(int32_t buffer)
{
	RECT rect;

	setRECT(&rect, 32, 488, 48, 24);
	StoreImage(&rect, (u_long *)buffer);
	DrawSync(0);
}

int32_t EVL_initShardSets(int32_t base)
{
	int32_t i;

	if ((base & 3) != 0) {
		base += 4 - (base & 3L);
	}

	MAIN_D_80135208 = (EvlShardSet *)base;
	base += 0x2d0;
	for (i = 0; i < 30; i++) {
		MAIN_D_80135208[i].timer = -1;
	}

	return base;
}

void EVL_setScratchTop(int32_t size)
{
	if ((size & 3) != 0) {
		size += 4 - (size & 3L);
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

void EVL_tickEvoSequence(int32_t instanceId)
{
	int32_t i;
	int32_t tmp;
	int32_t size;
	int32_t height;
	int32_t bone;
	int32_t obj;
	MATRIX *workm;
	VECTOR colorStart;
	VECTOR colorEnd;
	VECTOR viewRef;
	VECTOR viewPos;
	int32_t alpha;
	int32_t unused[3];
	RGB8 color;
	VECTOR *scale;
	int32_t rRatio;
	int32_t hRatio;
	SVECTOR rot;
	int32_t camSize;
	EvoSequenceData *data;
	PartnerEntity *partner;
	int32_t frame;

	data = &EVO_SEQUENCE_DATA;
	partner = data->partner;
	frame = data->timer;
	if (0) {
		unused[0] = 0;
	}

	if ((partner->digimonEntity.entity.anim.animId == 0xc) &&
	    (partner->digimonEntity.entity.anim.animFrame == partner->digimonEntity.entity.anim.frameCount)) {
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 1);
	}

	switch (data->state) {
	case 0:
		if (frame < 0x20) {
			alpha = lerp(0, 0xff, 0, 0x20, frame);
		} else {
			alpha = 0xff;
		}
		fadeoutCLUT1(alpha, EVL_D_80063F3C, 0);
		fadeoutCLUT2(alpha, EVL_D_80064D50, 0);
		if ((frame & 1) == 0) {
			EVL_fadeClutBank0((int16_t *)EVL_D_80065398, partner, EVL_D_800679E4, 0, 0x20, data->timer);
		} else {
			EVL_fadeClutBank1((int16_t *)EVL_D_8006569C, partner, EVL_D_800679E4, 0, 0x20, data->timer);
		}
		if (data->timer < 0x20) {
			break;
		}
		data->state = 1;
		EVL_setOtherEntitiesVisible(0);
		setMapLayerEnabled(0);
		EVL_D_800688E8 = GS_VIEWPOINT;
		MAIN_D_801351E4 = DRAWING_OFFSET_X;
		MAIN_D_801351E8 = DRAWING_OFFSET_Y;
		MAIN_D_801351EC = VIEWPORT_DISTANCE;
		EVL_D_80068908 = ENTITY_TABLE[1]->posData->location;
		MAIN_D_801351F0 = ENTITY_TABLE[1]->posData->rotation;
		loadMMDAsync(data->digimonId, 3, MAIN_D_801349E4, &data->modelData, (uint8_t *)&data->hasFinishedLoading);
		break;
	case 1:
		EVL_updateEvoCamera(&partner->digimonEntity.entity, 0, data->timer);
		if (data->timer != 0x6a) {
			break;
		}
		data->state = 2;
		playSound2(8, 1);
		playSound2(8, 0);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0xc);
		goto shards;
	case 2:
		if (EVL_D_80063F28[frame % 18] != 0) {
			color = MAIN_D_801349E8;
			color.g = lerp(0x32, 0xe6, 1, 5, EVL_D_80063F28[frame % 18]);
			EVL_spawnParticle(&EVL_D_80064D40, &color);
		}
		if ((data->timer >= 0x70) && (data->timer < 0xcdL)) {
			EVL_brightenDigimonClut((int16_t *)EVL_D_80065094, &partner->digimonEntity.entity, EVL_D_800679E4, 0x70, 0x9a, data->timer);
		}
		if ((data->timer >= 0x70) && (data->timer < 0x9bL)) {
			tmp = EVL_D_80063EFC[data->timer - 0x70L];
			if (tmp >= 0) {
shards:
				if (data->unk_0x8 < DIGIMON_DATA[partner->digimonEntity.entity.type].boneCount - 1) {
					bone = EVL_D_80067994[data->unk_0x8++];
					obj = DIGIMON_SKELETONS[partner->digimonEntity.entity.type][bone].objIndex;
					if (obj == -1) {
						goto shards;
					}
					playSound2(8, (rand() % 3) + 2);
					EVL_buildShardSet(&partner->digimonEntity.entity, obj, bone);
					PARTNER_WIREFRAME_SUB[bone] = 0;
					EVL_spawnSpark(&partner->digimonEntity.entity, (int16_t)bone, (int16_t)(0x11c - data->timer));
					workm = &partner->digimonEntity.entity.posData[bone].posMatrix.workm;
					*EFE_DATA_STACK++ = 1;
					*EFE_DATA_STACK++ = (int32_t)workm->t;
					*EFE_DATA_STACK++ = -1;
					*EFE_DATA_STACK++ = 0xa;
					*EFE_DATA_STACK++ = 0;
					*EFE_DATA_STACK++ = 0x333;
					colorStart.vx = colorStart.vy = colorStart.vz = 0x96;
					colorEnd.vx = colorEnd.vy = colorEnd.vz = 0x14;
					*EFE_DATA_STACK++ = (int32_t)&colorStart;
					*EFE_DATA_STACK++ = (int32_t)&colorEnd;
					EFECreateFlash();
				}
			}
		} else if ((int32_t)*(uint8_t **)&data->timer >= 0x9b) {
			if ((int32_t)*(uint8_t **)&data->timer < 0x11c) {
				goto shards;
			}
		}
		if ((int32_t)*(uint8_t **)&data->timer < 0xcc) {
			tmp = DIGIMON_DATA[partner->digimonEntity.entity.type].radius;
			size = DIGIMON_DATA[partner->digimonEntity.entity.type].height;
			height = DIGIMON_DATA[partner->digimonEntity.entity.type].height;
		} else if (((int32_t)*(uint8_t **)&data->timer >= 0xcc) && ((int32_t)*(uint8_t **)&data->timer < 0x108)) {
			scale = &partner->digimonEntity.entity.posData->scale;
			rRatio = (DIGIMON_DATA[data->digimonId].radius << 12) / DIGIMON_DATA[partner->digimonEntity.entity.type].radius;
			hRatio = (DIGIMON_DATA[data->digimonId].height << 12) / DIGIMON_DATA[partner->digimonEntity.entity.type].height;
			scale->vx = lerp(0x1000, rRatio, 0xcc, 0x108, data->timer);
			scale->vy = lerp(0x1000, hRatio, 0xcc, 0x108, data->timer);
			scale->vz = scale->vx;
			tmp = lerp(DIGIMON_DATA[partner->digimonEntity.entity.type].radius, DIGIMON_DATA[data->digimonId].radius, 0xcc, 0x108, data->timer);
			size = lerp(DIGIMON_DATA[partner->digimonEntity.entity.type].height, DIGIMON_DATA[data->digimonId].height, 0xcc, 0x108, data->timer);
			height = lerp(DIGIMON_DATA[partner->digimonEntity.entity.type].height, DIGIMON_DATA[data->digimonId].height, 0xcc, 0x108, data->timer);
		} else if ((int32_t)*(uint8_t **)&data->timer >= 0x108) {
			tmp = DIGIMON_DATA[data->digimonId].radius;
			size = DIGIMON_DATA[data->digimonId].height;
			height = DIGIMON_DATA[data->digimonId].height;
		}
		if (data->timer >= 0xa5) {
			if (data->timer >= 0xc3) {
				MAIN_D_801351F8 += 0x16;
			} else {
				MAIN_D_801351F8 += lerp(0, 0x16, 0xa4, 0xc2, (int32_t)*(uint8_t **)&data->timer);
			}
			rot = MAIN_D_801349EC;
			rot.vy = MAIN_D_801351F8 + partner->digimonEntity.entity.posData->rotation.vy;
			if (tmp < size) {
				camSize = size;
			} else {
				camSize = tmp;
			}
			camSize = (camSize * 5) + 0x4b0;
			EVL_calculateCameraVectors(&viewRef, &viewPos, &partner->digimonEntity.entity, &rot, camSize, height);
			GS_VIEWPOINT.vrx = viewRef.vx;
			GS_VIEWPOINT.vry = viewRef.vy;
			GS_VIEWPOINT.vrz = viewRef.vz;
			GS_VIEWPOINT.vpx = viewPos.vx;
			GS_VIEWPOINT.vpy = viewPos.vy;
			GS_VIEWPOINT.vpz = viewPos.vz;
		}
		if (frame == 0xd6) {
			stopSound();
			playSound(8, 5);
		}
		if ((frame >= 0xd7) && (frame < 0x11c) && ((frame & 1) == 0)) {
			workm = &partner->digimonEntity.entity.posData[1].posMatrix.workm;
			*EFE_DATA_STACK++ = 0;
			*EFE_DATA_STACK++ = (int32_t)workm->t;
			tmp = lerp(5, 1, 0xd6, 0x11c, frame);
			if ((frame & tmp) == 0) {
				*EFE_DATA_STACK++ = 0x21;
			} else {
				*EFE_DATA_STACK++ = 0xfa0;
			}
			*EFE_DATA_STACK++ = 0xa;
			*EFE_DATA_STACK++ = 0;
			tmp = lerp(0x333, 0x4800, 0xd6, 0x11c, frame);
			*EFE_DATA_STACK++ = tmp;
			tmp = lerp(0x32, 0x78, 0xd6, 0x11c, frame);
			colorStart.vx = tmp + (rand() % 100);
			colorStart.vy = tmp + (rand() % 100);
			colorStart.vz = tmp + (rand() % 100);
			colorEnd.vx = colorEnd.vy = colorEnd.vz = 0x14;
			*EFE_DATA_STACK++ = (int32_t)&colorStart;
			*EFE_DATA_STACK++ = (int32_t)&colorEnd;
			EFECreateFlash();
		}
		if (frame == 0x11c) {
			PARTNER_WIREFRAME_TOTAL = 0;
			for (i = 0; i < 0x28; i++) {
				PARTNER_WIREFRAME_SUB[i] = 0x10;
			}
			while (data->hasFinishedLoading != 0) {
				tickFileReadQueue(0);
			}
			EVL_applyEvolution(ENTITY_TABLE[1], &PARTNER_ENTITY.digimonEntity.stats, data->para, data->evoTarget);
			waitForSoundBufferLoading(3);
			partner = (PartnerEntity *)((uint32_t)&PARTNER_ENTITY);
			data->partner = partner;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
		}
		if ((frame >= 0x126) && (frame < 0x14f)) {
			PARTNER_WIREFRAME_TOTAL = lerp(0, 0x10, 0x126, 0x14e, frame);
		}
		if (frame < 0x162) {
			break;
		}
		data->state = 3;
		PARTNER_WIREFRAME_TOTAL = 0x10;
		EVL_D_80068918 = GS_VIEWPOINT;
		MAIN_D_801351FC = DRAWING_OFFSET_X;
		MAIN_D_80135200 = DRAWING_OFFSET_Y;
		MAIN_D_80135204 = VIEWPORT_DISTANCE;
		GS_VIEWPOINT = EVL_D_800688E8;
		DRAWING_OFFSET_X = MAIN_D_801351E4;
		DRAWING_OFFSET_Y = MAIN_D_801351E8;
		VIEWPORT_DISTANCE = MAIN_D_801351EC;
		break;
	case 3:
		data->state = 4;
		GS_VIEWPOINT = EVL_D_800688E8;
		DRAWING_OFFSET_X = MAIN_D_801351E4;
		DRAWING_OFFSET_Y = MAIN_D_801351E8;
		VIEWPORT_DISTANCE = MAIN_D_801351EC;
		fadeoutCLUT1(0, EVL_D_80063F3C, 0);
		fadeoutCLUT2(0, EVL_D_80064D50, 0);
		EVL_fadeClutBank0((int16_t *)EVL_D_80065398, partner, EVL_D_800679E4, 0, 1, 0);
		EVL_fadeClutBank1((int16_t *)EVL_D_8006569C, partner, EVL_D_800679E4, 0, 1, 0);
		EVL_setOtherEntitiesVisible(1);
		setMapLayerEnabled(1);
		forceUpdateBGM();
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0xb);
		EVL_releaseAllParticles();
		loadMapSounds2(getMapSoundId(CURRENT_SCREEN));
		break;
	case 4:
		if (partner->digimonEntity.entity.anim.animFrame == partner->digimonEntity.entity.anim.frameCount) {
			isSoundLoaded(0, 8);
			data->timer = -1;
			removeObject(0x80a, instanceId);
			return;
		}
		break;
	}

	data->timer += 1;
}

void EVL_renderEvoSequence(void)
{
}

void EVL_fadeClutBank0(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame)
{
	RECT rect;
	int32_t i;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t stp;
	int16_t noiseR;
	int16_t noiseG;
	int16_t noiseB;
	int16_t *src;
	int16_t *dst;

	src = srcClut;
	dst = dstClut;
	noiseR = rand() % 100;
	noiseG = rand() % 100;
	noiseB = rand() % 100;
	for (i = 0; i < 384; i++) {
		r = *src & 0x1f;
		g = (*src >> 5) & 0x1f;
		b = (*src >> 10) & 0x1f;
		stp = (*src++ >> 15) & 0x1;

		if (frame != startFrame) {
			stp = 1;
		}
		r = (int16_t)r * (endFrame - frame) / (endFrame - startFrame);
		g = (int16_t)g * (endFrame - frame) / (endFrame - startFrame);
		b = (int16_t)b * (endFrame - frame) / (endFrame - startFrame);

		*dst = r;
		*dst += g << 5;
		*dst += b << 10;
		*dst++ += stp << 15;
	}

	setRECT(&rect, 0, 488, 16, 24);
	LoadImage(&rect, (u_long *)dstClut);
}

void EVL_fadeClutBank1(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame)
{
	RECT rect;
	int32_t i;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t stp;
	int16_t noiseR;
	int16_t noiseG;
	int16_t noiseB;
	int16_t *src;
	int16_t *dst;

	src = srcClut;
	dst = dstClut;
	noiseR = rand() % 100;
	noiseG = rand() % 100;
	noiseB = rand() % 100;
	for (i = 0; i < 1152; i++) {
		r = *src & 0x1f;
		g = (*src >> 5) & 0x1f;
		b = (*src >> 10) & 0x1f;
		stp = (*src++ >> 15) & 0x1;

		if (frame != startFrame) {
			stp = 1;
		}
		r = (int16_t)r * (endFrame - frame) / (endFrame - startFrame);
		g = (int16_t)g * (endFrame - frame) / (endFrame - startFrame);
		b = (int16_t)b * (endFrame - frame) / (endFrame - startFrame);

		*dst = r;
		*dst += g << 5;
		*dst += b << 10;
		*dst++ += stp << 15;
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
	VECTOR viewRef;
	VECTOR viewPos;

	if (frame >= 0x11c) {
		return;
	}

	if ((frame >= 0x20) && (frame < 0x61)) {
		int32_t t;
		int32_t start;
		int32_t mid;
		int32_t end;
		SVECTOR pos;
		DVECTOR screen;
		SVECTOR rot;
		int32_t size;

		start = 0x20;
		mid = 0x40;
		end = 0x60;
		pos.vx = entity->posData->location.vx;
		pos.vy = entity->posData->location.vy;
		pos.vz = entity->posData->location.vz;
		worldPosToScreenPos(&pos, &screen);
		MAIN_D_801351F8 = 0x638;
		rot = MAIN_D_801349F8;
		rot.vy = MAIN_D_801351F8 + entity->posData->rotation.vy;
		if (DIGIMON_DATA[entity->type].height < DIGIMON_DATA[entity->type].radius) {
			size = DIGIMON_DATA[entity->type].radius;
		} else {
			size = DIGIMON_DATA[entity->type].height;
		}
		size = (size * 5) + 0x4b0;
		EVL_calculateCameraVectors(&viewRef, &viewPos, entity, &rot, size, DIGIMON_DATA[entity->type].height);
		if (frame <= mid) {
			GS_VIEWPOINT.vrx = lerp(EVL_D_800688E8.vrx, viewRef.vx, start, mid, frame);
			GS_VIEWPOINT.vry = lerp(EVL_D_800688E8.vry, viewRef.vy, start, mid, frame);
			GS_VIEWPOINT.vrz = lerp(EVL_D_800688E8.vrz, viewRef.vz, start, mid, frame);
			GS_VIEWPOINT.rz = 0;
			DRAWING_OFFSET_X = lerp(MAIN_D_801351E4, 0xa0, start, mid, frame);
			DRAWING_OFFSET_Y = lerp(MAIN_D_801351E8, 0x78, start, mid, frame);
		} else if (frame <= end) {
			GS_VIEWPOINT.vpx = lerp(EVL_D_800688E8.vpx, viewPos.vx, mid, end, frame);
			GS_VIEWPOINT.vpy = lerp(EVL_D_800688E8.vpy, viewPos.vy, mid, end, frame);
			GS_VIEWPOINT.vpz = lerp(EVL_D_800688E8.vpz, viewPos.vz, mid, end, frame);
			t = lerp(0, 0x14, 0, 0xc8, DIGIMON_DATA[entity->type].height);
			DRAWING_OFFSET_Y = lerp(0x78, t + 0x78, mid, end, frame);
			VIEWPORT_DISTANCE = lerp(MAIN_D_801351EC, 0x3e8, mid, end, frame);
		}
	}

	if (frame >= 0x61) {
		SVECTOR rot;
		int32_t size;

		MAIN_D_801351F8 += 0x16;
		rot = MAIN_D_80134A00;
		rot.vy = MAIN_D_801351F8 + entity->posData->rotation.vy;
		if (DIGIMON_DATA[entity->type].height < DIGIMON_DATA[entity->type].radius) {
			size = DIGIMON_DATA[entity->type].radius;
		} else {
			size = DIGIMON_DATA[entity->type].height;
		}
		size = (size * 5) + 0x4b0;
		EVL_calculateCameraVectors(&viewRef, &viewPos, entity, &rot, size, DIGIMON_DATA[entity->type].height);
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

// clang-format off
int32_t EVL_spawnSpark(entity, bone, timer)
	Entity *entity;
	int16_t bone;
	int16_t timer;
// clang-format on
{
	int32_t i;
	EvlSpark *spark;

	for (i = 0; i < 16; i++) {
		if (EVL_D_80068F84[i].bone == -1) {
			break;
		}
	}

	if (i == 16) {
		return -1;
	}

	spark = &EVL_D_80068F84[i];
	spark->bone = bone;
	spark->entity = entity;
	spark->timer = timer;
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

	p = &MAIN_D_80135208[id].timer;
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

	entry = &MAIN_D_80135208[index];
	shards = entry->centers;
	model = getEntityModelComponent(entry->model[0], 3);
	count = entry->centerCount;
	MAIN_D_80135210 = (uint8_t *)entry->primitives;
	MAIN_D_80135214 = (EvlModelVertex *)entry->vertices;
	MAIN_D_80135218[0] = ((((31 - entry->timer) * 74) / 30) + 54);
	MAIN_D_80135218[1] = MAIN_D_80135218[0];
	MAIN_D_80135218[2] = MAIN_D_80135218[0];

	while (count-- > 0) {
		if ((((int8_t *)MAIN_D_80135210)[3] == 0x34) || (((int8_t *)MAIN_D_80135210)[3] == 0x36)) {
			EVL_renderTriShard((EvlModelVertex *)shards, 0, 60, entry->timer, model);
			shards += 6;
			MAIN_D_80135210 += 0x1c;
		} else if ((((int8_t *)MAIN_D_80135210)[3] == 0x3c) || (((int8_t *)MAIN_D_80135210)[3] == 0x3e)) {
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
	POLY_FT3 *prim;
	EvlModelVertex *v;
	TMD_P_TG3 *tri;
	SVECTOR offset;

	tri = (TMD_P_TG3 *)MAIN_D_80135210;
	prim = (POLY_FT3 *)GsGetWorkBase();
	SetPolyFT3(prim);
	SetSemiTrans(prim, 1);
	setRGB0(prim, MAIN_D_80135218[0], MAIN_D_80135218[1], MAIN_D_80135218[2]);
	prim->tpage = model->pixelPage;
	prim->clut = tri->clut;
	setUV3(prim, tri->tu0, tri->tv0, tri->tu1, tri->tv1, tri->tu2, tri->tv2);

	offset.vx = drift->vx * timer / speed;
	offset.vy = drift->vy * timer / speed;
	offset.vz = drift->vz * timer / speed;

	v = &MAIN_D_80135214[tri->v0];
	a.vx = v->vx + offset.vx;
	a.vy = v->vy + offset.vy;
	a.vz = v->vz + offset.vz;
	v = &MAIN_D_80135214[tri->v1];
	b.vx = v->vx + offset.vx;
	b.vy = v->vy + offset.vy;
	b.vz = v->vz + offset.vz;
	v = &MAIN_D_80135214[tri->v2];
	c.vx = v->vx + offset.vx;
	c.vy = v->vy + offset.vy;
	c.vz = v->vz + offset.vz;
	prim->code |= 2;
	addScreenPolyFT3(prim, &a, &b, &c);
}

void EVL_brightenDigimonClut(int16_t *clut, Entity *entity, int16_t *dst, int32_t start, int32_t end, int32_t t)
{
	ModelComponent *model;
	TMDModel *tmd;
	RECT rect;
	int16_t red;
	int16_t green;
	int16_t blue;
	int16_t stp;
	int16_t redFactor;
	int16_t greenFactor;
	int16_t blueFactor;
	int16_t *src;
	int32_t i;

	model = getEntityModelComponent(entity->type, getEntityType(entity));
	tmd = model->modelPtr;
	src = clut;
	if (t > end) {
		t = end;
	}

	redFactor = rand() % 100;
	greenFactor = rand() % 100;
	blueFactor = rand() % 100;
	for (i = 0; i < 0x180; i++) {
		red = *src & 0x1f;
		green = (*src >> 5) & 0x1f;
		blue = (*src >> 10) & 0x1f;
		stp = (*src++ >> 15) & 1;
		if (red || green || blue) {
			if (t != start) {
				stp = 1;
			}

			red += redFactor * (((0x1f - red) * (t - start)) / (end - start)) / 100;
			green += greenFactor * (((0x1f - green) * (t - start)) / (end - start)) / 100;
			blue += blueFactor * (((0x1f - blue) * (t - start)) / (end - start)) / 100;
		}

		dst[i] = red;
		dst[i] += green << 5;
		dst[i] += blue << 10;
		dst[i] += stp << 15;
	}

	setRECT(&rect, (model->clutPage & 0x3f) << 4, model->clutPage >> 6, 0x10, 0x18);
	LoadImage(&rect, (u_long *)dst);
}

int32_t EVL_buildShardSet(Entity *entity, int32_t objIndex, int32_t bone)
{
	ModelComponent *model;
	TMDModel *tmd;
	TMDModel *header;
	struct TMD_STRUCT *objects;
	int32_t outStart;
	SVECTOR tmp;
	MATRIX m1;
	int32_t slot;
	SVECTOR *src;
	int32_t i;
	MATRIX m2;
	int32_t prim;
	int32_t j;
	SVECTOR *va;
	SVECTOR *vb;
	SVECTOR *vc;
	SVECTOR *vd;
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
	SVECTOR center;
	int32_t tri;
	int32_t quad;
	struct TMD_STRUCT *obj;
	int32_t out;
	EvlShardSet *entry;

	out = MAIN_D_8013520C;
	model = getEntityModelComponent(entity->type, 3);
	tmd = model->modelPtr;
	header = tmd;
	objects = (struct TMD_STRUCT *)((uint32_t)header + 12);
	obj = &objects[objIndex];

	for (slot = 0; slot < 30; slot++) {
		if (MAIN_D_80135208[slot].timer < 0) {
			break;
		}
	}
	if (slot == 30) {
		return -1;
	}

	calculateBoneMatrix(entity, bone, &m1);
	outStart = out;
	src = (SVECTOR *)obj->vertop;
	calculateBoneMatrix(entity, bone, &m2);
	for (i = 0; (uint32_t)i < obj->vern; i++) {
		ApplyMatrixSV(&m2, src++, &tmp);
		((EvlModelVertex *)out)->vx = tmp.vx + m2.t[0];
		((EvlModelVertex *)out)->vy = tmp.vy + m2.t[1];
		((EvlModelVertex *)out)->vz = tmp.vz + m2.t[2];
		out += sizeof(EvlModelVertex);
	}

	entry = &MAIN_D_80135208[slot];
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
			tri = prim;
			va = &((SVECTOR *)obj->nortop)[((TMD_P_TG3 *)tri)->n0];
			vb = &((SVECTOR *)obj->nortop)[((TMD_P_TG3 *)tri)->n1];
			vc = &((SVECTOR *)obj->nortop)[((TMD_P_TG3 *)tri)->n2];
			ApplyMatrixSV(&m1, va, &p0);
			ApplyMatrixSV(&m1, vb, &p1);
			ApplyMatrixSV(&m1, vc, &p2);
			center.vx = (p0.vx + p1.vx + p2.vx) / 3;
			center.vy = (p0.vy + p1.vy + p2.vy) / 3;
			center.vz = (p0.vz + p1.vz + p2.vz) / 3;
			((EvlModelVertex *)out)->vx = center.vx;
			((EvlModelVertex *)out)->vy = center.vy;
			((EvlModelVertex *)out)->vz = center.vz;
			out += sizeof(EvlModelVertex);
			prim += sizeof(TMD_P_TG3);
			break;
		case 0x3c:
		case 0x3e:
			quad = prim;
			va = &((SVECTOR *)obj->nortop)[((TMD_P_TG4 *)quad)->n0];
			vb = &((SVECTOR *)obj->nortop)[((TMD_P_TG4 *)quad)->n1];
			vc = &((SVECTOR *)obj->nortop)[((TMD_P_TG4 *)quad)->n2];
			vd = &((SVECTOR *)obj->nortop)[((TMD_P_TG4 *)quad)->n3];
			ApplyMatrixSV(&m1, va, &p0);
			ApplyMatrixSV(&m1, vb, &p1);
			ApplyMatrixSV(&m1, vc, &p2);
			ApplyMatrixSV(&m1, vd, &p3);
			center.vx = (p0.vx + p1.vx + p2.vx) / 3;
			center.vy = (p0.vy + p1.vy + p2.vy) / 3;
			center.vz = (p0.vz + p1.vz + p2.vz) / 3;
			((EvlModelVertex *)out)->vx = center.vx;
			((EvlModelVertex *)out)->vy = center.vy;
			((EvlModelVertex *)out)->vz = center.vz;
			out += sizeof(EvlModelVertex);
			prim += sizeof(TMD_P_TG4);
			break;
		}
	}

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
	SVECTOR offset;

	tri = (TMD_P_TG4 *)MAIN_D_80135210;
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	setRGB0(prim, MAIN_D_80135218[0], MAIN_D_80135218[1], MAIN_D_80135218[2]);
	prim->tpage = model->pixelPage;
	prim->clut = tri->clut;
	setUV4(prim, tri->tu0, tri->tv0, tri->tu1, tri->tv1, tri->tu2, tri->tv2, tri->tu3,
	       tri->tv3);

	offset.vx = drift->vx * timer / speed;
	offset.vy = drift->vy * timer / speed;
	offset.vz = drift->vz * timer / speed;

	v = &MAIN_D_80135214[tri->v0];
	a.vx = v->vx + offset.vx;
	a.vy = v->vy + offset.vy;
	a.vz = v->vz + offset.vz;
	v = &MAIN_D_80135214[tri->v1];
	b.vx = v->vx + offset.vx;
	b.vy = v->vy + offset.vy;
	b.vz = v->vz + offset.vz;
	v = &MAIN_D_80135214[tri->v2];
	c.vx = v->vx + offset.vx;
	c.vy = v->vy + offset.vy;
	c.vz = v->vz + offset.vz;
	v = &MAIN_D_80135214[tri->v3];
	d.vx = v->vx + offset.vx;
	d.vy = v->vy + offset.vy;
	d.vz = v->vz + offset.vz;
	prim->code |= 2;
	addScreenPolyFT4(prim, &a, &b, &c, &d);
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

void EVL_renderParticle(int32_t id)
{
	int32_t size;
	SVECTOR corners[4];
	DVECTOR screen[4];
	int32_t depth[4];
	EvlParticle *e;
	LINE_F2 *prim;
	int32_t i;

	e = &EVL_D_80068944[id];
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
		if ((depth[i] > 0x20) && (depth[i] < 0x1000L)) {
			if ((depth[(i + 1) % 4] > 0x20) && (depth[(i + 1) % 4] < 0x1000L)) {
				SetLineF2(prim);
				prim->r0 = lerp(e->r, 0, 0, 0x56, e->timer);
				prim->g0 = lerp(e->g, 0, 0, 0x56, e->timer);
				prim->b0 = lerp(e->b, 0, 0, 0x56, e->timer);
				prim->x0 = screen[i].vx;
				prim->y0 = screen[i].vy;
				prim->x1 = screen[(i + 1) % 4].vx;
				prim->y1 = screen[(i + 1) % 4].vy;
				AddPrim(ACTIVE_ORDERING_TABLE->org + 0xfa1, prim++);
			}
		}
	}

	GsSetWorkBase((PACKET *)prim);
}

void EVL_renderSparkStreak(int32_t id)
{
	MATRIX m;
	SVECTOR a;
	SVECTOR b;
	SVECTOR c;
	POLY_FT3 *prim;
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
	prim = (POLY_FT3 *)GsGetWorkBase();
	SetPolyFT3(prim);
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
	int32_t instance;
	int32_t j;
	int16_t order[80];
	int32_t bestVal;
	int32_t best;
	EvoSequenceData *data;
	PartnerEntity *partner;
	int32_t i;

	data = &EVO_SEQUENCE_DATA;
	instance = 0;
	partner = data->partner;
	data->timer = 0;
	data->unk_0x8 = 0;
	data->state = 0;
	copyVector(&EVL_D_80064D40, &partner->digimonEntity.entity.posData->location);
	downloadCLUT1(EVL_D_80063F3C);
	downloadCLUT2(EVL_D_80064D50);
	EVL_storeDigimonClut((int32_t)EVL_D_80065094, &partner->digimonEntity.entity);
	EVL_storeClutBank0((int32_t)EVL_D_80065398);
	EVL_storeClutBank1((int32_t)EVL_D_8006569C);
	EVL_initShardSets((int32_t)EVL_D_80065FA0);
	EVL_setScratchTop((int32_t)EVL_D_80066274);
	initializeFlashData((char *)EVL_D_800677B0.data);
	EVL_resetParticles();
	EVL_resetSparks();

	for (i = 0; i < 40; i++) {
		EVL_D_80067994[i] = -1;
	}

	EVL_D_80067994[DIGIMON_DATA[partner->digimonEntity.entity.type].boneCount - 1L] = -2;

	for (i = 1; i < DIGIMON_DATA[partner->digimonEntity.entity.type].boneCount; i++) {
		order[i] = rand() & 0xfff;
	}

	for (i = 1; i < DIGIMON_DATA[partner->digimonEntity.entity.type].boneCount; i++) {
		best = 1;
		bestVal = order[1];
		for (j = 2; j < DIGIMON_DATA[partner->digimonEntity.entity.type].boneCount; j++) {
			if (bestVal > order[j]) {
				best = j;
				bestVal = order[j];
			}
		}
		EVL_D_80067994[i - 1L] = best;
		order[best] = 0x1000;
	}

	addObject(0x80a, instance, EVL_tickEvoSequence, (RenderFunction)EVL_renderEvoSequence);
}

void EVL_applyEvolution(Entity *entity, Stats *stats, PartnerPara *para, int16_t digimonId)
{
	int32_t oldType;
	int32_t newId;
	uint8_t candidates[16];
	uint8_t *movePtr;
	int32_t i;
	int32_t count;
	int16_t x;
	int16_t y;
	int16_t z;
	int16_t rx;
	int16_t ry;
	int16_t rz;
	int16_t level;
	int16_t oldLevel;
	uint8_t moveBase;
	uint8_t best;
	uint8_t special;
	EvoStatsGains *gains;

	gains = &EVOLUTION_STATS_GAINS[digimonId];
	newId = gains->targetDigimon;
	oldLevel = DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level;
	level = DIGIMON_DATA[digimonId].level;
	if (digimonId == 0x0b || digimonId == 0x27 || digimonId == 0x35 || digimonId == 6 ||
	    PARTNER_ENTITY.digimonEntity.entity.type == 0x27L || level < 3) {
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
	level = DIGIMON_DATA[newId].level;
	if (level == 5 && HAS_USED_EVOITEM == 0) {
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
	count = 0;
	for (i = 0; i < 16; movePtr++, i++) {
		if (moveBase <= *movePtr && *movePtr <= moveBase + 8) {
			candidates[count] = *movePtr;
			count++;
		}
	}
	candidates[count] = 0xff;
	best = candidates[0];
	for (count = 0; candidates[count] != 0xff; count++) {
		if (MOVE_DATA[best].power > MOVE_DATA[candidates[count]].power && MOVE_DATA[candidates[count]].power != 0) {
			best = candidates[count];
		}
	}
	learnMove(best);
	for (movePtr = DIGIMON_DATA[newId].moves, i = 0; i < 16; i++) {
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
	level = DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level;
	x = entity->posData->location.vx;
	y = entity->posData->location.vy;
	z = entity->posData->location.vz;
	rx = entity->posData->rotation.vx;
	ry = entity->posData->rotation.vy;
	rz = entity->posData->rotation.vz;
	oldType = PARTNER_ENTITY.digimonEntity.entity.type;
	removeEntity(PARTNER_ENTITY.digimonEntity.entity.type, 1);
	ENTITY_TABLE[1] = NULL;
	thunkUnloadModel(oldType, 3);
	initializeEvolvedPartner(newId, x, y, z, rx, ry, rz);
	setDigimonRaised((uint16_t)newId);
	if (oldLevel != DIGIMON_DATA[newId].level) {
		para->evoTimer = 0;
	}
}

void EVL_scaleBaseStats(Stats *stats, int16_t pct, int16_t unused)
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
