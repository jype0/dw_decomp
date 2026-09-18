#include <stdlib.h>

#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/anim.h>
#include <dw/btl.h>
#include <dw/doo2.h>
#include <dw/dooa.h>
#include <dw/efe.h>
#include <dw/entity.h>
#include <dw/evolution.h>
#include <dw/file.h>
#include <dw/file_queue.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/sound_async.h>
#include <dw/types.h>
#include <dw/utils.h>
#include <dw/world_object.h>

#define DOOA_MMD_BUFFER		((uint8_t *)0x80020000)
#define DOOA_SHARD_BUFFER	0x80044800
#define DOOA_SHARD_BUFFER_SIZE	0x5dc0
#define DOOA_ORDERING_TABLE_0	((GsOT_TAG *)0x8008c000)
#define DOOA_ORDERING_TABLE_1	((GsOT_TAG *)0x8008e000)

typedef struct {
	VECTOR offset;
	int8_t unk_10[3];
	int8_t boneId;
} DooaSparkle;

typedef struct {
	int16_t vx;
	int16_t vy;
	int16_t vz;
} DooaShardVertex;

typedef struct {
	TMD_P_TG4 *prim;
	int16_t rotX;
	int16_t rotY;
	int16_t rotZ;
	int16_t centerX;
	int16_t centerY;
	int16_t centerZ;
	int16_t spinMax;
	int16_t spin;
	int32_t targetRadius;
	int32_t radius;
	int16_t dropDepth;
	int16_t fallSpeed;
	int16_t axisDistance;
	int16_t delay;
	DooaShardVertex vertex[3];
} DooaShard;

typedef struct {
	TMD_P_TG4 *prim;
	int16_t rotX;
	int16_t rotY;
	int16_t rotZ;
	int16_t centerX;
	int16_t centerY;
	int16_t centerZ;
	int16_t spinMax;
	int16_t spin;
	int32_t targetRadius;
	int32_t radius;
	int16_t dropDepth;
	int16_t fallSpeed;
	int16_t axisDistance;
	int16_t delay;
	DooaShardVertex vertex[4];
} DooaShardQuad;

typedef struct {
	int8_t v[36];
} ShardWaveSchedule;

typedef struct {
	int16_t v[32];
} DissolveScaleCurve;

extern int32_t VIEWPORT_DISTANCE;
extern int32_t MAIN_D_8013532C;
extern int32_t MAIN_D_80135330;
extern int32_t MAIN_D_80135334;
extern GsRVIEW2 GS_VIEWPOINT;
extern int8_t MAIN_D_80135364[8];
extern int16_t MAIN_D_80135324;
extern int32_t ACTIVE_FRAMEBUFFER;
extern SVECTOR MAIN_D_80135338;
extern int32_t MAIN_D_80135340;
extern VECTOR CAMERA_TARGET;
extern int8_t CAMERA_REACHED_TARGET;
extern int32_t FLASH_INSTANCE;
extern int8_t DOO2_LOADING_COMPLETE;
extern int32_t MAIN_D_80135328;
extern int32_t MAIN_D_80135348;
extern int32_t MAIN_D_8013534C;
extern int32_t MAIN_D_80135350;
extern int32_t MAIN_D_80135354;
extern int32_t MAIN_D_80135358;
extern SVECTOR MAIN_D_8013535C;

