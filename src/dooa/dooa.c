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
#include <dw/garbage.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/rng.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/sound_async.h>
#include <dw/types.h>
#include <dw/utils.h>
#include <dw/vecmath.h>
#include <dw/world_object.h>

#define DOOA_MMD_BUFFER		0x80020000
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
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t ACTIVE_FRAMEBUFFER;
extern VECTOR CAMERA_TARGET;
extern int8_t CAMERA_REACHED_TARGET;
extern int32_t FLASH_INSTANCE;

void DOOA_renderDigimonModel(Entity *entity, uint32_t otPoint);
int32_t DOOA_renderIrisWindow(Entity *entity, int32_t startFrame, long endFrame, long frame);
void DOOA_renderDissolve(int32_t instanceId);
int32_t lerp(long start, long end, int32_t t0, long t1, int32_t t);
int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out);
int32_t DOOA_hasIrisClosed(Entity *entity, int32_t startFrame, long endFrame, long frame);
void DOOA_saveShardClut(int32_t buffer);
void DOOA_setOtherEntitiesVisible(int32_t restore);
void DOOA_hideAllButPartner(void);
void DOOA_getOrbitPosition(VECTOR *outRef, VECTOR *outPos, VECTOR *position, SVECTOR *rotation, int32_t distance, int32_t height);
void DOOA_updateCutsceneCamera(VECTOR *position, int32_t angle, int32_t startFrame, int32_t endFrame, int32_t frame);
void DOOA_toggleShardFlicker(void);
void DOOA_renderRebirth(int32_t instanceId);
void DOOA_setShardState(int16_t state);
void DOOA_removeShardEffect(void);
void DOOA_showPlayerAndPartner(void);
void DOOA_fadeModelClut(int16_t *srcClut, void *unused, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame);
void DOOA_fadeShardClut(int16_t *srcClut, void *unused, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame);
int32_t DOOA_getStoredDigimonY(void);
int32_t DOOA_updateShards(int32_t instanceId);
int32_t DOOA_renderShards(int32_t instanceId);
int32_t DOOA_initShardEffect(Entity *entity, intptr_t addr, int32_t size);
void DOOA_saveEntityClut(int32_t buffer, Entity *entity);
void DOOA_saveModelClut(int32_t buffer);
void renderDropShadow(Entity *entity);
void EFECreateFlash(void);
void setMapLayerEnabled(int32_t enabled);
void DOOA_tickRebirth(int32_t instanceId);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
void DOOA_spawnBoneShards(DooaShardEffect *effect, int32_t boneIndex, long wireIndex);
char *initializeFlashData(char *base);
void setDeathMap(int32_t messageId, int32_t flag);
void DOOA_tickDissolve(int32_t instanceId);
void DOOA_initOrderingTable(void);
void DOOA_spawnShardWave(long wireIndex);
void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
int32_t addScreenPolyFT4(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
void tickCameraMovement(int32_t mode);
void setEFEFlashOffset(int32_t instance, int32_t x, int32_t y);
void waitForDeathMapLoading(int32_t mode);
void changeToDeathMap(void);
void downloadCLUT1(int16_t *clut);
void fadeoutCLUT1(int32_t alpha, int16_t *clut, int32_t mode);
void DOOA_storeDigimonY(void);
void renderParticleFlash(ParticleFlashData *params);

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

int16_t REINCARNATE_BABY_TYPE[4] = { 1, 15, 29, 43 };
SVECTOR MAIN_D_80134BB4 = { 0 };
int8_t DOOA_ENTITIES_VISIBLE = 1;

int16_t MAIN_D_80135324;
int32_t MAIN_D_80135328;
int32_t MAIN_D_8013532C;
int32_t MAIN_D_80135330;
int32_t MAIN_D_80135334;
SVECTOR MAIN_D_80135338;
int32_t DEATH_MAP_TARGET;
int8_t DOO2_LOADING_COMPLETE;
int32_t MAIN_D_80135348;
int32_t MAIN_D_8013534C;
int32_t MAIN_D_80135350;
int32_t MAIN_D_80135354;
int32_t MAIN_D_80135358;
SVECTOR MAIN_D_8013535C;
int8_t MAIN_D_80135364[8];

static void *dooa_sbss_order[] = {
	&MAIN_D_80135364,
	&MAIN_D_8013535C,
	&MAIN_D_80135358,
	&MAIN_D_80135354,
	&MAIN_D_80135350,
	&MAIN_D_8013534C,
	&MAIN_D_80135348,
	&DOO2_LOADING_COMPLETE,
	&DEATH_MAP_TARGET,
	&MAIN_D_80135338,
	&MAIN_D_80135334,
	&MAIN_D_80135330,
	&MAIN_D_8013532C,
	&MAIN_D_80135328,
	&MAIN_D_80135324,
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

char DOOA_EGG_TIM_PATH[] = "\\ETCDAT\\TAMA.TIM";
char DOOA_EGG_TMD_PATH[] = "\\ETCDAT\\TAMA.TMD";

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

GARBAGE(DOOA_tickDissolve, 10);

void DOOA_tickDissolve(int32_t instanceId)
{
#ifdef __MWERKS__
	extern void setEntityPosition(int32_t entityId, long x, long y, long z);
#endif
	int32_t index;
	GsRVIEW2 savedView;
	int32_t savedOffsetX;
	int32_t savedOffsetY;
	int32_t savedDistance;
	ShardWaveSchedule wireCounts;
	DooaFlash *target;
	DissolveScaleCurve heights;
	DooaFlash *landing;
	DooaFlash *flash;
	VECTOR startColor;
	VECTOR endColor;
	u_long *tim;
	u_long *file;
	GsIMAGE timInfo;
	RECT rect;
	VECTOR cameraTarget;
	DooaSequence *seq;
	Entity *entity;
	long work;

	seq = &DOOA_REINCARNATION_SEQ;
	entity = seq->entity;
	seq->frame++;

	if ((entity->anim.animId == 0xc) && (entity->anim.animFrame == entity->anim.frameCount)) {
		startAnimation((Entity *)&PARTNER_ENTITY, 1);
		DOOA_storeDigimonY();
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
		waitForDeathMapLoading(0);
		changeToDeathMap();
		stopBGM();
		loadMapSounds2(0x13);
		isSoundLoaded(0, 8);
		if (((PartnerEntity *)entity)->lives != 0) {
			setSomeDyingState();
		} else {
			setSomeDyingState();
		}
		entity = ENTITY_TABLE[1];
		seq->entity = entity;
		ENTITY_TABLE[1]->isOnMap = 0;
		loadDynamicLibrary(8, (uint8_t *)&DOO2_LOADING_COMPLETE, 1, 0, 0);
		while (DOO2_LOADING_COMPLETE != 0) {
			tickFileReadQueue(0);
		}
		DOOA_initShardEffect(entity, DOOA_SHARD_BUFFER, DOOA_SHARD_BUFFER_SIZE);
		DOOA_saveEntityClut((int32_t)DOOA_SAVED_ENTITY_CLUT, entity);
		DOOA_saveModelClut((int32_t)DOOA_MODEL_CLUT);
		DOOA_saveShardClut((int32_t)DOOA_SHARD_CLUT);
		MAIN_D_80135348 = DRAWING_OFFSET_X;
		MAIN_D_8013534C = DRAWING_OFFSET_Y;
		DOOA_SAVED_VIEW = GS_VIEWPOINT;
		if (DEATH_MAP_TARGET == 0xcd) {
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
		downloadCLUT1(DOOA_SCENE_CLUT);
		fadeoutCLUT1(0xff, DOOA_SCENE_CLUT, 0);
		break;
	case 1:
		entity->isOnMap = 2;
		if (seq->frame >= 0x56) {
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
			DOOA_updateCutsceneCamera(&entity->posData->location, entity->posData->rotation.vy, 0x23, 0x55, seq->frame);
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
			DOOA_storeDigimonY();
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
		if (seq->frame < 0x8d) {
			work = seq->frame - 0x69;
			PARTNER_WIREFRAME_TOTAL = wireCounts.v[work];
			if ((work == 0) || (wireCounts.v[work - 1] != wireCounts.v[work])) {
				DOOA_spawnShardWave(wireCounts.v[work]);
			}
		}
		if (seq->frame >= 0xbf) {
			seq->phase = 0x64;
			DOOA_SAVED_LOCATION.vy = entity->posData->location.vy;
			target = &seq->flash;
			target->targetY = DOOA_getStoredDigimonY() - (DIGIMON_DATA[entity->type].height + 100);
			target->pos.vx = entity->posData->location.vx;
			target->pos.vz = entity->posData->location.vz;
			stopSound();
			playSound(8, 1);
		}
		break;
	case 0x64:
		heights = DOOA_DISSOLVE_SCALE_CURVE;
		index = lerp(0, 0x1f, 0xbf, 0xd0, seq->frame);
		work = (((heights.v[index] - 100) * 200) / 100) + 100;
		entity->posData->scale.vy = (work << 12) / 100;
		entity->posData->scale.vx = ((10000 / work) << 12) / 100;
		entity->posData->scale.vx = lerp(entity->posData->scale.vx, entity->posData->scale.vx * 50 / 100, 0xbf, 0xd0, seq->frame);
		entity->posData->scale.vz = entity->posData->scale.vx;
		if (heights.v[index] >= 0x12d) {
			work = heights.v[index];
			work = 0x12c - (work - 0x12c);
			work = ((((work - 100) * 200) / 100) + 100);
			entity->posData->scale.vy = (work << 12) / 100;
			landing = &seq->flash;
			ENTITY_TABLE[1]->anim.animFlag &= 0xfe;
			entity->posData->location.vy = lerp(entity->posData->location.vy, landing->targetY, seq->frame - 1, seq->frame, seq->frame);
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
		EFECreateFlash();
		work = lerp(0, 0x600, 0xd1, 0x135, seq->frame);
		work = _sin(work) * 10 / 4096;
		setEFEFlashOffset(FLASH_INSTANCE, work, 0);
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
			file = tim = DOO2_D_80071EE4;
			GsGetTimInfo(tim + 1, &timInfo);
			setRECT(&rect, timInfo.px, timInfo.py, timInfo.pw, timInfo.ph);
			LoadImage(&rect, timInfo.pixel);
			GetTPage(timInfo.pmode & 3, 0, timInfo.px, timInfo.py);
			if ((timInfo.pmode >> 3) & 1) {
				setRECT(&rect, timInfo.cx, timInfo.cy, timInfo.cw, timInfo.ch);
				LoadImage(&rect, timInfo.clut);
				MAIN_D_80135328 = GetClut(timInfo.cx, timInfo.cy);
			}
			DOO2_saveClutTile((u_long)DOO2_D_80071B5C, MAIN_D_80135328);
			readFile(DOOA_EGG_TMD_PATH, DOO2_D_80071EE4);
			GsMapModelingData((unsigned long *)DOO2_D_80071EE4 + 1);
			DOO2_saveModelClut((u_long)DOO2_D_80071BE0);
		}
		work = 0x2c4 - seq->frame;
		DOOA_updateCutsceneCamera(&entity->posData->location, entity->posData->rotation.vy, 0x149, 0x17b, work);
		break;
	case 0x66:
		if ((seq->frame >= 0x185) && (seq->frame < 0x1b8)) {
			ENTITY_TABLE[2]->isOnMap = 1;
			work = lerp(0xff, 0, 0x185, 0x1b7, seq->frame);
			DOO2_fadeClut((int16_t *)DOO2_D_80071BE0, entity, DOOA_FADED_CLUT, 0, 0xff, work);
		}
		if (seq->frame >= 0x1b7) {
			seq->phase = 4;
		}
		break;
	case 4:
		cameraTarget = DOOA_CAMERA_TARGET_RESET;
		DRAWING_OFFSET_X = MAIN_D_80135348;
		DRAWING_OFFSET_Y = MAIN_D_8013534C;
		CAMERA_TARGET = cameraTarget;
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
	DooaSequence *seq;
	Entity *entity;

	seq = &DOOA_REINCARNATION_SEQ;
	entity = seq->entity;
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

int32_t DOOA_hasIrisClosed(Entity *entity, int32_t startFrame, long endFrame, long frame)
{
	SVECTOR worldPos;
	int32_t size;
	int32_t projected;
	int32_t radius;
	DVECTOR screenPos;
	int32_t layer;
	int32_t depth;
	int32_t result;

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
	projected = (uint32_t)(size * VIEWPORT_DISTANCE) / depth;
	if (radius <= ((projected << 12) / 256)) {
		layer = 0xfa0;
		result = 1;
	} else {
		layer = 0x21;
		result = 0;
	}

	return result;
}

int32_t DOOA_initShardEffect(Entity *entity, intptr_t addr, int32_t size)
{
	DooaShardEffect *effect = &DOOA_SHARD_EFFECT;

	if ((addr & 3) != 0) {
		size -= 4 - (addr & 3L);
		addr += 4 - (addr & 3L);
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

void DOOA_saveEntityClut(int32_t buffer, Entity *entity)
{
	ModelComponent *model;
	TMDModel *tmd;
	RECT rect;

	model = getEntityModelComponent(entity->type, getEntityType(entity));
	tmd = model->modelPtr;
	setRECT(&rect, (model->clutPage & 0x3f) << 4, model->clutPage >> 6, 16, 24);
	StoreImage(&rect, (u_long *)buffer);
	DrawSync(0);
}

void DOOA_saveModelClut(int32_t buffer)
{
	RECT rect;

	setRECT(&rect, 0, 488, 16, 24);
	StoreImage(&rect, (u_long *)buffer);
	DrawSync(0);
}

void DOOA_saveShardClut(int32_t buffer)
{
	RECT rect;

	setRECT(&rect, 48, 488, 32, 24);
	StoreImage(&rect, (u_long *)buffer);
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
	int32_t offsetY;
	int32_t start;
	int32_t height;
	DooaSequence *seq;
	SVECTOR worldPos;
	DVECTOR screenPos;
	SVECTOR rotation;
	int32_t distance;
	int32_t mid;
	int32_t end;

	start = startFrame;
	mid = (startFrame + endFrame) / 2;
	end = endFrame;
	seq = &DOOA_REINCARNATION_SEQ;
	height = DIGIMON_DATA[seq->entity->type].height;

	if ((frame < startFrame) || (frame > endFrame)) {
		return;
	}

	if ((frame < start) || (frame > end)) {
		return;
	}

	copyVector(&worldPos, position);
	worldPosToScreenPos(&worldPos, &screenPos);

	rotation = MAIN_D_80134BB4;
	rotation.vy = angle + 0x638;

	distance = (height * 5) + 1200;
	DOOA_getOrbitPosition(&viewRef, &viewPos, position, &rotation, distance, height);

	if (frame <= mid) {
		GS_VIEWPOINT.vrx = lerp(DOOA_CAMERA_START_VIEW.vrx, viewRef.vx, start, mid, frame);
		GS_VIEWPOINT.vry = lerp(DOOA_CAMERA_START_VIEW.vry, viewRef.vy, start, mid, frame);
		GS_VIEWPOINT.vrz = lerp(DOOA_CAMERA_START_VIEW.vrz, viewRef.vz, start, mid, frame);
		GS_VIEWPOINT.rz = 0;
		DRAWING_OFFSET_X = lerp(MAIN_D_8013532C, 160, start, mid, frame);
		DRAWING_OFFSET_Y = lerp(MAIN_D_80135330, 120, start, mid, frame);
		GS_VIEWPOINT.vpx = DOOA_CAMERA_START_VIEW.vpx;
		GS_VIEWPOINT.vpy = DOOA_CAMERA_START_VIEW.vpy;
		GS_VIEWPOINT.vpz = DOOA_CAMERA_START_VIEW.vpz;
		VIEWPORT_DISTANCE = MAIN_D_80135334;
	} else if (frame <= end) {
		GS_VIEWPOINT.vpx = lerp(DOOA_CAMERA_START_VIEW.vpx, viewPos.vx, mid, end, frame);
		GS_VIEWPOINT.vpy = lerp(DOOA_CAMERA_START_VIEW.vpy, viewPos.vy, mid, end, frame);
		GS_VIEWPOINT.vpz = lerp(DOOA_CAMERA_START_VIEW.vpz, viewPos.vz, mid, end, frame);
		offsetY = lerp(0, 20, 0, 200, height);
		DRAWING_OFFSET_Y = lerp(120, offsetY + 120, mid, end, frame);
		VIEWPORT_DISTANCE = lerp(MAIN_D_80135334, 1000, mid, end, frame);
		GS_VIEWPOINT.vrx = viewRef.vx;
		GS_VIEWPOINT.vry = viewRef.vy;
		GS_VIEWPOINT.vrz = viewRef.vz;
		DRAWING_OFFSET_X = 160;
	}
}

void DOOA_spawnShardWave(long wireIndex)
{
	DooaShardEffect *effect;
	Entity *entity;
	int32_t boneIndex;

	effect = &DOOA_SHARD_EFFECT;
	entity = effect->entity;

	for (boneIndex = 2; boneIndex < DIGIMON_DATA[entity->type].boneCount; boneIndex++) {
		DOOA_spawnBoneShards(effect, boneIndex, wireIndex);
	}
}

void DOOA_toggleShardFlicker(void)
{
	DooaShardEffect *effect;

	effect = &DOOA_SHARD_EFFECT;
	effect->flash = (effect->flash + 1) & 1;
}

void DOOA_renderDigimonModel(Entity *entity, uint32_t otPoint)
{
	MATRIX lightMatrix;
	VECTOR location;
	SVECTOR rotation;
	int32_t type;
	int32_t i;
	int32_t boneCount;
	uint8_t animId;
	PositionData *posData;
	int32_t entityIndex;

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

	type = entity->type;
	animId = entity->anim.animId;
	posData = entity->posData;
	boneCount = DIGIMON_DATA[type].boneCount;
	location = posData->location;
	rotation = posData->rotation;
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

int32_t DOOA_renderIrisWindow(Entity *entity, int32_t startFrame, long endFrame, long frame)
{
	SVECTOR pos;
	int32_t size;
	int32_t projected;
	int32_t radius;
	DVECTOR screen;
	int32_t z;
	int32_t layer;
	int32_t visible;
	ParticleFlashData flash;
	POLY_FT4 *prim;
	int32_t left;
	int32_t top;
	int32_t bottom;
	int32_t sl;
	int32_t sr;
	int32_t st;
	int32_t sb;
	int32_t scale;
	int32_t thickness;
	int32_t lx;
	int32_t ly;
	int32_t lw;
	int32_t lh;
	int32_t rx;
	int32_t ry;
	int32_t rw;
	int32_t rh;
	int32_t tx;
	int32_t ty;
	int32_t tw;
	int32_t th;
	int32_t bx;
	int32_t by;
	int32_t bw;
	int32_t bh;
	int32_t right;

	if (endFrame < frame) {
		frame = endFrame;
	}

	pos.vx = entity->posData->location.vx;
	pos.vy = entity->posData->location.vy - (DIGIMON_DATA[entity->type].height / 2);
	pos.vz = entity->posData->location.vz;
	size = getDistance(DIGIMON_DATA[entity->type].radius * 2, DIGIMON_DATA[entity->type].height, DIGIMON_DATA[entity->type].radius * 2);
	radius = (lerp(200, 0, startFrame, endFrame, frame) << 12) / 128;
	z = worldPosToScreenPos(&pos, &screen);
	if (z <= 0) {
		return 1;
	}

	projected = (uint32_t)(size * VIEWPORT_DISTANCE) / (uint32_t)z;
	if (radius <= (projected << 12) / 256) {
		layer = 0xfa0;
		visible = 1;
	} else {
		layer = 0x21;
		visible = 0;
	}

	layer = 0x22;
	flash.screenPos.vx = screen.vx;
	flash.screenPos.vy = screen.vy;
	flash.sizeX = flash.sizeY = 0x40;
	flash.tpage = getTPage(1, 2, 832, 256);
	flash.uBase = 0;
	flash.vBase = 0x80;
	flash.clut = getClut(0, 487);
	flash.color.r = 0x80;
	flash.color.g = 0x80;
	flash.color.b = 0x80;
	flash.colorScale = 0x80;
	flash.scale = radius;
	flash.depth = layer;
	renderParticleFlash(&flash);
	prim = (POLY_FT4 *)GsGetWorkBase();
	scale = radius;
	right = (scale << 8) / 4096;
	left = screen.vx - right;
	top = screen.vy - right;
	bottom = screen.vy + right;
	right = right + ((DVECTOR *)&screen)->vx;
	sl = left - (0xa0 - DRAWING_OFFSET_X);
	sr = right - (0xa0 - DRAWING_OFFSET_X);
	st = top - (0x78 - DRAWING_OFFSET_Y);
	sb = bottom - (0x78 - DRAWING_OFFSET_Y);
	thickness = lerp(4, 1, 0x1860, 0, radius);

	lx = left - (sl + 0xa0);
	ly = top - (st + 0x78);
	lw = (sl + 0xa0) + thickness;
	if (lw > 0) {
		lh = 0xf0;
		SetPolyFT4(prim);
		SetSemiTrans(prim, 2);
		prim->r0 = prim->g0 = prim->b0 = 0x80;
		prim->tpage = getTPage(1, 2, 832, 256);
		prim->clut = getClut(0, 487);
		setUVWH(prim, 0, 0x80, 3, 3);
		setXYWH(prim, lx, ly, lw, lh);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	rx = right - thickness;
	ry = top - (st + 0x78L);
	rw = (0xa0 - sr) + thickness;
	if (rw > 0) {
		rh = 0xf0;
		SetPolyFT4(prim);
		prim->r0 = prim->g0 = prim->b0 = 0x80;
		SetSemiTrans(prim, 2);
		prim->tpage = getTPage(1, 2, 832, 256);
		prim->clut = getClut(0, 487);
		setUVWH(prim, 0, 0x80, 3, 3);
		setXYWH(prim, rx, ry, rw, rh);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	tx = left;
	ty = top - ((int32_t)st + 0x78);
	tw = right - left;
	if (tw > 0) {
		th = ((int32_t)st + 0x78) + thickness;
		if (th > 0) {
			SetPolyFT4(prim);
			prim->r0 = prim->g0 = prim->b0 = 0x80;
			SetSemiTrans(prim, 2);
			prim->tpage = getTPage(1, 2, 832, 256);
			prim->clut = getClut(0, 487);
			setUVWH(prim, 0, 0x80, 3, 3);
			setXYWH(prim, tx, ty, tw, th);
			AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
		}
	}

	bx = left;
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
			setXYWH(prim, bx, by, bw, bh);
			AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
		}
	}

	GsSetWorkBase((PACKET *)prim);

	return visible;
}

void DOOA_tickRebirth(int32_t instanceId)
{
	VECTOR flashPos;
	VECTOR colorStart;
	VECTOR colorEnd;
	int32_t shardSet;
	VECTOR offset;
	DooaSequence *seq;
	int32_t level;
	Entity *entity;

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
		if (seq->frame >= 130) {
			MAIN_D_80135324 = 0x10;
		} else {
			MAIN_D_80135324 = lerp(1, 0x10, 100, 132, seq->frame);
		}
		if (seq->frame < 132) {
			break;
		}
		seq->phase = 0xce;
		DOOA_removeShardEffect();
		loadVLALL(REINCARNATE_BABY_TYPE[seq->eggSlot], GENERAL_BUFFER_PTR);
		loadMMDAsync(REINCARNATE_BABY_TYPE[seq->eggSlot], 3, DOOA_MMD_BUFFER, (EvoModelData *)seq->modelData,
		             (uint8_t *)&seq->isModelLoading);
		DOO2_resetShardSets(DOOA_SHARD_BUFFER);
		playSound(8, 6);
		break;
	case 0xce:
		if (seq->frame < 147) {
			seq->fadeLevel = lerp(5, 0, 132, 150, seq->frame);
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
		EFECreateFlash();
		if (seq->frame >= 152) {
			while (seq->isModelLoading != 0) {
				tickFileReadQueue(0);
			}
			reincarnatePartner((int32_t)ENTITY_TABLE[1], &PARTNER_ENTITY.digimonEntity.stats, &PARTNER_PARA, REINCARNATE_BABY_TYPE[seq->eggSlot]);
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
		if (seq->frame >= 202) {
			seq->phase = 0xd0;
			seq->sparkleIndex = 0;
			ENTITY_TABLE[1]->isOnMap = 1;
			startAnimation((Entity *)&PARTNER_ENTITY, 0x1c);
			fadeoutCLUT1(0, DOOA_SCENE_CLUT, 0);
			DOOA_fadeModelClut(DOOA_MODEL_CLUT, entity, DOOA_FADED_CLUT, 0, 1, 0);
			DOOA_fadeShardClut(DOOA_SHARD_CLUT, entity, DOOA_FADED_CLUT, 0, 1, 0);
			break;
		}
		if (seq->frame < 152) {
			break;
		}
		level = lerp(255, 0, 152, 202, seq->frame);
		fadeoutCLUT1(level, DOOA_SCENE_CLUT, 0);
		if ((seq->frame & 1) == 0) {
			DOOA_fadeModelClut(DOOA_MODEL_CLUT, entity, DOOA_FADED_CLUT, 0, 255, level);
		} else {
			DOOA_fadeShardClut(DOOA_SHARD_CLUT, entity, DOOA_FADED_CLUT, 0, 255, level);
		}
		break;
	case 0xd0:
		if (seq->sparkleIndex < 47) {
			offset = DOOA_SPARKLE_OFFSET;
			if (DOOA_SPARKLE_BONE_IDS[seq->sparkleIndex] >= 0) {
				shardSet = DOO2_buildShardSet(&offset, (u_long)DOO2_D_80071EE4,
				                              DOOA_SPARKLE_BONE_IDS[seq->sparkleIndex] + (seq->eggSlot * 6));
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

	panel = &DOOA_REINCARNATION_SEQ;
	for (i = 0; i < 6; i++) {
		if (MAIN_D_80135364[i] == 0) {
			GsLinkObject4((u_long)&DOO2_D_80071EE4[3], &obj, i + (panel->eggSlot * 6));
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
	DooaShardEffect *effect;

	effect = &DOOA_SHARD_EFFECT;
	effect->state = state;
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
	RECT rect;
	int32_t i;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t stp;
	int16_t *src;
	int16_t *dst;

	src = srcClut;
	dst = dstClut;
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
	DrawSync(0);
}

void DOOA_fadeShardClut(int16_t *srcClut, void *unused, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame)
{
	RECT rect;
	int32_t i;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t stp;
	int16_t *src;
	int16_t *dst;

	src = srcClut;
	dst = dstClut;
	for (i = 0; i < 768; i++) {
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

	setRECT(&rect, 48, 488, 32, 24);
	LoadImage(&rect, (u_long *)dstClut);
	DrawSync(0);
}

void DOOA_getOrbitPosition(VECTOR *outRef, VECTOR *outPos, VECTOR *position, SVECTOR *rotation, int32_t distance, int32_t height)
{
	MATRIX matrix;
	VECTOR direction;

	outRef->vx = position->vx;
	outRef->vy = (int32_t)position->vy - (height / 2);
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
	Entity *entity;
	int32_t minRadius;
	int32_t maxRadius;
	DooaShard *shard;
	DooaShardEffect *effect;
	TMD_P_TG4 **cursor;

	effect = &DOOA_SHARD_EFFECT;
	cursor = effect->shardBuffer;
	entity = effect->entity;
	if (effect->flash != 0) {
		effect->colorR = (rand() % 100) + 60;
		effect->colorG = (rand() % 100) + 60;
		effect->colorB = (rand() % 100) + 60;
	}

	while (*cursor != NULL) {
		shard = (DooaShard *)((intptr_t)cursor);
		switch (effect->state) {
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
				shard->spin += shard->spinMax / 100;
			}
			if (shard->spin > shard->spinMax) {
				shard->spin = shard->spinMax;
			}
			shard->rotY += shard->spin;
			if (shard->radius < shard->targetRadius) {
				if ((shard->targetRadius / 80) == 0) {
					shard->radius++;
				} else {
					shard->radius += shard->targetRadius / 100;
				}
			}
			shard->fallSpeed += 1;
			shard->centerY -= shard->fallSpeed >> 3;
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
				shard->centerY = lerp(-shard->dropDepth, DOOA_getStoredDigimonY() - 100, shard->targetRadius, 0, shard->radius);
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
	intptr_t cursor;
	ModelComponent *model;
	TMD_P_TG4 *triTmd;
	SVECTOR triA;
	SVECTOR triB;
	SVECTOR triC;
	SVECTOR triOffset;
	DooaShard *tri;
	MATRIX triMatrix;
	TMD_P_TG4 *quadTmd;
	SVECTOR quadA;
	SVECTOR quadB;
	SVECTOR quadC;
	SVECTOR quadD;
	SVECTOR quadOffset;
	DooaShardQuad *quad;
	MATRIX quadMatrix;
	DooaShardEffect *effect;
	POLY_FT3 *triPrim;
	POLY_FT4 *quadPrim;

	effect = &DOOA_SHARD_EFFECT;
	cursor = (intptr_t)effect->shardBuffer;
	model = getEntityModelComponent(effect->entity->type, 3);

	while (*(int32_t *)cursor != 0) {
		switch ((*(TMD_P_TG4 **)cursor)->cd) {
		case GPU_COM_TG3:
		case GPU_COM_TG3 | 2:
			triTmd = *(TMD_P_TG4 **)cursor;
			tri = (DooaShard *)cursor;
			if (tri->delay >= 0) {
				triPrim = (POLY_FT3 *)GsGetWorkBase();
				SetPolyFT3(triPrim);
				if (effect->flash != 0) {
					setRGB0(triPrim, effect->colorR, effect->colorG, effect->colorB);
					triPrim->tpage = getTPage(1, 0, 832, 256);
					triPrim->clut = getClut(0, 487);
					setUV3(triPrim, 0, 128, 3, 128, 0, 131);
				} else {
					setRGB0(triPrim, effect->colorR, effect->colorB, effect->colorG);
					triPrim->tpage = model->pixelPage;
					triPrim->clut = triTmd->clut;
					setUV3(triPrim, triTmd->tu0, triTmd->tv0, triTmd->tu1, triTmd->tv1, triTmd->tu2, triTmd->tv2);
				}
				triOffset.vx = tri->centerX * (int32_t)tri->radius / 4096;
				triOffset.vy = tri->centerY;
				triOffset.vz = tri->centerZ * tri->radius / 4096;
				triA.vx = triOffset.vx + tri->vertex[0].vx;
				triA.vy = triOffset.vy + tri->vertex[0].vy;
				triA.vz = triOffset.vz + tri->vertex[0].vz;
				triB.vx = triOffset.vx + tri->vertex[1].vx;
				triB.vy = triOffset.vy + tri->vertex[1].vy;
				triB.vz = triOffset.vz + tri->vertex[1].vz;
				triC.vx = triOffset.vx + tri->vertex[2].vx;
				triC.vy = triOffset.vy + tri->vertex[2].vy;
				triC.vz = triOffset.vz + tri->vertex[2].vz;
				RotMatrixZYX((SVECTOR *)&tri->rotX, &triMatrix);
				ApplyMatrixSV(&triMatrix, &triA, &triA);
				ApplyMatrixSV(&triMatrix, &triB, &triB);
				ApplyMatrixSV(&triMatrix, &triC, &triC);
				triA.vx += effect->entity->posData->location.vx;
				triA.vy += DOOA_getStoredDigimonY();
				triA.vz += effect->entity->posData->location.vz;
				triB.vx += effect->entity->posData->location.vx;
				triB.vy += DOOA_getStoredDigimonY();
				triB.vz += effect->entity->posData->location.vz;
				triC.vx += effect->entity->posData->location.vx;
				triC.vy += DOOA_getStoredDigimonY();
				triC.vz += effect->entity->posData->location.vz;
				addScreenPolyFT3(triPrim, &triA, &triB, &triC);
			}
			cursor += sizeof(DooaShard);
			break;
		case GPU_COM_TG4:
		case GPU_COM_TG4 | 2:
			quadTmd = *(TMD_P_TG4 **)cursor;
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
					quadPrim->clut = quadTmd->clut;
					setUV4(quadPrim, quadTmd->tu0, quadTmd->tv0, quadTmd->tu1, quadTmd->tv1, quadTmd->tu2, quadTmd->tv2, quadTmd->tu3, quadTmd->tv3);
				}
				quadOffset.vx = quad->centerX * (int32_t)quad->radius / 4096;
				quadOffset.vy = quad->centerY;
				quadOffset.vz = quad->centerZ * quad->radius / 4096;
				quadA.vx = quadOffset.vx + quad->vertex[0].vx;
				quadA.vy = quadOffset.vy + quad->vertex[0].vy;
				quadA.vz = quadOffset.vz + quad->vertex[0].vz;
				quadB.vx = quadOffset.vx + quad->vertex[1].vx;
				quadB.vy = quadOffset.vy + quad->vertex[1].vy;
				quadB.vz = quadOffset.vz + quad->vertex[1].vz;
				quadC.vx = quadOffset.vx + quad->vertex[2].vx;
				quadC.vy = quadOffset.vy + quad->vertex[2].vy;
				quadC.vz = quadOffset.vz + quad->vertex[2].vz;
				quadD.vx = quadOffset.vx + quad->vertex[3].vx;
				quadD.vy = quadOffset.vy + quad->vertex[3].vy;
				quadD.vz = quadOffset.vz + quad->vertex[3].vz;
				RotMatrixZYX((SVECTOR *)&quad->rotX, &quadMatrix);
				ApplyMatrixSV(&quadMatrix, &quadA, &quadA);
				ApplyMatrixSV(&quadMatrix, &quadB, &quadB);
				ApplyMatrixSV(&quadMatrix, &quadC, &quadC);
				ApplyMatrixSV(&quadMatrix, &quadD, &quadD);
				quadA.vx += effect->entity->posData->location.vx;
				quadA.vy += DOOA_getStoredDigimonY();
				quadA.vz += effect->entity->posData->location.vz;
				quadB.vx += effect->entity->posData->location.vx;
				quadB.vy += DOOA_getStoredDigimonY();
				quadB.vz += effect->entity->posData->location.vz;
				quadC.vx += effect->entity->posData->location.vx;
				quadC.vy += DOOA_getStoredDigimonY();
				quadC.vz += effect->entity->posData->location.vz;
				quadD.vx += effect->entity->posData->location.vx;
				quadD.vy += DOOA_getStoredDigimonY();
				quadD.vz += effect->entity->posData->location.vz;
				addScreenPolyFT4(quadPrim, &quadA, &quadB, &quadC, &quadD);
			}
			cursor += sizeof(DooaShardQuad);
			break;
		}
	}
}

void DOOA_spawnBoneShards(DooaShardEffect *effect, int32_t boneIndex, long wireIndex)
{
	intptr_t frags;
	Entity *entity;
	ModelComponent *model;
	TMDModel *tmd;
	TMDModel *header;
	struct TMD_STRUCT *objects;
	struct TMD_STRUCT *obj;
	int32_t objIndex;
	MATRIX boneMatrix;
	int32_t i;
	SVECTOR rotated;
	intptr_t vertOut;
	SVECTOR *src;
	intptr_t prim;
	int32_t j;
	intptr_t tri;
	intptr_t quad;
	DooaShardVertex *verts;
	DooaShardQuad *quadShard;
	DooaShard *triShard;

	frags = (intptr_t)effect->shardWrite;
	entity = effect->entity;
	model = getEntityModelComponent(entity->type, 3);
	tmd = model->modelPtr;
	header = tmd;
	objects = (struct TMD_STRUCT *)((uint32_t)header + 12);
	objIndex = DIGIMON_SKELETONS[entity->type][boneIndex].objIndex;
	if (objIndex == -1) {
		return;
	}

	obj = &objects[objIndex];
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
				tri = prim;
				triShard = (DooaShard *)frags;
				triShard->prim = (TMD_P_TG4 *)prim;
				triShard->centerX = (verts[((TMD_P_TG3 *)tri)->v0].vx + verts[((TMD_P_TG3 *)tri)->v1].vx + verts[((TMD_P_TG3 *)tri)->v2].vx) / 3;
				triShard->centerY = (verts[((TMD_P_TG3 *)tri)->v0].vy + verts[((TMD_P_TG3 *)tri)->v1].vy + verts[((TMD_P_TG3 *)tri)->v2].vy) / 3;
				triShard->centerZ = (verts[((TMD_P_TG3 *)tri)->v0].vz + verts[((TMD_P_TG3 *)tri)->v1].vz + verts[((TMD_P_TG3 *)tri)->v2].vz) / 3;
				triShard->vertex[0].vx = verts[((TMD_P_TG3 *)tri)->v0].vx - triShard->centerX;
				triShard->vertex[0].vy = verts[((TMD_P_TG3 *)tri)->v0].vy - triShard->centerY;
				triShard->vertex[0].vz = verts[((TMD_P_TG3 *)tri)->v0].vz - triShard->centerZ;
				triShard->vertex[1].vx = verts[((TMD_P_TG3 *)tri)->v1].vx - triShard->centerX;
				triShard->vertex[1].vy = verts[((TMD_P_TG3 *)tri)->v1].vy - triShard->centerY;
				triShard->vertex[1].vz = verts[((TMD_P_TG3 *)tri)->v1].vz - triShard->centerZ;
				triShard->vertex[2].vx = verts[((TMD_P_TG3 *)tri)->v2].vx - triShard->centerX;
				triShard->vertex[2].vy = verts[((TMD_P_TG3 *)tri)->v2].vy - triShard->centerY;
				triShard->vertex[2].vz = verts[((TMD_P_TG3 *)tri)->v2].vz - triShard->centerZ;
				triShard->centerX -= effect->entity->posData->location.vx;
				triShard->centerY -= DOOA_getStoredDigimonY();
				triShard->centerZ -= effect->entity->posData->location.vz;
				triShard->fallSpeed = 0;
				triShard->radius = 0x1000;
				triShard->rotX = 0;
				triShard->rotY = 0;
				triShard->rotZ = 0;
				triShard->axisDistance = getDistance(triShard->centerX, 0, triShard->centerZ);
				if (triShard->axisDistance == 0) {
					triShard->axisDistance = 1;
				}
				triShard->delay = 0;
				frags += sizeof(DooaShard);
				break;
			case GPU_COM_TG4:
			case GPU_COM_TG4 | 2:
				quad = prim;
				quadShard = (DooaShardQuad *)frags;
				quadShard->prim = (TMD_P_TG4 *)prim;
				quadShard->centerX = (verts[((TMD_P_TG4 *)quad)->v0].vx + verts[((TMD_P_TG4 *)quad)->v1].vx + verts[((TMD_P_TG4 *)quad)->v2].vx + verts[((TMD_P_TG4 *)quad)->v3].vx) / 4;
				quadShard->centerY = (verts[((TMD_P_TG4 *)quad)->v0].vy + verts[((TMD_P_TG4 *)quad)->v1].vy + verts[((TMD_P_TG4 *)quad)->v2].vy + verts[((TMD_P_TG4 *)quad)->v3].vy) / 4;
				quadShard->centerZ = (verts[((TMD_P_TG4 *)quad)->v0].vz + verts[((TMD_P_TG4 *)quad)->v1].vz + verts[((TMD_P_TG4 *)quad)->v2].vz + verts[((TMD_P_TG4 *)quad)->v3].vz) / 4;
				quadShard->vertex[0].vx = verts[((TMD_P_TG4 *)quad)->v0].vx - quadShard->centerX;
				quadShard->vertex[0].vy = verts[((TMD_P_TG4 *)quad)->v0].vy - quadShard->centerY;
				quadShard->vertex[0].vz = verts[((TMD_P_TG4 *)quad)->v0].vz - quadShard->centerZ;
				quadShard->vertex[1].vx = verts[((TMD_P_TG4 *)quad)->v1].vx - quadShard->centerX;
				quadShard->vertex[1].vy = verts[((TMD_P_TG4 *)quad)->v1].vy - quadShard->centerY;
				quadShard->vertex[1].vz = verts[((TMD_P_TG4 *)quad)->v1].vz - quadShard->centerZ;
				quadShard->vertex[2].vx = verts[((TMD_P_TG4 *)quad)->v2].vx - quadShard->centerX;
				quadShard->vertex[2].vy = verts[((TMD_P_TG4 *)quad)->v2].vy - quadShard->centerY;
				quadShard->vertex[2].vz = verts[((TMD_P_TG4 *)quad)->v2].vz - quadShard->centerZ;
				quadShard->vertex[3].vx = verts[((TMD_P_TG4 *)quad)->v3].vx - quadShard->centerX;
				quadShard->vertex[3].vy = verts[((TMD_P_TG4 *)quad)->v3].vy - quadShard->centerY;
				quadShard->vertex[3].vz = verts[((TMD_P_TG4 *)quad)->v3].vz - quadShard->centerZ;
				quadShard->centerX -= effect->entity->posData->location.vx;
				quadShard->centerY -= DOOA_getStoredDigimonY();
				quadShard->centerZ -= effect->entity->posData->location.vz;
				quadShard->fallSpeed = 0;
				quadShard->radius = 0x1000;
				quadShard->rotX = 0;
				quadShard->rotY = 0;
				quadShard->rotZ = 0;
				quadShard->axisDistance = getDistance(quadShard->centerX, 0, quadShard->centerZ);
				if (quadShard->axisDistance == 0) {
					quadShard->axisDistance = 1;
				}
				quadShard->delay = 0;
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

// clang-format off
int32_t DOOA_tick(partner, buffer, isInitialized)
	PartnerEntity *partner;
	void *buffer;
	int16_t isInitialized;
// clang-format on
{
	int32_t instance;
	DooaSequence *panel;
	int32_t messageId;

	panel = &DOOA_REINCARNATION_SEQ;
	instance = 0;
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
	addObject(0x80b, instance, DOOA_tickDissolve, DOOA_renderDissolve);
	DOOA_CAMERA_START_VIEW = GS_VIEWPOINT;
	MAIN_D_8013532C = DRAWING_OFFSET_X;
	MAIN_D_80135330 = DRAWING_OFFSET_Y;
	MAIN_D_80135334 = VIEWPORT_DISTANCE;
	DOOA_SAVED_LOCATION = ENTITY_TABLE[1]->posData->location;
	MAIN_D_80135338 = ENTITY_TABLE[1]->posData->rotation;
	if (partner->lives != 0) {
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 1);
	}
	DOOA_initOrderingTable();
	if ((isTriggerSet(0xdc) == 1) || (isTriggerSet(0xd6) == 1) || (readPStat(1) >= 50U)) {
		messageId = 0xcd;
	} else {
		messageId = 0xda;
	}
	DEATH_MAP_TARGET = messageId;
	setDeathMap(messageId, 1);
	return (intptr_t)buffer;
}

// clang-format off
int32_t DOOA_getSequenceState(unused, isInitialized)
	int32_t unused;
	int16_t isInitialized;
// clang-format on
{
	DooaSequence *sequence;
	int32_t instance;
	int32_t i;
	PartnerEntity *partner;

	sequence = &DOOA_REINCARNATION_SEQ;
	instance = 0;
	partner = (PartnerEntity *)sequence->entity;

	if (isInitialized != 0) {
		return sequence->frame;
	}

	if (partner->lives == 0) {
		sequence->eggSlot = rand() % 4;
	}

	addObject(0x80c, instance, DOOA_tickRebirth, DOOA_renderRebirth);
	sequence->phase = 200;
	sequence->fadeLevel = 0;
	sequence->frame = 0;
	MAIN_D_80135324 = 0;

	for (i = 0; i < 6; i++) {
		MAIN_D_80135364[i] = 0;
	}

	return instance;
}
