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
#include <dw/main.h>
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
#include <dw/version.h>
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
extern PACKET GS_WORK_BASES[2][0x14000];
extern DR_OFFSET DR_OFFSETS[2];
extern int32_t STD_CAMERA_STATE;
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t VIEWPORT_DISTANCE;
extern Entity *STD_OPPONENT_ENTITY;
extern uint8_t CURRENT_SCREEN;
extern char MAIN_D_80124C0C[][12];
extern char MAIN_D_80124C54[];
extern int32_t COMBAT_AREA_X;
extern int32_t COMBAT_AREA_Y;
extern uint8_t STD_MUSIC;
extern int32_t FLEE_DISABLED[2];
extern int32_t P2_AOE_TIMER;
extern char *MOVE_NAMES[];
extern int16_t STD_MATCH_RESULT;
extern int8_t BATTLE_TOGGLE_LIFEBAR;
extern int16_t INITIAL_COMBAT_STATS[][6];
extern DigimonEntity *BATTLE_TARGETED_DIGIMON;
extern DigimonEntity *BATTLE_ATTACKING_DIGIMON;

void STD_initializeBattleStartText(void);
void STD_removeBattleStartText(void);
void STD_initializeBattleStartTextBurst(void);
void STD_removeBattleStartTextBurst(void);
int32_t STD_isBattleStartTextFinished(void);
char *STD_initializeEFEEngine(char *base);
void STD_loadMoveEFE(int16_t *moves, int16_t *effectIds, int8_t *isLoaded);
void tickFileReadQueue(int32_t instanceId);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
int32_t entityCheckCollision(Entity *a, Entity *entity, int32_t c, int32_t d);
void handleBattleIdle(DigimonEntity *entity, Stats *stats, int32_t flags);
void createParticleFX();
void collisionGrace(Entity *a, Entity *entity, int32_t c, int32_t d);
int32_t STD_handlePartnerMoveCommand(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int32_t STD_tickMeleeAttack(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int16_t arg3);
void STD_startQueuedMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int16_t STD_getMostEffectiveMove(int32_t arg0, int16_t *flags);
void addEntityText(Entity *entity, long slot, int32_t color, int32_t value, uint8_t flag);
void setupModelMatrix(PositionData *posData);
void startAnimation(Entity *entity, uint8_t animId);
void tickAnimation(Entity *entity);
void addWithLimit(/* int16_t *value, int16_t amount, int16_t limit */);
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void STD_removeStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, uint8_t kind);
void STD_addStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, uint8_t kind);
void swapInt(int32_t *a, int32_t *b);
void STD_renderCounterDigits(int16_t x, int16_t y, int16_t digits, int32_t value, int32_t layer);
void STD_removeIntroText(int32_t i);
void STD_setViewpointRotationFromEntity(void);
void STD_applyViewpoint(void);
void STD_startCameraIntro(Entity *target, Entity *entity);
void STD_addWinScene(int32_t a);
void STD_addLoseScene(void);
void STD_removeFinisherChargeup(void);
void STD_removeFinisherAura(int32_t id);
void STD_setCameraParams(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f);
void STD_setVSPhase(int32_t arg);
void STD_removeVSPhase(void);
int32_t STD_interpolateClamped2(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end);
void STD_removeCameraIntro(void);
void STD_addFighterCounter(int32_t arg);
void STD_renderFighterCounter(void);
void STD_removeFighterCounter(void);
void STD_removeBracketHitFlash(int32_t i);
void STD_removeBracketProjectile(int16_t i);
void STD_tickBracket(void);
int32_t STD_isBracketFinished(void);
void STD_removeBracketIntro(void);
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
int32_t STD_hasAffordableMoves(int16_t *out, int16_t index);
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
int32_t STD_getUsableMoves(int16_t *out, int16_t index);
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
int32_t STD_selectMoveByPower(int32_t arg0, int16_t *flags);
int32_t STD_selectMoveByMpCost(int32_t arg0, int16_t *flags);
uint8_t STD_isFighterDefeated(uint8_t index);
void STD_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, uint8_t index);
void STD_tickCommandMenu(uint8_t i);
void STD_removeCommandMenu(int32_t i);
void STD_stopEFESubEffect(int32_t a, int32_t b);
int32_t STD_addAuraProjectile(Entity *e);
void STD_addIntroText(Entity *entity, int32_t id);
int32_t readVBALLSection(int32_t vabId, int32_t idx);
int32_t isSoundLoaded(int32_t mode, int32_t vabId);
void STD_runIntro(int16_t which);
void STD_applyManualCamera(void);
void STD_applyRotatingCamera(void);
void STD_selectRandomCamera(DigimonEntity *entity, int32_t mode, uint8_t sub);
void STD_tickCameraChase(void);
void STD_updateBracket(void);
void STD_drawBracket(void);
void STD_addBracketProjectile(int16_t i, int32_t owner, uint8_t flag);
void STD_setupParticipants(uint8_t *out, uint8_t *list);
void STD_tickBracketProjectile(int32_t id);
void STD_renderBracketProjectile(int16_t id);
void STD_combatSetup(void);
void STD_tickPartnerAI(void);
void STD_tickEnemyAI(void);
void STD_tickBattle(void);
void STD_tickBattleResultScreen(int16_t hasLostP1, uint8_t hasLostP2);
void STD_addTimeoutWindow(void);
void STD_renderPlayerMarker(int16_t id);
int16_t STD_applyPartnerStatsToFighter(DigimonEntity *attacker, DigimonEntity *defender, FighterData *fighter, int16_t move);
int32_t STD_calculateDamage(DigimonEntity *attacker, DigimonEntity *defender, int16_t move);
void STD_handleHitReaction(Entity *entity, FighterData *fighter, AttackObject *attack, int16_t index);
void STD_tickAttackHits(void);
void STD_applyMoveResult(void);
void STD_selectEnemyMove(DigimonEntity *digimon, FighterData *fighter, int16_t index);
void STD_selectPartnerMove(DigimonEntity *digimon, FighterData *fighter, int16_t index);
void STD_renderCommandMenu(uint8_t id);
void STD_addCommandMenu(uint8_t index);
int32_t STD_addStunEffect(Entity *entity, int32_t val);
int32_t STD_addConfusionEffect(Entity *entity);
int32_t STD_addPoisonEffect(Entity *entity);
void STD_removeStunEffect(int32_t id, Entity *entity);
void STD_removeConfusionEffect(int32_t id, Entity *entity);
void STD_removePoisonEffect(int32_t id, Entity *entity);
int32_t STD_addFinisherAura(Entity *entity, int32_t arg1);
void STD_initializeFinisherChargeup(int16_t tech);
void STD_setupMoveExecution(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int32_t STD_selectMoveTarget(Entity *entity, FighterData *fighter);
void STD_addBracketHitFlash(int16_t i);
void STD_renderBracket(void);
void STD_removeIntroStats(int32_t i);
void STD_removePlayerMarker(void);
int32_t STD_getDigitCount(int32_t value);
void STD_initializePlayerMarker(void);
int16_t STD_getEntityIndex(Entity *entity);
int16_t STD_getEntityIndex2(Entity *entity);
void STD_tickDigimonSenile(DigimonEntity *digimon, FighterData *fighter);
void STD_removeBracket(int16_t mode);
int32_t playMusic(int32_t font, int32_t track);
void STD_addBracket(int16_t track);
void STD_placePlayer1(int16_t which);
void STD_addBracketIntro(void);
void STD_placePlayer2(int16_t which);
void STD_tickBracketIntro(void);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uvX, int32_t uvY, int32_t width, int32_t height);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void STD_renderBracketIntro(void);
void STD_renderTimeoutText(void);
void STD_renderIntroStatBar(int32_t idx, int16_t value);
void STD_tickBracketHitFlash(int32_t i);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount, int32_t *digits);
int32_t STD_getFighterDistance(VECTOR *self, VECTOR *other, VECTOR *target);
int32_t STD_areAllEnemyDigimonDead(void);
void STD_tickFighterCounter(void);
void STD_applyEntityViewpoint(void);
void STD_updateFighterStatusVisuals(DigimonEntity *digimon, FighterData *fighter);
void handlePause(void);
void removePauseBox(void);
int16_t STD_deinitializeCombat(int16_t a, int16_t b);
int16_t STD_checkEndCondition(void);
void STD_initializeCombat(Entity *entity, Entity *other);
int16_t STD_combatMain(Entity *entity, Entity *other);
void STD_setPostIntroPosition(Entity *entity);
void STD_initializeBracket(void);
int16_t STD_getNearestEnemy(Entity *self, int16_t *flags);
void STD_addIntroStats(Entity *entity, int32_t id);
void STD_removeCombatObjects(void);
void STD_selectConfusedMove(DigimonEntity *digimon, FighterData *fighter, long index);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void STD_renderTimeoutWindow(int32_t id);
void SetPolyGT4(POLY_GT4 *prim);
uint32_t playSound(int32_t vabId, int32_t val);
void STD_tickDigimonAttackRanged(DigimonEntity *entity, DigimonEntity *other, FighterData *data, int16_t move);
void STD_renderIntroName(int32_t id);
void STD_tickIntroName(int32_t id);
void STD_renderIntroStatNumber(int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer);
void STD_renderIntroStats(int32_t id);
void STD_tickIntroStats(int32_t id);
void STD_renderIntroNameChar(int16_t x, int16_t y, int16_t size, uint8_t character);
void STD_drawStatLabelText(void);
int32_t BTL_getDistanceSquared(Entity *a, Entity *b);
int32_t BTL_getUsableMoves(int16_t *out, int16_t index);
void BTL_clearConfusion(DigimonEntity *digimon, FighterData *fighter);
void STD_tickVSPhase(void);
void STD_tickFrames(int16_t count);
int16_t BTL_calculateHitChance(DigimonEntity *attacker, DigimonEntity *defender, FighterData *fighter, int16_t move);
void BTL_retargetAfterHit(DigimonEntity *digimon, FighterData *fighter, AttackObject attack);
void BTL_startQueuedMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int32_t STD_isPositionNearEntity(Entity *entity, VECTOR *pos);
int32_t BTL_isMoveOnCooldown(Entity *entity, FighterData *fighter);
void BTL_setupMoveExecution(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void STD_setFlippedUV(POLY_FT4 *prim, int32_t a, int32_t b, int32_t h);
void VS__tickBattleResultScreen(uint8_t hasLostP1, uint8_t hasLostP2);
int32_t STD_isVersusModelSceneFinished(void);
void VS_selectRandomCamera(DigimonEntity *entity, int32_t type, int32_t value);
void STD_loadVersusSceneModel(void);
void STD_addVersusModelScene(void);
void STD_removeVersusModelScene(void);
void STD_setCameraYXZ(void);
void STD_setChampionCamera(void);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
void STD_setViewpointFromBone(Entity *entity, SVECTOR *offset, SVECTOR *rot, int32_t dist);
void STD_setRandomViewpoint(Entity *entity, int32_t idx);
void STD_renderBracketGlyph(int16_t x, int16_t y, uint8_t n, int32_t layer);
int32_t _atan(int32_t y, int32_t x);
int16_t STD_getMostEffectiveMove(int32_t index, int16_t *flags);
int32_t lerp(int32_t a, int32_t b, int32_t lo, int32_t hi, int32_t t);
void STD_updateCameraLerp(int32_t t, int8_t flip);
int32_t STD_interpolateClamped2(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e);
int32_t processSomeArenaArrays(int32_t count, int32_t t, int32_t *keys, int32_t *values, int32_t *slopes);
void STD_tickCameraIntro(void);
void STD_startCameraChase(Entity *entity, int32_t dx, int32_t side);
void STD_setCameraToEntity(void);
void STD_setCameraLookAtEntity(void);
void swapShort(int16_t *a, int16_t *b);
void STD_renderBracketDigimon(int16_t id);
void STD_renderMoveName(int32_t i);
void STD_setCameraOrbit(void);
void STD_removeAllStunEffects(void);
void STD_removeAllFinisherAuras(void);
void STD_removeAllPoisonEffects(void);
void STD_removeAllAuraProjectiles(void);
void STD_unloadAllEFESlots(void);
void STD_removeEFEEngine(void);
int32_t loadSB(void);
void initializeSomeArenaArrays(int32_t count, int32_t arg1, int32_t arg2, int32_t *out);
void STD_renderBracketGlyph(int16_t x, int16_t y, uint8_t n, int32_t layer);
void swapByte(uint8_t *a, uint8_t *b);
void STD_addFighterStatusBars(int32_t id);
void resetFlattenGlobal(void);
void initializeAttackObjects(void);
int32_t STD_areAllEnemyDigimonDead(void);
void STD_selectRandomCamera(DigimonEntity *entity, int32_t mode, uint8_t sub);
void STD_selectFighterTarget(DigimonEntity *digimon, FighterData *fighter, uint8_t target, int32_t arg3);
void removeEntityText(int32_t id);
void STD_removeFighterStatusBars(int32_t i);
int32_t STD_startEFE(int32_t i);

static void *std_main_functions[] = {
	STD_removeCommandMenu,
	STD_renderCommandMenu,
	STD_tickCommandMenu,
	STD_addCommandMenu,
	STD_setCommandIconUV,
	STD_renderMoveName,
	STD_getNearestEnemy,
	STD_selectPartnerMove,
	STD_selectEnemyMove,
	STD_isFighterDefeated,
	STD_selectMoveByMpCost,
	STD_selectMoveByPower,
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
	STD_getUsableMoves,
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
	STD_setupMoveExecution,
	STD_selectMoveTarget,
	STD_startQueuedMove,
	STD_applyChargeRequirement,
	STD_setupQueuedMove,
	STD_getDistanceSquared,
	STD_isMoveUsable,
	STD_battleTickFrame,
	STD_tickFrames,
	STD_tickAttackHits,
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
	STD_renderPlayerMarker,
	STD_startWalkingAnimation2,
	STD_renderTimeoutWindow,
	STD_renderTimeoutText,
	STD_combatMain,
	STD_findUnblockedRotation,
	STD_clearBlockedAttacks,
	STD_getEntityIndex2,
	STD_initializePlayerMarker,
	STD_startWalkingAnimation,
	STD_hasAffordableMoves,
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
	STD_tickDigimonAttackRanged,
	STD_tickMeleeAttack,
	STD_handlePartnerMoveCommand,
	STD_tickQueuedMove,
	STD_tickCooldownState,
	STD_tickChargeState,
	STD_tickDigimonSenile,
	STD_tickConfusedState,
	STD_tickStunState,
	STD_tickFlatState,
	STD_tickHitState,
	STD_tickAttackState,
	STD_getDigitCount,
	STD_faintDigimon,
	STD_addTimeoutWindow,
	STD_tickBattleResultScreen,
	STD_areAllEnemyDigimonDead,
	STD_resetFlatten,
	STD_removePlayerMarker,
	STD_removeCombatObjects,
	STD_deinitializeCombat,
	STD_tickBattle,
	STD_tickEnemyAI,
	STD_tickPartnerAI,
	STD_checkEndCondition,
	STD_combatSetup,
	STD_initializeCombat,
	STD_removeBracketIntro,
	STD_renderBracketIntro,
	STD_tickBracketIntro,
	STD_addBracketIntro,
	STD_renderBracketProjectile,
	STD_tickBracketProjectile,
	STD_tickBracketHitFlash,
	STD_isBracketFinished,
	STD_removeBracket,
	STD_renderBracket,
	STD_tickBracket,
	STD_addBracket,
	STD_setupParticipants,
	STD_initializeBracket,
	STD_renderBracketGlyph,
	STD_setFlippedUV,
	STD_removeBracketProjectile,
	STD_removeBracketHitFlash,
	STD_renderBracketDigimon,
	STD_addBracketHitFlash,
	STD_addBracketProjectile,
	STD_drawBracket,
	STD_updateBracket,
	STD_removeFighterCounter,
	STD_renderFighterCounter,
	STD_tickFighterCounter,
	STD_addFighterCounter,
	STD_renderCounterDigits,
	STD_applyEntityViewpoint,
	STD_removeCameraIntro,
	STD_startCameraIntro,
	STD_tickCameraIntro,
	STD_startCameraChase,
	STD_tickCameraChase,
	STD_interpolateClamped2,
	STD_isPositionNearEntity,
	STD_updateCameraLerp,
	STD_setViewpointFromBone,
	STD_getFighterDistance,
	STD_setChampionCamera,
	STD_setRandomViewpoint,
	STD_selectRandomCamera,
	STD_removeVSPhase,
	STD_tickVSPhase,
	STD_setVSPhase,
	STD_setCameraParams,
	STD_applyRotatingCamera,
	STD_applyViewpoint,
	STD_setCameraLookAtEntity,
	STD_setViewpointRotationFromEntity,
	STD_setCameraToEntity,
	STD_setCameraYXZ,
	STD_setCameraOrbit,
	STD_applyManualCamera,
	STD_renderIntroName,
	STD_tickIntroName,
	STD_renderIntroStatNumber,
	STD_renderIntroStats,
	STD_tickIntroStats,
	STD_runIntro,
	STD_renderIntroNameChar,
	STD_renderIntroStatBar,
	STD_addIntroStats,
	STD_removeIntroStats,
	STD_removeIntroText,
	STD_setPostIntroPosition,
	STD_addIntroText,
	STD_getEntityIndex,
	STD_drawStatLabelText,
	STD_placePlayer2,
	STD_placePlayer1,
};

uint8_t STD_INTRO_NAME_CHAR_SIZES[4] = { 64, 44, 38, 32 };
uint8_t STD_INTRO_NAME_CHAR_OFFSETS[4] = { 16, 7, 3, 0 };
/* HP */
char STD_STR_HP[] = "ＨＰ";
/* MP */
char STD_STR_MP[] = "ＭＰ";
SVECTOR STD_INTRO_CAMERA_STAGE1_POS = { 0 };
SVECTOR STD_INTRO_CAMERA_STAGE1_ROT = { 0, -1592, 0, 0 };
SVECTOR STD_CAMERA_CHASE_OFFSET = { 0 };
SVECTOR STD_CAMERA_CHASE_ROTATION = { 0, -1592, 0, 0 };
SVECTOR MAIN_D_80134838 = { 0 };
SVECTOR MAIN_D_80134840 = { 0 };
SVECTOR STD_CAMERA_INTRO_OFFSET = { 0 };
SVECTOR STD_CAMERA_INTRO_ROTATION = { 0 };
SVECTOR STD_INTRO_CAMERA_STAGE2_POS = { 0 };
SVECTOR STD_INTRO_CAMERA_STAGE2_ROT = { -227, 1479, 0, 0 };
SVECTOR MAIN_D_80134868 = { 0 };
int16_t STD_CARDINAL_ROTATIONS[4] = { 0, 1024, 2048, 3072 };
/* Dealt */
char STD_STR_ATAETA[] = "与えた";
uint8_t STD_BRAIN_TO_COMMAND_MAP[5] = { 2, 3, 4, 5, 6 };
uint8_t STD_YOUR_CALL_POWER_PRIO[4] = { 50, 20, 5, 0 };
uint8_t STD_YOUR_CALL_MP_PRIO[4] = { 50, 20, 10, 0 };
uint8_t STD_YOUR_CALL_WIDE_PRIO[4] = { 10, 5, 0, 0 };
#if VERSION_REGION_IS(NTSCJ)
char STD_STR_COMMAND_RUN[] = "にげる";
#else
char STD_STR_COMMAND_RUN[] = "Run";
char STD_STR_COMMAND_ATTACK[] = "Attack";
char STD_STR_COMMAND_AUTO[] = "Auto";
char STD_STR_COMMAND_CHANGE[] = "Change";
#endif
uint8_t STD_COMMAND_LABEL_U[5] = { 0, 11, 25, 39, 50 };
uint8_t STD_COMMAND_LABEL_W[5] = { 11, 14, 14, 11, 11 };

int16_t STD_STARTING_HP[2];
int16_t STD_DAMAGE[2];
int32_t STD_COMBAT_ACTIVE;
int32_t STD_LOADING_VAB_ID;
int32_t MAIN_D_8013510C;
uint8_t STD_TIMER;
int32_t STD_TIMER_ACTIVE;
int32_t STD_DISABLE_HITTING;
int16_t STD_DEFAULT_CAM_MIN_DISTANCE;
int32_t STD_PAD2_INPUT_PREVIOUS;
int32_t STD_PAD2_INPUT;
Entity *STD_FOCUSED_ENTITY;
uint8_t STD_CAMERA_TIMER;
TMDModel *STD_ARENA_MODEL;
int32_t STD_INTRO_STATS_ACTIVE;
int16_t STD_INTRO_DATA_FRAME_COUNT;
int16_t STD_INTRO_DATA_RENDERED_CHARACTERS;
uint8_t STD_INTRO_DATA_COLOR;
int16_t STD_INTRO_DATA_POS_X;
int16_t STD_INTRO_DATA_POS_Y;
uint8_t STD_INTRO_DATA_ANIM_FRAME;
CameraChase STD_INTRO_CAMERA_CHASE;
int32_t STD_IS_TIMER_INITIALIZED;
uint8_t STD_BRACKET_MATCH;
uint8_t STD_PLAYER_SLOT;
uint8_t PARTICIPANT_TYPES[8];
uint8_t STD_ROUND1_WINNERS[4];
uint8_t STD_ROUND2_WINNERS[2];
int32_t STD_BRACKET_FINISHED;
uint8_t STD_BRACKET_FADE;
uint8_t MAIN_D_80135165;
uint8_t MAIN_D_80135166;
uint8_t STD_BRACKET_INTRO_TIMER;
uint8_t STD_SAVED_CHARGE_MODE;
int32_t STD_IS_DRAW;
int16_t STD_VICTORY_FRAMES;
uint8_t STD_BATTLE_RESULT_TIMER;
int32_t STD_FINISHER_TIMER;
int32_t STD_FINISHER_AURA_ID;
int16_t STD_COMMAND_MENU_TOP[2];
int16_t STD_COMMAND_MENU_BOTTOM[2];
uint8_t STD_COMMAND_MENU_BLINK[2];
uint8_t STD_COMMAND_MENU_TIMER[2];
uint8_t STD_COMMAND_MENU_LAYOUT[2];
int8_t MAIN_D_8013518A[2];

static void *std_main_sbss_order[] = {
	&MAIN_D_8013518A,
	&STD_COMMAND_MENU_LAYOUT,
	&STD_COMMAND_MENU_TIMER,
	&STD_COMMAND_MENU_BLINK,
	&STD_COMMAND_MENU_BOTTOM,
	&STD_COMMAND_MENU_TOP,
	&STD_FINISHER_AURA_ID,
	&STD_FINISHER_TIMER,
	&STD_BATTLE_RESULT_TIMER,
	&STD_VICTORY_FRAMES,
	&STD_IS_DRAW,
	&STD_SAVED_CHARGE_MODE,
	&STD_BRACKET_INTRO_TIMER,
	&MAIN_D_80135166,
	&MAIN_D_80135165,
	&STD_BRACKET_FADE,
	&STD_BRACKET_FINISHED,
	STD_ROUND2_WINNERS,
	&STD_ROUND1_WINNERS,
	&PARTICIPANT_TYPES,
	&STD_PLAYER_SLOT,
	&STD_BRACKET_MATCH,
	&STD_IS_TIMER_INITIALIZED,
	&STD_INTRO_CAMERA_CHASE,
	&STD_INTRO_DATA_ANIM_FRAME,
	&STD_INTRO_DATA_POS_Y,
	&STD_INTRO_DATA_POS_X,
	&STD_INTRO_DATA_COLOR,
	&STD_INTRO_DATA_RENDERED_CHARACTERS,
	&STD_INTRO_DATA_FRAME_COUNT,
	&STD_INTRO_STATS_ACTIVE,
	&STD_ARENA_MODEL,
	&STD_CAMERA_TIMER,
	&STD_FOCUSED_ENTITY,
	&STD_PAD2_INPUT,
	&STD_PAD2_INPUT_PREVIOUS,
	&STD_DEFAULT_CAM_MIN_DISTANCE,
	&STD_DISABLE_HITTING,
	&STD_TIMER_ACTIVE,
	&STD_TIMER,
	&MAIN_D_8013510C,
	&STD_LOADING_VAB_ID,
	&STD_COMBAT_ACTIVE,
	STD_DAMAGE,
	STD_STARTING_HP,
};

// clang-format off
uint8_t STD_INTRO_DIGIMON_NAMES[112][14] = {
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

char STD_PATH_ARENA_MODEL_STDDAT_T_TOGI_TMD[] = "\\STDDAT\\T_TOGI.TMD";

char STD_PATH_ARENA_MODEL_STDDAT_B_TOGI_TMD[] = "\\STDDAT\\B_TOGI.TMD";

char *STD_ARENA_MODELS[3] = {
	STD_PATH_ARENA_MODEL_STDDAT_T_TOGI_TMD,
	STD_PATH_ARENA_MODEL_STDDAT_B_TOGI_TMD,
	(void *)0x00000000,
};

char STD_PATH_ARENA_TIM_STDDAT_T_TOGI_TIM[] = "\\STDDAT\\T_TOGI.TIM";

char STD_PATH_ARENA_TIM_STDDAT_B_TOGI_TIM[] = "\\STDDAT\\B_TOGI.TIM";

char *STD_ARENA_TIMS[3] = {
	STD_PATH_ARENA_TIM_STDDAT_T_TOGI_TIM,
	STD_PATH_ARENA_TIM_STDDAT_B_TOGI_TIM,
	(void *)0x00000000,
};

char STD_PATH_ARENA_COLLISION_STDDAT_B_TOGI_ATR[] = "\\STDDAT\\B_TOGI.ATR";

char *STD_ARENA_COLLISIONS[3] = {
	STD_PATH_ARENA_COLLISION_STDDAT_B_TOGI_ATR,
	STD_PATH_ARENA_COLLISION_STDDAT_B_TOGI_ATR,
	(void *)0x00000000,
};

int16_t STD_STAT_BAR_LIMITS[6] = {
	0x270f, 0x270f, 0x03e7, 0x03e7, 0x03e7, 0x03e7,
};

RGB8 STD_INTRO_NAME_COLORS[10] = {
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

CameraPreset STD_CAMERA_PRESETS[9] = {
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

int16_t STD_RANDOM_VIEWPOINTS[5][3] = {
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

int8_t STD_BRACKET_HOP_Y[24] = {
	0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x03, 0x04,
	0x05, 0x06, 0x07, 0x08, 0x0a, 0x0c, 0x0e, 0x10,
	0x12, 0x14, 0x17, 0x1a, 0x18, 0x17, 0x18, 0x1a,
};

int16_t STD_BRACKET_ROW_Y[8] = {
	0x001b, 0x0001, 0x0001, 0xffe7, 0xffe7, 0xffcd, 0xffcd, 0xffb8,
};

uint8_t STD_DIGIMON_SPRITE_CLUT[112] = {
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

uint8_t STD_BRACKET_PROJECTILE_ICONS[112] = {
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

uint8_t STD_BRACKET_PROJECTILE_CLUTS[16] = {
	0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x02,
	0x00, 0x02, 0x03, 0x03, 0x00, 0x03, 0x02, 0x00,
};

uint8_t STD_WIN_CHANCES[4][3] = {
	{ 0x32, 0x14, 0x05 },
	{ 0x50, 0x32, 0x0f },
	{ 0x5f, 0x55, 0x32 },
	{ 0x00, 0x00, 0x00 },
};

StdSrcA598 STD_BRACKET_PATHS[8] = {
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

#if VERSION_REGION_IS(NTSCJ)
char STD_STR_COMMAND_ATTACK[] = "おもいっきり";
char STD_STR_COMMAND_AUTO[] = "おまかせ";
char STD_STR_COMMAND_MODERATE[] = "ほどほど";
char STD_STR_COMMAND_DISTANCE[] = "はなれる";
char STD_STR_COMMAND_DEFENSIVE[] = "ガマンする";
char STD_STR_COMMAND_CHANGE[] = "ターゲットをかえる";
#else
char STD_STR_COMMAND_MODERATE[] = "Moderate";

char STD_STR_COMMAND_DISTANCE[] = "Distance";

char STD_STR_COMMAND_DEFENSIVE[] = "Defensive";
#endif

char *STD_COMMAND_NAMES[8] = {
	STD_STR_COMMAND_RUN,
	STD_STR_COMMAND_ATTACK,
	STD_STR_COMMAND_AUTO,
	STD_STR_COMMAND_MODERATE,
	STD_STR_COMMAND_DISTANCE,
	STD_STR_COMMAND_DEFENSIVE,
	STD_STR_COMMAND_CHANGE,
	(void *)0x00000000,
};

uint8_t STD_COMMAND_MENU_LAYOUTS[8][10] = {
	{ 0x00, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x04, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x03, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x04, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x03, 0xff },
};

uint8_t STD_COMMAND_ICON_UVS[8][2] = {
	{ 0x00, 0xc0 },
	{ 0x20, 0xc0 },
	{ 0x40, 0xc0 },
	{ 0x60, 0xc0 },
	{ 0x80, 0xc0 },
	{ 0xa0, 0xc0 },
	{ 0xc0, 0xc0 },
	{ 0x00, 0x00 },
};

uint8_t STD_SPECIAL_ICON_UVS[8][2] = {
	{ 0x00, 0xd0 },
	{ 0x20, 0xd0 },
	{ 0x40, 0xd0 },
	{ 0x60, 0xd0 },
	{ 0x80, 0xd0 },
	{ 0xa0, 0xd0 },
	{ 0xc0, 0xd0 },
	{ 0x00, 0x00 },
};
// clang-format on

void STD_removeCombatObjects(void)
{
	int32_t i;
	Entity *entity;

	if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]]->type == ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->type) {
		STD_removePlayerMarker();
	}
	STD_removeAllStunEffects();
	STD_removeAllFinisherAuras();
	STD_removeAllPoisonEffects();
	STD_removeAllAuraProjectiles();

	for (i = 0; i <= ENEMY_COUNT; ++i) {
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		STD_removeMoveEffect((DigimonEntity *)entity, &COMBAT_DATA_PTR->fighter[i]);
	}

	STD_unloadAllEFESlots();
	STD_removeEFEEngine();
}

void STD_placePlayer1(int16_t which)
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

int16_t STD_getEntityIndex(Entity *entity)
{
	int32_t i;

	for (i = 0; i < 10; i++) {
		if (ENTITY_TABLE[i] == entity) {
			return i;
		}
	}
}

void STD_placePlayer2(int16_t which)
{
	int16_t id;
	int16_t x;

	if (which == 1) {
		x = -0x708;
	} else {
		x = -0x9c4;
	}
	id = STD_getEntityIndex(STD_OPPONENT_ENTITY);
	setEntityPosition(id, x, 0, 0);
	setEntityRotation(id, 0, 0xc00, 0);
	startAnimation(STD_OPPONENT_ENTITY, 0x21);
}

void STD_drawStatLabelText(void)
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
void STD_addIntroText(entity, id)
	Entity *entity;
	int16_t id;
// clang-format on
{
	int32_t len;

	STD_INTRO_DATA_FRAME_COUNT = 4;
	STD_INTRO_DATA_RENDERED_CHARACTERS = 0;

	switch (DIGIMON_DATA[entity->type].special[0]) {
	case 0:
		STD_INTRO_DATA_COLOR = 3;
		break;
	case 1:
		STD_INTRO_DATA_COLOR = 1;
		break;
	case 2:
		STD_INTRO_DATA_COLOR = 6;
		break;
	case 3:
		STD_INTRO_DATA_COLOR = 2;
		break;
	case 4:
		STD_INTRO_DATA_COLOR = 4;
		break;
	case 5:
		STD_INTRO_DATA_COLOR = 0;
		break;
	case 6:
		STD_INTRO_DATA_COLOR = 5;
		break;
	default:
		STD_INTRO_DATA_COLOR = 0;
		break;
	}

	len = strlen(DIGIMON_DATA[entity->type].name) / 2;
	if (entity->type == 0x4e || entity->type == 0x3c) {
		len = 10;
	}

	STD_INTRO_DATA_POS_X = -(len * 16);
	STD_INTRO_DATA_POS_Y = 68;
	addObject(0x1ab, id, STD_tickIntroName, STD_renderIntroName);
}

void STD_setPostIntroPosition(Entity *entity)
{
	if (STD_INTRO_CAMERA_CHASE.timer != -1) {
		entity->posData->location = STD_INTRO_TARGET_POS;
		entity->anim.locX = STD_INTRO_TARGET_POS.vx << 15;
		entity->anim.locY = STD_INTRO_TARGET_POS.vy << 15;
		entity->anim.locZ = STD_INTRO_TARGET_POS.vz << 15;
		startAnimation(entity, 0x21);
		STD_INTRO_CAMERA_CHASE.timer = -1;
	}
}

// clang-format off
void STD_removeIntroText(i)
	int16_t i;
// clang-format on
{
	removeObject(0x1ab, i);
}

// clang-format off
void STD_removeIntroStats(i)
	int16_t i;
// clang-format on
{
	if (STD_INTRO_STATS_ACTIVE != 0) {
		STD_INTRO_STATS_ACTIVE = 0;
		removeObject(0x1a9, i);
	}
}

// clang-format off
void STD_addIntroStats(entity, id)
	Entity *entity;
	int16_t id;
// clang-format on
{
	if (STD_INTRO_STATS_ACTIVE != 1) {
		STD_INTRO_STATS_ACTIVE = 1;
		STD_D_8007B9BC[0] = -100;
		STD_D_8007B9BC[1] = -100;
		STD_D_8007B9BC[2] = -10;
		STD_D_8007B9BC[3] = -10;
		STD_D_8007B9BC[4] = -10;
		STD_D_8007B9BC[5] = -10;
		addObject(0x1a9, id, STD_tickIntroStats, STD_renderIntroStats);
	}
}

void STD_renderIntroStatBar(int32_t idx, int16_t value)
{
	POLY_F4 *prim;
	int16_t width;

	prim = (POLY_F4 *)GsGetWorkBase();
	SetPolyF4(prim);
	setRGB0(prim, 0x50, 0xc8, 0x50);
	width = (value * 100) / STD_STAT_BAR_LIMITS[idx];
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
		STD_renderMoveName(id);
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
				y = STD_COMMAND_MENU_TOP[id];
			} else {
				y = STD_COMMAND_MENU_TOP[id] + 0xa;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		} else {
			int32_t y;

			if ((COMBAT_DATA_PTR->player.hoveredCommand[id] % 2) == 1) {
				y = STD_COMMAND_MENU_TOP[id];
			} else {
				y = STD_COMMAND_MENU_TOP[id] + 0xa;
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
			setXY4(prim, x, ((i % 2) == 0) ? STD_COMMAND_MENU_TOP[id] + 3 : STD_COMMAND_MENU_TOP[id] + 0xd, x + 0x10, ((i % 2) == 0) ? STD_COMMAND_MENU_TOP[id] + 3 : STD_COMMAND_MENU_TOP[id] + 0xd, x, ((i % 2) == 0) ? STD_COMMAND_MENU_BOTTOM[id] - 0xd : STD_COMMAND_MENU_BOTTOM[id] - 3, x + 0x10, ((i % 2) == 0) ? STD_COMMAND_MENU_BOTTOM[id] - 0xd : STD_COMMAND_MENU_BOTTOM[id] - 3);
		} else {
			setXY4(prim, x, ((i % 2) == 1) ? STD_COMMAND_MENU_TOP[id] + 3 : STD_COMMAND_MENU_TOP[id] + 0xd, x + 0x10, ((i % 2) == 1) ? STD_COMMAND_MENU_TOP[id] + 3 : STD_COMMAND_MENU_TOP[id] + 0xd, x, ((i % 2) == 1) ? STD_COMMAND_MENU_BOTTOM[id] - 0xd : STD_COMMAND_MENU_BOTTOM[id] - 3, x + 0x10, ((i % 2) == 1) ? STD_COMMAND_MENU_BOTTOM[id] - 0xd : STD_COMMAND_MENU_BOTTOM[id] - 3);
		}
		if ((i == COMBAT_DATA_PTR->player.hoveredCommand[id]) && (STD_COMMAND_MENU_BLINK[id] == 1)) {
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
		setUVWH(prim, STD_COMMAND_LABEL_U[STD_COMMAND_MENU_LAYOUTS[STD_COMMAND_MENU_LAYOUT[id]][i]], 0xe0, STD_COMMAND_LABEL_W[STD_COMMAND_MENU_LAYOUTS[STD_COMMAND_MENU_LAYOUT[id]][i]], 31);
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
		setXY4(prim, (rowY - 0x8f) + id * 0xa6, STD_COMMAND_MENU_TOP[id], ((rowY - 0x8f) + width) + id * 0xa6, STD_COMMAND_MENU_TOP[id], (rowY - 0x8f) + id * 0xa6, STD_COMMAND_MENU_BOTTOM[id], ((rowY - 0x8f) + width) + id * 0xa6, STD_COMMAND_MENU_BOTTOM[id]);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void STD_renderIntroNameChar(int16_t x, int16_t y, int16_t size, uint8_t character)
{
	POLY_GT4 *prim;
	uint8_t u;
	uint8_t v;

	prim = (POLY_GT4 *)GsGetWorkBase();

	SetPolyGT4(prim);
	prim->tpage = getTPage(0, 0, 768, 0);
	setClut(prim, 0, 480);
	setRGB0(prim, STD_INTRO_NAME_COLORS[STD_INTRO_DATA_COLOR].r, STD_INTRO_NAME_COLORS[STD_INTRO_DATA_COLOR].g, STD_INTRO_NAME_COLORS[STD_INTRO_DATA_COLOR].b);
	setRGB1(prim, STD_INTRO_NAME_COLORS[STD_INTRO_DATA_COLOR].r, STD_INTRO_NAME_COLORS[STD_INTRO_DATA_COLOR].g, STD_INTRO_NAME_COLORS[STD_INTRO_DATA_COLOR].b);
	setRGB2(prim, STD_INTRO_NAME_COLORS[STD_INTRO_DATA_COLOR].r / 10, STD_INTRO_NAME_COLORS[STD_INTRO_DATA_COLOR].g / 10, STD_INTRO_NAME_COLORS[STD_INTRO_DATA_COLOR].b / 10);
	setRGB3(prim, STD_INTRO_NAME_COLORS[STD_INTRO_DATA_COLOR].r / 10, STD_INTRO_NAME_COLORS[STD_INTRO_DATA_COLOR].g / 10, STD_INTRO_NAME_COLORS[STD_INTRO_DATA_COLOR].b / 10);

	u = (character % 32) * 32;
	v = (character / 8) * 32;

	if (size < 64) {
		setUVWH(prim, u, v, (u != 0xe0 ? 32 : 31), (v != 0xe0 ? 32 : 31));
	} else {
		setUVWH(prim, u, v, 31, 31);
	}

	setXYWH(prim, x, y, size, size);
#if VERSION_REGION_IS(NTSCJ)
	AddPrim(ACTIVE_ORDERING_TABLE->org + 3, prim++);
#endif

	GsSetWorkBase((PACKET *)prim);
}

void STD_runIntro(int16_t which)
{
	int32_t dist;
#if VERSION_REGION_IS(NTSCJ)
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

	((DigimonEntity *)STD_OPPONENT_ENTITY)->stats.current.vabId = 5;
	STD_LOADING_VAB_ID = readVBALLSection(5, STD_OPPONENT_ENTITY->type);
	STD_placePlayer1(which);
	STD_placePlayer2(which);
	STD_drawStatLabelText();
	STD_startCameraChase(ENTITY_TABLE[1], dist, 0);
#if VERSION_REGION_IS(NTSCJ)
	id = STD_getEntityIndex(ENTITY_TABLE[1]);
#else
	idn = STD_getEntityIndex(ENTITY_TABLE[1]);
	id = idn;
#endif
	STD_addIntroText(ENTITY_TABLE[1], id);
	stopBGM();
	stopSound();
	playMusic(STD_MUSIC, 0);

	prev = 0;
	while ((ENTITY_TABLE[1]->anim.animFlag & 1) != 0) {
		pad = PadRead(1);
		STD_battleTickFrame();
		if (((pad & ~prev) & CONFIRM_BUTTON) != 0) {
			STD_setPostIntroPosition(ENTITY_TABLE[1]);
			prev = pad;
			break;
		}
		prev = pad;
	}

	STD_removeIntroText(id);
	STD_removeIntroStats(id);
	removeObject(0x1aa, 0);
	stopBGM();
	stopSound();
	isSoundLoaded(0, STD_LOADING_VAB_ID);
	STD_LOADING_VAB_ID = loadSB();

	STD_startCameraChase(STD_OPPONENT_ENTITY, -dist, 1);
#if VERSION_REGION_IS(NTSCJ)
	id = STD_getEntityIndex(STD_OPPONENT_ENTITY);
#else
	idn = STD_getEntityIndex(STD_OPPONENT_ENTITY);
	id = idn;
#endif
	STD_addIntroText(STD_OPPONENT_ENTITY, id);
	playMusic(STD_MUSIC, 1);

	while ((STD_OPPONENT_ENTITY->anim.animFlag & 1) != 0) {
		pad = PadRead(1);
		STD_battleTickFrame();
		if (((pad & ~prev) & CONFIRM_BUTTON) != 0) {
			STD_setPostIntroPosition(STD_OPPONENT_ENTITY);
			prev = pad;
			break;
		}
		prev = pad;
	}

	STD_removeIntroText(id);
	STD_removeIntroStats(id);
	removeObject(0x1aa, 0);
	stopBGM();
	stopSound();
	isSoundLoaded(0, STD_LOADING_VAB_ID);
}

// clang-format off
void STD_tickIntroStats(id)
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
void STD_renderIntroStats(id)
	int16_t id;
// clang-format on
{
	Stats *stats;
	int32_t i;

	for (i = 0; i < 6; ++i) {
		renderString(0, -100, i * 16 - 28, 48, 12, 0, i * 12, 0, 1);
		STD_renderIntroStatBar((int16_t)i, STD_D_8007B9BC[i]);
	}

	stats = &((DigimonEntity *)ENTITY_TABLE[id])->stats;
	if (STD_D_8007B9BC[0] != stats->current.currentHP || STD_D_8007B9BC[1] != stats->current.currentMP || STD_D_8007B9BC[2] != stats->base.off || STD_D_8007B9BC[3] != stats->base.def || STD_D_8007B9BC[4] != stats->base.speed || STD_D_8007B9BC[5] != stats->base.brain) {
		playSound(0, 0x16);
	} else {
		for (i = 0; i < 6; ++i) {
			STD_renderIntroStatNumber(52, (int16_t)(i * 16 - 28), 4, STD_D_8007B9BC[i], 3);
		}
	}
}

// clang-format off
void STD_renderIntroStatNumber(x, y, digits, value, layer)
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

void STD_tickIntroName(int32_t id)
{
	int32_t len;

	++STD_INTRO_DATA_FRAME_COUNT;

	len = strlen(DIGIMON_DATA[ENTITY_TABLE[id]->type].name) / 2;
	if (ENTITY_TABLE[id]->type == 0x4e || ENTITY_TABLE[id]->type == 0x3c) {
		len = 10;
	}

	if (len == STD_INTRO_DATA_RENDERED_CHARACTERS && STD_INTRO_DATA_ANIM_FRAME == 3) {
		if (STD_INTRO_CAMERA_CHASE.timer == 0) {
			startAnimation(ENTITY_TABLE[id], 0x23);
			STD_INTRO_CAMERA_CHASE.timer = 20;
		}

		if (STD_INTRO_DATA_POS_Y >= -71) {
			STD_INTRO_DATA_POS_Y -= 28;
		} else {
			STD_addIntroStats(ENTITY_TABLE[id], id);
		}
	}
}

// clang-format off
void STD_renderIntroName(id)
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

	if (STD_INTRO_DATA_FRAME_COUNT % 4 == 0) {
		if (STD_INTRO_DATA_RENDERED_CHARACTERS < charCount) {
			++STD_INTRO_DATA_RENDERED_CHARACTERS;
			STD_INTRO_DATA_ANIM_FRAME = 0;
		}
	} else if (STD_INTRO_DATA_ANIM_FRAME != 3) {
		++STD_INTRO_DATA_ANIM_FRAME;
	}

	charIndex = 0;
	for (i = 0; i < STD_INTRO_DATA_RENDERED_CHARACTERS; ++i) {
		character = STD_INTRO_DIGIMON_NAMES[ENTITY_TABLE[id]->type][charIndex++];
		if (character == 0x3d) {
			character = STD_INTRO_DIGIMON_NAMES[ENTITY_TABLE[id]->type][charIndex++];
		}

		if (i == STD_INTRO_DATA_RENDERED_CHARACTERS - 1) {
			y = STD_INTRO_DATA_POS_Y - STD_INTRO_NAME_CHAR_OFFSETS[STD_INTRO_DATA_ANIM_FRAME];
			size = STD_INTRO_NAME_CHAR_SIZES[STD_INTRO_DATA_ANIM_FRAME];
		} else {
			y = STD_INTRO_DATA_POS_Y;
			size = 32;
		}

		STD_renderIntroNameChar((int16_t)STD_INTRO_DATA_POS_X + i * 32, y, size, character);

		if (character == 0x1f || character == 0x25) {
			character = STD_INTRO_DIGIMON_NAMES[ENTITY_TABLE[id]->type][charIndex++];
			STD_renderIntroNameChar((int16_t)STD_INTRO_DATA_POS_X + i * 32, y, size, character);
		}
	}
}

void STD_applyManualCamera(void)
{
	if (STD_PAD2_INPUT & 0x1000) {
		STDVS_CAMERA.translation.vy += 0x14;
	}
	if (STD_PAD2_INPUT & 0x4000) {
		STDVS_CAMERA.translation.vy -= 0x14;
	}
	if (STD_PAD2_INPUT & 0x8000) {
		STDVS_CAMERA.translation.vx += 0x14;
	}
	if (STD_PAD2_INPUT & 0x2000) {
		STDVS_CAMERA.translation.vx -= 0x14;
	}
	if (STD_PAD2_INPUT & 0x4) {
		STDVS_CAMERA.translation.vz -= 0x14;
	}
	if (STD_PAD2_INPUT & 0x1) {
		STDVS_CAMERA.translation.vz += 0x14;
	}
	if (STD_PAD2_INPUT & ALT_BUTTON) {
		STDVS_CAMERA.rotation.vx += 0x20;
		STDVS_CAMERA.rotation.vx &= 0xfff;
	}
	if (STD_PAD2_INPUT & CANCEL_BUTTON) {
		STDVS_CAMERA.rotation.vx -= 0x20;
		STDVS_CAMERA.rotation.vx &= 0xfff;
	}
	if (STD_PAD2_INPUT & 0x80) {
		STDVS_CAMERA.rotation.vy -= 0x20;
		STDVS_CAMERA.rotation.vy &= 0xfff;
	}
	if (STD_PAD2_INPUT & CONFIRM_BUTTON) {
		STDVS_CAMERA.rotation.vy += 0x20;
		STDVS_CAMERA.rotation.vy &= 0xfff;
	}
	if (STD_PAD2_INPUT & 0x800) {
		STDVS_CAMERA.rotation.vx = 0;
		STDVS_CAMERA.rotation.vy = 0;
		STDVS_CAMERA.rotation.vz = 0;
		STDVS_CAMERA.translation.vx = 0;
		STDVS_CAMERA.translation.vy = 0;
		STDVS_CAMERA.translation.vz = 0xbb8;
	}
	STDVS_CAMERA.view.super = NULL;
	RotMatrix(&STDVS_CAMERA.rotation, &STDVS_CAMERA.view.view);
	TransMatrix(&STDVS_CAMERA.view.view, &STDVS_CAMERA.translation);
	STDVS_CAMERA.coord.flg = 0;
	GsSetView2(&STDVS_CAMERA.view);
}

void STD_setCameraOrbit(void)
{
	VECTOR *a;
	VECTOR *b;
	VECTOR diff;
	int32_t dist;
	int32_t d;
	int32_t ang;

	a = &STD_OPPONENT_ENTITY->posData->location;
	b = &PARTNER_ENTITY.digimonEntity.entity.posData->location;
	diff.vx = a->vx - b->vx;
	diff.vy = 0;
	diff.vz = a->vz - b->vz;
	dist = SquareRoot0(diff.vx * diff.vx + diff.vz * diff.vz);
	d = (dist * VIEWPORT_DISTANCE) / 200u;
	if (d < (int16_t)STD_DEFAULT_CAM_MIN_DISTANCE) {
		d = STD_DEFAULT_CAM_MIN_DISTANCE;
	}
	if (d < (int16_t)STD_DEFAULT_CAM_MIN_DISTANCE + 0x12c) {
		MAIN_D_8013510C = 1;
	} else {
		MAIN_D_8013510C = 0;
	}
	if (d > 0x1068) {
		d = 0x1068;
	}
	ang = 0x1000 - _atan(diff.vx, diff.vz);
	STDVS_CAMERA.translation.vx = b->vx + (int32_t)diff.vx / 2 + (d * diff.vz) / dist;
	STDVS_CAMERA.translation.vy = -0x3e8;
	STDVS_CAMERA.translation.vz = b->vz + diff.vz / 2 - (d * diff.vx) / dist;
	STDVS_CAMERA.rotation.vx = _atan(d, -0x2bc) + 0x800;
	STDVS_CAMERA.rotation.vy = ang + 0x800;
	STDVS_CAMERA.rotation.vz = 0;
	STDVS_CAMERA.view.view = GsIDMATRIX;
	STDVS_CAMERA.view.super = &STDVS_CAMERA.coord;
	RotMatrixYXZ(&STDVS_CAMERA.rotation, &STDVS_CAMERA.coord.coord);
	TransMatrix(&STDVS_CAMERA.coord.coord, &STDVS_CAMERA.translation);
	STDVS_CAMERA.coord.flg = 0;
	GsSetView2(&STDVS_CAMERA.view);
}

void STD_setCameraYXZ(void)
{
	VECTOR *a;
	VECTOR *b;

	a = &PARTNER_ENTITY.digimonEntity.entity.posData->location;
	b = &STD_OPPONENT_ENTITY->posData->location;
	STDVS_CAMERA.view.view = GsIDMATRIX;
	STDVS_CAMERA.view.super = &STDVS_CAMERA.coord;
	STDVS_CAMERA.translation.vx = a->vx + (b->vx - a->vx) / 2;
	STDVS_CAMERA.translation.vz = a->vz + (b->vz - a->vz) / 2;
	STDVS_CAMERA.translation.vy = -0x1f40;
	STDVS_CAMERA.rotation.vy = STDVS_CAMERA.rotation.vz = 0;
	STDVS_CAMERA.rotation.vx = -0x400;
	RotMatrixYXZ(&STDVS_CAMERA.rotation, &STDVS_CAMERA.coord.coord);
	TransMatrix(&STDVS_CAMERA.coord.coord, &STDVS_CAMERA.translation);
	STDVS_CAMERA.coord.flg = 0;
	GsSetView2(&STDVS_CAMERA.view);
}

void STD_setCameraToEntity(void)
{
	SVECTOR rot;
	VECTOR v;
	VECTOR out;

	rot = STDVS_CAMERA.rotation;
	rot.vy -= STD_FOCUSED_ENTITY->posData->rotation.vy;
	rot.vy &= 0xfff;
	RotMatrix(&rot, &STDVS_CAMERA.view.view);
	v = STDVS_CAMERA.translation;
	ApplyMatrixLV(&STDVS_CAMERA.view.view, &STD_FOCUSED_ENTITY->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix(&STDVS_CAMERA.view.view, &v);
	STDVS_CAMERA.coord.flg = 0;
	GsSetView2(&STDVS_CAMERA.view);
}

void STD_tickVSPhase(void)
{
	STD_PAD2_INPUT = (POLLED_INPUT >> 16) & 0xffff;
	STD_PAD2_INPUT_PREVIOUS = (POLLED_INPUT_PREVIOUS >> 16) & 0xffff;

	switch (STD_CAMERA_STATE) {
	case 0:
		STD_applyManualCamera();
		break;
	case 1:
		STD_setCameraOrbit();
		break;
	case 2:
		STD_setCameraYXZ();
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
		STD_setCameraLookAtEntity();
		break;
	case 8:
		STD_applyViewpoint();
		break;
	case 10:
		STD_applyRotatingCamera();
		break;
	case 11:
		STD_applyViewpoint();
		break;
	}
}

void STD_setViewpointRotationFromEntity(void)
{
	MATRIX *m;

	m = &STD_FOCUSED_ENTITY->posData[1].posMatrix.workm;
	GS_VIEWPOINT.vrx = m->t[0];
	GS_VIEWPOINT.vry = m->t[1];
	GS_VIEWPOINT.vrz = m->t[2];
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_setCameraLookAtEntity(void)
{
	VECTOR v;
	VECTOR out;
	VECTOR diff;
	VECTOR *selfPos;
	VECTOR *otherPos;
	Entity *other;

	selfPos = &STD_FOCUSED_ENTITY->posData->location;
	if (STD_FOCUSED_ENTITY == ENTITY_TABLE[1]) {
		other = STD_OPPONENT_ENTITY;
	} else {
		other = ENTITY_TABLE[1];
	}
	otherPos = &other->posData->location;
	diff.vx = otherPos->vx - selfPos->vx;
	diff.vy = 0;
	diff.vz = otherPos->vz - selfPos->vz;
	STDVS_CAMERA.rotation.vy = (-_atan(diff.vz, diff.vx) + 0x800) & 0xfff;
	RotMatrix(&STDVS_CAMERA.rotation, &STDVS_CAMERA.view.view);
	v = STDVS_CAMERA.translation;
	ApplyMatrixLV(&STDVS_CAMERA.view.view, &STD_FOCUSED_ENTITY->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix(&STDVS_CAMERA.view.view, &v);
	GsSetView2(&STDVS_CAMERA.view);
}

void STD_applyViewpoint(void)
{
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_applyRotatingCamera(void)
{
	if (STD_PAD2_INPUT & 0x1000) {
		STDVS_CAMERA.translation.vy += 0x14;
	}
	if (STD_PAD2_INPUT & 0x4000) {
		STDVS_CAMERA.translation.vy -= 0x14;
	}
	if (STD_PAD2_INPUT & 0x8000) {
		STDVS_CAMERA.translation.vx += 0x14;
	}
	if (STD_PAD2_INPUT & 0x2000) {
		STDVS_CAMERA.translation.vx -= 0x14;
	}
	if (STD_PAD2_INPUT & 0x4) {
		STDVS_CAMERA.translation.vz -= 0x14;
	}
	if (STD_PAD2_INPUT & 0x1) {
		STDVS_CAMERA.translation.vz += 0x14;
	}
	if (STD_PAD2_INPUT & ALT_BUTTON) {
		STDVS_CAMERA.rotation.vx += 0x20;
		STDVS_CAMERA.rotation.vx &= 0xfff;
	}
	if (STD_PAD2_INPUT & CANCEL_BUTTON) {
		STDVS_CAMERA.rotation.vx -= 0x20;
		STDVS_CAMERA.rotation.vx &= 0xfff;
	}
	STDVS_CAMERA.rotation.vy += 2;
	STDVS_CAMERA.rotation.vy &= 0xfff;
	if (STD_PAD2_INPUT & CONFIRM_BUTTON) {
		STDVS_CAMERA.rotation.vy += 0x20;
		STDVS_CAMERA.rotation.vy &= 0xfff;
	}
	STDVS_CAMERA.view.super = NULL;
	RotMatrix(&STDVS_CAMERA.rotation, &STDVS_CAMERA.view.view);
	TransMatrix(&STDVS_CAMERA.view.view, &STDVS_CAMERA.translation);
	STDVS_CAMERA.coord.flg = 0;
	GsSetView2(&STDVS_CAMERA.view);
}

void STD_setCameraParams(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f)
{
	STDVS_CAMERA.rotation.vx = a;
	STDVS_CAMERA.rotation.vy = b;
	STDVS_CAMERA.rotation.vz = c;
	STDVS_CAMERA.translation.vx = d;
	STDVS_CAMERA.translation.vy = e;
	STDVS_CAMERA.translation.vz = f;
}

void STD_setVSPhase(int32_t arg)
{
	addObject(0x1a8, 0, (TickFunction)STD_tickVSPhase, NULL);
	STD_CAMERA_STATE = arg;
	STD_CAMERA_TIMER = 0;
}

void STD_removeVSPhase(void)
{
	removeObject(0x1a8, 0);
}

void STD_selectRandomCamera(DigimonEntity *entity, int32_t mode, uint8_t sub)
{
	CameraPreset *p;

	if (mode != 5) {
		if (randomLimit(3) != 0) {
			return;
		}
	}
	STD_FOCUSED_ENTITY = &entity->entity;
	STDVS_CAMERA.view.super = NULL;
	if (sub != 3) {
		p = &STD_CAMERA_PRESETS[mode];
	} else {
		p = &STD_CAMERA_PRESETS[randomLimit(3) + 6];
	}
	STD_setCameraParams(p->unk0, p->unk2, p->unk4, p->unk6, p->unk8, p->unkA);
	if (mode < 5) {
		STD_CAMERA_STATE = 3;
		return;
	}
	if ((Entity *)entity != ENTITY_TABLE[1]) {
		STD_addWinScene(1);
	} else {
		STD_addLoseScene();
	}
	STD_FOCUSED_ENTITY = ENTITY_TABLE[1];
	STD_startCameraIntro(ENTITY_TABLE[1], STD_OPPONENT_ENTITY);
}

void STD_setRandomViewpoint(Entity *entity, int32_t idx)
{
	VECTOR v;
	VECTOR out;
	MATRIX m;

	if (randomLimit(3) != 0) {
		return;
	}
	if (STD_CAMERA_STATE == 7) {
		return;
	}

	VIEWPORT_DISTANCE = 0x1f4;
	GS_VIEWPOINT.super = NULL;

	if (idx < 4) {
		STD_FOCUSED_ENTITY = entity;
		STD_CAMERA_STATE = 4;
		RotMatrix(&STD_FOCUSED_ENTITY->posData->rotation, &m);
		v.vx = STD_RANDOM_VIEWPOINTS[idx][0];
		v.vy = STD_RANDOM_VIEWPOINTS[idx][1];
		v.vz = STD_RANDOM_VIEWPOINTS[idx][2];
		ApplyMatrixLV(&m, &v, &out);
		out.vx += STD_FOCUSED_ENTITY->posData->location.vx;
		out.vz += STD_FOCUSED_ENTITY->posData->location.vz;
		GS_VIEWPOINT.vpx = out.vx;
		GS_VIEWPOINT.vpy = out.vy;
		GS_VIEWPOINT.vpz = out.vz;
	} else {
		STD_CAMERA_STATE = 6;
		GS_VIEWPOINT.vpx = STD_RANDOM_VIEWPOINTS[idx][0];
		GS_VIEWPOINT.vpy = STD_RANDOM_VIEWPOINTS[idx][1];
		GS_VIEWPOINT.vpz = STD_RANDOM_VIEWPOINTS[idx][2];
	}

	GS_VIEWPOINT.rz = 0;
}

void STD_setChampionCamera(void)
{
	STD_CAMERA_STATE = 0xb;
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

	off = STD_INTRO_CAMERA_STAGE2_POS;
	rot = STD_INTRO_CAMERA_STAGE2_ROT;
	base = ((((DIGIMON_DATA[STD_FOCUSED_ENTITY->type].height +
	           DIGIMON_DATA[STD_FOCUSED_ENTITY->type].radius) /
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

	rot.vy += STD_FOCUSED_ENTITY->posData->rotation.vy;
	off.vy = (-DIGIMON_DATA[STD_FOCUSED_ENTITY->type].height * 2) / 3;
	STD_setViewpointFromBone(STD_FOCUSED_ENTITY, &off, &rot, dist);
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

int32_t STD_interpolateClamped2(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end)
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

	cc = &STD_INTRO_CAMERA_CHASE;
	if (cc->timer < 0x14) {
		return;
	}
	if (cc->timer < 0x14) {
		goto inc;
	}
	if (cc->timer == 0x14) {
		startAnimation(STD_FOCUSED_ENTITY, 0x23);
	}
	if (cc->phase == 0) {
		dist = STD_getFighterDistance(&STD_CAMERA_CHASE_LAST_POS, &STD_INTRO_TARGET_POS, &STD_FOCUSED_ENTITY->posData->location);
		if (dist >= 0x23) {
			STD_CAMERA_CHASE_LAST_POS = STD_FOCUSED_ENTITY->posData->location;
			cc->phase = 1;
			STD_CAMERA_STATE = 8;
		} else {
			off = STD_INTRO_CAMERA_STAGE1_POS;
			rot = STD_INTRO_CAMERA_STAGE1_ROT;
			if (cc->side == 0) {
				rot.vy = lerp(-0x638, -0x293, 0, 0x23, dist);
			} else {
				rot.vy = lerp(0x638, 0x293, 0, 0x23, dist);
			}
			rot.vy += STD_FOCUSED_ENTITY->posData->rotation.vy;
			off.vy = (-DIGIMON_DATA[STD_FOCUSED_ENTITY->type].height * 7) / 10;
			d2 = (((DIGIMON_DATA[STD_FOCUSED_ENTITY->type].height + DIGIMON_DATA[STD_FOCUSED_ENTITY->type].radius) / 2) * 0x5aa) / 450;
			STD_setViewpointFromBone(STD_FOCUSED_ENTITY, &off, &rot, d2);
			GS_VIEWPOINT.vpy = (off.vy * 7) / 10;
			if (GS_VIEWPOINT.vpy > -0xb4) {
				GS_VIEWPOINT.vpy = -0xb4;
			}
			goto inc;
		}
	}
	t = STD_getFighterDistance(&STD_CAMERA_CHASE_LAST_POS, &STD_INTRO_TARGET_POS, &STD_FOCUSED_ENTITY->posData->location);
	STD_updateCameraLerp(t, cc->side);
	if (STD_isPositionNearEntity(STD_FOCUSED_ENTITY, &STD_INTRO_TARGET_POS) == 1) {
		for (i = 0; i < 3; i++) {
			if (((DigimonEntity *)STD_FOCUSED_ENTITY)->stats.base.moves[i] != 0xff) {
				startAnimation(STD_FOCUSED_ENTITY, ((DigimonEntity *)STD_FOCUSED_ENTITY)->stats.base.moves[i]);
				break;
			}
		}
		STD_FOCUSED_ENTITY->anim.animFlag |= 2;
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

	STD_FOCUSED_ENTITY = entity;
	copyVector(&STD_CAMERA_CHASE_LAST_POS, &STD_FOCUSED_ENTITY->posData->location);
	STD_INTRO_TARGET_POS.vx = STD_CAMERA_CHASE_LAST_POS.vx - dx;
	STD_INTRO_TARGET_POS.vy = STD_CAMERA_CHASE_LAST_POS.vy;
	STD_INTRO_TARGET_POS.vz = STD_CAMERA_CHASE_LAST_POS.vz;
	startAnimation(STD_FOCUSED_ENTITY, 0x21);
	STD_CAMERA_STATE = 9;
	STD_INTRO_CAMERA_CHASE.timer = 0;
	STD_INTRO_CAMERA_CHASE.phase = 0;
	STD_INTRO_CAMERA_CHASE.side = side;
	addObject(0x1aa, 0, (TickFunction)STD_tickCameraChase, NULL);
	off = STD_CAMERA_CHASE_OFFSET;
	rot = STD_CAMERA_CHASE_ROTATION;
	if (STD_INTRO_CAMERA_CHASE.side == 0) {
		rot.vy = -0x638;
	} else {
		rot.vy = 0x638;
	}
	rot.vy += STD_FOCUSED_ENTITY->posData->rotation.vy;
	off.vy = (-DIGIMON_DATA[STD_FOCUSED_ENTITY->type].height * 2) / 3;
	dist = (((DIGIMON_DATA[STD_FOCUSED_ENTITY->type].height + DIGIMON_DATA[STD_FOCUSED_ENTITY->type].radius) / 2) * 0x5aa) / 450;
	STD_setViewpointFromBone(STD_FOCUSED_ENTITY, &off, &rot, dist);
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
#if VERSION_REGION_IS(NTSCJ)
	if ((p[0] >= 0x1e) && (p[0] < 0x3c)) {
#else
	if ((STD_D_8007B9BC[6] >= 0x1e) && (p[0] < 0x3c)) {
#endif
		STD_D_8007B9BC[8] = lerp(STD_D_8007B9BC[8], STD_D_8007B9BC[9], p[0], 0x3c, p[0] + 1);
	}

	if (p[0] >= 0x1e) {
		p[1] += (int16_t)STD_interpolateClamped2(0x1e, 0x3c, p[0], 0, 0x5b);
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

void STD_startCameraIntro(Entity *target, Entity *entity)
{
	SVECTOR off;
	SVECTOR rot;
	SVECTOR delta;
	int32_t d;
	int32_t i;

	STD_D_8007B9BC[6] = 0;
	STD_D_8007B9D0.entity = entity;
	if (target == NULL) {
		if (STD_FOCUSED_ENTITY == ENTITY_TABLE[1]) {
			target = STD_OPPONENT_ENTITY;
		} else {
			target = ENTITY_TABLE[1];
		}
	}
	STD_D_8007B9D0.target = target;
	STD_CAMERA_STATE = 8;
	addObject(0x1ad, 0, (TickFunction)STD_tickCameraIntro, NULL);

	off = STD_CAMERA_INTRO_OFFSET;
	rot = STD_CAMERA_INTRO_ROTATION;
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
	MATRIX *m;

	VIEWPORT_DISTANCE = 0x15e;
	GsSetProjection(VIEWPORT_DISTANCE);
	m = &STD_FOCUSED_ENTITY->posData->posMatrix.workm;
	GS_VIEWPOINT.vrx = m->t[0];
	GS_VIEWPOINT.vry = -DIGIMON_DATA[STD_FOCUSED_ENTITY->type].height * 2 / 3;
	GS_VIEWPOINT.vrz = m->t[2];
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
	if ((STD_IS_TIMER_INITIALIZED == 0) && (arg != 0)) {
		STD_TIMER = arg;
		addObject(0x1ac, 0, (TickFunction)STD_tickFighterCounter, (RenderFunction)STD_renderFighterCounter);
		STD_IS_TIMER_INITIALIZED = 1;
	}
}

void STD_tickFighterCounter(void)
{
	if (STD_TIMER_ACTIVE == 1) {
		BATTLE_FRAME_COUNT++;
		if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]]->anim.animId != 0x2b) {
			if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->anim.animId != 0x2b) {
				if (BATTLE_FRAME_COUNT % 0x14 == 0) {
					if (STD_TIMER != 0) {
						STD_TIMER--;
					}
				}
			}
		}
	}
}

void STD_renderFighterCounter(void)
{
	STD_renderCounterDigits(-0xd, -0x61, 2, STD_TIMER, 3);
}

void STD_removeFighterCounter(void)
{
	if (STD_IS_TIMER_INITIALIZED != 0) {
		removeObject(0x1ac, 0);
		STD_IS_TIMER_INITIALIZED = 0;
	}
}

void STD_updateBracket(void)
{
	int32_t i;
	StdUnkBAF4 *s;
	int32_t match;
	int16_t ab[2];
	int16_t arg;
	uint8_t flag;

	if (STD_BRACKET_MATCH < 5) {
		ab[0] = (STD_BRACKET_MATCH - 1) * 2;
		ab[1] = (STD_BRACKET_MATCH - 1) * 2 + 1;
	} else if (STD_BRACKET_MATCH < 7) {
		ab[0] = STD_ROUND1_WINNERS[(STD_BRACKET_MATCH - 5) * 2];
		ab[1] = STD_ROUND1_WINNERS[(STD_BRACKET_MATCH - 5) * 2 + 1];
	} else {
		ab[0] = STD_ROUND2_WINNERS[0];
		ab[1] = STD_ROUND2_WINNERS[1];
	}
	if (ab[0] == STD_PLAYER_SLOT || ab[1] == STD_PLAYER_SLOT) {
		match = 1;
	} else {
		match = 0;
	}

	for (i = 0; i < 8; i++) {
		if (i != ab[0] && i != ab[1]) {
			continue;
		}
		s = &STD_BRACKET_SLOTS[i];
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
			if (s->unk2 != STD_BRACKET_ROW_Y[s->unk4]) {
				s->unk2--;
			}
			if (s->unk8 % 10 == 0) {
				STD_BRACKET_SLOTS[i].unk5 = (s->unk5 + 1) & 1;
			}
			if (s->unk2 == STD_BRACKET_ROW_Y[s->unk4] && s->unk0 == STD_BRACKET_PATHS[i].unk0[s->unk4]) {
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
					if (STD_BRACKET_MATCH < 5) {
						if (i == STD_ROUND1_WINNERS[STD_BRACKET_MATCH - 1]) {
							flag = 1;
						}
					}
					if (STD_BRACKET_MATCH >= 5 && STD_BRACKET_MATCH < 7) {
						if (i == STD_ROUND2_WINNERS[STD_BRACKET_MATCH - 5]) {
							flag = 1;
						}
					}
					if (i == ab[0]) {
						arg = ab[1];
					} else {
						arg = ab[0];
					}
					STD_addBracketProjectile((int16_t)i, arg, flag);
					s->unk7++;
				}
			} else {
				STD_BRACKET_FADE += 3;
				if (STD_BRACKET_FADE >= 0x81) {
					STD_BRACKET_FADE = 0x80;
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
					STD_addBracketHitFlash((int16_t)i);
				} else if (s->unkC == 1) {
					STD_addBracketHitFlash((int16_t)i);
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
				s->unk2 = STD_BRACKET_ROW_Y[s->unk4] + STD_BRACKET_HOP_Y[s->unk8++];
				if (s->unk8 == 0x14) {
					playSound(8, 4);
				}
			} else {
				s->unk7++;
			}
			break;
		case 6:
			if (match != 0) {
				if (STD_MATCH_RESULT == 1) {
					if (i == STD_PLAYER_SLOT) {
						s->unk0 = STD_BRACKET_PATHS[i].unk0[s->unk4 + 1];
						s->unk5 = 0;
						s->unk6 = STD_BRACKET_PATHS[i].unk10[s->unk4 + 1];
						s->unk4 += 2;
						s->unk9 = 0;
						s->unk8 = 0;
						s->unk5 = 0;
						s->unk7 = 0;
						if (STD_BRACKET_MATCH < 5) {
							STD_ROUND1_WINNERS[STD_PLAYER_SLOT / 2] = STD_PLAYER_SLOT;
						}
						if (STD_BRACKET_MATCH < 7) {
							STD_ROUND2_WINNERS[STD_PLAYER_SLOT / 4] = STD_PLAYER_SLOT;
						}
						STD_BRACKET_MATCH++;
					} else {
						s->unk2 = STD_BRACKET_ROW_Y[s->unk4 - 1];
						s->unk5 = 3;
					}
				} else if (i == STD_PLAYER_SLOT) {
					s->unk2 = STD_BRACKET_ROW_Y[s->unk4 - 1];
					s->unk5 = 3;
				} else {
					s->unk0 = STD_BRACKET_PATHS[i].unk0[s->unk4 - 1];
					s->unk5 = 0;
				}
			} else if (s->unkC == 0) {
				s->unk8++;
				if (s->unk8 % 10 == 0) {
					s->unk5 = (s->unk5 + 1) & 1;
				}
				if (STD_BRACKET_SLOTS[STD_BRACKET_PROJECTILES[i].owner].unk7 == 6) {
					if (s->unk0 > STD_BRACKET_PATHS[i].unk0[s->unk4]) {
						s->unk0--;
					}
					if (s->unk0 < STD_BRACKET_PATHS[i].unk0[s->unk4]) {
						s->unk0++;
					}
					if (s->unk2 == STD_BRACKET_ROW_Y[s->unk4] && s->unk0 == STD_BRACKET_PATHS[i].unk0[s->unk4]) {
						s->unk6 = STD_BRACKET_PATHS[i].unk10[(int32_t)s->unk4];
						s->unk4++;
						s->unk9 = 0;
						s->unk8 = 0;
						s->unk5 = 0;
						s->unk7 = 0;
						STD_BRACKET_MATCH++;
					}
				}
			}
			break;
		case 7:
			s->unk8++;
			if (i == STD_PLAYER_SLOT) {
				s->unk0 = STD_BRACKET_PATHS[i].unk0[7];
				s->unk2 = STD_BRACKET_ROW_Y[7];
				s->unk5 = 0;
				s->unk6 = STD_BRACKET_PATHS[i].unk10[7];
			} else {
				s->unk0 = STD_BRACKET_PATHS[i].unk0[4];
				s->unk2 = STD_BRACKET_ROW_Y[4];
				s->unk6 = STD_BRACKET_PATHS[i].unk10[4];
				s->unk5 = 3;
			}
			if (s->unk8 >= 0x3d) {
				STD_BRACKET_FINISHED = 1;
			}
			break;
		}
	}

	for (i = 0; i < 8; i++) {
		STD_renderBracketDigimon((int16_t)i);
	}

	if (match != 0 && STD_BRACKET_FADE == 0x80 && STD_BRACKET_SLOTS[STD_PLAYER_SLOT].unk7 == 2) {
		STD_BRACKET_SLOTS[STD_PLAYER_SLOT].unk7 = (STD_BRACKET_MATCH != 7) ? 6 : 7;
		if (STD_BRACKET_MATCH == 7) {
			STD_BRACKET_SLOTS[STD_PLAYER_SLOT].unk8 = 0;
		}
		if (STD_PLAYER_SLOT == ab[0]) {
			STD_BRACKET_SLOTS[ab[1]].unk7 = (STD_BRACKET_MATCH != 7) ? 6 : 7;
			if (STD_BRACKET_MATCH == 7) {
				STD_BRACKET_SLOTS[ab[1]].unk8 = 0;
			}
		} else {
			STD_BRACKET_SLOTS[ab[0]].unk7 = (STD_BRACKET_MATCH != 7) ? 6 : 7;
			if (STD_BRACKET_MATCH == 7) {
				STD_BRACKET_SLOTS[ab[0]].unk8 = 0;
			}
		}
		STD_BRACKET_FINISHED = 1;
	}
}

void STD_drawBracket(void)
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
				c = STD_INTRO_DIGIMON_NAMES[PARTICIPANT_TYPES[i]][p++];
				STD_renderBracketGlyph((int16_t)(STD_BRACKET_PATHS[i].unk0[0] + 4),
				                       (int16_t)(STD_BRACKET_ROW_Y[0] + 0x12 + j * 8), c, 5);
				if (c == 0x1f || c == 0x25) {
					c = STD_INTRO_DIGIMON_NAMES[PARTICIPANT_TYPES[i]][p++];
					STD_renderBracketGlyph((int16_t)(STD_BRACKET_PATHS[i].unk0[0] + 4),
					                       (int16_t)(STD_BRACKET_ROW_Y[0] + 0x12 + j * 8), c, 5);
				}
			} else {
				c = STD_INTRO_DIGIMON_NAMES[PARTICIPANT_TYPES[i]][p++];
				if (c == 0x3d) {
					small = 1;
					c = STD_INTRO_DIGIMON_NAMES[PARTICIPANT_TYPES[i]][p++];
					shift = -((j - 1) * 8);
				}
				STD_renderBracketGlyph((int16_t)((small == 0) ? STD_BRACKET_PATHS[i].unk0[0] + 8 : STD_BRACKET_PATHS[i].unk0[0]),
				                       (int16_t)(shift + (STD_BRACKET_ROW_Y[0] + 0x12 + j * 8)), c, 5);
				if (c == 0x1f || c == 0x25) {
					c = STD_INTRO_DIGIMON_NAMES[PARTICIPANT_TYPES[i]][p++];
					STD_renderBracketGlyph((int16_t)((small == 0) ? STD_BRACKET_PATHS[i].unk0[0] + 8 : STD_BRACKET_PATHS[i].unk0[0]),
					                       (int16_t)(shift + (STD_BRACKET_ROW_Y[0] + 0x12 + j * 8)), c, 5);
				}
			}
		}
	}

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	if (STD_BRACKET_FADE != 0) {
		setSemiTrans(prim, 1);
	}
	setRGB0(prim, 0x80 - STD_BRACKET_FADE, 0x80 - STD_BRACKET_FADE, 0x80 - STD_BRACKET_FADE);
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

void STD_addBracketProjectile(int16_t i, int32_t owner, uint8_t flag)
{
	if (STD_BRACKET_SLOTS[i].unk6 == 1) {
		STD_BRACKET_PROJECTILES[i].x = STD_BRACKET_SLOTS[i].unk0 + 0x10;
		STD_BRACKET_PROJECTILES[i].y = STD_BRACKET_SLOTS[i].unk2;
		if (flag == 1) {
			STD_BRACKET_SLOTS[owner].unkC = 1;
		}
	} else {
		STD_BRACKET_PROJECTILES[i].x = STD_BRACKET_SLOTS[i].unk0 - 8;
		if (flag == 0) {
			STD_BRACKET_PROJECTILES[i].y = STD_BRACKET_SLOTS[i].unk2 + 8;
		} else {
			STD_BRACKET_PROJECTILES[i].y = STD_BRACKET_SLOTS[i].unk2;
			STD_BRACKET_SLOTS[owner].unkC = 1;
		}
	}
	STD_BRACKET_PROJECTILES[i].flag = flag;
	STD_BRACKET_PROJECTILES[i].owner = owner;
	addObject(0x1af, i, (TickFunction)STD_tickBracketProjectile, NULL);
}

void STD_addBracketHitFlash(int16_t i)
{
	addObject(0x1b0, i, STD_tickBracketHitFlash, 0);
}

void STD_renderBracketDigimon(int16_t id)
{
	POLY_FT4 *prim;
	uint8_t tile;
	uint8_t u;
	uint8_t v;
#if VERSION_REGION_IS(NTSCJ)
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
	if (STD_BRACKET_FADE != 0) {
		setSemiTrans(prim, 1);
	}
	prim->tpage = GetTPage(0, 1, (tile / 64) * 64 + 0x380, 0);
	prim->clut = GetClut((tile / 64) * 16 + 0x120, STD_DIGIMON_SPRITE_CLUT[tile] + 0x1e0);
	setRGB0(prim, 0x80 - STD_BRACKET_FADE, 0x80 - STD_BRACKET_FADE, 0x80 - STD_BRACKET_FADE);
	u = ((tile - 1) % 32) * 32 + (STD_BRACKET_SLOTS[id].unk5 & 1) * 16;
	v = ((tile - 1) / 8) * 32 + (STD_BRACKET_SLOTS[id].unk5 / 2) * 16;
	w = h = 0x10;
	if (u == 0xf0) {
		w = 0xf;
	}
	if (v == 0xf0) {
		h = 0xf;
	}
#if VERSION_REGION_IS(NTSCJ)
	setUVDataPolyFT4(prim, u, v, w, h);
	setPosDataPolyFT4(prim, STD_BRACKET_SLOTS[id].unk0, STD_BRACKET_SLOTS[id].unk2, w, h);
#else
	h2 = h2 = h;
	setUVDataPolyFT4(prim, u, v, w2 = w2 = w, h);
	setPosDataPolyFT4(prim, STD_BRACKET_SLOTS[id].unk0, STD_BRACKET_SLOTS[id].unk2, w2, h2);
#endif
	if (STD_BRACKET_SLOTS[id].unk6 == 1) {
		swapShort(&prim->x0, &prim->x1);
		swapShort(&prim->x2, &prim->x3);
		STD_setFlippedUV(prim, prim->u0, w, h);
	}
	AddPrim(ACTIVE_ORDERING_TABLE->org + 5, prim++);
	GsSetWorkBase((PACKET *)prim);
}

// clang-format off
void STD_removeBracketHitFlash(i)
	int16_t i;
// clang-format on
{
	removeObject(0x1b0, i);
}

void STD_removeBracketProjectile(int16_t i)
{
	removeObject(0x1af, i);
}

// clang-format off
void STD_setFlippedUV(prim, a, b, h)
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

void STD_renderBracketGlyph(int16_t x, int16_t y, uint8_t n, int32_t layer)
{
	POLY_FT4 *prim;
	uint8_t u;
	uint8_t v;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->tpage = GetTPage(0, 1, 0x340, 0x100);
	prim->clut = GetClut(0x30, 0x1e0);
	if (STD_BRACKET_FADE != 0) {
		setSemiTrans(prim, 1);
	}
	setRGB0(prim, 0x80 - STD_BRACKET_FADE, 0x80 - STD_BRACKET_FADE, 0x80 - STD_BRACKET_FADE);
	u = (n % 8) * 8;
	v = (n / 8) * 8;
	setUVDataPolyFT4(prim, u, v, (u != 0xf8) ? 8 : 7, (v != 0xf8) ? 8 : 7);
	setPosDataPolyFT4(prim, x, y, 8, 8);
#if VERSION_REGION_IS(NTSCJ)
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
#endif
	GsSetWorkBase((PACKET *)prim);
}

void STD_initializeBracket(void)
{
	StdUnkBAF4 *p;
	int32_t i;

	STD_BRACKET_MATCH = 1;
	for (i = 0; i < 8; i++) {
		p = &STD_BRACKET_SLOTS[i];
		p->unk0 = STD_BRACKET_PATHS[i].unk0[0];
		p->unk2 = STD_BRACKET_ROW_Y[0];
		p->unk5 = 0;
		p->unk4 = 1;
		p->unk6 = STD_BRACKET_PATHS[i].unk10[0];
		p->unk8 = 0;
		p->unk9 = 0;
		p->unkC = 0;
		p->unk7 = 0;
	}
}

void STD_setupParticipants(uint8_t *out, uint8_t *list)
{
	int32_t i;
	int32_t r;
	long j;
	uint8_t *p;

	STD_PLAYER_SLOT = randomLimit(8);
	PARTICIPANT_TYPES[STD_PLAYER_SLOT] = ENTITY_TABLE[1]->type;

	for (i = 1; i < 8; i++) {
		j = randomLimit(7) + 1;
		swapByte(&list[i], &list[j]);
	}

	p = list + 1;
	for (i = 0; i < 8; i++) {
		if (i != STD_PLAYER_SLOT) {
			PARTICIPANT_TYPES[i] = *p++;
		}
	}

	for (i = 0; i < 4; i++) {
		if (randomLimit(100) < STD_WIN_CHANCES[DIGIMON_DATA[PARTICIPANT_TYPES[i * 2]].level - 3][DIGIMON_DATA[PARTICIPANT_TYPES[i * 2 + 1L]].level - 3]) {
			STD_ROUND1_WINNERS[i] = i * 2;
		} else {
			STD_ROUND1_WINNERS[i] = i * 2 + 1;
		}
	}

	if (STD_PLAYER_SLOT < 4) {
		if (STD_PLAYER_SLOT < 2) {
			STD_ROUND2_WINNERS[0] = STD_ROUND1_WINNERS[1];
		} else {
			STD_ROUND2_WINNERS[0] = STD_ROUND1_WINNERS[0];
		}
		if (randomLimit(100) < STD_WIN_CHANCES[DIGIMON_DATA[PARTICIPANT_TYPES[STD_ROUND1_WINNERS[2]]].level - 3][DIGIMON_DATA[PARTICIPANT_TYPES[STD_ROUND1_WINNERS[3]]].level - 3]) {
			STD_ROUND2_WINNERS[1] = STD_ROUND1_WINNERS[2];
		} else {
			STD_ROUND2_WINNERS[1] = STD_ROUND1_WINNERS[3];
		}
	} else {
		if (randomLimit(100) < STD_WIN_CHANCES[DIGIMON_DATA[PARTICIPANT_TYPES[STD_ROUND1_WINNERS[0]]].level - 3][DIGIMON_DATA[PARTICIPANT_TYPES[STD_ROUND1_WINNERS[1]]].level - 3]) {
			STD_ROUND2_WINNERS[0] = STD_ROUND1_WINNERS[0];
		} else {
			STD_ROUND2_WINNERS[0] = STD_ROUND1_WINNERS[1];
		}
		if (STD_PLAYER_SLOT < 6) {
			STD_ROUND2_WINNERS[1] = STD_ROUND1_WINNERS[3];
		} else {
			STD_ROUND2_WINNERS[1] = STD_ROUND1_WINNERS[2];
		}
	}

	if (STD_PLAYER_SLOT % 2 == 0) {
		out[0] = PARTICIPANT_TYPES[STD_PLAYER_SLOT + 1];
	} else {
		out[0] = PARTICIPANT_TYPES[STD_PLAYER_SLOT - 1];
	}

	switch (STD_PLAYER_SLOT / 2) {
	case 0:
		out[1] = PARTICIPANT_TYPES[STD_ROUND1_WINNERS[1]];
		out[2] = PARTICIPANT_TYPES[STD_ROUND2_WINNERS[1]];
		break;
	case 1:
		out[1] = PARTICIPANT_TYPES[STD_ROUND1_WINNERS[0]];
		out[2] = PARTICIPANT_TYPES[STD_ROUND2_WINNERS[1]];
		break;
	case 2:
		out[1] = PARTICIPANT_TYPES[STD_ROUND1_WINNERS[3]];
		out[2] = PARTICIPANT_TYPES[STD_ROUND2_WINNERS[0]];
		break;
	case 3:
		out[1] = PARTICIPANT_TYPES[STD_ROUND1_WINNERS[2]];
		out[2] = PARTICIPANT_TYPES[STD_ROUND2_WINNERS[0]];
		break;
	}
}

void STD_addBracket(int16_t track)
{
	ENTITY_TABLE[1]->isOnScreen = 0;
	ENTITY_TABLE[1]->isOnMap = 0;
	STD_BRACKET_FINISHED = 0;
	STD_BRACKET_FADE = 0;
	addObject(0x1ae, 0, (TickFunction)STD_tickBracket, (RenderFunction)STD_renderBracket);
	stopBGM();
	stopSound();
	playMusic(0x1d, track);
}

void STD_tickBracket(void)
{
}

void STD_renderBracket(void)
{
	STD_updateBracket();
	STD_drawBracket();
}

void STD_removeBracket(int16_t mode)
{
	ENTITY_TABLE[1]->isOnScreen = 1;
	ENTITY_TABLE[1]->isOnMap = 1;
	removeObject(0x1ae, 0);
	if (mode != 2) {
		stopBGM();
		stopSound();
	}
}

int32_t STD_isBracketFinished(void)
{
	return STD_BRACKET_FINISHED;
}

void STD_tickBracketHitFlash(int32_t idx)
{
	POLY_FT4 *prim;

	if (STD_BRACKET_SLOTS[idx].unk8 >= 0x10) {
		STD_removeBracketHitFlash(idx);
	}
	if ((STD_BRACKET_SLOTS[idx].unk8 % 5) == 4) {
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 960, 0);
		prim->clut = GetClut(0x130, 0x1e0);
		setUVDataPolyFT4(prim, 0, 0xc8, 0x10, 0x10);
		setPosDataPolyFT4(prim, STD_BRACKET_SLOTS[idx].unk0, STD_BRACKET_SLOTS[idx].unk2, 0x10, 0x10);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 3, prim++);
		GsSetWorkBase((PACKET *)prim);
	}
}

// clang-format off
void STD_tickBracketProjectile(id)
	int16_t id;
// clang-format on
{
	int16_t d;

	if (STD_BRACKET_MATCH < 5) {
		d = 1;
	} else if (STD_BRACKET_MATCH < 7) {
		d = 3;
	}

	if (STD_BRACKET_SLOTS[id].unk6 == 1) {
		STD_BRACKET_PROJECTILES[id].x += d;
	} else {
		STD_BRACKET_PROJECTILES[id].x -= d;
	}
	STD_renderBracketProjectile(id);
}

void STD_renderBracketProjectile(int16_t id)
{
	POLY_FT4 *prim;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = GetTPage(0, 2, 0x3c0, 0);
	prim->clut = GetClut(0x130,
	                     STD_BRACKET_PROJECTILE_CLUTS[STD_BRACKET_PROJECTILE_ICONS[PARTICIPANT_TYPES[id]] - 1] + 0x1e0);
	setUVDataPolyFT4(prim, STD_BRACKET_PROJECTILE_ICONS[PARTICIPANT_TYPES[id]] * 8 - 8, 0xc0, 8, 8);
	setPosDataPolyFT4(prim, STD_BRACKET_PROJECTILES[id].x, STD_BRACKET_PROJECTILES[id].y, 8, 8);
	if (STD_BRACKET_SLOTS[id].unk6 == 1) {
		swapShort(&prim->x0, &prim->x1);
		swapShort(&prim->x2, &prim->x3);
		STD_setFlippedUV(prim, prim->u0, 8, 8);
	}
	AddPrim(ACTIVE_ORDERING_TABLE->org + 4, prim++);

	if (STD_BRACKET_PROJECTILES[id].flag == 1) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = GetTPage(0, 2, 0x3c0, 0);
		prim->clut = GetClut(0x130,
		                     STD_BRACKET_PROJECTILE_CLUTS[STD_BRACKET_PROJECTILE_ICONS[PARTICIPANT_TYPES[id]] - 1] + 0x1e0);
		setUVDataPolyFT4(prim, STD_BRACKET_PROJECTILE_ICONS[PARTICIPANT_TYPES[id]] * 8 - 8, 0xc0, 8, 8);
		setPosDataPolyFT4(prim, STD_BRACKET_PROJECTILES[id].x, STD_BRACKET_PROJECTILES[id].y + 8, 8, 8);
		if (STD_BRACKET_SLOTS[id].unk6 == 1) {
			swapShort(&prim->x0, &prim->x1);
			swapShort(&prim->x2, &prim->x3);
			STD_setFlippedUV(prim, prim->u0, 8, 8);
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 4, prim++);
	}
	GsSetWorkBase((PACKET *)prim);

	if (STD_BRACKET_SLOTS[id].unk6 == 1) {
		if (STD_BRACKET_SLOTS[STD_BRACKET_PROJECTILES[id].owner].unk0 - STD_BRACKET_PROJECTILES[id].x < 9) {
			STD_BRACKET_SLOTS[STD_BRACKET_PROJECTILES[id].owner].unkA[0] = 1;
			STD_removeBracketProjectile(id);
		}
	} else {
		if (STD_BRACKET_PROJECTILES[id].x - STD_BRACKET_SLOTS[STD_BRACKET_PROJECTILES[id].owner].unk0 < 0x11) {
			STD_BRACKET_SLOTS[STD_BRACKET_PROJECTILES[id].owner].unkA[0] = 1;
			STD_removeBracketProjectile(id);
		}
	}
}

void STD_addBracketIntro(void)
{
	ENTITY_TABLE[1]->isOnScreen = 0;
	ENTITY_TABLE[1]->isOnMap = 0;
	STD_BRACKET_FADE = 0x80;
	MAIN_D_80135165 = 0x80;
	MAIN_D_80135166 = 0x80;
	STD_BRACKET_INTRO_TIMER = 0;
	stopBGM();
	stopSound();
	playMusic(0x1d, 0);
	addObject(0x1a1, 0, (TickFunction)STD_tickBracketIntro, (RenderFunction)STD_renderBracketIntro);
}

void STD_tickBracketIntro(void)
{
	if (STD_BRACKET_INTRO_TIMER < 0x78) {
		STD_BRACKET_INTRO_TIMER++;
	}
	if (STD_BRACKET_INTRO_TIMER >= 0x65) {
		if (STD_BRACKET_FADE != 0) {
			STD_BRACKET_FADE -= 8;
		}
		if (MAIN_D_80135165 >= 9) {
			MAIN_D_80135165 -= 8;
		}
		if (MAIN_D_80135166 != 0) {
			MAIN_D_80135166 -= 8;
		}
	}
}

void STD_renderBracketIntro(void)
{
	POLY_FT4 *prim;

	STD_drawBracket();
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
	if (STD_BRACKET_FADE != 0x80) {
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

void STD_removeBracketIntro(void)
{
	removeObject(0x1a1, 0);
}

void STD_tickPartnerAI(void)
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
	} else if (!(*flagsPtr & (FIGHTER_FLAG_CONFUSED | FIGHTER_FLAG_STUNNED | FIGHTER_FLAG_FLATTENED | FIGHTER_FLAG_DEAD)) && (fighter->flatTimer == 0)) {
		COMBAT_DATA_PTR->player.commandDelay[0]--;
	}

	if (NO_AI_FLAG == 0) {
		if (stats->current.currentHP > fighter->hpDamageBuffer) {
			if (stats->base.brain < 0x12d) {
				if ((BATTLE_FRAME_COUNT % (((stats->base.brain / 2) + 1) * 20)) == 0) {
					if ((0x46 - PARTNER_PARA.discipline) > randomLimit(100)) {
						*flagsPtr |= FIGHTER_FLAG_SENILE;
						fighter->senileTimer = 100;
					}
				}
			}
		}
		if (fighter->cooldown >= 2) {
			fighter->cooldown--;
		}
		if (!(*flagsPtr & FIGHTER_FLAG_SENILE)) {
			STD_increaseSpeedBuffer(fighter, stats);
		}
	}

	if (*flagsPtr & (FIGHTER_FLAG_KNOCKED_BACK | FIGHTER_FLAG_ATTACKING | FIGHTER_FLAG_BLOCKING | FIGHTER_FLAG_DEAD)) {
		return;
	}

	if (stats->current.currentHP == 0) {
		STD_faintDigimon((DigimonEntity *)partner, fighter, 0);
		if (STD_areAllEnemyDigimonDead() == 0) {
			STD_selectRandomCamera((DigimonEntity *)partner, 5, 0);
		}
		STD_DISABLE_HITTING = 1;
		return;
	}

	STD_getRemainingEnemies(partner, enemies, &count);
	if ((count == 0) || (((DigimonEntity *)partner)->stats.current.currentHP <= fighter->hpDamageBuffer)) {
		handleBattleIdle((DigimonEntity *)partner, stats, *flagsPtr);
		fighter->moveRange = -1;
		STD_resetFlatten(0);
		STD_removeStatusEffects((DigimonEntity *)partner, fighter);
		*flagsPtr = 0;
		*flagsPtr |= FIGHTER_FLAG_TRANSFORMING;
		return;
	}

	if (NO_AI_FLAG != 0) {
		return;
	}

	if (!(*flagsPtr & (FIGHTER_FLAG_CONFUSED | FIGHTER_FLAG_STUNNED | FIGHTER_FLAG_FLATTENED | FIGHTER_FLAG_DEAD)) && (fighter->flatTimer == 0)) {
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
			((DigimonEntity *)partner)->stats.current.chargeMode = STD_SAVED_CHARGE_MODE;
		}
	}

	if ((*flagsPtr & FIGHTER_FLAG_FLATTENED) && ((BATTLE_FRAME_COUNT % 100) == 0)) {
		fighter->targetId = enemies[randomLimit(count)];
	}

	if (*flagsPtr & FIGHTER_FLAG_TRANSFORMING) {
		return;
	}

	if (*flagsPtr & FIGHTER_FLAG_FLATTENED) {
		fighter->queuedAnim = 0;
		fighter->targetId = 1;
		fighter->moveRange = 2;
		partner->flatSprite = 0;
		fighter->flags |= FIGHTER_FLAG_TRANSFORMING;
		return;
	}

	if (*flagsPtr & FIGHTER_FLAG_STUNNED) {
		return;
	}

	if (*flagsPtr & FIGHTER_FLAG_CONFUSED) {
		STD_selectConfusedMove((DigimonEntity *)partner, fighter, 0);
		return;
	}

	if (*flagsPtr & FIGHTER_FLAG_ON_CHARGEUP) {
		return;
	}

	if (*flagsPtr & FIGHTER_FLAG_ON_COOLDOWN) {
		return;
	}

	if (*flagsPtr & FIGHTER_FLAG_SENILE) {
		return;
	}

	fighter->targetId = 1;

	result = -1;
	switch (COMBAT_DATA_PTR->player.currentCommand[0]) {
	case 2:
		if (STD_hasAffordableMoves(moveFlags, 0) == 0) {
			fighter->cooldown = 0x50;
			fighter->flags |= FIGHTER_FLAG_ON_CHARGEUP;
			return;
		}
		result = STD_selectMoveByPower(0, moveFlags);
		((DigimonEntity *)partner)->stats.current.chargeMode = 0;
		break;
	case 4:
		if (STD_hasAffordableMoves(moveFlags, 0) == 0) {
			fighter->cooldown = 0x50;
			fighter->flags |= FIGHTER_FLAG_ON_CHARGEUP;
			return;
		}
		result = STD_selectMoveByMpCost(0, moveFlags);
		((DigimonEntity *)partner)->stats.current.chargeMode = 2;
		break;
	}

	if (result == -1) {
		STD_selectPartnerMove((DigimonEntity *)partner, fighter, 0);
	} else {
		STD_setupQueuedMove((DigimonEntity *)partner, fighter, 0, result);
	}
}

void STD_combatSetup(void)
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

	STD_DEFAULT_CAM_MIN_DISTANCE = 200;
	STD_initializeBattleStartText();

	frames = 0;
	finished = 0;
	playSound(0, 0x10);

	while (frames < 60 || finished == 0) {
		if (STD_DEFAULT_CAM_MIN_DISTANCE < 4200) {
			STD_DEFAULT_CAM_MIN_DISTANCE += 400;
		}

		++frames;
		finished = STD_isBattleStartTextFinished();
		STD_battleTickFrame();
	}

	STD_removeBattleStartText();
	STD_initializeBattleStartTextBurst();
	playSound(0, 0x11);

	while (STD_isBattleStartTextFinished() == 0) {
		if (STD_DEFAULT_CAM_MIN_DISTANCE > 1000) {
			STD_DEFAULT_CAM_MIN_DISTANCE -= 400;
		}

		STD_battleTickFrame();
	}

	STD_removeBattleStartTextBurst();

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

	STD_DEFAULT_CAM_MIN_DISTANCE = 1000;
	STD_TIMER_ACTIVE = 1;
	STD_CAMERA_STATE = 1;
	GAME_STATE = 4;
}

int16_t STD_checkEndCondition(void)
{
	Entity *other;
	int32_t i;

#if VERSION_EQUAL_OR_NEWER(JP_BOMBOM)
	if (COMBAT_DATA_PTR->fighter[0].hpDamageBuffer != 0) {
		return 0;
	}

	if (COMBAT_DATA_PTR->fighter[1].hpDamageBuffer != 0) {
		return 0;
	}
#endif

	if (ENTITY_TABLE[1]->anim.animId == 0x2b &&
	    (ENTITY_TABLE[1]->anim.animFlag & 1) == 0) {
		if (STD_areAllEnemyDigimonDead() == 0) {
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

	if (STD_TIMER == 0) {
		if ((PARTNER_ENTITY.digimonEntity.stats.current.currentHP - (*(FighterData **)&COMBAT_DATA_PTR)->hpDamageBuffer) <= 0) {
			return 0;
		}

		if (STD_areAllEnemyDigimonDead() != 0) {
			return 0;
		}

		STD_DAMAGE[0] = STD_STARTING_HP[0] - ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]])->stats.current.currentHP;
		STD_DAMAGE[1] = STD_STARTING_HP[1] - ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]])->stats.current.currentHP;
		COMBAT_DATA_PTR->fighter[0].hpDamageBuffer = 0;
		COMBAT_DATA_PTR->fighter[1].hpDamageBuffer = 0;

		if (STD_DAMAGE[0] > STD_DAMAGE[1]) {
			STD_tickBattleResultScreen(1, 0);
			return -1;
		}

		if (STD_DAMAGE[0] < STD_DAMAGE[1]) {
			STD_tickBattleResultScreen(0, 1);
			return 1;
		}

		if (STD_DAMAGE[0] == STD_DAMAGE[1]) {
			STD_tickBattleResultScreen(1, 1);
			return 2;
		}
	}

	return 0;
}

