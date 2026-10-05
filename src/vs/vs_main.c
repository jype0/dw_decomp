#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/attack_object.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/font.h>
#include <dw/item.h>
#include <dw/math.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/types.h>
#include <dw/vs.h>
#include <dw/world_object.h>

#include "common.h"

extern uint8_t TYPE_FACTORS[][7];
extern int16_t ENEMY_COUNT;
extern int32_t NO_AI_FLAG;
extern int32_t VS_DISABLE_HITTING;
extern int32_t FLEE_DISABLED[2];
extern int32_t P2_AOE_TIMER;
extern Entity *FINISHING_ENTITY;
extern int32_t VS_FINISHER_TIMER;
extern int32_t VS_ACTIVE_FINISHER_AURA_ID;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern int32_t DRAWING_OFFSET_Y;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[];
extern PACKET GS_WORK_BASES[2][0x14000];
extern DR_OFFSET DR_OFFSETS[2];
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t DRAWING_OFFSET_X;
extern int16_t MAIN_D_801352AC[2];
extern char *MOVE_NAMES[];
extern int32_t VS_CAMERA_STATE;
extern DigimonEntity *BATTLE_TARGETED_DIGIMON;
extern DigimonEntity *BATTLE_ATTACKING_DIGIMON;
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
                  int32_t f, int32_t g, int32_t h, int32_t i);
void createParticleFX();
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void renderObjects(void);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
void swapInt(int32_t *a, int32_t *b);
void startAnimation(Entity *entity, uint8_t animId);
int16_t VS_getAttackTech(AttackObject *attack);
int32_t VS_applyBuffMove(DigimonEntity *digimon, int32_t slot, int16_t anim);

int16_t VS_applyPartnerStatsToFighter(DigimonEntity *attacker, DigimonEntity *defender, FighterData *fighter, int16_t move);
int32_t VS_rollAttackOutcome(DigimonEntity *attacker, DigimonEntity *defender, int16_t move);
void VS_handleHitReaction(Entity *entity, FighterData *fighter, AttackObject *attack, int16_t index);
void VS_applyMoveStatus(DigimonEntity *digimon, FighterData *fighter, int32_t move);
int16_t VS_getFighterSlot(int16_t entityId);
int32_t VS_addBlockedAttack(FighterData *fighter, FighterData *other);
void VS_buffStats(DigimonEntity *digimon, int32_t slot, int32_t value, int16_t *stat, int32_t color, int32_t flag);
void VS_startAttackAnimation(Entity *entity, AttackObject *attack, uint8_t anim);
void VS_updateFighterStatusVisuals(DigimonEntity *digimon, FighterData *fighter);
void VS_clearStun(DigimonEntity *digimon, FighterData *fighter);
void VS_applyFlattenScale(VECTOR *scale, int32_t t);
void VS_addStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, uint8_t kind);
void VS_applyStretchScale(VECTOR *scale, int32_t angle);
void VS_applySquashScale(VECTOR *scale, int32_t angle);
void VS_removeStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, uint8_t kind);
void VS_addPoisonStatusVisual(DigimonEntity *digimon, FighterData *fighter);
void VS_addConfusionStatusVisual(DigimonEntity *digimon, FighterData *fighter);
void VS_addStunStatusVisual(DigimonEntity *digimon, FighterData *fighter);
int32_t VS_hasAffordableMoves(int16_t *out, int16_t index);
void VS_setFighterCooldown(DigimonEntity *digimon, FighterData *fighter);
int16_t VS_getRandomUsableMove(int16_t *flags);
int16_t VS_getStrongestMove(int32_t index, int16_t *flags);
int16_t VS_getMostEffectiveMove(int32_t index, int16_t *flags);
int16_t VS_getCheapestMove(int32_t index, int16_t *flags);
void VS_getHighestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count);
void VS_getLowestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count);
void VS_sortScoresDescending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count);
void VS_sortScoresAscending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count);
int16_t VS_calculateElementBonus(int16_t arg0, int16_t arg1);
int32_t VS_countLivingEnemies(void);
void VS_calculateScoreRanks(int32_t *values, int32_t *groups, int32_t count);
uint8_t VS_isFighterDefeated(uint8_t index);
void VS_renderMoveName(int32_t i);
void VS_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, int32_t index);
void addEntityText(DigimonEntity *digimon, long slot, int32_t color, int32_t value, uint8_t flag);
void addWithLimit(/* int16_t *value, int16_t amount, int16_t limit */);
void VS_stopEFESubEffect(int32_t a, int32_t b);
int32_t VS_addPoisonEffect(DigimonEntity *digimon);
void VS_removePoisonEffect(int32_t i, DigimonEntity *digimon);
int32_t VS_addConfusionEffect(DigimonEntity *digimon);
void VS_removeConfusionEffect(int32_t i, DigimonEntity *digimon);
int32_t VS_addStunEffect(DigimonEntity *digimon, int32_t val);
void VS_removeStunEffect(int32_t i, DigimonEntity *digimon);
void VS_removeTargetCursor(int16_t index);
void VS_removeFinisherAura(int32_t i);
int32_t VS_startEFE(int32_t script);

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

