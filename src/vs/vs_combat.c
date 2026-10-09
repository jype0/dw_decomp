#include <string.h>

#include <inline_n.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#include <dw/attack_object.h>
#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/efe.h>
#include <dw/entity.h>
#include <dw/fade.h>
#include <dw/file_queue.h>
#include <dw/font.h>
#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/version.h>
#include <dw/vs.h>
#include <dw/world_object.h>

extern int16_t VS_STARTING_HP[2];
extern uint8_t VS_MUSIC;
extern int32_t VIEWPORT_DISTANCE;
extern int16_t VS_DAMAGE[2];
extern int32_t P2_AOE_TIMER;
extern int32_t VS_TIMER_ACTIVE;
extern int32_t VS_DISABLE_HITTING;
extern int16_t VS_DEFAULT_CAM_MIN_DISTANCE;
extern int32_t VS_CAMERA_STATE;
extern uint8_t VS_CAMERA_TIMER;
extern uint8_t VS_TIMER;
extern int16_t VS_DISCIPLINE[2];
extern uint8_t PAUSE_BOX_VISIBLE;
extern uint8_t PAUSE_STATE;
extern int16_t VS_CURRENT_BATTLE;
extern int32_t COMBAT_AREA_Y;
extern int32_t COMBAT_AREA_X;

void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
                  int32_t f, int32_t g, int32_t h, int32_t i);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uvX, int32_t uvY,
                      int32_t width, int32_t height);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY,
                       int32_t width, int32_t height);
void createPauseBox(void);
void removePauseBox(void);
void handleBattleIdle(DigimonEntity *entity, Stats *stats, int32_t flags);
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void collisionGrace(Entity *target, Entity *entity, int32_t dx, int32_t dy);
void removeEntityText(int32_t id);
void startAnimation(Entity *entity, uint8_t animId);
void tickAnimation(Entity *entity);

void VS__renderIntroStatNumber(int32_t x, int32_t y, int32_t digits, int32_t value,
                               int32_t layer);
void VS__combatInit(void);
void VS__combatSetup(void);
int16_t VS__checkEndCondition(void);
void VS__tickDigimonAI(int32_t fighterId);
void VS__tickBattle(void);
void VS__handlePause(void);
int16_t VS__deinitializeCombat(int16_t lostP1, int16_t lostP2);
uint8_t VS__isButtonsPressed(int32_t buttons);
void VS__deinitializeStatusEffects(void);
void VS__removePlayerMarker(void);
void VS__resetFlatten(int16_t combatId);
int32_t VS__areAllEnemyDigimonDead(void);
void VS__tickBattleResultScreen(int32_t hasLostP1, int32_t hasLostP2);
void VS__addTimeoutWindow(void);
void VS__faintDigimon(DigimonEntity *entity, FighterData *fighter,
                      int16_t fighterId);
int32_t VS__getDigitCount(int32_t value);
void VS__tickDigimonAttacking(Entity *entity, DigimonEntity *target,
                              int32_t fighterId);
void VS__tickDigimonHitByAttack(Entity *entity, FighterData *fighter,
                                int32_t fighterId);
void VS__tickDigimonFlat(DigimonEntity *entity, DigimonEntity *other,
                         FighterData *data, int32_t fighterId);
void VS__tickDigimonStun(Entity *entity);
void VS__tickDigimonConfusion(DigimonEntity *entity, DigimonEntity *other,
                              FighterData *data, int32_t fighterId);
void VS__tickDigimonSenile(DigimonEntity *entity, FighterData *data);
void VS__tickDigimonOnChargeup(DigimonEntity *entity, DigimonEntity *other,
                               FighterData *data);
void VS__tickDigimonOnCooldown(DigimonEntity *entity, DigimonEntity *other,
                               FighterData *data);
void VS__tickDigimonOther(DigimonEntity *entity, DigimonEntity *other,
                          FighterData *data, int32_t fighterId);
int32_t VS__tickDigimonHoldDistance(DigimonEntity *entity, DigimonEntity *other,
                                    FighterData *data);
int32_t VS__tickDigimonAttackClose(DigimonEntity *entity, DigimonEntity *other,
                                   FighterData *data, int16_t fighterId);
void VS__tickDigimonAttackRanged(DigimonEntity *entity, DigimonEntity *other,
                                 FighterData *data, int16_t move);
uint32_t VS__entityGetMoveWithHighestDistance(DigimonEntity *entity);
void VS__setWalking(Entity *entity, Stats *stats, uint16_t flags);
void VS__tickDigimonRotateKeepDistance(DigimonEntity *entity, DigimonEntity *other,
                                       FighterData *data);
void VS__tickDigimonAttackLookAtTarget(DigimonEntity *entity, VECTOR *location, int16_t dx,
                                       int16_t dy);
void VS__tickDigimonAttackingLogic(int32_t fighterId);
void VS__confusedRotate(Entity *entity);
void VS__tickDigimonWaitingDistance(DigimonEntity *entity, DigimonEntity *other,
                                    FighterData *data);
void VS__tickDigimonMaintainDistance(DigimonEntity *entity, DigimonEntity *other,
                                     FighterData *data, uint32_t min, uint32_t max);
int32_t VS__getBaseDistance(Entity *a, Entity *b);
void VS__increaseSpeedBuffer(FighterData *fighter, Stats *stats);
int32_t VS__hasAffordableMoves2(uint16_t *array, int16_t fighterId);
void VS___setWalking(Entity *entity, Stats *stats, uint16_t flags);
void VS__initializePlayerMarker(void);
void VS__clearFighterDataTables(FighterData *fighter);
void VS__tickDigimonRotationKeepDistanceCollision(Entity *entity, int16_t *rotationY, int16_t type,
                                                  int16_t oldRotation);
int32_t VS__combatMain(void);
void VS__renderTimeoutText(void);
void VS__renderTimeoutWindow(int32_t id);
void VS__renderPlayerMarker(int32_t id);
void VS___tickVSInput();
void VS__tickVSInput(void);
void VS__tickDigimonP1(int32_t instanceId);
void VS__tickDigimonP2(int32_t instanceId);

#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
void VS_setupDemoCombat(void);
int16_t VS_checkDemoEndCondition(void);
void VS_tickDemoDigimonAI(int32_t fighterId);
void VS_cleanupDemoCombat(void);
void VS_runDemoCombat(void);
#endif

static void *vs_combat_functions[] = {
#if VERSION_IS(US)
	VS__tickDigimonP2,
	VS__tickDigimonP1,
	VS__tickVSInput,
	VS___tickVSInput,
#endif
	VS__renderPlayerMarker,
	VS__renderTimeoutWindow,
	VS__renderTimeoutText,
	VS__combatMain,
#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
	VS_runDemoCombat,
#endif
	VS__tickDigimonRotationKeepDistanceCollision,
	VS__clearFighterDataTables,
	VS__initializePlayerMarker,
	VS___setWalking,
	VS__hasAffordableMoves2,
#if !VERSION_IS(JP_TRIAL) && !VERSION_IS(JP_BOMBOM)
	VS__increaseSpeedBuffer,
#endif
	VS__getBaseDistance,
	VS__tickDigimonMaintainDistance,
	VS__tickDigimonWaitingDistance,
	VS__confusedRotate,
	VS__tickDigimonAttackingLogic,
	VS__tickDigimonAttackLookAtTarget,
	VS__tickDigimonRotateKeepDistance,
	VS__setWalking,
	VS__entityGetMoveWithHighestDistance,
	VS__tickDigimonAttackRanged,
	VS__tickDigimonAttackClose,
	VS__tickDigimonHoldDistance,
	VS__tickDigimonOther,
	VS__tickDigimonOnCooldown,
	VS__tickDigimonOnChargeup,
	VS__tickDigimonSenile,
	VS__tickDigimonConfusion,
	VS__tickDigimonStun,
	VS__tickDigimonFlat,
	VS__tickDigimonHitByAttack,
	VS__tickDigimonAttacking,
	VS__getDigitCount,
#if !VERSION_IS(JP_TRIAL) && !VERSION_IS(JP_BOMBOM)
	VS__faintDigimon,
#endif
	VS__addTimeoutWindow,
	VS__tickBattleResultScreen,
#if !VERSION_IS(JP_TRIAL) && !VERSION_IS(JP_BOMBOM)
	VS__areAllEnemyDigimonDead,
	VS__resetFlatten,
#endif
	VS__removePlayerMarker,
#if !VERSION_IS(JP_TRIAL) && !VERSION_IS(JP_BOMBOM)
	VS__deinitializeStatusEffects,
#endif
	VS__isButtonsPressed,
	VS__deinitializeCombat,
	VS__handlePause,
#if !VERSION_IS(JP_TRIAL) && !VERSION_IS(JP_BOMBOM)
	VS__tickBattle,
#endif
	VS__tickDigimonAI,
	VS__checkEndCondition,
	VS__combatSetup,
#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
	VS__deinitializeStatusEffects,
	VS__areAllEnemyDigimonDead,
	VS__resetFlatten,
	VS__faintDigimon,
	VS__increaseSpeedBuffer,
	VS_cleanupDemoCombat,
	VS__tickBattle,
	VS_tickDemoDigimonAI,
	VS_checkDemoEndCondition,
	VS_setupDemoCombat,
#endif
	VS__combatInit,
};

// clang-format off

int16_t DIRECTIONS[4] = {
	0x0000, 0x0400, 0x0800, 0x0c00,
};

char VS__STR_DEALT[] = "与えた";

uint8_t VS__COMMANDS[8] = {
	0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00,
};

/* Damage */
char STR_DAMAGE[] = "ダメージ";

#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
int32_t VS_DEMO_SKIPPED;
#endif
uint8_t VS__PAUSING_PLAYER;
uint32_t VS__CURRENT_INPUT;
uint32_t VS__PREVIOUS_INPUT;
uint8_t VS__CHARGE_MODES[4];
int32_t VS__IS_DRAW;
uint8_t VS__BATTLE_RESULT_TIMER;
int32_t VS_FINISHER_TIMER;
int32_t VS_ACTIVE_FINISHER_AURA_ID;