void DOOA_renderDigimonModel(Entity *entity, uint32_t otPoint);
int32_t DOOA_renderIrisWindow(Entity *entity, int32_t startFrame, int32_t endFrame, int32_t frame);
void DOOA_renderDissolve(int32_t instanceId);
int32_t getDistance(int32_t x, int32_t y, int32_t z);
int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t);
int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out);
int32_t DOOA_hasIrisClosed(Entity *entity, int32_t startFrame, int32_t endFrame, int32_t frame);
void DOOA_saveShardClut(u_long *pixels);
void DOOA_setOtherEntitiesVisible(int32_t restore);
void DOOA_hideAllButPartner(void);
void DOOA_getOrbitPosition(VECTOR *outRef, VECTOR *outPos, VECTOR *position, SVECTOR *rotation, int32_t distance, int32_t height);
void DOOA_updateCutsceneCamera(VECTOR *position, int32_t angle, int32_t startFrame, int32_t endFrame, int32_t frame);
void DOOA_toggleShardFlicker(void);
int32_t customRandom(int32_t low, int32_t high);
void DOOA_renderRebirth(int32_t instanceId);
void DOOA_setShardState(int16_t state);
void DOOA_removeShardEffect(void);
void DOOA_showPlayerAndPartner(void);
void DOOA_fadeModelClut(int16_t *srcClut, void *unused, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame);
void DOOA_fadeShardClut(int16_t *srcClut, void *unused, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame);
int32_t MAIN_func_800DA9F4(void);
int32_t DOOA_updateShards(int32_t instanceId);
int32_t DOOA_renderShards(int32_t instanceId);
int32_t DOOA_initShardEffect(Entity *entity, intptr_t addr, int32_t size);
void DOOA_saveEntityClut(u_long *pixels, Entity *entity);
void DOOA_saveModelClut(u_long *pixels);
void renderDropShadow(Entity *entity);
void createFlash(void);
void setMapLayerEnabled(int32_t enabled);
void DOOA_tickRebirth(int32_t instanceId);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
void DOOA_spawnBoneShards(DooaShardEffect *effect, int32_t boneIndex, int32_t wireIndex);
char *initializeFlashData(char *base);
void MAIN_func_800D91EC(int32_t messageId, int32_t flag);
void DOOA_tickDissolve(int32_t instanceId);
void DOOA_initOrderingTable(void);
void DOOA_spawnShardWave(int32_t wireIndex);
void MAIN_func_80092B60(POLY_FT4 *prim);
void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
int32_t add3DSpritePrim(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
void tickCameraMovement(int32_t mode);
void setEFEFlashOffset(int32_t instance, int32_t x, int32_t y);
void MAIN_func_800D91FC(int32_t mode);
void MAIN_func_800D9248(void);
void MAIN_func_800D9B60(int16_t *clut);
void MAIN_func_800D9BA8(int32_t alpha, int16_t *clut, int32_t mode);
void MAIN_func_800DA9C8(void);
void renderParticleFlash(int16_t *params);

static void *dooa_functions[] = {
	DOOA_getSequenceState,
	DOOA_tick,
	DOOA_spawnBoneShards,
	DOOA_renderShards,
	DOOA_updateShards,
	DOOA_getOrbitPosition,
	DOOA_fadeShardClut,
	DOOA_fadeModelClut,
	DOOA_showPlayerAndPartner,
	DOOA_removeShardEffect,
	DOOA_setShardState,
	DOOA_renderRebirth,
	DOOA_tickRebirth,
	DOOA_renderIrisWindow,
	DOOA_renderDigimonModel,
	DOOA_toggleShardFlicker,
	DOOA_spawnShardWave,
	DOOA_updateCutsceneCamera,
	DOOA_hideAllButPartner,
	DOOA_setOtherEntitiesVisible,
	DOOA_saveShardClut,
	DOOA_saveModelClut,
	DOOA_saveEntityClut,
	DOOA_initShardEffect,
	DOOA_hasIrisClosed,
	DOOA_initOrderingTable,
	DOOA_renderDissolve,
	DOOA_tickDissolve,
};

// clang-format off
ShardWaveSchedule DOOA_SHARD_WAVE_SCHEDULE = {
	{
		15, 15, 15, 15, 15, 14, 14, 14,
		14, 13, 13, 13, 12, 12, 12, 11,
		11, 11, 10, 10, 10,  9,  9,  8,
		 8,  7,  7,  6,  6,  5,  5,  4,
		 3,  2,  1,  0,
	}
};

DissolveScaleCurve DOOA_DISSOLVE_SCALE_CURVE = {
	{
		100,  94,  89,  85,  82,  80,  79,  79,
		100, 120, 139, 157, 174, 190, 205, 219,
		232, 244, 255, 265, 274, 282, 289, 295,
		300, 350, 395, 435, 470, 500, 525, 545,
	}
};
// clang-format on

VECTOR DOOA_CAMERA_TARGET_RESET = { 0, 0, 0, 0 };

char DOOA_EGG_TIM_PATH[20] = "\\ETCDAT\\TAMA.TIM";
char DOOA_EGG_TMD_PATH[20] = "\\ETCDAT\\TAMA.TMD";

VECTOR DOOA_FLASH_POSITION = { 0, -100, 0, 0 };

// clang-format off
int8_t DOOA_SPARKLE_BONE_IDS[48] = {
	-1, -1, -1, -1,  0, -1, -1, -1, -1, -1, -1, -1,
	-1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 2, -1, -1, -1, -1, -1, -1, -1, -1, -1,  3, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,  4,  5, -1,  0,
};
// clang-format on

VECTOR DOOA_SPARKLE_OFFSET = { 0, 0, 0, 0 };

static int32_t dooa__garbage__(int32_t seed)
{
	int32_t t0 = 1;
	int32_t t1 = 2;
	int32_t t2 = 3;
	int32_t t3 = 4;
	int32_t t4 = 5;
	int32_t t5 = 6;
	int32_t t6 = 7;
	int32_t t7 = 8;
	int32_t t8 = 9;
	int32_t t9 = 10;
	int32_t t10 = 11;
	int32_t t11 = 12;
	int32_t t12 = 13;
	int32_t t13 = 14;
	int32_t t14 = 15;
	int32_t t15 = 16;
	int32_t t16 = 17;
	return seed + t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8 + t9 + t10 + t11 + t12 + t13 + t14 + t15 + t16;
}

void DOOA_tickDissolve(int32_t instanceId)
{
	GsRVIEW2 savedView;
	ShardWaveSchedule wireCounts;
	DissolveScaleCurve heights;
	VECTOR startColor;
	VECTOR endColor;
	GsIMAGE timInfo;
	RECT rect;
	int32_t savedOffsetX;
	int32_t savedOffsetY;
	int32_t savedDistance;
	int16_t height;

	int32_t work;
	int32_t flashOffset;
	int32_t wireCount;
	DooaFlash *flash;
	DooaFlash *target;
	Entity *entity;
	DooaSequence *seq;

	seq = &DOOA_REINCARNATION_SEQ;
	entity = seq->entity;
	seq->frame++;

	if ((entity->anim.animId == 0xc) && (entity->anim.animFrame == entity->anim.frameCount)) {
		startAnimation((Entity *)&PARTNER_ENTITY, 1);
		MAIN_func_800DA9C8();
		DOOA_SAVED_LOCATION = entity->posData->location;
		MAIN_D_80135338 = entity->posData->rotation;
	}

	switch (seq->phase) {
	case 0:
		if (DOOA_hasIrisClosed(seq->entity, 0, 0x21, seq->frame) != 0) {
			ENTITY_TABLE[1]->isOnMap = 0;
		}
		if (seq->frame < 0x23) {
			break;
		}
		savedView = GS_VIEWPOINT;
		savedOffsetX = DRAWING_OFFSET_X;
		savedOffsetY = DRAWING_OFFSET_Y;
		savedDistance = VIEWPORT_DISTANCE;
		MAIN_func_800D91FC(0);
		MAIN_func_800D9248();
		stopBGM();
		loadMapSounds2(0x13);
		isSoundLoaded(0, 8);
		if (((PartnerEntity *)entity)->lives != 0) {
			MAIN_func_800DF5A0();
		} else {
			MAIN_func_800DF5A0();
		}
		entity = ENTITY_TABLE[1];
		seq->entity = entity;
		ENTITY_TABLE[1]->isOnMap = 0;
		loadDynamicLibrary(8, (uint8_t *)&DOO2_LOADING_COMPLETE, 1, 0, 0);
		while (DOO2_LOADING_COMPLETE != 0) {
			tickFileReadQueue(0);
		}
		DOOA_initShardEffect(entity, DOOA_SHARD_BUFFER, DOOA_SHARD_BUFFER_SIZE);
		DOOA_saveEntityClut((u_long *)DOOA_SAVED_ENTITY_CLUT, entity);
		DOOA_saveModelClut((u_long *)DOOA_MODEL_CLUT);
		DOOA_saveShardClut((u_long *)DOOA_SHARD_CLUT);
		MAIN_D_80135348 = DRAWING_OFFSET_X;
		MAIN_D_8013534C = DRAWING_OFFSET_Y;
		DOOA_SAVED_VIEW = GS_VIEWPOINT;
		if (MAIN_D_80135340 == 0xcd) {
			MAIN_D_80135350 = 0xbc;
			MAIN_D_80135354 = 0x78;
		} else {
			MAIN_D_80135350 = 0xa0;
			MAIN_D_80135354 = 0x78;
		}
		MAIN_D_80135358 = VIEWPORT_DISTANCE;
		GS_VIEWPOINT = savedView;
		DRAWING_OFFSET_X = savedOffsetX;
		DRAWING_OFFSET_Y = savedOffsetY;
		VIEWPORT_DISTANCE = savedDistance;
		GsSetRefView2(&GS_VIEWPOINT);
		GsSetProjection(VIEWPORT_DISTANCE);
		DOOA_PARTNER_POSITION.vx = 0;
		DOOA_PARTNER_POSITION.vy = 0;
		DOOA_PARTNER_POSITION.vz = 0;
		entity->posData->location = DOOA_SAVED_LOCATION;
		entity->posData->rotation = MAIN_D_80135338;
		if (((PartnerEntity *)entity)->lives != 0) {
			startAnimation((Entity *)&PARTNER_ENTITY, 1);
		} else {
			startAnimation((Entity *)&PARTNER_ENTITY, 0x2c);
			ENTITY_TABLE[1]->anim.animFlag &= 0xfe;
		}
		DOOA_setOtherEntitiesVisible(0);
		seq->phase = 1;
		setMapLayerEnabled(0);
		DOOA_hideAllButPartner();
		MAIN_func_800D9B60(DOOA_SCENE_CLUT);
		MAIN_func_800D9BA8(0xff, DOOA_SCENE_CLUT, 0);
		break;
	case 1:
		entity->isOnMap = 2;
		work = seq->frame;
		if (work >= 0x56) {
			seq->phase = 2;
			seq->phaseInitPending = 1;
			DOOA_CAMERA_START_VIEW = DOOA_SAVED_VIEW;
			MAIN_D_8013532C = MAIN_D_80135350;
			MAIN_D_80135330 = MAIN_D_80135354;
			MAIN_D_80135334 = MAIN_D_80135358;
			DOOA_SAVED_LOCATION = entity->posData->location;
			MAIN_D_80135338 = entity->posData->rotation;
			entity->posData->location = DOOA_PARTNER_POSITION;
			entityLookAtLocation(entity, &ENTITY_TABLE[0]->posData->location);
			MAIN_D_8013535C = entity->posData->rotation;
			setEntityPosition(1, DOOA_PARTNER_POSITION.vx, DOOA_PARTNER_POSITION.vy, DOOA_PARTNER_POSITION.vz);
			setupEntityMatrix(1);
			DOOA_updateCutsceneCamera(&entity->posData->location, entity->posData->rotation.vy, 0, 0x64, 0x63);
			GsSetRefView2(&GS_VIEWPOINT);
			GsSetProjection(VIEWPORT_DISTANCE);
		} else {
			DOOA_updateCutsceneCamera(&entity->posData->location, entity->posData->rotation.vy, 0x23, 0x55, work);
			GsSetRefView2(&GS_VIEWPOINT);
			GsSetProjection(VIEWPORT_DISTANCE);
		}
		break;
	case 2:
		ENTITY_TABLE[1]->isOnMap = 2;
		if (seq->phaseInitPending != 0) {
			setEntityPosition(1, DOOA_PARTNER_POSITION.vx, DOOA_PARTNER_POSITION.vy, DOOA_PARTNER_POSITION.vz);
			setupEntityMatrix(1);
			if (((PartnerEntity *)entity)->lives != 0) {
				startAnimation((Entity *)&PARTNER_ENTITY, 0xc);
			}
			seq->phaseInitPending = 0;
			MAIN_func_800DA9C8();
		}
		if (seq->frame == 0x69) {
			playSound2(8, 0);
		}
		if (seq->frame == 0x8c) {
			stopSound();
		}
		if (seq->frame < 0x69) {
			break;
		}
		wireCounts = DOOA_SHARD_WAVE_SCHEDULE;
		work = seq->frame;
		if (work < 0x8d) {
			work -= 0x69;
			PARTNER_WIREFRAME_TOTAL = (wireCounts.v + work)[0];
			wireCount = (wireCounts.v + work)[0];
			if ((work == 0) || ((wireCounts.v + work)[-1] != wireCount)) {
				DOOA_spawnShardWave(wireCount);
			}
		}
		if (seq->frame >= 0xbf) {
			seq->phase = 0x64;
			DOOA_SAVED_LOCATION.vy = entity->posData->location.vy;
			target = &seq->flash;
			target->targetY = MAIN_func_800DA9F4() - (DIGIMON_DATA[entity->type].height + 100);
			target->pos.vx = entity->posData->location.vx;
			target->pos.vz = entity->posData->location.vz;
			stopSound();
			playSound(8, 1);
		}
		break;
	case 0x64:
		heights = DOOA_DISSOLVE_SCALE_CURVE;
		work = lerp(0, 0x1f, 0xbf, 0xd0, seq->frame);
		work = height = heights.v[work];
		work = ((((work - 100) * 200) / 100) + 100);
		entity->posData->scale.vy = (work << 12) / 100;
		entity->posData->scale.vx = ((10000 / work) << 12) / 100;
		entity->posData->scale.vx = lerp(entity->posData->scale.vx, entity->posData->scale.vx * 50 / 100, 0xbf, 0xd0, seq->frame);
		entity->posData->scale.vz = entity->posData->scale.vx;
		if (height >= 0x12d) {
			work = height;
			work = 0x12c - (work - 0x12c);
			work = ((((work - 100) * 200) / 100) + 100);
			entity->posData->scale.vy = (work << 12) / 100;
			ENTITY_TABLE[1]->anim.animFlag &= 0xfe;
			entity->posData->location.vy = lerp(entity->posData->location.vy, seq->flash.targetY, seq->frame - 1, seq->frame, seq->frame);
			setEntityPosition(1, entity->posData->location.vx, entity->posData->location.vy, entity->posData->location.vz);
			setupEntityMatrix(1);
		}
		if (seq->frame >= 0xd1) {
			seq->phase = 0x65;
			WIREFRAME_COLOR_MIN = 0x37;
			WIREFRAME_COLOR_MAX = 0xff;
			PARTNER_WIREFRAME_TOTAL = 0x10;
			startAnimation(entity, 0x21);
			ENTITY_TABLE[1]->isOnMap = 0;
			ENTITY_TABLE[1]->anim.animFlag &= 0xfe;
			entity->posData->location.vy = DOOA_SAVED_LOCATION.vy;
			setEntityPosition(1, entity->posData->location.vx, entity->posData->location.vy, entity->posData->location.vz);
			setupEntityMatrix(1);
			playSound(8, 2);
		}
		break;
	case 0x65:
		flash = &seq->flash;
		flash->pos.vy = lerp(flash->targetY, 0, 0xd1, 0x135, seq->frame);
		EFE_PUSH1(int32_t, 0);
		EFE_PUSH1(VECTOR *, &flash->pos);
		EFE_PUSH1(int32_t, -1);
		EFE_PUSH1(int32_t, 8);
		EFE_PUSH1(int32_t, 0);
		EFE_PUSH1(int32_t, 0x14);
		startColor.vx = (rand() % 100) + 60;
		startColor.vy = (rand() % 100) + 60;
		startColor.vz = (rand() % 100) + 60;
		endColor.vx = endColor.vy = endColor.vz = 0x14;
		EFE_PUSH1(VECTOR *, &startColor);
		EFE_PUSH1(VECTOR *, &endColor);
		createFlash();
		flashOffset = _sin(lerp(0, 0x600, 0xd1, 0x135, seq->frame));
		flashOffset = flashOffset * 10 / 4096;
		setEFEFlashOffset(FLASH_INSTANCE, flashOffset, 0);
		if (seq->frame >= 0x135) {
			seq->phase = 3;
			DOOA_toggleShardFlicker();
			playSound2(8, 3);
		}
		break;
	case 3:
		if (seq->frame >= 0x17b) {
			seq->phase = 0x66;
			while (DOO2_LOADING_COMPLETE != 0) {
				tickFileReadQueue(0);
			}
			readFile(DOOA_EGG_TIM_PATH, DOO2_D_80071EE4);
			GsGetTimInfo(&DOO2_D_80071EE4[1], &timInfo);
			setRECT(&rect, timInfo.px, timInfo.py, timInfo.pw, timInfo.ph);
			LoadImage(&rect, timInfo.pixel);
			GetTPage(timInfo.pmode & 3, 0, timInfo.px, timInfo.py);
			if ((timInfo.pmode >> 3) & 1) {
				setRECT(&rect, timInfo.cx, timInfo.cy, timInfo.cw, timInfo.ch);
				LoadImage(&rect, timInfo.clut);
				MAIN_D_80135328 = GetClut(timInfo.cx, timInfo.cy);
			}
			DOO2_saveClutTile(DOO2_D_80071B5C, MAIN_D_80135328);
			readFile(DOOA_EGG_TMD_PATH, DOO2_D_80071EE4);
			GsMapModelingData((unsigned long *)DOO2_D_80071EE4 + 1);
			DOO2_saveModelClut(DOO2_D_80071BE0);
		}
		work = 0x2c4;
		work -= seq->frame;
		DOOA_updateCutsceneCamera(&entity->posData->location, entity->posData->rotation.vy, 0x149, 0x17b, work);
		break;
	case 0x66:
		if ((seq->frame >= 0x185) && (seq->frame < 0x1b8)) {
			ENTITY_TABLE[2]->isOnMap = 1;
			DOO2_fadeClut((int16_t *)DOO2_D_80071BE0, entity, DOOA_FADED_CLUT, 0, 0xff, lerp(0xff, 0, 0x185, 0x1b7, seq->frame));
		}
		if (seq->frame >= 0x1b7) {
			seq->phase = 4;
		}
		break;
	case 4:
		DRAWING_OFFSET_X = MAIN_D_80135348;
		DRAWING_OFFSET_Y = MAIN_D_8013534C;
		CAMERA_TARGET = DOOA_CAMERA_TARGET_RESET;
		tickCameraMovement(1);
		CAMERA_REACHED_TARGET = -1;
		removeObject(0x80b, instanceId);
		seq->frame = -1;
		stopSound();
		break;
	}
}

void DOOA_renderDissolve(int32_t instanceId)
{
	DooaSequence *seq = &DOOA_REINCARNATION_SEQ;

	if (seq->frame < 36) {
		if (ENTITY_TABLE[1]->isOnMap == 0) {
			DOOA_renderDigimonModel(seq->entity, 0x21);
		}
		DOOA_renderIrisWindow(seq->entity, 0, 0x21, seq->frame);
	}
}

void DOOA_initOrderingTable(void)
{
	DOOA_ORDERING_TABLE[0].length = 11;
	DOOA_ORDERING_TABLE[0].org = DOOA_ORDERING_TABLE_0;
	DOOA_ORDERING_TABLE[1].length = 11;
	DOOA_ORDERING_TABLE[1].org = DOOA_ORDERING_TABLE_1;
}

int32_t DOOA_hasIrisClosed(Entity *entity, int32_t startFrame, int32_t endFrame, int32_t frame)
{
	SVECTOR worldPos;
	DVECTOR screenPos;
	int32_t size;
	int32_t radius;
	int32_t depth;

	if (endFrame < frame) {
		frame = endFrame;
	}
	worldPos.vx = entity->posData->location.vx;
	worldPos.vy = entity->posData->location.vy - (DIGIMON_DATA[entity->type].height / 2);
	worldPos.vz = entity->posData->location.vz;
	size = getDistance(DIGIMON_DATA[entity->type].radius * 2, DIGIMON_DATA[entity->type].height, DIGIMON_DATA[entity->type].radius * 2);
	radius = (lerp(200, 0, startFrame, endFrame, frame) << 12) / 128;
	depth = worldPosToScreenPos(&worldPos, &screenPos);
	if (depth <= 0) {
		return 1;
	}
	if (((int32_t)(((uint32_t)(size * VIEWPORT_DISTANCE) / depth) << 12) / 256) >= radius) {
		return 1;
	}

	return 0;
}

int32_t DOOA_initShardEffect(Entity *entity, intptr_t addr, int32_t size)
{
	DooaShardEffect *effect = &DOOA_SHARD_EFFECT;
	int32_t rem = addr & 3;

	if (rem != 0) {
		size -= 4 - rem;
		addr += 4 - rem;
	}

	effect->state = 0;
	effect->prevState = effect->state;
	effect->entity = entity;
	effect->shardBuffer = (void *)addr;
	effect->shardWrite = effect->shardBuffer;
	*(int32_t *)effect->shardWrite = 0;
	effect->shardBytes = size;
	effect->colorR = 0x50;
	effect->colorG = 0x50;
	effect->colorB = 0x50;
	effect->flash = 0;
	addObject(0x608, 0, (TickFunction)DOOA_updateShards, (RenderFunction)DOOA_renderShards);
	return addr + size;
}

void DOOA_saveEntityClut(u_long *pixels, Entity *entity)
{
	ModelComponent *model;
	RECT rect;

	model = getEntityModelComponent(entity->type, getEntityType(entity));
	setRECT(&rect, (model->clutPage & 0x3f) << 4, model->clutPage >> 6, 16, 24);
	StoreImage(&rect, pixels);
	DrawSync(0);
}

void DOOA_saveModelClut(u_long *pixels)
{
	RECT rect;

	setRECT(&rect, 0, 488, 16, 24);
	StoreImage(&rect, pixels);
	DrawSync(0);
}

void DOOA_saveShardClut(u_long *pixels)
{
	RECT rect;

	setRECT(&rect, 48, 488, 32, 24);
	StoreImage(&rect, pixels);
	DrawSync(0);
}

void DOOA_setOtherEntitiesVisible(int32_t restore)
{
	int32_t i;

	DOOA_ENTITIES_VISIBLE = restore;

	if (restore == 0) {
		for (i = 0; i < ENTITY_MAX; i++) {
			if ((ENTITY_TABLE[i] != (Entity *)&PARTNER_ENTITY) && (ENTITY_TABLE[i]->isOnMap != 0)) {
				ENTITY_TABLE[i]->isOnMap = 0;
				DOOA_SAVED_ENTITY_VISIBILITY[i] = 1;
			} else {
				DOOA_SAVED_ENTITY_VISIBILITY[i] = 0;
			}
		}
	} else {
		for (i = 0; i < ENTITY_MAX; i++) {
			if (ENTITY_TABLE[i] != (Entity *)&PARTNER_ENTITY) {
				ENTITY_TABLE[i]->isOnMap = DOOA_SAVED_ENTITY_VISIBILITY[i];
			}
		}
	}
}

void DOOA_hideAllButPartner(void)
{
	int32_t i;

	for (i = 0; i < ENTITY_MAX; i++) {
		if (i == 1) {
			continue;
		}
		ENTITY_TABLE[i]->isOnMap = 0;
	}
}

void DOOA_updateCutsceneCamera(VECTOR *position, int32_t angle, int32_t startFrame, int32_t endFrame, int32_t frame)
{
	VECTOR viewRef;
	VECTOR viewPos;
	SVECTOR worldPos;
	DVECTOR screenPos;
	SVECTOR rotation;
	int32_t midFrame;
	int32_t height;
	int32_t offsetY;

	midFrame = (startFrame + endFrame) / 2;
	height = DIGIMON_DATA[DOOA_REINCARNATION_SEQ.entity->type].height;

	if ((frame < startFrame) || (frame > endFrame)) {
		return;
	}

	if ((frame < startFrame) || (frame > endFrame)) {
		return;
	}

	copyVector(&worldPos, position);
	worldPosToScreenPos(&worldPos, &screenPos);

	rotation = MAIN_D_80134BB4;
	rotation.vy = angle + 0x638;

	DOOA_getOrbitPosition(&viewRef, &viewPos, position, &rotation, (height * 5) + 1200, height);

	if (frame <= midFrame) {
		GS_VIEWPOINT.vrx = lerp(DOOA_CAMERA_START_VIEW.vrx, viewRef.vx, startFrame, midFrame, frame);
		GS_VIEWPOINT.vry = lerp(DOOA_CAMERA_START_VIEW.vry, viewRef.vy, startFrame, midFrame, frame);
		GS_VIEWPOINT.vrz = lerp(DOOA_CAMERA_START_VIEW.vrz, viewRef.vz, startFrame, midFrame, frame);
		GS_VIEWPOINT.rz = 0;
		DRAWING_OFFSET_X = lerp(MAIN_D_8013532C, 160, startFrame, midFrame, frame);
		DRAWING_OFFSET_Y = lerp(MAIN_D_80135330, 120, startFrame, midFrame, frame);
		GS_VIEWPOINT.vpx = DOOA_CAMERA_START_VIEW.vpx;
		GS_VIEWPOINT.vpy = DOOA_CAMERA_START_VIEW.vpy;
		GS_VIEWPOINT.vpz = DOOA_CAMERA_START_VIEW.vpz;
		VIEWPORT_DISTANCE = MAIN_D_80135334;
	} else if (frame <= endFrame) {
		GS_VIEWPOINT.vpx = lerp(DOOA_CAMERA_START_VIEW.vpx, viewPos.vx, midFrame, endFrame, frame);
		GS_VIEWPOINT.vpy = lerp(DOOA_CAMERA_START_VIEW.vpy, viewPos.vy, midFrame, endFrame, frame);
		GS_VIEWPOINT.vpz = lerp(DOOA_CAMERA_START_VIEW.vpz, viewPos.vz, midFrame, endFrame, frame);
		offsetY = lerp(0, 20, 0, 200, height);
		DRAWING_OFFSET_Y = lerp(120, offsetY + 120, midFrame, endFrame, frame);
		VIEWPORT_DISTANCE = lerp(MAIN_D_80135334, 1000, midFrame, endFrame, frame);
		GS_VIEWPOINT.vrx = viewRef.vx;
		GS_VIEWPOINT.vry = viewRef.vy;
		GS_VIEWPOINT.vrz = viewRef.vz;
		DRAWING_OFFSET_X = 160;
	}
}

void DOOA_spawnShardWave(int32_t wireIndex)
{
	DooaShardEffect *effect;
	Entity *entity;
	int32_t boneIndex;

	effect = &DOOA_SHARD_EFFECT;
	entity = DOOA_SHARD_EFFECT.entity;

	for (boneIndex = 2; boneIndex < DIGIMON_DATA[entity->type].boneCount; boneIndex++) {
		DOOA_spawnBoneShards(effect, boneIndex, wireIndex);
	}
}

void DOOA_toggleShardFlicker(void)
{
	DOOA_SHARD_EFFECT.flash = (DOOA_SHARD_EFFECT.flash + 1) & 1;
}

void DOOA_renderDigimonModel(Entity *entity, uint32_t otPoint)
{
	MATRIX lightMatrix;
	PositionData *posData;
	int32_t boneCount;
	int32_t entityIndex;
	int32_t i;

	for (entityIndex = 0; entityIndex < ENTITY_MAX; entityIndex++) {
		if (ENTITY_TABLE[entityIndex] == entity) {
			break;
		}
	}

	if (entityIndex == ENTITY_MAX) {
		return;
	}

	GsClearOt(0, 2, &DOOA_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
	DOOA_ORDERING_TABLE[ACTIVE_FRAMEBUFFER].point = otPoint;

	boneCount = DIGIMON_DATA[entity->type].boneCount;
	posData = entity->posData;
	lightMatrix = GsWSMATRIX;

	for (i = 0; i < boneCount; i++) {
		if (posData->obj.tmd != NULL) {
			GsGetLw(posData->obj.coord2, &lightMatrix);
			GsSetLightMatrix(&lightMatrix);
			GsGetLs(posData->obj.coord2, &lightMatrix);
			GsSetLsMatrix(&lightMatrix);
			GsSortObject4(&posData->obj, &DOOA_ORDERING_TABLE[ACTIVE_FRAMEBUFFER], 3, getScratchAddr(0));
		}
		posData++;
	}

	GsSortOt(&DOOA_ORDERING_TABLE[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	renderDropShadow(entity);
}

int32_t DOOA_renderIrisWindow(Entity *entity, int32_t startFrame, int32_t endFrame, int32_t frame)
{
	int32_t isVisible;
	int32_t boxBottom;
	int32_t rightEdge;
	int32_t bottomEdge;
	SVECTOR worldPos;
	DVECTOR screenPos;
	int16_t flash[14];
	POLY_FT4 *prim;
	int32_t modelSize;
	int32_t depth;
	int32_t size;
	int32_t thickness;
	int32_t boxTop;
	int32_t boxRight;
	int32_t leftEdge;
	int32_t topEdge;
	int32_t screenX;
	int32_t leftX;
	int32_t leftY;
	int32_t leftW;
	int32_t rightX;
	int32_t rightY;
	int32_t rightW;
	int32_t topY;
	int32_t topW;
	int32_t topH;
	int32_t bottomY;
	int32_t bottomW;

	if (endFrame < frame) {
		frame = endFrame;
	}

	worldPos.vx = entity->posData->location.vx;
	worldPos.vy = entity->posData->location.vy - (DIGIMON_DATA[entity->type].height / 2);
	worldPos.vz = entity->posData->location.vz;

	modelSize = getDistance(DIGIMON_DATA[entity->type].radius * 2, DIGIMON_DATA[entity->type].height, DIGIMON_DATA[entity->type].radius * 2);

	size = (lerp(200, 0, startFrame, endFrame, frame) << 12) / 128;

	depth = worldPosToScreenPos(&worldPos, &screenPos);
	if (depth <= 0) {
		return 1;
	}

	if ((((int32_t)((uint32_t)(modelSize * VIEWPORT_DISTANCE) / depth) << 12) / 256) >= size) {
		isVisible = 1;
	} else {
		isVisible = 0;
	}

	flash[0] = screenPos.vx;
	flash[1] = screenPos.vy;
	flash[6] = flash[7] = 0x40;
	flash[8] = 0xdd;
	((uint8_t *)flash)[0x12] = 0;
	((uint8_t *)flash)[0x13] = 0x80;
	flash[10] = 0x79c0;
	((uint8_t *)flash)[0x16] = 0x80;
	((uint8_t *)flash)[0x17] = 0x80;
	((uint8_t *)flash)[0x18] = 0x80;
	((uint8_t *)flash)[0x19] = 0x80;
	*(int32_t *)&flash[4] = size;
	flash[2] = 0x22;
	renderParticleFlash(flash);

	prim = (POLY_FT4 *)GsGetWorkBase();

	/* startFrame reused as iris radius, endFrame as box left edge */
	startFrame = (size << 8) / 4096;
	screenX = screenPos.vx;
	boxTop = screenPos.vy - startFrame;
	endFrame = screenX - startFrame;
	boxBottom = screenPos.vy + startFrame;
	boxRight = startFrame + screenX;

	leftEdge = endFrame - (160 - DRAWING_OFFSET_X);
	rightEdge = boxRight - (160 - DRAWING_OFFSET_X);
	topEdge = boxTop - (120 - DRAWING_OFFSET_Y);
	bottomEdge = boxBottom - (120 - DRAWING_OFFSET_Y);

	thickness = lerp(4, 1, 0x1860, 0, size);

	leftX = endFrame - (leftEdge + 160);
	leftY = boxTop - ((int32_t)topEdge + 120);
	leftW = (leftEdge + 160) + thickness;
	if (leftW > 0) {
		SetPolyFT4(prim);
		SetSemiTrans(prim, 2);
		prim->r0 = prim->g0 = prim->b0 = 0x80;
		prim->tpage = getTPage(1, 2, 832, 256);
		prim->clut = getClut(0, 487);
		setUVWH(prim, 0, 0x80, 3, 3);
		setXYWH(prim, leftX, leftY, leftW, 240);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x22, prim++);
	}

	rightY = boxTop - ((long)topEdge + 120);
	rightX = boxRight - thickness;
	rightW = (160 - rightEdge) + thickness;
	if (rightW > 0) {
		SetPolyFT4(prim);
		prim->r0 = prim->g0 = prim->b0 = 0x80;
		SetSemiTrans(prim, 2);
		prim->tpage = getTPage(1, 2, 832, 256);
		prim->clut = getClut(0, 487);
		setUVWH(prim, 0, 0x80, 3, 3);
		setXYWH(prim, rightX, rightY, rightW, 240);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x22, prim++);
	}

	topY = boxTop - (topEdge + 120);
	topW = boxRight - endFrame;
	if (topW > 0) {
		topH = (topEdge + 120) + thickness;
		if (topH > 0) {
			SetPolyFT4(prim);
			prim->r0 = prim->g0 = prim->b0 = 0x80;
			SetSemiTrans(prim, 2);
			prim->tpage = getTPage(1, 2, 832, 256);
			prim->clut = getClut(0, 487);
			setUVWH(prim, 0, 0x80, 3, 3);
			setXYWH(prim, endFrame, topY, topW, topH);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 0x22, prim++);
		}
	}

	bottomW = (long)boxRight - endFrame;
	bottomY = boxBottom - thickness;
	if (bottomW > 0) {
		/* boxRight reused as bottom bar height */
		boxRight = (120 - bottomEdge) + thickness;
		if (boxRight > 0) {
			SetPolyFT4(prim);
			prim->r0 = prim->g0 = prim->b0 = 0x80;
			SetSemiTrans(prim, 2);
			prim->tpage = getTPage(1, 2, 832, 256);
			prim->clut = getClut(0, 487);
			setUVWH(prim, 0, 0x80, 3, 3);
			setXYWH(prim, endFrame, bottomY, bottomW, boxRight);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 0x22, prim++);
		}
	}

	GsSetWorkBase((PACKET *)prim);
	return isVisible;
}

