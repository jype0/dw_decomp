#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libcd.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/aabb.h>
#include <dw/entity.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/vs.h>

#include "common.h"

#define EFE_POP(ptr, type) ((type) *--(ptr))
#define EFE_PUSH(ptr, type, value) (*(ptr)++ = (int32_t)(type)(value))

#define EFE_POP1(type) EFE_POP(EFE_DATA_STACK, type)
#define EFE_PUSH1(type, value) EFE_PUSH(EFE_DATA_STACK, type, value)
#define EFE_DROP1() (--EFE_DATA_STACK)

#define EFE_POP2(type) EFE_POP(MAIN_D_80134CFC, type)
#define EFE_PUSH2(type, value) EFE_PUSH(MAIN_D_80134CFC, type, value)

#define EFE_SCRATCH ((EfeScratch *)getScratchAddr(0))

typedef struct {
	int32_t position[3];
	int32_t rotation[3];
} EfeTransform;

typedef struct {
	int32_t frame;
	EfeTransform transform;
} EfeInstance;

typedef struct {
	int16_t boneId;
	int16_t positionX;
	int16_t positionY;
	int16_t positionZ;
} EfeBoneOffset;

typedef struct {
	int16_t *inst;
	int16_t *someInst;
	EfeInstance *instance;
	EfeInstance *parentInstance;
	Entity *sourceEntity;
	Entity *targetEntity;
	EfeBoneOffset *boneOffset;
} EfeSubEffect;

typedef struct {
	int32_t frame;
	SVECTOR position;
	SVECTOR rotation;
	SVECTOR velocity;
	char *typeData;
	Entity *owner;
} EfeAura;

typedef struct {
	int16_t state;
	int16_t pad;
	int8_t *isLoaded;
	int16_t *moves;
	int16_t *effectIds;
	ModelComponent *model;
} EfeLoad;

typedef struct {
	int32_t rotation[3];
} EfeRot;

typedef struct {
	GsDOBJ2 obj;
	SVECTOR rot;
	MATRIX m0;
	MATRIX m1;
	MATRIX m2;
	VECTOR *scale;
	int32_t id;
} EfeScratch;

extern EfeInstance *EFE_INSTANCE;
extern int32_t *MAIN_D_80134D0C;
extern int32_t MAIN_D_80134CE0;
extern char *MAIN_D_80134D10;
extern int32_t MAIN_D_80134D14;
extern int32_t MAIN_D_80134D18;
extern char *MAIN_D_80134CCC;
extern int32_t MAIN_D_80134CE4;
extern int32_t MAIN_D_80134CF0;
extern int32_t EFE_SCRIPT_MEM1_DATA[];
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern int32_t EFE_PARENT_INSTANCE;
extern int8_t MAIN_D_80134B20[4];
extern int8_t MAIN_D_80134B24[4];
extern int8_t MAIN_D_80134B4C[6];
extern SVECTOR MAIN_D_80134B54;
extern SVECTOR MAIN_D_80134B70;
extern SVECTOR MAIN_D_80134B78;
extern SVECTOR MAIN_D_80134B80;
extern SVECTOR MAIN_D_80134B88;
extern int32_t MAIN_D_80134CD0;
extern int32_t MAIN_D_801352F4;
extern char *MAIN_D_801352F8;
extern int32_t MAIN_D_801352FC;
extern char *MAIN_D_80135300;
extern int32_t MAIN_D_80139B54[];
extern ModelComponent UNKNOWN_MODEL[16];
extern int32_t VS_D_80070B44[];
extern void *VS_D_80070B48[];
extern void (*VS_D_80070E94[][8])(int32_t *);
extern int32_t (*VS_D_80070F34[])(int32_t);
extern VECTOR VS_D_80070FA4;
extern VECTOR VS_D_80071044;
extern VECTOR VS_D_80071064;
extern int16_t VS_D_80071084[];
extern SVECTOR VS_D_800731C4[];
extern GsSPRITE VS_POISON_BUBBLE_SPRITE;
extern int32_t MAIN_D_80134CD4;
extern int16_t MAIN_D_80139AB0[];
extern void (*VS_jtbl_80070E4C[])(void);
extern EfeSubEffect *MAIN_D_80134CE8;
extern VECTOR VS_D_80071054;
extern char MAIN_D_80134B1C[8];
extern int16_t *MAIN_D_80134D00;
extern int16_t EFE_LOADED_MOVE_DATA[];
extern int16_t MAIN_D_80134D04;
extern int16_t VS_D_80072FC0[];
extern int16_t VS_D_80073070[][8];
extern int16_t VS_D_800730B0[][6];
extern int16_t VS_D_800730EC[][4];
extern int16_t VS_D_800731B4[];
extern int32_t MAIN_D_80134CD8;
extern int32_t MAIN_D_80134D08;
extern GsRVIEW2 MAIN_D_80139B34;
extern int32_t VS_D_80072FA0[];
extern int32_t MAIN_D_801352EC;
extern int32_t MAIN_D_801352F0;
extern int16_t MAIN_D_80139B20[];
extern int8_t *MAIN_D_80139B24[];
extern int32_t MAIN_D_80139B28[];
extern int32_t MAIN_D_80139B2C[];
extern int32_t MAIN_D_801352E4;
extern int32_t MAIN_D_80139AD0[][2];
extern void (*VS_jtbl_80072E1C[])(void);
extern int16_t VS_D_80073050[][4];
extern int32_t *EFE_DATA_STACK;
extern int32_t *MAIN_D_80134CFC;
extern int32_t MAIN_D_80134CEC;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t VIEWPORT_DISTANCE;

