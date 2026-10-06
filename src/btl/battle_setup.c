#include <dw/anim.h>
#include <dw/attack_object.h>
#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/item.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/script.h>

extern int8_t BATTLE_TOGGLE_LIFEBAR;
extern int16_t DEATH_COUNTDOWN;
extern int32_t IS_TAMERLESS_BATTLE;
extern int8_t TAMER_START_TILE_X;
extern int8_t TAMER_START_TILE_Y;
extern int8_t TAMER_WAYPOINT_ACTIVE;
extern int8_t TAMER_WAYPOINT_COUNT;
extern int8_t TAMER_WAYPOINT_X[];
extern int8_t TAMER_WAYPOINT_Y[];

void drawInventoryText(void);
void BTL_removeFinisherAura(int32_t arg0);
int32_t isInvisible(Entity *entity);
void entityLookAtTile(Entity *entity, int32_t tileX, int32_t tileY);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void partnerTickCollision(void);
void startBattleIdleAnimation(DigimonEntity *entity, Stats *stats, int32_t flags);
void playSound(int32_t soundId, uint32_t flag);
void clearTamerWaypoints(void);

int16_t BTL_CARDINAL_ROTATIONS[4] = { 0, 1024, 2048, 3072 };
uint8_t BRAIN_TO_COMMAND_MAP[5] = { 3, 4, 5, 7, 8 };

uint8_t BTL_SAVED_CHARGE_MODE;
long BTL_FINISHER_AURA_ID;
int32_t BTL_FINISHER_TIMER;

static void *battle_setup_sbss_order[] = {
	&BTL_FINISHER_TIMER,
	&BTL_FINISHER_AURA_ID,
	&BTL_SAVED_CHARGE_MODE,
};

void BTL_initializeCombat(void)
{
	int32_t i;
	FighterData *f;
	Stats *stats;
	int32_t j;
	int16_t *dst;
	DigimonEntity *digimon;
	int16_t brains;

	BTL_initializeDeathCountdown();
	resetFlattenGlobal();
	BTL_FINISHER_AURA_ID = -1;
	BTL_initializePartnerTile();
	BTL_initializeEnemyHPBarSprites();
	COMBAT_DATA_PTR->player.remainingChargeupTime[0] = -1;
	BTL_SAVED_CHARGE_MODE = PARTNER_ENTITY.digimonEntity.stats.current.chargeMode;
	FINISHING_ENTITY = NULL;
	FLEE_DISABLED[1] = 0;
	BTL_FINISHER_TIMER = 0;
	DEATH_COUNTDOWN = FLEE_TIMER = 0;
	BATTLE_TOGGLE_LIFEBAR = 0;
	BATTLE_FRAME_COUNT = 1;
	COMBAT_DATA_PTR->player.unk7 = 0;
	COMBAT_DATA_PTR->player.changeTarget = 0;
	NO_AI_FLAG = 0;
	HAS_TAKEN_DAMAGE = 0;
	IS_TAMERLESS_BATTLE = isTriggerSet(2);
	for (i = 0; i < 2; i++) {
		COMBAT_DATA_PTR->player.unk5[i] = 0xff;
	}

	initializeAttackObjects();
	for (i = 0; i < 0xc; i++) {
		COMBAT_DATA_PTR->player.usedMoves[i] = 0xff;
	}

	BTL_addEnemyHPBars();
	BTL_initializePartnerStatusBars();
	drawInventoryText();
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
		COMBAT_DATA_PTR->player.numCommands[0] = BRAIN_TO_COMMAND_MAP[brains / 100];
	} else {
		COMBAT_DATA_PTR->player.numCommands[0] = 9;
	}

	COMBAT_DATA_PTR->player.availableCommands[0][0] = 0xb;
	COMBAT_DATA_PTR->player.availableCommands[0][1] = 1;

	switch (COMBAT_DATA_PTR->player.numCommands[0]) {
	case 3:
		COMBAT_DATA_PTR->player.availableCommands[0][2] = 3;
		break;
	case 4:
		COMBAT_DATA_PTR->player.availableCommands[0][2] = 2;
		COMBAT_DATA_PTR->player.availableCommands[0][3] = 3;
		break;
	case 5:
		COMBAT_DATA_PTR->player.availableCommands[0][2] = 4;
		COMBAT_DATA_PTR->player.availableCommands[0][3] = 2;
		COMBAT_DATA_PTR->player.availableCommands[0][4] = 3;
		break;
	case 7:
		COMBAT_DATA_PTR->player.availableCommands[0][2] = 7;
		COMBAT_DATA_PTR->player.availableCommands[0][3] = 5;
		COMBAT_DATA_PTR->player.availableCommands[0][4] = 4;
		COMBAT_DATA_PTR->player.availableCommands[0][5] = 2;
		COMBAT_DATA_PTR->player.availableCommands[0][6] = 3;
		break;
	case 8:
		COMBAT_DATA_PTR->player.availableCommands[0][2] = 7;
		COMBAT_DATA_PTR->player.availableCommands[0][3] = 6;
		COMBAT_DATA_PTR->player.availableCommands[0][4] = 5;
		COMBAT_DATA_PTR->player.availableCommands[0][5] = 4;
		COMBAT_DATA_PTR->player.availableCommands[0][6] = 2;
		COMBAT_DATA_PTR->player.availableCommands[0][7] = 3;
		break;
	case 9:
		COMBAT_DATA_PTR->player.availableCommands[0][2] = 7;
		COMBAT_DATA_PTR->player.availableCommands[0][3] = 6;
		COMBAT_DATA_PTR->player.availableCommands[0][4] = 5;
		i = 5;
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

	COMBAT_DATA_PTR->player.startingHP = PARTNER_ENTITY.digimonEntity.stats.current.currentHP;
	BTL_initializeCommandMenu();
}