void DOOA_tickRebirth(int32_t instanceId)
{
	VECTOR flashPos;
	VECTOR colorStart;
	VECTOR colorEnd;
	DooaSparkle sparkle;
	DooaSequence *seq;
	Entity *entity;
	int32_t level;
	int32_t frame;

	seq = &DOOA_REINCARNATION_SEQ;
	entity = seq->entity;
	seq->frame++;
	seq->fadeLevel = 0;

	switch (seq->phase) {
	case 0xc8:
		seq->phase = 0xc9;
		/* fall through */
	case 0xc9:
		if (seq->frame < 10) {
			break;
		}
		seq->phase = 0xca;
		DOOA_setShardState(1);
		stopSound();
		playSound(8, 5);
		break;
	case 0xca:
		if (seq->frame < 50) {
			break;
		}
		seq->phase = 0xcb;
		break;
	case 0xcb:
		if (seq->frame < 100) {
			break;
		}
		seq->phase = 0xcd;
		DOOA_setShardState(3);
		break;
	case 0xcd:
		frame = seq->frame;
		if (frame >= 130) {
			MAIN_D_80135324 = 0x10;
		} else {
			MAIN_D_80135324 = lerp(1, 0x10, 100, 132, frame);
		}
		if (seq->frame < 132) {
			break;
		}
		seq->phase = 0xce;
		DOOA_removeShardEffect();
		loadVLALL(EGG_DIGIMON_TYPES[seq->eggSlot], GENERAL_BUFFER_PTR);
		loadMMDAsync(EGG_DIGIMON_TYPES[seq->eggSlot], 3, DOOA_MMD_BUFFER, (EvoModelData *)seq->modelData,
		             (uint8_t *)&seq->isModelLoading);
		DOO2_resetShardSets(DOOA_SHARD_BUFFER);
		playSound(8, 6);
		break;
	case 0xce:
		frame = seq->frame;
		if (frame < 147) {
			seq->fadeLevel = lerp(5, 0, 132, 150, frame);
		}
		flashPos = DOOA_FLASH_POSITION;
		EFE_PUSH1(int32_t, 0);
		EFE_PUSH1(VECTOR *, &flashPos);
		EFE_PUSH1(int32_t, -1);
		EFE_PUSH1(int32_t, 4);
		EFE_PUSH1(int32_t, 0);
		level = lerp(20, 10, 132, 152, seq->frame);
		EFE_PUSH1(int32_t, level * 25 * 4096 / 1000);
		level = lerp(120, 10, 132, 152, seq->frame);
		colorStart.vx = level + (rand() % ((level * 50) / 120));
		colorStart.vy = level + (rand() % ((level * 50) / 120));
		colorStart.vz = level + (rand() % ((level * 50) / 120));
		colorEnd.vx = colorEnd.vy = colorEnd.vz = level / 6;
		EFE_PUSH1(VECTOR *, &colorStart);
		EFE_PUSH1(VECTOR *, &colorEnd);
		createFlash();
		if (seq->frame >= 152) {
			while (seq->isModelLoading != 0) {
				tickFileReadQueue(0);
			}
			reincarnatePartner((int32_t)ENTITY_TABLE[1], &PARTNER_ENTITY.digimonEntity.stats, &PARTNER_PARA, EGG_DIGIMON_TYPES[seq->eggSlot]);
			waitForSoundBufferLoading(3);
			entity = (Entity *)&PARTNER_ENTITY;
			seq->entity = entity;
			setEntityPosition(1, 0, 0, 0);
			DOOA_setOtherEntitiesVisible(1);
			ENTITY_TABLE[2]->isOnMap = 1;
			DOOA_showPlayerAndPartner();
			ENTITY_TABLE[1]->isOnMap = 0;
			DOOA_fadeModelClut(DOOA_MODEL_CLUT, entity, DOOA_FADED_CLUT, 0, 1, 1);
			DOOA_fadeShardClut(DOOA_SHARD_CLUT, entity, DOOA_FADED_CLUT, 0, 1, 1);
			setMapLayerEnabled(1);
			seq->phase = 0xcf;
		}
		break;
	case 0xcf:
		frame = seq->frame;
		if (frame >= 202) {
			seq->phase = 0xd0;
			seq->sparkleIndex = 0;
			ENTITY_TABLE[1]->isOnMap = 1;
			startAnimation((Entity *)&PARTNER_ENTITY, 0x1c);
			MAIN_func_800D9BA8(0, DOOA_SCENE_CLUT, 0);
			DOOA_fadeModelClut(DOOA_MODEL_CLUT, entity, DOOA_FADED_CLUT, 0, 1, 0);
			DOOA_fadeShardClut(DOOA_SHARD_CLUT, entity, DOOA_FADED_CLUT, 0, 1, 0);
			break;
		}
		if (seq->frame < 152) {
			break;
		}
		level = lerp(255, 0, 152, 202, frame);
		MAIN_func_800D9BA8(level, DOOA_SCENE_CLUT, 0);
		if ((seq->frame & 1) == 0) {
			DOOA_fadeModelClut(DOOA_MODEL_CLUT, entity, DOOA_FADED_CLUT, 0, 255, level);
		} else {
			DOOA_fadeShardClut(DOOA_SHARD_CLUT, entity, DOOA_FADED_CLUT, 0, 255, level);
		}
		break;
	case 0xd0:
		if (seq->sparkleIndex < 47) {
			sparkle.offset = DOOA_SPARKLE_OFFSET;
			if ((sparkle.boneId = DOOA_SPARKLE_BONE_IDS[seq->sparkleIndex]) >= 0) {
				DOO2_buildShardSet(&sparkle.offset, DOO2_D_80071EE4, (seq->eggSlot * 6) + sparkle.boneId);
				MAIN_D_80135364[DOOA_SPARKLE_BONE_IDS[seq->sparkleIndex]] = -1;
				playSound(8, 7);
			}
			seq->sparkleIndex++;
			break;
		}
		if (seq->frame < 262) {
			break;
		}
		seq->phase = 7;
		startAnimation((Entity *)&PARTNER_ENTITY, 0xb);
		break;
	case 7:
		if (entity->anim.animFrame != entity->anim.frameCount) {
			break;
		}
		DOO2_releaseAllShardSets();
		removeObject(0x80c, instanceId);
		seq->frame = -1;
		break;
	}
}