void MAIN_func_80092B60(POLY_FT4 *prim);
void MAIN_func_80092C18(PACKET *prim, RECT *rect);
int32_t MAIN_func_800DA740(int16_t *rect, DVECTOR *line);
void MAIN_func_800E4470(MATRIX *m, SVECTOR *out);
void downloadSomeImage();
void modifySomeImage(int32_t dim);
int32_t addFileReadRequest(char *path, uint8_t *buffer, uint8_t *isRunning, void *callback, void *callbackParam, CdlLOC *loc, int32_t size);
CdlLOC *getEFEDATEntry(int32_t id);
char *VS_initializeParticleEmitters(char *base);
void VS_tickEFEEngine(void);
void VS_renderEFEEngine(void);
void VS_clearEFESoundChannels(void);
void VS_stopEFESounds(void);
void VS_loadNextEFEFile(int16_t *arg);
void VS_unloadEFESlot(int32_t idx);
void VS_runEFESlotScript(int32_t idx);
void VS_setupLoadedEFEFile(void);
void VS_handleEFEFileLoaded(int32_t arg);
void VS_tickEFEUVAnimation(int32_t idx);
void VS_tickParticleEmitters(void);
void VS_renderParticleEmitters(void);
int16_t VS_offsetEFEPrimitiveUVs(char *base, int32_t idx, int32_t du, int32_t dv);
int32_t VS_startEFE(int32_t i);
char *VS_getEFEHeapPointer(void);
char *VS_getEFETextureSection(char *p);
char *VS_getEFEModelSection(char *p);
int32_t VS_getEFEFileId(char *p);
void VS_isTargetUnhit(void);
void VS_markEFEFinished(void);
void VS_getViewportDistance2(void);
void VS_disableMapLayer(void);
void VS_renderScreenFade(void);
void VS_applyBoxAttackHit(void);
void VS_applyRadiusAttackHit(void);
void VS_applyLineAttackHit(void);
void VS_faceTargetEntity(void);
void VS_renderScreenOverlay(void);
void VS_renderRingTube(void);
void VS_renderRibbonStrip(void);
void VS_tickRibbonPoints(void);
void VS_initializeRibbonPoints(void);
void VS_renderRadialWaves(void);
void VS_getViewportDistance(void);
void VS_addClutLoadPrim(void);
void VS_drawTMDScreenSpace(void);
void VS_loadClutColors(void);
void VS_drawTMDYXZ(void);
void VS_getCameraRotation(void);
void VS_selectRandomTargetEntity(void);
void VS_convertToViewSpace(void);
void VS_maskVectorByScalar(void);
void VS_divideVectorByScalar(void);
void VS_multiplyVectorByScalar(void);
void VS_render3DTexturedQuad(void);
void VS_setTransformToBoneMatrix(void);
void VS_renderWireframeBox(void);
void VS_discardEFEOperand(void);
void VS_renderWireframeGrid(void);
void VS_render2DTexturedQuad(void);
void VS_restoreCameraView(void);
void VS_setupFixedCamera(void);
void VS_getSourceBoneTransform(void);
void VS_copyToParentTransform(void);
void VS_renderEFELine(void);
void VS_combineRotations(void);
void VS_normalizeRotationAngles2(void);
void VS_rotateVectorByAngles(void);
void VS_getTargetBoneTransform(void);
void VS_centerTransformOnEntities(void);
void VS_shiftVectorsRight(void);
void VS_maskVectors(void);
void VS_divideVectors(void);
void VS_multiplyVectors(void);
void VS_subtractVectors(void);
void VS_addVectors(void);
void VS_copyVector(void);
void VS_getVectorLength(void);
void VS_setTargetToHitEntity(void);
void VS_normalizeRotationAngles(void);
void VS_findHitEntity(void);
void VS_getVectorEulerAngles(void);
void VS_printDebugValue(void);
void VS_getRandomInRange(void);
void VS_interpolateValue(void);
void VS_calculateCosine(void);
void VS_calculateSine(void);
void VS_getSourceDigimonSize(void);
void VS_applyHomingMovement(void);
void VS_getUVAnimTimer(void);
void VS_checkTargetCollision(void);
void VS_rotateTransformTowardPoint(void);
void VS_setTransformToSourceBone(void);
void VS_renderParallaxSprites(void);
void VS_renderScrollingBackground(void);
void VS_setTransformToBoneOffset(void);
void VS_playEFESound(void);
void VS_addSourceEntityParticleFX(void);
void VS_copyFromParentTransform(void);
void VS_calculatePolarOffset(void);
void VS_renderProjectedSprite(void);
void VS_getTargetDigimonSize(void);
void VS_renderParticleFlashSprite(void);
void VS_projectPositionToScreen(void);
void VS_renderScreenSprite(void);
void VS_addCloudEffect(void);
void VS_selectNextTargetEntity(void);
void VS_addParticleEmitter(void);
void VS_setEFEModelObjectColor(void);
void VS_copyTargetEntityPosition(void);
void VS_steerTransformTowardPoint(void);
void VS_interpolateVector(void);
void VS_discardEFEOperandPair(void);
void VS_getScatteredSpawnPosition(void);
void VS_checkCollisionWithDefaultPower(void);
void VS_addAttackObjectToTarget(void);
void VS_setTransformToTargetBone(void);
void VS_renderCenteredSprite(void);
void VS_initializeEFETransform(void);
void VS_drawTMD(void);
void VS_initializeSubEffectInstructions(void);
void VS_initializeUVAnim(void);
void VS_checkTechCompatibility(void);
void VS_spawnEFESubEffect(void);
void VS_popEFEValueToVariable(void);
void VS_returnFromEFESubroutine(void);
void VS_dispatchEFESubOpcode(void);
void VS_callEFESubroutine(void);
void VS_pushEFEVariableAddress(void);
void VS_pushEFEVariable(void);
void VS_pushEFEImmediate(void);
void VS_jumpEFEScript(void);
void VS_stopEFEScript(void);
void VS_branchEFEOnComparison(void);
void VS_applyEFEVariableOperator(void);
void VS_loadEFEIndexedVariable(void);
void VS_loadEFERandomValue(void);
void VS_loadEFEVariable(void);
void VS_loadEFEImmediate(void);
int32_t VS_shiftRightInt32Variable(int32_t *p);
int32_t VS_shiftLeftInt32Variable(int32_t *p);
int32_t VS_moduloInt32Variable(int32_t *p);
int32_t VS_divideInt32Variable(int32_t *p);
int32_t VS_multiplyInt32Variable(int32_t *p);
int32_t VS_subtractInt32Variable(int32_t *p);
int32_t VS_addInt32Variable(int32_t *p);
int32_t VS_setInt32Variable(int32_t *p);
int32_t VS_shiftRightInt8Variable(int8_t *p);
int32_t VS_shiftLeftInt8Variable(int8_t *p);
int32_t VS_moduloInt8Variable(int8_t *p);
int32_t VS_divideInt8Variable(int8_t *p);
int32_t VS_multiplyInt8Variable(int8_t *p);
int32_t VS_subtractInt8Variable(int8_t *p);
int32_t VS_addInt8Variable(int8_t *p);
int32_t VS_setInt8Variable(int8_t *p);
int32_t VS_shiftRightInt16Variable(int16_t *p);
int32_t VS_shiftLeftInt16Variable(int16_t *p);
int32_t VS_moduloInt16Variable(int16_t *p);
int32_t VS_divideInt16Variable(int16_t *p);
int32_t VS_multiplyInt16Variable(int16_t *p);
int32_t VS_subtractInt16Variable(int16_t *p);
int32_t VS_addInt16Variable(int16_t *p);
int32_t VS_setInt16Variable(int16_t *p);
int32_t VS_compareGreaterOrEqual(int32_t x);
int32_t VS_compareGreater(int32_t x);
int32_t VS_compareLessOrEqual(int32_t x);
int32_t VS_compareLess(int32_t x);
int32_t VS_compareNotEqual(int32_t x);
int32_t VS_compareEqual(int32_t x);
int16_t VS_calculateAttackHitPosition(SVECTOR *out, int32_t *self, int32_t *other, int32_t y);
void VS_renderParallelLines(SVECTOR *a, SVECTOR *b, int32_t n, SVECTOR *from, SVECTOR *to, int32_t *col);
int32_t VS_interpolateClamped(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end);
void VS_initializeEFESubOpcodeTable(void);
void VS_dispatchEFEOpcode(int32_t op);
int32_t VS_runEFEScript(int32_t script);
void VS_resetPoisonBubbles(void);
int32_t VS_addPoisonBubble(int32_t arg);
void VS_tickPoisonBubble(int32_t i);
void VS_renderPoisonBubble(int32_t i);
void VS_tickPoisonEffect(int32_t i);
void VS_renderPoisonEffect(void);
void VS_initializePoisonBubble(void);
void VS_tickConfusionEffect(int32_t i);
void VS_renderConfusionEffect(int32_t idx);
void VS_initializeConfusionEffect(char *base);
void VS_resetStunSubEffects(void);
void VS_tickStunEffect(int32_t i);
void VS_renderStunEffect(int32_t idx);
void VS_removeAllStunSubEffects(void);
int32_t VS_addStunSubEffect(DigimonEntity *digimon);
void VS_tickStunSubEffect(int32_t i);
void VS_renderStunSubEffect(int32_t i);
void VS_initializeStunEffect(char *base);
void VS_setTMDObjectColor(int32_t idx, int32_t *color, char *base);
void VS_tickFinisherAura(int32_t i);
void VS_renderFinisherAura(int32_t idx);
void VS_renderFinisherAuraSpark(char *pos, int32_t scale, SVECTOR *dir, uint8_t *col);
void VS_initializeFinisherAuraModel(char *tim, char *base);
void VS_removeFinisherAura(int32_t i);
int16_t VS_tickAuraProjectile(int32_t idx);
void VS_renderAuraProjectile(int32_t i);
char *VS_initializeAuraProjectiles(char *base);
void setShortWithStride(int16_t *ptr, int16_t value, int32_t count, int32_t stride);
int32_t getDistance(int32_t x, int32_t y, int32_t z);
void createCloudFX(int16_t *pos);
int32_t addEntityParticleFX(int32_t *typePtr, int32_t timer);
int32_t customRandom(int32_t a, int32_t b);
void matrixToEuler2(MATRIX *m, SVECTOR *out);
void toEulerAngles(SVECTOR *out, int32_t x, int32_t y, int32_t z);
void multiplyRotations(SVECTOR *a, SVECTOR *b);
int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t);
void unloadModel(int32_t id, int32_t flag);
int32_t getOriginalType(int32_t type);
int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
void renderParticleFlash(int16_t *params);
int32_t addAttackObject(int32_t a, int32_t b, int16_t *rect, int32_t d, int32_t e, int32_t f);
void renderLinePrimitive(uint32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t order, uint32_t mode);
void calculatePosition(GsCOORDINATE2 *coord, MATRIX *matrix);
char *initializeFlashData(char *base);
void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
int32_t add3DSpritePrim(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
void setFileReadCallback2(void *callback, int32_t arg);
void renderTMDModel(uint8_t *buffer, int32_t id, GsCOORDINATE2 *coord, GsCOORDINATE2 *super, VECTOR *trans, SVECTOR *rot, VECTOR *scale);
void translateConditionFXToEntity(Entity *entity, SVECTOR *out);
void renderSprite(GsSPRITE *sprite, int16_t x, int16_t y, int32_t distance, int32_t width, int32_t height);
void GsGetTimInfo(unsigned long *tim, GsIMAGE *img);
void setMapLayerEnabled(int32_t enabled);
void addObject(int32_t objectId, int32_t instanceId, void *tick, void *render);
void VS_stopEFESubEffect(int32_t a, int32_t b);
int32_t VS_addPoisonEffect(DigimonEntity *digimon);
void VS_removePoisonEffect(int32_t i, DigimonEntity *digimon);
int32_t VS_addConfusionEffect(DigimonEntity *digimon);
void VS_removeConfusionEffect(int32_t i, DigimonEntity *digimon);
int32_t VS_addStunEffect(DigimonEntity *digimon, int32_t val);
void VS_removeStunEffect(int32_t i, DigimonEntity *digimon);
void removeObject(int32_t objectId, int32_t instanceId);

static void *vs_effect_functions[] = {
	VS_removeAllAuraProjectiles,
	VS_addAuraProjectile,
	VS_initializeAuraProjectiles,
	VS_renderAuraProjectile,
	VS_tickAuraProjectile,
	VS_removeAllFinisherAuras,
	VS_removeFinisherAura,
	VS_addFinisherAura,
	VS_initializeFinisherAuraModel,
	VS_renderFinisherAuraSpark,
	VS_renderFinisherAura,
	VS_tickFinisherAura,
	VS_setTMDObjectColor,
	VS_removeAllStunEffects,
	VS_removeStunEffect,
	VS_addStunEffect,
	VS_initializeStunEffect,
	VS_renderStunSubEffect,
	VS_tickStunSubEffect,
	VS_addStunSubEffect,
	VS_removeAllStunSubEffects,
	VS_renderStunEffect,
	VS_tickStunEffect,
	VS_resetStunSubEffects,
	VS_removeConfusionEffect,
	VS_addConfusionEffect,
	VS_initializeConfusionEffect,
	VS_renderConfusionEffect,
	VS_tickConfusionEffect,
	VS_removeAllPoisonEffects,
	VS_removePoisonEffect,
	VS_addPoisonEffect,
	VS_initializePoisonBubble,
	VS_renderPoisonEffect,
	VS_tickPoisonEffect,
	VS_renderPoisonBubble,
	VS_tickPoisonBubble,
	VS_addPoisonBubble,
	VS_resetPoisonBubbles,
	VS_runEFEScript,
	VS_dispatchEFEOpcode,
	VS_initializeEFESubOpcodeTable,
	VS_interpolateClamped,
	VS_renderParallelLines,
	VS_calculateAttackHitPosition,
	VS_compareEqual,
	VS_compareNotEqual,
	VS_compareLess,
	VS_compareLessOrEqual,
	VS_compareGreater,
	VS_compareGreaterOrEqual,
	VS_setInt16Variable,
	VS_addInt16Variable,
	VS_subtractInt16Variable,
	VS_multiplyInt16Variable,
	VS_divideInt16Variable,
	VS_moduloInt16Variable,
	VS_shiftLeftInt16Variable,
	VS_shiftRightInt16Variable,
	VS_setInt8Variable,
	VS_addInt8Variable,
	VS_subtractInt8Variable,
	VS_multiplyInt8Variable,
	VS_divideInt8Variable,
	VS_moduloInt8Variable,
	VS_shiftLeftInt8Variable,
	VS_shiftRightInt8Variable,
	VS_setInt32Variable,
	VS_addInt32Variable,
	VS_subtractInt32Variable,
	VS_multiplyInt32Variable,
	VS_divideInt32Variable,
	VS_moduloInt32Variable,
	VS_shiftLeftInt32Variable,
	VS_shiftRightInt32Variable,
	VS_loadEFEImmediate,
	VS_loadEFEVariable,
	VS_loadEFERandomValue,
	VS_loadEFEIndexedVariable,
	VS_applyEFEVariableOperator,
	VS_branchEFEOnComparison,
	VS_stopEFEScript,
	VS_jumpEFEScript,
	VS_pushEFEImmediate,
	VS_pushEFEVariable,
	VS_pushEFEVariableAddress,
	VS_callEFESubroutine,
	VS_dispatchEFESubOpcode,
	VS_returnFromEFESubroutine,
	VS_popEFEValueToVariable,
	VS_spawnEFESubEffect,
	VS_checkTechCompatibility,
	VS_initializeUVAnim,
	VS_initializeSubEffectInstructions,
	VS_drawTMD,
	VS_initializeEFETransform,
	VS_renderCenteredSprite,
	VS_setTransformToTargetBone,
	VS_addAttackObjectToTarget,
	VS_checkCollisionWithDefaultPower,
	VS_getScatteredSpawnPosition,
	VS_discardEFEOperandPair,
	VS_interpolateVector,
	VS_steerTransformTowardPoint,
	VS_copyTargetEntityPosition,
	VS_setEFEModelObjectColor,
	VS_addParticleEmitter,
	VS_selectNextTargetEntity,
	VS_addCloudEffect,
	VS_renderScreenSprite,
	VS_projectPositionToScreen,
	VS_renderParticleFlashSprite,
	VS_getTargetDigimonSize,
	VS_renderProjectedSprite,
	VS_calculatePolarOffset,
	VS_copyFromParentTransform,
	VS_addSourceEntityParticleFX,
	VS_playEFESound,
	VS_setTransformToBoneOffset,
	VS_renderScrollingBackground,
	VS_renderParallaxSprites,
	VS_setTransformToSourceBone,
	VS_rotateTransformTowardPoint,
	VS_checkTargetCollision,
	VS_getUVAnimTimer,
	VS_applyHomingMovement,
	VS_getSourceDigimonSize,
	VS_calculateSine,
	VS_calculateCosine,
	VS_interpolateValue,
	VS_getRandomInRange,
	VS_printDebugValue,
	VS_getVectorEulerAngles,
	VS_findHitEntity,
	VS_normalizeRotationAngles,
	VS_setTargetToHitEntity,
	VS_getVectorLength,
	VS_copyVector,
	VS_addVectors,
	VS_subtractVectors,
	VS_multiplyVectors,
	VS_divideVectors,
	VS_maskVectors,
	VS_shiftVectorsRight,
	VS_centerTransformOnEntities,
	VS_getTargetBoneTransform,
	VS_rotateVectorByAngles,
	VS_normalizeRotationAngles2,
	VS_combineRotations,
	VS_renderEFELine,
	VS_copyToParentTransform,
	VS_getSourceBoneTransform,
	VS_setupFixedCamera,
	VS_restoreCameraView,
	VS_render2DTexturedQuad,
	VS_renderWireframeGrid,
	VS_discardEFEOperand,
	VS_renderWireframeBox,
	VS_setTransformToBoneMatrix,
	VS_render3DTexturedQuad,
	VS_multiplyVectorByScalar,
	VS_divideVectorByScalar,
	VS_maskVectorByScalar,
	VS_convertToViewSpace,
	VS_selectRandomTargetEntity,
	VS_getCameraRotation,
	VS_drawTMDYXZ,
	VS_loadClutColors,
	VS_drawTMDScreenSpace,
	VS_addClutLoadPrim,
	VS_getViewportDistance,
	VS_renderRadialWaves,
	VS_initializeRibbonPoints,
	VS_tickRibbonPoints,
	VS_renderRibbonStrip,
	VS_renderRingTube,
	VS_renderScreenOverlay,
	VS_faceTargetEntity,
	VS_applyLineAttackHit,
	VS_applyRadiusAttackHit,
	VS_applyBoxAttackHit,
	VS_renderScreenFade,
	VS_disableMapLayer,
	VS_getViewportDistance2,
	VS_markEFEFinished,
	VS_isTargetUnhit,
	VS_getEFEFileId,
	VS_getEFEModelSection,
	VS_getEFETextureSection,
	VS_getEFEHeapPointer,
	VS_stopEFESubEffect,
	VS_startEFE,
	VS_unloadAllEFESlots,
	VS_loadMoveEFE,
	VS_removeEFEEngine,
	VS_initializeEFEEngine,
	VS_offsetEFEPrimitiveUVs,
	VS_renderParticleEmitters,
	VS_tickParticleEmitters,
	VS_tickEFEUVAnimation,
	VS_handleEFEFileLoaded,
	VS_setupLoadedEFEFile,
	VS_runEFESlotScript,
	VS_unloadEFESlot,
	VS_loadNextEFEFile,
	VS_stopEFESounds,
	VS_clearEFESoundChannels,
	VS_renderEFEEngine,
	VS_tickEFEEngine,
	VS_initializeParticleEmitters,
};

char *VS_initializeParticleEmitters(char *base)
{
	int32_t i;
	int32_t off;

	MAIN_D_80134CCC = base;
	for (i = 0, off = 0; i < 4; i++) {
		*(int32_t *)(MAIN_D_80134CCC + off) = 0;
		off += 0x1b4;
	}

	return base + 0x23c4;
}

void VS_tickEFEEngine(void)
{
	int32_t i;
	int32_t j;
	int32_t n;

	int32_t (*p)[2];
	int32_t v;
	int32_t t;

	setMapLayerEnabled(1);

	MAIN_D_80134CD4 = 0;
	while (MAIN_D_80134CD4 < 0x10) {
		if (MAIN_D_80139AB0[MAIN_D_80134CD4] == 0) {
			MAIN_D_80139AB0[MAIN_D_80134CD4] = 1;
		}
		MAIN_D_80134CD4++;
	}

	MAIN_D_80134CD4 = 0;
	MAIN_D_80134D0C = (int32_t *)MAIN_D_80134D10;
	while (MAIN_D_80134CD4 < 0x10) {
		t = MAIN_D_80134D0C[2];
		if (t != 0) {
			MAIN_D_80134D08 = t;
			n = ((int16_t *)MAIN_D_80134D0C)[10];
			MAIN_D_80134CD0 = 0;
			while (MAIN_D_80134CD0 < n) {
				if (MAIN_D_80134D0C == NULL) {
				}
				MAIN_D_80134CE8 = (EfeSubEffect *)((int32_t)((uint32_t)(int32_t)((uint32_t)(char *)(MAIN_D_80134CD0 * 28)) + (int32_t)MAIN_D_80134D0C[4]));
				if ((int32_t)MAIN_D_80134CE8->inst != 0) {
					EFE_INSTANCE = MAIN_D_80134CE8->instance;
					EFE_PARENT_INSTANCE = (int32_t)MAIN_D_80134CE8->parentInstance;
					EFE_INSTANCE->frame = EFE_INSTANCE->frame + 1;
					VS_runEFEScript((int32_t)MAIN_D_80134CE8->inst);
				}
				MAIN_D_80134CD0++;
			}
			n = MAIN_D_80134D0C[7];
			for (j = 0; j < n; j++) {
				VS_tickEFEUVAnimation(j);
			}
		}
		MAIN_D_80134D0C = (int32_t *)((char *)MAIN_D_80134D0C + 0x28);
		MAIN_D_80134CD4++;
	}

	VS_tickParticleEmitters();

	p = MAIN_D_80139AD0;
	for (i = 0; i < 0xa; i++) {
		v = (*p)[0];
		if ((v >= 0) && (**(int32_t **)((char *)(*p)[1] + 8) < 0)) {
			stopSoundMask(v);
			(*p)[0] = -1;
		}
		p++;
	}
}

void VS_renderEFEEngine(void)
{
	int32_t i;
	int32_t j;
	int32_t n;
	int32_t off;
	int32_t off2;
	int32_t t;

	MAIN_D_80134D0C = (int32_t *)MAIN_D_80134D10;
	for (i = 0; i < 0x10; i++) {
		n = MAIN_D_80134D0C[7];
		for (j = 0, off = 0; j < n; j++, off += 0x20) {
			*(int16_t *)((char *)off + MAIN_D_80134D0C[8] + 2) = 0;
		}
		t = MAIN_D_80134D0C[2];
		if (t != 0) {
			MAIN_D_80134D08 = t;
			n = ((int16_t *)MAIN_D_80134D0C)[10];
			for (j = 0, off2 = 0; j < n; j++, off2 += 0x1c) {
				MAIN_D_80134CE8 = (EfeSubEffect *)((char *)MAIN_D_80134D0C[4] + off2);
				if ((int32_t)MAIN_D_80134CE8->inst != 0) {
					EFE_INSTANCE = MAIN_D_80134CE8->instance;
					EFE_PARENT_INSTANCE = (int32_t)MAIN_D_80134CE8->parentInstance;
					VS_runEFEScript((int32_t)MAIN_D_80134CE8->someInst);
				}
			}
		}
		MAIN_D_80134D0C = (int32_t *)((char *)MAIN_D_80134D0C + 0x28);
	}

	VS_renderParticleEmitters();
}

void VS_clearEFESoundChannels(void)
{
	int32_t i;

	for (i = 0; i < 0xa; i++) {
		MAIN_D_80139AD0[i][0] = -1;
	}
}

void VS_stopEFESounds(void)
{
	int32_t i;
	int32_t *p;
	int32_t v;

	p = &MAIN_D_80139AD0[0][0];
	for (i = 0; i < 10; i++) {
		v = *p;
		if (v >= 0) {
			stopSoundMask(v);
			*p = -1;
		}
	}
}

char *VS_getEFEHeapPointer(void)
{
	return (char *)MAIN_D_80134D14;
}

void VS_loadNextEFEFile(int16_t *arg)
{
	EfeLoad *p;
	ModelComponent *m;
	int32_t id;
	int32_t i;
	char path[32];
	CdlLOC loc;

	p = (EfeLoad *)(int32_t)arg;
	p->state++;
	id = *p->moves++;
	if (id < 0) {
		*p->isLoaded = 0;
		return;
	}

	id -= 0x100;
	if ((id < 0) || (id >= 0x17a)) {
		*p->isLoaded = -4;
		return;
	}

	for (m = UNKNOWN_MODEL, i = 0; i < 16; m++, i++) {
		if (m->useCount == 0) {
			break;
		}
	}

	if (i == 16) {
		*p->isLoaded = -5;
		return;
	}

	m->useCount = id + 0x100;
	m->mmdPtr = (void *)VS_getEFEHeapPointer();
	p->model = m;
	loc = *getEFEDATEntry(id + 0x100);
	addFileReadRequest(path, (uint8_t *)m->mmdPtr, NULL, (void *)VS_handleEFEFileLoaded, arg, &loc, 0x5000);
}

void VS_unloadEFESlot(int32_t idx)
{
	int32_t i;
	int32_t n;
	int16_t *p;
	int32_t off;

	*(char **)&MAIN_D_80134D0C = MAIN_D_80134D10 + (idx * 40);
	n = MAIN_D_80134D0C[7];
	for (i = 0, off = 0; i < n; i++, off += 0x20) {
		p = (int16_t *)((char *)MAIN_D_80134D0C[8] + off);
		if (p[0] != 0) {
			p[0] = 0;
			p[6] = 0;
		}
	}

	unloadModel(MAIN_D_80134D0C[0], 1);
	MAIN_D_80134D0C[2] = 0;
}

void VS_runEFESlotScript(int32_t idx)
{
	EFE_INSTANCE = NULL;

	*(char **)&MAIN_D_80134D0C = MAIN_D_80134D10 + (idx * 40);
	MAIN_D_80134D08 = MAIN_D_80134D0C[2];
	MAIN_D_80134CE8 = (EfeSubEffect *)MAIN_D_80134D0C[4];
	MAIN_D_80134CE4 = -1;
	VS_runEFEScript(((int32_t *)(MAIN_D_80134D10 + (idx * 40)))[3]);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_setupLoadedEFEFile);

void VS_handleEFEFileLoaded(int32_t arg)
{
	MAIN_D_801352E4 = 0;
	setFileReadCallback2(VS_setupLoadedEFEFile, arg);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_tickEFEUVAnimation);

void VS_tickParticleEmitters(void)
{
	SVECTOR rot;
	MATRIX m;
	char *p;
	char *owner;
	int32_t i;
	int16_t *q;
	int32_t k;
	int32_t t;

	p = MAIN_D_80134CCC;
	for (i = 0; i < 4; p = (char *)((int32_t)((uint32_t)p + 0x1b4)), i++) {
		if (*(char **)p == NULL) {
			continue;
		}
		owner = *(char **)p;
		if (*(int32_t *)owner == -1) {
			*(int32_t *)p = 0;
			continue;
		}

		k = 0;
		while (1) {
			if (((int16_t (*)[10])p)[k][8] <= 0) {
				break;
			}
			k++;
		}

		if (k != 0x14) {
			q = &((int16_t (*)[10])p)[k][8];
			q[0] = ((int16_t *)p)[3];
			q[1] = ((int16_t *)p)[4];
			rot.vx = (((rand() & 0x7f) - 0x40) << 12) / 64;
			rot.vy = (((rand() & 0x7f) - 0x40) << 12) / 64;
			rot.vz = (((rand() & 0x7f) - 0x40) << 12) / 64;
			q[3] = 0;
			q[2] = 0;
			q[4] = ((int16_t *)p)[3];
			RotMatrixZYX(&rot, &m);
			ApplyMatrixSV(&m, (SVECTOR *)&q[2], (SVECTOR *)&q[2]);
			q[6] = *(int32_t *)((int32_t)((uint32_t)(char *)owner + 4));
			q[7] = *(int32_t *)((int32_t)((uint32_t)(char *)owner + 8));
			q[8] = *(int32_t *)((int32_t)((uint32_t)(char *)owner + 0xc));
		}

		for (k = 0; k < 0x14; k++) {
			if (((int16_t (*)[10])p)[k][8] > 0) {
				q = &((int16_t (*)[10])p)[k][8];
				q[0] = q[0] - q[1];
				q[1] -= ((int16_t *)p)[5];
			}
		}

		t = ((int16_t *)p)[2] - 1;
		((int16_t *)p)[2] = t;
		if ((int16_t)t <= 0) {
			*(int32_t *)p = 0;
		}
	}
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_renderParticleEmitters);

int16_t VS_offsetEFEPrimitiveUVs(char *base, int32_t idx, int32_t du, int32_t dv)
{
	uint8_t code;
	char *p;
	int32_t i;
	int32_t n;

	base = (char *)((int32_t)base + 0xc + (idx * 28));
	n = ((int32_t *)base)[5];
	p = ((char **)base)[4];
	for (i = 0; i < n; i++) {
		code = (*(int32_t *)p >> 24) & 0xff;
		if (code & 1) {
			goto setpath;
		}
		if (code & 4) {
			p[4] = p[4] + du;
			p[5] = p[5] + dv;
			p[8] = p[8] + du;
			p[9] = p[9] + dv;
			p[0xc] = p[0xc] + du;
			p[0xd] = p[0xd] + dv;
			if (code & 8) {
				p[0x10] = p[0x10] + du;
				p[0x11] = p[0x11] + dv;
				if (code & 0x10) {
					goto s1;
				}
				p += 0x20;
				goto next;
s1:
				p += 0x24;
				goto next;
			} else {
				if (code & 0x10) {
					goto s2;
				}
				p += 0x18;
				goto next;
s2:
				p += 0x1c;
				goto next;
			}
		}
		goto next;
setpath:
		if (code & 4) {
			p[4] = p[4] + du;
			p[5] = p[5] + dv;
			p[8] = p[8] + du;
			p[9] = p[9] + dv;
			p[0xc] = p[0xc] + du;
			p[0xd] = p[0xd] + dv;
			if (code & 8) {
				p[0x10] = p[0x10] + du;
				p[0x11] = p[0x11] + dv;
				if (code & 0x10) {
					goto s3;
				}
				p += 0x20;
				goto next;
s3:
				p += 0x2c;
				goto next;
			} else {
				if (code & 0x10) {
					goto s4;
				}
				p += 0x1c;
				goto next;
s4:
				p += 0x24;
				goto next;
			}
		}
next:;
	}
}

char *VS_initializeEFEEngine(char *base)
{
	int32_t i;
	int32_t off;

	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	MAIN_D_80134D10 = base;
	for (i = 0, off = 0; i < 0x10; i++, off += 0x28) {
		*(int32_t *)((char *)off + (int32_t)MAIN_D_80134D10 + 8) = 0;
		*(int32_t *)((char *)off + (int32_t)MAIN_D_80134D10 + 0x1c) = 0;
	}

	base = (char *)((int32_t)base + 0x280);
	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	base = VS_initializeParticleEmitters(base);
	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	base = initializeFlashData(base);
	base = VS_initializeAuraProjectiles(base);
	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	MAIN_D_80134D18 = (int32_t)base;
	MAIN_D_80134D14 = (int32_t)base;
	addObject(0x500, 0, VS_tickEFEEngine, VS_renderEFEEngine);
	VS_initializeEFESubOpcodeTable();
	base = (char *)((int32_t)base + 0x41000);
	VS_clearEFESoundChannels();

	return base;
}

void VS_removeEFEEngine(void)
{
	VS_removeAllAuraProjectiles();
	VS_stopEFESounds();
	removeObject(0x500, 0);
}

void VS_loadMoveEFE(int16_t *moves, int16_t *effectIds, int8_t *isLoaded)
{
	downloadSomeImage(moves);
	MAIN_D_80139B20[0] = -1;
	MAIN_D_80139B24[0] = isLoaded;
	*MAIN_D_80139B24[0] = 1;
	MAIN_D_80139B28[0] = (int32_t)moves;
	MAIN_D_80139B2C[0] = (int32_t)effectIds;
	VS_loadNextEFEFile(MAIN_D_80139B20);
}

void VS_unloadAllEFESlots(void)
{
	int32_t i;

	for (i = 0; i < 0x10; i++) {
		if (EFE_LOADED_MOVE_DATA[i] != -1) {
			VS_unloadEFESlot(i);
			EFE_LOADED_MOVE_DATA[i] = -1;
		}
	}

	MAIN_D_80134D14 = MAIN_D_80134D18;
}

int32_t VS_startEFE(int32_t i)
{
	if ((i < 0) || (i >= 0x10)) {
		return -1;
	}

	if (EFE_LOADED_MOVE_DATA[i] < 0) {
		return -1;
	}

	MAIN_D_80139AB0[i] = -1;
	VS_runEFESlotScript(i);
}

void VS_stopEFESubEffect(int32_t a, int32_t b)
{
	char *p;
	char *q;

	if ((b < 0) || (a < 0) || (a >= 0x10)) {
		return;
	}

	if (MAIN_D_80134D10 != NULL) {
	}

	p = (char *)(a * 40) + (int32_t)MAIN_D_80134D10;
	q = ((char **)p)[4];
	MAIN_D_80134CE8 = (EfeSubEffect *)(q + (b * 28));
	MAIN_D_80134CE8->inst = NULL;
	MAIN_D_80134CE8->instance->frame = -1;
}

char *VS_getEFETextureSection(char *p)
{
	int32_t a;
	int32_t b;
	char *base;

	base = p;
	p = (char *)((uint32_t)p + 0x34);
	a = ((int32_t *)base)[7];
	b = ((int32_t *)base)[9];
	if ((b - a) == 0) {
		return NULL;
	}

	return p + ((int32_t *)base)[7];
}

char *VS_getEFEModelSection(char *p)
{
	int32_t a;
	int32_t b;
	char *base;

	base = p;
	p = (char *)((uint32_t)p + 0x34);
	a = ((int32_t *)base)[5];
	b = ((int32_t *)base)[6];
	if ((b - a) == 0) {
		return NULL;
	}

	return p + ((int32_t *)base)[5];
}

int32_t VS_getEFEFileId(char *p)
{
	return *(int32_t *)(p + 0x30);
}

void VS_isTargetUnhit(void)
{
	int32_t *out;

	out = EFE_POP1(int32_t *);
	if (((int8_t *)(int32_t)MAIN_D_80134CE8->targetEntity)[0x53] == 0) {
		*out = 1;
	} else {
		*out = 0;
	}
}

void VS_markEFEFinished(void)
{
	MAIN_D_80139AB0[MAIN_D_80134CD4] = 0;
}

void VS_getViewportDistance2(void)
{
	int32_t *out;

	out = EFE_POP1(int32_t *);
	*out = VIEWPORT_DISTANCE;
}

void VS_disableMapLayer(void)
{
	setMapLayerEnabled(0);
}

void VS_renderScreenFade(void)
{
	modifySomeImage(EFE_POP1(int32_t));
}

void VS_applyBoxAttackHit(void)
{
	SVECTOR center;
	AABB box;
	int32_t r;
	int32_t *out;
	int32_t *ext;
	int32_t id;
	int32_t idx;
	int32_t j;
	Entity *e;
	int32_t ent;

	r = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	ext = EFE_POP1(int32_t *);
	*out = 0;
	if (r < 0) {
		r = MAIN_D_80134CD0;
	}

	center.vx = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	center.vy = *(int32_t *)((int32_t)EFE_INSTANCE + 8);
	center.vz = *(int32_t *)((int32_t)EFE_INSTANCE + 0xc);
	box.center = &center;
	box.extent.vx = ext[0];
	box.extent.vy = ext[1];
	box.extent.vz = ext[2];
	MAIN_D_80134CD8 = 1;
	while (1) {
		id = findAABBHitEntity(&box, MAIN_D_80134CE8->sourceEntity, MAIN_D_80134CD8);
		MAIN_D_80134CD8 = id;
		if (id == -1) {
			return;
		}
		idx = (*(int32_t *)&MAIN_D_80134CD8);
		e = ENTITY_TABLE[idx];
		if (((int8_t *)e)[0x53] == 0) {
			for (j = 1; j < 10; j++) {
				ent = (int32_t)ENTITY_TABLE[j];
				if (ent == (int32_t)MAIN_D_80134CE8->sourceEntity) {
					break;
				}
			}
			((int8_t *)ENTITY_TABLE[idx])[0x53] = 1;

			addAttackObject(MAIN_D_80134CD8, 1, (int16_t *)&center, (int32_t)((uint32_t)(char *)MAIN_D_80134CD4), r, j);
			*out = 1;
			return;
		}
		MAIN_D_80134CD8 = idx + 1;
	}
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_applyRadiusAttackHit);

void VS_applyLineAttackHit(void)
{
	DVECTOR line[2];
	int16_t rect[4];
	SVECTOR pos;
	int32_t *out;
	int32_t *arg;
	int32_t j;
	int32_t r;
	Entity *e;
	int32_t ent;

	EFE_DROP1();
	out = EFE_POP1(int32_t *);
	arg = EFE_POP1(int32_t *);
	*out = 0;
	line[0].vx = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	line[0].vy = *(int32_t *)((int32_t)EFE_INSTANCE + 0xc);
	line[1].vx = *(int32_t *)((char *)((int32_t *)(int32_t)MAIN_D_80134CE8->sourceEntity)[1] + 0x78);
	line[1].vy = *(int32_t *)((char *)((int32_t *)(int32_t)MAIN_D_80134CE8->sourceEntity)[1] + 0x80);
	for (MAIN_D_80134CD8 = 1; MAIN_D_80134CD8 < 10; MAIN_D_80134CD8++) {
		e = ENTITY_TABLE[(*(int32_t *)&MAIN_D_80134CD8)];
		if (e == MAIN_D_80134CE8->sourceEntity) {
			continue;
		}
		if (e == NULL) {
			continue;
		}
		if (((int8_t *)e)[0x53] != 0) {
			continue;
		}
		if (((int8_t *)e)[0x34] == 0) {
			continue;
		}
		r = DIGIMON_DATA[*(int32_t *)e].radius;
		rect[0] = *(int32_t *)((char *)((int32_t *)e)[1] + 0x78) - r;
		rect[2] = *(int32_t *)((char *)((int32_t *)e)[1] + 0x78) + r;
		rect[1] = *(int32_t *)((char *)((int32_t *)e)[1] + 0x80) - r;
		rect[3] = *(int32_t *)((char *)((int32_t *)e)[1] + 0x80) + r;
		if (MAIN_func_800DA740(rect, line) != -1) {
			continue;
		}
		if (*(int32_t *)((int32_t)EFE_INSTANCE + 8) < (-DIGIMON_DATA[*(int32_t *)e].height - arg[1])) {
			continue;
		}
		for (j = 1; j < 10; j++) {
			ent = (int32_t)ENTITY_TABLE[j];
			if (ent == (int32_t)MAIN_D_80134CE8->sourceEntity) {
				break;
			}
		}
		((int8_t *)ENTITY_TABLE[MAIN_D_80134CD8])[0x53] = 1;
		VS_calculateAttackHitPosition(&pos, (int32_t *)e, ((int32_t **)MAIN_D_80134CE8)[4], DIGIMON_DATA[*(int32_t *)e].radius);
		pos.vy = *(int32_t *)((int32_t)EFE_INSTANCE + 8);
		addAttackObject(MAIN_D_80134CD8, 1, (int16_t *)&pos, (int32_t)((uint32_t)(char *)MAIN_D_80134CD4), MAIN_D_80134CD0, j);
		*out = 1;
		return;
	}
}

void VS_faceTargetEntity(void)
{
	PositionData *pd;
	Entity *ent;

	if (MAIN_D_80134CE8->targetEntity == NULL) {
		return;
	}

	ent = MAIN_D_80134CE8->targetEntity;
	entityLookAtLocation(MAIN_D_80134CE8->sourceEntity, &ent->posData->location);
	pd = MAIN_D_80134CE8->sourceEntity->posData;
	RotMatrix(&pd->rotation, &pd->posMatrix.coord);
	ScaleMatrix(&pd->posMatrix.coord, &pd->scale);
	TransMatrix(&pd->posMatrix.coord, &pd->location);
	pd->posMatrix.flg = 0;
}

void VS_renderScreenOverlay(void)
{
	POLY_F4 *p;
	DR_TPAGE *dr;
	int32_t *col;
	int32_t mode;
	int32_t layer;

	mode = EFE_POP1(int32_t);
	col = EFE_POP1(int32_t *);
	layer = EFE_POP1(int32_t);

	p = (POLY_F4 *)GsGetWorkBase();
	setXY4(p, -0xa0, -0x78, 0xa0, -0x78, -0xa0, 0x78, 0xa0, 0x78);
	setRGB0(p, col[0], col[1], col[2]);
	setPolyF4(p);
	setSemiTrans(p, mode >> 2);
	setShadeTex(p, 0);
	addPrim(ACTIVE_ORDERING_TABLE->org + layer, p);
	GsSetWorkBase((PACKET *)(p + 1));

	dr = (DR_TPAGE *)GsGetWorkBase();
	setDrawTPage(dr, 1, 1, getTPage(0, mode & 3, 0, 0));
	addPrim(ACTIVE_ORDERING_TABLE->org + layer, dr);
	GsSetWorkBase((PACKET *)(dr + 1));
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_renderRingTube);

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_renderRibbonStrip);

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_tickRibbonPoints);

