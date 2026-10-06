#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <inline_n.h>
#include <libcd.h>
#include <libetc.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/aabb.h>
#include <dw/attack_object.h>
#include <dw/battle.h>
#include <dw/btl.h>
#include <dw/efe.h>
#include <dw/garbage.h>
#include <dw/graphics.h>
#include <dw/line.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/rng.h>
#include <dw/sound.h>
#include <dw/vecmath.h>
#include <dw/world_object.h>

#include "common.h"

#if defined(VERSION_JP)
#define BTL_EFFECT_CONST
#else
#define BTL_EFFECT_CONST const
#endif

typedef struct {
	int32_t opcode;
	void (*handler)(void);
} EFESubOpcode;

extern DigimonEntity *BATTLE_TARGETED_DIGIMON;
extern DigimonEntity *BATTLE_ATTACKING_DIGIMON;
extern int16_t MAIN_D_80134CDC;
extern int32_t VIEWPORT_DISTANCE;
extern int32_t UNKNOWN_MODEL_TAKEN[16];
extern uint8_t *BUFF_MODEL[];

void setRotTransMatrix(MATRIX *m);
void setMapLayerEnabled(int32_t enabled);
void GsGetTimInfo(unsigned long *tim, GsIMAGE *img);
void updateTMDTextureData(char *tmd, int32_t clutX, int32_t x, int32_t y, int32_t tpage);
void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
int32_t addScreenPolyFT4(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
void renderSprite(GsSPRITE *sprite, int32_t x, int32_t y, int32_t distance, int32_t width, int32_t height);
void translateConditionFXToEntity(Entity *entity, SVECTOR *out);
CdlLOC *getEFEDATEntry(int32_t id);
int32_t addFileReadRequest(char *path, uint8_t *buffer, uint8_t *isRunning, void *callback, void *callbackParam, CdlLOC *loc, int32_t size);
void BTL_renderPoisonBubble(int32_t i);
int32_t doSomethingWithSomePoints(int16_t *rect, DVECTOR *line);
void BTL_applyLineAttackHit(void);
void BTL_renderRadialWaves(void);
void BTL_renderRibbonStrip(void);
char *initializeFlashData(char *base);
void BTL_renderRingTube(void);
void BTL_renderScrollingBackground(void);
void BTL_applyHomingMovement(void);
void renderParticleFlash(ParticleFlashData *params);
void getDrawingOffsetCopy(int32_t *x, int32_t *y);
int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t);
void BTL_tickStunEffect(int32_t i);
void BTL_renderStunSubEffect(int32_t i);
void BTL_renderFinisherAura(int32_t idx);
void renderTMDModel(uint8_t *buffer, int32_t id, GsCOORDINATE2 *coord, GsCOORDINATE2 *super, VECTOR *trans, SVECTOR *rot, VECTOR *scale);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out);
void BTL_renderConfusionEffect(int32_t i);
int32_t getOriginalType(int32_t type);
void BTL_loadNextEFEFile(EfeLoad *arg);
void BTL_runEFESlotScript(int32_t i);
void getRViewCopy(GsRVIEW2 *view);
void getViewportDistanceCopy(int32_t *out);
int32_t BTL_addPoisonBubble(Entity *entity);
void createCloudFX(SVECTOR *pos);
void BTL_removeFinisherAura(int32_t index);
int32_t addEntityParticleFX(Entity *owner, int32_t timer);
void setInt16WithStride(int16_t *ptr, int16_t value, int32_t count, int32_t stride);
void setFileReadCallback2(void *callback, void *param);
int32_t BTL_setupLoadedEFEFile(EfeLoad *load);
void BTL_stopEFESounds(void);
void BTL_renderPoisonEffect(void);
void BTL_handleEFEFileLoaded(EfeLoad *load);
int32_t BTL_getEFEHeapPointer(void);
void BTL_markEFEFinished(void);
void BTL_getViewportDistance2(void);
void BTL_disableMapLayer(void);
void BTL_getViewportDistance(void);
void BTL_discardEFEOperand(void);
void BTL_printDebugValue(void);
void BTL_discardEFEOperandPair(void);
void BTL_checkCollisionWithDefaultPower(void);
void BTL_pushEFEImmediate(void);
void BTL_jumpEFEScript(void);
void BTL_stopEFEScript(void);
void BTL_loadEFEImmediate(void);
int32_t BTL_shiftRightInt32Variable(int32_t p);
int32_t BTL_shiftLeftInt32Variable(int32_t p);
int32_t BTL_moduloInt32Variable(int32_t p);
int32_t BTL_divideInt32Variable(int32_t p);
int32_t BTL_multiplyInt32Variable(int32_t p);
int32_t BTL_subtractInt32Variable(int32_t p);
int32_t BTL_addInt32Variable(int32_t p);
int32_t BTL_setInt32Variable(int32_t p);
int32_t BTL_shiftRightInt8Variable(int32_t p);
int32_t BTL_shiftLeftInt8Variable(int32_t p);
int32_t BTL_moduloInt8Variable(int32_t p);
int32_t BTL_divideInt8Variable(int32_t p);
int32_t BTL_multiplyInt8Variable(int32_t p);
int32_t BTL_subtractInt8Variable(int32_t p);
int32_t BTL_addInt8Variable(int32_t p);
int32_t BTL_setInt8Variable(int32_t p);
int32_t BTL_shiftRightInt16Variable(int32_t p);
int32_t BTL_shiftLeftInt16Variable(int32_t p);
int32_t BTL_moduloInt16Variable(int32_t p);
int32_t BTL_divideInt16Variable(int32_t p);
int32_t BTL_multiplyInt16Variable(int32_t p);
int32_t BTL_subtractInt16Variable(int32_t p);
int32_t BTL_addInt16Variable(int32_t p);
int32_t BTL_setInt16Variable(int32_t p);
int32_t BTL_compareGreaterOrEqual(int32_t x);
int32_t BTL_compareGreater(int32_t x);
int32_t BTL_compareLessOrEqual(int32_t x);
int32_t BTL_compareLess(int32_t x);
int32_t BTL_compareNotEqual(int32_t x);
int32_t BTL_compareEqual(int32_t x);
void BTL_dispatchEFEOpcode(int32_t op);
void BTL_resetPoisonBubbles(void);
char *BTL_initializeParticleEmitters(char *base);
void BTL_tickEFEEngine(void);
void BTL_renderEFEEngine(void);
void BTL_clearEFESoundChannels(void);
void BTL_unloadEFESlot(int32_t idx);
void BTL_tickEFEUVAnimation(int32_t idx);
void BTL_tickParticleEmitters(void);
void BTL_renderParticleEmitters(void);
int16_t BTL_offsetEFEPrimitiveUVs(char *base, int32_t idx, int8_t du, int8_t dv);
char *BTL_initializeEFEEngine(char *base);
void BTL_loadMoveEFE(int16_t *moves, int16_t *effectIds, int8_t *isLoaded);
int32_t BTL_startEFE(int32_t i);
void BTL_stopEFESubEffect(int32_t a, int32_t b);
char *BTL_getEFETextureSection(char *p);
char *BTL_getEFEModelSection(char *p);
int32_t BTL_getEFEFileId(char *data);
void BTL_isTargetUnhit(void);
void BTL_renderScreenFade(void);
void BTL_applyBoxAttackHit(void);
void BTL_applyRadiusAttackHit(void);
void BTL_faceTargetEntity(void);
void BTL_renderScreenOverlay(void);
void BTL_tickRibbonPoints(void);
void BTL_initializeRibbonPoints(void);
void BTL_addClutLoadPrim(void);
void BTL_drawTMDScreenSpace(void);
void BTL_loadClutColors(void);
void BTL_drawTMDYXZ(void);
void BTL_getCameraRotation(void);
void BTL_selectRandomTargetEntity(void);
void BTL_convertToViewSpace(void);
void BTL_maskVectorByScalar(void);
void BTL_divideVectorByScalar(void);
void BTL_multiplyVectorByScalar(void);
void BTL_render3DTexturedQuad(void);
void BTL_setTransformToBoneMatrix(void);
void BTL_renderWireframeBox(void);
void BTL_renderWireframeGrid(void);
void BTL_render2DTexturedQuad(void);
void BTL_restoreCameraView(void);
void BTL_setupFixedCamera(void);
void BTL_getSourceBoneTransform(void);
void BTL_copyToParentTransform(void);
void BTL_combineRotations(void);
void BTL_normalizeRotationAngles2(void);
void BTL_rotateVectorByAngles(void);
void EFECreateFlash(void);
void EFERotateVector(void);
void BTL_getTargetBoneTransform(void);
void BTL_centerTransformOnEntities(void);
void BTL_shiftVectorsRight(void);
void BTL_maskVectors(void);
void BTL_divideVectors(void);
void BTL_multiplyVectors(void);
void BTL_subtractVectors(void);
void BTL_addVectors(void);
void BTL_copyVector(void);
void BTL_getVectorLength(void);
void BTL_setTargetToHitEntity(void);
void BTL_normalizeRotationAngles(void);
void BTL_findHitEntity(void);
void BTL_getVectorEulerAngles(void);
void BTL_getRandomInRange(void);
void BTL_interpolateValue(void);
void BTL_calculateCosine(void);
void BTL_calculateSine(void);
void BTL_getSourceDigimonSize(void);
void BTL_getUVAnimTimer(void);
void BTL_checkTargetCollision(void);
void BTL_rotateTransformTowardPoint(void);
void BTL_setTransformToSourceBone(void);
void BTL_renderParallaxSprites(void);
void BTL_setTransformToBoneOffset(void);
void BTL_playEFESound(void);
void BTL_addSourceEntityParticleFX(void);
void BTL_copyFromParentTransform(void);
void BTL_calculatePolarOffset(void);
void BTL_renderProjectedSprite(void);
void BTL_getTargetDigimonSize(void);
void BTL_renderParticleFlashSprite(void);
void BTL_projectPositionToScreen(void);
void BTL_renderScreenSprite(void);
void BTL_addCloudEffect(void);
void BTL_selectNextTargetEntity(void);
void BTL_addParticleEmitter(void);
void BTL_setEFEModelObjectColor(void);
void BTL_copyTargetEntityPosition(void);
void BTL_steerTransformTowardPoint(void);
void BTL_interpolateVector(void);
void BTL_getScatteredSpawnPosition(void);
void BTL_addAttackObjectToTarget(void);
void BTL_setTransformToTargetBone(void);
void BTL_renderCenteredSprite(void);
void BTL_initializeEFETransform(void);
void BTL_drawTMD(void);
void BTL_initializeSubEffectInstructions(void);
void BTL_initializeUVAnim(void);
void BTL_checkTechCompatibility(void);
void BTL_spawnEFESubEffect(void);
void BTL_popEFEValueToVariable(void);
void BTL_returnFromEFESubroutine(void);
void BTL_dispatchEFESubOpcode(void);
void BTL_callEFESubroutine(void);
void BTL_pushEFEVariableAddress(void);
void BTL_pushEFEVariable(void);
void BTL_branchEFEOnComparison(void);
void BTL_applyEFEVariableOperator(void);
void BTL_loadEFEIndexedVariable(void);
void BTL_loadEFERandomValue(void);
void BTL_loadEFEVariable(void);
int16_t BTL_calculateAttackHitPosition(SVECTOR *out, Entity *self, Entity *other, int32_t y);
void BTL_renderParallelLines(SVECTOR *a, SVECTOR *b, int16_t n, SVECTOR *from, SVECTOR *to, int32_t *col);
int32_t BTL_interpolateClamped(int32_t lo, int32_t hi, long t, int32_t start, int32_t end);
void BTL_initializeEFESubOpcodeTable(void);
int32_t BTL_runEFEScript(int32_t script);
void BTL_tickPoisonBubble(int32_t i);
void BTL_tickPoisonEffect(int32_t i);
void BTL_initializePoisonBubble(void);
int32_t BTL_addPoisonEffect(Entity *entity);
void BTL_removePoisonEffect(int32_t i, Entity *entity);
void BTL_tickConfusionEffect(int32_t i);
void BTL_initializeConfusionEffect(char *base);
int32_t BTL_addConfusionEffect(Entity *entity);
void BTL_removeConfusionEffect(int32_t i, Entity *entity);
void BTL_initializeStunEffect(char *base);
void BTL_resetStunSubEffects(void);
void BTL_renderStunEffect(int32_t idx);
void BTL_removeAllStunSubEffects(void);
int32_t BTL_addStunSubEffect(Entity *entity);
void BTL_tickStunSubEffect(int32_t i);
int32_t BTL_addStunEffect(Entity *entity, int32_t val);
void BTL_removeStunEffect(int32_t i, Entity *entity);
void BTL_setTMDObjectColor(int32_t idx, int32_t *color, int32_t base);
void BTL_tickFinisherAura(int32_t i);
void BTL_renderFinisherAuraSpark(VECTOR *pos, int32_t scale, SVECTOR *dir, uint8_t *col);
void BTL_initializeFinisherAuraModel(char *tim, char *base);
int32_t BTL_addFinisherAura(Entity *entity, int32_t duration);
void BTL_tickAuraProjectile(int32_t id);
void BTL_renderAuraProjectile(int32_t i);
char *BTL_initializeAuraProjectiles(char *base);
int32_t BTL_addAuraProjectile(Entity *e);
void BTL_renderEFELine(void);
void BTL_tickBuffDisk(int32_t i);
void BTL_renderBuffRings(int32_t i);
void renderFXParticle(SVECTOR *worldPos, int32_t scale, RGB8 *rgb);
void BTL_tickBuffTrails(int32_t i);
void BTL_renderBuffTrails(int32_t i);
void BTL_removeItemParticles(int32_t index);
void BTL_removeBuffDiskEffect(int32_t index);
void BTL_initializeItemParticleVelocities(void);
void BTL_initializeBuffTrails(void);
void BTL_removeBuffTrails(int32_t instanceId);
void BTL_initializeUnk3(void);
void BTL_tickItemParticles(int32_t idx);
void BTL_renderItemParticles(int32_t idx);
void BTL_initializeBattleItemParticles(void);
int32_t BTL_addItemParticles(Entity *e);
void BTL_renderBuffDisk(int32_t i);
void BTL_addBuffTrails(int32_t i, Entity *e);
int32_t BTL_addBuffDiskEffect(Entity *e);
void BTL_tickBuffRings(int32_t idx);
void BTL_renderBuffRingsSpark(VECTOR *pos, int32_t scale, SVECTOR *dir, uint8_t *col);
void BTL_initializeUnk2(void);
int32_t BTL_addBuffRingsEffect(int32_t idx, Entity *e);
long RotTransPers4(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, long *sxy0, long *sxy1, long *sxy2, long *sxy3, long *p, long *flag);

static void *battle_effect_functions[] = {
	BTL_removeAllBuffRingsEffects,
	BTL_addBuffRingsEffect,
	BTL_initializeUnk2,
	BTL_renderBuffRingsSpark,
	BTL_renderBuffRings,
	BTL_tickBuffRings,
	BTL_removeAllBuffDiskEffects,
	BTL_removeBuffDiskEffect,
	BTL_addBuffDiskEffect,
	BTL_initializeUnk3,
	BTL_renderBuffTrails,
	BTL_tickBuffTrails,
	BTL_removeBuffTrails,
	BTL_addBuffTrails,
	BTL_renderBuffDisk,
	BTL_tickBuffDisk,
	BTL_initializeBuffTrails,
	BTL_removeAllItemParticles,
	BTL_removeItemParticles,
	BTL_addItemParticles,
	BTL_initializeBattleItemParticles,
	BTL_renderItemParticles,
	BTL_tickItemParticles,
	BTL_initializeItemParticleVelocities,
	BTL_removeAllAuraProjectiles,
	BTL_addAuraProjectile,
	BTL_initializeAuraProjectiles,
	BTL_renderAuraProjectile,
	BTL_tickAuraProjectile,
	BTL_removeAllFinisherAuras,
	BTL_removeFinisherAura,
	BTL_addFinisherAura,
	BTL_initializeFinisherAuraModel,
	BTL_renderFinisherAuraSpark,
	BTL_renderFinisherAura,
	BTL_tickFinisherAura,
	BTL_setTMDObjectColor,
	BTL_removeAllStunEffects,
	BTL_removeStunEffect,
	BTL_addStunEffect,
	BTL_initializeStunEffect,
	BTL_renderStunSubEffect,
	BTL_tickStunSubEffect,
	BTL_addStunSubEffect,
	BTL_removeAllStunSubEffects,
	BTL_renderStunEffect,
	BTL_tickStunEffect,
	BTL_resetStunSubEffects,
	BTL_removeConfusionEffect,
	BTL_addConfusionEffect,
	BTL_initializeConfusionEffect,
	BTL_renderConfusionEffect,
	BTL_tickConfusionEffect,
	BTL_removeAllPoisonEffects,
	BTL_removePoisonEffect,
	BTL_addPoisonEffect,
	BTL_initializePoisonBubble,
	BTL_renderPoisonEffect,
	BTL_tickPoisonEffect,
	BTL_renderPoisonBubble,
	BTL_tickPoisonBubble,
	BTL_addPoisonBubble,
	BTL_resetPoisonBubbles,
	BTL_runEFEScript,
	BTL_dispatchEFEOpcode,
	BTL_initializeEFESubOpcodeTable,
	BTL_interpolateClamped,
	BTL_renderParallelLines,
	BTL_calculateAttackHitPosition,
	BTL_compareEqual,
	BTL_compareNotEqual,
	BTL_compareLess,
	BTL_compareLessOrEqual,
	BTL_compareGreater,
	BTL_compareGreaterOrEqual,
	BTL_setInt16Variable,
	BTL_addInt16Variable,
	BTL_subtractInt16Variable,
	BTL_multiplyInt16Variable,
	BTL_divideInt16Variable,
	BTL_moduloInt16Variable,
	BTL_shiftLeftInt16Variable,
	BTL_shiftRightInt16Variable,
	BTL_setInt8Variable,
	BTL_addInt8Variable,
	BTL_subtractInt8Variable,
	BTL_multiplyInt8Variable,
	BTL_divideInt8Variable,
	BTL_moduloInt8Variable,
	BTL_shiftLeftInt8Variable,
	BTL_shiftRightInt8Variable,
	BTL_setInt32Variable,
	BTL_addInt32Variable,
	BTL_subtractInt32Variable,
	BTL_multiplyInt32Variable,
	BTL_divideInt32Variable,
	BTL_moduloInt32Variable,
	BTL_shiftLeftInt32Variable,
	BTL_shiftRightInt32Variable,
	BTL_loadEFEImmediate,
	BTL_loadEFEVariable,
	BTL_loadEFERandomValue,
	BTL_loadEFEIndexedVariable,
	BTL_applyEFEVariableOperator,
	BTL_branchEFEOnComparison,
	BTL_stopEFEScript,
	BTL_jumpEFEScript,
	BTL_pushEFEImmediate,
	BTL_pushEFEVariable,
	BTL_pushEFEVariableAddress,
	BTL_callEFESubroutine,
	BTL_dispatchEFESubOpcode,
	BTL_returnFromEFESubroutine,
	BTL_popEFEValueToVariable,
	BTL_spawnEFESubEffect,
	BTL_checkTechCompatibility,
	BTL_initializeUVAnim,
	BTL_initializeSubEffectInstructions,
	BTL_drawTMD,
	BTL_initializeEFETransform,
	BTL_renderCenteredSprite,
	BTL_setTransformToTargetBone,
	BTL_addAttackObjectToTarget,
	BTL_checkCollisionWithDefaultPower,
	BTL_getScatteredSpawnPosition,
	BTL_discardEFEOperandPair,
	BTL_interpolateVector,
	BTL_steerTransformTowardPoint,
	BTL_copyTargetEntityPosition,
	BTL_setEFEModelObjectColor,
	BTL_addParticleEmitter,
	BTL_selectNextTargetEntity,
	BTL_addCloudEffect,
	BTL_renderScreenSprite,
	BTL_projectPositionToScreen,
	BTL_renderParticleFlashSprite,
	BTL_getTargetDigimonSize,
	BTL_renderProjectedSprite,
	BTL_calculatePolarOffset,
	BTL_copyFromParentTransform,
	BTL_addSourceEntityParticleFX,
	BTL_playEFESound,
	BTL_setTransformToBoneOffset,
	BTL_renderScrollingBackground,
	BTL_renderParallaxSprites,
	BTL_setTransformToSourceBone,
	BTL_rotateTransformTowardPoint,
	BTL_checkTargetCollision,
	BTL_getUVAnimTimer,
	BTL_applyHomingMovement,
	BTL_getSourceDigimonSize,
	BTL_calculateSine,
	BTL_calculateCosine,
	BTL_interpolateValue,
	BTL_getRandomInRange,
	BTL_printDebugValue,
	BTL_getVectorEulerAngles,
	BTL_findHitEntity,
	BTL_normalizeRotationAngles,
	BTL_setTargetToHitEntity,
	BTL_getVectorLength,
	BTL_copyVector,
	BTL_addVectors,
	BTL_subtractVectors,
	BTL_multiplyVectors,
	BTL_divideVectors,
	BTL_maskVectors,
	BTL_shiftVectorsRight,
	BTL_centerTransformOnEntities,
	BTL_getTargetBoneTransform,
	BTL_rotateVectorByAngles,
	BTL_normalizeRotationAngles2,
	BTL_combineRotations,
	BTL_renderEFELine,
	BTL_copyToParentTransform,
	BTL_getSourceBoneTransform,
	BTL_setupFixedCamera,
	BTL_restoreCameraView,
	BTL_render2DTexturedQuad,
	BTL_renderWireframeGrid,
	BTL_discardEFEOperand,
	BTL_renderWireframeBox,
	BTL_setTransformToBoneMatrix,
	BTL_render3DTexturedQuad,
	BTL_multiplyVectorByScalar,
	BTL_divideVectorByScalar,
	BTL_maskVectorByScalar,
	BTL_convertToViewSpace,
	BTL_selectRandomTargetEntity,
	BTL_getCameraRotation,
	BTL_drawTMDYXZ,
	BTL_loadClutColors,
	BTL_drawTMDScreenSpace,
	BTL_addClutLoadPrim,
	BTL_getViewportDistance,
	BTL_renderRadialWaves,
	BTL_initializeRibbonPoints,
	BTL_tickRibbonPoints,
	BTL_renderRibbonStrip,
	BTL_renderRingTube,
	BTL_renderScreenOverlay,
	BTL_faceTargetEntity,
	BTL_applyLineAttackHit,
	BTL_applyRadiusAttackHit,
	BTL_applyBoxAttackHit,
	BTL_renderScreenFade,
	BTL_disableMapLayer,
	BTL_getViewportDistance2,
	BTL_markEFEFinished,
	BTL_isTargetUnhit,
	BTL_getEFEFileId,
	BTL_getEFEModelSection,
	BTL_getEFETextureSection,
	BTL_getEFEHeapPointer,
	BTL_stopEFESubEffect,
	BTL_startEFE,
	BTL_unloadAllEFESlots,
	BTL_loadMoveEFE,
	BTL_removeEFEEngine,
	BTL_initializeEFEEngine,
	BTL_offsetEFEPrimitiveUVs,
	BTL_renderParticleEmitters,
	BTL_tickParticleEmitters,
	BTL_tickEFEUVAnimation,
	BTL_handleEFEFileLoaded,
	BTL_setupLoadedEFEFile,
	BTL_runEFESlotScript,
	BTL_unloadEFESlot,
	BTL_loadNextEFEFile,
	BTL_stopEFESounds,
	BTL_clearEFESoundChannels,
	BTL_renderEFEEngine,
	BTL_tickEFEEngine,
	BTL_initializeParticleEmitters,
};

char BTL_FMT_D[] = "%d\n";
int8_t BTL_LINE_OFFSET_X[4] = { 1, 0, -1, 0 };
int8_t BTL_LINE_OFFSET_Y[4] = { 0, 1, 0, -1 };
int8_t BTL_WIREFRAME_BOX_SIGN_X[8] = { -1, 1, 1, -1, -1, 1, 1, -1 };
int8_t BTL_WIREFRAME_BOX_SIGN_Y[8] = { -1, -1, 1, 1, -1, -1, 1, 1 };
int8_t BTL_WIREFRAME_BOX_SIGN_Z[8] = { -1, -1, -1, -1, 1, 1, 1, 1 };
int32_t BTL_RADIAL_WAVE_COLOR = 0x808080;
uint8_t BTL_RIBBON_UVS[8] = { 104, 0, 135, 0, 104, 31, 135, 31 };
int8_t BTL_POISON_BUBBLE_FRAME_U[6] = { 0, 16, 32, 48, 64, 80 };
SVECTOR BTL_STUN_FX_ROTATION = { 0 };
int16_t BTL_FINISHER_AURA_OBJECTS[3] = { 0, 1, 2 };
SVECTOR BTL_FINISHER_AURA_ROTATION = { 0 };
RGB8 BTL_FINISHER_AURA_COLOR = { 0xcc, 0xa8, 0x28 };
SVECTOR BTL_AURA_PROJECTILE_VERTEX_0 = { 0, -50, -50, 0 };
SVECTOR BTL_AURA_PROJECTILE_VERTEX_1 = { 0, -50, 50, 0 };
SVECTOR BTL_AURA_PROJECTILE_VERTEX_2 = { 0, 50, -50, 0 };
SVECTOR BTL_AURA_PROJECTILE_VERTEX_3 = { 0, 50, 50, 0 };
uint8_t BTL_BUFF_RING_COLOR_R[4] = { 180, 100, 235, 180 };
uint8_t BTL_BUFF_RING_COLOR_G[4] = { 20, 20, 150, 180 };
uint8_t BTL_BUFF_RING_COLOR_B[4] = { 255, 255, 220, 180 };

int32_t BTL_EFE_LOAD_STATE;
int32_t BTL_EFE_LOAD_SLOT;
int32_t BTL_CONFUSION_FX_MODEL;
char *BTL_STUN_FX_MODEL;
int32_t BTL_FINISHER_AURA_MODEL;
EfeAura *BTL_FLAT_BULLET_PTR;

static void *battle_effect_sbss_order[] = {
	&BTL_FLAT_BULLET_PTR,
	&BTL_FINISHER_AURA_MODEL,
	&BTL_STUN_FX_MODEL,
	&BTL_CONFUSION_FX_MODEL,
	&BTL_EFE_LOAD_SLOT,
	&BTL_EFE_LOAD_STATE,
};

// clang-format off
#if defined(VERSION_JP)
char BTL_STR_LISTENS_TO[] = "C1の命令を聞くようになった！W";
char BTL_STR_WHITE_WAIT[] = "C1を落としたW";
char BTL_STR_WAS_INJURED[] = "C1はRケガをしてしまったW";
char BTL_STR_SET_TECHNIQUE[] = "C7「そうびした技」R";
char BTL_STR_PUT_UP_WITH_IT[] = "C7「ガマンだ！」R";
char BTL_STR_MOVE_AWAY_CHANGE_TARGET[] = "C7「はなれろ！」R「ターゲット変更！」R";
char BTL_STR_KEEP_IT_DOWN[] = "C7「ほどほど！」R";
char BTL_STR_GO_ALL_THE_WAY[] = "C7「思いっきり！」R";
char BTL_STR_MP_CONSUMPTION_BONUS[] = "C1かしこさによる「ＭＰ消費減R少ボーナス」を習得した！W技の消費ＭＰがR";
char BTL_STR_MP_BONUS_PERCENT[] = "％だけ減るぞ！W";
char BTL_STR_LEARNED[] = "RC1を覚えた！W";
#else
const char BTL_STR_LISTENS_TO[32] = "Listens to #C1! #W";
const char BTL_STR_DROPPED[] = "#R#C1dropped #C7";
const char BTL_STR_WAS_INJURED[] = "#C1#R was injured #W";
const char BTL_STR_SET_TECHNIQUE[20] = "#C7set technique #R";
const char BTL_STR_PUT_UP_WITH_IT[] = "#C7Put up with it! #R";
const char BTL_STR_MOVE_AWAY_CHANGE_TARGET[32] = "#C7Move away!#RChange target!#R";
const char BTL_STR_KEEP_IT_DOWN[] = "#C7Keep it down!#R";
const char BTL_STR_GO_ALL_THE_WAY[] = "#C7Go all the way!#R";
const char BTL_STR_MP_CONSUMPTION_BONUS[] = "#C1MP Consumption Bonus!";
const char BTL_STR_REDUCED_BY[] = "reduced by";
const char BTL_STR_LEARNED[16] = "#R#C1learned!#W";
#endif

BTL_EFFECT_CONST MATRIX BTL_BATTLE_START_TEXT_MATRIX = {
	{
		0x100a, 0x0000, 0x0000, 0x0000,
		0x08e4, 0xf299, 0x0000, 0x0d5e,
		0x08e9,
	},
	{ 0x00000000, 0xfffffffe, 0x000002d4 },
};

