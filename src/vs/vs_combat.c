#include <string.h>

#include <inline_n.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#include <dw/anim.h>
#include <dw/attack_object.h>
#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/file_queue.h>
#include <dw/font.h>
#include <dw/garbage.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/vs.h>
#include <dw/world_object.h>

extern int16_t MAIN_D_8013527C[2];
extern uint8_t MAIN_D_80135274;
extern int32_t VIEWPORT_DISTANCE;
extern int16_t MAIN_D_80135280[2];
extern int32_t MAIN_D_80134D84;
extern int32_t MAIN_D_80134F40;
extern int32_t MAIN_D_80134F4C;
extern int32_t MAIN_D_8013528C;
extern int32_t MAIN_D_80135290;
extern uint8_t MAIN_D_80134F3C;
extern int16_t MAIN_D_80135294;
extern int32_t MAIN_D_80135268;
extern uint8_t MAIN_D_8013529C;
extern uint8_t MAIN_D_80135288;
extern int16_t MAIN_D_80135278[2];
extern uint8_t MAIN_D_80134E78[2];
extern uint8_t MAIN_D_80134F30;
extern uint8_t MAIN_D_80134F3D;
extern uint32_t MAIN_D_80134F34;
extern uint32_t MAIN_D_80134F38;
extern uint8_t MAIN_D_80134F44;
extern int16_t MAIN_D_80135264;
extern char *MAIN_D_8013526C;
extern char *MAIN_D_80135270;
extern int32_t MAIN_D_80134F48;
extern int32_t COMBAT_AREA_CENTER_Y;
extern int32_t COMBAT_AREA_CENTER_X;

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

void VS__renderIntroStatNumber(int32_t x, int32_t y, int32_t digits, int32_t value,
                               int32_t layer);
void VS__combatInit(void);
void VS__combatSetup(void);
int32_t VS__checkEndCondition(void);
void VS__tickDigimonAi(uint8_t fighterId);
void VS__tickBattle(void);
void VS__handlePause(void);
int32_t VS__deinitializeCombat(int16_t lostP1, int16_t lostP2);
int32_t VS__isButtonsPressed(int32_t buttons);
void VS__deinitializeStatusEffects(void);
void VS__removePlayerMarker(void);
void VS__resetFlatten(int16_t combatId);
int32_t VS__areAllEnemyDigimonDead(void);
void VS__tickBattleResultScreen(uint8_t hasLostP1, uint8_t hasLostP2);
void VS__addTimeoutWindow(void);
void VS__faintDigimon(DigimonEntity *entity, FighterData *fighter,
                      uint8_t fighterId);
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
                                     FighterData *data, int32_t min, int32_t max);
int32_t VS__getBaseDistance(Entity *a, Entity *b);
void VS__increaseSpeedBuffer(FighterData *fighter, Stats *stats);
int32_t VS__hasAffordableMoves2(uint16_t *array, uint8_t fighterId);
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

static void *vs_combat_functions[] = {
	VS__tickDigimonP2,
	VS__tickDigimonP1,
	VS__tickVSInput,
	VS___tickVSInput,
	VS__renderPlayerMarker,
	VS__renderTimeoutWindow,
	VS__renderTimeoutText,
	VS__combatMain,
	VS__tickDigimonRotationKeepDistanceCollision,
	VS__clearFighterDataTables,
	VS__initializePlayerMarker,
	VS___setWalking,
	VS__hasAffordableMoves2,
	VS__increaseSpeedBuffer,
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
	VS__faintDigimon,
	VS__addTimeoutWindow,
	VS__tickBattleResultScreen,
	VS__areAllEnemyDigimonDead,
	VS__resetFlatten,
	VS__removePlayerMarker,
	VS__deinitializeStatusEffects,
	VS__isButtonsPressed,
	VS__deinitializeCombat,
	VS__handlePause,
	VS__tickBattle,
	VS__tickDigimonAi,
	VS__checkEndCondition,
	VS__combatSetup,
	VS__combatInit,
};

// clang-format off

int16_t MAIN_D_80134510[4] = {
	0x0000, 0x0400, 0x0800, 0x0c00,
};

char MAIN_D_80134518[] = "与えた";

uint8_t MAIN_D_80134520[8] = {
	0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00,
};

/* Damage */
char STR_DAMEEJI[] = "ダメージ";

// clang-format on

GARBAGE(VS__combatInit, 21);