void BTL_handleBattleIntro(void)
{
	int32_t i;
	NPCEntity *npc;
	long j;
	int32_t timer;
	int32_t count;
	int16_t found[8];
	VECTOR pos;
	SVECTOR flee;
	int32_t allOffScreen;
	int32_t done;
	int16_t wp;
	int16_t result;
	int16_t tileX;
	int16_t tileY;

	startBattleIdleAnimation(&PARTNER_ENTITY.digimonEntity, &PARTNER_ENTITY.digimonEntity.stats, COMBAT_DATA_PTR->fighter[0].flags);
	entityLookAtLocation(&PARTNER_ENTITY.digimonEntity.entity, &ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->posData->location);
	BTL_initializeBattleStartText();
	count = 0;
	for (i = 2; i < 0xa; i++) {
		npc = (NPCEntity *)ENTITY_TABLE[i];
		if (isInvisible(&npc->digimonEntity.entity) == 0) {
			for (j = 1; j <= ENEMY_COUNT; j++) {
				if (COMBAT_DATA_PTR->player.entityIds[j] == i) {
					break;
				}
			}
			if (j == (ENEMY_COUNT + 1)) {
				startAnimation(&npc->digimonEntity.entity, 0x24);
				npc->digimonEntity.entity.anim.animFlag |= 4;
				flee = npc->flee;
				copyVector(&pos, &flee);
				entityLookAtLocation(&npc->digimonEntity.entity, &pos);
				found[count++] = i;
			}
		}
	}

	timer = 0;
	done = 0;
	if (count == 0) {
		allOffScreen = 1;
	} else {
		allOffScreen = 0;
	}

	wp = TAMER_WAYPOINT_COUNT - 1;
	TAMER_WAYPOINT_ACTIVE = 0;
	startAnimation(&TAMER_ENTITY.entity, 3);
	playSound(0, 0x10);

	while ((timer < 0xc8) || (allOffScreen == 0) || (done == 0)) {
		if (ENTITY_TABLE[0]->anim.animId != 1) {
			getModelTile(&ENTITY_TABLE[0]->posData->location, &tileX, &tileY);
			if (wp >= 0) {
				entityLookAtTile(ENTITY_TABLE[0], TAMER_WAYPOINT_X[wp], TAMER_WAYPOINT_Y[wp]);
				if ((tileX == TAMER_WAYPOINT_X[wp]) && (tileY == TAMER_WAYPOINT_Y[wp])) {
					wp--;
					TAMER_WAYPOINT_COUNT--;
				}
				result = entityCheckCollision(ENTITY_TABLE[1], ENTITY_TABLE[0], 0x136, 0xe6);
			} else {
				entityLookAtTile(ENTITY_TABLE[0], TAMER_START_TILE_X, TAMER_START_TILE_Y);
				if ((tileX == TAMER_START_TILE_X) && (tileY == TAMER_START_TILE_Y)) {
					result = 0xb;
				} else {
					result = entityCheckCollision(ENTITY_TABLE[1], ENTITY_TABLE[0], 0x12c, 0xdc);
				}
			}
		}
		if (wp == -1) {
			for (i = 1; i <= ENEMY_COUNT; i++) {
			}
		}
		if ((result == 0xb) && (ENTITY_TABLE[0]->anim.animId != 1)) {
			startAnimation(ENTITY_TABLE[0], 1);
			entityLookAtLocation(ENTITY_TABLE[0], &ENTITY_TABLE[1]->posData->location);
			timer = 0xc6;
		}
		timer++;
		for (i = 0; i < count; i++) {
			if (entityIsOffScreen(ENTITY_TABLE[found[i]], 0x140, 0xf0) == 0) {
				break;
			}
		}
		if (i == count) {
			allOffScreen = 1;
		}
		done = BTL_isBattleStartTextFinished();
		partnerTickCollision();
		entityLookAtLocation(&PARTNER_ENTITY.digimonEntity.entity, &ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->posData->location);
		BTL_battleTickFrame();
	}

	if (wp != -1) {
		TAMER_WAYPOINT_COUNT = wp + 1;
		TAMER_WAYPOINT_ACTIVE = TAMER_WAYPOINT_COUNT - 1;
	} else {
		clearTamerWaypoints();
	}

	if (ENTITY_TABLE[0]->anim.animId != 1) {
		startAnimation(ENTITY_TABLE[0], 1);
		entityLookAtLocation(ENTITY_TABLE[0], &ENTITY_TABLE[1]->posData->location);
	}

	for (i = 0; i < count; i++) {
		ENTITY_TABLE[found[i]]->isOnMap = 0;
		ENTITY_TABLE[found[i]]->anim.animFlag &= 0xfb;
	}

	BTL_removeBattleStartText();
	BTL_initializeBattleStartTextBurst();
	playSound(0, 0x11);
	while (BTL_isBattleStartTextFinished() == 0) {
		BTL_battleTickFrame();
	}

	BTL_removeBattleStartTextBurst();
	GAME_STATE = 1;
}

