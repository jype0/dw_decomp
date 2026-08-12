#include <libetc.h>
#include <libgpu.h>
#include <libgte.h>
#include <mwinline_n.h>

#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/sound_async.h>
#include <dw/swap.h>

#include "common.h"

typedef struct {
	int16_t flags[4];
	int16_t enemies[5];
	int16_t count;
} TargetChoice;

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
	int16_t enemies[4];
	int16_t flags[5];
	int16_t score;
	int16_t choice;
	int16_t count;
} TargetSearch;

extern int8_t PARTNER_PREVIOUS_TILE_X;
extern int8_t PARTNER_PREVIOUS_TILE_Y;
extern uint8_t MAIN_D_80125F70[][7];
extern int32_t COMBAT_AREA_CENTER_X;
extern int32_t COMBAT_AREA_CENTER_Y;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[];
extern PACKET GS_WORK_BASES[];
extern char DRAW_OFFSETS[];
extern DigimonEntity *MAIN_D_80134EF4;
extern DigimonEntity *MAIN_D_80134EF8;
extern uint8_t CURRENT_SCREEN;
extern uint8_t MAIN_D_80134D64;
extern int16_t MAIN_D_801346D8[4];
extern uint8_t MAIN_D_80135094;
extern int8_t PARTNER_WAYPOINT_COUNT;
extern int8_t PARTNER_WAYPOINT_CURRENT;
extern int8_t PARTNER_WAYPOINT_X[];
extern int8_t PARTNER_WAYPOINT_Y[];
extern uint8_t BTL_D_80072ED8[];
extern uint8_t BTL_D_80072ED9[];
extern uint8_t BTL_D_80072EE8[];
extern uint8_t BTL_D_80072EE9[];
extern uint8_t MAIN_D_801346E8[5];
extern uint8_t MAIN_D_801346EC[4];
extern uint8_t MAIN_D_801346F0[4];
extern uint8_t MAIN_D_801346F4[4];

void removeObject(int32_t objectId, int32_t instanceId);
void addObject(int32_t objectId, int32_t instanceId, void *tick, void *render);
void getEntityTileFromModel(Entity *entity, int8_t *outTileX, int8_t *outTileY);
void tickObjects(void);
void renderObjects(void);
void handlePause(void);
void MAIN_func_800E642C(void);
void BTL_tickPartnerAI(void);
void BTL_tickEnemyAI(void);
void BTL_tickFighterStates(void);
void BTL_tickPartnerWaypointTrail(void);
void BTL_increaseSpeedBuffer(FighterData *fighter, Stats *stats);
int32_t isOffScreen(DVECTOR *xy, int16_t w, int16_t h);
void BTL_tickStatusEffects(void);
int32_t BTL_startEFE(int32_t arg0);
int32_t BTL_applyBuffMove(DigimonEntity *digimon, int32_t slot, int32_t anim);
int32_t getMapSoundId(int32_t mapId);
void handleBattleEndBox(void);
void renderLinePrimitive(uint32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t order, uint32_t mode);
void renderTrianglePrimitive(int32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t otz, int32_t flag);
void damageTick(FighterData *fighter, Stats *stats);
void MAIN_func_800E5724(int32_t a, int32_t b, int32_t x, int32_t y, int32_t value, int32_t order);
int32_t BTL_calculateDamage(DigimonEntity *attacker, DigimonEntity *defender, int16_t move);
void BTL_deinitializeCombat(void);
void closeInventoryBoxes(void);
void resetStatsAfterCombat(void);
void removeEntityText(int32_t id);
void BTL_selectEnemyMove(DigimonEntity *digimon, FighterData *fighter, int16_t index);
void BTL_selectPartnerMove(DigimonEntity *digimon, FighterData *fighter, int16_t index);
void BTL_tickChargeState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void BTL_tickAttackHits(void);
void BTL_tickConfusedState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3);
void BTL_initializeFinisherChargeup(int16_t tech);
int32_t BTL_addFinisherAura(Entity *entity, int32_t arg1);
void BTL_setupMoveExecution(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
int16_t BTL_calculateHitChance(DigimonEntity *attacker, DigimonEntity *defender, FighterData *fighter, int16_t move);
void BTL_applyMoveStatus(DigimonEntity *digimon, FighterData *fighter, int32_t move);
int32_t BTL_battleMain(void);
void BTL_getWeakestEnemy(Entity *self, FighterData *fighter, int16_t *outScore, int16_t *outIndex);
int16_t BTL_calculateTargetScore(DigimonEntity *self, DigimonEntity *other);
void BTL_addBuffRingsEffect(int16_t kind, Entity *entity);
void BTL_healStatusEffect(int32_t arg0);
int16_t BTL_getNearestEnemy(Entity *self, int16_t *flags);
void BTL_tickAttackState(Entity *entity, DigimonEntity *target, int32_t id);
void handleBattleIdle(DigimonEntity *digimon, Stats *stats, int32_t flags);
void createParticleFX(uint8_t kind, int32_t count, void *arg2, Entity *entity, int32_t arg4);
void BTL_resetFlatten(int16_t index);
void BTL_handleVictorySequence(void);
int32_t BTL_addPoisonEffect(DigimonEntity *digimon, FighterData *fighter);
int32_t BTL_addConfusionEffect(DigimonEntity *digimon, FighterData *fighter);
int32_t BTL_addStunEffect(DigimonEntity *digimon, int16_t arg1);
void BTL_removePoisonEffect(int32_t id, DigimonEntity *digimon);
void BTL_removeConfusionEffect(int32_t id, DigimonEntity *digimon);
void BTL_removeStunEffect(int32_t id, DigimonEntity *digimon);
uint32_t BTL_getMoveWithHighestDistance(DigimonEntity *digimon);
void BTL_getRemainingEnemies(Entity *self, int16_t *out, int16_t *count);
void BTL_findUnblockedRotation(Entity *entity, int16_t *rot, int16_t hit, int16_t orig);
void collisionGrace(Entity *a, Entity *entity, int32_t c, int32_t d);
void BTL_moveTowardLocation(DigimonEntity *digimon, VECTOR *target, int16_t dx, int16_t dy);
void BTL_selectFighterTarget(DigimonEntity *digimon, FighterData *fighter, uint8_t target, int32_t arg3);
void BTL_clearConfusion(DigimonEntity *digimon, FighterData *fighter);
void BTL_faintDigimon(DigimonEntity *digimon, FighterData *fighter, int16_t arg2);
void BTL_addStunStatusVisual(DigimonEntity *digimon, FighterData *fighter);
int32_t BTL_handlePartnerMoveCommand(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void BTL_tickRangedAttack(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int16_t move);
int32_t BTL_tickMeleeAttack(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int16_t arg3);
void BTL_startQueuedMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void addPartnerWaypoint(int16_t index, int8_t x, int8_t y);
void MAIN_func_800D39F8(void);
void BTL_removeFinisherAura(int32_t arg0);
int32_t popAttackObject(uint8_t entityId, AttackObject *out);
int32_t isFiveTileWidePathOpen(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void BTL_addAuraProjectile(Entity *entity);
void BTL_handleHitReaction(Entity *entity, FighterData *fighter, AttackObject *attack, int16_t index);
void BTL_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, int32_t index);
void BTL_retargetAfterHit(DigimonEntity *digimon, FighterData *fighter, AttackObject attack);
void BTL_applyFlattenScale(VECTOR *scale, int32_t t);
void BTL_tickQueuedMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3);
void BTL_tickFlatState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3);
void BTL_confusedRotate(Entity *entity);
void BTL_startHitAnimation(Entity *entity, AttackObject *attack, int32_t animId);
void BTL_removeStatusEffects(DigimonEntity *digimon, FighterData *fighter);
void BTL_applyChargeRequirement(DigimonEntity *digimon, FighterData *fighter, int16_t tech);
int32_t BTL_addBlockedAttack(FighterData *fighter, FighterData *other);
void BTL_setupQueuedMove(DigimonEntity *digimon, FighterData *fighter, int16_t arg2, int32_t moveIndex);
void BTL_tickFighterAction(int32_t arg0);
int16_t BTL_getStrongestMove(int32_t arg0, int16_t *flags);
int16_t BTL_getMostEffectiveMove(int32_t arg0, int16_t *flags);
int16_t BTL_getCheapestMove(int32_t arg0, int16_t *flags);
void BTL_tickHitState(Entity *entity, FighterData *fighter, int32_t arg2);
int32_t BTL_selectMoveByPower(int32_t arg0, int16_t *flags);
int32_t BTL_selectMoveByMpCost(int32_t arg0, int16_t *flags);
void addWithLimit(int16_t *stat, int32_t value, int32_t limit);
void addEntityText(DigimonEntity *digimon, int32_t slot, int16_t color, int32_t value, uint8_t flag);
void BTL_buffStats(DigimonEntity *digimon, int32_t slot, int16_t value, int16_t *stat, int16_t color, uint8_t flag);
int16_t BTL_getRandomUsableMove(int16_t *flags);
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
int32_t BTL_getContactRangeSquared(int32_t *a, int32_t *b);
void BTL_maintainTargetDistance(DigimonEntity *attacker, DigimonEntity *target, FighterData *fighter);
void BTL_maintainDistanceRange(DigimonEntity *attacker, DigimonEntity *target, FighterData *fighter, uint32_t arg3, uint32_t arg4);
int32_t BTL_calculateElementBonus(int32_t arg0, int32_t arg1);
void BTL_applySquashScale(VECTOR *scale, int32_t angle);
void BTL_healFlatten(Entity *entity, FighterData *fighter, int32_t arg2);
void BTL_addConfusionStatusVisual(DigimonEntity *digimon, FighterData *fighter);
int32_t BTL_isMoveOnCooldown(Entity *entity, FighterData *fighter);
void BTL_calculateScoreRanks(int32_t *values, int32_t *groups, int32_t count);
void BTL_stopEFESubEffect(int32_t arg0, int8_t arg1);
void BTL_removeStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, int32_t arg2);
uint8_t BTL_isFighterDefeated(uint8_t index);
void BTL_tickSenileState(Entity *entity, FighterData *fighter);
void BTL_removeMoveEffect(DigimonEntity *digimon, FighterData *fighter);
void BTL_applyStretchScale(VECTOR *scale, int32_t angle);
void BTL_clearPoison(DigimonEntity *digimon, FighterData *fighter);
int16_t BTL_getNpcEntityIndex(Entity *entity);
int32_t BTL_countLivingEnemies(void);
void BTL_tickStunState(Entity *entity);
void BTL_clearBlockedAttacks(FighterData *fighter);
int16_t BTL_getFighterSlot(int16_t entityId);
int32_t BTL_getDistanceSquared(Entity *a, Entity *b);
void BTL_addStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, int32_t arg2);
void BTL_addPoisonStatusVisual(DigimonEntity *digimon, FighterData *fighter);
void BTL_removeEnemyHPBars(void);
void BTL_setWalking(Entity *entity, Stats *stats, uint16_t flags);
void BTL_startWalkingAnimation(Entity *entity, Stats *stats, uint16_t flags);
void BTL_renderEnemyHPBars(void);
void BTL_resetFighterAction(FighterData *fighter);
void BTL_setFighterCooldown(DigimonEntity *digimon, FighterData *fighter);
int32_t BTL_getUsableMoves(int16_t *out, int16_t index);
int16_t BTL_getAttackTech(AttackObject *attack);
void BTL_addFinisherProgress(FighterData *fighter, int16_t amount);
void BTL_getStatusAfflictedEnemies(Entity *self, int16_t *out, int16_t *count);
void BTL_sortScoresDescending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count);
void BTL_sortScoresAscending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count);
void BTL_removeCombatObjects(void);
int32_t BTL_hasAffordableMoves(int16_t *out, uint8_t index);
int32_t BTL_isMoveUsable(DigimonEntity *digimon, FighterData *fighter, int16_t slot);
void BTL_tickCooldownState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void BTL_clearStun(DigimonEntity *digimon, FighterData *fighter);
void BTL_backAwayFromTarget(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void removeBuffModelObject(void);
void BTL_getHighestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count);
void BTL_getLowestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count);
void BTL_selectConfusedMove(DigimonEntity *digimon, FighterData *fighter, int32_t tech);

