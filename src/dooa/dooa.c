#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <stdlib.h>

#include <dw/btl.h>
#include <dw/doo.h>
#include <dw/entity.h>
#include <dw/evolution.h>
#include <dw/file.h>
#include <dw/file_queue.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/sound_async.h>
#include <dw/types.h>
#include <dw/utils.h>
#include <dw/world_object.h>

typedef struct {
	int16_t state;
	int16_t prevState;
	Entity *entity;
	int32_t unk_08;
	int32_t shardBuffer;
	int32_t shardWrite;
	int32_t shardBytes;
	uint8_t colorR;
	uint8_t colorG;
	uint8_t colorB;
	int8_t flash;
} DooaShardEffect;

typedef struct {
	uint32_t useCount;
	void *modelPtr;
	int32_t *animTablePtr;
	void *mmdPtr;
	uint16_t pixelPage;
	uint16_t clutPage;
} DooaModelComponent;

typedef struct {
	VECTOR offset;
	int8_t unk_10[3];
	int8_t boneId;
} DooaSparkle;

typedef struct {
	int32_t id;
	uint32_t flags;
	int32_t nobj;
	struct TMD_STRUCT obj[0];
} DooaTMDModel;

typedef struct {
	int8_t objIndex;
	int8_t parentIndex;
} SkeletonBone;

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
} RebirthScaleCurve;

extern int32_t VIEWPORT_DISTANCE;
extern int8_t MAIN_D_80134BBC;
extern int8_t DOOA_SAVED_ENTITY_VISIBILITY[];
extern Entity *DOOA_D_8008786C[];
extern GsRVIEW2 DOOA_D_80084BA4;
extern VECTOR DOOA_D_80084BB0;
extern SVECTOR MAIN_D_80134BB4;
extern int32_t MAIN_D_8013532C;
extern int32_t MAIN_D_80135330;
extern int32_t MAIN_D_80135334;
extern GsRVIEW2 GS_VIEWPOINT;
extern int8_t DOOA_SHARD_FLICKER[];
extern uint32_t DOO2_D_80071EF0[];
extern int8_t MAIN_D_80135364[8];
extern int16_t MAIN_D_80135324;
extern int16_t DOOA_SHARD_EFFECT[];
extern DooaShard *DOOA_SHARD_LIST[];
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT DOOA_REINCARNATION_OT[];
extern VECTOR DOOA_D_80084970;
extern int8_t DOOA_SPARKLE_BONE_IDS[];
extern VECTOR DOOA_D_800849B0;
extern int16_t DOOA_D_80084ED8[];
extern int16_t DOOA_D_800851DC[];
extern int16_t DOOA_D_80085B10[];
extern int16_t DOOA_D_80086914[];
extern void *DOO2_D_80071EE4[];
extern int16_t EGG_DIGIMON_TYPES[4];
extern int32_t *EFE_DATA_STACK;
extern SkeletonBone *DIGIMON_SKELETONS[];
extern int8_t WIREFRAME_RNG_TABLE[];
extern char DOOA_D_800849C0[];
extern VECTOR DOOA_D_80084BC4;
extern SVECTOR MAIN_D_80135338;
extern int32_t MAIN_D_80135340;
extern int32_t *DOOA_SHARD_ENTITY[];
extern ShardWaveSchedule DOOA_SHARD_WAVE_SCHEDULE;
extern RebirthScaleCurve DOOA_REBIRTH_SCALE_CURVE;
extern VECTOR DOOA_D_80084938;
extern char DOOA_D_80084948[];
extern char DOOA_D_8008495C[];
extern uint8_t DOOA_D_80084BD4[];
extern GsRVIEW2 DOOA_D_80085AE0;
extern VECTOR DOOA_D_80085B00;
extern u_long DOO2_D_80071B5C[];
extern u_long DOO2_D_80071BE0[];
extern u_long DOO2_D_80071EE8[];
extern VECTOR CAMERA_TARGET;
extern int8_t CAMERA_REACHED_TARGET;
extern int32_t PARTNER_WIREFRAME_TOTAL;
extern int16_t WIREFRAME_COLOR_MIN;
extern int16_t WIREFRAME_COLOR_MAX;
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
int32_t DOOA_hasIrisClosed(Entity *entity, int32_t startRadius, int32_t endRadius, int32_t frame);
void DOOA_uploadShardClut(u_long *pixels);
void DOOA_setOtherEntitiesHidden(int32_t restore);
void DOOA_hideAllButPartner(void);
void DOOA_getOrbitPosition(VECTOR *outRef, VECTOR *outPos, VECTOR *position, SVECTOR *rotation,
                           int32_t distance, int32_t height);
void DOOA_updateCutsceneCamera(VECTOR *position, int32_t angle, int32_t startFrame, int32_t endFrame,
                               int32_t frame);
void DOOA_toggleShardFlicker(void);
void DOO2_renderWireframeModel(GsDOBJ2 *obj, int32_t wireThreshold);
void DOO2_renderSparkStreak(int32_t *pos, SVECTOR *rot);
int32_t customRandom(int32_t low, int32_t high);
int32_t rand(void);
void DOOA_renderRebirth(int32_t instanceId);
void DOOA_setShardState(int16_t value);
int32_t removeObject();
void DOOA_removeShardEffect(void);
void DOOA_showPlayerAndPartner(void);
void DOOA_fadeModelClut(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame);
void DOOA_fadeShardClut(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame);
void DOOA_getOrbitPosition(VECTOR *offset, VECTOR *pos, VECTOR *src, SVECTOR *rotation, int32_t distance, int32_t height);
int32_t lerp(int32_t a, int32_t b, int32_t c, int32_t d, int32_t t);
int32_t MAIN_func_800DA9F4(void);
int32_t DOOA_updateShards(int32_t instanceId);
int32_t DOOA_renderShards(int32_t instance);
int32_t DOOA_initShardEffect(Entity *entity, int32_t addr, int32_t size);
int32_t getEntityType(Entity *entity);
DooaModelComponent *getEntityModelComponent(int32_t instance, int32_t type);
void DOOA_uploadModelTexture(u_long *pixels, Entity *entity);
void DOOA_uploadModelClut(u_long *pixels);
void renderDropShadow(Entity *entity);
void createFlash(void);
void setMapLayerEnabled(int32_t enabled);
void setEntityPosition(int32_t entityId, int32_t x, int32_t y, int32_t z);
void loadMMDAsync(int32_t digimonType, int32_t entityType, uint8_t *buffer,
                  void *modelData, int8_t *readComplete);
