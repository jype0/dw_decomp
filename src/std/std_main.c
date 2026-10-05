#include <stdlib.h>
#include <string.h>

#include <inline_n.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/aabb.h>
#include <dw/attack_object.h>
#include <dw/battle.h>
#include <dw/clock.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/font.h>
#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/item.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/rng.h>
#include <dw/sound.h>
#include <dw/std.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/vecmath.h>
#include <dw/world_object.h>

#include "common.h"

typedef struct {
	int16_t score[5];
	int16_t best;
} MoveRanking;

typedef struct {
	int16_t flags[4];
	int16_t enemies[5];
	int16_t count;
} TargetChoice;

typedef struct {
	int16_t timer;
	int8_t phase;
	int8_t side;
} CameraChase;

extern int16_t ENEMY_COUNT;
extern int16_t BATTLE_FRAME_COUNT;
extern Entity *FINISHING_ENTITY;
extern int32_t NO_AI_FLAG;
extern uint8_t *GENERAL_BUFFER_PTR;
extern int8_t GAME_STATE;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern uint8_t TYPE_FACTORS[][7];
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[];
extern PACKET GS_WORK_BASES[];
extern char DR_OFFSETS[];
extern int32_t MAIN_D_801350EC;
extern SVECTOR STDVS_VIEW_ROTATION[];
extern VECTOR STDVS_VIEW_TRANSLATION;
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t VIEWPORT_DISTANCE;
extern Entity *MAIN_D_801350E8;
extern uint8_t CURRENT_SCREEN;
extern char MAIN_D_80124C0C[][12];
extern char MAIN_D_80124C54[];
extern int32_t COMBAT_AREA_X;
extern int32_t COMBAT_AREA_Y;
extern uint8_t MAIN_D_801350F8;
extern GsVIEW2 STDVS_VIEW;
extern GsCOORDINATE2 MAIN_D_801B1BBC;
extern int32_t FLEE_DISABLED[2];
extern int32_t VS_P2_AOE_TIMER;
extern char *MOVE_NAMES[];
extern int16_t MAIN_D_801350E4;
extern int8_t BATTLE_TOGGLE_LIFEBAR;
extern int16_t INITIAL_COMBAT_STATS[][6];
extern DigimonEntity *BATTLE_TARGETED_DIGIMON;
extern DigimonEntity *BATTLE_ATTACKING_DIGIMON;