void DOOA_renderRebirth(int32_t instanceId)
{
	GsDOBJ2 obj;
	GsCOORDINATE2 coord;
	MATRIX lightMatrix;
	MATRIX lsMatrix;
	VECTOR pos;
	SVECTOR rot;
	int32_t i;
	DooaSequence *panel;
	u_long tmd;

	panel = &DOOA_REINCARNATION_SEQ;
	i = 0;
	tmd = (u_long)&DOO2_D_80071EE4[3];

	for (; i < 6; i++) {
		if (MAIN_D_80135364[i] == 0) {
			GsLinkObject4(tmd, &obj, i + (panel->eggSlot * 6));
			obj.attribute = 0;
			GsInitCoordinate2(NULL, &coord);
			obj.coord2 = &coord;
			GsGetLws(obj.coord2, &lightMatrix, &lsMatrix);
			GsSetLightMatrix(&lightMatrix);
			GsSetLsMatrix(&lsMatrix);
			DOO2_renderWireframeModel(&obj, MAIN_D_80135324);
		}
	}

	if (panel->fadeLevel >= 0) {
		for (i = 0; i < panel->fadeLevel; i++) {
			int32_t range;

			range = customRandom(-170, 1024);
			rot.vx = rand() % range;
			rot.vy = rand();
			rot.vz = 0;
			pos.vx = panel->entity->posData->location.vx;
			pos.vy = panel->entity->posData->location.vy - 100;
			pos.vz = panel->entity->posData->location.vz;
			DOO2_renderSparkStreak((int32_t *)&pos, &rot);
		}
	}
}