uint8_t VS_YOUR_CALL_POWER_PRIO[4] = { 50, 20, 5, 0 };
uint8_t VS_YOUR_CALL_MP_PRIO[4] = { 50, 20, 10, 0 };
uint8_t VS_YOUR_CALL_WIDE_PRIO[4] = { 10, 5, 0, 0 };
int8_t MAIN_D_80134AC8[2] = { -1, -1 };
#if defined(VERSION_JP)
char MAIN_D_80134ACC[] = "にげる";
char MAIN_D_80134AD0[] = "おもいっきり";
char MAIN_D_80134AD8[] = "おまかせ";
char VS_D_80070720[] = "ほどほど";
char VS_D_8007072C[] = "はなれる";
char VS_D_80070738[] = "ガマンする";
char MAIN_D_80134AE0[] = "ターゲットをかえる";
#else
char MAIN_D_80134ACC[] = "Run";
char MAIN_D_80134AD0[] = "Attack";
char MAIN_D_80134AD8[] = "Auto";
char VS_D_80070720[] = "Moderate";
char VS_D_8007072C[] = "Distance";
char VS_D_80070738[] = "Defensive";
char MAIN_D_80134AE0[] = "Change";
#endif

// clang-format off
char *VS_D_80070744[8] = {
	MAIN_D_80134ACC,
	MAIN_D_80134AD0,
	MAIN_D_80134AD8,
	VS_D_80070720,
	VS_D_8007072C,
	VS_D_80070738,
	MAIN_D_80134AE0,
	(void *)0x00000000,
};

uint8_t VS_D_80070764[8][10] = {
	{ 0x00, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x04, 0xff, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x03, 0xff, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x04, 0xff, 0xff },
	{ 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x03, 0xff },
};

uint8_t VS_D_800707B4[8][2] = {
	{ 0x00, 0xc0 },
	{ 0x20, 0xc0 },
	{ 0x40, 0xc0 },
	{ 0x60, 0xc0 },
	{ 0x80, 0xc0 },
	{ 0xa0, 0xc0 },
	{ 0xc0, 0xc0 },
	{ 0x00, 0x00 },
};