void VS_initializeRibbonPoints(void)
{
	SVECTOR *q;
	int32_t i;
	int32_t v;

	q = EFE_POP1(SVECTOR *);
	for (i = 0, v = -0x708; i < 10; i++, v += 0x190) {
		q[i].vx = v;
		q[i].vy = customRandom(-0x96, 0x96);
		q[i].pad = customRandom(-0x1e, 0x1e);
		q[i].vz = 0;
	}
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_renderRadialWaves);

void VS_getViewportDistance(void)
{
	int32_t *out;

	out = EFE_POP1(int32_t *);
	*out = VIEWPORT_DISTANCE;
}

void VS_addClutLoadPrim(void)
{
	RECT rect;
	int32_t idx;
	char *src;
	int32_t y;
	int32_t z;
	PACKET *prim;

	idx = EFE_POP1(int32_t);
	src = EFE_POP1(char *);
	y = EFE_POP1(int32_t);
	z = EFE_POP1(int32_t);
	src += idx * 2;
	prim = GsGetWorkBase();
	GsSetWorkBase(prim + 0x44);
	rect.x = (((uint16_t *)MAIN_D_80134D0C[6])[9] & 0x3f) << 4;
	rect.y = ((((uint16_t *)MAIN_D_80134D0C[6])[9] >> 6) & 0x1ff) + y;
	rect.w = 0x10;
	rect.h = 1;
	MAIN_func_80092C18(prim, &rect);
	memcpy(prim + 0x10, src, 0x20);
	AddPrim(ACTIVE_ORDERING_TABLE->org + (z >> 4), prim);
}

void VS_drawTMDScreenSpace(void)
{
	EFE_SCRATCH->scale = EFE_POP1(VECTOR *);
	EFE_SCRATCH->id = EFE_POP1(int32_t);
	EFE_SCRATCH->rot.vx = ((VECTOR *)((int32_t)EFE_INSTANCE + 0x10))->vx;
	EFE_SCRATCH->rot.vy = ((VECTOR *)((int32_t)EFE_INSTANCE + 0x10))->vy;
	EFE_SCRATCH->rot.vz = ((VECTOR *)((int32_t)EFE_INSTANCE + 0x10))->vz;
	RotMatrixYXZ(&EFE_SCRATCH->rot, &EFE_SCRATCH->m0);
	ScaleMatrix(&EFE_SCRATCH->m0, EFE_SCRATCH->scale);
	EFE_SCRATCH->m0.t[0] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	EFE_SCRATCH->m0.t[1] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[1];
	EFE_SCRATCH->m0.t[2] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[2];
	EFE_SCRATCH->m0.t[0] += -DRAWING_OFFSET_X + 0xa0;
	EFE_SCRATCH->m0.t[1] += -DRAWING_OFFSET_Y + 0x78;
	TransposeMatrix(&GsWSMATRIX, &EFE_SCRATCH->m2);
	MulMatrix0(&EFE_SCRATCH->m2, &EFE_SCRATCH->m0, &EFE_SCRATCH->m1);
	GsSetLightMatrix(&EFE_SCRATCH->m1);
	GsSetLsMatrix(&EFE_SCRATCH->m0);
	GsLinkObject4((unsigned long)(((char **)MAIN_D_80134D0C[6])[1] + 0xc), &EFE_SCRATCH->obj, EFE_SCRATCH->id);
	EFE_SCRATCH->obj.coord2 = NULL;
	EFE_SCRATCH->obj.attribute = 0;
	GsSortObject4(&EFE_SCRATCH->obj, ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0x2c));
}

void VS_loadClutColors(void)
{
	uint16_t clut[16];
	RECT rect;
	int32_t y;
	int32_t x;
	int32_t count;
	uint16_t *src;
	ModelComponent *m;
	int32_t i;

	src = EFE_POP1(uint16_t *);
	count = EFE_POP1(int32_t);
	x = EFE_POP1(int32_t);
	y = EFE_POP1(int32_t);

	if ((count > 0) && (count <= 16)) {
		m = (ModelComponent *)MAIN_D_80134D0C[6];
		for (i = 0; i < count; i++) {
			clut[i] = *src++;
			clut[i] += *src++ << 5;
			clut[i] += *src++ << 10;
			clut[i] += *src++ << 15;
		}
		rect.x = ((m->clutPage & 0x3f) << 4) + x;
		rect.y = (m->clutPage >> 6) + y;
		rect.w = count;
		rect.h = 1;
		LoadImage(&rect, (u_long *)clut);
		DrawSync(0);
	}
}