static void *vs_combat_sbss_order[] = {
	&VS_ACTIVE_FINISHER_AURA_ID,
	&VS_FINISHER_TIMER,
	&VS__BATTLE_RESULT_TIMER,
	&VS__IS_DRAW,
	VS__CHARGE_MODES,
	&VS__PREVIOUS_INPUT,
	&VS__CURRENT_INPUT,
	&VS__PAUSING_PLAYER,
#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
	&VS_DEMO_SKIPPED,
#endif
};

// clang-format on

GARBAGE(VS__combatInit, 21);

void VS__combatInit(void)
{
	int32_t slot;
	int16_t *out;
	DigimonEntity *entity;
	long i;
	FighterData *fighter;
	Stats *stats;
	int16_t brain;

	VS_DAMAGE[0] = 0;
	VS_DAMAGE[1] = 0;
	VS__IS_DRAW = 0;

	resetFlattenGlobal();
	initializeAttackObjects();
	VS_addFighterCounter(99);

	if (ENTITY_TABLE[1]->type == ENTITY_TABLE[2]->type) {
		VS__initializePlayerMarker();
	}

	VS_ACTIVE_FINISHER_AURA_ID = -1;

	for (i = 0; i < 2; ++i) {
		VS__CHARGE_MODES[i] = ((DigimonEntity *)ENTITY_TABLE[i + 1])->stats.current.chargeMode;
		COMBAT_DATA_PTR->player.remainingChargeupTime[i] = -1;
	}

#if VERSION_IS(US)
	FLEE_DISABLED[1] = 0;
#endif
	FLEE_DISABLED[0] = 1;
	NO_AI_FLAG = 0;
	BATTLE_FRAME_COUNT = 1;
	VS_TIMER_ACTIVE = 0;
	VS_DISABLE_HITTING = 0;
#if !VERSION_IS(US)
	FLEE_DISABLED[1] = 0;
#endif
	P2_AOE_TIMER = 0;
	ENEMY_COUNT = 1;

	COMBAT_DATA_PTR->player.entityIds[0] = 1;
	COMBAT_DATA_PTR->player.entityIds[1] = 2;

	for (i = 0; i <= ENEMY_COUNT; ++i) {
		stats = &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]])->stats;
		VS_STARTING_HP[i] = stats->current.currentHP;
		out = (int16_t *)&INITIAL_COMBAT_STATS[i];
		*out++ = stats->base.hp;
		*out++ = stats->base.mp;
		*out++ = stats->base.off;
		*out++ = stats->base.def;
		*out++ = stats->base.speed;
		*out = stats->base.brain;
	}

	VS_addFighterStatusBars(0);
	VS_addFighterStatusBars(1);

	fighter = COMBAT_DATA_PTR->fighter;
	for (i = 0; i <= ENEMY_COUNT; ++i) {
		entity = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		stats = &entity->stats;
		stats->current.isHit = 0;

		fighter->targetId = (i + 1) & 1;
		fighter->hpDamageBuffer = 0;
		fighter->mpDamageBuffer = 0;
		fighter->flags = 0;
		fighter->moveRange = 0;
		fighter->flatTimer = 0;
		fighter->invulnerableTimer = 0;
		fighter->cooldown = 0;
		fighter->finisherProgress = 0;
		fighter->statusFxId = -1;
		fighter->activeEffectSlot = -1;
		fighter->speedBuffer = 100;
		fighter->unk15 = 0;
		fighter->hasCollidedWhileDistanceCmd = 0;

		if (stats->base.brain < 400) {
			fighter->buffsRemaining = stats->base.brain / 100 + 1;
		} else if (stats->base.brain < 600) {
			fighter->buffsRemaining = 4;
		} else {
			fighter->buffsRemaining = 5;
		}

		fighter->buffPrioTimer = stats->base.brain / 10 + 5;
		fighter->finisherGoal = 3000 - stats->base.speed;

		for (slot = 0; slot < 150; ++slot) {
			fighter->table1[slot] = -1;
			fighter->table2[i] = -1;
		}

		++fighter;
	}

	for (i = 0; i < 2; ++i) {
		brain = ((DigimonEntity *)ENTITY_TABLE[i + 1])->stats.base.brain;
		if (brain < 500) {
			COMBAT_DATA_PTR->player.numCommands[i] = VS__COMMANDS[brain / 100];
		} else {
			COMBAT_DATA_PTR->player.numCommands[i] = 7;
		}

		COMBAT_DATA_PTR->player.availableCommands[i][0] = 0xb;

		switch (COMBAT_DATA_PTR->player.numCommands[i]) {
		case 2:
			COMBAT_DATA_PTR->player.availableCommands[i][1] = 3;
			break;
		case 3:
			COMBAT_DATA_PTR->player.availableCommands[i][1] = 2;
			COMBAT_DATA_PTR->player.availableCommands[i][2] = 3;
			break;
		case 4:
			COMBAT_DATA_PTR->player.availableCommands[i][1] = 4;
			COMBAT_DATA_PTR->player.availableCommands[i][2] = 2;
			COMBAT_DATA_PTR->player.availableCommands[i][3] = 3;
			break;
		case 5:
			COMBAT_DATA_PTR->player.availableCommands[i][1] = 5;
			COMBAT_DATA_PTR->player.availableCommands[i][2] = 4;
			COMBAT_DATA_PTR->player.availableCommands[i][3] = 2;
			COMBAT_DATA_PTR->player.availableCommands[i][4] = 3;
			break;
		case 6:
			COMBAT_DATA_PTR->player.availableCommands[i][1] = 6;
			COMBAT_DATA_PTR->player.availableCommands[i][2] = 5;
			COMBAT_DATA_PTR->player.availableCommands[i][3] = 4;
			COMBAT_DATA_PTR->player.availableCommands[i][4] = 2;
			COMBAT_DATA_PTR->player.availableCommands[i][5] = 3;
			break;
		case 7:
			COMBAT_DATA_PTR->player.availableCommands[i][1] = 6;
			COMBAT_DATA_PTR->player.availableCommands[i][2] = 5;
			slot = 3;

			if (((DigimonEntity *)ENTITY_TABLE[i + 1])->stats.base.moves[2] != 0xff) {
				COMBAT_DATA_PTR->player.availableCommands[i][slot++] = 10;
			}

			if (((DigimonEntity *)ENTITY_TABLE[i + 1])->stats.base.moves[1] != 0xff) {
				COMBAT_DATA_PTR->player.availableCommands[i][slot++] = 9;
			}

			if (((DigimonEntity *)ENTITY_TABLE[i + 1])->stats.base.moves[0] != 0xff) {
				COMBAT_DATA_PTR->player.availableCommands[i][slot++] = 8;
			}

			COMBAT_DATA_PTR->player.availableCommands[i][slot++] = 3;
			COMBAT_DATA_PTR->player.numCommands[i] = slot;
			break;
		}

		COMBAT_DATA_PTR->player.hoveredCommand[i] =
			COMBAT_DATA_PTR->player.numCommands[i] - 1;
		COMBAT_DATA_PTR->player.currentCommand[i] = COMBAT_DATA_PTR->player.bufferedCommand[i] = 3;
	}

	VS_addCommandMenu(0);
	VS_addCommandMenu(1);
}

#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
void VS_setupDemoCombat(void)
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
	startAnimation(ENTITY_TABLE[1], 0x21);
	entityLookAtLocation(ENTITY_TABLE[1], &ENTITY_TABLE[2]->posData->location);
	startAnimation(ENTITY_TABLE[2], 0x21);
	entityLookAtLocation(ENTITY_TABLE[2], &ENTITY_TABLE[1]->posData->location);
	moveCount = 0;
	VS_initializeEFEEngine((char *)GENERAL_BUFFER_PTR);

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
	VS_loadMoveEFE(moves, effectIds, &isBusy);

	while (isBusy > 0) {
		tickFileReadQueue(0);
	}

	VS_DEFAULT_CAM_MIN_DISTANCE = 200;
	VS_initializeBattleStartText();

	frames = 0;
	finished = 0;
	playSound(0, 0x10);

	while (frames < 60 || finished == 0) {
		if (VS_DEFAULT_CAM_MIN_DISTANCE < 4200) {
			VS_DEFAULT_CAM_MIN_DISTANCE += 400;
		}

		++frames;
		finished = VS_isBattleStartTextFinished();
		if (POLLED_INPUT & 0x800) {
			VS_DEMO_SKIPPED = 1;
		}
		if (VS_DEMO_SKIPPED == 1) {
			break;
		}
		VS_tickFrame();
	}

	VS_removeBattleStartText();
	VS_initializeBattleStartTextBurst();
	playSound(0, 0x11);

	while (VS_isBattleStartTextFinished() == 0) {
		if (VS_DEFAULT_CAM_MIN_DISTANCE > 1000) {
			VS_DEFAULT_CAM_MIN_DISTANCE -= 400;
		}

		if (POLLED_INPUT & 0x800) {
			VS_DEMO_SKIPPED = 1;
		}
		if (VS_DEMO_SKIPPED == 1) {
			break;
		}
		VS_tickFrame();
	}

	VS_removeBattleStartTextBurst();

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

	VS_DEFAULT_CAM_MIN_DISTANCE = 1000;
	VS_TIMER_ACTIVE = 1;
	VS_CAMERA_STATE = 1;
	GAME_STATE = 4;
}