void STD_initializeCombat(Entity *entity, Entity *other)
{
	int32_t i;
	FighterData *f;
	Stats *stats;
	int32_t j;
	int16_t *dst;
	DigimonEntity *digimon;
	int16_t brains;

	STD_DAMAGE[0] = 0;
	STD_DAMAGE[1] = 0;
	STD_IS_DRAW = 0;
	STD_addFighterCounter(0x63);
	STD_VICTORY_FRAMES = 0;
	if (entity->type == other->type) {
		STD_initializePlayerMarker();
	}

	resetFlattenGlobal();
	STD_FINISHER_AURA_ID = -1;
	COMBAT_DATA_PTR->player.remainingChargeupTime[0] = -1;
	STD_SAVED_CHARGE_MODE = PARTNER_ENTITY.digimonEntity.stats.current.chargeMode;
#if VERSION_REGION_IS(NTSCJ)
	FINISHING_ENTITY = NULL;
	STD_FINISHER_TIMER = 0;
	FLEE_DISABLED[1] = 0;
#else
	FLEE_DISABLED[1] = 0;
	FINISHING_ENTITY = NULL;
	STD_FINISHER_TIMER = 0;
#endif
	P2_AOE_TIMER = 0;
	COMBAT_DATA_PTR->player.unk7 = 0;
	COMBAT_DATA_PTR->player.changeTarget = 0;
	NO_AI_FLAG = 0;
	BATTLE_TOGGLE_LIFEBAR = 0;
	FLEE_DISABLED[0] = 1;
	BATTLE_FRAME_COUNT = 1;
	STD_TIMER_ACTIVE = 0;
	STD_DISABLE_HITTING = 0;
	for (i = 0; i < 2; i++) {
		COMBAT_DATA_PTR->player.unk5[i] = 0xff;
	}

	initializeAttackObjects();
	ENEMY_COUNT = 1;
	COMBAT_DATA_PTR->player.entityIds[0] = 1;
	COMBAT_DATA_PTR->player.entityIds[1] = STD_getEntityIndex2(other);
	for (i = 0; i < 0xc; i++) {
		COMBAT_DATA_PTR->player.usedMoves[i] = 0xff;
	}

	COMBAT_DATA_PTR->player.startingHP = PARTNER_ENTITY.digimonEntity.stats.current.currentHP;
	STD_COMBAT_ACTIVE = 1;
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
		f->activeEffectSlot = -1;
		f->speedBuffer = 0x64;
		f->unk15 = 0;
		f->hasCollidedWhileDistanceCmd = 0;
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
		COMBAT_DATA_PTR->player.numCommands[0] = STD_BRAIN_TO_COMMAND_MAP[brains / 100];
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
		STD_STARTING_HP[i] = stats->current.currentHP;
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

	STD_addFighterStatusBars(0);
	STD_addFighterStatusBars(1);
	STD_addCommandMenu(0);
}