BTL_EFFECT_CONST int16_t BTL_BATTLE_START_TEXT_POSITIONS[155][2] = {
	{ 0xff54, 0xffd0 },
	{ 0xff5c, 0xffd0 },
	{ 0xff64, 0xffd0 },
	{ 0xff6c, 0xffd0 },
	{ 0xff74, 0xffd0 },
	{ 0xff54, 0xffdc },
	{ 0xff5c, 0xffdc },
	{ 0xff74, 0xffdc },
	{ 0xff7c, 0xffdc },
	{ 0xff54, 0xffe8 },
	{ 0xff5c, 0xffe8 },
	{ 0xff74, 0xffe8 },
	{ 0xff7c, 0xffe8 },
	{ 0xff54, 0xfff4 },
	{ 0xff5c, 0xfff4 },
	{ 0xff64, 0xfff4 },
	{ 0xff6c, 0xfff4 },
	{ 0xff74, 0xfff4 },
	{ 0xff54, 0x0000 },
	{ 0xff5c, 0x0000 },
	{ 0xff74, 0x0000 },
	{ 0xff7c, 0x0000 },
	{ 0xff54, 0x000c },
	{ 0xff5c, 0x000c },
	{ 0xff74, 0x000c },
	{ 0xff7c, 0x000c },
	{ 0xff54, 0x0018 },
	{ 0xff5c, 0x0018 },
	{ 0xff74, 0x0018 },
	{ 0xff7c, 0x0018 },
	{ 0xff54, 0x0024 },
	{ 0xff5c, 0x0024 },
	{ 0xff64, 0x0024 },
	{ 0xff6c, 0x0024 },
	{ 0xff74, 0x0024 },
	{ 0xffa4, 0xffd0 },
	{ 0xffac, 0xffd0 },
	{ 0xffa4, 0xffdc },
	{ 0xffac, 0xffdc },
	{ 0xff9c, 0xffe8 },
	{ 0xffa4, 0xffe8 },
	{ 0xffac, 0xffe8 },
	{ 0xffb4, 0xffe8 },
	{ 0xff94, 0xfff4 },
	{ 0xff9c, 0xfff4 },
	{ 0xffb4, 0xfff4 },
	{ 0xffbc, 0xfff4 },
	{ 0xff94, 0x0000 },
	{ 0xff9c, 0x0000 },
	{ 0xffb4, 0x0000 },
	{ 0xffbc, 0x0000 },
	{ 0xff8c, 0x000c },
	{ 0xff94, 0x000c },
	{ 0xff9c, 0x000c },
	{ 0xffa4, 0x000c },
	{ 0xffac, 0x000c },
	{ 0xffb4, 0x000c },
	{ 0xffbc, 0x000c },
	{ 0xffc4, 0x000c },
	{ 0xff8c, 0x0018 },
	{ 0xff94, 0x0018 },
	{ 0xffbc, 0x0018 },
	{ 0xffc4, 0x0018 },
	{ 0xff8c, 0x0024 },
	{ 0xff94, 0x0024 },
	{ 0xffbc, 0x0024 },
	{ 0xffc4, 0x0024 },
	{ 0xffd4, 0xffd0 },
	{ 0xffdc, 0xffd0 },
	{ 0xffe4, 0xffd0 },
	{ 0xffec, 0xffd0 },
	{ 0xfff4, 0xffd0 },
	{ 0xfffc, 0xffd0 },
	{ 0xffe4, 0xffdc },
	{ 0xffec, 0xffdc },
	{ 0xffe4, 0xffe8 },
	{ 0xffec, 0xffe8 },
	{ 0xffe4, 0xfff4 },
	{ 0xffec, 0xfff4 },
	{ 0xffe4, 0x0000 },
	{ 0xffec, 0x0000 },
	{ 0xffe4, 0x000c },
	{ 0xffec, 0x000c },
	{ 0xffe4, 0x0018 },
	{ 0xffec, 0x0018 },
	{ 0xffe4, 0x0024 },
	{ 0xffec, 0x0024 },
	{ 0x0014, 0xffd0 },
	{ 0x001c, 0xffd0 },
	{ 0x0024, 0xffd0 },
	{ 0x002c, 0xffd0 },
	{ 0x0034, 0xffd0 },
	{ 0x003c, 0xffd0 },
	{ 0x0024, 0xffdc },
	{ 0x002c, 0xffdc },
	{ 0x0024, 0xffe8 },
	{ 0x002c, 0xffe8 },
	{ 0x0024, 0xfff4 },
	{ 0x002c, 0xfff4 },
	{ 0x0024, 0x0000 },
	{ 0x002c, 0x0000 },
	{ 0x0024, 0x000c },
	{ 0x002c, 0x000c },
	{ 0x0024, 0x0018 },
	{ 0x002c, 0x0018 },
	{ 0x0024, 0x0024 },
	{ 0x002c, 0x0024 },
	{ 0x004c, 0xffd0 },
	{ 0x0054, 0xffd0 },
	{ 0x004c, 0xffdc },
	{ 0x0054, 0xffdc },
	{ 0x004c, 0xffe8 },
	{ 0x0054, 0xffe8 },
	{ 0x004c, 0xfff4 },
	{ 0x0054, 0xfff4 },
	{ 0x004c, 0x0000 },
	{ 0x0054, 0x0000 },
	{ 0x004c, 0x000c },
	{ 0x0054, 0x000c },
	{ 0x004c, 0x0018 },
	{ 0x0054, 0x0018 },
	{ 0x004c, 0x0024 },
	{ 0x0054, 0x0024 },
	{ 0x005c, 0x0024 },
	{ 0x0064, 0x0024 },
	{ 0x006c, 0x0024 },
	{ 0x0074, 0x0024 },
	{ 0x0084, 0xffd0 },
	{ 0x008c, 0xffd0 },
	{ 0x0094, 0xffd0 },
	{ 0x009c, 0xffd0 },
	{ 0x00a4, 0xffd0 },
	{ 0x00ac, 0xffd0 },
	{ 0x0084, 0xffdc },
	{ 0x008c, 0xffdc },
	{ 0x0084, 0xffe8 },
	{ 0x008c, 0xffe8 },
	{ 0x0084, 0xfff4 },
	{ 0x008c, 0xfff4 },
	{ 0x0094, 0xfff4 },
	{ 0x009c, 0xfff4 },
	{ 0x00a4, 0xfff4 },
	{ 0x00ac, 0xfff4 },
	{ 0x0084, 0x0000 },
	{ 0x008c, 0x0000 },
	{ 0x0084, 0x000c },
	{ 0x008c, 0x000c },
	{ 0x0084, 0x0018 },
	{ 0x008c, 0x0018 },
	{ 0x0084, 0x0024 },
	{ 0x008c, 0x0024 },
	{ 0x0094, 0x0024 },
	{ 0x009c, 0x0024 },
	{ 0x00a4, 0x0024 },
	{ 0x00ac, 0x0024 },
};

BTL_EFFECT_CONST int16_t BTL_STATUS_BAR_X[8] = {
	0x00a0, 0x0081, 0x0068, 0x0053, 0x0042, 0x0037, 0x0030, 0x002e,
};

BTL_EFFECT_CONST int32_t BTL_FINISHER_PULSE[12] = {
	0x00000020, 0x00000040, 0x00000060, 0x00000080,
	0x000000a0, 0x000000c0, 0x000000e0, 0x000000ff,
	0x000000e0, 0x000000c0, 0x000000a0, 0x00000080,
};

BTL_EFFECT_CONST BarSprite BTL_STATUS_BAR_SPRITES[6] = {
	{ 0x01ec, 0x80, 0xa8, 0x68, 0x08, 0x0000, 0x0000 },
	{ 0x01eb, 0x90, 0xb0, 0x0b, 0x0b, 0x0003, 0xfffe },
	{ 0x01eb, 0x80, 0xb0, 0x02, 0x02, 0x0012, 0x0003 },
	{ 0x01ec, 0x80, 0xa8, 0x68, 0x08, 0x0000, 0x0000 },
	{ 0x01eb, 0x9b, 0xb0, 0x0b, 0x0b, 0x0003, 0xfffe },
	{ 0x01eb, 0x84, 0xb0, 0x02, 0x02, 0x0012, 0x0003 },
};

BTL_EFFECT_CONST EFESubOpcode BTL_EFE_SUB_OPCODES[97] = {
	{ 0, BTL_checkTechCompatibility },
	{ 1, BTL_initializeUVAnim },
	{ 2, BTL_initializeSubEffectInstructions },
	{ 3, BTL_drawTMD },
	{ 5, BTL_initializeEFETransform },
	{ 7, EFERotateVector },
	{ 4, BTL_renderCenteredSprite },
	{ 8, BTL_setTransformToTargetBone },
	{ 9, BTL_addAttackObjectToTarget },
	{ 6, BTL_checkCollisionWithDefaultPower },
	{ 10, BTL_getScatteredSpawnPosition },
	{ 11, BTL_discardEFEOperandPair },
	{ 12, BTL_interpolateVector },
	{ 13, BTL_steerTransformTowardPoint },
	{ 14, BTL_copyTargetEntityPosition },
	{ 15, BTL_setEFEModelObjectColor },
	{ 16, BTL_addParticleEmitter },
	{ 17, BTL_selectNextTargetEntity },
	{ 18, EFECreateFlash },
	{ 19, BTL_addCloudEffect },
	{ 20, BTL_renderScreenSprite },
	{ 21, BTL_projectPositionToScreen },
	{ 22, BTL_renderParticleFlashSprite },
	{ 23, BTL_getTargetDigimonSize },
	{ 24, BTL_renderProjectedSprite },
	{ 25, BTL_calculatePolarOffset },
	{ 26, BTL_copyFromParentTransform },
	{ 27, BTL_addSourceEntityParticleFX },
	{ 28, BTL_playEFESound },
	{ 29, BTL_setTransformToBoneOffset },
	{ 30, BTL_renderScrollingBackground },
	{ 31, BTL_renderParallaxSprites },
	{ 32, BTL_setTransformToSourceBone },
	{ 33, BTL_rotateTransformTowardPoint },
	{ 34, BTL_checkTargetCollision },
	{ 35, BTL_getUVAnimTimer },
	{ 36, BTL_applyHomingMovement },
	{ 37, BTL_getSourceDigimonSize },
	{ 38, BTL_calculateSine },
	{ 39, BTL_calculateCosine },
	{ 40, BTL_interpolateValue },
	{ 41, BTL_getRandomInRange },
	{ 42, BTL_printDebugValue },
	{ 43, BTL_getVectorEulerAngles },
	{ 44, BTL_findHitEntity },
	{ 45, BTL_normalizeRotationAngles },
	{ 46, BTL_setTargetToHitEntity },
	{ 47, BTL_getVectorLength },
	{ 48, BTL_copyVector },
	{ 49, BTL_addVectors },
	{ 50, BTL_subtractVectors },
	{ 51, BTL_multiplyVectors },
	{ 52, BTL_divideVectors },
	{ 53, BTL_maskVectors },
	{ 54, BTL_shiftVectorsRight },
	{ 55, BTL_centerTransformOnEntities },
	{ 56, BTL_getTargetBoneTransform },
	{ 57, BTL_rotateVectorByAngles },
	{ 58, BTL_normalizeRotationAngles2 },
	{ 59, BTL_combineRotations },
	{ 60, BTL_renderEFELine },
	{ 61, BTL_copyToParentTransform },
	{ 62, BTL_getSourceBoneTransform },
	{ 63, BTL_setupFixedCamera },
	{ 64, BTL_restoreCameraView },
	{ 65, BTL_render2DTexturedQuad },
	{ 66, BTL_renderWireframeGrid },
	{ 67, BTL_discardEFEOperand },
	{ 68, BTL_renderWireframeBox },
	{ 69, BTL_setTransformToBoneMatrix },
	{ 70, BTL_render3DTexturedQuad },
	{ 71, BTL_multiplyVectorByScalar },
	{ 72, BTL_divideVectorByScalar },
	{ 73, BTL_maskVectorByScalar },
	{ 74, BTL_convertToViewSpace },
	{ 75, BTL_selectRandomTargetEntity },
	{ 76, BTL_getCameraRotation },
	{ 77, BTL_drawTMDYXZ },
	{ 78, BTL_loadClutColors },
	{ 79, BTL_drawTMDScreenSpace },
	{ 80, BTL_addClutLoadPrim },
	{ 81, BTL_getViewportDistance },
	{ 82, BTL_renderRadialWaves },
	{ 83, BTL_initializeRibbonPoints },
	{ 84, BTL_tickRibbonPoints },
	{ 85, BTL_renderRibbonStrip },
	{ 86, BTL_renderRingTube },
	{ 87, BTL_renderScreenOverlay },
	{ 88, BTL_faceTargetEntity },
	{ 89, BTL_applyLineAttackHit },
	{ 90, BTL_applyRadiusAttackHit },
	{ 91, BTL_applyBoxAttackHit },
	{ 92, BTL_renderScreenFade },
	{ 93, BTL_disableMapLayer },
	{ 94, BTL_getViewportDistance2 },
	{ 95, BTL_markEFEFinished },
	{ 96, BTL_isTargetUnhit },
};

void (*BTL_jtbl_80073604[18])(void) = {
	BTL_loadEFEImmediate,
	BTL_loadEFEVariable,
	BTL_loadEFERandomValue,
	BTL_loadEFEIndexedVariable,
	BTL_applyEFEVariableOperator,
	BTL_stopEFEScript,
	BTL_stopEFEScript,
	BTL_branchEFEOnComparison,
	BTL_stopEFEScript,
	BTL_jumpEFEScript,
	BTL_pushEFEImmediate,
	BTL_pushEFEVariable,
	BTL_pushEFEVariableAddress,
	BTL_callEFESubroutine,
	BTL_dispatchEFESubOpcode,
	BTL_returnFromEFESubroutine,
	BTL_popEFEValueToVariable,
	BTL_spawnEFESubEffect,
};

int32_t (*BTL_EFE_VARIABLE_OPERATORS[5][8])() = {
	{
		BTL_setInt8Variable,
		BTL_addInt8Variable,
		BTL_subtractInt8Variable,
		BTL_multiplyInt8Variable,
		BTL_divideInt8Variable,
		BTL_moduloInt8Variable,
		BTL_shiftLeftInt8Variable,
		BTL_shiftRightInt8Variable,
	},
	{
		BTL_setInt8Variable,
		BTL_addInt8Variable,
		BTL_subtractInt8Variable,
		BTL_multiplyInt8Variable,
		BTL_divideInt8Variable,
		BTL_moduloInt8Variable,
		BTL_shiftLeftInt8Variable,
		BTL_shiftRightInt8Variable,
	},
	{
		BTL_setInt16Variable,
		BTL_addInt16Variable,
		BTL_subtractInt16Variable,
		BTL_multiplyInt16Variable,
		BTL_divideInt16Variable,
		BTL_moduloInt16Variable,
		BTL_shiftLeftInt16Variable,
		BTL_shiftRightInt16Variable,
	},
	{
		BTL_setInt8Variable,
		BTL_addInt8Variable,
		BTL_subtractInt8Variable,
		BTL_multiplyInt8Variable,
		BTL_divideInt8Variable,
		BTL_moduloInt8Variable,
		BTL_shiftLeftInt8Variable,
		BTL_shiftRightInt8Variable,
	},
	{
		BTL_setInt32Variable,
		BTL_addInt32Variable,
		BTL_subtractInt32Variable,
		BTL_multiplyInt32Variable,
		BTL_divideInt32Variable,
		BTL_moduloInt32Variable,
		BTL_shiftLeftInt32Variable,
		BTL_shiftRightInt32Variable,
	},
};

int32_t (*BTL_EFE_COMPARISONS[6])(int32_t) = {
	BTL_compareEqual,
	BTL_compareNotEqual,
	BTL_compareLess,
	BTL_compareLessOrEqual,
	BTL_compareGreater,
	BTL_compareGreaterOrEqual,
};

uint8_t BTL_WIREFRAME_BOX_LINES[16] = {
	0x00, 0x01, 0x02, 0x03, 0x05, 0x04, 0x07, 0x06,
	0x01, 0x05, 0x06, 0x02, 0x04, 0x00, 0x03, 0x07,
};

uint8_t BTL_RADIAL_WAVE_UVS[24] = {
	0x07, 0x10, 0x07, 0x1f, 0x00, 0x10, 0x00, 0x1f,
	0x08, 0x00, 0x08, 0x0f, 0x17, 0x00, 0x17, 0x0f,
	0x17, 0x00, 0x17, 0x0f, 0x08, 0x00, 0x08, 0x0f,
};

CVECTOR BTL_RIBBON_COLORS[3] = {
	{ 0x7c, 0x7c, 0x7c, 0x00 },
	{ 0x00, 0x7c, 0x7c, 0x00 },
	{ 0x3c, 0x7c, 0x3c, 0x00 },
};

GsSPRITE BTL_POISON_BUBBLE_SPRITE = {
	0x50000000,
	0x0000,
	0x0000,
	0x0010,
	0x0010,
	0x005f,
	0x20,
	0xb0,
	0x0110,
	0x01e7,
	0x80,
	0x80,
	0x80,
	0x0008,
	0x0008,
	0x0000,
	0x0000,
	0x00000000,
};

VECTOR BTL_CONFUSION_SCALE = { 0x00001000, 0x00001000, 0x00001000, 0x00000000 };

GsSPRITE BTL_D_8007376C = {
	0x50000000,
	0x0000,
	0x0000,
	0x0028,
	0x0016,
	0x003e,
	0x00,
	0x80,
	0x0100,
	0x01e0,
	0x80,
	0x80,
	0x80,
	0x0014,
	0x002c,
	0x1000,
	0x1000,
	0x00000000,
};

GsSPRITE BTL_D_80073790 = {
	0x50000000,
	0x0000,
	0x0000,
	0x000c,
	0x0016,
	0x003e,
	0x28,
	0x80,
	0x0100,
	0x01e0,
	0x80,
	0x80,
	0x80,
	0x0006,
	0x0016,
	0x1000,
	0x1000,
	0x00000000,
};

GsSPRITE BTL_STUN_DIGIT_SPRITE = {
	0x50000000,
	0x0000,
	0x0000,
	0x0008,
	0x0009,
	0x003e,
	0x00,
	0xb7,
	0x0100,
	0x01e2,
	0x80,
	0x80,
	0x80,
	0x000d,
	0x0026,
	0x1000,
	0x1000,
	0x00000000,
};

GsSPRITE BTL_STUN_SMALL_DIGIT_SPRITE = {
	0x50000000,
	0x0000,
	0x0000,
	0x0004,
	0x0007,
	0x003e,
	0x00,
	0x96,
	0x0100,
	0x01e2,
	0x80,
	0x80,
	0x80,
	0xfffe,
	0x0025,
	0x1000,
	0x1000,
	0x00000000,
};

VECTOR BTL_STUN_SUB_EFFECT_SCALE = { 0x00003000, 0x00003000, 0x00003000, 0x00000000 };

VECTOR BTL_D_8007380C = { 0x0000003c, 0x0000003c, 0x0000003c, 0x00000000 };

VECTOR BTL_D_8007381C = { 0x0000005a, 0x0000005a, 0x0000005a, 0x00000000 };

VECTOR BTL_FINISHER_AURA_SCALE = { 0x00001000, 0x00001000, 0x00001000, 0x00000000 };

EfeAuraType BTL_AURA_PROJECTILE_TYPES[129] = {
	{ 0x0000, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x0001, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0070 },
	{ 0x0002, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0070 },
	{ 0x0003, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0010 },
	{ 0x0004, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0018 },
	{ 0x0005, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0008 },
	{ 0x0006, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0007, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x0008, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0008 },
	{ 0x0009, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x000a, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x000b, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0040 },
	{ 0x000c, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0000 },
	{ 0x000d, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0030 },
	{ 0x000e, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0038 },
	{ 0x000f, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0070 },
	{ 0x0010, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0070 },
	{ 0x0011, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0010 },
	{ 0x0012, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0018 },
	{ 0x0013, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0008 },
	{ 0x0014, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0015, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x0016, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0008 },
	{ 0x0017, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0018, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x0019, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0040 },
	{ 0x001a, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0000 },
	{ 0x001b, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0030 },
	{ 0x001c, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0048 },
	{ 0x001d, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0070 },
	{ 0x001e, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0070 },
	{ 0x001f, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0010 },
	{ 0x0020, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0018 },
	{ 0x0021, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0008 },
	{ 0x0022, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0023, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x0024, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0008 },
	{ 0x0025, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0026, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x0027, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0040 },
	{ 0x0028, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0000 },
	{ 0x0029, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0030 },
	{ 0x002a, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0048 },
	{ 0x002b, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0070 },
	{ 0x002c, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0070 },
	{ 0x002d, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0010 },
	{ 0x002e, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0018 },
	{ 0x002f, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0008 },
	{ 0x0030, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0031, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x0032, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0008 },
	{ 0x0033, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0034, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0060 },
	{ 0x0035, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0040 },
	{ 0x0036, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0000 },
	{ 0x0037, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0030 },
	{ 0x0038, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0050 },
	{ 0x0039, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x003a, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0068 },
	{ 0x003b, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0058 },
	{ 0x003c, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0058 },
	{ 0x003d, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x003e, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x003f, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0040, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0000 },
	{ 0x0041, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0038 },
	{ 0x0042, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0050 },
	{ 0x0043, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0058 },
	{ 0x0044, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0045, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0010 },
	{ 0x0046, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0038 },
	{ 0x0047, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0010 },
	{ 0x0048, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0049, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0040 },
	{ 0x004a, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0018 },
	{ 0x004b, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0008 },
	{ 0x004c, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0000 },
	{ 0x004d, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0040 },
	{ 0x004e, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0060 },
	{ 0x004f, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x0050, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0060 },
	{ 0x0051, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0052, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0010 },
	{ 0x0053, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0018 },
	{ 0x0054, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0010 },
	{ 0x0055, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0056, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0018 },
	{ 0x0057, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0040 },
	{ 0x0058, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0008 },
	{ 0x0059, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x005a, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0058 },
	{ 0x005b, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x005c, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x005d, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x005e, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x005f, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0008 },
	{ 0x0060, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0010 },
	{ 0x0061, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0060 },
	{ 0x0062, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0010 },
	{ 0x0063, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0040 },
	{ 0x0064, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0065, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0010 },
	{ 0x0066, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0058 },
	{ 0x0067, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0060 },
	{ 0x0068, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0069, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0008 },
	{ 0x006a, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x006b, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x006c, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x006d, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x006e, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0020 },
	{ 0x006f, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0058 },
	{ 0x0070, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0058 },
	{ 0x0071, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0028 },
	{ 0x0072, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x0073, 0x0000, 0x0000, 0xff6a, 0xff38, 0x0058 },
	{ 0x0074, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x0075, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x0076, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x0077, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x0078, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x0079, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x007a, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x007b, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x007c, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x007d, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x007e, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0x007f, 0x0000, 0x0000, 0xff6a, 0xff38, 0xffff },
	{ 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000 },
};

#if defined(VERSION_JP)
VECTOR BTL_AURA_PROJECTILE_SCALE = { 0x00001000, 0x00001000, 0x00001000, 0x00000000 };
#endif

int16_t BTL_BUFF_RING_OBJECTS[5] = {
	0x0003, 0x0000, 0x0001, 0x0002, 0x0004,
};
// clang-format on

char *BTL_initializeParticleEmitters(char *base)
{
	int32_t i;

	EFE_PARTICLE_EMITTERS = (EfeParticleEffect *)base;
	for (i = 0; i < 4; i++) {
		EFE_PARTICLE_EMITTERS[i].transform = NULL;
	}

	return base + 0x23c4;
}

void BTL_tickEFEEngine(void)
{
	int32_t n;
	int32_t i;
	int32_t j;
	EfeSound *p;

	setMapLayerEnabled(1);

	COMBAT_EFFECT_ITR = 0;
	while (COMBAT_EFFECT_ITR < 0x10) {
		if (UNUSED_EFE_ARRAY[COMBAT_EFFECT_ITR] == 0) {
			UNUSED_EFE_ARRAY[COMBAT_EFFECT_ITR] = 1;
		}
		COMBAT_EFFECT_ITR++;
	}

	EFE_DATA_ITERATOR = EFE_DATA_PTR;
	COMBAT_EFFECT_ITR = 0;
	while (COMBAT_EFFECT_ITR < 0x10) {
		if (EFE_DATA_ITERATOR->data != 0L) {
			EFE_SCRIPT_HEAD = EFE_DATA_ITERATOR->data;
			n = EFE_DATA_ITERATOR->numSubEffects;
			EFE_SUB_EFFECT_INDEX = 0;
			while (EFE_SUB_EFFECT_INDEX < n) {
				EFE_SCRIPT_CONTEXT = &EFE_DATA_ITERATOR->subEffects[EFE_SUB_EFFECT_INDEX];
				if ((int32_t)EFE_SCRIPT_CONTEXT->inst != 0) {
					EFE_CURRENT_DATA_SEGMENT = EFE_SCRIPT_CONTEXT->instance;
					EFE_PREVIOUS_DATA_SEGMENT = (int32_t)EFE_SCRIPT_CONTEXT->parentInstance;
					((EfeInstance *)(int32_t)EFE_CURRENT_DATA_SEGMENT)->frame++;
					BTL_runEFEScript((int32_t)EFE_SCRIPT_CONTEXT->inst);
				}
				EFE_SUB_EFFECT_INDEX++;
			}
			n = EFE_DATA_ITERATOR->numObjects;
			for (j = 0; j < n; j++) {
				BTL_tickEFEUVAnimation(j);
			}
		}
		EFE_DATA_ITERATOR++;
		COMBAT_EFFECT_ITR++;
	}

	BTL_tickParticleEmitters();

	p = EFE_SOUND_DATA;
	for (i = 0; i < 0xa; i++) {
		if ((p->mask >= 0L) && (p->context->instance->frame < 0)) {
			thunkStopSoundMask(p->mask);
			p->mask = -1;
		}
		p++;
	}
}

void BTL_renderEFEEngine(void)
{
	int32_t i;
	int32_t j;
	int32_t n;

	EFE_DATA_ITERATOR = EFE_DATA_PTR;
	for (i = 0; i < 0x10; i++) {
		n = EFE_DATA_ITERATOR->numObjects;
		for (j = 0; j < n; j++) {
			EFE_DATA_ITERATOR->uvAnims[j].unk2 = 0;
		}
		if (EFE_DATA_ITERATOR->data != 0L) {
			EFE_SCRIPT_HEAD = EFE_DATA_ITERATOR->data;
			n = EFE_DATA_ITERATOR->numSubEffects;
			for (j = 0; j < n; j++) {
				EFE_SCRIPT_CONTEXT = &EFE_DATA_ITERATOR->subEffects[j];
				if ((int32_t)EFE_SCRIPT_CONTEXT->inst != 0) {
					EFE_CURRENT_DATA_SEGMENT = EFE_SCRIPT_CONTEXT->instance;
					EFE_PREVIOUS_DATA_SEGMENT = (int32_t)EFE_SCRIPT_CONTEXT->parentInstance;
					BTL_runEFEScript((int32_t)EFE_SCRIPT_CONTEXT->someInst);
				}
			}
		}
		EFE_DATA_ITERATOR++;
	}

	BTL_renderParticleEmitters();
}

void BTL_clearEFESoundChannels(void)
{
	int32_t i;

	for (i = 0; i < 0xa; i++) {
		EFE_SOUND_DATA[i].mask = -1;
	}
}

void BTL_stopEFESounds(void)
{
	int32_t i;
	int32_t *p;

	p = &EFE_SOUND_DATA[0].mask;
	for (i = 0; i < 10; i++) {
		if (*p >= 0L) {
			thunkStopSoundMask(*p);
			*p = -1;
		}
	}
}

int32_t BTL_getEFEHeapPointer(void)
{
	return EFE_HEAP_POINTER;
}

void BTL_loadNextEFEFile(EfeLoad *arg)
{
	EfeLoad *p;
	ModelComponent *m;
	int32_t id;
	int16_t *moves;
	int32_t i;
	char path[32];
	CdlLOC loc;

	p = (EfeLoad *)(int32_t)arg;
	moves = p->moves;
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
	m->mmdPtr = (void *)BTL_getEFEHeapPointer();
	p->model = m;
	loc = *getEFEDATEntry(id + 0x100);
	addFileReadRequest(path, (uint8_t *)m->mmdPtr, NULL, (void *)BTL_handleEFEFileLoaded, arg, &loc, 0x5000);
}

void BTL_unloadEFESlot(int32_t idx)
{
	int32_t i;
	int32_t n;
	EfeUvAnim *anim;

	EFE_DATA_ITERATOR = &EFE_DATA_PTR[idx];
	n = EFE_DATA_ITERATOR->numObjects;
	for (i = 0; i < n; i++) {
		anim = &EFE_DATA_ITERATOR->uvAnims[i];
		if (anim->unk0 != 0) {
			anim->unk0 = 0;
			anim->numKeyframes = 0;
		}
	}

	unloadModel(EFE_DATA_ITERATOR->effectId, 1);
	EFE_DATA_ITERATOR->data = 0;
}

void BTL_runEFESlotScript(int32_t idx)
{
	EFE_CURRENT_DATA_SEGMENT = NULL;

	EFE_DATA_ITERATOR = &EFE_DATA_PTR[idx];
	EFE_SCRIPT_HEAD = EFE_DATA_ITERATOR->data;
	EFE_SCRIPT_CONTEXT = EFE_DATA_ITERATOR->subEffects;
	EFE_ACTIVE_SECTION = -1;
	BTL_runEFEScript((int32_t)EFE_DATA_PTR[idx].startScript);
}

char *BTL_getEFETextureSection(char *p)
{
	EfeFileHeader *hdr;
	char *section;

	hdr = (EfeFileHeader *)p;
	p = (char *)((uint32_t)p + 0x34);
	if ((hdr->dataEnd - hdr->timStart) == 0) {
		section = NULL;
	} else {
		section = p + hdr->timStart;
	}

	return section;
}

