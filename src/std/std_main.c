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
#include <dw/clock.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/evl.h>
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
	int32_t victimId;
	int32_t unk1;
	SVECTOR position;
	int32_t effectId;
	int32_t unk3;
	int32_t casterId;
} AttackObject;

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

extern StdCameraPreset STD_D_8007A390[];
extern StdUnkBAF4 STD_D_8007BAF4[8];
extern StdSrcA598 STD_D_8007A598[8];
extern int16_t ENEMY_COUNT;
extern int16_t MAIN_D_80134D66;
extern Entity *MAIN_D_80134D60;
extern int32_t MAIN_D_80134D74;
extern int32_t MAIN_D_80135118;
extern int32_t MAIN_D_80135174;
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
extern int8_t MAIN_D_8013512C;
extern int16_t MAIN_D_801B1C0C[];
extern int16_t MAIN_D_801B1C0E[];
extern int16_t MAIN_D_801B1C10[];
extern int32_t MAIN_D_801B1C14[];
extern int32_t MAIN_D_801B1C18[];
extern int32_t MAIN_D_801B1C1C[];
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
extern int32_t MAIN_D_801B1B98[];
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
extern int32_t MAIN_D_801B1BB8[];
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
extern int16_t STD_D_8007BA44[];
extern int16_t STD_D_8007BA9C[];
extern char *MOVE_NAMES[];
extern char *STD_D_8007A688[];