int16_t VS_checkDemoEndCondition(void)
{
	Entity *other;
	int32_t i;

#if VERSION_EQUAL_OR_NEWER(JP_TRIAL)
	if (COMBAT_DATA_PTR->fighter[0].hpDamageBuffer != 0) {
		return 0;
	}

	if (COMBAT_DATA_PTR->fighter[1].hpDamageBuffer != 0) {
		return 0;
	}
#endif

	if (ENTITY_TABLE[1]->anim.animId == 0x2b &&
	    (ENTITY_TABLE[1]->anim.animFlag & 1) == 0) {
		if (VS__areAllEnemyDigimonDead() == 0) {
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
		if (((DigimonEntity *)ENTITY_TABLE[1])->stats.current.currentHP -
		            COMBAT_DATA_PTR->fighter[0].hpDamageBuffer >
		    0) {
			return 1;
		}

		if (ENTITY_TABLE[1]->anim.animId != 0x2b ||
		    (ENTITY_TABLE[1]->anim.animFlag & 1) != 0) {
			return 0;
		}

		return 2;
	}

	if (VS_TIMER == 0) {
		if ((*(DigimonEntity **)&ENTITY_TABLE[1])->stats.current.currentHP -
		            (*(FighterData **)&COMBAT_DATA_PTR)->hpDamageBuffer <=
		    0) {
			return 0;
		}

		if (VS__areAllEnemyDigimonDead() != 0) {
			return 0;
		}

		VS_DAMAGE[0] = VS_STARTING_HP[0] - ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]])->stats.current.currentHP;
		VS_DAMAGE[1] = VS_STARTING_HP[1] - ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]])->stats.current.currentHP;
		COMBAT_DATA_PTR->fighter[0].hpDamageBuffer = 0;
		COMBAT_DATA_PTR->fighter[1].hpDamageBuffer = 0;

		if (VS_DAMAGE[0] > VS_DAMAGE[1]) {
			return -1;
		}

		if (VS_DAMAGE[0] < VS_DAMAGE[1]) {
			return 1;
		}

		if (VS_DAMAGE[0] == VS_DAMAGE[1]) {
			return 2;
		}
	}

	return 0;
}

// clang-format off
void VS_tickDemoDigimonAI(fighterId)
	int16_t fighterId;
// clang-format on
{
	FighterData *data;
	uint16_t *flags;
	FighterData *otherData;
	Stats *stats;
	DigimonEntity *entity;
	DigimonEntity *other;
	int16_t otherId;

	entity = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighterId]];
	stats = &entity->stats;
	data = &COMBAT_DATA_PTR->fighter[fighterId];
	flags = &data->flags;

	if (BATTLE_FRAME_COUNT % 20 == 0 && data->buffPrioTimer != 0) {
		data->buffPrioTimer--;
	}

	if (COMBAT_DATA_PTR->player.commandDelay[fighterId] == 0) {
		COMBAT_DATA_PTR->player.currentCommand[fighterId] =
			COMBAT_DATA_PTR->player.bufferedCommand[fighterId];
	} else if ((*flags & 0x800e) == 0 && data->flatTimer == 0) {
		COMBAT_DATA_PTR->player.commandDelay[fighterId]--;
	}

	if (NO_AI_FLAG == 0) {
		if ((stats->current.currentHP > data->hpDamageBuffer) &&
		    (stats->base.brain <= 300) &&
		    (BATTLE_FRAME_COUNT % ((stats->base.brain / 2 + 1) * 20) == 0) &&
		    (70 - VS_DISCIPLINE[fighterId] > randomLimit(100))) {
			*flags |= 0x2000;
			data->senileTimer = 100;
		}

		if (data->cooldown > 1) {
			data->cooldown--;
		}

		if ((*flags & 0x2000) == 0) {
			VS__increaseSpeedBuffer(data, stats);
		}
	}

	if ((*flags & 0x80b0) != 0) {
		return;
	}

	otherId = (fighterId + 1) & 1;
	other = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[otherId]];
	otherData = &COMBAT_DATA_PTR->fighter[otherId];

	if (stats->current.currentHP == 0) {
		VS__faintDigimon(entity, data, fighterId);

		VS_DISABLE_HITTING = 1;
		return;
	}

	if ((other->stats.current.currentHP <= otherData->hpDamageBuffer) ||
	    (entity->stats.current.currentHP <= data->hpDamageBuffer)) {
		handleBattleIdle(entity, stats, *flags);
		data->moveRange = -1;
		VS__resetFlatten(fighterId);
		VS_removeStatusEffects(entity, data);
		*flags = 0;
		*flags |= 0x40;
		return;
	}

	if ((*flags & 0x40) != 0) {
		return;
	}

	if ((*flags & 0x8) != 0) {
		data->queuedAnim = 0;
		data->targetId = otherId;
		data->moveRange = 2;
		entity->entity.flatSprite = 0;
		data->flags |= 0x40;
		return;
	}

	if ((*flags & 0x4) != 0) {
		return;
	}

	if ((*flags & 0x2) != 0) {
		VS_queueRandomMove(entity, data, (long)fighterId);
		return;
	}

	if ((*flags & 0x800) != 0) {
		return;
	}

	if ((*flags & 0x1000) != 0) {
		return;
	}

	if ((*flags & 0x2000) != 0) {
		return;
	}

	data->targetId = otherId;
	VS_selectPartnerMove(entity, data, fighterId);
}

void VS_cleanupDemoCombat(void)
{
	int32_t i;

	GAME_STATE = 5;
	VS__deinitializeStatusEffects();
	for (i = 0; i < 12; i++) {
		removeEFEFlash(i);
	}
	for (i = 0; i < 20; i++) {
		removeEntityParticleFX(i);
	}
	removeAllCloudFX();
	removeAllParticleFX();
	for (i = 0; i <= ENEMY_COUNT; i++) {
		removeEntityText(i);
		VS__resetFlatten(i);
		VS_removeStatusEffects((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]],
		                       &COMBAT_DATA_PTR->fighter[i]);
		COMBAT_DATA_PTR->fighter[i].flags = 0;
	}
	VS_removeFighterStatusBars(0);
	VS_removeFighterStatusBars(1);
	VS_removeFighterCounter();
	stopBGM();
	stopSound();
	GAME_STATE = 0;
}
#endif

void VS__combatSetup(void)
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
	startAnimation(ENTITY_TABLE[1], 0x21);
	entityLookAtLocation(ENTITY_TABLE[1], &ENTITY_TABLE[2]->posData->location);
	startAnimation(ENTITY_TABLE[2], 0x21);
	entityLookAtLocation(ENTITY_TABLE[2], &ENTITY_TABLE[1]->posData->location);
	moveCount = 0;
	VS_initializeEFEEngine((char *)GENERAL_BUFFER_PTR);

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
	VS_loadMoveEFE(moves, effectIds, &isBusy);

	while (isBusy > 0) {
		tickFileReadQueue(0);
	}

	VS_DEFAULT_CAM_MIN_DISTANCE = 200;
	VS_initializeBattleStartText();

	frames = 0;
	finished = 0;
	playSound(0, 0x10);

	while (frames < 60 || finished == 0) {
		if (VS_DEFAULT_CAM_MIN_DISTANCE < 4200) {
			VS_DEFAULT_CAM_MIN_DISTANCE += 400;
		}

		++frames;
		finished = VS_isBattleStartTextFinished();
		VS_tickFrame();
	}

	VS_removeBattleStartText();
	VS_initializeBattleStartTextBurst();
	playSound(0, 0x11);

	while (VS_isBattleStartTextFinished() == 0) {
		if (VS_DEFAULT_CAM_MIN_DISTANCE > 1000) {
			VS_DEFAULT_CAM_MIN_DISTANCE -= 400;
		}

		VS_tickFrame();
	}

	VS_removeBattleStartTextBurst();

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

	VS_DEFAULT_CAM_MIN_DISTANCE = 1000;
	VS_TIMER_ACTIVE = 1;
	VS_CAMERA_STATE = 1;
	GAME_STATE = 4;
}

int16_t VS__checkEndCondition(void)
{
	Entity *other;
	int32_t i;

#if VERSION_EQUAL_OR_NEWER(JP_TRIAL)
	if (COMBAT_DATA_PTR->fighter[0].hpDamageBuffer != 0) {
		return 0;
	}

	if (COMBAT_DATA_PTR->fighter[1].hpDamageBuffer != 0) {
		return 0;
	}
#endif

	if (ENTITY_TABLE[1]->anim.animId == 0x2b &&
	    (ENTITY_TABLE[1]->anim.animFlag & 1) == 0) {
		if (VS__areAllEnemyDigimonDead() == 0) {
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
		if (((DigimonEntity *)ENTITY_TABLE[1])->stats.current.currentHP -
		            COMBAT_DATA_PTR->fighter[0].hpDamageBuffer >
		    0) {
			return 1;
		}

		if (ENTITY_TABLE[1]->anim.animId != 0x2b ||
		    (ENTITY_TABLE[1]->anim.animFlag & 1) != 0) {
			return 0;
		}

		return 2;
	}

	if (VS_TIMER == 0) {
		if ((*(DigimonEntity **)&ENTITY_TABLE[1])->stats.current.currentHP -
		            (*(FighterData **)&COMBAT_DATA_PTR)->hpDamageBuffer <=
		    0) {
			return 0;
		}

		if (VS__areAllEnemyDigimonDead() != 0) {
			return 0;
		}

		VS_DAMAGE[0] = VS_STARTING_HP[0] - ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]])->stats.current.currentHP;
		VS_DAMAGE[1] = VS_STARTING_HP[1] - ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]])->stats.current.currentHP;
		COMBAT_DATA_PTR->fighter[0].hpDamageBuffer = 0;
		COMBAT_DATA_PTR->fighter[1].hpDamageBuffer = 0;

		if (VS_DAMAGE[0] > VS_DAMAGE[1]) {
			VS__tickBattleResultScreen(1, 0);
			return -1;
		}

		if (VS_DAMAGE[0] < VS_DAMAGE[1]) {
			VS__tickBattleResultScreen(0, 1);
			return 1;
		}

		if (VS_DAMAGE[0] == VS_DAMAGE[1]) {
			VS__tickBattleResultScreen(1, 1);
			return 2;
		}
	}

	return 0;
}

// clang-format off
void VS__tickDigimonAI(fighterId)
	int16_t fighterId;