static void *battle_main_functions[] = {
	BTL_setCommandIconUV,
	BTL_selectPartnerMove,
	BTL_selectEnemyMove,
	BTL_getNearestEnemy,
	BTL_isFighterDefeated,
	BTL_selectFighterTarget,
	BTL_selectMoveByMpCost,
	BTL_selectMoveByPower,
	BTL_getRemainingEnemies,
	BTL_selectConfusedMove,
	BTL_calculateScoreRanks,
	BTL_countLivingEnemies,
	BTL_calculateElementBonus,
	BTL_sortScoresAscending,
	BTL_sortScoresDescending,
	BTL_calculateTargetScore,
	BTL_getWeakestEnemy,
	BTL_getStatusAfflictedEnemies,
	BTL_getNpcEntityIndex,
	BTL_getLowestScoredMove,
	BTL_getHighestScoredMove,
	BTL_getCheapestMove,
	BTL_getMostEffectiveMove,
	BTL_getStrongestMove,
	BTL_getRandomUsableMove,
	BTL_setFighterCooldown,
	BTL_getUsableMoves,
	BTL_healStatusEffect,
	BTL_removeStatusEffects,
	BTL_addStunStatusVisual,
	BTL_addConfusionStatusVisual,
	BTL_addPoisonStatusVisual,
	BTL_resetFighterAction,
	BTL_tickStatusEffects,
	BTL_healFlatten,
	BTL_clearPoison,
	BTL_removeStatusEffectVisual,
	BTL_applySquashScale,
	BTL_applyStretchScale,
	BTL_addStatusEffectVisual,
	BTL_applyFlattenScale,
	BTL_clearStun,
	BTL_clearConfusion,
	BTL_addFinisherProgress,
	BTL_battleTickFrame,
	BTL_removeMoveEffect,
	BTL_setupMoveExecution,
	BTL_isMoveOnCooldown,
	BTL_startQueuedMove,
	BTL_applyChargeRequirement,
	BTL_setupQueuedMove,
	BTL_getDistanceSquared,
	BTL_isMoveUsable,
	BTL_renderEnemyHPBars,
	BTL_buffStats,
	BTL_tickAttackHits,
	BTL_startHitAnimation,
	BTL_addBlockedAttack,
	BTL_getFighterSlot,
	BTL_applyMoveStatus,
	BTL_handleHitReaction,
	BTL_calculateDamage,
	BTL_retargetAfterHit,
	BTL_calculateHitChance,
	BTL_applyBuffMove,
	BTL_getAttackTech,
	BTL_battleMain,
	BTL_findUnblockedRotation,
	BTL_clearBlockedAttacks,
	BTL_addEnemyHPBars,
	BTL_initializeEnemyHPBarSprites,
	BTL_initializePartnerTile,
	BTL_startWalkingAnimation,
	BTL_hasAffordableMoves,
	BTL_faintDigimon,
	BTL_increaseSpeedBuffer,
	BTL_getContactRangeSquared,
	BTL_maintainDistanceRange,
	BTL_maintainTargetDistance,
	BTL_confusedRotate,
	BTL_tickFighterAction,
	BTL_backAwayFromTarget,
	BTL_getMoveWithHighestDistance,
	BTL_moveTowardLocation,
	BTL_setWalking,
	BTL_tickRangedAttack,
	BTL_tickMeleeAttack,
	BTL_handlePartnerMoveCommand,
	BTL_tickQueuedMove,
	BTL_tickCooldownState,
	BTL_tickChargeState,
	BTL_tickSenileState,
	BTL_tickConfusedState,
	BTL_tickStunState,
	BTL_tickFlatState,
	BTL_tickHitState,
	BTL_tickAttackState,
	BTL_handleVictorySequence,
	BTL_resetFlatten,
	BTL_removeEnemyHPBars,
	BTL_removeCombatObjects,
	BTL_deinitializeCombat,
	BTL_tickPartnerWaypointTrail,
	BTL_tickFighterStates,
	BTL_tickEnemyAI,
	BTL_tickPartnerAI,
};

void BTL_tickPartnerAI(void)
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
			BTL_increaseSpeedBuffer(fighter, stats);
		}
	}

	if (*flagsPtr & 0x80b0) {
		return;
	}

	if (stats->current.currentHP == 0) {
		BTL_faintDigimon((DigimonEntity *)partner, fighter, 0);
		return;
	}

	BTL_getRemainingEnemies(partner, enemies, &count);
	if (count == 0) {
		handleBattleIdle((DigimonEntity *)partner, stats, *flagsPtr);
		fighter->moveRange = -1;
		BTL_resetFlatten(0);
		BTL_removeStatusEffects((DigimonEntity *)partner, fighter);
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
					fighter->targetId = BTL_getNearestEnemy(partner, targetFlags);
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
			if (BTL_isMoveUsable((DigimonEntity *)partner, fighter, COMBAT_DATA_PTR->player.currentCommand[0] - 8) == 0) {
				break;
			}
			if ((fighter->targetId != 0xff) && (BTL_isFighterDefeated(fighter->targetId) != 0)) {
				BTL_selectFighterTarget((DigimonEntity *)partner, fighter, fighter->targetId, 0);
			}
			index = COMBAT_DATA_PTR->player.currentCommand[0] - 8;
			if ((((DigimonEntity *)partner)->stats.base.moves[COMBAT_DATA_PTR->player.currentCommand[0] - 8] != fighter->queuedAnim) || (fighter->moveRange <= 0)) {
				BTL_setupQueuedMove((DigimonEntity *)partner, fighter, 0, (COMBAT_DATA_PTR->player.currentCommand[0] - 8) & 0xff);
			}
			BTL_applyChargeRequirement((DigimonEntity *)partner, fighter, DIGIMON_DATA[partner->type].moves[fighter->queuedAnim - 0x2e]);
			return;
		case 11:
			if ((fighter->targetId != 0) && (BTL_isFighterDefeated(fighter->targetId) != 0)) {
				BTL_selectFighterTarget((DigimonEntity *)partner, fighter, fighter->targetId, 0);
			}
			if ((((DigimonEntity *)partner)->stats.base.moves[3] != fighter->queuedAnim) || (fighter->moveRange <= 0)) {
				BTL_setupQueuedMove((DigimonEntity *)partner, fighter, 0, 3);
			}
			return;
		}
		if ((COMBAT_DATA_PTR->player.currentCommand[0] != 2) && (COMBAT_DATA_PTR->player.currentCommand[0] != 4)) {
			((DigimonEntity *)partner)->stats.current.chargeMode = MAIN_D_80135078;
		}
	}

	if ((*flagsPtr & 8) && ((MAIN_D_80134D66 % 100) == 0)) {
		fighter->targetId = enemies[random(count)];
	}

	if (*flagsPtr & 0x40) {
		return;
	}

	if (COMBAT_DATA_PTR->player.currentCommand[0] == 1) {
		fighter->moveRange = 1;
		*flagsPtr |= 0x40;
	}

	flags = *flagsPtr;
	if (flags & 8) {
		fighter->queuedAnim = 0;
		BTL_selectFighterTarget((DigimonEntity *)partner, fighter, fighter->targetId, 0);
		fighter->moveRange = 2;
		partner->flatSprite = 0;
		fighter->flags |= 0x40;
		return;
	}

	if (flags & 4) {
		return;
	}

	if (flags & 2) {
		BTL_selectConfusedMove((DigimonEntity *)partner, fighter, 0);
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

	if (!(flags & 0x400)) {
		BTL_selectFighterTarget((DigimonEntity *)partner, fighter, fighter->targetId, 0);
	}

	result = -1;
	switch (COMBAT_DATA_PTR->player.currentCommand[0]) {
	case 2:
		if (BTL_hasAffordableMoves(moveFlags, 0) == 0) {
			fighter->cooldown = 0x50;
			fighter->flags |= 0x800;
			return;
		}
		result = BTL_selectMoveByPower(0, moveFlags);
		((DigimonEntity *)partner)->stats.current.chargeMode = 0;
		break;
	case 4:
		if (BTL_hasAffordableMoves(moveFlags, 0) == 0) {
			fighter->cooldown = 0x50;
			fighter->flags |= 0x800;
			return;
		}
		result = BTL_selectMoveByMpCost(0, moveFlags);
		((DigimonEntity *)partner)->stats.current.chargeMode = 2;
		break;
	}

	if (result == -1) {
		BTL_selectPartnerMove((DigimonEntity *)partner, fighter, 0);
	} else {
		BTL_setupQueuedMove((DigimonEntity *)partner, fighter, 0, result & 0xff);
	}
}

void BTL_tickEnemyAI(void)
{
	DVECTOR out;
	SVECTOR pos;
	CombatData *combat;
	Entity *partner;
	Entity *entity;
	FighterData *fighter;
	Stats *stats;
	uint16_t *flagsPtr;
	uint16_t flags;
	MATRIX *m;
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
		flagsPtr = &fighter->flags;
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		stats = &((DigimonEntity *)entity)->stats;
		base = &stats->base;
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
				BTL_increaseSpeedBuffer(fighter, stats);
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
			BTL_faintDigimon((DigimonEntity *)entity, fighter, i);
			continue;
		}
		if (fighter->hpDamageBuffer >= stats->current.currentHP) {
			handleBattleIdle((DigimonEntity *)entity, stats, flags);
			fighter->moveRange = -1;
			BTL_resetFlatten(i);
			BTL_removeStatusEffects((DigimonEntity *)entity, fighter);
			fighter->flags &= 0xfff0;
			continue;
		}
		if (((DigimonEntity *)partner)->stats.current.currentHP == 0) {
			handleBattleIdle((DigimonEntity *)entity, stats, flags);
			BTL_resetFlatten(i);
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
			BTL_selectConfusedMove((DigimonEntity *)entity, fighter, i);
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
					BTL_setupQueuedMove((DigimonEntity *)entity, fighter, i, 3);
					fighter->finisherProgress = 0;
					continue;
				}
			}
		}
		if (!(*flagsPtr & 0x400)) {
			fighter->targetId = 0;
		}
		GsSetLsMatrix(&GsWSMATRIX);
		m = &entity->posData[1].posMatrix.workm;
		pos.vx = m->t[0];
		pos.vy = m->t[1];
		pos.vz = m->t[2];
		gte_ldv0(&pos);
		gte_rtps();
		gte_stsxy(&out);
		if (isOffScreen(&out, 0xc8, 0xa0) == 1) {
			stats->current.chargeMode = 2;
		} else {
			stats->current.chargeMode = random(2);
		}
		BTL_selectEnemyMove((DigimonEntity *)entity, fighter, i);
	}
}