uint8_t VS_D_800707C4[8][2] = {
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

int32_t VS_hasAffordableMoves(int16_t *out, int16_t index)
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
	int16_t slot;
	int32_t i;
	int16_t tech;

	if (attack->effectId != 0x179) {
		slot = VS_getFighterSlot(attack->casterId);
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

int32_t VS_applyBuffMove(DigimonEntity *digimon, int32_t slot, int16_t anim)
{
	Stats *stats;

	stats = &digimon->stats;
	switch (anim) {
	case 0x29:
		VS_buffStats(digimon, slot, (int16_t)(stats->base.off * 3 / 10), &stats->base.off, 0xb, 3);
		break;
	case 0x2a:
		VS_buffStats(digimon, slot, (int16_t)(stats->base.off / 10), &stats->base.off, 0xb, 3);
		VS_buffStats(digimon, slot, (int16_t)(stats->base.def * 5 / 100), &stats->base.def, 0xb, 4);
		VS_buffStats(digimon, slot, (int16_t)(stats->base.speed / 10), &stats->base.speed, 0xb, 5);
		break;
	case 0x22:
		VS_buffStats(digimon, slot, (int16_t)(stats->base.def / 5), &stats->base.def, 0xb, 4);
		VS_buffStats(digimon, slot, (int16_t)(stats->base.speed / 10), &stats->base.speed, 0xb, 5);
		break;
	case 0x15:
		VS_buffStats(digimon, slot, (int16_t)(stats->base.off * 7 / 100), &stats->base.off, 0xb, 3);
		VS_buffStats(digimon, slot, (int16_t)(stats->base.def * 8 / 100), &stats->base.def, 0xb, 4);
		VS_buffStats(digimon, slot, (int16_t)(stats->base.speed * 7 / 100), &stats->base.speed, 0xb, 5);
		break;
	case 0x1e:
		VS_buffStats(digimon, slot, (int16_t)(stats->base.off / 4), &stats->base.off, 0xb, 3);
		VS_buffStats(digimon, slot, (int16_t)(stats->base.def * 3 / 20), &stats->base.def, 0xb, 4);
		VS_buffStats(digimon, slot, (int16_t)(stats->base.speed * 3 / 20), &stats->base.speed, 0xb, 5);
		break;
	default:
		return 0;
	}

	return 1;
}

int16_t VS_applyPartnerStatsToFighter(DigimonEntity *attacker, DigimonEntity *defender, FighterData *fighter, int16_t move)
{
	int32_t x;
	int16_t result;

	if (VS_DISABLE_HITTING != 0) {
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

int32_t VS_rollAttackOutcome(DigimonEntity *attacker, DigimonEntity *defender, int16_t move)
{
	int32_t diff;
	uint8_t eff[3];
	int16_t atk;
	int16_t def;
	long i;
	int32_t dmg;

	for (i = 0; i < 3; i++) {
		if (DIGIMON_DATA[defender->entity.type].special[i] != 0xff) {
			eff[i] = TYPE_FACTORS[MOVE_DATA[move].special][DIGIMON_DATA[defender->entity.type].special[i]];
		} else {
			eff[i] = 10;
		}
	}

	atk = attacker->stats.base.off;
	def = defender->stats.base.def;
	if (move == 0x2d) {
		def = def * 3 / 10;
	}

	if (move >= 0x3a && move < 0x71) {
		dmg = (atk + MOVE_DATA[move].power) * (eff[2] + (eff[0] + eff[1])) / 30;
		if (COMBAT_DATA_PTR->player.finisherChargeup[0] >= 0x29) {
			dmg = dmg * COMBAT_DATA_PTR->player.finisherChargeup[0] / 40;
		}
		dmg = dmg * (randomLimit(0x15) + 0x5a) / 100;
	} else {
		diff = atk - def;
		if (diff >= 0x1f5) {
			diff = 0x1f4;
		}
		if (diff < -0x1f4) {
			diff = -0x1f4;
		}
		dmg = (eff[2] + (eff[0] + eff[1])) * (MOVE_DATA[move].power + diff * MOVE_DATA[move].power / 500) / 30 * (randomLimit(0x15) + 0x5a) / 100;
	}

	if (dmg <= 0) {
		dmg = 1;
	}
	if (dmg >= 0x2710) {
		dmg = 0x270f;
	}

	return dmg;
}

void VS_handleHitReaction(Entity *entity, FighterData *fighter, AttackObject *attack, int16_t index)
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
			VS_startAttackAnimation(entity, attack, 0x28);
		} else {
			*rotY = _atan(d.vz, d.vx);
			VS_startAttackAnimation(entity, attack, 0x29);
		}
	} else if (!(0 > *rotY) && *rotY < 0x400) {
		if ((!(0 > ang) && ang <= *rotY + 0x400) || (ang >= *rotY + 0xc00 && ang < 0x1000)) {
			*rotY = ang;
			VS_startAttackAnimation(entity, attack, 0x28);
		} else {
			*rotY = _atan(d.vz, d.vx);
			VS_startAttackAnimation(entity, attack, 0x29);
		}
	} else {
		if ((*rotY - 0x400 <= ang && ang < 0x1000) || (!(0 > ang) && *rotY - 0xc00 >= ang)) {
			*rotY = ang;
			VS_startAttackAnimation(entity, attack, 0x28);
		} else {
			*rotY = _atan(d.vz, d.vx);
			VS_startAttackAnimation(entity, attack, 0x29);
		}
	}
}

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
	if (randomLimit(100) < chance) {
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
				fighter->confusionTimer = randomLimit(0x65) + 200;
				VS_addConfusionStatusVisual(digimon, fighter);
				VS_resetFighterAction(fighter);
			}
			break;
		case 3:
			if (!(fighter->flags & 4)) {
				fighter->flags |= 4;
				fighter->stunTimer = randomLimit(0x29) + 200;
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
void VS_buffStats(digimon, slot, value, stat, color, flag)
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

void VS_startAttackAnimation(Entity *entity, AttackObject *attack, uint8_t anim)
{
	int16_t tech;

	tech = VS_getAttackTech(attack);
	startAnimation(entity, anim);
	createParticleFX(0, 1, &attack->position, entity, 0x11);
}

void VS_resolveAttack(void)
{
	int32_t j;
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
		tech = VS_getAttackTech(&attack);
		if (DIGIMON_DATA[entity->type].moves[(uint32_t)(entity->anim.animId - 0x2e)] == 0x2d) {
			if (COMBAT_DATA_PTR->player.entityIds[fighter->targetId] == attack.casterId) {
				((DigimonEntity *)entity)->stats.current.isHit = 0;
				continue;
			}
		}
		if (VS_applyBuffMove((DigimonEntity *)entity, (int16_t)i, (int16_t)tech) != 0) {
			((DigimonEntity *)entity)->stats.current.isHit = 0;
			continue;
		}
		VS_removeMoveEffect((DigimonEntity *)entity, fighter);
		fighter->flags &= 0xff8f;
		attacker = ENTITY_TABLE[attack.casterId];
		chance = VS_applyPartnerStatsToFighter((DigimonEntity *)attacker, (DigimonEntity *)entity, fighter, tech);
		if (entity == FINISHING_ENTITY) {
			VS_removeTargetCursor(i);
			if (VS_ACTIVE_FINISHER_AURA_ID != -1L) {
				VS_removeFinisherAura(VS_ACTIVE_FINISHER_AURA_ID);
			}
			NO_AI_FLAG = 0;
			FINISHING_ENTITY = NULL;
		}
		if (randomLimit(100) < chance) {
			if (VS_CAMERA_STATE == 6 && MOVE_DATA[tech].range == 1) {
				goto skipViewpoint;
			}
			VS_setRandomViewpoint(entity, randomLimit(4));
skipViewpoint:
			dmg = VS_rollAttackOutcome((DigimonEntity *)attacker, (DigimonEntity *)entity, tech);
			fighter->hpDamageBuffer += dmg;
			if (fighter->hpDamageBuffer >= 0x2710) {
				fighter->hpDamageBuffer = 0x270f;
			}
			fighter->flags |= 0x10;
			VS_handleHitReaction(entity, fighter, &attack, i);
			sub->unk25 = 0;
			addEntityText((DigimonEntity *)entity, i, 0, dmg, 0);
			fighter->invulnerableTimer = MOVE_DATA[tech].iframes;
			entity->anim.animFlag &= 0xfe;
			VS_applyMoveStatus((DigimonEntity *)entity, fighter, tech);
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
						fighter->targetId = VS_getFighterSlot(attack.casterId);
						fighter->moveRange = 1;
						VS_startFighterMove((DigimonEntity *)entity, (DigimonEntity *)attacker, fighter);
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
		if (VS_addBlockedAttack(fighter, (FighterData *)&attack) != 0) {
			createParticleFX(0, 2, &attack.position, entity, 0x11);
		}
		if ((fighter->flags & 0x80) && (fighter->invulnerableTimer > 0)) {
			goto blocked;
		}
		if (MOVE_DATA[tech].range == 1) {
			dmg = VS_rollAttackOutcome((DigimonEntity *)attacker, (DigimonEntity *)entity, tech);
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
		VS_addFinisherProgress(fighter, fighter->finisherGoal * 3 / 50);
		for (j = 0; ENEMY_COUNT >= j; j++) {
			if (attacker == ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[j]]) {
				VS_addFinisherProgress(&COMBAT_DATA_PTR->fighter[j], COMBAT_DATA_PTR->fighter[j].finisherGoal * 4 / 50);
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

int32_t VS_isMoveUsable(DigimonEntity *digimon, FighterData *fighter, int16_t slot)
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

	mp = (int16_t)(MOVE_DATA[tech].mpCost * 3);
	if (digimon->stats.base.brain >= 700) {
		if (digimon->stats.base.brain == 999) {
			mp -= (int16_t)(mp / 5);
		} else if (digimon->stats.base.brain >= 900) {
			mp -= (int16_t)(mp * 15 / 100);
		} else if (digimon->stats.base.brain >= 800) {
			mp -= (int16_t)(mp / 10);
		} else {
			mp -= (int16_t)(mp / 20);
		}
	}

	if (digimon->stats.current.currentMP >= mp) {
		return 1;
	}

	return 0;
}

int32_t VS_getDistanceSquared(Entity *a, Entity *b)
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

void VS_setupQueuedMove(DigimonEntity *digimon, FighterData *fighter, int16_t arg2, uint8_t moveIndex)
{
	int16_t tech;
	Stats *stats;

	fighter->unk15 = 0;
	stats = &digimon->stats;
	fighter->queuedAnim = stats->base.moves[moveIndex];
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

void VS_startFighterMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
	int16_t tech;

	if (NO_AI_FLAG != 0) {
		if (FINISHING_ENTITY != &digimon->entity) {
			return;
		}
		if (VS_FINISHER_TIMER > 0) {
			VS_FINISHER_TIMER--;
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
			VS_addTargetCursor((int16_t)((&digimon->entity == ENTITY_TABLE[1]) ? 0 : 1), tech);
			entityLookAtLocation(&digimon->entity, &target->entity.posData->location);
			startAnimation(&digimon->entity, fighter->queuedAnim);
			digimon->entity.anim.animFlag &= 0xfe;
			VS_ACTIVE_FINISHER_AURA_ID = VS_addFinisherAura(&digimon->entity, 0x50);
			VS_FINISHER_TIMER = 0x50;
			return;
		}
	}
	if (VS_selectMoveTarget(&digimon->entity, fighter) != 0) {
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
	fighter->flags |= 0x20;
	if ((fighter->flags & 8) == 0) {
		VS_playMoveEffect(digimon, target, fighter);
		return;
	}
	fighter->flatAttackTimer = 0x1e;
}

int32_t VS_selectMoveTarget(Entity *entity, FighterData *fighter)
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

void VS_playMoveEffect(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter)
{
#ifdef __MWERKS__
	extern void VS_selectRandomCamera(DigimonEntity *, int32_t, uint8_t);
#endif
	int32_t tech;
	int16_t cost;
	int16_t brain;
	int32_t i;
	int32_t n;

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

	VS_removeMoveEffect(digimon, fighter);
	if ((tech >= 0x3a) && (tech < 0x71)) {
		NO_AI_FLAG = 1;
	}

	if (!((tech >= 0x3a) && (tech < 0x71))) {
		cost = MOVE_DATA[tech].mpCost * 3;
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

	digimon->stats.current.unk1 = tech + 0x100;
	for (i = 0; i < 3; i++) {
		if (fighter->queuedAnim == digimon->stats.base.moves[i]) {
			break;
		}
	}

	if (i != 4) {
		digimon->stats.current.efeSubEffect = VS_startEFE(fighter->effectSlot[i]);
		fighter->unk11 = fighter->effectSlot[i];
	}

	if ((MOVE_DATA[tech].range == 4) && (fighter->buffsRemaining != 0)) {
		fighter->buffsRemaining--;
	}

	fighter->speedBuffer -= MOVE_DATA[tech].power;
	if (fighter->speedBuffer < -0x9b) {
		fighter->speedBuffer = -0x9b;
	}

	if (VS_CAMERA_STATE == 3) {
		if (MOVE_DATA[entityGetTechFromAnim(VS_FOCUSED_ENTITY, VS_FOCUSED_ENTITY->anim.animId)].range == 3) {
			return;
		}
	}

	if (VS_CAMERA_STATE == 6 && VS_CAMERA_STATE == 3) {
		return;
	}

	if ((MOVE_DATA[tech].range == 1) || (MOVE_DATA[tech].range == 4)) {
		n = 2;
	} else {
		n = 5;
	}

	VS_selectRandomCamera(digimon, randomLimit(n), MOVE_DATA[tech].range);
}

void VS_removeMoveEffect(DigimonEntity *digimon, FighterData *fighter)
{
	if (fighter->unk11 != -1L) {
		VS_stopEFESubEffect(fighter->unk11, digimon->stats.current.efeSubEffect);
	}
	digimon->stats.current.efeSubEffect = -1;
	fighter->unk11 = -1;
}

void VS_tickFrame(void)
{
	POLLED_INPUT_PREVIOUS = POLLED_INPUT;
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
	GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
	GsSwapDispBuff();
	GsSortClear(0, 0, 0, ACTIVE_ORDERING_TABLE);
	GsDrawOt(ACTIVE_ORDERING_TABLE);
}

void VS_addFinisherProgress(FighterData *fighter, int16_t amount)
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

void VS_updateFighterStatusVisuals(DigimonEntity *digimon, FighterData *fighter)
{
	VS_resetFighterAction(fighter);
	if (digimon == (DigimonEntity *)ENTITY_TABLE[1]) {
		fighter->targetId = 1;
	} else {
		fighter->targetId = 0;
	}

	fighter->flags &= 0xfffd;
	fighter->confusionTimer = 0;
	if (((fighter->flags & 0xc) == 0) && (fighter->flatTimer == 0)) {
		fighter->flags &= 0xffbf;
		VS_removeStatusEffectVisual(digimon, fighter, 2);
		if (fighter->flags & 1) {
			VS_addStatusEffectVisual(digimon, fighter, 1);
		}
	}
}

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
			scale->vz = scale->vy = scale->vx = 0x1000 - ((6 - t) << 11);
		} else if (0 <= t) {
			scale->vz = scale->vy = scale->vx = (4 - t) << 10;
		}
	}
}

void VS_addStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, uint8_t kind)
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
	scale->vy = _sin(angle) + 0x1000;
	scale->vz = scale->vx = 0x1000 - (_sin(angle) / 2);
}

void VS_applySquashScale(VECTOR *scale, int32_t angle)
{
	scale->vy = 0x1000 - (_sin(angle) / 2);
	scale->vz = scale->vx = (_sin(angle) / 2) + 0x1000;
}

void VS_removeStatusEffectVisual(DigimonEntity *digimon, FighterData *fighter, uint8_t kind)
{
	if (fighter->statusFxId != -1L) {
		switch (kind) {
		case 1:
			VS_removePoisonEffect(fighter->statusFxId, digimon);
			break;
		case 2:
			VS_removeConfusionEffect(fighter->statusFxId, digimon);
			break;
		case 3:
			VS_removeStunEffect(fighter->statusFxId, digimon);
			break;
		}
		fighter->statusFxId = -1;
	}
}

void VS_applyMoveResult(void)
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
				VS_updateFighterStatusVisuals((DigimonEntity *)entity, fighter);
			}
		}
		if (fighter->flags & 4) {
			if (NO_AI_FLAG == 0) {
				fighter->stunTimer--;
			}
			if (fighter->stunTimer == 0) {
				VS_clearStun((DigimonEntity *)entity, fighter);
			}
		}
		if (fighter->flatTimer <= 0) {
			continue;
		}
		if ((NO_AI_FLAG == 0) || (fighter->flatTimer < 0x42)) {
			fighter->flatTimer--;
		}
		VS_applyFlattenScale(&entity->posData->scale, fighter->flatTimer);
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
					VS_addStatusEffectVisual((DigimonEntity *)entity, fighter, 3);
					fighter->moveRange = 0;
				}
				if (fighter->flags & 2) {
					VS_addStatusEffectVisual((DigimonEntity *)entity, fighter, 2);
				}
				if (fighter->flags & 1) {
					VS_addStatusEffectVisual((DigimonEntity *)entity, fighter, 1);
				}
				fighter->moveRange = 0;
				VS_resetFighterAction(fighter);
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

	return picked[randomLimit(count)];
}