int32_t BTL_isBattleFinished(void)
{
	Entity *entity;
	Stats *stats;
	int32_t i;

	entity = ENTITY_TABLE[1];
	if ((FLEE_TIMER >= 0x29) && ((PARTNER_ENTITY.digimonEntity.stats.current.currentHP - COMBAT_DATA_PTR->fighter[0].hpDamageBuffer) > 0)) {
		return 1;
	}

	if (entity->anim.animId == 0x2c) {
		DEATH_COUNTDOWN = 0;
		if (!(entity->anim.animFlag & 1)) {
			COMBAT_DATA_PTR->fighter[0].flags &= ~FIGHTER_FLAG_DEAD;
			PARTNER_ENTITY.digimonEntity.stats.current.isHit = 0;
			if (NO_AI_FLAG != 0) {
				if (BTL_FINISHER_AURA_ID != -1) {
					BTL_removeFinisherAura(BTL_FINISHER_AURA_ID);
				}
				NO_AI_FLAG = 0;
				FINISHING_ENTITY = NULL;
			}
			for (i = 0; ENEMY_COUNT >= i; i++) {
				COMBAT_DATA_PTR->fighter[i].cooldown = 0;
				COMBAT_DATA_PTR->fighter[i].flags &= ~(FIGHTER_FLAG_TRANSFORMING | FIGHTER_FLAG_10 | FIGHTER_FLAG_ON_CHARGEUP | FIGHTER_FLAG_ON_COOLDOWN | FIGHTER_FLAG_SENILE);
				stats = &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]])->stats;
				if (!(COMBAT_DATA_PTR->fighter[i].flags & FIGHTER_FLAG_DEAD) || ((stats->current.currentHP - COMBAT_DATA_PTR->fighter[i].hpDamageBuffer) > 0)) {
					COMBAT_DATA_PTR->fighter[i].moveRange = 0;
				}
			}
		}
	}

	if ((entity->anim.animId == 0x2b) && !(entity->anim.animFlag & 1)) {
		if (TAMER_ITEM.worldItem.type == 0xff) {
			DEATH_COUNTDOWN++;
		}
		if (DEATH_COUNTDOWN == 0x14) {
			BTL_addDeathCountdown(ENTITY_TABLE[1]);
		}
		if (DEATH_COUNTDOWN >= 0xab) {
			return 1;
		}
		return 0;
	}

	for (i = 1; ENEMY_COUNT >= i; i++) {
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		if ((entity->anim.animId != 0x2b) || (entity->anim.animFlag & 1)) {
			return 0;
		}
	}

	if ((PARTNER_ENTITY.digimonEntity.stats.current.currentHP - COMBAT_DATA_PTR->fighter[0].hpDamageBuffer) <= 0) {
		return 0;
	}

	return 1;
}