void BTL_tickFighterStates(void)
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

	combat = (uint32_t)COMBAT_DATA_PTR;
	for (i = 0, fighter = (FighterData *)combat; ENEMY_COUNT >= i; i++, fighter = (FighterData *)((int32_t)fighter + 0x168)) {
		flags = &fighter->flags;
		combat = (uint32_t)COMBAT_DATA_PTR;
		entity = (DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)i + combat))[0x66c]];
		BTL_addFinisherProgress(fighter, 1);
		id = fighter->targetId;

		if (id != 0xff) {
			combat = (uint32_t)COMBAT_DATA_PTR;
			target = (DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)id + combat))[0x66c]];
		} else {
			target = NULL;
		}
		if (*flags & 0x20) {
			BTL_tickAttackState(&entity->entity, target, i);
		} else if ((*flags & 0x10) || (*flags & 0x80)) {
			BTL_tickHitState(&entity->entity, fighter, i);
		} else if (fighter->moveRange != -1) {
			if (*flags & 8) {
				BTL_tickFlatState(entity, target, fighter, i);
			} else if (*flags & 4) {
				BTL_tickStunState(&entity->entity);
			} else if (*flags & 2) {
				BTL_tickConfusedState(entity, target, fighter, i);
			} else if (*flags & 0x2000) {
				BTL_tickSenileState(&entity->entity, fighter);
			} else if (*flags & 0x800) {
				BTL_tickChargeState(entity, target, fighter);
			} else if (*flags & 0x1000) {
				BTL_tickCooldownState(entity, target, fighter);
			} else {
				BTL_tickQueuedMove(entity, target, fighter, i);
			}
		}
	}

	BTL_tickAttackHits();
	BTL_tickStatusEffects();
}

void BTL_tickPartnerWaypointTrail(void)
{
	int8_t tamerTileX;
	int8_t tamerTileY;
	int8_t partnerTileX;
	int8_t partnerTileY;
	int32_t j;
	int32_t n;
	int16_t src;
	int16_t dst;
	int32_t i;

	getEntityTileFromModel(ENTITY_TABLE[1], &partnerTileX, &partnerTileY);
	if ((partnerTileX != PARTNER_PREVIOUS_TILE_X) || (partnerTileY != PARTNER_PREVIOUS_TILE_Y)) {
		getEntityTileFromModel(ENTITY_TABLE[0], &tamerTileX, &tamerTileY);
		if (isFiveTileWidePathOpen(tamerTileX, tamerTileY, partnerTileX, partnerTileY) == 1) {
			if (PARTNER_WAYPOINT_COUNT != 0) {
				if (isFiveTileWidePathOpen(partnerTileX, partnerTileY, PARTNER_WAYPOINT_X[PARTNER_WAYPOINT_CURRENT], PARTNER_WAYPOINT_Y[PARTNER_WAYPOINT_CURRENT]) == 1) {
					n = PARTNER_WAYPOINT_COUNT;
					i = (*(int8_t *)&PARTNER_WAYPOINT_CURRENT) + n;
					while (n > 0) {
						j = i - 1;
						src = j % 30;
						dst = i % 30;
						PARTNER_WAYPOINT_X[dst] = PARTNER_WAYPOINT_X[src];
						PARTNER_WAYPOINT_Y[dst] = PARTNER_WAYPOINT_Y[src];
						n--;
						i = j;
					}
					addPartnerWaypoint(PARTNER_WAYPOINT_CURRENT, PARTNER_PREVIOUS_TILE_X, PARTNER_PREVIOUS_TILE_Y);
				}
			} else {
				MAIN_func_800D39F8();
				addPartnerWaypoint(PARTNER_WAYPOINT_CURRENT, PARTNER_PREVIOUS_TILE_X, PARTNER_PREVIOUS_TILE_Y);
			}
		} else {
			MAIN_func_800D39F8();
		}
	}

	PARTNER_PREVIOUS_TILE_X = partnerTileX;
	PARTNER_PREVIOUS_TILE_Y = partnerTileY;
}

void BTL_deinitializeCombat(void)
{
	Entity *entity;
	int16_t *src;
	Stats *stats;
	int32_t i;
	int32_t j;
	int32_t found;

	PARTNER_ENTITY.digimonEntity.stats.current.chargeMode = MAIN_D_80135078;
	GAME_STATE = 2;
	BTL_removeCombatObjects();
	closeInventoryBoxes();
	for (i = 0; ENEMY_COUNT >= i; i++) {
		BTL_resetFlatten(i);
		BTL_removeStatusEffects((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]], &COMBAT_DATA_PTR->fighter[i]);
		if (i != 0) {
			src = (int16_t *)&INITIAL_COMBAT_STATS[i];
			stats = &((DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)i + (uint32_t)COMBAT_DATA_PTR))[0x66c]])->stats;
			stats->current.currentHP = *src++;
			stats->current.currentMP = *src++;
			stats->base.off = *src++;
			stats->base.def = *src++;
			stats->base.speed = *src++;
			stats->base.brain = *src;
		}
		COMBAT_DATA_PTR->fighter[i].flags = 0;
	}

	if ((PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) && (COMBAT_DATA_PTR->player.currentCommand[0] != 1)) {
		BTL_handleVictorySequence();
	} else {
		resetStatsAfterCombat();
		BTL_removeDeathCountdown();
		for (i = 0; ENEMY_COUNT >= i; i++) {
			removeEntityText(i);
		}
	}

	MAIN_D_80135094 = 0;
	while (BTL_isCommandMenuClosed() == 0) {
		BTL_battleTickFrame();
	}

	BTL_removePartnerStatusBars();
	if (MAIN_D_80134D7C[0] != 0) {
		for (i = 2; i < 10; i++) {
			if (ENTITY_TABLE[i] != NULL) {
				found = 0;
				for (j = 1; ENEMY_COUNT >= j; j++) {
					if (COMBAT_DATA_PTR->player.entityIds[j] == i) {
						found = 1;
					}
				}
				if (found == 0) {
					if (ENTITY_TABLE[i]->isOnScreen == 1) {
						ENTITY_TABLE[i]->isOnMap = 0;
					} else {
						ENTITY_TABLE[i]->isOnMap = 1;
					}
				}
			}
		}
	} else {
		for (i = 2; i < 10; i++) {
			if (ENTITY_TABLE[i] != NULL) {
				entity = ENTITY_TABLE[i];
				if (entity->isOnScreen == 1) {
					entity->anim.animFlag &= 0xfb;
					ENTITY_TABLE[i]->isOnMap = 0;
				} else {
					entity->isOnMap = 1;
				}
			}
		}
	}

	stopBGM();
	stopSound();
}

void BTL_removeCombatObjects(void)
{
	int32_t i;

	BTL_removeAllFinisherAuras();
	BTL_removeAllPoisonEffects();
	BTL_removeAllStunEffects();
	BTL_removeAllBuffRingsEffects();
	BTL_removeAllBuffDiskEffects();
	BTL_removeAllItemParticles();
	removeBuffModelObject();
	BTL_removeEnemyHPBars();
	BTL_removeAllAuraProjectiles();

	for (i = 0; ENEMY_COUNT >= i; i++) {
		BTL_removeMoveEffect((DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)i + (uint32_t)COMBAT_DATA_PTR))[0x66c]], &COMBAT_DATA_PTR->fighter[i]);
	}

	BTL_unloadAllEFESlots();
	BTL_removeEFEEngine();
}

void BTL_removeEnemyHPBars(void)
{
	removeObject(0x191, 0);
}

void BTL_resetFlatten(int16_t index)
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

void BTL_handleVictorySequence(void)
{
	VECTOR loc;
	SVECTOR tmp;
	Entity *entity;
	Stats *stats;
	int32_t t;
	int32_t i;
	int32_t sound;
	int32_t n;
	int32_t ready;
	int32_t done;

	n = 0;
	ready = 0;
	startAnimation(ENTITY_TABLE[0], 1);
	startAnimation(ENTITY_TABLE[1], 0x21);
	stopBGM();
	stopSound();
	playSound(0, 0x15);

	t = 0;
	sound = loadMapSounds2(getMapSoundId(CURRENT_SCREEN));
	for (;;) {
		if ((isSoundLoaded(1, sound) == -1) || (t < 0x29)) {
			BTL_battleTickFrame();
			t++;
		} else {
			break;
		}
	}

	playSound(0, 0x18);
	startAnimation(ENTITY_TABLE[0], 0xb);
	startAnimation(ENTITY_TABLE[1], 0x2a);
	stats = &PARTNER_ENTITY.digimonEntity.stats;
	while ((ready == 0) || (n == 0)) {
		if (!(ENTITY_TABLE[1]->anim.animFlag & 1)) {
			n = 1;
			startBattleIdleAnimation(&PARTNER_ENTITY.digimonEntity, stats, COMBAT_DATA_PTR->fighter[0].flags);
		}
		if (!(ENTITY_TABLE[0]->anim.animFlag & 1)) {
			ready = 1;
			startAnimation(ENTITY_TABLE[0], 1);
		}
		BTL_battleTickFrame();
	}

	handleBattleEndBox();
	for (i = 1; ENEMY_COUNT >= i; i++) {
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		startAnimation(entity, 0x2c);
	}

	if (MAIN_D_80134D7C[0] != 0) {
		for (;;) {
			for (i = 1; ENEMY_COUNT >= i; i++) {
				if (!(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]]->anim.animFlag & 1)) {
					break;
				}
			}
			if (i == (ENEMY_COUNT + 1)) {
				break;
			}
			BTL_battleTickFrame();
		}
		for (i = 0; ENEMY_COUNT >= i; i++) {
			startAnimation(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]], 0x21);
		}
		return;
	}

	done = 0;
	while (done == 0) {
		n = 0;
		for (i = 1; ENEMY_COUNT >= i; i++) {
			entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
			if (!(entity->anim.animFlag & 1) && (entity->anim.animId == 0x2c)) {
				startAnimation(entity, 0x24);
				entity->anim.animFlag |= 4;
				tmp = ((NPCEntity *)entity)->flee;
				loc.vx = tmp.vx;
				loc.vy = tmp.vy;
				loc.vz = tmp.vz;
				entityLookAtLocation(entity, &loc);
			}
			if (entityIsOffScreen(entity, 0x140, 0xf0) != 0) {
				n++;
			}
		}
		if (n == ENEMY_COUNT) {
			done = 1;
		}
		BTL_battleTickFrame();
	}
}

void BTL_tickAttackState(Entity *entity, DigimonEntity *target, int32_t id)
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

	BTL_tickFighterAction(id);
}

void BTL_tickHitState(Entity *entity, FighterData *fighter, int32_t arg2)
{
	BTL_tickFighterAction(arg2);
	if (!(entity->anim.animFlag & 1)) {
		fighter->invulnerableTimer--;
		if (fighter->invulnerableTimer == 0) {
			entity->anim.animFlag |= 1;
		}
	}
}