void STD_initializeBattleStartText(void);
void STD_func_8006A044(void);
void STD_initializeBattleStartTextBurst(void);
void STD_func_8006A508(void);
int32_t STD_func_8006A514(void);
char *STD_initializeEFEEngine(char *base);
void STD_loadMoveEFE(int16_t *moves, int16_t *effectIds, int8_t *isLoaded);
void tickFileReadQueue(int32_t instanceId);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
int32_t entityCheckCollision(Entity *a, Entity *entity, int32_t c, int32_t d);
void handleBattleIdle(DigimonEntity *entity, Stats *stats, int32_t flags);
void createParticleFX();
void collisionGrace(Entity *a, Entity *entity, int32_t c, int32_t d);
int32_t STD_func_80061AA8(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int32_t STD_tickMeleeAttack(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int16_t arg3);
void STD_func_80065540(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int16_t STD_getMostEffectiveMove(int32_t arg0, int16_t *flags);
void addEntityText(DigimonEntity *digimon, long slot, int32_t color, int32_t value, uint8_t flag);
void setupModelMatrix(PositionData *posData);
void startAnimation(Entity *entity, uint8_t animId);
void tickAnimation(Entity *entity);
void addWithLimit(/* int16_t *value, int16_t amount, int16_t limit */);
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void STD_removeStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, uint8_t kind);
void STD_addStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, uint8_t kind);
void swapInt(int32_t *a, int32_t *b);
void STD_renderCounterDigits(int16_t x, int16_t y, int16_t digits, int32_t value, int32_t layer);
void STD_func_80058898(int32_t i);
void STD_setViewpointRotationFromEntity(void);
void STD_applyViewpoint(void);
void STD_func_8005B688(Entity *target, Entity *entity);
void STD_func_8006CCE0(int32_t a);
void STD_func_8006C6DC(void);
void STD_removeFinisherChargeup(void);
void STD_removeFinisherAura(int32_t id);
void STD_setCameraParams(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f);
void STD_setVSPhase(int32_t arg);
void STD_func_8005A550(void);
int32_t STD_func_8005ADFC(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end);
void STD_removeCameraIntro(void);
void STD_addFighterCounter(int32_t arg);
void STD_renderFighterCounter(void);
void STD_removeFighterCounter(void);
void STD_func_8005D7A8(int32_t i);
void STD_func_8005D7B4(int16_t i);
void STD_func_8005DF64(void);
int32_t STD_func_8005DFF8(void);
void STD_func_8005E898(void);
void STD_resetFlatten(int16_t index);
void STD_faintDigimon(DigimonEntity *digimon, FighterData *fighter, int16_t arg2);
void STD_tickAttackState(Entity *entity, DigimonEntity *target, int32_t id);
void STD_tickHitState(Entity *entity, FighterData *fighter, int32_t arg2);
void STD_tickFlatState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3);
void STD_tickStunState(Entity *entity);
void STD_tickConfusedState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3);
void STD_tickChargeState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void STD_tickCooldownState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void STD_tickQueuedMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3);
uint32_t STD_getMoveWithHighestDistance(DigimonEntity *digimon);
void STD_setWalking(Entity *entity, Stats *stats, uint16_t flags);
void STD_backAwayFromTarget(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void STD_moveTowardLocation(DigimonEntity *digimon, VECTOR *target, int16_t dx, int16_t dy);
void STD_tickFighterAction(int32_t index);
void STD_confusedRotate(Entity *entity);
void STD_maintainTargetDistance(DigimonEntity *attacker, DigimonEntity *target, FighterData *fighter);
void STD_maintainDistanceRange(DigimonEntity *attacker, DigimonEntity *target, FighterData *fighter, uint32_t nearLimit, uint32_t farLimit);
int32_t STD_getContactRangeSquared(int32_t *a, int32_t *b);
void STD_increaseSpeedBuffer(FighterData *fighter, Stats *stats);
int32_t STD_func_80062BD8(int16_t *out, int16_t index);
void STD_startWalkingAnimation(Entity *entity, Stats *stats, uint16_t flags);
void STD_clearBlockedAttacks(FighterData *fighter);
void STD_findUnblockedRotation(Entity *entity, int16_t *rot, int16_t hit, int16_t orig);
void STD_startWalkingAnimation2(Entity *entity, Stats *stats, uint16_t flags);
int16_t STD_getAttackTech(AttackObject *attack);
int32_t STD_applyBuffMove(DigimonEntity *digimon, int32_t slot, int16_t anim);
void STD_applyMoveStatus(DigimonEntity *digimon, FighterData *fighter, int32_t move);
int16_t STD_getFighterSlot(int16_t entityId);
int32_t STD_addBlockedAttack(FighterData *fighter, FighterData *other);
void STD_buffStats(DigimonEntity *digimon, int32_t slot, int32_t value, int16_t *stat, int32_t color, int32_t flag);
void STD_startHitAnimation(Entity *entity, AttackObject *attack, uint8_t animId);
void STD_battleTickFrame(void);
int32_t STD_isMoveUsable(DigimonEntity *digimon, FighterData *fighter, int16_t slot);
int32_t STD_getDistanceSquared(Entity *a, Entity *b);
void STD_setupQueuedMove(DigimonEntity *digimon, FighterData *fighter, int16_t arg2, uint8_t moveIndex);
void STD_applyChargeRequirement(DigimonEntity *digimon, FighterData *fighter, int16_t tech);
void STD_removeMoveEffect(DigimonEntity *digimon, FighterData *fighter);
void STD_addFinisherProgress(FighterData *fighter, int16_t amount);
void STD_clearStun(DigimonEntity *digimon, FighterData *fighter);
void STD_applyFlattenScale(VECTOR *scale, int32_t t);
void STD_applyStretchScale(VECTOR *scale, int32_t angle);
void STD_applySquashScale(VECTOR *scale, int32_t angle);
void STD_resetFighterAction(FighterData *fighter);
void STD_addPoisonStatusVisual(DigimonEntity *digimon, FighterData *fighter);
void STD_addConfusionStatusVisual(DigimonEntity *digimon, FighterData *fighter);
void STD_addStunStatusVisual(DigimonEntity *digimon, FighterData *fighter);
void STD_removeStatusEffects(DigimonEntity *digimon, FighterData *fighter);
int32_t STD_func_80066A50(int16_t *out, int16_t index);
void STD_setFighterCooldown(DigimonEntity *digimon, FighterData *fighter);
int16_t STD_getRandomUsableMove(int16_t *flags);
int16_t STD_getStrongestMove(int32_t index, int16_t *flags);
int16_t STD_getCheapestMove(int32_t index, int16_t *flags);
void STD_getHighestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count);
void STD_getLowestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count);
int16_t STD_getNpcEntityIndex(Entity *entity);
void STD_sortScoresDescending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count);
void STD_sortScoresAscending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count);
int16_t STD_calculateElementBonus(int16_t arg0, int16_t arg1);
int32_t STD_countLivingEnemies(void);
void STD_calculateScoreRanks(int32_t *values, int32_t *groups, int32_t count);
void STD_getRemainingEnemies(Entity *self, int16_t *out, int16_t *count);
int32_t STD_func_800675E8(int32_t arg0, int16_t *flags);
int32_t STD_func_80067660(int32_t arg0, int16_t *flags);
uint8_t STD_isFighterDefeated(uint8_t index);
void STD_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, uint8_t index);
void STD_tickCommandMenu(uint8_t i);
void STD_removeCommandMenu(int32_t i);
void STD_stopEFESubEffect(int32_t a, int32_t b);
int32_t STD_addAuraProjectile(Entity *e);
void STD_func_80058684(Entity *entity, int32_t id);
int32_t readVBALLSection(int32_t vabId, int32_t idx);
int32_t isSoundLoaded(int32_t mode, int32_t vabId);
void STD_func_80058E28(int16_t which);
void STD_func_80059908(void);
void STD_func_8005A1BC(void);
void STD_func_8005A55C(DigimonEntity *entity, int32_t mode, uint8_t sub);
void STD_tickCameraChase(void);
void STD_func_8005C1E4(void);
void STD_func_8005CE9C(void);
void STD_func_8005D398(int16_t i, int32_t owner, uint8_t flag);
void STD_func_8005D9F4(uint8_t *out, uint8_t *list);
void STD_func_8005E124(int32_t id);
void STD_func_8005E1E4(int16_t id);
void STD_func_8005EF84(void);
void STD_func_8005F650(void);
void STD_func_8005FDDC(void);
void STD_func_800602A8(void);
void STD_func_80060C14(int16_t hasLostP1, uint8_t hasLostP2);
void STD_func_80060EBC(void);
void STD_func_80063508(int16_t id);
int16_t STD_applyPartnerStatsToFighter(DigimonEntity *attacker, DigimonEntity *defender, FighterData *fighter, int16_t move);
int32_t STD_calculateDamage(DigimonEntity *attacker, DigimonEntity *defender, int16_t move);
void STD_handleHitReaction(Entity *entity, FighterData *fighter, AttackObject *attack, int16_t index);
void STD_func_800647F8(void);
void STD_applyMoveResult(void);
void STD_func_80067744(DigimonEntity *digimon, FighterData *fighter, int16_t index);
void STD_func_80067A30(DigimonEntity *digimon, FighterData *fighter, int16_t index);
void STD_renderCommandMenu(uint8_t id);
void STD_addCommandMenu(uint8_t index);
int32_t STD_addStunEffect(DigimonEntity *digimon, int32_t val);
int32_t STD_addConfusionEffect(DigimonEntity *digimon);
int32_t STD_func_80077664(DigimonEntity *digimon);
void STD_removeStunEffect(int32_t id, DigimonEntity *digimon);
void STD_removeConfusionEffect(int32_t id, DigimonEntity *digimon);
void STD_removePoisonEffect(int32_t id, DigimonEntity *digimon);
int32_t STD_addFinisherAura(Entity *entity, int32_t arg1);
void STD_func_80069134(int16_t tech);
void STD_func_800658B4(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int32_t STD_selectMoveTarget(Entity *entity, FighterData *fighter);
void STD_func_8005D538(int16_t i);
void STD_func_8005DF6C(void);
void STD_func_800588A4(int32_t i);
void STD_func_80060AA0(void);
int32_t STD_func_80061124(int32_t value);
void STD_func_80062D14(void);
int16_t STD_func_80058634(Entity *entity);
int16_t STD_func_80062D5C(Entity *entity);
void STD_func_800615D8(DigimonEntity *digimon, FighterData *fighter);
void STD_func_8005DF94(int16_t mode);
int32_t playMusic(int32_t font, int32_t track);
void STD_func_8005DEEC(int16_t track);
void STD_func_80058494(int16_t which);
void STD_func_8005E5E0(void);
void STD_func_80058504(int16_t which);
void STD_func_8005E660(void);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uvX, int32_t uvY, int32_t width, int32_t height);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void STD_func_8005E6E4(void);
void STD_func_8006324C(void);
void STD_func_80058958(int32_t idx, int16_t value);
void STD_func_8005E004(int32_t i);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount, int32_t *digits);
int32_t STD_getFighterDistance(VECTOR *self, VECTOR *other, VECTOR *target);
int32_t STD_func_80060B98(void);
void STD_tickFighterCounter(void);
void STD_applyEntityViewpoint(void);
void STD_updateFighterStatusVisuals(DigimonEntity *digimon, FighterData *fighter);
void handlePause(void);
void removePauseBox(void);
int16_t STD_func_80060620(int16_t a, int16_t b);
int16_t STD_func_8005F354(void);
void STD_func_8005E8A4(Entity *entity, Entity *other);
int16_t STD_func_8006314C(Entity *entity, Entity *other);
void STD_func_800587F0(Entity *entity);
void STD_func_8005D964(void);
int16_t STD_getNearestEnemy(Entity *self, int16_t *flags);
void STD_func_800588D4(Entity *entity, int32_t id);
void STD_func_80060998(void);
void STD_selectConfusedMove(DigimonEntity *digimon, FighterData *fighter, long index);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void STD_func_80063300(int32_t id);
void SetPolyGT4(POLY_GT4 *prim);
uint32_t playSound(int32_t vabId, int32_t val);
void STD_func_80061F44(DigimonEntity *entity, DigimonEntity *other, FighterData *data, int16_t move);
void STD_func_80059658(int32_t id);
void STD_func_80059524(int32_t id);
void STD_func_800593D0(int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer);
void STD_func_80059204(int32_t id);
void STD_func_80059080(int32_t id);
void STD_func_80058A60(int16_t x, int16_t y, int16_t size, uint8_t character);
void STD_func_8005858C(void);
int32_t BTL_getDistanceSquared(Entity *a, Entity *b);
int32_t BTL_getUsableMoves(int16_t *out, int16_t index);
void BTL_clearConfusion(DigimonEntity *digimon, FighterData *fighter);
void STD_func_8005A44C(void);
void STD_func_80064FCC(int16_t count);
int16_t BTL_calculateHitChance(DigimonEntity *attacker, DigimonEntity *defender, FighterData *fighter, int16_t move);
void BTL_retargetAfterHit(DigimonEntity *digimon, FighterData *fighter, AttackObject attack);
void BTL_startQueuedMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int32_t STD_isPositionNearEntity(Entity *entity, VECTOR *pos);
int32_t BTL_isMoveOnCooldown(Entity *entity, FighterData *fighter);
void BTL_setupMoveExecution(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void STD_func_8005D7C0(POLY_FT4 *prim, int32_t a, int32_t b, int32_t h);
void VS__tickBattleResultScreen(uint8_t hasLostP1, uint8_t hasLostP2);
int32_t STD_isVersusModelSceneFinished(void);
void VS_selectRandomCamera(DigimonEntity *entity, int32_t type, int32_t value);
void STD_func_8006B2BC(void);
void STD_func_8006B468(void);
void STD_func_8006B6E8(void);
void STD_func_80059DBC(void);
void STD_func_8005A830(void);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
void STD_setViewpointFromBone(Entity *entity, SVECTOR *offset, SVECTOR *rot, int32_t dist);
void STD_setRandomViewpoint(Entity *entity, int32_t idx);
void STD_func_8005D814(int16_t x, int16_t y, uint8_t n, int32_t layer);
int32_t _atan(int32_t y, int32_t x);
int16_t STD_getMostEffectiveMove(int32_t index, int16_t *flags);
int32_t lerp(int32_t a, int32_t b, int32_t lo, int32_t hi, int32_t t);
void STD_updateCameraLerp(int32_t t, int8_t flip);
int32_t STD_func_8005ADFC(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e);
int32_t processSomeArenaArrays(int32_t count, int32_t t, int32_t *keys, int32_t *values, int32_t *slopes);
void STD_tickCameraIntro(void);
void STD_startCameraChase(Entity *entity, int32_t dx, int32_t side);
void STD_setCameraToEntity(void);
void STD_func_8005A054(void);
void swapShort(int16_t *a, int16_t *b);
void STD_func_8005D550(int16_t id);
void STD_func_80068388(int32_t i);
void STD_func_80059B70(void);
void STD_removeAllStunEffects(void);
void STD_func_800791E0(void);
void STD_removeAllPoisonEffects(void);
void STD_func_80079874(void);
void STD_unloadAllEFESlots(void);
void STD_removeEFEEngine(void);
int32_t loadSB(void);
void initializeSomeArenaArrays(int32_t count, int32_t arg1, int32_t arg2, int32_t *out);
void STD_func_8005D814(int16_t x, int16_t y, uint8_t n, int32_t layer);
void swapByte(uint8_t *a, uint8_t *b);
void STD_func_8006AD00(int32_t id);
void resetFlattenGlobal(void);
void initializeAttackObjects(void);
int32_t STD_func_80060B98(void);
void STD_func_8005A55C(DigimonEntity *entity, int32_t mode, uint8_t sub);
void STD_selectFighterTarget(DigimonEntity *digimon, FighterData *fighter, uint8_t target, int32_t arg3);
void removeEntityText(int32_t id);
void STD_func_8006B1E4(int32_t i);
int32_t STD_startEFE(int32_t i);

static void *std_main_functions[] = {
	STD_removeCommandMenu,
	STD_renderCommandMenu,
	STD_tickCommandMenu,
	STD_addCommandMenu,
	STD_setCommandIconUV,
	STD_func_80068388,
	STD_getNearestEnemy,
	STD_func_80067A30,
	STD_func_80067744,
	STD_isFighterDefeated,
	STD_func_80067660,
	STD_func_800675E8,
	STD_getRemainingEnemies,
	STD_selectConfusedMove,
	STD_calculateScoreRanks,
	STD_countLivingEnemies,
	STD_calculateElementBonus,
	STD_sortScoresAscending,
	STD_sortScoresDescending,
	STD_getNpcEntityIndex,
	STD_getLowestScoredMove,
	STD_getHighestScoredMove,
	STD_getCheapestMove,
	STD_getMostEffectiveMove,
	STD_getStrongestMove,
	STD_getRandomUsableMove,
	STD_setFighterCooldown,
	STD_func_80066A50,
	STD_removeStatusEffects,
	STD_addStunStatusVisual,
	STD_addConfusionStatusVisual,
	STD_addPoisonStatusVisual,
	STD_resetFighterAction,
	STD_applyMoveResult,
	STD_removeStatusEffectVisual,
	STD_applySquashScale,
	STD_applyStretchScale,
	STD_addStatusEffectVisual,
	STD_applyFlattenScale,
	STD_clearStun,
	STD_updateFighterStatusVisuals,
	STD_addFinisherProgress,
	STD_removeMoveEffect,
	STD_func_800658B4,
	STD_selectMoveTarget,
	STD_func_80065540,
	STD_applyChargeRequirement,
	STD_setupQueuedMove,
	STD_getDistanceSquared,
	STD_isMoveUsable,
	STD_battleTickFrame,
	STD_func_80064FCC,
	STD_func_800647F8,
	STD_startHitAnimation,
	STD_buffStats,
	STD_addBlockedAttack,
	STD_getFighterSlot,
	STD_applyMoveStatus,
	STD_handleHitReaction,
	STD_calculateDamage,
	STD_applyPartnerStatsToFighter,
	STD_applyBuffMove,
	STD_getAttackTech,
	STD_func_80063508,
	STD_startWalkingAnimation2,
	STD_func_80063300,
	STD_func_8006324C,
	STD_func_8006314C,
	STD_findUnblockedRotation,
	STD_clearBlockedAttacks,
	STD_func_80062D5C,
	STD_func_80062D14,
	STD_startWalkingAnimation,
	STD_func_80062BD8,
	STD_increaseSpeedBuffer,
	STD_getContactRangeSquared,
	STD_maintainDistanceRange,
	STD_maintainTargetDistance,
	STD_confusedRotate,
	STD_tickFighterAction,
	STD_moveTowardLocation,
	STD_backAwayFromTarget,
	STD_setWalking,
	STD_getMoveWithHighestDistance,
	STD_func_80061F44,
	STD_tickMeleeAttack,
	STD_func_80061AA8,
	STD_tickQueuedMove,
	STD_tickCooldownState,
	STD_tickChargeState,
	STD_func_800615D8,
	STD_tickConfusedState,
	STD_tickStunState,
	STD_tickFlatState,
	STD_tickHitState,
	STD_tickAttackState,
	STD_func_80061124,
	STD_faintDigimon,
	STD_func_80060EBC,
	STD_func_80060C14,
	STD_func_80060B98,
	STD_resetFlatten,
	STD_func_80060AA0,
	STD_func_80060998,
	STD_func_80060620,
	STD_func_800602A8,
	STD_func_8005FDDC,
	STD_func_8005F650,
	STD_func_8005F354,
	STD_func_8005EF84,
	STD_func_8005E8A4,
	STD_func_8005E898,
	STD_func_8005E6E4,
	STD_func_8005E660,
	STD_func_8005E5E0,
	STD_func_8005E1E4,
	STD_func_8005E124,
	STD_func_8005E004,
	STD_func_8005DFF8,
	STD_func_8005DF94,
	STD_func_8005DF6C,
	STD_func_8005DF64,
	STD_func_8005DEEC,
	STD_func_8005D9F4,
	STD_func_8005D964,
	STD_func_8005D814,
	STD_func_8005D7C0,
	STD_func_8005D7B4,
	STD_func_8005D7A8,
	STD_func_8005D550,
	STD_func_8005D538,
	STD_func_8005D398,
	STD_func_8005CE9C,
	STD_func_8005C1E4,
	STD_removeFighterCounter,
	STD_renderFighterCounter,
	STD_tickFighterCounter,
	STD_addFighterCounter,
	STD_renderCounterDigits,
	STD_applyEntityViewpoint,
	STD_removeCameraIntro,
	STD_func_8005B688,
	STD_tickCameraIntro,
	STD_startCameraChase,
	STD_tickCameraChase,
	STD_func_8005ADFC,
	STD_isPositionNearEntity,
	STD_updateCameraLerp,
	STD_setViewpointFromBone,
	STD_getFighterDistance,
	STD_func_8005A830,
	STD_setRandomViewpoint,
	STD_func_8005A55C,
	STD_func_8005A550,
	STD_func_8005A44C,
	STD_setVSPhase,
	STD_setCameraParams,
	STD_func_8005A1BC,
	STD_applyViewpoint,
	STD_func_8005A054,
	STD_setViewpointRotationFromEntity,
	STD_setCameraToEntity,
	STD_func_80059DBC,
	STD_func_80059B70,
	STD_func_80059908,
	STD_func_80059658,
	STD_func_80059524,
	STD_func_800593D0,
	STD_func_80059204,
	STD_func_80059080,
	STD_func_80058E28,
	STD_func_80058A60,
	STD_func_80058958,
	STD_func_800588D4,
	STD_func_800588A4,
	STD_func_80058898,
	STD_func_800587F0,
	STD_func_80058684,
	STD_func_80058634,
	STD_func_8005858C,
	STD_func_80058504,
	STD_func_80058494,
};

uint8_t MAIN_D_80134800[4] = { 64, 44, 38, 32 };
uint8_t MAIN_D_80134804[4] = { 16, 7, 3, 0 };
/* HP */
char STD_STR_HP[] = "ＨＰ";
/* MP */
char STD_STR_MP[] = "ＭＰ";
SVECTOR MAIN_D_80134818 = { 0 };
SVECTOR MAIN_D_80134820 = { 0, -1592, 0, 0 };
SVECTOR MAIN_D_80134828 = { 0 };
SVECTOR MAIN_D_80134830 = { 0, -1592, 0, 0 };
SVECTOR MAIN_D_80134838 = { 0 };
SVECTOR MAIN_D_80134840 = { 0 };
SVECTOR MAIN_D_80134848 = { 0 };
SVECTOR MAIN_D_80134850 = { 0 };
SVECTOR MAIN_D_80134858 = { 0 };
SVECTOR MAIN_D_80134860 = { -227, 1479, 0, 0 };
SVECTOR MAIN_D_80134868 = { 0 };
int16_t MAIN_D_80134870[4] = { 0, 1024, 2048, 3072 };
/* Dealt */
char STD_STR_ATAETA[] = "与えた";
uint8_t MAIN_D_80134880[5] = { 2, 3, 4, 5, 6 };
uint8_t MAIN_D_80134888[4] = { 50, 20, 5, 0 };
uint8_t MAIN_D_8013488C[4] = { 50, 20, 10, 0 };
uint8_t MAIN_D_80134890[4] = { 10, 5, 0, 0 };
#if defined(VERSION_JP)
char MAIN_D_80134894[] = "にげる";
#else
char MAIN_D_80134894[] = "Run";
char MAIN_D_80134898[] = "Attack";
char MAIN_D_801348A0[] = "Auto";
char MAIN_D_801348A8[] = "Change";
#endif
uint8_t MAIN_D_801348B0[5] = { 0, 11, 25, 39, 50 };
uint8_t MAIN_D_801348B8[5] = { 11, 14, 14, 11, 11 };

int16_t MAIN_D_801350FC;
int16_t MAIN_D_801350FE;
int16_t MAIN_D_80135100;
int16_t MAIN_D_80135102;
int32_t MAIN_D_80135104;
int32_t MAIN_D_80135108;
int32_t MAIN_D_8013510C;
uint8_t MAIN_D_80135110;
int32_t MAIN_D_80135114;
int32_t MAIN_D_80135118;
int16_t MAIN_D_8013511C;
int32_t MAIN_D_80135120;
int32_t MAIN_D_80135124;
char **MAIN_D_80135128;
uint8_t MAIN_D_8013512C;
void *MAIN_D_80135130;
int32_t MAIN_D_80135134;
int16_t MAIN_D_80135138;
int16_t MAIN_D_8013513A;
uint8_t MAIN_D_8013513C;
int16_t MAIN_D_8013513E;
int16_t MAIN_D_80135140;
uint8_t MAIN_D_80135142;
CameraChase MAIN_D_80135144;
int32_t MAIN_D_80135148;
uint8_t MAIN_D_8013514C;
uint8_t MAIN_D_8013514D;
uint8_t PARTICIPANT_TYPES[8];
uint8_t MAIN_D_80135158[4];
uint8_t MAIN_D_8013515C;
uint8_t MAIN_D_8013515D;
int32_t MAIN_D_80135160;
uint8_t MAIN_D_80135164;
uint8_t MAIN_D_80135165;
uint8_t MAIN_D_80135166;
uint8_t MAIN_D_80135167;
uint8_t MAIN_D_80135168;
int32_t MAIN_D_8013516C;
int16_t MAIN_D_80135170;
uint8_t MAIN_D_80135172;
int32_t MAIN_D_80135174;
int32_t MAIN_D_80135178;
int16_t MAIN_D_8013517C[2];
int16_t MAIN_D_80135180[2];
uint8_t MAIN_D_80135184[2];
uint8_t MAIN_D_80135186[2];
uint8_t MAIN_D_80135188[2];
int8_t MAIN_D_8013518A[2];

static void *std_main_sbss_order[] = {
	&MAIN_D_8013518A,
	&MAIN_D_80135188,
	&MAIN_D_80135186,
	&MAIN_D_80135184,
	&MAIN_D_80135180,
	&MAIN_D_8013517C,
	&MAIN_D_80135178,
	&MAIN_D_80135174,
	&MAIN_D_80135172,
	&MAIN_D_80135170,
	&MAIN_D_8013516C,
	&MAIN_D_80135168,
	&MAIN_D_80135167,
	&MAIN_D_80135166,
	&MAIN_D_80135165,
	&MAIN_D_80135164,
	&MAIN_D_80135160,
	&MAIN_D_8013515D,
	&MAIN_D_8013515C,
	&MAIN_D_80135158,
	&PARTICIPANT_TYPES,
	&MAIN_D_8013514D,
	&MAIN_D_8013514C,
	&MAIN_D_80135148,
	&MAIN_D_80135144,
	&MAIN_D_80135142,
	&MAIN_D_80135140,
	&MAIN_D_8013513E,
	&MAIN_D_8013513C,
	&MAIN_D_8013513A,
	&MAIN_D_80135138,
	&MAIN_D_80135134,
	&MAIN_D_80135130,
	&MAIN_D_8013512C,
	&MAIN_D_80135128,
	&MAIN_D_80135124,
	&MAIN_D_80135120,
	&MAIN_D_8013511C,
	&MAIN_D_80135118,
	&MAIN_D_80135114,
	&MAIN_D_80135110,
	&MAIN_D_8013510C,
	&MAIN_D_80135108,
	&MAIN_D_80135104,
	&MAIN_D_80135102,
	&MAIN_D_80135100,
	&MAIN_D_801350FE,
	&MAIN_D_801350FC,
};

// clang-format off
uint8_t STD_D_80079CBC[112][14] = {
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x2c, 0x18, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x0c, 0x0e, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x00, 0x1f, 0x0a, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x2b, 0x18, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x0a, 0x0d, 0x01, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x1b, 0x1f, 0x29, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x03, 0x00, 0x1f, 0x1c, 0x05, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1b, 0x26, 0x05, 0x24, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x33, 0x05, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x11, 0x2d, 0x1f, 0x1c, 0x05, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x22, 0x33, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x33, 0x18, 0x07, 0x3d, 0x1f, 0x0a, 0x0d, 0x01,
		0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x30, 0x33, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x16, 0x17, 0x1f, 0x1d, 0x1e, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x25, 0x2a, 0x21, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1a, 0x24, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x08, 0x1f, 0x2a, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x03, 0x0d, 0x09, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x08, 0x1f, 0x2a, 0x1b, 0x06, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x03, 0x15, 0x1f, 0x11, 0x27, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x28, 0x2d, 0x1f, 0x1c, 0x05, 0x34, 0x15,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x08, 0x07, 0x07, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x3a, 0x09, 0x1f, 0x18, 0x07, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x2c, 0x03, 0x2d, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x2b, 0x1f, 0x11, 0x2d, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x12, 0x08, 0x07, 0x3d, 0x1f, 0x0a, 0x0d, 0x01,
		0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x33, 0x18, 0x07, 0x30, 0x33, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x2b, 0x2d, 0x1f, 0x18, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x25, 0x2c, 0x3c, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1c, 0x0c, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x25, 0x28, 0x18, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x0a, 0x23, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x3a, 0x21, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x04, 0x2d, 0x1f, 0x08, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x11, 0x27, 0x07, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x0b, 0x15, 0x18, 0x07, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x28, 0x0b, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x1c, 0x06, 0x34, 0x1f, 0x0b, 0x34, 0x15,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x12, 0x08, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x00, 0x15, 0x1f, 0x1c, 0x0e, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x09, 0x0e, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x03, 0x1b, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x3a, 0x05, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x18, 0x23, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x25, 0x29, 0x3c, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x25, 0x28, 0x07, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x34, 0x24, 0x0a, 0x0e, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x0d, 0x04, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x11, 0x2d, 0x05, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x0c, 0x08, 0x1c, 0x06, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x0a, 0x0f, 0x1f, 0x08, 0x2d, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x34, 0x1f, 0x11, 0x2e, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x20, 0x21, 0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x33, 0x1f, 0x08, 0x1f, 0x1c, 0x05, 0x34, 0x15,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x25, 0x29, 0x2f, 0x0c, 0x0e, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x1b, 0x1f, 0x11, 0x18, 0x30, 0x34, 0x15,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x25, 0x2b, 0x15, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x01, 0x1f, 0x08, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x2c, 0x02, 0x04, 0x02, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x2b, 0x05, 0x0a, 0x07, 0x3d, 0x08, 0x1f, 0x2a,
		0x1b, 0x06, 0x34, 0x15, 0x00, 0x00,
	},
	{
		0x33, 0x1f, 0x08, 0x3d, 0x11, 0x2d, 0x1f, 0x1c,
		0x05, 0x34, 0x15, 0x00, 0x00, 0x00,
	},
	{
		0x02, 0x27, 0x00, 0x3d, 0x1f, 0x08, 0x07, 0x07,
		0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x25, 0x28, 0x15, 0x1f, 0x11, 0x2e, 0x34, 0x15,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x09, 0x1f, 0x08, 0x1f, 0x1c, 0x05, 0x34,
		0x15, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x33, 0x18, 0x07, 0x03, 0x1b, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x02, 0x35, 0x15, 0x1f, 0x1b, 0x34, 0x15,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x38, 0x15, 0x30, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x0c, 0x1a, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x2a, 0x0d, 0x00, 0x06, 0x1f, 0x10, 0x34, 0x15,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x0f, 0x07, 0x3d, 0x16, 0x17, 0x1f, 0x1d, 0x1e,
		0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x3a, 0x09, 0x00, 0x1f, 0x0a, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x29, 0x36, 0x2d, 0x1f, 0x08, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x25, 0x2a, 0x05, 0x19, 0x20, 0x3d, 0x12, 0x08,
		0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x1c, 0x0a, 0x23, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x11, 0x30, 0x3a, 0x21, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x18, 0x15, 0x0a, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x0d, 0x2f, 0x1f, 0x1c, 0x3d, 0x1f, 0x2b, 0x1f,
		0x11, 0x2d, 0x34, 0x15, 0x00, 0x00,
	},
	{
		0x1f, 0x11, 0x2e, 0x15, 0x1f, 0x0a, 0x07, 0x3d,
		0x34, 0x1f, 0x11, 0x2e, 0x34, 0x15,
	},
	{
		0x21, 0x13, 0x3d, 0x1f, 0x1c, 0x06, 0x34, 0x1f,
		0x0b, 0x34, 0x15, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x0c, 0x1f, 0x2a, 0x06, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1a, 0x19, 0x1f, 0x18, 0x07, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x10, 0x01, 0x0b, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x34, 0x1f, 0x1c, 0x09, 0x1f, 0x2b, 0x18, 0x34,
		0x15, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1c, 0x01, 0x00, 0x1f, 0x0a, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x25, 0x29, 0x2f, 0x1f, 0x1c, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x00, 0x07, 0x05, 0x02, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x0b, 0x0d, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x02, 0x35, 0x2d, 0x31, 0x06, 0x34, 0x15,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x2a, 0x2d, 0x1f, 0x08, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1b, 0x2f, 0x08, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x34, 0x06, 0x11, 0x27, 0x07, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x08, 0x2d, 0x1f, 0x1c, 0x0e, 0x34, 0x15,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x32, 0x2d, 0x19, 0x36, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x00, 0x01, 0x12, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x00, 0x08, 0x1c, 0x06, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1a, 0x08, 0x01, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x11, 0x2e, 0x2d, 0x30, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x0a, 0x06, 0x00, 0x00, 0x1f, 0x0a, 0x34, 0x15,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x10, 0x2f, 0x14, 0x2d, 0x34, 0x15, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x00, 0x01, 0x12, 0x1f, 0x1b, 0x1f, 0x29, 0x34,
		0x15, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x18, 0x2d, 0x0a, 0x06, 0x1f, 0x10, 0x34,
		0x15, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x10, 0x15, 0x1f, 0x1c, 0x3d, 0x38, 0x15, 0x30,
		0x34, 0x15, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x12, 0x24, 0x2d, 0x3d, 0x1f, 0x0c, 0x1f, 0x2a,
		0x06, 0x34, 0x15, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x2a, 0x07, 0x2d, 0x33, 0x05, 0x34, 0x15,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x0a, 0x07, 0x07, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x13, 0x2d, 0x1f, 0x28, 0x2d, 0x3d, 0x1f, 0x1c,
		0x05, 0x34, 0x15, 0x00, 0x00, 0x00,
	},
	{
		0x14, 0x02, 0x07, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x0c, 0x2d, 0x0d, 0x34, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x04, 0x18, 0x30, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1f, 0x0b, 0x0c, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x1b, 0x15, 0x1c, 0x34, 0x15, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
};

char STD_D_8007A2DC[] = "\\STDDAT\\T_TOGI.TMD";

char STD_D_8007A2F0[] = "\\STDDAT\\B_TOGI.TMD";

char *STD_D_8007A304[3] = {
	STD_D_8007A2DC,
	STD_D_8007A2F0,
	(void *)0x00000000,
};

char STD_D_8007A310[] = "\\STDDAT\\T_TOGI.TIM";

char STD_D_8007A324[] = "\\STDDAT\\B_TOGI.TIM";

char *STD_D_8007A338[3] = {
	STD_D_8007A310,
	STD_D_8007A324,
	(void *)0x00000000,
};

char STD_D_8007A344[] = "\\STDDAT\\B_TOGI.ATR";

char *STD_D_8007A358[3] = {
	STD_D_8007A344,
	STD_D_8007A344,
	(void *)0x00000000,
};

int16_t STD_D_8007A364[6] = {
	0x270f, 0x270f, 0x03e7, 0x03e7, 0x03e7, 0x03e7,
};

RGB8 STD_D_8007A370[10] = {
	{ 0x80, 0x80, 0x80 },
	{ 0xc8, 0x64, 0x32 },
	{ 0x1e, 0xff, 0x1e },
	{ 0xd0, 0x1e, 0x50 },
	{ 0x1e, 0x80, 0x80 },
	{ 0xc8, 0xc8, 0x00 },
	{ 0x32, 0xb4, 0xc8 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
};

CameraPreset STD_D_8007A390[9] = {
	{ 0x00e0, 0x0140, 0x0000, 0x0000, 0x00c8, 0x05dc },
	{ 0x00e0, 0x0ec0, 0x0000, 0x0000, 0x00c8, 0x05dc },
	{ 0x0fa0, 0x0140, 0x0000, 0x0000, 0x0104, 0x04b0 },
	{ 0x0fa0, 0x0ec0, 0x0000, 0x0000, 0x0104, 0x04b0 },
	{ 0x00e0, 0x0800, 0x0000, 0x0000, 0x0104, 0x05dc },
	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x00c8, 0x05dc },
	{ 0x00e0, 0x0140, 0x0000, 0x0000, 0x0190, 0x0bb8 },
	{ 0x00e0, 0x0ec0, 0x0000, 0x0000, 0x0190, 0x0bb8 },
	{ 0x00e0, 0x0800, 0x0000, 0x0000, 0x0208, 0x0bb8 },
};

int16_t STD_D_8007A3FC[5][3] = {
	{ 0xfe00, 0xfe00, 0xfb50 },
	{ 0x0200, 0xfe00, 0xfb50 },
	{ 0xfe00, 0xfe00, 0x04b0 },
	{ 0x0200, 0xfe00, 0x04b0 },
	{ 0x0bb8, 0xfa24, 0x0bb8 },
};

int32_t STD_D_8007A41C[22] = {
	0xffffffff, 0x00000000, 0x0000001e, 0x0000002d,
	0x0000003c, 0x00000050, 0x00000064, 0x00000078,
	0x0000008c, 0x000000a0, 0x000000b4, 0x000000c8,
	0x000000dc, 0x000000f0, 0x00000104, 0x00000118,
	0x0000012c, 0x00000140, 0x00000154, 0x00000168,
	0x00000000, 0x00000000,
};

int8_t STD_D_8007A474[24] = {
	0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x03, 0x04,
	0x05, 0x06, 0x07, 0x08, 0x0a, 0x0c, 0x0e, 0x10,
	0x12, 0x14, 0x17, 0x1a, 0x18, 0x17, 0x18, 0x1a,
};

int16_t STD_D_8007A48C[8] = {
	0x001b, 0x0001, 0x0001, 0xffe7, 0xffe7, 0xffcd, 0xffcd, 0xffb8,
};

uint8_t STD_D_8007A49C[112] = {
	0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x01,
	0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x03,
	0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x04, 0x01, 0x02, 0x03,
	0x06, 0x03, 0x03, 0x00, 0x03, 0x00, 0x04, 0x02,
	0x01, 0x03, 0x01, 0x03, 0x03, 0x01, 0x00, 0x00,
	0x01, 0x04, 0x02, 0x02, 0x00, 0x02, 0x02, 0x04,
	0x02, 0x04, 0x03, 0x03, 0x04, 0x02, 0x02, 0x00,
	0x02, 0x04, 0x05, 0x03, 0x02, 0x03, 0x03, 0x02,
	0x02, 0x03, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00,
};

uint8_t STD_D_8007A50C[112] = {
	0x00, 0x00, 0x00, 0x03, 0x04, 0x02, 0x06, 0x05,
	0x02, 0x06, 0x05, 0x09, 0x01, 0x07, 0x08, 0x0f,
	0x0f, 0x03, 0x04, 0x02, 0x06, 0x05, 0x02, 0x06,
	0x05, 0x09, 0x01, 0x07, 0x0a, 0x0f, 0x0f, 0x03,
	0x04, 0x02, 0x06, 0x05, 0x02, 0x06, 0x05, 0x09,
	0x01, 0x07, 0x0a, 0x0f, 0x0f, 0x03, 0x04, 0x02,
	0x06, 0x05, 0x02, 0x06, 0x0d, 0x09, 0x01, 0x07,
	0x0b, 0x05, 0x0e, 0x0c, 0x0c, 0x05, 0x0b, 0x06,
	0x01, 0x08, 0x0b, 0x0c, 0x06, 0x03, 0x08, 0x03,
	0x06, 0x09, 0x04, 0x02, 0x01, 0x09, 0x0d, 0x05,
	0x0d, 0x06, 0x03, 0x04, 0x03, 0x06, 0x03, 0x04,
	0x03, 0x06, 0x0c, 0x05, 0x06, 0x05, 0x06, 0x02,
	0x03, 0x0d, 0x03, 0x09, 0x06, 0x03, 0x0c, 0x0d,
	0x06, 0x02, 0x05, 0x06, 0x06, 0x05, 0x05, 0x0c,
};

uint8_t STD_D_8007A57C[16] = {
	0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x02,
	0x00, 0x02, 0x03, 0x03, 0x00, 0x03, 0x02, 0x00,
};

uint8_t STD_D_8007A58C[4][3] = {
	{ 0x32, 0x14, 0x05 },
	{ 0x50, 0x32, 0x0f },
	{ 0x5f, 0x55, 0x32 },
	{ 0x00, 0x00, 0x00 },
};

StdSrcA598 STD_D_8007A598[8] = {
	{
		{ 0xff7f, 0xff7f, 0xff92, 0xff92, 0xffb4, 0xffb4, 0xfff9, 0xfff9 },
		{ 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
	},
	{
		{ 0xffa4, 0xffa4, 0xff92, 0xff92, 0xffb4, 0xffb4, 0xfff9, 0xfff9 },
		{ 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
	},
	{
		{ 0xffc3, 0xffc3, 0xffd5, 0xffd5, 0xffb4, 0xffb4, 0xfff9, 0xfff9 },
		{ 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01 },
	},
	{
		{ 0xffe8, 0xffe8, 0xffd5, 0xffd5, 0xffb4, 0xffb4, 0xfff9, 0xfff9 },
		{ 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01 },
	},
	{
		{ 0x0009, 0x0009, 0x001c, 0x001c, 0x003d, 0x003d, 0xfff9, 0xfff9 },
		{ 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00 },
	},
	{
		{ 0x002e, 0x002e, 0x001c, 0x001c, 0x003d, 0x003d, 0xfff9, 0xfff9 },
		{ 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00 },
	},
	{
		{ 0x004d, 0x004d, 0x005f, 0x005f, 0x003d, 0x003d, 0xfff9, 0xfff9 },
		{ 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	},
	{
		{ 0x0072, 0x0072, 0x005f, 0x005f, 0x003d, 0x003d, 0xfff9, 0xfff9 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	},
};

/* Damage */
char STD_STR_DAMEEJI[] = "ダメージ";

#if defined(VERSION_JP)
char MAIN_D_80134898[] = "おもいっきり";
char MAIN_D_801348A0[] = "おまかせ";
char STD_D_8007A664[] = "ほどほど";
char STD_D_8007A670[] = "はなれる";
char STD_D_8007A67C[] = "ガマンする";
char MAIN_D_801348A8[] = "ターゲットをかえる";
#else
char STD_D_8007A664[] = "Moderate";

char STD_D_8007A670[] = "Distance";

char STD_D_8007A67C[] = "Defensive";
#endif

char *STD_D_8007A688[8] = {
	MAIN_D_80134894,
	MAIN_D_80134898,
	MAIN_D_801348A0,
	STD_D_8007A664,
	STD_D_8007A670,
	STD_D_8007A67C,
	MAIN_D_801348A8,
	(void *)0x00000000,
};

uint8_t STD_D_8007A6A8[8][10] = {
	{ 0x00, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x04, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x03, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x04, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x03, 0xff },
};

uint8_t STD_D_8007A6F8[16] = {
	0x00, 0xc0, 0x20, 0xc0, 0x40, 0xc0, 0x60, 0xc0,
	0x80, 0xc0, 0xa0, 0xc0, 0xc0, 0xc0, 0x00, 0x00,
};

uint8_t STD_D_8007A708[16] = {
	0x00, 0xd0, 0x20, 0xd0, 0x40, 0xd0, 0x60, 0xd0,
	0x80, 0xd0, 0xa0, 0xd0, 0xc0, 0xd0, 0x00, 0x00,
};
// clang-format on

void STD_func_80060998(void)
{
	int32_t i;
	Entity *entity;

	if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]]->type == ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->type) {
		STD_func_80060AA0();
	}
	STD_removeAllStunEffects();
	STD_func_800791E0();
	STD_removeAllPoisonEffects();
	STD_func_80079874();

	for (i = 0; i <= ENEMY_COUNT; ++i) {
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		STD_removeMoveEffect((DigimonEntity *)entity, &COMBAT_DATA_PTR->fighter[i]);
	}

	STD_unloadAllEFESlots();
	STD_removeEFEEngine();
}

void STD_func_80058494(int16_t which)
{
	int16_t x;

	if (which == 1) {
		x = 0x708;
	} else {
		x = 0x9c4;
	}
	setEntityPosition(1, x, 0, 0);
	setEntityRotation(1, 0, 0x400, 0);
	startAnimation(ENTITY_TABLE[1], 0x21);
}

int16_t STD_func_80058634(Entity *entity)
{
	int32_t i;

	for (i = 0; i < 10; i++) {
		if (ENTITY_TABLE[i] == entity) {
			return i;
		}
	}
}

void STD_func_80058504(int16_t which)
{
	int16_t id;
	int16_t x;

	if (which == 1) {
		x = -0x708;
	} else {
		x = -0x9c4;
	}
	id = STD_func_80058634(MAIN_D_801350E8);
	setEntityPosition(id, x, 0, 0);
	setEntityRotation(id, 0, 0xc00, 0);
	startAnimation(MAIN_D_801350E8, 0x21);
}

void STD_func_8005858C(void)
{
	int32_t i;

	clearTextArea();
	drawString(STD_STR_HP, 0, 0);
	drawString(STD_STR_MP, 0, 12);

	for (i = 2; i < 6; i++) {
		drawString(MAIN_D_80124C0C[i], 0, i * 12);
		DrawSync(0);
	}

	drawString(MAIN_D_80124C54, 0, 0xf0);
}

// clang-format off
void STD_func_80058684(entity, id)
	Entity *entity;
	int16_t id;
// clang-format on
{
	int32_t len;

	MAIN_D_80135138 = 4;
	MAIN_D_8013513A = 0;

	switch (DIGIMON_DATA[entity->type].special[0]) {
	case 0:
		MAIN_D_8013513C = 3;
		break;
	case 1:
		MAIN_D_8013513C = 1;
		break;
	case 2:
		MAIN_D_8013513C = 6;
		break;
	case 3:
		MAIN_D_8013513C = 2;
		break;
	case 4:
		MAIN_D_8013513C = 4;
		break;
	case 5:
		MAIN_D_8013513C = 0;
		break;
	case 6:
		MAIN_D_8013513C = 5;
		break;
	default:
		MAIN_D_8013513C = 0;
		break;
	}

	len = strlen(DIGIMON_DATA[entity->type].name) / 2;
	if (entity->type == 0x4e || entity->type == 0x3c) {
		len = 10;
	}

	MAIN_D_8013513E = -(len * 16);
	MAIN_D_80135140 = 68;
	addObject(0x1ab, id, STD_func_80059524, STD_func_80059658);
}

void STD_func_800587F0(Entity *entity)
{
	if (MAIN_D_80135144.timer != -1) {
		entity->posData->location = STD_D_8007B6F4;
		entity->anim.locX = STD_D_8007B6F4.vx << 15;
		entity->anim.locY = STD_D_8007B6F4.vy << 15;
		entity->anim.locZ = STD_D_8007B6F4.vz << 15;
		startAnimation(entity, 0x21);
		MAIN_D_80135144.timer = -1;
	}
}

// clang-format off
void STD_func_80058898(i)
	int16_t i;
// clang-format on
{
	removeObject(0x1ab, i);
}

// clang-format off
void STD_func_800588A4(i)
	int16_t i;
// clang-format on
{
	if (MAIN_D_80135134 != 0) {
		MAIN_D_80135134 = 0;
		removeObject(0x1a9, i);
	}
}

// clang-format off
void STD_func_800588D4(entity, id)
	Entity *entity;
	int16_t id;
// clang-format on
{
	if (MAIN_D_80135134 != 1) {
		MAIN_D_80135134 = 1;
		STD_D_8007B9BC[0] = -100;
		STD_D_8007B9BC[1] = -100;
		STD_D_8007B9BC[2] = -10;
		STD_D_8007B9BC[3] = -10;
		STD_D_8007B9BC[4] = -10;
		STD_D_8007B9BC[5] = -10;
		addObject(0x1a9, id, STD_func_80059080, STD_func_80059204);
	}
}

void STD_func_80058958(int32_t idx, int16_t value)
{
	POLY_F4 *prim;
	int16_t width;

	prim = (POLY_F4 *)GsGetWorkBase();
	SetPolyF4(prim);
	setRGB0(prim, 0x50, 0xc8, 0x50);
	width = (value * 100) / STD_D_8007A364[idx];
	if (width == 0) {
		width = 1;
	}
	setXYWH(prim, -0x32, idx * 16 - 0x1a, width, 8);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[3], prim++);
	GsSetWorkBase((PACKET *)prim);
}

GARBAGE(STD_renderCommandMenu, 17);

void STD_renderCommandMenu(uint8_t id)
{
	POLY_FT4 *prim;
	int32_t i;
	int16_t rowY;
	int16_t width;
	int16_t base;
	int16_t x;
	int16_t count;

	base = (id * 0xa6) - 0x8c + ((COMBAT_DATA_PTR->player.numCommands[id] - 1) * 0xe);
	if (GAME_STATE == 4) {
		STD_func_80068388(id);
	}

	prim = (POLY_FT4 *)GsGetWorkBase();
	count = COMBAT_DATA_PTR->player.numCommands[id] - 1;

	if (GAME_STATE == 4) {
		x = base - (COMBAT_DATA_PTR->player.hoveredCommand[id] * 0xe);
		SetPolyFT4(prim);
		prim->tpage = getTPage(0, 0, 960, 256);
		setClut(prim, 282, 497);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVDataPolyFT4(prim, 0x3d, 0xe0, 0x16, 0x16);
		if ((count % 2) == 0) {
			int32_t y;

			if ((COMBAT_DATA_PTR->player.hoveredCommand[id] % 2) == 0) {
				y = MAIN_D_8013517C[id];
			} else {
				y = MAIN_D_8013517C[id] + 0xa;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		} else {
			int32_t y;

			if ((COMBAT_DATA_PTR->player.hoveredCommand[id] % 2) == 1) {
				y = MAIN_D_8013517C[id];
			} else {
				y = MAIN_D_8013517C[id] + 0xa;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	for (i = 1; i < COMBAT_DATA_PTR->player.numCommands[id]; i++) {
		SetPolyFT4(prim);
		prim->tpage = getTPage(0, 0, 960, 256);
		setClut(prim, 272, 496);
		setRGB0(prim, 0x80, 0x80, 0x80);
		STD_setCommandIconUV((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]], prim, COMBAT_DATA_PTR->player.availableCommands[id][i]);
		if (i > 0) {
			rowY = ((i - 1) * 0xe) + 0xb;
		} else {
			rowY = 0;
		}
		if ((i == 0) || (i == (COMBAT_DATA_PTR->player.numCommands[id] - 1))) {
			width = 0xb;
		} else {
			width = 0xe;
		}
		x = (int16_t)base - (i * 0xe);
		if ((count % 2) == 0) {
			setXY4(prim, x, ((i % 2) == 0) ? MAIN_D_8013517C[id] + 3 : MAIN_D_8013517C[id] + 0xd, x + 0x10, ((i % 2) == 0) ? MAIN_D_8013517C[id] + 3 : MAIN_D_8013517C[id] + 0xd, x, ((i % 2) == 0) ? MAIN_D_80135180[id] - 0xd : MAIN_D_80135180[id] - 3, x + 0x10, ((i % 2) == 0) ? MAIN_D_80135180[id] - 0xd : MAIN_D_80135180[id] - 3);
		} else {
			setXY4(prim, x, ((i % 2) == 1) ? MAIN_D_8013517C[id] + 3 : MAIN_D_8013517C[id] + 0xd, x + 0x10, ((i % 2) == 1) ? MAIN_D_8013517C[id] + 3 : MAIN_D_8013517C[id] + 0xd, x, ((i % 2) == 1) ? MAIN_D_80135180[id] - 0xd : MAIN_D_80135180[id] - 3, x + 0x10, ((i % 2) == 1) ? MAIN_D_80135180[id] - 0xd : MAIN_D_80135180[id] - 3);
		}
		if ((i == COMBAT_DATA_PTR->player.hoveredCommand[id]) && (MAIN_D_80135184[id] == 1)) {
			prim->u0 += 0x10;
			prim->u1 += 0x10;
			prim->u2 += 0x10;
			prim->u3 += 0x10;
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	for (i = 0; i < COMBAT_DATA_PTR->player.numCommands[id]; i++) {
		SetPolyFT4(prim);
		setSemiTrans(prim, 1);
		setTPage(prim, 0, 0, 960, 256);
		setClut(prim, 272, 497);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVWH(prim, MAIN_D_801348B0[STD_D_8007A6A8[MAIN_D_80135188[id]][i]], 0xe0, MAIN_D_801348B8[STD_D_8007A6A8[MAIN_D_80135188[id]][i]], 31);
		if (i > 0) {
			rowY = ((i - 1) * 0xe) + 0xb;
		} else {
			rowY = 0;
		}
		if ((i == 0) || (i == (COMBAT_DATA_PTR->player.numCommands[id] - 1))) {
			width = 0xb;
		} else {
			width = 0xe;
		}
		setXY4(prim, (rowY - 0x8f) + id * 0xa6, MAIN_D_8013517C[id], ((rowY - 0x8f) + width) + id * 0xa6, MAIN_D_8013517C[id], (rowY - 0x8f) + id * 0xa6, MAIN_D_80135180[id], ((rowY - 0x8f) + width) + id * 0xa6, MAIN_D_80135180[id]);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void STD_func_80058A60(int16_t x, int16_t y, int16_t size, uint8_t character)
{
	POLY_GT4 *prim;
	uint8_t u;
	uint8_t v;

	prim = (POLY_GT4 *)GsGetWorkBase();

	SetPolyGT4(prim);
	prim->tpage = getTPage(0, 0, 768, 0);
	setClut(prim, 0, 480);
	setRGB0(prim, STD_D_8007A370[MAIN_D_8013513C].r, STD_D_8007A370[MAIN_D_8013513C].g, STD_D_8007A370[MAIN_D_8013513C].b);
	setRGB1(prim, STD_D_8007A370[MAIN_D_8013513C].r, STD_D_8007A370[MAIN_D_8013513C].g, STD_D_8007A370[MAIN_D_8013513C].b);
	setRGB2(prim, STD_D_8007A370[MAIN_D_8013513C].r / 10, STD_D_8007A370[MAIN_D_8013513C].g / 10, STD_D_8007A370[MAIN_D_8013513C].b / 10);
	setRGB3(prim, STD_D_8007A370[MAIN_D_8013513C].r / 10, STD_D_8007A370[MAIN_D_8013513C].g / 10, STD_D_8007A370[MAIN_D_8013513C].b / 10);

	u = (character % 32) * 32;
	v = (character / 8) * 32;

	if (size < 64) {
		setUVWH(prim, u, v, (u != 0xe0 ? 32 : 31), (v != 0xe0 ? 32 : 31));
	} else {
		setUVWH(prim, u, v, 31, 31);
	}

	setXYWH(prim, x, y, size, size);
#if defined(VERSION_JP)
	AddPrim(ACTIVE_ORDERING_TABLE->org + 3, prim++);
#endif

	GsSetWorkBase((PACKET *)prim);
}

void STD_func_80058E28(int16_t which)
{
	int32_t dist;
#if defined(VERSION_JP)
	int16_t id;
#else
	int32_t id;
	int16_t idn;
#endif
	uint32_t pad;
	uint32_t prev;

	if (which == 1) {
		dist = 0x514;
	} else {
		dist = 0x7d0;
	}

	((DigimonEntity *)MAIN_D_801350E8)->stats.current.vabId = 5;
	MAIN_D_80135108 = readVBALLSection(5, MAIN_D_801350E8->type);
	STD_func_80058494(which);
	STD_func_80058504(which);
	STD_func_8005858C();
	STD_startCameraChase(ENTITY_TABLE[1], dist, 0);
#if defined(VERSION_JP)
	id = STD_func_80058634(ENTITY_TABLE[1]);
#else
	idn = STD_func_80058634(ENTITY_TABLE[1]);
	id = idn;
#endif
	STD_func_80058684(ENTITY_TABLE[1], id);
	stopBGM();
	stopSound();
	playMusic(MAIN_D_801350F8, 0);

	prev = 0;
	while ((ENTITY_TABLE[1]->anim.animFlag & 1) != 0) {
		pad = PadRead(1);
		STD_battleTickFrame();
		if (((pad & ~prev) & CONFIRM_BUTTON) != 0) {
			STD_func_800587F0(ENTITY_TABLE[1]);
			prev = pad;
			break;
		}
		prev = pad;
	}

	STD_func_80058898(id);
	STD_func_800588A4(id);
	removeObject(0x1aa, 0);
	stopBGM();
	stopSound();
	isSoundLoaded(0, MAIN_D_80135108);
	MAIN_D_80135108 = loadSB();

	STD_startCameraChase(MAIN_D_801350E8, -dist, 1);
#if defined(VERSION_JP)
	id = STD_func_80058634(MAIN_D_801350E8);
#else
	idn = STD_func_80058634(MAIN_D_801350E8);
	id = idn;
#endif
	STD_func_80058684(MAIN_D_801350E8, id);
	playMusic(MAIN_D_801350F8, 1);

	while ((MAIN_D_801350E8->anim.animFlag & 1) != 0) {
		pad = PadRead(1);
		STD_battleTickFrame();
		if (((pad & ~prev) & CONFIRM_BUTTON) != 0) {
			STD_func_800587F0(MAIN_D_801350E8);
			prev = pad;
			break;
		}
		prev = pad;
	}

	STD_func_80058898(id);
	STD_func_800588A4(id);
	removeObject(0x1aa, 0);
	stopBGM();
	stopSound();
	isSoundLoaded(0, MAIN_D_80135108);
}

// clang-format off
void STD_func_80059080(id)
	int16_t id;
// clang-format on
{
	Stats *stats;

	STD_D_8007B9BC[0] += 200;
	STD_D_8007B9BC[1] += 200;
	STD_D_8007B9BC[2] += 20;
	STD_D_8007B9BC[3] += 20;
	STD_D_8007B9BC[4] += 20;
	STD_D_8007B9BC[5] += 20;

	stats = &((DigimonEntity *)ENTITY_TABLE[id])->stats;
	if (stats->current.currentHP < STD_D_8007B9BC[0]) {
		STD_D_8007B9BC[0] = stats->current.currentHP;
	}

	if (stats->current.currentMP < STD_D_8007B9BC[1]) {
		STD_D_8007B9BC[1] = stats->current.currentMP;
	}

	if (stats->base.off < STD_D_8007B9BC[2]) {
		STD_D_8007B9BC[2] = stats->base.off;
	}

	if (stats->base.def < STD_D_8007B9BC[3]) {
		STD_D_8007B9BC[3] = stats->base.def;
	}

	if (stats->base.speed < STD_D_8007B9BC[4]) {
		STD_D_8007B9BC[4] = stats->base.speed;
	}

	if (stats->base.brain < STD_D_8007B9BC[5]) {
		STD_D_8007B9BC[5] = stats->base.brain;
	}
}

// clang-format off
void STD_func_80059204(id)
	int16_t id;
// clang-format on
{
	Stats *stats;
	int32_t i;

	for (i = 0; i < 6; ++i) {
		renderString(0, -100, i * 16 - 28, 48, 12, 0, i * 12, 0, 1);
		STD_func_80058958((int16_t)i, STD_D_8007B9BC[i]);
	}

	stats = &((DigimonEntity *)ENTITY_TABLE[id])->stats;
	if (STD_D_8007B9BC[0] != stats->current.currentHP || STD_D_8007B9BC[1] != stats->current.currentMP || STD_D_8007B9BC[2] != stats->base.off || STD_D_8007B9BC[3] != stats->base.def || STD_D_8007B9BC[4] != stats->base.speed || STD_D_8007B9BC[5] != stats->base.brain) {
		playSound(0, 0x16);
	} else {
		for (i = 0; i < 6; ++i) {
			STD_func_800593D0(52, (int16_t)(i * 16 - 28), 4, STD_D_8007B9BC[i], 3);
		}
	}
}

// clang-format off
void STD_func_800593D0(x, y, digits, value, layer)
	int16_t x;
	int16_t y;
	int16_t digits;
	int32_t value;
	int32_t layer;
// clang-format on
{
	POLY_FT4 *prim;
	int32_t i;
	int32_t count;
	int32_t buf[6];

	prim = (POLY_FT4 *)GsGetWorkBase();

	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 832, 0);
		setClut(prim, 16, 480);
		setUVDataPolyFT4(prim, buf[i] * 12, 32, 12, 12);
		setPosDataPolyFT4(prim, x + ((digits - 1) - i) * 12, y, 12, 12);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void STD_func_80059524(int32_t id)
{
	int32_t len;

	++MAIN_D_80135138;

	len = strlen(DIGIMON_DATA[ENTITY_TABLE[id]->type].name) / 2;
	if (ENTITY_TABLE[id]->type == 0x4e || ENTITY_TABLE[id]->type == 0x3c) {
		len = 10;
	}

	if (len == MAIN_D_8013513A && MAIN_D_80135142 == 3) {
		if (MAIN_D_80135144.timer == 0) {
			startAnimation(ENTITY_TABLE[id], 0x23);
			MAIN_D_80135144.timer = 20;
		}

		if (MAIN_D_80135140 >= -71) {
			MAIN_D_80135140 -= 28;
		} else {
			STD_func_800588D4(ENTITY_TABLE[id], id);
		}
	}
}

// clang-format off
void STD_func_80059658(id)
	int16_t id;
// clang-format on
{
	int32_t charCount;
	int32_t i;
	int32_t charIndex;
	int16_t y;
	int16_t size;
	uint8_t character;

	charCount = strlen(DIGIMON_DATA[ENTITY_TABLE[id]->type].name) / 2;
	if (ENTITY_TABLE[id]->type == 0x4e || ENTITY_TABLE[id]->type == 0x3c) {
		charCount = 10;
	}

	if (MAIN_D_80135138 % 4 == 0) {
		if (MAIN_D_8013513A < charCount) {
			++MAIN_D_8013513A;
			MAIN_D_80135142 = 0;
		}
	} else if (MAIN_D_80135142 != 3) {
		++MAIN_D_80135142;
	}

	charIndex = 0;
	for (i = 0; i < MAIN_D_8013513A; ++i) {
		character = STD_D_80079CBC[ENTITY_TABLE[id]->type][charIndex++];
		if (character == 0x3d) {
			character = STD_D_80079CBC[ENTITY_TABLE[id]->type][charIndex++];
		}

		if (i == MAIN_D_8013513A - 1) {
			y = MAIN_D_80135140 - MAIN_D_80134804[MAIN_D_80135142];
			size = MAIN_D_80134800[MAIN_D_80135142];
		} else {
			y = MAIN_D_80135140;
			size = 32;
		}

		STD_func_80058A60((int16_t)MAIN_D_8013513E + i * 32, y, size, character);

		if (character == 0x1f || character == 0x25) {
			character = STD_D_80079CBC[ENTITY_TABLE[id]->type][charIndex++];
			STD_func_80058A60((int16_t)MAIN_D_8013513E + i * 32, y, size, character);
		}
	}
}

void STD_func_80059908(void)
{
	if (MAIN_D_80135124 & 0x1000) {
		STDVS_VIEW_TRANSLATION.vy += 0x14;
	}
	if (MAIN_D_80135124 & 0x4000) {
		STDVS_VIEW_TRANSLATION.vy -= 0x14;
	}
	if (MAIN_D_80135124 & 0x8000) {
		STDVS_VIEW_TRANSLATION.vx += 0x14;
	}
	if (MAIN_D_80135124 & 0x2000) {
		STDVS_VIEW_TRANSLATION.vx -= 0x14;
	}
	if (MAIN_D_80135124 & 0x4) {
		STDVS_VIEW_TRANSLATION.vz -= 0x14;
	}
	if (MAIN_D_80135124 & 0x1) {
		STDVS_VIEW_TRANSLATION.vz += 0x14;
	}
	if (MAIN_D_80135124 & ALT_BUTTON) {
		STDVS_VIEW_ROTATION[0].vx += 0x20;
		STDVS_VIEW_ROTATION[0].vx &= 0xfff;
	}
	if (MAIN_D_80135124 & CANCEL_BUTTON) {
		STDVS_VIEW_ROTATION[0].vx -= 0x20;
		STDVS_VIEW_ROTATION[0].vx &= 0xfff;
	}
	if (MAIN_D_80135124 & 0x80) {
		STDVS_VIEW_ROTATION[0].vy -= 0x20;
		STDVS_VIEW_ROTATION[0].vy &= 0xfff;
	}
	if (MAIN_D_80135124 & CONFIRM_BUTTON) {
		STDVS_VIEW_ROTATION[0].vy += 0x20;
		STDVS_VIEW_ROTATION[0].vy &= 0xfff;
	}
	if (MAIN_D_80135124 & 0x800) {
		STDVS_VIEW_ROTATION[0].vx = 0;
		STDVS_VIEW_ROTATION[0].vy = 0;
		STDVS_VIEW_ROTATION[0].vz = 0;
		STDVS_VIEW_TRANSLATION.vx = 0;
		STDVS_VIEW_TRANSLATION.vy = 0;
		STDVS_VIEW_TRANSLATION.vz = 0xbb8;
	}
	STDVS_VIEW.super = NULL;
	RotMatrix(STDVS_VIEW_ROTATION, &STDVS_VIEW.view);
	TransMatrix(&STDVS_VIEW.view, &STDVS_VIEW_TRANSLATION);
	MAIN_D_801B1BBC.flg = 0;
	GsSetView2(&STDVS_VIEW);
}

void STD_func_80059B70(void)
{
	VECTOR *a;
	VECTOR *b;
	VECTOR diff;
	int32_t dist;
	int32_t d;
	int32_t ang;

	a = &MAIN_D_801350E8->posData->location;
	b = &PARTNER_ENTITY.digimonEntity.entity.posData->location;
	diff.vx = a->vx - b->vx;
	diff.vy = 0;
	diff.vz = a->vz - b->vz;
	dist = SquareRoot0(diff.vx * diff.vx + diff.vz * diff.vz);
	d = (dist * VIEWPORT_DISTANCE) / 200u;
	if (d < (int16_t)MAIN_D_8013511C) {
		d = MAIN_D_8013511C;
	}
	if (d < (int16_t)MAIN_D_8013511C + 0x12c) {
		MAIN_D_8013510C = 1;
	} else {
		MAIN_D_8013510C = 0;
	}
	if (d > 0x1068) {
		d = 0x1068;
	}
	ang = 0x1000 - _atan(diff.vx, diff.vz);
	STDVS_VIEW_TRANSLATION.vx = b->vx + (int32_t)diff.vx / 2 + (d * diff.vz) / dist;
	STDVS_VIEW_TRANSLATION.vy = -0x3e8;
	STDVS_VIEW_TRANSLATION.vz = b->vz + diff.vz / 2 - (d * diff.vx) / dist;
	STDVS_VIEW_ROTATION[0].vx = _atan(d, -0x2bc) + 0x800;
	STDVS_VIEW_ROTATION[0].vy = ang + 0x800;
	STDVS_VIEW_ROTATION[0].vz = 0;
	STDVS_VIEW.view = GsIDMATRIX;
	STDVS_VIEW.super = &MAIN_D_801B1BBC;
	RotMatrixYXZ(STDVS_VIEW_ROTATION, &MAIN_D_801B1BBC.coord);
	TransMatrix(&MAIN_D_801B1BBC.coord, &STDVS_VIEW_TRANSLATION);
	MAIN_D_801B1BBC.flg = 0;
	GsSetView2(&STDVS_VIEW);
}

void STD_func_80059DBC(void)
{
	VECTOR *a;
	VECTOR *b;

	a = &PARTNER_ENTITY.digimonEntity.entity.posData->location;
	b = &MAIN_D_801350E8->posData->location;
	STDVS_VIEW.view = GsIDMATRIX;
	STDVS_VIEW.super = &MAIN_D_801B1BBC;
	STDVS_VIEW_TRANSLATION.vx = a->vx + (b->vx - a->vx) / 2;
	STDVS_VIEW_TRANSLATION.vz = a->vz + (b->vz - a->vz) / 2;
	STDVS_VIEW_TRANSLATION.vy = -0x1f40;
	STDVS_VIEW_ROTATION[0].vy = STDVS_VIEW_ROTATION[0].vz = 0;
	STDVS_VIEW_ROTATION[0].vx = -0x400;
	RotMatrixYXZ(STDVS_VIEW_ROTATION, &MAIN_D_801B1BBC.coord);
	TransMatrix(&MAIN_D_801B1BBC.coord, &STDVS_VIEW_TRANSLATION);
	MAIN_D_801B1BBC.flg = 0;
	GsSetView2(&STDVS_VIEW);
}

void STD_setCameraToEntity(void)
{
	SVECTOR rot;
	VECTOR v;
	VECTOR out;

	rot = STDVS_VIEW_ROTATION[0];
	rot.vy -= ((Entity *)MAIN_D_80135128)->posData->rotation.vy;
	rot.vy &= 0xfff;
	RotMatrix(&rot, &STDVS_VIEW.view);
	v = STDVS_VIEW_TRANSLATION;
	ApplyMatrixLV(&STDVS_VIEW.view, &((Entity *)MAIN_D_80135128)->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix(&STDVS_VIEW.view, &v);
	MAIN_D_801B1BBC.flg = 0;
	GsSetView2(&STDVS_VIEW);
}

void STD_func_8005A44C(void)
{
	MAIN_D_80135124 = (POLLED_INPUT >> 16) & 0xffff;
	MAIN_D_80135120 = (POLLED_INPUT_PREVIOUS >> 16) & 0xffff;

	switch (MAIN_D_801350EC) {
	case 0:
		STD_func_80059908();
		break;
	case 1:
		STD_func_80059B70();
		break;
	case 2:
		STD_func_80059DBC();
		break;
	case 3:
	case 5:
		STD_setCameraToEntity();
		break;
	case 4:
	case 6:
		STD_setViewpointRotationFromEntity();
		break;
	case 9:
		STD_applyEntityViewpoint();
		break;
	case 7:
		STD_func_8005A054();
		break;
	case 8:
		STD_applyViewpoint();
		break;
	case 10:
		STD_func_8005A1BC();
		break;
	case 11:
		STD_applyViewpoint();
		break;
	}
}

void STD_setViewpointRotationFromEntity(void)
{
	MATRIX *m;

	m = (MATRIX *)(MAIN_D_80135128[1] + 0xbc);
	GS_VIEWPOINT.vrx = m->t[0];
	GS_VIEWPOINT.vry = m->t[1];
	GS_VIEWPOINT.vrz = m->t[2];
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_func_8005A054(void)
{
	VECTOR v;
	VECTOR out;
	VECTOR diff;
	VECTOR *selfPos;
	VECTOR *otherPos;
	Entity *other;

	selfPos = &((Entity *)MAIN_D_80135128)->posData->location;
	if ((Entity *)MAIN_D_80135128 == ENTITY_TABLE[1]) {
		other = MAIN_D_801350E8;
	} else {
		other = ENTITY_TABLE[1];
	}
	otherPos = &other->posData->location;
	diff.vx = otherPos->vx - selfPos->vx;
	diff.vy = 0;
	diff.vz = otherPos->vz - selfPos->vz;
	STDVS_VIEW_ROTATION[0].vy = (-_atan(diff.vz, diff.vx) + 0x800) & 0xfff;
	RotMatrix(STDVS_VIEW_ROTATION, &STDVS_VIEW.view);
	v = STDVS_VIEW_TRANSLATION;
	ApplyMatrixLV(&STDVS_VIEW.view, &((Entity *)MAIN_D_80135128)->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix(&STDVS_VIEW.view, &v);
	GsSetView2(&STDVS_VIEW);
}

void STD_applyViewpoint(void)
{
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_func_8005A1BC(void)
{
	if (MAIN_D_80135124 & 0x1000) {
		STDVS_VIEW_TRANSLATION.vy += 0x14;
	}
	if (MAIN_D_80135124 & 0x4000) {
		STDVS_VIEW_TRANSLATION.vy -= 0x14;
	}
	if (MAIN_D_80135124 & 0x8000) {
		STDVS_VIEW_TRANSLATION.vx += 0x14;
	}
	if (MAIN_D_80135124 & 0x2000) {
		STDVS_VIEW_TRANSLATION.vx -= 0x14;
	}
	if (MAIN_D_80135124 & 0x4) {
		STDVS_VIEW_TRANSLATION.vz -= 0x14;
	}
	if (MAIN_D_80135124 & 0x1) {
		STDVS_VIEW_TRANSLATION.vz += 0x14;
	}
	if (MAIN_D_80135124 & ALT_BUTTON) {
		STDVS_VIEW_ROTATION[0].vx += 0x20;
		STDVS_VIEW_ROTATION[0].vx &= 0xfff;
	}
	if (MAIN_D_80135124 & CANCEL_BUTTON) {
		STDVS_VIEW_ROTATION[0].vx -= 0x20;
		STDVS_VIEW_ROTATION[0].vx &= 0xfff;
	}
	STDVS_VIEW_ROTATION[0].vy += 2;
	STDVS_VIEW_ROTATION[0].vy &= 0xfff;
	if (MAIN_D_80135124 & CONFIRM_BUTTON) {
		STDVS_VIEW_ROTATION[0].vy += 0x20;
		STDVS_VIEW_ROTATION[0].vy &= 0xfff;
	}
	STDVS_VIEW.super = NULL;
	RotMatrix(STDVS_VIEW_ROTATION, &STDVS_VIEW.view);
	TransMatrix(&STDVS_VIEW.view, &STDVS_VIEW_TRANSLATION);
	MAIN_D_801B1BBC.flg = 0;
	GsSetView2(&STDVS_VIEW);
}

void STD_setCameraParams(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f)
{
	STDVS_VIEW_ROTATION[0].vx = a;
	STDVS_VIEW_ROTATION[0].vy = b;
	STDVS_VIEW_ROTATION[0].vz = c;
	STDVS_VIEW_TRANSLATION.vx = d;
	STDVS_VIEW_TRANSLATION.vy = e;
	STDVS_VIEW_TRANSLATION.vz = f;
}

void STD_setVSPhase(int32_t arg)
{
	addObject(0x1a8, 0, (TickFunction)STD_func_8005A44C, NULL);
	MAIN_D_801350EC = arg;
	MAIN_D_8013512C = 0;
}

void STD_func_8005A550(void)
{
	removeObject(0x1a8, 0);
}

void STD_func_8005A55C(DigimonEntity *entity, int32_t mode, uint8_t sub)
{
	CameraPreset *p;

	if (mode != 5) {
		if (randomLimit(3) != 0) {
			return;
		}
	}
	MAIN_D_80135128 = (char **)entity;
	STDVS_VIEW.super = NULL;
	if (sub != 3) {
		p = &STD_D_8007A390[mode];
	} else {
		p = &STD_D_8007A390[randomLimit(3) + 6];
	}
	STD_setCameraParams(p->unk0, p->unk2, p->unk4, p->unk6, p->unk8, p->unkA);
	if (mode < 5) {
		MAIN_D_801350EC = 3;
		return;
	}
	if ((Entity *)entity != ENTITY_TABLE[1]) {
		STD_func_8006CCE0(1);
	} else {
		STD_func_8006C6DC();
	}
	MAIN_D_80135128 = (char **)ENTITY_TABLE[1];
	STD_func_8005B688(ENTITY_TABLE[1], MAIN_D_801350E8);
}

void STD_setRandomViewpoint(Entity *entity, int32_t idx)
{
	VECTOR v;
	VECTOR out;
	MATRIX m;

	if (randomLimit(3) != 0) {
		return;
	}
	if (MAIN_D_801350EC == 7) {
		return;
	}

	VIEWPORT_DISTANCE = 0x1f4;
	GS_VIEWPOINT.super = NULL;

	if (idx < 4) {
		MAIN_D_80135128 = (char **)entity;
		MAIN_D_801350EC = 4;
		RotMatrix(&((Entity *)MAIN_D_80135128)->posData->rotation, &m);
		v.vx = STD_D_8007A3FC[idx][0];
		v.vy = STD_D_8007A3FC[idx][1];
		v.vz = STD_D_8007A3FC[idx][2];
		ApplyMatrixLV(&m, &v, &out);
		out.vx += ((Entity *)MAIN_D_80135128)->posData->location.vx;
		out.vz += ((Entity *)MAIN_D_80135128)->posData->location.vz;
		GS_VIEWPOINT.vpx = out.vx;
		GS_VIEWPOINT.vpy = out.vy;
		GS_VIEWPOINT.vpz = out.vz;
	} else {
		MAIN_D_801350EC = 6;
		GS_VIEWPOINT.vpx = STD_D_8007A3FC[idx][0];
		GS_VIEWPOINT.vpy = STD_D_8007A3FC[idx][1];
		GS_VIEWPOINT.vpz = STD_D_8007A3FC[idx][2];
	}

	GS_VIEWPOINT.rz = 0;
}

void STD_func_8005A830(void)
{
	MAIN_D_801350EC = 0xb;
	GS_VIEWPOINT.vpx = 0;
	GS_VIEWPOINT.vpy = -0xc8;
	GS_VIEWPOINT.vpz = -((DIGIMON_DATA[ENTITY_TABLE[1]->type].radius * 2) + 0x320);
	GS_VIEWPOINT.vrx = 0;
	GS_VIEWPOINT.vry = (-DIGIMON_DATA[ENTITY_TABLE[1]->type].height * 2) / 3;
	GS_VIEWPOINT.vrz = 0;
	GS_VIEWPOINT.rz = 0;
	VIEWPORT_DISTANCE = 0x200;
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

int32_t STD_getFighterDistance(VECTOR *self, VECTOR *other, VECTOR *target)
{
	int32_t toTarget;
	int32_t toOther;

	toTarget = getDistance(self->vx - target->vx, self->vy - target->vy, self->vz - target->vz);
	toOther = getDistance(other->vx - self->vx, other->vy - self->vy, other->vz - self->vz);
	return (toTarget * 100) / toOther;
}

void STD_setViewpointFromBone(Entity *entity, SVECTOR *offset, SVECTOR *rot, int32_t dist)
{
	MATRIX m1;
	SVECTOR out1;
	MATRIX m2;
	SVECTOR bone;
	MATRIX m3;
	VECTOR v;

	calculateBoneMatrix(entity, offset->pad, &m1);
	ApplyMatrixSV(&m1, offset, &out1);
	GS_VIEWPOINT.vrx = m1.t[0] + out1.vx;
	GS_VIEWPOINT.vry = m1.t[1] + out1.vy;
	GS_VIEWPOINT.vrz = m1.t[2] + out1.vz;
	bone = MAIN_D_80134868;
	calculateBoneMatrix(entity, 1, &m2);
	ApplyMatrixSV(&m2, &bone, &bone);
	GS_VIEWPOINT.vry = m2.t[1] + bone.vy;
	RotMatrixZYX(rot, &m3);
	v.vx = 0;
	v.vy = 0;
	v.vz = dist;
	ApplyMatrixLV(&m3, &v, (VECTOR *)&GS_VIEWPOINT);
	GS_VIEWPOINT.vpx += GS_VIEWPOINT.vrx;
	GS_VIEWPOINT.vpy += GS_VIEWPOINT.vry;
	GS_VIEWPOINT.vpz += GS_VIEWPOINT.vrz;
	VIEWPORT_DISTANCE = 0x15e;
}

void STD_updateCameraLerp(int32_t t, int8_t flip)
{
	SVECTOR off;
	SVECTOR rot;
	int32_t base;
	int32_t dist;
	int32_t dbl;

	off = MAIN_D_80134858;
	rot = MAIN_D_80134860;
	base = ((((DIGIMON_DATA[((Entity *)MAIN_D_80135128)->type].height +
	           DIGIMON_DATA[((Entity *)MAIN_D_80135128)->type].radius) /
	          2) *
	         0x62c) /
	        450);
	dbl = base * 2;
	rot.vx = lerp(0x2aa, 0xe3, 0, 0x64, t);
	rot.vy = lerp(0x5c7, 0xa38, 0, 0x64, t);

	if (rot.vy < 0x801) {
		dist = lerp(base, dbl, 0x5c7, 0x800, rot.vy);
	} else {
		dist = lerp(dbl, base, 0x800, 0xa38, rot.vy);
	}

	if (flip != 0) {
		rot.vy = -rot.vy;
	}

	rot.vy += ((Entity *)MAIN_D_80135128)->posData->rotation.vy;
	off.vy = (-DIGIMON_DATA[((Entity *)MAIN_D_80135128)->type].height * 2) / 3;
	STD_setViewpointFromBone((Entity *)MAIN_D_80135128, &off, &rot, dist);
}

int32_t STD_isPositionNearEntity(Entity *entity, VECTOR *pos)
{
	if (pos->vx - 50 > entity->posData->location.vx) {
		goto no;
	}
	if (entity->posData->location.vx > pos->vx + 50) {
		goto no;
	}
	if (pos->vz - 50 > entity->posData->location.vz) {
		goto no;
	}
	if (entity->posData->location.vz > pos->vz + 50) {
		goto no;
	}

	return 1;
no:
	return 0;
}

int32_t STD_func_8005ADFC(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end)
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

void STD_tickCameraChase(void)
{
	int32_t t;
	SVECTOR off;
	SVECTOR rot;
	int32_t d2;
	CameraChase *cc;
	int32_t dist;
	int32_t i;

	cc = &MAIN_D_80135144;
	if (cc->timer < 0x14) {
		return;
	}
	if (cc->timer < 0x14) {
		goto inc;
	}
	if (cc->timer == 0x14) {
		startAnimation((Entity *)MAIN_D_80135128, 0x23);
	}
	if (cc->phase == 0) {
		dist = STD_getFighterDistance(&STD_D_8007B704, &STD_D_8007B6F4, &((Entity *)MAIN_D_80135128)->posData->location);
		if (dist >= 0x23) {
			STD_D_8007B704 = ((Entity *)MAIN_D_80135128)->posData->location;
			cc->phase = 1;
			MAIN_D_801350EC = 8;
		} else {
			off = MAIN_D_80134818;
			rot = MAIN_D_80134820;
			if (cc->side == 0) {
				rot.vy = lerp(-0x638, -0x293, 0, 0x23, dist);
			} else {
				rot.vy = lerp(0x638, 0x293, 0, 0x23, dist);
			}
			rot.vy += ((Entity *)MAIN_D_80135128)->posData->rotation.vy;
			off.vy = (-DIGIMON_DATA[((Entity *)MAIN_D_80135128)->type].height * 7) / 10;
			d2 = (((DIGIMON_DATA[((Entity *)MAIN_D_80135128)->type].height + DIGIMON_DATA[((Entity *)MAIN_D_80135128)->type].radius) / 2) * 0x5aa) / 450;
			STD_setViewpointFromBone((Entity *)MAIN_D_80135128, &off, &rot, d2);
			GS_VIEWPOINT.vpy = (off.vy * 7) / 10;
			if (GS_VIEWPOINT.vpy > -0xb4) {
				GS_VIEWPOINT.vpy = -0xb4;
			}
			goto inc;
		}
	}
	t = STD_getFighterDistance(&STD_D_8007B704, &STD_D_8007B6F4, &((Entity *)MAIN_D_80135128)->posData->location);
	STD_updateCameraLerp(t, cc->side);
	if (STD_isPositionNearEntity((Entity *)MAIN_D_80135128, &STD_D_8007B6F4) == 1) {
		for (i = 0; i < 3; i++) {
			if (((uint8_t *)MAIN_D_80135128 + i)[0x44] != 0xff) {
				startAnimation((Entity *)MAIN_D_80135128, ((uint8_t *)MAIN_D_80135128 + i)[0x44]);
				break;
			}
		}
		((Entity *)MAIN_D_80135128)->anim.animFlag |= 2;
		cc->timer = -1;
		return;
	}
inc:
	cc->timer++;
}

// clang-format off
void STD_startCameraChase(entity, dx, side)
	Entity *entity;
	int16_t dx;
	int8_t side;
// clang-format on
{
	SVECTOR off;
	SVECTOR rot;
	int32_t dist;

	MAIN_D_80135128 = (char **)entity;
	copyVector(&STD_D_8007B704, &((Entity *)MAIN_D_80135128)->posData->location);
	STD_D_8007B6F4.vx = STD_D_8007B704.vx - dx;
	STD_D_8007B6F4.vy = STD_D_8007B704.vy;
	STD_D_8007B6F4.vz = STD_D_8007B704.vz;
	startAnimation((Entity *)MAIN_D_80135128, 0x21);
	MAIN_D_801350EC = 9;
	MAIN_D_80135144.timer = 0;
	MAIN_D_80135144.phase = 0;
	MAIN_D_80135144.side = side;
	addObject(0x1aa, 0, (TickFunction)STD_tickCameraChase, NULL);
	off = MAIN_D_80134828;
	rot = MAIN_D_80134830;
	if (MAIN_D_80135144.side == 0) {
		rot.vy = -0x638;
	} else {
		rot.vy = 0x638;
	}
	rot.vy += ((Entity *)MAIN_D_80135128)->posData->rotation.vy;
	off.vy = (-DIGIMON_DATA[((Entity *)MAIN_D_80135128)->type].height * 2) / 3;
	dist = (((DIGIMON_DATA[((Entity *)MAIN_D_80135128)->type].height + DIGIMON_DATA[((Entity *)MAIN_D_80135128)->type].radius) / 2) * 0x5aa) / 450;
	STD_setViewpointFromBone((Entity *)MAIN_D_80135128, &off, &rot, dist);
	GS_VIEWPOINT.vpy = (off.vy * 3) / 10;
	if (GS_VIEWPOINT.vpy > -0xb4) {
		GS_VIEWPOINT.vpy = -0xb4;
	}
}

void STD_tickCameraIntro(void)
{
	SVECTOR off;
	SVECTOR rot;
	int16_t *p;

	p = &STD_D_8007B9BC[6];
#if defined(VERSION_JP)
	if ((p[0] >= 0x1e) && (p[0] < 0x3c)) {
#else
	if ((STD_D_8007B9BC[6] >= 0x1e) && (p[0] < 0x3c)) {
#endif
		STD_D_8007B9BC[8] = lerp(STD_D_8007B9BC[8], STD_D_8007B9BC[9], p[0], 0x3c, p[0] + 1);
	}

	if (p[0] >= 0x1e) {
		p[1] += (int16_t)STD_func_8005ADFC(0x1e, 0x3c, p[0], 0, 0x5b);
	}

	off = MAIN_D_80134838;
	rot = MAIN_D_80134840;
	rot.vy = STD_D_8007B9BC[7];
	STD_D_8007B9BC[8] = processSomeArenaArrays(0x16, p[0], STD_D_8007A41C, STD_D_8007B9EC, STD_D_8007BA44);
	STD_setViewpointFromBone(*(Entity **)&p[6], &off, &rot, STD_D_8007B9BC[8]);
	GS_VIEWPOINT.vry = (-DIGIMON_DATA[STD_D_8007B9D0.target->type].height * 2) / 3;
	GS_VIEWPOINT.vpy = -processSomeArenaArrays(0x16, p[0], STD_D_8007A41C, STD_D_8007BA9C, STD_D_8007BA44);
	p[0]++;
}

void STD_func_8005B688(Entity *target, Entity *entity)
{
	SVECTOR off;
	SVECTOR rot;
	SVECTOR delta;
	int32_t d;
	int32_t i;

	STD_D_8007B9BC[6] = 0;
	STD_D_8007B9D0.entity = entity;
	if (target == NULL) {
		if ((Entity *)MAIN_D_80135128 == ENTITY_TABLE[1]) {
			target = MAIN_D_801350E8;
		} else {
			target = ENTITY_TABLE[1];
		}
	}
	STD_D_8007B9D0.target = target;
	MAIN_D_801350EC = 8;
	addObject(0x1ad, 0, (TickFunction)STD_tickCameraIntro, NULL);

	off = MAIN_D_80134848;
	rot = MAIN_D_80134850;
	delta.vx = entity->posData->location.vx - target->posData->location.vx;
	delta.vz = entity->posData->location.vz - target->posData->location.vz;
	STD_D_8007B9BC[7] = (-_atan(delta.vz, delta.vx) + 0x7de) & 0xfff;
	rot.vy = STD_D_8007B9BC[7];
	STD_D_8007B9BC[8] = getDistance(delta.vx, 0, delta.vz);
	STD_D_8007B9BC[8] += 0x2bc;
	if (STD_D_8007B9BC[8] < 0x5dc) {
		STD_D_8007B9BC[8] = 0x5dc;
	}
	STD_setViewpointFromBone(target, &off, &rot, STD_D_8007B9BC[8]);

	STD_D_8007B9BC[9] = DIGIMON_DATA[target->type].radius * 3 * 2;
	GS_VIEWPOINT.vry = (-DIGIMON_DATA[target->type].height * 2) / 3;
	GS_VIEWPOINT.vpy = -DIGIMON_DATA[entity->type].height;
	STD_D_8007B9D0.unk0 = GS_VIEWPOINT.vpy;
	STD_D_8007B9D0.unk2 = GS_VIEWPOINT.vpy * 250 / 100;
	STD_D_8007BA9C[0] = DIGIMON_DATA[entity->type].height;
	STD_D_8007BA9C[1] = DIGIMON_DATA[entity->type].height;
	STD_D_8007BA9C[2] = DIGIMON_DATA[entity->type].height * 200 / 100;
	STD_D_8007BA9C[3] = DIGIMON_DATA[target->type].height * 380 / 100;
	STD_D_8007BAAC[0] = DIGIMON_DATA[target->type].height * 250 / 100;
	for (i = 5; i < 0x16; i++) {
		STD_D_8007BA9C[i] =
			customRandom(0x50, DIGIMON_DATA[target->type].height * 180 / 100);
	}
	initializeSomeArenaArrays(0x16, (int32_t)STD_D_8007A41C, (int32_t)STD_D_8007BA9C, STD_D_8007BA44);

	d = getDistance(delta.vx, 0, delta.vz) + 0x2bc;
	STD_D_8007B9EC[0] = d;
	STD_D_8007B9EC[1] = d;
	if (d < 0x5dc) {
		d = 0x5dc;
	}
	STD_D_8007B9EC[2] = d * 120 / 100;
	STD_D_8007B9EC[3] = d * 80 / 100;
	STD_D_8007B9FC[0] = STD_D_8007B9BC[9];
	for (i = 5; i < 0x16; i++) {
		STD_D_8007B9EC[i] =
			customRandom(STD_D_8007B9BC[9] * 45 * 2 / 100,
		                     STD_D_8007B9BC[9] * 45 * 4 / 100);
	}
}

void STD_removeCameraIntro(void)
{
	removeObject(0x1ad, 0);
	STD_D_8007B9BC[6] = -1;
}

void STD_applyEntityViewpoint(void)
{
	char *p;

	VIEWPORT_DISTANCE = 0x15e;
	GsSetProjection(VIEWPORT_DISTANCE);
	p = MAIN_D_80135128[1] + 0x34;
	GS_VIEWPOINT.vrx = *(int32_t *)(p + 0x14);
	GS_VIEWPOINT.vry = -DIGIMON_DATA[(int32_t)MAIN_D_80135128[0]].height * 2 / 3;
	GS_VIEWPOINT.vrz = *(int32_t *)(p + 0x1c);
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_renderCounterDigits(int16_t x, int16_t y, int16_t digits, int32_t value, int32_t layer)
{
	POLY_FT4 *prim;
	int32_t i;
	int32_t count;
	int32_t buf[6];

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = getTPage(0, 0, 832, 0);
	prim->clut = GetClut(0x10, 0x1e0);
	setUVDataPolyFT4(prim, 0x78, 0x30, 8, 0x12);
	setPosDataPolyFT4(prim, -0x1c, -0x63, 8, 0x12);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = getTPage(0, 0, 832, 0);
	prim->clut = GetClut(0x10, 0x1e0);
	setUVDataPolyFT4(prim, 0x88, 0x30, 8, 0x12);
	setPosDataPolyFT4(prim, 0x14, -0x63, 8, 0x12);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 832, 0);
		prim->clut = GetClut(0x10, 0x1e0);
		setUVDataPolyFT4(prim, buf[i] * 12, 0x30, 0xc, 0xf);
		setPosDataPolyFT4(prim, x + ((digits - 1) - i) * 14, y, 0xc, 0xf);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = GetTPage(1, 0, 0x2c0, 0);
	prim->clut = GetClut(0x200, 0xff);
	setUVDataPolyFT4(prim, 0, 0x78, 0x42, 0x1d);
	setPosDataPolyFT4(prim, -0x21, -0x68, 0x42, 0x1d);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void STD_addFighterCounter(int32_t arg)
{
	if ((MAIN_D_80135148 == 0) && (arg != 0)) {
		MAIN_D_80135110 = arg;
		addObject(0x1ac, 0, (TickFunction)STD_tickFighterCounter, (RenderFunction)STD_renderFighterCounter);
		MAIN_D_80135148 = 1;
	}
}

void STD_tickFighterCounter(void)
{
	if (MAIN_D_80135114 == 1) {
		BATTLE_FRAME_COUNT++;
		if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]]->anim.animId != 0x2b) {
			if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->anim.animId != 0x2b) {
				if (BATTLE_FRAME_COUNT % 0x14 == 0) {
					if (MAIN_D_80135110 != 0) {
						MAIN_D_80135110--;
					}
				}
			}
		}
	}
}

void STD_renderFighterCounter(void)
{
	STD_renderCounterDigits(-0xd, -0x61, 2, MAIN_D_80135110, 3);
}

void STD_removeFighterCounter(void)
{
	if (MAIN_D_80135148 != 0) {
		removeObject(0x1ac, 0);
		MAIN_D_80135148 = 0;
	}
}

void STD_func_8005C1E4(void)
{
	int32_t i;
	StdUnkBAF4 *s;
	int32_t match;
	int16_t ab[2];
	int16_t arg;
	uint8_t flag;

	if (MAIN_D_8013514C < 5) {
		ab[0] = (MAIN_D_8013514C - 1) * 2;
		ab[1] = (MAIN_D_8013514C - 1) * 2 + 1;
	} else if (MAIN_D_8013514C < 7) {
		ab[0] = MAIN_D_80135158[(MAIN_D_8013514C - 5) * 2];
		ab[1] = MAIN_D_80135158[(MAIN_D_8013514C - 5) * 2 + 1];
	} else {
		ab[0] = MAIN_D_8013515C;
		ab[1] = MAIN_D_8013515D;
	}
	if (ab[0] == MAIN_D_8013514D || ab[1] == MAIN_D_8013514D) {
		match = 1;
	} else {
		match = 0;
	}

	for (i = 0; i < 8; i++) {
		if (i != ab[0] && i != ab[1]) {
			continue;
		}
		s = &STD_D_8007BAF4[i];
		switch (s->unk7) {
		case 0:
			s->unk8++;
			s->unk9++;
			if (s->unk8 % 10 == 0) {
				s->unk5 = (s->unk5 + 1) & 1;
			}
			if (s->unk9 >= 0x29) {
				s->unk9 = 0;
				s->unk7++;
			}
			break;
		case 1:
			s->unk8++;
			s->unk9++;
			if (s->unk9 == 5) {
				playSound(8, 5);
			}
			if (s->unk2 != STD_D_8007A48C[s->unk4]) {
				s->unk2--;
			}
			if (s->unk8 % 10 == 0) {
				STD_D_8007BAF4[i].unk5 = (s->unk5 + 1) & 1;
			}
			if (s->unk2 == STD_D_8007A48C[s->unk4] && s->unk0 == STD_D_8007A598[i].unk0[s->unk4]) {
				s->unkA[1] = 0;
				s->unk9 = 0;
				s->unk7++;
			}
			break;
		case 2:
			s->unk8++;
			s->unk9++;
			if (s->unk8 % 10 == 0) {
				s->unk5 = (s->unk5 + 1) & 1;
			}
			if (match == 0) {
				if (s->unk9 >= 0x29) {
					s->unk5 = 2;
					playSound(8, 1);
					s->unk8 = 0;
					s->unk9 = 0;
					s->unkA[0] = 0;
					flag = 0;
					if (MAIN_D_8013514C < 5) {
						if (i == MAIN_D_80135158[MAIN_D_8013514C - 1]) {
							flag = 1;
						}
					}
					if (MAIN_D_8013514C >= 5 && MAIN_D_8013514C < 7) {
						if (i == (&MAIN_D_8013515C)[MAIN_D_8013514C - 5]) {
							flag = 1;
						}
					}
					if (i == ab[0]) {
						arg = ab[1];
					} else {
						arg = ab[0];
					}
					STD_func_8005D398((int16_t)i, arg, flag);
					s->unk7++;
				}
			} else {
				MAIN_D_80135164 += 3;
				if (MAIN_D_80135164 >= 0x81) {
					MAIN_D_80135164 = 0x80;
				}
			}
			break;
		case 3:
			if (s->unk5 == 2) {
				s->unk9++;
			} else {
				s->unk8++;
				if (s->unk8 % 10 == 0) {
					s->unk5 = (s->unk5 + 1) & 1;
				}
			}
			if (s->unk9 >= 0x15) {
				s->unk5 = 0;
				s->unk8 = 0;
				s->unk9 = 0;
			}
			if (s->unkA[0] == 1) {
				s->unkA[1] = 3;
				s->unk9 = 0;
				s->unk8 = 0;
				s->unk7++;
				if (s->unkA[1] != 3) {
					STD_func_8005D538((int16_t)i);
				} else if (s->unkC == 1) {
					STD_func_8005D538((int16_t)i);
				} else {
					s->unk6 = (s->unk6 + 1) & 1;
				}
			}
			break;
		case 4:
			s->unk8++;
			if (s->unkA[1] != 3) {
				s->unk5 = 3;
				if (s->unk8 >= 0x15) {
					s->unk8 = 0;
					s->unk7 = 2;
				}
			} else if (s->unkC == 1) {
				s->unk5 = 3;
				if (s->unk8 >= 0x1f) {
					s->unk8 = 0;
					s->unk7++;
					playSound(8, 3);
				}
			} else {
				if (s->unk8 == 0x14) {
					s->unk6 = (s->unk6 + 1) & 1;
				}
				if (s->unk8 >= 0x1f) {
					s->unk4++;
					s->unk7 += 2;
					if (s->unk5 == 2) {
						s->unk5 = 0;
					}
				}
			}
			break;
		case 5:
			if (s->unk8 < 0x18) {
				s->unk2 = STD_D_8007A48C[s->unk4] + STD_D_8007A474[s->unk8++];
				if (s->unk8 == 0x14) {
					playSound(8, 4);
				}
			} else {
				s->unk7++;
			}
			break;
		case 6:
			if (match != 0) {
				if (MAIN_D_801350E4 == 1) {
					if (i == MAIN_D_8013514D) {
						s->unk0 = STD_D_8007A598[i].unk0[s->unk4 + 1];
						s->unk5 = 0;
						s->unk6 = STD_D_8007A598[i].unk10[s->unk4 + 1];
						s->unk4 += 2;
						s->unk9 = 0;
						s->unk8 = 0;
						s->unk5 = 0;
						s->unk7 = 0;
						if (MAIN_D_8013514C < 5) {
							MAIN_D_80135158[MAIN_D_8013514D / 2] = MAIN_D_8013514D;
						}
						if (MAIN_D_8013514C < 7) {
							(&MAIN_D_8013515C)[MAIN_D_8013514D / 4] = MAIN_D_8013514D;
						}
						MAIN_D_8013514C++;
					} else {
						s->unk2 = STD_D_8007A48C[s->unk4 - 1];
						s->unk5 = 3;
					}
				} else if (i == MAIN_D_8013514D) {
					s->unk2 = STD_D_8007A48C[s->unk4 - 1];
					s->unk5 = 3;
				} else {
					s->unk0 = STD_D_8007A598[i].unk0[s->unk4 - 1];
					s->unk5 = 0;
				}
			} else if (s->unkC == 0) {
				s->unk8++;
				if (s->unk8 % 10 == 0) {
					s->unk5 = (s->unk5 + 1) & 1;
				}
				if (STD_D_8007BAF4[STD_D_8007BB64[i].owner].unk7 == 6) {
					if (s->unk0 > STD_D_8007A598[i].unk0[s->unk4]) {
						s->unk0--;
					}
					if (s->unk0 < STD_D_8007A598[i].unk0[s->unk4]) {
						s->unk0++;
					}
					if (s->unk2 == STD_D_8007A48C[s->unk4] && s->unk0 == STD_D_8007A598[i].unk0[s->unk4]) {
						s->unk6 = STD_D_8007A598[i].unk10[(int32_t)s->unk4];
						s->unk4++;
						s->unk9 = 0;
						s->unk8 = 0;
						s->unk5 = 0;
						s->unk7 = 0;
						MAIN_D_8013514C++;
					}
				}
			}
			break;
		case 7:
			s->unk8++;
			if (i == MAIN_D_8013514D) {
				s->unk0 = STD_D_8007A598[i].unk0[7];
				s->unk2 = STD_D_8007A48C[7];
				s->unk5 = 0;
				s->unk6 = STD_D_8007A598[i].unk10[7];
			} else {
				s->unk0 = STD_D_8007A598[i].unk0[4];
				s->unk2 = STD_D_8007A48C[4];
				s->unk6 = STD_D_8007A598[i].unk10[4];
				s->unk5 = 3;
			}
			if (s->unk8 >= 0x3d) {
				MAIN_D_80135160 = 1;
			}
			break;
		}
	}

	for (i = 0; i < 8; i++) {
		STD_func_8005D550((int16_t)i);
	}

	if (match != 0 && MAIN_D_80135164 == 0x80 && STD_D_8007BAF4[MAIN_D_8013514D].unk7 == 2) {
		STD_D_8007BAF4[MAIN_D_8013514D].unk7 = (MAIN_D_8013514C != 7) ? 6 : 7;
		if (MAIN_D_8013514C == 7) {
			STD_D_8007BAF4[MAIN_D_8013514D].unk8 = 0;
		}
		if (MAIN_D_8013514D == ab[0]) {
			STD_D_8007BAF4[ab[1]].unk7 = (MAIN_D_8013514C != 7) ? 6 : 7;
			if (MAIN_D_8013514C == 7) {
				STD_D_8007BAF4[ab[1]].unk8 = 0;
			}
		} else {
			STD_D_8007BAF4[ab[0]].unk7 = (MAIN_D_8013514C != 7) ? 6 : 7;
			if (MAIN_D_8013514C == 7) {
				STD_D_8007BAF4[ab[0]].unk8 = 0;
			}
		}
		MAIN_D_80135160 = 1;
	}
}

void STD_func_8005CE9C(void)
{
	POLY_FT4 *prim;
	GsBOXF box;
	long i;
	int32_t j;
	int32_t p;
	int32_t len;
	int32_t small;
	uint8_t c;
	int16_t shift;

	for (i = 0; i < 8; i++) {
		len = strlen(DIGIMON_DATA[PARTICIPANT_TYPES[i]].name) / 2;
		if (PARTICIPANT_TYPES[i] == 0x4e || PARTICIPANT_TYPES[i] == 0x3c) {
			len = 10;
		}
		p = 0;
		shift = 0;
		small = 0;
		for (j = 0; j < len; j++) {
			if (len < 8) {
				c = STD_D_80079CBC[PARTICIPANT_TYPES[i]][p++];
				STD_func_8005D814((int16_t)(STD_D_8007A598[i].unk0[0] + 4),
				                  (int16_t)(STD_D_8007A48C[0] + 0x12 + j * 8), c, 5);
				if (c == 0x1f || c == 0x25) {
					c = STD_D_80079CBC[PARTICIPANT_TYPES[i]][p++];
					STD_func_8005D814((int16_t)(STD_D_8007A598[i].unk0[0] + 4),
					                  (int16_t)(STD_D_8007A48C[0] + 0x12 + j * 8), c, 5);
				}
			} else {
				c = STD_D_80079CBC[PARTICIPANT_TYPES[i]][p++];
				if (c == 0x3d) {
					small = 1;
					c = STD_D_80079CBC[PARTICIPANT_TYPES[i]][p++];
					shift = -((j - 1) * 8);
				}
				STD_func_8005D814((int16_t)((small == 0) ? STD_D_8007A598[i].unk0[0] + 8 : STD_D_8007A598[i].unk0[0]),
				                  (int16_t)(shift + (STD_D_8007A48C[0] + 0x12 + j * 8)), c, 5);
				if (c == 0x1f || c == 0x25) {
					c = STD_D_80079CBC[PARTICIPANT_TYPES[i]][p++];
					STD_func_8005D814((int16_t)((small == 0) ? STD_D_8007A598[i].unk0[0] + 8 : STD_D_8007A598[i].unk0[0]),
					                  (int16_t)(shift + (STD_D_8007A48C[0] + 0x12 + j * 8)), c, 5);
				}
			}
		}
	}

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	if (MAIN_D_80135164 != 0) {
		setSemiTrans(prim, 1);
	}
	setRGB0(prim, 0x80 - MAIN_D_80135164, 0x80 - MAIN_D_80135164, 0x80 - MAIN_D_80135164);
	setUVDataPolyFT4(prim, 0, 0x78, 0xff, 0x87);
	setPosDataPolyFT4(prim, -0x80, -0x61, 0xff, 0x87);
	prim->tpage = GetTPage(0, 1, 0x340, 0);
	prim->clut = GetClut(0x20, 0x1e0);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 5, prim++);
	GsSetWorkBase((PACKET *)prim);

	box.attribute = 0x40000000;
	box.x = -0xa0;
	box.y = -0x78;
	box.r = box.g = box.b = 0x20;
	setWH(&box, 0x140, 0xf0);
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 5);
}

void STD_func_8005D398(int16_t i, int32_t owner, uint8_t flag)
{
	if (STD_D_8007BAF4[i].unk6 == 1) {
		STD_D_8007BB64[i].x = STD_D_8007BAF4[i].unk0 + 0x10;
		STD_D_8007BB64[i].y = STD_D_8007BAF4[i].unk2;
		if (flag == 1) {
			STD_D_8007BAF4[owner].unkC = 1;
		}
	} else {
		STD_D_8007BB64[i].x = STD_D_8007BAF4[i].unk0 - 8;
		if (flag == 0) {
			STD_D_8007BB64[i].y = STD_D_8007BAF4[i].unk2 + 8;
		} else {
			STD_D_8007BB64[i].y = STD_D_8007BAF4[i].unk2;
			STD_D_8007BAF4[owner].unkC = 1;
		}
	}
	STD_D_8007BB64[i].flag = flag;
	STD_D_8007BB64[i].owner = owner;
	addObject(0x1af, i, (TickFunction)STD_func_8005E124, NULL);
}

void STD_func_8005D538(int16_t i)
{
	addObject(0x1b0, i, STD_func_8005E004, 0);
}

void STD_func_8005D550(int16_t id)
{
	POLY_FT4 *prim;
	uint8_t tile;
	uint8_t u;
	uint8_t v;
#if defined(VERSION_JP)
	uint8_t w;
	uint8_t h;
#else
	int32_t w;
	int32_t h;
	int32_t w2;
	int32_t h2;
#endif

	tile = PARTICIPANT_TYPES[id];
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	if (MAIN_D_80135164 != 0) {
		setSemiTrans(prim, 1);
	}
	prim->tpage = GetTPage(0, 1, (tile / 64) * 64 + 0x380, 0);
	prim->clut = GetClut((tile / 64) * 16 + 0x120, STD_D_8007A49C[tile] + 0x1e0);
	setRGB0(prim, 0x80 - MAIN_D_80135164, 0x80 - MAIN_D_80135164, 0x80 - MAIN_D_80135164);
	u = ((tile - 1) % 32) * 32 + (STD_D_8007BAF4[id].unk5 & 1) * 16;
	v = ((tile - 1) / 8) * 32 + (STD_D_8007BAF4[id].unk5 / 2) * 16;
	w = h = 0x10;
	if (u == 0xf0) {
		w = 0xf;
	}
	if (v == 0xf0) {
		h = 0xf;
	}
#if defined(VERSION_JP)
	setUVDataPolyFT4(prim, u, v, w, h);
	setPosDataPolyFT4(prim, STD_D_8007BAF4[id].unk0, STD_D_8007BAF4[id].unk2, w, h);
#else
	h2 = h2 = h;
	setUVDataPolyFT4(prim, u, v, w2 = w2 = w, h);
	setPosDataPolyFT4(prim, STD_D_8007BAF4[id].unk0, STD_D_8007BAF4[id].unk2, w2, h2);
#endif
	if (STD_D_8007BAF4[id].unk6 == 1) {
		swapShort(&prim->x0, &prim->x1);
		swapShort(&prim->x2, &prim->x3);
		STD_func_8005D7C0(prim, prim->u0, w, h);
	}
	AddPrim(ACTIVE_ORDERING_TABLE->org + 5, prim++);
	GsSetWorkBase((PACKET *)prim);
}

// clang-format off
void STD_func_8005D7A8(i)
	int16_t i;
// clang-format on
{
	removeObject(0x1b0, i);
}

void STD_func_8005D7B4(int16_t i)
{
	removeObject(0x1af, i);
}

// clang-format off
void STD_func_8005D7C0(prim, a, b, h)
	POLY_FT4 *prim;
	int16_t a;
	int16_t b;
	int16_t h;
// clang-format on
{
	if ((a -= 1) < 0) {
		a = 0;
		b--;
	}
	setUVDataPolyFT4(prim, a, prim->v0, b, h);
}

void STD_func_8005D814(int16_t x, int16_t y, uint8_t n, int32_t layer)
{
	POLY_FT4 *prim;
	uint8_t u;
	uint8_t v;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->tpage = GetTPage(0, 1, 0x340, 0x100);
	prim->clut = GetClut(0x30, 0x1e0);
	if (MAIN_D_80135164 != 0) {
		setSemiTrans(prim, 1);
	}
	setRGB0(prim, 0x80 - MAIN_D_80135164, 0x80 - MAIN_D_80135164, 0x80 - MAIN_D_80135164);
	u = (n % 8) * 8;
	v = (n / 8) * 8;
	setUVDataPolyFT4(prim, u, v, (u != 0xf8) ? 8 : 7, (v != 0xf8) ? 8 : 7);
	setPosDataPolyFT4(prim, x, y, 8, 8);
#if defined(VERSION_JP)
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
#endif
	GsSetWorkBase((PACKET *)prim);
}

void STD_func_8005D964(void)
{
	StdUnkBAF4 *p;
	int32_t i;

	MAIN_D_8013514C = 1;
	for (i = 0; i < 8; i++) {
		p = &STD_D_8007BAF4[i];
		p->unk0 = STD_D_8007A598[i].unk0[0];
		p->unk2 = STD_D_8007A48C[0];
		p->unk5 = 0;
		p->unk4 = 1;
		p->unk6 = STD_D_8007A598[i].unk10[0];
		p->unk8 = 0;
		p->unk9 = 0;
		p->unkC = 0;
		p->unk7 = 0;
	}
}

void STD_func_8005D9F4(uint8_t *out, uint8_t *list)
{
	int32_t i;
	int32_t r;
	long j;
	uint8_t *p;

	MAIN_D_8013514D = randomLimit(8);
	PARTICIPANT_TYPES[MAIN_D_8013514D] = ENTITY_TABLE[1]->type;

	for (i = 1; i < 8; i++) {
		j = randomLimit(7) + 1;
		swapByte(&list[i], &list[j]);
	}

	p = list + 1;
	for (i = 0; i < 8; i++) {
		if (i != MAIN_D_8013514D) {
			PARTICIPANT_TYPES[i] = *p++;
		}
	}

	for (i = 0; i < 4; i++) {
		if (randomLimit(100) < STD_D_8007A58C[DIGIMON_DATA[PARTICIPANT_TYPES[i * 2]].level - 3][DIGIMON_DATA[PARTICIPANT_TYPES[i * 2 + 1L]].level - 3]) {
			MAIN_D_80135158[i] = i * 2;
		} else {
			MAIN_D_80135158[i] = i * 2 + 1;
		}
	}

	if (MAIN_D_8013514D < 4) {
		if (MAIN_D_8013514D < 2) {
			MAIN_D_8013515C = MAIN_D_80135158[1];
		} else {
			MAIN_D_8013515C = MAIN_D_80135158[0];
		}
		if (randomLimit(100) < STD_D_8007A58C[DIGIMON_DATA[PARTICIPANT_TYPES[MAIN_D_80135158[2]]].level - 3][DIGIMON_DATA[PARTICIPANT_TYPES[MAIN_D_80135158[3]]].level - 3]) {
			MAIN_D_8013515D = MAIN_D_80135158[2];
		} else {
			MAIN_D_8013515D = MAIN_D_80135158[3];
		}
	} else {
		if (randomLimit(100) < STD_D_8007A58C[DIGIMON_DATA[PARTICIPANT_TYPES[MAIN_D_80135158[0]]].level - 3][DIGIMON_DATA[PARTICIPANT_TYPES[MAIN_D_80135158[1]]].level - 3]) {
			MAIN_D_8013515C = MAIN_D_80135158[0];
		} else {
			MAIN_D_8013515C = MAIN_D_80135158[1];
		}
		if (MAIN_D_8013514D < 6) {
			MAIN_D_8013515D = MAIN_D_80135158[3];
		} else {
			MAIN_D_8013515D = MAIN_D_80135158[2];
		}
	}

	if (MAIN_D_8013514D % 2 == 0) {
		out[0] = PARTICIPANT_TYPES[MAIN_D_8013514D + 1];
	} else {
		out[0] = PARTICIPANT_TYPES[MAIN_D_8013514D - 1];
	}

	switch (MAIN_D_8013514D / 2) {
	case 0:
		out[1] = PARTICIPANT_TYPES[MAIN_D_80135158[1]];
		out[2] = PARTICIPANT_TYPES[MAIN_D_8013515D];
		break;
	case 1:
		out[1] = PARTICIPANT_TYPES[MAIN_D_80135158[0]];
		out[2] = PARTICIPANT_TYPES[MAIN_D_8013515D];
		break;
	case 2:
		out[1] = PARTICIPANT_TYPES[MAIN_D_80135158[3]];
		out[2] = PARTICIPANT_TYPES[MAIN_D_8013515C];
		break;
	case 3:
		out[1] = PARTICIPANT_TYPES[MAIN_D_80135158[2]];
		out[2] = PARTICIPANT_TYPES[MAIN_D_8013515C];
		break;
	}
}

void STD_func_8005DEEC(int16_t track)
{
	ENTITY_TABLE[1]->isOnScreen = 0;
	ENTITY_TABLE[1]->isOnMap = 0;
	MAIN_D_80135160 = 0;
	MAIN_D_80135164 = 0;
	addObject(0x1ae, 0, (TickFunction)STD_func_8005DF64, (RenderFunction)STD_func_8005DF6C);
	stopBGM();
	stopSound();
	playMusic(0x1d, track);
}

void STD_func_8005DF64(void)
{
}

void STD_func_8005DF6C(void)
{
	STD_func_8005C1E4();
	STD_func_8005CE9C();
}

void STD_func_8005DF94(int16_t mode)
{
	ENTITY_TABLE[1]->isOnScreen = 1;
	ENTITY_TABLE[1]->isOnMap = 1;
	removeObject(0x1ae, 0);
	if (mode != 2) {
		stopBGM();
		stopSound();
	}
}

int32_t STD_func_8005DFF8(void)
{
	return MAIN_D_80135160;
}

void STD_func_8005E004(int32_t idx)
{
	POLY_FT4 *prim;

	if (STD_D_8007BAF4[idx].unk8 >= 0x10) {
		STD_func_8005D7A8(idx);
	}
	if ((STD_D_8007BAF4[idx].unk8 % 5) == 4) {
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 960, 0);
		prim->clut = GetClut(0x130, 0x1e0);
		setUVDataPolyFT4(prim, 0, 0xc8, 0x10, 0x10);
		setPosDataPolyFT4(prim, STD_D_8007BAF4[idx].unk0, STD_D_8007BAF4[idx].unk2, 0x10, 0x10);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 3, prim++);
		GsSetWorkBase((PACKET *)prim);
	}
}

// clang-format off
void STD_func_8005E124(id)
	int16_t id;
// clang-format on
{
	int16_t d;

	if (MAIN_D_8013514C < 5) {
		d = 1;
	} else if (MAIN_D_8013514C < 7) {
		d = 3;
	}

	if (STD_D_8007BAF4[id].unk6 == 1) {
		STD_D_8007BB64[id].x += d;
	} else {
		STD_D_8007BB64[id].x -= d;
	}
	STD_func_8005E1E4(id);
}

void STD_func_8005E1E4(int16_t id)
{
	POLY_FT4 *prim;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = GetTPage(0, 2, 0x3c0, 0);
	prim->clut = GetClut(0x130,
	                     STD_D_8007A57C[STD_D_8007A50C[PARTICIPANT_TYPES[id]] - 1] + 0x1e0);
	setUVDataPolyFT4(prim, STD_D_8007A50C[PARTICIPANT_TYPES[id]] * 8 - 8, 0xc0, 8, 8);
	setPosDataPolyFT4(prim, STD_D_8007BB64[id].x, STD_D_8007BB64[id].y, 8, 8);
	if (STD_D_8007BAF4[id].unk6 == 1) {
		swapShort(&prim->x0, &prim->x1);
		swapShort(&prim->x2, &prim->x3);
		STD_func_8005D7C0(prim, prim->u0, 8, 8);
	}
	AddPrim(ACTIVE_ORDERING_TABLE->org + 4, prim++);

	if (STD_D_8007BB64[id].flag == 1) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = GetTPage(0, 2, 0x3c0, 0);
		prim->clut = GetClut(0x130,
		                     STD_D_8007A57C[STD_D_8007A50C[PARTICIPANT_TYPES[id]] - 1] + 0x1e0);
		setUVDataPolyFT4(prim, STD_D_8007A50C[PARTICIPANT_TYPES[id]] * 8 - 8, 0xc0, 8, 8);
		setPosDataPolyFT4(prim, STD_D_8007BB64[id].x, STD_D_8007BB64[id].y + 8, 8, 8);
		if (STD_D_8007BAF4[id].unk6 == 1) {
			swapShort(&prim->x0, &prim->x1);
			swapShort(&prim->x2, &prim->x3);
			STD_func_8005D7C0(prim, prim->u0, 8, 8);
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 4, prim++);
	}
	GsSetWorkBase((PACKET *)prim);

	if (STD_D_8007BAF4[id].unk6 == 1) {
		if (STD_D_8007BAF4[STD_D_8007BB64[id].owner].unk0 - STD_D_8007BB64[id].x < 9) {
			STD_D_8007BAF4[STD_D_8007BB64[id].owner].unkA[0] = 1;
			STD_func_8005D7B4(id);
		}
	} else {
		if (STD_D_8007BB64[id].x - STD_D_8007BAF4[STD_D_8007BB64[id].owner].unk0 < 0x11) {
			STD_D_8007BAF4[STD_D_8007BB64[id].owner].unkA[0] = 1;
			STD_func_8005D7B4(id);
		}
	}
}

void STD_func_8005E5E0(void)
{
	ENTITY_TABLE[1]->isOnScreen = 0;
	ENTITY_TABLE[1]->isOnMap = 0;
	MAIN_D_80135164 = 0x80;
	MAIN_D_80135165 = 0x80;
	MAIN_D_80135166 = 0x80;
	MAIN_D_80135167 = 0;
	stopBGM();
	stopSound();
	playMusic(0x1d, 0);
	addObject(0x1a1, 0, (TickFunction)STD_func_8005E660, (RenderFunction)STD_func_8005E6E4);
}

void STD_func_8005E660(void)
{
	if (MAIN_D_80135167 < 0x78) {
		MAIN_D_80135167++;
	}
	if (MAIN_D_80135167 >= 0x65) {
		if (MAIN_D_80135164 != 0) {
			MAIN_D_80135164 -= 8;
		}
		if (MAIN_D_80135165 >= 9) {
			MAIN_D_80135165 -= 8;
		}
		if (MAIN_D_80135166 != 0) {
			MAIN_D_80135166 -= 8;
		}
	}
}

void STD_func_8005E6E4(void)
{
	POLY_FT4 *prim;

	STD_func_8005CE9C();
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = GetTPage(0, 2, 0x380, 0x1c0);
	prim->clut = GetClut(0x100, 0x1f6);
	setRGB0(prim, MAIN_D_80135165, MAIN_D_80135165, MAIN_D_80135165);
	setUVDataPolyFT4(prim, 0xfa, 0xfd, 2, 2);
	setPosDataPolyFT4(prim, -0xa0, -0x78, 0x140, 0xf0);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[3], prim++);
	SetPolyFT4(prim);
	if (MAIN_D_80135164 != 0x80) {
		SetSemiTrans(prim, 1);
	}
	setRGB0(prim, MAIN_D_80135166, MAIN_D_80135166, MAIN_D_80135166);
	prim->tpage = GetTPage(1, 1, 0x180, 0);
	prim->clut = GetClut(0, 0x1e6);
	setUVDataPolyFT4(prim, 0, 0, 0xff, (CURRENT_SCREEN != 0x6a) ? 0x80 : 0x7d);
	setPosDataPolyFT4(prim, -0x80, -0x40, 0xff, (CURRENT_SCREEN != 0x6a) ? 0x80 : 0x7d);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[2], prim++);
	GsSetWorkBase((PACKET *)prim);
}

void STD_func_8005E898(void)
{
	removeObject(0x1a1, 0);
}

void STD_func_8005F650(void)
{
	int32_t i;
	int32_t result;
	int16_t enemies[4];
	int16_t moveFlags[4];
	Stats *stats;
	int16_t targetFlags[4];
	int16_t count;
	FighterData *fighter;
	uint16_t *flagsPtr;
	Entity *partner;

	partner = ENTITY_TABLE[1];
	stats = &((DigimonEntity *)partner)->stats;
	fighter = COMBAT_DATA_PTR->fighter;
	flagsPtr = &fighter->flags;
	if (COMBAT_DATA_PTR->player.commandDelay[0] == 0) {
		COMBAT_DATA_PTR->player.currentCommand[0] = COMBAT_DATA_PTR->player.bufferedCommand[0];
	} else if (!(*flagsPtr & 0x800e) && (fighter->flatTimer == 0)) {
		COMBAT_DATA_PTR->player.commandDelay[0]--;
	}

	if (NO_AI_FLAG == 0) {
		if (stats->current.currentHP > fighter->hpDamageBuffer) {
			if (stats->base.brain < 0x12d) {
				if ((BATTLE_FRAME_COUNT % (((stats->base.brain / 2) + 1) * 20)) == 0) {
					if ((0x46 - PARTNER_PARA.discipline) > randomLimit(100)) {
						*flagsPtr |= 0x2000;
						fighter->senileTimer = 100;
					}
				}
			}
		}
		if (fighter->cooldown >= 2) {
			fighter->cooldown--;
		}
		if (!(*flagsPtr & 0x2000)) {
			STD_increaseSpeedBuffer(fighter, stats);
		}
	}

	if (*flagsPtr & 0x80b0) {
		return;
	}

	if (stats->current.currentHP == 0) {
		STD_faintDigimon((DigimonEntity *)partner, fighter, 0);
		if (STD_func_80060B98() == 0) {
			STD_func_8005A55C((DigimonEntity *)partner, 5, 0);
		}
		MAIN_D_80135118 = 1;
		return;
	}

	STD_getRemainingEnemies(partner, enemies, &count);
	if ((count == 0) || (((DigimonEntity *)partner)->stats.current.currentHP <= fighter->hpDamageBuffer)) {
		handleBattleIdle((DigimonEntity *)partner, stats, *flagsPtr);
		fighter->moveRange = -1;
		STD_resetFlatten(0);
		STD_removeStatusEffects((DigimonEntity *)partner, fighter);
		*flagsPtr = 0;
		*flagsPtr |= 0x40;
		return;
	}

	if (NO_AI_FLAG != 0) {
		return;
	}

	if (!(*flagsPtr & 0x800e) && (fighter->flatTimer == 0)) {
		switch (COMBAT_DATA_PTR->player.currentCommand[0]) {
		case 7:
			if (fighter->targetId == 0xff) {
				break;
			}
			if (COMBAT_DATA_PTR->player.changeTarget != 1) {
				break;
			}
			switch (count) {
			case 2:
				if (fighter->targetId == enemies[0]) {
					fighter->targetId = enemies[1];
				} else {
					fighter->targetId = enemies[0];
				}
				break;
			case 3:
				if ((COMBAT_DATA_PTR->player.unk5[0] == 0xff) && (COMBAT_DATA_PTR->player.unk5[1] == 0xff)) {
					for (i = 1; i < 4; i++) {
						targetFlags[i] = 1;
					}
					fighter->targetId = STD_getNearestEnemy(partner, targetFlags);
					COMBAT_DATA_PTR->player.unk5[COMBAT_DATA_PTR->player.unk7] = fighter->targetId;
					COMBAT_DATA_PTR->player.unk7 = (COMBAT_DATA_PTR->player.unk7 + 1) & 1;
				} else {
					for (i = 1; i < 4; i++) {
						if ((fighter->targetId != i) && (COMBAT_DATA_PTR->player.unk5[(COMBAT_DATA_PTR->player.unk7 + 1) & 1] != i)) {
							break;
						}
					}
					COMBAT_DATA_PTR->player.unk5[COMBAT_DATA_PTR->player.unk7] = fighter->targetId;
					COMBAT_DATA_PTR->player.unk7 = (COMBAT_DATA_PTR->player.unk7 + 1) & 1;
					fighter->targetId = i;
				}
				break;
			}
			COMBAT_DATA_PTR->player.changeTarget = 0;
			return;
		case 8:
		case 9:
		case 10:
			if (STD_isMoveUsable((DigimonEntity *)partner, fighter, COMBAT_DATA_PTR->player.currentCommand[0] - 8) == 0) {
				break;
			}
			fighter->targetId = 1;
			if ((((DigimonEntity *)partner)->stats.base.moves[COMBAT_DATA_PTR->player.currentCommand[0] - 8] != fighter->queuedAnim) || (fighter->moveRange <= 0)) {
				STD_setupQueuedMove((DigimonEntity *)partner, fighter, 0, COMBAT_DATA_PTR->player.currentCommand[0] - 8);
			}
			STD_applyChargeRequirement((DigimonEntity *)partner, fighter, DIGIMON_DATA[partner->type].moves[fighter->queuedAnim - 0x2e]);
			return;
		case 11:
			fighter->targetId = 1;
			if ((((DigimonEntity *)partner)->stats.base.moves[3] != fighter->queuedAnim) || (fighter->moveRange <= 0)) {
				STD_setupQueuedMove((DigimonEntity *)partner, fighter, 0, 3);
			}
			STD_applyChargeRequirement((DigimonEntity *)partner, fighter, DIGIMON_DATA[partner->type].moves[fighter->queuedAnim - 0x2e]);
			return;
		}
		if ((COMBAT_DATA_PTR->player.currentCommand[0] != 2) && (COMBAT_DATA_PTR->player.currentCommand[0] != 4)) {
			((DigimonEntity *)partner)->stats.current.chargeMode = MAIN_D_80135168;
		}
	}

	if ((*flagsPtr & 8) && ((BATTLE_FRAME_COUNT % 100) == 0)) {
		fighter->targetId = enemies[randomLimit(count)];
	}

	if (*flagsPtr & 0x40) {
		return;
	}

	if (*flagsPtr & 8) {
		fighter->queuedAnim = 0;
		fighter->targetId = 1;
		fighter->moveRange = 2;
		partner->flatSprite = 0;
		fighter->flags |= 0x40;
		return;
	}

	if (*flagsPtr & 4) {
		return;
	}

	if (*flagsPtr & 2) {
		STD_selectConfusedMove((DigimonEntity *)partner, fighter, 0);
		return;
	}

	if (*flagsPtr & 0x800) {
		return;
	}

	if (*flagsPtr & 0x1000) {
		return;
	}

	if (*flagsPtr & 0x2000) {
		return;
	}

	fighter->targetId = 1;

	result = -1;
	switch (COMBAT_DATA_PTR->player.currentCommand[0]) {
	case 2:
		if (STD_func_80062BD8(moveFlags, 0) == 0) {
			fighter->cooldown = 0x50;
			fighter->flags |= 0x800;
			return;
		}
		result = STD_func_800675E8(0, moveFlags);
		((DigimonEntity *)partner)->stats.current.chargeMode = 0;
		break;
	case 4:
		if (STD_func_80062BD8(moveFlags, 0) == 0) {
			fighter->cooldown = 0x50;
			fighter->flags |= 0x800;
			return;
		}
		result = STD_func_80067660(0, moveFlags);
		((DigimonEntity *)partner)->stats.current.chargeMode = 2;
		break;
	}

	if (result == -1) {
		STD_func_80067A30((DigimonEntity *)partner, fighter, 0);
	} else {
		STD_setupQueuedMove((DigimonEntity *)partner, fighter, 0, result);
	}
}

void STD_func_8005EF84(void)
{
	int32_t frames;
	int32_t effect;
	int32_t finished;
	int32_t moveCount;
	int16_t moves[18];
	int16_t effectIds[18];
	long i;
	long j;
	DigimonEntity *entity;
	int8_t isBusy;

	GAME_STATE = 5;
	STD_startWalkingAnimation2(&PARTNER_ENTITY.digimonEntity.entity,
	                           &PARTNER_ENTITY.digimonEntity.stats,
	                           COMBAT_DATA_PTR->fighter[0].flags);
	entityLookAtLocation(&PARTNER_ENTITY.digimonEntity.entity,
	                     &ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->posData->location);

	for (i = 1; i <= ENEMY_COUNT; ++i) {
		startAnimation(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]], 0x21);
		entityLookAtLocation(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]],
		                     &ENTITY_TABLE[1]->posData->location);
	}

	moveCount = 0;
	STD_initializeEFEEngine((char *)GENERAL_BUFFER_PTR);

	for (i = 0; i <= ENEMY_COUNT; ++i) {
		entity = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		entity->stats.current.unk1 = -1;
		entity->stats.current.efeSubEffect = -1;

		for (j = 0; j < 4; ++j) {
			if (entity->stats.base.moves[j] != 0xff) {
				effect = DIGIMON_DATA[entity->entity.type].moves[entity->stats.base.moves[j] - 0x2e] + 0x100;
				moves[moveCount++] = effect;
			}
		}
	}

	moves[moveCount] = -1;
	STD_loadMoveEFE(moves, effectIds, &isBusy);

	while (isBusy > 0) {
		tickFileReadQueue(0);
	}

	MAIN_D_8013511C = 200;
	STD_initializeBattleStartText();

	frames = 0;
	finished = 0;
	playSound(0, 0x10);

	while (frames < 60 || finished == 0) {
		if (MAIN_D_8013511C < 4200) {
			MAIN_D_8013511C += 400;
		}

		++frames;
		finished = STD_func_8006A514();
		STD_battleTickFrame();
	}

	STD_func_8006A044();
	STD_initializeBattleStartTextBurst();
	playSound(0, 0x11);

	while (STD_func_8006A514() == 0) {
		if (MAIN_D_8013511C > 1000) {
			MAIN_D_8013511C -= 400;
		}

		STD_battleTickFrame();
	}

	STD_func_8006A508();

	moveCount = 0;
	for (i = 0; i <= ENEMY_COUNT; ++i) {
		entity = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];

		for (j = 0; j < 4; ++j) {
			if (entity->stats.base.moves[j] == 0xff) {
				COMBAT_DATA_PTR->fighter[i].effectSlot[j] = -1;
			} else {
				COMBAT_DATA_PTR->fighter[i].effectSlot[j] = effectIds[moveCount++];
			}
		}
	}

	MAIN_D_8013511C = 1000;
	MAIN_D_80135114 = 1;
	MAIN_D_801350EC = 1;
	GAME_STATE = 4;
}