void DOOA_setShardState(int16_t state)
{
	DOOA_SHARD_EFFECT.state = state;
}

void DOOA_removeShardEffect(void)
{
	removeObject(0x608, 0);
}

void DOOA_showPlayerAndPartner(void)
{
	ENTITY_TABLE[0]->isOnMap = 1;
	ENTITY_TABLE[1]->isOnMap = 1;
}

void DOOA_fadeModelClut(int16_t *srcClut, void *unused, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame)
{
	int32_t i;
	int16_t stp;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t *dst;
	RECT rect;

	dst = dstClut;
	for (i = 0; i < 384; i++) {
		int32_t den;
		int32_t num;

		num = endFrame - frame;
		den = endFrame - startFrame;

		r = *srcClut & 0x1f;
		g = (*srcClut >> 5) & 0x1f;
		b = (*srcClut >> 10) & 0x1f;
		stp = (*srcClut++ >> 15) & 0x1;

		if (frame != startFrame) {
			stp = 1;
		}

		r = r * num / den;
		g = g * num / den;
		b = b * num / den;

		*dst = r;
		*dst += g << 5;
		*dst += b << 10;
		*dst++ += stp << 15;
	}

	setRECT(&rect, 0, 488, 16, 24);
	LoadImage(&rect, (u_long *)dstClut);
	DrawSync(0);
}