// clang-format on
{
	DigimonEntity *entity;
	FighterData *data;
	uint16_t *flags;
	int32_t move;
	uint16_t array[4];
	FighterData *otherData;
	Stats *stats;
	DigimonEntity *other;
	int16_t otherId;

	entity = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighterId]];
	stats = &entity->stats;
	data = &COMBAT_DATA_PTR->fighter[fighterId];
	flags = &data->flags;

	if (BATTLE_FRAME_COUNT % 20 == 0 && data->buffPrioTimer != 0) {
		data->buffPrioTimer--;
	}

	if (COMBAT_DATA_PTR->player.commandDelay[fighterId] == 0) {
		COMBAT_DATA_PTR->player.currentCommand[fighterId] =
			COMBAT_DATA_PTR->player.bufferedCommand[fighterId];
	} else if ((*flags & 0x800e) == 0 && data->flatTimer == 0) {
		COMBAT_DATA_PTR->player.commandDelay[fighterId]--;
	}

	if (NO_AI_FLAG == 0) {
		if ((stats->current.currentHP > data->hpDamageBuffer) &&
		    (stats->base.brain <= 300) &&
		    (BATTLE_FRAME_COUNT % ((stats->base.brain / 2 + 1) * 20) == 0) &&
		    (70 - VS_DISCIPLINE[fighterId] > randomLimit(100))) {
			*flags |= 0x2000;
			data->senileTimer = 100;
		}

		if (data->cooldown > 1) {
			data->cooldown--;
		}

		if ((*flags & 0x2000) == 0) {
			VS__increaseSpeedBuffer(data, stats);
		}
	}

	if ((*flags & 0x80b0) != 0) {
		return;
	}

	otherId = (fighterId + 1) & 1;
	other = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[otherId]];
	otherData = &COMBAT_DATA_PTR->fighter[otherId];

	if (stats->current.currentHP == 0) {
		VS__faintDigimon(entity, data, fighterId);

		if (other->stats.current.currentHP > otherData->hpDamageBuffer) {
			VS_selectRandomCamera(entity, 5, 0);
		}

		VS_DISABLE_HITTING = 1;
		return;
	}

	if ((other->stats.current.currentHP <= otherData->hpDamageBuffer) ||
	    (entity->stats.current.currentHP <= data->hpDamageBuffer)) {
		handleBattleIdle(entity, stats, *flags);
		data->moveRange = -1;
		VS__resetFlatten(fighterId);
		VS_removeStatusEffects(entity, data);
		*flags = 0;
		*flags |= 0x40;
		return;
	}

	if (NO_AI_FLAG != 0) {
		return;
	}

	if ((*flags & 0x800e) == 0 && data->flatTimer == 0) {
		switch (COMBAT_DATA_PTR->player.currentCommand[fighterId]) {
		case 8:
		case 9:
		case 10:
			if (VS_isMoveUsable(entity, data, COMBAT_DATA_PTR->player.currentCommand[fighterId] - 8) != 0) {
				data->targetId = otherId;
				if ((entity->stats.base.moves[COMBAT_DATA_PTR->player.currentCommand[fighterId] - 8] !=
				     data->queuedAnim) ||
				    (data->moveRange <= 0)) {
					VS_setupQueuedMove(entity, data, fighterId,
					                   COMBAT_DATA_PTR->player.currentCommand[fighterId] - 8);
				}
				VS_applyChargeRequirement(entity, data,
				                          DIGIMON_DATA[entity->entity.type].moves[data->queuedAnim - 0x2e]);
				return;
			}
			break;
		case 11:
			data->targetId = otherId;
			if ((entity->stats.base.moves[3] != data->queuedAnim) || (data->moveRange <= 0)) {
				VS_setupQueuedMove(entity, data, fighterId, 3);
			}
			VS_applyChargeRequirement(entity, data,
			                          DIGIMON_DATA[entity->entity.type].moves[data->queuedAnim - 0x2e]);
			return;
		}

		if (COMBAT_DATA_PTR->player.currentCommand[fighterId] != 2 &&
		    COMBAT_DATA_PTR->player.currentCommand[fighterId] != 4) {
			entity->stats.current.chargeMode = VS__CHARGE_MODES[fighterId];
		}
	}

	if ((*flags & 0x40) != 0) {
		return;
	}

	if ((*flags & 0x8) != 0) {
		data->queuedAnim = 0;
		data->targetId = otherId;
		data->moveRange = 2;
		entity->entity.flatSprite = 0;
		data->flags |= 0x40;
		return;
	}

	if ((*flags & 0x4) != 0) {
		return;
	}

	if ((*flags & 0x2) != 0) {
		VS_queueRandomMove(entity, data, (long)fighterId);
		return;
	}

	if ((*flags & 0x800) != 0) {
		return;
	}

	if ((*flags & 0x1000) != 0) {
		return;
	}

	if ((*flags & 0x2000) != 0) {
		return;
	}

	data->targetId = otherId;
	move = -1;

	switch (COMBAT_DATA_PTR->player.currentCommand[fighterId]) {
	case 2:
		if (VS__hasAffordableMoves2(array, fighterId) == 0) {
			data->cooldown = 0x50;
			data->flags |= 0x800;
			return;
		}
		move = VS_selectMoveByPower((int16_t)fighterId, array);
		entity->stats.current.chargeMode = 0;
		break;
	case 4:
		if (VS__hasAffordableMoves2(array, fighterId) == 0) {
			data->cooldown = 0x50;
			data->flags |= 0x800;
			return;
		}
		move = VS_selectMoveByMpCost((int16_t)fighterId, array);
		entity->stats.current.chargeMode = 2;
		break;
	}

	if (move == -1) {
		VS_selectPartnerMove(entity, data, fighterId);
	} else {
		VS_setupQueuedMove(entity, data, fighterId, move);
	}
}

void VS__tickBattle(void)
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
		VS_addFinisherProgress(fighter, 1);
		id = fighter->targetId;

		if (id != 0xff) {
			target = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[id]];
		} else {
			target = NULL;
		}
		if (*flags & FIGHTER_FLAG_ATTACKING) {
			VS__tickDigimonAttacking(&entity->entity, target, i);
		} else if ((*flags & FIGHTER_FLAG_KNOCKED_BACK) || (*flags & FIGHTER_FLAG_BLOCKING)) {
			VS__tickDigimonHitByAttack(&entity->entity, fighter, i);
		} else if (fighter->moveRange != -1) {
			if (*flags & FIGHTER_FLAG_FLATTENED) {
				VS__tickDigimonFlat(entity, target, fighter, i);
			} else if (*flags & FIGHTER_FLAG_STUNNED) {
				VS__tickDigimonStun(&entity->entity);
			} else if (*flags & FIGHTER_FLAG_CONFUSED) {
				VS__tickDigimonConfusion(entity, target, fighter, i);
			} else if (*flags & FIGHTER_FLAG_SENILE) {
				VS__tickDigimonSenile(entity, fighter);
			} else if (*flags & FIGHTER_FLAG_ON_CHARGEUP) {
				VS__tickDigimonOnChargeup(entity, target, fighter);
			} else if (*flags & FIGHTER_FLAG_ON_COOLDOWN) {
				VS__tickDigimonOnCooldown(entity, target, fighter);
			} else {
				VS__tickDigimonOther(entity, target, fighter, i);
			}
		}
	}

	VS_resolveAttack();
	VS_applyMoveResult();
	if (VS_CAMERA_STATE != 7 && VS_CAMERA_STATE != 8) {
		if (VS_CAMERA_TIMER != 0) {
			VS_CAMERA_TIMER--;
			if (VS_CAMERA_TIMER == 0) {
				VS_CAMERA_STATE = 1;
			}
		}
		if ((BATTLE_FRAME_COUNT % 600) == 0 && randomLimit(2) == 1) {
			VS_CAMERA_STATE = 6;
			VS_setRandomViewpoint(ENTITY_TABLE[1], 4);
			VS_CAMERA_TIMER = randomLimit(0x29) + 0x3c;
		}
	}

	for (i = 0; ENEMY_COUNT >= i; i++) {
		if (COMBAT_DATA_PTR->fighter[i].flags & FIGHTER_FLAG_ATTACKING) {
			break;
		}
		if (COMBAT_DATA_PTR->fighter[i].flags & FIGHTER_FLAG_KNOCKED_BACK) {
			break;
		}
		if (VS_CAMERA_STATE == 7) {
			break;
		}
		if (VS_CAMERA_STATE == 8) {
			break;
		}
	}

	if (i == ENEMY_COUNT + 1) {
		VS_CAMERA_STATE = 1;
	}
}

void VS__handlePause(void)
{
	while (PAUSE_STATE != 0 && PAUSE_BOX_VISIBLE >= 2) {
		if (VS__PAUSING_PLAYER == VS__isButtonsPressed(0x800)) {
			PAUSE_STATE = (PAUSE_STATE + 1) & 1;
			VS__PAUSING_PLAYER = 0;
		}
	}

	if (VS__PAUSING_PLAYER == 0) {
		VS__PAUSING_PLAYER = VS__isButtonsPressed(0x800);
		if (VS__PAUSING_PLAYER != 0) {
			PAUSE_STATE = (PAUSE_STATE + 1) & 1;
		}
	}

	if (PAUSE_STATE != 0) {
		createPauseBox();
		++PAUSE_BOX_VISIBLE;
	} else {
		removePauseBox();
		PAUSE_BOX_VISIBLE = 0;
	}
}

int16_t VS__deinitializeCombat(int16_t lostP1, int16_t lostP2)
{
	int32_t i;
	int32_t j;
	int16_t frames;
	Stats *stats;

	((DigimonEntity *)ENTITY_TABLE[1])->stats.current.chargeMode = VS__CHARGE_MODES[0];
	((DigimonEntity *)ENTITY_TABLE[2])->stats.current.chargeMode = VS__CHARGE_MODES[1];
	GAME_STATE = 5;
	VS__deinitializeStatusEffects();

	for (i = 0; i <= ENEMY_COUNT; ++i) {
		removeEntityText(i);
		VS__resetFlatten(i);
		VS_removeStatusEffects((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]],
		                       &COMBAT_DATA_PTR->fighter[i]);
		COMBAT_DATA_PTR->fighter[i].flags = 0;
	}

	if (lostP1 == lostP2) {
		if (VS_TIMER != 0 || VS__IS_DRAW == 0) {
			stopBGM();
			stopSound();
			VS_loadVersusSceneModel();
			VS_addVersusModelScene();

			while (VS_isVersusModelSceneFinished() == 0) {
				VS_tickFrame();
			}

			VS_removeVersusModelScene();
		}
	} else {
		frames = 140;
	}

	if (lostP1 != lostP2) {
		startAnimation(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[lostP1]], 0x2a);
		ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[lostP1]]->anim.animFlag |= 2;
		j = 0;

		stopBGM();
		stopSound();
		playMusic(VS_MUSIC, 3);

		for (; j < frames; ++j) {
			VS_tickFrame();
			if ((ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[lostP1]]->anim.animFlag & 1) == 0) {
				startAnimation(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[lostP1]], 0x2a);
				ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[lostP1]]->anim.animFlag |= 2;
			}
		}
	}

	for (i = 0; i < 20; ++i) {
		VS_tickFrame();
	}

	VS_removeFighterStatusBars(0);
	VS_removeFighterStatusBars(1);
	VS_removeCommandMenu(0);
	VS_removeCommandMenu(1);
	VS_removeFighterCounter();
	stopBGM();
	stopSound();

	for (i = 0; i < 2; ++i) {
		stats = &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]])->stats;
		stats->base.off = INITIAL_COMBAT_STATS[i].offense;
		stats->base.def = INITIAL_COMBAT_STATS[i].defense;
		stats->base.speed = INITIAL_COMBAT_STATS[i].speed;
	}

	GAME_STATE = 0;

	if (lostP1 == lostP2) {
		return 2;
	}

	return lostP1;
}