int16_t STD_func_8005F354(void)
{
	Entity *other;
	int32_t i;

#if !defined(VERSION_JP)
	if (COMBAT_DATA_PTR->fighter[0].hpDamageBuffer != 0) {
		return 0;
	}

	if (COMBAT_DATA_PTR->fighter[1].hpDamageBuffer != 0) {
		return 0;
	}
#endif

	if (ENTITY_TABLE[1]->anim.animId == 0x2b &&
	    (ENTITY_TABLE[1]->anim.animFlag & 1) == 0) {
		if (STD_func_80060B98() == 0) {
			return -1;
		}

		for (i = 1; i <= ENEMY_COUNT; ++i) {
			other = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
			if (other->anim.animId != 0x2b ||
			    (other->anim.animFlag & 1) != 0) {
				return 0;
			}
		}

		return 2;
	}

	for (i = 1; i <= ENEMY_COUNT; ++i) {
		other = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		if (other->anim.animId != 0x2b ||
		    (other->anim.animFlag & 1) != 0) {
			break;
		}
	}

	if (i == ENEMY_COUNT + 1) {
		if ((PARTNER_ENTITY.digimonEntity.stats.current.currentHP - COMBAT_DATA_PTR->fighter[0].hpDamageBuffer) > 0) {
			return 1;
		}

		if (ENTITY_TABLE[1]->anim.animId != 0x2b ||
		    (ENTITY_TABLE[1]->anim.animFlag & 1) != 0) {
			return 0;
		}

		return 2;
	}

	if (MAIN_D_80135110 == 0) {
		if ((PARTNER_ENTITY.digimonEntity.stats.current.currentHP - (*(FighterData **)&COMBAT_DATA_PTR)->hpDamageBuffer) <= 0) {
			return 0;
		}

		if (STD_func_80060B98() != 0) {
			return 0;
		}

		MAIN_D_80135100 = MAIN_D_801350FC - ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]])->stats.current.currentHP;
		MAIN_D_80135102 = MAIN_D_801350FE - ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]])->stats.current.currentHP;
		COMBAT_DATA_PTR->fighter[0].hpDamageBuffer = 0;
		COMBAT_DATA_PTR->fighter[1].hpDamageBuffer = 0;

		if (MAIN_D_80135100 > MAIN_D_80135102) {
			STD_func_80060C14(1, 0);
			return -1;
		}

		if (MAIN_D_80135100 < MAIN_D_80135102) {
			STD_func_80060C14(0, 1);
			return 1;
		}

		if (MAIN_D_80135100 == MAIN_D_80135102) {
			STD_func_80060C14(1, 1);
			return 2;
		}
	}

	return 0;
}