void VS_drawTMDYXZ(void)
{
	typedef struct {
		int32_t rotation[3];
	} EfeRot;

	EFE_SCRATCH->scale = EFE_POP1(VECTOR *);
	EFE_SCRATCH->id = EFE_POP1(int32_t);
	EFE_SCRATCH->rot.vx = ((EfeRot *)((int32_t)EFE_INSTANCE + 0x10))->rotation[0];
	EFE_SCRATCH->rot.vy = ((EfeRot *)((int32_t)EFE_INSTANCE + 0x10))->rotation[1];
	EFE_SCRATCH->rot.vz = ((EfeRot *)((int32_t)EFE_INSTANCE + 0x10))->rotation[2];
	RotMatrixYXZ(&EFE_SCRATCH->rot, &EFE_SCRATCH->m1);
	ScaleMatrix(&EFE_SCRATCH->m1, EFE_SCRATCH->scale);
	EFE_SCRATCH->m1.t[0] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	EFE_SCRATCH->m1.t[1] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[1];
	EFE_SCRATCH->m1.t[2] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[2];
	GsMulCoord0(&GsWSMATRIX, &EFE_SCRATCH->m1, &EFE_SCRATCH->m0);
	if (EFE_SCRATCH->m0.t[2] < -0x12c) {
		return;
	}

	if (EFE_SCRATCH->m0.t[2] >= 0x10000) {
		return;
	}

	GsSetLightMatrix(&EFE_SCRATCH->m1);
	GsSetLsMatrix(&EFE_SCRATCH->m0);
	GsLinkObject4((unsigned long)(((char **)MAIN_D_80134D0C[6])[1] + 0xc), &EFE_SCRATCH->obj, EFE_SCRATCH->id);
	EFE_SCRATCH->obj.coord2 = NULL;
	EFE_SCRATCH->obj.attribute = 0;
	GsSortObject4(&EFE_SCRATCH->obj, ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0x2c));
}

void VS_getCameraRotation(void)
{
	MATRIX m;
	SVECTOR rot;
	int32_t *out;

	out = EFE_POP1(int32_t *);
	TransposeMatrix(&GsWSMATRIX, &m);
	matrixToEuler2(&m, &rot);
	out[0] = rot.vx;
	out[1] = rot.vy;
	out[2] = rot.vz;
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_selectRandomTargetEntity);

void VS_convertToViewSpace(void)
{
	MATRIX m1;
	MATRIX m2;
	SVECTOR rot;
	VECTOR *rotIn;
	VECTOR *trans;
	VECTOR *out2;
	VECTOR *out1;

	rotIn = EFE_POP1(VECTOR *);
	trans = EFE_POP1(VECTOR *);
	out2 = EFE_POP1(VECTOR *);
	out1 = EFE_POP1(VECTOR *);
	rot.vx = rotIn->vx;
	rot.vy = rotIn->vy;
	rot.vz = rotIn->vz;
	((VECTOR *)m1.t)->vx = trans->vx;
	((VECTOR *)m1.t)->vy = trans->vy;
	((VECTOR *)m1.t)->vz = trans->vz;
	RotMatrixYXZ(&rot, &m1);
	GsMulCoord0(&GsWSMATRIX, &m1, &m2);
	matrixToEuler2(&m2, &rot);
	out1->vx = ((VECTOR *)m2.t)->vx;
	out1->vy = ((VECTOR *)m2.t)->vy;
	out1->vz = ((VECTOR *)m2.t)->vz;
	out2->vx = rot.vx;
	out2->vy = rot.vy;
	out2->vz = rot.vz;
}

void VS_maskVectorByScalar(void)
{
	int32_t mask;
	int32_t *v;

	mask = EFE_POP1(int32_t);
	v = EFE_POP1(int32_t *);
	v[0] &= mask;
	v[1] &= mask;
	v[2] &= mask;
}

void VS_divideVectorByScalar(void)
{
	int32_t k;
	int32_t *v;

	k = EFE_POP1(int32_t);
	v = EFE_POP1(int32_t *);
	v[0] /= k;
	v[1] /= k;
	v[2] /= k;
}

void VS_multiplyVectorByScalar(void)
{
	int32_t k;
	int32_t *v;

	k = EFE_POP1(int32_t);
	v = EFE_POP1(int32_t *);
	v[0] *= k;
	v[1] *= k;
	v[2] *= k;
}

void VS_render3DTexturedQuad(void)
{
	SVECTOR a;
	SVECTOR b;
	SVECTOR c;
	SVECTOR d;
	POLY_FT4 *prim;
	ModelComponent *m;
	int32_t u0off;
	int32_t v0off;
	int32_t *col;
	int32_t semi;
	int32_t du;
	int32_t dv;
	int32_t clutY;
	int32_t *p1;
	int32_t *p2;
	int32_t *p3;
	int32_t *p4;

	semi = EFE_POP1(int32_t);
	clutY = EFE_POP1(int32_t);
	dv = EFE_POP1(int32_t);
	du = EFE_POP1(int32_t);
	v0off = EFE_POP1(int32_t);
	u0off = EFE_POP1(int32_t);
	col = EFE_POP1(int32_t *);
	p4 = EFE_POP1(int32_t *);
	p3 = EFE_POP1(int32_t *);
	p2 = EFE_POP1(int32_t *);
	p1 = EFE_POP1(int32_t *);
	m = (ModelComponent *)MAIN_D_80134D0C[6];

	a.vx = p1[0];
	a.vy = p1[1];
	a.vz = p1[2];
	b.vx = p2[0];
	b.vy = p2[1];
	b.vz = p2[2];
	c.vx = p3[0];
	c.vy = p3[1];
	c.vz = p3[2];
	d.vx = p4[0];
	d.vy = p4[1];
	d.vz = p4[2];

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	if (semi != 0) {
		setSemiTrans(prim, 1);
	}

	prim->r0 = col[0];
	prim->g0 = col[1];
	prim->b0 = col[2];
	prim->tpage = m->pixelPage | semi;
	prim->clut = GetClut((m->clutPage & 0x3f) << 4, (m->clutPage >> 6) + clutY);
	prim->u0 = m->pixelOffsetX + u0off;
	prim->v0 = m->pixelOffsetY + v0off;
	prim->u1 = du + (m->pixelOffsetX + u0off);
	prim->v1 = m->pixelOffsetY + v0off;
	prim->u2 = m->pixelOffsetX + u0off;
	prim->v2 = dv + (m->pixelOffsetY + v0off);
	prim->u3 = du + (m->pixelOffsetX + u0off);
	prim->v3 = dv + (m->pixelOffsetY + v0off);
	add3DSpritePrim(prim, &a, &b, &c, &d);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_setTransformToBoneMatrix);

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_renderWireframeBox);

void VS_discardEFEOperand(void)
{
	EFE_DROP1();
}

void VS_renderWireframeGrid(void)
{
	SVECTOR c0;
	SVECTOR c1;
	SVECTOR c2;
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
	MATRIX m;
	SVECTOR rot;
	int32_t *base;
	int32_t *rotSrc;
	int32_t y;
	int32_t *col;
	int32_t x1;
	int32_t x2;
	int32_t z1;
	int32_t z2;
	int32_t n1;
	int32_t n2;

	y = EFE_POP1(int32_t);
	n2 = EFE_POP1(int32_t);
	z2 = EFE_POP1(int32_t);
	z1 = EFE_POP1(int32_t);
	n1 = EFE_POP1(int32_t);
	x2 = EFE_POP1(int32_t);
	x1 = EFE_POP1(int32_t);
	col = EFE_POP1(int32_t *);
	rotSrc = EFE_POP1(int32_t *);
	base = EFE_POP1(int32_t *);

	PushMatrix();
	rot.vx = rotSrc[0];
	rot.vy = rotSrc[1];
	rot.vz = rotSrc[2];
	RotMatrixZYX(&rot, &m);
	p0.vx = x1;
	p0.vy = y;
	p0.vz = z1;
	ApplyMatrixSV(&m, &p0, &p0);
	p1.vx = x2;
	p1.vy = y;
	p1.vz = z1;
	ApplyMatrixSV(&m, &p1, &p1);
	p2.vx = x1;
	p2.vy = y;
	p2.vz = z2;
	ApplyMatrixSV(&m, &p2, &p2);
	p3.vx = x2;
	p3.vy = y;
	p3.vz = z2;
	ApplyMatrixSV(&m, &p3, &p3);
	PopMatrix();

	c0.vx = c2.vx = base[0] + p0.vx;
	c0.vy = c2.vy = base[1] + p0.vy;
	c0.vz = c2.vz = base[2] + p0.vz;
	c1.vx = base[0] + p2.vx;
	c1.vy = base[1] + p2.vy;
	c1.vz = base[2] + p2.vz;
	VS_renderParallelLines(&c0, &c1, (int16_t)n1, &p0, &p1, col);
	c1.vx = base[0] + p1.vx;
	c1.vy = base[1] + p1.vy;
	c1.vz = base[2] + p1.vz;
	VS_renderParallelLines(&c2, &c1, (int16_t)n2, &p0, &p2, col);
}

void VS_render2DTexturedQuad(void)
{
	int32_t depth;
	int32_t *col;
	int32_t du;
	int32_t dv;
	int32_t clutY;
	int32_t a;
	int32_t b;
	POLY_FT4 *prim;
	ModelComponent *m;
	int32_t semi;
	int32_t u0off;
	int32_t v0off;
	int32_t c;
	int32_t d;

	semi = EFE_POP1(int32_t);
	clutY = EFE_POP1(int32_t);
	dv = EFE_POP1(int32_t);
	du = EFE_POP1(int32_t);
	v0off = EFE_POP1(int32_t);
	u0off = EFE_POP1(int32_t);
	col = EFE_POP1(int32_t *);
	depth = EFE_POP1(int32_t);
	d = EFE_POP1(int32_t);
	c = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t);
	a = EFE_POP1(int32_t);

	if ((depth > 0x20) && (depth < 0x1000)) {
		m = (ModelComponent *)MAIN_D_80134D0C[6];
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		if (semi < 0) {
			semi = -semi & 0xffff;
			prim->x0 = ((int32_t *)a)[0];
			prim->y0 = ((int32_t *)a)[1];
			prim->x1 = ((int32_t *)b)[0];
			prim->y1 = ((int32_t *)b)[1];
			prim->x2 = ((int32_t *)c)[0];
			prim->y2 = ((int32_t *)c)[1];
			prim->x3 = ((int32_t *)d)[0];
			prim->y3 = ((int32_t *)d)[1];
		} else {
			prim->x0 = a;
			prim->y0 = b;
			prim->x1 = a + c;
			prim->y1 = b;
			prim->x2 = a;
			prim->y2 = b + d;
			prim->x3 = a + c;
			prim->y3 = b + d;
		}
		if (semi != 0) {
			setSemiTrans(prim, 1);
		}
		prim->r0 = col[0];
		prim->g0 = col[1];
		prim->b0 = col[2];
		prim->tpage = m->pixelPage | semi;
		prim->clut = GetClut((m->clutPage & 0x3f) << 4, (m->clutPage >> 6) + clutY);
		prim->u0 = m->pixelOffsetX + u0off;
		prim->v0 = m->pixelOffsetY + v0off;
		prim->u1 = du + (m->pixelOffsetX + u0off);
		prim->v1 = m->pixelOffsetY + v0off;
		prim->u2 = m->pixelOffsetX + u0off;
		prim->v2 = dv + (m->pixelOffsetY + v0off);
		prim->u3 = du + (m->pixelOffsetX + u0off);
		prim->v3 = dv + (m->pixelOffsetY + v0off);
		AddPrim(ACTIVE_ORDERING_TABLE->org + depth, prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}
}

void VS_restoreCameraView(void)
{
	GsSetProjection(VIEWPORT_DISTANCE);
	DRAWING_OFFSET_X = MAIN_D_801352EC;
	DRAWING_OFFSET_Y = MAIN_D_801352F0;
	GsWSMATRIX = *(MATRIX *)VS_D_80072FA0;
	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
}

void VS_setupFixedCamera(void)
{
	*(MATRIX *)VS_D_80072FA0 = GsWSMATRIX;
	MAIN_D_801352EC = DRAWING_OFFSET_X;
	MAIN_D_801352F0 = DRAWING_OFFSET_Y;
	MAIN_D_80139B34.vpx = 0;
	MAIN_D_80139B34.vpz = -0x7d0;
	MAIN_D_80139B34.vpy = 0;
	MAIN_D_80139B34.vrx = 0;
	MAIN_D_80139B34.vry = 0;
	MAIN_D_80139B34.vrz = 0;
	MAIN_D_80139B34.rz = 0;
	MAIN_D_80139B34.super = NULL;
	GsSetRefView2(&MAIN_D_80139B34);
	DRAWING_OFFSET_X = 0xa0;
	DRAWING_OFFSET_Y = 0x78;
}

void VS_getSourceBoneTransform(void)
{
	MATRIX m;
	SVECTOR rot;
	int32_t *rotOut;
	int32_t *posOut;
	int32_t bone;

	rotOut = EFE_POP1(int32_t *);
	posOut = EFE_POP1(int32_t *);
	bone = EFE_POP1(int32_t);
	calculateBoneMatrix(MAIN_D_80134CE8->sourceEntity, bone, &m);
	matrixToEuler2(&m, &rot);
	rotOut[0] = rot.vx;
	rotOut[1] = rot.vy;
	rotOut[2] = rot.vz;
	posOut[0] = m.t[0];
	posOut[1] = m.t[1];
	posOut[2] = m.t[2];
}

void VS_copyToParentTransform(void)
{
	int32_t *dst;
	int32_t src;
	int32_t off;
	int32_t size;

	size = EFE_POP1(int32_t);
	off = EFE_POP1(int32_t);
	src = EFE_POP1(int32_t);
	dst = (int32_t *)(EFE_PARENT_INSTANCE + off);
	switch (size) {
	case 0xc:
		*dst++ = *(int32_t *)src;
		*dst++ = *(int32_t *)(src + 4);
		*dst = *(int32_t *)(src + 8);
		break;
	case 4:
		*dst = *(int32_t *)src;
		break;
	case 8:
		*dst++ = *(int32_t *)src;
		*dst = *(int32_t *)(src + 4);
		break;
	case 2:
		*(int16_t *)dst = *(int16_t *)src;
		break;
	case 1:
		*(int8_t *)dst = *(int8_t *)src;
		break;
	}
}

void VS_renderEFELine(void)
{
	int32_t *col;
	int32_t i;
	int32_t depth;
	int32_t y0;
	int32_t x1;
	int32_t y1;
	int32_t x0;
	int32_t flags;

	flags = EFE_POP1(int32_t);
	depth = EFE_POP1(int32_t);
	col = EFE_POP1(int32_t *);
	x0 = EFE_POP1(int32_t);
	y1 = EFE_POP1(int32_t);
	x1 = EFE_POP1(int32_t);
	y0 = EFE_POP1(int32_t);

	if ((depth > 0x20) && (depth < 0x1000)) {
		if ((flags & 0x20) != 0) {
			for (i = 0; i < 4; i++) {
				renderLinePrimitive(((col[0] * 50 / 100) & 0xff) | (((col[1] * 50 / 100) & 0xff) << 8) | (((col[2] * 50 / 100) & 0xff) << 16), y0 + MAIN_D_80134B20[i], x1 + MAIN_D_80134B24[i], y1 + MAIN_D_80134B20[i], x0 + MAIN_D_80134B24[i], depth, 5);
			}
		} else {
			renderLinePrimitive((col[0] & 0xff) | ((col[1] & 0xff) << 8) | ((col[2] & 0xff) << 16), y0, x1, y1, x0, depth, 0);
		}
	}
}

void VS_combineRotations(void)
{
	SVECTOR r1;
	SVECTOR r2;
	VECTOR *b;
	VECTOR *a;

	b = EFE_POP1(VECTOR *);
	a = EFE_POP1(VECTOR *);
	r1.vx = a->vx;
	r1.vy = a->vy;
	r1.vz = a->vz;
	r2.vx = b->vx;
	r2.vy = b->vy;
	r2.vz = b->vz;
	multiplyRotations(&r1, &r2);
	a->vx = r1.vx;
	a->vy = r1.vy;
	a->vz = r1.vz;
}

void VS_normalizeRotationAngles2(void)
{
	MATRIX m;
	SVECTOR rot;
	int32_t *v;
	int32_t *out;

	v = EFE_POP1(int32_t *);
	out = EFE_POP1(int32_t *);
	rot.vx = v[0];
	rot.vy = v[1];
	rot.vz = v[2];
	RotMatrixYXZ(&rot, &m);
	MAIN_func_800E4470(&m, &rot);
	out[0] = rot.vx;
	out[1] = rot.vy;
	out[2] = rot.vz;
}