void startAnimation(Entity *entity, int32_t animId);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
int32_t entityCheckCollision(Entity *a, Entity *entity, int32_t c, int32_t d);
void handleBattleIdle(DigimonEntity *entity, Stats *stats, int32_t flags);
void createParticleFX();
void collisionGrace(Entity *a, Entity *entity, int32_t c, int32_t d);
int32_t STD_func_80061AA8(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int32_t STD_tickMeleeAttack(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int16_t arg3);
void STD_func_80065540(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int16_t STD_getMostEffectiveMove(int32_t arg0, int16_t *flags);
void addWithLimit(int16_t *stat, int32_t value, int32_t limit);
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
void STD_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, int32_t index);
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
void STD_func_8005D398(int32_t i, int32_t owner, int32_t flag);
void STD_func_8005D9F4(void);
void STD_func_8005E124(void);
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
void STD_renderCommandMenu(void);
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
void setEntityPosition(int32_t entityId, int16_t x, int32_t y, int32_t z);
void setEntityRotation(int32_t entityId, int32_t x, int32_t y, int32_t z);
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
void clearTextArea(void);
void drawString(char *text, int32_t color, int32_t pos);
int32_t STD_func_80060B98(void);
void STD_tickFighterCounter(void);
void STD_applyEntityViewpoint(void);
void STD_updateFighterStatusVisuals(DigimonEntity *digimon, FighterData *fighter);
void handlePause(void);
void MAIN_func_800E642C(void);
int16_t STD_func_80060620(int32_t a, int32_t b);
int32_t STD_func_8005F354(void);
void STD_func_8005E8A4(Entity *entity, int32_t arg);
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
int32_t popAttackObject(uint8_t entityId, AttackObject *out);
int32_t STD_isPositionNearEntity(Entity *entity, VECTOR *pos);
int32_t BTL_isMoveOnCooldown(Entity *entity, FighterData *fighter);
void BTL_setupMoveExecution(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void STD_func_8005D7C0(POLY_FT4 *prim, int16_t a, int16_t b, int32_t h);
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
void STD_func_8005D814(int32_t x, int32_t y, int32_t tile);
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
void STD_func_8005D550(int32_t i);
void clearTextSubArea(RECT *rect);
void STD_func_80068388(int32_t i);
void STD_func_80059B70(void);

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
	prim->r0 = 0x50;
	prim->g0 = 0xC8;
	prim->b0 = 0x50;
	width = (value * 100) / STD_D_8007A364[idx];
	if (width == 0) {
		width = 1;
	}
	prim->x0 = -0x32;
	prim->y0 = idx * 16 - 0x1A;
	prim->x1 = width - 0x32;
	prim->y1 = idx * 16 - 0x1A;
	prim->x2 = -0x32;
	prim->y2 = idx * 16 - 0x12;
	prim->x3 = width - 0x32;
	prim->y3 = idx * 16 - 0x12;
	AddPrim(&ACTIVE_ORDERING_TABLE->org[3], prim++);
	GsSetWorkBase((PACKET *)prim);
}

void STD_func_80058A60(int16_t x, int16_t y, int16_t size, uint8_t character)
{
	POLY_GT4 *prim;
	uint8_t u;
	uint8_t v;

	prim = (POLY_GT4 *)GsGetWorkBase();

	MAIN_func_80092BB0(prim);
	prim->tpage = 12;
	prim->clut = GetClut(0, 0x1e0);
	prim->r0 = STD_D_8007A370[MAIN_D_8013513C * 3];
	prim->g0 = STD_D_8007A371[MAIN_D_8013513C * 3];
	prim->b0 = STD_D_8007A372[MAIN_D_8013513C * 3];
	prim->r1 = STD_D_8007A370[MAIN_D_8013513C * 3];
	prim->g1 = STD_D_8007A371[MAIN_D_8013513C * 3];
	prim->b1 = STD_D_8007A372[MAIN_D_8013513C * 3];
	prim->r2 = STD_D_8007A370[MAIN_D_8013513C * 3] / 10;
	prim->g2 = STD_D_8007A371[MAIN_D_8013513C * 3] / 10;
	prim->b2 = STD_D_8007A372[MAIN_D_8013513C * 3] / 10;
	prim->r3 = STD_D_8007A370[MAIN_D_8013513C * 3] / 10;
	prim->g3 = STD_D_8007A371[MAIN_D_8013513C * 3] / 10;
	prim->b3 = STD_D_8007A372[MAIN_D_8013513C * 3] / 10;

	u = (character % 32) * 32;
	v = (character / 8) * 32;

	if (size < 64) {
		prim->u0 = u;
		prim->v0 = v;
		prim->u1 = u + (u != 0xe0 ? 32 : 31);
		prim->v1 = v;
		prim->u2 = u;
		prim->v2 = v + (v != 0xe0 ? 32 : 31);
		prim->u3 = u + (u != 0xe0 ? 32 : 31);
		prim->v3 = v + (v != 0xe0 ? 32 : 31);
	} else {
		prim->u0 = u;
		prim->v0 = v;
		prim->u1 = u + 31;
		prim->v1 = v;
		prim->u2 = u;
		prim->v2 = v + 31;
		prim->u3 = u + 31;
		prim->v3 = v + 31;
	}

	prim->x0 = x;
	prim->y0 = y;
	prim->x1 = x + size;
	prim->y1 = y;
	prim->x2 = x;
	prim->y2 = y + size;
	prim->x3 = x + size;
	prim->y3 = y + size;

	GsSetWorkBase((PACKET *)prim);
}

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_80058E28);

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
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
		prim->tpage = 13;
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
		MAIN_D_801B1C14[1] += 0x14;
	}
	if (pad & 0x4000) {
		MAIN_D_801B1C14[1] -= 0x14;
	}
	if (pad & 0x8000) {
		MAIN_D_801B1C14[0] += 0x14;
	}
	if (pad & 0x2000) {
		MAIN_D_801B1C14[0] -= 0x14;
	}
	if (pad & 0x4) {
		MAIN_D_801B1C14[2] -= 0x14;
	}
	if (pad & 0x1) {
		MAIN_D_801B1C14[2] += 0x14;
	}
	if (pad & 0x20) {
		MAIN_D_801B1C0C[0] += 0x20;
		MAIN_D_801B1C0C[0] &= 0xfff;
	}
	if (pad & 0x10) {
		MAIN_D_801B1C0C[0] -= 0x20;
		MAIN_D_801B1C0C[0] &= 0xfff;
	}
	if (pad & 0x80) {
		MAIN_D_801B1C0C[1] -= 0x20;
		MAIN_D_801B1C0C[1] &= 0xfff;
	}
	if (pad & 0x40) {
		MAIN_D_801B1C0C[1] += 0x20;
		MAIN_D_801B1C0C[1] &= 0xfff;
	}
	if (pad & 0x800) {
		MAIN_D_801B1C0C[0] = 0;
		MAIN_D_801B1C0C[1] = 0;
		MAIN_D_801B1C0C[2] = 0;
		MAIN_D_801B1C14[0] = 0;
		MAIN_D_801B1C14[1] = 0;
		MAIN_D_801B1C14[2] = 0xbb8;
	}
	MAIN_D_801B1BB8[0] = 0;
	RotMatrix((SVECTOR *)MAIN_D_801B1C0C, (MATRIX *)MAIN_D_801B1B98);
	TransMatrix((MATRIX *)MAIN_D_801B1B98, (VECTOR *)MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2((GsVIEW2 *)MAIN_D_801B1B98);
}

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_80059B70);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_80059DBC);