char *BTL_getEFEModelSection(char *p)
{
	EfeFileHeader *hdr;
	char *section;

	hdr = (EfeFileHeader *)p;
	p = (char *)((uint32_t)p + 0x34);
	if ((hdr->tmdEnd - hdr->tmdStart) == 0) {
		section = NULL;
	} else {
		section = p + hdr->tmdStart;
	}

	return section;
}

int32_t BTL_getEFEFileId(char *data)
{
	EfeFileHeader *header;

	header = (EfeFileHeader *)data;
	return header->effectId;
}

int32_t BTL_setupLoadedEFEFile(EfeLoad *load)
{
	ModelComponent *m;
	EfeLoad *ld;
	int32_t k;
	int32_t fileMove;
	int16_t *moves;
	int32_t lastMove;
	int32_t idx;
	char *data;
	int32_t fileId;
	char *tim;
	char *tmd;
	int32_t i;
	GsIMAGE im;
	RECT rect;
	GsIMAGE im2;
	RECT rect2;
	int32_t j;
	int32_t n;
	EfeFileHeader hdr;
	char *base;
	int32_t heap;
	int32_t *tmdp;
	int32_t ce;
	int32_t s;

	ld = ld = load;
	moves = ld->moves;
	m = ld->model;
	data = (char *)m->mmdPtr;
	tim = BTL_getEFETextureSection(data);
	m->modelPtr = (TMDModel *)(tmd = BTL_getEFEModelSection(data));
	fileId = BTL_getEFEFileId(data);

	switch (BTL_EFE_LOAD_STATE) {
	case 0:
		if (tim != NULL) {
			for (i = 0; UNKNOWN_MODEL_TAKEN[i] != 0 && i < 0x10; i++) {
			}
			if (i == 0x10) {
				*ld->isLoaded = -2;
				goto end;
			}
			UNKNOWN_MODEL_TAKEN[i] = 1;
			m->pixelPage = i / 8 + 0x19;
			m->clutPage = ((i % 4 * 6 + 0x1e8) << 6) | ((i / 4 * 16 + 0x80) >> 4 & 0x3f);
			m->pixelOffsetX = 0;
			m->pixelOffsetY = i % 8 << 5;
			m->modelId = i;
			GsGetTimInfo((unsigned long *)(tim + 4), &im);
			im.px = (m->pixelPage % 16 << 6) + m->pixelOffsetX;
			im.py = (m->pixelPage / 16 << 8) + m->pixelOffsetY;
			setRECT(&rect, im.px, im.py, im.pw, im.ph);
			LoadImage(&rect, im.pixel);
		}
		GsGetTimInfo((unsigned long *)(tim + 4), &im2);
		im2.cx = (m->clutPage & 0x3f) << 4;
		im2.cy = m->clutPage >> 6;
		if ((im2.pmode >> 3 & 1) != 0) {
			setRECT(&rect2, im2.cx, im2.cy, im2.cw, im2.ch);
			LoadImage(&rect2, im2.clut);
		}
		BTL_EFE_LOAD_STATE = 1;
	case 1:
		if (tmd != NULL) {
			GsMapModelingData((unsigned long *)&m->modelPtr->flags);
			updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
			                     m->clutPage - 0x7a08);
		}
		BTL_EFE_LOAD_STATE = 2;
	case 2:
		fileMove = ((EfeFileHeader *)m->mmdPtr)->effectId;
		lastMove = ld->moves[-1];
		if (fileMove != lastMove) {
			*ld->isLoaded = -3;
			goto end;
		}
		EFE_LOADED_MOVE_DATA[16] = -1;
		k = 0;
		while (1) {
			if (EFE_LOADED_MOVE_DATA[k] == -1) {
				break;
			}
			k++;
		}
		if (k == 0x10) {
			*ld->isLoaded = -1;
			goto end;
		}
		BTL_EFE_LOAD_SLOT = k;
		if (EFE_LOADED_MOVE_DATA[k] == -1) {
			BTL_EFE_LOAD_STATE = 0x15;
		} else {
			BTL_EFE_LOAD_STATE = 3;
		}
		return 1;
	case 0x15:
		idx = BTL_EFE_LOAD_SLOT;
		EFE_DATA_ITERATOR = &EFE_DATA_PTR[idx];
		EFE_DATA_ITERATOR->model = m;
		hdr = *(EfeFileHeader *)m->mmdPtr;
		base = (char *)m->mmdPtr + 0x34;
		EFE_HEAP_POINTER += (uint32_t)hdr.tmdEnd + 0x34;
		EFE_DATA_ITERATOR->data = (int32_t)base;
		heap = EFE_HEAP_POINTER;
		tmdp = (int32_t *)m->modelPtr;
		if (hdr.tmdEnd - hdr.tmdStart == 0) {
			EFE_DATA_ITERATOR->numObjects = 0;
		} else {
			EFE_DATA_ITERATOR->numObjects = tmdp[2];
		}
		EFE_DATA_ITERATOR->numSubEffects = hdr.numSubEffects;
		EFE_DATA_ITERATOR->subEffects = (EfeSubEffect *)((uint32_t)base + hdr.subEffects);
		for (j = 0; j < hdr.numSubEffects; j++) {
			EFE_DATA_ITERATOR->subEffects[j].inst = NULL;
		}
		n = EFE_DATA_ITERATOR->numObjects;
		if (n != 0) {
			EFE_DATA_ITERATOR->uvAnims = (EfeUvAnim *)((uint32_t)base + hdr.uvAnims);
			for (j = 0; j < n; j++) {
				EFE_DATA_ITERATOR->uvAnims[j].unk0 = 0;
				EFE_DATA_ITERATOR->uvAnims[j].numKeyframes = 0;
			}
		}
		EFE_DATA_ITERATOR->effectId = hdr.effectId;
		EFE_DATA_ITERATOR->startScript = (int16_t *)((int32_t)base + hdr.startScript);
		EFE_DATA_ITERATOR->initScript = (int16_t *)((int32_t)base + hdr.initScript);
		EFE_DATA_ITERATOR->uvAnimsEnd = (EfeUvAnim *)((int32_t)base + hdr.uvAnimsEnd);
		EFE_CURRENT_DATA_SEGMENT = NULL;
		EFE_SCRIPT_HEAD = EFE_DATA_ITERATOR->data;
		EFE_SCRIPT_CONTEXT = EFE_DATA_ITERATOR->subEffects;
		EFE_ACTIVE_SECTION = -1;
		EFE_SCRIPT_PTR = EFE_DATA_ITERATOR->initScript;
		EFE_DATA_STACK = EFE_SCRIPT_MEM1_DATA;
		EFE_CALL_STACK = EFE_CALL_STACK_BUFFER;
		*EFE_CALL_STACK++ = 0;
		BTL_EFE_LOAD_STATE = 0x16;
		return 1;
	case 0x16:
		idx = BTL_EFE_LOAD_SLOT;
		EFE_DATA_ITERATOR = &EFE_DATA_PTR[idx];
		for (s = 0; s < 4; s++) {
			EFE_SCRIPT_CURRENT_VALUE = *EFE_SCRIPT_PTR;
			BTL_dispatchEFEOpcode(EFE_SCRIPT_CURRENT_VALUE & 0xff);
			if (EFE_SCRIPT_PTR == NULL) {
				break;
			}
		}
		if (s == 4) {
			return 1;
		}
		if (EFE_SCRIPT_CONTEXT->instance->frame == -1) {
			EFE_SCRIPT_CONTEXT->inst = NULL;
		}
		ce = EFE_ACTIVE_SECTION;
		k = BTL_EFE_LOAD_SLOT;
		if (ce >= -1L) {
			EFE_LOADED_MOVE_DATA[k] = EFE_DATA_ITERATOR->effectId;
		} else {
			EFE_LOADED_MOVE_DATA[k] = ce;
		}
		k = BTL_EFE_LOAD_SLOT;
		*ld->effectIds++ = k;
		BTL_loadNextEFEFile(load);
		BTL_EFE_LOAD_STATE = 3;
		return 0;
	}
end:;
}

void BTL_handleEFEFileLoaded(EfeLoad *load)
{
	EfeLoad *ld;
	int16_t *moves;
	ModelComponent *model;

	ld = load;
	moves = ld->moves;
	model = ld->model;
	BTL_EFE_LOAD_STATE = 0;
	setFileReadCallback2(BTL_setupLoadedEFEFile, load);
}

void BTL_tickEFEUVAnimation(int32_t idx)
{
	EfeUvAnim *anim;

	anim = &EFE_DATA_ITERATOR->uvAnims[idx];
	if (anim->numKeyframes != 0) {
		++anim->uvFrame;
		if (anim->uvFrame >= anim->uv->frames) {
			anim->uvFrame = 0;
			anim->keyframe++;
			if (anim->keyframe >= anim->numKeyframes) {
				anim->uv = anim->uvData;
				anim->keyframe = 0;
			} else {
				anim->uv = anim->uv + 1;
			}

			BTL_offsetEFEPrimitiveUVs((char *)EFE_DATA_ITERATOR->model->modelPtr, idx,
			                          *(int8_t *)((uint32_t)anim->uv + 2),
			                          *(int8_t *)((uint32_t)anim->uv + 3));
		}
		anim->frame++;
		anim->frame %= anim->numFrames;
	}
}

void BTL_tickParticleEmitters(void)
{
	int32_t i;
	EfeInstance *owner;
	SVECTOR rot;
	MATRIX m;
	EfeParticleEffect *e;
	EfeParticle *pt;
	int32_t k;

	e = EFE_PARTICLE_EMITTERS;
	for (i = 0; i < 4; e++, i++) {
		if (e->transform == NULL) {
			continue;
		}
		owner = e->transform;
		if (owner->frame == -1) {
			e->transform = NULL;
			continue;
		}

		k = 0;
		while (1) {
			if (e->particles[k].distance <= 0) {
				break;
			}
			k++;
		}

		if (k != 0x14) {
			pt = &e->particles[k];
			pt->distance = e->startOffset;
			pt->velocity = e->startVelocity;
			rot.vx = (((rand() & 0x7f) - 0x40) << 12) / 64;
			rot.vy = (((rand() & 0x7f) - 0x40) << 12) / 64;
			rot.vz = (((rand() & 0x7f) - 0x40) << 12) / 64;
			pt->direction.vx = pt->direction.vy = 0;
			pt->direction.vz = e->startOffset;
			RotMatrixZYX(&rot, &m);
			ApplyMatrixSV(&m, &pt->direction, &pt->direction);

			pt->position.vx = *(int32_t *)((uint32_t)&owner->transform.position.vx);
			pt->position.vy = *(int32_t *)((uint32_t)&owner->transform.position.vy);
			pt->position.vz = *(int32_t *)((uint32_t)&owner->transform.position.vz);
		}

		for (k = 0; k < 0x14; k++) {
			if (e->particles[k].distance > 0) {
				pt = &e->particles[k];
				pt->distance -= pt->velocity;
				pt->velocity -= e->acceleration;
			}
		}

		if (--e->frames <= 0) {
			e->transform = NULL;
		}
	}
}

void BTL_renderParticleEmitters(void)
{
	int32_t i;
	int32_t k;
	int32_t f0;
	int32_t f1;
	EfeInstance *owner;
	SVECTOR a;
	SVECTOR b;
	int32_t depth;
	int32_t z;
	DVECTOR s0v;
	DVECTOR s1v;
	int8_t kind;
	EfeParticleEffect *e;
	EfeParticle *pt;

	e = EFE_PARTICLE_EMITTERS;
	for (i = 0; i < 4; e++, i++) {
		if (e->transform == NULL) {
			continue;
		}

		owner = e->transform;
		kind = e->type;
		for (k = 0; k < 0x14; k++) {
			if (e->particles[k].distance <= 0) {
				continue;
			}

			pt = &e->particles[k];
			switch (kind) {
			case 0:
				f0 = pt->distance << 4;
				f1 = (pt->distance + 7) << 4;
				break;
			case 1:
				f0 = pt->distance << 4;
				f1 = pt->distance << 2;
				break;
			case 3:
				f0 = (e->startOffset - pt->distance) << 4;
				f1 = (e->startOffset - pt->distance) << 2;
				break;
			case 2:
				f0 = (e->startOffset - pt->distance) << 4;
				f1 = ((e->startOffset - pt->distance) - 5) << 4;
				break;
			}

			f0 = f0 / e->startOffset;
			f1 = f1 / e->startOffset;
			a.vx = pt->position.vx + ((pt->direction.vx * f0) >> 8);
			a.vy = pt->position.vy + ((pt->direction.vy * f0) >> 8);
			a.vz = pt->position.vz + ((pt->direction.vz * f0) >> 8);
			b.vx = pt->position.vx + ((pt->direction.vx * f1) >> 8);
			b.vy = pt->position.vy + ((pt->direction.vy * f1) >> 8);
			b.vz = pt->position.vz + ((pt->direction.vz * f1) >> 8);
			depth = worldPosToScreenPos(&a, &s0v);
			depth = depth >> 2;
			if (depth < 0x3e8) {
				goto modeTest;
			}
drawLine:
			gte_ldv0(&b);
			gte_rtps();
			gte_stsxy(&s1v);
			gte_stszotz(&z);
			drawLine2P((uint8_t)e->color.r | ((uint8_t)e->color.g << 8) | ((uint8_t)e->color.b << 16),
			           s0v.vx, s0v.vy, s1v.vx, s1v.vy,
			           (depth + z) >> 3, 0);
			goto nextParticle;
modeTest:
			if ((kind & 1) == 1) {
				goto drawLine;
			}
			renderFXParticle(&a, 0x19, (RGB8 *)&e->color);
nextParticle:;
		}
	}
}

int16_t BTL_offsetEFEPrimitiveUVs(char *base, int32_t idx, int8_t du, int8_t dv)
{
	uint8_t code;
	char *p;
	struct TMD_STRUCT *obj;
	int32_t i;
	int32_t n;

	base = (char *)((int32_t)base + 0xc);
	obj = (struct TMD_STRUCT *)base;
	obj += idx;
	n = obj->primn;
	p = (char *)obj->primtop;
	for (i = 0; i < n; i++) {
		code = (*(int32_t *)p >> 24) & 0xff;
		if (code & 1) {
			goto setpath;
		}
		if (code & 4) {
			p[4] += du;
			p[5] += dv;
			p[8] += du;
			p[9] += dv;
			p[0xc] += du;
			p[0xd] += dv;
			if (code & 8) {
				p[0x10] += du;
				p[0x11] += dv;
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
			p[4] += du;
			p[5] += dv;
			p[8] += du;
			p[9] += dv;
			p[0xc] += du;
			p[0xd] += dv;
			if (code & 8) {
				p[0x10] += du;
				p[0x11] += dv;
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

char *BTL_initializeEFEEngine(char *base)
{
	int32_t i;

	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	EFE_DATA_PTR = (EfeSlot *)base;
	for (i = 0; i < 0x10; i++) {
		EFE_DATA_PTR[i].data = 0;
		EFE_DATA_PTR[i].numObjects = 0;
	}

	base = (char *)((int32_t)base + 0x280);
	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	base = BTL_initializeParticleEmitters(base);
	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	base = initializeFlashData(base);
	base = BTL_initializeAuraProjectiles(base);
	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	EFE_HEAP_BASE = (int32_t)base;
#if defined(VERSION_JP)
	EFE_HEAP_POINTER = EFE_HEAP_BASE;
#else
	EFE_HEAP_POINTER = (int32_t)base;
#endif
	addObject(0x500, 0, (TickFunction)BTL_tickEFEEngine, (RenderFunction)BTL_renderEFEEngine);
	BTL_initializeEFESubOpcodeTable();
	base = (char *)((int32_t)base + 0x41000);
	BTL_clearEFESoundChannels();

	return base;
}

void BTL_removeEFEEngine(void)
{
	BTL_removeAllAuraProjectiles();
	BTL_stopEFESounds();
	removeObject(0x500, 0);
}

void BTL_loadMoveEFE(int16_t *moves, int16_t *effectIds, int8_t *isLoaded)
{
	EFE_LOAD_REQUEST.state = -1;
	EFE_LOAD_REQUEST.isLoaded = isLoaded;
	*EFE_LOAD_REQUEST.isLoaded = 1;
	EFE_LOAD_REQUEST.moves = moves;
	EFE_LOAD_REQUEST.effectIds = effectIds;
	BTL_loadNextEFEFile(&EFE_LOAD_REQUEST);
}

void BTL_unloadAllEFESlots(void)
{
	int32_t i;

	for (i = 0; i < 0x10; i++) {
		if (EFE_LOADED_MOVE_DATA[i] != -1) {
			BTL_unloadEFESlot(i);
			EFE_LOADED_MOVE_DATA[i] = -1;
		}
	}

	EFE_HEAP_POINTER = EFE_HEAP_BASE;
}

int32_t BTL_startEFE(int32_t i)
{
	if ((i < 0) || (i >= 0x10)) {
		return -1;
	}

	if (EFE_LOADED_MOVE_DATA[i] < 0) {
		return -1;
	}

	UNUSED_EFE_ARRAY[i] = -1;
	BTL_runEFESlotScript(i);
}

void BTL_stopEFESubEffect(int32_t a, int32_t b)
{
	if ((b < 0) || (a < 0) || (a >= 0x10)) {
		return;
	}

	if (EFE_DATA_PTR != NULL) {
	}

	EFE_SCRIPT_CONTEXT = &EFE_DATA_PTR[a].subEffects[b];
	EFE_SCRIPT_CONTEXT->inst = NULL;
	EFE_SCRIPT_CONTEXT->instance->frame = -1;
}

void BTL_isTargetUnhit(void)
{
	int32_t *out;

	out = EFE_POP1(int32_t *);
	if (((DigimonEntity *)EFE_SCRIPT_CONTEXT->targetEntity)->stats.current.isHit == 0) {
		*out = 1;
	} else {
		*out = 0;
	}
}

void BTL_markEFEFinished(void)
{
	UNUSED_EFE_ARRAY[COMBAT_EFFECT_ITR] = 0;
}

void BTL_getViewportDistance2(void)
{
	int32_t *out;

	out = EFE_POP1(int32_t *);
	*out = VIEWPORT_DISTANCE;
}

void BTL_disableMapLayer(void)
{
	setMapLayerEnabled(0);
}

void BTL_renderScreenFade(void)
{
	int32_t ofx;
	int32_t ofy;
	POLY_FT4 *p;
	int32_t c;
	int32_t z;

	c = EFE_POP1(int32_t);
	p = (POLY_FT4 *)GsGetWorkBase();
	z = 0xffe;
	getDrawingOffsetCopy(&ofx, &ofy);
	SetPolyFT4(p);
	SetSemiTrans(p, 1);
	p->tpage = getTPage(1, 2, 832, 256);
	p->clut = getClut(0, 487);
	setXY4(p, -ofx, -ofy, 0x140 - ofx, -ofy, -ofx, 0xf0 - ofy, 0x140 - ofx, 0xf0 - ofy);
	setUVWH(p, 0, 0x80, 3, 3);
	setRGB0(p, c, c, c);
	AddPrim(ACTIVE_ORDERING_TABLE->org + z, p);
	p++;
	GsSetWorkBase((PACKET *)p);
}

void BTL_applyBoxAttackHit(void)
{
	int32_t r;
	SVECTOR center;
	AABB box;
	int32_t *out;
	EfeVector *ext;
	int32_t j;

	r = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	ext = EFE_POP1(EfeVector *);
	*out = 0;
	if (r < 0) {
		r = EFE_SUB_EFFECT_INDEX;
	}

	center.vx = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vx;
	center.vy = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 8);
	center.vz = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0xc);
	box.center = &center;
	copyVector(&box.extent, ext);
	EFE_HIT_ENTITY_INDEX = 1;
	while (1) {
		if ((EFE_HIT_ENTITY_INDEX = findAABBHitEntity(&box, EFE_SCRIPT_CONTEXT->sourceEntity, EFE_HIT_ENTITY_INDEX)) == -1) {
			return;
		}
		if (((DigimonEntity *)ENTITY_TABLE[EFE_HIT_ENTITY_INDEX])->stats.current.isHit == 0) {
			for (j = 1; j < 10; j++) {
				if (ENTITY_TABLE[j] == EFE_SCRIPT_CONTEXT->sourceEntity) {
					break;
				}
			}
			((DigimonEntity *)ENTITY_TABLE[EFE_HIT_ENTITY_INDEX])->stats.current.isHit = 1;
			addAttackObject(EFE_HIT_ENTITY_INDEX, 1, &center, COMBAT_EFFECT_ITR, r, j);
			*out = 1;
			return;
		}
		EFE_HIT_ENTITY_INDEX++;
	}
}

void BTL_applyRadiusAttackHit(void)
{
	int32_t i;
	int32_t r;
	int32_t *hitFlag;
	Entity *e;
	int32_t dz;
	int32_t radius;
	int32_t dx;
	SVECTOR pos;

	hitFlag = EFE_POP1(int32_t *);
	r = EFE_POP1(int32_t);
	*hitFlag = 0;
	r = r * r;
	EFE_HIT_ENTITY_INDEX = 1;
	while (EFE_HIT_ENTITY_INDEX < 10) {
		e = ENTITY_TABLE[*(int32_t *)&EFE_HIT_ENTITY_INDEX];
		if (e == EFE_SCRIPT_CONTEXT->sourceEntity) {
			goto next;
		}
		if (e == NULL) {
			goto next;
		}
		if (((DigimonEntity *)e)->stats.current.isHit != 0) {
			goto next;
		}
		if (e->isOnMap == 0) {
			goto next;
		}
		radius = DIGIMON_DATA[e->type].radius;
		radius *= radius;
		radius += r;
		dx = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4) - (int32_t)e->posData->location.vx;
		dz = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0xc) - (int32_t)e->posData->location.vz;
		dz = dz * dz + dx * dx;
		if (radius < dz) {
			goto next;
		}
		for (i = 1; i < 10; i++) {
			if (ENTITY_TABLE[i] == EFE_SCRIPT_CONTEXT->sourceEntity) {
				break;
			}
		}
		((DigimonEntity *)ENTITY_TABLE[*(int32_t *)&EFE_HIT_ENTITY_INDEX])->stats.current.isHit = 1;
		BTL_calculateAttackHitPosition(&pos, e, EFE_SCRIPT_CONTEXT->sourceEntity, DIGIMON_DATA[e->type].radius);
		pos.vy = -DIGIMON_DATA[e->type].height / 2;
		addAttackObject(EFE_HIT_ENTITY_INDEX, 1, &pos, COMBAT_EFFECT_ITR, EFE_SUB_EFFECT_INDEX, i);
		*hitFlag = 1;
next:
		EFE_HIT_ENTITY_INDEX++;
	}
}

void BTL_applyLineAttackHit(void)
{
	int32_t *out;
	int32_t *arg;
	int32_t width;
	DVECTOR line[2];
	int16_t rect[4];
	long r;
	SVECTOR pos;
	int32_t j;
	Entity *e;

	width = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	arg = EFE_POP1(int32_t *);
	*out = 0;
	line[0].vx = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vx;
	line[0].vy = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0xc);
	line[1].vx = EFE_SCRIPT_CONTEXT->sourceEntity->posData->location.vx;
	line[1].vy = EFE_SCRIPT_CONTEXT->sourceEntity->posData->location.vz;
	for (EFE_HIT_ENTITY_INDEX = 1; EFE_HIT_ENTITY_INDEX < 10; EFE_HIT_ENTITY_INDEX++) {
		e = ENTITY_TABLE[(*(int32_t *)&EFE_HIT_ENTITY_INDEX)];
		if (e == EFE_SCRIPT_CONTEXT->sourceEntity) {
			continue;
		}
		if (e == NULL) {
			continue;
		}
		if (((DigimonEntity *)e)->stats.current.isHit != 0) {
			continue;
		}
		if (e->isOnMap == 0) {
			continue;
		}
		r = DIGIMON_DATA[e->type].radius;
		rect[0] = e->posData->location.vx - r;
		rect[2] = e->posData->location.vx + r;
		rect[1] = e->posData->location.vz - r;
		rect[3] = e->posData->location.vz + r;
		if (doSomethingWithSomePoints(rect, line) != -1) {
			continue;
		}
		if (*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 8) < (-DIGIMON_DATA[e->type].height - arg[1])) {
			continue;
		}
		for (j = 1; j < 10; j++) {
			if (ENTITY_TABLE[j] == EFE_SCRIPT_CONTEXT->sourceEntity) {
				break;
			}
		}
		((DigimonEntity *)ENTITY_TABLE[EFE_HIT_ENTITY_INDEX])->stats.current.isHit = 1;
		BTL_calculateAttackHitPosition(&pos, e, EFE_SCRIPT_CONTEXT->sourceEntity, DIGIMON_DATA[e->type].radius);
		pos.vy = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 8);
		addAttackObject(EFE_HIT_ENTITY_INDEX, 1, &pos, COMBAT_EFFECT_ITR, EFE_SUB_EFFECT_INDEX, j);
		*out = 1;
		return;
	}
}

void BTL_faceTargetEntity(void)
{
	PositionData *pd;

	if (EFE_SCRIPT_CONTEXT->targetEntity == NULL) {
		return;
	}

	entityLookAtLocation(EFE_SCRIPT_CONTEXT->sourceEntity, &EFE_SCRIPT_CONTEXT->targetEntity->posData->location);
	pd = EFE_SCRIPT_CONTEXT->sourceEntity->posData;
	RotMatrix(&pd->rotation, &pd->posMatrix.coord);
	ScaleMatrix(&pd->posMatrix.coord, &pd->scale);
	TransMatrix(&pd->posMatrix.coord, &pd->location);
	pd->posMatrix.flg = 0;
}