uint8_t VS__isButtonsPressed(int32_t buttons)
{
	uint32_t pad;
	uint32_t prev;

	VS__PREVIOUS_INPUT = VS__CURRENT_INPUT;
	VS__CURRENT_INPUT = PadRead(1);

	if (buttons & (VS__CURRENT_INPUT & ~VS__PREVIOUS_INPUT)) {
		return 1;
	}

	pad = VS__CURRENT_INPUT;
	prev = VS__PREVIOUS_INPUT;
	VS__CURRENT_INPUT = (VS__CURRENT_INPUT >> 16) & 0xffff;
	VS__PREVIOUS_INPUT = (VS__PREVIOUS_INPUT >> 16) & 0xffff;

	if (buttons & (VS__CURRENT_INPUT & ~VS__PREVIOUS_INPUT)) {
		VS__CURRENT_INPUT = pad;
		VS__PREVIOUS_INPUT = prev;

		return 2;
	}

	VS__CURRENT_INPUT = pad;
	VS__PREVIOUS_INPUT = prev;
	return 0;
}

void VS__deinitializeStatusEffects(void)
{
	int32_t i;
	Entity *entity;

	if (ENTITY_TABLE[1]->type == ENTITY_TABLE[2]->type) {
		VS__removePlayerMarker();
	}
	VS_removeAllStunEffects();
	VS_removeAllFinisherAuras();
	VS_removeAllPoisonEffects();
	VS_removeAllAuraProjectiles();

	for (i = 0; i <= ENEMY_COUNT; ++i) {
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		VS_removeMoveEffect((DigimonEntity *)entity, &COMBAT_DATA_PTR->fighter[i]);
	}

	VS_unloadAllEFESlots();
	VS_removeEFEEngine();
}

void VS__removePlayerMarker(void)
{
	removeObject(0x1a3, 0);
	removeObject(0x1a3, 1);
}

void VS__resetFlatten(int16_t combatId)
{
	Entity *entity;
	FighterData *fighter;

	entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[combatId]];
	fighter = &COMBAT_DATA_PTR->fighter[combatId];

	entity->flatSprite = -1;
	fighter->flags &= ~FIGHTER_FLAG_FLATTENED;
	fighter->flatTimer = 0;
	entity->posData->scale.vx = 0x1000;
	entity->posData->scale.vy = 0x1000;
	entity->posData->scale.vz = 0x1000;
}

int32_t VS__areAllEnemyDigimonDead(void)
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

// clang-format off
void VS__tickBattleResultScreen(hasLostP1, hasLostP2)
	int16_t hasLostP1;
	int16_t hasLostP2;
// clang-format on
{
	VS__BATTLE_RESULT_TIMER = 0;
	addObject(0x1a2, 0, NULL, (RenderFunction)VS__renderTimeoutText);
	stopBGM();

	if (hasLostP1 == hasLostP2) {
		DigimonEntity *entityP1;
		DigimonEntity *entityP2;

		VS__IS_DRAW = 1;

		entityP1 = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]];
		entityP2 = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]];
		handleBattleIdle(entityP1, &entityP1->stats,
		                 COMBAT_DATA_PTR->fighter[0].flags);
		handleBattleIdle(entityP2, &entityP2->stats,
		                 COMBAT_DATA_PTR->fighter[1].flags);
		VS_tickFrame();
		VS_tickFrame();
		VS_loadVersusSceneModel();
		VS__addTimeoutWindow();

		while (VS__BATTLE_RESULT_TIMER < 61) {
			if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & CONFIRM_BUTTON) != 0) {
				break;
			}
			VS_tickFrame();
			++VS__BATTLE_RESULT_TIMER;
		}

		removeAnimatedUIBox(0, NULL);
		VS_addVersusModelScene();

		while (VS_isVersusModelSceneFinished() == 0) {
			VS_tickFrame();
		}

		VS_removeVersusModelScene();
	} else {
		DigimonEntity *winner;
		DigimonEntity *loser;

		winner = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[hasLostP1]];
		loser = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[hasLostP2]];

		entityLookAtLocation(&winner->entity, &loser->entity.posData->location);
		handleBattleIdle(winner, &winner->stats,
		                 COMBAT_DATA_PTR->fighter[hasLostP1].flags);
		VS__faintDigimon(loser, &COMBAT_DATA_PTR->fighter[hasLostP2],
		                 hasLostP2);

		while (VS__BATTLE_RESULT_TIMER < 121) {
			if (VS__BATTLE_RESULT_TIMER >= 61 &&
			    (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & CONFIRM_BUTTON) != 0) {
				break;
			}
			if (VS__BATTLE_RESULT_TIMER == 60) {
				VS__addTimeoutWindow();
			}
			VS_tickFrame();
			++VS__BATTLE_RESULT_TIMER;
		}

		VS_selectRandomCamera(loser, 5, 0);
		entityLookAtLocation(&winner->entity, &loser->entity.posData->location);
		handleBattleIdle(winner, &winner->stats,
		                 COMBAT_DATA_PTR->fighter[hasLostP1].flags);
		removeAnimatedUIBox(0, NULL);
	}
}

void VS__addTimeoutWindow(void)
{
	RECT finalPos;
	RECT startPos;
	Stats *stats;
	RegisteredDigimon *fighter1;
	RegisteredDigimon *fighter2;

	fighter1 = &VS_DIGIMON_P1_PTR[VS_BATTLE_SETUP.fighters[0][VS_CURRENT_BATTLE]];
	fighter2 = &VS_DIGIMON_P2_PTR[VS_BATTLE_SETUP.fighters[1][VS_CURRENT_BATTLE]];

	clearTextArea();
	drawString(VS__STR_DEALT, 6, 0);
	drawString(STR_DAMAGE, 0, 12);
	drawString(fighter1->name, (120 - (strlen(fighter1->name) * 6)) / 2, 24);
	drawString(fighter2->name, (120 - (strlen(fighter2->name) * 6)) / 2, 36);
	DrawSync(0);
	removeObject(0x1a2, 0);

	stats = &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]])->stats;
	setRECT(&startPos, -10, -10, 20, 20);

	setRECT(&finalPos, -132, -27, 264, 54);

	createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, NULL,
	                    VS__renderTimeoutWindow);
}

void VS__faintDigimon(DigimonEntity *entity, FighterData *fighter,
                      int16_t fighterId)
{
	entity->stats.current.isHit = 1;
	fighter->flags |= FIGHTER_FLAG_DEAD;

	startAnimation(&entity->entity, 0x2b);
	VS__resetFlatten(fighterId);
	VS_removeStatusEffects(entity, fighter);

	fighter->flags &= ~(FIGHTER_FLAG_POISONED | FIGHTER_FLAG_CONFUSED | FIGHTER_FLAG_STUNNED | FIGHTER_FLAG_FLATTENED | FIGHTER_FLAG_KNOCKED_BACK | FIGHTER_FLAG_ATTACKING | FIGHTER_FLAG_BLOCKING);
	fighter->flags |= FIGHTER_FLAG_TRANSFORMING;
	fighter->moveRange = -1;

	VS_resetFighterAction(fighter);
}

int32_t VS__getDigitCount(int32_t value)
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

void VS__tickDigimonAttacking(Entity *entity, DigimonEntity *target,
                              int32_t fighterId)
{
	long i;
	Entity *other;

	if ((entity->anim.animFlag & 1) == 0) {
		if (entity->anim.frameCount != entity->anim.animFrame) {
			for (i = 0; i <= ENEMY_COUNT; ++i) {
				if (i != fighterId) {
					other = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
					if (DIGIMON_DATA[other->type].moves[(uint32_t)(other->anim.animId - 0x2e)] == 0x2d) {
						if (fighterId == COMBAT_DATA_PTR->fighter[i].targetId) {
							return;
						}
					}
				}
			}
			entity->anim.animFlag |= 1;
		}
	}
	VS__tickDigimonAttackingLogic(fighterId);
}

void VS__tickDigimonHitByAttack(Entity *entity, FighterData *fighter,
                                int32_t fighterId)
{
	VS__tickDigimonAttackingLogic(fighterId);
	if ((entity->anim.animFlag & 1) == 0) {
		fighter->invulnerableTimer--;
		if (fighter->invulnerableTimer == 0) {
			entity->anim.animFlag |= 1;
		}
	}
}

void VS__tickDigimonFlat(DigimonEntity *entity, DigimonEntity *other,
                         FighterData *data, int32_t fighterId)
{
	if (NO_AI_FLAG != 0) {
		handleBattleIdle(entity, &entity->stats, data->flags);
		return;
	}

	if (VS__tickDigimonHoldDistance(entity, other, data) != 0) {
		return;
	}

	VS__tickDigimonAttackRanged(entity, other, data, 0x79);

	if ((data->flags & FIGHTER_FLAG_ATTACKING) != 0) {
		entity->entity.flatSprite = 2;
	}
}

void VS__tickDigimonStun(Entity *entity)
{
	if (entity->anim.animId != 0x22) {
		startAnimation(entity, 0x22);
	}

	entity->anim.animFlag &= 0xfe;
}

