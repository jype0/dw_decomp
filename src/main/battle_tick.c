#include <libgte.h>

#include <dw/anim.h>
#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/fade.h>
#include <dw/input.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/ui.h>
#include <dw/version.h>

typedef struct {
	int16_t spawnX[10];
	int16_t spawnY[10];
	int16_t spawnZ[10];
	int16_t rotation[10];
	int16_t targetMap[10];
	int16_t targetExit[10];
} MapWarps;

extern MapWarps MAP_WARPS;
extern uint8_t CURRENT_EXIT;
extern int8_t TAMER_START_TILE_X;
extern int8_t TAMER_START_TILE_Y;
extern int8_t TAMER_WAYPOINT_COUNT;
extern int8_t TAMER_WAYPOINT_X[];
extern int8_t TAMER_WAYPOINT_Y[];
extern uint8_t BATTLE_TOGGLE_LIFEBAR;
extern int32_t IS_TAMERLESS_BATTLE;

void BTL_getRemainingEnemies(Entity *self, int16_t *out, int16_t *count);
void entityLookAtTile(Entity *entity, int32_t tileX, int32_t tileY);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void addInventoryUI(void);

void handleCommands(void);
void handleFleeing(void);
void tamerTickBattle(int32_t instanceId);
void partnerTickBattle(int32_t instanceId);
void NPCEntityTickBattle(int32_t instanceId);

int32_t PLAYER_COMBAT_IDLE_TIMER;

static void *battle_tick_functions[] = {
	NPCEntityTickBattle,
	partnerTickBattle,
	tamerTickBattle,
	handleFleeing,
	handleCommands,
};

void handleCommands(void)
{
	SVECTOR rot;
	VECTOR target;
	VECTOR in;
	VECTOR out;
	MATRIX m;
	int16_t enemies[4];
	int16_t tileX;
	int16_t tileY;
	int16_t count;

	if (IS_TAMERLESS_BATTLE == 1) {
		return;
	}

	if (UI_BOX_DATA[0].state != 0) {
		return;
	}

	if (COMBAT_DATA_PTR->player.currentCommand[0] == 1) {
		if ((FLEE_TIMER == 0x14) &&
		    ((PARTNER_ENTITY.digimonEntity.stats.current.currentHP -
		      COMBAT_DATA_PTR->fighter[0].hpDamageBuffer) > 0)) {
			fadeToBlack(0x14);
		}

		handleFleeing();
		FLEE_TIMER++;

		return;
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x8000) {
		playSound(0, 2);
		COMBAT_DATA_PTR->player.hoveredCommand[0]++;

		if ((FLEE_DISABLED[0] != 0) &&
		    (COMBAT_DATA_PTR->player.hoveredCommand[0] == 1)) {
			COMBAT_DATA_PTR->player.hoveredCommand[0]++;
		}

		if (COMBAT_DATA_PTR->player.hoveredCommand[0] >
		    (COMBAT_DATA_PTR->player.numCommands[0] - 1)) {
			if (FLEE_DISABLED[0] != 0) {
				COMBAT_DATA_PTR->player.hoveredCommand[0] = 2;
			} else {
				COMBAT_DATA_PTR->player.hoveredCommand[0] = 1;
			}
		}
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x2000) {
		playSound(0, 2);
		COMBAT_DATA_PTR->player.hoveredCommand[0]--;

		if ((FLEE_DISABLED[0] != 0) &&
		    (COMBAT_DATA_PTR->player.hoveredCommand[0] == 1)) {
			COMBAT_DATA_PTR->player.hoveredCommand[0]--;
		}

		if (COMBAT_DATA_PTR->player.hoveredCommand[0] <= 0) {
			COMBAT_DATA_PTR->player.hoveredCommand[0] =
				COMBAT_DATA_PTR->player.numCommands[0] - 1;
		}
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON) {
		playSound(0, 3);
		BTL_getRemainingEnemies(ENTITY_TABLE[1], enemies, &count);

		if (count == 0) {
			return;
		}

		if (ENTITY_TABLE[0]->anim.animId != 0xe) {
			startAnimation(ENTITY_TABLE[0], 0xe);
		}

		COMBAT_DATA_PTR->player.bufferedCommand[0] =
			COMBAT_DATA_PTR->player.availableCommands[0][COMBAT_DATA_PTR->player.hoveredCommand[0]];

		if (PARTNER_PARA.discipline < 0x46) {
			COMBAT_DATA_PTR->player.commandDelay[0] = 0xa0 - (PARTNER_PARA.discipline / 10);
		} else {
			COMBAT_DATA_PTR->player.commandDelay[0] = (0xa - (PARTNER_PARA.discipline / 10)) * 10;
		}

		COMBAT_DATA_PTR->player.commandDelay[0] = 0;
		switch (COMBAT_DATA_PTR->player.bufferedCommand[0]) {
		case 1:
			COMBAT_DATA_PTR->player.commandDelay[0] = 0;
			COMBAT_DATA_PTR->player.currentCommand[0] = 1;
			getModelTile(&ENTITY_TABLE[0]->posData->location, &tileX, &tileY);

			if ((tileX == TAMER_START_TILE_X) && (tileY == TAMER_START_TILE_Y)) {
				target.vx = MAP_WARPS.spawnX[CURRENT_EXIT];
				target.vy = 0;
				target.vz = MAP_WARPS.spawnZ[CURRENT_EXIT];
				in.vx = 0;
				in.vy = 0;
				in.vz = -0xbb8;
				rot.vx = 0;
				rot.vy = (MAP_WARPS.rotation[CURRENT_EXIT] + 0x800) & 0xfff;
				rot.vz = 0;
				RotMatrix(&rot, &m);
				ApplyMatrixLV(&m, &in, &out);
				target.vx = target.vx + out.vx;
				target.vz = target.vz + out.vz;
				entityLookAtLocation(ENTITY_TABLE[0], &target);
			}

			startAnimation(ENTITY_TABLE[0], 3);
			break;

		case 7:
			COMBAT_DATA_PTR->player.changeTarget = 1;
			break;
		}

		BTL_drawCommandShout(COMBAT_DATA_PTR->player.bufferedCommand[0]);
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x80) {
		if (COMBAT_DATA_PTR->fighter[0].finisherProgress ==
		    COMBAT_DATA_PTR->fighter[0].finisherGoal) {
			COMBAT_DATA_PTR->player.bufferedCommand[0] = 0xb;
			playSound(0, 3);
			COMBAT_DATA_PTR->player.commandDelay[0] = 0;
			COMBAT_DATA_PTR->player.currentCommand[0] = 0xb;
			BTL_drawCommandShout(COMBAT_DATA_PTR->player.bufferedCommand[0]);
		}
	}
}