void STD_tickEnemyAI(void)
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
							*flagsPtr |= FIGHTER_FLAG_SENILE;
							fighter->senileTimer = 100;
						}
					}
				}
			}
			if (!(*flagsPtr & FIGHTER_FLAG_SENILE)) {
				STD_increaseSpeedBuffer(fighter, stats);
			}
			if (fighter->cooldown >= 2) {
				fighter->cooldown--;
			}
		}
		if (*flagsPtr & (FIGHTER_FLAG_KNOCKED_BACK | FIGHTER_FLAG_ATTACKING | FIGHTER_FLAG_BLOCKING | FIGHTER_FLAG_DEAD)) {
			continue;
		}
		if (stats->current.currentHP == 0) {
			STD_faintDigimon((DigimonEntity *)entity, fighter, i);
			if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP > COMBAT_DATA_PTR->fighter[0].hpDamageBuffer) {
				STD_selectRandomCamera((DigimonEntity *)entity, 5, 0);
			}
			STD_DISABLE_HITTING = 1;
			continue;
		}
		if (stats->current.currentHP <= fighter->hpDamageBuffer) {
			handleBattleIdle((DigimonEntity *)entity, stats, *flagsPtr);
			fighter->moveRange = -1;
			STD_resetFlatten(i);
			STD_removeStatusEffects((DigimonEntity *)entity, fighter);
			fighter->flags &= ~(FIGHTER_FLAG_POISONED | FIGHTER_FLAG_CONFUSED | FIGHTER_FLAG_STUNNED | FIGHTER_FLAG_FLATTENED);
			continue;
		}
		if (((DigimonEntity *)partner)->stats.current.currentHP == 0) {
			handleBattleIdle((DigimonEntity *)entity, stats, *flagsPtr);
			STD_resetFlatten(i);
			*flagsPtr &= ~(FIGHTER_FLAG_KNOCKED_BACK | FIGHTER_FLAG_ATTACKING | FIGHTER_FLAG_BLOCKING);
			*flagsPtr |= FIGHTER_FLAG_TRANSFORMING;
			fighter->moveRange = -1;
			continue;
		}
		if (NO_AI_FLAG != 0) {
			return;
		}
		if (*flagsPtr & FIGHTER_FLAG_TRANSFORMING) {
			continue;
		}
		if (*flagsPtr & FIGHTER_FLAG_FLATTENED) {
			fighter->queuedAnim = 0;
			fighter->targetId = 0;
			fighter->moveRange = 2;
			startAnimation(entity, 0x23);
			entity->flatSprite = 0;
			fighter->flags |= FIGHTER_FLAG_TRANSFORMING;
			continue;
		}
		if (*flagsPtr & FIGHTER_FLAG_STUNNED) {
			continue;
		}
		if (*flagsPtr & FIGHTER_FLAG_CONFUSED) {
			STD_selectConfusedMove((DigimonEntity *)entity, fighter, i);
			continue;
		}
		if (*flagsPtr & FIGHTER_FLAG_SENILE) {
			continue;
		}
		if (*flagsPtr & FIGHTER_FLAG_ON_CHARGEUP) {
			continue;
		}
		if (*flagsPtr & FIGHTER_FLAG_ON_COOLDOWN) {
			continue;
		}
		if (!(*flagsPtr & (FIGHTER_FLAG_CONFUSED | FIGHTER_FLAG_STUNNED | FIGHTER_FLAG_FLATTENED | FIGHTER_FLAG_ON_CHARGEUP | FIGHTER_FLAG_ON_COOLDOWN | FIGHTER_FLAG_DEAD)) && (fighter->flatTimer == 0)) {
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
		if (!(*flagsPtr & FIGHTER_FLAG_10)) {
			fighter->targetId = 0;
		}
		if (randomLimit(10) == 0) {
			stats->current.chargeMode = 2;
		} else {
			stats->current.chargeMode = randomLimit(2);
		}
		STD_selectEnemyMove((DigimonEntity *)entity, fighter, i);
	}
}