void VS_rotateVectorByAngles(void)
{
	MATRIX m;
	VECTOR res;
	SVECTOR rot;
	int32_t *v;
	int32_t *w;

	v = EFE_POP1(int32_t *);
	w = EFE_POP1(int32_t *);
	rot.vx = w[0];
	rot.vy = w[1];
	rot.vz = w[2];
	RotMatrixYXZ(&rot, &m);
	ApplyMatrixLV(&m, (VECTOR *)v, &res);
	v[0] = res.vx;
	v[1] = res.vy;
	v[2] = res.vz;
}

void VS_getTargetBoneTransform(void)
{
	MATRIX m;
	SVECTOR rot;
	int32_t *rotOut;
	int32_t *posOut;
	int32_t idx;
	GsCOORDINATE2 *coord;
	GsCOORDINATE2 *matrix;

	rotOut = EFE_POP1(int32_t *);
	posOut = EFE_POP1(int32_t *);
	idx = EFE_POP1(int32_t);
	matrix = (GsCOORDINATE2 *)(((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + 0x10);
	coord = (GsCOORDINATE2 *)(((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + (idx * 136) + 0x10);
	RotMatrix((SVECTOR *)(((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + 0x70), &matrix->coord);
	ScaleMatrix(&matrix->coord, (VECTOR *)(((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + 0x60));
	TransMatrix(&matrix->coord, (VECTOR *)(((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + 0x78));
	calculatePosition(coord, &m);
	matrixToEuler2(&m, &rot);
	rotOut[0] = rot.vx;
	rotOut[1] = rot.vy;
	rotOut[2] = rot.vz;
	posOut[0] = m.t[0];
	posOut[1] = m.t[1];
	posOut[2] = m.t[2];
}

void VS_centerTransformOnEntities(void)
{
	EfeTransform *sum;
	int32_t i;
	int32_t count;
	int32_t *p;
	Entity *e;

	count = 0;
	sum = (EfeTransform *)((int32_t)EFE_INSTANCE + 4);
	sum->position[0] = 0;
	sum->position[1] = 0;
	sum->position[2] = 0;
	*(int32_t *)((int32_t)EFE_INSTANCE + 0x10) = 0;
	*(int32_t *)((int32_t)EFE_INSTANCE + 0x14) = 0;
	*(int32_t *)((int32_t)EFE_INSTANCE + 0x18) = 0;
	for (i = 1; i < 10; i++) {
		if (ENTITY_TABLE[i] == NULL) {
			continue;
		}
		e = ENTITY_TABLE[i];
		if (e == MAIN_D_80134CE8->sourceEntity) {
			continue;
		}
		if (((int8_t *)e)[0x34] == 0) {
			continue;
		}
		count++;
		p = (int32_t *)(((char **)e)[1] + 0x34);
		sum->position[0] = sum->position[0] + p[5];
		sum->position[1] = sum->position[1] + p[6];
		sum->position[2] = sum->position[2] + p[7];
	}

	sum->position[0] /= count;
	sum->position[1] /= count;
	sum->position[2] /= count;
}

void VS_shiftVectorsRight(void)
{
	int32_t *a;
	int32_t *b;

	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	a[0] >>= b[0];
	a[1] >>= b[1];
	a[2] >>= b[2];
}

void VS_maskVectors(void)
{
	int32_t *a;
	int32_t *b;

	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	a[0] &= b[0];
	a[1] &= b[1];
	a[2] &= b[2];
}

void VS_divideVectors(void)
{
	int32_t *a;
	int32_t *b;

	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	a[0] /= b[0];
	a[1] /= b[1];
	a[2] /= b[2];
}

void VS_multiplyVectors(void)
{
	int32_t *a;
	int32_t *b;

	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	a[0] *= b[0];
	a[1] *= b[1];
	a[2] *= b[2];
}

void VS_subtractVectors(void)
{
	int32_t *a;
	int32_t *b;

	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	a[0] -= b[0];
	a[1] -= b[1];
	a[2] -= b[2];
}

void VS_addVectors(void)
{
	int32_t *a;
	int32_t *b;

	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	a[0] += b[0];
	a[1] += b[1];
	a[2] += b[2];
}

void VS_copyVector(void)
{
	int32_t *dst;
	int32_t *src;

	src = EFE_POP1(int32_t *);
	dst = EFE_POP1(int32_t *);
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

void VS_getVectorLength(void)
{
	int32_t *v;
	int32_t *out;

	v = EFE_POP1(int32_t *);
	out = EFE_POP1(int32_t *);
	*out = getDistance(v[0], v[1], v[2]);
}

void VS_setTargetToHitEntity(void)
{
	MAIN_D_80134CE8->targetEntity = ENTITY_TABLE[MAIN_D_80134CD8];
}

void VS_normalizeRotationAngles(void)
{
	SVECTOR rot;
	SVECTOR res;
	MATRIX m;
	int32_t *v;
	int32_t *out;

	v = EFE_POP1(int32_t *);
	out = EFE_POP1(int32_t *);
	rot.vx = v[0];
	rot.vy = v[1];
	rot.vz = v[2];
	RotMatrixYXZ(&rot, &m);
	MAIN_func_800E4470(&m, &res);
	out[0] = res.vx;
	out[1] = res.vy;
	out[2] = res.vz;
}

void VS_findHitEntity(void)
{
	SVECTOR center;
	AABB box;
	int32_t *ext;
	int32_t mode;
	int32_t *out;
	int32_t id;
	int32_t idx;

	ext = EFE_POP1(int32_t *);
	mode = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	*out = 0;
	center.vx = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	center.vy = *(int32_t *)((int32_t)EFE_INSTANCE + 8);
	center.vz = *(int32_t *)((int32_t)EFE_INSTANCE + 0xc);
	box.center = &center;
	box.extent.vx = ext[0];
	box.extent.vy = ext[1];
	box.extent.vz = ext[2];
	MAIN_D_80134CD8 = 1;
	while (1) {
		id = findAABBHitEntity(&box, MAIN_D_80134CE8->sourceEntity, MAIN_D_80134CD8);
		MAIN_D_80134CD8 = id;
		if (id == -1) {
			return;
		}
		switch (mode) {
		case 0:
			idx = (*(int32_t *)&MAIN_D_80134CD8);
			if (((int8_t *)ENTITY_TABLE[idx])[0x53] == 0) {
				*out = 1;
				return;
			}
			MAIN_D_80134CD8 = idx + 1;
			break;
		case 1:
			*out = 1;
			return;
		default:
			return;
		}
	}
}

void VS_getVectorEulerAngles(void)
{
	SVECTOR rot;
	int32_t *v;
	int32_t *out;

	v = EFE_POP1(int32_t *);
	out = EFE_POP1(int32_t *);
	toEulerAngles(&rot, v[0], v[1], v[2]);
	out[0] = rot.vx;
	out[1] = rot.vy;
	out[2] = rot.vz;
}

void VS_printDebugValue(void)
{
	printf(MAIN_D_80134B1C, EFE_POP1(int32_t));
}

void VS_getRandomInRange(void)
{
	int32_t a;
	int32_t b;
	int32_t *out;

	a = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	*out = customRandom(b, a);
}

void VS_interpolateValue(void)
{
	int32_t *out;
	int32_t lo;
	int32_t hi;
	int32_t t;
	int32_t start;
	int32_t end;
	int32_t tmp;

	end = EFE_POP1(int32_t);
	start = EFE_POP1(int32_t);
	t = EFE_POP1(int32_t);
	hi = EFE_POP1(int32_t);
	lo = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	if (hi < lo) {
		tmp = lo;
		lo = hi;
		hi = tmp;
	}

	if (t < lo) {
		t = lo;
	}

	if (hi < t) {
		t = hi;
	}

	if (lo == hi) {
		*out = start;
	} else {
		*out = start + ((t - lo) * (end - start) / (hi - lo));
	}
}

void VS_calculateCosine(void)
{
	int32_t *out;
	int32_t scale;
	int32_t angle;

	out = EFE_POP1(int32_t *);
	scale = EFE_POP1(int32_t);
	angle = EFE_POP1(int32_t);
	*out = (scale * rcos(angle & 0xfff)) >> 12;
}

void VS_calculateSine(void)
{
	int32_t *out;
	int32_t scale;
	int32_t angle;

	out = EFE_POP1(int32_t *);
	scale = EFE_POP1(int32_t);
	angle = EFE_POP1(int32_t);
	*out = (scale * rsin(angle & 0xfff)) >> 12;
}

void VS_getSourceDigimonSize(void)
{
	int32_t *out;
	int32_t type;
	int16_t radius;
	int16_t height;

	out = EFE_POP1(int32_t *);
	type = *(int32_t *)(int32_t)MAIN_D_80134CE8->sourceEntity;
	radius = DIGIMON_DATA[type].radius;
	height = DIGIMON_DATA[type].height;
	out[0] = radius;
	out[1] = height;
	out[2] = radius;
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_applyHomingMovement);

void VS_getUVAnimTimer(void)
{
	int32_t off;
	int32_t *out;
	int32_t idx;
	char *base;

	idx = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	off = (uint32_t)idx;
	base = (char *)MAIN_D_80134D0C[8];
	*out = ((int16_t *)(base + (off * 32)))[9];
}

void VS_checkTargetCollision(void)
{
	int32_t r;
	int32_t *flag;
	int32_t *out2;
	int32_t *out;
	char *tgt;
	int32_t d0;
	int32_t d1;
	int16_t ang;
	int32_t t;

	r = EFE_POP1(int32_t);
	flag = EFE_POP1(int32_t *);
	out2 = EFE_POP1(int32_t *);
	out = EFE_POP1(int32_t *);
	tgt = ((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + 0x48;
	d0 = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0] - *(int32_t *)tgt;
	d1 = *(int32_t *)((int32_t)EFE_INSTANCE + 0xc) - *(int32_t *)(tgt + 8);
	if ((r * r) < ((d0 * d0) + (d1 * d1))) {
		goto zero;
	}

	*flag = 1;
	ang = _atan(d0, d1);
	ang = ang - 0x400;
	ang = -ang;
	ang = ang & 0xfff;
	t = ang + 0x800;
	*out2 = t;
	ang = t;
	ang = ang >> 3;
	out[2] = (r * _sin(0x80 - ang)) >> 12;
	out[0] = (r * _cos(0x80 - ang)) >> 12;
	return;
zero:
	*flag = 0;
}

void VS_rotateTransformTowardPoint(void)
{
	int32_t *p;
	EfeTransform *q;
	int32_t angle;

	p = EFE_POP1(int32_t *);
	q = (EfeTransform *)((int32_t)EFE_INSTANCE + 4);
	angle = _atan(p[0] - q->position[0], p[2] - q->position[2]);
	angle = -(angle - 0x400) & 0xfff;
	*(int32_t *)((int32_t)EFE_INSTANCE + (int32_t)&((EfeInstance *)0)->transform.rotation[1]) = angle;
}

void VS_setTransformToSourceBone(void)
{
	int32_t idx;
	int32_t *p;
	char *src;
	int32_t s1;
	int32_t s2;

	idx = EFE_POP1(int32_t);
	p = &((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	src = ((char **)(int32_t)MAIN_D_80134CE8->sourceEntity)[1] + (idx * 136) + 0x34;
	*p++ = *(int32_t *)(src + 0x14);
	*p++ = *(int32_t *)(src + 0x18);
	*p++ = *(int32_t *)(src + 0x1c);
	*p++ = 0;
	*p++ = ((int16_t *)((char **)(int32_t)MAIN_D_80134CE8->sourceEntity)[1])[0x39];
	*p = 0;
	s1 = _sin(-(p[-1] >> 3));
	s2 = _sin(-(((p[-1] >> 3) + 0x80) & 0x1ff));
	p[-5] = p[-5] + ((s1 * 25) >> 12);
	p[-3] = p[-3] + ((s2 * 25) >> 12);
}

void VS_renderParallaxSprites(void)
{
	POLY_FT4 *prim;
	int16_t *p;
	ModelComponent *m;
	int32_t ox;
	int32_t oy;
	int16_t x;
	int16_t y;
	int16_t sz;

	oy = EFE_POP1(int32_t);
	ox = EFE_POP1(int32_t);
	p = EFE_POP1(int16_t *);
	m = (ModelComponent *)MAIN_D_80134D0C[6];
	prim = (POLY_FT4 *)GsGetWorkBase();

	while (1) {
		if (p[0] < 0) {
			break;
		}
		SetPolyFT4(prim);
		prim->r0 = prim->g0 = prim->b0 = 0x80;
		prim->tpage = m->pixelPage | 0x20;
		prim->clut = m->clutPage + 0x40;
		prim->u0 = m->pixelOffsetX + p[4];
		prim->v0 = m->pixelOffsetY;
		prim->u1 = m->pixelOffsetX + p[4] + 0x1f;
		prim->v1 = m->pixelOffsetY;
		prim->u2 = m->pixelOffsetX + p[4];
		prim->v2 = m->pixelOffsetY + 0x1f;
		prim->u3 = m->pixelOffsetX + p[4] + 0x1f;
		prim->v3 = m->pixelOffsetY + 0x1f;
		x = (p[3] * (p[1] + ox)) >> 7;
		y = (p[3] * (p[2] + oy)) >> 7;
		x = x % 400;
		y = y % 320;
		if (x < 0) {
			x = x + 360;
		}
		if (y < 0) {
			y = y + 280;
		}
		y = y - 0xa0;
		x = x - 0xc8;
		sz = (p[0] * p[3]) >> 8;
		prim->x0 = x;
		prim->y0 = y;
		prim->x1 = x + sz;
		prim->y1 = y;
		prim->x2 = x;
		prim->y2 = y + sz;
		prim->x3 = x + sz;
		prim->y3 = y + sz;
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x1e, prim);
		prim++;
		p += 5;
	}

	GsSetWorkBase((PACKET *)prim);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_renderScrollingBackground);

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_setTransformToBoneOffset);

void VS_playEFESound(void)
{
	int32_t id;
	int32_t i;

	int32_t (*p)[2];

	id = EFE_POP1(int32_t);
	if (id < 0) {
		return;
	}

	if ((id != 0x12) && (id != 0x20) && (id != 0x21) && (id != 0x22)) {
		playSound(8, id);
		return;
	}

	p = MAIN_D_80139AD0;
	for (i = 0; i < 10; i++) {
		if ((*p)[0] < 0) {
			(*p)[0] = playSound2(8, id);
			(*p)[1] = (int32_t)MAIN_D_80134CE8;
			return;
		}
		p++;
	}
}

void VS_addSourceEntityParticleFX(void)
{
	int32_t timer;

	timer = EFE_POP1(int32_t);
	addEntityParticleFX((int32_t *)(int32_t)MAIN_D_80134CE8->sourceEntity, timer);
}

void VS_copyFromParentTransform(void)
{
	int32_t *src;
	int32_t dst;
	int32_t off;
	int32_t size;

	size = EFE_POP1(int32_t);
	off = EFE_POP1(int32_t);
	dst = EFE_POP1(int32_t);
	src = (int32_t *)(EFE_PARENT_INSTANCE + off);
	switch (size) {
	case 0xc:
		*(int32_t *)dst = *src++;
		*(int32_t *)(dst + 4) = *src++;
		*(int32_t *)(dst + 8) = *src;
		break;
	case 4:
		*(int32_t *)dst = *src;
		break;
	case 2:
		*(int16_t *)dst = *(int16_t *)src;
		break;
	case 1:
		*(int8_t *)dst = *(int8_t *)src;
		break;
	}
}

void VS_calculatePolarOffset(void)
{
	int32_t r;
	int32_t ang;
	int32_t *out;

	r = EFE_POP1(int32_t);
	ang = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	out[2] = (r * _sin(0x80 - ang)) >> 12;
	out[0] = (r * _cos(0x80 - ang)) >> 12;
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_renderProjectedSprite);

void VS_getTargetDigimonSize(void)
{
	int32_t *out;
	int32_t type;
	int16_t radius;
	int16_t height;

	out = EFE_POP1(int32_t *);
	type = *(int32_t *)(int32_t)MAIN_D_80134CE8->targetEntity;
	radius = DIGIMON_DATA[type].radius;
	height = DIGIMON_DATA[type].height;
	out[0] = radius;
	out[1] = height;
	out[2] = radius;
}

void VS_renderParticleFlashSprite(void)
{
	int16_t p[14];
	char *hdr;
	int32_t n;

	hdr = (char *)MAIN_D_80134D0C[6];
	n = EFE_POP1(int32_t);
	p[2] = EFE_POP1(int32_t);
	*(int32_t *)&p[4] = EFE_POP1(int16_t);
	p[7] = EFE_POP1(int32_t);
	p[6] = EFE_POP1(int32_t);
	((int8_t *)p)[0x13] = EFE_POP1(int32_t) + ((uint8_t *)hdr)[0x15];
	((int8_t *)p)[0x12] = EFE_POP1(int32_t) + ((uint8_t *)hdr)[0x14];
	p[1] = EFE_POP1(int32_t);
	p[0] = EFE_POP1(int32_t);
	if (p[2] < 0xa) {
		return;
	}

	if (p[2] >= 0x1000) {
		return;
	}

	p[8] = ((uint16_t *)hdr)[8] | 0x20;
	p[10] = ((uint16_t *)hdr)[9] + (n << 6);

	((uint8_t *)p)[0x16] = ((uint8_t *)p)[0x17] = ((uint8_t *)p)[0x18] = ((uint8_t *)p)[0x19] = 0x80;
	renderParticleFlash(p);
}

void VS_projectPositionToScreen(void)
{
	DVECTOR screen;
	SVECTOR pos;
	int32_t *depthOut;
	int32_t *yOut;
	int32_t *xOut;
	int32_t *src;

	depthOut = EFE_POP1(int32_t *);
	yOut = EFE_POP1(int32_t *);
	xOut = EFE_POP1(int32_t *);
	src = EFE_POP1(int32_t *);
	pos.vx = src[0];
	pos.vy = src[1];
	pos.vz = src[2];
	*depthOut = worldPosToScreenPos(&pos, &screen);
	*depthOut >>= 4;
	*xOut = screen.vx;
	*yOut = screen.vy;
}

void VS_renderScreenSprite(void)
{
	GsSPRITE sprite;
	ModelComponent *m;
	int32_t flip;
	int32_t depth;

	m = (ModelComponent *)MAIN_D_80134D0C[6];
	flip = EFE_POP1(int32_t);
	sprite.scaley = EFE_POP1(int32_t);
	sprite.scalex = EFE_POP1(int32_t);
	depth = EFE_POP1(int32_t);
	sprite.rotate = EFE_POP1(int32_t) << 12;
	sprite.my = EFE_POP1(int32_t);
	sprite.mx = EFE_POP1(int32_t);
	sprite.h = EFE_POP1(int32_t);
	sprite.w = EFE_POP1(int32_t);
	sprite.v = EFE_POP1(int32_t) + m->pixelOffsetY;
	sprite.u = EFE_POP1(int32_t) + m->pixelOffsetX;
	sprite.y = EFE_POP1(int32_t);
	sprite.x = EFE_POP1(int32_t);

	if (flip < 0) {
		sprite.x += (int16_t)(0xa0 - DRAWING_OFFSET_X);
		sprite.y += (int16_t)(0x78 - DRAWING_OFFSET_Y);
		sprite.cy = -flip + (m->clutPage >> 6);
	} else {
		sprite.cy = flip + (m->clutPage >> 6);
	}

	sprite.attribute = 0x50000000;
	sprite.tpage = m->pixelPage | 0x20;
	sprite.cx = (m->clutPage & 0x3f) << 4;
	sprite.r = sprite.g = sprite.b = 0x80;

	if ((depth >= 0) && (depth < 0x1000)) {
		GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, (uint16_t)depth);
	}
}

void VS_addCloudEffect(void)
{
	int16_t pos[3];
	int32_t *v;

	v = EFE_POP1(int32_t *);
	pos[0] = v[0];
	pos[1] = v[1];
	pos[2] = v[2];
	createCloudFX(pos);
}

void VS_selectNextTargetEntity(void)
{
	int32_t *out;
	Entity *e;

	out = EFE_POP1(int32_t *);
	if (MAIN_D_80134CE0 >= 10) {
		*out = -1;
		return;
	}

	for (; MAIN_D_80134CE0 < 10; MAIN_D_80134CE0++) {
		if (ENTITY_TABLE[MAIN_D_80134CE0] == NULL) {
			continue;
		}
		e = ENTITY_TABLE[MAIN_D_80134CE0];
		if (e == MAIN_D_80134CE8->sourceEntity) {
			continue;
		}
		if (((int8_t *)e)[0x34] == 0) {
			continue;
		}
		if (((int8_t *)e)[0x35] == 0) {
			continue;
		}
		if (((int16_t *)e)[0x26] > 0) {
			break;
		}
	}

	if (MAIN_D_80134CE0 >= 10) {
		*out = -1;
		return;
	}

	MAIN_D_80134CE8->targetEntity = ENTITY_TABLE[*(int32_t *)&MAIN_D_80134CE0];
	*out = MAIN_D_80134CE0++;
}

void VS_addParticleEmitter(void)
{
	int32_t n;
	int32_t *vec;
	int32_t a;
	int32_t b;
	int32_t i;
	char *p;

	a = EFE_POP1(int32_t);
	vec = EFE_POP1(int32_t *);
	n = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t);
	for (i = 0; i < 4; i++) {
		if (((int32_t (*)[0x6d])MAIN_D_80134CCC)[i][0] == 0) {
			break;
		}
	}

	if (i == 4) {
		return;
	}

	p = *(char **)&MAIN_D_80134CCC + (i * 0x1b4);
	p[0xf] = b;
	for (i = 0; i < 0x15; i++) {
		((int16_t (*)[10])p + i)[0][8] = 0;
	}

	*(int32_t *)p = (int32_t)EFE_INSTANCE;
	*(int16_t *)(p + 4) = a;
	p[0xc] = vec[0];
	p[0xd] = vec[1];
	p[0xe] = vec[2];
	*(int16_t *)(p + 6) = n * 16;
	*(int16_t *)(p + 8) = n / 8 * 16;
	*(int16_t *)(p + 0xa) = n / 160 * 16;
}

void VS_setEFEModelObjectColor(void)
{
	int32_t *rec;
	int32_t *color;
	int32_t idx;
	int32_t i;
	int32_t count;
	int32_t t;

	char (*pr)[0x20];
	char (*pg)[0x20];
	char (*pb)[0x20];
	int32_t *ent;

	color = EFE_POP1(int32_t *);
	idx = EFE_POP1(int32_t);
	ent = (int32_t *)((uint32_t)((char **)MAIN_D_80134D0C[6])[1] + 0xc);
	ent = (int32_t *)((int32_t)ent + (idx * 28));
	rec = (int32_t *)ent[4];
	count = ent[5];
	pr = (char (*)[0x20])((char *)rec + 0x14);
	i = 0;
	pg = (char (*)[0x20])((char *)rec + 0x15);
	pb = (char (*)[0x20])((char *)rec + 0x16);
	for (; i < count; i++) {
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

void VS_copyTargetEntityPosition(void)
{
	int32_t *out;
	int32_t *m;

	out = EFE_POP1(int32_t *);
	m = (int32_t *)(((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + 0x34);
	out[0] = m[5];
	out[1] = m[6];
	out[2] = m[7];
}

void VS_steerTransformTowardPoint(void)
{
	MATRIX m;
	SVECTOR in;
	SVECTOR rot;
	SVECTOR out;
	EfeTransform *pos;
	int32_t turn;
	int32_t speed;
	int32_t *target;
	int32_t d;

	turn = EFE_POP1(int32_t);
	speed = EFE_POP1(int32_t);
	target = EFE_POP1(int32_t *);
	pos = (EfeTransform *)((int32_t)EFE_INSTANCE + 4);
	rot.vx = 0;
	rot.vy = -*(int32_t *)((int32_t)EFE_INSTANCE + 0x14);
	rot.vz = 0;
	in.vx = target[0] - pos->position[0];
	in.vy = 0;
	in.vz = target[2] - pos->position[2];
	RotMatrixZYX(&rot, &m);
	ApplyMatrixSV(&m, &in, &out);
	d = _atan(out.vx, out.vz);
	d = -(d - 0x400) & 0xfff;
	if (d >= 0x801) {
		d -= 0x1000;
	}

	if (d > 0) {
		if (d < turn) {
			turn = d;
		}
	} else if (-d < turn) {
		turn = d;
	} else {
		turn = -turn;
	}

	in.vx = 0;
	in.vy = 0;
	in.vz = -speed * 8;
	rot.vx = *(int32_t *)((int32_t)EFE_INSTANCE + 0x10);
	*(int32_t *)((int32_t)EFE_INSTANCE + 0x14) += turn;
	rot.vy = *(int32_t *)((int32_t)EFE_INSTANCE + 0x14);
	rot.vz = *(int32_t *)((int32_t)EFE_INSTANCE + 0x18);
	RotMatrixZYX(&rot, &m);
	ApplyMatrixSV(&m, &in, &out);
	pos->position[0] += out.vx >> 3;
	pos->position[1] += out.vy >> 3;
	pos->position[2] += out.vz >> 3;
}

void VS_interpolateVector(void)
{
	int32_t *out;
	int32_t t0;
	int32_t t1;
	int32_t t;
	int32_t *b;
	int32_t *a;

	out = EFE_POP1(int32_t *);
	t = EFE_POP1(int32_t);
	t1 = EFE_POP1(int32_t);
	t0 = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	out[0] = lerp(a[0], b[0], t0, t1, t);
	out[1] = lerp(a[1], b[1], t0, t1, t);
	out[2] = lerp(a[2], b[2], t0, t1, t);
}

void VS_discardEFEOperandPair(void)
{
	EFE_DROP1();
	EFE_DROP1();
}

void VS_getScatteredSpawnPosition(void)
{
	int32_t r;
	int32_t *out;
	int32_t *p;
	int32_t *src;

	r = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);

	if (EFE_PARENT_INSTANCE == 0) {
	}

	if (EFE_PARENT_INSTANCE == 0) {
		p = (int32_t *)(((char **)(int32_t)MAIN_D_80134CE8->sourceEntity)[1] + (*(int16_t *)(int32_t)MAIN_D_80134CE8->boneOffset * 136) + 0x34);
		out[0] = p[5];
		out[1] = p[6];
		out[2] = p[7];
	} else {
		src = (int32_t *)(EFE_PARENT_INSTANCE + 4);
		out[0] = *src++;
		out[1] = *src++;
		out[2] = *src;
	}

	if (r > 0) {
		out[0] = out[0] + ((rand() % r) - (r >> 1));
		out[1] = out[1] + ((rand() % r) - (r >> 1));
		out[2] = out[2] + ((rand() % r) - (r >> 1));
	}
}

void VS_checkCollisionWithDefaultPower(void)
{
	EFE_PUSH1(int32_t, -1);
	VS_applyBoxAttackHit();
}

void VS_addAttackObjectToTarget(void)
{
	int16_t pos[4];
	int32_t i;
	int32_t j;
	int32_t e;

	if (((int8_t *)(int32_t)MAIN_D_80134CE8->targetEntity)[0x53] != 0) {
		return;
	}

	pos[0] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	pos[1] = *(int32_t *)((int32_t)EFE_INSTANCE + 8);
	pos[2] = *(int32_t *)((int32_t)EFE_INSTANCE + 0xc);
	for (i = 1; i < 10; i++) {
		if ((int32_t)ENTITY_TABLE[i] == (int32_t)MAIN_D_80134CE8->targetEntity) {
			break;
		}
	}

	((int8_t *)(int32_t)MAIN_D_80134CE8->targetEntity)[0x53] = 1;
	for (j = 1; j < 10; j++) {
		e = (int32_t)ENTITY_TABLE[j];
		if (e == (int32_t)MAIN_D_80134CE8->sourceEntity) {
			break;
		}
	}

	addAttackObject(i, 1, pos, MAIN_D_80134CD4, MAIN_D_80134CD0, j);
}

void VS_setTransformToTargetBone(void)
{
	int32_t r;
	int32_t idx;
	int32_t *p;
	char *src;
	int32_t s1;
	int32_t s2;

	r = EFE_POP1(int32_t);
	idx = EFE_POP1(int32_t);
	p = &((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	src = ((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + (idx * 136) + 0x34;
	*p++ = *(int32_t *)(src + 0x14);
	*p++ = *(int32_t *)(src + 0x18);
	*p++ = *(int32_t *)(src + 0x1c);
	*p++ = 0;
	*p++ = ((int16_t *)((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1])[0x39];
	*p = 0;
	s1 = _sin(-(p[-1] >> 3));
	s2 = _sin(-(((p[-1] >> 3) + 0x80) & 0x1ff));
	p[-5] = p[-5] + ((r * s1) >> 12);
	p[-3] = p[-3] + ((r * s2) >> 12);
}

void VS_renderCenteredSprite(void)
{
	int32_t y;
	int32_t x;

	y = EFE_POP1(int32_t);
	x = EFE_POP1(int32_t);
	EFE_PUSH1(int32_t, x + 1);
	EFE_PUSH1(int32_t, y + 1);
	EFE_PUSH1(int32_t, x / 2);
	EFE_PUSH1(int32_t, y / 2);
	EFE_PUSH1(int32_t, 0);
	EFE_PUSH1(int32_t, 5);
	EFE_PUSH1(int32_t, 0);
	VS_renderProjectedSprite();
}

void VS_initializeEFETransform(void)
{
	int32_t *src;
	int32_t *dst;
	int32_t *chk;

	dst = &((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	chk = (int32_t *)EFE_PARENT_INSTANCE;
	if (chk == NULL) {
		VS_setTransformToBoneOffset();
		return;
	}

	src = (int32_t *)((int32_t)chk + 4);
	*dst++ = *src++;
	*dst++ = *src++;
	*dst++ = *src++;
	*dst++ = *src++;
	*dst++ = *src++;
	*dst = *src;
}

void VS_drawTMD(void)
{
	typedef struct {
		int32_t rotation[3];
	} EfeRot;

	EFE_SCRATCH->scale = EFE_POP1(VECTOR *);
	EFE_SCRATCH->id = EFE_POP1(int32_t);
	EFE_SCRATCH->rot.vx = ((EfeRot *)((int32_t)EFE_INSTANCE + 0x10))->rotation[0];
	EFE_SCRATCH->rot.vy = ((EfeRot *)((int32_t)EFE_INSTANCE + 0x10))->rotation[1];
	EFE_SCRATCH->rot.vz = ((EfeRot *)((int32_t)EFE_INSTANCE + 0x10))->rotation[2];
	RotMatrix(&EFE_SCRATCH->rot, &EFE_SCRATCH->m1);
	ScaleMatrix(&EFE_SCRATCH->m1, EFE_SCRATCH->scale);
	EFE_SCRATCH->m1.t[0] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	EFE_SCRATCH->m1.t[1] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[1];
	EFE_SCRATCH->m1.t[2] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[2];
	GsMulCoord0(&GsWSMATRIX, &EFE_SCRATCH->m1, &EFE_SCRATCH->m0);
	if (EFE_SCRATCH->m0.t[2] < -0x12c) {
		return;
	}

	if (EFE_SCRATCH->m0.t[2] >= 0x10000) {
		return;
	}

	GsSetLightMatrix(&EFE_SCRATCH->m1);
	GsSetLsMatrix(&EFE_SCRATCH->m0);
	GsLinkObject4((unsigned long)(((char **)MAIN_D_80134D0C[6])[1] + 0xc), &EFE_SCRATCH->obj, EFE_SCRATCH->id);
	EFE_SCRATCH->obj.coord2 = NULL;
	EFE_SCRATCH->obj.attribute = 0;
	GsSortObject4(&EFE_SCRATCH->obj, ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0x2c));
}

void VS_initializeSubEffectInstructions(void)
{
	int32_t b;
	int32_t a;

	a = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t);
	EFE_INSTANCE->frame = 0;
	MAIN_D_80134CE8->inst = (int16_t *)b;
	MAIN_D_80134CE8->someInst = (int16_t *)a;
}

void VS_initializeUVAnim(void)
{
	int32_t val;
	int32_t ptr;
	int32_t idx;
	int32_t nv;
	int32_t *rec;
	int32_t i;
	int32_t *q;
	int32_t t;

	ptr = EFE_POP1(int32_t);
	val = EFE_POP1(int32_t);
	idx = EFE_POP1(int32_t);
	if ((idx < 0) || (idx >= MAIN_D_80134D0C[7])) {
		MAIN_D_80134CE4 = -2;
		MAIN_D_80134D00 = NULL;
		return;
	}

	nv = 8;
	rec = (int32_t *)((char *)MAIN_D_80134D0C[nv] + (nv * (4 * idx)));
	((int16_t *)rec)[0] = val;
	if (ptr == -1) {
		return;
	}

	((int16_t *)rec)[6] = *(int32_t *)ptr;
	rec[6] = ptr + 4;
	rec[7] = rec[6];
	((int16_t *)rec)[8] = 0;
	((int16_t *)rec)[7] = 0;
	((int16_t *)rec)[9] = 1;
	((int16_t *)rec)[10] = 0;
	q = (int32_t *)rec[6];
	for (i = 0; i < ((int16_t *)rec)[6]; i++) {
		t = *(int16_t *)q;
		((int16_t *)rec)[10] += t;
		q++;
	}
}

void VS_checkTechCompatibility(void)
{
	int16_t *p;
	int32_t type;

	p = EFE_POP1(int16_t *);
	type = (int16_t)getOriginalType(*(int32_t *)MAIN_D_80134CE8->sourceEntity);
	while (*p != type) {
		if (*p < 0) {
			MAIN_D_80134CE4 = -1;
			MAIN_D_80134D00 = NULL;
			break;
		}
		p = (int16_t *)((int32_t)p + 0xa);
	}

	MAIN_D_80134CF0 = (int32_t)(p + 1);
	MAIN_D_80134CE8->boneOffset = (EfeBoneOffset *)MAIN_D_80134CF0;
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_spawnEFESubEffect);

void VS_popEFEValueToVariable(void)
{
	int32_t val;

	if (MAIN_D_80134D00[2] == 0) {
		val = EFE_POP1(int32_t);
		*(int32_t *)(MAIN_D_80134D00[1] + (int32_t)EFE_INSTANCE) = val;
	} else {
		val = EFE_POP1(int32_t);
		*(int32_t *)(MAIN_D_80134D00[1] + MAIN_D_80134D08) = val;
	}

	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

void VS_returnFromEFESubroutine(void)
{
	MAIN_D_80134D00 = EFE_POP2(int16_t *);
	if (MAIN_D_80134D00 != NULL) {
		MAIN_D_80134CE8 = EFE_POP2(EfeSubEffect *);
		EFE_INSTANCE = MAIN_D_80134CE8->instance;
		EFE_PARENT_INSTANCE = (int32_t)MAIN_D_80134CE8->parentInstance;
		MAIN_D_80134CE0 = EFE_POP2(int32_t);
	}
}

void VS_dispatchEFESubOpcode(void)
{
	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 2);
	VS_jtbl_80072E1C[MAIN_D_80134D04 >> 8]();
}

void VS_callEFESubroutine(void)
{
	int16_t *ip;

	ip = MAIN_D_80134D00;
	EFE_PUSH2(int32_t, MAIN_D_80134CE0);
	EFE_PUSH2(EfeSubEffect *, MAIN_D_80134CE8);
	EFE_PUSH2(int16_t *, MAIN_D_80134D00 + 3);
	MAIN_D_80134D00 = (int16_t *)(ip[1] + MAIN_D_80134D08);
}

void VS_pushEFEVariableAddress(void)
{
	int32_t ip;

	ip = (int32_t)MAIN_D_80134D00;

	*EFE_DATA_STACK = ((int16_t *)ip)[1];
	if (((int16_t *)ip)[2] == 0) {
		*EFE_DATA_STACK++ += (int32_t)EFE_INSTANCE;
	} else {
		*EFE_DATA_STACK++ += MAIN_D_80134D08;
	}

	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

void VS_pushEFEVariable(void)
{
	int32_t *p;

	if (MAIN_D_80134D00[2] == 0) {
		p = (int32_t *)(MAIN_D_80134D00[1] + (int32_t)EFE_INSTANCE);
	} else {
		p = (int32_t *)(MAIN_D_80134D00[1] + MAIN_D_80134D08);
	}

	if ((MAIN_D_80134D04 & 0xf00) == 0x400) {
		EFE_PUSH1(int32_t, *p);
	} else if ((MAIN_D_80134D04 & 0xf00) == 0x200) {
		EFE_PUSH1(int32_t, *(int16_t *)p);
	} else {
		EFE_PUSH1(int32_t, *(int8_t *)p);
	}

	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

void VS_pushEFEImmediate(void)
{
	EFE_PUSH1(int32_t, MAIN_D_80134D00[1]);
	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

void VS_jumpEFEScript(void)
{
	MAIN_D_80134D00 = (int16_t *)(MAIN_D_80134D00[1] + MAIN_D_80134D08);
}

void VS_stopEFEScript(void)
{
	MAIN_D_80134D04 &= 0xff;
	MAIN_D_80134D00 = NULL;
}

void VS_branchEFEOnComparison(void)
{
	int16_t *ip;
	int32_t res;

	ip = MAIN_D_80134D00;
	if ((MAIN_D_80134D04 & 0xf00) == 0x400) {
		if (ip[2] == 0) {
			res = VS_D_80070F34[MAIN_D_80134D04 >> 12](*(int32_t *)(ip[1] + (int32_t)EFE_INSTANCE));
		} else {
			res = VS_D_80070F34[MAIN_D_80134D04 >> 12](*(int32_t *)(ip[1] + MAIN_D_80134D08));
		}
	} else if ((MAIN_D_80134D04 & 0xf00) == 0x200) {
		if (ip[2] == 0) {
			res = VS_D_80070F34[MAIN_D_80134D04 >> 12](*(int16_t *)(ip[1] + (int32_t)EFE_INSTANCE));
		} else {
			res = VS_D_80070F34[MAIN_D_80134D04 >> 12](*(int16_t *)(ip[1] + MAIN_D_80134D08));
		}
	} else {
		if (ip[2] == 0) {
			res = VS_D_80070F34[MAIN_D_80134D04 >> 12](*(int8_t *)(ip[1] + (int32_t)EFE_INSTANCE));
		} else {
			res = VS_D_80070F34[MAIN_D_80134D04 >> 12](*(int8_t *)(ip[1] + MAIN_D_80134D08));
		}
	}

	if (res == 0) {
		MAIN_D_80134D00 = (int16_t *)(ip[3] + MAIN_D_80134D08);
	} else {
		MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 0xa);
	}
}

void VS_applyEFEVariableOperator(void)
{
	if (MAIN_D_80134D00[2] == 0) {
		VS_D_80070E94[(MAIN_D_80134D04 >> 8) & 0xf][MAIN_D_80134D04 >> 12]((int32_t *)(MAIN_D_80134D00[1] + (int32_t)EFE_INSTANCE));
	} else {
		VS_D_80070E94[(MAIN_D_80134D04 >> 8) & 0xf][MAIN_D_80134D04 >> 12]((int32_t *)(MAIN_D_80134D00[1] + MAIN_D_80134D08));
	}

	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

void VS_loadEFEIndexedVariable(void)
{
	int16_t *ip;
	int32_t idx;

	ip = MAIN_D_80134D00;
	if ((MAIN_D_80134D04 & 0xf000) == 0x4000) {
		if (ip[4] == 0) {
			idx = *(int32_t *)(ip[3] + (int32_t)EFE_INSTANCE);
		} else {
			idx = *(int32_t *)(ip[3] + MAIN_D_80134D08);
		}
	} else if ((MAIN_D_80134D04 & 0xf000) == 0x2000) {
		if (ip[4] == 0) {
			idx = *(int16_t *)(ip[3] + (int32_t)EFE_INSTANCE);
		} else {
			idx = *(int16_t *)(ip[3] + MAIN_D_80134D08);
		}
	} else {
		if (ip[4] == 0) {
			idx = *(int8_t *)(ip[3] + (int32_t)EFE_INSTANCE);
		} else {
			idx = *(int8_t *)(ip[3] + MAIN_D_80134D08);
		}
	}

	if ((MAIN_D_80134D04 & 0xf00) == 0x400) {
		(*(int32_t *)&MAIN_D_80134CEC) = *(int32_t *)(ip[1] + (idx * 4) + MAIN_D_80134D08);
	} else if ((MAIN_D_80134D04 & 0xf00) == 0x200) {
		MAIN_D_80134CEC = *(int16_t *)(ip[1] + (idx * 2) + MAIN_D_80134D08);
	} else {
		MAIN_D_80134CEC = *(int8_t *)(ip[1] + idx + MAIN_D_80134D08);
	}

	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 0xa);
}

void VS_loadEFERandomValue(void)
{
	MAIN_D_80134CEC = rand();
	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 2);
}

void VS_loadEFEVariable(void)
{
	int16_t *ip;

	ip = MAIN_D_80134D00;
	if ((MAIN_D_80134D04 & 0xf00) == 0x400) {
		if (ip[2] == 0) {
			(*(int32_t *)&MAIN_D_80134CEC) = *(int32_t *)(ip[1] + (int32_t)EFE_INSTANCE);
		} else {
			MAIN_D_80134CEC = *(int32_t *)(ip[1] + MAIN_D_80134D08);
		}
	} else if ((MAIN_D_80134D04 & 0xf00) == 0x200) {
		if (ip[2] == 0) {
			MAIN_D_80134CEC = *(int16_t *)(ip[1] + (int32_t)EFE_INSTANCE);
		} else {
			MAIN_D_80134CEC = *(int16_t *)(ip[1] + MAIN_D_80134D08);
		}
	} else {
		if (ip[2] == 0) {
			MAIN_D_80134CEC = *(int8_t *)(ip[1] + (int32_t)EFE_INSTANCE);
		} else {
			MAIN_D_80134CEC = *(int8_t *)(ip[1] + MAIN_D_80134D08);
		}
	}

	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

void VS_loadEFEImmediate(void)
{
	MAIN_D_80134CEC = MAIN_D_80134D00[1];
	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

int32_t VS_shiftRightInt32Variable(int32_t *p)
{
	*p >>= MAIN_D_80134CEC;
}

int32_t VS_shiftLeftInt32Variable(int32_t *p)
{
	*p <<= MAIN_D_80134CEC;
}

int32_t VS_moduloInt32Variable(int32_t *p)
{
	*p %= MAIN_D_80134CEC;
}

int32_t VS_divideInt32Variable(int32_t *p)
{
	*p /= MAIN_D_80134CEC;
}

int32_t VS_multiplyInt32Variable(int32_t *p)
{
	*p *= MAIN_D_80134CEC;
}

int32_t VS_subtractInt32Variable(int32_t *p)
{
	*p -= MAIN_D_80134CEC;
}

int32_t VS_addInt32Variable(int32_t *p)
{
	*p += MAIN_D_80134CEC;
}

int32_t VS_setInt32Variable(int32_t *p)
{
	*p = MAIN_D_80134CEC;
}

int32_t VS_shiftRightInt8Variable(int8_t *p)
{
	*p >>= MAIN_D_80134CEC;
}

int32_t VS_shiftLeftInt8Variable(int8_t *p)
{
	*p <<= MAIN_D_80134CEC;
}

int32_t VS_moduloInt8Variable(int8_t *p)
{
	*p %= MAIN_D_80134CEC;
}

int32_t VS_divideInt8Variable(int8_t *p)
{
	*p /= MAIN_D_80134CEC;
}

int32_t VS_multiplyInt8Variable(int8_t *p)
{
	*p *= MAIN_D_80134CEC;
}

int32_t VS_subtractInt8Variable(int8_t *p)
{
	*p -= MAIN_D_80134CEC;
}

int32_t VS_addInt8Variable(int8_t *p)
{
	*p += MAIN_D_80134CEC;
}

int32_t VS_setInt8Variable(int8_t *p)
{
	*p = MAIN_D_80134CEC;
}

int32_t VS_shiftRightInt16Variable(int16_t *p)
{
	*p >>= MAIN_D_80134CEC;
}

int32_t VS_shiftLeftInt16Variable(int16_t *p)
{
	*p <<= MAIN_D_80134CEC;
}

int32_t VS_moduloInt16Variable(int16_t *p)
{
	*p %= MAIN_D_80134CEC;
}

int32_t VS_divideInt16Variable(int16_t *p)
{
	*p /= MAIN_D_80134CEC;
}

int32_t VS_multiplyInt16Variable(int16_t *p)
{
	*p *= MAIN_D_80134CEC;
}

int32_t VS_subtractInt16Variable(int16_t *p)
{
	*p -= MAIN_D_80134CEC;
}

int32_t VS_addInt16Variable(int16_t *p)
{
	*p += MAIN_D_80134CEC;
}

int32_t VS_setInt16Variable(int16_t *p)
{
	*p = MAIN_D_80134CEC;
}

int32_t VS_compareGreaterOrEqual(int32_t x)
{
	if (x >= MAIN_D_80134CEC) {
		return 0;
	}

	return -1;
}

int32_t VS_compareGreater(int32_t x)
{
	if (x > MAIN_D_80134CEC) {
		return 0;
	}

	return -1;
}

int32_t VS_compareLessOrEqual(int32_t x)
{
	if (x <= MAIN_D_80134CEC) {
		return 0;
	}

	return -1;
}

int32_t VS_compareLess(int32_t x)
{
	if (x < MAIN_D_80134CEC) {
		return 0;
	}

	return -1;
}

int32_t VS_compareNotEqual(int32_t x)
{
	if (x != MAIN_D_80134CEC) {
		return 0;
	}

	return -1;
}

int32_t VS_compareEqual(int32_t x)
{
	if (x == MAIN_D_80134CEC) {
		return 0;
	}

	return -1;
}

int16_t VS_calculateAttackHitPosition(SVECTOR *out, int32_t *self, int32_t *other, int32_t y)
{
	SVECTOR rot;
	MATRIX m;
	char *sp2;
	int32_t dx;
	int32_t dz;

	other = (int32_t *)other[1];
	sp2 = (char *)self[1];
	dx = (int16_t)(*(int32_t *)((char *)other + 0x78) - *(int32_t *)(sp2 + 0x78));
	dz = (int16_t)(*(int32_t *)((char *)other + 0x80) - *(int32_t *)(sp2 + 0x80));
	rot.vx = 0;
	rot.vy = (_atan(dz, dx) + 0x800) & 0xfff;
	rot.vz = 0;
	RotMatrixZYX(&rot, &m);
	out->vx = 0;
	out->vy = -DIGIMON_DATA[self[0]].height;
	out->vz = y;
	ApplyMatrixSV(&m, out, out);
	out->vx += (int16_t)*(int32_t *)((char *)self[1] + 0x78);
	out->vz += (int16_t)*(int32_t *)((char *)self[1] + 0x80);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_renderParallelLines);

int32_t VS_interpolateClamped(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end)
{
	int32_t tmp;

	if (hi < lo) {
		tmp = lo;
		lo = hi;
		hi = tmp;
	}

	t = t < lo ? lo : (hi < t ? hi : t);
	if (lo >= t) {
		return start;
	}

	return start + ((end - start) * (t - lo) / (hi - lo));
}

void VS_initializeEFESubOpcodeTable(void)
{
	int32_t i;

	for (i = 0; (uint32_t)i < 0x61; i++) {
		if ((uint32_t)((int32_t (*)[2])VS_D_80070B44)[i][0] >= 0x61) {
			exit(1);
		}
		VS_jtbl_80072E1C[((int32_t (*)[2])VS_D_80070B44)[i][0]] = (void (*)(void))((void *(*)[2])VS_D_80070B48)[i][0];
	}
}

void VS_dispatchEFEOpcode(int32_t op)
{
	VS_jtbl_80070E4C[op]();
}

int32_t VS_runEFEScript(int32_t script)
{
	EFE_DATA_STACK = EFE_SCRIPT_MEM1_DATA;
	MAIN_D_80134CFC = MAIN_D_80139B54;
	MAIN_D_80134D00 = (int16_t *)script;
	EFE_PUSH2(int32_t, 0);
	while (MAIN_D_80134D00 != NULL) {
		MAIN_D_80134D04 = **(int16_t **)&MAIN_D_80134D00;
		VS_jtbl_80070E4C[MAIN_D_80134D04 & 0xff]();
	}

	if (MAIN_D_80134CE8->instance->frame == -1) {
		MAIN_D_80134CE8->inst = NULL;
	}

	return MAIN_D_80134CE4;
}

void VS_resetPoisonBubbles(void)
{
	setShortWithStride(VS_D_80072FC0, -1, 0xc, 0xc);
}

int32_t VS_addPoisonBubble(int32_t arg)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 0xc; i++) {
		if (((int16_t (*)[6])VS_D_80072FC0)[i][0] == -1) {
			break;
		}
	}

	if (i == 0xc) {
		return -1;
	}

	p = ((int16_t (*)[6])VS_D_80072FC0)[i];
	p[0] = 0;
	*(int32_t *)&p[4] = arg;
	p[2] = (rand() % 100) - 0x32;
	p[3] = (rand() % 100) - 0x32;
	addObject(0x805, i, VS_tickPoisonBubble, VS_renderPoisonBubble);

	return i;
}

void VS_tickPoisonBubble(int32_t i)
{
	int16_t *p;

	p = &VS_D_80072FC0[i * 6];
	p[0]++;
	if (p[0] >= 0x28) {
		p[0] = -1;
		removeObject(0x805, i);
	}
}

void VS_renderPoisonBubble(int32_t i)
{
	SVECTOR pos;
	DVECTOR screen;
	int16_t *p;
	int16_t frame;
	int32_t otz;
	int32_t d;

	p = &VS_D_80072FC0[i * 6];
	frame = p[0];
	translateConditionFXToEntity((Entity *)*(int32_t *)&p[4], &pos);
	pos.vx += p[2];
	pos.vy -= (int16_t)lerp(0x32, 0xc8, 0, 0x28, frame);
	pos.vz += p[3];
	otz = worldPosToScreenPos(&pos, &screen);
	d = _sin((int16_t)lerp(0, 0x500, 0, 0x28, frame)) * 0x14 / 4096;
	screen.vx += (int16_t)(d * VIEWPORT_DISTANCE / otz);
	if ((otz > 0x200) && (otz < 0x10000)) {
		VS_POISON_BUBBLE_SPRITE.u = MAIN_D_80134B4C[(frame >> 1) % 6] + 0x20;
		renderSprite(&VS_POISON_BUBBLE_SPRITE, screen.vx, screen.vy, otz, 0x4ea4, 0x4ea4);
	}
}

void VS_tickPoisonEffect(int32_t i)
{
	int16_t *p;

	p = VS_D_80073050[i];
	p[0]++;
	p[0] %= 0x1e;
	if (p[0] == 1) {
		VS_addPoisonBubble(*(int32_t *)&p[2]);
	}
}

void VS_renderPoisonEffect(void)
{
}

void VS_initializePoisonBubble(void)
{
	int32_t i;

	for (i = 0; i < 4; i++) {
		VS_D_80073050[i][0] = -1;
	}

	VS_resetPoisonBubbles();
}

int32_t VS_addPoisonEffect(DigimonEntity *digimon)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 4; i++) {
		if (VS_D_80073050[i][0] == -1) {
			break;
		}
	}

	if (i == 4) {
		return -1;
	}

	p = VS_D_80073050[i];
	p[0] = 0;
	*(int32_t *)&p[2] = (int32_t)digimon;
	addObject(0x808, i, VS_tickPoisonEffect, VS_renderPoisonEffect);

	return i;
}

void VS_removePoisonEffect(int32_t i, DigimonEntity *digimon)
{
	int16_t *p;

	p = VS_D_80073050[i];
	if ((i >= 0) && (i < 4) && (*(int32_t *)&p[2] == (int32_t)digimon)) {
		p[0] = -1;
		removeObject(0x808, i);
	}
}

void VS_removeAllPoisonEffects(void)
{
	int32_t i;

	for (i = 0; i < 4; i++) {
		removeObject(0x808, i);
	}

	for (i = 0; i < 0xc; i++) {
		removeObject(0x805, i);
	}
}

void VS_tickConfusionEffect(int32_t i)
{
	int16_t *p;

	p = VS_D_80073070[i];
	p[1] += 7;
	p[2] += 0x5b;
}

void VS_renderConfusionEffect(int32_t idx)
{
	SVECTOR pos;
	GsCOORDINATE2 coord;
	SVECTOR rot;
	VECTOR trans;
	int32_t ang;
	int16_t *p;
	int32_t i;
	int32_t a;
	int32_t b;

	p = VS_D_80073070[idx];
	for (i = 0, a = 0, b = 0; i < 3; i++, b += 0x555, a += 0xaa) {
		translateConditionFXToEntity((Entity *)((int32_t *)p)[3], &pos);
		ang = p[1] + a;
		trans.vx = pos.vx + (_sin(ang) * 0x78 / 4096);
		trans.vy = pos.vy - 0x78;
		trans.vz = pos.vz + (_cos(ang) * 0x78 / 4096);
		rot.vx = 0;
		rot.vy = (p + i)[3] + (p[2] - 0x400 + b);
		rot.vz = 0xe3;
		renderTMDModel((uint8_t *)MAIN_D_801352F4, 0, &coord, NULL, &trans, &rot, &VS_D_80070FA4);
	}
}

void VS_initializeConfusionEffect(char *base)
{
	MAIN_D_801352F4 = (int32_t)base;
	GsMapModelingData((unsigned long *)((char *)MAIN_D_801352F4 + 4));
	setShortWithStride((int16_t *)VS_D_80073070, -1, 4, 0x10);
}

int32_t VS_addConfusionEffect(DigimonEntity *digimon)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 4; i++) {
		if (VS_D_80073070[i][0] == -1) {
			break;
		}
	}

	if (i == 4) {
		return -1;
	}

	p = VS_D_80073070[i];
	p[0] = 0;
	p[1] = 0;
	p[2] = 0;
	*(int32_t *)&p[6] = (int32_t)digimon;
	p[3] = rand();
	p[4] = rand();
	p[5] = rand();
	p[2] = 0;
	addObject(0x806, i, VS_tickConfusionEffect, VS_renderConfusionEffect);

	return i;
}

void VS_removeConfusionEffect(int32_t i, DigimonEntity *digimon)
{
	int16_t *p;

	p = VS_D_80073070[i];
	if ((i >= 0) && (i < 4) && (*(int32_t *)&p[6] == (int32_t)digimon)) {
		p[0] = -1;
		removeObject(0x806, i);
	}
}

void VS_initializeStunEffect(char *base)
{
	int32_t i;

	for (i = 0; i < 5; i++) {
		VS_D_800730B0[i][0] = -1;
	}
	VS_resetStunSubEffects();
	MAIN_D_801352F8 = base;
	GsMapModelingData((unsigned long *)(base + 4));
}

void VS_resetStunSubEffects(void)
{
	int32_t i;

	for (i = 0; i < 0x19; i++) {
		VS_D_800730EC[i][0] = -1;
	}
}

void VS_tickStunEffect(int32_t i)
{
	int16_t *p;

	p = VS_D_800730B0[i];
	p[0] = p[0] + 1;
	switch (p[2]) {
	case 0:
		if ((p[0] % 6) == 0) {
			VS_addStunSubEffect(*(DigimonEntity **)&p[4]);
		}
		if (p[0] >= p[1]) {
			p[2] = 1;
			p[0] = 1;
		}
		break;
	case 1:
		if (p[0] >= 8) {
			p[2] = 2;
			p[0] = 1;
		}
		break;
	case 2:
		if (p[0] >= 5) {
			VS_removeStunEffect(i, *(DigimonEntity **)&p[4]);
		}
		break;
	}
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_renderStunEffect);

void VS_removeAllStunSubEffects(void)
{
	int32_t i;

	for (i = 0; i < 0x19; i++) {
		VS_D_800730EC[i][0] = -1;
		removeObject(0x810, i);
	}
}

int32_t VS_addStunSubEffect(DigimonEntity *digimon)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 25; i++) {
		if (VS_D_800730EC[i][0] == -1) {
			break;
		}
	}

	if (i == 25) {
		return -1;
	}

	p = VS_D_800730EC[i];
	p[0] = 0;
	*(int32_t *)&p[2] = (int32_t)digimon;
	addObject(0x810, i, VS_tickStunSubEffect, VS_renderStunSubEffect);

	return i;
}

void VS_tickStunSubEffect(int32_t i)
{
	int16_t *p;

	p = VS_D_800730EC[i];
	p[0]++;
	if (p[0] >= 0x10) {
		p[0] = -1;
		removeObject(0x810, i);
	}
}

void VS_renderStunSubEffect(int32_t i)
{
	GsCOORDINATE2 coord;
	VECTOR trans;
	SVECTOR rot;
	VECTOR scale;
	int16_t *p;
	int32_t z;
	int32_t y;
	int32_t x;
	int32_t s;

	p = VS_D_800730EC[i];
	x = (int16_t)((Entity **)p)[1]->posData->location.vx;
	y = (int16_t)lerp(-DIGIMON_DATA[((Entity **)p)[1]->type].height * 113 / 100, ((Entity **)p)[1]->posData->location.vy, 0, 0xf, p[0]);
	z = (int16_t)((Entity **)p)[1]->posData->location.vz;
	s = DIGIMON_DATA[((Entity **)p)[1]->type].radius * 0x4000 / 350;
	if (p[0] < 4) {
		s = lerp(s * 10 / 100, s, 0, 4, p[0]);
	}

	rot = MAIN_D_80134B54;
	scale = VS_D_80071044;
	trans.vx = x;
	trans.vy = y;
	trans.vz = z;
	scale.vx = scale.vz = s;
	renderTMDModel((uint8_t *)MAIN_D_801352F8, 0, &coord, NULL, &trans, &rot, &scale);
}

int32_t VS_addStunEffect(DigimonEntity *digimon, int32_t val)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 5; i++) {
		if (VS_D_800730B0[i][0] == -1) {
			break;
		}
	}

	if (i == 5) {
		return -1;
	}

	p = VS_D_800730B0[i];
	p[0] = 0;
	p[1] = val;
	p[2] = 0;
	*(int32_t *)&p[4] = (int32_t)digimon;
	addObject(0x80f, i, VS_tickStunEffect, VS_renderStunEffect);

	return i;
}

void VS_removeStunEffect(int32_t i, DigimonEntity *digimon)
{
	int16_t *p;

	p = VS_D_800730B0[i];
	if ((i >= 0) && (i < 5) && (*(int32_t *)&p[4] == (int32_t)digimon)) {
		p[0] = -1;
		removeObject(0x80f, i);
	}
}

void VS_removeAllStunEffects(void)
{
	int32_t i;

	VS_removeAllStunSubEffects();
	for (i = 0; i < 5; i++) {
		VS_D_800730B0[i][0] = -1;
		removeObject(0x80f, i);
	}
}

void VS_setTMDObjectColor(int32_t idx, int32_t *color, char *base)
{
	int32_t *rec;
	int32_t i;
	int32_t count;
	int32_t t;

	char (*pr)[0x20];
	char (*pg)[0x20];
	char (*pb)[0x20];

	rec = (int32_t *)((int32_t)((uint32_t)base + 0xc) + (idx * 28));
	count = rec[5];
	rec = (int32_t *)rec[4];
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

void VS_tickFinisherAura(int32_t i)
{
	int16_t *p;

	p = &VS_D_800731B4[i * 4];
	p[0]++;
	if (p[0] > p[1]) {
		removeObject(0x80d, i);
		p[0] = -1;
	}
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_renderFinisherAura);

void VS_renderFinisherAuraSpark(char *pos, int32_t scale, SVECTOR *dir, uint8_t *col)
{
	POLY_FT4 *prim;
	SVECTOR c;
	SVECTOR a;
	SVECTOR b;

	prim = (POLY_FT4 *)GsGetWorkBase();
	MAIN_func_80092B60(prim);
	SetSemiTrans(prim, 1);
	prim->r0 = col[0];
	prim->g0 = col[1];
	prim->b0 = col[2];
	prim->tpage = 0x3c;
	prim->clut = 0x7a4c;
	prim->u0 = 0x30;
	prim->v0 = 0xa8;
	prim->u1 = 0x37;
	prim->v1 = 0xa8;
	prim->u2 = 0x30;
	prim->v2 = 0xaf;
	a.vx = dir[0].vx * scale / 4096;
	a.vy = dir[0].vy * scale / 4096;
	a.vz = dir[0].vz * scale / 4096;
	b.vx = dir[1].vx * scale / 4096;
	b.vy = dir[1].vy * scale / 4096;
	b.vz = dir[1].vz * scale / 4096;
	c.vx = ((int32_t *)pos)[0];
	c.vy = ((int32_t *)pos)[1];
	c.vz = ((int32_t *)pos)[2];
	a.vx += (int16_t)((int32_t *)pos)[0];
	a.vy += (int16_t)((int32_t *)pos)[1];
	a.vz += (int16_t)((int32_t *)pos)[2];
	b.vx += (int16_t)((int32_t *)pos)[0];
	b.vy += (int16_t)((int32_t *)pos)[1];
	b.vz += (int16_t)((int32_t *)pos)[2];
	setSemiTrans(prim, 1);
	addScreenPolyFT3(prim, &c, &a, &b);
}

void VS_initializeFinisherAuraModel(char *tim, char *base)
{
	MATRIX m;
	SVECTOR v;
	SVECTOR rot;
	GsIMAGE image;
	RECT rect;
	VECTOR ca;
	VECTOR cb;
	int32_t j;
	int32_t i;
	SVECTOR *p;

	GsGetTimInfo((unsigned long *)tim + 1, &image);
	rect.x = image.px;
	rect.y = image.py;
	rect.w = image.pw;
	rect.h = image.ph;
	LoadImage(&rect, image.pixel);
	GetTPage(image.pmode & 3, 0, image.px, image.py);
	if (((image.pmode >> 3) & 1) != 0) {
		rect.x = image.cx;
		rect.y = image.cy;
		rect.w = image.cw;
		rect.h = image.ch;
		LoadImage(&rect, image.clut);
		GetClut(image.cx, image.cy);
	}

	MAIN_D_801352FC = (int32_t)base;
	GsMapModelingData((unsigned long *)((char *)MAIN_D_801352FC + 4));
	ca = VS_D_80071054;
	cb = VS_D_80071064;
	VS_setTMDObjectColor(0, (int32_t *)&ca, (char *)MAIN_D_801352FC);
	VS_setTMDObjectColor(1, (int32_t *)&ca, (char *)MAIN_D_801352FC);
	VS_setTMDObjectColor(2, (int32_t *)&ca, (char *)MAIN_D_801352FC);
	VS_setTMDObjectColor(3, (int32_t *)&cb, (char *)MAIN_D_801352FC);
	VS_setTMDObjectColor(4, (int32_t *)&ca, (char *)MAIN_D_801352FC);

	for (j = 0; j < 2; j++) {
		((int16_t (*)[4])VS_D_800731B4)[j][0] = -1;
	}

	p = VS_D_800731C4;
	for (i = 0; i < 0xa; i++) {
		v.vx = (rand() % 30) + 30;
		v.vy = 0;
		v.vz = (rand() % 0x190) + 0x320;
		rot.vx = rand() % 0x1000 * 180 / 360;
		rot.vy = rand();
		rot.vz = 0;
		RotMatrixZYX(&rot, &m);
		ApplyMatrixSV(&m, &v, p++);
		v.vx = -v.vx;
		ApplyMatrixSV(&m, &v, p++);
	}
}

int32_t VS_addFinisherAura(int32_t arg, int32_t val)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 2; i++) {
		if (((int16_t (*)[4])VS_D_800731B4)[i][0] < 0) {
			break;
		}
	}

	if (i == 2) {
		return -1;
	}

	p = &VS_D_800731B4[i * 4];
	p[0] = 0;
	p[1] = val;
	*(int32_t *)&p[2] = arg;
	addObject(0x80d, i, VS_tickFinisherAura, VS_renderFinisherAura);

	return i;
}

void VS_removeFinisherAura(int32_t i)
{
	removeObject(0x80d, i);
	VS_D_800731B4[i * 4] = -1;
}

void VS_removeAllFinisherAuras(void)
{
	int32_t i;

	for (i = 0; i < 2; i++) {
		VS_removeFinisherAura(i);
	}
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_effect", VS_tickAuraProjectile);

void VS_renderAuraProjectile(int32_t i)
{
	MATRIX m;
	SVECTOR a;
	SVECTOR b;
	SVECTOR c;
	SVECTOR d;
	char *p;
	POLY_FT4 *prim;

	p = MAIN_D_80135300 + (i * 36);
	prim = (POLY_FT4 *)GsGetWorkBase();
	RotMatrix((SVECTOR *)(p + 0xc), &m);
	ApplyMatrixSV(&m, &MAIN_D_80134B70, &a);
	ApplyMatrixSV(&m, &MAIN_D_80134B78, &b);
	ApplyMatrixSV(&m, &MAIN_D_80134B80, &c);
	ApplyMatrixSV(&m, &MAIN_D_80134B88, &d);
	a.vx += ((int16_t *)p)[2];
	a.vy += ((int16_t *)p)[3];
	a.vz += ((int16_t *)p)[4];
	b.vx += ((int16_t *)p)[2];
	b.vy += ((int16_t *)p)[3];
	b.vz += ((int16_t *)p)[4];
	c.vx += ((int16_t *)p)[2];
	c.vy += ((int16_t *)p)[3];
	c.vz += ((int16_t *)p)[4];
	d.vx += ((int16_t *)p)[2];
	d.vy += ((int16_t *)p)[3];
	d.vz += ((int16_t *)p)[4];
	SetPolyFT4(prim);
	setSemiTrans(prim, 1);
	prim->r0 = prim->g0 = prim->b0 = 0x80;
	prim->tpage = 0x3c;
	prim->clut = 0x7b0c;
	prim->u0 = ((int16_t *)((char **)p)[7])[5] + 0x60;
	prim->v0 = 0xa0;
	prim->u1 = ((int16_t *)((char **)p)[7])[5] + 0x67;
	prim->v1 = 0xa0;
	prim->u2 = ((int16_t *)((char **)p)[7])[5] + 0x60;
	prim->v2 = 0xa7;
	prim->u3 = ((int16_t *)((char **)p)[7])[5] + 0x67;
	prim->v3 = 0xa7;
	add3DSpritePrim(prim, &a, &b, &c, &d);
}

char *VS_initializeAuraProjectiles(char *base)
{
	int32_t i;
	int32_t off;

	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	MAIN_D_80135300 = base;
	base = (char *)((int32_t)base + 0x120);
	for (i = 0, off = 0; i < 8; i++, off += 0x24) {
		*(int32_t *)(MAIN_D_80135300 + off) = -1;
	}

	return base;
}

int32_t VS_addAuraProjectile(Entity *e)
{
	EfeAura *p;
	MATRIX *q;
	char *tbl;
	int32_t i;

	p = (EfeAura *)MAIN_D_80135300;
	tbl = (char *)VS_D_80071084 + (getOriginalType(e->type) * 12);
	for (i = 0; i < 8; i++) {
		if (p[i].frame < 0) {
			break;
		}
	}

	if (i == 8) {
		return -1;
	}

	p = (EfeAura *)((int32_t)p + (i * 36));
	p->frame = 0;
	p->owner = e;
	p->typeData = tbl;
	q = (MATRIX *)((char *)e->posData + 0x34);
	p->position.vx = 0;
	p->position.vy = -DIGIMON_DATA[e->type].height * 50 / 100;
	p->position.vz = -DIGIMON_DATA[e->type].height * 50 / 100;
	ApplyMatrixSV(q, &p->position, &p->position);
	p->position.vx += (int16_t)q->t[0];
	p->position.vy += (int16_t)q->t[1];
	p->position.vz += (int16_t)q->t[2];
	p->rotation.vx = 0;
	p->rotation.vy = *(int16_t *)((char *)e->posData + 0x72);
	p->rotation.vz = 0;
	p->velocity.vx = 0;
	p->velocity.vy = 0;
	p->velocity.vz = -0x23;
	ApplyMatrixSV(q, &p->velocity, &p->velocity);
	addObject(0x179, (int16_t)i, VS_tickAuraProjectile, VS_renderAuraProjectile);

	return i;
}

void VS_removeAllAuraProjectiles(void)
{
	int32_t i;
	int32_t off;

	for (i = 0, off = 0; i < 8; i++, off += 0x24) {
		if (*(int32_t *)(MAIN_D_80135300 + off) >= 0) {
			*(int32_t *)(MAIN_D_80135300 + off) = -1;
			removeObject(0x179, (int16_t)i);
		}
	}
}