void BTL_renderScreenOverlay(void)
{
	POLY_F4 *p;
	DR_TPAGE *dr;
	EfeColor *col;
	int32_t mode;
	int32_t layer;

	mode = EFE_POP1(int32_t);
	col = EFE_POP1(EfeColor *);
	layer = EFE_POP1(int32_t);

	p = (POLY_F4 *)GsGetWorkBase();
	setXYWH(p, -0xa0, -0x78, 320, 240);
	setRGB0(p, col->red, col->green, col->blue);
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

void BTL_renderRingTube(void)
{
	VECTOR *center;
	int32_t n;
	int32_t *radius;
	int32_t *heights;
	int32_t *colors;
	int32_t start;
	SVECTOR pos;
	int16_t *pts;
	int32_t r;
	int32_t ang;
	int32_t z;
	int32_t j;
	int32_t i;
	DVECTOR screen;
	int16_t count;
	POLY_FT4 *p;
	int16_t *q;
	ModelComponent *m;

	start = EFE_POP1(int32_t);
	colors = EFE_POP1(int32_t *);
	heights = EFE_POP1(int32_t *);
	radius = EFE_POP1(int32_t *);
	n = EFE_POP1(int32_t);
	center = EFE_POP1(VECTOR *);

	if (n < 2) {
		return;
	}

	p = (POLY_FT4 *)GsGetWorkBase();
	count = (n - 1) * 10;
	q = (int16_t *)&p[count];
	pts = q;
	m = EFE_DATA_ITERATOR->model;
	for (i = 0; i < n; i++) {
		r = radius[i];
		for (j = 0; j < 10; j++) {
			ang = j * 0x200 / 10;
			pos.vx = center->vx + ((r * _cos(0x80 - ang)) >> 12);
			pos.vy = (int32_t)center->vy + heights[i];
			pos.vz = center->vz + ((r * _sin(0x80 - ang)) >> 12);
			z = worldPosToScreenPos(&pos, &screen);
			z >>= 4;
			*q++ = screen.vx;
			*q++ = screen.vy;
			if (z >= 0x21 && z < 0x1000) {
				*q++ = z;
			} else {
				*q++ = -1;
			}
		}
	}

	q = pts;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < 10; q += 3, j++) {
			SetPolyFT4(p);
			p->code |= 2;
			p->r0 = p->g0 = p->b0 = colors[i];
			p->tpage = m->pixelPage | 0x20;
			p->clut = GetClut((m->clutPage & 0x3f) << 4, m->clutPage >> 6);
			setUVWH(p, m->pixelOffsetX + ((i + start) & 1) * 24, m->pixelOffsetY, 0x17, 0x1f);
			if (j == 9) {
				goto wrap;
			}
#if defined(VERSION_JP)
			if (q[2] < 0x21 || q[5] < 0x21 || q[32] < 0x21 || q[35] < 0x21) {
#else
			if (q[2] < 0x21 || q[2] >= 0x1000 || q[5] < 0x21 || q[5] >= 0x1000 || q[32] < 0x21 || q[32] >= 0x1000 ||
			    q[35] < 0x21 || q[35] >= 0x1000) {
#endif
				continue;
			}
			setXY2(p, q[33], q[34], q[3], q[4]);
emit:
			p->x2 = q[30];
			p->y2 = q[31];
			p->x3 = q[0];
			p->y3 = q[1];
			AddPrim(&ACTIVE_ORDERING_TABLE->org[q[2]], p);
			p++;
			continue;
wrap:
#if defined(VERSION_JP)
			if (q[2] < 0x21 || q[-25] < 0x21 || q[32] < 0x21 || q[5] < 0x21) {
#else
			if (q[2] < 0x21 || q[2] >= 0x1000 || q[-25] < 0x21 || q[-25] >= 0x1000 || q[32] < 0x21 || q[32] >= 0x1000 ||
			    q[5] < 0x21 || q[5] >= 0x1000) {
#endif
				continue;
			}
			setXY2(p, q[3], q[4], q[-27], q[-26]);
			goto emit;
		}
	}
	GsSetWorkBase((PACKET *)p);
}

void BTL_tickRibbonPoints(void)
{
	SVECTOR *q;
	int32_t i;
	int32_t t;

	q = EFE_POP1(SVECTOR *);
	EFE_RIBBON_SCRATCH->frame = EFE_CURRENT_DATA_SEGMENT->frame;
	for (i = 0; i < 10; i++) {
		if (((EFE_RIBBON_SCRATCH->frame + i * 8) % 10) == 0) {
			q[i].pad = customRandom(-0xf, 0xf);
		}
		q[i].vy += q[i].pad;
		if (q[i].vy < -0x96) {
			t = -0x96;
		} else {
			t = q[i].vy > 0x96 ? 0x96 : q[i].vy;
		}
		q[i].vy = t;
		q[i].vz += q[i].pad;
		q[i].vz = q[i].vz < -0x96 ? -0x96 : (q[i].vz > 0x96 ? 0x96 : q[i].vz);
	}
}

void BTL_initializeRibbonPoints(void)
{
	int32_t i;
	SVECTOR *q;

	q = EFE_POP1(SVECTOR *);
	for (i = 0; i < 10; i++) {
		q[i].vx = i * 0x190 - 0x708;
		q[i].vy = customRandom(-0x96, 0x96);
		q[i].pad = customRandom(-0x1e, 0x1e);
		q[i].vz = 0;
	}
}

void BTL_renderRadialWaves(void)
{
	POLY_FT4 *prim;
	int16_t *uv;
	int32_t i;

	for (i = 0; i < 0x18U; i += 2) {
		BTL_RADIAL_WAVE_UVS[i] += EFE_DATA_ITERATOR->model->pixelOffsetX;
		BTL_RADIAL_WAVE_UVS[i + 1] += EFE_DATA_ITERATOR->model->pixelOffsetY;
	}
	EFE_WAVE_SCRATCH->tpage = EFE_DATA_ITERATOR->model->pixelPage | 0x20;
	EFE_WAVE_SCRATCH->clut = EFE_DATA_ITERATOR->model->clutPage + 0x40;
	EFE_SCRATCH->scale = EFE_POP1(VECTOR *);
	EFE_SCRATCH->rot.vx = ((int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x10))[0];
	EFE_SCRATCH->rot.vy = ((int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x10))[1];
	EFE_SCRATCH->rot.vz = ((int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x10))[2];
	EFE_SCRATCH->m1.t[0] = ((int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))[0];
	EFE_SCRATCH->m1.t[1] = ((int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))[1];
	EFE_SCRATCH->m1.t[2] = ((int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))[2];
	EFE_WAVE_SCRATCH->phase = EFE_CURRENT_DATA_SEGMENT->frame * -400;

	for (EFE_WAVE_SCRATCH->ring = 0; EFE_WAVE_SCRATCH->ring < 6; (EFE_WAVE_SCRATCH->ring)++) {
		EFE_SCRATCH->rot.vy += 0x2aa;
		RotMatrixYXZ(&EFE_SCRATCH->rot, &EFE_SCRATCH->m1);
		ScaleMatrix(&EFE_SCRATCH->m1, EFE_SCRATCH->scale);
		GsMulCoord0(&GsWSMATRIX, &EFE_SCRATCH->m1, &EFE_SCRATCH->m0);
		GsSetLightMatrix(&EFE_SCRATCH->m1);
		GsSetLsMatrix(&EFE_SCRATCH->m0);
		EFE_WAVE_SCRATCH->radius = 0xc8;
		for (; EFE_WAVE_SCRATCH->radius < 0xbb8;
		     EFE_WAVE_SCRATCH->radius += 0x64) {
			EFE_WAVE_SCRATCH->height = (EFE_WAVE_SCRATCH->radius < 0x2ef)
			                                   ? BTL_interpolateClamped(0xc8, 0x2ee, *(long *)&EFE_WAVE_SCRATCH->radius, 0xc8, 0xfa)
			                           : (EFE_WAVE_SCRATCH->radius < 0x8cb)
			                                   ? BTL_interpolateClamped(0x2ee, 0x8ca, *(long *)&EFE_WAVE_SCRATCH->radius, 0xfa, 0x7d)
			                                   : BTL_interpolateClamped(0x8ca, 0xbb8, *(long *)&EFE_WAVE_SCRATCH->radius, 0x7d, 0xa);
			i = EFE_WAVE_SCRATCH->phase + EFE_WAVE_SCRATCH->radius * 4 +
			    EFE_WAVE_SCRATCH->ring * 0x309;
			EFE_WAVE_SCRATCH->height =
				((EFE_WAVE_SCRATCH->height * rsin(i)) >> 12) -
				EFE_WAVE_SCRATCH->height - 0xc8;
			EFE_WAVE_SCRATCH->halfWidth = EFE_WAVE_SCRATCH->radius / 5;
			if (EFE_WAVE_SCRATCH->radius != 0xc8) {
				prim = (POLY_FT4 *)GsGetWorkBase();
				EFE_WAVE_SCRATCH->quad[0].vx = -EFE_WAVE_SCRATCH->halfWidth;
				EFE_WAVE_SCRATCH->quad[0].vy = EFE_WAVE_SCRATCH->height;
				EFE_WAVE_SCRATCH->quad[0].vz = EFE_WAVE_SCRATCH->radius;
				EFE_WAVE_SCRATCH->quad[1].vx = EFE_WAVE_SCRATCH->halfWidth;
				EFE_WAVE_SCRATCH->quad[1].vy = EFE_WAVE_SCRATCH->height;
				EFE_WAVE_SCRATCH->quad[1].vz = EFE_WAVE_SCRATCH->radius;
				EFE_WAVE_SCRATCH->quad[2].vx = -EFE_WAVE_SCRATCH->prevHalfWidth;
				EFE_WAVE_SCRATCH->quad[2].vy = EFE_WAVE_SCRATCH->prevHeight;
				EFE_WAVE_SCRATCH->quad[2].vz = EFE_WAVE_SCRATCH->prevRadius;
				EFE_WAVE_SCRATCH->quad[3].vx = EFE_WAVE_SCRATCH->prevHalfWidth;
				EFE_WAVE_SCRATCH->quad[3].vy = EFE_WAVE_SCRATCH->prevHeight;
				EFE_WAVE_SCRATCH->quad[3].vz = EFE_WAVE_SCRATCH->prevRadius;
				EFE_PROJ_SCRATCH->otz = RotTransPers4(
					&EFE_WAVE_SCRATCH->quad[0], &EFE_WAVE_SCRATCH->quad[1], &EFE_WAVE_SCRATCH->quad[2],
					&EFE_WAVE_SCRATCH->quad[3], (long *)&prim->x0, (long *)&prim->x1,
					(long *)&prim->x2, (long *)&prim->x3, &EFE_PROJ_SCRATCH->p,
					&EFE_PROJ_SCRATCH->flag);
				if ((EFE_PROJ_SCRATCH->flag & 0x80000000) == 0) {
					((int32_t *)prim)[1] = BTL_RADIAL_WAVE_COLOR;
					prim->clut = EFE_WAVE_SCRATCH->clut;
					prim->tpage = EFE_WAVE_SCRATCH->tpage;
					uv = (EFE_WAVE_SCRATCH->radius == 0x12c)   ? (int16_t *)&BTL_RADIAL_WAVE_UVS[8]
					     : (EFE_WAVE_SCRATCH->radius >= 0xb54) ? (int16_t *)&BTL_RADIAL_WAVE_UVS[16]
					                                           : (int16_t *)BTL_RADIAL_WAVE_UVS;
					*(int16_t *)&prim->u0 = uv[0];
					*(int16_t *)&prim->u1 = uv[1];
					*(int16_t *)&prim->u2 = uv[2];
					*(int16_t *)&prim->u3 = uv[3];
					setlen(prim, 9);
					prim->code = 0x2c;
					setSemiTrans(prim, 1);
					setShadeTex(prim, 0);
					addPrim(ACTIVE_ORDERING_TABLE->org + (EFE_PROJ_SCRATCH->otz >> 2),
					        prim);
					GsSetWorkBase((PACKET *)(prim + 1));
				}
			}
			EFE_WAVE_SCRATCH->prevHalfWidth = EFE_WAVE_SCRATCH->halfWidth;
			EFE_WAVE_SCRATCH->prevHeight = EFE_WAVE_SCRATCH->height;
			EFE_WAVE_SCRATCH->prevRadius = EFE_WAVE_SCRATCH->radius;
		}
	}
	for (i = 0; i < 0x18U; i += 2) {
		BTL_RADIAL_WAVE_UVS[i] -= EFE_DATA_ITERATOR->model->pixelOffsetX;
		BTL_RADIAL_WAVE_UVS[i + 1] -= EFE_DATA_ITERATOR->model->pixelOffsetY;
	}
}

void BTL_getViewportDistance(void)
{
	int32_t *out;

	out = EFE_POP1(int32_t *);
	*out = VIEWPORT_DISTANCE;
}

void BTL_addClutLoadPrim(void)
{
	char *src;
	int32_t z;
	int32_t y;
	int32_t idx;
	RECT rect;
	DR_LOAD *prim;

	idx = EFE_POP1(int32_t);
	src = EFE_POP1(char *);
	y = EFE_POP1(int32_t);
	z = EFE_POP1(int32_t);
	src += idx * 2;
	prim = (DR_LOAD *)GsGetWorkBase();
	GsSetWorkBase((PACKET *)(prim + 1));
	setRECT(&rect, (EFE_DATA_ITERATOR->model->clutPage & 0x3f) << 4, ((EFE_DATA_ITERATOR->model->clutPage >> 6) & 0x1ff) + y, 0x10, 1);
	SetDrawLoad(prim, &rect);
	memcpy(prim->p, src, 0x20);
	AddPrim(ACTIVE_ORDERING_TABLE->org + (z >> 4), prim);
}

void BTL_drawTMDScreenSpace(void)
{
	EFE_SCRATCH->scale = EFE_POP1(VECTOR *);
	EFE_SCRATCH->id = EFE_POP1(int32_t);
	copyVector(&EFE_SCRATCH->rot, (VECTOR *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x10));
	RotMatrixYXZ(&EFE_SCRATCH->rot, &EFE_SCRATCH->m0);
	ScaleMatrix(&EFE_SCRATCH->m0, EFE_SCRATCH->scale);
	EFE_SCRATCH->m0.t[0] = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vx;
	EFE_SCRATCH->m0.t[1] = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vy;
	EFE_SCRATCH->m0.t[2] = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vz;
	EFE_SCRATCH->m0.t[0] += -DRAWING_OFFSET_X + 0xa0;
	EFE_SCRATCH->m0.t[1] += -DRAWING_OFFSET_Y + 0x78;
	TransposeMatrix(&GsWSMATRIX, &EFE_SCRATCH->m2);
	MulMatrix0(&EFE_SCRATCH->m2, &EFE_SCRATCH->m0, &EFE_SCRATCH->m1);
	GsSetLightMatrix(&EFE_SCRATCH->m1);
	GsSetLsMatrix(&EFE_SCRATCH->m0);
	GsLinkObject4((unsigned long)EFE_DATA_ITERATOR->model->modelPtr->obj, &EFE_SCRATCH->obj, EFE_SCRATCH->id);
	EFE_SCRATCH->obj.coord2 = NULL;
	EFE_SCRATCH->obj.attribute = 0;
	GsSortObject4(&EFE_SCRATCH->obj, ACTIVE_ORDERING_TABLE, 2, EFE_SORT_WORKSPACE);
}

void BTL_loadClutColors(void)
{
	int32_t y;
	int32_t x;
	int32_t count;
	uint16_t *src;
	ModelComponent *m;
	int32_t i;
	uint16_t clut[16];
	RECT rect;

	src = EFE_POP1(uint16_t *);
	count = EFE_POP1(int32_t);
	x = EFE_POP1(int32_t);
	y = EFE_POP1(int32_t);

	if ((count > 0) && (count <= 16)) {
		m = EFE_DATA_ITERATOR->model;
		for (i = 0; i < count; i++) {
			clut[i] = *src++;
			clut[i] += *src++ << 5;
			clut[i] += *src++ << 10;
			clut[i] += *src++ << 15;
		}
		setRECT(&rect, ((m->clutPage & 0x3f) << 4) + x, (m->clutPage >> 6) + y, count, 1);
		LoadImage(&rect, (u_long *)clut);
		DrawSync(0);
	}
}

void BTL_drawTMDYXZ(void)
{
	EFE_SCRATCH->scale = EFE_POP1(VECTOR *);
	EFE_SCRATCH->id = EFE_POP1(int32_t);
	EFE_SCRATCH->rot.vx = ((int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x10))[0];
	EFE_SCRATCH->rot.vy = ((int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x10))[1];
	EFE_SCRATCH->rot.vz = ((int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x10))[2];
	RotMatrixYXZ(&EFE_SCRATCH->rot, &EFE_SCRATCH->m1);
	ScaleMatrix(&EFE_SCRATCH->m1, EFE_SCRATCH->scale);
	EFE_SCRATCH->m1.t[0] = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vx;
	EFE_SCRATCH->m1.t[1] = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vy;
	EFE_SCRATCH->m1.t[2] = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vz;
	GsMulCoord0(&GsWSMATRIX, &EFE_SCRATCH->m1, &EFE_SCRATCH->m0);
	if (EFE_SCRATCH->m0.t[2] < -0x12c) {
		return;
	}

	if (EFE_SCRATCH->m0.t[2] >= 0x10000) {
		return;
	}

	GsSetLightMatrix(&EFE_SCRATCH->m1);
	GsSetLsMatrix(&EFE_SCRATCH->m0);
	GsLinkObject4((unsigned long)EFE_DATA_ITERATOR->model->modelPtr->obj, &EFE_SCRATCH->obj, EFE_SCRATCH->id);
	EFE_SCRATCH->obj.coord2 = NULL;
	EFE_SCRATCH->obj.attribute = 0;
	GsSortObject4(&EFE_SCRATCH->obj, ACTIVE_ORDERING_TABLE, 2, EFE_SORT_WORKSPACE);
}

void BTL_getCameraRotation(void)
{
	MATRIX m;
	SVECTOR rot;
	EfeVector *out;

	out = EFE_POP1(EfeVector *);
	TransposeMatrix(&GsWSMATRIX, &m);
	matrixToEuler2(&m, &rot);
	copyVector(out, &rot);
}

void BTL_selectRandomTargetEntity(void)
{
	Entity *entity;
	int32_t excludeSelf;
	int32_t n;

	excludeSelf = *--EFE_DATA_STACK;
	for (n = rand() % 8; n >= 0; n--) {
		EFE_TARGET_ENTITY_INDEX++;
		while (1) {
			while (EFE_TARGET_ENTITY_INDEX < 10) {
				entity = ENTITY_TABLE[EFE_TARGET_ENTITY_INDEX];
				if (entity != NULL && entity->isOnMap != 0 && ((DigimonEntity *)entity)->stats.current.currentHP > 0) {
					if (excludeSelf != 0 || entity != EFE_SCRIPT_CONTEXT->sourceEntity) {
						goto next;
					}
				}
				EFE_TARGET_ENTITY_INDEX++;
			}
			if (++EFE_TARGET_ENTITY_INDEX >= 10) {
				EFE_TARGET_ENTITY_INDEX = 1;
			}
		}
next:;
	}
	EFE_SCRIPT_CONTEXT->targetEntity = entity;
}

void BTL_convertToViewSpace(void)
{
	VECTOR *out1;
	VECTOR *out2;
	VECTOR *trans;
	VECTOR *rotIn;
	MATRIX m1;
	MATRIX m2;
	SVECTOR rot;

	rotIn = EFE_POP1(VECTOR *);
	trans = EFE_POP1(VECTOR *);
	out2 = EFE_POP1(VECTOR *);
	out1 = EFE_POP1(VECTOR *);
	copyVector(&rot, rotIn);
	copyVector((VECTOR *)m1.t, trans);
	RotMatrixYXZ(&rot, &m1);
	GsMulCoord0(&GsWSMATRIX, &m1, &m2);
	matrixToEuler2(&m2, &rot);
	copyVector(out1, (VECTOR *)m2.t);
	copyVector(out2, &rot);
}

void BTL_maskVectorByScalar(void)
{
	int32_t mask;
	EfeVector *v;

	mask = EFE_POP1(int32_t);
	v = EFE_POP1(EfeVector *);
	v->vx &= mask;
	v->vy &= mask;
	v->vz &= mask;
}

void BTL_divideVectorByScalar(void)
{
	int32_t k;
	EfeVector *v;

	k = EFE_POP1(int32_t);
	v = EFE_POP1(EfeVector *);
	v->vx /= k;
	v->vy /= k;
	v->vz /= k;
}

void BTL_multiplyVectorByScalar(void)
{
	int32_t k;
	EfeVector *v;

	k = EFE_POP1(int32_t);
	v = EFE_POP1(EfeVector *);
	v->vx *= k;
	v->vy *= k;
	v->vz *= k;
}

void BTL_render3DTexturedQuad(void)
{
	POLY_FT4 *prim;
	ModelComponent *m;
	EfeVector *p1;
	EfeVector *p2;
	EfeVector *p3;
	EfeVector *p4;
	EfeColor *col;
	int32_t u0off;
	int32_t v0off;
	int32_t du;
	int32_t dv;
	int32_t clutY;
	int32_t semi;
	SVECTOR a;
	SVECTOR b;
	SVECTOR c;
	SVECTOR d;

	semi = EFE_POP1(int32_t);
	clutY = EFE_POP1(int32_t);
	dv = EFE_POP1(int32_t);
	du = EFE_POP1(int32_t);
	v0off = EFE_POP1(int32_t);
	u0off = EFE_POP1(int32_t);
	col = EFE_POP1(EfeColor *);
	p4 = EFE_POP1(EfeVector *);
	p3 = EFE_POP1(EfeVector *);
	p2 = EFE_POP1(EfeVector *);
	p1 = EFE_POP1(EfeVector *);
	m = EFE_DATA_ITERATOR->model;

	copyVector(&a, p1);
	copyVector(&b, p2);
	copyVector(&c, p3);
	copyVector(&d, p4);

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	if (semi != 0) {
		prim->code |= 2;
	}

	setRGB0(prim, col->red, col->green, col->blue);
	prim->tpage = m->pixelPage | semi;
	prim->clut = GetClut((m->clutPage & 0x3f) << 4, (m->clutPage >> 6) + clutY);
	setUVWH(prim, m->pixelOffsetX + u0off, m->pixelOffsetY + v0off, du, dv);
	addScreenPolyFT4(prim, &a, &b, &c, &d);
}

void BTL_setTransformToBoneMatrix(void)
{
	MATRIX m;
	SVECTOR v;
	SVECTOR out;
	long *p;

	p = (long *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4);
	calculateBoneMatrix(EFE_SCRIPT_CONTEXT->sourceEntity, EFE_SCRIPT_CONTEXT->boneOffset->boneId, &m);
	v.vx = EFE_SCRIPT_CONTEXT->boneOffset->positionX;
	v.vy = EFE_SCRIPT_CONTEXT->boneOffset->positionY;
	v.vz = EFE_SCRIPT_CONTEXT->boneOffset->positionZ;
	ApplyMatrixSV(&m, &v, &out);
	p[0] = out.vx;
	p[1] = out.vy;
	p[2] = out.vz;
	*p++ += m.t[0];
	*p++ += m.t[1];
	*p++ += m.t[2];
	matrixToEuler2(&m, &out);
	*p++ = out.vx;
	*p++ = out.vy;
	*p = out.vz;
}

void BTL_renderWireframeBox(void)
{
	EfeVector *base;
	EfeVector *ext;
	EfeColor *col;
	SVECTOR p;
	DVECTOR pts[8];
	GsOT_TAG *ot;
	int32_t k;
	int32_t ox;
	int32_t oy;
	MATRIX m;
	SVECTOR rot;
	VECTOR scale;
	uint8_t *idx;
	LINE_F4 *prim;
	int32_t i;
	int16_t bx;
	int16_t by;
	int16_t bz;
	int16_t ex;
	int16_t ey;
	int16_t ez;

	col = EFE_POP1(EfeColor *);
	ext = EFE_POP1(EfeVector *);
	base = EFE_POP1(EfeVector *);

	PushMatrix();
	rot.vx = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x10);
	rot.vy = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x14);
	rot.vz = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x18);
	RotMatrixZYX(&rot, &m);
	scale.vx = 0x1000;
	scale.vy = 0x1000;
	scale.vz = 0x1000;
	ScaleMatrix(&m, &scale);
	PopMatrix();
	setRotTransMatrix(&GsWSMATRIX);
	ex = ext->vx;
	ey = ext->vy;
	ez = ext->vz;
	bx = base->vx;
	by = base->vy;
	bz = base->vz;
	PushMatrix();
	getDrawingOffsetCopy(&ox, &oy);
	for (i = 0; i < 8; i++) {
		p.vx = ex * BTL_WIREFRAME_BOX_SIGN_X[i];
		p.vy = ey * BTL_WIREFRAME_BOX_SIGN_Y[i];
		p.vz = ez * BTL_WIREFRAME_BOX_SIGN_Z[i];
		ApplyMatrixSV(&m, &p, &p);
		p.vx += bx;
		p.vy += by;
		p.vz += bz;
		setRotTransMatrix(&GsWSMATRIX);
		gte_ldv0(&p);
		gte_rtps();
		gte_stsxy(&pts[i]);
		pts[i].vx += (int16_t)(0xa0 - ox);
		pts[i].vy += (int16_t)(0x78 - oy);
	}

	PopMatrix();
	ot = ACTIVE_ORDERING_TABLE->org;
	prim = (LINE_F4 *)GsGetWorkBase();
	idx = BTL_WIREFRAME_BOX_LINES;
	for (k = 0; k < 4; k++) {
		SetLineF4(prim);
		setRGB0(prim, col->red, col->green, col->blue);
		setXY4(prim, pts[*idx].vx, pts[*idx++].vy, pts[*idx].vx, pts[*idx++].vy, pts[*idx].vx, pts[*idx++].vy, pts[*idx].vx, pts[*idx++].vy);
		AddPrim(ot + 0x22, prim);
		prim++;
	}

	GsSetWorkBase((PACKET *)prim);
}

void BTL_discardEFEOperand(void)
{
	int32_t value;

	value = EFE_POP1(int32_t);
}

void BTL_renderWireframeGrid(void)
{
	EfeVector *base;
	EfeVector *rotSrc;
	int32_t y;
	int32_t *col;
	int32_t x1;
	int32_t x2;
	int32_t n1;
	int32_t z1;
	int32_t z2;
	int32_t n2;
	SVECTOR c0;
	SVECTOR c1;
	SVECTOR c2;
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
	MATRIX m;
	SVECTOR rot;

	y = EFE_POP1(int32_t);
	n2 = EFE_POP1(int32_t);
	z2 = EFE_POP1(int32_t);
	z1 = EFE_POP1(int32_t);
	n1 = EFE_POP1(int32_t);
	x2 = EFE_POP1(int32_t);
	x1 = EFE_POP1(int32_t);
	col = EFE_POP1(int32_t *);
	rotSrc = EFE_POP1(EfeVector *);
	base = EFE_POP1(EfeVector *);

	PushMatrix();
	copyVector(&rot, rotSrc);
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

	c0.vx = c2.vx = base->vx + p0.vx;
	c0.vy = c2.vy = base->vy + p0.vy;
	c0.vz = c2.vz = base->vz + p0.vz;
	c1.vx = base->vx + p2.vx;
	c1.vy = base->vy + p2.vy;
	c1.vz = base->vz + p2.vz;
	BTL_renderParallelLines(&c0, &c1, (int16_t)n1, &p0, &p1, col);
	c1.vx = base->vx + p1.vx;
	c1.vy = base->vy + p1.vy;
	c1.vz = base->vz + p1.vz;
	BTL_renderParallelLines(&c2, &c1, (int16_t)n2, &p0, &p2, col);
}

void BTL_render2DTexturedQuad(void)
{
	int32_t a;
	int32_t b;
	int32_t c;
	int32_t d;
	int32_t depth;
	EfeColor *col;
	int32_t u0off;
	int32_t v0off;
	int32_t du;
	int32_t dv;
	POLY_FT4 *prim;
	ModelComponent *m;
	int32_t clutY;
	int32_t semi;

	semi = EFE_POP1(int32_t);
	clutY = EFE_POP1(int32_t);
	dv = EFE_POP1(int32_t);
	du = EFE_POP1(int32_t);
	v0off = EFE_POP1(int32_t);
	u0off = EFE_POP1(int32_t);
	col = EFE_POP1(EfeColor *);
	depth = EFE_POP1(int32_t);
	d = EFE_POP1(int32_t);
	c = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t);
	a = EFE_POP1(int32_t);

	if ((depth > 0x20) && (depth < 0x1000)) {
		m = EFE_DATA_ITERATOR->model;
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		if (semi < 0) {
			semi = -semi & 0xffff;
			setXY4(prim, ((int32_t *)a)[0], ((int32_t *)a)[1], ((int32_t *)b)[0], ((int32_t *)b)[1], ((int32_t *)c)[0], ((int32_t *)c)[1], ((int32_t *)d)[0], ((int32_t *)d)[1]);
		} else {
			setXYWH(prim, a, b, c, d);
		}
		if (semi != 0) {
			prim->code |= 2;
		}
		setRGB0(prim, col->red, col->green, col->blue);
		prim->tpage = m->pixelPage | semi;
		prim->clut = GetClut((m->clutPage & 0x3f) << 4, (m->clutPage >> 6) + clutY);
		setUVWH(prim, m->pixelOffsetX + u0off, m->pixelOffsetY + v0off, du, dv);
		AddPrim(ACTIVE_ORDERING_TABLE->org + depth, prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}
}

void BTL_restoreCameraView(void)
{
	GsRVIEW2 view;
	int32_t dist;
	int32_t x;
	int32_t y;

	getRViewCopy(&view);
	getViewportDistanceCopy(&dist);
	getDrawingOffsetCopy(&x, &y);
	DRAWING_OFFSET_X = x;
	DRAWING_OFFSET_Y = y;
	GsSetProjection(dist);
	GsSetRefView2(&view);
}

void BTL_setupFixedCamera(void)
{
	int32_t ret;

	GsSetProjection(0x200);
	EFE_FIXED_VIEW.vpx = 0;
	EFE_FIXED_VIEW.vpz = -0x7d0;
	EFE_FIXED_VIEW.vpy = 0;
	EFE_FIXED_VIEW.vrx = 0;
	EFE_FIXED_VIEW.vry = 0;
	EFE_FIXED_VIEW.vrz = 0;
	EFE_FIXED_VIEW.rz = 0;
	EFE_FIXED_VIEW.super = NULL;
	ret = GsSetRefView2(&EFE_FIXED_VIEW);
	DRAWING_OFFSET_X = 0xa0;
	DRAWING_OFFSET_Y = 0x78;
}

void BTL_getSourceBoneTransform(void)
{
	MATRIX m;
	SVECTOR rot;
	EfeVector *posOut;
	EfeVector *rotOut;
	int32_t bone;

	rotOut = EFE_POP1(EfeVector *);
	posOut = EFE_POP1(EfeVector *);
	bone = EFE_POP1(int32_t);
	calculateBoneMatrix(EFE_SCRIPT_CONTEXT->sourceEntity, bone, &m);
	matrixToEuler2(&m, &rot);
	copyVector(rotOut, &rot);
	posOut->vx = m.t[0];
	posOut->vy = m.t[1];
	posOut->vz = m.t[2];
}

