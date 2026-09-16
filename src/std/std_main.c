#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libcd.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <mwinline_n.h>

#include <dw/aabb.h>
#include <dw/anim.h>
#include <dw/attack_object.h>
#include <dw/clock.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/font.h>
#include <dw/item.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/world_object.h>

#include "common.h"

typedef struct {
	int16_t x;
	int16_t y;
	uint8_t owner;
	uint8_t flag;
} StdUnkBB64;

typedef struct {
	int16_t score[5];
	int16_t best;
} MoveRanking;

typedef struct {
	int16_t unk0;
	int16_t unk2;
	int16_t unk4;
	int16_t unk6;
	int16_t unk8;
	int16_t unkA;
} StdCameraPreset;

typedef struct {
	int16_t flags[4];
	int16_t enemies[5];
	int16_t count;
} TargetChoice;

typedef struct {
	int16_t unk0;
	int16_t unk2;
	uint8_t unk4;
	uint8_t unk5;
	uint8_t unk6;
	uint8_t unk7;
	uint8_t unk8;
	uint8_t unk9;
	uint8_t unkA[2];
	uint8_t unkC;
	uint8_t unkD;
} StdUnkBAF4;

typedef struct {
	int16_t unk0;
	uint8_t pad2[0xE];
	uint8_t unk10;
	uint8_t pad11[0x7];
} StdSrcA598;

typedef struct {
	int16_t timer;
	int8_t phase;
	int8_t side;
} CameraChase;

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
void addEntityText(DigimonEntity *digimon, int32_t slot, int16_t color, int32_t value, uint8_t flag);
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void STD_removeStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, int32_t arg2);
void STD_addStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, int32_t arg2);
void swapInt(int32_t *a, int32_t *b);
void STD_renderCounterDigits(int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer);
void STD_func_80058898(int32_t i);
void STD_setViewpointRotationFromEntity(void);
void STD_applyViewpoint(void);
void STD_func_8005B688(Entity *a, Entity *b);
void STD_func_8006CCE0(int32_t a);
void STD_func_8006C6DC(void);
void STD_removeFinisherChargeup(void);
void STD_removeFinisherAura(int32_t id);
void STD_setCameraParams(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f);
void STD_setVSPhase(int32_t arg);
void STD_func_8005A550(void);
int32_t STD_func_8005ADFC(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end);
void STD_removeCameraIntro(void);
void STD_addFighterCounter(uint8_t arg);
void STD_renderFighterCounter(void);
void STD_removeFighterCounter(void);
void STD_func_8005D7A8(int32_t i);
void STD_func_8005D7B4(int32_t i);
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
int32_t STD_func_80062BD8(int16_t *out, uint8_t index);
void STD_startWalkingAnimation(Entity *entity, Stats *stats, uint16_t flags);
void STD_clearBlockedAttacks(FighterData *fighter);
void STD_findUnblockedRotation(Entity *entity, int16_t *rot, int16_t hit, int16_t orig);
void STD_startWalkingAnimation2(Entity *entity, Stats *stats, uint16_t flags);
int16_t STD_getAttackTech(AttackObject *attack);
int32_t STD_applyBuffMove(DigimonEntity *digimon, int32_t slot, int32_t anim);
void STD_applyMoveStatus(DigimonEntity *digimon, FighterData *fighter, int32_t move);
int16_t STD_getFighterSlot(int16_t entityId);
int32_t STD_addBlockedAttack(FighterData *fighter, FighterData *other);
void STD_buffStats(DigimonEntity *digimon, int32_t slot, int16_t value, int16_t *stat, int16_t color, uint8_t flag);
void STD_startHitAnimation(Entity *entity, AttackObject *attack, int32_t animId);
void STD_battleTickFrame(void);
int32_t STD_isMoveUsable(DigimonEntity *digimon, FighterData *fighter, int16_t slot);
int32_t STD_getDistanceSquared(Entity *a, Entity *b);
void STD_setupQueuedMove(DigimonEntity *digimon, FighterData *fighter, int16_t arg2, int32_t moveIndex);
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
int32_t STD_calculateElementBonus(int32_t arg0, int32_t arg1);
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
void STD_func_80058E28(int32_t mode);
void STD_func_80059908(void);
void STD_func_8005A1BC(void);
void STD_func_8005A55C(DigimonEntity *entity, int32_t type, int32_t value);
void STD_tickCameraChase(void);
void STD_func_8005C1E4(void);
void STD_func_8005CE9C(void);
void STD_func_8005D398(int16_t i, int32_t owner, int32_t flag);
void STD_func_8005D9F4(uint8_t *out, uint8_t *list);
void STD_func_8005E124(int32_t id);
void STD_func_8005E1E4(int32_t id);
void STD_func_8005EF84(void);
void STD_func_8005F650(void);
void STD_func_8005FDDC(void);
void STD_func_800602A8(void);
void STD_func_80060C14(uint8_t hasLostP1, uint8_t hasLostP2);
void STD_func_80060EBC(void);
void STD_func_80063508(int32_t id);
int16_t STD_applyPartnerStatsToFighter(DigimonEntity *attacker, DigimonEntity *defender, FighterData *fighter, int16_t move);
int32_t STD_calculateDamage(DigimonEntity *attacker, DigimonEntity *defender, int16_t move);
void STD_handleHitReaction(Entity *entity, FighterData *fighter, AttackObject *attack, int16_t index);
void STD_func_800647F8(void);
void STD_applyMoveResult(void);
void STD_func_80067744(DigimonEntity *digimon, FighterData *fighter, int16_t index);
void STD_func_80067A30(DigimonEntity *digimon, FighterData *fighter, int16_t index);
void STD_renderCommandMenu(uint8_t id);
void STD_addCommandMenu(uint8_t index);
int32_t STD_addStunEffect(DigimonEntity *digimon, int16_t arg1);
int32_t STD_addConfusionEffect(DigimonEntity *digimon, FighterData *fighter);
int32_t STD_func_80077664(DigimonEntity *digimon, FighterData *fighter);
void STD_removeStunEffect(int32_t id, DigimonEntity *digimon);
void STD_removeConfusionEffect(int32_t id, DigimonEntity *digimon);
void STD_removePoisonEffect(int32_t id, DigimonEntity *digimon);
int32_t STD_addFinisherAura(Entity *entity, int32_t arg1);
void STD_func_80069134(int16_t tech);
void STD_func_800658B4(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int32_t STD_selectMoveTarget(Entity *entity, FighterData *fighter);
void STD_func_8005D538(int32_t i);
void STD_func_8005DF6C(void);
void STD_func_800588A4(int32_t i);
void STD_func_80060AA0(void);
int32_t STD_func_80061124(int32_t value);
void STD_func_80062D14(void);
int16_t STD_func_80058634(Entity *entity);
int16_t STD_func_80062D5C(Entity *entity);
void STD_func_800615D8(DigimonEntity *digimon, FighterData *fighter);
void STD_func_8005DF94(int32_t mode);
int32_t playMusic(int32_t font, int32_t track);
void STD_func_8005DEEC(int32_t track);
void STD_func_80058494(int32_t which);
void STD_func_8005E5E0(void);
void STD_func_80058504(int32_t which);
void STD_func_8005E660(void);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uvX, int32_t uvY, int32_t width, int32_t height);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void STD_func_8005E6E4(void);
void STD_func_8006324C(void);
void STD_func_80058958(int32_t idx, int32_t value);
void STD_func_8005E004(int32_t i);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount, int32_t *digits);
int32_t getDistance(int32_t x, int32_t y, int32_t z);
int32_t STD_getFighterDistance(VECTOR *self, VECTOR *other, VECTOR *target);
int32_t STD_func_80060B98(void);
void STD_tickFighterCounter(void);
void STD_applyEntityViewpoint(void);
void STD_updateFighterStatusVisuals(DigimonEntity *digimon, FighterData *fighter);
void handlePause(void);
void MAIN_func_800E642C(void);
int16_t STD_func_80060620(int32_t a, int32_t b);
int32_t STD_func_8005F354(void);
void STD_func_8005E8A4(Entity *entity, Entity *other);
int16_t STD_func_8006314C(Entity *entity, int32_t arg);
void STD_func_800587F0(Entity *entity);
void STD_func_8005D964(void);
int16_t STD_getNearestEnemy(Entity *self, int16_t *flags);
void STD_func_800588D4(Entity *entity, int32_t id);
void STD_func_80060998(void);
void STD_selectConfusedMove(DigimonEntity *digimon, FighterData *fighter, int32_t tech);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void STD_func_80063300(int32_t id);
void MAIN_func_80092BB0(POLY_GT4 *prim);
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
void VS__func_800F1E9C(Entity *entity, int32_t id);
void BTL_clearConfusion(DigimonEntity *digimon, FighterData *fighter);
void STD_func_8005A44C(void);
void STD_func_80064FCC(unsigned short count);
int16_t BTL_calculateHitChance(DigimonEntity *attacker, DigimonEntity *defender, FighterData *fighter, int16_t move);
void BTL_retargetAfterHit(DigimonEntity *digimon, FighterData *fighter, AttackObject attack);
void BTL_startQueuedMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int32_t STD_isPositionNearEntity(Entity *entity, VECTOR *pos);
int32_t BTL_isMoveOnCooldown(Entity *entity, FighterData *fighter);
void BTL_setupMoveExecution(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void STD_func_8005D7C0(POLY_FT4 *prim, int32_t a, int32_t b, int32_t h);
void VS__func_800F4CB4(uint8_t hasLostP1, uint8_t hasLostP2);
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
void STD_func_8005D814(int32_t x, int32_t y, int32_t n, int32_t size);
int32_t _atan(int32_t y, int32_t x);
int16_t STD_getMostEffectiveMove(int32_t index, int16_t *flags);
int32_t lerp(int32_t a, int32_t b, int32_t lo, int32_t hi, int32_t t);
void STD_updateCameraLerp(int32_t t, int32_t flip);
int32_t STD_func_8005ADFC(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e);
int32_t MAIN_func_800DA63C(int32_t a, int32_t b, int16_t *c, int16_t *d, int16_t *e);
void STD_tickCameraIntro(void);
void STD_startCameraChase(Entity *entity, int32_t dx, int32_t side);
void STD_setCameraToEntity(void);
void STD_func_8005A054(void);
void swapShort(int16_t *a, int16_t *b);
void STD_func_8005D550(int16_t i);
void STD_func_80068388(int32_t i);
void STD_func_80059B70(void);
void STD_removeAllStunEffects(void);
void STD_func_800791E0(void);
void STD_removeAllPoisonEffects(void);
void STD_func_80079874(void);
void STD_unloadAllEFESlots(void);
void STD_removeEFEEngine(void);
int32_t STD_renderCommandMenu__garbage__(int32_t i);
int32_t loadSB(void);
int32_t customRandom(int32_t lo, int32_t hi);
void MAIN_func_800DA60C(int32_t n, int16_t *a, int16_t *b, int16_t *c);
void STD_func_8005D814(int32_t x, int32_t y, int32_t n, int32_t size);
void swapByte(uint8_t *a, uint8_t *b);
void STD_func_8006AD00(int32_t id);
void resetFlattenGlobal(void);
void initializeAttackObjects(void);
int32_t STD_func_80060B98(void);
void STD_func_8005A55C(DigimonEntity *entity, int32_t type, int32_t value);
void STD_selectFighterTarget(DigimonEntity *digimon, FighterData *fighter, uint8_t target, int32_t arg3);
void removeEntityText(int32_t id);
void STD_func_8006B1E4(int32_t i);
int32_t STD_startEFE(int32_t i);

extern StdUnkBB64 STD_D_8007BB64[8];
extern StdCameraPreset STD_D_8007A390[];
extern StdUnkBAF4 STD_D_8007BAF4[8];
extern StdSrcA598 STD_D_8007A598[8];
extern int16_t ENEMY_COUNT;
extern int16_t MAIN_D_80134D66;
extern Entity *MAIN_D_80134D60;
extern int32_t MAIN_D_80134D74;
extern int32_t MAIN_D_80135118;
extern int32_t MAIN_D_80135174;
extern uint8_t *GENERAL_BUFFER_PTR;
extern int16_t MAIN_D_8013511C;
extern int8_t GAME_STATE;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern int32_t MAIN_D_80135160;
extern uint8_t MAIN_D_80125F70[][7];
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[];
extern PACKET GS_WORK_BASES[];
extern char DRAW_OFFSETS[];
extern int16_t MAIN_D_80134870[4];
extern uint8_t STD_D_8007A6F8[];
extern uint8_t STD_D_8007A6F9[];
extern uint8_t STD_D_8007A708[];
extern uint8_t STD_D_8007A709[];
extern uint8_t MAIN_D_80135184[2];
extern uint8_t MAIN_D_80135186[2];
extern int8_t MAIN_D_8013518A[2];
extern int32_t MAIN_D_801350EC;
extern char **MAIN_D_80135128;
extern uint8_t MAIN_D_8013512C;
extern SVECTOR MAIN_D_801B1C0C[];
extern VECTOR MAIN_D_801B1C14;
extern GsRVIEW2 GS_VIEWPOINT;
extern int16_t STD_D_8007B9C8[];
extern uint8_t MAIN_D_80135110;
extern int32_t MAIN_D_80135148;
extern int32_t VIEWPORT_DISTANCE;
extern int16_t MAIN_D_8013517C[2];
extern int32_t MAIN_D_80135178;
extern int32_t MAIN_D_80135134;
extern uint8_t MAIN_D_80135164;
extern uint8_t MAIN_D_80135165;
extern uint8_t MAIN_D_80135166;
extern uint8_t MAIN_D_80135167;
extern Entity *MAIN_D_801350E8;
extern uint8_t CURRENT_SCREEN;
extern int16_t STD_D_8007A364[];
extern int16_t STD_D_8007B9BC[];
extern int16_t STD_D_8007B9BE[];
extern int16_t STD_D_8007B9C0[];
extern int16_t STD_D_8007B9C2[];
extern int16_t STD_D_8007B9C4[];
extern int16_t STD_D_8007B9C6[];
extern uint8_t MAIN_D_8013514C;
extern int16_t STD_D_8007A48C[8];
extern char MAIN_D_80134808[8];
extern char MAIN_D_80134810[8];
extern char MAIN_D_80124C24[];
extern char MAIN_D_80124C54[];
extern char MAIN_D_80134878[8];
extern char STD_D_8007A658[];
extern int16_t MAIN_D_80135144;
extern VECTOR STD_D_8007B6F4;
extern int32_t MAIN_D_80135114;
extern int32_t COMBAT_AREA_CENTER_X;
extern int32_t COMBAT_AREA_CENTER_Y;
extern uint8_t MAIN_D_801350F8;
extern GsVIEW2 MAIN_D_801B1B98;
extern int32_t MAIN_D_801B1BBC[];
extern uint8_t STD_D_8007A370[];
extern uint8_t STD_D_8007A371[];
extern uint8_t STD_D_8007A372[];
extern uint8_t MAIN_D_8013513C;
extern int16_t MAIN_D_80135138;
extern int16_t MAIN_D_8013513A;
extern int16_t MAIN_D_8013513E;
extern int16_t MAIN_D_80135140;
extern uint8_t MAIN_D_80135142;
extern uint8_t STD_D_80079CBC[][14];
extern uint8_t MAIN_D_80134800[4];
extern uint8_t MAIN_D_80134804[4];
extern int16_t MAIN_D_80135100;
extern int16_t MAIN_D_80135102;
extern int32_t MAIN_D_80135124;
extern int32_t MAIN_D_80135120;
extern int32_t MAIN_D_80134D7C[2];
extern int32_t MAIN_D_80134D84;
extern uint8_t MAIN_D_80135172;
extern int32_t MAIN_D_8013516C;
extern SVECTOR MAIN_D_80134868;
extern int16_t STD_D_8007A3FC[];
extern int16_t STD_D_8007A3FE[];
extern int16_t STD_D_8007A400[];
extern SVECTOR MAIN_D_80134838;
extern SVECTOR MAIN_D_80134840;
extern SVECTOR MAIN_D_80134858;
extern SVECTOR MAIN_D_80134860;
extern int16_t STD_D_8007B9CA[];
extern int16_t STD_D_8007B9CC[];
extern int16_t STD_D_8007B9CE[];
extern Entity *STD_D_8007B9D4[];
extern int16_t STD_D_8007A41C[];
extern int16_t STD_D_8007B9EC[];
extern int32_t STD_D_8007B9F0[];
extern int32_t STD_D_8007B9F4[];
extern int32_t STD_D_8007B9F8[];
extern int32_t STD_D_8007B9FC[];
extern int16_t STD_D_8007BA44[];
extern int16_t STD_D_8007BA9C[];
extern int32_t STD_D_8007BAA0[];
extern int32_t STD_D_8007BAA4[];
extern int32_t STD_D_8007BAA8[];
extern int32_t STD_D_8007BAAC[];
extern char *MOVE_NAMES[];
extern char *STD_D_8007A688[];
extern uint8_t MAIN_D_80134888[4];
extern uint8_t MAIN_D_8013488C[4];
extern uint8_t MAIN_D_80134890[4];
extern int16_t MAIN_D_801350FC;
extern int16_t MAIN_D_801350FE;
extern int16_t MAIN_D_80135180[2];
extern uint8_t MAIN_D_80135188[2];
extern uint8_t MAIN_D_801348B0[8];
extern uint8_t MAIN_D_801348B8[8];
extern uint8_t STD_D_8007A6A8[][10];
extern int32_t MAIN_D_80135108;
extern int16_t MAIN_D_8013511C;
extern int32_t MAIN_D_8013510C;
extern MATRIX MAIN_D_801B1BC0;
extern VECTOR STD_D_8007B704;
extern SVECTOR MAIN_D_80134818;
extern SVECTOR MAIN_D_80134820;
extern VECTOR STD_D_8007B704;
extern int8_t MAIN_D_80135147;
extern SVECTOR MAIN_D_80134828;
extern SVECTOR MAIN_D_80134830;
extern int16_t STD_D_8007B9D0[];
extern int16_t STD_D_8007B9D2[];
extern Entity *STD_D_8007B9D8[];
extern SVECTOR MAIN_D_80134848;
extern SVECTOR MAIN_D_80134850;
extern uint8_t MAIN_D_8013514D;
extern uint8_t MAIN_D_80135158[4];
extern uint8_t MAIN_D_8013515C;
extern uint8_t MAIN_D_8013515D;
extern int8_t STD_D_8007A474[];
extern int16_t MAIN_D_801350E4;
extern uint8_t MAIN_D_80135150[8];
extern uint8_t MAIN_D_80135150[8];
extern uint8_t STD_D_8007A49C[];
extern uint8_t MAIN_D_8013514D;
extern uint8_t MAIN_D_80135150[8];
extern uint8_t MAIN_D_80135151[7];
extern uint8_t MAIN_D_80135158[4];
extern uint8_t MAIN_D_80135159;
extern uint8_t MAIN_D_8013515A;
extern uint8_t MAIN_D_8013515B;
extern uint8_t MAIN_D_8013515C;
extern uint8_t MAIN_D_8013515D;
extern uint8_t STD_D_8007A58C[][3];
extern uint8_t MAIN_D_80135150[8];
extern uint8_t STD_D_8007A50C[];
extern uint8_t STD_D_8007A57C[];
extern int16_t MAIN_D_80135170;
extern uint8_t MAIN_D_80135168;
extern int32_t MAIN_D_80135104;
extern int8_t MAIN_D_80134D64;
extern uint8_t MAIN_D_80134880[5];
extern int16_t INITIAL_COMBAT_STATS[][6];
extern int32_t MAIN_D_80135118;
extern uint8_t MAIN_D_80135168;
extern uint8_t MAIN_D_80135168;
extern int16_t MAIN_D_80135170;
extern int32_t MAIN_D_80135104;
extern int16_t INITIAL_COMBAT_STATS[][6];
extern DigimonEntity *MAIN_D_80134EF4;
extern DigimonEntity *MAIN_D_80134EF8;
extern int16_t MAIN_D_80135180[2];
extern uint8_t MAIN_D_80135188[2];
extern int16_t MAIN_D_80135180[2];
extern uint8_t MAIN_D_80135188[2];

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

void STD_func_80058494(int32_t which)
{
	int16_t x;

	if (which == 1) {
		x = 0x708;
	} else {
		x = 0x9C4;
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

void STD_func_80058504(int32_t which)
{
	int16_t x;
	int16_t id;

	if (which == 1) {
		x = -0x708;
	} else {
		x = -0x9C4;
	}
	id = STD_func_80058634(MAIN_D_801350E8);
	setEntityPosition(id, x, 0, 0);
	setEntityRotation(id, 0, 0xC00, 0);
	startAnimation(MAIN_D_801350E8, 0x21);
}

void STD_func_8005858C(void)
{
	int32_t i;
	int32_t y;
	char *text;

	clearTextArea();
	drawString(MAIN_D_80134808, 0, 0);
	drawString(MAIN_D_80134810, 0, 12);

	for (i = 2, y = 24, text = MAIN_D_80124C24; i < 6; ++i, text += 12, y += 12) {
		drawString(text, 0, y);
		DrawSync(0);
	}

	drawString(MAIN_D_80124C54, 0, 0xf0);
}

void STD_func_80058684(Entity *entity, int32_t id)
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
	if (MAIN_D_80135144 != -1) {
		entity->posData->location = STD_D_8007B6F4;
		entity->anim.locX = STD_D_8007B6F4.vx << 15;
		entity->anim.locY = STD_D_8007B6F4.vy << 15;
		entity->anim.locZ = STD_D_8007B6F4.vz << 15;
		startAnimation(entity, 0x21);
		MAIN_D_80135144 = -1;
	}
}

void STD_func_80058898(int32_t i)
{
	removeObject(0x1AB, i);
}

void STD_func_800588A4(int32_t i)
{
	if (MAIN_D_80135134 != 0) {
		MAIN_D_80135134 = 0;
		removeObject(0x1A9, i);
	}
}

void STD_func_800588D4(Entity *entity, int32_t id)
{
	if (MAIN_D_80135134 != 1) {
		MAIN_D_80135134 = 1;
		STD_D_8007B9BC[0] = -100;
		STD_D_8007B9BE[0] = -100;
		STD_D_8007B9C0[0] = -10;
		STD_D_8007B9C2[0] = -10;
		STD_D_8007B9C4[0] = -10;
		STD_D_8007B9C6[0] = -10;
		addObject(0x1a9, id, STD_func_80059080, STD_func_80059204);
	}
}

void STD_func_80058958(int32_t idx, int32_t value)
{
	POLY_F4 *prim;
	int16_t width;

	prim = (POLY_F4 *)GsGetWorkBase();
	SetPolyF4(prim);
	setRGB0(prim, 0x50, 0xC8, 0x50);
	width = (value * 100) / STD_D_8007A364[idx];
	if (width == 0) {
		width = 1;
	}
	setXYWH(prim, -0x32, idx * 16 - 0x1A, width, 8);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[3], prim++);
	GsSetWorkBase((PACKET *)prim);
}

int32_t STD_renderCommandMenu__garbage__(int32_t i)
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

	v0 = i + 0;
	v1 = i + 1;
	v2 = i + 2;
	v3 = i + 3;
	v4 = i + 4;
	v5 = i + 5;
	v6 = i + 6;
	v7 = i + 7;
	v8 = i + 8;
	v9 = i + 9;
	v10 = i + 10;
	v11 = i + 11;
	v12 = i + 12;
	v13 = i + 13;
	v14 = i + 14;
	v15 = i + 15;
	v16 = i + 16;
	return v0 + v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v10 + v11 + v12 + v13 + v14 + v15 + v16;
}