// clang-format off
int16_t VS_getStrongestMove(index, flags)
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

	VS_getHighestScoredMove(score, flags, &best, 3);

	return best;
}

// clang-format off
int16_t VS_getMostEffectiveMove(index, flags)
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

	VS_getHighestScoredMove(score, flags, &best, 3);

	return best;
}

// clang-format off
int16_t VS_getCheapestMove(index, flags)
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

	VS_getLowestScoredMove(score, flags, &best, 3);

	return best;
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

void VS_getLowestScoredMove(int16_t *values, int16_t *marks, int16_t *out, int16_t count)
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

void VS_sortScoresDescending(int32_t *values, int32_t *keys, int32_t *groups, int32_t count)
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
			if (bestValue > values[j]) {
				best = j;
				bestValue = values[j];
			}
		}
		swapInt(&values[i], &values[best]);
		swapInt(&keys[i], &keys[best]);
	}

	VS_calculateScoreRanks(values, groups, count);
}

void VS_selectPartnerMove(DigimonEntity *digimon, FighterData *fighter, int16_t index)
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

	if (VS_hasAffordableMoves(flags, index) == 0) {
		VS_setFighterCooldown(digimon, fighter);
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
		VS_sortScoresDescending(values, keys, groups, 3);
		for (i = 0; i < 3; i++) {
			if (stats->base.moves[keys[i]] == 0xff) {
				weights[keys[i]] += 5;
			} else if (flags[keys[i]] != 0) {
				weights[keys[i]] = VS_YOUR_CALL_POWER_PRIO[groups[i]];
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
		VS_sortScoresAscending(values, keys, groups, 3);
		for (i = 0; i < 3; i++) {
			if (stats->base.moves[keys[i]] == 0xff) {
				weights[keys[i]] += 5;
			} else if (flags[keys[i]] != 0) {
				weights[keys[i]] = VS_YOUR_CALL_MP_PRIO[groups[i]];
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
			weights[i] += fighter->buffPrioTimer + VS_calculateElementBonus(MOVE_DATA[tech].special, DIGIMON_DATA[digimon->entity.type].special[0]);
			continue;
		}
		weights[i] += VS_calculateElementBonus(MOVE_DATA[tech].special, DIGIMON_DATA[ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighter->targetId]]->type].special[0]);
		if (MOVE_DATA[tech].range != 3) {
			continue;
		}
		count = VS_countLivingEnemies();
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
				VS_sortScoresDescending(values, keys, groups, 3);
				for (i = 0; i < 3; i++) {
					if (stats->base.moves[i] == 0xff) {
						continue;
					}
					if (flags[i] == 0) {
						continue;
					}
					weights[keys[i]] = VS_YOUR_CALL_WIDE_PRIO[groups[i]];
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
		VS_setupQueuedMove(digimon, fighter, index, i);
	} else {
		fighter->cooldown = 0x50;
		fighter->flags |= 0x800;
	}
}

int16_t VS_calculateElementBonus(int16_t arg0, int16_t arg1)
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
	groups[0] = group;
	for (i = 1; i < count; i++) {
		if (values[i] == (values + i)[-1]) {
			groups[i] = group;
		} else {
			groups[i] = ++group;
		}
	}
}

