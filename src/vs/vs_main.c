#include <libgpu.h>
#include <libgte.h>

#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/math.h>
#include <dw/move.h>
#include <dw/types.h>
#include <dw/vs.h>

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

extern uint8_t MAIN_D_80125F70[][7];
extern uint8_t VS_D_800707B4[];
extern uint8_t VS_D_800707B5[];
extern uint8_t VS_D_800707C4[];
extern uint8_t VS_D_800707C5[];
extern int16_t ENEMY_COUNT;

void createParticleFX();
void startAnimation(Entity *e, int32_t anim);
int16_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void swapInt(int32_t *a, int32_t *b);
int16_t VS_getAttackTech(AttackObject *attack);
int32_t VS_applyBuffMove(DigimonEntity *digimon, int32_t slot, int32_t anim);
void VS_applyPartnerStatsToFighter(void);
void VS_rollAttackOutcome(void);
void VS_handleHitReaction(Entity *entity, FighterData *fighter, AttackObject *attack, int16_t index);
void VS_applyMoveStatus(DigimonEntity *digimon, FighterData *fighter, int32_t move);
int16_t VS_getFighterSlot(int16_t entityId);
int32_t VS_addBlockedAttack(FighterData *fighter, FighterData *other);
void VS_buffStats(DigimonEntity *digimon, int32_t slot, int16_t value, int16_t *stat, int16_t color, uint8_t flag);
void VS_startAttackAnimation(Entity *entity, AttackObject *attack, int32_t anim);
void VS_updateFighterStatusVisuals(void);
void VS_clearStun(DigimonEntity *digimon, FighterData *fighter);
void VS_applyFlattenScale(VECTOR *scale, int32_t t);
void VS_addStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, int32_t kind);
void VS_applyStretchScale(VECTOR *scale, int32_t angle);
void VS_applySquashScale(VECTOR *scale, int32_t angle);
void VS_removeStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, int32_t kind);
void VS_addPoisonStatusVisual(DigimonEntity *digimon, FighterData *fighter);
void VS_addConfusionStatusVisual(DigimonEntity *digimon, FighterData *fighter);
void VS_addStunStatusVisual(DigimonEntity *digimon, FighterData *fighter);
int32_t VS_hasAffordableMoves(int16_t *out, uint8_t index);
void VS_setFighterCooldown(DigimonEntity *digimon, FighterData *fighter);
int16_t VS_getRandomUsableMove(int16_t *flags);
int16_t VS_getStrongestMove(int32_t index, int16_t *flags);
int16_t VS_getMostEffectiveMove(int32_t index, int16_t *flags);
int16_t VS_getCheapestMove(int32_t index, int16_t *flags);
void VS_getHighestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count);
void VS_getLowestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count);
void VS_sortScoresDescending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count);
void VS_sortScoresAscending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count);
int32_t VS_calculateElementBonus(int32_t arg0, int32_t arg1);
int32_t VS_countLivingEnemies(void);
void VS_calculateScoreRanks(int32_t *values, int32_t *groups, int32_t count);
uint8_t VS_isFighterDefeated(uint8_t index);
void VS_renderMoveName(void);
void VS_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, int32_t index);
void addEntityText(DigimonEntity *digimon, int32_t slot, int16_t color, int32_t value, uint8_t flag);
void addWithLimit(int16_t *stat, int32_t value, int32_t limit);
void VS_stopEFESubEffect(int32_t a, int32_t b);
int32_t VS_addPoisonEffect(DigimonEntity *digimon);
void VS_removePoisonEffect(int32_t i, DigimonEntity *digimon);
int32_t VS_addConfusionEffect(DigimonEntity *digimon);
void VS_removeConfusionEffect(int32_t i, DigimonEntity *digimon);
int32_t VS_addStunEffect(DigimonEntity *digimon, int32_t val);
void VS_removeStunEffect(int32_t i, DigimonEntity *digimon);