void STD_func_8005E8A4(Entity *entity, Entity *other)
{
	int32_t i;
	FighterData *f;
	Stats *stats;
	int32_t j;
	int16_t *dst;
	DigimonEntity *digimon;
	int16_t brains;

	((int16_t *)&MAIN_D_80135100)[0] = 0;
	((int16_t *)&MAIN_D_80135100)[1] = 0;
	MAIN_D_8013516C = 0;
	STD_addFighterCounter(0x63);
	MAIN_D_80135170 = 0;
	if (entity->type == other->type) {
		STD_func_80062D14();
	}

	resetFlattenGlobal();
	MAIN_D_80135178 = -1;
	COMBAT_DATA_PTR->player.remainingChargeupTime[0] = -1;
	MAIN_D_80135168 = PARTNER_ENTITY.digimonEntity.stats.current.chargeMode;
#if defined(VERSION_JP)
	FINISHING_ENTITY = NULL;
	MAIN_D_80135174 = 0;
	FLEE_DISABLED[1] = 0;
#else
	FLEE_DISABLED[1] = 0;
	FINISHING_ENTITY = NULL;
	MAIN_D_80135174 = 0;
#endif
	VS_P2_AOE_TIMER = 0;
	COMBAT_DATA_PTR->player.unk7 = 0;
	COMBAT_DATA_PTR->player.changeTarget = 0;
	NO_AI_FLAG = 0;
	BATTLE_TOGGLE_LIFEBAR = 0;
	FLEE_DISABLED[0] = 1;
	BATTLE_FRAME_COUNT = 1;
	MAIN_D_80135114 = 0;
	MAIN_D_80135118 = 0;
	for (i = 0; i < 2; i++) {
		COMBAT_DATA_PTR->player.unk5[i] = 0xff;
	}

	initializeAttackObjects();
	ENEMY_COUNT = 1;
	COMBAT_DATA_PTR->player.entityIds[0] = 1;
	COMBAT_DATA_PTR->player.entityIds[1] = STD_func_80062D5C(other);
	for (i = 0; i < 0xc; i++) {
		COMBAT_DATA_PTR->player.usedMoves[i] = 0xff;
	}

	COMBAT_DATA_PTR->player.startingHP = PARTNER_ENTITY.digimonEntity.stats.current.currentHP;
	MAIN_D_80135104 = 1;
	f = COMBAT_DATA_PTR->fighter;
	COMBAT_DATA_PTR->player.blockedCount = 0;
	COMBAT_DATA_PTR->player.hitCount = 0;
	COMBAT_DATA_PTR->player.unk2 = 0;
	COMBAT_DATA_PTR->player.statusedCount = 0;

	for (i = 0; i <= ENEMY_COUNT; i++) {
		digimon = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		stats = &digimon->stats;
		stats->current.isHit = 0;
		f->targetId = 0xff;
		f->hpDamageBuffer = 0;
		f->mpDamageBuffer = 0;
		f->flags = 0;
		f->moveRange = 0;
		COMBAT_DATA_PTR->player.unk1[i].unk25 = 0xff;
		f->flatTimer = 0;
		f->invulnerableTimer = 0;
		f->cooldown = 0;
		f->finisherProgress = 0;
		f->statusFxId = -1;
		f->unk11 = -1;
		f->speedBuffer = 0x64;
		f->unk15 = 0;
		f->unk16 = 0;
		if (stats->base.brain < 0x190) {
			f->buffsRemaining = (stats->base.brain / 100) + 1;
		} else if (stats->base.brain < 0x258) {
			f->buffsRemaining = 4;
		} else {
			f->buffsRemaining = 5;
		}
		f->buffPrioTimer = (stats->base.brain / 10) + 5;
		f->finisherGoal = 0xbb8 - stats->base.speed;
		for (j = 0; j < 0x96; j++) {
			f->table1[j] = -1;
			f->table2[i] = -1;
		}
		f++;
	}

	brains = PARTNER_ENTITY.digimonEntity.stats.base.brain;
	if (brains < 0x1f4) {
		COMBAT_DATA_PTR->player.numCommands[0] = MAIN_D_80134880[brains / 100];
	} else {
		COMBAT_DATA_PTR->player.numCommands[0] = 7;
	}

	COMBAT_DATA_PTR->player.availableCommands[0][0] = 0xb;

	switch (COMBAT_DATA_PTR->player.numCommands[0]) {
	case 2:
		COMBAT_DATA_PTR->player.availableCommands[0][1] = 3;
		break;
	case 3:
		COMBAT_DATA_PTR->player.availableCommands[0][1] = 2;
		COMBAT_DATA_PTR->player.availableCommands[0][2] = 3;
		break;
	case 4:
		COMBAT_DATA_PTR->player.availableCommands[0][1] = 4;
		COMBAT_DATA_PTR->player.availableCommands[0][2] = 2;
		COMBAT_DATA_PTR->player.availableCommands[0][3] = 3;
		break;
	case 5:
		COMBAT_DATA_PTR->player.availableCommands[0][1] = 5;
		COMBAT_DATA_PTR->player.availableCommands[0][2] = 4;
		COMBAT_DATA_PTR->player.availableCommands[0][3] = 2;
		COMBAT_DATA_PTR->player.availableCommands[0][4] = 3;
		break;
	case 6:
		COMBAT_DATA_PTR->player.availableCommands[0][1] = 6;
		COMBAT_DATA_PTR->player.availableCommands[0][2] = 5;
		COMBAT_DATA_PTR->player.availableCommands[0][3] = 4;
		COMBAT_DATA_PTR->player.availableCommands[0][4] = 2;
		COMBAT_DATA_PTR->player.availableCommands[0][5] = 3;
		break;
	case 7:
		COMBAT_DATA_PTR->player.availableCommands[0][1] = 6;
		COMBAT_DATA_PTR->player.availableCommands[0][2] = 5;
		i = 3;
		if (PARTNER_ENTITY.digimonEntity.stats.base.moves[2] != 0xff) {
			COMBAT_DATA_PTR->player.availableCommands[0][i++] = 0xa;
		}
		if (PARTNER_ENTITY.digimonEntity.stats.base.moves[1] != 0xff) {
			COMBAT_DATA_PTR->player.availableCommands[0][i++] = 9;
		}
		if (PARTNER_ENTITY.digimonEntity.stats.base.moves[0] != 0xff) {
			COMBAT_DATA_PTR->player.availableCommands[0][i++] = 8;
		}
		COMBAT_DATA_PTR->player.availableCommands[0][i++] = 3;
		COMBAT_DATA_PTR->player.numCommands[0] = i;
		break;
	}

	COMBAT_DATA_PTR->player.hoveredCommand[0] = COMBAT_DATA_PTR->player.numCommands[0] - 1;
	COMBAT_DATA_PTR->player.currentCommand[0] = COMBAT_DATA_PTR->player.bufferedCommand[0] = 3;

	for (i = 0; i <= ENEMY_COUNT; i++) {
		stats = &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]])->stats;
		(&MAIN_D_801350FC)[i] = stats->current.currentHP;
		dst = (int16_t *)&INITIAL_COMBAT_STATS[i];
		*dst++ = stats->base.hp;
		*dst++ = stats->base.mp;
		*dst++ = stats->base.off;
		*dst++ = stats->base.def;
		*dst++ = stats->base.speed;
		*dst = stats->base.brain;
	}

	if ((PARTNER_PARA.condition & 0x60) != 0) {
		stats = &PARTNER_ENTITY.digimonEntity.stats;
		stats->base.off -= (int16_t)(stats->base.off / 5);
		stats->base.def -= (int16_t)(stats->base.def / 5);
		stats->base.speed -= (int16_t)(stats->base.speed / 5);
		stats->base.brain -= (int16_t)(stats->base.brain / 5);
	}

	STD_func_8006AD00(0);
	STD_func_8006AD00(1);
	STD_addCommandMenu(0);
}