void handleFleeing(void)
{
	SVECTOR rot;
	MATRIX m;
	VECTOR target;
	VECTOR in;
	VECTOR out;
	int16_t tileX;
	int16_t tileY;
	int16_t i;

	i = TAMER_WAYPOINT_COUNT - 1;
	getModelTile(&ENTITY_TABLE[0]->posData->location, &tileX, &tileY);

	if (i >= 0) {
		entityLookAtTile(ENTITY_TABLE[0], TAMER_WAYPOINT_X[i], TAMER_WAYPOINT_Y[i]);

		if ((tileX == TAMER_WAYPOINT_X[i]) && (tileY == TAMER_WAYPOINT_Y[i])) {
			TAMER_WAYPOINT_COUNT--;
		}

		return;
	}

	entityLookAtTile(ENTITY_TABLE[0], TAMER_START_TILE_X, TAMER_START_TILE_Y);

	if ((tileX == TAMER_START_TILE_X) && (tileY == TAMER_START_TILE_Y)) {
		target.vx = MAP_WARPS.spawnX[CURRENT_EXIT];
		target.vy = 0;
		target.vz = MAP_WARPS.spawnZ[CURRENT_EXIT];
		in.vx = 0;
		in.vy = 0;
		in.vz = -0xbb8;
		rot.vx = 0;
		rot.vy = (MAP_WARPS.rotation[CURRENT_EXIT] + 0x800) & 0xfff;
		rot.vz = 0;
		RotMatrix(&rot, &m);
		ApplyMatrixLV(&m, &in, &out);
		target.vx = target.vx + out.vx;
		target.vz = target.vz + out.vz;
		entityLookAtLocation(ENTITY_TABLE[0], &target);
		ENTITY_TABLE[0]->anim.animFlag |= 2;
	}
}

// clang-format off
void tamerTickBattle(instanceId)
	int16_t instanceId;
// clang-format on
{
	Entity *tamer;
	Entity *partner;
	VECTOR target;

	tamer = ENTITY_TABLE[instanceId];

	if (GAME_STATE == 1) {
		if (COMBAT_DATA_PTR->player.currentCommand[0] != 1) {
			partner = ENTITY_TABLE[1];

			if (IS_TAMERLESS_BATTLE == 0) {
#if VERSION_IS(US)
				if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CANCEL_BUTTON) != 0) {
#else
				if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & ALT_BUTTON) != 0) {
#endif
					addInventoryUI();
				}
			}

			target.vx = partner->posData->location.vx;
			target.vy = 0;
			target.vz = partner->posData->location.vz;
			entityLookAtLocation(tamer, &partner->posData->location);

			if (UI_BOX_DATA[0].state == 0) {
				if ((NO_AI_FLAG == 0) || (FINISHING_ENTITY != ENTITY_TABLE[1])) {
					if ((tamer->anim.animId == 6) || (tamer->anim.animId == 0xe)) {
						if ((tamer->anim.animFlag & 1) == 0) {
							startAnimation(tamer, 1);
						}
					} else if (tamer->anim.animId != 1) {
						startAnimation(tamer, 1);
					}
				} else if (tamer->anim.animId != 0xa) {
					startAnimation(tamer, 0xa);
				}
			}
		}

		if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x100) != 0) {
			BATTLE_TOGGLE_LIFEBAR = (BATTLE_TOGGLE_LIFEBAR + 1) & 1;
		}

		handleCommands();
	}

	if (ENTITY_TABLE[0]->anim.animId == 1) {
		PLAYER_COMBAT_IDLE_TIMER = PLAYER_COMBAT_IDLE_TIMER + 1;
	} else {
		PLAYER_COMBAT_IDLE_TIMER = 0;
	}

	if (PLAYER_COMBAT_IDLE_TIMER >= 0xab) {
		startAnimation(ENTITY_TABLE[0], 1);
		PLAYER_COMBAT_IDLE_TIMER = 0;
	}

	tickAnimation(tamer);
}

// clang-format off
void partnerTickBattle(instanceId)
	int16_t instanceId;
// clang-format on
{
	tickAnimation(ENTITY_TABLE[instanceId]);
}

void NPCEntityTickBattle(int32_t instanceId)
{
	tickAnimation(ENTITY_TABLE[instanceId]);
	if (ENTITY_TABLE[instanceId]->anim.animFlag & 4) {
		tickAnimation(ENTITY_TABLE[instanceId]);
	}
}