static void *vs_main_functions[] = {
	VS_setCommandIconUV,
	VS_renderMoveName,
	VS_isFighterDefeated,
	VS_selectPartnerMove,
	VS_selectMoveByMpCost,
	VS_selectMoveByPower,
	VS_queueRandomMove,
	VS_calculateScoreRanks,
	VS_countLivingEnemies,
	VS_calculateElementBonus,
	VS_sortScoresAscending,
	VS_sortScoresDescending,
	VS_getLowestScoredMove,
	VS_getHighestScoredMove,
	VS_getCheapestMove,
	VS_getMostEffectiveMove,
	VS_getStrongestMove,
	VS_getRandomUsableMove,
	VS_setFighterCooldown,
	VS_hasAffordableMoves,
	VS_removeStatusEffects,
	VS_addStunStatusVisual,
	VS_addConfusionStatusVisual,
	VS_addPoisonStatusVisual,
	VS_resetFighterAction,
	VS_applyMoveResult,
	VS_removeStatusEffectVisual,
	VS_applySquashScale,
	VS_applyStretchScale,
	VS_addStatusEffectVisual,
	VS_applyFlattenScale,
	VS_clearStun,
	VS_updateFighterStatusVisuals,
	VS_addFinisherProgress,
	VS_tickFrame,
	VS_removeMoveEffect,
	VS_playMoveEffect,
	VS_selectMoveTarget,
	VS_startFighterMove,
	VS_applyChargeRequirement,
	VS_setupQueuedMove,
	VS_getDistanceSquared,
	VS_isMoveUsable,
	VS_resolveAttack,
	VS_startAttackAnimation,
	VS_buffStats,
	VS_addBlockedAttack,
	VS_getFighterSlot,
	VS_applyMoveStatus,
	VS_handleHitReaction,
	VS_rollAttackOutcome,
	VS_applyPartnerStatsToFighter,
	VS_applyBuffMove,
	VS_getAttackTech,
};

int32_t VS_hasAffordableMoves(int16_t *out, uint8_t index)
{
	DigimonEntity *digimon;
	FighterData *fighter;
	int32_t found;
	int32_t i;

	digimon = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]];
	fighter = &COMBAT_DATA_PTR->fighter[index];
	found = 0;

	for (i = 0; i < 4; ++i) {
		if (VS_isMoveUsable(digimon, fighter, i) != 0) {
			out[i] = 1;
			found = 1;
		} else {
			out[i] = 0;
		}
	}

	return found;
}

int16_t VS_getAttackTech(AttackObject *attack)
{
	DigimonEntity *digimon;
	int16_t slot;
	int16_t tech;
	int32_t i;

	if (attack->effectId != 0x179) {
		slot = VS_getFighterSlot(attack->casterId);
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

int32_t VS_applyBuffMove(DigimonEntity *digimon, int32_t slot, int32_t anim)
{
	Stats *stats;

	stats = &digimon->stats;
	switch (anim) {
	case 0x29:
		VS_buffStats(digimon, slot, stats->base.off * 3 / 10, &stats->base.off, 0xb, 3);
		break;
	case 0x2a:
		VS_buffStats(digimon, slot, stats->base.off / 10, &stats->base.off, 0xb, 3);
		VS_buffStats(digimon, slot, stats->base.def * 5 / 100, &stats->base.def, 0xb, 4);
		VS_buffStats(digimon, slot, stats->base.speed / 10, &stats->base.speed, 0xb, 5);
		break;
	case 0x22:
		VS_buffStats(digimon, slot, stats->base.def / 5, &stats->base.def, 0xb, 4);
		VS_buffStats(digimon, slot, stats->base.speed / 10, &stats->base.speed, 0xb, 5);
		break;
	case 0x15:
		VS_buffStats(digimon, slot, stats->base.off * 7 / 100, &stats->base.off, 0xb, 3);
		VS_buffStats(digimon, slot, stats->base.def * 8 / 100, &stats->base.def, 0xb, 4);
		VS_buffStats(digimon, slot, stats->base.speed * 7 / 100, &stats->base.speed, 0xb, 5);
		break;
	case 0x1e:
		VS_buffStats(digimon, slot, stats->base.off / 4, &stats->base.off, 0xb, 3);
		VS_buffStats(digimon, slot, stats->base.def * 3 / 20, &stats->base.def, 0xb, 4);
		VS_buffStats(digimon, slot, stats->base.speed * 3 / 20, &stats->base.speed, 0xb, 5);
		break;
	default:
		return 0;
	}

	return 1;
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_applyPartnerStatsToFighter);

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_rollAttackOutcome);

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_handleHitReaction);