void STD_func_8005FDDC(void)
{
	long i;
	CombatData *combat;
	Entity *entity;
	Entity *partner;
	int16_t anim;
	FighterData *fighter;
	uint16_t *flagsPtr;
	Stats *stats;

	combat = COMBAT_DATA_PTR;
	partner = ENTITY_TABLE[1];
	if ((BATTLE_FRAME_COUNT % 20) == 0) {
		for (i = 0; i <= ENEMY_COUNT; i++) {
			if (combat->fighter[i].buffPrioTimer != 0) {
				combat->fighter[i].buffPrioTimer--;
			}
		}
	}

	for (i = 1; i <= ENEMY_COUNT; i++) {
		fighter = &combat->fighter[i];
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		stats = &((DigimonEntity *)entity)->stats;
		flagsPtr = &fighter->flags;
		stats->base.movesPrio[0] = 30;
		stats->base.movesPrio[1] = 30;
		stats->base.movesPrio[2] = 30;
		stats->base.movesPrio[3] = 0;
		if (NO_AI_FLAG == 0) {
			if (stats->current.currentHP > fighter->hpDamageBuffer) {
				if (stats->base.brain < 0x12d) {
					if ((BATTLE_FRAME_COUNT % (((stats->base.brain / 2) + 1) * 20)) == 0) {
						if (((0x12c - stats->base.brain) / 4) > randomLimit(100)) {
							*flagsPtr |= 0x2000;
							fighter->senileTimer = 100;
						}
					}
				}
			}
			if (!(*flagsPtr & 0x2000)) {
				STD_increaseSpeedBuffer(fighter, stats);
			}
			if (fighter->cooldown >= 2) {
				fighter->cooldown--;
			}
		}
		if (*flagsPtr & 0x80b0) {
			continue;
		}
		if (stats->current.currentHP == 0) {
			STD_faintDigimon((DigimonEntity *)entity, fighter, i);
			if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP > COMBAT_DATA_PTR->fighter[0].hpDamageBuffer) {
				STD_func_8005A55C((DigimonEntity *)entity, 5, 0);
			}
			MAIN_D_80135118 = 1;
			continue;
		}
		if (stats->current.currentHP <= fighter->hpDamageBuffer) {
			handleBattleIdle((DigimonEntity *)entity, stats, *flagsPtr);
			fighter->moveRange = -1;
			STD_resetFlatten(i);
			STD_removeStatusEffects((DigimonEntity *)entity, fighter);
			fighter->flags &= 0xfff0;
			continue;
		}
		if (((DigimonEntity *)partner)->stats.current.currentHP == 0) {
			handleBattleIdle((DigimonEntity *)entity, stats, *flagsPtr);
			STD_resetFlatten(i);
			*flagsPtr &= 0xff4f;
			*flagsPtr |= 0x40;
			fighter->moveRange = -1;
			continue;
		}
		if (NO_AI_FLAG != 0) {
			return;
		}
		if (*flagsPtr & 0x40) {
			continue;
		}
		if (*flagsPtr & 8) {
			fighter->queuedAnim = 0;
			fighter->targetId = 0;
			fighter->moveRange = 2;
			startAnimation(entity, 0x23);
			entity->flatSprite = 0;
			fighter->flags |= 0x40;
			continue;
		}
		if (*flagsPtr & 4) {
			continue;
		}
		if (*flagsPtr & 2) {
			STD_selectConfusedMove((DigimonEntity *)entity, fighter, i);
			continue;
		}
		if (*flagsPtr & 0x2000) {
			continue;
		}
		if (*flagsPtr & 0x800) {
			continue;
		}
		if (*flagsPtr & 0x1000) {
			continue;
		}
		if (!(*flagsPtr & 0x980e) && (fighter->flatTimer == 0)) {
			anim = ((DigimonEntity *)entity)->stats.base.moves[3];
			if (anim != 0xff) {
				anim = entityGetTechFromAnim(entity, anim);
				if ((anim >= 0x3a) && (anim < 0x71) && (fighter->finisherProgress == fighter->finisherGoal)) {
					fighter->targetId = 0;
					STD_setupQueuedMove((DigimonEntity *)entity, fighter, i, 3);
					fighter->finisherProgress = 0;
					continue;
				}
			}
		}
		if (!(*flagsPtr & 0x400)) {
			fighter->targetId = 0;
		}
		if (randomLimit(10) == 0) {
			stats->current.chargeMode = 2;
		} else {
			stats->current.chargeMode = randomLimit(2);
		}
		STD_func_80067744((DigimonEntity *)entity, fighter, i);
	}
}

void STD_func_800602A8(void)
{
	long id;
	DigimonEntity *entity;
	DigimonEntity *target;
	CombatData *combat;
	long i;
	FighterData *fighter;
	uint16_t *flags;

	if (FLEE_DISABLED[1] > 0) {
		FLEE_DISABLED[1]--;
	}

	if (VS_P2_AOE_TIMER > 0) {
		VS_P2_AOE_TIMER--;
	}

	combat = COMBAT_DATA_PTR;
	for (i = 0; ENEMY_COUNT >= i; i++) {
		fighter = &combat->fighter[i];
		flags = &fighter->flags;
		entity = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		STD_addFinisherProgress(fighter, 1);
		id = fighter->targetId;

		if (id != 0xff) {
			target = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]];
		} else {
			target = NULL;
		}
		if (*flags & 0x20) {
			STD_tickAttackState(&entity->entity, target, i);
		} else if ((*flags & 0x10) || (*flags & 0x80)) {
			STD_tickHitState(&entity->entity, fighter, i);
		} else if (fighter->moveRange != -1) {
			if (*flags & 8) {
				STD_tickFlatState(entity, target, fighter, i);
			} else if (*flags & 4) {
				STD_tickStunState(&entity->entity);
			} else if (*flags & 2) {
				STD_tickConfusedState(entity, target, fighter, i);
			} else if (*flags & 0x2000) {
				STD_func_800615D8(entity, fighter);
			} else if (*flags & 0x800) {
				STD_tickChargeState(entity, target, fighter);
			} else if (*flags & 0x1000) {
				STD_tickCooldownState(entity, target, fighter);
			} else {
				STD_tickQueuedMove(entity, target, fighter, i);
			}
		}
	}

	STD_func_800647F8();
	STD_applyMoveResult();
	if (MAIN_D_801350EC != 7 && MAIN_D_801350EC != 8) {
		if (MAIN_D_8013512C != 0) {
			MAIN_D_8013512C--;
			if (MAIN_D_8013512C == 0) {
				MAIN_D_801350EC = 1;
			}
		}
		if ((BATTLE_FRAME_COUNT % 600) == 0 && randomLimit(2) == 1) {
			MAIN_D_801350EC = 6;
			STD_setRandomViewpoint(ENTITY_TABLE[1], 4);
			MAIN_D_8013512C = randomLimit(0x29) + 0x3c;
		}
	}

	for (i = 0; ENEMY_COUNT >= i; i++) {
		if (COMBAT_DATA_PTR->fighter[i].flags & 0x20) {
			break;
		}
		if (COMBAT_DATA_PTR->fighter[i].flags & 0x10) {
			break;
		}
		if (MAIN_D_8013512C != 0) {
			break;
		}
		if (MAIN_D_801350EC == 7) {
			break;
		}
		if (MAIN_D_801350EC == 8) {
			break;
		}
	}

	if (i == ENEMY_COUNT + 1) {
		MAIN_D_801350EC = 1;
	}
}