void VS__tickDigimonConfusion(DigimonEntity *entity, DigimonEntity *other,
                              FighterData *data, int32_t fighterId)
{
	if (NO_AI_FLAG != 0) {
		handleBattleIdle(entity, &entity->stats, data->flags);
		return;
	}

	if ((data->flags & FIGHTER_FLAG_ON_COOLDOWN) != 0 &&
	    ((data->flags & FIGHTER_FLAG_ON_COOLDOWN) != 0 || (data->flags & FIGHTER_FLAG_ON_CHARGEUP) != 0)) {
		VS__confusedRotate(&entity->entity);
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		collisionGrace(NULL, &entity->entity, 280, 200);
		if (data->cooldown < 2) {
			data->flags &= ~FIGHTER_FLAG_ON_COOLDOWN;
			data->cooldown = 0;
		}
	} else if (other == NULL) {
		VS__confusedRotate(&entity->entity);
		if (VS__tickDigimonAttackClose(entity, NULL, data, fighterId) != 0) {
			collisionGrace(NULL, &entity->entity, 280, 200);
		}

		if ((data->flags & FIGHTER_FLAG_ATTACKING) != 0) {
			return;
		}

		if (randomLimit(100) < 5) {
			handleBattleIdle(entity, &entity->stats, data->flags);
			VS_startFighterMove(entity, other, data);
		}
	} else {
		switch (data->moveRange) {
		case 1:
			if (VS__tickDigimonAttackClose(entity, other, data, fighterId) != 0) {
				collisionGrace(&other->entity,
				               &entity->entity, 280, 200);
			}
			break;
		case 2:
		case 3:
			VS__tickDigimonAttackRanged(entity, other, data,
			                            entityGetTechFromAnim(&entity->entity,
			                                                  data->queuedAnim));
			break;
		case 4:
			handleBattleIdle(entity, &entity->stats, data->flags);
			VS_startFighterMove(entity, other, data);
			break;
		}
	}
}

void VS__tickDigimonSenile(DigimonEntity *entity, FighterData *data)
{
	data->senileTimer--;
	if (data->senileTimer == 0) {
		data->flags &= ~(FIGHTER_FLAG_TRANSFORMING | FIGHTER_FLAG_SENILE);
	} else {
		handleBattleIdle(entity, &entity->stats, data->flags);
	}
}

void VS__tickDigimonOnChargeup(DigimonEntity *entity, DigimonEntity *other,
                               FighterData *data)
{
	int32_t result;
	int16_t tech;

	if (NO_AI_FLAG != 0) {
		handleBattleIdle(entity, &entity->stats, data->flags);
		return;
	}

	result = VS__tickDigimonHoldDistance(entity, other, data);
	if (data->cooldown != 0) {
		if (result == 0) {
			switch (entity->stats.current.chargeMode) {
			case 0:
				VS__tickDigimonWaitingDistance(entity, other, data);
				break;
			case 1:
				handleBattleIdle(entity, &entity->stats,
				                 data->flags);
				entityLookAtLocation(&entity->entity,
				                     &other->entity.posData->location);
				data->hasCollidedWhileDistanceCmd = 0;
				break;
			case 2:
				VS__setWalking(&entity->entity, &entity->stats,
				               data->flags);
				VS__tickDigimonRotateKeepDistance(entity, other, data);
				break;
			}
		}

		if (data->cooldown < 2) {
			data->flags &= ~(FIGHTER_FLAG_TRANSFORMING | FIGHTER_FLAG_ON_CHARGEUP);
			data->cooldown = 0;
		}
	} else if (result == 0) {
		switch (entity->stats.current.chargeMode) {
		case 0:
			handleBattleIdle(entity, &entity->stats, data->flags);
			entityLookAtLocation(&entity->entity,
			                     &other->entity.posData->location);
			data->hasCollidedWhileDistanceCmd = 0;
			if (data->speedBuffer > 0) {
				data->flags &= ~FIGHTER_FLAG_ON_CHARGEUP;
			}
			break;
		case 1:
			VS__tickDigimonWaitingDistance(entity, other, data);
			tech = (int16_t)entityGetTechFromAnim(&entity->entity,
			                                      data->queuedAnim);
			if (data->speedBuffer == 100 ||
			    data->speedBuffer >= MOVE_DATA[tech].power) {
				data->flags &= ~FIGHTER_FLAG_ON_CHARGEUP;
			}
			break;
		case 2:
			VS__setWalking(&entity->entity, &entity->stats,
			               data->flags);
			VS__tickDigimonRotateKeepDistance(entity, other, data);
			if (data->speedBuffer == 100) {
				data->flags &= ~FIGHTER_FLAG_ON_CHARGEUP;
			}
			break;
		}
	}
}

void VS__tickDigimonOnCooldown(DigimonEntity *entity, DigimonEntity *other,
                               FighterData *data)
{
	if (NO_AI_FLAG != 0) {
		handleBattleIdle(entity, &entity->stats, data->flags);
		return;
	}

	switch (entity->stats.current.chargeMode) {
	case 0:
	case 1:
		VS__tickDigimonWaitingDistance(entity, other, data);
		break;
	case 2:
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__tickDigimonRotateKeepDistance(entity, other, data);
		break;
	}

	if (data->cooldown < 2) {
		data->flags &= ~(FIGHTER_FLAG_TRANSFORMING | FIGHTER_FLAG_ON_COOLDOWN);
		data->cooldown = 0;
	}
}

void VS__tickDigimonOther(DigimonEntity *entity, DigimonEntity *other,
                          FighterData *data, int32_t fighterId)
{
	if (NO_AI_FLAG != 0 && FINISHING_ENTITY != &entity->entity) {
		handleBattleIdle(entity, &entity->stats, data->flags);
		return;
	}

	if (VS__tickDigimonHoldDistance(entity, other, data) != 0) {
		return;
	}

	switch (data->moveRange) {
	case 1:
		if (VS__tickDigimonAttackClose(entity, other, data, fighterId) != 0) {
			collisionGrace(&other->entity, &entity->entity, 280,
			               200);
		}
		break;
	case 2:
	case 3:
		VS__tickDigimonAttackRanged(entity, other, data,
		                            DIGIMON_DATA[entity->entity.type].moves[data->queuedAnim - 0x2e]);
		break;
	case 4:
		handleBattleIdle(entity, &entity->stats, data->flags);
		VS_startFighterMove(entity, other, data);
		break;
	}
}

int32_t VS__tickDigimonHoldDistance(DigimonEntity *entity, DigimonEntity *other,
                                    FighterData *data)
{
	int16_t id;
	uint32_t threshold;

	if (NO_AI_FLAG != 0) {
		return 0;
	}

	if (ENTITY_TABLE[1] == &entity->entity) {
		id = 0;
	} else {
		id = 1;
	}

	switch (COMBAT_DATA_PTR->player.currentCommand[id]) {
	case 6:
		handleBattleIdle(entity, &entity->stats, data->flags);
		entityLookAtLocation(&entity->entity,
		                     &ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[data->targetId]]->posData->location);
		data->hasCollidedWhileDistanceCmd = 0;

		return 1;
	case 5:
		threshold = VS__entityGetMoveWithHighestDistance(other) + 640000;
		if (entity->entity.anim.animId >= 0x23 &&
		    entity->entity.anim.animId < 0x25) {
			threshold += 160000;
		}

		if (VS_getDistanceSquared(&entity->entity, &other->entity) < threshold) {
			VS__setWalking(&entity->entity, &entity->stats, data->flags);
			VS__tickDigimonRotateKeepDistance(entity, other, data);
		} else {
			data->hasCollidedWhileDistanceCmd = 0;
			handleBattleIdle(entity, &entity->stats, data->flags);
			entityLookAtLocation(&entity->entity,
			                     &other->entity.posData->location);
		}
		return 1;
	}

	return 0;
}

int32_t VS__tickDigimonAttackClose(DigimonEntity *entity, DigimonEntity *other,
                                   FighterData *data, int16_t fighterId)
{
	int16_t *rotation;
	PositionData *pos;
	uint32_t distance;
	int32_t radius;
	int16_t savedRotation;
	int16_t tech;

	pos = entity->entity.posData;
	rotation = &pos->rotation.vy;
	entity->entity.anim.animFlag &= 0xfd;
	savedRotation = *rotation;
	if (other != NULL) {
		distance = VS_getDistanceSquared(&entity->entity, &other->entity);
		radius = DIGIMON_DATA[entity->entity.type].radius + DIGIMON_DATA[other->entity.type].radius;
		radius = radius * radius;
		if (distance <= radius) {
			handleBattleIdle(entity, &entity->stats, data->flags);
			if (NO_AI_FLAG != 0) {
				if (FINISHING_ENTITY != &entity->entity) {
					return 0;
				}
				if (VS_FINISHER_TIMER > 0) {
					VS_FINISHER_TIMER--;
					entityLookAtLocation(&entity->entity, &other->entity.posData->location);
					return 0;
				}
			} else {
				tech = entityGetTechFromAnim(&entity->entity, data->queuedAnim);
				if (tech >= 0x3a && tech < 0x71) {
					NO_AI_FLAG = 1;
				}
				if (NO_AI_FLAG != 0) {
					FINISHING_ENTITY = &entity->entity;
					VS_addTargetCursor(fighterId, tech);
					startAnimation(&entity->entity, data->queuedAnim);
					entity->entity.anim.animFlag &= 0xfe;
					VS_ACTIVE_FINISHER_AURA_ID = VS_addFinisherAura(&entity->entity, 80);
					VS_FINISHER_TIMER = 80;
					return 0;
				}
			}
			if (VS_selectMoveTarget((Entity *)entity, data) != 0) {
				return 0;
			}
			startAnimation(&entity->entity, data->queuedAnim);
			data->flags |= FIGHTER_FLAG_ATTACKING;
			VS_playMoveEffect(entity, other, data);
			return 0;
		}
		if (NO_AI_FLAG == 0) {
			VS__setWalking(&entity->entity, &entity->stats, data->flags);
			entityLookAtLocation(&entity->entity, &other->entity.posData->location);
			goto check;
		}
		if (FINISHING_ENTITY != &entity->entity) {
			return 0;
		}
		if (VS_FINISHER_TIMER > 0) {
			VS_FINISHER_TIMER--;
			entityLookAtLocation(&entity->entity, &other->entity.posData->location);
			return 0;
		}
		startAnimation(&entity->entity, data->queuedAnim);
		data->flags |= FIGHTER_FLAG_ATTACKING;
		VS_playMoveEffect(entity, other, data);
		return 0;
	}

	VS__setWalking(&entity->entity, &entity->stats, data->flags);
check:
	if (entityCheckCollision(NULL, &entity->entity, 280, 200) == -1) {
		return 0;
	} else {
		*rotation = savedRotation;
		return 1;
	}
}