void VS_applyMoveStatus(DigimonEntity *digimon, FighterData *fighter, int32_t move)
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
				VS_addPoisonStatusVisual(digimon, fighter);
			}
			break;
		case 2:
			if (!(fighter->flags & 2)) {
				fighter->flags |= 2;
				fighter->confusionTimer = random(0x65) + 200;
				VS_addConfusionStatusVisual(digimon, fighter);
				VS_resetFighterAction(fighter);
			}
			break;
		case 3:
			if (!(fighter->flags & 4)) {
				fighter->flags |= 4;
				fighter->stunTimer = random(0x29) + 200;
				VS_addStunStatusVisual(digimon, fighter);
				VS_resetFighterAction(fighter);
			}
			break;
		case 4:
			if (!(fighter->flags & 8)) {
				fighter->flatTimer = -1;
				VS_removeStatusEffects(digimon, fighter);
				VS_resetFighterAction(fighter);
			}
			break;
		}
		if (fighter == COMBAT_DATA_PTR->fighter) {
			COMBAT_DATA_PTR->player.statusedCount++;
		}
	}
}

int16_t VS_getFighterSlot(int16_t entityId)
{
	int32_t i;

	for (i = 0; ENEMY_COUNT >= i; i++) {
		if (COMBAT_DATA_PTR->player.entityIds[i] == entityId) {
			return i;
		}
	}

	return -1;
}

int32_t VS_addBlockedAttack(FighterData *fighter, FighterData *other)
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

void VS_buffStats(DigimonEntity *digimon, int32_t slot, int16_t value, int16_t *stat, int16_t color, uint8_t flag)
{
	addWithLimit(stat, value, 0x3e7);
	addEntityText(digimon, slot, color, value, flag);
}

void VS_startAttackAnimation(Entity *entity, AttackObject *attack, int32_t anim)
{
	VS_getAttackTech(attack);
	startAnimation(entity, anim);
	createParticleFX(0, 1, &attack->position, entity, 0x11);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_resolveAttack);

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_isMoveUsable);

int32_t VS_getDistanceSquared(Entity *a, Entity *b)
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

void VS_setupQueuedMove(DigimonEntity *digimon, FighterData *fighter, int16_t arg2, int32_t moveIndex)
{
	int16_t tech;

	fighter->unk15 = 0;
	fighter->queuedAnim = digimon->stats.base.moves[moveIndex];
	tech = entityGetTechFromAnim(&digimon->entity, fighter->queuedAnim);
	fighter->moveRange = MOVE_DATA[tech].range;
	VS_applyChargeRequirement(digimon, fighter, tech);
	fighter->flags |= 0x40;
}

void VS_applyChargeRequirement(DigimonEntity *digimon, FighterData *fighter, int16_t tech)
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

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_startFighterMove);

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_selectMoveTarget);

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_playMoveEffect);

void VS_removeMoveEffect(DigimonEntity *digimon, FighterData *fighter)
{
	int32_t id;

	id = fighter->unk11;
	do {
		if (id != -1) {
			VS_stopEFESubEffect(id, *(int8_t *)&digimon->stats.current.unk2_1);
		}
	} while (0);
	*(int8_t *)&digimon->stats.current.unk2_1 = -1;
	fighter->unk11 = -1;
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_tickFrame);

void VS_addFinisherProgress(FighterData *fighter, int16_t amount)
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

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_updateFighterStatusVisuals);

void VS_clearStun(DigimonEntity *digimon, FighterData *fighter)
{
	VS_resetFighterAction(fighter);
	digimon->entity.anim.animFlag |= 1;
	fighter->flags &= ~4;
	fighter->stunTimer = 0;
	if (fighter->flags & 8) {
		return;
	}

	if (fighter->flatTimer != 0) {
		return;
	}

	VS_removeStatusEffectVisual(digimon, fighter, 3);
	if (fighter->flags & 2) {
		VS_addStatusEffectVisual(digimon, fighter, 2);
	}

	if (fighter->flags & 1) {
		VS_addStatusEffectVisual(digimon, fighter, 1);
	}
}