int16_t STD_func_80060620(int16_t a, int16_t b)
{
	Stats *stats;
	int32_t i;
	int32_t k;

	PARTNER_ENTITY.digimonEntity.stats.current.chargeMode = MAIN_D_80135168;
	GAME_STATE = 5;
	STD_func_80060998();
	for (i = 0; i <= ENEMY_COUNT; i++) {
		removeEntityText(i);
		STD_resetFlatten(i);
		STD_removeStatusEffects((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]], &COMBAT_DATA_PTR->fighter[i]);
		COMBAT_DATA_PTR->fighter[i].flags = 0;
	}
	if (a == b) {
		if (MAIN_D_80135110 != 0 || MAIN_D_8013516C == 0) {
			STD_func_8006B2BC();
			STD_func_8006B468();
			while (STD_isVersusModelSceneFinished() == 0) {
				STD_battleTickFrame();
			}
			STD_func_8006B6E8();
		}
	} else {
		if (a == 0) {
			MAIN_D_80135170 = 0x64;
		} else {
			MAIN_D_80135170 = 0x78;
		}
	}
	if (a != b) {
		startAnimation(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[a]], 0x2a);
		k = 0;
		stopBGM();
		stopSound();
		if (a == 0) {
			playMusic(MAIN_D_801350F8, 3);
		} else {
			playMusic(MAIN_D_801350F8, 4);
		}
		for (; k < MAIN_D_80135170; k++) {
			STD_battleTickFrame();
			if ((ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[a]]->anim.animFlag & 1) == 0) {
				startAnimation(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[a]], 0x2a);
			}
		}
	}
	MAIN_D_80135104 = 0;
	for (i = 0; i < 0x14; i++) {
		STD_battleTickFrame();
	}
	STD_func_8006B1E4(0);
	STD_func_8006B1E4(1);
	STD_removeCommandMenu(0);
	STD_removeFighterCounter();
	stopBGM();
	stopSound();
	stats = &((DigimonEntity *)ENTITY_TABLE[1])->stats;
	stats->base.off = INITIAL_COMBAT_STATS[0][2];
	stats->base.def = INITIAL_COMBAT_STATS[0][3];
	stats->base.speed = INITIAL_COMBAT_STATS[0][4];
	stats->base.brain = INITIAL_COMBAT_STATS[0][5];
	stats->current.currentHP += (int16_t)(stats->base.hp / 5);
	stats->current.currentMP += (int16_t)(stats->base.mp / 5);
	if (stats->current.currentHP > stats->base.hp) {
		stats->current.currentHP = stats->base.hp;
	}
	if (stats->current.currentMP > stats->base.mp) {
		stats->current.currentMP = stats->base.mp;
	}

	return (a == 0) ? 1 : -1;
}

void STD_func_80060AA0(void)
{
	removeObject(0x1a3, 0);
	removeObject(0x1a3, 1);
}

void STD_resetFlatten(int16_t index)
{
	Entity *entity;
	FighterData *fighter;

	entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]];
	fighter = &COMBAT_DATA_PTR->fighter[index];
	entity->flatSprite = -1;
	fighter->flags &= 0xfff7;
	fighter->flatTimer = 0;
	if (entity->type != 0x71) {
		entity->posData->scale.vx = 0x1000;
		entity->posData->scale.vy = 0x1000;
		entity->posData->scale.vz = 0x1000;
	} else {
		entity->posData->scale.vx = 0x1800;
		entity->posData->scale.vy = 0x1800;
		entity->posData->scale.vz = 0x1800;
	}
}

int32_t STD_func_80060B98(void)
{
	int32_t i;

	for (i = 1; i <= ENEMY_COUNT; ++i) {
		DigimonEntity *digimon = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];

		if (digimon->stats.current.currentHP - COMBAT_DATA_PTR->fighter[i].hpDamageBuffer > 0) {
			return 0;
		}
	}

	return 1;
}

void STD_func_80060C14(int16_t hasLostP1, uint8_t hasLostP2)
{
	DigimonEntity *e0;
	DigimonEntity *e1;

	MAIN_D_80135172 = 0;
	addObject(0x1a2, 0, NULL, (RenderFunction)STD_func_8006324C);
	stopBGM();
	if (hasLostP1 == hasLostP2) {
		DigimonEntity *p0;
		DigimonEntity *p1;

		MAIN_D_8013516C = 1;
		p0 = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]];
		p1 = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]];
		handleBattleIdle(p0, &p0->stats, COMBAT_DATA_PTR->fighter[0].flags);
		handleBattleIdle(p1, &p1->stats, COMBAT_DATA_PTR->fighter[1].flags);
		STD_battleTickFrame();
		STD_battleTickFrame();
		STD_func_8006B2BC();
		STD_func_80060EBC();
		while (MAIN_D_80135172 < 0x3d) {
			if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON) {
				break;
			}
			STD_battleTickFrame();
			MAIN_D_80135172++;
		}
		removeAnimatedUIBox(0, 0);
		STD_func_8006B468();
		while (STD_isVersusModelSceneFinished() == 0) {
			STD_battleTickFrame();
		}
		STD_func_8006B6E8();
		return;
	}
	e0 = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[hasLostP1]];
	e1 = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[hasLostP2]];
	handleBattleIdle(e0, &e0->stats, COMBAT_DATA_PTR->fighter[hasLostP1].flags);
	STD_faintDigimon(e1, &COMBAT_DATA_PTR->fighter[hasLostP2], hasLostP2);
	while (MAIN_D_80135172 < 0x79) {
		if (MAIN_D_80135172 >= 0x3d) {
			if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON) {
				break;
			}
		}
		if (MAIN_D_80135172 == 0x3c) {
			STD_func_80060EBC();
		}
		STD_battleTickFrame();
		MAIN_D_80135172++;
	}
	STD_func_8005A55C(e1, 5, 0);
	entityLookAtLocation(&e0->entity, &e1->entity.posData->location);
	removeAnimatedUIBox(0, 0);
}

void STD_func_80060EBC(void)
{
	RECT finalPos;
	RECT startPos;
	Stats *stats;

	clearTextArea();
	drawString(STD_STR_ATAETA, 6, 0);
	drawString(STD_STR_DAMEEJI, 0, 12);
	drawString(PARTNER_ENTITY.name, (120 - strlen(PARTNER_ENTITY.name) * 6) / 2, 24);
	drawString(DIGIMON_DATA[ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->type].name, (120 - strlen(DIGIMON_DATA[ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->type].name) * 6) / 2, 36);
	DrawSync(0);
	removeObject(0x1a2, 0);
	stats = &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]])->stats;

	setRECT(&startPos, -10, -10, 20, 20);

	setRECT(&finalPos, -132, -27, 264, 54);

	createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, NULL, (RenderFunction)STD_func_80063300);
}

void STD_faintDigimon(DigimonEntity *digimon, FighterData *fighter, int16_t arg2)
{
	digimon->stats.current.isHit = 1;
	fighter->flags |= 0x8000;
	startAnimation(&digimon->entity, 0x2b);
	STD_resetFlatten(arg2);
	STD_removeStatusEffects(digimon, fighter);
	fighter->flags &= 0xff40;
	fighter->flags |= 0x40;
	fighter->moveRange = -1;
	STD_resetFighterAction(fighter);
}

int32_t STD_func_80061124(int32_t value)
{
	if (value < 10) {
		return 1;
	}
	if (value < 100) {
		return 2;
	}
	if (value < 1000) {
		return 3;
	}

	return 4;
}

void STD_tickAttackState(Entity *entity, DigimonEntity *target, int32_t id)
{
	int32_t i;
	Entity *other;

	if ((entity->anim.animFlag & 1) == 0) {
		if (entity->anim.frameCount != entity->anim.animFrame) {
			for (i = 0; i <= ENEMY_COUNT; ++i) {
				if (i != id) {
					other = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
					if (DIGIMON_DATA[other->type].moves[(uint32_t)(other->anim.animId - 0x2e)] == 0x2d) {
						if (id == COMBAT_DATA_PTR->fighter[i].targetId) {
							return;
						}
					}
				}
			}
			if (i) {
			}
			entity->anim.animFlag |= 1;
		}
	}

	STD_tickFighterAction(id);
}

void STD_tickHitState(Entity *entity, FighterData *fighter, int32_t arg2)
{
	STD_tickFighterAction(arg2);
	if (!(entity->anim.animFlag & 1)) {
		fighter->invulnerableTimer--;
		if (fighter->invulnerableTimer == 0) {
			entity->anim.animFlag |= 1;
		}
	}
}

void STD_tickFlatState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3)
{
	if (NO_AI_FLAG != 0) {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		return;
	}

	if (STD_func_80061AA8(digimon, target, fighter) == 0) {
		STD_func_80061F44(digimon, target, fighter, 0x79);
		if (fighter->flags & 0x20) {
			digimon->entity.flatSprite = 2;
		}
	}
}

void STD_tickStunState(Entity *entity)
{
	if (entity->anim.animId != 0x22) {
		startAnimation(entity, 0x22);
	}

	entity->anim.animFlag &= 0xfe;
}

void STD_tickConfusedState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3)
{
	if (NO_AI_FLAG != 0) {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		return;
	}

	if ((fighter->flags & 0x1000) || (fighter->flags & 0x800)) {
		STD_confusedRotate(&digimon->entity);
		STD_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
		collisionGrace(NULL, &digimon->entity, 0x118, 0xc8);
		if (fighter->cooldown < 2) {
			fighter->flags &= 0xefff;
			fighter->cooldown = 0;
		}
		return;
	}

	if (target == NULL) {
		STD_confusedRotate(&digimon->entity);
		if (STD_tickMeleeAttack(digimon, NULL, fighter, arg3) != 0) {
			collisionGrace(NULL, &digimon->entity, 0x118, 0xc8);
		}
		if (fighter->flags & 0x20) {
			return;
		}
		if (randomLimit(100) >= 5) {
			return;
		}
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		STD_func_80065540(digimon, target, fighter);
		return;
	}

	switch (fighter->moveRange) {
	case 1:
		if (STD_tickMeleeAttack(digimon, target, fighter, arg3) != 0) {
			collisionGrace(&target->entity, &digimon->entity, 0x118, 0xc8);
		}
		break;
	case 2:
	case 3:
		STD_func_80061F44(digimon, target, fighter, entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim));
		break;
	case 4:
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		STD_func_80065540(digimon, target, fighter);
		break;
	}
}

void STD_func_800615D8(DigimonEntity *digimon, FighterData *fighter)
{
	fighter->senileTimer--;
	if (fighter->senileTimer == 0) {
		fighter->flags &= 0xdfbf;
	} else {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
	}
}

void STD_tickChargeState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	int32_t r;
	int16_t tech;

	if (NO_AI_FLAG != 0) {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		return;
	}

	r = STD_func_80061AA8(digimon, target, fighter);
	if (fighter->cooldown != 0) {
		if (r == 0) {
			switch (digimon->stats.current.chargeMode) {
			case 0:
				STD_maintainTargetDistance(digimon, target, fighter);
				break;
			case 1:
				handleBattleIdle(digimon, &digimon->stats, fighter->flags);
				entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
				fighter->unk16 = 0;
				break;
			case 2:
				STD_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
				STD_backAwayFromTarget(digimon, target, fighter);
				break;
			}
		}
		if (fighter->cooldown < 2) {
			fighter->flags &= 0xf7bf;
			fighter->cooldown = 0;
		}
		return;
	}

	if (r != 0) {
		return;
	}

	switch (digimon->stats.current.chargeMode) {
	case 0:
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
		fighter->unk16 = 0;
		if (fighter->speedBuffer > 0) {
			fighter->flags &= 0xf7ff;
		}
		break;
	case 1:
		STD_maintainTargetDistance(digimon, target, fighter);
		tech = entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim);
		if ((fighter->speedBuffer == 100) || (fighter->speedBuffer >= MOVE_DATA[tech].power)) {
			fighter->flags &= 0xf7ff;
		}
		break;
	case 2:
		STD_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
		STD_backAwayFromTarget(digimon, target, fighter);
		if (fighter->speedBuffer == 100) {
			fighter->flags &= 0xf7ff;
		}
		break;
	}
}

void STD_tickCooldownState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	if (NO_AI_FLAG != 0) {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		return;
	}

	switch (digimon->stats.current.chargeMode) {
	case 0:
	case 1:
		STD_maintainTargetDistance(digimon, target, fighter);
		break;
	case 2:
		STD_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
		STD_backAwayFromTarget(digimon, target, fighter);
		break;
	}

	if (fighter->cooldown < 2) {
		fighter->flags &= ~0x1040;
		fighter->cooldown = 0;
	}
}

void STD_tickQueuedMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3)
{
	if ((NO_AI_FLAG != 0) && (FINISHING_ENTITY != &digimon->entity)) {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		return;
	}

	if (STD_func_80061AA8(digimon, target, fighter) == 0) {
		switch (fighter->moveRange) {
		case 1:
			if (STD_tickMeleeAttack(digimon, target, fighter, arg3) != 0) {
				collisionGrace(&target->entity, &digimon->entity, 0x118, 0xc8);
			}
			break;
		case 2:
		case 3:
			STD_func_80061F44(digimon, target, fighter, DIGIMON_DATA[digimon->entity.type].moves[fighter->queuedAnim - 0x2e]);
			break;
		case 4:
			handleBattleIdle(digimon, &digimon->stats, fighter->flags);
			STD_func_80065540(digimon, target, fighter);
			break;
		}
	}
}

int32_t STD_func_80061AA8(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	uint32_t range;

	if (NO_AI_FLAG != 0) {
		return 0;
	}

	if (digimon == (DigimonEntity *)ENTITY_TABLE[1]) {
		switch (COMBAT_DATA_PTR->player.currentCommand[0]) {
		case 6:
			handleBattleIdle(digimon, &digimon->stats, fighter->flags);
			entityLookAtLocation(&digimon->entity, (VECTOR *)((char *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighter->targetId]]->posData + 0x78));
			fighter->unk16 = 0;
			return 1;
		case 5:
			range = STD_getMoveWithHighestDistance(target) + 0x9c400;
			if ((digimon->entity.anim.animId >= 0x23) && (digimon->entity.anim.animId < 0x25)) {
				range += 0x27100;
			}

			if ((uint32_t)STD_getDistanceSquared(&digimon->entity, &target->entity) < range) {
				STD_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
				STD_backAwayFromTarget(digimon, target, fighter);
			} else {
				fighter->unk16 = 0;
				handleBattleIdle(digimon, &digimon->stats, fighter->flags);
				entityLookAtLocation(&digimon->entity, (VECTOR *)((char *)target->entity.posData + 0x78));
			}

			return 1;
		}
	}

	return 0;
}

int32_t STD_tickMeleeAttack(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int16_t arg3)
{
	int16_t *rot;
	PositionData *pos;
	uint32_t dist;
	int32_t radius;
	int16_t orig;
	int16_t tech;

	pos = digimon->entity.posData;
	rot = &pos->rotation.vy;
	digimon->entity.anim.animFlag &= 0xfd;
	orig = *rot;
	if (target != NULL) {
		dist = STD_getDistanceSquared(&digimon->entity, &target->entity);
		radius = DIGIMON_DATA[digimon->entity.type].radius + DIGIMON_DATA[target->entity.type].radius;
		radius = radius * radius;
		if (dist <= radius) {
			handleBattleIdle(digimon, &digimon->stats, fighter->flags);
			if (NO_AI_FLAG != 0) {
				if (FINISHING_ENTITY != &digimon->entity) {
					return 0;
				}
				if (MAIN_D_80135174 > 0) {
					MAIN_D_80135174--;
					entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
					return 0;
				}
			} else {
				tech = entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim);
				if ((tech >= 0x3a) && (tech < 0x71)) {
					NO_AI_FLAG = 1;
				}
				if (NO_AI_FLAG != 0) {
					FINISHING_ENTITY = &digimon->entity;
					if (digimon == (DigimonEntity *)ENTITY_TABLE[1]) {
						STD_func_80069134(tech);
					}
					startAnimation(&digimon->entity, fighter->queuedAnim);
					digimon->entity.anim.animFlag &= 0xfe;
					MAIN_D_80135178 = STD_addFinisherAura(&digimon->entity, 0x50);
					MAIN_D_80135174 = 0x50;
					return 0;
				}
			}
			if (STD_selectMoveTarget(&digimon->entity, fighter) != 0) {
				return 0;
			}
			startAnimation(&digimon->entity, fighter->queuedAnim);
			fighter->flags |= 0x20;
			STD_func_800658B4(digimon, target, fighter);
			return 0;
		}
		if (NO_AI_FLAG == 0) {
			STD_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			goto check;
		}
		if (FINISHING_ENTITY != &digimon->entity) {
			return 0;
		}
		if (MAIN_D_80135174 > 0) {
			MAIN_D_80135174--;
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			return 0;
		}
		startAnimation(&digimon->entity, fighter->queuedAnim);
		fighter->flags |= 0x20;
		STD_func_800658B4(digimon, target, fighter);
		return 0;
	}

	STD_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
check:
	if (entityCheckCollision(NULL, &digimon->entity, 0x118, 0xc8) == -1) {
		return 0;
	} else {
		*rot = orig;
		return 1;
	}
}

void STD_func_80061F44(DigimonEntity *entity, DigimonEntity *other,
                       FighterData *data, int16_t move)
{
	uint32_t distance;
	uint32_t total;

	if (data->unk15 > 100) {
		handleBattleIdle(entity, &entity->stats, data->flags);

		++data->unk15;
		if (data->unk15 > 160) {
			data->flags &= 0xffbf;
			data->unk15 = 0;
		}

		return;
	}

	if (NO_AI_FLAG != 0 && &entity->entity == FINISHING_ENTITY) {
		STD_func_80065540(entity, other, data);
		return;
	}

	distance = STD_getDistanceSquared(&entity->entity, &other->entity);
	total = DIGIMON_DATA[entity->entity.type].radius + DIGIMON_DATA[other->entity.type].radius;
	total = *(uint32_t *)&MOVE_DATA[move].distance + total * total;

	if (distance > total + total * 3 / 10) {
		STD_setWalking(&entity->entity, &entity->stats, data->flags);
		STD_moveTowardLocation(entity, &other->entity.posData->location,
		                       280, 200);
		++data->unk15;
	} else if (distance < total - total * 3 / 10) {
		STD_setWalking(&entity->entity, &entity->stats, data->flags);
		STD_backAwayFromTarget(entity, other, data);
		++data->unk15;
	} else {
		data->unk15 = 0;

		handleBattleIdle(entity, &entity->stats, data->flags);

		if ((data->flags & 8) != 0) {
			if (BATTLE_FRAME_COUNT % 40 == 0) {
				STD_func_80065540(entity, other, data);
			} else {
				entityLookAtLocation(&entity->entity,
				                     &other->entity.posData->location);
			}
		} else {
			STD_func_80065540(entity, other, data);
		}
	}
}

uint32_t STD_getMoveWithHighestDistance(DigimonEntity *digimon)
{
	uint32_t best;
	int32_t i;
	uint8_t tech;
	uint8_t anim;

	best = 0;
	for (i = 0; i < 4; i++) {
		anim = digimon->stats.base.moves[i];
		if (anim != 0xff) {
			tech = entityGetTechFromAnim(&digimon->entity, anim);
			if (best < MOVE_DATA[tech].distance) {
				best = MOVE_DATA[tech].distance;
			}
		}
	}

	return best;
}

void STD_setWalking(Entity *entity, Stats *stats, uint16_t flags)
{
	if ((entity->anim.animId != 0x24) && (entity->anim.animId != 0x23)) {
		STD_startWalkingAnimation(entity, stats, flags);
	}
}

void STD_backAwayFromTarget(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	int16_t *targetRot;
	int16_t *rot;
	int16_t orig;
	int16_t away;
	int16_t ccw;
	int16_t cw;
	int16_t hit;
	int16_t diff;

	rot = &digimon->entity.posData->rotation.vy;
	orig = *rot;
	entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
	away = (*rot + 0x800) & 0xfff;

	if (fighter->unk16 == 0) {
		*rot = away;

		hit = entityCheckCollision(NULL, &digimon->entity, 0x118, 0xc8);
		if (hit != -1) {
			fighter->unk16 = 1;
			STD_findUnblockedRotation(&digimon->entity, rot, hit, orig);
		}

		return;
	}

	*rot = orig;
	targetRot = &target->entity.posData->rotation.vy;
	if (orig == *targetRot) {
		*rot = (*rot + 0x400) & 0xfff;

		if (entityCheckCollision(NULL, &digimon->entity, 0x118, 0xc8) == -1) {
			return;
		}

		*rot = orig;
		*rot = (*rot + 0xc00) & 0xfff;

		if (entityCheckCollision(NULL, &digimon->entity, 0x118, 0xc8) == -1) {
			return;
		}

		*rot = orig;

		hit = entityCheckCollision(NULL, &digimon->entity, 0x118, 0xc8);
		if (hit != -1) {
			STD_findUnblockedRotation(&digimon->entity, rot, hit, orig);
		}

		return;
	}

	ccw = orig - away;
	if (ccw < 0) {
		ccw = (ccw + 0x1000) & 0xfff;
	}

	cw = away - orig;
	if (cw < 0) {
		cw = (cw + 0x1000) & 0xfff;
	}

	diff = ccw - cw;
	if (diff < 0) {
		if (ccw > 20) {
			*rot = (*rot + 0xfec) & 0xfff;
		} else {
			*rot = (*rot + 0x1000 - ccw) & 0xfff;
		}
	} else {
		if (cw > 20) {
			*rot = (*rot + 20) & 0xfff;
		} else {
			*rot = (*rot + cw) & 0xfff;
		}
	}

	if (entityCheckCollision(NULL, &digimon->entity, 0x118, 0xc8) == -1) {
		return;
	}

	*rot = orig;

	hit = entityCheckCollision(NULL, &digimon->entity, 0x118, 0xc8);
	if (hit != -1) {
		STD_findUnblockedRotation(&digimon->entity, rot, hit, orig);
	}
}

void STD_moveTowardLocation(DigimonEntity *digimon, VECTOR *target, int16_t dx, int16_t dy)
{
	int16_t facing;

	facing = digimon->entity.posData->rotation.vy;
	entityLookAtLocation(&digimon->entity, target);
	if (entityCheckCollision(NULL, &digimon->entity, dx, dy) != -1) {
		digimon->entity.posData->rotation.vy = facing;
		collisionGrace(NULL, &digimon->entity, dx, dy);
	}
}

void STD_tickFighterAction(int32_t index)
{
	DigimonEntity *digimon;
	FighterData *fighter;

	digimon = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]];
	fighter = &COMBAT_DATA_PTR->fighter[index];
	if (entityCheckCollision(NULL, &digimon->entity, 0x118, 0xc8) != -1) {
		digimon->entity.anim.animFlag |= 2;
	} else {
		digimon->entity.anim.animFlag &= 5;
	}

	if ((NO_AI_FLAG != 0) && (FINISHING_ENTITY == &digimon->entity) && (MAIN_D_80135174 > 0)) {
		return;
	}

	if ((fighter->flags & 0x28) == 0x28) {
		fighter->flatAttackTimer--;
		switch (fighter->flatAttackTimer) {
		case 0x1c:
			STD_addAuraProjectile(&digimon->entity);
			break;
		case 0xa:
			digimon->entity.flatSprite = 0;
			break;
		case 0:
			digimon->entity.anim.animFlag &= 0xfe;
			break;
		}
	}

	if (digimon->entity.anim.animFlag & 1) {
		return;
	}

	if (FINISHING_ENTITY == &digimon->entity) {
		NO_AI_FLAG = 0;
		FINISHING_ENTITY = NULL;
	}

	fighter->unk16 = 0;
	if (fighter->flags & 0x20) {
		if (index == 0) {
			COMBAT_DATA_PTR->player.hitCount++;
		}
		fighter->flags &= 0xfbff;
		fighter->flags |= 0x1000;
		fighter->cooldown = 0x28;
		STD_addFinisherProgress(fighter, fighter->finisherGoal * 2 / 50);
	}

	if (fighter->invulnerableTimer <= 0) {
		if (fighter->flags & 8) {
			digimon->entity.flatSprite = 0;
		}
		if (!(fighter->flags & 0x20)) {
			digimon->stats.current.isHit = 0;
		}
		if (fighter->flags & 0x80) {
			fighter->flags &= 0xff7f;
			STD_clearBlockedAttacks(fighter);
		} else {
			fighter->flags &= 0xff0f;
		}
	}

	if (!(fighter->flags & 0x10)) {
		if (fighter->flatTimer == -1) {
			fighter->flatTimer = 0x41;
		}
	} else {
		fighter->senileTimer = 0;
		fighter->flags &= 0xdfff;
	}
}

void STD_confusedRotate(Entity *entity)
{
	int16_t *rot;

	if (randomLimit(0xa) >= 8) {
		rot = &entity->posData->rotation.vy;
		*rot += randomLimit(0x400) - 0x200;
		if (*rot < 0) {
			*rot += 0x1000;
		} else {
			*rot %= 4096;
		}
	}
}

void STD_maintainTargetDistance(DigimonEntity *attacker, DigimonEntity *target, FighterData *fighter)
{
	if ((attacker->entity.anim.animId >= 0x23) && (attacker->entity.anim.animId < 0x25)) {
		STD_maintainDistanceRange(attacker, target, fighter, 0x27100, 0x4e200);
	} else {
		STD_maintainDistanceRange(attacker, target, fighter, 0, 0x75300);
	}
}

void STD_maintainDistanceRange(DigimonEntity *attacker, DigimonEntity *target, FighterData *fighter, uint32_t nearLimit, uint32_t farLimit)
{
	uint32_t dist;
	uint32_t reach;

	dist = STD_getDistanceSquared(&attacker->entity, &target->entity);
	reach = STD_getContactRangeSquared(&attacker->entity.type, &target->entity.type);
	if (dist < (reach + nearLimit)) {
		STD_setWalking(&attacker->entity, &attacker->stats, fighter->flags);
		STD_backAwayFromTarget(attacker, target, fighter);
	} else if (dist > (reach + farLimit)) {
		STD_setWalking(&attacker->entity, &attacker->stats, fighter->flags);
		STD_moveTowardLocation(attacker, &target->entity.posData->location, 0x118, 0xc8);
	} else {
		fighter->unk16 = 0;
		handleBattleIdle(attacker, &attacker->stats, fighter->flags);
		entityLookAtLocation(&attacker->entity, &target->entity.posData->location);
	}
}

int32_t STD_getContactRangeSquared(int32_t *a, int32_t *b)
{
	int32_t reach;

	reach = DIGIMON_DATA[*a].radius + DIGIMON_DATA[*b].radius + 0xc8;

	return reach * reach;
}

void STD_increaseSpeedBuffer(FighterData *fighter, Stats *stats)
{
	if (fighter->speedBuffer < 0x64) {
		if ((BATTLE_FRAME_COUNT % 2) == 0) {
			fighter->speedBuffer += (stats->base.speed / 100) + 1;
		}
		if (fighter->speedBuffer >= 0x65) {
			fighter->speedBuffer = 0x64;
		}
	}
}

int32_t STD_func_80062BD8(int16_t *out, int16_t index)
{
	DigimonEntity *digimon;
	FighterData *fighter;
	int32_t found;
	int32_t i;

	digimon = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]];
	fighter = &COMBAT_DATA_PTR->fighter[index];
	found = 0;

	for (i = 0; i < 4; ++i) {
		if (STD_isMoveUsable(digimon, fighter, i) != 0) {
			out[i] = 1;
			found = 1;
		} else {
			out[i] = 0;
		}
	}

	return found;
}

void STD_startWalkingAnimation(Entity *entity, Stats *stats, uint16_t flags)
{
	int32_t walking;
	int32_t animId;

	walking = 1;

	if (!(flags & 1)) {
		if (stats->current.currentHP > (stats->base.hp / 5)) {
			walking = 0;
		}
	}

	if (walking != 0) {
		animId = 0x23;
	} else {
		animId = 0x24;
	}

	startAnimation(entity, (uint8_t)animId);
}

void STD_func_80062D14(void)
{
	addObject(0x1a3, 0, 0, (RenderFunction)STD_func_80063508);
	addObject(0x1a3, 1, 0, (RenderFunction)STD_func_80063508);
}

int16_t STD_func_80062D5C(Entity *entity)
{
	int32_t i;

	for (i = 0; i < 10; i++) {
		if (ENTITY_TABLE[i] == entity) {
			return i;
		}
	}
}

void STD_clearBlockedAttacks(FighterData *fighter)
{
	int32_t i;

	for (i = 0; i < 0x96; i++) {
		if (fighter->table1[i] == -1) {
			break;
		}
		fighter->table1[i] = -1;
		fighter->table2[i] = -1;
	}
}

void STD_findUnblockedRotation(Entity *entity, int16_t *rot, int16_t hit, int16_t orig)
{
	int16_t cand[4];
	int32_t i;
	int16_t base;

	if ((GAME_STATE == 4) && (hit == 0xa)) {
		hit = 0xb;
	}

	if (hit != 0xb) {
		goto grace;
	}

	base = *rot / 1024;
	cand[0] = MAIN_D_80134870[base];
	cand[1] = (MAIN_D_80134870[base] + 0x400) & 0xfff;
	for (i = 0; i < 2; i++) {
		*rot = cand[i];
		if (entityCheckCollision(NULL, entity, 0x118, 0xc8) == -1) {
			break;
		}
	}

	switch (i) {
	case 0:
		for (i = 0; i < 3; i++) {
			*rot = cand[i] = (MAIN_D_80134870[base] + 0xc00 + (i * 0x200)) & 0xfff;
			if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
				goto common;
			}
		}
		break;
	case 1:
		for (i = 0; i < 3; i++) {
			*rot = cand[i] = (MAIN_D_80134870[base] + 0x400 + (i * 0x200)) & 0xfff;
			if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
				goto common;
			}
		}
		break;
	default:
		for (i = 0; i < 3; i++) {
			*rot = cand[i] = (MAIN_D_80134870[base] + 0x800 + (i * 0x200)) & 0xfff;
			if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
				break;
			}
		}
		if (i == 3) {
			*rot = (orig + 0x800) & 0xfff;
			if (entityCheckCollision(NULL, entity, 0x118, 0xc8) == -1) {
				return;
			}
		}
		break;
	}

common:
	if (i != 3) {
		*rot = orig;
		collisionGrace(NULL, entity, 0x118, 0xc8);
		return;
	}

	*rot = cand[0] + randomLimit(0x400);
	if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
		*rot = orig;
		collisionGrace(NULL, entity, 0x118, 0xc8);
	}

	return;
grace:
	*rot = orig;
	collisionGrace(NULL, entity, 0x118, 0xc8);
}