void MAIN_func_800D9BA8(int32_t level, int16_t *src, int32_t arg2);
int32_t DOO2_buildShardSet(VECTOR *offset, void *modelList, int32_t modelIndex);
void DOO2_resetShardSets(int32_t size);
void DOO2_releaseAllShardSets(void);
void DOOA_tickRebirth(int32_t instanceId);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
void DOOA_spawnBoneShards(DooaShardEffect *effect, int32_t boneIndex, int32_t wireIndex);
char *initializeFlashData(char *base);
void startAnimation(Entity *entity, int32_t animId);
int32_t isTriggerSet(uint16_t trigger);
uint8_t readPStat(int32_t index);
void MAIN_func_800D91EC(int32_t messageId, int32_t flag);
void DOOA_tickDissolve(int32_t instanceId);
void DOOA_initOrderingTables(void);
void DOOA_spawnShardWave(int32_t arg0);
void MAIN_func_80092B60(POLY_FT4 *prim);
void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
int32_t add3DSpritePrim(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
void setEntityPosition(int32_t id, int32_t x, int32_t y, int32_t z);
void setupEntityMatrix(int32_t id);
void tickCameraMovement(int32_t mode);
void setEFEFlashOffset(int32_t instance, int32_t x, int32_t y);
void MAIN_func_800D91FC(int32_t mode);
void MAIN_func_800D9248(void);
void MAIN_func_800D9B60(int16_t *clut);
void MAIN_func_800D9BA8(int32_t alpha, int16_t *clut, int32_t mode);
void MAIN_func_800DA9C8(void);
void MAIN_func_800DF5A0(void);
int32_t DOOA_hasIrisClosed(Entity *entity, int32_t a, int32_t b, int32_t frame);
void DOOA_setOtherEntitiesHidden(int32_t mode);
void DOOA_updateCutsceneCamera(VECTOR *pos, int32_t rotY, int32_t t0, int32_t t1, int32_t t);
void DOO2_uploadModelClut(u_long *pixels);
void DOO2_uploadClutTile(u_long *pixels, int32_t tile);
void DOO2_fadeClut(int16_t *srcClut, void *entity, int16_t *dstClut,
                   int32_t startFrame, int32_t endFrame, int32_t frame);
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
	DOOA_setOtherEntitiesHidden,
	DOOA_uploadShardClut,
	DOOA_uploadModelClut,
	DOOA_uploadModelTexture,
	DOOA_initShardEffect,
	DOOA_hasIrisClosed,
	DOOA_initOrderingTables,
	DOOA_renderDissolve,
	DOOA_tickDissolve,
};

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
	RebirthScaleCurve heights;
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
	DooFlash *flash;
	DooFlash *target;
	Entity *entity;
	DooSequence *seq;

	seq = DOOA_REINCARNATION_SEQ;
	entity = seq->entity;
	seq->frame++;

	if ((entity->anim.animId == 0xc) &&
	    (entity->anim.animFrame == entity->anim.frameCount)) {
		startAnimation((Entity *)&PARTNER_ENTITY, 1);
		MAIN_func_800DA9C8();
		DOOA_D_80084BC4 = entity->posData->location;
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
		DOOA_initShardEffect(entity, 0x80044800, 0x5dc0);
		DOOA_uploadModelTexture((u_long *)DOOA_D_80084BD4, entity);
		DOOA_uploadModelClut((u_long *)DOOA_D_80084ED8);
		DOOA_uploadShardClut((u_long *)DOOA_D_800851DC);
		MAIN_D_80135348 = DRAWING_OFFSET_X;
		MAIN_D_8013534C = DRAWING_OFFSET_Y;
		DOOA_D_80085AE0 = GS_VIEWPOINT;
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
		DOOA_D_80085B00.vx = 0;
		DOOA_D_80085B00.vy = 0;
		DOOA_D_80085B00.vz = 0;
		entity->posData->location = DOOA_D_80084BC4;
		entity->posData->rotation = MAIN_D_80135338;
		if (((PartnerEntity *)entity)->lives != 0) {
			startAnimation((Entity *)&PARTNER_ENTITY, 1);
		} else {
			startAnimation((Entity *)&PARTNER_ENTITY, 0x2c);
			ENTITY_TABLE[1]->anim.animFlag &= 0xfe;
		}
		DOOA_setOtherEntitiesHidden(0);
		seq->phase = 1;
		setMapLayerEnabled(0);
		DOOA_hideAllButPartner();
		MAIN_func_800D9B60(DOOA_D_80085B10);
		MAIN_func_800D9BA8(0xff, DOOA_D_80085B10, 0);
		break;
	case 1:
		entity->isOnMap = 2;
		work = seq->frame;
		if (work >= 0x56) {
			seq->phase = 2;
			seq->phaseInitPending = 1;
			DOOA_D_80084BA4 = DOOA_D_80085AE0;
			MAIN_D_8013532C = MAIN_D_80135350;
			MAIN_D_80135330 = MAIN_D_80135354;
			MAIN_D_80135334 = MAIN_D_80135358;
			DOOA_D_80084BC4 = entity->posData->location;
			MAIN_D_80135338 = entity->posData->rotation;
			entity->posData->location = DOOA_D_80085B00;
			entityLookAtLocation(entity, &ENTITY_TABLE[0]->posData->location);
			MAIN_D_8013535C = entity->posData->rotation;
			setEntityPosition(1, DOOA_D_80085B00.vx, DOOA_D_80085B00.vy,
			                  DOOA_D_80085B00.vz);
			setupEntityMatrix(1);
			DOOA_updateCutsceneCamera(&entity->posData->location,
			                          entity->posData->rotation.vy, 0, 0x64, 0x63);
			GsSetRefView2(&GS_VIEWPOINT);
			GsSetProjection(VIEWPORT_DISTANCE);
		} else {
			DOOA_updateCutsceneCamera(&entity->posData->location,
			                          entity->posData->rotation.vy, 0x23, 0x55,
			                          work);
			GsSetRefView2(&GS_VIEWPOINT);
			GsSetProjection(VIEWPORT_DISTANCE);
		}
		break;
	case 2:
		ENTITY_TABLE[1]->isOnMap = 2;
		if (seq->phaseInitPending != 0) {
			setEntityPosition(1, DOOA_D_80085B00.vx, DOOA_D_80085B00.vy,
			                  DOOA_D_80085B00.vz);
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
			work = work - 0x69;
			PARTNER_WIREFRAME_TOTAL = (wireCounts.v + work)[0];
			wireCount = (wireCounts.v + work)[0];
			if ((work == 0) || ((wireCounts.v + work)[-1] != wireCount)) {
				DOOA_spawnShardWave(wireCount);
			}
		}
		if (seq->frame >= 0xbf) {
			seq->phase = 0x64;
			DOOA_D_80084BC4.vy = entity->posData->location.vy;
			target = &seq->flash;
			target->targetY = MAIN_func_800DA9F4() -
			                  (DIGIMON_DATA[entity->type].height + 100);
			target->pos.vx = entity->posData->location.vx;
			target->pos.vz = entity->posData->location.vz;
			stopSound();
			playSound(8, 1);
		}
		break;
	case 0x64:
		heights = DOOA_REBIRTH_SCALE_CURVE;
		work = lerp(0, 0x1f, 0xbf, 0xd0, seq->frame);
		work = height = heights.v[work];
		work = ((((work - 100) * 200) / 100) + 100);
		entity->posData->scale.vy = (work << 12) / 100;
		entity->posData->scale.vx = ((10000 / work) << 12) / 100;
		entity->posData->scale.vx = lerp(entity->posData->scale.vx,
		                                 entity->posData->scale.vx * 50 / 100,
		                                 0xbf, 0xd0, seq->frame);
		entity->posData->scale.vz = entity->posData->scale.vx;
		if (height >= 0x12d) {
			work = height;
			work = 0x12c - (work - 0x12c);
			work = ((((work - 100) * 200) / 100) + 100);
			entity->posData->scale.vy = (work << 12) / 100;
			ENTITY_TABLE[1]->anim.animFlag &= 0xfe;
			entity->posData->location.vy =
				lerp(entity->posData->location.vy,
			             seq->flash.targetY,
			             seq->frame - 1, seq->frame, seq->frame);
			setEntityPosition(1, entity->posData->location.vx,
			                  entity->posData->location.vy,
			                  entity->posData->location.vz);
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
			entity->posData->location.vy = DOOA_D_80084BC4.vy;
			setEntityPosition(1, entity->posData->location.vx,
			                  entity->posData->location.vy,
			                  entity->posData->location.vz);
			setupEntityMatrix(1);
			playSound(8, 2);
		}
		break;
	case 0x65:
		flash = &seq->flash;
		flash->pos.vy = lerp(flash->targetY, 0, 0xd1, 0x135, seq->frame);
		*EFE_DATA_STACK++ = 0;
		*EFE_DATA_STACK++ = (int32_t)&flash->pos;
		*EFE_DATA_STACK++ = -1;
		*EFE_DATA_STACK++ = 8;
		*EFE_DATA_STACK++ = 0;
		*EFE_DATA_STACK++ = 0x14;
		startColor.vx = (rand() % 100) + 60;
		startColor.vy = (rand() % 100) + 60;
		startColor.vz = (rand() % 100) + 60;
		endColor.vx = endColor.vy = endColor.vz = 0x14;
		*EFE_DATA_STACK++ = (int32_t)&startColor;
		*EFE_DATA_STACK++ = (int32_t)&endColor;
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
			readFile(DOOA_D_80084948, DOO2_D_80071EE4);
			GsGetTimInfo(DOO2_D_80071EE8, &timInfo);
			rect.x = timInfo.px;
			rect.y = timInfo.py;
			rect.w = timInfo.pw;
			rect.h = timInfo.ph;
			LoadImage(&rect, timInfo.pixel);
			GetTPage(timInfo.pmode & 3, 0, timInfo.px, timInfo.py);
			if ((timInfo.pmode >> 3) & 1) {
				rect.x = timInfo.cx;
				rect.y = timInfo.cy;
				rect.w = timInfo.cw;
				rect.h = timInfo.ch;
				LoadImage(&rect, timInfo.clut);
				MAIN_D_80135328 = GetClut(timInfo.cx, timInfo.cy);
			}
			DOO2_uploadClutTile(DOO2_D_80071B5C, MAIN_D_80135328);
			readFile(DOOA_D_8008495C, DOO2_D_80071EE4);
			GsMapModelingData(DOO2_D_80071EE8);
			DOO2_uploadModelClut(DOO2_D_80071BE0);
		}
		work = 0x2c4;
		work = work - seq->frame;
		DOOA_updateCutsceneCamera(&entity->posData->location,
		                          entity->posData->rotation.vy, 0x149, 0x17b,
		                          work);
		break;
	case 0x66:
		if ((seq->frame >= 0x185) && (seq->frame < 0x1b8)) {
			ENTITY_TABLE[2]->isOnMap = 1;
			DOO2_fadeClut((int16_t *)DOO2_D_80071BE0, entity,
			              DOOA_D_80086914, 0, 0xff,
			              lerp(0xff, 0, 0x185, 0x1b7, seq->frame));
		}
		if (seq->frame >= 0x1b7) {
			seq->phase = 4;
		}
		break;
	case 4:
		DRAWING_OFFSET_X = MAIN_D_80135348;
		DRAWING_OFFSET_Y = MAIN_D_8013534C;
		CAMERA_TARGET = DOOA_D_80084938;
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
	DooSequence *seq = DOOA_REINCARNATION_SEQ;

	if (seq->frame < 36) {
		if (ENTITY_TABLE[1]->isOnMap == 0) {
			DOOA_renderDigimonModel(seq->entity, 0x21);
		}
		DOOA_renderIrisWindow(seq->entity, 0, 0x21, seq->frame);
	}
}