void BTL_tickFlatState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3)
{
	if (MAIN_D_80134D74 != 0) {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		return;
	}

	if (BTL_handlePartnerMoveCommand(digimon, target, fighter) == 0) {
		BTL_tickRangedAttack(digimon, target, fighter, 0x79);
		if (fighter->flags & 0x20) {
			digimon->entity.flatSprite = 2;
		}
	}
}

void BTL_tickStunState(Entity *entity)
{
	if (entity->anim.animId != 0x22) {
		startAnimation(entity, 0x22);
	}

	entity->anim.animFlag &= 0xfe;
}

void BTL_tickConfusedState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3)
{
	if (MAIN_D_80134D74 != 0) {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		return;
	}

	if ((fighter->flags & 0x1000) || (fighter->flags & 0x800)) {
		BTL_confusedRotate(&digimon->entity);
		BTL_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
		collisionGrace(NULL, &digimon->entity, 0x118, 0xc8);
		if (fighter->cooldown < 2) {
			fighter->flags &= 0xefff;
			fighter->cooldown = 0;
		}
		return;
	}

	if (target == NULL) {
		BTL_confusedRotate(&digimon->entity);
		if (BTL_tickMeleeAttack(digimon, NULL, fighter, arg3) != 0) {
			collisionGrace(NULL, &digimon->entity, 0x118, 0xc8);
		}
		if (fighter->flags & 0x20) {
			return;
		}
		if (random(100) >= 5) {
			return;
		}
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		BTL_startQueuedMove(digimon, target, fighter);
		return;
	}

	switch (fighter->moveRange) {
	case 1:
		if (BTL_tickMeleeAttack(digimon, target, fighter, arg3) != 0) {
			collisionGrace(&target->entity, &digimon->entity, 0x118, 0xc8);
		}
		break;
	case 2:
	case 3:
		BTL_tickRangedAttack(digimon, target, fighter, entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim));
		break;
	case 4:
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		BTL_startQueuedMove(digimon, target, fighter);
		break;
	}
}

void BTL_tickSenileState(Entity *entity, FighterData *fighter)
{
	fighter->senileTimer--;
	if (fighter->senileTimer == 0) {
		fighter->flags &= 0xdfbf;
		return;
	}

	if (entity->anim.animId != 0x13) {
		startAnimation(entity, 0x13);
	}
}

void BTL_tickChargeState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	int32_t r;
	int16_t tech;

	if (MAIN_D_80134D74 != 0) {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		return;
	}

	r = BTL_handlePartnerMoveCommand(digimon, target, fighter);
	if (fighter->cooldown != 0) {
		if (r == 0) {
			switch (digimon->stats.current.chargeMode) {
			case 0:
				BTL_maintainTargetDistance(digimon, target, fighter);
				break;
			case 1:
				handleBattleIdle(digimon, &digimon->stats, fighter->flags);
				entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
				fighter->unk16 = 0;
				break;
			case 2:
				BTL_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
				BTL_backAwayFromTarget(digimon, target, fighter);
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
		BTL_maintainTargetDistance(digimon, target, fighter);
		tech = entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim);
		if ((fighter->speedBuffer == 100) || (fighter->speedBuffer >= MOVE_DATA[tech].power)) {
			fighter->flags &= 0xf7ff;
		}
		break;
	case 2:
		BTL_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
		BTL_backAwayFromTarget(digimon, target, fighter);
		if (fighter->speedBuffer == 100) {
			fighter->flags &= 0xf7ff;
		}
		break;
	}
}

void BTL_tickCooldownState(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	if (MAIN_D_80134D74 != 0) {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		return;
	}

	switch (digimon->stats.current.chargeMode) {
	case 0:
	case 1:
		BTL_maintainTargetDistance(digimon, target, fighter);
		break;
	case 2:
		BTL_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
		BTL_backAwayFromTarget(digimon, target, fighter);
		break;
	}

	if (fighter->cooldown < 2) {
		fighter->flags &= ~0x1040;
		fighter->cooldown = 0;
	}
}

void BTL_tickQueuedMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int32_t arg3)
{
	if ((MAIN_D_80134D74 != 0) && (MAIN_D_80134D60 != &digimon->entity)) {
		handleBattleIdle(digimon, &digimon->stats, fighter->flags);
		return;
	}

	if (BTL_handlePartnerMoveCommand(digimon, target, fighter) == 0) {
		switch (fighter->moveRange) {
		case 1:
			if (BTL_tickMeleeAttack(digimon, target, fighter, arg3) != 0) {
				collisionGrace(&target->entity, &digimon->entity, 0x118, 0xc8);
			}
			break;
		case 2:
		case 3:
			BTL_tickRangedAttack(digimon, target, fighter, DIGIMON_DATA[digimon->entity.type].moves[fighter->queuedAnim - 0x2e]);
			break;
		case 4:
			handleBattleIdle(digimon, &digimon->stats, fighter->flags);
			BTL_startQueuedMove(digimon, target, fighter);
			break;
		}
	}
}

int32_t BTL_handlePartnerMoveCommand(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	uint32_t reach;

	if (MAIN_D_80134D74 != 0) {
		return 0;
	}

	if (digimon == (DigimonEntity *)ENTITY_TABLE[1]) {
		switch (COMBAT_DATA_PTR->player.currentCommand[0]) {
		case 1:
			BTL_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
			BTL_moveTowardLocation(digimon, &ENTITY_TABLE[0]->posData->location, 0, 0);
			if (entityIsOffScreen(ENTITY_TABLE[1], 0x140, 0xf0) != 0) {
				startAnimation(ENTITY_TABLE[1], 0x21);
				ENTITY_TABLE[1]->anim.animFlag |= 2;
			}
			fighter->unk16 = 0;
			return 1;
		case 6:
			handleBattleIdle(digimon, &digimon->stats, fighter->flags);
			entityLookAtLocation(&digimon->entity, &ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighter->targetId]]->posData->location);
			fighter->unk16 = 0;
			return 1;
		case 5:
			reach = BTL_getMoveWithHighestDistance(target) + 0x9c400;
			if ((digimon->entity.anim.animId >= 0x23) && (digimon->entity.anim.animId < 0x25)) {
				reach += 0x27100;
			}
			if (BTL_getDistanceSquared(&digimon->entity, &target->entity) < reach) {
				BTL_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
				BTL_backAwayFromTarget(digimon, target, fighter);
			} else {
				fighter->unk16 = 0;
				handleBattleIdle(digimon, &digimon->stats, fighter->flags);
				entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			}
			return 1;
		}
	}

	return 0;
}

int32_t BTL_tickMeleeAttack(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int16_t arg3)
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
		dist = BTL_getDistanceSquared(&digimon->entity, &target->entity);
		radius = DIGIMON_DATA[digimon->entity.type].radius + DIGIMON_DATA[target->entity.type].radius;
		if ((radius * radius) >= dist) {
			handleBattleIdle(digimon, &digimon->stats, fighter->flags);
			if (MAIN_D_80134D74 != 0) {
				if (MAIN_D_80134D60 != &digimon->entity) {
					return 0;
				}
				if (MAIN_D_80135080 > 0) {
					MAIN_D_80135080--;
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
						BTL_initializeFinisherChargeup(tech);
					}
					startAnimation(&digimon->entity, fighter->queuedAnim);
					digimon->entity.anim.animFlag &= 0xfe;
					MAIN_D_8013507C = BTL_addFinisherAura(&digimon->entity, 0x50);
					MAIN_D_80135080 = 0x50;
					return 0;
				}
			}
			if (BTL_isMoveOnCooldown(&digimon->entity, fighter) != 0) {
				return 0;
			}
			startAnimation(&digimon->entity, fighter->queuedAnim);
			fighter->flags |= 0x20;
			BTL_setupMoveExecution(digimon, target, fighter);
			return 0;
		}
		if (MAIN_D_80134D74 == 0) {
			BTL_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			goto check;
		}
		if (MAIN_D_80134D60 != &digimon->entity) {
			return 0;
		}
		if (MAIN_D_80135080 > 0) {
			MAIN_D_80135080--;
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			return 0;
		}
		startAnimation(&digimon->entity, fighter->queuedAnim);
		fighter->flags |= 0x20;
		BTL_setupMoveExecution(digimon, target, fighter);
		return 0;
	}

	BTL_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
check:
	if (entityCheckCollision(NULL, &digimon->entity, 0x118, 0xc8) == -1) {
		return 0;
	} else {
		*rot = orig;
		return 1;
	}
}

void BTL_tickRangedAttack(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter, int16_t move)
{
	uint32_t dist;
	uint32_t total;
	int32_t radius;
	int32_t sq;

	if (fighter->unk15 > 0x50) {
		BTL_startQueuedMove(digimon, target, fighter);
		fighter->unk15 = 0;
		return;
	}

	if ((MAIN_D_80134D74 != 0) && (&digimon->entity == MAIN_D_80134D60)) {
		BTL_startQueuedMove(digimon, target, fighter);
		return;
	}

	dist = BTL_getDistanceSquared(&digimon->entity, &target->entity);
	radius = DIGIMON_DATA[digimon->entity.type].radius + DIGIMON_DATA[target->entity.type].radius;
	total = *(uint32_t *)&MOVE_DATA[move].distance + (radius * radius);
	if ((total + (total * 3 / 10)) < dist) {
		BTL_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
		BTL_moveTowardLocation(digimon, &target->entity.posData->location, 0x118, 0xc8);
		fighter->unk15++;
		return;
	}

	if (dist < (total - (total * 3 / 10))) {
		BTL_setWalking(&digimon->entity, &digimon->stats, fighter->flags);
		BTL_backAwayFromTarget(digimon, target, fighter);
		fighter->unk15++;
		return;
	}

	fighter->unk15 = 0;
	handleBattleIdle(digimon, &digimon->stats, fighter->flags);
	if (fighter->flags & 8) {
		if ((MAIN_D_80134D66 % 40) == 0) {
			BTL_startQueuedMove(digimon, target, fighter);
		} else {
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
		}
	} else {
		BTL_startQueuedMove(digimon, target, fighter);
	}
}

void BTL_setWalking(Entity *entity, Stats *stats, uint16_t flags)
{
	if ((entity->anim.animId != 0x24) && (entity->anim.animId != 0x23)) {
		BTL_startWalkingAnimation(entity, stats, flags);
	}
}

void BTL_moveTowardLocation(DigimonEntity *digimon, VECTOR *target, int16_t dx, int16_t dy)
{
	int16_t facing;

	facing = digimon->entity.posData->rotation.vy;
	entityLookAtLocation(&digimon->entity, target);
	if (entityCheckCollision(NULL, &digimon->entity, dx, dy) != -1) {
		digimon->entity.posData->rotation.vy = facing;
		collisionGrace(NULL, &digimon->entity, dx, dy);
	}
}

uint32_t BTL_getMoveWithHighestDistance(DigimonEntity *digimon)
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

void BTL_backAwayFromTarget(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
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
			BTL_findUnblockedRotation(&digimon->entity, rot, hit, orig);
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
			BTL_findUnblockedRotation(&digimon->entity, rot, hit, orig);
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
		BTL_findUnblockedRotation(&digimon->entity, rot, hit, orig);
	}
}