void VS_queueRandomMove(DigimonEntity *digimon, FighterData *fighter, int32_t tech)
{
	int16_t flags[4];

	if (randomLimit(10) < 7) {
		fighter->targetId = 0xff;
	} else {
		if (&digimon->entity == ENTITY_TABLE[1]) {
			fighter->targetId = 1;
		} else {
			fighter->targetId = 0;
		}
	}

	if (VS_hasAffordableMoves(flags, tech) == 0) {
		VS_setFighterCooldown(digimon, fighter);
	} else {
		VS_setupQueuedMove(digimon, fighter, tech, VS_getRandomUsableMove(flags));
	}
}

// clang-format off
int32_t VS_selectMoveByPower(arg0, flags)
	int32_t arg0;
	int16_t *flags;
// clang-format on
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

// clang-format off
int32_t VS_selectMoveByMpCost(arg0, flags)
	int32_t arg0;
	int16_t *flags;
// clang-format on
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

void VS_renderMoveName(int32_t i)
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
		                             ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]])->stats.base.moves[cmd - 8]);
		drawString(MOVE_NAMES[tech], 0, (i * 12) + 0xd8);
	} else {
		drawString(VS_D_80070744[cmd - 1], 0, (i * 12) + 0xd8);
	}

#if defined(VERSION_JP)
	renderString(0, (i * 160) - 0x8c, MAIN_D_801352AC[i] - 0xe, 0x90, 0xc, 0, (i * 12) + 0xd8, 7, 1);
#else
	renderString(0, (int32_t)(n * 160) - 0x8c, MAIN_D_801352AC[i] - 0xe, 0x90, 0xc, 0, (i * 12) + 0xd8, 7, 1);
#endif
}

void VS_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, int32_t index)
{
	int16_t c;
	int16_t eff;
	int16_t n;

	if ((index >= 8U) && (index < 0xcU)) {
		eff = MOVE_DATA[entityGetTechFromAnim(&digimon->entity, digimon->stats.base.moves[index - 8])].special;
		setUVWH(prim, VS_D_800707C4[eff][0], VS_D_800707C4[eff][1], 0x10, 0xf);
	} else {
		setUVWH(prim, VS_D_800707B4[index - 1][0], VS_D_800707B4[index - 1][1], 0x10, 0xf);
	}
}