int16_t STD_func_8006314C(Entity *entity, Entity *other)
{
	int16_t result;

	COMBAT_AREA_X = 0;
	COMBAT_AREA_Y = 0;
	stopBGM();
	playMusic(MAIN_D_801350F8, 2);
	STD_func_8005E8A4(entity, other);
	STD_func_8005EF84();
	while (1) {
		result = STD_func_8005F354();
		if (result != 0) {
			break;
		}

		STD_func_8005F650();
		STD_func_8005FDDC();
		STD_func_800602A8();
		STD_battleTickFrame();
		handlePause();
	}

	removePauseBox();
	if (result == -1) {
		STD_func_80060620(1, 0);
	} else if (result == 1) {
		STD_func_80060620(0, 1);
	} else {
		result = STD_func_80060620(1, 1);
	}

	return result;
}

void STD_func_8006324C(void)
{
	POLY_FT4 *prim;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->tpage = getTPage(0, 0, 832, 0);
	prim->clut = GetClut(0x10, 0x1e0);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setUVDataPolyFT4(prim, 0, 8, 0x90, 0x18);
	setPosDataPolyFT4(prim, -0x48, -0xc, 0x90, 0x18);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[5], prim++);
	GsSetWorkBase((PACKET *)prim);
}

void STD_func_80063300(int32_t id)
{
	int16_t x;
	int16_t y;
	int32_t digits;

	x = UI_BOX_DATA[id].finalPos.x;
	y = UI_BOX_DATA[id].finalPos.y;
	renderString(0, x + 6, y + 6, 120, 12, 0, 24, 6 - id, 1);
	renderString(0, x + 138, y + 6, 120, 12, 0, 36, 6 - id, 1);
	renderString(0, x + 108, y + 24, 48, 24, 0, 0, 6 - id, 1);

	digits = STD_func_80061124(MAIN_D_80135102);
	STD_func_800593D0(x + 42 + (48 - digits * 12) / 2, y + 30, digits,
	                  MAIN_D_80135102, 6 - id);
	digits = STD_func_80061124(MAIN_D_80135100);
	STD_func_800593D0(x + 174 + (48 - digits * 12) / 2, y + 30, digits,
	                  MAIN_D_80135100, 6 - id);
}

void STD_startWalkingAnimation2(Entity *entity, Stats *stats, uint16_t flags)
{
	int32_t walking;
	int32_t animId;

	walking = 1;

	if (!(flags & 1)) {
		if (stats->current.currentHP > (stats->base.hp / 5)) {
			walking = 0;
		}
	}

	if (walking != 0) {
		animId = 0x22;
	} else {
		animId = 0x21;
	}

	startAnimation(entity, (uint8_t)animId);
}

void STD_func_80063508(int16_t id)
{
	POLY_FT4 *prim;
	MATRIX *m;
	SVECTOR pos;
	Entity *entity;
	uint32_t otz;
	int32_t offset;
	int16_t sxy[2];

	entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]];
	GsSetLsMatrix(&GsWSMATRIX);
	m = &entity->posData[1].posMatrix.workm;
	pos.vx = m->t[0];
	pos.vy = m->t[1];
	pos.vz = m->t[2];
	gte_ldv0(&pos);
	gte_rtps();
	gte_stsxy(sxy);
	gte_stszotz(&otz);
	sxy[0] -= (int16_t)(160 - DRAWING_OFFSET_X);
	sxy[1] -= (int16_t)(120 - DRAWING_OFFSET_Y);
	offset = VIEWPORT_DISTANCE * (DIGIMON_DATA[entity->type].radius / 2) / (otz * 4);
	sxy[0] += (int16_t)offset;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->tpage = getTPage(0, 0, 768, 0);
	prim->clut = GetClut(0, 0x1e1);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setUVWH(prim, id * 48 + 0xa0, 0xe0, (id == 0 ? 0x30 : 0x2f), 31);
	setXYWH(prim, sxy[0], sxy[1] - 0x20, (id == 0 ? 0x30 : 0x2f), 31);
	AddPrim(ACTIVE_ORDERING_TABLE->org + otz, prim++);

	GsSetWorkBase((PACKET *)prim);
}

void STD_func_800647F8(void)
{
	long j;
	FighterData *fighter;
	Entity *entity;
	long i;
	int32_t dmg;
	int32_t chance;
	int32_t tech;
	uint8_t *moves;
	VECTOR loc;
	FighterData *fighters;
	PlayerDataSub *sub;
	AttackObject attack;
	Entity *attacker;
	int32_t handled;

	fighter = COMBAT_DATA_PTR->fighter;
	fighters = fighter;
	sub = &COMBAT_DATA_PTR->player.unk1[0];
	for (i = 0; i <= ENEMY_COUNT; i++, fighter++, sub++) {
		if (fighter->flags & 0x8000) {
			continue;
		}
		if (popAttackObject(COMBAT_DATA_PTR->player.entityIds[i], &attack) == 0) {
			continue;
		}
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		tech = STD_getAttackTech(&attack);
		if (DIGIMON_DATA[entity->type].moves[(uint32_t)(entity->anim.animId - 0x2e)] == 0x2d) {
			if (COMBAT_DATA_PTR->player.entityIds[fighter->targetId] == attack.casterId) {
				((DigimonEntity *)entity)->stats.current.isHit = 0;
				continue;
			}
		}
		if (STD_applyBuffMove((DigimonEntity *)entity, (int16_t)i, (int16_t)tech) != 0) {
			((DigimonEntity *)entity)->stats.current.isHit = 0;
			continue;
		}
		STD_removeMoveEffect((DigimonEntity *)entity, fighter);
		fighter->flags &= 0xff8f;
		attacker = ENTITY_TABLE[attack.casterId];
		chance = STD_applyPartnerStatsToFighter((DigimonEntity *)attacker, (DigimonEntity *)entity, fighter, tech);
		if (entity == FINISHING_ENTITY) {
			if (i == 0) {
				STD_removeFinisherChargeup();
			}
			if (MAIN_D_80135178 != -1L) {
				STD_removeFinisherAura(MAIN_D_80135178);
			}
			NO_AI_FLAG = 0;
			FINISHING_ENTITY = NULL;
		}
		if (randomLimit(100) < chance) {
			if (MAIN_D_801350EC == 6 && MOVE_DATA[tech].range == 1 && MAIN_D_801350EC == 3) {
				if (MOVE_DATA[entityGetTechFromAnim((Entity *)MAIN_D_80135128, ((Entity *)MAIN_D_80135128)->anim.animId)].range == 3) {
					goto skipViewpoint;
				}
			}
			STD_setRandomViewpoint(entity, randomLimit(4));
skipViewpoint:
			dmg = STD_calculateDamage((DigimonEntity *)attacker, (DigimonEntity *)entity, tech);
			if (i == 0) {
				if ((dmg * 100 / PARTNER_ENTITY.digimonEntity.stats.base.hp) >= 0x14) {
					COMBAT_DATA_PTR->player.unk2++;
				}
			}
			fighter->hpDamageBuffer += dmg;
			if (fighter->hpDamageBuffer >= 0x2710) {
				fighter->hpDamageBuffer = 0x270f;
			}
			fighter->flags |= 0x10;
			STD_handleHitReaction(entity, fighter, &attack, i);
			sub->unk25 = 0;
			addEntityText((DigimonEntity *)entity, i, 0, dmg, 0);
			fighter->invulnerableTimer = MOVE_DATA[tech].iframes;
			entity->anim.animFlag &= 0xfe;
			STD_applyMoveStatus((DigimonEntity *)entity, fighter, tech);
			continue;
		}
		handled = 0;
		if (!(fighter->flags & 0x80) && (MOVE_DATA[tech].range == 1) && (DIGIMON_DATA[entity->type].moves[(uint32_t)(entity->anim.animId - 0x2e)] != 0x2d)) {
			moves = ((DigimonEntity *)entity)->stats.base.moves;
			for (j = 0; j < 4; j++) {
				if (((DigimonEntity *)entity)->stats.current.currentMP < 0xa5) {
					break;
				}
				if ((moves[j] != 0xff) && (DIGIMON_DATA[entity->type].moves[moves[j] - 0x2e] == 0x2d)) {
					if (randomLimit(100) < (((DigimonEntity *)entity)->stats.base.speed / DIGIMON_DATA[entity->type].level)) {
						fighter->queuedAnim = moves[j];
						fighter->targetId = STD_getFighterSlot(attack.casterId);
						fighter->moveRange = 1;
						STD_func_80065540((DigimonEntity *)entity, (DigimonEntity *)attacker, fighter);
						attacker->anim.animFlag &= 0xfe;
						handled = 1;
						((DigimonEntity *)entity)->stats.current.isHit = 0;
						break;
					}
				}
			}
		}
		if (handled != 0) {
			continue;
		}
		if (STD_addBlockedAttack(fighter, (FighterData *)&attack) != 0) {
			createParticleFX(0, 2, &attack.position, entity, 0x11);
		}
		if ((fighter->flags & 0x80) && (fighter->invulnerableTimer > 0)) {
			goto blocked;
		}
		if (MOVE_DATA[tech].range == 1) {
			dmg = STD_calculateDamage((DigimonEntity *)attacker, (DigimonEntity *)entity, tech);
			dmg = dmg * (randomLimit(0x15) + 0xa) / 100;
			if (dmg <= 0) {
				dmg = 1;
			}
			fighter->hpDamageBuffer += dmg;
			if (fighter->hpDamageBuffer >= 0x2710) {
				fighter->hpDamageBuffer = 0x270f;
			}
			sub->unk25 = 0;
			addEntityText((DigimonEntity *)entity, i, 0, dmg, 0);
		}
		STD_addFinisherProgress(fighter, fighter->finisherGoal * 3 / 50);
		for (j = 0; ENEMY_COUNT >= j; j++) {
			if (attacker == ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[j]]) {
				STD_addFinisherProgress(&COMBAT_DATA_PTR->fighter[j], COMBAT_DATA_PTR->fighter[j].finisherGoal * 4 / 50);
				break;
			}
		}
		if (i == 0) {
			COMBAT_DATA_PTR->player.blockedCount++;
		}
		loc.vx = attack.position.vx;
		loc.vy = 0;
		loc.vz = attack.position.vz;
		entityLookAtLocation(entity, &loc);
		fighter->flags |= 0x80;
		startAnimation(entity, 0x25);
		entity->anim.animFlag &= 0xfe;
blocked:
		fighter->invulnerableTimer = 3;
		((DigimonEntity *)entity)->stats.current.isHit = 0;
	}
}

int16_t STD_getAttackTech(AttackObject *attack)
{
	int16_t slot;
	int32_t i;
	int16_t tech;

	if (attack->effectId != 0x179) {
		slot = STD_getFighterSlot(attack->casterId);
		for (i = 0; i < 4; i++) {
			if (attack->effectId == COMBAT_DATA_PTR->fighter[slot].effectSlot[i]) {
				break;
			}
		}
		tech = entityGetTechFromAnim(ENTITY_TABLE[attack->casterId], ((DigimonEntity *)ENTITY_TABLE[attack->casterId])->stats.base.moves[i]);
	} else {
		tech = 0x79;
	}

	return tech;
}

int32_t STD_applyBuffMove(DigimonEntity *digimon, int32_t slot, int16_t anim)
{
	Stats *stats;

	stats = &digimon->stats;
	switch (anim) {
	case 0x29:
		STD_buffStats(digimon, slot, (int16_t)(stats->base.off * 3 / 10), &stats->base.off, 0xb, 3);
		break;
	case 0x2a:
		STD_buffStats(digimon, slot, (int16_t)(stats->base.off / 10), &stats->base.off, 0xb, 3);
		STD_buffStats(digimon, slot, (int16_t)(stats->base.def * 5 / 100), &stats->base.def, 0xb, 4);
		STD_buffStats(digimon, slot, (int16_t)(stats->base.speed / 10), &stats->base.speed, 0xb, 5);
		break;
	case 0x22:
		STD_buffStats(digimon, slot, (int16_t)(stats->base.def / 5), &stats->base.def, 0xb, 4);
		STD_buffStats(digimon, slot, (int16_t)(stats->base.speed / 10), &stats->base.speed, 0xb, 5);
		break;
	case 0x15:
		STD_buffStats(digimon, slot, (int16_t)(stats->base.off * 7 / 100), &stats->base.off, 0xb, 3);
		STD_buffStats(digimon, slot, (int16_t)(stats->base.def * 8 / 100), &stats->base.def, 0xb, 4);
		STD_buffStats(digimon, slot, (int16_t)(stats->base.speed * 7 / 100), &stats->base.speed, 0xb, 5);
		break;
	case 0x1e:
		STD_buffStats(digimon, slot, (int16_t)(stats->base.off / 4), &stats->base.off, 0xb, 3);
		STD_buffStats(digimon, slot, (int16_t)(stats->base.def * 3 / 20), &stats->base.def, 0xb, 4);
		STD_buffStats(digimon, slot, (int16_t)(stats->base.speed * 3 / 20), &stats->base.speed, 0xb, 5);
		break;
	default:
		return 0;
	}

	return 1;
}

int16_t STD_applyPartnerStatsToFighter(DigimonEntity *attacker, DigimonEntity *defender, FighterData *fighter, int16_t move)
{
	int32_t x;
	int16_t result;

	if (MAIN_D_80135118 != 0) {
		return 0;
	}

	if (fighter->flags & 0x200c) {
		return 100;
	}

	if (PARTNER_PARA.condition & 0x40) {
		return 100;
	}

	if (fighter->speedBuffer <= 0) {
		return 100;
	}

	if (defender->entity.anim.animId >= 0x2eU) {
		return 100;
	}

	if ((move >= 0x3a) && (move < 0x71)) {
		if (fighter->flags & 0x80) {
			fighter->flags &= 0xff7f;
			fighter->invulnerableTimer = 0;
		}
		return 100;
	}

	if (fighter->flags & 0x80) {
		return 0;
	}

	x = (MOVE_DATA[move].accuracy / 2) * (defender->stats.base.speed - (attacker->stats.base.speed / 10)) / 999;
	if ((defender->entity.anim.animId == 0x21) || (defender->entity.anim.animId == 0x22)) {
		x = x * 6 / 5;
	}

	result = (int16_t)(MOVE_DATA[move].accuracy - x);
	if (result < 0) {
		result = 0;
	}

	if (result > 100) {
		result = 100;
	}

	return result;
}

int32_t STD_calculateDamage(DigimonEntity *attacker, DigimonEntity *defender, int16_t move)
{
	int32_t diff;
	uint8_t mult[3];
	long i;
	int32_t dmg;
	int16_t off;
	int16_t def;

	for (i = 0; i < 3; i++) {
		if (DIGIMON_DATA[defender->entity.type].special[i] != 0xff) {
			mult[i] = TYPE_FACTORS[MOVE_DATA[move].special][DIGIMON_DATA[defender->entity.type].special[i]];
		} else {
			mult[i] = 10;
		}
	}
	off = attacker->stats.base.off;
	def = defender->stats.base.def;
	if (move == 0x2d) {
		def = def * 3 / 10;
	}
	if ((move >= 0x3a) && (move < 0x71)) {
		dmg = (off + MOVE_DATA[move].power) * (mult[0] + mult[1] + mult[2]) / 30;
	} else {
		diff = off - def;
		if (diff >= 0x1f5) {
			diff = 0x1f4;
		}
		if (diff < -0x1f4) {
			diff = -0x1f4;
		}
		dmg = (mult[0] + mult[1] + mult[2]) * (MOVE_DATA[move].power + diff * MOVE_DATA[move].power / 500) / 30 * (randomLimit(0x15) + 0x5a) / 100;
	}
	if ((move >= 0x3a) && (move < 0x71)) {
		if ((Entity *)attacker == ENTITY_TABLE[1]) {
			if (COMBAT_DATA_PTR->player.finisherChargeup[0] >= 0x29) {
				dmg = dmg * COMBAT_DATA_PTR->player.finisherChargeup[0] / 40;
			}
		} else {
			dmg = dmg * (randomLimit(0x65) + 0x64) / 100;
		}
		dmg = dmg * (randomLimit(0x15) + 0x5a) / 100;
	}
	if (dmg <= 0) {
		dmg = 1;
	}
	if (dmg >= 10000) {
		dmg = 9999;
	}
	return dmg;
}

void STD_handleHitReaction(Entity *entity, FighterData *fighter, AttackObject *attack, int16_t index)
{
	VECTOR d;
	VECTOR n;
	VECTOR *loc;
	VECTOR a;
	PositionData *posData;
	int16_t *rotY;
	int16_t ang;

	posData = entity->posData;
	loc = &entity->posData->location;
	rotY = &posData->rotation.vy;
	a.vx = attack->position.vx;
	a.vy = 0;
	a.vz = attack->position.vz;
	d.vx = loc->vx - a.vx;
	d.vy = 0;
	d.vz = loc->vz - a.vz;
	n.vx = -d.vx;
	n.vy = 0;
	n.vz = -d.vz;
	ang = _atan(n.vz, n.vx);
	if (fighter->flags & 8) {
		*rotY = ang;
		entity->flatSprite = 3;
		startAnimation(entity, 0x28);
		return;
	}
	if (*rotY >= 0x400 && *rotY < 0xc00) {
		if (*rotY - 0x400 <= ang && *rotY + 0x400 >= ang) {
			*rotY = ang;
			STD_startHitAnimation(entity, attack, 0x28);
		} else {
			*rotY = _atan(d.vz, d.vx);
			STD_startHitAnimation(entity, attack, 0x29);
		}
	} else if (!(0 > *rotY) && *rotY < 0x400) {
		if ((!(0 > ang) && ang <= *rotY + 0x400) || (ang >= *rotY + 0xc00 && ang < 0x1000)) {
			*rotY = ang;
			STD_startHitAnimation(entity, attack, 0x28);
		} else {
			*rotY = _atan(d.vz, d.vx);
			STD_startHitAnimation(entity, attack, 0x29);
		}
	} else {
		if ((*rotY - 0x400 <= ang && ang < 0x1000) || (!(0 > ang) && *rotY - 0xc00 >= ang)) {
			*rotY = ang;
			STD_startHitAnimation(entity, attack, 0x28);
		} else {
			*rotY = _atan(d.vz, d.vx);
			STD_startHitAnimation(entity, attack, 0x29);
		}
	}
}

void STD_applyMoveStatus(DigimonEntity *digimon, FighterData *fighter, int32_t move)
{
	int32_t chance;

	if (fighter->flags & 0x100) {
		return;
	}

	if (MOVE_DATA[move].statusChance == 0) {
		return;
	}

	chance = MOVE_DATA[move].statusChance;
	if (randomLimit(100) < chance) {
		switch (MOVE_DATA[move].status) {
		case 1:
			if (!(fighter->flags & 1)) {
				fighter->flags |= 1;
				fighter->poisonTimer = 100;
				STD_addPoisonStatusVisual(digimon, fighter);
			}
			break;
		case 2:
			if (!(fighter->flags & 2)) {
				fighter->flags |= 2;
				fighter->confusionTimer = randomLimit(0x65) + 200;
				STD_addConfusionStatusVisual(digimon, fighter);
				STD_resetFighterAction(fighter);
			}
			break;
		case 3:
			if (!(fighter->flags & 4)) {
				fighter->flags |= 4;
				fighter->stunTimer = randomLimit(0x29) + 200;
				STD_addStunStatusVisual(digimon, fighter);
				STD_resetFighterAction(fighter);
			}
			break;
		case 4:
			if (!(fighter->flags & 8)) {
				fighter->flatTimer = -1;
				STD_removeStatusEffects(digimon, fighter);
				STD_resetFighterAction(fighter);
			}
			break;
		}
		if (fighter == COMBAT_DATA_PTR->fighter) {
			COMBAT_DATA_PTR->player.statusedCount++;
		}
	}
}

int16_t STD_getFighterSlot(int16_t entityId)
{
	int32_t i;

	for (i = 0; ENEMY_COUNT >= i; i++) {
		if (COMBAT_DATA_PTR->player.entityIds[i] == entityId) {
			return i;
		}
	}

	return -1;
}

int32_t STD_addBlockedAttack(FighterData *fighter, FighterData *other)
{
	int32_t i;

	if (0) {
		i = 0;
	}
	for (i = 0; i < 0x95; i++) {
		if (fighter->table1[i] == -1) {
			break;
		}
		if ((fighter->table1[i] == other->effectSlot[3]) && (fighter->table2[i] == other->unk11)) {
			return 0;
		}
	}

	fighter->table1[i] = other->effectSlot[3];
	fighter->table2[i] = other->unk11;

	return 1;
}

// clang-format off
void STD_buffStats(digimon, slot, value, stat, color, flag)
	DigimonEntity *digimon;
	int16_t slot;
	int16_t value;
	int16_t *stat;
	int16_t color;
	uint8_t flag;
// clang-format on
{
	addWithLimit(stat, value, 0x3e7);
	addEntityText(digimon, slot, color, value, flag);
}

void STD_startHitAnimation(Entity *entity, AttackObject *attack, uint8_t animId)
{
	int16_t tech;

	tech = STD_getAttackTech(attack);
	startAnimation(entity, animId);
	createParticleFX(MOVE_DATA[tech].special, 1, &attack->position, entity, MOVE_DATA[tech].iframes + 0x10);
}

void STD_func_80064FCC(int16_t count)
{
	int32_t i;

	for (i = 0; i <= count; i++) {
		STD_battleTickFrame();
	}
}

void STD_battleTickFrame(void)
{
	POLLED_INPUT = PadRead(1);
	ACTIVE_FRAMEBUFFER = GsGetActiveBuff();
	ACTIVE_ORDERING_TABLE = &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER];
	GsSetWorkBase(&GS_WORK_BASES[ACTIVE_FRAMEBUFFER * 0x14000]);
	GsClearOt(0, 0, ACTIVE_ORDERING_TABLE);
	tickObjects();
	renderObjects();
	AddPrim((char *)ACTIVE_ORDERING_TABLE->org + 0x80, &DR_OFFSETS[ACTIVE_FRAMEBUFFER * 0xc]);
	DrawSync(0);
	VSync(3);
	POLLED_INPUT_PREVIOUS = POLLED_INPUT;
	GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
	GsSwapDispBuff();
	GsSortClear(0, 0, 0, ACTIVE_ORDERING_TABLE);
	GsDrawOt(ACTIVE_ORDERING_TABLE);
}

int32_t STD_isMoveUsable(DigimonEntity *digimon, FighterData *fighter, int16_t slot)
{
	uint8_t move;
	int16_t tech;
	int16_t mp;

	move = digimon->stats.base.moves[slot];
	if (move == 0xff) {
		return 0;
	}

	tech = entityGetTechFromAnim(&digimon->entity, move);
	if (tech == 0x2d) {
		return 0;
	}

	if ((tech >= 0x3a) && (tech < 0x71)) {
		return 0;
	}

	if ((MOVE_DATA[tech].range == 4) && (fighter->buffsRemaining == 0)) {
		return 0;
	}

	if ((fighter->targetId == 0xff) && ((MOVE_DATA[tech].unk3 & 1) == 1)) {
		return 0;
	}

	mp = MOVE_DATA[tech].mpCost * 3;
	if (digimon == (DigimonEntity *)ENTITY_TABLE[1]) {
		if (PARTNER_ENTITY.digimonEntity.stats.base.brain >= 700) {
			if (PARTNER_ENTITY.digimonEntity.stats.base.brain == 999) {
				mp -= mp / 5;
			} else if (PARTNER_ENTITY.digimonEntity.stats.base.brain >= 900) {
				mp -= mp * 15 / 100;
			} else if (PARTNER_ENTITY.digimonEntity.stats.base.brain >= 800) {
				mp -= mp / 10;
			} else {
				mp -= mp / 20;
			}
		}
		if (PARTNER_PARA.condition & 0x60) {
			mp += mp / 2;
		}
	}

	if (digimon->stats.current.currentMP >= mp) {
		return 1;
	}

	return 0;
}

int16_t STD_getNearestEnemy(Entity *self, int16_t *flags)
{
	int32_t i;
	uint32_t dist;
	uint32_t bestDist;
	Entity *other;
	int16_t best;

	best = 0xff;
	bestDist = -1;
	for (i = 0; ENEMY_COUNT >= i; i++) {
		if (flags[i] == 1) {
			other = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
			if ((self != other) && (STD_isFighterDefeated(i) == 0)) {
				dist = STD_getDistanceSquared(self, other);
				if (dist < bestDist) {
					bestDist = dist;
					best = i;
				}
			}
		}
	}

	return best;
}

int32_t STD_getDistanceSquared(Entity *a, Entity *b)
{
	VECTOR d;
	VECTOR *pa;
	VECTOR *pb;

	pa = &a->posData->location;
	pb = &b->posData->location;
	d.vx = pa->vx - pb->vx;
	d.vy = 0;
	d.vz = pa->vz - pb->vz;

	return (d.vx * d.vx) + (d.vz * d.vz);
}

void STD_selectConfusedMove(DigimonEntity *digimon, FighterData *fighter, long index)
{
	TargetChoice choice;

	if (randomLimit(10) < 7) {
		fighter->targetId = 0xff;
	} else {
		STD_getRemainingEnemies(&digimon->entity, choice.enemies, &choice.count);
		fighter->targetId = choice.enemies[randomLimit(choice.count)];
	}

	if (STD_func_80066A50(choice.flags, index) == 0) {
		STD_setFighterCooldown(digimon, fighter);
	} else {
		STD_setupQueuedMove(digimon, fighter, index, STD_getRandomUsableMove(choice.flags));
	}
}

int16_t STD_getNpcEntityIndex(Entity *entity)
{
	int32_t i;

	for (i = 2; i < 0xa; i++) {
		if (entity == ENTITY_TABLE[i]) {
			return i - 2;
		}
	}

	return -1;
}

void STD_func_80067744(DigimonEntity *digimon, FighterData *fighter, int16_t index)
{
	int32_t j;
	int32_t pick;
	int32_t total;
	int16_t flags[4];
	int16_t weights[4];
	int16_t tech;
	int32_t i;

	if (STD_func_80066A50(flags, index) == 0) {
		STD_setFighterCooldown(digimon, fighter);
		return;
	}

	for (i = 0; i < 4; i++) {
		weights[i] = digimon->stats.base.movesPrio[i];
	}

	for (i = 0; i < 4; i++) {
		if (digimon->stats.base.moves[i] == 0xff) {
			continue;
		}
		if (flags[i] == 0) {
			weights[i] = 0;
			continue;
		}
		tech = entityGetTechFromAnim(&digimon->entity, digimon->stats.base.moves[i]);
		for (j = 1; ENEMY_COUNT >= j; j++) {
			if (j != index) {
				if ((((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[j]])->stats.current.currentHP - COMBAT_DATA_PTR->fighter[j].hpDamageBuffer) > 0) {
					break;
				}
			}
		}
		if (j != (ENEMY_COUNT + 1)) {
			if (NPC_ENTITIES[STD_getNpcEntityIndex(&digimon->entity)].unk1 == 0) {
				if (MOVE_DATA[tech].range == 3) {
					weights[i] += 0x14;
				}
			}
		}
		if (MOVE_DATA[tech].range == 4) {
			weights[i] += fighter->buffPrioTimer;
		}
	}

	total = 0;
	for (i = 0; i < 4; i++) {
		total += weights[i];
	}

	pick = randomLimit(total);
	total = 0;
	for (i = 0; i < 4; i++) {
		if (weights[i] != 0) {
			total += weights[i];
			if (pick < total) {
				break;
			}
		}
	}

	if (digimon->stats.base.moves[i] != 0xff) {
		STD_setupQueuedMove(digimon, fighter, index, (uint8_t)i);
	} else {
		fighter->cooldown = 0x50;
		fighter->flags |= 0x800;
	}
}

void STD_func_80067A30(DigimonEntity *digimon, FighterData *fighter, int16_t index)
{
	int32_t j;
	int32_t pick;
	int32_t total;
	int32_t values[3];
	int32_t keys[3];
	int32_t groups[3];
	int16_t flags[4];
	int16_t weights[4];
	int32_t count;
	int16_t tech;
	Stats *stats;
	int32_t i;

	if (STD_func_80066A50(flags, index) == 0) {
		STD_setFighterCooldown(digimon, fighter);
		return;
	}

	for (i = 0; i < 3; i++) {
		weights[i] = 0;
		keys[i] = i;
	}

	stats = &digimon->stats;
	switch (stats->current.chargeMode) {
	case 0:
		for (i = 0; i < 3; i++) {
			if (flags[i] == 0) {
				values[i] = -1;
			} else {
				tech = entityGetTechFromAnim(&digimon->entity, stats->base.moves[i]);
				values[i] = MOVE_DATA[tech].power;
			}
		}
		STD_sortScoresDescending(values, keys, groups, 3);
		for (i = 0; i < 3; i++) {
			if (stats->base.moves[keys[i]] == 0xff) {
				weights[keys[i]] += 5;
			} else if (flags[keys[i]] != 0) {
				weights[keys[i]] = MAIN_D_80134888[groups[i]];
			}
		}
		break;
	case 1:
		for (i = 0; i < 3; i++) {
			if (stats->base.moves[i] == 0xff) {
				weights[i] += 5;
			}
			if (flags[i] != 0) {
				weights[i] += 0x14;
			}
		}
		break;
	case 2:
		for (i = 0; i < 3; i++) {
			if (flags[i] == 0) {
				values[i] = 10000;
			} else {
				tech = entityGetTechFromAnim(&digimon->entity, stats->base.moves[i]);
				values[i] = MOVE_DATA[tech].mpCost * 3;
			}
		}
		STD_sortScoresAscending(values, keys, groups, 3);
		for (i = 0; i < 3; i++) {
			if (stats->base.moves[keys[i]] == 0xff) {
				weights[keys[i]] += 5;
			} else if (flags[keys[i]] != 0) {
				weights[keys[keys[i]]] = MAIN_D_8013488C[groups[i]];
			}
		}
		break;
	}

	for (i = 0; i < 3; i++) {
		if (stats->base.moves[i] == 0xff) {
			continue;
		}
		if (flags[i] == 0) {
			weights[i] = 0;
			continue;
		}
		tech = entityGetTechFromAnim(&digimon->entity, stats->base.moves[i]);
		if (MOVE_DATA[tech].range == 4) {
			weights[i] += fighter->buffPrioTimer + STD_calculateElementBonus(MOVE_DATA[tech].special, DIGIMON_DATA[digimon->entity.type].special[0]);
			continue;
		}
		weights[i] += STD_calculateElementBonus(MOVE_DATA[tech].special, DIGIMON_DATA[ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighter->targetId]]->type].special[0]);
		if (MOVE_DATA[tech].range != 3) {
			continue;
		}
		count = STD_countLivingEnemies();
		switch (digimon->stats.current.chargeMode) {
		case 0:
			if ((stats->base.brain >= 0xc8) && (count >= 2)) {
				weights[i] += count * 10;
			}
			break;
		case 1:
			if ((stats->base.brain >= 0xc8) && (count >= 2)) {
				weights[i] += count * 15;
			}
			break;
		case 2:
			if ((stats->base.brain >= 0xc8) && (count >= 2)) {
				weights[i] += count * 15;
			} else if ((stats->current.currentHP * 100 / stats->base.hp) < 0x1f) {
				for (j = 0; j < 3; j++) {
					if (flags[i] == 0) {
						values[i] = -1;
					} else {
						tech = entityGetTechFromAnim(&digimon->entity, stats->base.moves[i]);
						values[i] = MOVE_DATA[tech].distance;
					}
				}
				STD_sortScoresDescending(values, keys, groups, 3);
				for (i = 0; i < 3; i++) {
					if (stats->base.moves[i] == 0xff) {
						continue;
					}
					if (flags[i] == 0) {
						continue;
					}
					weights[keys[i]] = MAIN_D_80134890[groups[i]];
				}
			}
			break;
		}
	}

	total = 0;
	for (i = 0; i < 3; i++) {
		total += weights[i];
	}

	pick = randomLimit(total);
	total = 0;
	for (i = 0; i < 3; i++) {
		if (weights[i] != 0) {
			total += weights[i];
			if (pick < total) {
				break;
			}
		}
	}

	if (digimon->stats.base.moves[i] != 0xff) {
		STD_setupQueuedMove(digimon, fighter, index, i);
	} else {
		fighter->cooldown = 0x50;
		fighter->flags |= 0x800;
	}
}