void STD_tickBattle(void)
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

	if (P2_AOE_TIMER > 0) {
		P2_AOE_TIMER--;
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
		if (*flags & FIGHTER_FLAG_ATTACKING) {
			STD_tickAttackState(&entity->entity, target, i);
		} else if ((*flags & FIGHTER_FLAG_KNOCKED_BACK) || (*flags & FIGHTER_FLAG_BLOCKING)) {
			STD_tickHitState(&entity->entity, fighter, i);
		} else if (fighter->moveRange != -1) {
			if (*flags & FIGHTER_FLAG_FLATTENED) {
				STD_tickFlatState(entity, target, fighter, i);
			} else if (*flags & FIGHTER_FLAG_STUNNED) {
				STD_tickStunState(&entity->entity);
			} else if (*flags & FIGHTER_FLAG_CONFUSED) {
				STD_tickConfusedState(entity, target, fighter, i);
			} else if (*flags & FIGHTER_FLAG_SENILE) {
				STD_tickDigimonSenile(entity, fighter);
			} else if (*flags & FIGHTER_FLAG_ON_CHARGEUP) {
				STD_tickChargeState(entity, target, fighter);
			} else if (*flags & FIGHTER_FLAG_ON_COOLDOWN) {
				STD_tickCooldownState(entity, target, fighter);
			} else {
				STD_tickQueuedMove(entity, target, fighter, i);
			}
		}
	}

	STD_tickAttackHits();
	STD_applyMoveResult();
	if (STD_CAMERA_STATE != 7 && STD_CAMERA_STATE != 8) {
		if (STD_CAMERA_TIMER != 0) {
			STD_CAMERA_TIMER--;
			if (STD_CAMERA_TIMER == 0) {
				STD_CAMERA_STATE = 1;
			}
		}
		if ((BATTLE_FRAME_COUNT % 600) == 0 && randomLimit(2) == 1) {
			STD_CAMERA_STATE = 6;
			STD_setRandomViewpoint(ENTITY_TABLE[1], 4);
			STD_CAMERA_TIMER = randomLimit(0x29) + 0x3c;
		}
	}

	for (i = 0; ENEMY_COUNT >= i; i++) {
		if (COMBAT_DATA_PTR->fighter[i].flags & FIGHTER_FLAG_ATTACKING) {
			break;
		}
		if (COMBAT_DATA_PTR->fighter[i].flags & FIGHTER_FLAG_KNOCKED_BACK) {
			break;
		}
		if (STD_CAMERA_TIMER != 0) {
			break;
		}
		if (STD_CAMERA_STATE == 7) {
			break;
		}
		if (STD_CAMERA_STATE == 8) {
			break;
		}
	}

	if (i == ENEMY_COUNT + 1) {
		STD_CAMERA_STATE = 1;
	}
}