void DOOA_fadeShardClut(int16_t *srcClut, void *unused, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame)
{
	int32_t i;
	int16_t stp;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t *dst;
	RECT rect;

	dst = dstClut;
	for (i = 0; i < 768; i++) {
		int32_t den;
		int32_t num;

		num = endFrame - frame;
		den = endFrame - startFrame;

		r = *srcClut & 0x1f;
		g = (*srcClut >> 5) & 0x1f;
		b = (*srcClut >> 10) & 0x1f;
		stp = (*srcClut++ >> 15) & 0x1;

		if (frame != startFrame) {
			stp = 1;
		}

		r = r * num / den;
		g = g * num / den;
		b = b * num / den;

		*dst = r;
		*dst += g << 5;
		*dst += b << 10;
		*dst++ += stp << 15;
	}

	setRECT(&rect, 48, 488, 32, 24);
	LoadImage(&rect, (u_long *)dstClut);
	DrawSync(0);
}

void DOOA_getOrbitPosition(VECTOR *outRef, VECTOR *outPos, VECTOR *position, SVECTOR *rotation, int32_t distance, int32_t height)
{
	MATRIX matrix;
	VECTOR direction;
	int32_t y;

	outRef->vx = position->vx;
	y = position->vy;
	outRef->vy = y - (height / 2);
	outRef->vz = position->vz;
	RotMatrixZYX(rotation, &matrix);
	direction.vx = 0;
	direction.vy = 0;
	direction.vz = distance;
	ApplyMatrixLV(&matrix, &direction, outPos);
	outPos->vx += outRef->vx;
	outPos->vy += (outRef->vy - 400) - (height * 2);
	outPos->vz += outRef->vz;
}

int32_t DOOA_updateShards(int32_t instanceId)
{
	TMD_P_TG4 **cursor;
	DooaShard *shard;
	DooaShardEffect *effect;
	int16_t state;

	effect = &DOOA_SHARD_EFFECT;

	cursor = DOOA_SHARD_EFFECT.shardBuffer;
	if (DOOA_SHARD_EFFECT.flash != 0) {
		effect->colorR = (rand() % 100) + 60;
		effect->colorG = (rand() % 100) + 60;
		effect->colorB = (rand() % 100) + 60;
	}

	while (*cursor != NULL) {
		shard = (DooaShard *)((intptr_t)cursor);
		state = effect->state;
		switch (state) {
		case 0:
			shard->fallSpeed += 1;
			if (shard->fallSpeed >= 0x400) {
				shard->fallSpeed = 0x400;
			}
			shard->centerY += shard->fallSpeed;
			if (shard->centerY > 0) {
				shard->centerY = 0;
			}
			break;
		case 1:
			if (effect->state != effect->prevState) {
				int32_t minRadius;
				int32_t maxRadius;

				shard->spin = 0;
				shard->spinMax = (rand() % 0x155) + 0xe3;
				shard->radius = 0x1000;
				minRadius = 0x64000 / shard->axisDistance;
				maxRadius = 0x190000 / shard->axisDistance;
				shard->targetRadius = minRadius + (rand() % (maxRadius - minRadius));
				shard->fallSpeed = 0;
				shard->dropDepth = rand() % 0x226;
			}
			if ((shard->spinMax / 80) == 0) {
				shard->spin += 1;
			} else {
				int16_t step;

				step = shard->spinMax / 100;
				shard->spin += step;
			}
			if (shard->spin > shard->spinMax) {
				shard->spin = shard->spinMax;
			}
			shard->rotY += shard->spin;
			if (shard->targetRadius > shard->radius) {
				if ((shard->targetRadius / 80) == 0) {
					shard->radius++;
				} else {
					shard->radius += shard->targetRadius / 100;
				}
			}
			shard->fallSpeed += 1;
			{
				int16_t drop;

				drop = shard->fallSpeed >> 3;
				shard->centerY -= drop;
			}
			if (shard->centerY < -shard->dropDepth) {
				shard->centerY = -shard->dropDepth;
			}
			break;
		case 2:
			shard->rotY += shard->spin;
			break;
		case 3:
			if (effect->state != effect->prevState) {
				shard->delay = rand() % 20;
				shard->targetRadius = shard->radius;
			}
			if (shard->delay > 0) {
				--shard->delay;
			} else {
				shard->radius -= (shard->targetRadius / 10) + 1;
				if (shard->radius < 0) {
					shard->radius = 0;
					shard->delay = -1;
				}
				shard->centerY = lerp(-shard->dropDepth, MAIN_func_800DA9F4() - 100, shard->targetRadius, 0, shard->radius);
			}
			shard->rotY += shard->spin;
			shard->rotY %= 4096;
			break;
		}

		switch ((*cursor)->cd) {
		case GPU_COM_TG3:
		case GPU_COM_TG3 | 2:
			cursor = (TMD_P_TG4 **)((intptr_t)cursor + (int32_t)sizeof(DooaShard));
			break;
		case GPU_COM_TG4:
		case GPU_COM_TG4 | 2:
			cursor = (TMD_P_TG4 **)((intptr_t)cursor + (int32_t)sizeof(DooaShardQuad));
			break;
		}
	}

	effect->prevState = effect->state;
}