void BTL_copyToParentTransform(void)
{
	int32_t *dst;
	int32_t src;
	int32_t off;
	int32_t size;

	size = EFE_POP1(int32_t);
	off = EFE_POP1(int32_t);
	src = EFE_POP1(int32_t);
	dst = (int32_t *)(EFE_PREVIOUS_DATA_SEGMENT + off);
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

void BTL_renderEFELine(void)
{
	EfeColor *col;
	int32_t i;
	int32_t depth;
	int32_t y0;
	int32_t x1;
	int32_t y1;
	int32_t x0;
	int32_t flags;

	flags = EFE_POP1(int32_t);
	depth = EFE_POP1(int32_t);
	col = EFE_POP1(EfeColor *);
	x0 = EFE_POP1(int32_t);
	y1 = EFE_POP1(int32_t);
	x1 = EFE_POP1(int32_t);
	y0 = EFE_POP1(int32_t);

	if ((depth > 0x20) && (depth < 0x1000)) {
		if ((flags & 0x20) != 0) {
			for (i = 0; i < 4; i++) {
				drawLine2P(((col->red * 50 / 100) & 0xff) | (((col->green * 50 / 100) & 0xff) << 8) | (((col->blue * 50 / 100) & 0xff) << 16), y0 + BTL_LINE_OFFSET_X[i], x1 + BTL_LINE_OFFSET_Y[i], y1 + BTL_LINE_OFFSET_X[i], x0 + BTL_LINE_OFFSET_Y[i], depth, 5);
			}
		} else {
			drawLine2P((col->red & 0xff) | ((col->green & 0xff) << 8) | ((col->blue & 0xff) << 16), y0, x1, y1, x0, depth, 0);
		}
	}
}

void BTL_combineRotations(void)
{
	SVECTOR r1;
	SVECTOR r2;
	VECTOR *b;
	VECTOR *a;

	b = EFE_POP1(VECTOR *);
	a = EFE_POP1(VECTOR *);
	copyVector(&r1, a);
	copyVector(&r2, b);
	multiplyRotations(&r1, &r2);
	copyVector(a, &r1);
}

void BTL_normalizeRotationAngles2(void)
{
	MATRIX m;
	SVECTOR rot;
	EfeVector *out;
	EfeVector *v;

	v = EFE_POP1(EfeVector *);
	out = EFE_POP1(EfeVector *);
	copyVector(&rot, v);
	RotMatrixYXZ(&rot, &m);
	matrixToEuler1(&m, &rot);
	copyVector(out, &rot);
}

void BTL_rotateVectorByAngles(void)
{
	MATRIX m;
	VECTOR res;
	SVECTOR rot;
	EfeVector *v;
	EfeVector *w;

	v = EFE_POP1(EfeVector *);
	w = EFE_POP1(EfeVector *);
	copyVector(&rot, w);
	RotMatrixYXZ(&rot, &m);
	ApplyMatrixLV(&m, (VECTOR *)v, &res);
	copyVector(v, &res);
}

void BTL_getTargetBoneTransform(void)
{
	EfeVector *rotOut;
	EfeVector *posOut;
	int32_t idx;
	GsCOORDINATE2 *coord;
	MATRIX m;
	SVECTOR rot;
	GsCOORDINATE2 *matrix;

	rotOut = EFE_POP1(EfeVector *);
	posOut = EFE_POP1(EfeVector *);
	idx = EFE_POP1(int32_t);
	matrix = (GsCOORDINATE2 *)(((char **)(int32_t)EFE_SCRIPT_CONTEXT->targetEntity)[1] + 0x10);
	coord = (GsCOORDINATE2 *)(((char **)(int32_t)EFE_SCRIPT_CONTEXT->targetEntity)[1] + (idx * 136) + 0x10);
	RotMatrix((SVECTOR *)(((char **)(int32_t)EFE_SCRIPT_CONTEXT->targetEntity)[1] + 0x70), &matrix->coord);
	ScaleMatrix(&matrix->coord, (VECTOR *)(((char **)(int32_t)EFE_SCRIPT_CONTEXT->targetEntity)[1] + 0x60));
	TransMatrix(&matrix->coord, (VECTOR *)(((char **)(int32_t)EFE_SCRIPT_CONTEXT->targetEntity)[1] + 0x78));
	calculatePosition(coord, &m);
	matrixToEuler2(&m, &rot);
	copyVector(rotOut, &rot);
	posOut->vx = m.t[0];
	posOut->vy = m.t[1];
	posOut->vz = m.t[2];
}

void BTL_centerTransformOnEntities(void)
{
	EfeTransform *sum;
	int32_t i;
	int32_t count;
	MATRIX *m;

	sum = (EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4);
	sum->position.vx = 0;
	sum->position.vy = 0;
	sum->position.vz = 0;
	*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x10) = 0;
	*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x14) = 0;
	*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x18) = 0;
	count = 0;
	for (i = 1; i < 10; i++) {
		if (ENTITY_TABLE[i] == NULL) {
			continue;
		}
		if (ENTITY_TABLE[i] == EFE_SCRIPT_CONTEXT->sourceEntity) {
			continue;
		}
		if (ENTITY_TABLE[i]->isOnMap == 0) {
			continue;
		}
		count++;
		m = &ENTITY_TABLE[i]->posData->posMatrix.workm;
		sum->position.vx = sum->position.vx + m->t[0];
		sum->position.vy = sum->position.vy + m->t[1];
		sum->position.vz = sum->position.vz + m->t[2];
	}

	sum->position.vx /= count;
	sum->position.vy /= count;
	sum->position.vz /= count;
}

void BTL_shiftVectorsRight(void)
{
	EfeVector *a;
	EfeVector *b;

	b = EFE_POP1(EfeVector *);
	a = EFE_POP1(EfeVector *);
	a->vx >>= b->vx;
	a->vy >>= b->vy;
	a->vz >>= b->vz;
}

void BTL_maskVectors(void)
{
	EfeVector *a;
	EfeVector *b;

	b = EFE_POP1(EfeVector *);
	a = EFE_POP1(EfeVector *);
	a->vx &= b->vx;
	a->vy &= b->vy;
	a->vz &= b->vz;
}

void BTL_divideVectors(void)
{
	EfeVector *a;
	EfeVector *b;

	b = EFE_POP1(EfeVector *);
	a = EFE_POP1(EfeVector *);
	a->vx /= b->vx;
	a->vy /= b->vy;
	a->vz /= b->vz;
}

void BTL_multiplyVectors(void)
{
	EfeVector *a;
	EfeVector *b;

	b = EFE_POP1(EfeVector *);
	a = EFE_POP1(EfeVector *);
	a->vx *= b->vx;
	a->vy *= b->vy;
	a->vz *= b->vz;
}

void BTL_subtractVectors(void)
{
	EfeVector *a;
	EfeVector *b;

	b = EFE_POP1(EfeVector *);
	a = EFE_POP1(EfeVector *);
	a->vx -= b->vx;
	a->vy -= b->vy;
	a->vz -= b->vz;
}

void BTL_addVectors(void)
{
	EfeVector *a;
	EfeVector *b;

	b = EFE_POP1(EfeVector *);
	a = EFE_POP1(EfeVector *);
	addVector(a, b);
}

void BTL_copyVector(void)
{
	EfeVector *dst;
	EfeVector *src;

	src = EFE_POP1(EfeVector *);
	dst = EFE_POP1(EfeVector *);
	copyVector(dst, src);
}

void BTL_getVectorLength(void)
{
	EfeVector *v;
	int32_t *out;

	v = EFE_POP1(EfeVector *);
	out = EFE_POP1(int32_t *);
	*out = getDistance(v->vx, v->vy, v->vz);
}

void BTL_setTargetToHitEntity(void)
{
	EFE_SCRIPT_CONTEXT->targetEntity = ENTITY_TABLE[EFE_HIT_ENTITY_INDEX];
}

void BTL_normalizeRotationAngles(void)
{
	SVECTOR rot;
	SVECTOR res;
	MATRIX m;
	EfeVector *out;
	EfeVector *v;

	v = EFE_POP1(EfeVector *);
	out = EFE_POP1(EfeVector *);
	copyVector(&rot, v);
	RotMatrixYXZ(&rot, &m);
	matrixToEuler1(&m, &res);
	copyVector(out, &res);
}

void BTL_findHitEntity(void)
{
	SVECTOR center;
	AABB box;
	int32_t *out;
	EfeVector *ext;
	int32_t mode;

	ext = EFE_POP1(EfeVector *);
	mode = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	*out = 0;
	center.vx = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vx;
	center.vy = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 8);
	center.vz = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0xc);
	box.center = &center;
	copyVector(&box.extent, ext);
	EFE_HIT_ENTITY_INDEX = 1;
	while (1) {
		if ((EFE_HIT_ENTITY_INDEX = findAABBHitEntity(&box, EFE_SCRIPT_CONTEXT->sourceEntity, EFE_HIT_ENTITY_INDEX)) == -1) {
			return;
		}
		switch (mode) {
		case 0:
			if (((DigimonEntity *)ENTITY_TABLE[EFE_HIT_ENTITY_INDEX])->stats.current.isHit == 0) {
				*out = 1;
				return;
			}
			EFE_HIT_ENTITY_INDEX++;
			break;
		case 1:
			*out = 1;
			return;
		default:
			return;
		}
	}
}

void BTL_getVectorEulerAngles(void)
{
	SVECTOR rot;
	EfeVector *out;
	EfeVector *v;

	v = EFE_POP1(EfeVector *);
	out = EFE_POP1(EfeVector *);
	toEulerAngles(&rot, v->vx, v->vy, v->vz);
	copyVector(out, &rot);
}

void BTL_printDebugValue(void)
{
	int32_t value;

	value = EFE_POP1(int32_t);
	printf(BTL_FMT_D, value);
}

void BTL_getRandomInRange(void)
{
	int32_t b;
	int32_t a;
	int32_t *out;

	a = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	*out = customRandom(b, a);
}

void BTL_interpolateValue(void)
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

void BTL_calculateCosine(void)
{
	int32_t *out;
	int32_t angle;
	int32_t scale;

	out = EFE_POP1(int32_t *);
	scale = EFE_POP1(int32_t);
	angle = EFE_POP1(int32_t);
	*out = (scale * rcos(angle & 0xfff)) >> 12;
}

void BTL_calculateSine(void)
{
	int32_t *out;
	int32_t angle;
	int32_t scale;

	out = EFE_POP1(int32_t *);
	scale = EFE_POP1(int32_t);
	angle = EFE_POP1(int32_t);
	*out = (scale * rsin(angle & 0xfff)) >> 12;
}

void BTL_getSourceDigimonSize(void)
{
	EfeVector *out;
	int16_t radius;
	int16_t height;

	out = EFE_POP1(EfeVector *);
	radius = DIGIMON_DATA[EFE_SCRIPT_CONTEXT->sourceEntity->type].radius;
	height = DIGIMON_DATA[EFE_SCRIPT_CONTEXT->sourceEntity->type].height;
	out->vx = radius;
	out->vy = height;
	out->vz = radius;
}

void BTL_applyHomingMovement(void)
{
	EfeVector *target;
	int32_t minSpeed;
	int32_t maxSpeed;
	int32_t unused;
	int32_t limit;
	int32_t dy;
	int32_t dp;
	int32_t *pos;
	SVECTOR d;
	SVECTOR rot;
	SVECTOR out;
	MATRIX m;
	int32_t *speed;
	int32_t yaw;
	int32_t pitch;

	limit = EFE_POP1(int32_t);
	speed = EFE_POP1(int32_t *);
	unused = EFE_POP1(int32_t);
	maxSpeed = EFE_POP1(int32_t);
	minSpeed = EFE_POP1(int32_t);
	target = EFE_POP1(EfeVector *);

	pos = (int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4);
	d.vx = target->vx - pos[0];
	d.vy = target->vy - pos[1];
	d.vz = target->vz - pos[2];
	rot.vx = 0;
	rot.vy = -*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x14);
	rot.vz = 0;
	RotMatrixZYX(&rot, &m);
	ApplyMatrixSV(&m, &d, &out);

	yaw = _atan(out.vx, out.vz);
	yaw -= 0x400;
	yaw = -yaw;
	yaw &= 0xfff;
	if (yaw >= 0x801) {
		yaw -= 0x1000;
	}
	if (yaw > 0) {
		if (yaw < limit) {
			dy = yaw;
		} else {
			dy = limit;
		}
	} else {
		if (-yaw < limit) {
			dy = yaw;
		} else {
			dy = -limit;
		}
	}
	*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x14) += dy;

	pitch = _atan(out.vy, out.vz);
	pitch -= 0x400;
	pitch -= *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x18);
	pitch &= 0xfff;
	if (pitch >= 0x801) {
		pitch -= 0x1000;
	}
	if (pitch > 0) {
		if (pitch < limit) {
			dp = pitch;
		} else {
			dp = limit;
		}
	} else {
		if (-pitch < limit) {
			dp = pitch;
		} else {
			dp = -limit;
		}
	}
	*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x18) += dp;

	if (((*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x18) & 0xfff) >= 0x400L) && ((*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x18) & 0xfff) < 0xc00)) {
		*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x18) = 0x800 - *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x18);
	}

	if (limit * 40 / 100 >= abs(dp) + abs(dy)) {
		*speed = *speed * 130;
		*speed = *speed / 100;
		if (maxSpeed < *speed) {
			*speed = maxSpeed;
		}
	} else {
		*speed = *speed * 82;
		*speed = *speed / 100;
		if (*speed < minSpeed) {
			*speed = minSpeed;
		}
	}

	d.vx = 0;
	d.vy = 0;
	d.vz = -*speed << 3;
	rot.vx = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x18);
	rot.vy = 0;
	rot.vz = 0;
	RotMatrixZYX(&rot, &m);
	ApplyMatrixSV(&m, &d, &out);
	rot.vx = 0;
	rot.vy = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x14);
	rot.vz = 0;
	RotMatrixZYX(&rot, &m);
	ApplyMatrixSV(&m, &out, &out);
	pos[0] += out.vx >> 3;
	pos[1] += out.vy >> 3;
	pos[2] += out.vz >> 3;
}

void BTL_getUVAnimTimer(void)
{
	int32_t *out;
	EfeUvAnim *anim;
	int32_t idx;

	idx = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	anim = &EFE_DATA_ITERATOR->uvAnims[idx];
	*out = anim->frame;
}

void BTL_checkTargetCollision(void)
{
	EfeVector *out;
	int32_t *flag;
	int32_t *out2;
	int32_t r;
	long *tgt;
	int32_t dist;
	int32_t d0;
	int32_t d1;
	int16_t ang;

	r = EFE_POP1(int32_t);
	flag = EFE_POP1(int32_t *);
	out2 = EFE_POP1(int32_t *);
	out = EFE_POP1(EfeVector *);
	tgt = EFE_SCRIPT_CONTEXT->targetEntity->posData->posMatrix.workm.t;
	d0 = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vx - tgt[0];
	d1 = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0xc) - tgt[2];
	dist = d0 * d0 + d1 * d1;
	if (r * r < dist) {
		goto zero;
	}

	*flag = 1;
	ang = _atan(d0, d1);
	ang -= 0x400;
	ang = -ang;
	ang &= 0xfff;
	*out2 = ang + 0x800;
	ang += 0x800;
	ang >>= 3;
	out->vz = (r * _sin(0x80 - ang)) >> 12;
	out->vx = (r * _cos(0x80 - ang)) >> 12;
	return;
zero:
	*flag = 0;
}

void BTL_rotateTransformTowardPoint(void)
{
	EfeVector *p;
	EfeTransform *q;
	int32_t angle;

	p = EFE_POP1(EfeVector *);
	q = (EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4);
	angle = _atan(p->vx - q->position.vx, p->vz - q->position.vz);
	angle -= 0x400;
	angle = -angle;
	angle &= 0xfff;
	*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + (int32_t)&((EfeInstance *)0)->transform.rotation.vy) = angle;
}

void BTL_setTransformToSourceBone(void)
{
	int32_t idx;
	int32_t *p;
	MATRIX *bone;
	int32_t s1;
	int32_t s2;

	idx = EFE_POP1(int32_t);
	p = &((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vx;
	bone = &EFE_SCRIPT_CONTEXT->sourceEntity->posData[idx].posMatrix.workm;
	*p++ = bone->t[0];
	*p++ = bone->t[1];
	*p++ = bone->t[2];
	*p++ = 0;
	*p++ = EFE_SCRIPT_CONTEXT->sourceEntity->posData->rotation.vy;
	*p = 0;
	s1 = _sin(-(p[-1] >> 3));
	s2 = _sin(-(((p[-1] >> 3) + 0x80) & 0x1ff));
	p[-5] = p[-5] + ((s1 * 25) >> 12);
	p[-3] = p[-3] + ((s2 * 25) >> 12);
}

void BTL_renderParallaxSprites(void)
{
	POLY_FT4 *prim;
	EfeParallaxSprite *p;
	ModelComponent *m;
	int32_t ox;
	int32_t oy;
	int16_t x;
	int16_t y;
	int16_t sz;

	oy = EFE_POP1(int32_t);
	ox = EFE_POP1(int32_t);
	p = EFE_POP1(EfeParallaxSprite *);
	m = EFE_DATA_ITERATOR->model;
	prim = (POLY_FT4 *)GsGetWorkBase();

	while (1) {
		if (p->size < 0) {
			break;
		}
		SetPolyFT4(prim);
		prim->r0 = prim->g0 = prim->b0 = 0x80;
		prim->tpage = m->pixelPage | 0x20;
		prim->clut = m->clutPage + 0x40;
		setUVWH(prim, m->pixelOffsetX + p->u, m->pixelOffsetY, 0x1f, 0x1f);
		x = (p->depth * (p->x + ox)) >> 7;
		y = (p->depth * (p->y + oy)) >> 7;
		x %= 400;
		y %= 320;
		if (x < 0) {
			x += 360;
		}
		if (y < 0) {
			y += 280;
		}
		x -= 0xc8;
		y -= 0xa0;
		sz = (p->size * p->depth) >> 8;
		setXYWH(prim, x, y, sz, sz);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x1e, prim);
		prim++;
		p += 1;
	}

	GsSetWorkBase((PACKET *)prim);
}

void BTL_renderScrollingBackground(void)
{
	int16_t tx;
	ModelComponent *model;
	int16_t ty;
	int16_t ux;
	int16_t uy;
	char *tiles;
	int16_t col;
	POLY_FT4 *prim;
	int16_t row;
	int16_t *pal;
	int32_t sx;
	int32_t sy;
	int32_t color;
	int16_t x0;
	int16_t y0;

	color = EFE_POP1(int32_t);
	sy = EFE_POP1(int32_t);
	sx = EFE_POP1(int32_t);
	pal = EFE_POP1(int16_t *);
	tiles = EFE_POP1(char *);

	sx %= 0x140;
	tx = (sx >> 5) % 11;
	if (sx < 0) {
		sx += 0x140;
		tx += 10;
	}

	sy %= 0x100;
	ty = (sy >> 5) % 9;
	if (sy < 0) {
		sy += 0x100;
		ty += 8;
	}

	model = EFE_DATA_ITERATOR->model;
	prim = (POLY_FT4 *)GsGetWorkBase();
	y0 = (sy & 0x1f) - 0x98;
	for (row = 0; row < 9; row++) {
		x0 = (sx & 0x1f) - 0xc0;
		uy = ((ty + row) % 9) * 11;
		for (col = 0; col < 11; col++) {
			ux = (tx + col) % 11;
			SetPolyFT4(prim);
			SetSemiTrans(prim, 1);
			prim->code |= 2;
			prim->r0 = prim->g0 = prim->b0 = color;
			prim->tpage = model->pixelPage | 0x20;
			prim->clut = model->clutPage;
			setUVWH(prim, model->pixelOffsetX + pal[tiles[uy + ux]], model->pixelOffsetY, 0x1f, 0x1f);
			setXYWH(prim, x0, y0, 0x20, 0x20);
			AddPrim(ACTIVE_ORDERING_TABLE->org + 0x1e, prim);
			prim++;
			x0 += 0x20;
		}
		y0 += 0x20;
	}

	GsSetWorkBase((PACKET *)prim);
}

void BTL_setTransformToBoneOffset(void)
{
	MATRIX m;
	SVECTOR in;
	SVECTOR out;
	long *p;

	p = (long *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4);
	calculateBoneMatrix(EFE_SCRIPT_CONTEXT->sourceEntity,
	                    EFE_SCRIPT_CONTEXT->boneOffset->boneId, &m);
	in.vx = EFE_SCRIPT_CONTEXT->boneOffset->positionX;
	in.vy = EFE_SCRIPT_CONTEXT->boneOffset->positionY;
	in.vz = EFE_SCRIPT_CONTEXT->boneOffset->positionZ;
	ApplyMatrixSV(&m, &in, &out);
	p[0] = out.vx;
	p[1] = out.vy;
	p[2] = out.vz;
	*p++ += m.t[0];
	*p++ += m.t[1];
	*p++ += m.t[2];
	*p++ = 0;
	*p++ = EFE_SCRIPT_CONTEXT->sourceEntity->posData->rotation.vy;
	*p = 0;
}

void BTL_playEFESound(void)
{
	int32_t id;
	int32_t i;

	EfeSound *p;

	id = EFE_POP1(int32_t);
	if (id < 0) {
		return;
	}

	if ((id != 0x12) && (id != 0x20) && (id != 0x21) && (id != 0x22)) {
		playSound(8, id);
		return;
	}

	p = EFE_SOUND_DATA;
	for (i = 0; i < 10; i++) {
		if (p->mask < 0) {
			p->mask = playSound2(8, id);
			p->context = EFE_SCRIPT_CONTEXT;
			return;
		}
		p++;
	}
}

void BTL_addSourceEntityParticleFX(void)
{
	int32_t timer;

	timer = EFE_POP1(int32_t);
	addEntityParticleFX(EFE_SCRIPT_CONTEXT->sourceEntity, timer);
}

void BTL_copyFromParentTransform(void)
{
	int32_t *src;
	int32_t dst;
	int32_t off;
	int32_t size;

	size = EFE_POP1(int32_t);
	off = EFE_POP1(int32_t);
	dst = EFE_POP1(int32_t);
	src = (int32_t *)(EFE_PREVIOUS_DATA_SEGMENT + off);
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

void BTL_calculatePolarOffset(void)
{
	int32_t ang;
	int32_t r;
	EfeVector *out;

	r = EFE_POP1(int32_t);
	ang = EFE_POP1(int32_t);
	out = EFE_POP1(EfeVector *);
	out->vz = (r * _sin(0x80 - ang)) >> 12;
	out->vx = (r * _cos(0x80 - ang)) >> 12;
}

GARBAGE(BTL_renderProjectedSprite, 69);

void BTL_renderProjectedSprite(void)
{
	ModelComponent *m;
	VECTOR *col;

	m = EFE_DATA_ITERATOR->model;
	EFE_SPRITE_SCRATCH->sprite.tpage = m->pixelPage | 0x20;
	EFE_SPRITE_SCRATCH->sprite.cx = (m->clutPage & 0x3f) << 4;
	EFE_SPRITE_SCRATCH->sprite.cy = m->clutPage >> 6;
	EFE_SPRITE_SCRATCH->sprite.u = m->pixelOffsetX;
	EFE_SPRITE_SCRATCH->sprite.v = m->pixelOffsetY;
	EFE_SPRITE_SCRATCH->sprite.cy += (int16_t)EFE_POP1(int32_t);
	EFE_SPRITE_SCRATCH->sprite.attribute = EFE_POP1(int32_t) << 28;
	EFE_SPRITE_SCRATCH->sprite.rotate = EFE_POP1(int32_t) << 12;
	EFE_SPRITE_SCRATCH->sprite.my = EFE_POP1(int32_t);
	EFE_SPRITE_SCRATCH->sprite.mx = EFE_POP1(int32_t);
	EFE_SPRITE_SCRATCH->sprite.h = EFE_POP1(int32_t);
	EFE_SPRITE_SCRATCH->sprite.w = EFE_POP1(int32_t);
	EFE_SPRITE_SCRATCH->sprite.v += EFE_POP1(int32_t);
	EFE_SPRITE_SCRATCH->sprite.u += EFE_POP1(int32_t);
	EFE_SPRITE_SCRATCH->sprite.scaley = EFE_POP1(int32_t);
	EFE_SPRITE_SCRATCH->sprite.scalex = EFE_POP1(int32_t);
	col = EFE_POP1(VECTOR *);
	EFE_SPRITE_SCRATCH->sprite.r = col->vx;
	EFE_SPRITE_SCRATCH->sprite.g = col->vy;
	EFE_SPRITE_SCRATCH->sprite.b = col->vz;
	EFE_SPRITE_SCRATCH->position = EFE_POP1(VECTOR *);
	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	copyVector(&EFE_SPRITE_SCRATCH->point, EFE_SPRITE_SCRATCH->position);
	EFE_SPRITE_SCRATCH->otz = RotTransPers(&EFE_SPRITE_SCRATCH->point, &EFE_SPRITE_SCRATCH->sxy, &EFE_SPRITE_SCRATCH->p, &EFE_SPRITE_SCRATCH->flag);
	if ((EFE_SPRITE_SCRATCH->flag & 0x80000000) == 0) {
		*(int32_t *)&EFE_SPRITE_SCRATCH->sprite.x = EFE_SPRITE_SCRATCH->sxy;
		EFE_SPRITE_SCRATCH->sprite.scalex = ((uint32_t)(EFE_SPRITE_SCRATCH->sprite.scalex * VIEWPORT_DISTANCE) << 5) / (uint32_t)EFE_SPRITE_SCRATCH->otz;
		EFE_SPRITE_SCRATCH->sprite.scaley = ((uint32_t)(EFE_SPRITE_SCRATCH->sprite.scaley * VIEWPORT_DISTANCE) << 5) / (uint32_t)EFE_SPRITE_SCRATCH->otz;
		EFE_SPRITE_SCRATCH->otz = (EFE_SPRITE_SCRATCH->otz - 0xa) >> 2;
		if (EFE_SPRITE_SCRATCH->otz >= 0x20 && EFE_SPRITE_SCRATCH->otz < 0x1000) {
			GsSortSprite(&EFE_SPRITE_SCRATCH->sprite, ACTIVE_ORDERING_TABLE, (uint16_t)EFE_SPRITE_SCRATCH->otz);
		}
	}
}

void BTL_getTargetDigimonSize(void)
{
	EfeVector *out;
	int16_t radius;
	int16_t height;

	out = EFE_POP1(EfeVector *);
	radius = DIGIMON_DATA[EFE_SCRIPT_CONTEXT->targetEntity->type].radius;
	height = DIGIMON_DATA[EFE_SCRIPT_CONTEXT->targetEntity->type].height;
	out->vx = radius;
	out->vy = height;
	out->vz = radius;
}

void BTL_renderParticleFlashSprite(void)
{
	ParticleFlashData flash;
	ModelComponent *m;
	int32_t n;

	m = EFE_DATA_ITERATOR->model;
	n = EFE_POP1(int32_t);
	flash.depth = EFE_POP1(int32_t);
	flash.scale = EFE_POP1(int16_t);
	flash.sizeY = EFE_POP1(int32_t);
	flash.sizeX = EFE_POP1(int32_t);
	flash.vBase = EFE_POP1(int32_t) + m->pixelOffsetY;
	flash.uBase = EFE_POP1(int32_t) + m->pixelOffsetX;
	flash.screenPos.vy = EFE_POP1(int32_t);
	flash.screenPos.vx = EFE_POP1(int32_t);
	if (flash.depth < 0xa) {
		return;
	}

	if (flash.depth >= 0x1000) {
		return;
	}

	flash.tpage = m->pixelPage | 0x20;
	flash.clut = m->clutPage + (n << 6);
	flash.color.r = flash.color.g = flash.color.b = flash.colorScale = 0x80;
	renderParticleFlash(&flash);
}

void BTL_projectPositionToScreen(void)
{
	DVECTOR screen;
	SVECTOR pos;
	int32_t *depthOut;
	int32_t *xOut;
	int32_t *yOut;
	EfeVector *src;

	depthOut = EFE_POP1(int32_t *);
	yOut = EFE_POP1(int32_t *);
	xOut = EFE_POP1(int32_t *);
	src = EFE_POP1(EfeVector *);
	copyVector(&pos, src);
	*depthOut = worldPosToScreenPos(&pos, &screen);
	*depthOut >>= 4;
	*xOut = screen.vx;
	*yOut = screen.vy;
}

void BTL_renderScreenSprite(void)
{
	GsSPRITE sprite;
	int32_t ox;
	int32_t oy;
	ModelComponent *m;
	int32_t flip;
	int32_t depth;

	m = EFE_DATA_ITERATOR->model;
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
		getDrawingOffsetCopy(&ox, &oy);
		sprite.x += (int16_t)(0xa0 - ox);
		sprite.y += (int16_t)(0x78 - oy);
		sprite.cy = -flip + (m->clutPage >> 6);
	} else {
		sprite.cy = flip + (m->clutPage >> 6);
	}

	sprite.attribute = 0x50000000;
	sprite.tpage = m->pixelPage | 0x20;
	sprite.cx = (m->clutPage & 0x3f) << 4;
	sprite.r = sprite.g = sprite.b = 0x80;

#if defined(VERSION_JP)
	GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, (uint16_t)depth);
#else
	if ((depth >= 0) && (depth < 0x1000)) {
		GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, (uint16_t)depth);
	}
#endif
}

void BTL_addCloudEffect(void)
{
	SVECTOR pos;
	EfeVector *v;

	v = EFE_POP1(EfeVector *);
	pos.vx = v->vx;
	pos.vy = v->vy;
	pos.vz = v->vz;
	createCloudFX(&pos);
}

void BTL_selectNextTargetEntity(void)
{
	int32_t *out;

	out = EFE_POP1(int32_t *);
	if (EFE_TARGET_ENTITY_INDEX >= 10) {
		*out = -1;
		return;
	}

	for (; EFE_TARGET_ENTITY_INDEX < 10; EFE_TARGET_ENTITY_INDEX++) {
		if (ENTITY_TABLE[EFE_TARGET_ENTITY_INDEX] == NULL) {
			continue;
		}
		if (ENTITY_TABLE[EFE_TARGET_ENTITY_INDEX] == EFE_SCRIPT_CONTEXT->sourceEntity) {
			continue;
		}
		if (ENTITY_TABLE[EFE_TARGET_ENTITY_INDEX]->isOnMap == 0) {
			continue;
		}
		if (ENTITY_TABLE[EFE_TARGET_ENTITY_INDEX]->isOnScreen == 0) {
			continue;
		}
		if (((DigimonEntity *)ENTITY_TABLE[EFE_TARGET_ENTITY_INDEX])->stats.current.currentHP > 0) {
			break;
		}
	}

	if (EFE_TARGET_ENTITY_INDEX >= 10) {
		*out = -1;
		return;
	}

	EFE_SCRIPT_CONTEXT->targetEntity = ENTITY_TABLE[EFE_TARGET_ENTITY_INDEX];
	*out = EFE_TARGET_ENTITY_INDEX++;
}