int16_t STD_deinitializeCombat(int16_t a, int16_t b)
{
	Stats *stats;
	int32_t i;
	int32_t k;

	PARTNER_ENTITY.digimonEntity.stats.current.chargeMode = STD_SAVED_CHARGE_MODE;
	GAME_STATE = 5;
	STD_removeCombatObjects();
	for (i = 0; i <= ENEMY_COUNT; i++) {
		removeEntityText(i);
		STD_resetFlatten(i);
		STD_removeStatusEffects((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]], &COMBAT_DATA_PTR->fighter[i]);
		COMBAT_DATA_PTR->fighter[i].flags = 0;
	}
	if (a == b) {
		if (STD_TIMER != 0 || STD_IS_DRAW == 0) {
			STD_loadVersusSceneModel();
			STD_addVersusModelScene();
			while (STD_isVersusModelSceneFinished() == 0) {
				STD_battleTickFrame();
			}
			STD_removeVersusModelScene();
		}
	} else {
		if (a == 0) {
			STD_VICTORY_FRAMES = 0x64;
		} else {
			STD_VICTORY_FRAMES = 0x78;
		}
	}
	if (a != b) {
		startAnimation(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[a]], 0x2a);
		k = 0;
		stopBGM();
		stopSound();
		if (a == 0) {
			playMusic(STD_MUSIC, 3);
		} else {
			playMusic(STD_MUSIC, 4);
		}
		for (; k < STD_VICTORY_FRAMES; k++) {
			STD_battleTickFrame();
			if ((ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[a]]->anim.animFlag & 1) == 0) {
				startAnimation(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[a]], 0x2a);
			}
		}
	}
	STD_COMBAT_ACTIVE = 0;
	for (i = 0; i < 0x14; i++) {
		STD_battleTickFrame();
	}
	STD_removeFighterStatusBars(0);
	STD_removeFighterStatusBars(1);
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