void STD_setCameraToEntity(void)
{
	SVECTOR rot;
	VECTOR v;
	VECTOR out;

	rot = *(SVECTOR *)MAIN_D_801B1C0C;
	rot.vy -= ((Entity *)MAIN_D_80135128)->posData->rotation.vy;
	rot.vy &= 0xfff;
	RotMatrix(&rot, (MATRIX *)MAIN_D_801B1B98);
	v = *(VECTOR *)MAIN_D_801B1C14;
	ApplyMatrixLV((MATRIX *)MAIN_D_801B1B98, &((Entity *)MAIN_D_80135128)->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix((MATRIX *)MAIN_D_801B1B98, &v);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2((GsVIEW2 *)MAIN_D_801B1B98);
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

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005A054);

void STD_applyViewpoint(void)
{
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_func_8005A1BC(void)
{
	int32_t pad;

	if ((pad = MAIN_D_80135124) & 0x1000) {
		MAIN_D_801B1C14[1] += 0x14;
	}
	if (pad & 0x4000) {
		MAIN_D_801B1C14[1] -= 0x14;
	}
	if (pad & 0x8000) {
		MAIN_D_801B1C14[0] += 0x14;
	}
	if (pad & 0x2000) {
		MAIN_D_801B1C14[0] -= 0x14;
	}
	if (pad & 0x4) {
		MAIN_D_801B1C14[2] -= 0x14;
	}
	if (pad & 0x1) {
		MAIN_D_801B1C14[2] += 0x14;
	}
	if (pad & 0x20) {
		MAIN_D_801B1C0C[0] += 0x20;
		MAIN_D_801B1C0C[0] &= 0xfff;
	}
	if (pad & 0x10) {
		MAIN_D_801B1C0C[0] -= 0x20;
		MAIN_D_801B1C0C[0] &= 0xfff;
	}
	MAIN_D_801B1C0C[1] += 2;
	MAIN_D_801B1C0C[1] &= 0xfff;
	if (pad & 0x40) {
		MAIN_D_801B1C0C[1] += 0x20;
		MAIN_D_801B1C0C[1] &= 0xfff;
	}
	MAIN_D_801B1BB8[0] = 0;
	RotMatrix((SVECTOR *)MAIN_D_801B1C0C, (MATRIX *)MAIN_D_801B1B98);
	TransMatrix((MATRIX *)MAIN_D_801B1B98, (VECTOR *)MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2((GsVIEW2 *)MAIN_D_801B1B98);
}
void STD_setCameraParams(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f)
{
	MAIN_D_801B1C0C[0] = a;
	MAIN_D_801B1C0E[0] = b;
	MAIN_D_801B1C10[0] = c;
	MAIN_D_801B1C14[0] = d;
	MAIN_D_801B1C18[0] = e;
	MAIN_D_801B1C1C[0] = f;
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
	MAIN_D_801B1BB8[0] = 0;
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

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_tickCameraChase);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_startCameraChase);

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

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005B688);

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
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	prim->tpage = 0xD;
	prim->clut = GetClut(0x10, 0x1E0);
	setUVDataPolyFT4(prim, 0x78, 0x30, 8, 0x12);
	setPosDataPolyFT4(prim, -0x1C, -0x63, 8, 0x12);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	SetPolyFT4(prim);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	prim->tpage = 0xD;
	prim->clut = GetClut(0x10, 0x1E0);
	setUVDataPolyFT4(prim, 0x88, 0x30, 8, 0x12);
	setPosDataPolyFT4(prim, 0x14, -0x63, 8, 0x12);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	width = digits;
	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		SetPolyFT4(prim);
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
		prim->tpage = 0xD;
		prim->clut = GetClut(0x10, 0x1E0);
		setUVDataPolyFT4(prim, buf[i] * 12, 0x30, 0xC, 0xF);
		setPosDataPolyFT4(prim, x + ((((int32_t)width - 1) - i) * 14), y, 0xC, 0xF);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	SetPolyFT4(prim);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
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

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005C1E4);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005CE9C);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005D398);

void STD_func_8005D538(int32_t i)
{
	addObject(0x1B0, i, STD_func_8005E004, 0);
}

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005D550);

void STD_func_8005D7A8(int32_t i)
{
	removeObject(0x1B0, i);
}