void BTL_addParticleEmitter(void)
{
	int32_t b;
	int32_t n;
	EfeVector *vec;
	int32_t a;
	int32_t i;
	EfeParticleEffect *e;

	a = EFE_POP1(int32_t);
	vec = EFE_POP1(EfeVector *);
	n = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t);
	for (i = 0; i < 4; i++) {
		if (EFE_PARTICLE_EMITTERS[i].transform == NULL) {
			break;
		}
	}

	if (i == 4) {
		return;
	}

	e = &EFE_PARTICLE_EMITTERS[i];
	e->type = b;
	for (i = 0; i < 0x15; i++) {
		e->particles[i].distance = 0;
	}

	e->transform = EFE_CURRENT_DATA_SEGMENT;
	e->frames = a;
	e->color.r = vec->vx;
	e->color.g = vec->vy;
	e->color.b = vec->vz;
	e->startOffset = n * 16;
	e->startVelocity = n / 8 * 16;
	e->acceleration = n / 160 * 16;
}

void BTL_setEFEModelObjectColor(void)
{
#if defined(VERSION_JP)
	char *prim;
	EfeColor *color;
	int32_t *ent;
	int32_t idx;
	int32_t i;
	int32_t count;
	uint8_t t;

	color = EFE_POP1(EfeColor *);
	idx = EFE_POP1(int32_t);
	ent = (int32_t *)EFE_DATA_ITERATOR->model->modelPtr->obj;
	ent = (int32_t *)((int32_t)ent + (idx * 28));
	count = ent[5];
	prim = (char *)ent[4];
	for (i = 0; i < count; i++) {
		t = *(int32_t *)prim >> 24;
		switch (t) {
		case 0x2d:
		case 0x2f:
			prim[0x14] = (int16_t)color->red;
			prim[0x15] = (int16_t)color->green;
			prim[0x16] = (int16_t)color->blue;
			prim += 0x20;
			break;
		}
	}
#else
	int32_t *rec;
	EfeColor *color;
	int32_t idx;
	int32_t i;
	int32_t count;
	int32_t t;

	char (*pr)[0x20];
	char (*pg)[0x20];
	char (*pb)[0x20];
	int32_t *ent;

	color = EFE_POP1(EfeColor *);
	idx = EFE_POP1(int32_t);
	ent = (int32_t *)EFE_DATA_ITERATOR->model->modelPtr->obj;
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
			(*pr)[0] = (int16_t)color->red;
			(*pg)[0] = (int16_t)color->green;
			(*pb)[0] = (int16_t)color->blue;
			rec = (int32_t *)((int32_t)rec + 0x20);
			pr++;
			pg++;
			pb++;
		}
	}
#endif
}

void BTL_copyTargetEntityPosition(void)
{
	EfeVector *out;
	int32_t *m;

	out = EFE_POP1(EfeVector *);
	m = (int32_t *)(((char **)(int32_t)EFE_SCRIPT_CONTEXT->targetEntity)[1] + 0x34);
	out->vx = m[5];
	out->vy = m[6];
	out->vz = m[7];
}

void BTL_steerTransformTowardPoint(void)
{
	EfeVector *target;
	int32_t speed;
	MATRIX m;
	SVECTOR in;
	SVECTOR rot;
	SVECTOR out;
	EfeTransform *pos;
	int32_t turn;
	int32_t d;

	turn = EFE_POP1(int32_t);
	speed = EFE_POP1(int32_t);
	target = EFE_POP1(EfeVector *);
	pos = (EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4);
	rot.vx = 0;
	rot.vy = -*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x14);
	rot.vz = 0;
	in.vx = target->vx - pos->position.vx;
	in.vy = 0;
	in.vz = target->vz - pos->position.vz;
	RotMatrixZYX(&rot, &m);
	ApplyMatrixSV(&m, &in, &out);
	d = _atan(out.vx, out.vz);
	d -= 0x400;
	d = -d;
	d &= 0xfff;
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
	rot.vx = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x10);
	*(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x14) += turn;
	rot.vy = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x14);
	rot.vz = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x18);
	RotMatrixZYX(&rot, &m);
	ApplyMatrixSV(&m, &in, &out);
	pos->position.vx += out.vx >> 3;
	pos->position.vy += out.vy >> 3;
	pos->position.vz += out.vz >> 3;
}

void BTL_interpolateVector(void)
{
	int32_t t0;
	int32_t t1;
	int32_t t;
	EfeVector *a;
	EfeVector *b;
	EfeVector *out;

	out = EFE_POP1(EfeVector *);
	t = EFE_POP1(int32_t);
	t1 = EFE_POP1(int32_t);
	t0 = EFE_POP1(int32_t);
	b = EFE_POP1(EfeVector *);
	a = EFE_POP1(EfeVector *);
	out->vx = lerp(a->vx, b->vx, t0, t1, t);
	out->vy = lerp(a->vy, b->vy, t0, t1, t);
	out->vz = lerp(a->vz, b->vz, t0, t1, t);
}

void BTL_discardEFEOperandPair(void)
{
	int32_t second;
	int32_t first;

	first = EFE_POP1(int32_t);
	second = EFE_POP1(int32_t);
}

void BTL_getScatteredSpawnPosition(void)
{
	int32_t r;
	EfeVector *out;
	int32_t *p;
	int32_t *src;

	r = EFE_POP1(int32_t);
	out = EFE_POP1(EfeVector *);

	if (EFE_PREVIOUS_DATA_SEGMENT == 0) {
	}

	if (EFE_PREVIOUS_DATA_SEGMENT == 0) {
		p = (int32_t *)(((char **)(int32_t)EFE_SCRIPT_CONTEXT->sourceEntity)[1] + (*(int16_t *)(int32_t)EFE_SCRIPT_CONTEXT->boneOffset * 136) + 0x34);
		out->vx = p[5];
		out->vy = p[6];
		out->vz = p[7];
	} else {
		src = (int32_t *)(EFE_PREVIOUS_DATA_SEGMENT + 4);
		out->vx = *src++;
		out->vy = *src++;
		out->vz = *src;
	}

	if (r > 0) {
		out->vx = out->vx + ((rand() % r) - (r >> 1));
		out->vy = out->vy + ((rand() % r) - (r >> 1));
		out->vz = out->vz + ((rand() % r) - (r >> 1));
	}
}

void BTL_checkCollisionWithDefaultPower(void)
{
	EFE_PUSH1(int32_t, -1);
	BTL_applyBoxAttackHit();
}

void BTL_addAttackObjectToTarget(void)
{
	SVECTOR pos;
	int32_t i;
	int32_t j;

	if (((DigimonEntity *)EFE_SCRIPT_CONTEXT->targetEntity)->stats.current.isHit != 0) {
		return;
	}

	pos.vx = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vx;
	pos.vy = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 8);
	pos.vz = *(int32_t *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0xc);
	for (i = 1; i < 10; i++) {
		if ((int32_t)ENTITY_TABLE[i] == (int32_t)EFE_SCRIPT_CONTEXT->targetEntity) {
			break;
		}
	}

	((DigimonEntity *)EFE_SCRIPT_CONTEXT->targetEntity)->stats.current.isHit = 1;
	for (j = 1; j < 10; j++) {
		if (ENTITY_TABLE[j] == EFE_SCRIPT_CONTEXT->sourceEntity) {
			break;
		}
	}

	addAttackObject(i, 1, &pos, COMBAT_EFFECT_ITR, EFE_SUB_EFFECT_INDEX, j);
}