void STD_removePlayerMarker(void)
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
	fighter->flags &= ~FIGHTER_FLAG_FLATTENED;
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

int32_t STD_areAllEnemyDigimonDead(void)
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

void STD_tickBattleResultScreen(int16_t hasLostP1, uint8_t hasLostP2)
{
	DigimonEntity *e0;
	DigimonEntity *e1;

	STD_BATTLE_RESULT_TIMER = 0;
	addObject(0x1a2, 0, NULL, (RenderFunction)STD_renderTimeoutText);
	stopBGM();
	if (hasLostP1 == hasLostP2) {
		DigimonEntity *p0;
		DigimonEntity *p1;

		STD_IS_DRAW = 1;
		p0 = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]];
		p1 = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]];
		handleBattleIdle(p0, &p0->stats, COMBAT_DATA_PTR->fighter[0].flags);
		handleBattleIdle(p1, &p1->stats, COMBAT_DATA_PTR->fighter[1].flags);
		STD_battleTickFrame();
		STD_battleTickFrame();
		STD_loadVersusSceneModel();
		STD_addTimeoutWindow();
		while (STD_BATTLE_RESULT_TIMER < 0x3d) {
			if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON) {
				break;
			}
			STD_battleTickFrame();
			STD_BATTLE_RESULT_TIMER++;
		}
		removeAnimatedUIBox(0, 0);
		STD_addVersusModelScene();
		while (STD_isVersusModelSceneFinished() == 0) {
			STD_battleTickFrame();
		}
		STD_removeVersusModelScene();
		return;
	}
	e0 = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[hasLostP1]];
	e1 = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[hasLostP2]];
	handleBattleIdle(e0, &e0->stats, COMBAT_DATA_PTR->fighter[hasLostP1].flags);
	STD_faintDigimon(e1, &COMBAT_DATA_PTR->fighter[hasLostP2], hasLostP2);
	while (STD_BATTLE_RESULT_TIMER < 0x79) {
		if (STD_BATTLE_RESULT_TIMER >= 0x3d) {
			if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON) {
				break;
			}
		}
		if (STD_BATTLE_RESULT_TIMER == 0x3c) {
			STD_addTimeoutWindow();
		}
		STD_battleTickFrame();
		STD_BATTLE_RESULT_TIMER++;
	}
	STD_selectRandomCamera(e1, 5, 0);
	entityLookAtLocation(&e0->entity, &e1->entity.posData->location);
	removeAnimatedUIBox(0, 0);
}