void BTL_tickFighterAction(int32_t index)
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

	if ((MAIN_D_80134D74 != 0) && (MAIN_D_80134D60 == &digimon->entity) && (MAIN_D_80135080 > 0)) {
		return;
	}

	if ((fighter->flags & 0x28) == 0x28) {
		fighter->flatAttackTimer--;
		switch (fighter->flatAttackTimer) {
		case 0x1c:
			BTL_addAuraProjectile(&digimon->entity);
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
		BTL_addFinisherProgress(fighter, fighter->finisherGoal * 2 / 50);
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
			BTL_clearBlockedAttacks(fighter);
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

void BTL_confusedRotate(Entity *entity)
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

void BTL_maintainTargetDistance(DigimonEntity *attacker, DigimonEntity *target, FighterData *fighter)
{
	if ((attacker->entity.anim.animId >= 0x23) && (attacker->entity.anim.animId < 0x25)) {
		BTL_maintainDistanceRange(attacker, target, fighter, 0x27100, 0x4e200);
	} else {
		BTL_maintainDistanceRange(attacker, target, fighter, 0, 0x75300);
	}
}

void BTL_maintainDistanceRange(DigimonEntity *attacker, DigimonEntity *target, FighterData *fighter, uint32_t nearLimit, uint32_t farLimit)
{
	uint32_t dist;
	uint32_t reach;

	dist = BTL_getDistanceSquared(&attacker->entity, &target->entity);
	reach = BTL_getContactRangeSquared(&attacker->entity.type, &target->entity.type);
	if (dist < (reach + nearLimit)) {
		BTL_setWalking(&attacker->entity, &attacker->stats, fighter->flags);
		BTL_backAwayFromTarget(attacker, target, fighter);
	} else if ((reach + farLimit) < dist) {
		BTL_setWalking(&attacker->entity, &attacker->stats, fighter->flags);
		BTL_moveTowardLocation(attacker, &target->entity.posData->location, 0x118, 0xc8);
	} else {
		fighter->unk16 = 0;
		handleBattleIdle(attacker, &attacker->stats, fighter->flags);
		entityLookAtLocation(&attacker->entity, &target->entity.posData->location);
	}
}

int32_t BTL_getContactRangeSquared(int32_t *a, int32_t *b)
{
	int32_t reach;

	reach = DIGIMON_DATA[*a].radius + DIGIMON_DATA[*b].radius + 0xc8;

	return reach * reach;
}

void BTL_increaseSpeedBuffer(FighterData *fighter, Stats *stats)
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

void BTL_faintDigimon(DigimonEntity *digimon, FighterData *fighter, int16_t arg2)
{
	*(int8_t *)&digimon->stats.current.unk2_2 = 1;
	fighter->flags |= 0x8000;
	startAnimation(&digimon->entity, 0x2b);
	BTL_resetFlatten(arg2);
	BTL_removeStatusEffects(digimon, fighter);
	fighter->flags &= 0xff40;
	fighter->flags |= 0x40;
	fighter->moveRange = -1;
	BTL_resetFighterAction(fighter);
}

int32_t BTL_hasAffordableMoves(int16_t *out, uint8_t index)
{
	DigimonEntity *digimon;
	FighterData *fighter;
	int32_t found;
	int32_t i;

	digimon = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]];
	fighter = &COMBAT_DATA_PTR->fighter[index];
	found = 0;

	for (i = 0; i < 4; ++i) {
		if (BTL_isMoveUsable(digimon, fighter, i) != 0) {
			out[i] = 1;
			found = 1;
		} else {
			out[i] = 0;
		}
	}

	return found;
}

void BTL_startWalkingAnimation(Entity *entity, Stats *stats, uint16_t flags)
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

void BTL_initializePartnerTile(void)
{
	getEntityTileFromModel(ENTITY_TABLE[1], &PARTNER_PREVIOUS_TILE_X, &PARTNER_PREVIOUS_TILE_Y);
}

void BTL_initializeEnemyHPBarSprites(void)
{
	PlayerDataSub *slot;
	int32_t i;

	slot = COMBAT_DATA_PTR->player.unk1;
	for (i = 0; i < 4; i++) {
		slot->unk1 = 0;
		slot->unk2 = 0x32;
		slot->unk3 = 3;
		slot->unk5 = 0x58;
		slot->unk6 = 0xec;
		slot->unk7 = 0x110;
		slot->unk8 = 0x1f3;
		slot->unk4 = 0x1f;
		slot->unk9 = 0x80;
		slot->unk10 = 0xff;
		slot->unk11 = 0x80;
		slot++;
	}
}

void BTL_addEnemyHPBars(void)
{
	addObject(0x191, 0, NULL, BTL_renderEnemyHPBars);
}

void BTL_clearBlockedAttacks(FighterData *fighter)
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

void BTL_findUnblockedRotation(Entity *entity, int16_t *rot, int16_t hit, int16_t orig)
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
	cand[0] = MAIN_D_801346D8[base];
	cand[1] = (MAIN_D_801346D8[base] + 0x400) & 0xfff;
	for (i = 0; i < 2; i++) {
		*rot = cand[i];
		if (entityCheckCollision(NULL, entity, 0x118, 0xc8) == -1) {
			break;
		}
	}

	switch (i) {
	case 0:
		for (i = 0; i < 3; i++) {
			*rot = cand[i] = (MAIN_D_801346D8[base] + 0xc00 + (i * 0x200)) & 0xfff;
			if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
				goto common;
			}
		}
		break;
	case 1:
		for (i = 0; i < 3; i++) {
			*rot = cand[i] = (MAIN_D_801346D8[base] + 0x400 + (i * 0x200)) & 0xfff;
			if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
				goto common;
			}
		}
		break;
	default:
		for (i = 0; i < 3; i++) {
			*rot = cand[i] = (MAIN_D_801346D8[base] + 0x800 + (i * 0x200)) & 0xfff;
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

int32_t BTL_battleMain(void)
{
	COMBAT_AREA_CENTER_X = 0xa0 - DRAWING_OFFSET_X;
	COMBAT_AREA_CENTER_Y = 0x78 - DRAWING_OFFSET_Y;
	BTL_initializeCombat();
	BTL_handleBattleIntro();
	for (;;) {
		if (BTL_isBattleFinished() != 0) {
			break;
		}
		BTL_tickPartnerAI();
		BTL_tickEnemyAI();
		BTL_tickFighterStates();
		BTL_tickPartnerWaypointTrail();
		BTL_battleTickFrame();
		handlePause();
		MAIN_D_80134D66++;
	}

	MAIN_func_800E642C();
	BTL_deinitializeCombat();
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP == 0) {
		COMBAT_DATA_PTR->player.currentCommand[0] = 0;
		return -1;
	}

	if (COMBAT_DATA_PTR->player.currentCommand[0] == 1) {
		COMBAT_DATA_PTR->player.currentCommand[0] = 0;
		return 0;
	}

	return 1;
}

int16_t BTL_getAttackTech(AttackObject *attack)
{
	DigimonEntity *digimon;
	int16_t slot;
	int16_t tech;
	int32_t i;

	if (attack->effectId != 0x179) {
		slot = BTL_getFighterSlot(attack->casterId);
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

int32_t BTL_applyBuffMove(DigimonEntity *digimon, int32_t slot, int32_t anim)
{
	Stats *stats;

	stats = &digimon->stats;
	switch (anim) {
	case 0x29:
		BTL_buffStats(digimon, slot, stats->base.off * 3 / 10, &stats->base.off, 0xb, 3);
		break;
	case 0x2a:
		BTL_buffStats(digimon, slot, stats->base.off / 10, &stats->base.off, 0xb, 3);
		BTL_buffStats(digimon, slot, stats->base.def * 5 / 100, &stats->base.def, 0xb, 4);
		BTL_buffStats(digimon, slot, stats->base.speed / 10, &stats->base.speed, 0xb, 5);
		break;
	case 0x22:
		BTL_buffStats(digimon, slot, stats->base.def / 5, &stats->base.def, 0xb, 4);
		BTL_buffStats(digimon, slot, stats->base.speed / 10, &stats->base.speed, 0xb, 5);
		break;
	case 0x15:
		BTL_buffStats(digimon, slot, stats->base.off * 7 / 100, &stats->base.off, 0xb, 3);
		BTL_buffStats(digimon, slot, stats->base.def * 8 / 100, &stats->base.def, 0xb, 4);
		BTL_buffStats(digimon, slot, stats->base.speed * 7 / 100, &stats->base.speed, 0xb, 5);
		break;
	case 0x1e:
		BTL_buffStats(digimon, slot, stats->base.off / 4, &stats->base.off, 0xb, 3);
		BTL_buffStats(digimon, slot, stats->base.def * 3 / 20, &stats->base.def, 0xb, 4);
		BTL_buffStats(digimon, slot, stats->base.speed * 3 / 20, &stats->base.speed, 0xb, 5);
		break;
	default:
		return 0;
	}

	return 1;
}

int16_t BTL_calculateHitChance(DigimonEntity *attacker, DigimonEntity *defender, FighterData *fighter, int16_t move)
{
	uint32_t anim;
	int32_t value;
	int32_t half;
	int32_t x;
	int16_t result;

	if (DIGIMON_DATA[defender->entity.type].level < 3) {
		return 100;
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

void BTL_retargetAfterHit(DigimonEntity *digimon, FighterData *fighter, AttackObject attack)
{
	if (digimon == (DigimonEntity *)ENTITY_TABLE[1]) {
		if (((MAIN_D_801346E8[digimon->stats.current.chargeMode] - PARTNER_PARA.discipline) <= random(100)) && (BTL_isFighterDefeated(fighter->targetId) == 0)) {
			return;
		}
		BTL_selectFighterTarget(digimon, fighter, BTL_getFighterSlot(attack.casterId), 1);
	} else {
		BTL_selectFighterTarget(digimon, fighter, BTL_getFighterSlot(attack.casterId), 1);
	}
}

INCLUDE_ASM("asm/btl/nonmatchings/battle_main", BTL_calculateDamage);

INCLUDE_ASM("asm/btl/nonmatchings/battle_main", BTL_handleHitReaction);

void BTL_applyMoveStatus(DigimonEntity *digimon, FighterData *fighter, int32_t move)
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
				BTL_addPoisonStatusVisual(digimon, fighter);
			}
			break;
		case 2:
			if (!(fighter->flags & 2)) {
				fighter->flags |= 2;
				fighter->confusionTimer = random(0x65) + 200;
				BTL_addConfusionStatusVisual(digimon, fighter);
				BTL_resetFighterAction(fighter);
			}
			break;
		case 3:
			if (!(fighter->flags & 4)) {
				fighter->flags |= 4;
				fighter->stunTimer = random(0x29) + 200;
				BTL_addStunStatusVisual(digimon, fighter);
				BTL_resetFighterAction(fighter);
			}
			break;
		case 4:
			if (!(fighter->flags & 8)) {
				fighter->flatTimer = -1;
				BTL_removeStatusEffects(digimon, fighter);
				BTL_resetFighterAction(fighter);
			}
			break;
		}
		if (fighter == COMBAT_DATA_PTR->fighter) {
			COMBAT_DATA_PTR->player.statusedCount++;
		}
	}
}