int32_t DOOA_renderShards(int32_t instanceId)
{
	SVECTOR triA;
	SVECTOR triB;
	SVECTOR triC;
	MATRIX triMatrix;
	SVECTOR quadA;
	SVECTOR quadB;
	SVECTOR quadC;
	SVECTOR quadD;
	MATRIX quadMatrix;
	TMD_P_TG4 *tmdPrim;
	POLY_FT3 *triPrim;
	POLY_FT4 *quadPrim;
	DooaShard *tri;
	DooaShardQuad *quad;
	DooaShardEffect *effect;
	intptr_t cursor;
	ModelComponent *model;
	int32_t triScale;
	int32_t quadScale;
	int16_t triOx;
	int16_t triOy;
	int16_t triOz;
	int16_t quadOx;
	int16_t quadOy;
	int16_t quadOz;
	uint8_t mode;

	cursor = (intptr_t)DOOA_SHARD_EFFECT.shardBuffer;
	effect = &DOOA_SHARD_EFFECT;
	model = getEntityModelComponent(DOOA_SHARD_EFFECT.entity->type, 3);

	while (*(int32_t *)cursor != 0) {
		tmdPrim = *(TMD_P_TG4 **)cursor;
		mode = tmdPrim->cd;
		switch (mode) {
		case GPU_COM_TG3:
		case GPU_COM_TG3 | 2:
			tri = (DooaShard *)cursor;
			if (tri->delay >= 0) {
				triPrim = (POLY_FT3 *)GsGetWorkBase();
				MAIN_func_80092B60((POLY_FT4 *)triPrim);
				if (effect->flash != 0) {
					setRGB0(triPrim, effect->colorR, effect->colorG, effect->colorB);
					triPrim->tpage = getTPage(1, 0, 832, 256);
					triPrim->clut = getClut(0, 487);
					setUV3(triPrim, 0, 128, 3, 128, 0, 131);
				} else {
					setRGB0(triPrim, effect->colorR, effect->colorB, effect->colorG);
					triPrim->tpage = model->pixelPage;
					triPrim->clut = tmdPrim->clut;
					setUV3(triPrim, tmdPrim->tu0, tmdPrim->tv0, tmdPrim->tu1, tmdPrim->tv1, tmdPrim->tu2, tmdPrim->tv2);
				}
				triScale = tri->radius;
				triOx = tri->centerX * triScale / 4096;
				triOy = tri->centerY;
				triScale = tri->radius;
				triOz = tri->centerZ * triScale / 4096;
				triA.vx = triOx + tri->vertex[0].vx;
				triA.vy = triOy + tri->vertex[0].vy;
				triA.vz = triOz + tri->vertex[0].vz;
				triB.vx = triOx + tri->vertex[1].vx;
				triB.vy = triOy + tri->vertex[1].vy;
				triB.vz = triOz + tri->vertex[1].vz;
				triC.vx = triOx + tri->vertex[2].vx;
				triC.vy = triOy + tri->vertex[2].vy;
				triC.vz = triOz + tri->vertex[2].vz;
				RotMatrixZYX((SVECTOR *)&tri->rotX, &triMatrix);
				ApplyMatrixSV(&triMatrix, &triA, &triA);
				ApplyMatrixSV(&triMatrix, &triB, &triB);
				ApplyMatrixSV(&triMatrix, &triC, &triC);
				triA.vx += effect->entity->posData->location.vx;
				triA.vy += MAIN_func_800DA9F4();
				triA.vz += effect->entity->posData->location.vz;
				triB.vx += effect->entity->posData->location.vx;
				triB.vy += MAIN_func_800DA9F4();
				triB.vz += effect->entity->posData->location.vz;
				triC.vx += effect->entity->posData->location.vx;
				triC.vy += MAIN_func_800DA9F4();
				triC.vz += effect->entity->posData->location.vz;
				addScreenPolyFT3(triPrim, &triA, &triB, &triC);
			}
			cursor += sizeof(DooaShard);
			break;
		case GPU_COM_TG4:
		case GPU_COM_TG4 | 2:
			quad = (DooaShardQuad *)cursor;
			if (quad->delay >= 0) {
				quadPrim = (POLY_FT4 *)GsGetWorkBase();
				SetPolyFT4(quadPrim);
				if (effect->flash != 0) {
					setRGB0(quadPrim, effect->colorR, effect->colorG, effect->colorB);
					quadPrim->tpage = getTPage(1, 0, 832, 256);
					quadPrim->clut = getClut(0, 487);
					setUVWH(quadPrim, 0, 128, 3, 3);
				} else {
					setRGB0(quadPrim, effect->colorR, effect->colorB, effect->colorG);
					quadPrim->tpage = model->pixelPage;
					quadPrim->clut = tmdPrim->clut;
					setUV4(quadPrim, tmdPrim->tu0, tmdPrim->tv0, tmdPrim->tu1, tmdPrim->tv1, tmdPrim->tu2, tmdPrim->tv2, tmdPrim->tu3, tmdPrim->tv3);
				}
				quadScale = quad->radius;
				quadOx = quad->centerX * quadScale / 4096;
				quadOy = quad->centerY;
				quadScale = quad->radius;
				quadOz = quad->centerZ * quadScale / 4096;
				quadA.vx = quadOx + quad->vertex[0].vx;
				quadA.vy = quadOy + quad->vertex[0].vy;
				quadA.vz = quadOz + quad->vertex[0].vz;
				quadB.vx = quadOx + quad->vertex[1].vx;
				quadB.vy = quadOy + quad->vertex[1].vy;
				quadB.vz = quadOz + quad->vertex[1].vz;
				quadC.vx = quadOx + quad->vertex[2].vx;
				quadC.vy = quadOy + quad->vertex[2].vy;
				quadC.vz = quadOz + quad->vertex[2].vz;
				quadD.vx = quadOx + quad->vertex[3].vx;
				quadD.vy = quadOy + quad->vertex[3].vy;
				quadD.vz = quadOz + quad->vertex[3].vz;
				RotMatrixZYX((SVECTOR *)&quad->rotX, &quadMatrix);
				ApplyMatrixSV(&quadMatrix, &quadA, &quadA);
				ApplyMatrixSV(&quadMatrix, &quadB, &quadB);
				ApplyMatrixSV(&quadMatrix, &quadC, &quadC);
				ApplyMatrixSV(&quadMatrix, &quadD, &quadD);
				quadA.vx += effect->entity->posData->location.vx;
				quadA.vy += MAIN_func_800DA9F4();
				quadA.vz += effect->entity->posData->location.vz;
				quadB.vx += effect->entity->posData->location.vx;
				quadB.vy += MAIN_func_800DA9F4();
				quadB.vz += effect->entity->posData->location.vz;
				quadC.vx += effect->entity->posData->location.vx;
				quadC.vy += MAIN_func_800DA9F4();
				quadC.vz += effect->entity->posData->location.vz;
				quadD.vx += effect->entity->posData->location.vx;
				quadD.vy += MAIN_func_800DA9F4();
				quadD.vz += effect->entity->posData->location.vz;
				add3DSpritePrim(quadPrim, &quadA, &quadB, &quadC, &quadD);
			}
			cursor += sizeof(DooaShardQuad);
			break;
		}
	}
}