void STD_addTimeoutWindow(void)
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

	createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, NULL, (RenderFunction)STD_renderTimeoutWindow);
}

void STD_faintDigimon(DigimonEntity *digimon, FighterData *fighter, int16_t arg2)
{
	digimon->stats.current.isHit = 1;
	fighter->flags |= FIGHTER_FLAG_DEAD;
	startAnimation(&digimon->entity, 0x2b);
	STD_resetFlatten(arg2);
	STD_removeStatusEffects(digimon, fighter);
	fighter->flags &= ~(FIGHTER_FLAG_POISONED | FIGHTER_FLAG_CONFUSED | FIGHTER_FLAG_STUNNED | FIGHTER_FLAG_FLATTENED | FIGHTER_FLAG_KNOCKED_BACK | FIGHTER_FLAG_ATTACKING | FIGHTER_FLAG_BLOCKING);
	fighter->flags |= FIGHTER_FLAG_TRANSFORMING;
	fighter->moveRange = -1;
	STD_resetFighterAction(fighter);
}

int32_t STD_getDigitCount(int32_t value)
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

	if (STD_handlePartnerMoveCommand(digimon, target, fighter) == 0) {
		STD_tickDigimonAttackRanged(digimon, target, fighter, 0x79);
		if (fighter->flags & FIGHTER_FLAG_ATTACKING) {
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

	if ((fighter->flags & FIGHTER_FLAG_ON_COOLDOWN) || (fighter->flags & FIGHTER_FLAG_ON_CHARGEUP)) {
		STD_confusedRotate(&digimon->entity);
		STD_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
		collisionGrace(NULL, &digimon->entity, 0x118, 0xc8);
		if (fighter->cooldown < 2) {
			fighter->flags &= ~FIGHTER_FLAG_ON_COOLDOWN;
			fighter->cooldown = 0;
		}
		return;
	}

	if (target == NULL) {
		STD_confusedRotate(&digimon->entity);
		if (STD_tickMeleeAttack(digimon, NULL, fighter, arg3) != 0) {
			collisionGrace(NULL, &digimon->entity, 0x118, 0xc8);
		}
		if (fighter->flags & FIGHTER_FLAG_ATTACKING) {
			return;
		}
		if (randomLimit(100) >= 5) {
			return;
		}
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		STD_startQueuedMove(digimon, target, fighter);
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
		STD_tickDigimonAttackRanged(digimon, target, fighter, entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim));
		break;
	case 4:
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		STD_startQueuedMove(digimon, target, fighter);
		break;
	}
}

void STD_tickDigimonSenile(DigimonEntity *digimon, FighterData *fighter)
{
	fighter->senileTimer--;
	if (fighter->senileTimer == 0) {
		fighter->flags &= ~(FIGHTER_FLAG_TRANSFORMING | FIGHTER_FLAG_SENILE);
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

	r = STD_handlePartnerMoveCommand(digimon, target, fighter);
	if (fighter->cooldown != 0) {
		if (r == 0) {
			switch (digimon->stats.current.chargeMode) {
			case 0:
				STD_maintainTargetDistance(digimon, target, fighter);
				break;
			case 1:
				handleBattleIdle(digimon, &digimon->stats, fighter->flags);
				entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
				fighter->hasCollidedWhileDistanceCmd = 0;
				break;
			case 2:
				STD_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
				STD_backAwayFromTarget(digimon, target, fighter);
				break;
			}
		}
		if (fighter->cooldown < 2) {
			fighter->flags &= ~(FIGHTER_FLAG_TRANSFORMING | FIGHTER_FLAG_ON_CHARGEUP);
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
		fighter->hasCollidedWhileDistanceCmd = 0;
		if (fighter->speedBuffer > 0) {
			fighter->flags &= ~FIGHTER_FLAG_ON_CHARGEUP;
		}
		break;
	case 1:
		STD_maintainTargetDistance(digimon, target, fighter);
		tech = entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim);
		if ((fighter->speedBuffer == 100) || (fighter->speedBuffer >= MOVE_DATA[tech].power)) {
			fighter->flags &= ~FIGHTER_FLAG_ON_CHARGEUP;
		}
		break;
	case 2:
		STD_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
		STD_backAwayFromTarget(digimon, target, fighter);
		if (fighter->speedBuffer == 100) {
			fighter->flags &= ~FIGHTER_FLAG_ON_CHARGEUP;
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
		fighter->flags &= ~(FIGHTER_FLAG_TRANSFORMING | FIGHTER_FLAG_ON_COOLDOWN);
		fighter->cooldown = 0;
	}
}

void STD_tickQueuedMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3)
{
	if ((NO_AI_FLAG != 0) && (FINISHING_ENTITY != &digimon->entity)) {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		return;
	}

	if (STD_handlePartnerMoveCommand(digimon, target, fighter) == 0) {
		switch (fighter->moveRange) {
		case 1:
			if (STD_tickMeleeAttack(digimon, target, fighter, arg3) != 0) {
				collisionGrace(&target->entity, &digimon->entity, 0x118, 0xc8);
			}
			break;
		case 2:
		case 3:
			STD_tickDigimonAttackRanged(digimon, target, fighter, DIGIMON_DATA[digimon->entity.type].moves[fighter->queuedAnim - 0x2e]);
			break;
		case 4:
			handleBattleIdle(digimon, &digimon->stats, fighter->flags);
			STD_startQueuedMove(digimon, target, fighter);
			break;
		}
	}
}

int32_t STD_handlePartnerMoveCommand(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
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
			fighter->hasCollidedWhileDistanceCmd = 0;
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
				fighter->hasCollidedWhileDistanceCmd = 0;
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
				if (STD_FINISHER_TIMER > 0) {
					STD_FINISHER_TIMER--;
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
						STD_initializeFinisherChargeup(tech);
					}
					startAnimation(&digimon->entity, fighter->queuedAnim);
					digimon->entity.anim.animFlag &= 0xfe;
					STD_FINISHER_AURA_ID = STD_addFinisherAura(&digimon->entity, 0x50);
					STD_FINISHER_TIMER = 0x50;
					return 0;
				}
			}
			if (STD_selectMoveTarget(&digimon->entity, fighter) != 0) {
				return 0;
			}
			startAnimation(&digimon->entity, fighter->queuedAnim);
			fighter->flags |= FIGHTER_FLAG_ATTACKING;
			STD_setupMoveExecution(digimon, target, fighter);
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
		if (STD_FINISHER_TIMER > 0) {
			STD_FINISHER_TIMER--;
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			return 0;
		}
		startAnimation(&digimon->entity, fighter->queuedAnim);
		fighter->flags |= FIGHTER_FLAG_ATTACKING;
		STD_setupMoveExecution(digimon, target, fighter);
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