void DOOA_initOrderingTables(void)
{
	DOOA_REINCARNATION_OT[0].length = 11;
	DOOA_REINCARNATION_OT[0].org = (GsOT_TAG *)0x8008c000;
	DOOA_REINCARNATION_OT[1].length = 11;
	DOOA_REINCARNATION_OT[1].org = (GsOT_TAG *)0x8008e000;
}

int32_t DOOA_hasIrisClosed(Entity *entity, int32_t startRadius, int32_t endRadius, int32_t frame)
{
	SVECTOR worldPos;
	DVECTOR screenPos;
	int32_t size;
	int32_t radius;
	int32_t depth;

	if (endRadius < frame) {
		frame = endRadius;
	}
	worldPos.vx = entity->posData->location.vx;
	worldPos.vy = entity->posData->location.vy - (DIGIMON_DATA[entity->type].height / 2);
	worldPos.vz = entity->posData->location.vz;
	size = getDistance(DIGIMON_DATA[entity->type].radius * 2,
	                   DIGIMON_DATA[entity->type].height,
	                   DIGIMON_DATA[entity->type].radius * 2);
	radius = (lerp(200, 0, startRadius, endRadius, frame) << 12) / 128;
	depth = worldPosToScreenPos(&worldPos, &screenPos);
	if (depth <= 0) {
		return 1;
	}
	if (((int32_t)(((uint32_t)(size * VIEWPORT_DISTANCE) / (uint32_t)depth) << 12) / 256) >= radius) {
		return 1;
	}

	return 0;
}