void DOOA_spawnBoneShards(DooaShardEffect *effect, int32_t boneIndex, int32_t wireIndex)
{
	MATRIX boneMatrix;
	SVECTOR rotated;
	Entity *entity;
	struct TMD_STRUCT *objs;
	struct TMD_STRUCT *obj;
	SVECTOR *src;
	DooaShardVertex *verts;
	DooaShard *frag;
	int32_t objIndex;
	intptr_t vertOut;
	intptr_t frags;
	intptr_t prim;
	int32_t i;
	int32_t j;

	entity = effect->entity;
	frags = (intptr_t)effect->shardWrite;
	objs = getEntityModelComponent(entity->type, 3)->modelPtr->obj;
	objIndex = DIGIMON_SKELETONS[entity->type][boneIndex].objIndex;
	if (objIndex == -1) {
		return;
	}

	obj = &objs[objIndex];
	vertOut = (intptr_t)GsGetWorkBase();
	src = (SVECTOR *)obj->vertop;
	calculateBoneMatrix(entity, boneIndex, &boneMatrix);
	verts = (DooaShardVertex *)vertOut;
	for (i = 0; i < obj->vern; i++) {
		ApplyMatrixSV(&boneMatrix, src++, &rotated);
		((DooaShardVertex *)vertOut)->vx = rotated.vx + boneMatrix.t[0];
		((DooaShardVertex *)vertOut)->vy = rotated.vy + boneMatrix.t[1];
		((DooaShardVertex *)vertOut)->vz = rotated.vz + boneMatrix.t[2];
		vertOut += sizeof(DooaShardVertex);
	}

	prim = (intptr_t)obj->primtop;
	for (j = 0; j < obj->primn; j++) {
		if (((uintptr_t)frags + sizeof(DooaShardQuad) + sizeof(TMD_P_TG4 *)) >= ((intptr_t)effect->shardBuffer + effect->shardBytes)) {
			break;
		}
		if (((rand() & 3) != 0) && (WIREFRAME_RNG_TABLE[j & 0xf] == wireIndex)) {
			switch (((int8_t *)prim)[3]) {
			case GPU_COM_TG3:
			case GPU_COM_TG3 | 2:
				frag = (DooaShard *)frags;
				frag->prim = (TMD_P_TG4 *)prim;
				frag->centerX = (verts[((TMD_P_TG3 *)prim)->v0].vx + verts[((TMD_P_TG3 *)prim)->v1].vx + verts[((TMD_P_TG3 *)prim)->v2].vx) / 3;
				frag->centerY = (verts[((TMD_P_TG3 *)prim)->v0].vy + verts[((TMD_P_TG3 *)prim)->v1].vy + verts[((TMD_P_TG3 *)prim)->v2].vy) / 3;
				frag->centerZ = (verts[((TMD_P_TG3 *)prim)->v0].vz + verts[((TMD_P_TG3 *)prim)->v1].vz + verts[((TMD_P_TG3 *)prim)->v2].vz) / 3;
				frag->vertex[0].vx = verts[((TMD_P_TG3 *)prim)->v0].vx - frag->centerX;
				frag->vertex[0].vy = verts[((TMD_P_TG3 *)prim)->v0].vy - frag->centerY;
				frag->vertex[0].vz = verts[((TMD_P_TG3 *)prim)->v0].vz - frag->centerZ;
				frag->vertex[1].vx = verts[((TMD_P_TG3 *)prim)->v1].vx - frag->centerX;
				frag->vertex[1].vy = verts[((TMD_P_TG3 *)prim)->v1].vy - frag->centerY;
				frag->vertex[1].vz = verts[((TMD_P_TG3 *)prim)->v1].vz - frag->centerZ;
				frag->vertex[2].vx = verts[((TMD_P_TG3 *)prim)->v2].vx - frag->centerX;
				frag->vertex[2].vy = verts[((TMD_P_TG3 *)prim)->v2].vy - frag->centerY;
				frag->vertex[2].vz = verts[((TMD_P_TG3 *)prim)->v2].vz - frag->centerZ;
				frag->centerX -= effect->entity->posData->location.vx;
				frag->centerY -= MAIN_func_800DA9F4();
				frag->centerZ -= effect->entity->posData->location.vz;
				frag->fallSpeed = 0;
				frag->radius = 0x1000;
				frag->rotX = 0;
				frag->rotY = 0;
				frag->rotZ = 0;
				frag->axisDistance = getDistance(frag->centerX, 0, frag->centerZ);
				if (frag->axisDistance == 0) {
					frag->axisDistance = 1;
				}
				frag->delay = 0;
				frags += sizeof(DooaShard);
				break;
			case GPU_COM_TG4:
			case GPU_COM_TG4 | 2:
				frag = (DooaShard *)frags;
				frag->prim = (TMD_P_TG4 *)prim;
				frag->centerX = (verts[((TMD_P_TG4 *)prim)->v0].vx + verts[((TMD_P_TG4 *)prim)->v1].vx + verts[((TMD_P_TG4 *)prim)->v2].vx + verts[((TMD_P_TG4 *)prim)->v3].vx) / 4;
				frag->centerY = (verts[((TMD_P_TG4 *)prim)->v0].vy + verts[((TMD_P_TG4 *)prim)->v1].vy + verts[((TMD_P_TG4 *)prim)->v2].vy + verts[((TMD_P_TG4 *)prim)->v3].vy) / 4;
				frag->centerZ = (verts[((TMD_P_TG4 *)prim)->v0].vz + verts[((TMD_P_TG4 *)prim)->v1].vz + verts[((TMD_P_TG4 *)prim)->v2].vz + verts[((TMD_P_TG4 *)prim)->v3].vz) / 4;
				frag->vertex[0].vx = verts[((TMD_P_TG4 *)prim)->v0].vx - frag->centerX;
				frag->vertex[0].vy = verts[((TMD_P_TG4 *)prim)->v0].vy - frag->centerY;
				frag->vertex[0].vz = verts[((TMD_P_TG4 *)prim)->v0].vz - frag->centerZ;
				frag->vertex[1].vx = verts[((TMD_P_TG4 *)prim)->v1].vx - frag->centerX;
				frag->vertex[1].vy = verts[((TMD_P_TG4 *)prim)->v1].vy - frag->centerY;
				frag->vertex[1].vz = verts[((TMD_P_TG4 *)prim)->v1].vz - frag->centerZ;
				frag->vertex[2].vx = verts[((TMD_P_TG4 *)prim)->v2].vx - frag->centerX;
				frag->vertex[2].vy = verts[((TMD_P_TG4 *)prim)->v2].vy - frag->centerY;
				frag->vertex[2].vz = verts[((TMD_P_TG4 *)prim)->v2].vz - frag->centerZ;
				frag->vertex[3].vx = verts[((TMD_P_TG4 *)prim)->v3].vx - frag->centerX;
				frag->vertex[3].vy = verts[((TMD_P_TG4 *)prim)->v3].vy - frag->centerY;
				frag->vertex[3].vz = verts[((TMD_P_TG4 *)prim)->v3].vz - frag->centerZ;
				frag->centerX -= effect->entity->posData->location.vx;
				frag->centerY -= MAIN_func_800DA9F4();
				frag->centerZ -= effect->entity->posData->location.vz;
				frag->fallSpeed = 0;
				frag->radius = 0x1000;
				frag->rotX = 0;
				frag->rotY = 0;
				frag->rotZ = 0;
				frag->axisDistance = getDistance(frag->centerX, 0, frag->centerZ);
				if (frag->axisDistance == 0) {
					frag->axisDistance = 1;
				}
				frag->delay = 0;
				frags += sizeof(DooaShardQuad);
				break;
			}
		}
		switch (((int8_t *)prim)[3]) {
		case GPU_COM_TG3:
		case GPU_COM_TG3 | 2:
			prim += sizeof(TMD_P_TG3);
			break;
		case GPU_COM_TG4:
		case GPU_COM_TG4 | 2:
			prim += sizeof(TMD_P_TG4);
			break;
		}
	}

	((DooaShard *)frags)->prim = 0;
	effect->shardWrite = (void *)frags;
}

int32_t DOOA_tick(PartnerEntity *partner, void *buffer, int32_t isInitialized)
{
	DooaSequence *panel;
	Entity *player;
	int32_t messageId;

	panel = &DOOA_REINCARNATION_SEQ;
	if (isInitialized != 0) {
		return panel->frame;
	}
	initializeFlashData((char *)DOOA_FLASH_DATA.data);
	if (partner->lives != 0) {
		panel->frame = 0;
	} else {
		panel->frame = 0x23;
		partner->digimonEntity.entity.isOnMap = 0;
	}
	panel->entity = &partner->digimonEntity.entity;
	panel->phase = 0;
	addObject(0x80b, 0, DOOA_tickDissolve, DOOA_renderDissolve);
	DOOA_CAMERA_START_VIEW = GS_VIEWPOINT;
	MAIN_D_8013532C = DRAWING_OFFSET_X;
	MAIN_D_80135330 = DRAWING_OFFSET_Y;
	MAIN_D_80135334 = VIEWPORT_DISTANCE;
	player = ENTITY_TABLE[1];
	DOOA_SAVED_LOCATION = player->posData->location;
	MAIN_D_80135338 = player->posData->rotation;
	if (partner->lives != 0) {
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 1);
	}
	DOOA_initOrderingTable();
	if ((isTriggerSet(0xdc) == 1) || (isTriggerSet(0xd6) == 1) || (readPStat(1) >= 50)) {
		messageId = 0xcd;
	} else {
		messageId = 0xda;
	}
	MAIN_D_80135340 = messageId;
	MAIN_func_800D91EC(messageId, 1);
	return (intptr_t)buffer;
}

int32_t DOOA_getSequenceState(int32_t unused, int32_t isInitialized)
{
	PartnerEntity *partner;
	DooaSequence *sequence;
	int32_t i;

	partner = (PartnerEntity *)DOOA_REINCARNATION_SEQ.entity;
	sequence = &DOOA_REINCARNATION_SEQ;

	if (isInitialized != 0) {
		return sequence->frame;
	}

	if (partner->lives == 0) {
		sequence->eggSlot = rand() % 4;
	}

	addObject(0x80c, 0, DOOA_tickRebirth, DOOA_renderRebirth);
	sequence->phase = 200;
	sequence->fadeLevel = 0;
	sequence->frame = 0;
	MAIN_D_80135324 = 0;

	for (i = 0; i < 6; i++) {
		MAIN_D_80135364[i] = 0;
	}

	return 0;
}