void VS_applyFlattenScale(VECTOR *scale, int32_t t)
{
	int32_t value;

	if (t < 0x40) {
		if (t >= 0x30) {
			VS_applyStretchScale(scale, (0x40 - t) << 4);
		} else if (t >= 0x20) {
			VS_applySquashScale(scale, (0x30 - t) << 4);
		} else if (t >= 0x18) {
			VS_applyStretchScale(scale, (0x20 - t) << 5);
		} else if (t >= 0x10) {
			VS_applySquashScale(scale, (0x18 - t) << 5);
		} else if (t >= 0xc) {
			VS_applyStretchScale(scale, (0x10 - t) << 6);
		} else if (t >= 8) {
			VS_applySquashScale(scale, (0xc - t) << 6);
		} else if (t >= 6) {
			VS_applyStretchScale(scale, (8 - t) << 7);
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

void VS_addStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, int32_t kind)
{
	if (fighter->statusFxId == -1) {
		switch (kind) {
		case 1:
			fighter->statusFxId = VS_addPoisonEffect(digimon);
			break;
		case 2:
			fighter->statusFxId = VS_addConfusionEffect(digimon);
			break;
		case 3:
			fighter->statusFxId = VS_addStunEffect(digimon, fighter->stunTimer);
			break;
		}
	}
}

void VS_applyStretchScale(VECTOR *scale, int32_t angle)
{
	int32_t half;

	scale->vy = _sin(angle) + 0x1000;
	half = 0x1000 - (_sin(angle) / 2);
	scale->vx = half;
	scale->vz = half;
}

void VS_applySquashScale(VECTOR *scale, int32_t angle)
{
	int32_t half;

	scale->vy = 0x1000 - (_sin(angle) / 2);
	half = (_sin(angle) / 2) + 0x1000;
	scale->vx = half;
	scale->vz = half;
}

void VS_removeStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, int32_t kind)
{
	int32_t id;

	id = fighter->statusFxId;
	do {
		if (id != -1) {
			switch (kind) {
			case 1:
				VS_removePoisonEffect(id, digimon);
				break;
			case 2:
				VS_removeConfusionEffect(id, digimon);
				break;
			case 3:
				VS_removeStunEffect(id, digimon);
				break;
			}
			fighter->statusFxId = -1;
		}
	} while (0);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_applyMoveResult);

void VS_resetFighterAction(FighterData *fighter)
{
	fighter->cooldown = 0;
	fighter->senileTimer = 0;
	fighter->flags &= 0xc7ff;
}

void VS_addPoisonStatusVisual(DigimonEntity *digimon, FighterData *fighter)
{
	if (!(fighter->flags & 0xe) && (fighter->flatTimer == 0)) {
		VS_addStatusEffectVisual(digimon, fighter, 1);
	}
}

void VS_addConfusionStatusVisual(DigimonEntity *digimon, FighterData *fighter)
{
	if (!(fighter->flags & 0xc) && (fighter->flatTimer == 0)) {
		if (fighter->flags & 1) {
			VS_removeStatusEffectVisual(digimon, fighter, 1);
		}
		VS_addStatusEffectVisual(digimon, fighter, 2);
	}
}

void VS_addStunStatusVisual(DigimonEntity *digimon, FighterData *fighter)
{
	if (!(fighter->flags & 8) && (fighter->flatTimer == 0)) {
		if (fighter->flags & 2) {
			VS_removeStatusEffectVisual(digimon, fighter, 2);
		}
		if (fighter->flags & 1) {
			VS_removeStatusEffectVisual(digimon, fighter, 1);
		}
		VS_addStatusEffectVisual(digimon, fighter, 3);
	}
}

void VS_removeStatusEffects(DigimonEntity *digimon, FighterData *fighter)
{
	if (fighter->flags & 4) {
		VS_removeStatusEffectVisual(digimon, fighter, 3);
	}

	if (fighter->flags & 2) {
		VS_removeStatusEffectVisual(digimon, fighter, 2);
	}

	if (fighter->flags & 1) {
		VS_removeStatusEffectVisual(digimon, fighter, 1);
	}
}

void VS_setFighterCooldown(DigimonEntity *digimon, FighterData *fighter)
{
	fighter->cooldown = 0x50;
	fighter->flags |= 0x800;
}

int16_t VS_getRandomUsableMove(int16_t *flags)
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

int16_t VS_getStrongestMove(int32_t index, int16_t *flags)
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

	VS_getHighestScoredMove(rank.score, flags, &rank.best, 3);

	return rank.best;
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_getMostEffectiveMove);