void VS__combatInit(void)
{
	Stats *stats;
	FighterData *fighter;
	int16_t *out;
	int32_t i;
	int32_t slot;
	int32_t brain;

	MAIN_D_80135280[0] = 0;
	MAIN_D_80135280[1] = 0;
	MAIN_D_80134F40 = 0;

	resetFlattenGlobal();
	initializeAttackObjects();
	VS_addFighterCounter(99);

	if (ENTITY_TABLE[1]->type == ENTITY_TABLE[2]->type) {
		VS__initializePlayerMarker();
	}

	MAIN_D_80134F4C = -1;

	for (i = 0; i < 2; ++i) {
		(&MAIN_D_80134F3C)[i] =
			((DigimonEntity *)(&ENTITY_TABLE[1])[i])->stats.current.chargeMode;
		((int8_t *)COMBAT_DATA_PTR->player.remainingChargeupTime)[i] = -1;
	}

	MAIN_D_80134D7C[1] = 0;
	MAIN_D_80134D7C[0] = 1;
	MAIN_D_80134D66 = 1;
	ENEMY_COUNT = 1;
	MAIN_D_80134D74 = 0;
	MAIN_D_8013528C = 0;
	MAIN_D_80135290 = 0;
	MAIN_D_80134D84 = 0;

	COMBAT_DATA_PTR->player.entityIds[0] = 1;
	COMBAT_DATA_PTR->player.entityIds[1] = 2;

	for (i = 0; i <= ENEMY_COUNT; ++i) {
		stats = &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]])->stats;
		MAIN_D_8013527C[i] = stats->current.currentHP;
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
		stats = &((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]])->stats;
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
		fighter->unk11 = -1;
		fighter->speedBuffer = 100;
		fighter->unk15 = 0;
		fighter->unk16 = 0;

		if (stats->base.brain < 400) {
			fighter->buffsRemaining = stats->base.brain / 100 + 1;
		} else if (stats->base.brain < 600) {
			fighter->buffsRemaining = 4;
		} else {
			fighter->buffsRemaining = 5;
		}

		fighter->buffPrioTimer = stats->base.brain / 10 + 5;
		slot = 3000 - stats->base.speed;

		fighter->finisherGoal = slot;

		for (slot = 0; slot < 150; ++slot) {
			((int8_t *)fighter + slot)[0x3c] = -1;
			((int8_t *)fighter + i)[0xd2] = -1;
		}

		++fighter;
	}

	for (i = 0; i < 2; ++i) {
		brain = ((DigimonEntity *)(&ENTITY_TABLE[1])[i])->stats.base.brain;
		if (brain < 500) {
			COMBAT_DATA_PTR->player.numCommands[i] = MAIN_D_80134520[brain / 100];
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

			if (((DigimonEntity *)(&ENTITY_TABLE[1])[i])->stats.base.moves[2] != 0xff) {
				COMBAT_DATA_PTR->player.availableCommands[i][slot++] = 10;
			}

			if (((DigimonEntity *)(&ENTITY_TABLE[1])[i])->stats.base.moves[1] != 0xff) {
				COMBAT_DATA_PTR->player.availableCommands[i][slot++] = 9;
			}

			if (((DigimonEntity *)(&ENTITY_TABLE[1])[i])->stats.base.moves[0] != 0xff) {
				COMBAT_DATA_PTR->player.availableCommands[i][slot++] = 8;
			}

			COMBAT_DATA_PTR->player.availableCommands[i][slot++] = 3;
			COMBAT_DATA_PTR->player.numCommands[i] = slot;
			break;
		}

		COMBAT_DATA_PTR->player.hoveredCommand[i] =
			COMBAT_DATA_PTR->player.numCommands[i] - 1;
		COMBAT_DATA_PTR->player.bufferedCommand[i] = 3;
		COMBAT_DATA_PTR->player.currentCommand[i] = 3;
	}

	VS_addCommandMenu(0);
	VS_addCommandMenu(1);
}

void VS__combatSetup(void)
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
			if ((move = entity->stats.base.moves[j]) != 0xff) {
				moves[moveCount++] =
					DIGIMON_DATA[entity->entity.type].moves[move - 0x2e] + 0x100;
			}
		}
	}

	moves[moveCount] = -1;
	VS_loadMoveEFE(moves, effectIds, &isBusy);

	while (isBusy > 0) {
		tickFileReadQueue(0);
	}

	MAIN_D_80135294 = 200;
	VS_initializeBattleStartText();

	frames = 0;
	finished = 0;
	playSound(0, 0x10);

	while (frames < 60 || finished == 0) {
		if (MAIN_D_80135294 < 4200) {
			MAIN_D_80135294 += 400;
		}

		++frames;
		finished = VS_isBattleStartTextFinished();
		VS_tickFrame();
	}

	VS_removeBattleStartText();
	VS_initializeBattleStartTextBurst();
	playSound(0, 0x11);

	while (VS_isBattleStartTextFinished() == 0) {
		if (MAIN_D_80135294 > 1000) {
			MAIN_D_80135294 -= 400;
		}

		VS_tickFrame();
	}

	VS_removeBattleStartTextBurst();

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

	MAIN_D_80135294 = 1000;
	MAIN_D_8013528C = 1;
	MAIN_D_80135268 = 1;
	GAME_STATE = 4;
}