int16_t BTL_getFighterSlot(int16_t entityId)
{
	int32_t i;

	for (i = 0; ENEMY_COUNT >= i; i++) {
		if (COMBAT_DATA_PTR->player.entityIds[i] == entityId) {
			return i;
		}
	}

	return -1;
}

int32_t BTL_addBlockedAttack(FighterData *fighter, FighterData *other)
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

void BTL_startHitAnimation(Entity *entity, AttackObject *attack, int32_t animId)
{
	int16_t tech;

	tech = BTL_getAttackTech(attack);
	startAnimation(entity, animId);
	createParticleFX(MOVE_DATA[tech].special, 1, &attack->position, entity, MOVE_DATA[tech].iframes + 0x10);
}

void BTL_tickAttackHits(void)
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
		if (MAIN_D_80134D68 >= 0x13) {
			continue;
		}
		entity = ENTITY_TABLE[((uint8_t *)((uint32_t)i + (uint32_t)COMBAT_DATA_PTR))[0x66c]];
		tech = BTL_getAttackTech(&attack);
		moveIdx = entity->anim.animId - 0x2e;
		if (DIGIMON_DATA[entity->type].moves[moveIdx] == 0x2d) {
			if (COMBAT_DATA_PTR->player.entityIds[fighter->targetId] == attack.casterId) {
				((DigimonEntity *)entity)->stats.current.unk2_2 = 0;
				continue;
			}
		}
		if (BTL_applyBuffMove((DigimonEntity *)entity, (int16_t)i, (int16_t)tech) != 0) {
			((DigimonEntity *)entity)->stats.current.unk2_2 = 0;
			continue;
		}
		BTL_removeMoveEffect((DigimonEntity *)entity, fighter);
		fighter->flags &= 0xff8f;
		attacker = ENTITY_TABLE[attack.casterId];
		dmg = BTL_calculateHitChance((DigimonEntity *)attacker, (DigimonEntity *)entity, fighter, tech);
		if (entity == MAIN_D_80134D60) {
			if (i == 0) {
				BTL_removeFinisherChargeup();
			}
			if ((*(int32_t *)&MAIN_D_8013507C) != -1) {
				BTL_removeFinisherAura(MAIN_D_8013507C);
			}
			MAIN_D_80134D74 = 0;
			MAIN_D_80134D60 = NULL;
		}
		if (random(100) < dmg) {
			BTL_retargetAfterHit((DigimonEntity *)entity, fighter, attack);
			dmg = BTL_calculateDamage((DigimonEntity *)attacker, (DigimonEntity *)entity, tech);
			MAIN_D_80134D70 = 1;
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
			BTL_handleHitReaction(entity, fighter, &attack, i);
			sub->unk25 = 0;
			addEntityText((DigimonEntity *)entity, i, 0, dmg, 0);
			fighter->invulnerableTimer = MOVE_DATA[tech].iframes;
			entity->anim.animFlag &= 0xfe;
			BTL_applyMoveStatus((DigimonEntity *)entity, fighter, tech);
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
						fighter->targetId = BTL_getFighterSlot(attack.casterId);
						fighter->moveRange = 1;
						BTL_startQueuedMove((DigimonEntity *)entity, (DigimonEntity *)attacker, fighter);
						attacker->anim.animFlag &= 0xfe;
						handled = 1;
						((DigimonEntity *)entity)->stats.current.unk2_2 = 0;
						break;
					}
				}
			}
		}
		if (handled != 0) {
			continue;
		}
		if (BTL_addBlockedAttack(fighter, (FighterData *)&attack) != 0) {
			createParticleFX(0, 2, hitPos, entity, 0x11);
		}
		if ((fighter->flags & 0x80) && (fighter->invulnerableTimer > 0)) {
			goto blocked;
		}
		if (MOVE_DATA[tech].range == 1) {
			dmg = BTL_calculateDamage((DigimonEntity *)attacker, (DigimonEntity *)entity, tech);
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
		BTL_addFinisherProgress(fighter, fighter->finisherGoal * 3 / 50);
		for (j = 0; ENEMY_COUNT >= j; j++) {
			if (attacker == ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[j]]) {
				BTL_addFinisherProgress(&COMBAT_DATA_PTR->fighter[j], COMBAT_DATA_PTR->fighter[j].finisherGoal * 4 / 50);
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
		((DigimonEntity *)entity)->stats.current.unk2_2 = 0;
	}
}

void BTL_buffStats(DigimonEntity *digimon, int32_t slot, int16_t value, int16_t *stat, int16_t color, uint8_t flag)
{
	addWithLimit(stat, value, 0x3e7);
	addEntityText(digimon, slot, color, value, flag);
}

void BTL_renderEnemyHPBars(void)
{
	int16_t pos[2];
	CombatData *combat;
	PlayerDataSub *spr;
	FighterData *fighter;
	Stats *stats;
	Entity *entity;
	int32_t i;
	int16_t x;
	int16_t y;
	int32_t w;

	combat = COMBAT_DATA_PTR;
	GsSetLsMatrix(&GsWSMATRIX);
	for (i = 1, fighter = &combat->fighter[1]; ENEMY_COUNT >= i; i++, fighter = (FighterData *)((int32_t)fighter + 0x168)) {
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		spr = &COMBAT_DATA_PTR->player.unk1[i];
		if (MAIN_D_80134D64 == 0) {
			if (spr->unk25 >= 0x15) {
				continue;
			}
		} else {
			if ((spr->unk25 >= 0x15) && (fighter->flags & 0x8000)) {
				continue;
			}
		}
		stats = &((DigimonEntity *)entity)->stats;
		if (stats->current.currentHP == 0) {
			fighter->hpDamageBuffer = 0;
		}
		if (!(fighter->flags & 0x10) && (fighter->hpDamageBuffer == 0)) {
			spr->unk25++;
		}
		getEntityScreenPos(entity, 1, pos);
		if (pos[1] < -0x4a) {
			y = -0x64;
		} else {
			y = pos[1] - 0x1a;
		}
		if (pos[0] >= 0x42) {
			x = 0x55;
		} else {
			x = pos[0] + 0x14;
		}
		renderTrianglePrimitive(0xab40a, x, y, x + 0x36, y, x + 0x36, y + 6, 7, 0);
		renderTrianglePrimitive(0xab40a, x, y + 1, x, y + 6, x + 0x35, y + 6, 7, 0);
		renderLinePrimitive(0xab40a, x, y + 6, pos[0], pos[1], 7, 0);
		spr->pos.vx = x + 2;
		spr->pos.vy = y + 2;
		damageTick(fighter, stats);
		w = stats->current.currentHP * 50 / stats->base.hp;
		if (w != spr->unk2) {
			spr->unk2 = w;
			if (spr->unk2 >= 0xf) {
				spr->unk9 = ((0x32 - spr->unk2) * 127 / 35) + 0x80;
				spr->unk10 = 0xff;
			} else if ((spr->unk2 >= 5) && (spr->unk2 < 0xf)) {
				spr->unk9 = 0xff;
				spr->unk10 = 0xff - ((0xf - spr->unk2) * 127 / 10);
			} else {
				spr->unk9 = 0xff;
				spr->unk10 = 0x80;
			}
			spr->unk11 = 0x80;
		}
		GsSortFastSprite((GsSPRITE *)spr, ACTIVE_ORDERING_TABLE, 7);
		MAIN_func_800E5724(0, 4, x + 0x21, y - 9, stats->current.currentHP, 7);
	}
}

int32_t BTL_isMoveUsable(DigimonEntity *digimon, FighterData *fighter, int16_t slot)
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

int32_t BTL_getDistanceSquared(Entity *a, Entity *b)
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

void BTL_setupQueuedMove(DigimonEntity *digimon, FighterData *fighter, int16_t arg2, int32_t moveIndex)
{
	int16_t tech;

	fighter->unk15 = 0;
	fighter->queuedAnim = digimon->stats.base.moves[moveIndex];
	tech = entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim);
	fighter->moveRange = MOVE_DATA[tech].range;
	BTL_applyChargeRequirement(digimon, fighter, tech);
	fighter->flags |= 0x40;
}

void BTL_applyChargeRequirement(DigimonEntity *digimon, FighterData *fighter, int16_t tech)
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

void BTL_startQueuedMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	int16_t tech;

	if (MAIN_D_80134D74 != 0) {
		if (MAIN_D_80134D60 != &digimon->entity) {
			return;
		}
		if (MAIN_D_80135080 > 0) {
			MAIN_D_80135080--;
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			return;
		}
	} else {
		tech = entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim);
		if ((tech >= 0x3a) && (tech < 0x71)) {
			MAIN_D_80134D74 = 1;
		}
		if (MAIN_D_80134D74 != 0) {
			MAIN_D_80134D60 = &digimon->entity;
			if (digimon == (DigimonEntity *)ENTITY_TABLE[1]) {
				BTL_initializeFinisherChargeup(tech);
			}
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			startAnimation(&digimon->entity, fighter->queuedAnim);
			digimon->entity.anim.animFlag &= 0xfe;
			MAIN_D_8013507C = BTL_addFinisherAura(&digimon->entity, 0x50);
			MAIN_D_80135080 = 0x50;
			return;
		}
	}

	if (BTL_isMoveOnCooldown(&digimon->entity, fighter) != 0) {
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

	if (MOVE_DATA[tech].unk3 & 2) {
		MAIN_D_80134D7C[1] = 0x6e;
	}

	startAnimation(&digimon->entity, fighter->queuedAnim);
	fighter->flags |= 0x20;
	if (!(fighter->flags & 8)) {
		BTL_setupMoveExecution(digimon, target, fighter);
	} else {
		fighter->flatAttackTimer = 0x1e;
	}
}

int32_t BTL_isMoveOnCooldown(Entity *entity, FighterData *fighter)
{
	int16_t tech;

	tech = entityGetTechFromAnim(entity, fighter->queuedAnim);
	if ((MOVE_DATA[tech].unk3 & 2) && (MAIN_D_80134D7C[1] > 0)) {
		return 1;
	}

	return 0;
}

void BTL_setupMoveExecution(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	int32_t tech;
	int16_t cost;
	int16_t brain;
	int32_t i;

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
					for (i = 1; ENEMY_COUNT >= i; i++) {
						if (BTL_isFighterDefeated(i) != 1) {
							fighter->targetId = i;
							MAIN_D_80134EF4 = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
						}
					}
				}
			}
		}
	} else {
		MAIN_D_80134EF4 = digimon;
	}

	BTL_removeMoveEffect(digimon, fighter);
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
			if (MAIN_D_80134D7C[0] == 0) {
				digimon->stats.current.currentMP -= (int16_t)(MOVE_DATA[tech].mpCost * 3);
			}
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
		digimon->stats.current.unk2_1 = BTL_startEFE(fighter->effectSlot[i]);
		fighter->unk11 = fighter->effectSlot[i];
	}

	if ((MOVE_DATA[tech].range == 4) && (fighter->buffsRemaining != 0)) {
		fighter->buffsRemaining--;
	}

	fighter->speedBuffer -= MOVE_DATA[tech].power;
	if (fighter->speedBuffer < -0x9b) {
		fighter->speedBuffer = -0x9b;
	}
}