int16_t VS_getCheapestMove(int32_t index, int16_t *flags)
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

	VS_getLowestScoredMove(rank.score, flags, &rank.best, 3);

	return rank.best;
}

void VS_getHighestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count)
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

void VS_getLowestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count)
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

void VS_sortScoresDescending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count)
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

	VS_calculateScoreRanks(values, groups, count);
}

void VS_sortScoresAscending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count)
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

	VS_calculateScoreRanks(values, groups, count);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_selectPartnerMove);

int32_t VS_calculateElementBonus(int32_t arg0, int32_t arg1)
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

int32_t VS_countLivingEnemies(void)
{
	int32_t count;
	int32_t i;

	count = 0;
	for (i = 1; ENEMY_COUNT >= i; i++) {
		if (VS_isFighterDefeated(i) == 0) {
			count++;
		}
	}

	return count;
}

void VS_calculateScoreRanks(int32_t *values, int32_t *groups, int32_t count)
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

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_queueRandomMove);

int32_t VS_selectMoveByPower(int32_t arg0, int16_t *flags)
{
	int16_t result;

	result = VS_getStrongestMove(arg0, flags);
	if (result != -1) {
		return result;
	}

	result = VS_getMostEffectiveMove(arg0, flags);
	if (result != -1) {
		return result;
	}

	return VS_getRandomUsableMove(flags);
}

int32_t VS_selectMoveByMpCost(int32_t arg0, int16_t *flags)
{
	int16_t result;

	result = VS_getCheapestMove(arg0, flags);
	if (result != -1) {
		return result;
	}

	result = VS_getMostEffectiveMove(arg0, flags);
	if (result != -1) {
		return result;
	}

	return VS_getRandomUsableMove(flags);
}

uint8_t VS_isFighterDefeated(uint8_t index)
{
	if ((((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[index]])->stats.current.currentHP - COMBAT_DATA_PTR->fighter[index].hpDamageBuffer) <= 0) {
		return 1;
	}

	return 0;
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_main", VS_renderMoveName);

void VS_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, int32_t index)
{
	int16_t c;
	int16_t eff;
	int16_t n;

	if ((index >= 8U) && (index < 0xcU)) {
		eff = MOVE_DATA[entityGetTechFromAnim(&digimon->entity, digimon->stats.base.moves[index - 8])].special;
		prim->u0 = VS_D_800707C4[eff * 2];
		prim->v0 = VS_D_800707C5[eff * 2];
		prim->u1 = VS_D_800707C4[eff * 2] + 0x10;
		prim->v1 = VS_D_800707C5[eff * 2];
		prim->u2 = VS_D_800707C4[eff * 2];
		prim->v2 = VS_D_800707C5[eff * 2] + 0xf;
		prim->u3 = VS_D_800707C4[eff * 2] + 0x10;
		prim->v3 = VS_D_800707C5[eff * 2] + 0xf;
	} else {
		prim->u0 = VS_D_800707B4[(index - 1) * 2];
		prim->v0 = VS_D_800707B5[(index - 1) * 2];
		prim->u1 = VS_D_800707B4[(index - 1) * 2] + 0x10;
		prim->v1 = VS_D_800707B5[(index - 1) * 2];
		prim->u2 = VS_D_800707B4[(index - 1) * 2];
		prim->v2 = VS_D_800707B5[(index - 1) * 2] + 0xf;
		prim->u3 = VS_D_800707B4[(index - 1) * 2] + 0x10;
		prim->v3 = VS_D_800707B5[(index - 1) * 2] + 0xf;
	}
}