void STD_renderCommandMenu(uint8_t id)
{
	POLY_FT4 *prim;
	int32_t i;
	int32_t off;
	int16_t base;
	int16_t count;
	int16_t x;
	int32_t y;
	int16_t rowY;
	int16_t width;

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
		prim->clut = GetClut(0x11a, 0x1f1);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVDataPolyFT4(prim, 0x3d, 0xe0, 0x16, 0x16);
		if ((count % 2) == 0) {
			if ((COMBAT_DATA_PTR->player.hoveredCommand[id] % 2) == 0) {
				y = MAIN_D_8013517C[id];
			} else {
				y = MAIN_D_8013517C[id] + 0xa;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		} else {
			if ((COMBAT_DATA_PTR->player.hoveredCommand[id] % 2) == 1) {
				y = MAIN_D_8013517C[id];
			} else {
				y = MAIN_D_8013517C[id] + 0xa;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	for (i = 1, off = 0xe; i < COMBAT_DATA_PTR->player.numCommands[id]; i++, off += 0xe) {
		SetPolyFT4(prim);
		prim->tpage = getTPage(0, 0, 960, 256);
		prim->clut = GetClut(0x110, 0x1f0);
		setRGB0(prim, 0x80, 0x80, 0x80);
		STD_setCommandIconUV((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]], prim, COMBAT_DATA_PTR->player.availableCommands[id][i]);
		x = base - off;
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
		prim->tpage = GetTPage(0, 0, 0x3c0, 0x100);
		prim->clut = GetClut(0x110, 0x1f1);
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

	MAIN_func_80092BB0(prim);
	prim->tpage = getTPage(0, 0, 768, 0);
	prim->clut = GetClut(0, 0x1e0);
	setRGB0(prim, STD_D_8007A370[MAIN_D_8013513C * 3], STD_D_8007A371[MAIN_D_8013513C * 3], STD_D_8007A372[MAIN_D_8013513C * 3]);
	setRGB1(prim, STD_D_8007A370[MAIN_D_8013513C * 3], STD_D_8007A371[MAIN_D_8013513C * 3], STD_D_8007A372[MAIN_D_8013513C * 3]);
	setRGB2(prim, STD_D_8007A370[MAIN_D_8013513C * 3] / 10, STD_D_8007A371[MAIN_D_8013513C * 3] / 10, STD_D_8007A372[MAIN_D_8013513C * 3] / 10);
	setRGB3(prim, STD_D_8007A370[MAIN_D_8013513C * 3] / 10, STD_D_8007A371[MAIN_D_8013513C * 3] / 10, STD_D_8007A372[MAIN_D_8013513C * 3] / 10);

	u = (character % 32) * 32;
	v = (character / 8) * 32;

	if (size < 64) {
		setUVWH(prim, u, v, (u != 0xe0 ? 32 : 31), (v != 0xe0 ? 32 : 31));
	} else {
		setUVWH(prim, u, v, 31, 31);
	}

	setXYWH(prim, x, y, size, size);

	GsSetWorkBase((PACKET *)prim);
}

void STD_func_80058E28(int32_t which)
{
	int32_t dist;
	int32_t id;
	int16_t idn;
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
	idn = STD_func_80058634(ENTITY_TABLE[1]);
	id = idn;
	STD_func_80058684(ENTITY_TABLE[1], id);
	stopBGM();
	stopSound();
	playMusic(MAIN_D_801350F8, 0);

	prev = 0;
	while ((ENTITY_TABLE[1]->anim.animFlag & 1) != 0) {
		pad = PadRead(1);
		STD_battleTickFrame();
		if (((pad & ~prev) & 0x40) != 0) {
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
	idn = STD_func_80058634(MAIN_D_801350E8);
	id = idn;
	STD_func_80058684(MAIN_D_801350E8, id);
	playMusic(MAIN_D_801350F8, 1);

	while ((MAIN_D_801350E8->anim.animFlag & 1) != 0) {
		pad = PadRead(1);
		STD_battleTickFrame();
		if (((pad & ~prev) & 0x40) != 0) {
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

void STD_func_80059080(int32_t id)
{
	Stats *stats;

	STD_D_8007B9BC[0] += 200;
	STD_D_8007B9BE[0] += 200;
	STD_D_8007B9C0[0] += 20;
	STD_D_8007B9C2[0] += 20;
	STD_D_8007B9C4[0] += 20;
	STD_D_8007B9C6[0] += 20;

	stats = &((DigimonEntity *)ENTITY_TABLE[id])->stats;
	if (stats->current.currentHP < STD_D_8007B9BC[0]) {
		STD_D_8007B9BC[0] = stats->current.currentHP;
	}

	if (stats->current.currentMP < STD_D_8007B9BE[0]) {
		STD_D_8007B9BE[0] = stats->current.currentMP;
	}

	if (stats->base.off < STD_D_8007B9C0[0]) {
		STD_D_8007B9C0[0] = stats->base.off;
	}

	if (stats->base.def < STD_D_8007B9C2[0]) {
		STD_D_8007B9C2[0] = stats->base.def;
	}

	if (stats->base.speed < STD_D_8007B9C4[0]) {
		STD_D_8007B9C4[0] = stats->base.speed;
	}

	if (stats->base.brain < STD_D_8007B9C6[0]) {
		STD_D_8007B9C6[0] = stats->base.brain;
	}
}

void STD_func_80059204(int32_t id)
{
	Stats *stats;
	int32_t i;

	for (i = 0; i < 6; ++i) {
		renderString(0, -100, i * 16 - 28, 48, 12, 0, i * 12, 0, 1);
		STD_func_80058958((int16_t)i, STD_D_8007B9BC[i]);
	}

	stats = &((DigimonEntity *)ENTITY_TABLE[id])->stats;
	if (STD_D_8007B9BC[0] != stats->current.currentHP || STD_D_8007B9BE[0] != stats->current.currentMP || STD_D_8007B9C0[0] != stats->base.off || STD_D_8007B9C2[0] != stats->base.def || STD_D_8007B9C4[0] != stats->base.speed || STD_D_8007B9C6[0] != stats->base.brain) {
		playSound(0, 0x16);
	} else {
		for (i = 0; i < 6; ++i) {
			STD_func_800593D0(52, (int16_t)(i * 16 - 28), 4, STD_D_8007B9BC[i], 3);
		}
	}
}

void STD_func_800593D0(int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer)
{
	POLY_FT4 *prim;
	int32_t i;
	uint32_t width;
	int32_t count;
	int32_t buf[6];

	prim = (POLY_FT4 *)GsGetWorkBase();

	width = digits;
	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 832, 0);
		prim->clut = GetClut(16, 480);
		setUVDataPolyFT4(prim, buf[i] * 12, 32, 12, 12);
		setPosDataPolyFT4(prim, x + (((int32_t)width - 1) - i) * 12, y, 12, 12);
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
		if (MAIN_D_80135144 == 0) {
			startAnimation(ENTITY_TABLE[id], 0x23);
			MAIN_D_80135144 = 20;
		}

		if (MAIN_D_80135140 >= -71) {
			MAIN_D_80135140 -= 28;
		} else {
			STD_func_800588D4(ENTITY_TABLE[id], id);
		}
	}
}

void STD_func_80059658(int32_t id)
{
	int32_t charCount;
	uint32_t entityIndex;
	int32_t charIndex;
	int32_t i;
	int16_t y;
	int16_t size;
	uint8_t character;

	entityIndex = id;
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
		character = STD_D_80079CBC[ENTITY_TABLE[entityIndex]->type][charIndex++];
		if (character == 0x3d) {
			character = STD_D_80079CBC[ENTITY_TABLE[entityIndex]->type][charIndex++];
		}

		if (i == MAIN_D_8013513A - 1) {
			y = MAIN_D_80135140 - MAIN_D_80134804[MAIN_D_80135142];
			size = MAIN_D_80134800[MAIN_D_80135142];
		} else {
			size = 32;
			y = MAIN_D_80135140;
		}

		STD_func_80058A60((int16_t)(MAIN_D_8013513E + i * 32), y, size, character);

		if (character == 0x1f || character == 0x25) {
			character = STD_D_80079CBC[ENTITY_TABLE[entityIndex]->type][charIndex++];
			STD_func_80058A60((int16_t)(MAIN_D_8013513E + i * 32), y, size, character);
		}
	}
}

void STD_func_80059908(void)
{
	int32_t pad;

	if ((pad = MAIN_D_80135124) & 0x1000) {
		MAIN_D_801B1C14.vy += 0x14;
	}
	if (pad & 0x4000) {
		MAIN_D_801B1C14.vy -= 0x14;
	}
	if (pad & 0x8000) {
		MAIN_D_801B1C14.vx += 0x14;
	}
	if (pad & 0x2000) {
		MAIN_D_801B1C14.vx -= 0x14;
	}
	if (pad & 0x4) {
		MAIN_D_801B1C14.vz -= 0x14;
	}
	if (pad & 0x1) {
		MAIN_D_801B1C14.vz += 0x14;
	}
	if (pad & 0x20) {
		MAIN_D_801B1C0C[0].vx += 0x20;
		MAIN_D_801B1C0C[0].vx &= 0xfff;
	}
	if (pad & 0x10) {
		MAIN_D_801B1C0C[0].vx -= 0x20;
		MAIN_D_801B1C0C[0].vx &= 0xfff;
	}
	if (pad & 0x80) {
		MAIN_D_801B1C0C[0].vy -= 0x20;
		MAIN_D_801B1C0C[0].vy &= 0xfff;
	}
	if (pad & 0x40) {
		MAIN_D_801B1C0C[0].vy += 0x20;
		MAIN_D_801B1C0C[0].vy &= 0xfff;
	}
	if (pad & 0x800) {
		MAIN_D_801B1C0C[0].vx = 0;
		MAIN_D_801B1C0C[0].vy = 0;
		MAIN_D_801B1C0C[0].vz = 0;
		MAIN_D_801B1C14.vx = 0;
		MAIN_D_801B1C14.vy = 0;
		MAIN_D_801B1C14.vz = 0xbb8;
	}
	MAIN_D_801B1B98.super = NULL;
	RotMatrix(MAIN_D_801B1C0C, &MAIN_D_801B1B98.view);
	TransMatrix(&MAIN_D_801B1B98.view, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void STD_func_80059B70(void)
{
	VECTOR *a;
	VECTOR *b;
	int32_t dx;
	int32_t dz;
	int32_t dist;
	int32_t d;
	int32_t ang;
	MATRIX *m;
	int32_t limA;
	int32_t limB;
	int32_t limC;

	a = &MAIN_D_801350E8->posData->location;
	b = &PARTNER_ENTITY.digimonEntity.entity.posData->location;
	dx = a->vx - b->vx;
	dz = a->vz - b->vz;
	dist = SquareRoot0(dx * dx + dz * dz);
	d = (dist * VIEWPORT_DISTANCE) / 200u;
	limA = MAIN_D_8013511C;
	limB = limB = limA;
	limC = limC = limA;
	if (d < limA) {
		d = limB;
	}
	if (d < limC + 0x12c) {
		MAIN_D_8013510C = 1;
	} else {
		MAIN_D_8013510C = 0;
	}
	if (d > 0x1068) {
		d = 0x1068;
	}
	ang = 0x1000 - _atan(dx, dz);
	MAIN_D_801B1C14.vx = b->vx + dx / 2 + (d * dz) / dist;
	MAIN_D_801B1C14.vy = -0x3e8;
	MAIN_D_801B1C14.vz = b->vz + dz / 2 - (d * dx) / dist;
	MAIN_D_801B1C0C[0].vx = _atan(d, -0x2bc) + 0x800;
	MAIN_D_801B1C0C[0].vy = ang + 0x800;
	MAIN_D_801B1C0C[0].vz = 0;
	MAIN_D_801B1B98.view = GsIDMATRIX;
	MAIN_D_801B1B98.super = (GsCOORDINATE2 *)MAIN_D_801B1BBC;
	RotMatrixYXZ(MAIN_D_801B1C0C, m = m = &MAIN_D_801B1BC0);
	TransMatrix(m, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void STD_func_80059DBC(void)
{
	VECTOR *a;
	VECTOR *b;
	MATRIX *m;

	a = &PARTNER_ENTITY.digimonEntity.entity.posData->location;
	b = &MAIN_D_801350E8->posData->location;
	MAIN_D_801B1B98.view = GsIDMATRIX;
	MAIN_D_801B1B98.super = (GsCOORDINATE2 *)MAIN_D_801B1BBC;
	MAIN_D_801B1C14.vx = a->vx + (b->vx - a->vx) / 2;
	MAIN_D_801B1C14.vz = a->vz + (b->vz - a->vz) / 2;
	MAIN_D_801B1C14.vy = -0x1f40;
	MAIN_D_801B1C0C[0].vz = 0;
	MAIN_D_801B1C0C[0].vy = 0;
	MAIN_D_801B1C0C[0].vx = -0x400;
	RotMatrixYXZ(MAIN_D_801B1C0C, m = m = &MAIN_D_801B1BC0);
	TransMatrix(m, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void STD_setCameraToEntity(void)
{
	SVECTOR rot;
	VECTOR v;
	VECTOR out;

	rot = MAIN_D_801B1C0C[0];
	rot.vy -= ((Entity *)MAIN_D_80135128)->posData->rotation.vy;
	rot.vy &= 0xfff;
	RotMatrix(&rot, &MAIN_D_801B1B98.view);
	v = MAIN_D_801B1C14;
	ApplyMatrixLV(&MAIN_D_801B1B98.view, &((Entity *)MAIN_D_80135128)->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix(&MAIN_D_801B1B98.view, &v);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
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
	Entity *self;
	Entity *other;
	VECTOR *selfPos;
	VECTOR *otherPos;
	int32_t dx;
	int32_t dz;

	self = (Entity *)MAIN_D_80135128;
	selfPos = &self->posData->location;
	if (self == ENTITY_TABLE[1]) {
		other = MAIN_D_801350E8;
	} else {
		other = ENTITY_TABLE[1];
	}
	otherPos = &other->posData->location;
	dx = otherPos->vx - selfPos->vx;
	do {
	} while (0);
	dz = otherPos->vz - selfPos->vz;
	MAIN_D_801B1C0C[0].vy = (-_atan(dz, dx) + 0x800) & 0xfff;
	RotMatrix(MAIN_D_801B1C0C, &MAIN_D_801B1B98.view);
	v = MAIN_D_801B1C14;
	ApplyMatrixLV(&MAIN_D_801B1B98.view,
	              &((Entity *)MAIN_D_80135128)->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix(&MAIN_D_801B1B98.view, &v);
	GsSetView2(&MAIN_D_801B1B98);
}

void STD_applyViewpoint(void)
{
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_func_8005A1BC(void)
{
	int32_t pad;

	if ((pad = MAIN_D_80135124) & 0x1000) {
		MAIN_D_801B1C14.vy += 0x14;
	}
	if (pad & 0x4000) {
		MAIN_D_801B1C14.vy -= 0x14;
	}
	if (pad & 0x8000) {
		MAIN_D_801B1C14.vx += 0x14;
	}
	if (pad & 0x2000) {
		MAIN_D_801B1C14.vx -= 0x14;
	}
	if (pad & 0x4) {
		MAIN_D_801B1C14.vz -= 0x14;
	}
	if (pad & 0x1) {
		MAIN_D_801B1C14.vz += 0x14;
	}
	if (pad & 0x20) {
		MAIN_D_801B1C0C[0].vx += 0x20;
		MAIN_D_801B1C0C[0].vx &= 0xfff;
	}
	if (pad & 0x10) {
		MAIN_D_801B1C0C[0].vx -= 0x20;
		MAIN_D_801B1C0C[0].vx &= 0xfff;
	}
	MAIN_D_801B1C0C[0].vy += 2;
	MAIN_D_801B1C0C[0].vy &= 0xfff;
	if (pad & 0x40) {
		MAIN_D_801B1C0C[0].vy += 0x20;
		MAIN_D_801B1C0C[0].vy &= 0xfff;
	}
	MAIN_D_801B1B98.super = NULL;
	RotMatrix(MAIN_D_801B1C0C, &MAIN_D_801B1B98.view);
	TransMatrix(&MAIN_D_801B1B98.view, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void STD_setCameraParams(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f)
{
	MAIN_D_801B1C0C[0].vx = a;
	MAIN_D_801B1C0C[0].vy = b;
	MAIN_D_801B1C0C[0].vz = c;
	MAIN_D_801B1C14.vx = d;
	MAIN_D_801B1C14.vy = e;
	MAIN_D_801B1C14.vz = f;
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

void STD_func_8005A55C(DigimonEntity *entity, int32_t mode, int32_t sub)
{
	StdCameraPreset *p;

	if (mode != 5) {
		if (random(3) != 0) {
			return;
		}
	}
	MAIN_D_801B1B98.super = NULL;
	MAIN_D_80135128 = (char **)entity;
	if (sub != 3) {
		p = &STD_D_8007A390[mode];
	} else {
		p = &STD_D_8007A390[random(3) + 6];
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

	if (random(3) != 0) {
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
		RotMatrix(&entity->posData->rotation, &m);
		v.vx = STD_D_8007A3FC[idx * 3];
		v.vy = STD_D_8007A3FE[idx * 3];
		v.vz = STD_D_8007A400[idx * 3];
		ApplyMatrixLV(&m, &v, &out);
		out.vx += ((Entity *)MAIN_D_80135128)->posData->location.vx;
		out.vz += ((Entity *)MAIN_D_80135128)->posData->location.vz;
		GS_VIEWPOINT.vpx = out.vx;
		GS_VIEWPOINT.vpy = out.vy;
		GS_VIEWPOINT.vpz = out.vz;
	} else {
		MAIN_D_801350EC = 6;
		GS_VIEWPOINT.vpx = STD_D_8007A3FC[idx * 3];
		GS_VIEWPOINT.vpy = STD_D_8007A3FE[idx * 3];
		GS_VIEWPOINT.vpz = STD_D_8007A400[idx * 3];
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
	GsSetProjection(0x200);
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

void STD_updateCameraLerp(int32_t t, int32_t flip)
{
	SVECTOR off;
	SVECTOR rot;
	int32_t base;
	int32_t dbl;
	int32_t dist;

	off = MAIN_D_80134858;
	rot = MAIN_D_80134860;
	base = ((((DIGIMON_DATA[((Entity *)MAIN_D_80135128)->type].height + DIGIMON_DATA[((Entity *)MAIN_D_80135128)->type].radius) / 2) * 0x62c) / 450);
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
	if (pos->vx + 50 < entity->posData->location.vx) {
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
	SVECTOR off;
	SVECTOR rot;
	CameraChase *cc;
	int32_t dist;
	int32_t d2;
	int32_t slot;
	int32_t i;

	cc = (CameraChase *)&MAIN_D_80135144;
	if (MAIN_D_80135144 < 0x14) {
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
	dist = STD_getFighterDistance(&STD_D_8007B704, &STD_D_8007B6F4, &((Entity *)MAIN_D_80135128)->posData->location);
	STD_updateCameraLerp(dist, cc->side);
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

void STD_startCameraChase(Entity *entity, int32_t dx, int32_t side)
{
	SVECTOR off;
	SVECTOR rot;
	int32_t dist;

	MAIN_D_80135128 = (char **)entity;
	copyVector(&STD_D_8007B704, &entity->posData->location);
	STD_D_8007B6F4.vx = STD_D_8007B704.vx - dx;
	STD_D_8007B6F4.vy = STD_D_8007B704.vy;
	STD_D_8007B6F4.vz = STD_D_8007B704.vz;
	startAnimation(entity, 0x21);
	MAIN_D_801350EC = 9;
	((CameraChase *)&MAIN_D_80135144)->timer = 0;
	((CameraChase *)&MAIN_D_80135144)->phase = 0;
	((CameraChase *)&MAIN_D_80135144)->side = side;
	addObject(0x1aa, 0, (TickFunction)STD_tickCameraChase, NULL);
	off = MAIN_D_80134828;
	rot = MAIN_D_80134830;
	if (MAIN_D_80135147 == 0) {
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

	p = STD_D_8007B9C8;
	if ((p[0] >= 0x1e) && (p[0] < 0x3c)) {
		STD_D_8007B9CC[0] = lerp(STD_D_8007B9CC[0], STD_D_8007B9CE[0], p[0], 0x3c, p[0] + 1);
	}

	if (p[0] >= 0x1e) {
		p[1] += (int16_t)STD_func_8005ADFC(0x1e, 0x3c, p[0], 0, 0x5b);
	}

	off = MAIN_D_80134838;
	rot = MAIN_D_80134840;
	rot.vy = STD_D_8007B9CA[0];
	STD_D_8007B9CC[0] = MAIN_func_800DA63C(0x16, p[0], STD_D_8007A41C, STD_D_8007B9EC, STD_D_8007BA44);
	STD_setViewpointFromBone(*(Entity **)&p[6], &off, &rot, STD_D_8007B9CC[0]);
	GS_VIEWPOINT.vry = (-DIGIMON_DATA[STD_D_8007B9D4[0]->type].height * 2) / 3;
	GS_VIEWPOINT.vpy = -MAIN_func_800DA63C(0x16, p[0], STD_D_8007A41C, STD_D_8007BA9C, STD_D_8007BA44);
	p[0]++;
}

void STD_func_8005B688(Entity *target, Entity *entity)
{
	SVECTOR off;
	SVECTOR rot;
	int16_t dx;
	int16_t dz;
	int32_t d;
	int32_t i;

	STD_D_8007B9C8[0] = 0;
	STD_D_8007B9D8[0] = entity;
	if (target == NULL) {
		if ((Entity *)MAIN_D_80135128 == ENTITY_TABLE[1]) {
			target = MAIN_D_801350E8;
		} else {
			target = ENTITY_TABLE[1];
		}
	}
	STD_D_8007B9D4[0] = target;
	MAIN_D_801350EC = 8;
	addObject(0x1ad, 0, (TickFunction)STD_tickCameraIntro, NULL);

	off = MAIN_D_80134848;
	rot = MAIN_D_80134850;
	dx = entity->posData->location.vx - target->posData->location.vx;
	dz = entity->posData->location.vz - target->posData->location.vz;
	STD_D_8007B9CA[0] = (-_atan(dz, dx) + 0x7de) & 0xfff;
	rot.vy = STD_D_8007B9CA[0];
	STD_D_8007B9CC[0] = getDistance(dx, 0, dz);
	STD_D_8007B9CC[0] = STD_D_8007B9CC[0] + 0x2bc;
	if (STD_D_8007B9CC[0] < 0x5dc) {
		STD_D_8007B9CC[0] = 0x5dc;
	}
	STD_setViewpointFromBone(target, &off, &rot, STD_D_8007B9CC[0]);

	STD_D_8007B9CE[0] = DIGIMON_DATA[target->type].radius * 3 * 2;
	GS_VIEWPOINT.vry = (-DIGIMON_DATA[target->type].height * 2) / 3;
	GS_VIEWPOINT.vpy = -DIGIMON_DATA[entity->type].height;
	STD_D_8007B9D0[0] = GS_VIEWPOINT.vpy;
	STD_D_8007B9D2[0] = GS_VIEWPOINT.vpy * 250 / 100;
	((int32_t *)STD_D_8007BA9C)[0] = DIGIMON_DATA[entity->type].height;
	STD_D_8007BAA0[0] = DIGIMON_DATA[entity->type].height;
	STD_D_8007BAA4[0] = DIGIMON_DATA[entity->type].height * 200 / 100;
	STD_D_8007BAA8[0] = DIGIMON_DATA[target->type].height * 380 / 100;
	STD_D_8007BAAC[0] = DIGIMON_DATA[target->type].height * 250 / 100;
	for (i = 5; i < 0x16; i++) {
		((int32_t *)STD_D_8007BA9C)[i] =
			customRandom(0x50, DIGIMON_DATA[target->type].height * 180 / 100);
	}
	MAIN_func_800DA60C(0x16, STD_D_8007A41C, STD_D_8007BA9C, STD_D_8007BA44);

	d = getDistance(dx, 0, dz) + 0x2bc;
	((int32_t *)STD_D_8007B9EC)[0] = d;
	STD_D_8007B9F0[0] = d;
	if (d < 0x5dc) {
		d = 0x5dc;
	}
	STD_D_8007B9F4[0] = d * 120 / 100;
	STD_D_8007B9F8[0] = d * 80 / 100;
	STD_D_8007B9FC[0] = STD_D_8007B9CE[0];
	for (i = 5; i < 0x16; i++) {
		((int32_t *)STD_D_8007B9EC)[i] =
			customRandom(STD_D_8007B9CE[0] * 45 * 2 / 100,
		                     STD_D_8007B9CE[0] * 45 * 4 / 100);
	}
}

void STD_removeCameraIntro(void)
{
	removeObject(0x1ad, 0);
	STD_D_8007B9C8[0] = -1;
}

void STD_applyEntityViewpoint(void)
{
	char *p;

	VIEWPORT_DISTANCE = 0x15E;
	GsSetProjection(0x15E);
	p = MAIN_D_80135128[1] + 0x34;
	GS_VIEWPOINT.vrx = *(int32_t *)(p + 0x14);
	GS_VIEWPOINT.vry = -DIGIMON_DATA[(int32_t)MAIN_D_80135128[0]].height * 2 / 3;
	GS_VIEWPOINT.vrz = *(int32_t *)(p + 0x1C);
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_renderCounterDigits(int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer)
{
	POLY_FT4 *prim;
	int32_t i;
	uint32_t width;
	int32_t count;
	int32_t buf[6];

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = getTPage(0, 0, 832, 0);
	prim->clut = GetClut(0x10, 0x1E0);
	setUVDataPolyFT4(prim, 0x78, 0x30, 8, 0x12);
	setPosDataPolyFT4(prim, -0x1C, -0x63, 8, 0x12);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = getTPage(0, 0, 832, 0);
	prim->clut = GetClut(0x10, 0x1E0);
	setUVDataPolyFT4(prim, 0x88, 0x30, 8, 0x12);
	setPosDataPolyFT4(prim, 0x14, -0x63, 8, 0x12);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	width = digits;
	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 832, 0);
		prim->clut = GetClut(0x10, 0x1E0);
		setUVDataPolyFT4(prim, buf[i] * 12, 0x30, 0xC, 0xF);
		setPosDataPolyFT4(prim, x + ((((int32_t)width - 1) - i) * 14), y, 0xC, 0xF);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = GetTPage(1, 0, 0x2C0, 0);
	prim->clut = GetClut(0x200, 0xFF);
	setUVDataPolyFT4(prim, 0, 0x78, 0x42, 0x1D);
	setPosDataPolyFT4(prim, -0x21, -0x68, 0x42, 0x1D);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void STD_addFighterCounter(uint8_t arg)
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
		MAIN_D_80134D66++;
		if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]]->anim.animId != 0x2B) {
			if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->anim.animId != 0x2B) {
				if (MAIN_D_80134D66 % 0x14 == 0) {
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
	int32_t va;
	int16_t ab[2];
	int32_t v;
	int32_t flag;
	int16_t arg;
	int32_t st;
	int32_t st2;
	int16_t b;
	uint8_t u;
	int32_t n7;

	if (MAIN_D_8013514C < 5) {
		ab[0] = (MAIN_D_8013514C - 1) * 2;
		v = (MAIN_D_8013514C - 1) * 2;
		ab[1] = v + 1;
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
	va = ab[0];
	b = ab[1];

	for (i = 0; i < 8; i++) {
		s = &STD_D_8007BAF4[i];
		if (i != ab[0] && i != ab[1]) {
			continue;
		}
		n7 = s->unk7;
		switch (n7) {
		case 0:
			s->unk8++;
			s->unk9++;
			if (s->unk8 % 10 == 0) {
				s->unk5 = (s->unk5 + 1) & 1;
			}
			if (s->unk9 < 0x29) {
				continue;
			}
			s->unk9 = 0;
			s->unk7++;
			continue;
		case 1:
			s->unk8++;
			s->unk9++;
			if (s->unk9 == 5) {
				playSound(8, 5);
			}
			if (s->unk2 != STD_D_8007A48C[s->unk4]) {
				s->unk2 -= 1;
			}
			if (s->unk8 % 10 == 0) {
				STD_D_8007BAF4[i].unk5 = (s->unk5 + 1) & 1;
			}
			if (s->unk2 != STD_D_8007A48C[s->unk4]) {
				continue;
			}
			if (s->unk0 != (&STD_D_8007A598[i].unk0)[s->unk4]) {
				continue;
			}
			s->unkA[1] = 0;
			s->unk9 = 0;
			s->unk7++;
			continue;
		case 2:
			s->unk8++;
			s->unk9++;
			if (s->unk8 % 10 == 0) {
				s->unk5 = (s->unk5 + 1) & 1;
			}
			if (match == 0) {
				if (s->unk9 < 0x29) {
					continue;
				}
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
				if (!(MAIN_D_8013514C < 5) && MAIN_D_8013514C < 7) {
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
				continue;
			}
			MAIN_D_80135164 += 3;
			if (MAIN_D_80135164 < 0x81) {
				continue;
			}
			MAIN_D_80135164 = 0x80;
			continue;
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
			if (s->unkA[0] != 1) {
				continue;
			}
			s->unkA[1] = 3;
			s->unk9 = 0;
			s->unk8 = 0;
			s->unk7++;
			if (s->unkA[1] != 3) {
				STD_func_8005D538((int16_t)i);
				continue;
			}
			if (s->unkC == 1) {
				STD_func_8005D538((int16_t)i);
				continue;
			}
			s->unk6 = (s->unk6 + 1) & 1;
			continue;
		case 4:
			s->unk8++;
			if (s->unkA[1] != 3) {
				s->unk5 = 3;
				if (s->unk8 < 0x15) {
					continue;
				}
				s->unk8 = 0;
				s->unk7 = 2;
				continue;
			}
			if (s->unkC == 1) {
				s->unk5 = 3;
				if (s->unk8 < 0x1f) {
					continue;
				}
				s->unk8 = 0;
				s->unk7++;
				playSound(8, 3);
				continue;
			}
			if (s->unk8 == 0x14) {
				s->unk6 = (s->unk6 + 1) & 1;
			}
			if (s->unk8 < 0x1f) {
				continue;
			}
			s->unk4++;
			s->unk7 += 2;
			if (s->unk5 != 2) {
				continue;
			}
			s->unk5 = 0;
			continue;
		case 5:
			if (s->unk8 < 0x18) {
				v = STD_D_8007A48C[s->unk4];
				u = s->unk8++;
				s->unk2 = STD_D_8007A474[u] + v;
				if (s->unk8 != 0x14) {
					continue;
				}
				playSound(8, 4);
				continue;
			}
			s->unk7 = (uint32_t)n7 + 1;
			continue;
		case 6:
			if (match != 0) {
				if (MAIN_D_801350E4 == 1) {
					if (i == MAIN_D_8013514D) {
						s->unk0 = (&STD_D_8007A598[i].unk0)[s->unk4 + 1];
						s->unk5 = 0;
						s->unk6 = (&STD_D_8007A598[i].unk10)[s->unk4 + 1];
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
						continue;
					}
					s->unk2 = STD_D_8007A48C[s->unk4 - 1];
					s->unk5 = 3;
					continue;
				}
				if (i == MAIN_D_8013514D) {
					s->unk2 = STD_D_8007A48C[s->unk4 - 1];
					s->unk5 = 3;
					continue;
				}
				s->unk0 = (&STD_D_8007A598[i].unk0)[s->unk4 - 1];
				s->unk5 = 0;
				continue;
			}
			if (s->unkC != 0) {
				continue;
			}
			s->unk8++;
			if (s->unk8 % 10 == 0) {
				s->unk5 = (s->unk5 + 1) & 1;
			}
			if (STD_D_8007BAF4[STD_D_8007BB64[i].owner].unk7 != 6) {
				continue;
			}
			if (s->unk0 > (&STD_D_8007A598[i].unk0)[s->unk4]) {
				s->unk0 -= 1;
			}
			if (s->unk0 < (&STD_D_8007A598[i].unk0)[s->unk4]) {
				s->unk0 += 1;
			}
			if (s->unk2 != STD_D_8007A48C[s->unk4]) {
				continue;
			}
			if (s->unk0 != (&STD_D_8007A598[i].unk0)[s->unk4]) {
				continue;
			}
			s->unk6 = (&STD_D_8007A598[i].unk10)[s->unk4];
			s->unk4++;
			s->unk9 = 0;
			s->unk8 = 0;
			s->unk5 = 0;
			s->unk7 = 0;
			MAIN_D_8013514C++;
			continue;
		case 7:
			s->unk8++;
			if (i == MAIN_D_8013514D) {
				s->unk0 = (&STD_D_8007A598[i].unk0)[7];
				s->unk2 = STD_D_8007A48C[7];
				s->unk5 = 0;
				s->unk6 = (&STD_D_8007A598[i].unk10)[7];
			} else {
				s->unk0 = (&STD_D_8007A598[i].unk0)[4];
				s->unk2 = STD_D_8007A48C[4];
				s->unk6 = (&STD_D_8007A598[i].unk10)[4];
				s->unk5 = 3;
			}
			if (s->unk8 < 0x3d) {
				continue;
			}
			MAIN_D_80135160 = 1;
			continue;
		}
	}

	for (i = 0; i < 8; i++) {
		STD_func_8005D550((int16_t)i);
	}

	if (match == 0) {
		return;
	}
	if (MAIN_D_80135164 != 0x80) {
		return;
	}
	if (STD_D_8007BAF4[MAIN_D_8013514D].unk7 != 2) {
		return;
	}
	if (MAIN_D_8013514C != 7) {
		st = 6;
	} else {
		st = 7;
	}
	STD_D_8007BAF4[MAIN_D_8013514D].unk7 = st;
	if (MAIN_D_8013514C == 7) {
		STD_D_8007BAF4[MAIN_D_8013514D].unk8 = 0;
	}
	if (MAIN_D_8013514D == va) {
		if (MAIN_D_8013514C != 7) {
			st2 = 6;
		} else {
			st2 = 7;
		}
		STD_D_8007BAF4[ab[1]].unk7 = st2;
		if (MAIN_D_8013514C == 7) {
			STD_D_8007BAF4[ab[1]].unk8 = 0;
		}
	} else {
		if (MAIN_D_8013514C != 7) {
			st2 = 6;
		} else {
			st2 = 7;
		}
		STD_D_8007BAF4[ab[0]].unk7 = st2;
		if (MAIN_D_8013514C == 7) {
			STD_D_8007BAF4[ab[0]].unk8 = 0;
		}
	}
	MAIN_D_80135160 = 1;
}

void STD_func_8005CE9C(void)
{
	POLY_FT4 *prim;
	GsBOXF box;
	int32_t len;
	int32_t i;
	int32_t p;
	int32_t c;
	int32_t off;
	int32_t x;
	int32_t j;
	int32_t small;
	int16_t shift;
	int32_t k;
	int32_t v;

	for (i = 0; i < 8; i++) {
		len = strlen(DIGIMON_DATA[MAIN_D_80135150[i]].name) / 2;
		if (MAIN_D_80135150[i] == 0x4e || MAIN_D_80135150[i] == 0x3c) {
			len = 10;
		}
		p = 0;
		shift = 0;
		small = 0;
		j = 0;
		x = 0;
		off = i;
		for (; j < len; j++, x += 8) {
			k = j - 1;
			if (len < 8) {
				c = STD_D_80079CBC[MAIN_D_80135150[i]][p++];
				STD_func_8005D814((int16_t)(STD_D_8007A598[off].unk0 + 4),
				                  (int16_t)(STD_D_8007A48C[0] + 0x12 + x), c, 5);
				if (c == 0x1f || c == 0x25) {
					c = STD_D_80079CBC[MAIN_D_80135150[i]][p++];
					STD_func_8005D814((int16_t)(STD_D_8007A598[off].unk0 + 4),
					                  (int16_t)(STD_D_8007A48C[0] + 0x12 + x), c, 5);
				}
			} else {
				c = STD_D_80079CBC[MAIN_D_80135150[i]][p++];
				if (c == 0x3d) {
					c = STD_D_80079CBC[MAIN_D_80135150[i]][p++];
					small = 1;
					shift = -(k * 8);
				}
				if (small == 0) {
					v = STD_D_8007A598[off].unk0 + 8;
				} else {
					v = STD_D_8007A598[off].unk0;
				}
				STD_func_8005D814((int16_t)v,
				                  (int16_t)(shift + (STD_D_8007A48C[0] + 0x12 + x)), c, 5);
				if (c == 0x1f || c == 0x25) {
					c = STD_D_80079CBC[MAIN_D_80135150[i]][p++];
					if (small == 0) {
						v = STD_D_8007A598[off].unk0 + 8;
					} else {
						v = STD_D_8007A598[off].unk0;
					}
					STD_func_8005D814((int16_t)v,
					                  (int16_t)(shift + (STD_D_8007A48C[0] + 0x12 + x)), c, 5);
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

void STD_func_8005D398(int16_t i, int32_t owner, int32_t flag)
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

void STD_func_8005D538(int32_t i)
{
	addObject(0x1B0, i, STD_func_8005E004, 0);
}

void STD_func_8005D550(int16_t id)
{
	POLY_FT4 *prim;
	int32_t tile;
	int32_t t;
	uint8_t u;
	uint8_t v;
	int32_t w;
	int32_t h;
	int32_t w2;
	int32_t h2;

	tile = MAIN_D_80135150[id];
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	if (MAIN_D_80135164 != 0) {
		setSemiTrans(prim, 1);
	}
	t = t = tile;
	prim->tpage = GetTPage(0, 1, (tile / 64) * 64 + 0x380, 0);
	prim->clut = GetClut((tile / 64) * 16 + 0x120, STD_D_8007A49C[tile] + 0x1e0);
	setRGB0(prim, 0x80 - MAIN_D_80135164, 0x80 - MAIN_D_80135164, 0x80 - MAIN_D_80135164);
	u = ((t - 1) % 32) * 32 + (STD_D_8007BAF4[id].unk5 & 1) * 16;
	v = ((t - 1) / 8) * 32 + (STD_D_8007BAF4[id].unk5 / 2) * 16;
	h = 0x10;
	w = h;
	if (u == 0xf0) {
		w = 0xf;
	}
	if (v == 0xf0) {
		h = 0xf;
	}
	h2 = h2 = h;
	setUVDataPolyFT4(prim, u, v, w2 = w2 = w, h);
	setPosDataPolyFT4(prim, STD_D_8007BAF4[id].unk0, STD_D_8007BAF4[id].unk2, w2, h2);
	if (STD_D_8007BAF4[id].unk6 == 1) {
		swapShort(&prim->x0, &prim->x1);
		swapShort(&prim->x2, &prim->x3);
		STD_func_8005D7C0(prim, prim->u0, w, h);
	}
	AddPrim(ACTIVE_ORDERING_TABLE->org + 5, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void STD_func_8005D7A8(int32_t i)
{
	removeObject(0x1B0, i);
}

void STD_func_8005D7B4(int32_t i)
{
	removeObject(0x1AF, i);
}

// clang-format off
void STD_func_8005D7C0(prim, a, b, h)
	POLY_FT4 *prim;
	int16_t a;
	int16_t b;
	int32_t h;
// clang-format on
{
	int32_t x;
	int32_t w;

	w = b;
	x = a - 1;
	a = x;
	if ((int16_t)x < 0) {
		a = 0;
		w = (int16_t)(w - 1);
	}
	setUVDataPolyFT4(prim, a, prim->v0, w, h);
}

void STD_func_8005D814(int32_t x, int32_t y, int32_t n, int32_t size)
{
	POLY_FT4 *prim;
	uint8_t u;
	uint8_t v;
	int32_t k;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->tpage = GetTPage(0, 1, 0x340, 0x100);
	prim->clut = GetClut(0x30, 0x1e0);
	if (MAIN_D_80135164 != 0) {
		setSemiTrans(prim, 1);
	}
	setRGB0(prim, 0x80 - MAIN_D_80135164, 0x80 - MAIN_D_80135164, 0x80 - MAIN_D_80135164);
	k = k = n;
	u = (n % 8) * 8;
	v = (k / 8) * 8;
	setUVDataPolyFT4(prim, u, v, (u != 0xf8) ? 8 : 7, (v != 0xf8) ? 8 : 7);
	setPosDataPolyFT4(prim, x, y, 8, 8);
	GsSetWorkBase((PACKET *)prim);
}

void STD_func_8005D964(void)
{
	StdUnkBAF4 *p;
	int32_t i;

	MAIN_D_8013514C = 1;
	for (i = 0; i < 8; i++) {
		p = &STD_D_8007BAF4[i];
		p->unk0 = STD_D_8007A598[i].unk0;
		p->unk2 = STD_D_8007A48C[0];
		p->unk5 = 0;
		p->unk4 = 1;
		p->unk6 = STD_D_8007A598[i].unk10;
		p->unk8 = 0;
		p->unk9 = 0;
		p->unkC = 0;
		p->unk7 = 0;
	}
}

void STD_func_8005D9F4(uint8_t *out, uint8_t *list)
{
	int32_t i;
	int32_t j;
	uint8_t *p;
	int32_t r;
	int32_t k;

	MAIN_D_8013514D = random(8);
	MAIN_D_80135150[MAIN_D_8013514D] = ENTITY_TABLE[1]->type;

	for (i = 1; i < 8; i++) {
		swapByte(&list[i], &list[random(7) + 1]);
	}

	p = list + 1;
	for (i = 0; i < 8; i++) {
		if (i != MAIN_D_8013514D) {
			MAIN_D_80135150[i] = *p++;
		}
	}

	for (i = 0, j = 0; i < 4; i++, j += 2) {
		r = random(100);
		k = j;
		if (r < STD_D_8007A58C[DIGIMON_DATA[MAIN_D_80135150[j]].level - 3][DIGIMON_DATA[MAIN_D_80135151[j]].level - 3]) {
			MAIN_D_80135158[i] = k;
		} else {
			MAIN_D_80135158[i] = k + 1;
		}
	}

	if (MAIN_D_8013514D < 4) {
		if (MAIN_D_8013514D < 2) {
			MAIN_D_8013515C = MAIN_D_80135159;
		} else {
			MAIN_D_8013515C = MAIN_D_80135158[0];
		}
		r = random(100);
		if (r < STD_D_8007A58C[DIGIMON_DATA[MAIN_D_80135150[MAIN_D_8013515A]].level - 3][DIGIMON_DATA[MAIN_D_80135150[MAIN_D_8013515B]].level - 3]) {
			MAIN_D_8013515D = MAIN_D_8013515A;
		} else {
			MAIN_D_8013515D = MAIN_D_8013515B;
		}
	} else {
		r = random(100);
		if (r < STD_D_8007A58C[DIGIMON_DATA[MAIN_D_80135150[MAIN_D_80135158[0]]].level - 3][DIGIMON_DATA[MAIN_D_80135150[MAIN_D_80135159]].level - 3]) {
			MAIN_D_8013515C = MAIN_D_80135158[0];
		} else {
			MAIN_D_8013515C = MAIN_D_80135159;
		}
		if (MAIN_D_8013514D < 6) {
			MAIN_D_8013515D = MAIN_D_8013515B;
		} else {
			MAIN_D_8013515D = MAIN_D_8013515A;
		}
	}

	if (MAIN_D_8013514D % 2 == 0) {
		out[0] = MAIN_D_80135150[MAIN_D_8013514D + 1];
	} else {
		out[0] = MAIN_D_80135150[MAIN_D_8013514D - 1];
	}

	switch (MAIN_D_8013514D / 2) {
	case 0:
		out[1] = MAIN_D_80135150[MAIN_D_80135159];
		out[2] = MAIN_D_80135150[MAIN_D_8013515D];
		break;
	case 1:
		out[1] = MAIN_D_80135150[MAIN_D_80135158[0]];
		out[2] = MAIN_D_80135150[MAIN_D_8013515D];
		break;
	case 2:
		out[1] = MAIN_D_80135150[MAIN_D_8013515B];
		out[2] = MAIN_D_80135150[MAIN_D_8013515C];
		break;
	case 3:
		out[1] = MAIN_D_80135150[MAIN_D_8013515A];
		out[2] = MAIN_D_80135150[MAIN_D_8013515C];
		break;
	}
}

void STD_func_8005DEEC(int32_t track)
{
	ENTITY_TABLE[1]->isOnScreen = 0;
	ENTITY_TABLE[1]->isOnMap = 0;
	MAIN_D_80135160 = 0;
	MAIN_D_80135164 = 0;
	addObject(0x1AE, 0, (TickFunction)STD_func_8005DF64, (RenderFunction)STD_func_8005DF6C);
	stopBGM();
	stopSound();
	playMusic(0x1D, track);
}

void STD_func_8005DF64(void)
{
}

void STD_func_8005DF6C(void)
{
	STD_func_8005C1E4();
	STD_func_8005CE9C();
}

void STD_func_8005DF94(int32_t mode)
{
	ENTITY_TABLE[1]->isOnScreen = 1;
	ENTITY_TABLE[1]->isOnMap = 1;
	removeObject(0x1AE, 0);
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

void STD_func_8005E124(int32_t id)
{
	int16_t d;
	int32_t i;

	if (MAIN_D_8013514C < 5) {
		d = 1;
	} else if (MAIN_D_8013514C < 7) {
		d = 3;
	}

	i = i = id;
	if (STD_D_8007BAF4[id].unk6 == 1) {
		STD_D_8007BB64[i].x += d;
	} else {
		STD_D_8007BB64[i].x -= d;
	}
	STD_func_8005E1E4(id);
}

void STD_func_8005E1E4(int32_t id)
{
	POLY_FT4 *prim;
	int32_t n;
	int32_t o;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = GetTPage(0, 2, 0x3c0, 0);
	n = n = id;
	prim->clut = GetClut(0x130,
	                     STD_D_8007A57C[STD_D_8007A50C[MAIN_D_80135150[id]] - 1] + 0x1e0);
	setUVDataPolyFT4(prim, STD_D_8007A50C[MAIN_D_80135150[n]] * 8 - 8, 0xc0, 8, 8);
	setPosDataPolyFT4(prim, STD_D_8007BB64[n].x, STD_D_8007BB64[n].y, 8, 8);
	if (STD_D_8007BAF4[n].unk6 == 1) {
		swapShort(&prim->x0, &prim->x1);
		swapShort(&prim->x2, &prim->x3);
		STD_func_8005D7C0(prim, prim->u0, 8, 8);
	}
	AddPrim(ACTIVE_ORDERING_TABLE->org + 4, prim++);

	if (STD_D_8007BB64[n].flag == 1) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = GetTPage(0, 2, 0x3c0, 0);
		prim->clut = GetClut(0x130,
		                     STD_D_8007A57C[STD_D_8007A50C[MAIN_D_80135150[n]] - 1] + 0x1e0);
		setUVDataPolyFT4(prim, STD_D_8007A50C[MAIN_D_80135150[n]] * 8 - 8, 0xc0, 8, 8);
		setPosDataPolyFT4(prim, STD_D_8007BB64[n].x, STD_D_8007BB64[n].y + 8, 8, 8);
		if (STD_D_8007BAF4[n].unk6 == 1) {
			swapShort(&prim->x0, &prim->x1);
			swapShort(&prim->x2, &prim->x3);
			STD_func_8005D7C0(prim, prim->u0, 8, 8);
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 4, prim++);
	}
	GsSetWorkBase((PACKET *)prim);

	if (STD_D_8007BAF4[n].unk6 == 1) {
		o = STD_D_8007BB64[n].owner;
		if (STD_D_8007BAF4[o].unk0 - STD_D_8007BB64[n].x < 9) {
			STD_D_8007BAF4[o].unkA[0] = 1;
			STD_func_8005D7B4(id);
		}
	} else {
		o = STD_D_8007BB64[n].owner;
		if (STD_D_8007BB64[n].x - STD_D_8007BAF4[o].unk0 < 0x11) {
			STD_D_8007BAF4[o].unkA[0] = 1;
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
	playMusic(0x1D, 0);
	addObject(0x1A1, 0, (TickFunction)STD_func_8005E660, (RenderFunction)STD_func_8005E6E4);
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
	int32_t height;

	STD_func_8005CE9C();
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = GetTPage(0, 2, 0x380, 0x1C0);
	prim->clut = GetClut(0x100, 0x1F6);
	setRGB0(prim, MAIN_D_80135165, MAIN_D_80135165, MAIN_D_80135165);
	setUVDataPolyFT4(prim, 0xFA, 0xFD, 2, 2);
	setPosDataPolyFT4(prim, -0xA0, -0x78, 0x140, 0xF0);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[3], prim++);
	SetPolyFT4(prim);
	if (MAIN_D_80135164 != 0x80) {
		SetSemiTrans(prim, 1);
	}
	setRGB0(prim, MAIN_D_80135166, MAIN_D_80135166, MAIN_D_80135166);
	prim->tpage = GetTPage(1, 1, 0x180, 0);
	prim->clut = GetClut(0, 0x1E6);
	if (CURRENT_SCREEN != 0x6A) {
		height = 0x80;
	} else {
		height = 0x7D;
	}
	setUVDataPolyFT4(prim, 0, 0, 0xFF, height);
	if (CURRENT_SCREEN != 0x6A) {
		height = 0x80;
	} else {
		height = 0x7D;
	}
	setPosDataPolyFT4(prim, -0x80, -0x40, 0xFF, height);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[2], prim++);
	GsSetWorkBase((PACKET *)prim);
}

void STD_func_8005E898(void)
{
	removeObject(0x1A1, 0);
}

void STD_func_8005F650(void)
{
	int16_t enemies[4];
	int16_t moveFlags[4];
	int16_t targetFlags[4];
	int16_t count;
	Entity *partner;
	FighterData *fighter;
	Stats *stats;
	uint16_t *flagsPtr;
	uint16_t flags;
	int32_t chance;
	int32_t index;
	int32_t result;
	int32_t i;

	fighter = COMBAT_DATA_PTR->fighter;
	partner = ENTITY_TABLE[1];
	stats = &((DigimonEntity *)partner)->stats;
	flagsPtr = &COMBAT_DATA_PTR->fighter[0].flags;
	if (COMBAT_DATA_PTR->player.commandDelay[0] == 0) {
		COMBAT_DATA_PTR->player.currentCommand[0] = COMBAT_DATA_PTR->player.bufferedCommand[0];
	} else if (!(*flagsPtr & 0x800e) && (fighter->flatTimer == 0)) {
		COMBAT_DATA_PTR->player.commandDelay[0]--;
	}

	if (MAIN_D_80134D74 == 0) {
		if (stats->current.currentHP > fighter->hpDamageBuffer) {
			if (stats->base.brain < 0x12d) {
				if ((MAIN_D_80134D66 % (((stats->base.brain / 2) + 1) * 20)) == 0) {
					chance = 0x46 - PARTNER_PARA.discipline;
					if (random(100) < chance) {
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

	if (MAIN_D_80134D74 != 0) {
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
			index = COMBAT_DATA_PTR->player.currentCommand[0] - 8;
			if ((((DigimonEntity *)partner)->stats.base.moves[COMBAT_DATA_PTR->player.currentCommand[0] - 8] != fighter->queuedAnim) || (fighter->moveRange <= 0)) {
				STD_setupQueuedMove((DigimonEntity *)partner, fighter, 0, (COMBAT_DATA_PTR->player.currentCommand[0] - 8) & 0xff);
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

	if ((*flagsPtr & 8) && ((MAIN_D_80134D66 % 100) == 0)) {
		fighter->targetId = enemies[random(count)];
	}

	flags = *flagsPtr;
	if (flags & 0x40) {
		return;
	}

	if (flags & 8) {
		fighter->queuedAnim = 0;
		fighter->targetId = 1;
		fighter->moveRange = 2;
		partner->flatSprite = 0;
		fighter->flags |= 0x40;
		return;
	}

	if (flags & 4) {
		return;
	}

	if (flags & 2) {
		STD_selectConfusedMove((DigimonEntity *)partner, fighter, 0);
		return;
	}

	if (flags & 0x800) {
		return;
	}

	if (flags & 0x1000) {
		return;
	}

	if (flags & 0x2000) {
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
		STD_setupQueuedMove((DigimonEntity *)partner, fighter, 0, result & 0xff);
	}
}

void STD_func_8005EF84(void)
{
	int16_t moves[18];
	int16_t effectIds[18];
	int8_t isBusy;
	int32_t moveCount;
	int32_t frames;
	int32_t finished;
	int32_t i;
	int32_t j;
	DigimonEntity *entity;
	uint8_t move;

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
			if ((move = entity->stats.base.moves[j]) != 0xff) {
				moves[moveCount++] =
					DIGIMON_DATA[entity->entity.type].moves[move - 0x2e] + 0x100;
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
			move = entity->stats.base.moves[j];
			if (move == 0xff) {
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

int32_t STD_func_8005F354(void)
{
	Entity *other;
	int32_t i;

	if (COMBAT_DATA_PTR->fighter[0].hpDamageBuffer != 0) {
		return 0;
	}

	if (COMBAT_DATA_PTR->fighter[1].hpDamageBuffer != 0) {
		return 0;
	}

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
		FighterData *fighter = *(FighterData **)&COMBAT_DATA_PTR;

		if ((PARTNER_ENTITY.digimonEntity.stats.current.currentHP - fighter->hpDamageBuffer) <= 0) {
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
	int32_t j;
	int16_t brains;
	int16_t *dst;
	FighterData *f;
	Stats *stats;

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
	MAIN_D_80134D7C[1] = 0;
	MAIN_D_80134D60 = NULL;
	MAIN_D_80135174 = 0;
	MAIN_D_80134D84 = 0;
	COMBAT_DATA_PTR->player.unk7 = 0;
	COMBAT_DATA_PTR->player.changeTarget = 0;
	MAIN_D_80134D7C[0] = 1;
	MAIN_D_80134D66 = 1;
	MAIN_D_80134D74 = 0;
	MAIN_D_80134D64 = 0;
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

	for (i = 0; ENEMY_COUNT >= i; i++) {
		stats = &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]])->stats;
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
	COMBAT_DATA_PTR->player.bufferedCommand[0] = 3;
	COMBAT_DATA_PTR->player.currentCommand[0] = 3;

	for (i = 0; ENEMY_COUNT >= i; i++) {
		stats = &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]])->stats;
		(&MAIN_D_801350FC)[i] = stats->current.currentHP;
		dst = (int16_t *)&INITIAL_COMBAT_STATS[i];
		*dst++ = stats->base.hp;
		*dst++ = stats->base.mp;
		*dst++ = stats->base.off;
		*dst++ = stats->base.def;
		*dst++ = stats->base.speed;
		*dst++ = stats->base.brain;
	}

	if ((PARTNER_PARA.condition & 0x60) != 0) {
		stats = &PARTNER_ENTITY.digimonEntity.stats;
		stats->base.off = stats->base.off - (int16_t)(stats->base.off / 5);
		stats->base.def = stats->base.def - (int16_t)(stats->base.def / 5);
		stats->base.speed = stats->base.speed - (int16_t)(stats->base.speed / 5);
		stats->base.brain = stats->base.brain - (int16_t)(stats->base.brain / 5);
	}

	STD_func_8006AD00(0);
	STD_func_8006AD00(1);
	STD_addCommandMenu(0);
}

void STD_func_8005FDDC(void)
{
	CombatData *combat;
	Entity *partner;
	Entity *entity;
	FighterData *fighter;
	uint16_t *flagsPtr;
	Stats *stats;
	uint16_t flags;
	int32_t chance;
	int16_t anim;
	BaseStats *base;
	int32_t i;

	combat = COMBAT_DATA_PTR;
	partner = ENTITY_TABLE[1];
	if ((MAIN_D_80134D66 % 20) == 0) {
		for (i = 0; ENEMY_COUNT >= i; i++) {
			if (combat->fighter[i].buffPrioTimer != 0) {
				combat->fighter[i].buffPrioTimer--;
			}
		}
	}

	for (i = 1, fighter = &combat->fighter[1]; ENEMY_COUNT >= i; i++, fighter = (FighterData *)((int32_t)fighter + 0x168)) {
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		stats = &((DigimonEntity *)entity)->stats;
		base = &stats->base;
		stats->base.movesPrio[0] = 30;
		stats->base.movesPrio[1] = 30;
		stats->base.movesPrio[2] = 30;
		stats->base.movesPrio[3] = 0;
		flagsPtr = &fighter->flags;
		if (MAIN_D_80134D74 == 0) {
			if (stats->current.currentHP > fighter->hpDamageBuffer) {
				if (stats->base.brain < 0x12d) {
					if ((MAIN_D_80134D66 % (((stats->base.brain / 2) + 1) * 20)) == 0) {
						chance = (0x12c - base->brain) / 4;
						if (random(100) < chance) {
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
		flags = *flagsPtr;
		if (flags & 0x80b0) {
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
		if (fighter->hpDamageBuffer >= stats->current.currentHP) {
			handleBattleIdle((DigimonEntity *)entity, stats, flags);
			fighter->moveRange = -1;
			STD_resetFlatten(i);
			STD_removeStatusEffects((DigimonEntity *)entity, fighter);
			fighter->flags &= 0xfff0;
			continue;
		}
		if (((DigimonEntity *)partner)->stats.current.currentHP == 0) {
			handleBattleIdle((DigimonEntity *)entity, stats, flags);
			STD_resetFlatten(i);
			*flagsPtr &= 0xff4f;
			*flagsPtr |= 0x40;
			fighter->moveRange = -1;
			continue;
		}
		if (MAIN_D_80134D74 != 0) {
			return;
		}
		if (flags & 0x40) {
			continue;
		}
		if (flags & 8) {
			fighter->queuedAnim = 0;
			fighter->targetId = 0;
			fighter->moveRange = 2;
			startAnimation(entity, 0x23);
			entity->flatSprite = 0;
			fighter->flags |= 0x40;
			continue;
		}
		if (flags & 4) {
			continue;
		}
		if (flags & 2) {
			STD_selectConfusedMove((DigimonEntity *)entity, fighter, i);
			continue;
		}
		if (flags & 0x2000) {
			continue;
		}
		if (flags & 0x800) {
			continue;
		}
		if (flags & 0x1000) {
			continue;
		}
		if (!(flags & 0x980e) && (fighter->flatTimer == 0)) {
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
		if (random(10) == 0) {
			stats->current.chargeMode = 2;
		} else {
			stats->current.chargeMode = random(2);
		}
		STD_func_80067744((DigimonEntity *)entity, fighter, i);
	}
}

void STD_func_800602A8(void)
{
	DigimonEntity *entity;
	DigimonEntity *target;
	FighterData *fighter;
	uint16_t *flags;
	uint32_t combat;
	int32_t id;
	int32_t i;

	if (MAIN_D_80134D7C[1] > 0) {
		MAIN_D_80134D7C[1]--;
	}

	if (MAIN_D_80134D84 > 0) {
		MAIN_D_80134D84--;
	}

	combat = (uint32_t)COMBAT_DATA_PTR;
	for (i = 0, fighter = (FighterData *)combat; ENEMY_COUNT >= i; i++, fighter = (FighterData *)((int32_t)fighter + 0x168)) {
		flags = &fighter->flags;
		combat = (uint32_t)COMBAT_DATA_PTR;
		entity = (DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)i + combat))[0x66c]];
		STD_addFinisherProgress(fighter, 1);
		id = fighter->targetId;

		if (id != 0xff) {
			combat = (uint32_t)COMBAT_DATA_PTR;
			target = (DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)id + combat))[0x66c]];
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
		if ((MAIN_D_80134D66 % 600) == 0 && random(2) == 1) {
			MAIN_D_801350EC = 6;
			STD_setRandomViewpoint(ENTITY_TABLE[1], 4);
			MAIN_D_8013512C = random(0x29) + 0x3c;
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

int16_t STD_func_80060620(int32_t a, int32_t b)
{
	Stats *stats;
	int32_t i;
	int32_t j;
	int32_t off;

	PARTNER_ENTITY.digimonEntity.stats.current.chargeMode = MAIN_D_80135168;
	GAME_STATE = 5;
	STD_func_80060998();
	for (i = 0, off = 0; ENEMY_COUNT >= i; i++, off += 0x168) {
		removeEntityText(i);
		STD_resetFlatten(i);
		STD_removeStatusEffects((DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)COMBAT_DATA_PTR + (uint32_t)i))[0x66c]], (FighterData *)((uint32_t)COMBAT_DATA_PTR + off));
		((FighterData *)((uint32_t)off + (uint32_t)COMBAT_DATA_PTR))->flags = 0;
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
		startAnimation(ENTITY_TABLE[((uint8_t *)((uint32_t)COMBAT_DATA_PTR + (uint32_t)a))[0x66c]], 0x2a);
		i = 0;
		stopBGM();
		stopSound();
		if (a == 0) {
			playMusic(MAIN_D_801350F8, 3);
		} else {
			playMusic(MAIN_D_801350F8, 4);
		}
		j = j = a;
		for (; i < MAIN_D_80135170; i++) {
			STD_battleTickFrame();
			if ((ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[j]]->anim.animFlag & 1) == 0) {
				startAnimation(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[j]], 0x2a);
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
	removeObject(0x1A3, 0);
	removeObject(0x1A3, 1);
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

	for (i = 1; i <= ENEMY_COUNT; i++) {
		if (*(int16_t *)((char *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]] + 0x4C) - COMBAT_DATA_PTR->fighter[i].hpDamageBuffer > 0) {
			return 0;
		}
	}

	return 1;
}

void STD_func_80060C14(uint8_t hasLostP1, uint8_t hasLostP2)
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
			if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40) {
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
			if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40) {
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
	char *name;

	clearTextArea();
	drawString(MAIN_D_80134878, 6, 0);
	drawString(STD_D_8007A658, 0, 12);
	name = PARTNER_ENTITY.name;
	drawString(name, (120 - strlen(name) * 6) / 2, 24);
	drawString(DIGIMON_DATA[ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->type].name, (120 - strlen(DIGIMON_DATA[ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->type].name) * 6) / 2, 36);
	DrawSync(0);
	removeObject(0x1a2, 0);

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

	if ((entity->anim.animFlag & 1) == 0) {
		if (entity->anim.frameCount != entity->anim.animFrame) {
			for (i = 0; i <= ENEMY_COUNT; ++i) {
				if (i != id) {
					Entity *other = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
					uint32_t moveIdx = other->anim.animId - 0x2e;

					if (DIGIMON_DATA[other->type].moves[moveIdx] == 0x2d) {
						if (id == COMBAT_DATA_PTR->fighter[i].targetId) {
							return;
						}
					}
				}
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
	if (MAIN_D_80134D74 != 0) {
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
	if (MAIN_D_80134D74 != 0) {
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
		if (random(100) >= 5) {
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
		fighter->flags &= 0xDFBF;
	} else {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
	}
}

void STD_tickChargeState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	int32_t r;
	int16_t tech;

	if (MAIN_D_80134D74 != 0) {
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
	if (MAIN_D_80134D74 != 0) {
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
	if ((MAIN_D_80134D74 != 0) && (MAIN_D_80134D60 != &digimon->entity)) {
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

	if (MAIN_D_80134D74 != 0) {
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
			range = STD_getMoveWithHighestDistance(target) + 0x9C400;
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
	int16_t orig;
	int16_t tech;
	uint32_t dist;
	int32_t radius;

	rot = &digimon->entity.posData->rotation.vy;
	digimon->entity.anim.animFlag &= 0xfd;
	orig = *rot;
	if (target != NULL) {
		dist = STD_getDistanceSquared(&digimon->entity, &target->entity);
		radius = DIGIMON_DATA[digimon->entity.type].radius + DIGIMON_DATA[target->entity.type].radius;
		if ((radius * radius) >= dist) {
			handleBattleIdle(digimon, &digimon->stats, fighter->flags);
			if (MAIN_D_80134D74 != 0) {
				if (MAIN_D_80134D60 != &digimon->entity) {
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
					MAIN_D_80134D74 = 1;
				}
				if (MAIN_D_80134D74 != 0) {
					MAIN_D_80134D60 = &digimon->entity;
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
		if (MAIN_D_80134D74 == 0) {
			STD_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			goto check;
		}
		if (MAIN_D_80134D60 != &digimon->entity) {
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

void STD_func_80061F44(DigimonEntity *entity, DigimonEntity *other, FighterData *data, int16_t move)
{
	uint32_t distance;
	int32_t range;
	uint32_t maxDistance;
	uint32_t minDistance;

	if (data->unk15 > 100) {
		handleBattleIdle(entity, &entity->stats, data->flags);

		++data->unk15;
		if (data->unk15 > 160) {
			data->flags &= 0xffbf;
			data->unk15 = 0;
		}

		return;
	}

	if (MAIN_D_80134D74 != 0 && &entity->entity == MAIN_D_80134D60) {
		STD_func_80065540(entity, other, data);
		return;
	}

	distance = STD_getDistanceSquared(&entity->entity, &other->entity);
	range = (DIGIMON_DATA[entity->entity.type].radius + DIGIMON_DATA[other->entity.type].radius);
	maxDistance = *(uint32_t *)&MOVE_DATA[move].distance + range * range;

	if (maxDistance + (minDistance = maxDistance * 3 / 10) < distance) {
		STD_setWalking(&entity->entity, &entity->stats, data->flags);
		STD_moveTowardLocation(entity, &other->entity.posData->location, 280, 200);
		++data->unk15;
	} else if (distance < maxDistance - minDistance) {
		STD_setWalking(&entity->entity, &entity->stats, data->flags);
		STD_backAwayFromTarget(entity, other, data);
		++data->unk15;
	} else {
		data->unk15 = 0;

		handleBattleIdle(entity, &entity->stats, data->flags);

		if ((data->flags & 8) != 0) {
			if (MAIN_D_80134D66 % 40 == 0) {
				STD_func_80065540(entity, other, data);
			} else {
				entityLookAtLocation(&entity->entity, &other->entity.posData->location);
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
	int16_t *rot;
	int16_t orig;
	int16_t away;
	int16_t hit;
	int16_t ccw;
	int16_t cw;

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
	if (orig == target->entity.posData->rotation.vy) {
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

	if ((int16_t)(ccw - cw) < 0) {
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

	digimon = (DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)index + (uint32_t)COMBAT_DATA_PTR))[0x66c]];
	fighter = &COMBAT_DATA_PTR->fighter[index];
	if (entityCheckCollision(NULL, &digimon->entity, 0x118, 0xc8) != -1) {
		digimon->entity.anim.animFlag |= 2;
	} else {
		digimon->entity.anim.animFlag &= 5;
	}

	if ((MAIN_D_80134D74 != 0) && (MAIN_D_80134D60 == &digimon->entity) && (MAIN_D_80135174 > 0)) {
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

	if (MAIN_D_80134D60 == &digimon->entity) {
		MAIN_D_80134D74 = 0;
		MAIN_D_80134D60 = NULL;
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
	int16_t angle;

	if (random(0xa) >= 8) {
		rot = &entity->posData->rotation.vy;
		*rot += random(0x400) - 0x200;
		angle = *rot;
		if (angle < 0) {
			*rot = angle + 0x1000;
		} else {
			*rot = angle % 4096;
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
	} else if ((reach + farLimit) < dist) {
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
		if ((MAIN_D_80134D66 % 2) == 0) {
			fighter->speedBuffer += (stats->base.speed / 100) + 1;
		}
		if (fighter->speedBuffer >= 0x65) {
			fighter->speedBuffer = 0x64;
		}
	}
}

int32_t STD_func_80062BD8(int16_t *out, uint8_t index)
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
	addObject(0x1A3, 0, 0, (RenderFunction)STD_func_80063508);
	addObject(0x1A3, 1, 0, (RenderFunction)STD_func_80063508);
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

	*rot = cand[0] + random(0x400);
	if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
		*rot = orig;
		collisionGrace(NULL, entity, 0x118, 0xc8);
	}

	return;
grace:
	*rot = orig;
	collisionGrace(NULL, entity, 0x118, 0xc8);
}

int16_t STD_func_8006314C(Entity *entity, int32_t arg)
{
	int16_t result;

	COMBAT_AREA_CENTER_X = 0;
	COMBAT_AREA_CENTER_Y = 0;
	stopBGM();
	playMusic(MAIN_D_801350F8, 2);
	STD_func_8005E8A4(entity, (Entity *)arg);
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

	MAIN_func_800E642C();
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
	prim->clut = GetClut(0x10, 0x1E0);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setUVDataPolyFT4(prim, 0, 8, 0x90, 0x18);
	setPosDataPolyFT4(prim, -0x48, -0xC, 0x90, 0x18);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[5], prim++);
	GsSetWorkBase((PACKET *)prim);
}

void STD_func_80063300(int32_t id)
{
	int16_t x;
	int16_t y;
	int32_t layer;
	int32_t digits;

	x = UI_BOX_DATA[id].finalPos.x;
	y = UI_BOX_DATA[id].finalPos.y;
	renderString(0, x + 6, y + 6, 120, 12, 0, 24, (layer = 6 - id), 1);
	renderString(0, x + 138, y + 6, 120, 12, 0, 36, layer, 1);
	renderString(0, x + 108, y + 24, 48, 24, 0, 0, layer, 1);

	digits = STD_func_80061124(MAIN_D_80135102);
	STD_func_800593D0(x + 42 + (48 - digits * 12) / 2, y + 30, digits, MAIN_D_80135102, layer);
	digits = STD_func_80061124(MAIN_D_80135100);
	STD_func_800593D0(x + 174 + (48 - digits * 12) / 2, y + 30, digits, MAIN_D_80135100, layer);
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

void STD_func_80063508(int32_t id)
{
	Entity *entity;
	POLY_FT4 *prim;
	MATRIX *m;
	SVECTOR pos;
	uint32_t otz;
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
	sxy[0] += (int16_t)(VIEWPORT_DISTANCE * (DIGIMON_DATA[entity->type].radius / 2) / (otz * 4));

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
	SVECTOR *hitPos;
	VECTOR loc;
	AttackObject attack;
	FighterData *fighter;
	PlayerDataSub *sub;
	Entity *attacker;
	uint8_t *moves;
	int32_t tech;
	int32_t dmg;
	int32_t handled;
	int32_t id;
	uint32_t moveIdx;
	int32_t i;
	int32_t j;
	Entity *entity;
	int32_t t;

	fighter = COMBAT_DATA_PTR->fighter;
	sub = &COMBAT_DATA_PTR->player.unk1[0];
	i = 0;
	hitPos = &attack.position;
	for (; ENEMY_COUNT >= i; i++, fighter++, sub++) {
		if (fighter->flags & 0x8000) {
			continue;
		}
		if (popAttackObject(COMBAT_DATA_PTR->player.entityIds[i], &attack) == 0) {
			continue;
		}
		entity = ENTITY_TABLE[((uint8_t *)((uint32_t)i + (uint32_t)COMBAT_DATA_PTR))[0x66c]];
		tech = STD_getAttackTech(&attack);
		moveIdx = entity->anim.animId - 0x2e;
		if (DIGIMON_DATA[entity->type].moves[moveIdx] == 0x2d) {
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
		dmg = STD_applyPartnerStatsToFighter((DigimonEntity *)attacker, (DigimonEntity *)entity, fighter, tech);
		if (entity == MAIN_D_80134D60) {
			if (i == 0) {
				STD_removeFinisherChargeup();
			}
			if ((*(int32_t *)&MAIN_D_80135178) != -1) {
				STD_removeFinisherAura(MAIN_D_80135178);
			}
			MAIN_D_80134D74 = 0;
			MAIN_D_80134D60 = NULL;
		}
		if (random(100) < dmg) {
			if (MAIN_D_801350EC == 6 && MOVE_DATA[tech].range == 1 && MAIN_D_801350EC == 3) {
				t = entityGetTechFromAnim((Entity *)MAIN_D_80135128, ((Entity *)MAIN_D_80135128)->anim.animId);
				if (MOVE_DATA[t].range == 3) {
					goto skipViewpoint;
				}
			}
			STD_setRandomViewpoint(entity, random(4));
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
					if (random(100) < (((DigimonEntity *)entity)->stats.base.speed / DIGIMON_DATA[entity->type].level)) {
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
			createParticleFX(0, 2, hitPos, entity, 0x11);
		}
		if ((fighter->flags & 0x80) && (fighter->invulnerableTimer > 0)) {
			goto blocked;
		}
		if (MOVE_DATA[tech].range == 1) {
			dmg = STD_calculateDamage((DigimonEntity *)attacker, (DigimonEntity *)entity, tech);
			dmg = dmg * (random(0x15) + 0xa) / 100;
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
	DigimonEntity *digimon;
	int16_t slot;
	int16_t tech;
	int32_t i;

	if (attack->effectId != 0x179) {
		slot = STD_getFighterSlot(attack->casterId);
		for (i = 0; i < 4; i++) {
			if (attack->effectId == COMBAT_DATA_PTR->fighter[slot].effectSlot[i]) {
				break;
			}
		}
		digimon = (DigimonEntity *)ENTITY_TABLE[attack->casterId];
		tech = entityGetTechFromAnim(&digimon->entity, digimon->stats.base.moves[i]);
	} else {
		tech = 0x79;
	}

	return tech;
}

int32_t STD_applyBuffMove(DigimonEntity *digimon, int32_t slot, int32_t anim)
{
	Stats *stats;

	stats = &digimon->stats;
	switch (anim) {
	case 0x29:
		STD_buffStats(digimon, slot, stats->base.off * 3 / 10, &stats->base.off, 0xb, 3);
		break;
	case 0x2a:
		STD_buffStats(digimon, slot, stats->base.off / 10, &stats->base.off, 0xb, 3);
		STD_buffStats(digimon, slot, stats->base.def * 5 / 100, &stats->base.def, 0xb, 4);
		STD_buffStats(digimon, slot, stats->base.speed / 10, &stats->base.speed, 0xb, 5);
		break;
	case 0x22:
		STD_buffStats(digimon, slot, stats->base.def / 5, &stats->base.def, 0xb, 4);
		STD_buffStats(digimon, slot, stats->base.speed / 10, &stats->base.speed, 0xb, 5);
		break;
	case 0x15:
		STD_buffStats(digimon, slot, stats->base.off * 7 / 100, &stats->base.off, 0xb, 3);
		STD_buffStats(digimon, slot, stats->base.def * 8 / 100, &stats->base.def, 0xb, 4);
		STD_buffStats(digimon, slot, stats->base.speed * 7 / 100, &stats->base.speed, 0xb, 5);
		break;
	case 0x1e:
		STD_buffStats(digimon, slot, stats->base.off / 4, &stats->base.off, 0xb, 3);
		STD_buffStats(digimon, slot, stats->base.def * 3 / 20, &stats->base.def, 0xb, 4);
		STD_buffStats(digimon, slot, stats->base.speed * 3 / 20, &stats->base.speed, 0xb, 5);
		break;
	default:
		return 0;
	}

	return 1;
}

int16_t STD_applyPartnerStatsToFighter(DigimonEntity *attacker, DigimonEntity *defender, FighterData *fighter, int16_t move)
{
	uint32_t anim;
	int32_t value;
	int32_t half;
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

	anim = defender->entity.anim.animId;
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

	value = defender->stats.base.speed - (attacker->stats.base.speed / 10);
	half = MOVE_DATA[move].accuracy / 2;
	x = half * value / 999;
	if ((anim == 0x21) || (anim == 0x22)) {
		x = x * 6 / 5;
	}

	result = MOVE_DATA[move].accuracy - x;
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
	uint8_t mult[3];
	int32_t i;
	int32_t dmg;
	int32_t diff;
	int16_t off;
	int16_t def;

	for (i = 0; i < 3; i++) {
		if (DIGIMON_DATA[defender->entity.type].special[i] != 0xff) {
			mult[i] = MAIN_D_80125F70[MOVE_DATA[move].special][DIGIMON_DATA[defender->entity.type].special[i]];
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
		dmg = (mult[0] + mult[1] + mult[2]) * (MOVE_DATA[move].power + diff * MOVE_DATA[move].power / 500) / 30 * (random(0x15) + 0x5a) / 100;
	}
	if ((move >= 0x3a) && (move < 0x71)) {
		if ((Entity *)attacker == ENTITY_TABLE[1]) {
			if (COMBAT_DATA_PTR->player.finisherChargeup[0] >= 0x29) {
				dmg = dmg * COMBAT_DATA_PTR->player.finisherChargeup[0] / 40;
			}
		} else {
			dmg = dmg * (random(0x65) + 0x64) / 100;
		}
		dmg = dmg * (random(0x15) + 0x5a) / 100;
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
	VECTOR *loc;
	int16_t *rotY;
	int32_t ax;
	int32_t az;
	int32_t dx;
	int32_t dz;
	int32_t nx;
	int16_t ang;

	loc = &entity->posData->location;
	rotY = &entity->posData->rotation.vy;
	ax = ax = attack->position.vx;
	az = az = attack->position.vz;
	dx = loc->vx - ax;
	dz = loc->vz - az;
	nx = nx = -dx;
	ang = _atan(-dz, nx);
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
			*rotY = _atan(dz, dx);
			STD_startHitAnimation(entity, attack, 0x29);
		}
	} else if (!(0 > *rotY) && *rotY < 0x400) {
		if ((!(0 > ang) && ang <= *rotY + 0x400) || (ang >= *rotY + 0xc00 && ang < 0x1000)) {
			*rotY = ang;
			STD_startHitAnimation(entity, attack, 0x28);
		} else {
			*rotY = _atan(dz, dx);
			STD_startHitAnimation(entity, attack, 0x29);
		}
	} else {
		if ((*rotY - 0x400 <= ang && ang < 0x1000) || (!(0 > ang) && *rotY - 0xc00 >= ang)) {
			*rotY = ang;
			STD_startHitAnimation(entity, attack, 0x28);
		} else {
			*rotY = _atan(dz, dx);
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
	if (random(100) < chance) {
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
				fighter->confusionTimer = random(0x65) + 200;
				STD_addConfusionStatusVisual(digimon, fighter);
				STD_resetFighterAction(fighter);
			}
			break;
		case 3:
			if (!(fighter->flags & 4)) {
				fighter->flags |= 4;
				fighter->stunTimer = random(0x29) + 200;
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

void STD_buffStats(DigimonEntity *digimon, int32_t slot, int16_t value, int16_t *stat, int16_t color, uint8_t flag)
{
	addWithLimit(stat, value, 0x3e7);
	addEntityText(digimon, slot, color, value, flag);
}

void STD_startHitAnimation(Entity *entity, AttackObject *attack, int32_t animId)
{
	int16_t tech;

	tech = STD_getAttackTech(attack);
	startAnimation(entity, animId);
	createParticleFX(MOVE_DATA[tech].special, 1, &attack->position, entity, MOVE_DATA[tech].iframes + 0x10);
}

void STD_func_80064FCC(unsigned short count)
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
	AddPrim((char *)ACTIVE_ORDERING_TABLE->org + 0x80, &DRAW_OFFSETS[ACTIVE_FRAMEBUFFER * 0xc]);
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
	int16_t tech;
	int16_t mp;

	if (digimon->stats.base.moves[slot] == 0xff) {
		return 0;
	}

	mp = entityGetTechFromAnim(&digimon->entity, digimon->stats.base.moves[slot]);
	tech = mp;
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
				mp = mp - (int16_t)(mp / 5);
			} else if (PARTNER_ENTITY.digimonEntity.stats.base.brain >= 900) {
				mp = mp - (int16_t)(mp * 15 / 100);
			} else if (PARTNER_ENTITY.digimonEntity.stats.base.brain >= 800) {
				mp = mp - (int16_t)(mp / 10);
			} else {
				mp = mp - (int16_t)(mp / 20);
			}
		}
		if (PARTNER_PARA.condition & 0x60) {
			mp = mp + (int16_t)(mp / 2);
		}
	}

	if (digimon->stats.current.currentMP >= mp) {
		return 1;
	}

	return 0;
}

int16_t STD_getNearestEnemy(Entity *self, int16_t *flags)
{
	Entity *other;
	int16_t best;
	uint32_t bestDist;
	uint32_t dist;
	int32_t i;

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
	VECTOR *pa;
	VECTOR *pb;
	int32_t dx;
	int32_t dz;

	pa = &a->posData->location;
	pb = &b->posData->location;
	dx = pa->vx - pb->vx;
	dz = pa->vz - pb->vz;

	return (dx * dx) + (dz * dz);
}

void STD_selectConfusedMove(DigimonEntity *digimon, FighterData *fighter, int32_t tech)
{
	TargetChoice choice;

	if (random(10) < 7) {
		fighter->targetId = 0xff;
	} else {
		STD_getRemainingEnemies(&digimon->entity, choice.enemies, &choice.count);
		fighter->targetId = choice.enemies[random(choice.count)];
	}

	if (STD_func_80066A50(choice.flags, tech) == 0) {
		STD_setFighterCooldown(digimon, fighter);
	} else {
		STD_setupQueuedMove(digimon, fighter, tech, STD_getRandomUsableMove(choice.flags) & 0xff);
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
	int16_t flags[4];
	int16_t weights[4];
	int16_t tech;
	int32_t total;
	int32_t pick;
	int32_t i;
	int32_t j;

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

	pick = random(total);
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
	Stats *stats;
	int32_t values[3];
	int32_t keys[3];
	int32_t groups[3];
	int16_t flags[4];
	int16_t weights[4];
	int16_t w;
	int16_t tech;
	int16_t bonus;
	int32_t count;
	int32_t total;
	int32_t pick;
	int32_t i;
	int32_t j;

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
			bonus = fighter->buffPrioTimer + STD_calculateElementBonus(MOVE_DATA[tech].special, DIGIMON_DATA[digimon->entity.type].special[0]);
			weights[i] += bonus;
			continue;
		}
		weights[i] = weights[i] + STD_calculateElementBonus(MOVE_DATA[tech].special, DIGIMON_DATA[ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighter->targetId]]->type].special[0]);
		if (MOVE_DATA[tech].range != 3) {
			continue;
		}
		count = STD_countLivingEnemies();
		switch (digimon->stats.current.chargeMode) {
		case 0:
			if ((stats->base.brain >= 0xc8) && (count >= 2)) {
				bonus = count * 10;
				weights[i] += bonus;
			}
			break;
		case 1:
			if ((stats->base.brain >= 0xc8) && (count >= 2)) {
				bonus = count * 15;
				weights[i] += bonus;
			}
			break;
		case 2:
			if ((stats->base.brain >= 0xc8) && (count >= 2)) {
				bonus = count * 15;
				weights[i] += bonus;
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
					w = MAIN_D_80134890[groups[i]];
					weights[keys[i]] = w;
				}
			}
			break;
		}
	}

	total = 0;
	for (i = 0; i < 3; i++) {
		total += weights[i];
	}

	pick = random(total);
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
		STD_setupQueuedMove(digimon, fighter, index, (uint8_t)i);
	} else {
		fighter->cooldown = 0x50;
		fighter->flags |= 0x800;
	}
}

void STD_setupQueuedMove(DigimonEntity *digimon, FighterData *fighter, int16_t arg2, int32_t moveIndex)
{
	int16_t tech;

	fighter->unk15 = 0;
	fighter->queuedAnim = digimon->stats.base.moves[moveIndex];
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
	int32_t anim;
	uint32_t a2;

	if (MAIN_D_80134D74 != 0) {
		if (MAIN_D_80134D60 != &digimon->entity) {
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
			MAIN_D_80134D74 = 1;
		}
		if (MAIN_D_80134D74 != 0) {
			MAIN_D_80134D60 = &digimon->entity;
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
		anim = target->entity.anim.animId;
		a2 = anim;
		if (anim == 0x28) {
			return;
		}
		if (a2 == 0x29) {
			return;
		}
		if (digimon != target) {
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
		}
	}
	if ((MOVE_DATA[tech].unk3 & 2) != 0) {
		if (&digimon->entity == ENTITY_TABLE[1]) {
			MAIN_D_80134D7C[1] = 0x6e;
		} else {
			MAIN_D_80134D84 = 0x6e;
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
	int32_t i;
	int16_t tech;
	Entity *e;
	FighterData *f;

	tech = entityGetTechFromAnim(entity, fighter->queuedAnim);
	if ((MOVE_DATA[tech].unk3 & 2) != 0) {
		if (entity == ENTITY_TABLE[1]) {
			if (MAIN_D_80134D7C[1] > 0) {
				return 1;
			}
		} else {
			if (MAIN_D_80134D84 > 0) {
				return 1;
			}
		}
		for (i = 0; i <= ENEMY_COUNT; i++) {
			e = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
			f = (FighterData *)((uint8_t *)COMBAT_DATA_PTR + i * 0x168);
			if (entity == e) {
				continue;
			}
			if ((f->flags & 0x20) == 0) {
				continue;
			}
			tech = entityGetTechFromAnim(e, e->anim.animId);
			if ((MOVE_DATA[tech].unk3 & 2) == 0) {
				continue;
			}
			return 1;
		}
	}

	return 0;
}

void STD_func_800658B4(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	int32_t tech;
	int16_t cost;
	int16_t brain;
	int32_t i;
	int32_t t;
	int32_t n;

	MAIN_D_80134EF8 = digimon;
	tech = entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim);
	if (fighter->moveRange != 4) {
		MAIN_D_80134EF4 = target;
		if (target == NULL) {
			if (MOVE_DATA[tech].unk3 & 1) {
				if (digimon != (DigimonEntity *)ENTITY_TABLE[1]) {
					MAIN_D_80134EF4 = (DigimonEntity *)ENTITY_TABLE[1];
					fighter->targetId = 0;
				} else {
					MAIN_D_80134EF4 = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]];
					fighter->targetId = 1;
				}
			}
		}
	} else {
		MAIN_D_80134EF4 = digimon;
	}

	STD_removeMoveEffect(digimon, fighter);
	if (digimon == (DigimonEntity *)ENTITY_TABLE[1]) {
		if (!((tech >= 0x3a) && (tech < 0x71))) {
			cost = MOVE_DATA[tech].mpCost * 3;
			if (PARTNER_PARA.condition & 0x60) {
				cost = cost + (int16_t)(cost / 2);
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
		t = entityGetTechFromAnim((Entity *)MAIN_D_80135128, ((Entity *)MAIN_D_80135128)->anim.animId);
		if (MOVE_DATA[t].range == 3) {
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

	STD_func_8005A55C((DigimonEntity *)digimon, random(n), MOVE_DATA[tech].range);
}

void STD_removeMoveEffect(DigimonEntity *digimon, FighterData *fighter)
{
	int32_t id;

	id = fighter->unk11;
	do {
		if (id != -1) {
			STD_stopEFESubEffect(id, digimon->stats.current.efeSubEffect);
		}
	} while (0);
	digimon->stats.current.efeSubEffect = -1;
	fighter->unk11 = -1;
}

void STD_addFinisherProgress(FighterData *fighter, int16_t amount)
{
	DigimonEntity *digimon;
	int16_t tech;
	int32_t i;

	for (i = 0; ENEMY_COUNT >= i; i++) {
		if (&COMBAT_DATA_PTR->fighter[i] == fighter) {
			digimon = (DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)COMBAT_DATA_PTR + (uint32_t)i))[0x66c]];
			tech = entityGetTechFromAnim(&digimon->entity, digimon->stats.base.moves[3]);
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
	CombatData *combat;
	PlayerDataSub *sub;
	Entity *entity;
	Stats *stats;
	FighterData *fighter;
	int32_t dmg;
	int32_t i;

	combat = COMBAT_DATA_PTR;
	sub = combat->player.unk1;
	i = 0;

	for (; ENEMY_COUNT >= i; i++, sub++) {
		fighter = &combat->fighter[i];
		if (fighter->flags & 0x8000) {
			continue;
		}
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		stats = &((DigimonEntity *)entity)->stats;
		if (fighter->flags & 1) {
			if (MAIN_D_80134D74 == 0) {
				fighter->poisonTimer--;
			}
			if (fighter->poisonTimer == 0) {
				fighter->poisonTimer = 100;
				dmg = stats->base.hp * (random(3) + 1) / 100;
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
			if ((MAIN_D_80134D74 == 0) && (fighter->confusionTimer != 0)) {
				fighter->confusionTimer--;
			}
			if ((fighter->confusionTimer == 0) && !(combat->fighter[0].flags & 0x20)) {
				STD_updateFighterStatusVisuals((DigimonEntity *)entity, fighter);
			}
		}
		if (fighter->flags & 4) {
			if (MAIN_D_80134D74 == 0) {
				fighter->stunTimer--;
			}
			if (fighter->stunTimer == 0) {
				STD_clearStun((DigimonEntity *)entity, fighter);
			}
		}
		if (fighter->flatTimer <= 0) {
			continue;
		}
		if ((MAIN_D_80134D74 == 0) || (fighter->flatTimer < 0x42)) {
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
				fighter->flatTimer = random(0x51) + 0xe0;
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

	fighter->flags &= 0xFFFD;
	fighter->confusionTimer = 0;
	if (((fighter->flags & 0xC) == 0) && (fighter->flatTimer == 0)) {
		fighter->flags &= 0xFFBF;
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
	int32_t value;

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
			value = 0x1000 - ((6 - t) << 11);
			scale->vx = value;
			scale->vy = value;
			scale->vz = value;
		} else if (0 <= t) {
			value = (4 - t) << 10;
			scale->vx = value;
			scale->vy = value;
			scale->vz = value;
		}
	}
}

void STD_addStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, int32_t kind)
{
	if (fighter->statusFxId == -1) {
		switch (kind) {
		case 1:
			fighter->statusFxId = STD_func_80077664(digimon, fighter);
			break;
		case 2:
			fighter->statusFxId = STD_addConfusionEffect(digimon, fighter);
			break;
		case 3:
			fighter->statusFxId = STD_addStunEffect(digimon, fighter->stunTimer);
			break;
		}
	}
}

void STD_applyStretchScale(VECTOR *scale, int32_t angle)
{
	int32_t half;

	scale->vy = _sin(angle) + 0x1000;
	half = 0x1000 - (_sin(angle) / 2);
	scale->vx = half;
	scale->vz = half;
}

void STD_applySquashScale(VECTOR *scale, int32_t angle)
{
	int32_t half;

	scale->vy = 0x1000 - (_sin(angle) / 2);
	half = (_sin(angle) / 2) + 0x1000;
	scale->vx = half;
	scale->vz = half;
}

void STD_removeStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, int32_t kind)
{
	int32_t id;

	id = fighter->statusFxId;
	do {
		if (id != -1) {
			switch (kind) {
			case 1:
				STD_removePoisonEffect(id, digimon);
				break;
			case 2:
				STD_removeConfusionEffect(id, digimon);
				break;
			case 3:
				STD_removeStunEffect(id, digimon);
				break;
			}
			fighter->statusFxId = -1;
		}
	} while (0);
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

	return picked[random(count)];
}

int16_t STD_getStrongestMove(int32_t index, int16_t *flags)
{
	MoveRanking rank;
	DigimonEntity *digimon;
	uint8_t *moves;
	int16_t tech;
	int32_t i;

	digimon = (DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)index + (uint32_t)COMBAT_DATA_PTR))[0x66c]];
	moves = digimon->stats.base.moves;
	for (i = 0; i < 3; i++) {
		if (flags[i] == 1) {
			tech = entityGetTechFromAnim(&digimon->entity, moves[i]);
			rank.score[i] = MOVE_DATA[tech].power;
		} else {
			rank.score[i] = -1;
		}
	}

	STD_getHighestScoredMove(rank.score, flags, &rank.best, 3);

	return rank.best;
}

int16_t STD_getMostEffectiveMove(int32_t index, int16_t *flags)
{
	MoveRanking rank;
	DigimonEntity *digimon;
	Entity *target;
	uint8_t *moves;
	int16_t tech;
	int32_t i;
	int32_t idx;

	idx = idx = index;
	digimon = (DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)index + (uint32_t)COMBAT_DATA_PTR))[0x66c]];
	moves = digimon->stats.base.moves;
	target = ENTITY_TABLE[((uint8_t *)COMBAT_DATA_PTR + ((FighterData *)COMBAT_DATA_PTR)[idx].targetId)[0x66c]];
	for (i = 0; i < 3; i++) {
		if (flags[i] == 1) {
			tech = entityGetTechFromAnim(&digimon->entity, moves[i]);
			rank.score[i] = MAIN_D_80125F70[MOVE_DATA[tech].special][DIGIMON_DATA[target->type].special[0]];
		} else {
			rank.score[i] = -1;
		}
	}

	STD_getHighestScoredMove(rank.score, flags, &rank.best, 3);

	return rank.best;
}

int16_t STD_getCheapestMove(int32_t index, int16_t *flags)
{
	MoveRanking rank;
	DigimonEntity *digimon;
	uint8_t *moves;
	int16_t tech;
	int32_t i;

	digimon = (DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)index + (uint32_t)COMBAT_DATA_PTR))[0x66c]];
	moves = digimon->stats.base.moves;
	for (i = 0; i < 3; i++) {
		if (flags[i] == 1) {
			tech = entityGetTechFromAnim(&digimon->entity, moves[i]);
			rank.score[i] = MOVE_DATA[tech].mpCost * 3;
		} else {
			rank.score[i] = 1000;
		}
	}

	STD_getLowestScoredMove(rank.score, flags, &rank.best, 3);

	return rank.best;
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
			*out = i;
			hits++;
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
		if (values[i] < best) {
			best = values[i];
		}
	}

	hits = 0;
	for (i = 0; i < count; i++) {
		if (best == values[i]) {
			*out = i;
			hits++;
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
	int32_t best;
	int32_t bestValue;

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
			if (values[j] < bestValue) {
				best = j;
				bestValue = values[j];
			}
		}
		swapInt(&values[i], &values[best]);
		swapInt(&keys[i], &keys[best]);
	}

	STD_calculateScoreRanks(values, groups, count);
}

int32_t STD_calculateElementBonus(int32_t arg0, int32_t arg1)
{
	int32_t result;

	switch (MAIN_D_80125F70[arg0][arg1]) {
	case 20:
		result = 10;
		break;
	case 15:
		result = 7;
		break;
	case 10:
		result = 5;
		break;
	case 5:
		result = 3;
		break;
	case 2:
		result = 1;
		break;
	}

	return result;
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
	groups[0] = 0;
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
	int32_t i;

	*count = 0;
	for (i = 0; ENEMY_COUNT >= i; i++) {
		if ((self != ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]]) && (STD_isFighterDefeated(i) == 0)) {
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
	int32_t y;
	uint8_t cmd;
	uint32_t n;
	int32_t len;
	int16_t tech;
	uint32_t y2;

	n = i;
	y = (i * 12) + 0xd8;
	rect.x = 0;
	y2 = y;
	rect.y = y;
	setWH(&rect, 0x90, 0xc);
	clearTextSubArea(&rect);
	cmd = COMBAT_DATA_PTR->player.availableCommands[i][COMBAT_DATA_PTR->player.hoveredCommand[i]];

	if ((cmd >= 8) && (cmd < 0xc)) {
		tech = entityGetTechFromAnim(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]], PARTNER_ENTITY.digimonEntity.stats.base.moves[cmd - 8]);
		drawString(MOVE_NAMES[tech], 0, y2);
		len = 0xc;
	} else {
		drawString(STD_D_8007A688[cmd - 1], 0, y2);
		len = 0xc;
	}

	renderString(0, (int32_t)(n * 160) - 0x8c, MAIN_D_8013517C[i] - 0xe, 0x90, len, 0, y2, 7, 1);
}

void STD_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, uint8_t index)
{
	int16_t c;
	int16_t eff;
	int16_t n;

	if ((index >= 8U) && (index < 0xcU)) {
		eff = MOVE_DATA[entityGetTechFromAnim(&digimon->entity, digimon->stats.base.moves[index - 8])].special;
		setUVWH(prim, STD_D_8007A708[eff * 2], STD_D_8007A709[eff * 2], 0x10, 0xf);
	} else {
		setUVWH(prim, STD_D_8007A6F8[(index - 1) * 2], STD_D_8007A6F9[(index - 1) * 2], 0x10, 0xf);
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