void VS__tickDigimonAttackRanged(DigimonEntity *entity, DigimonEntity *other,
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
		VS_startFighterMove(entity, other, data);
		return;
	}

	distance = VS_getDistanceSquared(&entity->entity, &other->entity);
	total = DIGIMON_DATA[entity->entity.type].radius + DIGIMON_DATA[other->entity.type].radius;
	total = *(uint32_t *)&MOVE_DATA[move].distance + total * total;

	if (distance > total + total * 3 / 10) {
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__tickDigimonAttackLookAtTarget(entity, &other->entity.posData->location,
		                                  280, 200);
		++data->unk15;
	} else if (distance < total - total * 3 / 10) {
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__tickDigimonRotateKeepDistance(entity, other, data);
		++data->unk15;
	} else {
		data->unk15 = 0;

		handleBattleIdle(entity, &entity->stats, data->flags);

		if ((data->flags & FIGHTER_FLAG_FLATTENED) != 0) {
			if (BATTLE_FRAME_COUNT % 40 == 0) {
				VS_startFighterMove(entity, other, data);
			} else {
				entityLookAtLocation(&entity->entity,
				                     &other->entity.posData->location);
			}
		} else {
			VS_startFighterMove(entity, other, data);
		}
	}
}

uint32_t VS__entityGetMoveWithHighestDistance(DigimonEntity *entity)
{
	uint32_t highest;
	int32_t i;
	uint8_t tech;
	uint8_t move;

	highest = 0;
	for (i = 0; i < 4; ++i) {
		move = entity->stats.base.moves[i];
		if (move != 0xff) {
			tech = entityGetTechFromAnim(&entity->entity, move);
			if (highest < MOVE_DATA[tech].distance) {
				highest = MOVE_DATA[tech].distance;
			}
		}
	}

	return highest;
}

void VS__setWalking(Entity *entity, Stats *stats, uint16_t flags)
{
	if (entity->anim.animId == 0x24 || entity->anim.animId == 0x23) {
		return;
	}

	VS___setWalking(entity, stats, flags);
}

void VS__tickDigimonRotateKeepDistance(DigimonEntity *entity, DigimonEntity *other,
                                       FighterData *data)
{
	int16_t *targetRot;
	int16_t *rot;
	int16_t orig;
	int16_t away;
	int16_t ccw;
	int16_t cw;
	int16_t hit;
	int16_t diff;

	rot = &entity->entity.posData->rotation.vy;
	orig = *rot;
	entityLookAtLocation(&entity->entity, &other->entity.posData->location);
	away = (*rot + 0x800) & 0xfff;

	if (data->hasCollidedWhileDistanceCmd == 0) {
		*rot = away;

		hit = entityCheckCollision(NULL, &entity->entity, 280, 200);
		if (hit != -1) {
			data->hasCollidedWhileDistanceCmd = 1;
			VS__tickDigimonRotationKeepDistanceCollision(&entity->entity, rot, hit, orig);
		}

		return;
	}

	*rot = orig;
	targetRot = &other->entity.posData->rotation.vy;
	if (orig == *targetRot) {
		*rot = (*rot + 0x400) & 0xfff;

		if (entityCheckCollision(NULL, &entity->entity, 280, 200) == -1) {
			return;
		}

		*rot = orig;
		*rot = (*rot + 0xc00) & 0xfff;

		if (entityCheckCollision(NULL, &entity->entity, 280, 200) == -1) {
			return;
		}

		*rot = orig;

		hit = entityCheckCollision(NULL, &entity->entity, 280, 200);
		if (hit != -1) {
			VS__tickDigimonRotationKeepDistanceCollision(&entity->entity, rot, hit, orig);
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

	if (entityCheckCollision(NULL, &entity->entity, 280, 200) == -1) {
		return;
	}

	*rot = orig;

	hit = entityCheckCollision(NULL, &entity->entity, 280, 200);
	if (hit != -1) {
		VS__tickDigimonRotationKeepDistanceCollision(&entity->entity, rot, hit, orig);
	}
}

void VS__tickDigimonAttackLookAtTarget(DigimonEntity *entity, VECTOR *location, int16_t dx,
                                       int16_t dy)
{
	int16_t rotationY;

	rotationY = entity->entity.posData->rotation.vy;
	entityLookAtLocation(&entity->entity, location);

	if (entityCheckCollision(NULL, &entity->entity, dx, dy) != -1) {
		entity->entity.posData->rotation.vy = rotationY;
		collisionGrace(NULL, &entity->entity, dx, dy);
	}
}

void VS__tickDigimonAttackingLogic(int32_t fighterId)
{
	DigimonEntity *entity;
	FighterData *fighter;

	entity = (DigimonEntity *)
		ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighterId]];
	fighter = &COMBAT_DATA_PTR->fighter[fighterId];

	if (entityCheckCollision(NULL, &entity->entity, 280, 200) != -1) {
		entity->entity.anim.animFlag |= 2;
	} else {
		entity->entity.anim.animFlag &= 5;
	}

	if ((fighter->flags & (FIGHTER_FLAG_FLATTENED | FIGHTER_FLAG_ATTACKING)) == 0x28) {
		--fighter->flatAttackTimer;
		switch (fighter->flatAttackTimer) {
		case 28:
			VS_addAuraProjectile(&entity->entity);
			break;
		case 10:
			entity->entity.flatSprite = 0;
			break;
		case 0:
			entity->entity.anim.animFlag &= 0xfe;
			break;
		}
	}

	if (entity->entity.anim.animFlag & 1) {
		return;
	}

	if (FINISHING_ENTITY == &entity->entity) {
		NO_AI_FLAG = 0;
		FINISHING_ENTITY = NULL;
	}

	fighter->hasCollidedWhileDistanceCmd = 0;

	if (fighter->flags & FIGHTER_FLAG_ATTACKING) {
		if (fighterId == 0) {
			++COMBAT_DATA_PTR->player.hitCount;
		}

		fighter->flags &= ~FIGHTER_FLAG_10;
		fighter->flags |= FIGHTER_FLAG_ON_COOLDOWN;
		fighter->cooldown = 40;

		VS_addFinisherProgress(fighter, fighter->finisherGoal * 2 / 50);
	}

	if (fighter->invulnerableTimer <= 0) {
		if (fighter->flags & FIGHTER_FLAG_FLATTENED) {
			entity->entity.flatSprite = 0;
		}

		if ((fighter->flags & FIGHTER_FLAG_ATTACKING) == 0) {
			entity->stats.current.isHit = 0;
		}

		if (fighter->flags & FIGHTER_FLAG_BLOCKING) {
			fighter->flags &= ~FIGHTER_FLAG_BLOCKING;
			VS__clearFighterDataTables(fighter);
		} else {
			fighter->flags &= ~(FIGHTER_FLAG_KNOCKED_BACK | FIGHTER_FLAG_ATTACKING | FIGHTER_FLAG_TRANSFORMING | FIGHTER_FLAG_BLOCKING);
		}
	}

	if ((fighter->flags & FIGHTER_FLAG_KNOCKED_BACK) == 0) {
		if (fighter->flatTimer == -1) {
			fighter->flatTimer = 0x41;
		}
	} else {
		fighter->senileTimer = 0;
		fighter->flags &= ~FIGHTER_FLAG_SENILE;
	}
}

void VS__confusedRotate(Entity *entity)
{
	int16_t *rotationY;

	if (randomLimit(10) < 8) {
		return;
	}

	rotationY = &entity->posData->rotation.vy;

	*rotationY += randomLimit(0x400) - 0x200;
	if (*rotationY < 0) {
		*rotationY += 4096;
	} else {
		*rotationY %= 4096;
	}
}

void VS__tickDigimonWaitingDistance(DigimonEntity *entity, DigimonEntity *other,
                                    FighterData *data)
{
	if ((entity->entity.anim.animId >= 0x23) &&
	    (entity->entity.anim.animId < 0x25)) {
		VS__tickDigimonMaintainDistance(entity, other, data, 160000, 320000);
	} else {
		VS__tickDigimonMaintainDistance(entity, other, data, 0, 480000);
	}
}

void VS__tickDigimonMaintainDistance(DigimonEntity *entity, DigimonEntity *other,
                                     FighterData *data, uint32_t min, uint32_t max)
{
	uint32_t actualDistance;
	uint32_t baseDistance;

	actualDistance = VS_getDistanceSquared(&entity->entity, &other->entity);
	baseDistance = VS__getBaseDistance(&entity->entity, &other->entity);

	if (actualDistance < baseDistance + min) {
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__tickDigimonRotateKeepDistance(entity, other, data);
	} else if (actualDistance > baseDistance + max) {
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__tickDigimonAttackLookAtTarget(entity, &other->entity.posData->location,
		                                  280, 200);
	} else {
		data->hasCollidedWhileDistanceCmd = 0;
		handleBattleIdle(entity, &entity->stats, data->flags);
		entityLookAtLocation(&entity->entity,
		                     &other->entity.posData->location);
	}
}

int32_t VS__getBaseDistance(Entity *a, Entity *b)
{
	int32_t range;

	range = (DIGIMON_DATA[a->type].radius +
	         DIGIMON_DATA[b->type].radius + 200);

	return range * range;
}

void VS__increaseSpeedBuffer(FighterData *fighter, Stats *stats)
{
	if (fighter->speedBuffer < 100) {
		if (BATTLE_FRAME_COUNT % 2 == 0) {
			fighter->speedBuffer += (stats->base.speed / 100) + 1;
		}

		if (fighter->speedBuffer > 100) {
			fighter->speedBuffer = 100;
		}
	}
}

int32_t VS__hasAffordableMoves2(uint16_t *array, int16_t fighterId)
{
	Entity *entity;
	FighterData *fighter;
	int32_t found;
	int32_t i;

	entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighterId]];
	fighter = &COMBAT_DATA_PTR->fighter[fighterId];
	found = 0;

	for (i = 0; i < 4; ++i) {
		if (VS_isMoveUsable((DigimonEntity *)entity, fighter, i) != 0) {
			array[i] = 1;
			found = 1;
		} else {
			array[i] = 0;
		}
	}

	return found;
}