int32_t VS__checkEndCondition(void)
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

	if (MAIN_D_80135288 == 0) {
		FighterData *fighter = *(FighterData **)&COMBAT_DATA_PTR;

		if ((*(DigimonEntity **)&ENTITY_TABLE[1])->stats.current.currentHP -
		            fighter->hpDamageBuffer <=
		    0) {
			return 0;
		}

		if (VS__areAllEnemyDigimonDead() != 0) {
			return 0;
		}

		MAIN_D_80135280[0] = MAIN_D_8013527C[0] - ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]])->stats.current.currentHP;
		MAIN_D_80135280[1] = MAIN_D_8013527C[1] - ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]])->stats.current.currentHP;
		COMBAT_DATA_PTR->fighter[0].hpDamageBuffer = 0;
		COMBAT_DATA_PTR->fighter[1].hpDamageBuffer = 0;

		if (MAIN_D_80135280[0] > MAIN_D_80135280[1]) {
			VS__tickBattleResultScreen(1, 0);
			return -1;
		}

		if (MAIN_D_80135280[0] < MAIN_D_80135280[1]) {
			VS__tickBattleResultScreen(0, 1);
			return 1;
		}

		if (MAIN_D_80135280[0] == MAIN_D_80135280[1]) {
			VS__tickBattleResultScreen(1, 1);
			return 2;
		}
	}

	return 0;
}