int32_t DOOA_initShardEffect(Entity *entity, int32_t addr, int32_t size)
{
	DooaShardEffect *effect = (DooaShardEffect *)DOOA_SHARD_EFFECT;
	int32_t rem = addr & 3;

	if (rem != 0) {
		size -= 4 - rem;
		addr += 4 - rem;
	}

	effect->state = 0;
	effect->prevState = effect->state;
	effect->entity = entity;
	effect->shardBuffer = addr;
	effect->shardWrite = effect->shardBuffer;
	*(int32_t *)effect->shardWrite = 0;
	effect->shardBytes = size;
	effect->colorR = 0x50;
	effect->colorG = 0x50;
	effect->colorB = 0x50;
	effect->flash = 0;
	addObject(0x608, 0, (TickFunction)DOOA_updateShards,
	          (RenderFunction)DOOA_renderShards);
	return addr + size;
}

void DOOA_uploadModelTexture(u_long *pixels, Entity *entity)
{
	DooaModelComponent *model;
	RECT rect;

	model = getEntityModelComponent(entity->type, getEntityType(entity));
	rect.x = (model->clutPage & 0x3f) << 4;
	rect.y = model->clutPage >> 6;
	rect.w = 16;
	rect.h = 24;
	StoreImage(&rect, pixels);
	DrawSync(0);
}

void DOOA_uploadModelClut(u_long *pixels)
{
	RECT rect;

	setRECT(&rect, 0, 488, 16, 24);
	StoreImage(&rect, pixels);
	DrawSync(0);
}

void DOOA_uploadShardClut(u_long *pixels)
{
	RECT rect;

	setRECT(&rect, 48, 488, 32, 24);
	StoreImage(&rect, pixels);
	DrawSync(0);
}