void BTL_removeMoveEffect(DigimonEntity *digimon, FighterData *fighter)
{
	int32_t id;

	id = fighter->unk11;
	do {
		if (id != -1) {
			BTL_stopEFESubEffect(id, *(int8_t *)&digimon->stats.current.unk2_1);
		}
	} while (0);
	*(int8_t *)&digimon->stats.current.unk2_1 = -1;
	fighter->unk11 = -1;
}

void BTL_battleTickFrame(void)
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

void BTL_addFinisherProgress(FighterData *fighter, int16_t amount)
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

void BTL_clearConfusion(DigimonEntity *digimon, FighterData *fighter)
{
	BTL_resetFighterAction(fighter);
	BTL_selectFighterTarget(digimon, fighter, fighter->targetId, 0);
	fighter->flags &= 0xfffd;
	fighter->confusionTimer = 0;
	if (!(fighter->flags & 0xc) && (fighter->flatTimer == 0)) {
		fighter->flags &= 0xffbf;
		BTL_removeStatusEffectVisual(digimon, fighter, 2);
		if (fighter->flags & 1) {
			BTL_addStatusEffectVisual(digimon, fighter, 1);
		}
	}
}

void BTL_clearStun(DigimonEntity *digimon, FighterData *fighter)
{
	BTL_resetFighterAction(fighter);
	digimon->entity.anim.animFlag |= 1;
	fighter->flags &= ~4;
	fighter->stunTimer = 0;
	if (fighter->flags & 8) {
		return;
	}

	if (fighter->flatTimer != 0) {
		return;
	}

	BTL_removeStatusEffectVisual(digimon, fighter, 3);
	if (fighter->flags & 2) {
		BTL_addStatusEffectVisual(digimon, fighter, 2);
	}

	if (fighter->flags & 1) {
		BTL_addStatusEffectVisual(digimon, fighter, 1);
	}
}

void BTL_applyFlattenScale(VECTOR *scale, int32_t t)
{
	int32_t value;

	if (t < 0x40) {
		if (t >= 0x30) {
			BTL_applyStretchScale(scale, (0x40 - t) << 4);
		} else if (t >= 0x20) {
			BTL_applySquashScale(scale, (0x30 - t) << 4);
		} else if (t >= 0x18) {
			BTL_applyStretchScale(scale, (0x20 - t) << 5);
		} else if (t >= 0x10) {
			BTL_applySquashScale(scale, (0x18 - t) << 5);
		} else if (t >= 0xc) {
			BTL_applyStretchScale(scale, (0x10 - t) << 6);
		} else if (t >= 8) {
			BTL_applySquashScale(scale, (0xc - t) << 6);
		} else if (t >= 6) {
			BTL_applyStretchScale(scale, (8 - t) << 7);
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

void BTL_addStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, int32_t kind)
{
	if (fighter->statusFxId == -1) {
		switch (kind) {
		case 1:
			fighter->statusFxId = BTL_addPoisonEffect(digimon, fighter);
			break;
		case 2:
			fighter->statusFxId = BTL_addConfusionEffect(digimon, fighter);
			break;
		case 3:
			fighter->statusFxId = BTL_addStunEffect(digimon, fighter->stunTimer);
			break;
		}
	}
}

void BTL_applyStretchScale(VECTOR *scale, int32_t angle)
{
	int32_t half;

	scale->vy = _sin(angle) + 0x1000;
	half = 0x1000 - (_sin(angle) / 2);
	scale->vx = half;
	scale->vz = half;
}

void BTL_applySquashScale(VECTOR *scale, int32_t angle)
{
	int32_t half;

	scale->vy = 0x1000 - (_sin(angle) / 2);
	half = (_sin(angle) / 2) + 0x1000;
	scale->vx = half;
	scale->vz = half;
}

void BTL_removeStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, int32_t kind)
{
	int32_t id;

	id = fighter->statusFxId;
	do {
		if (id != -1) {
			switch (kind) {
			case 1:
				BTL_removePoisonEffect(id, digimon);
				break;
			case 2:
				BTL_removeConfusionEffect(id, digimon);
				break;
			case 3:
				BTL_removeStunEffect(id, digimon);
				break;
			}
			fighter->statusFxId = -1;
		}
	} while (0);
}

void BTL_clearPoison(DigimonEntity *digimon, FighterData *fighter)
{
	fighter->flags &= 0xfffe;
	fighter->poisonTimer = 0;
	if (!(fighter->flags & 0xe) && (fighter->flatTimer == 0)) {
		BTL_removeStatusEffectVisual(digimon, fighter, 1);
	}
}

void BTL_healFlatten(Entity *entity, FighterData *fighter, int32_t arg2)
{
	if (fighter->flatTimer >= 0x42) {
		fighter->flatTimer = 0x41;
	}

	if (fighter->flatTimer != 0) {
		if (!(fighter->flags & 8)) {
			fighter->flags |= 8;
			if (fighter->flatTimer >= 4) {
				entity->flatSprite = 0;
			}
			fighter->flatTimer = 0x41;
		}
	}
}

void BTL_tickStatusEffects(void)
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
				BTL_clearConfusion((DigimonEntity *)entity, fighter);
			}
		}
		if (fighter->flags & 4) {
			if (MAIN_D_80134D74 == 0) {
				fighter->stunTimer--;
			}
			if (fighter->stunTimer == 0) {
				BTL_clearStun((DigimonEntity *)entity, fighter);
			}
		}
		if (fighter->flatTimer <= 0) {
			continue;
		}
		if ((MAIN_D_80134D74 == 0) || (fighter->flatTimer < 0x42)) {
			fighter->flatTimer--;
		}
		BTL_applyFlattenScale(&entity->posData->scale, fighter->flatTimer);
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
					BTL_addStatusEffectVisual((DigimonEntity *)entity, fighter, 3);
					fighter->moveRange = 0;
				}
				if (fighter->flags & 2) {
					BTL_addStatusEffectVisual((DigimonEntity *)entity, fighter, 2);
				}
				if (fighter->flags & 1) {
					BTL_addStatusEffectVisual((DigimonEntity *)entity, fighter, 1);
				}
				fighter->moveRange = 0;
				BTL_resetFighterAction(fighter);
				if (entity->type == 0x71) {
					entity->posData->scale.vx = 0x1800;
					entity->posData->scale.vy = 0x1800;
					entity->posData->scale.vz = 0x1800;
				}
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

void BTL_resetFighterAction(FighterData *fighter)
{
	fighter->cooldown = 0;
	fighter->senileTimer = 0;
	fighter->flags &= 0xc7ff;
}

void BTL_addPoisonStatusVisual(DigimonEntity *digimon, FighterData *fighter)
{
	if (!(fighter->flags & 0xe) && (fighter->flatTimer == 0)) {
		BTL_addStatusEffectVisual(digimon, fighter, 1);
	}
}

void BTL_addConfusionStatusVisual(DigimonEntity *digimon, FighterData *fighter)
{
	if (!(fighter->flags & 0xc) && (fighter->flatTimer == 0)) {
		if (fighter->flags & 1) {
			BTL_removeStatusEffectVisual(digimon, fighter, 1);
		}
		BTL_addStatusEffectVisual(digimon, fighter, 2);
	}
}

void BTL_addStunStatusVisual(DigimonEntity *digimon, FighterData *fighter)
{
	if (!(fighter->flags & 8) && (fighter->flatTimer == 0)) {
		if (fighter->flags & 2) {
			BTL_removeStatusEffectVisual(digimon, fighter, 2);
		}
		if (fighter->flags & 1) {
			BTL_removeStatusEffectVisual(digimon, fighter, 1);
		}
		BTL_addStatusEffectVisual(digimon, fighter, 3);
	}
}

void BTL_removeStatusEffects(DigimonEntity *digimon, FighterData *fighter)
{
	if (fighter->flags & 4) {
		BTL_removeStatusEffectVisual(digimon, fighter, 3);
	}

	if (fighter->flags & 2) {
		BTL_removeStatusEffectVisual(digimon, fighter, 2);
	}

	if (fighter->flags & 1) {
		BTL_removeStatusEffectVisual(digimon, fighter, 1);
	}
}

void BTL_healStatusEffect(int32_t arg0)
{
	FighterData *fighter;
	uint16_t *flags;
	int16_t kind;

	fighter = COMBAT_DATA_PTR->fighter;
	flags = &fighter->flags;
	if (*flags & 1) {
		kind = 0;
	}

	if (*flags & 2) {
		kind = 2;
	}

	if (*flags & 4) {
		kind = 1;
	}

	if (*flags & 8) {
		kind = 3;
	}

	if ((*flags & 0xf) && (arg0 == 1)) {
		BTL_addBuffRingsEffect(kind, ENTITY_TABLE[1]);
	}

	if (*flags & 1) {
		BTL_clearPoison((DigimonEntity *)ENTITY_TABLE[1], fighter);
	}

	if (*flags & 2) {
		BTL_clearConfusion((DigimonEntity *)ENTITY_TABLE[1], fighter);
	}

	if (*flags & 4) {
		BTL_clearStun((DigimonEntity *)ENTITY_TABLE[1], fighter);
	}

	if (*flags & 8) {
		BTL_healFlatten(ENTITY_TABLE[1], fighter, 0);
	}
}

int32_t BTL_getUsableMoves(int16_t *out, int16_t index)
{
	DigimonEntity *digimon;
	FighterData *fighter;
	int32_t found;
	int32_t i;

	digimon = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]];
	fighter = &COMBAT_DATA_PTR->fighter[index];
	found = 0;

	for (i = 0; i < 4; ++i) {
		if (BTL_isMoveUsable(digimon, fighter, i) != 0) {
			out[i] = 1;
			found = 1;
		} else {
			out[i] = 0;
		}
	}

	return found;
}

void BTL_setFighterCooldown(DigimonEntity *digimon, FighterData *fighter)
{
	fighter->cooldown = 0x50;
	fighter->flags |= 0x800;
}

int16_t BTL_getRandomUsableMove(int16_t *flags)
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

int16_t BTL_getStrongestMove(int32_t index, int16_t *flags)
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

	BTL_getHighestScoredMove(rank.score, flags, &rank.best, 3);

	return rank.best;
}

INCLUDE_ASM("asm/btl/nonmatchings/battle_main", BTL_getMostEffectiveMove);

int16_t BTL_getCheapestMove(int32_t index, int16_t *flags)
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

	BTL_getLowestScoredMove(rank.score, flags, &rank.best, 3);

	return rank.best;
}

void BTL_getHighestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count)
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

void BTL_getLowestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count)
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

int16_t BTL_getNpcEntityIndex(Entity *entity)
{
	int32_t i;

	for (i = 2; i < 0xa; i++) {
		if (entity == ENTITY_TABLE[i]) {
			return i - 2;
		}
	}

	return -1;
}

void BTL_getStatusAfflictedEnemies(Entity *self, int16_t *out, int16_t *count)
{
	CombatData *combat;
	int32_t i;

	*count = 0;
	combat = COMBAT_DATA_PTR;
	for (i = 0; ENEMY_COUNT >= i; i++) {
		if ((self != ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]]) && (BTL_isFighterDefeated(i) == 0) && ((combat->fighter[i].flags & 0xf) != 0)) {
			out[*count] = i;
			(*count)++;
		}
	}
}