void STD_setupQueuedMove(DigimonEntity *digimon, FighterData *fighter, int16_t arg2, uint8_t moveIndex)
{
	int16_t tech;
	Stats *stats;

	fighter->unk15 = 0;
	stats = &digimon->stats;
	fighter->queuedAnim = stats->base.moves[moveIndex];
	tech = entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim);
	fighter->moveRange = MOVE_DATA[tech].range;
	STD_applyChargeRequirement(digimon, fighter, tech);
	fighter->flags |= 0x40;
}

void STD_applyChargeRequirement(DigimonEntity *digimon, FighterData *fighter, int16_t tech)
{
	switch (digimon->stats.current.chargeMode) {
	case 0:
		if (fighter->speedBuffer <= 0) {
			fighter->flags |= 0x800;
		}
		break;
	case 1:
		if ((fighter->speedBuffer != 100) && (fighter->speedBuffer < MOVE_DATA[tech].power)) {
			fighter->flags |= 0x800;
		}
		break;
	case 2:
		if (fighter->speedBuffer < 100) {
			fighter->flags |= 0x800;
		}
		break;
	}
}

void STD_func_80065540(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	int16_t tech;

	if (NO_AI_FLAG != 0) {
		if (FINISHING_ENTITY != &digimon->entity) {
			return;
		}
		if (MAIN_D_80135174 > 0) {
			MAIN_D_80135174--;
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			return;
		}
	} else {
		tech = entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim);
		if (tech >= 0x3a && tech < 0x71) {
			NO_AI_FLAG = 1;
		}
		if (NO_AI_FLAG != 0) {
			FINISHING_ENTITY = &digimon->entity;
			if (&digimon->entity == ENTITY_TABLE[1]) {
				STD_func_80069134(tech);
			}
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			startAnimation(&digimon->entity, fighter->queuedAnim);
			digimon->entity.anim.animFlag &= 0xfe;
			MAIN_D_80135178 = STD_addFinisherAura(&digimon->entity, 0x50);
			MAIN_D_80135174 = 0x50;
			return;
		}
	}
	if (STD_selectMoveTarget(&digimon->entity, fighter) != 0) {
		return;
	}
	if (target != NULL) {
		if (target->entity.anim.animId == 0x28) {
			return;
		}
		if (target->entity.anim.animId == 0x29) {
			return;
		}
		if (digimon != target) {
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
		}
	}
	if ((MOVE_DATA[tech].unk3 & 2) != 0) {
		if (&digimon->entity == ENTITY_TABLE[1]) {
			FLEE_DISABLED[1] = 0x6e;
		} else {
			VS_P2_AOE_TIMER = 0x6e;
		}
	}
	startAnimation(&digimon->entity, fighter->queuedAnim);
	fighter->flags |= 0x20;
	if ((fighter->flags & 8) == 0) {
		STD_func_800658B4(digimon, target, fighter);
		return;
	}
	fighter->flatAttackTimer = 0x1e;
}

int32_t STD_selectMoveTarget(Entity *entity, FighterData *fighter)
{
	int16_t tech;
	int16_t otherTech;
	int32_t i;
	Entity *e;
	FighterData *f;

	tech = entityGetTechFromAnim(entity, fighter->queuedAnim);
	if ((MOVE_DATA[tech].unk3 & 2) != 0) {
		if (entity == ENTITY_TABLE[1]) {
			if (FLEE_DISABLED[1] > 0) {
				return 1;
			}
		} else {
			if (VS_P2_AOE_TIMER > 0) {
				return 1;
			}
		}
		for (i = 0; i <= ENEMY_COUNT; i++) {
			e = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
			f = &COMBAT_DATA_PTR->fighter[i];
			if (entity == e) {
				continue;
			}
			if ((f->flags & 0x20) == 0) {
				continue;
			}
			otherTech = entityGetTechFromAnim(e, e->anim.animId);
			if ((MOVE_DATA[otherTech].unk3 & 2) == 0) {
				continue;
			}
			return 1;
		}
	}

	return 0;
}

void STD_func_800658B4(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	long i;
	int32_t t;
	int32_t n;
	int32_t tech;
	int16_t cost;
	int16_t brain;

	BATTLE_ATTACKING_DIGIMON = digimon;
	tech = entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim);
	if (fighter->moveRange != 4) {
		BATTLE_TARGETED_DIGIMON = target;
		if (target == NULL) {
			if (MOVE_DATA[tech].unk3 & 1) {
				if (digimon != (DigimonEntity *)ENTITY_TABLE[1]) {
					BATTLE_TARGETED_DIGIMON = (DigimonEntity *)ENTITY_TABLE[1];
					fighter->targetId = 0;
				} else {
					BATTLE_TARGETED_DIGIMON = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]];
					fighter->targetId = 1;
				}
			}
		}
	} else {
		BATTLE_TARGETED_DIGIMON = digimon;
	}

	STD_removeMoveEffect(digimon, fighter);
	if (digimon == (DigimonEntity *)ENTITY_TABLE[1]) {
		if (!((tech >= 0x3a) && (tech < 0x71))) {
			cost = MOVE_DATA[tech].mpCost * 3;
			if (PARTNER_PARA.condition & 0x60) {
				cost += cost / 2;
			}
			brain = digimon->stats.base.brain;
			if (brain >= 0x2bc) {
				if (brain == 0x3e7) {
					cost = (int16_t)(cost * 4 / 5);
				} else if ((brain >= 0x384) && (brain < 0x3e7)) {
					cost = (int16_t)(cost * 17 / 20);
				} else if ((brain >= 0x320) && (brain < 0x384)) {
					cost = (int16_t)(cost * 9 / 10);
				} else {
					cost = (int16_t)(cost * 19 / 20);
				}
			}
			fighter->mpDamageBuffer += cost;
		}
	} else {
		if (!((tech >= 0x3a) && (tech < 0x71))) {
			digimon->stats.current.currentMP -= (int16_t)(MOVE_DATA[tech].mpCost * 3);
		}
		if (tech < 0x3a) {
			for (i = 0; i < 0xc; i++) {
				if (COMBAT_DATA_PTR->player.usedMoves[i] == tech) {
					break;
				}
				if (COMBAT_DATA_PTR->player.usedMoves[i] == 0xff) {
					COMBAT_DATA_PTR->player.usedMoves[i] = tech;
					break;
				}
			}
		}
	}

	digimon->stats.current.unk1 = tech + 0x100;
	for (i = 0; i < 3; i++) {
		if (fighter->queuedAnim == digimon->stats.base.moves[i]) {
			break;
		}
	}

	if (i != 4) {
		digimon->stats.current.efeSubEffect = STD_startEFE(fighter->effectSlot[i]);
		fighter->unk11 = fighter->effectSlot[i];
	}

	if ((MOVE_DATA[tech].range == 4) && (fighter->buffsRemaining != 0)) {
		fighter->buffsRemaining--;
	}

	fighter->speedBuffer -= MOVE_DATA[tech].power;
	if (fighter->speedBuffer < -0x9b) {
		fighter->speedBuffer = -0x9b;
	}

	if (MAIN_D_801350EC == 3) {
		if (MOVE_DATA[entityGetTechFromAnim((Entity *)MAIN_D_80135128, ((Entity *)MAIN_D_80135128)->anim.animId)].range == 3) {
			return;
		}
	}

	if (MAIN_D_801350EC == 6) {
		if (MAIN_D_801350EC != 3) {
			return;
		}
		if (digimon != (DigimonEntity *)ENTITY_TABLE[1]) {
			return;
		}
	}

	if ((MOVE_DATA[tech].range == 1) || (MOVE_DATA[tech].range == 4)) {
		n = 2;
	} else {
		n = 5;
	}

	STD_func_8005A55C((DigimonEntity *)digimon, randomLimit(n), MOVE_DATA[tech].range);
}

void STD_removeMoveEffect(DigimonEntity *digimon, FighterData *fighter)
{
	if (fighter->unk11 != -1L) {
		STD_stopEFESubEffect(fighter->unk11, digimon->stats.current.efeSubEffect);
	}
	digimon->stats.current.efeSubEffect = -1;
	fighter->unk11 = -1;
}

void STD_addFinisherProgress(FighterData *fighter, int16_t amount)
{
	int16_t tech;
	int32_t i;

	for (i = 0; ENEMY_COUNT >= i; i++) {
		if (&COMBAT_DATA_PTR->fighter[i] == fighter) {
			tech = entityGetTechFromAnim(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]], ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]])->stats.base.moves[3]);
			if ((tech < 0x3a) || (tech >= 0x71)) {
				return;
			}
		}
	}

	fighter->finisherProgress += amount;
	if (fighter->finisherProgress > fighter->finisherGoal) {
		fighter->finisherProgress = fighter->finisherGoal;
	}
}

void STD_applyMoveResult(void)
{
	long i;
	int32_t dmg;
	CombatData *combat;
	PlayerDataSub *sub;
	Entity *entity;
	Stats *stats;
	FighterData *fighter;

	combat = COMBAT_DATA_PTR;
	sub = COMBAT_DATA_PTR->player.unk1;
	i = 0;

	for (; i <= ENEMY_COUNT; i++, sub++) {
		fighter = &combat->fighter[i];
		if (fighter->flags & 0x8000) {
			continue;
		}
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		stats = &((DigimonEntity *)entity)->stats;
		if (fighter->flags & 1) {
			if (NO_AI_FLAG == 0) {
				fighter->poisonTimer--;
			}
			if (fighter->poisonTimer == 0) {
				fighter->poisonTimer = 100;
				dmg = stats->base.hp * (randomLimit(3) + 1) / 100;
				fighter->hpDamageBuffer += dmg;
				if (fighter->hpDamageBuffer >= 0x2710) {
					fighter->hpDamageBuffer = 0x270f;
				}
				if (i != 0) {
					COMBAT_DATA_PTR->player.unk1[i].unk25 = 0;
				}
				addEntityText((DigimonEntity *)entity, i, 0xc, dmg, 0);
			}
		}
		if (fighter->flags & 2) {
			if ((NO_AI_FLAG == 0) && (fighter->confusionTimer != 0)) {
				fighter->confusionTimer--;
			}
			if ((fighter->confusionTimer == 0) && !(combat->fighter[0].flags & 0x20)) {
				STD_updateFighterStatusVisuals((DigimonEntity *)entity, fighter);
			}
		}
		if (fighter->flags & 4) {
			if (NO_AI_FLAG == 0) {
				fighter->stunTimer--;
			}
			if (fighter->stunTimer == 0) {
				STD_clearStun((DigimonEntity *)entity, fighter);
			}
		}
		if (fighter->flatTimer <= 0) {
			continue;
		}
		if ((NO_AI_FLAG == 0) || (fighter->flatTimer < 0x42)) {
			fighter->flatTimer--;
		}
		STD_applyFlattenScale(&entity->posData->scale, fighter->flatTimer);
		if (fighter->flags & 8) {
			switch (fighter->flatTimer) {
			case 0x40:
				if ((fighter->flags & 0x10) || (fighter->flags & 0x20)) {
					fighter->flatTimer++;
				} else {
					startAnimation(entity, 0x22);
					fighter->moveRange = -1;
					fighter->flags |= 0x40;
					stats->current.isHit = 1;
				}
				break;
			case 3:
				entity->flatSprite = -1;
				break;
			case 0:
				fighter->flags &= 0xffb7;
				stats->current.isHit = 0;
				if (fighter->flags & 4) {
					STD_addStatusEffectVisual((DigimonEntity *)entity, fighter, 3);
					fighter->moveRange = 0;
				}
				if (fighter->flags & 2) {
					STD_addStatusEffectVisual((DigimonEntity *)entity, fighter, 2);
				}
				if (fighter->flags & 1) {
					STD_addStatusEffectVisual((DigimonEntity *)entity, fighter, 1);
				}
				fighter->moveRange = 0;
				STD_resetFighterAction(fighter);
				break;
			}
		} else {
			switch (fighter->flatTimer) {
			case 0x40:
				startAnimation(entity, 0x22);
				fighter->moveRange = -1;
				fighter->flags |= 0x40;
				stats->current.isHit = 1;
				break;
			case 3:
				entity->flatSprite = 0;
				break;
			case 0:
				fighter->flags |= 8;
				fighter->flatTimer = randomLimit(0x51) + 0xe0;
				stats->current.isHit = 0;
				fighter->flags &= 0xffbf;
				break;
			}
		}
	}
}

void STD_updateFighterStatusVisuals(DigimonEntity *digimon, FighterData *fighter)
{
	STD_resetFighterAction(fighter);
	if (digimon == (DigimonEntity *)ENTITY_TABLE[1]) {
		fighter->targetId = 1;
	} else {
		fighter->targetId = 0;
	}

	fighter->flags &= 0xfffd;
	fighter->confusionTimer = 0;
	if (((fighter->flags & 0xc) == 0) && (fighter->flatTimer == 0)) {
		fighter->flags &= 0xffbf;
		STD_removeStatusEffectVisual(digimon, fighter, 2);
		if (fighter->flags & 1) {
			STD_addStatusEffectVisual(digimon, fighter, 1);
		}
	}
}

void STD_clearStun(DigimonEntity *digimon, FighterData *fighter)
{
	STD_resetFighterAction(fighter);
	digimon->entity.anim.animFlag |= 1;
	fighter->flags &= ~4;
	fighter->stunTimer = 0;
	if (fighter->flags & 8) {
		return;
	}

	if (fighter->flatTimer != 0) {
		return;
	}

	STD_removeStatusEffectVisual(digimon, fighter, 3);
	if (fighter->flags & 2) {
		STD_addStatusEffectVisual(digimon, fighter, 2);
	}

	if (fighter->flags & 1) {
		STD_addStatusEffectVisual(digimon, fighter, 1);
	}
}

void STD_applyFlattenScale(VECTOR *scale, int32_t t)
{
	if (t < 0x40) {
		if (t >= 0x30) {
			STD_applyStretchScale(scale, (0x40 - t) << 4);
		} else if (t >= 0x20) {
			STD_applySquashScale(scale, (0x30 - t) << 4);
		} else if (t >= 0x18) {
			STD_applyStretchScale(scale, (0x20 - t) << 5);
		} else if (t >= 0x10) {
			STD_applySquashScale(scale, (0x18 - t) << 5);
		} else if (t >= 0xc) {
			STD_applyStretchScale(scale, (0x10 - t) << 6);
		} else if (t >= 8) {
			STD_applySquashScale(scale, (0xc - t) << 6);
		} else if (t >= 6) {
			STD_applyStretchScale(scale, (8 - t) << 7);
		} else if (t >= 4) {
			scale->vz = scale->vy = scale->vx = 0x1000 - ((6 - t) << 11);
		} else if (0 <= t) {
			scale->vz = scale->vy = scale->vx = (4 - t) << 10;
		}
	}
}

void STD_addStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, uint8_t kind)
{
	if (fighter->statusFxId == -1) {
		switch (kind) {
		case 1:
			fighter->statusFxId = STD_func_80077664(digimon);
			break;
		case 2:
			fighter->statusFxId = STD_addConfusionEffect(digimon);
			break;
		case 3:
			fighter->statusFxId = STD_addStunEffect(digimon, fighter->stunTimer);
			break;
		}
	}
}

void STD_applyStretchScale(VECTOR *scale, int32_t angle)
{
	scale->vy = _sin(angle) + 0x1000;
	scale->vz = scale->vx = 0x1000 - (_sin(angle) / 2);
}

void STD_applySquashScale(VECTOR *scale, int32_t angle)
{
	scale->vy = 0x1000 - (_sin(angle) / 2);
	scale->vz = scale->vx = (_sin(angle) / 2) + 0x1000;
}

void STD_removeStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, uint8_t kind)
{
	if (fighter->statusFxId != -1L) {
		switch (kind) {
		case 1:
			STD_removePoisonEffect(fighter->statusFxId, digimon);
			break;
		case 2:
			STD_removeConfusionEffect(fighter->statusFxId, digimon);
			break;
		case 3:
			STD_removeStunEffect(fighter->statusFxId, digimon);
			break;
		}
		fighter->statusFxId = -1;
	}
}

void STD_resetFighterAction(FighterData *fighter)
{
	fighter->cooldown = 0;
	fighter->senileTimer = 0;
	fighter->flags &= 0xc7ff;
}

void STD_addPoisonStatusVisual(DigimonEntity *digimon, FighterData *fighter)
{
	if (!(fighter->flags & 0xe) && (fighter->flatTimer == 0)) {
		STD_addStatusEffectVisual(digimon, fighter, 1);
	}
}

void STD_addConfusionStatusVisual(DigimonEntity *digimon, FighterData *fighter)
{
	if (!(fighter->flags & 0xc) && (fighter->flatTimer == 0)) {
		if (fighter->flags & 1) {
			STD_removeStatusEffectVisual(digimon, fighter, 1);
		}
		STD_addStatusEffectVisual(digimon, fighter, 2);
	}
}

void STD_addStunStatusVisual(DigimonEntity *digimon, FighterData *fighter)
{
	if (!(fighter->flags & 8) && (fighter->flatTimer == 0)) {
		if (fighter->flags & 2) {
			STD_removeStatusEffectVisual(digimon, fighter, 2);
		}
		if (fighter->flags & 1) {
			STD_removeStatusEffectVisual(digimon, fighter, 1);
		}
		STD_addStatusEffectVisual(digimon, fighter, 3);
	}
}

void STD_removeStatusEffects(DigimonEntity *digimon, FighterData *fighter)
{
	if (fighter->flags & 4) {
		STD_removeStatusEffectVisual(digimon, fighter, 3);
	}

	if (fighter->flags & 2) {
		STD_removeStatusEffectVisual(digimon, fighter, 2);
	}

	if (fighter->flags & 1) {
		STD_removeStatusEffectVisual(digimon, fighter, 1);
	}
}

int32_t STD_func_80066A50(int16_t *out, int16_t index)
{
	DigimonEntity *digimon;
	FighterData *fighter;
	int32_t found;
	int32_t i;

	digimon = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]];
	fighter = &COMBAT_DATA_PTR->fighter[index];
	found = 0;

	for (i = 0; i < 4; ++i) {
		if (STD_isMoveUsable(digimon, fighter, i) != 0) {
			out[i] = 1;
			found = 1;
		} else {
			out[i] = 0;
		}
	}

	return found;
}

void STD_setFighterCooldown(DigimonEntity *digimon, FighterData *fighter)
{
	fighter->cooldown = 0x50;
	fighter->flags |= 0x800;
}

int16_t STD_getRandomUsableMove(int16_t *flags)
{
	int16_t picked[4];
	int32_t i;
	int32_t count;

	count = 0;
	for (i = 0; i < 4; i++) {
		if (flags[i] == 1) {
			picked[count++] = i;
		}
	}

	return picked[randomLimit(count)];
}

// clang-format off
int16_t STD_getStrongestMove(index, flags)
	int16_t index;
	int16_t *flags;
// clang-format on
{
	int16_t score[3];
	int16_t best;
	int16_t tech;
	uint8_t *moves;
	DigimonEntity *digimon;
	int32_t i;

	digimon = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]];
	moves = digimon->stats.base.moves;
	for (i = 0; i < 3; i++) {
		if (flags[i] == 1) {
			tech = entityGetTechFromAnim(&digimon->entity, moves[i]);
			score[i] = MOVE_DATA[tech].power;
		} else {
			score[i] = -1;
		}
	}

	STD_getHighestScoredMove(score, flags, &best, 3);

	return best;
}

// clang-format off
int16_t STD_getMostEffectiveMove(index, flags)
	int16_t index;
	int16_t *flags;
// clang-format on
{
	uint8_t *moves;
	DigimonEntity *digimon;
	int16_t score[3];
	Entity *target;
	int16_t best;
	int16_t tech;
	int32_t i;

	digimon = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]];
	moves = digimon->stats.base.moves;
	target = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[COMBAT_DATA_PTR->fighter[index].targetId]];
	for (i = 0; i < 3; i++) {
		if (flags[i] == 1) {
			tech = entityGetTechFromAnim(&digimon->entity, moves[i]);
			score[i] = TYPE_FACTORS[MOVE_DATA[tech].special][DIGIMON_DATA[target->type].special[0]];
		} else {
			score[i] = -1;
		}
	}

	STD_getHighestScoredMove(score, flags, &best, 3);

	return best;
}

// clang-format off
int16_t STD_getCheapestMove(index, flags)
	int16_t index;
	int16_t *flags;
// clang-format on
{
	int16_t score[3];
	int16_t best;
	int16_t tech;
	uint8_t *moves;
	DigimonEntity *digimon;
	int32_t i;

	digimon = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]];
	moves = digimon->stats.base.moves;
	for (i = 0; i < 3; i++) {
		if (flags[i] == 1) {
			tech = entityGetTechFromAnim(&digimon->entity, moves[i]);
			score[i] = MOVE_DATA[tech].mpCost * 3;
		} else {
			score[i] = 1000;
		}
	}

	STD_getLowestScoredMove(score, flags, &best, 3);

	return best;
}

void STD_getHighestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count)
{
	int32_t i;
	int32_t hits;
	int16_t best;

	best = values[0];
	for (i = 1; i < count; i++) {
		if (best < values[i]) {
			best = values[i];
		}
	}

	hits = 0;
	for (i = 0; i < count; i++) {
		if (best == values[i]) {
			hits++;
			*out = i;
			marks[i] = 1;
		} else {
			marks[i] = -1;
		}
	}

	if (hits >= 2) {
		*out = -1;
	}
}

void STD_getLowestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count)
{
	int32_t i;
	int32_t hits;
	int16_t best;

	best = values[0];
	for (i = 1; i < count; i++) {
		if (best > values[i]) {
			best = values[i];
		}
	}

	hits = 0;
	for (i = 0; i < count; i++) {
		if (best == values[i]) {
			hits++;
			*out = i;
			marks[i] = 1;
		} else {
			marks[i] = -1;
		}
	}

	if (hits >= 2) {
		*out = -1;
	}
}

void STD_sortScoresDescending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count)
{
	int32_t i;
	int32_t j;
	int32_t bestValue;
	int32_t best;

	for (i = 0; i < count; i++) {
		bestValue = values[i];
		best = i;
		for (j = i; j < count; j++) {
			if (bestValue < values[j]) {
				best = j;
				bestValue = values[j];
			}
		}
		swapInt(&values[i], &values[best]);
		swapInt(&keys[i], &keys[best]);
	}

	STD_calculateScoreRanks(values, groups, count);
}

void STD_sortScoresAscending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count)
{
	int32_t i;
	int32_t j;
	int32_t best;
	int32_t bestValue;

	for (i = 0; i < count; i++) {
		bestValue = values[i];
		best = i;
		for (j = i; j < count; j++) {
			if (bestValue > values[j]) {
				best = j;
				bestValue = values[j];
			}
		}
		swapInt(&values[i], &values[best]);
		swapInt(&keys[i], &keys[best]);
	}

	STD_calculateScoreRanks(values, groups, count);
}

int16_t STD_calculateElementBonus(int16_t arg0, int16_t arg1)
{
	switch (TYPE_FACTORS[arg0][arg1]) {
	case 20:
		return 10;
	case 15:
		return 7;
	case 10:
		return 5;
	case 5:
		return 3;
	case 2:
		return 1;
	}
}

int32_t STD_countLivingEnemies(void)
{
	int32_t count;
	int32_t i;

	count = 0;
	for (i = 1; ENEMY_COUNT >= i; i++) {
		if (STD_isFighterDefeated(i) == 0) {
			count++;
		}
	}

	return count;
}

void STD_calculateScoreRanks(int32_t *values, int32_t *groups, int32_t count)
{
	int32_t i;
	int32_t group;

	group = 0;
	groups[0] = group;
	for (i = 1; i < count; i++) {
		if (values[i] == (values + i)[-1]) {
			groups[i] = group;
		} else {
			groups[i] = ++group;
		}
	}
}

void STD_getRemainingEnemies(Entity *self, int16_t *out, int16_t *count)
{
	CombatData *combat;
	int32_t i;
	Entity *entity;

	*count = 0;
	combat = COMBAT_DATA_PTR;
	for (i = 0; i <= ENEMY_COUNT; i++) {
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		if ((self != entity) && (STD_isFighterDefeated(i) == 0)) {
			out[*count] = i;
			(*count)++;
		}
	}
}

int32_t STD_func_800675E8(int32_t arg0, int16_t *flags)
{
	int16_t result;

	result = STD_getStrongestMove(arg0, flags);
	if (result != -1) {
		return result;
	}

	result = STD_getMostEffectiveMove(arg0, flags);
	if (result != -1) {
		return result;
	}

	return STD_getRandomUsableMove(flags);
}

int32_t STD_func_80067660(int32_t arg0, int16_t *flags)
{
	int16_t result;

	result = STD_getCheapestMove(arg0, flags);
	if (result != -1) {
		return result;
	}

	result = STD_getMostEffectiveMove(arg0, flags);
	if (result != -1) {
		return result;
	}

	return STD_getRandomUsableMove(flags);
}

uint8_t STD_isFighterDefeated(uint8_t index)
{
	if ((((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]])->stats.current.currentHP - COMBAT_DATA_PTR->fighter[index].hpDamageBuffer) <= 0) {
		return 1;
	}

	return 0;
}

void STD_func_80068388(int32_t i)
{
	RECT rect;
	uint8_t cmd;
	int16_t tech;
#if !defined(VERSION_JP)
	uint32_t n;

	n = i;
#endif
	setRECT(&rect, 0, (i * 12) + 0xd8, 0x90, 0xc);
	clearTextSubArea(&rect);
	cmd = COMBAT_DATA_PTR->player.availableCommands[i][COMBAT_DATA_PTR->player.hoveredCommand[i]];

	if ((cmd >= 8) && (cmd < 0xc)) {
		tech = entityGetTechFromAnim(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]],
		                             PARTNER_ENTITY.digimonEntity.stats.base.moves[cmd - 8]);
		drawString(MOVE_NAMES[tech], 0, (i * 12) + 0xd8);
	} else {
		drawString(STD_D_8007A688[cmd - 1], 0, (i * 12) + 0xd8);
	}

#if defined(VERSION_JP)
	renderString(0, (i * 160) - 0x8c, MAIN_D_8013517C[i] - 0xe, 0x90, 0xc, 0, (i * 12) + 0xd8, 7, 1);
#else
	renderString(0, (int32_t)(n * 160) - 0x8c, MAIN_D_8013517C[i] - 0xe, 0x90, 0xc, 0, (i * 12) + 0xd8, 7, 1);
#endif
}

void STD_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, uint8_t index)
{
	int16_t c;
	int16_t eff;
	int16_t n;

	if ((index >= 8U) && (index < 0xcU)) {
		eff = MOVE_DATA[entityGetTechFromAnim(&digimon->entity, digimon->stats.base.moves[index - 8])].special;
		setUVWH(prim, STD_D_8007A708[eff * 2], (&STD_D_8007A708[1])[eff * 2], 0x10, 0xf);
	} else {
		setUVWH(prim, STD_D_8007A6F8[(index - 1) * 2], (&STD_D_8007A6F8[1])[(index - 1) * 2], 0x10, 0xf);
	}
}

void STD_addCommandMenu(uint8_t index)
{
	MAIN_D_8013517C[index] = 0x44;
	MAIN_D_80135180[index] = MAIN_D_8013517C[index] + 0x20;
	MAIN_D_80135184[index] = 0;
	MAIN_D_80135186[index] = 0;

	switch (COMBAT_DATA_PTR->player.numCommands[index]) {
	case 2:
		MAIN_D_80135188[index] = 0;
		break;
	case 3:
		MAIN_D_80135188[index] = 1;
		break;
	case 4:
		MAIN_D_80135188[index] = 2;
		break;
	case 5:
		MAIN_D_80135188[index] = 3;
		break;
	case 6:
		MAIN_D_80135188[index] = 4;
		break;
	case 7:
		MAIN_D_80135188[index] = 5;
		break;
	case 8:
		MAIN_D_80135188[index] = 6;
		break;
	case 9:
		MAIN_D_80135188[index] = 7;
		break;
	}

	MAIN_D_8013518A[index] = 0;
	addObject(0x198, index, (TickFunction)STD_tickCommandMenu, (RenderFunction)STD_renderCommandMenu);
}

void STD_tickCommandMenu(uint8_t i)
{
	MAIN_D_80135186[i]++;
	if (GAME_STATE != 0) {
		if (GAME_STATE == 4) {
			if ((MAIN_D_80135186[i] % 8) == 0) {
				MAIN_D_80135184[i] = (MAIN_D_80135184[i] + 1) & 1;
			}
		}
	}
}

void STD_removeCommandMenu(int32_t i)
{
	MAIN_D_8013518A[i] = 0;
	removeObject(0x198, i);
}