void VS___setWalking(Entity *entity, Stats *stats, uint16_t flags)
{
	int32_t walking;
	int32_t animId;

	walking = 1;

	if (!(flags & 1)) {
		if (stats->current.currentHP > stats->base.hp / 5) {
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

void VS__initializePlayerMarker(void)
{
	addObject(0x1a3, 0, NULL, VS__renderPlayerMarker);
	addObject(0x1a3, 1, NULL, VS__renderPlayerMarker);
}

void VS__clearFighterDataTables(FighterData *fighter)
{
	int32_t i;

	for (i = 0; i < 150; i++) {
		if (fighter->table1[i] == -1) {
			break;
		}
		fighter->table1[i] = -1;
		fighter->table2[i] = -1;
	}
}

void VS__tickDigimonRotationKeepDistanceCollision(Entity *entity, int16_t *rotationY, int16_t type,
                                                  int16_t oldRotation)
{
	int16_t angles[3];
	int32_t i;
	int16_t index;

	if (GAME_STATE == 4 && type == 10) {
		type = 11;
	}

	if (type == 11) {
		index = *rotationY / 1024;
		angles[0] = DIRECTIONS[index];
		angles[1] = (DIRECTIONS[index] + 0x400) & 0xfff;
		for (i = 0; i < 2; ++i) {
			*rotationY = angles[i];
			if (entityCheckCollision(NULL, entity, 0x118, 0xc8) == -1) {
				break;
			}
		}

		switch (i) {
		case 0:
			for (i = 0; i < 3; ++i) {
				*rotationY = angles[i] = (DIRECTIONS[index] + 0xc00 + i * 0x200) & 0xfff;
				if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
					break;
				}
			}
			break;
		case 1:
			for (i = 0; i < 3; ++i) {
				*rotationY = angles[i] = (DIRECTIONS[index] + 0x400 + i * 0x200) & 0xfff;
				if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
					break;
				}
			}
			break;
		default:
			for (i = 0; i < 3; ++i) {
				*rotationY = angles[i] = (DIRECTIONS[index] + 0x800 + i * 0x200) & 0xfff;
				if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
					break;
				}
			}
			if (i == 3) {
				*rotationY = (oldRotation + 0x800) & 0xfff;
				if (entityCheckCollision(NULL, entity, 0x118, 0xc8) == -1) {
					return;
				}
			}
			break;
		}

		if (i != 3) {
			*rotationY = oldRotation;
			collisionGrace(0, entity, 0x118, 0xc8);
		} else {
			*rotationY = angles[0] + randomLimit(0x400);
			if (entityCheckCollision(NULL, entity, 0x118, 0xc8) == -1) {
				return;
			}
			*rotationY = oldRotation;
			collisionGrace(0, entity, 0x118, 0xc8);
		}
	} else {
		*rotationY = oldRotation;
		collisionGrace(0, entity, 0x118, 0xc8);
	}
}

#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
void VS_runDemoCombat(void)
{
	int16_t result;

	VS_DEMO_SKIPPED = 0;
	VS__PAUSING_PLAYER = 0;
	COMBAT_AREA_X = 0;
	COMBAT_AREA_Y = 0;

	stopBGM();
	stopSound();
	playMusic(VS_MUSIC, 2);
	VS__combatInit();
	VS_removeCommandMenu(0);
	VS_removeCommandMenu(1);
	VS_removeFighterCounter();
	VS_addFighterCounter(0x1e);
	VS_setupDemoCombat();

	while (1) {
		result = VS_checkDemoEndCondition();
		if (POLLED_INPUT & 0x800) {
			VS_DEMO_SKIPPED = 1;
		}
		if ((result != 0) || (VS_DEMO_SKIPPED == 1)) {
			break;
		}

		VS_tickDemoDigimonAI(0);
		VS_tickDemoDigimonAI(1);
		VS__tickBattle();
		VS_tickFrame();
	}

	VS_tickFrame();
	fadeToBlack(1);
	VS_tickFrame();
	VS_tickFrame();
	VS_cleanupDemoCombat();
	stopBGM();
	stopSound();
	VS_removeResultModelScene();
}
#endif

int32_t VS__combatMain(void)
{
	int16_t result;

	VS__PAUSING_PLAYER = 0;
	COMBAT_AREA_X = 0;
	COMBAT_AREA_Y = 0;

	stopBGM();
	playMusic(VS_MUSIC, 2);
	VS__combatInit();
	VS__combatSetup();

	while (1) {
		result = VS__checkEndCondition();
		if (result != 0) {
			break;
		}

		VS__tickDigimonAI(0);
		VS__tickDigimonAI(1);
		VS__tickBattle();
		VS_tickFrame();
		VS__handlePause();
	}

	removePauseBox();

	if (result == -1) {
		VS__deinitializeCombat(1, 0);
	} else if (result == 1) {
		VS__deinitializeCombat(0, 1);
	} else {
		result = VS__deinitializeCombat(1, 1);
	}

	stopBGM();
	VS_removeResultModelScene();

	return result;
}

void VS__renderTimeoutText(void)
{
	POLY_FT4 *prim = (POLY_FT4 *)GsGetWorkBase();

	SetPolyFT4(prim);
	prim->tpage = getTPage(0, 0, 832, 0);
	setClut(prim, 16, 480);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setUVDataPolyFT4(prim, 0, 8, 144, 24);
	setPosDataPolyFT4(prim, -72, -12, 144, 24);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 5, prim++);

	GsSetWorkBase((PACKET *)prim);
}

void VS__renderTimeoutWindow(int32_t id)
{
	int16_t x;
	int16_t y;
	int32_t digits;

	x = UI_BOX_DATA[id].finalPos.x;
	y = UI_BOX_DATA[id].finalPos.y;
	renderString(0, x + 6, y + 6, 120, 12, 0, 24, 6 - id, 1);
	renderString(0, x + 138, y + 6, 120, 12, 0, 36, 6 - id, 1);
	renderString(0, x + 108, y + 24, 48, 24, 0, 0, 6 - id, 1);

	digits = VS__getDigitCount(VS_DAMAGE[1]);
	VS__renderIntroStatNumber(x + 42 + (48 - digits * 12) / 2, y + 30, digits,
	                          VS_DAMAGE[1], 6 - id);
	digits = VS__getDigitCount(VS_DAMAGE[0]);
	VS__renderIntroStatNumber(x + 174 + (48 - digits * 12) / 2, y + 30, digits,
	                          VS_DAMAGE[0], 6 - id);
}

// clang-format off
void VS__renderPlayerMarker(id)
	int16_t id;
// clang-format on
{
	POLY_FT4 *prim;
	MATRIX *m;
	Entity *entity;
	SVECTOR pos;
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
	prim->tpage = getTPage(0, 0, 384, 0);
	setClut(prim, 0, 498);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setUVWH(prim, id * 48 + 0x88, 0xd8, 48, 32);
	setXYWH(prim, sxy[0], sxy[1] - 32, 48, 32);
	AddPrim(ACTIVE_ORDERING_TABLE->org + otz, prim++);

	GsSetWorkBase((PACKET *)prim);
}

#if VERSION_IS(US)
void VS__tickVSInput(void)
{
	if (GAME_STATE == 4) {
		VS___tickVSInput();
	}
}

void VS___tickVSInput(int32_t player)
{
	uint32_t input;
	uint32_t previous;
	uint32_t cur;
	uint32_t prev;

	input = POLLED_INPUT;
	previous = POLLED_INPUT_PREVIOUS;

	cur = input;
	prev = previous;

	if (player == 1) {
		POLLED_INPUT = (cur >> 16) & 0xffff;
		POLLED_INPUT_PREVIOUS = (prev >> 16) & 0xffff;
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x8000) {
		uint32_t p;

		playSound(0, 2);

		p = player;
		++COMBAT_DATA_PTR->player.hoveredCommand[player];
		if ((COMBAT_DATA_PTR->player.numCommands[p] - 1) <
		    COMBAT_DATA_PTR->player.hoveredCommand[p]) {
			COMBAT_DATA_PTR->player.hoveredCommand[p] = 1;
		}
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x2000) {
		uint32_t p;

		playSound(0, 2);

		p = player;
		--COMBAT_DATA_PTR->player.hoveredCommand[player];
		if (COMBAT_DATA_PTR->player.hoveredCommand[p] <= 0) {
			COMBAT_DATA_PTR->player.hoveredCommand[p] =
				COMBAT_DATA_PTR->player.numCommands[p] - 1;
		}
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40) {
		uint32_t p;

		playSound(0, 3);

		p = player;
		COMBAT_DATA_PTR->player.bufferedCommand[p] =
			COMBAT_DATA_PTR->player.availableCommands[p][COMBAT_DATA_PTR->player.hoveredCommand[p]];
		if (VS_DISCIPLINE[p] < 70) {
			COMBAT_DATA_PTR->player.commandDelay[p] =
				160 - VS_DISCIPLINE[p] / 10;
		} else {
			COMBAT_DATA_PTR->player.commandDelay[p] =
				(10 - VS_DISCIPLINE[p] / 10) * 10;
		}

		COMBAT_DATA_PTR->player.commandDelay[p] = 0;
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x80) {
		unsigned long p;

		p = player;
		if (COMBAT_DATA_PTR->fighter[player].finisherProgress ==
		    COMBAT_DATA_PTR->fighter[player].finisherGoal) {
			COMBAT_DATA_PTR->player.bufferedCommand[p] = 0xb;
			playSound(0, 3);
			COMBAT_DATA_PTR->player.commandDelay[p] = 0;
			COMBAT_DATA_PTR->player.currentCommand[p] = 0xb;
		}
	}

	POLLED_INPUT = input;
	POLLED_INPUT_PREVIOUS = previous;
}

void VS__tickDigimonP1(int32_t instanceId)
{
	tickAnimation(ENTITY_TABLE[instanceId]);
}

void VS__tickDigimonP2(int32_t instanceId)
{
	tickAnimation(ENTITY_TABLE[instanceId]);
}
#endif