void BTL_setTransformToTargetBone(void)
{
	int32_t r;
	int32_t idx;
	int32_t *p;
	MATRIX *bone;
	int32_t s1;
	int32_t s2;

	r = EFE_POP1(int32_t);
	idx = EFE_POP1(int32_t);
	p = &((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vx;
	bone = &EFE_SCRIPT_CONTEXT->targetEntity->posData[idx].posMatrix.workm;
	*p++ = bone->t[0];
	*p++ = bone->t[1];
	*p++ = bone->t[2];
	*p++ = 0;
	*p++ = EFE_SCRIPT_CONTEXT->targetEntity->posData->rotation.vy;
	*p = 0;
	s1 = _sin(-(p[-1] >> 3));
	s2 = _sin(-(((p[-1] >> 3) + 0x80) & 0x1ff));
	p[-5] = p[-5] + ((r * s1) >> 12);
	p[-3] = p[-3] + ((r * s2) >> 12);
}

void BTL_renderCenteredSprite(void)
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
	BTL_renderProjectedSprite();
}

void BTL_initializeEFETransform(void)
{
#if defined(VERSION_JP)
	int32_t *dst;
	int32_t *src;
#else
	int32_t *src;
	int32_t *dst;
	int32_t *chk;
#endif

	dst = &((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vx;
#if defined(VERSION_JP)
	if (EFE_PREVIOUS_DATA_SEGMENT == NULL) {
		BTL_setTransformToBoneOffset();
		return;
	}

	src = (int32_t *)((int32_t)EFE_PREVIOUS_DATA_SEGMENT + 4);
#else
	chk = (int32_t *)EFE_PREVIOUS_DATA_SEGMENT;
	if (chk == NULL) {
		BTL_setTransformToBoneOffset();
		return;
	}

	src = (int32_t *)((int32_t)chk + 4);
#endif
	*dst++ = *src++;
	*dst++ = *src++;
	*dst++ = *src++;
	*dst++ = *src++;
	*dst++ = *src++;
	*dst = *src;
}

void BTL_drawTMD(void)
{
	EFE_SCRATCH->scale = EFE_POP1(VECTOR *);
	EFE_SCRATCH->id = EFE_POP1(int32_t);
	copyVector(&EFE_SCRATCH->rot, (VECTOR *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x10));
	RotMatrix(&EFE_SCRATCH->rot, &EFE_SCRATCH->m1);
	ScaleMatrix(&EFE_SCRATCH->m1, EFE_SCRATCH->scale);
	EFE_SCRATCH->m1.t[0] = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vx;
	EFE_SCRATCH->m1.t[1] = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vy;
	EFE_SCRATCH->m1.t[2] = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vz;
	GsMulCoord0(&GsWSMATRIX, &EFE_SCRATCH->m1, &EFE_SCRATCH->m0);
	if (EFE_SCRATCH->m0.t[2] < -0x12c) {
		return;
	}

	if (EFE_SCRATCH->m0.t[2] >= 0x10000) {
		return;
	}

	GsSetLightMatrix(&EFE_SCRATCH->m1);
	GsSetLsMatrix(&EFE_SCRATCH->m0);
	GsLinkObject4((unsigned long)EFE_DATA_ITERATOR->model->modelPtr->obj, &EFE_SCRATCH->obj, EFE_SCRATCH->id);
	EFE_SCRATCH->obj.coord2 = NULL;
	EFE_SCRATCH->obj.attribute = 0;
	GsSortObject4(&EFE_SCRATCH->obj, ACTIVE_ORDERING_TABLE, 2, EFE_SORT_WORKSPACE);
}

void BTL_initializeSubEffectInstructions(void)
{
	int32_t b;
	int32_t a;

	a = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t);
	EFE_CURRENT_DATA_SEGMENT->frame = 0;
	EFE_SCRIPT_CONTEXT->inst = (int16_t *)b;
	EFE_SCRIPT_CONTEXT->someInst = (int16_t *)a;
}

void BTL_initializeUVAnim(void)
{
	int32_t val;
	int32_t idx;
	int32_t ptr;
	EfeUvAnim *anim;
	int32_t i;
	uint32_t q;

	ptr = EFE_POP1(int32_t);
	val = EFE_POP1(int32_t);
	idx = EFE_POP1(int32_t);
	if ((idx < 0) || (idx >= EFE_DATA_ITERATOR->numObjects)) {
		EFE_ACTIVE_SECTION = -2;
		EFE_SCRIPT_PTR = NULL;
		return;
	}

	anim = &EFE_DATA_ITERATOR->uvAnims[idx];
	anim->unk0 = val;
	if (ptr == -1) {
		return;
	}

	anim->numKeyframes = *(int32_t *)ptr;
	anim->uvData = (EfeUvKeyframe *)(ptr + 4);
	anim->uv = anim->uvData;
	anim->uvFrame = 0;
	anim->keyframe = 0;
	anim->frame = 1;
	anim->numFrames = 0;
	q = (uint32_t)anim->uvData;
	for (i = 0; i < anim->numKeyframes; i++) {
		anim->numFrames += (int32_t)*(int16_t *)q;
		q += 4;
	}
}

void BTL_checkTechCompatibility(void)
{
	EfeTechBoneOffset *entry;
	int16_t type;

	entry = EFE_POP1(EfeTechBoneOffset *);
	type = (int16_t)getOriginalType(EFE_SCRIPT_CONTEXT->sourceEntity->type);
	while (entry->type != type) {
		if (entry->type < 0) {
			EFE_ACTIVE_SECTION = -1;
			EFE_SCRIPT_PTR = NULL;
			break;
		}
		entry = (EfeTechBoneOffset *)((int32_t)entry + 0xa);
	}

	EFE_BONE_OFFSET = &entry->offset;
	EFE_SCRIPT_CONTEXT->boneOffset = EFE_BONE_OFFSET;
}

void BTL_spawnEFESubEffect(void)
{
	Entity *src;
	int16_t i;
	Entity *tgt;
	int16_t *ip;
	int16_t n;
	int16_t stride;
	EfeInstance *instance;

	ip = EFE_SCRIPT_PTR;
	EFE_TARGET_ENTITY_INDEX = 1;
	n = ip[1];
	instance = (EfeInstance *)(ip[2] + EFE_SCRIPT_HEAD);
	stride = ip[4];
	for (i = 0; i < n; i++) {
		if (instance->frame == -1) {
			break;
		}
		instance = (EfeInstance *)((int32_t)instance + stride);
	}

	if (i == n) {
		BTL_returnFromEFESubroutine();
		return;
	}

	if (EFE_ACTIVE_SECTION == -1) {
		EFE_ACTIVE_SECTION = i;
	}

	src = EFE_SCRIPT_CONTEXT->sourceEntity;
	tgt = EFE_SCRIPT_CONTEXT->targetEntity;
	for (;;) {
		if (EFE_SCRIPT_CONTEXT->inst == NULL) {
			break;
		}
		EFE_SCRIPT_CONTEXT++;
		if ((int32_t)EFE_SCRIPT_CONTEXT >= (int32_t)EFE_DATA_ITERATOR->model->modelPtr) {
			BTL_returnFromEFESubroutine();
			return;
		}
	}

	EFE_PREVIOUS_DATA_SEGMENT = (int32_t)EFE_CURRENT_DATA_SEGMENT;
	EFE_CURRENT_DATA_SEGMENT = instance;
	EFE_SCRIPT_CONTEXT->instance = EFE_CURRENT_DATA_SEGMENT;
	EFE_SCRIPT_CONTEXT->parentInstance = (EfeInstance *)EFE_PREVIOUS_DATA_SEGMENT;
	if (EFE_PREVIOUS_DATA_SEGMENT == 0) {
		EFE_SCRIPT_CONTEXT->sourceEntity = (Entity *)BATTLE_ATTACKING_DIGIMON;
		EFE_SCRIPT_CONTEXT->targetEntity = (Entity *)BATTLE_TARGETED_DIGIMON;
	} else {
		EFE_SCRIPT_CONTEXT->sourceEntity = src;
		EFE_SCRIPT_CONTEXT->targetEntity = tgt;
	}

	MAIN_D_80134CDC = 1;
	EFE_SCRIPT_PTR = (int16_t *)((int32_t)EFE_SCRIPT_PTR + 0xc);
}

void BTL_popEFEValueToVariable(void)
{
	int16_t *pc;

	pc = (int16_t *)(int32_t)EFE_SCRIPT_PTR;
	if (pc[2] == 0) {
		*(int32_t *)(pc[1] + (int32_t)EFE_CURRENT_DATA_SEGMENT) = EFE_POP1(int32_t);
	} else {
		*(int32_t *)(pc[1] + EFE_SCRIPT_HEAD) = EFE_POP1(int32_t);
	}

	EFE_SCRIPT_PTR = (int16_t *)((int32_t)EFE_SCRIPT_PTR + 6);
}

void BTL_returnFromEFESubroutine(void)
{
	EFE_SCRIPT_PTR = EFE_POP2(int16_t *);
	if (EFE_SCRIPT_PTR != NULL) {
		EFE_SCRIPT_CONTEXT = EFE_POP2(EfeSubEffect *);
		EFE_CURRENT_DATA_SEGMENT = EFE_SCRIPT_CONTEXT->instance;
		EFE_PREVIOUS_DATA_SEGMENT = (int32_t)EFE_SCRIPT_CONTEXT->parentInstance;
		EFE_TARGET_ENTITY_INDEX = EFE_POP2(int32_t);
	}
}

void BTL_dispatchEFESubOpcode(void)
{
	EFE_SCRIPT_PTR = (int16_t *)((int32_t)EFE_SCRIPT_PTR + 2);
	BTL_EFE_SUB_OPCODE_HANDLERS[EFE_SCRIPT_CURRENT_VALUE >> 8]();
}

void BTL_callEFESubroutine(void)
{
	int16_t *ip;

	ip = EFE_SCRIPT_PTR;
	EFE_PUSH2(int32_t, EFE_TARGET_ENTITY_INDEX);
	EFE_PUSH2(EfeSubEffect *, EFE_SCRIPT_CONTEXT);
	EFE_PUSH2(int16_t *, EFE_SCRIPT_PTR + 3);
	EFE_SCRIPT_PTR = (int16_t *)(ip[1] + EFE_SCRIPT_HEAD);
}

void BTL_pushEFEVariableAddress(void)
{
	int32_t ip;

	ip = (int32_t)EFE_SCRIPT_PTR;

	*EFE_DATA_STACK = ((int16_t *)ip)[1];
	if (((int16_t *)ip)[2] == 0) {
		*EFE_DATA_STACK++ += (int32_t)EFE_CURRENT_DATA_SEGMENT;
	} else {
		*EFE_DATA_STACK++ += EFE_SCRIPT_HEAD;
	}

	EFE_SCRIPT_PTR = (int16_t *)((int32_t)EFE_SCRIPT_PTR + 6);
}

void BTL_pushEFEVariable(void)
{
	int16_t *pc;
	int32_t p;

	pc = (int16_t *)(int32_t)EFE_SCRIPT_PTR;
	if (pc[2] == 0) {
		p = pc[1] + (int32_t)EFE_CURRENT_DATA_SEGMENT;
	} else {
		p = pc[1] + EFE_SCRIPT_HEAD;
	}

	if ((EFE_SCRIPT_CURRENT_VALUE & 0xf00) == 0x400) {
		EFE_PUSH1(int32_t, *(int32_t *)p);
	} else if ((EFE_SCRIPT_CURRENT_VALUE & 0xf00) == 0x200) {
		EFE_PUSH1(int32_t, *(int16_t *)p);
	} else {
		EFE_PUSH1(int32_t, *(int8_t *)p);
	}

	EFE_SCRIPT_PTR = (int16_t *)((int32_t)EFE_SCRIPT_PTR + 6);
}

void BTL_pushEFEImmediate(void)
{
	int16_t *pc;

	pc = (int16_t *)(int32_t)EFE_SCRIPT_PTR;
	EFE_PUSH1(int32_t, pc[1]);
	EFE_SCRIPT_PTR = (int16_t *)((int32_t)EFE_SCRIPT_PTR + 6);
}

void BTL_jumpEFEScript(void)
{
	int16_t *pc;

	pc = (int16_t *)(int32_t)EFE_SCRIPT_PTR;
	EFE_SCRIPT_PTR = (int16_t *)(pc[1] + EFE_SCRIPT_HEAD);
}

void BTL_stopEFEScript(void)
{
	EFE_SCRIPT_CURRENT_VALUE = EFE_SCRIPT_CURRENT_VALUE & 0xff;
	EFE_SCRIPT_PTR = NULL;
}

void BTL_branchEFEOnComparison(void)
{
	int16_t *ip;
	int32_t res;

	ip = EFE_SCRIPT_PTR;
	if ((EFE_SCRIPT_CURRENT_VALUE & 0xf00) == 0x400) {
		if (ip[2] == 0) {
			res = BTL_EFE_COMPARISONS[EFE_SCRIPT_CURRENT_VALUE >> 12](*(int32_t *)(ip[1] + (int32_t)EFE_CURRENT_DATA_SEGMENT));
		} else {
			res = BTL_EFE_COMPARISONS[EFE_SCRIPT_CURRENT_VALUE >> 12](*(int32_t *)(ip[1] + EFE_SCRIPT_HEAD));
		}
	} else if ((EFE_SCRIPT_CURRENT_VALUE & 0xf00) == 0x200) {
		if (ip[2] == 0) {
			res = BTL_EFE_COMPARISONS[EFE_SCRIPT_CURRENT_VALUE >> 12](*(int16_t *)(ip[1] + (int32_t)EFE_CURRENT_DATA_SEGMENT));
		} else {
			res = BTL_EFE_COMPARISONS[EFE_SCRIPT_CURRENT_VALUE >> 12](*(int16_t *)(ip[1] + EFE_SCRIPT_HEAD));
		}
	} else {
		if (ip[2] == 0) {
			res = BTL_EFE_COMPARISONS[EFE_SCRIPT_CURRENT_VALUE >> 12](*(int8_t *)(ip[1] + (int32_t)EFE_CURRENT_DATA_SEGMENT));
		} else {
			res = BTL_EFE_COMPARISONS[EFE_SCRIPT_CURRENT_VALUE >> 12](*(int8_t *)(ip[1] + EFE_SCRIPT_HEAD));
		}
	}

	if (res == 0) {
		EFE_SCRIPT_PTR = (int16_t *)(ip[3] + EFE_SCRIPT_HEAD);
	} else {
		EFE_SCRIPT_PTR = (int16_t *)((int32_t)EFE_SCRIPT_PTR + 0xa);
	}
}

void BTL_applyEFEVariableOperator(void)
{
	int16_t *pc;

	pc = (int16_t *)(int32_t)EFE_SCRIPT_PTR;
	if (pc[2] == 0) {
		BTL_EFE_VARIABLE_OPERATORS[(EFE_SCRIPT_CURRENT_VALUE >> 8) & 0xf][EFE_SCRIPT_CURRENT_VALUE >> 12](pc[1] + (int32_t)EFE_CURRENT_DATA_SEGMENT);
	} else {
		BTL_EFE_VARIABLE_OPERATORS[(EFE_SCRIPT_CURRENT_VALUE >> 8) & 0xf][EFE_SCRIPT_CURRENT_VALUE >> 12](pc[1] + EFE_SCRIPT_HEAD);
	}

	EFE_SCRIPT_PTR = (int16_t *)((int32_t)EFE_SCRIPT_PTR + 6);
}

void BTL_loadEFEIndexedVariable(void)
{
	int16_t *ip;
	int32_t idx;

	ip = EFE_SCRIPT_PTR;
	if ((EFE_SCRIPT_CURRENT_VALUE & 0xf000) == 0x4000) {
		if (ip[4] == 0) {
			idx = *(int32_t *)(ip[3] + (int32_t)EFE_CURRENT_DATA_SEGMENT);
		} else {
			idx = *(int32_t *)(ip[3] + EFE_SCRIPT_HEAD);
		}
	} else if ((EFE_SCRIPT_CURRENT_VALUE & 0xf000) == 0x2000) {
		if (ip[4] == 0) {
			idx = *(int16_t *)(ip[3] + (int32_t)EFE_CURRENT_DATA_SEGMENT);
		} else {
			idx = *(int16_t *)(ip[3] + EFE_SCRIPT_HEAD);
		}
	} else {
		if (ip[4] == 0) {
			idx = *(int8_t *)(ip[3] + (int32_t)EFE_CURRENT_DATA_SEGMENT);
		} else {
			idx = *(int8_t *)(ip[3] + EFE_SCRIPT_HEAD);
		}
	}

	if ((EFE_SCRIPT_CURRENT_VALUE & 0xf00) == 0x400) {
		(*(int32_t *)&EFE_SCRIPT_REGISTER) = *(int32_t *)(ip[1] + (idx * 4) + EFE_SCRIPT_HEAD);
	} else if ((EFE_SCRIPT_CURRENT_VALUE & 0xf00) == 0x200) {
		EFE_SCRIPT_REGISTER = *(int16_t *)(ip[1] + (idx * 2) + EFE_SCRIPT_HEAD);
	} else {
		EFE_SCRIPT_REGISTER = *(int8_t *)(ip[1] + idx + EFE_SCRIPT_HEAD);
	}

	EFE_SCRIPT_PTR = (int16_t *)((int32_t)EFE_SCRIPT_PTR + 0xa);
}

void BTL_loadEFERandomValue(void)
{
	EFE_SCRIPT_REGISTER = rand();
	EFE_SCRIPT_PTR = (int16_t *)((int32_t)EFE_SCRIPT_PTR + 2);
}

void BTL_loadEFEVariable(void)
{
	int16_t *ip;

	ip = EFE_SCRIPT_PTR;
	if ((EFE_SCRIPT_CURRENT_VALUE & 0xf00) == 0x400) {
		if (ip[2] == 0) {
			(*(int32_t *)&EFE_SCRIPT_REGISTER) = *(int32_t *)(ip[1] + (int32_t)EFE_CURRENT_DATA_SEGMENT);
		} else {
			EFE_SCRIPT_REGISTER = *(int32_t *)(ip[1] + EFE_SCRIPT_HEAD);
		}
	} else if ((EFE_SCRIPT_CURRENT_VALUE & 0xf00) == 0x200) {
		if (ip[2] == 0) {
			EFE_SCRIPT_REGISTER = *(int16_t *)(ip[1] + (int32_t)EFE_CURRENT_DATA_SEGMENT);
		} else {
			EFE_SCRIPT_REGISTER = *(int16_t *)(ip[1] + EFE_SCRIPT_HEAD);
		}
	} else {
		if (ip[2] == 0) {
			EFE_SCRIPT_REGISTER = *(int8_t *)(ip[1] + (int32_t)EFE_CURRENT_DATA_SEGMENT);
		} else {
			EFE_SCRIPT_REGISTER = *(int8_t *)(ip[1] + EFE_SCRIPT_HEAD);
		}
	}

	EFE_SCRIPT_PTR = (int16_t *)((int32_t)EFE_SCRIPT_PTR + 6);
}

void BTL_loadEFEImmediate(void)
{
	int16_t *pc;

	pc = (int16_t *)(int32_t)EFE_SCRIPT_PTR;
	EFE_SCRIPT_REGISTER = pc[1];
	EFE_SCRIPT_PTR = (int16_t *)((int32_t)EFE_SCRIPT_PTR + 6);
}

int32_t BTL_shiftRightInt32Variable(int32_t p)
{
	*(int32_t *)p >>= EFE_SCRIPT_REGISTER;
}

int32_t BTL_shiftLeftInt32Variable(int32_t p)
{
	*(int32_t *)p <<= EFE_SCRIPT_REGISTER;
}

int32_t BTL_moduloInt32Variable(int32_t p)
{
	*(int32_t *)p %= EFE_SCRIPT_REGISTER;
}

int32_t BTL_divideInt32Variable(int32_t p)
{
	*(int32_t *)p /= EFE_SCRIPT_REGISTER;
}

int32_t BTL_multiplyInt32Variable(int32_t p)
{
	*(int32_t *)p *= EFE_SCRIPT_REGISTER;
}

int32_t BTL_subtractInt32Variable(int32_t p)
{
	*(int32_t *)p -= EFE_SCRIPT_REGISTER;
}

int32_t BTL_addInt32Variable(int32_t p)
{
	*(int32_t *)p += EFE_SCRIPT_REGISTER;
}

int32_t BTL_setInt32Variable(int32_t p)
{
	*(int32_t *)p = EFE_SCRIPT_REGISTER;
}

int32_t BTL_shiftRightInt8Variable(int32_t p)
{
	*(int8_t *)p >>= EFE_SCRIPT_REGISTER;
}

int32_t BTL_shiftLeftInt8Variable(int32_t p)
{
	*(int8_t *)p <<= EFE_SCRIPT_REGISTER;
}

int32_t BTL_moduloInt8Variable(int32_t p)
{
	*(int8_t *)p %= EFE_SCRIPT_REGISTER;
}

int32_t BTL_divideInt8Variable(int32_t p)
{
	*(int8_t *)p /= EFE_SCRIPT_REGISTER;
}

int32_t BTL_multiplyInt8Variable(int32_t p)
{
	*(int8_t *)p *= EFE_SCRIPT_REGISTER;
}

int32_t BTL_subtractInt8Variable(int32_t p)
{
	*(int8_t *)p -= EFE_SCRIPT_REGISTER;
}

int32_t BTL_addInt8Variable(int32_t p)
{
	*(int8_t *)p += EFE_SCRIPT_REGISTER;
}

int32_t BTL_setInt8Variable(int32_t p)
{
	*(int8_t *)p = EFE_SCRIPT_REGISTER;
}

int32_t BTL_shiftRightInt16Variable(int32_t p)
{
	*(int16_t *)p >>= EFE_SCRIPT_REGISTER;
}

int32_t BTL_shiftLeftInt16Variable(int32_t p)
{
	*(int16_t *)p <<= EFE_SCRIPT_REGISTER;
}

int32_t BTL_moduloInt16Variable(int32_t p)
{
	*(int16_t *)p %= EFE_SCRIPT_REGISTER;
}

int32_t BTL_divideInt16Variable(int32_t p)
{
	*(int16_t *)p /= EFE_SCRIPT_REGISTER;
}

int32_t BTL_multiplyInt16Variable(int32_t p)
{
	*(int16_t *)p *= EFE_SCRIPT_REGISTER;
}

int32_t BTL_subtractInt16Variable(int32_t p)
{
	*(int16_t *)p -= EFE_SCRIPT_REGISTER;
}

int32_t BTL_addInt16Variable(int32_t p)
{
	*(int16_t *)p += EFE_SCRIPT_REGISTER;
}

int32_t BTL_setInt16Variable(int32_t p)
{
	*(int16_t *)p = EFE_SCRIPT_REGISTER;
}

int32_t BTL_compareGreaterOrEqual(int32_t x)
{
	if (x >= EFE_SCRIPT_REGISTER) {
		return 0;
	}

	return -1;
}

int32_t BTL_compareGreater(int32_t x)
{
	if (x > EFE_SCRIPT_REGISTER) {
		return 0;
	}

	return -1;
}

int32_t BTL_compareLessOrEqual(int32_t x)
{
	if (x <= EFE_SCRIPT_REGISTER) {
		return 0;
	}

	return -1;
}

int32_t BTL_compareLess(int32_t x)
{
	if (x < EFE_SCRIPT_REGISTER) {
		return 0;
	}

	return -1;
}

int32_t BTL_compareNotEqual(int32_t x)
{
	if (x != EFE_SCRIPT_REGISTER) {
		return 0;
	}

	return -1;
}

int32_t BTL_compareEqual(int32_t x)
{
	if (x == EFE_SCRIPT_REGISTER) {
		return 0;
	}

	return -1;
}

int16_t BTL_calculateAttackHitPosition(SVECTOR *out, Entity *self, Entity *other, int32_t y)
{
	SVECTOR diff;
	SVECTOR rot;
	MATRIX m;

	diff.vx = other->posData->location.vx - self->posData->location.vx;
	diff.vz = other->posData->location.vz - self->posData->location.vz;
	rot.vx = 0;
	rot.vy = (_atan(diff.vz, diff.vx) + 0x800) & 0xfff;
	rot.vz = 0;
	RotMatrixZYX(&rot, &m);
	out->vx = 0;
	out->vy = -DIGIMON_DATA[self->type].height;
	out->vz = y;
	ApplyMatrixSV(&m, out, out);
	out->vx += (int16_t)self->posData->location.vx;
	out->vz += (int16_t)self->posData->location.vz;
}

void BTL_renderParallelLines(SVECTOR *a, SVECTOR *b, int16_t n, SVECTOR *from, SVECTOR *to, int32_t *col)
{
	LINE_F2 *prim;
	int32_t i;
	int32_t depth;
	int32_t ox;
	int32_t oy;
	int16_t dx;
	int16_t dy;
	int16_t dz;

	prim = (LINE_F2 *)GsGetWorkBase();
	dx = (to->vx - from->vx) / n;
	dy = (to->vy - from->vy) / n;
	dz = (to->vz - from->vz) / n;
	for (i = 0; i <= n; i++) {
		SetLineF2(prim);
		setRGB0(prim, col[0], col[1], col[2]);
		depth = worldPosToScreenPos(a, (DVECTOR *)&prim->x0);
		if ((depth > 0x200) && (depth < 0x10000)) {
			depth = worldPosToScreenPos(b, (DVECTOR *)&prim->x1);
			getDrawingOffsetCopy(&ox, &oy);
			prim->x0 += (int16_t)(0xa0 - ox);
			prim->x1 += (int16_t)(0xa0 - ox);
			prim->y0 += (int16_t)(0x78 - oy);
			prim->y1 += (int16_t)(0x78 - oy);
			if ((depth > 0x200) && (depth < 0x10000)) {
				AddPrim(ACTIVE_ORDERING_TABLE->org + 0xffd, prim);
				prim++;
			}
		}
		a->vx += dx;
		a->vy += dy;
		a->vz += dz;
		b->vx += dx;
		b->vy += dy;
		b->vz += dz;
	}

	GsSetWorkBase((PACKET *)prim);
}

int32_t BTL_interpolateClamped(int32_t lo, int32_t hi, long t, int32_t start, int32_t end)
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

void BTL_renderRibbonStrip(void)
{
	POLY_GT4 *prim;
	SVECTOR *pts;
	int32_t i;

	EFE_RIBBON_SCRATCH->width = EFE_POP1(int32_t);
	EFE_SCRATCH->scale = EFE_POP1(VECTOR *);
	pts = EFE_POP1(SVECTOR *);

	for (i = 0; i < 8U; i += 2) {
		BTL_RIBBON_UVS[i] += EFE_DATA_ITERATOR->model->pixelOffsetX;
		BTL_RIBBON_UVS[i + 1] += EFE_DATA_ITERATOR->model->pixelOffsetY;
	}

	EFE_RIBBON_SCRATCH->tpage = EFE_DATA_ITERATOR->model->pixelPage | 0x20;
	EFE_RIBBON_SCRATCH->clut = EFE_DATA_ITERATOR->model->clutPage + 0x80;
	EFE_RIBBON_SCRATCH->frame = EFE_CURRENT_DATA_SEGMENT->frame;
	i = (uint32_t)(EFE_RIBBON_SCRATCH->frame / 10) % 3;
	EFE_RIBBON_SCRATCH->color.r = BTL_interpolateClamped(0, 10, EFE_RIBBON_SCRATCH->frame % 10, BTL_RIBBON_COLORS[i].r,
	                                                     BTL_RIBBON_COLORS[(uint32_t)(i + 1) % 3].r);
	EFE_RIBBON_SCRATCH->color.g = BTL_interpolateClamped(0, 10, EFE_RIBBON_SCRATCH->frame % 10, BTL_RIBBON_COLORS[i].g,
	                                                     BTL_RIBBON_COLORS[(uint32_t)(i + 1) % 3].g);
	EFE_RIBBON_SCRATCH->color.b = BTL_interpolateClamped(0, 10, EFE_RIBBON_SCRATCH->frame % 10, BTL_RIBBON_COLORS[i].b,
	                                                     BTL_RIBBON_COLORS[(uint32_t)(i + 1) % 3].b);
	if (EFE_RIBBON_SCRATCH->frame < 0xf) {
		i = BTL_interpolateClamped(1, 7, EFE_RIBBON_SCRATCH->frame, 0, 0x1000);
	} else {
		i = BTL_interpolateClamped(0x17, 0x1e, EFE_RIBBON_SCRATCH->frame, 0x1000, 0);
	}
	EFE_RIBBON_SCRATCH->color.r = EFE_RIBBON_SCRATCH->color.r * i >> 12;
	EFE_RIBBON_SCRATCH->color.g = EFE_RIBBON_SCRATCH->color.g * i >> 12;
	EFE_RIBBON_SCRATCH->color.b = EFE_RIBBON_SCRATCH->color.b * i >> 12;
	EFE_RIBBON_SCRATCH->colorHalf.r = EFE_RIBBON_SCRATCH->color.r >> 1;
	EFE_RIBBON_SCRATCH->colorHalf.g = EFE_RIBBON_SCRATCH->color.g >> 1;
	EFE_RIBBON_SCRATCH->colorHalf.b = EFE_RIBBON_SCRATCH->color.b >> 1;

	copyVector(&EFE_SCRATCH->rot, (VECTOR *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 0x10));
	RotMatrixYXZ(&EFE_SCRATCH->rot, &EFE_SCRATCH->m1);
	ScaleMatrix(&EFE_SCRATCH->m1, EFE_SCRATCH->scale);
	EFE_SCRATCH->m1.t[0] = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vx;
	EFE_SCRATCH->m1.t[1] = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vy;
	EFE_SCRATCH->m1.t[2] = ((EfeTransform *)((int32_t)EFE_CURRENT_DATA_SEGMENT + 4))->position.vz;
	GsMulCoord0(&GsWSMATRIX, &EFE_SCRATCH->m1, &EFE_SCRATCH->m0);
	GsSetLightMatrix(&EFE_SCRATCH->m1);
	GsSetLsMatrix(&EFE_SCRATCH->m0);

	for (i = 0; i < 10; i++) {
		EFE_RIBBON_SCRATCH->edge[i][0].vx = pts[i].vx;
		EFE_RIBBON_SCRATCH->edge[i][0].vy = pts[i].vy - 0x1f4;
		EFE_RIBBON_SCRATCH->edge[i][0].vz = pts[i].vz;
		EFE_RIBBON_SCRATCH->edge[i][1].vx = pts[i].vx;
		EFE_RIBBON_SCRATCH->edge[i][1].vy = pts[i].vy + 0xc8;
		EFE_RIBBON_SCRATCH->edge[i][1].vz = pts[i].vz;
	}

	for (i = 1; i < 10; i++) {
		prim = (POLY_GT4 *)GsGetWorkBase();
		EFE_RIBBON_SCRATCH->proj.otz = RotTransPers4(
			&EFE_RIBBON_SCRATCH->edge[i - 1][0], &EFE_RIBBON_SCRATCH->edge[i][0],
			&EFE_RIBBON_SCRATCH->edge[i - 1][1], &EFE_RIBBON_SCRATCH->edge[i][1],
			(long *)&prim->x0, (long *)&prim->x1, (long *)&prim->x2,
			(long *)&prim->x3, &EFE_RIBBON_SCRATCH->proj.p, &EFE_RIBBON_SCRATCH->proj.flag);
		if ((EFE_RIBBON_SCRATCH->proj.flag & 0x80000000) == 0) {
			prim->tpage = EFE_RIBBON_SCRATCH->tpage;
			prim->clut = EFE_RIBBON_SCRATCH->clut;
			if (i == 1) {
				*(int32_t *)&prim->r0 = 0;
				*(int32_t *)&prim->r2 = 0;
			} else {
				*(int32_t *)&prim->r0 = *(int32_t *)&EFE_RIBBON_SCRATCH->colorHalf;
				*(int32_t *)&prim->r2 = *(int32_t *)&EFE_RIBBON_SCRATCH->color;
			}
			if (i == 9) {
				*(int32_t *)&prim->r1 = 0;
				*(int32_t *)&prim->r3 = 0;
			} else {
				*(int32_t *)&prim->r1 = *(int32_t *)&EFE_RIBBON_SCRATCH->colorHalf;
				*(int32_t *)&prim->r3 = *(int32_t *)&EFE_RIBBON_SCRATCH->color;
			}
			*(int16_t *)&prim->u0 = *(int16_t *)BTL_RIBBON_UVS;
			*(int16_t *)&prim->u1 = *(int16_t *)&BTL_RIBBON_UVS[2];
			*(int16_t *)&prim->u2 = *(int16_t *)&BTL_RIBBON_UVS[4];
			*(int16_t *)&prim->u3 = *(int16_t *)&BTL_RIBBON_UVS[6];
			((uint8_t *)prim)[3] = 0xc;
			prim->code = 0x3c;
			setSemiTrans(prim, 1);
			setShadeTex(prim, 0);
			addPrim(ACTIVE_ORDERING_TABLE->org + (EFE_RIBBON_SCRATCH->proj.otz >> 2), prim);
			GsSetWorkBase((PACKET *)((char *)prim + 0x34));
		}
	}

	for (i = 0; i < 8U; i += 2) {
		BTL_RIBBON_UVS[i] -= EFE_DATA_ITERATOR->model->pixelOffsetX;
		BTL_RIBBON_UVS[i + 1] -= EFE_DATA_ITERATOR->model->pixelOffsetY;
	}
}

void BTL_initializeEFESubOpcodeTable(void)
{
	int32_t i;

	for (i = 0; (uint32_t)i < 0x61; i++) {
		if ((uint32_t)BTL_EFE_SUB_OPCODES[i].opcode >= 0x61) {
			exit(1);
		}
		BTL_EFE_SUB_OPCODE_HANDLERS[BTL_EFE_SUB_OPCODES[i].opcode] = BTL_EFE_SUB_OPCODES[i].handler;
	}
}

void BTL_dispatchEFEOpcode(int32_t op)
{
	BTL_jtbl_80073604[op]();
}

int32_t BTL_runEFEScript(int32_t script)
{
	EFE_SCRIPT_PTR = (int16_t *)script;
	EFE_DATA_STACK = EFE_SCRIPT_MEM1_DATA;
	EFE_CALL_STACK = EFE_CALL_STACK_BUFFER;
	EFE_PUSH2(int32_t, 0);
	while (EFE_SCRIPT_PTR != NULL) {
		EFE_SCRIPT_CURRENT_VALUE = **(int16_t **)&EFE_SCRIPT_PTR;
		BTL_jtbl_80073604[EFE_SCRIPT_CURRENT_VALUE & 0xff]();
	}

	if (EFE_SCRIPT_CONTEXT->instance->frame == -1) {
		EFE_SCRIPT_CONTEXT->inst = NULL;
	}

	return EFE_ACTIVE_SECTION;
}

void BTL_resetPoisonBubbles(void)
{
	setInt16WithStride(&BTL_POISON_BUBBLES[0].frame, -1, 0xc, 0xc);
}

int32_t BTL_addPoisonBubble(Entity *entity)
{
	int32_t i;
	EfePoisonBubble *p;

	for (i = 0; i < 0xc; i++) {
		if (BTL_POISON_BUBBLES[i].frame == -1) {
			break;
		}
	}

	if (i == 0xc) {
		return -1;
	}

	p = &BTL_POISON_BUBBLES[i];
	p->frame = 0;
	p->entity = entity;
	p->offsetX = (rand() % 100) - 0x32;
	p->offsetZ = (rand() % 100) - 0x32;
	addObject(0x805, i, BTL_tickPoisonBubble, BTL_renderPoisonBubble);

	return i;
}

void BTL_tickPoisonBubble(int32_t i)
{
	EfePoisonBubble *p;

	p = &BTL_POISON_BUBBLES[i];
	p->frame++;
	if (p->frame >= 0x28) {
		p->frame = -1;
		removeObject(0x805, i);
	}
}

void BTL_renderPoisonBubble(int32_t i)
{
	SVECTOR pos;
	DVECTOR screen;
	EfePoisonBubble *p;
	int16_t frame;
	int32_t otz;
	int32_t d;
	int16_t angle;

	p = &BTL_POISON_BUBBLES[i];
	frame = p->frame;
	translateConditionFXToEntity(p->entity, &pos);
	pos.vx += p->offsetX;
	pos.vy -= (int16_t)lerp(0x32, 0xc8, 0, 0x28, frame);
	pos.vz += p->offsetZ;
	otz = worldPosToScreenPos(&pos, &screen);
	angle = lerp(0, 0x500, 0, 0x28, frame);
	d = _sin(angle) * 0x14 / 4096;
	screen.vx += (int16_t)(d * (int32_t)VIEWPORT_DISTANCE / otz);
	if ((otz > 0x200) && (otz < 0x10000)) {
		BTL_POISON_BUBBLE_SPRITE.u = BTL_POISON_BUBBLE_FRAME_U[(frame >> 1) % 6] + 0x20;
		renderSprite(&BTL_POISON_BUBBLE_SPRITE, screen.vx, screen.vy, otz, 0x4ea4, 0x4ea4);
	}
}

void BTL_tickPoisonEffect(int32_t i)
{
	EfePoison *p;

	p = &BTL_POISON_EFFECTS[i];
	p->frame++;
	p->frame %= 0x1e;
	if (p->frame == 1) {
		BTL_addPoisonBubble(p->entity);
	}
}

void BTL_renderPoisonEffect(void)
{
}

void BTL_initializePoisonBubble(void)
{
	int32_t i;

	for (i = 0; i < 4; i++) {
		BTL_POISON_EFFECTS[i].frame = -1;
	}

	BTL_resetPoisonBubbles();
}

int32_t BTL_addPoisonEffect(Entity *entity)
{
	int32_t i;
	EfePoison *p;

	for (i = 0; i < 4; i++) {
		if (BTL_POISON_EFFECTS[i].frame == -1) {
			break;
		}
	}

	if (i == 4) {
		return -1;
	}

	p = &BTL_POISON_EFFECTS[i];
	p->frame = 0;
	p->entity = entity;
	addObject(0x808, i, BTL_tickPoisonEffect, (RenderFunction)BTL_renderPoisonEffect);

	return i;
}

void BTL_removePoisonEffect(int32_t i, Entity *entity)
{
	EfePoison *p;

	p = &BTL_POISON_EFFECTS[i];
	if ((i >= 0) && (i < 4) && (p->entity == entity)) {
		p->frame = -1;
		removeObject(0x808, i);
	}
}

void BTL_removeAllPoisonEffects(void)
{
	int32_t i;

	for (i = 0; i < 4; i++) {
		removeObject(0x808, i);
	}

	for (i = 0; i < 0xc; i++) {
		removeObject(0x805, i);
	}
}

void BTL_tickConfusionEffect(int32_t i)
{
	EfeConfusion *p;

	p = &BTL_CONFUSION_EFFECTS[i];
	p->angle += 7;
	p->spin += 0x5b;
}

void BTL_renderConfusionEffect(int32_t idx)
{
	SVECTOR pos;
	GsCOORDINATE2 coord;
	SVECTOR rot;
	VECTOR trans;
	int32_t ang;
	EfeConfusion *p;
	int32_t i;

	p = &BTL_CONFUSION_EFFECTS[idx];
	for (i = 0; i < 3; i++) {
		translateConditionFXToEntity(p->entity, &pos);
		ang = p->angle + (i * 0xaa);
		trans.vx = pos.vx + (_sin(ang) * 0x78 / 4096);
		trans.vy = pos.vy - 0x78;
		trans.vz = pos.vz + (_cos(ang) * 0x78 / 4096);
		rot.vx = 0;
		rot.vy = p->offsets[i] + (p->spin - 0x400 + (i * 0x555));
		rot.vz = 0xe3;
		renderTMDModel((uint8_t *)BTL_CONFUSION_FX_MODEL, 0, &coord, NULL, &trans, &rot, &BTL_CONFUSION_SCALE);
	}
}

void BTL_initializeConfusionEffect(char *base)
{
	BTL_CONFUSION_FX_MODEL = (int32_t)base;
	GsMapModelingData((unsigned long *)((char *)BTL_CONFUSION_FX_MODEL + 4));
	setInt16WithStride(&BTL_CONFUSION_EFFECTS[0].frame, -1, 4, 0x10);
}

int32_t BTL_addConfusionEffect(Entity *entity)
{
	int32_t i;
	EfeConfusion *p;

	for (i = 0; i < 4; i++) {
		if (BTL_CONFUSION_EFFECTS[i].frame == -1) {
			break;
		}
	}

	if (i == 4) {
		return -1;
	}

	p = &BTL_CONFUSION_EFFECTS[i];
	p->frame = 0;
	p->angle = 0;
	p->spin = 0;
	p->entity = entity;
	p->offsets[0] = rand();
	p->offsets[1] = rand();
	p->offsets[2] = rand();
	p->spin = 0;
	addObject(0x806, i, BTL_tickConfusionEffect, BTL_renderConfusionEffect);

	return i;
}

void BTL_removeConfusionEffect(int32_t i, Entity *entity)
{
	EfeConfusion *p;

	p = &BTL_CONFUSION_EFFECTS[i];
	if ((i >= 0) && (i < 4) && (p->entity == entity)) {
		p->frame = -1;
		removeObject(0x806, i);
	}
}

// clang-format off
void BTL_initializeStunEffect(base)
	char *base;
// clang-format on
{
	int32_t i;

	for (i = 0; i < 5; i++) {
		BTL_STUN_EFFECTS[i].frame = -1;
	}

	BTL_resetStunSubEffects();
	BTL_STUN_FX_MODEL = base;
	GsMapModelingData((unsigned long *)(BTL_STUN_FX_MODEL + 4));
}

void BTL_resetStunSubEffects(void)
{
	int32_t i;

	for (i = 0; i < 0x19; i++) {
		BTL_STUN_SUB_EFFECTS[i].frame = -1;
	}
}

void BTL_tickStunEffect(int32_t i)
{
	EfeStun *p;

	p = &BTL_STUN_EFFECTS[i];
	p->frame++;
	switch (p->state) {
	case 0:
		if ((p->frame % 6) == 0) {
			BTL_addStunSubEffect(p->entity);
		}
		if (p->frame >= p->duration) {
			p->state = 1;
			p->frame = 1;
		}
		break;
	case 1:
		if (p->frame >= 8) {
			p->state = 2;
			p->frame = 1;
		}
		break;
	case 2:
		if (p->frame >= 5) {
			BTL_removeStunEffect(i, p->entity);
		}
		break;
	}
}

void BTL_renderStunEffect(int32_t idx)
{
	SVECTOR pos;
	DVECTOR screen;
	int32_t otz;
	EfeStun *p;
	int32_t scale;
	int32_t value;
	int32_t cy;

	p = &BTL_STUN_EFFECTS[idx];
	switch (p->state) {
	case 0:
		value = (p->duration - p->frame) * 100 / 20;
		scale = 0x1000;
		cy = 0;
		break;
	case 1:
		value = 0;
		scale = 0x1000;
		cy = p->frame % 2;
		break;
	case 2:
		value = 0;
		scale = lerp(0x1000, 0xcc, 0, 5, p->frame);
		cy = 0;
		break;
	}

	pos.vx = p->entity->posData->location.vx;
	pos.vy = -DIGIMON_DATA[p->entity->type].height * 113 / 100;
	pos.vz = p->entity->posData->location.vz;
	otz = worldPosToScreenPos(&pos, &screen);

	BTL_STUN_DIGIT_SPRITE.x = screen.vx;
	BTL_STUN_DIGIT_SPRITE.y = screen.vy;
	BTL_STUN_DIGIT_SPRITE.scalex = scale;
	BTL_STUN_DIGIT_SPRITE.scaley = scale;
	BTL_STUN_DIGIT_SPRITE.mx = 0xe;
	BTL_STUN_DIGIT_SPRITE.u = (value / 1000 % 10) * 8;
	GsSortSprite(&BTL_STUN_DIGIT_SPRITE, ACTIVE_ORDERING_TABLE, 0x22);
	BTL_STUN_DIGIT_SPRITE.mx = 7;
	BTL_STUN_DIGIT_SPRITE.u = (value / 100 % 10) * 8;
	GsSortSprite(&BTL_STUN_DIGIT_SPRITE, ACTIVE_ORDERING_TABLE, 0x22);

	BTL_STUN_SMALL_DIGIT_SPRITE.x = screen.vx;
	BTL_STUN_SMALL_DIGIT_SPRITE.y = screen.vy;
	BTL_STUN_SMALL_DIGIT_SPRITE.scalex = scale;
	BTL_STUN_SMALL_DIGIT_SPRITE.scaley = scale;
	BTL_STUN_SMALL_DIGIT_SPRITE.mx = -2;
	BTL_STUN_SMALL_DIGIT_SPRITE.u = (value / 10 % 10) * 4;
	GsSortSprite(&BTL_STUN_SMALL_DIGIT_SPRITE, ACTIVE_ORDERING_TABLE, 0x22);
	BTL_STUN_SMALL_DIGIT_SPRITE.mx = -8;
	BTL_STUN_SMALL_DIGIT_SPRITE.u = (value % 10) * 4;
	GsSortSprite(&BTL_STUN_SMALL_DIGIT_SPRITE, ACTIVE_ORDERING_TABLE, 0x22);

	BTL_D_8007376C.x = screen.vx;
	BTL_D_8007376C.y = screen.vy;
	BTL_D_8007376C.cy = cy + 0x1e0;
	BTL_D_8007376C.scalex = scale;
	BTL_D_8007376C.scaley = scale;
	GsSortSprite(&BTL_D_8007376C, ACTIVE_ORDERING_TABLE, 0x22);

	BTL_D_80073790.x = screen.vx;
	BTL_D_80073790.y = screen.vy;
	BTL_D_80073790.cy = cy + 0x1e0;
	BTL_D_80073790.scalex = scale;
	BTL_D_80073790.scaley = scale;
	GsSortSprite(&BTL_D_80073790, ACTIVE_ORDERING_TABLE, 0x22);
}

void BTL_removeAllStunSubEffects(void)
{
	int32_t i;

	for (i = 0; i < 0x19; i++) {
		BTL_STUN_SUB_EFFECTS[i].frame = -1;
		removeObject(0x810, i);
	}
}

int32_t BTL_addStunSubEffect(Entity *entity)
{
	int32_t i;
	EfeStunSpark *p;

	for (i = 0; i < 0x19; i++) {
		if (BTL_STUN_SUB_EFFECTS[i].frame == -1) {
			break;
		}
	}

	if (i == 0x19) {
		return -1;
	}

	p = &BTL_STUN_SUB_EFFECTS[i];
	p->frame = 0;
	p->entity = entity;
	addObject(0x810, i, BTL_tickStunSubEffect, BTL_renderStunSubEffect);

	return i;
}

void BTL_tickStunSubEffect(int32_t i)
{
	EfeStunSpark *p;

	p = &BTL_STUN_SUB_EFFECTS[i];
	p->frame++;
	if (p->frame >= 0x10) {
		p->frame = -1;
		removeObject(0x810, i);
	}
}

void BTL_renderStunSubEffect(int32_t i)
{
	SVECTOR pos;
	GsCOORDINATE2 coord;
	VECTOR trans;
	SVECTOR rot;
	VECTOR scale;
	EfeStunSpark *p;
	int32_t s;

	p = &BTL_STUN_SUB_EFFECTS[i];
	pos.vx = p->entity->posData->location.vx;
	pos.vy = lerp(-DIGIMON_DATA[p->entity->type].height * 113 / 100, p->entity->posData->location.vy, 0, 0xf, p->frame);
	pos.vz = p->entity->posData->location.vz;
	s = DIGIMON_DATA[p->entity->type].radius * 0x4000 / 350;
	if (p->frame < 4) {
		s = lerp(s * 10 / 100, s, 0, 4, p->frame);
	}

	rot = BTL_STUN_FX_ROTATION;
	scale = BTL_STUN_SUB_EFFECT_SCALE;
	copyVector(&trans, &pos);
	scale.vx = scale.vz = s;
	renderTMDModel((uint8_t *)BTL_STUN_FX_MODEL, 0, &coord, NULL, &trans, &rot, &scale);
}

// clang-format off
int32_t BTL_addStunEffect(entity, val)
	Entity *entity;
	int16_t val;
// clang-format on
{
	int32_t i;
	EfeStun *p;

	for (i = 0; i < 5; i++) {
		if (BTL_STUN_EFFECTS[i].frame == -1) {
			break;
		}
	}

	if (i == 5) {
		return -1;
	}

	p = &BTL_STUN_EFFECTS[i];
	p->frame = 0;
	p->duration = val;
	p->state = 0;
	p->entity = entity;
	addObject(0x80f, i, BTL_tickStunEffect, BTL_renderStunEffect);

	return i;
}

void BTL_removeStunEffect(int32_t i, Entity *entity)
{
	EfeStun *p;

	p = &BTL_STUN_EFFECTS[i];
	if ((i >= 0) && (i < 5) && (p->entity == entity)) {
		p->frame = -1;
		removeObject(0x80f, i);
	}
}

void BTL_removeAllStunEffects(void)
{
	int32_t i;

	BTL_removeAllStunSubEffects();
	for (i = 0; i < 5; i++) {
		BTL_STUN_EFFECTS[i].frame = -1;
		removeObject(0x80f, i);
	}
}

void BTL_setTMDObjectColor(int32_t idx, int32_t *color, int32_t base)
{
#if defined(VERSION_JP)
	int32_t i;
	int32_t count;
	uint8_t t;
	struct TMD_STRUCT *obj;
	char *prim;

	obj = (struct TMD_STRUCT *)((uint32_t)base + 0xc);
	obj = (struct TMD_STRUCT *)((int32_t)obj + (idx * 28));
	count = obj->primn;
	prim = (char *)obj->primtop;
	for (i = 0; i < count; i++) {
		t = *(int32_t *)prim >> 24;
		switch (t) {
		case 0x2d:
		case 0x2f:
			prim[0x14] = (int16_t)color[0];
			prim[0x15] = (int16_t)color[1];
			prim[0x16] = (int16_t)color[2];
			prim = (char *)((int32_t)prim + 0x20);
			break;
		}
	}
#else
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
#endif
}

void BTL_tickFinisherAura(int32_t i)
{
	EfeFinisherAura *p;
	Entity *entity;

	p = &BTL_FINISHER_AURAS[i];
	entity = p->entity;
	p->frame++;
	if (p->frame > p->duration) {
		removeObject(0x80d, i);
		p->frame = -1;
	}
}

void BTL_renderFinisherAura(int32_t id)
{
	int32_t sx;
	int32_t a;
	int32_t t;
	GsCOORDINATE2 coord;
	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;
	EfeFinisherAura *fa;
	Entity *e;
	int32_t sy;

	fa = &BTL_FINISHER_AURAS[id];
	e = fa->entity;

	if (fa->frame < 10) {
		a = lerp(0, 0x80, 0, 9, fa->frame);
		sx = (0x1000 - _cos(a)) * 0xb34 / 0x1000 + 0x4cc;
	} else {
		sx = 0x1000;
	}
	sx = sx * DIGIMON_DATA[e->type].radius / 150;

	t = lerp(0, 0x200, 0, 0x10, fa->frame);
	if (fa->frame < 5) {
		sy = fa->frame * 0x1000 / 4;
	} else {
		sy = 0x1000;
	}
	sy = sy * DIGIMON_DATA[e->type].height / 350;
	sy = sy * 85 / 100 + sy * 15 / 100 * _sin(t) / 0x1000;

	rot = BTL_FINISHER_AURA_ROTATION;
	scale = BTL_FINISHER_AURA_SCALE;
	copyVector(&trans, &e->posData->location);
	scale.vx = scale.vz = sx;
	scale.vy = sy;
	renderTMDModel(*(uint8_t **)&BTL_FINISHER_AURA_MODEL, BTL_FINISHER_AURA_OBJECTS[fa->frame / 2 % 3], &coord, NULL, &trans, &rot, &scale);
	renderTMDModel(*(uint8_t **)&BTL_FINISHER_AURA_MODEL, 3, &coord, NULL, &trans, &rot, &scale);
	rot.vy = rand();
	scale.vx = scale.vz = sx * 140 / 100;
	renderTMDModel(*(uint8_t **)&BTL_FINISHER_AURA_MODEL, 4, &coord, NULL, &trans, &rot, &scale);

	if (fa->frame < 7) {
		RGB8 col;
		int32_t i;
		int32_t s;

		col = BTL_FINISHER_AURA_COLOR;
		s = fa->frame * 0x1000 / 7;
		s = s * DIGIMON_DATA[e->type].radius / 150;
		col.r = (uint32_t)col.r * (7 - fa->frame) / 7;
		col.g = (uint32_t)col.g * (7 - fa->frame) / 7;
		col.b = (uint32_t)col.b * (7 - fa->frame) / 7;
		for (i = 0; i < 20; i += 2) {
			BTL_renderFinisherAuraSpark(&e->posData->location, s, &BTL_FINISHER_AURA_SPARKS[i], (uint8_t *)&col);
		}
	}
}

void BTL_renderFinisherAuraSpark(VECTOR *pos, int32_t scale, SVECTOR *dir, uint8_t *col)
{
	POLY_FT3 *prim;
	SVECTOR c;
	SVECTOR a;
	SVECTOR b;

	prim = (POLY_FT3 *)GsGetWorkBase();
	SetPolyFT3(prim);
	SetSemiTrans(prim, 1);
	setRGB0(prim, col[0], col[1], col[2]);
	prim->tpage = getTPage(0, 1, 768, 256);
	prim->clut = getClut(192, 489);
	setUV3(prim, 0x30, 0xa8, 0x37, 0xa8, 0x30, 0xaf);
	a.vx = dir[0].vx * scale / 4096;
	a.vy = dir[0].vy * scale / 4096;
	a.vz = dir[0].vz * scale / 4096;
	b.vx = dir[1].vx * scale / 4096;
	b.vy = dir[1].vy * scale / 4096;
	b.vz = dir[1].vz * scale / 4096;
	copyVector(&c, pos);
	addVector(&a, pos);
	addVector(&b, pos);
	prim->code |= 2;
	addScreenPolyFT3(prim, &c, &a, &b);
}

void BTL_initializeFinisherAuraModel(char *tim, char *base)
{
	MATRIX m;
	SVECTOR v;
	SVECTOR rot;
	unsigned long *timp;
	unsigned long *timq;
	GsIMAGE image;
	RECT rect;
	VECTOR ca;
	VECTOR cb;
	int32_t j;
	int32_t i;
	SVECTOR *p;

	timq = timp = (unsigned long *)tim;
	GsGetTimInfo(timp + 1, &image);
	setRECT(&rect, image.px, image.py, image.pw, image.ph);
	LoadImage(&rect, image.pixel);
	GetTPage(image.pmode & 3, 0, image.px, image.py);
	if (((image.pmode >> 3) & 1) != 0) {
		setRECT(&rect, image.cx, image.cy, image.cw, image.ch);
		LoadImage(&rect, image.clut);
		GetClut(image.cx, image.cy);
	}

	BTL_FINISHER_AURA_MODEL = (int32_t)base;
	GsMapModelingData((unsigned long *)((char *)BTL_FINISHER_AURA_MODEL + 4));
	ca = BTL_D_8007380C;
	cb = BTL_D_8007381C;
	BTL_setTMDObjectColor(0, (int32_t *)&ca, BTL_FINISHER_AURA_MODEL);
	BTL_setTMDObjectColor(1, (int32_t *)&ca, BTL_FINISHER_AURA_MODEL);
	BTL_setTMDObjectColor(2, (int32_t *)&ca, BTL_FINISHER_AURA_MODEL);
	BTL_setTMDObjectColor(3, (int32_t *)&cb, BTL_FINISHER_AURA_MODEL);
	BTL_setTMDObjectColor(4, (int32_t *)&ca, BTL_FINISHER_AURA_MODEL);

	for (j = 0; j < 2; j++) {
		BTL_FINISHER_AURAS[j].frame = -1;
	}

	p = BTL_FINISHER_AURA_SPARKS;
	for (i = 0; i < 0xa; i++) {
		v.vx = (rand() % 30) + 30;
		v.vy = 0;
		v.vz = (rand() % 0x190) + 0x320;
		rot.vx = rand() % 0x1000 * 180 / 360;
		rot.vy = rand();
		rot.vz = 0;
		RotMatrixZYX(&rot, &m);
		ApplyMatrixSV(&m, &v, p++);
		v.vx *= -1;
		ApplyMatrixSV(&m, &v, p++);
	}
}

int32_t BTL_addFinisherAura(Entity *entity, int32_t duration)
{
	int32_t i;
	EfeFinisherAura *p;

	for (i = 0; i < 2; i++) {
		if (BTL_FINISHER_AURAS[i].frame < 0) {
			break;
		}
	}

	if (i == 2) {
		return -1;
	}

	p = &BTL_FINISHER_AURAS[i];
	p->frame = 0;
	p->duration = duration;
	p->entity = entity;
	addObject(0x80d, i, BTL_tickFinisherAura, BTL_renderFinisherAura);

	return i;
}

void BTL_removeFinisherAura(int32_t i)
{
	removeObject(0x80d, i);
	BTL_FINISHER_AURAS[i].frame = -1;
}

void BTL_removeAllFinisherAuras(void)
{
	int32_t i;

	for (i = 0; i < 2; i++) {
		BTL_removeFinisherAura(i);
	}
}

void BTL_tickAuraProjectile(int32_t id)
{
	AABB box;
	EfeAura *a;
	int32_t hit;
	int32_t j;

	a = &BTL_FLAT_BULLET_PTR[id];
	a->frame++;
	if (a->frame >= 0x3a) {
		a->frame = -1;
		removeObject(0x179, (int16_t)id);
		return;
	}
	addVector(&a->position, &a->velocity);
	box.center = &a->position;
	box.extent.vx = 0x2d;
	box.extent.vy = 0xc8;
	box.extent.vz = 0x2d;
	if ((hit = findAABBHitEntity(&box, a->owner, 1)) == -1) {
		return;
	}
	if (((DigimonEntity *)ENTITY_TABLE[hit])->stats.current.isHit != 0) {
		return;
	}
	for (j = 1; j < 10; j++) {
		if (ENTITY_TABLE[j] == a->owner) {
			break;
		}
	}
	((DigimonEntity *)ENTITY_TABLE[hit])->stats.current.isHit = 1;
	addAttackObject(hit, 1, &a->position, 0x179, 0, j);
	a->frame = -1;
	removeObject(0x179, (int16_t)id);
}

void BTL_renderAuraProjectile(int32_t i)
{
	MATRIX m;
#if defined(VERSION_JP)
	VECTOR scale;
#endif
	SVECTOR a;
	SVECTOR b;
	SVECTOR c;
	SVECTOR d;
	EfeAura *aura;
	POLY_FT4 *prim;

	aura = &BTL_FLAT_BULLET_PTR[i];
	prim = (POLY_FT4 *)GsGetWorkBase();
#if defined(VERSION_JP)
	scale = BTL_AURA_PROJECTILE_SCALE;
#endif
	RotMatrix(&aura->rotation, &m);
	ApplyMatrixSV(&m, &BTL_AURA_PROJECTILE_VERTEX_0, &a);
	ApplyMatrixSV(&m, &BTL_AURA_PROJECTILE_VERTEX_1, &b);
	ApplyMatrixSV(&m, &BTL_AURA_PROJECTILE_VERTEX_2, &c);
	ApplyMatrixSV(&m, &BTL_AURA_PROJECTILE_VERTEX_3, &d);
	addVector(&a, &aura->position);
	addVector(&b, &aura->position);
	addVector(&c, &aura->position);
	addVector(&d, &aura->position);
	SetPolyFT4(prim);
	prim->code |= 2;
	prim->r0 = prim->g0 = prim->b0 = 0x80;
	prim->tpage = getTPage(0, 1, 768, 256);
	prim->clut = getClut(192, 492);
	setUVWH(prim, aura->type->u + 0x60, 0xa0, 7, 7);
	addScreenPolyFT4(prim, &a, &b, &c, &d);
}

char *BTL_initializeAuraProjectiles(char *base)
{
	int32_t i;

	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	BTL_FLAT_BULLET_PTR = (EfeAura *)base;
	base = (char *)((int32_t)base + 0x120);
	for (i = 0; i < 8; i++) {
		BTL_FLAT_BULLET_PTR[i].frame = -1;
	}

	return base;
}

int32_t BTL_addAuraProjectile(Entity *e)
{
	EfeAura *aura;
	int32_t i;
	EfeAuraType *type;
	MATRIX *q;

	aura = BTL_FLAT_BULLET_PTR;
	type = BTL_AURA_PROJECTILE_TYPES;
	type = &BTL_AURA_PROJECTILE_TYPES[getOriginalType(e->type)];
	for (i = 0; i < 8; i++) {
		if (aura[i].frame < 0) {
			break;
		}
	}

	if (i == 8) {
		return -1;
	}

	aura = aura + i;
	aura->frame = 0;
	aura->owner = e;
	aura->type = type;
	q = &e->posData->posMatrix.workm;
	aura->position.vx = 0;
	aura->position.vy = -DIGIMON_DATA[e->type].height * 50 / 100;
	aura->position.vz = -DIGIMON_DATA[e->type].height * 50 / 100;
	ApplyMatrixSV(q, &aura->position, &aura->position);
	aura->position.vx += (int16_t)q->t[0];
	aura->position.vy += (int16_t)q->t[1];
	aura->position.vz += (int16_t)q->t[2];
	aura->rotation.vx = 0;
	aura->rotation.vy = e->posData->rotation.vy;
	aura->rotation.vz = 0;
	aura->velocity.vx = 0;
	aura->velocity.vy = 0;
	aura->velocity.vz = -0x23;
	ApplyMatrixSV(q, &aura->velocity, &aura->velocity);
	addObject(0x179, (int16_t)i, BTL_tickAuraProjectile, BTL_renderAuraProjectile);

	return i;
}

void BTL_removeAllAuraProjectiles(void)
{
	int32_t i;

	for (i = 0; i < 8; i++) {
		if (BTL_FLAT_BULLET_PTR[i].frame >= 0) {
			BTL_FLAT_BULLET_PTR[i].frame = -1;
			removeObject(0x179, (int16_t)i);
		}
	}
}

void BTL_initializeItemParticleVelocities(void)
{
	int32_t r;
	MATRIX m;
	SVECTOR in;
	SVECTOR rot;
	SVECTOR out;
	BtlParticleVelocity *dst;
	BtlParticleDrag *dst2;
	int32_t i;

	dst = BTL_ITEM_PARTICLE_VELOCITIES;
	dst2 = BTL_ITEM_PARTICLE_DRAG;
	for (i = 0; i < 0x14; i++) {
		in.vx = in.vz = 0;
		in.vy = -0x230;
		rot.vx = (rand() % 1000) - 0x1f4;
		rot.vy = rand() % 0x1000;
		rot.vz = 0;
		RotMatrixZYX(&rot, &m);
		ApplyMatrixSV(&m, &in, &out);
		copyVector(dst, &out);
		dst++;
		if (rot.vx < 0) {
			rot.vx = -rot.vx;
		}
		r = lerp(0x3c, 0x1e, 0, 0x226, rot.vx);
		dst2->x = out.vx / r;
		dst2->z = out.vz / r;
		dst2++;
	}
}

void BTL_tickItemParticles(int32_t idx)
{
	BtlItemParticleEffect *b;
	int32_t i;
	BtlItemParticle *fx;

	b = &BTL_ITEM_PARTICLE_EFFECTS[idx];
	fx = b->particles;
	if (--b->timer < 0) {
		removeObject(0x818, idx);
		return;
	}

	for (i = 0; i < 0x14; i++) {
		fx->life--;
		fx->positionX += fx->velocityX -= BTL_ITEM_PARTICLE_DRAG[i].x;
		fx->positionY += fx->velocityY += 0x36;
		fx->positionZ += fx->velocityZ -= BTL_ITEM_PARTICLE_DRAG[i].z;
		fx->brightness += fx->fadeStep;
		fx++;
	}
}

void BTL_renderItemParticles(int32_t idx)
{
	SVECTOR pos;
	RGB8 rgb;
	BtlItemParticle *fx;
	int32_t i;

	fx = BTL_ITEM_PARTICLE_EFFECTS[idx].particles;
	rgb.r = rgb.g = rgb.b = fx->brightness >> 4;
	for (i = 0; i < 0x14; i++) {
		if (fx->life >= 0) {
			pos.vx = fx->positionX >> 4;
			pos.vy = fx->positionY >> 4;
			pos.vz = fx->positionZ >> 4;
			renderFXParticle(&pos, 0x14, &rgb);
		}
		fx++;
	}
}

void BTL_initializeBattleItemParticles(void)
{
	setInt16WithStride(&BTL_ITEM_PARTICLE_EFFECTS[0].timer, -1, 4, 0x234);
	BTL_initializeItemParticleVelocities();
}

int32_t BTL_addItemParticles(Entity *e)
{
	int32_t i;
	int32_t j;
	MATRIX *workm;
	BtlItemParticleEffect *eff;
	BtlItemParticle *fx;
	BtlParticleVelocity *src;

	src = BTL_ITEM_PARTICLE_VELOCITIES;
	for (i = 0; i < 4; i++) {
		if (BTL_ITEM_PARTICLE_EFFECTS[i].timer < 0) {
			break;
		}
	}

	if (i == 4) {
		return -1;
	}

	eff = &BTL_ITEM_PARTICLE_EFFECTS[i];
	eff->timer = 0x28;
	fx = eff->particles;
	for (j = 0; j < 0x14; j++) {
		fx->life = 0x19;
		workm = &e->posData->posMatrix.workm;
		fx->positionX = workm->t[0] << 4;
		fx->positionY = (workm->t[1] - DIGIMON_DATA[e->type].height) << 4;
		fx->positionZ = workm->t[2] << 4;
		fx->velocityX = src->vx;
		fx->velocityY = src->vy;
		fx->velocityZ = src->vz;
		fx->brightness = 0xff0;
		fx->fadeStep = -0xff0 / fx->life;
		fx++;
		src++;
	}

	addObject(0x818, i, BTL_tickItemParticles, BTL_renderItemParticles);

	return i;
}

void BTL_removeItemParticles(int32_t i)
{
	/*
	 * BUG: the bounds check is unconditionally true; it should be
	 * ((i >= 0) && (i < 4)). The bug stays harmless because its only
	 * caller, BTL_removeAllItemParticles, keeps i within 0..3.
	 */
	if ((i >= 0) || (i < 4)) {
		BTL_ITEM_PARTICLE_EFFECTS[i].timer = -1;
		removeObject(0x818, i);
	}
}

void BTL_removeAllItemParticles(void)
{
	int32_t i;

	for (i = 0; i < 4; i++) {
		BTL_removeItemParticles(i);
	}
}

void BTL_initializeBuffTrails(void)
{
	int32_t i;
	long j;
	int32_t r;
	int32_t k;
	int16_t pool[8];
	int32_t s;
	SVECTOR rot;
	SVECTOR a;
	SVECTOR b;
	MATRIX m;

	/*
	 * BUG: BTL_BUFF_TRAILS has a single element, but this loop writes
	 * eight, running off the end. The i == 1 and i == 2 writes are
	 * reinitialized immediately afterwards, and the i >= 3 writes land
	 * past the overlay image where nothing is allocated, so there is no
	 * observable effect.
	 */
	for (i = 0; i < 8; i++) {
		BTL_BUFF_TRAILS[i].frame = -1;
	}

	for (i = 0; i < 8; i++) {
		pool[i] = i;
	}

	for (i = 0; i < 8; i++) {
		r = rand() % (8 - i);
		k = pool[r];
		for (j = r; j < (8 - i); j++) {
			pool[j] = pool[j + 1];
		}
		s = ((rand() % 0x14) + 0x50) * 200 / 100;
		rot.vx = 0;
		rot.vy = (k * 0x1000 / 8) + (rand() % 0x200);
		rot.vz = 0;
		a.vx = 0x1e;
		a.vy = 0;
		a.vz = s;
		b.vx = -0x1e;
		b.vy = 0;
		b.vz = s;
		RotMatrixZYX(&rot, &m);
		ApplyMatrixSV(&m, &a, &a);
		ApplyMatrixSV(&m, &b, &b);
		BTL_BUFF_TRAIL_A_X[i] = a.vx;
		BTL_BUFF_TRAIL_A_Z[i] = a.vz;
		BTL_BUFF_TRAIL_B_X[i] = b.vx;
		BTL_BUFF_TRAIL_B_Z[i] = b.vz;
	}
}

void BTL_tickBuffDisk(int32_t i)
{
	EfeBuffDisk *d;

	d = &BTL_BUFF_DISKS[i];
	if (d->frame >= 0x43) {
		BTL_removeBuffDiskEffect(i);
		return;
	}

	if (d->frame < 0x43) {
		if (d->frame < 0xf) {
			d->scaleXZ = lerp(d->scaleTargetXZ / 3, d->scaleTargetXZ, 0, 0xf, d->frame);
			d->scaleY = lerp(d->scaleTargetY / 3, d->scaleTargetY, 0, 0xf, d->frame);
		} else if (d->frame < 0x37) {
			d->scaleXZ = d->scaleTargetXZ;
			d->scaleY = d->scaleTargetY;
		} else {
			d->scaleXZ = lerp(d->scaleTargetXZ, d->scaleTargetXZ * 10 / 100, 0x37, 0x43, d->frame);
			d->scaleY = lerp(d->scaleTargetY, d->scaleTargetY * 10 / 100, 0x37, 0x43, d->frame);
		}
		d->rotation.vy += 0x32;
	}

	d->frame += 1;
}

void BTL_renderBuffDisk(int32_t i)
{
	VECTOR scale;
	GsCOORDINATE2 coord;
	EfeBuffDisk *d;

	d = &BTL_BUFF_DISKS[i];
	scale.vx = scale.vz = d->scaleXZ;
	scale.vy = d->scaleY;
	renderTMDModel(BUFF_MODEL[0], 5, &coord, NULL, (VECTOR *)&d->bone->t[0], &d->rotation, &scale);
}

void BTL_addBuffTrails(int32_t i, Entity *e)
{
	EfeTrailEffect *t;
	EfeTrail *q;
	int32_t j;

	t = &BTL_BUFF_TRAILS[i];
	q = t->trails;
	t->frame = 0;
	t->matrix = &e->posData->posMatrix.workm;
	t->entity = e;
	for (j = 0; j < 8; j++) {
		q->life = -1;
		q++;
	}

	addObject(0x816, i, BTL_tickBuffTrails, BTL_renderBuffTrails);
}

void BTL_removeBuffTrails(int32_t instanceId)
{
	removeObject(0x816, instanceId);
}

void BTL_tickBuffTrails(int32_t i)
{
	EfeTrailEffect *t;
	EfeTrail *r;
	int32_t k;
	int32_t j;
	int32_t radius;

	t = &BTL_BUFF_TRAILS[i];
	r = t->trails;

	if ((t->frame >= 0) && (t->frame < 0x3b)) {
		k = t->frame % 8;
		r += k;
		r->life = 8;
		r->p[0].vy = r->p[1].vy = r->p[2].vy = r->p[3].vy = 0;
		radius = DIGIMON_DATA[t->entity->type].radius;
		r->p[0].vx = r->p[2].vx = t->matrix->t[0] + (radius * BTL_BUFF_TRAIL_A_X[k] / 200);
		r->p[0].vy = r->p[2].vy = t->matrix->t[1];
		r->p[0].vz = r->p[2].vz = t->matrix->t[2] + (radius * BTL_BUFF_TRAIL_A_Z[k] / 200);
		r->p[1].vx = r->p[3].vx = t->matrix->t[0] + (radius * BTL_BUFF_TRAIL_B_X[k] / 200);
		r->p[1].vy = r->p[3].vy = t->matrix->t[1];
		r->p[1].vz = r->p[3].vz = t->matrix->t[2] + (radius * BTL_BUFF_TRAIL_B_Z[k] / 200);
	}

	r = t->trails;
	for (j = 0; j < 8; j++) {
		r->life--;
		r->p[0].vy -= 100;
		r->p[1].vy -= 100;
		r->p[2].vy -= 0x25;
		r->p[3].vy -= 0x25;
		r++;
	}

	t->frame += 1;
}

void BTL_renderBuffTrails(int32_t i)
{
	POLY_FT4 *prim;
	EfeTrail *r;
	int32_t j;

	r = BTL_BUFF_TRAILS[i].trails;
	for (j = 0; j < 8; j++) {
		if (r->life > 0) {
			prim = (POLY_FT4 *)GsGetWorkBase();
			SetPolyFT4(prim);
			SetSemiTrans(prim, 1);
			prim->tpage = getTPage(0, 1, 768, 256);
			prim->clut = getClut(192, 489);
			setUV4(prim, 0x5f, 0xa0, 0x5f, 0xa7, 0x30, 0xa0, 0x30, 0xa7);
			setRGB0(prim, r->life * 200 / 8, r->life * 255 / 8, r->life * 180 / 8);
			addScreenPolyFT4(prim, &r->p[0], &r->p[1], &r->p[2], &r->p[3]);
		}
		r++;
	}
}

void BTL_initializeUnk3(void)
{
	BTL_BUFF_DISKS[0].frame = -1;
	BTL_initializeBuffTrails();
}

int32_t BTL_addBuffDiskEffect(Entity *e)
{
	int32_t i;
	EfeBuffDisk *d;

	for (i = 0; i <= 0; i++) {
		if (BTL_BUFF_DISKS[i].frame == -1) {
			break;
		}
	}

	if (i == 1) {
		return -1;
	}

	d = &BTL_BUFF_DISKS[i];
	d->frame = 0;
	d->bone = &e->posData->posMatrix.workm;
	d->entity = e;
	d->rotation.vx = 0;
	d->rotation.vy = 0;
	d->rotation.vz = 0;
	d->scaleXZ = 0;
	d->scaleTargetXZ = DIGIMON_DATA[e->type].radius * 0xbb8 / 200;
	d->scaleTargetY = DIGIMON_DATA[e->type].height * 0xb22 / 200;
	addObject(0x815, i, BTL_tickBuffDisk, BTL_renderBuffDisk);
	BTL_addBuffTrails(i, e);

	return i;
}

void BTL_removeBuffDiskEffect(int32_t i)
{
	EfeBuffDisk *d;

	d = &BTL_BUFF_DISKS[i];
	if (i < 0) {
		return;
	}

	if (i > 0) {
		return;
	}

	d->frame = -1;
	removeObject(0x815, i);
	BTL_removeBuffTrails(i);
}

void BTL_removeAllBuffDiskEffects(void)
{
	int32_t i;

	for (i = 0; i <= 0; i++) {
		BTL_removeBuffDiskEffect(i);
	}
}

void BTL_tickBuffRings(int32_t idx)
{
	EfeBuffRings *b;
	int32_t j;

	b = &BTL_BUFF_RINGS[idx];
	if (b->frame >= 0x41) {
		b->frame = -1;
		removeObject(0x814, idx);
		return;
	}

	if (b->frame < 0x39) {
		if (b->frame < 0xf) {
			b->scale = lerp(b->scaleTarget / 3, b->scaleTarget, 0, 0xf, b->frame);
		} else if (b->frame < 0x2d) {
			b->scale = b->scaleTarget;
		} else {
			b->scale = lerp(b->scaleTarget, b->scaleTarget * 70 / 100, 0x2d, 0x39, b->frame);
		}
		for (j = 0; j < 5; j++) {
			b->rotation[j].vx -= (int16_t)(8 - j * 4 * 5);
			b->rotation[j].vy = 0;
			b->rotation[j].vz += (int16_t)((8 - (j * 4)) * 5);
		}
	}

	b->frame++;
}

void BTL_renderBuffRings(int32_t i)
{
	int32_t v;
	GsCOORDINATE2 coords[5];
	VECTOR zero;
	VECTOR *trans;
	VECTOR scale;
	int8_t col[4];
	GsCOORDINATE2 *super;
	EfeBuffRings *b;
	int32_t j;
	int32_t d;

	b = &BTL_BUFF_RINGS[i];
	zero.vx = zero.vy = zero.vz = 0;

	if (b->frame < 0x39) {
		super = NULL;
		trans = (VECTOR *)&b->bone->t[0];
		for (j = 0; j < 5; j++) {
			scale.vx = scale.vy = scale.vz = b->scale * (j + 10) / 10;
			renderTMDModel(BUFF_MODEL[0], BTL_BUFF_RING_OBJECTS[j], &coords[j], super, trans, &b->rotation[j], &scale);
			super = &coords[j];
			trans = &zero;
		}
		return;
	}

	v = lerp(0, b->scaleTarget, 0x39, 0x41, b->frame);
	d = b->scaleTarget - v;
	col[0] = b->color.r * d / b->scaleTarget;
	col[1] = b->color.g * d / b->scaleTarget;
	col[2] = b->color.b * d / b->scaleTarget;
	for (j = 0; j < 0x20; j += 2) {
		BTL_renderBuffRingsSpark((VECTOR *)&b->bone->t[0], v, &BTL_BUFF_RING_SPARKS[j], (uint8_t *)col);
	}
}

void BTL_renderBuffRingsSpark(VECTOR *pos, int32_t scale, SVECTOR *dir, uint8_t *col)
{
	POLY_FT3 *prim;
	SVECTOR c;
	SVECTOR a;
	SVECTOR b;

	prim = (POLY_FT3 *)GsGetWorkBase();
	SetPolyFT3(prim);
	SetSemiTrans(prim, 1);
	setRGB0(prim, col[0], col[1], col[2]);
	prim->tpage = getTPage(0, 1, 768, 256);
	prim->clut = getClut(192, 489);
	setUV3(prim, 0x30, 0xa8, 0x37, 0xa8, 0x30, 0xaf);
	a.vx = dir[0].vx * scale / 4096;
	a.vy = dir[0].vy * scale / 4096;
	a.vz = dir[0].vz * scale / 4096;
	b.vx = dir[1].vx * scale / 4096;
	b.vy = dir[1].vy * scale / 4096;
	b.vz = dir[1].vz * scale / 4096;
	copyVector(&c, pos);
	addVector(&a, pos);
	addVector(&b, pos);
	prim->code |= 2;
	addScreenPolyFT3(prim, &c, &a, &b);
}

void BTL_initializeUnk2(void)
{
	MATRIX m;
	SVECTOR v;
	SVECTOR rot;
	int32_t j;
	int32_t i;
	SVECTOR *p;

	setInt16WithStride(&BTL_BUFF_RINGS[0].frame, -1, 1, 0x6c);
	p = BTL_BUFF_RING_SPARKS;
	for (i = 0; i < 0x10; i++) {
		v.vx = (rand() % 0x32) + 0x32;
		v.vy = 0;
		v.vz = (rand() % 0x190) + 0x320;
		rot.vx = rand();
		rot.vy = rand();
		rot.vz = rand();
		RotMatrixZYX(&rot, &m);
		ApplyMatrixSV(&m, &v, p++);
		v.vx *= -1;
		ApplyMatrixSV(&m, &v, p++);
	}
}

// clang-format off
int32_t BTL_addBuffRingsEffect(idx, e)
	int16_t idx;
	Entity *e;
// clang-format on
{
	int32_t i;
	EfeBuffRings *b;
	int32_t j;

	for (i = 0; i <= 0; i++) {
		if (BTL_BUFF_RINGS[i].frame < 0) {
			break;
		}
	}

	if (i == 1) {
		return -1;
	}

	b = &BTL_BUFF_RINGS[i];
	b->frame = 0;
	b->buffId = idx;
	b->scale = 0;
	for (j = 0; j < 5; j++) {
		b->rotation[j].vx = 0x258;
		b->rotation[j].vy = 0;
		b->rotation[j].vz = 0x258;
	}

	b->bone = &e->posData[1].posMatrix.workm;
	b->color.r = BTL_BUFF_RING_COLOR_R[idx];
	b->color.g = BTL_BUFF_RING_COLOR_G[idx];
	b->color.b = BTL_BUFF_RING_COLOR_B[idx];
	for (j = 0; j < 0x10; j++) {
		b->unk48[j] = -1;
	}

	b->scaleTarget = 0x1000 - ((0xc8 - DIGIMON_DATA[e->type].radius) * 400 / 200);
	addObject(0x814, i, BTL_tickBuffRings, BTL_renderBuffRings);

	return i;
}

void BTL_removeAllBuffRingsEffects(void)
{
	int32_t i;

	for (i = 0; i <= 0; i++) {
		BTL_BUFF_RINGS[i].frame = -1;
		removeObject(0x814, i);
	}
}