void DOOA_setOtherEntitiesHidden(int32_t restore)
{
	int32_t i;

	MAIN_D_80134BBC = restore;

	if (restore == 0) {
		for (i = 0; i < ENTITY_MAX; i++) {
			if ((ENTITY_TABLE[i] != (Entity *)&PARTNER_ENTITY) &&
			    (ENTITY_TABLE[i]->isOnMap != 0)) {
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

void DOOA_updateCutsceneCamera(VECTOR *position, int32_t angle, int32_t startFrame, int32_t endFrame,
                               int32_t frame)
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
	height = DIGIMON_DATA[DOOA_D_8008786C[0]->type].height;

	if ((frame < startFrame) || (frame > endFrame)) {
		return;
	}

	if ((frame < startFrame) || (frame > endFrame)) {
		return;
	}

	worldPos.vx = position->vx;
	worldPos.vy = position->vy;
	worldPos.vz = position->vz;
	worldPosToScreenPos(&worldPos, &screenPos);

	rotation = MAIN_D_80134BB4;
	rotation.vy = angle + 0x638;

	DOOA_getOrbitPosition(&viewRef, &viewPos, position, &rotation, (height * 5) + 1200, height);

	if (frame <= midFrame) {
		GS_VIEWPOINT.vrx = lerp(DOOA_D_80084BB0.vx, viewRef.vx, startFrame, midFrame, frame);
		GS_VIEWPOINT.vry = lerp(DOOA_D_80084BB0.vy, viewRef.vy, startFrame, midFrame, frame);
		GS_VIEWPOINT.vrz = lerp(DOOA_D_80084BB0.vz, viewRef.vz, startFrame, midFrame, frame);
		GS_VIEWPOINT.rz = 0;
		DRAWING_OFFSET_X = lerp(MAIN_D_8013532C, 160, startFrame, midFrame, frame);
		DRAWING_OFFSET_Y = lerp(MAIN_D_80135330, 120, startFrame, midFrame, frame);
		GS_VIEWPOINT.vpx = DOOA_D_80084BA4.vpx;
		GS_VIEWPOINT.vpy = DOOA_D_80084BA4.vpy;
		GS_VIEWPOINT.vpz = DOOA_D_80084BA4.vpz;
		VIEWPORT_DISTANCE = MAIN_D_80135334;
	} else if (frame <= endFrame) {
		GS_VIEWPOINT.vpx = lerp(DOOA_D_80084BA4.vpx, viewPos.vx, midFrame, endFrame, frame);
		GS_VIEWPOINT.vpy = lerp(DOOA_D_80084BA4.vpy, viewPos.vy, midFrame, endFrame, frame);
		GS_VIEWPOINT.vpz = lerp(DOOA_D_80084BA4.vpz, viewPos.vz, midFrame, endFrame, frame);
		offsetY = lerp(0, 20, 0, 200, height);
		DRAWING_OFFSET_Y = lerp(120, offsetY + 120, midFrame, endFrame, frame);
		VIEWPORT_DISTANCE = lerp(MAIN_D_80135334, 1000, midFrame, endFrame, frame);
		GS_VIEWPOINT.vrx = viewRef.vx;
		GS_VIEWPOINT.vry = viewRef.vy;
		GS_VIEWPOINT.vrz = viewRef.vz;
		DRAWING_OFFSET_X = 160;
	}
}

void DOOA_spawnShardWave(int32_t arg0)
{
	int16_t *model;
	int32_t *digimonId;
	int32_t boneIndex;

	model = DOOA_SHARD_EFFECT;
	digimonId = DOOA_SHARD_ENTITY[0];

	for (boneIndex = 2; boneIndex < DIGIMON_DATA[*digimonId].boneCount; boneIndex++) {
		DOOA_spawnBoneShards((DooaShardEffect *)model, boneIndex, arg0);
	}
}

void DOOA_toggleShardFlicker(void)
{
	DOOA_SHARD_FLICKER[0] = (DOOA_SHARD_FLICKER[0] + 1) & 1;
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

	GsClearOt(0, 2, &DOOA_REINCARNATION_OT[ACTIVE_FRAMEBUFFER]);
	DOOA_REINCARNATION_OT[ACTIVE_FRAMEBUFFER].point = otPoint;

	boneCount = DIGIMON_DATA[entity->type].boneCount;
	posData = entity->posData;
	lightMatrix = GsWSMATRIX;

	for (i = 0; i < boneCount; i++) {
		if (posData->obj.tmd != NULL) {
			GsGetLw(posData->obj.coord2, &lightMatrix);
			GsSetLightMatrix(&lightMatrix);
			GsGetLs(posData->obj.coord2, &lightMatrix);
			GsSetLsMatrix(&lightMatrix);
			GsSortObject4(&posData->obj, &DOOA_REINCARNATION_OT[ACTIVE_FRAMEBUFFER], 3,
			              getScratchAddr(0));
		}
		posData++;
	}

	GsSortOt(&DOOA_REINCARNATION_OT[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
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
	int32_t x1;
	int32_t y1;
	int32_t w1;
	int32_t x2;
	int32_t y2;
	int32_t w2;
	int32_t y3;
	int32_t w3;
	int32_t h3;
	int32_t y4;
	int32_t w4;

	if (endFrame < frame) {
		frame = endFrame;
	}

	worldPos.vx = entity->posData->location.vx;
	worldPos.vy = entity->posData->location.vy - (DIGIMON_DATA[entity->type].height / 2);
	worldPos.vz = entity->posData->location.vz;

	modelSize = getDistance(DIGIMON_DATA[entity->type].radius * 2,
	                        DIGIMON_DATA[entity->type].height,
	                        DIGIMON_DATA[entity->type].radius * 2);

	size = (lerp(200, 0, startFrame, endFrame, frame) << 12) / 128;

	depth = worldPosToScreenPos(&worldPos, &screenPos);
	if (depth <= 0) {
		return 1;
	}

	if ((((int32_t)((uint32_t)(modelSize * VIEWPORT_DISTANCE) / (uint32_t)depth) << 12) / 256) >= size) {
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

	x1 = endFrame - (leftEdge + 160);
	y1 = boxTop - ((int32_t)topEdge + 120);
	w1 = (leftEdge + 160) + thickness;
	if (w1 > 0) {
		SetPolyFT4(prim);
		SetSemiTrans(prim, 2);
		prim->r0 = prim->g0 = prim->b0 = 0x80;
		prim->tpage = 0xdd;
		prim->clut = 0x79c0;
		prim->u0 = 0;
		prim->v0 = 0x80;
		prim->u1 = 3;
		prim->v1 = 0x80;
		prim->u2 = 0;
		prim->v2 = 0x83;
		prim->u3 = 3;
		prim->v3 = 0x83;
		prim->x0 = x1;
		prim->y0 = y1;
		prim->x1 = x1 + w1;
		prim->y1 = y1;
		prim->x2 = x1;
		prim->y2 = y1 + 240;
		prim->x3 = x1 + w1;
		prim->y3 = y1 + 240;
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x22, prim++);
	}

	y2 = boxTop - ((long)topEdge + 120);
	x2 = boxRight - thickness;
	w2 = (160 - rightEdge) + thickness;
	if (w2 > 0) {
		SetPolyFT4(prim);
		prim->r0 = prim->g0 = prim->b0 = 0x80;
		SetSemiTrans(prim, 2);
		prim->tpage = 0xdd;
		prim->clut = 0x79c0;
		prim->u0 = 0;
		prim->v0 = 0x80;
		prim->u1 = 3;
		prim->v1 = 0x80;
		prim->u2 = 0;
		prim->v2 = 0x83;
		prim->u3 = 3;
		prim->v3 = 0x83;
		prim->x0 = x2;
		prim->y0 = y2;
		prim->x1 = x2 + w2;
		prim->y1 = y2;
		prim->x2 = x2;
		prim->y2 = y2 + 240;
		prim->x3 = x2 + w2;
		prim->y3 = y2 + 240;
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x22, prim++);
	}

	y3 = boxTop - (topEdge + 120);
	w3 = boxRight - endFrame;
	if (w3 > 0) {
		h3 = (topEdge + 120) + thickness;
		if (h3 > 0) {
			SetPolyFT4(prim);
			prim->r0 = prim->g0 = prim->b0 = 0x80;
			SetSemiTrans(prim, 2);
			prim->tpage = 0xdd;
			prim->clut = 0x79c0;
			prim->u0 = 0;
			prim->v0 = 0x80;
			prim->u1 = 3;
			prim->v1 = 0x80;
			prim->u2 = 0;
			prim->v2 = 0x83;
			prim->u3 = 3;
			prim->v3 = 0x83;
			prim->x0 = endFrame;
			prim->y0 = y3;
			prim->x1 = endFrame + w3;
			prim->y1 = y3;
			prim->x2 = endFrame;
			prim->y2 = y3 + h3;
			prim->x3 = endFrame + w3;
			prim->y3 = y3 + h3;
			AddPrim(ACTIVE_ORDERING_TABLE->org + 0x22, prim++);
		}
	}

	w4 = (long)boxRight - endFrame;
	y4 = boxBottom - thickness;
	if (w4 > 0) {
		boxRight = (120 - bottomEdge) + thickness;
		if (boxRight > 0) {
			SetPolyFT4(prim);
			prim->r0 = prim->g0 = prim->b0 = 0x80;
			SetSemiTrans(prim, 2);
			prim->tpage = 0xdd;
			prim->clut = 0x79c0;
			prim->u0 = 0;
			prim->v0 = 0x80;
			prim->u1 = 3;
			prim->v1 = 0x80;
			prim->u2 = 0;
			prim->v2 = 0x83;
			prim->u3 = 3;
			prim->v3 = 0x83;
			prim->x0 = endFrame;
			prim->y0 = y4;
			prim->x1 = endFrame + w4;
			prim->y1 = y4;
			prim->x2 = endFrame;
			prim->y2 = y4 + boxRight;
			prim->x3 = endFrame + w4;
			prim->y3 = y4 + boxRight;
			AddPrim(ACTIVE_ORDERING_TABLE->org + 0x22, prim++);
		}
	}

	GsSetWorkBase((PACKET *)prim);
	return isVisible;
}

void DOOA_tickRebirth(int32_t instanceId)
{
	VECTOR flashColor;
	VECTOR colorStart;
	VECTOR colorEnd;
	DooaSparkle sparkle;
	DooSequence *seq;
	Entity *entity;
	int32_t level;
	int32_t frame;

	seq = DOOA_REINCARNATION_SEQ;
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
		loadMMDAsync(EGG_DIGIMON_TYPES[seq->eggSlot], 3, (uint8_t *)0x80020000,
		             seq->modelData, &seq->isModelLoading);
		DOO2_resetShardSets(0x80044800);
		playSound(8, 6);
		break;
	case 0xce:
		frame = seq->frame;
		if (frame < 147) {
			seq->fadeLevel = lerp(5, 0, 132, 150, frame);
		}
		flashColor = DOOA_D_80084970;
		*EFE_DATA_STACK++ = 0;
		*EFE_DATA_STACK++ = (int32_t)&flashColor;
		*EFE_DATA_STACK++ = -1;
		*EFE_DATA_STACK++ = 4;
		*EFE_DATA_STACK++ = 0;
		level = lerp(20, 10, 132, 152, seq->frame);
		*EFE_DATA_STACK++ = level * 25 * 4096 / 1000;
		level = lerp(120, 10, 132, 152, seq->frame);
		colorStart.vx = level + (rand() % ((level * 50) / 120));
		colorStart.vy = level + (rand() % ((level * 50) / 120));
		colorStart.vz = level + (rand() % ((level * 50) / 120));
		colorEnd.vx = colorEnd.vy = colorEnd.vz = level / 6;
		*EFE_DATA_STACK++ = (int32_t)&colorStart;
		*EFE_DATA_STACK++ = (int32_t)&colorEnd;
		createFlash();
		if (seq->frame >= 152) {
			while (seq->isModelLoading != 0) {
				tickFileReadQueue(0);
			}
			reincarnatePartner((int32_t)ENTITY_TABLE[1],
			                   &PARTNER_ENTITY.digimonEntity.stats, &PARTNER_PARA,
			                   EGG_DIGIMON_TYPES[seq->eggSlot]);
			waitForSoundBufferLoading(3);
			entity = (Entity *)&PARTNER_ENTITY;
			seq->entity = entity;
			setEntityPosition(1, 0, 0, 0);
			DOOA_setOtherEntitiesHidden(1);
			ENTITY_TABLE[2]->isOnMap = 1;
			DOOA_showPlayerAndPartner();
			ENTITY_TABLE[1]->isOnMap = 0;
			DOOA_fadeModelClut(DOOA_D_80084ED8, entity, DOOA_D_80086914, 0, 1, 1);
			DOOA_fadeShardClut(DOOA_D_800851DC, entity, DOOA_D_80086914, 0, 1, 1);
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
			MAIN_func_800D9BA8(0, DOOA_D_80085B10, 0);
			DOOA_fadeModelClut(DOOA_D_80084ED8, entity, DOOA_D_80086914, 0, 1, 0);
			DOOA_fadeShardClut(DOOA_D_800851DC, entity, DOOA_D_80086914, 0, 1, 0);
			break;
		}
		if (seq->frame < 152) {
			break;
		}
		level = lerp(255, 0, 152, 202, frame);
		MAIN_func_800D9BA8(level, DOOA_D_80085B10, 0);
		if ((seq->frame & 1) == 0) {
			DOOA_fadeModelClut(DOOA_D_80084ED8, entity, DOOA_D_80086914, 0, 255, level);
		} else {
			DOOA_fadeShardClut(DOOA_D_800851DC, entity, DOOA_D_80086914, 0, 255, level);
		}
		break;
	case 0xd0:
		if (seq->sparkleIndex < 47) {
			sparkle.offset = DOOA_D_800849B0;
			if ((sparkle.boneId = DOOA_SPARKLE_BONE_IDS[seq->sparkleIndex]) >= 0) {
				DOO2_buildShardSet(&sparkle.offset, DOO2_D_80071EE4,
				                   (seq->eggSlot * 6) + sparkle.boneId);
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
	DooSequence *panel;
	u_long tmd;

	panel = DOOA_REINCARNATION_SEQ;
	i = 0;
	tmd = (u_long)DOO2_D_80071EF0;

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

void DOOA_setShardState(int16_t value)
{
	DOOA_SHARD_EFFECT[0] = value;
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

void DOOA_fadeModelClut(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame)
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
		*dst += (int16_t)(g << 5);
		*dst += (int16_t)(b << 10);
		*dst++ += (int16_t)(stp << 15);
	}

	rect.x = 0;
	rect.y = 488;
	rect.w = 16;
	rect.h = 24;
	LoadImage(&rect, (u_long *)dstClut);
	DrawSync(0);
}

void DOOA_fadeShardClut(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame)
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
		*dst += (int16_t)(g << 5);
		*dst += (int16_t)(b << 10);
		*dst++ += (int16_t)(stp << 15);
	}

	rect.x = 48;
	rect.y = 488;
	rect.w = 32;
	rect.h = 24;
	LoadImage(&rect, (u_long *)dstClut);
	DrawSync(0);
}

void DOOA_getOrbitPosition(VECTOR *offset, VECTOR *pos, VECTOR *src, SVECTOR *rotation, int32_t distance, int32_t height)
{
	MATRIX matrix;
	VECTOR direction;
	int32_t y;

	offset->vx = src->vx;
	y = src->vy;
	offset->vy = y - (height / 2);
	offset->vz = src->vz;
	RotMatrixZYX(rotation, &matrix);
	direction.vx = 0;
	direction.vy = 0;
	direction.vz = distance;
	ApplyMatrixLV(&matrix, &direction, pos);
	pos->vx += offset->vx;
	pos->vy += (offset->vy - 400) - (height * 2);
	pos->vz += offset->vz;
}

int32_t DOOA_updateShards(int32_t instanceId)
{
	DooaShard *piece;
	DooaShard *p;
	DooaShardEffect *effect;
	int16_t state;

	effect = (DooaShardEffect *)DOOA_SHARD_EFFECT;

	piece = DOOA_SHARD_LIST[0];
	if (DOOA_SHARD_FLICKER[0] != 0) {
		effect->colorR = (rand() % 100) + 60;
		effect->colorG = (rand() % 100) + 60;
		effect->colorB = (rand() % 100) + 60;
	}

	while (piece->prim != NULL) {
		p = (DooaShard *)((int32_t)piece);
		state = effect->state;
		switch (state) {
		case 0:
			p->fallSpeed = p->fallSpeed + 1;
			if (p->fallSpeed >= 0x400) {
				p->fallSpeed = 0x400;
			}
			p->centerY = p->centerY + p->fallSpeed;
			if (p->centerY > 0) {
				p->centerY = 0;
			}
			break;
		case 1:
			if (effect->state != effect->prevState) {
				int32_t minRadius;
				int32_t maxRadius;

				p->spin = 0;
				p->spinMax = (rand() % 0x155) + 0xe3;
				p->radius = 0x1000;
				minRadius = 0x64000 / p->axisDistance;
				maxRadius = 0x190000 / p->axisDistance;
				p->targetRadius = minRadius + (rand() % (maxRadius - minRadius));
				p->fallSpeed = 0;
				p->dropDepth = rand() % 0x226;
			}
			if ((p->spinMax / 80) == 0) {
				p->spin = p->spin + 1;
			} else {
				int16_t step;

				step = p->spinMax / 100;
				p->spin = p->spin + step;
			}
			if (p->spin > p->spinMax) {
				p->spin = p->spinMax;
			}
			p->rotY = p->rotY + p->spin;
			if (p->targetRadius > p->radius) {
				if ((p->targetRadius / 80) == 0) {
					p->radius++;
				} else {
					p->radius += p->targetRadius / 100;
				}
			}
			p->fallSpeed = p->fallSpeed + 1;
			{
				int16_t drop;

				drop = p->fallSpeed >> 3;
				p->centerY = p->centerY - drop;
			}
			if (p->centerY < -p->dropDepth) {
				p->centerY = -p->dropDepth;
			}
			break;
		case 2:
			p->rotY = p->rotY + p->spin;
			break;
		case 3:
			if (effect->state != effect->prevState) {
				p->delay = rand() % 20;
				p->targetRadius = p->radius;
			}
			if (p->delay > 0) {
				p->delay--;
			} else {
				p->radius = p->radius - ((p->targetRadius / 10) + 1);
				if (p->radius < 0) {
					p->radius = 0;
					p->delay = -1;
				}
				p->centerY = lerp(-p->dropDepth, MAIN_func_800DA9F4() - 100, p->targetRadius, 0, p->radius);
			}
			p->rotY = p->rotY + p->spin;
			p->rotY = p->rotY % 4096;
			break;
		}

		switch (piece->prim->cd) {
		case 0x34:
		case 0x36:
			piece = (DooaShard *)((int32_t)piece + 0x38);
			break;
		case 0x3c:
		case 0x3e:
			piece = (DooaShard *)((int32_t)piece + 0x3c);
			break;
		}
	}

	effect->prevState = effect->state;
}

int32_t DOOA_renderShards(int32_t instance)
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
	int32_t cursor;
	DooaModelComponent *model;
	int32_t triScale;
	int32_t quadScale;
	int16_t triOx;
	int16_t triOy;
	int16_t triOz;
	int16_t quadOx;
	int16_t quadOy;
	int16_t quadOz;
	uint8_t mode;

	cursor = (int32_t)DOOA_SHARD_LIST[0];
	effect = (DooaShardEffect *)DOOA_SHARD_EFFECT;
	model = getEntityModelComponent(((Entity *)DOOA_SHARD_ENTITY[0])->type, 3);

	while (*(int32_t *)cursor != 0) {
		tmdPrim = *(TMD_P_TG4 **)cursor;
		mode = tmdPrim->cd;
		switch (mode) {
		case 0x34:
		case 0x36:
			tri = (DooaShard *)cursor;
			if (tri->delay >= 0) {
				triPrim = (POLY_FT3 *)GsGetWorkBase();
				MAIN_func_80092B60((POLY_FT4 *)triPrim);
				if (effect->flash != 0) {
					triPrim->r0 = effect->colorR;
					triPrim->g0 = effect->colorG;
					triPrim->b0 = effect->colorB;
					triPrim->tpage = 0x9d;
					triPrim->clut = 0x79c0;
					triPrim->u0 = 0;
					triPrim->v0 = 128;
					triPrim->u1 = 3;
					triPrim->v1 = 128;
					triPrim->u2 = 0;
					triPrim->v2 = 131;
				} else {
					triPrim->r0 = effect->colorR;
					triPrim->g0 = effect->colorB;
					triPrim->b0 = effect->colorG;
					triPrim->tpage = model->pixelPage;
					triPrim->clut = tmdPrim->clut;
					triPrim->u0 = tmdPrim->tu0;
					triPrim->v0 = tmdPrim->tv0;
					triPrim->u1 = tmdPrim->tu1;
					triPrim->v1 = tmdPrim->tv1;
					triPrim->u2 = tmdPrim->tu2;
					triPrim->v2 = tmdPrim->tv2;
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
				triA.vx = triA.vx + (int16_t)effect->entity->posData->location.vx;
				triA.vy = triA.vy + (int16_t)MAIN_func_800DA9F4();
				triA.vz = triA.vz + (int16_t)effect->entity->posData->location.vz;
				triB.vx = triB.vx + (int16_t)effect->entity->posData->location.vx;
				triB.vy = triB.vy + (int16_t)MAIN_func_800DA9F4();
				triB.vz = triB.vz + (int16_t)effect->entity->posData->location.vz;
				triC.vx = triC.vx + (int16_t)effect->entity->posData->location.vx;
				triC.vy = triC.vy + (int16_t)MAIN_func_800DA9F4();
				triC.vz = triC.vz + (int16_t)effect->entity->posData->location.vz;
				addScreenPolyFT3(triPrim, &triA, &triB, &triC);
			}
			cursor = cursor + 0x38;
			break;
		case 0x3c:
		case 0x3e:
			quad = (DooaShardQuad *)cursor;
			if (quad->delay >= 0) {
				quadPrim = (POLY_FT4 *)GsGetWorkBase();
				SetPolyFT4(quadPrim);
				if (effect->flash != 0) {
					quadPrim->r0 = effect->colorR;
					quadPrim->g0 = effect->colorG;
					quadPrim->b0 = effect->colorB;
					quadPrim->tpage = 0x9d;
					quadPrim->clut = 0x79c0;
					quadPrim->u0 = 0;
					quadPrim->v0 = 128;
					quadPrim->u1 = 3;
					quadPrim->v1 = 128;
					quadPrim->u2 = 0;
					quadPrim->v2 = 131;
					quadPrim->u3 = 3;
					quadPrim->v3 = 131;
				} else {
					quadPrim->r0 = effect->colorR;
					quadPrim->g0 = effect->colorB;
					quadPrim->b0 = effect->colorG;
					quadPrim->tpage = model->pixelPage;
					quadPrim->clut = tmdPrim->clut;
					quadPrim->u0 = tmdPrim->tu0;
					quadPrim->v0 = tmdPrim->tv0;
					quadPrim->u1 = tmdPrim->tu1;
					quadPrim->v1 = tmdPrim->tv1;
					quadPrim->u2 = tmdPrim->tu2;
					quadPrim->v2 = tmdPrim->tv2;
					quadPrim->u3 = tmdPrim->tu3;
					quadPrim->v3 = tmdPrim->tv3;
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
				quadA.vx = quadA.vx + (int16_t)effect->entity->posData->location.vx;
				quadA.vy = quadA.vy + (int16_t)MAIN_func_800DA9F4();
				quadA.vz = quadA.vz + (int16_t)effect->entity->posData->location.vz;
				quadB.vx = quadB.vx + (int16_t)effect->entity->posData->location.vx;
				quadB.vy = quadB.vy + (int16_t)MAIN_func_800DA9F4();
				quadB.vz = quadB.vz + (int16_t)effect->entity->posData->location.vz;
				quadC.vx = quadC.vx + (int16_t)effect->entity->posData->location.vx;
				quadC.vy = quadC.vy + (int16_t)MAIN_func_800DA9F4();
				quadC.vz = quadC.vz + (int16_t)effect->entity->posData->location.vz;
				quadD.vx = quadD.vx + (int16_t)effect->entity->posData->location.vx;
				quadD.vy = quadD.vy + (int16_t)MAIN_func_800DA9F4();
				quadD.vz = quadD.vz + (int16_t)effect->entity->posData->location.vz;
				add3DSpritePrim(quadPrim, &quadA, &quadB, &quadC, &quadD);
			}
			cursor = cursor + 0x3c;
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
	int32_t vertOut;
	int32_t frags;
	int32_t prim;
	int32_t i;
	int32_t j;

	entity = effect->entity;
	frags = effect->shardWrite;
	objs = ((DooaTMDModel *)getEntityModelComponent(entity->type, 3)->modelPtr)->obj;
	objIndex = DIGIMON_SKELETONS[entity->type][boneIndex].objIndex;
	if (objIndex == -1) {
		return;
	}

	obj = &objs[objIndex];
	vertOut = (int32_t)GsGetWorkBase();
	src = (SVECTOR *)obj->vertop;
	calculateBoneMatrix(entity, boneIndex, &boneMatrix);
	verts = (DooaShardVertex *)vertOut;
	for (i = 0; i < obj->vern; i++) {
		ApplyMatrixSV(&boneMatrix, src++, &rotated);
		((DooaShardVertex *)vertOut)->vx = rotated.vx + boneMatrix.t[0];
		((DooaShardVertex *)vertOut)->vy = rotated.vy + boneMatrix.t[1];
		((DooaShardVertex *)vertOut)->vz = rotated.vz + boneMatrix.t[2];
		vertOut += 6;
	}

	prim = (int32_t)obj->primtop;
	for (j = 0; j < obj->primn; j++) {
		if (((uint32_t)frags + 0x40) >= (uint32_t)(effect->shardBuffer + effect->shardBytes)) {
			break;
		}
		if (((rand() & 3) != 0) && (WIREFRAME_RNG_TABLE[j & 0xf] == wireIndex)) {
			switch (((int8_t *)prim)[3]) {
			case 0x34:
			case 0x36:
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
				frag->centerX -= (int16_t)effect->entity->posData->location.vx;
				frag->centerY -= (int16_t)MAIN_func_800DA9F4();
				frag->centerZ -= (int16_t)effect->entity->posData->location.vz;
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
				frags += 0x38;
				break;
			case 0x3c:
			case 0x3e:
				frag = (DooaShard *)frags;
				frag->prim = (TMD_P_TG4 *)prim;
				frag->centerX = (verts[((TMD_P_TG4 *)prim)->v0].vx + verts[((TMD_P_TG4 *)prim)->v1].vx + verts[((TMD_P_TG4 *)prim)->v2].vx +
				                 verts[((TMD_P_TG4 *)prim)->v3].vx) /
				                4;
				frag->centerY = (verts[((TMD_P_TG4 *)prim)->v0].vy + verts[((TMD_P_TG4 *)prim)->v1].vy + verts[((TMD_P_TG4 *)prim)->v2].vy +
				                 verts[((TMD_P_TG4 *)prim)->v3].vy) /
				                4;
				frag->centerZ = (verts[((TMD_P_TG4 *)prim)->v0].vz + verts[((TMD_P_TG4 *)prim)->v1].vz + verts[((TMD_P_TG4 *)prim)->v2].vz +
				                 verts[((TMD_P_TG4 *)prim)->v3].vz) /
				                4;
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
				frag->centerX -= (int16_t)effect->entity->posData->location.vx;
				frag->centerY -= (int16_t)MAIN_func_800DA9F4();
				frag->centerZ -= (int16_t)effect->entity->posData->location.vz;
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
				frags += 0x3c;
				break;
			}
		}
		switch (((int8_t *)prim)[3]) {
		case 0x34:
		case 0x36:
			prim += 0x1c;
			break;
		case 0x3c:
		case 0x3e:
			prim += 0x24;
			break;
		}
	}

	((DooaShard *)frags)->prim = 0;
	effect->shardWrite = frags;
}

int32_t DOOA_tick(PartnerEntity *partner, void *buffer, int32_t isInitialized)
{
	DooSequence *panel;
	Entity *player;
	int32_t messageId;

	panel = DOOA_REINCARNATION_SEQ;
	if (isInitialized != 0) {
		return panel->frame;
	}
	initializeFlashData(DOOA_D_800849C0);
	if (partner->lives != 0) {
		panel->frame = 0;
	} else {
		panel->frame = 0x23;
		partner->digimonEntity.entity.isOnMap = 0;
	}
	panel->entity = &partner->digimonEntity.entity;
	panel->phase = 0;
	addObject(0x80b, 0, DOOA_tickDissolve, DOOA_renderDissolve);
	DOOA_D_80084BA4 = GS_VIEWPOINT;
	MAIN_D_8013532C = DRAWING_OFFSET_X;
	MAIN_D_80135330 = DRAWING_OFFSET_Y;
	MAIN_D_80135334 = VIEWPORT_DISTANCE;
	player = ENTITY_TABLE[1];
	DOOA_D_80084BC4 = player->posData->location;
	MAIN_D_80135338 = player->posData->rotation;
	if (partner->lives != 0) {
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 1);
	}
	DOOA_initOrderingTables();
	if ((isTriggerSet(0xdc) == 1) || (isTriggerSet(0xd6) == 1) ||
	    (readPStat(1) >= 50)) {
		messageId = 0xcd;
	} else {
		messageId = 0xda;
	}
	MAIN_D_80135340 = messageId;
	MAIN_func_800D91EC(messageId, 1);
	return (int32_t)buffer;
}

int32_t DOOA_getSequenceState(int32_t unused, int32_t isInitialized)
{
	PartnerEntity *partner;
	DooSequence *sequence;
	int32_t i;

	partner = (PartnerEntity *)DOOA_D_8008786C[0];
	sequence = DOOA_REINCARNATION_SEQ;

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