void BTL_getWeakestEnemy(Entity *self, FighterData *fighter, int16_t *outScore, int16_t *outIndex)
{
	Entity *other;
	int16_t best;
	int32_t i;

	best = 1000;
	*outIndex = 0xff;
	for (i = 0; ENEMY_COUNT >= i; i++) {
		other = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		if ((other != self) && (BTL_isFighterDefeated(i) == 0)) {
			if ((fighter->targetId == 0xff) || (other != ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighter->targetId]])) {
				*outScore = BTL_calculateTargetScore((DigimonEntity *)self, (DigimonEntity *)other);
				if (*outScore < best) {
					best = *outScore;
					*outIndex = i;
				}
			}
		}
	}
}

INCLUDE_ASM("asm/btl/nonmatchings/battle_main", BTL_calculateTargetScore);

void BTL_sortScoresDescending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count)
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

	BTL_calculateScoreRanks(values, groups, count);
}

void BTL_sortScoresAscending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count)
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

	BTL_calculateScoreRanks(values, groups, count);
}

void BTL_selectPartnerMove(DigimonEntity *digimon, FighterData *fighter, int16_t index)
{
	Stats *stats;
	int32_t values[3];
	int32_t keys[3];
	int32_t groups[3];
	int16_t flags[4];
	int16_t weights[4];
	int16_t tech;
	int16_t bonus;
	int32_t count;
	int32_t total;
	int32_t pick;
	int32_t i;
	int32_t j;

	if (BTL_getUsableMoves(flags, index) == 0) {
		BTL_setFighterCooldown(digimon, fighter);
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
		BTL_sortScoresDescending(values, keys, groups, 3);
		for (i = 0; i < 3; i++) {
			if (stats->base.moves[keys[i]] == 0xff) {
				weights[keys[i]] += 5;
			} else if (flags[keys[i]] != 0) {
				weights[keys[i]] = MAIN_D_801346EC[groups[i]];
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
		BTL_sortScoresAscending(values, keys, groups, 3);
		for (i = 0; i < 3; i++) {
			if (stats->base.moves[keys[i]] == 0xff) {
				weights[keys[i]] += 5;
			} else if (flags[keys[i]] != 0) {
				weights[keys[i]] = MAIN_D_801346F0[groups[i]];
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
			bonus = fighter->buffPrioTimer + BTL_calculateElementBonus(MOVE_DATA[tech].special, DIGIMON_DATA[digimon->entity.type].special[0]);
			weights[i] += bonus;
			continue;
		}
		weights[i] = weights[i] + BTL_calculateElementBonus(MOVE_DATA[tech].special, DIGIMON_DATA[ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighter->targetId]]->type].special[0]);
		if (MOVE_DATA[tech].range != 3) {
			continue;
		}
		count = BTL_countLivingEnemies();
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
				BTL_sortScoresDescending(values, keys, groups, 3);
				for (i = 0; i < 3; i++) {
					if (stats->base.moves[i] == 0xff) {
						continue;
					}
					if (flags[i] == 0) {
						continue;
					}
					weights[keys[i]] = MAIN_D_801346F4[groups[i]];
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
		BTL_setupQueuedMove(digimon, fighter, index, (uint8_t)i);
	} else {
		fighter->cooldown = 0x50;
		fighter->flags |= 0x800;
	}
}

int32_t BTL_calculateElementBonus(int32_t arg0, int32_t arg1)
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

int32_t BTL_countLivingEnemies(void)
{
	int32_t count;
	int32_t i;

	count = 0;
	for (i = 1; ENEMY_COUNT >= i; i++) {
		if (BTL_isFighterDefeated(i) == 0) {
			count++;
		}
	}

	return count;
}

void BTL_calculateScoreRanks(int32_t *values, int32_t *groups, int32_t count)
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

void BTL_selectConfusedMove(DigimonEntity *digimon, FighterData *fighter, int32_t tech)
{
	TargetChoice choice;

	if (random(10) < 7) {
		fighter->targetId = 0xff;
	} else {
		BTL_getRemainingEnemies(&digimon->entity, choice.enemies, &choice.count);
		fighter->targetId = choice.enemies[random(choice.count)];
	}

	if (BTL_getUsableMoves(choice.flags, tech) == 0) {
		BTL_setFighterCooldown(digimon, fighter);
	} else {
		BTL_setupQueuedMove(digimon, fighter, tech, BTL_getRandomUsableMove(choice.flags) & 0xff);
	}
}

void BTL_getRemainingEnemies(Entity *self, int16_t *out, int16_t *count)
{
	int32_t i;

	*count = 0;
	for (i = 0; ENEMY_COUNT >= i; i++) {
		if ((self != ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]]) && (BTL_isFighterDefeated(i) == 0)) {
			out[*count] = i;
			(*count)++;
		}
	}
}

int32_t BTL_selectMoveByPower(int32_t arg0, int16_t *flags)
{
	int16_t result;

	result = BTL_getStrongestMove(arg0, flags);
	if (result != -1) {
		return result;
	}

	result = BTL_getMostEffectiveMove(arg0, flags);
	if (result != -1) {
		return result;
	}

	return BTL_getRandomUsableMove(flags);
}

int32_t BTL_selectMoveByMpCost(int32_t arg0, int16_t *flags)
{
	int16_t result;

	result = BTL_getCheapestMove(arg0, flags);
	if (result != -1) {
		return result;
	}

	result = BTL_getMostEffectiveMove(arg0, flags);
	if (result != -1) {
		return result;
	}

	return BTL_getRandomUsableMove(flags);
}

void BTL_selectFighterTarget(DigimonEntity *digimon, FighterData *fighter, uint8_t target, int32_t arg3)
{
	TargetSearch t;
	int16_t c;
	int16_t brain;
	int16_t eff;
	int16_t n;
	int32_t i;

	if (((arg3 != 0) && (digimon == (DigimonEntity *)ENTITY_TABLE[1])) || (digimon != (DigimonEntity *)ENTITY_TABLE[1])) {
		if (target != 0xff) {
			if (digimon != (DigimonEntity *)ENTITY_TABLE[1]) {
				if ((NPC_ENTITIES[BTL_getNpcEntityIndex(&digimon->entity)].unk1 == 1) || (random(100) >= 0x47)) {
					fighter->targetId = 0;
					return;
				}
			}
			if (BTL_isFighterDefeated(target) != 0) {
				return;
			}
			fighter->flags |= 0x400;
			fighter->targetId = target;
		} else {
			fighter->targetId = 0;
		}
		return;
	}

	if (COMBAT_DATA_PTR->player.currentCommand[0] == 7) {
		if ((fighter->targetId != 0xff) && (BTL_isFighterDefeated(fighter->targetId) == 0)) {
			return;
		}
	}

	for (i = 0; ENEMY_COUNT >= i; i++) {
		t.enemies[i] = -1;
	}

	brain = digimon->stats.base.brain;
	if (brain >= 0x12c) {
		BTL_getStatusAfflictedEnemies(&digimon->entity, t.enemies, &t.count);
		n = t.count;
		if (n >= 2) {
			fighter->targetId = t.enemies[random(n)];
			return;
		}
		if (n == 1) {
			fighter->targetId = t.enemies[0];
			return;
		}
		BTL_getWeakestEnemy(&digimon->entity, fighter, &t.score, &t.choice);
		if ((fighter->targetId == 0xff) || (BTL_isFighterDefeated(fighter->targetId) != 0)) {
			fighter->targetId = t.choice;
			return;
		}
		eff = BTL_calculateTargetScore(digimon, (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighter->targetId]]);
		c = t.choice;
		if (c == 0xff) {
			return;
		}
		if ((eff - t.score) < 0x28) {
			return;
		}
		fighter->targetId = c;
		return;
	}

	if (brain >= 0x96) {
		if ((fighter->targetId != 0xff) && (BTL_isFighterDefeated(fighter->targetId) == 0)) {
			return;
		}
		BTL_getWeakestEnemy(&digimon->entity, fighter, &t.score, &t.choice);
		fighter->targetId = t.choice;
		return;
	}

	for (i = 0; ENEMY_COUNT >= i; i++) {
		t.flags[i] = 1;
	}

	fighter->targetId = BTL_getNearestEnemy(&digimon->entity, t.flags);
}

int16_t BTL_getNearestEnemy(Entity *self, int16_t *flags)
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
			if ((self != other) && (BTL_isFighterDefeated(i) == 0)) {
				dist = BTL_getDistanceSquared(self, other);
				if (dist < bestDist) {
					bestDist = dist;
					best = i;
				}
			}
		}
	}

	return best;
}

uint8_t BTL_isFighterDefeated(uint8_t index)
{
	if ((((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]])->stats.current.currentHP - COMBAT_DATA_PTR->fighter[index].hpDamageBuffer) <= 0) {
		return 1;
	}

	return 0;
}

void BTL_selectEnemyMove(DigimonEntity *digimon, FighterData *fighter, int16_t index)
{
	int16_t flags[4];
	int16_t weights[4];
	int16_t tech;
	int32_t total;
	int32_t pick;
	int32_t i;
	int32_t j;

	if (BTL_getUsableMoves(flags, index) == 0) {
		digimon->stats.current.chargeMode = 2;
		BTL_setFighterCooldown(digimon, fighter);
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
			if (NPC_ENTITIES[BTL_getNpcEntityIndex(&digimon->entity)].unk1 == 0) {
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
		BTL_setupQueuedMove(digimon, fighter, index, (uint8_t)i);
	} else {
		fighter->cooldown = 0x50;
		fighter->flags |= 0x800;
	}
}

void BTL_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, int32_t index)
{
	int16_t c;
	int16_t eff;
	int16_t n;

	if ((index >= 8U) && (index < 0xcU)) {
		eff = MOVE_DATA[entityGetTechFromAnim(&digimon->entity, digimon->stats.base.moves[index - 8])].special;
		prim->u0 = BTL_D_80072EE8[eff * 2];
		prim->v0 = BTL_D_80072EE9[eff * 2];
		prim->u1 = BTL_D_80072EE8[eff * 2] + 0x10;
		prim->v1 = BTL_D_80072EE9[eff * 2];
		prim->u2 = BTL_D_80072EE8[eff * 2];
		prim->v2 = BTL_D_80072EE9[eff * 2] + 0xf;
		prim->u3 = BTL_D_80072EE8[eff * 2] + 0x10;
		prim->v3 = BTL_D_80072EE9[eff * 2] + 0xf;
	} else {
		prim->u0 = BTL_D_80072ED8[(index - 1) * 2];
		prim->v0 = BTL_D_80072ED9[(index - 1) * 2];
		prim->u1 = BTL_D_80072ED8[(index - 1) * 2] + 0x10;
		prim->v1 = BTL_D_80072ED9[(index - 1) * 2];
		prim->u2 = BTL_D_80072ED8[(index - 1) * 2];
		prim->v2 = BTL_D_80072ED9[(index - 1) * 2] + 0xf;
		prim->u3 = BTL_D_80072ED8[(index - 1) * 2] + 0x10;
		prim->v3 = BTL_D_80072ED9[(index - 1) * 2] + 0xf;
	}
}