void STD_tickDigimonAttackRanged(DigimonEntity *entity, DigimonEntity *other,
                                 FighterData *data, int16_t move)
{
	uint32_t distance;
	uint32_t total;

	if (data->unk15 > 100) {
		handleBattleIdle(entity, &entity->stats, data->flags);

		++data->unk15;
		if (data->unk15 > 160) {
			data->flags &= ~FIGHTER_FLAG_TRANSFORMING;
			data->unk15 = 0;
		}

		return;
	}

	if (NO_AI_FLAG != 0 && &entity->entity == FINISHING_ENTITY) {
		STD_startQueuedMove(entity, other, data);
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

		if ((data->flags & FIGHTER_FLAG_FLATTENED) != 0) {
			if (BATTLE_FRAME_COUNT % 40 == 0) {
				STD_startQueuedMove(entity, other, data);
			} else {
				entityLookAtLocation(&entity->entity,
				                     &other->entity.posData->location);
			}
		} else {
			STD_startQueuedMove(entity, other, data);
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

	if (fighter->hasCollidedWhileDistanceCmd == 0) {
		*rot = away;

		hit = entityCheckCollision(NULL, &digimon->entity, 0x118, 0xc8);
		if (hit != -1) {
			fighter->hasCollidedWhileDistanceCmd = 1;
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

	if ((NO_AI_FLAG != 0) && (FINISHING_ENTITY == &digimon->entity) && (STD_FINISHER_TIMER > 0)) {
		return;
	}

	if ((fighter->flags & (FIGHTER_FLAG_FLATTENED | FIGHTER_FLAG_ATTACKING)) == 0x28) {
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

	fighter->hasCollidedWhileDistanceCmd = 0;
	if (fighter->flags & FIGHTER_FLAG_ATTACKING) {
		if (index == 0) {
			COMBAT_DATA_PTR->player.hitCount++;
		}
		fighter->flags &= ~FIGHTER_FLAG_10;
		fighter->flags |= FIGHTER_FLAG_ON_COOLDOWN;
		fighter->cooldown = 0x28;
		STD_addFinisherProgress(fighter, fighter->finisherGoal * 2 / 50);
	}

	if (fighter->invulnerableTimer <= 0) {
		if (fighter->flags & FIGHTER_FLAG_FLATTENED) {
			digimon->entity.flatSprite = 0;
		}
		if (!(fighter->flags & FIGHTER_FLAG_ATTACKING)) {
			digimon->stats.current.isHit = 0;
		}
		if (fighter->flags & FIGHTER_FLAG_BLOCKING) {
			fighter->flags &= ~FIGHTER_FLAG_BLOCKING;
			STD_clearBlockedAttacks(fighter);
		} else {
			fighter->flags &= ~(FIGHTER_FLAG_KNOCKED_BACK | FIGHTER_FLAG_ATTACKING | FIGHTER_FLAG_TRANSFORMING | FIGHTER_FLAG_BLOCKING);
		}
	}

	if (!(fighter->flags & FIGHTER_FLAG_KNOCKED_BACK)) {
		if (fighter->flatTimer == -1) {
			fighter->flatTimer = 0x41;
		}
	} else {
		fighter->senileTimer = 0;
		fighter->flags &= ~FIGHTER_FLAG_SENILE;
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
		fighter->hasCollidedWhileDistanceCmd = 0;
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

int32_t STD_hasAffordableMoves(int16_t *out, int16_t index)
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

void STD_initializePlayerMarker(void)
{
	addObject(0x1a3, 0, 0, (RenderFunction)STD_renderPlayerMarker);
	addObject(0x1a3, 1, 0, (RenderFunction)STD_renderPlayerMarker);
}

int16_t STD_getEntityIndex2(Entity *entity)
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
	cand[0] = STD_CARDINAL_ROTATIONS[base];
	cand[1] = (STD_CARDINAL_ROTATIONS[base] + 0x400) & 0xfff;
	for (i = 0; i < 2; i++) {
		*rot = cand[i];
		if (entityCheckCollision(NULL, entity, 0x118, 0xc8) == -1) {
			break;
		}
	}

	switch (i) {
	case 0:
		for (i = 0; i < 3; i++) {
			*rot = cand[i] = (STD_CARDINAL_ROTATIONS[base] + 0xc00 + (i * 0x200)) & 0xfff;
			if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
				goto common;
			}
		}
		break;
	case 1:
		for (i = 0; i < 3; i++) {
			*rot = cand[i] = (STD_CARDINAL_ROTATIONS[base] + 0x400 + (i * 0x200)) & 0xfff;
			if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
				goto common;
			}
		}
		break;
	default:
		for (i = 0; i < 3; i++) {
			*rot = cand[i] = (STD_CARDINAL_ROTATIONS[base] + 0x800 + (i * 0x200)) & 0xfff;
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

int16_t STD_combatMain(Entity *entity, Entity *other)
{
	int16_t result;

	COMBAT_AREA_X = 0;
	COMBAT_AREA_Y = 0;
	stopBGM();
	playMusic(STD_MUSIC, 2);
	STD_initializeCombat(entity, other);
	STD_combatSetup();
	while (1) {
		result = STD_checkEndCondition();
		if (result != 0) {
			break;
		}

		STD_tickPartnerAI();
		STD_tickEnemyAI();
		STD_tickBattle();
		STD_battleTickFrame();
		handlePause();
	}

	removePauseBox();
	if (result == -1) {
		STD_deinitializeCombat(1, 0);
	} else if (result == 1) {
		STD_deinitializeCombat(0, 1);
	} else {
		result = STD_deinitializeCombat(1, 1);
	}

	return result;
}

void STD_renderTimeoutText(void)
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

void STD_renderTimeoutWindow(int32_t id)
{
	int16_t x;
	int16_t y;
	int32_t digits;

	x = UI_BOX_DATA[id].finalPos.x;
	y = UI_BOX_DATA[id].finalPos.y;
	renderString(0, x + 6, y + 6, 120, 12, 0, 24, 6 - id, 1);
	renderString(0, x + 138, y + 6, 120, 12, 0, 36, 6 - id, 1);
	renderString(0, x + 108, y + 24, 48, 24, 0, 0, 6 - id, 1);

	digits = STD_getDigitCount(STD_DAMAGE[1]);
	STD_renderIntroStatNumber(x + 42 + (48 - digits * 12) / 2, y + 30, digits,
	                          STD_DAMAGE[1], 6 - id);
	digits = STD_getDigitCount(STD_DAMAGE[0]);
	STD_renderIntroStatNumber(x + 174 + (48 - digits * 12) / 2, y + 30, digits,
	                          STD_DAMAGE[0], 6 - id);
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

void STD_renderPlayerMarker(int16_t id)
{
	POLY_FT4 *prim;
	MATRIX *m;
	SVECTOR pos;
	Entity *entity;
	uint32_t otz;
	int32_t offset;
	DVECTOR sxy;

	entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]];
	GsSetLsMatrix(&GsWSMATRIX);
	m = &entity->posData[1].posMatrix.workm;
	pos.vx = m->t[0];
	pos.vy = m->t[1];
	pos.vz = m->t[2];
	gte_ldv0(&pos);
	gte_rtps();
	gte_stsxy(&sxy);
	gte_stszotz(&otz);
	sxy.vx -= (int16_t)(160 - DRAWING_OFFSET_X);
	sxy.vy -= (int16_t)(120 - DRAWING_OFFSET_Y);
	offset = VIEWPORT_DISTANCE * (DIGIMON_DATA[entity->type].radius / 2) / (otz * 4);
	sxy.vx += (int16_t)offset;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->tpage = getTPage(0, 0, 768, 0);
	prim->clut = GetClut(0, 0x1e1);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setUVWH(prim, id * 48 + 0xa0, 0xe0, (id == 0 ? 0x30 : 0x2f), 31);
	setXYWH(prim, sxy.vx, sxy.vy - 0x20, (id == 0 ? 0x30 : 0x2f), 31);
	AddPrim(ACTIVE_ORDERING_TABLE->org + otz, prim++);

	GsSetWorkBase((PACKET *)prim);
}

void STD_tickAttackHits(void)
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
		if (fighter->flags & FIGHTER_FLAG_DEAD) {
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
		fighter->flags &= ~(FIGHTER_FLAG_KNOCKED_BACK | FIGHTER_FLAG_ATTACKING | FIGHTER_FLAG_TRANSFORMING);
		attacker = ENTITY_TABLE[attack.casterId];
		chance = STD_applyPartnerStatsToFighter((DigimonEntity *)attacker, (DigimonEntity *)entity, fighter, tech);
		if (entity == FINISHING_ENTITY) {
			if (i == 0) {
				STD_removeFinisherChargeup();
			}
			if (STD_FINISHER_AURA_ID != -1L) {
				STD_removeFinisherAura(STD_FINISHER_AURA_ID);
			}
			NO_AI_FLAG = 0;
			FINISHING_ENTITY = NULL;
		}
		if (randomLimit(100) < chance) {
			if (STD_CAMERA_STATE == 6 && MOVE_DATA[tech].range == 1 && STD_CAMERA_STATE == 3) {
				if (MOVE_DATA[entityGetTechFromAnim(STD_FOCUSED_ENTITY, STD_FOCUSED_ENTITY->anim.animId)].range == 3) {
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
			fighter->flags |= FIGHTER_FLAG_KNOCKED_BACK;
			STD_handleHitReaction(entity, fighter, &attack, i);
			sub->unk25 = 0;
			addEntityText(entity, i, 0, dmg, 0);
			fighter->invulnerableTimer = MOVE_DATA[tech].iframes;
			entity->anim.animFlag &= 0xfe;
			STD_applyMoveStatus((DigimonEntity *)entity, fighter, tech);
			continue;
		}
		handled = 0;
		if (!(fighter->flags & FIGHTER_FLAG_BLOCKING) && (MOVE_DATA[tech].range == 1) && (DIGIMON_DATA[entity->type].moves[(uint32_t)(entity->anim.animId - 0x2e)] != 0x2d)) {
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
						STD_startQueuedMove((DigimonEntity *)entity, (DigimonEntity *)attacker, fighter);
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
		if ((fighter->flags & FIGHTER_FLAG_BLOCKING) && (fighter->invulnerableTimer > 0)) {
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
			addEntityText(entity, i, 0, dmg, 0);
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
		fighter->flags |= FIGHTER_FLAG_BLOCKING;
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

	if (STD_DISABLE_HITTING != 0) {
		return 0;
	}

	if (fighter->flags & (FIGHTER_FLAG_STUNNED | FIGHTER_FLAG_FLATTENED | FIGHTER_FLAG_SENILE)) {
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
		if (fighter->flags & FIGHTER_FLAG_BLOCKING) {
			fighter->flags &= ~FIGHTER_FLAG_BLOCKING;
			fighter->invulnerableTimer = 0;
		}
		return 100;
	}

	if (fighter->flags & FIGHTER_FLAG_BLOCKING) {
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
	if (fighter->flags & FIGHTER_FLAG_FLATTENED) {
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

	if (fighter->flags & FIGHTER_FLAG_PROTECTED) {
		return;
	}

	if (MOVE_DATA[move].statusChance == 0) {
		return;
	}

	chance = MOVE_DATA[move].statusChance;
	if (randomLimit(100) < chance) {
		switch (MOVE_DATA[move].status) {
		case 1:
			if (!(fighter->flags & FIGHTER_FLAG_POISONED)) {
				fighter->flags |= FIGHTER_FLAG_POISONED;
				fighter->poisonTimer = 100;
				STD_addPoisonStatusVisual(digimon, fighter);
			}
			break;
		case 2:
			if (!(fighter->flags & FIGHTER_FLAG_CONFUSED)) {
				fighter->flags |= FIGHTER_FLAG_CONFUSED;
				fighter->confusionTimer = randomLimit(0x65) + 200;
				STD_addConfusionStatusVisual(digimon, fighter);
				STD_resetFighterAction(fighter);
			}
			break;
		case 3:
			if (!(fighter->flags & FIGHTER_FLAG_STUNNED)) {
				fighter->flags |= FIGHTER_FLAG_STUNNED;
				fighter->stunTimer = randomLimit(0x29) + 200;
				STD_addStunStatusVisual(digimon, fighter);
				STD_resetFighterAction(fighter);
			}
			break;
		case 4:
			if (!(fighter->flags & FIGHTER_FLAG_FLATTENED)) {
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
		if ((fighter->table1[i] == other->effectSlot[3]) && (fighter->table2[i] == other->activeEffectSlot)) {
			return 0;
		}
	}

	fighter->table1[i] = other->effectSlot[3];
	fighter->table2[i] = other->activeEffectSlot;

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
	addEntityText(&digimon->entity, slot, color, value, flag);
}

void STD_startHitAnimation(Entity *entity, AttackObject *attack, uint8_t animId)
{
	int16_t tech;

	tech = STD_getAttackTech(attack);
	startAnimation(entity, animId);
	createParticleFX(MOVE_DATA[tech].special, 1, &attack->position, entity, MOVE_DATA[tech].iframes + 0x10);
}

void STD_tickFrames(int16_t count)
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
	GsSetWorkBase(GS_WORK_BASES[ACTIVE_FRAMEBUFFER]);
	GsClearOt(0, 0, ACTIVE_ORDERING_TABLE);
	tickObjects();
	renderObjects();
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0x20, &DR_OFFSETS[ACTIVE_FRAMEBUFFER]);
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

	if (STD_getUsableMoves(choice.flags, index) == 0) {
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

void STD_selectEnemyMove(DigimonEntity *digimon, FighterData *fighter, int16_t index)
{
	int32_t j;
	int32_t pick;
	int32_t total;
	int16_t flags[4];
	int16_t weights[4];
	int16_t tech;
	int32_t i;

	if (STD_getUsableMoves(flags, index) == 0) {
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
		fighter->flags |= FIGHTER_FLAG_ON_CHARGEUP;
	}
}

void STD_selectPartnerMove(DigimonEntity *digimon, FighterData *fighter, int16_t index)
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

	if (STD_getUsableMoves(flags, index) == 0) {
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
				weights[keys[i]] = STD_YOUR_CALL_POWER_PRIO[groups[i]];
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
				weights[keys[keys[i]]] = STD_YOUR_CALL_MP_PRIO[groups[i]];
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
					weights[keys[i]] = STD_YOUR_CALL_WIDE_PRIO[groups[i]];
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
		fighter->flags |= FIGHTER_FLAG_ON_CHARGEUP;
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
	fighter->flags |= FIGHTER_FLAG_TRANSFORMING;
}

void STD_applyChargeRequirement(DigimonEntity *digimon, FighterData *fighter, int16_t tech)
{
	switch (digimon->stats.current.chargeMode) {
	case 0:
		if (fighter->speedBuffer <= 0) {
			fighter->flags |= FIGHTER_FLAG_ON_CHARGEUP;
		}
		break;
	case 1:
		if ((fighter->speedBuffer != 100) && (fighter->speedBuffer < MOVE_DATA[tech].power)) {
			fighter->flags |= FIGHTER_FLAG_ON_CHARGEUP;
		}
		break;
	case 2:
		if (fighter->speedBuffer < 100) {
			fighter->flags |= FIGHTER_FLAG_ON_CHARGEUP;
		}
		break;
	}
}

void STD_startQueuedMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	int16_t tech;

	if (NO_AI_FLAG != 0) {
		if (FINISHING_ENTITY != &digimon->entity) {
			return;
		}
		if (STD_FINISHER_TIMER > 0) {
			STD_FINISHER_TIMER--;
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
				STD_initializeFinisherChargeup(tech);
			}
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			startAnimation(&digimon->entity, fighter->queuedAnim);
			digimon->entity.anim.animFlag &= 0xfe;
			STD_FINISHER_AURA_ID = STD_addFinisherAura(&digimon->entity, 0x50);
			STD_FINISHER_TIMER = 0x50;
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
			P2_AOE_TIMER = 0x6e;
		}
	}
	startAnimation(&digimon->entity, fighter->queuedAnim);
	fighter->flags |= FIGHTER_FLAG_ATTACKING;
	if ((fighter->flags & FIGHTER_FLAG_FLATTENED) == 0) {
		STD_setupMoveExecution(digimon, target, fighter);
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
			if (P2_AOE_TIMER > 0) {
				return 1;
			}
		}
		for (i = 0; i <= ENEMY_COUNT; i++) {
			e = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
			f = &COMBAT_DATA_PTR->fighter[i];
			if (entity == e) {
				continue;
			}
			if ((f->flags & FIGHTER_FLAG_ATTACKING) == 0) {
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

void STD_setupMoveExecution(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
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
		fighter->activeEffectSlot = fighter->effectSlot[i];
	}

	if ((MOVE_DATA[tech].range == 4) && (fighter->buffsRemaining != 0)) {
		fighter->buffsRemaining--;
	}

	fighter->speedBuffer -= MOVE_DATA[tech].power;
	if (fighter->speedBuffer < -0x9b) {
		fighter->speedBuffer = -0x9b;
	}

	if (STD_CAMERA_STATE == 3) {
		if (MOVE_DATA[entityGetTechFromAnim(STD_FOCUSED_ENTITY, STD_FOCUSED_ENTITY->anim.animId)].range == 3) {
			return;
		}
	}

	if (STD_CAMERA_STATE == 6) {
		if (STD_CAMERA_STATE != 3) {
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

	STD_selectRandomCamera((DigimonEntity *)digimon, randomLimit(n), MOVE_DATA[tech].range);
}

void STD_removeMoveEffect(DigimonEntity *digimon, FighterData *fighter)
{
	if (fighter->activeEffectSlot != -1L) {
		STD_stopEFESubEffect(fighter->activeEffectSlot, digimon->stats.current.efeSubEffect);
	}
	digimon->stats.current.efeSubEffect = -1;
	fighter->activeEffectSlot = -1;
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
		if (fighter->flags & FIGHTER_FLAG_DEAD) {
			continue;
		}
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		stats = &((DigimonEntity *)entity)->stats;
		if (fighter->flags & FIGHTER_FLAG_POISONED) {
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
				addEntityText(entity, i, 0xc, dmg, 0);
			}
		}
		if (fighter->flags & FIGHTER_FLAG_CONFUSED) {
			if ((NO_AI_FLAG == 0) && (fighter->confusionTimer != 0)) {
				fighter->confusionTimer--;
			}
			if ((fighter->confusionTimer == 0) && !(combat->fighter[0].flags & FIGHTER_FLAG_ATTACKING)) {
				STD_updateFighterStatusVisuals((DigimonEntity *)entity, fighter);
			}
		}
		if (fighter->flags & FIGHTER_FLAG_STUNNED) {
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
		if (fighter->flags & FIGHTER_FLAG_FLATTENED) {
			switch (fighter->flatTimer) {
			case 0x40:
				if ((fighter->flags & FIGHTER_FLAG_KNOCKED_BACK) || (fighter->flags & FIGHTER_FLAG_ATTACKING)) {
					fighter->flatTimer++;
				} else {
					startAnimation(entity, 0x22);
					fighter->moveRange = -1;
					fighter->flags |= FIGHTER_FLAG_TRANSFORMING;
					stats->current.isHit = 1;
				}
				break;
			case 3:
				entity->flatSprite = -1;
				break;
			case 0:
				fighter->flags &= ~(FIGHTER_FLAG_FLATTENED | FIGHTER_FLAG_TRANSFORMING);
				stats->current.isHit = 0;
				if (fighter->flags & FIGHTER_FLAG_STUNNED) {
					STD_addStatusEffectVisual((DigimonEntity *)entity, fighter, 3);
					fighter->moveRange = 0;
				}
				if (fighter->flags & FIGHTER_FLAG_CONFUSED) {
					STD_addStatusEffectVisual((DigimonEntity *)entity, fighter, 2);
				}
				if (fighter->flags & FIGHTER_FLAG_POISONED) {
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
				fighter->flags |= FIGHTER_FLAG_TRANSFORMING;
				stats->current.isHit = 1;
				break;
			case 3:
				entity->flatSprite = 0;
				break;
			case 0:
				fighter->flags |= FIGHTER_FLAG_FLATTENED;
				fighter->flatTimer = randomLimit(0x51) + 0xe0;
				stats->current.isHit = 0;
				fighter->flags &= ~FIGHTER_FLAG_TRANSFORMING;
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

	fighter->flags &= ~FIGHTER_FLAG_CONFUSED;
	fighter->confusionTimer = 0;
	if (((fighter->flags & (FIGHTER_FLAG_STUNNED | FIGHTER_FLAG_FLATTENED)) == 0) && (fighter->flatTimer == 0)) {
		fighter->flags &= ~FIGHTER_FLAG_TRANSFORMING;
		STD_removeStatusEffectVisual(digimon, fighter, 2);
		if (fighter->flags & FIGHTER_FLAG_POISONED) {
			STD_addStatusEffectVisual(digimon, fighter, 1);
		}
	}
}

void STD_clearStun(DigimonEntity *digimon, FighterData *fighter)
{
	STD_resetFighterAction(fighter);
	digimon->entity.anim.animFlag |= 1;
	fighter->flags &= ~FIGHTER_FLAG_STUNNED;
	fighter->stunTimer = 0;
	if (fighter->flags & FIGHTER_FLAG_FLATTENED) {
		return;
	}

	if (fighter->flatTimer != 0) {
		return;
	}

	STD_removeStatusEffectVisual(digimon, fighter, 3);
	if (fighter->flags & FIGHTER_FLAG_CONFUSED) {
		STD_addStatusEffectVisual(digimon, fighter, 2);
	}

	if (fighter->flags & FIGHTER_FLAG_POISONED) {
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
			fighter->statusFxId = STD_addPoisonEffect(&digimon->entity);
			break;
		case 2:
			fighter->statusFxId = STD_addConfusionEffect(&digimon->entity);
			break;
		case 3:
			fighter->statusFxId = STD_addStunEffect(&digimon->entity, fighter->stunTimer);
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
			STD_removePoisonEffect(fighter->statusFxId, &digimon->entity);
			break;
		case 2:
			STD_removeConfusionEffect(fighter->statusFxId, &digimon->entity);
			break;
		case 3:
			STD_removeStunEffect(fighter->statusFxId, &digimon->entity);
			break;
		}
		fighter->statusFxId = -1;
	}
}

void STD_resetFighterAction(FighterData *fighter)
{
	fighter->cooldown = 0;
	fighter->senileTimer = 0;
	fighter->flags &= ~(FIGHTER_FLAG_ON_CHARGEUP | FIGHTER_FLAG_ON_COOLDOWN | FIGHTER_FLAG_SENILE);
}

void STD_addPoisonStatusVisual(DigimonEntity *digimon, FighterData *fighter)
{
	if (!(fighter->flags & (FIGHTER_FLAG_CONFUSED | FIGHTER_FLAG_STUNNED | FIGHTER_FLAG_FLATTENED)) && (fighter->flatTimer == 0)) {
		STD_addStatusEffectVisual(digimon, fighter, 1);
	}
}

void STD_addConfusionStatusVisual(DigimonEntity *digimon, FighterData *fighter)
{
	if (!(fighter->flags & (FIGHTER_FLAG_STUNNED | FIGHTER_FLAG_FLATTENED)) && (fighter->flatTimer == 0)) {
		if (fighter->flags & FIGHTER_FLAG_POISONED) {
			STD_removeStatusEffectVisual(digimon, fighter, 1);
		}
		STD_addStatusEffectVisual(digimon, fighter, 2);
	}
}

void STD_addStunStatusVisual(DigimonEntity *digimon, FighterData *fighter)
{
	if (!(fighter->flags & FIGHTER_FLAG_FLATTENED) && (fighter->flatTimer == 0)) {
		if (fighter->flags & FIGHTER_FLAG_CONFUSED) {
			STD_removeStatusEffectVisual(digimon, fighter, 2);
		}
		if (fighter->flags & FIGHTER_FLAG_POISONED) {
			STD_removeStatusEffectVisual(digimon, fighter, 1);
		}
		STD_addStatusEffectVisual(digimon, fighter, 3);
	}
}

void STD_removeStatusEffects(DigimonEntity *digimon, FighterData *fighter)
{
	if (fighter->flags & FIGHTER_FLAG_STUNNED) {
		STD_removeStatusEffectVisual(digimon, fighter, 3);
	}

	if (fighter->flags & FIGHTER_FLAG_CONFUSED) {
		STD_removeStatusEffectVisual(digimon, fighter, 2);
	}

	if (fighter->flags & FIGHTER_FLAG_POISONED) {
		STD_removeStatusEffectVisual(digimon, fighter, 1);
	}
}

int32_t STD_getUsableMoves(int16_t *out, int16_t index)
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
	fighter->flags |= FIGHTER_FLAG_ON_CHARGEUP;
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

int32_t STD_selectMoveByPower(int32_t arg0, int16_t *flags)
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

int32_t STD_selectMoveByMpCost(int32_t arg0, int16_t *flags)
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

void STD_renderMoveName(int32_t i)
{
	RECT rect;
	uint8_t cmd;
	int16_t tech;
#if !VERSION_REGION_IS(NTSCJ)
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
		drawString(STD_COMMAND_NAMES[cmd - 1], 0, (i * 12) + 0xd8);
	}

#if VERSION_REGION_IS(NTSCJ)
	renderString(0, (i * 160) - 0x8c, STD_COMMAND_MENU_TOP[i] - 0xe, 0x90, 0xc, 0, (i * 12) + 0xd8, 7, 1);
#else
	renderString(0, (int32_t)(n * 160) - 0x8c, STD_COMMAND_MENU_TOP[i] - 0xe, 0x90, 0xc, 0, (i * 12) + 0xd8, 7, 1);
#endif
}

void STD_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, uint8_t index)
{
	int16_t c;
	int16_t eff;
	int16_t n;

	if ((index >= 8U) && (index < 0xcU)) {
		eff = MOVE_DATA[entityGetTechFromAnim(&digimon->entity, digimon->stats.base.moves[index - 8])].special;
		setUVWH(prim, STD_SPECIAL_ICON_UVS[eff][0], STD_SPECIAL_ICON_UVS[eff][1], 0x10, 0xf);
	} else {
		setUVWH(prim, STD_COMMAND_ICON_UVS[index - 1][0], STD_COMMAND_ICON_UVS[index - 1][1], 0x10, 0xf);
	}
}

void STD_addCommandMenu(uint8_t index)
{
	STD_COMMAND_MENU_TOP[index] = 0x44;
	STD_COMMAND_MENU_BOTTOM[index] = STD_COMMAND_MENU_TOP[index] + 0x20;
	STD_COMMAND_MENU_BLINK[index] = 0;
	STD_COMMAND_MENU_TIMER[index] = 0;

	switch (COMBAT_DATA_PTR->player.numCommands[index]) {
	case 2:
		STD_COMMAND_MENU_LAYOUT[index] = 0;
		break;
	case 3:
		STD_COMMAND_MENU_LAYOUT[index] = 1;
		break;
	case 4:
		STD_COMMAND_MENU_LAYOUT[index] = 2;
		break;
	case 5:
		STD_COMMAND_MENU_LAYOUT[index] = 3;
		break;
	case 6:
		STD_COMMAND_MENU_LAYOUT[index] = 4;
		break;
	case 7:
		STD_COMMAND_MENU_LAYOUT[index] = 5;
		break;
	case 8:
		STD_COMMAND_MENU_LAYOUT[index] = 6;
		break;
	case 9:
		STD_COMMAND_MENU_LAYOUT[index] = 7;
		break;
	}

	MAIN_D_8013518A[index] = 0;
	addObject(0x198, index, (TickFunction)STD_tickCommandMenu, (RenderFunction)STD_renderCommandMenu);
}

void STD_tickCommandMenu(uint8_t i)
{
	STD_COMMAND_MENU_TIMER[i]++;
	if (GAME_STATE != 0) {
		if (GAME_STATE == 4) {
			if ((STD_COMMAND_MENU_TIMER[i] % 8) == 0) {
				STD_COMMAND_MENU_BLINK[i] = (STD_COMMAND_MENU_BLINK[i] + 1) & 1;
			}
		}
	}
}

void STD_removeCommandMenu(int32_t i)
{
	MAIN_D_8013518A[i] = 0;
	removeObject(0x198, i);
}