void STD_func_8005D7B4(int32_t i)
{
	removeObject(0x1AF, i);
}

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005D7C0);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005D814);

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

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005D9F4);

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

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005E004);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005E124);

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
	prim->r0 = MAIN_D_80135165;
	prim->g0 = MAIN_D_80135165;
	prim->b0 = MAIN_D_80135165;
	setUVDataPolyFT4(prim, 0xFA, 0xFD, 2, 2);
	setPosDataPolyFT4(prim, -0xA0, -0x78, 0x140, 0xF0);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[3], prim++);
	SetPolyFT4(prim);
	if (MAIN_D_80135164 != 0x80) {
		SetSemiTrans(prim, 1);
	}
	prim->r0 = MAIN_D_80135166;
	prim->g0 = MAIN_D_80135166;
	prim->b0 = MAIN_D_80135166;
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

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005E8A4);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005EF84);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005F354);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005F650);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_8005FDDC);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_800602A8);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_80060620);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_80060998);

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
	*(int8_t *)&entity->flatSprite = -1;
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

	startPos.x = -10;
	startPos.y = -10;
	startPos.w = 20;
	startPos.h = 20;

	finalPos.x = -132;
	finalPos.y = -27;
	finalPos.w = 264;
	finalPos.h = 54;

	createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, NULL, (RenderFunction)STD_func_80063300);
}
void STD_faintDigimon(DigimonEntity *digimon, FighterData *fighter, int16_t arg2)
{
	*(int8_t *)&digimon->stats.current.unk2_2 = 1;
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
			digimon->stats.current.unk2_2 = 0;
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
	STD_func_8005E8A4(entity, arg);
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
	prim->tpage = 0xD;
	prim->clut = GetClut(0x10, 0x1E0);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
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
	prim->tpage = 0xc;
	prim->clut = GetClut(0, 0x1e1);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	prim->u0 = id * 48 + 0xa0;
	prim->v0 = 0xe0;
	prim->u1 = (id * 48 + 0xa0) + (id == 0 ? 0x30 : 0x2f);
	prim->v1 = 0xe0;
	prim->u2 = id * 48 + 0xa0;
	prim->v2 = 0xff;
	prim->u3 = (id * 48 + 0xa0) + (id == 0 ? 0x30 : 0x2f);
	prim->v3 = 0xff;
	prim->x0 = sxy[0];
	prim->y0 = sxy[1] - 0x20;
	prim->x1 = sxy[0] + (id == 0 ? 0x30 : 0x2f);
	prim->y1 = sxy[1] - 0x20;
	prim->x2 = sxy[0];
	prim->y2 = sxy[1] - 1;
	prim->x3 = sxy[0] + (id == 0 ? 0x30 : 0x2f);
	prim->y3 = sxy[1] - 1;
	AddPrim(ACTIVE_ORDERING_TABLE->org + otz, prim++);

	GsSetWorkBase((PACKET *)prim);
}

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_800647F8);

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

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_calculateDamage);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_handleHitReaction);

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

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_80067744);

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_80067A30);

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

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_func_800658B4);

void STD_removeMoveEffect(DigimonEntity *digimon, FighterData *fighter)
{
	int32_t id;

	id = fighter->unk11;
	do {
		if (id != -1) {
			STD_stopEFESubEffect(id, *(int8_t *)&digimon->stats.current.unk2_1);
		}
	} while (0);
	*(int8_t *)&digimon->stats.current.unk2_1 = -1;
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
					stats->current.unk2_2 = 1;
				}
				break;
			case 3:
				*(int8_t *)&entity->flatSprite = -1;
				break;
			case 0:
				fighter->flags &= 0xffb7;
				stats->current.unk2_2 = 0;
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
				stats->current.unk2_2 = 1;
				break;
			case 3:
				entity->flatSprite = 0;
				break;
			case 0:
				fighter->flags |= 8;
				fighter->flatTimer = random(0x51) + 0xe0;
				stats->current.unk2_2 = 0;
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

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_getMostEffectiveMove);

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
	rect.w = 0x90;
	rect.h = 0xc;
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

void STD_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, int32_t index)
{
	int16_t c;
	int16_t eff;
	int16_t n;

	if ((index >= 8U) && (index < 0xcU)) {
		eff = MOVE_DATA[entityGetTechFromAnim(&digimon->entity, digimon->stats.base.moves[index - 8])].special;
		prim->u0 = STD_D_8007A708[eff * 2];
		prim->v0 = STD_D_8007A709[eff * 2];
		prim->u1 = STD_D_8007A708[eff * 2] + 0x10;
		prim->v1 = STD_D_8007A709[eff * 2];
		prim->u2 = STD_D_8007A708[eff * 2];
		prim->v2 = STD_D_8007A709[eff * 2] + 0xf;
		prim->u3 = STD_D_8007A708[eff * 2] + 0x10;
		prim->v3 = STD_D_8007A709[eff * 2] + 0xf;
	} else {
		prim->u0 = STD_D_8007A6F8[(index - 1) * 2];
		prim->v0 = STD_D_8007A6F9[(index - 1) * 2];
		prim->u1 = STD_D_8007A6F8[(index - 1) * 2] + 0x10;
		prim->v1 = STD_D_8007A6F9[(index - 1) * 2];
		prim->u2 = STD_D_8007A6F8[(index - 1) * 2];
		prim->v2 = STD_D_8007A6F9[(index - 1) * 2] + 0xf;
		prim->u3 = STD_D_8007A6F8[(index - 1) * 2] + 0x10;
		prim->v3 = STD_D_8007A6F9[(index - 1) * 2] + 0xf;
	}
}

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_addCommandMenu);

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

INCLUDE_ASM("asm/std/nonmatchings/std_main", STD_renderCommandMenu);

void STD_removeCommandMenu(int32_t i)
{
	MAIN_D_8013518A[i] = 0;
	removeObject(0x198, i);
}