void VS__tickDigimonAi(uint8_t fighterId)
{
	DigimonEntity *entity;
	DigimonEntity *other;
	FighterData *data;
	FighterData *otherData;
	Stats *stats;
	uint16_t *flags;
	int16_t otherId;
	unsigned long id;
	int32_t move;
	uint16_t array[4];

	id = fighterId;
	entity = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[fighterId]];
	data = &COMBAT_DATA_PTR->fighter[id];
	stats = &entity->stats;
	flags = &data->flags;

	if (MAIN_D_80134D66 % 20 == 0 && data->buffPrioTimer != 0) {
		data->buffPrioTimer--;
	}

	if (COMBAT_DATA_PTR->player.commandDelay[id] == 0) {
		COMBAT_DATA_PTR->player.currentCommand[id] =
			COMBAT_DATA_PTR->player.bufferedCommand[id];
	} else if ((*flags & 0x800e) == 0 && data->flatTimer == 0) {
		COMBAT_DATA_PTR->player.commandDelay[id]--;
	}

	if (MAIN_D_80134D74 == 0) {
		if ((stats->current.currentHP > data->hpDamageBuffer) &&
		    (stats->base.brain <= 300) &&
		    (MAIN_D_80134D66 % ((stats->base.brain / 2 + 1) * 20) == 0) &&
		    (70 - MAIN_D_80135278[id] > randomLimit(100))) {
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

		MAIN_D_80135290 = 1;
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

	if (MAIN_D_80134D74 != 0) {
		return;
	}

	if ((*flags & 0x800e) == 0 && data->flatTimer == 0) {
		switch (COMBAT_DATA_PTR->player.currentCommand[id]) {
		case 8:
		case 9:
		case 10:
			if (VS_isMoveUsable((DigimonEntity *)&entity->entity, data,
			                    COMBAT_DATA_PTR->player.currentCommand[id] - 8) != 0) {
				data->targetId = otherId;
				if ((entity->stats.base.moves[COMBAT_DATA_PTR->player.currentCommand[id] - 8] != data->queuedAnim) ||
				    (data->moveRange <= 0)) {
					VS_setupQueuedMove(entity, data, (uint8_t)fighterId,
					                   (uint8_t)(COMBAT_DATA_PTR->player.currentCommand[id] - 8));
				}
				VS_applyChargeRequirement(entity, data,
				                          DIGIMON_DATA[entity->entity.type].moves[data->queuedAnim - 0x2e]);
				return;
			}
			break;
		case 11:
			data->targetId = otherId;
			if ((entity->stats.base.moves[3] != data->queuedAnim) ||
			    (data->moveRange <= 0)) {
				VS_setupQueuedMove(entity, data, (uint8_t)fighterId, (uint8_t)3);
			}
			VS_applyChargeRequirement(entity, data,
			                          DIGIMON_DATA[entity->entity.type].moves[data->queuedAnim - 0x2e]);
			return;
		}

		if (COMBAT_DATA_PTR->player.currentCommand[id] != 2 && COMBAT_DATA_PTR->player.currentCommand[id] != 4) {
			entity->stats.current.chargeMode = (&MAIN_D_80134F3C)[id];
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
		VS_queueRandomMove(entity, data, id);
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

	switch (COMBAT_DATA_PTR->player.currentCommand[id]) {
	case 2:
		if (VS__hasAffordableMoves2(array, fighterId) == 0) {
			data->cooldown = 0x50;
			data->flags |= 0x800;
			return;
		}
		move = VS_selectMoveByPower((uint8_t)fighterId, array);
		entity->stats.current.chargeMode = 0;
		break;
	case 4:
		if (VS__hasAffordableMoves2(array, fighterId) == 0) {
			data->cooldown = 0x50;
			data->flags |= 0x800;
			return;
		}
		move = VS_selectMoveByMpCost((uint8_t)fighterId, array);
		entity->stats.current.chargeMode = 2;
		break;
	}

	if (move == -1) {
		VS_selectPartnerMove(entity, data, fighterId);
	} else {
		VS_setupQueuedMove(entity, data, (uint8_t)fighterId, (uint8_t)move);
	}
}

void VS__tickBattle(void)
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
		VS_addFinisherProgress(fighter, 1);
		id = fighter->targetId;

		if (id != 0xff) {
			combat = (uint32_t)COMBAT_DATA_PTR;
			target = (DigimonEntity *)ENTITY_TABLE[((uint8_t *)((uint32_t)id + combat))[0x66c]];
		} else {
			target = NULL;
		}
		if (*flags & 0x20) {
			VS__tickDigimonAttacking(&entity->entity, target, i);
		} else if ((*flags & 0x10) || (*flags & 0x80)) {
			VS__tickDigimonHitByAttack(&entity->entity, fighter, i);
		} else if (fighter->moveRange != -1) {
			if (*flags & 8) {
				VS__tickDigimonFlat(entity, target, fighter, i);
			} else if (*flags & 4) {
				VS__tickDigimonStun(&entity->entity);
			} else if (*flags & 2) {
				VS__tickDigimonConfusion(entity, target, fighter, i);
			} else if (*flags & 0x2000) {
				VS__tickDigimonSenile(entity, fighter);
			} else if (*flags & 0x800) {
				VS__tickDigimonOnChargeup(entity, target, fighter);
			} else if (*flags & 0x1000) {
				VS__tickDigimonOnCooldown(entity, target, fighter);
			} else {
				VS__tickDigimonOther(entity, target, fighter, i);
			}
		}
	}

	VS_resolveAttack();
	VS_applyMoveResult();
	if (MAIN_D_80135268 != 7 && MAIN_D_80135268 != 8) {
		if (MAIN_D_8013529C != 0) {
			MAIN_D_8013529C--;
			if (MAIN_D_8013529C == 0) {
				MAIN_D_80135268 = 1;
			}
		}
		if ((MAIN_D_80134D66 % 600) == 0 && randomLimit(2) == 1) {
			MAIN_D_80135268 = 6;
			VS_setRandomViewpoint(ENTITY_TABLE[1], 4);
			MAIN_D_8013529C = randomLimit(0x29) + 0x3c;
		}
	}

	for (i = 0; ENEMY_COUNT >= i; i++) {
		if (COMBAT_DATA_PTR->fighter[i].flags & 0x20) {
			break;
		}
		if (COMBAT_DATA_PTR->fighter[i].flags & 0x10) {
			break;
		}
		if (MAIN_D_80135268 == 7) {
			break;
		}
		if (MAIN_D_80135268 == 8) {
			break;
		}
	}

	if (i == ENEMY_COUNT + 1) {
		MAIN_D_80135268 = 1;
	}
}

void VS__handlePause(void)
{
	while (MAIN_D_80134E78[1] != 0 && MAIN_D_80134E78[0] >= 2) {
		if (MAIN_D_80134F30 == VS__isButtonsPressed(0x800)) {
			MAIN_D_80134F30 = 0;
			MAIN_D_80134E78[1] = (MAIN_D_80134E78[1] + 1) & 1;
		}
	}

	if (MAIN_D_80134F30 == 0) {
		MAIN_D_80134F30 = VS__isButtonsPressed(0x800);
		if (MAIN_D_80134F30 != 0) {
			MAIN_D_80134E78[1] = (MAIN_D_80134E78[1] + 1) & 1;
		}
	}

	if (MAIN_D_80134E78[1] != 0) {
		createPauseBox();
		++MAIN_D_80134E78[0];
	} else {
		removePauseBox();
		MAIN_D_80134E78[0] = 0;
	}
}

int32_t VS__deinitializeCombat(int16_t lostP1, int16_t lostP2)
{
	int32_t i;
	int16_t frames;
	Stats *stats;

	((DigimonEntity *)ENTITY_TABLE[1])->stats.current.chargeMode = MAIN_D_80134F3C;
	((DigimonEntity *)ENTITY_TABLE[2])->stats.current.chargeMode = MAIN_D_80134F3D;
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
		if (MAIN_D_80135288 != 0 || MAIN_D_80134F40 == 0) {
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
		i = 0;

		stopBGM();
		stopSound();
		playMusic(MAIN_D_80135274, 3);

		for (; i < frames; ++i) {
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

int32_t VS__isButtonsPressed(int32_t buttons)
{
	uint32_t pad;
	uint32_t prev;

	MAIN_D_80134F38 = MAIN_D_80134F34;
	MAIN_D_80134F34 = PadRead(1);

	if ((buttons & ((pad = MAIN_D_80134F34) & ~(prev = MAIN_D_80134F38))) != 0) {
		return 1;
	}

	MAIN_D_80134F34 = (uint16_t)(pad >> 16);
	MAIN_D_80134F38 = (uint16_t)(prev >> 16);
	if ((buttons & (MAIN_D_80134F34 & ~MAIN_D_80134F38)) != 0) {
		MAIN_D_80134F34 = pad;
		MAIN_D_80134F38 = prev;

		return 2;
	}

	MAIN_D_80134F34 = pad;
	MAIN_D_80134F38 = prev;
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
	fighter->flags &= 0xfff7;
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

void VS__tickBattleResultScreen(uint8_t hasLostP1, uint8_t hasLostP2)
{
	MAIN_D_80134F44 = 0;
	addObject(0x1a2, 0, NULL, (RenderFunction)VS__renderTimeoutText);
	stopBGM();

	if (hasLostP1 == hasLostP2) {
		DigimonEntity *entityP1;
		DigimonEntity *entityP2;

		MAIN_D_80134F40 = 1;

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

		while (MAIN_D_80134F44 < 61) {
			if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x40) != 0) {
				break;
			}
			VS_tickFrame();
			++MAIN_D_80134F44;
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
		uint32_t timer;

		winner = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[hasLostP1]];
		loser = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[hasLostP2]];

		entityLookAtLocation(&winner->entity, &loser->entity.posData->location);
		handleBattleIdle(winner, &winner->stats,
		                 COMBAT_DATA_PTR->fighter[hasLostP1].flags);
		VS__faintDigimon(loser, &COMBAT_DATA_PTR->fighter[hasLostP2],
		                 hasLostP2);

		while ((timer = MAIN_D_80134F44) < 121) {
			if (timer >= 61 &&
			    (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x40) != 0) {
				break;
			}
			if (timer == 60) {
				VS__addTimeoutWindow();
			}
			VS_tickFrame();
			++MAIN_D_80134F44;
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
	char *name1;
	char *name2;

	name1 = MAIN_D_8013526C + VS_D_800716A8[MAIN_D_80135264] * 64;
	name2 = MAIN_D_80135270 + (&VS_D_800716A8[5])[MAIN_D_80135264] * 64;

	clearTextArea();
	drawString(MAIN_D_80134518, 6, 0);
	drawString(STR_DAMEEJI, 0, 12);
	drawString(name1 + 14, (120 - strlen(name1 + 14) * 6) / 2, 24);
	drawString(name2 + 14, (120 - strlen(name2 + 14) * 6) / 2, 36);
	DrawSync(0);
	removeObject(0x1a2, 0);

	setRECT(&startPos, -10, -10, 20, 20);

	setRECT(&finalPos, -132, -27, 264, 54);

	createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, NULL,
	                    VS__renderTimeoutWindow);
}

void VS__faintDigimon(DigimonEntity *entity, FighterData *fighter,
                      uint8_t fighterId)
{
	entity->stats.current.isHit = 1;
	fighter->flags |= 0x8000;

	startAnimation(&entity->entity, 0x2b);
	VS__resetFlatten(fighterId);
	VS_removeStatusEffects(entity, fighter);

	fighter->flags &= 0xff40;
	fighter->flags |= 0x40;
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
	int32_t i;

	if ((entity->anim.animFlag & 1) == 0) {
		if (entity->anim.frameCount != entity->anim.animFrame) {
			for (i = 0; i <= ENEMY_COUNT; ++i) {
				if (i != fighterId) {
					Entity *other = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
					uint32_t moveIdx = other->anim.animId - 0x2e;

					if (DIGIMON_DATA[other->type].moves[moveIdx] == 0x2d) {
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
	if (MAIN_D_80134D74 != 0) {
		handleBattleIdle(entity, &entity->stats, data->flags);
		return;
	}

	if (VS__tickDigimonHoldDistance(entity, other, data) != 0) {
		return;
	}

	VS__tickDigimonAttackRanged(entity, other, data, 0x79);

	if ((data->flags & 0x20) != 0) {
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
	int32_t range;

	if (MAIN_D_80134D74 != 0) {
		handleBattleIdle(entity, &entity->stats, data->flags);
		return;
	}

	if ((data->flags & 0x1000) != 0 &&
	    ((data->flags & 0x1000) != 0 || (data->flags & 0x800) != 0)) {
		VS__confusedRotate(&entity->entity);
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		collisionGrace(NULL, &entity->entity, 280, 200);
		if (data->cooldown < 2) {
			data->flags &= 0xefff;
			data->cooldown = 0;
		}
	} else if (other == NULL) {
		VS__confusedRotate(&entity->entity);
		if (VS__tickDigimonAttackClose(entity, NULL, data, fighterId) != 0) {
			collisionGrace(NULL, &entity->entity, 280, 200);
		}

		if ((data->flags & 0x20) != 0) {
			return;
		}

		if (randomLimit(100) < 5) {
			handleBattleIdle(entity, &entity->stats, data->flags);
			VS_startFighterMove(entity, other, data);
		}
	} else {
		range = data->moveRange;
		switch (range) {
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
		data->flags &= 0xdfbf;
	} else {
		handleBattleIdle(entity, &entity->stats, data->flags);
	}
}

void VS__tickDigimonOnChargeup(DigimonEntity *entity, DigimonEntity *other,
                               FighterData *data)
{
	int32_t result;
	int16_t tech;

	if (MAIN_D_80134D74 != 0) {
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
				data->unk16 = 0;
				break;
			case 2:
				VS__setWalking(&entity->entity, &entity->stats,
				               data->flags);
				VS__tickDigimonRotateKeepDistance(entity, other, data);
				break;
			}
		}

		if (data->cooldown < 2) {
			data->flags &= 0xf7bf;
			data->cooldown = 0;
		}
	} else if (result == 0) {
		switch (entity->stats.current.chargeMode) {
		case 0:
			handleBattleIdle(entity, &entity->stats, data->flags);
			entityLookAtLocation(&entity->entity,
			                     &other->entity.posData->location);
			data->unk16 = 0;
			if (data->speedBuffer > 0) {
				data->flags &= 0xf7ff;
			}
			break;
		case 1:
			VS__tickDigimonWaitingDistance(entity, other, data);
			tech = (int16_t)entityGetTechFromAnim(&entity->entity,
			                                      data->queuedAnim);
			if (data->speedBuffer == 100 ||
			    data->speedBuffer >= MOVE_DATA[tech].power) {
				data->flags &= 0xf7ff;
			}
			break;
		case 2:
			VS__setWalking(&entity->entity, &entity->stats,
			               data->flags);
			VS__tickDigimonRotateKeepDistance(entity, other, data);
			if (data->speedBuffer == 100) {
				data->flags &= 0xf7ff;
			}
			break;
		}
	}
}

void VS__tickDigimonOnCooldown(DigimonEntity *entity, DigimonEntity *other,
                               FighterData *data)
{
	if (MAIN_D_80134D74 != 0) {
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
		data->flags &= 0xefbf;
		data->cooldown = 0;
	}
}

void VS__tickDigimonOther(DigimonEntity *entity, DigimonEntity *other,
                          FighterData *data, int32_t fighterId)
{
	int32_t range;

	if (MAIN_D_80134D74 != 0 && MAIN_D_80134D60 != &entity->entity) {
		handleBattleIdle(entity, &entity->stats, data->flags);
		return;
	}

	if (VS__tickDigimonHoldDistance(entity, other, data) != 0) {
		return;
	}

	range = data->moveRange;
	switch (range) {
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
	uint8_t command;
	uint32_t threshold;
	uint32_t distance;

	if (MAIN_D_80134D74 != 0) {
		return 0;
	}

	if (ENTITY_TABLE[1] == &entity->entity) {
		id = 0;
	} else {
		id = 1;
	}

	command = COMBAT_DATA_PTR->player.currentCommand[id];
	switch (command) {
	case 6:
		handleBattleIdle(entity, &entity->stats, data->flags);
		entityLookAtLocation(&entity->entity,
		                     &ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[data->targetId]]->posData->location);
		data->unk16 = 0;

		return 1;
	case 5:
		threshold = VS__entityGetMoveWithHighestDistance(other) + 640000;
		if (entity->entity.anim.animId >= 0x23 &&
		    entity->entity.anim.animId < 0x25) {
			threshold += 160000;
		}

		distance = VS_getDistanceSquared(&entity->entity, &other->entity);
		if (distance < threshold) {
			VS__setWalking(&entity->entity, &entity->stats, data->flags);
			VS__tickDigimonRotateKeepDistance(entity, other, data);
		} else {
			data->unk16 = 0;
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
	int16_t savedRotation;
	uint32_t distance;
	int32_t radius;
	int32_t tech;

	rotation = &entity->entity.posData->rotation.vy;
	entity->entity.anim.animFlag &= 0xfd;
	savedRotation = *rotation;

	if (other != NULL) {
		distance = VS_getDistanceSquared(&entity->entity, &other->entity);
		radius = DIGIMON_DATA[entity->entity.type].radius +
		         DIGIMON_DATA[other->entity.type].radius;
		if (radius * radius >= distance) {
			handleBattleIdle(entity, &entity->stats, data->flags);

			if (MAIN_D_80134D74 != 0) {
				if (MAIN_D_80134D60 != &entity->entity) {
					return 0;
				}

				if (MAIN_D_80134F48 > 0) {
					MAIN_D_80134F48--;
					entityLookAtLocation(&entity->entity,
					                     &other->entity.posData->location);

					return 0;
				}
			} else {
				tech = data->queuedAnim;
				tech = (int16_t)entityGetTechFromAnim(&entity->entity,
				                                      tech);
				if (tech >= 0x3a && tech < 0x71) {
					MAIN_D_80134D74 = 1;
				}

				if (MAIN_D_80134D74 != 0) {
					MAIN_D_80134D60 = &entity->entity;
					VS_addTargetCursor(fighterId);
					startAnimation(&entity->entity,
					               data->queuedAnim);
					entity->entity.anim.animFlag &= 0xfe;
					MAIN_D_80134F4C = VS_addFinisherAura((int32_t)&entity->entity, 80);
					MAIN_D_80134F48 = 80;

					return 0;
				}
			}

			if (VS_selectMoveTarget((Entity *)entity, data) != 0) {
				return 0;
			}

			startAnimation(&entity->entity, data->queuedAnim);
			data->flags |= 0x20;
			VS_playMoveEffect(entity, other, data);

			return 0;
		}

		if (MAIN_D_80134D74 == 0) {
			VS__setWalking(&entity->entity, &entity->stats,
			               data->flags);
			entityLookAtLocation(&entity->entity,
			                     &other->entity.posData->location);
		} else {
			if (MAIN_D_80134D60 != &entity->entity) {
				return 0;
			}

			if (MAIN_D_80134F48 > 0) {
				MAIN_D_80134F48--;
				entityLookAtLocation(&entity->entity,
				                     &other->entity.posData->location);

				return 0;
			}

			startAnimation(&entity->entity, data->queuedAnim);
			data->flags |= 0x20;
			VS_playMoveEffect(entity, other, data);

			return 0;
		}
	} else {
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
	}

	if (entityCheckCollision(NULL, &entity->entity, 280, 200) == -1) {
		return 0;
	}

	*rotation = savedRotation;

	return 1;
}

void VS__tickDigimonAttackRanged(DigimonEntity *entity, DigimonEntity *other,
                                 FighterData *data, int16_t move)
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
		VS_startFighterMove(entity, other, data);
		return;
	}

	distance = VS_getDistanceSquared(&entity->entity, &other->entity);
	range = (DIGIMON_DATA[entity->entity.type].radius +
	         DIGIMON_DATA[other->entity.type].radius);
	maxDistance = *(uint32_t *)&MOVE_DATA[move].distance + range * range;

	if (maxDistance + (minDistance = maxDistance * 3 / 10) < distance) {
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__tickDigimonAttackLookAtTarget(entity, &other->entity.posData->location,
		                                  280, 200);
		++data->unk15;
	} else if (distance < maxDistance - minDistance) {
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__tickDigimonRotateKeepDistance(entity, other, data);
		++data->unk15;
	} else {
		data->unk15 = 0;

		handleBattleIdle(entity, &entity->stats, data->flags);

		if ((data->flags & 8) != 0) {
			if (MAIN_D_80134D66 % 40 == 0) {
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
	uint8_t move;
	uint8_t tech;

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
	int16_t *rotationY;
	int16_t initRotation;
	int16_t reversedY;
	int16_t result;
	int16_t val1;
	int16_t val2;

	rotationY = &entity->entity.posData->rotation.vy;
	initRotation = *rotationY;
	entityLookAtLocation(&entity->entity,
	                     &other->entity.posData->location);
	reversedY = (*rotationY + 0x800) & 0xfff;

	if (data->unk16 == 0) {
		*rotationY = reversedY;

		result = entityCheckCollision(NULL, &entity->entity, 280, 200);
		if (result != -1) {
			data->unk16 = 1;
			VS__tickDigimonRotationKeepDistanceCollision(&entity->entity, rotationY, result,
			                                             initRotation);
		}

		return;
	}

	*rotationY = initRotation;
	if (initRotation == other->entity.posData->rotation.vy) {
		*rotationY = (*rotationY + 0x400) & 0xfff;

		if (entityCheckCollision(NULL, &entity->entity, 280, 200) == -1) {
			return;
		}

		*rotationY = initRotation;
		*rotationY = (*rotationY + 0xc00) & 0xfff;

		if (entityCheckCollision(NULL, &entity->entity, 280, 200) == -1) {
			return;
		}

		*rotationY = initRotation;

		result = entityCheckCollision(NULL, &entity->entity, 280, 200);
		if (result != -1) {
			VS__tickDigimonRotationKeepDistanceCollision(&entity->entity, rotationY, result,
			                                             initRotation);
		}

		return;
	}

	val1 = initRotation - reversedY;
	if (val1 < 0) {
		val1 = (val1 + 0x1000) & 0xfff;
	}

	val2 = reversedY - initRotation;
	if (val2 < 0) {
		val2 = (val2 + 0x1000) & 0xfff;
	}

	if ((int16_t)(val1 - val2) < 0) {
		if (val1 > 20) {
			*rotationY = (*rotationY + 0xfec) & 0xfff;
		} else {
			*rotationY = (*rotationY + 0x1000 - val1) & 0xfff;
		}
	} else {
		if (val2 > 20) {
			*rotationY = (*rotationY + 20) & 0xfff;
		} else {
			*rotationY = (*rotationY + val2) & 0xfff;
		}
	}

	if (entityCheckCollision(NULL, &entity->entity, 280, 200) == -1) {
		return;
	}

	*rotationY = initRotation;

	result = entityCheckCollision(NULL, &entity->entity, 280, 200);
	if (result != -1) {
		VS__tickDigimonRotationKeepDistanceCollision(&entity->entity, rotationY, result,
		                                             initRotation);
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

	if ((fighter->flags & 0x28) == 0x28) {
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

	if (MAIN_D_80134D60 == &entity->entity) {
		MAIN_D_80134D74 = 0;
		MAIN_D_80134D60 = NULL;
	}

	fighter->unk16 = 0;

	if (fighter->flags & 0x20) {
		if (fighterId == 0) {
			++COMBAT_DATA_PTR->player.hitCount;
		}

		fighter->flags &= 0xfbff;
		fighter->flags |= 0x1000;
		fighter->cooldown = 40;

		VS_addFinisherProgress(fighter, fighter->finisherGoal * 2 / 50);
	}

	if (fighter->invulnerableTimer <= 0) {
		if (fighter->flags & 0x8) {
			entity->entity.flatSprite = 0;
		}

		if ((fighter->flags & 0x20) == 0) {
			entity->stats.current.isHit = 0;
		}

		if (fighter->flags & 0x80) {
			fighter->flags &= 0xff7f;
			VS__clearFighterDataTables(fighter);
		} else {
			fighter->flags &= 0xff0f;
		}
	}

	if ((fighter->flags & 0x10) == 0) {
		if (fighter->flatTimer == -1) {
			fighter->flatTimer = 0x41;
		}
	} else {
		fighter->senileTimer = 0;
		fighter->flags &= 0xdfff;
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
                                     FighterData *data, int32_t min, int32_t max)
{
	uint32_t actualDistance;
	uint32_t baseDistance;

	actualDistance = VS_getDistanceSquared(&entity->entity, &other->entity);
	baseDistance = VS__getBaseDistance(&entity->entity, &other->entity);

	if (actualDistance < baseDistance + min) {
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__tickDigimonRotateKeepDistance(entity, other, data);
	} else if (baseDistance + max < actualDistance) {
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__tickDigimonAttackLookAtTarget(entity, &other->entity.posData->location,
		                                  280, 200);
	} else {
		data->unk16 = 0;
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
		if (MAIN_D_80134D66 % 2 == 0) {
			fighter->speedBuffer += (stats->base.speed / 100) + 1;
		}

		if (fighter->speedBuffer > 100) {
			fighter->speedBuffer = 100;
		}
	}
}

int32_t VS__hasAffordableMoves2(uint16_t *array, uint8_t fighterId)
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
	int32_t slot;

	for (i = 0; i < 150; ++i) {
		slot = i;

		if (fighter->table1[slot] == -1) {
			break;
		}

		fighter->table1[slot] = -1;
		fighter->table2[slot] = -1;
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
		angles[0] = MAIN_D_80134510[index];
		angles[1] = (MAIN_D_80134510[index] + 0x400) & 0xfff;
		for (i = 0; i < 2; ++i) {
			*rotationY = angles[i];
			if (entityCheckCollision(NULL, entity, 0x118, 0xc8) == -1) {
				break;
			}
		}

		switch (i) {
		case 0:
			for (i = 0; i < 3; ++i) {
				*rotationY = angles[i] = (MAIN_D_80134510[index] + 0xc00 + i * 0x200) & 0xfff;
				if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
					break;
				}
			}
			break;
		case 1:
			for (i = 0; i < 3; ++i) {
				*rotationY = angles[i] = (MAIN_D_80134510[index] + 0x400 + i * 0x200) & 0xfff;
				if (entityCheckCollision(NULL, entity, 0x118, 0xc8) != -1) {
					break;
				}
			}
			break;
		default:
			for (i = 0; i < 3; ++i) {
				*rotationY = angles[i] = (MAIN_D_80134510[index] + 0x800 + i * 0x200) & 0xfff;
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

int32_t VS__combatMain(void)
{
	int16_t result;

	MAIN_D_80134F30 = 0;
	COMBAT_AREA_CENTER_X = 0;
	COMBAT_AREA_CENTER_Y = 0;

	stopBGM();
	playMusic(MAIN_D_80135274, 2);
	VS__combatInit();
	VS__combatSetup();

	while (1) {
		result = VS__checkEndCondition();
		if (result != 0) {
			break;
		}

		VS__tickDigimonAi(0);
		VS__tickDigimonAi(1);
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
	int32_t layer;
	int32_t digits;

	x = UI_BOX_DATA[id].finalPos.x;
	y = UI_BOX_DATA[id].finalPos.y;
	renderString(0, x + 6, y + 6, 120, 12, 0, 24, (layer = 6 - id), 1);
	renderString(0, x + 138, y + 6, 120, 12, 0, 36, layer, 1);
	renderString(0, x + 108, y + 24, 48, 24, 0, 0, layer, 1);

	digits = VS__getDigitCount(MAIN_D_80135280[1]);
	VS__renderIntroStatNumber(x + 42 + (48 - digits * 12) / 2, y + 30, digits,
	                          MAIN_D_80135280[1], layer);
	digits = VS__getDigitCount(MAIN_D_80135280[0]);
	VS__renderIntroStatNumber(x + 174 + (48 - digits * 12) / 2, y + 30, digits,
	                          MAIN_D_80135280[0], layer);
}

void VS__renderPlayerMarker(int32_t id)
{
	Entity *entity;
	POLY_FT4 *prim;
	MATRIX *m;
	SVECTOR pos;
	uint32_t otz;
	int16_t sxy[2];
	int32_t yOffset = 0;

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
	sxy[0] += (int16_t)(VIEWPORT_DISTANCE *
	                    (DIGIMON_DATA[entity->type].radius / 2) /
	                    (otz * 4));

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->tpage = getTPage(0, 0, 384, 0);
	setClut(prim, 0, 498);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setUVWH(prim, id * 48 + 0x88, 0xd8, 48, 32);
	setXY4(prim, sxy[0], sxy[1] - 32, sxy[0] + 48, sxy[1] - 32, sxy[0], sxy[1] + yOffset, sxy[0] + 48, sxy[1] + yOffset);
	AddPrim(ACTIVE_ORDERING_TABLE->org + otz, prim++);

	GsSetWorkBase((PACKET *)prim);
}

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
		if (MAIN_D_80135278[p] < 70) {
			COMBAT_DATA_PTR->player.commandDelay[p] =
				160 - MAIN_D_80135278[p] / 10;
		} else {
			COMBAT_DATA_PTR->player.commandDelay[p] =
				(10 - MAIN_D_80135278[p] / 10) * 10;
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
