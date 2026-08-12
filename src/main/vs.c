#include <string.h>

#include <libgpu.h>
#include <libgs.h>
#include <mwinline_n.h>

#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/world_object.h>

#include "common.h"

typedef struct {
	int16_t clut;
	uint8_t u;
	uint8_t v;
	uint8_t w;
	uint8_t h;
	uint8_t x;
	uint8_t y;
} VsUISprite;

void setEntityPosition(int32_t entityId, int32_t x, int32_t y, int32_t z);
void setEntityRotation(int32_t entityId, int16_t x, int16_t y, int16_t z);
void startAnimation(Entity *entity, uint8_t animId);
void clearTextArea(void);
void drawString(char *text, int32_t color, int32_t pos);
uint32_t PadRead(int32_t id);
void VS_startCameraChase(Entity *entity, int32_t offset, int32_t id);
void VS_tickFrame(void);
void fadeFromBlack(int32_t frames);
int32_t playMusic(int32_t font, int32_t track);
void stopBGM(void);
void stopSound(void);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
		  int32_t f, int32_t g, int32_t h, int32_t i);
uint32_t playSound(int32_t vabId, int32_t val);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
			  int32_t *digits);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uvX, int32_t uvY,
		      int32_t width, int32_t height);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY,
		       int32_t width, int32_t height);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
void tickFileReadQueue(int32_t instanceId);
void VS_initializeEFEEngine(void *buffer);
void VS_loadMoveEFE(int16_t *moves, int16_t *effectIds, int8_t *isBusy);
void VS_initializeBattleStartText(void);
void VS_removeBattleStartText(void);
void VS_initializeBattleStartTextBurst(void);
void VS_removeBattleStartTextBurst(void);
int32_t VS_isBattleStartTextFinished(void);
void createPauseBox(void);
void MAIN_func_800E642C(void);
void MAIN_func_80092BB0(POLY_GT4 *prim);
void VS_removeAllStunEffects(void);
void VS_removeAllFinisherAuras(void);
void VS_removeAllPoisonEffects(void);
void VS_removeAllAuraProjectiles(void);
void VS_removeMoveEffect(Entity *entity, FighterData *fighter);
void VS_unloadAllEFESlots(void);
void VS_removeEFEEngine(void);
void VS_removeStatusEffects(DigimonEntity *entity, FighterData *fighter);
void VS_resetFighterAction(FighterData *fighter);
void handleBattleIdle(DigimonEntity *entity, Stats *stats, int32_t flags);
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void VS_startFighterMove(DigimonEntity *entity, DigimonEntity *other,
		      FighterData *data);
void collisionGrace(Entity *target, Entity *entity, int32_t dx, int32_t dy);
uint32_t VS_getDistanceSquared(Entity *a, Entity *b);
int32_t entityCheckCollision(Entity *a, Entity *entity, int32_t c, int32_t d);
void VS_addAuraProjectile(Entity *entity);
void VS_addFinisherProgress(FighterData *fighter, int16_t value);
int32_t VS_isMoveUsable(Entity *entity, FighterData *fighter,
			 int16_t moveId);
void tickAnimation(Entity *entity);
int32_t loadTIMFile(char *path, void *buffer);
void fadeToBlack(int32_t frames);
void removeStaticUIBox(int32_t id);
void VS_initializeVS(void);
void loadStackedTIMFile(char *path);
void resetFlattenGlobal(void);
void initializeAttackObjects(void);
void removeEntityText(int32_t id);
void VS_addFighterCounter(int32_t seconds);
void VS_addFighterStatusBars(int32_t id);
void VS_addCommandMenu(int32_t id);
void VS_selectRandomCamera(DigimonEntity *entity, int32_t type, int32_t value);
void VS_setupQueuedMove(DigimonEntity *entity, FighterData *data,
		      uint8_t fighterId, uint8_t move);
void VS_applyChargeRequirement(DigimonEntity *entity, FighterData *data,
		      uint8_t move);
void VS_queueRandomMove(DigimonEntity *entity, FighterData *data,
		      int32_t fighterId);
int32_t VS_selectMoveByPower(uint8_t fighterId, uint16_t *array);
int32_t VS_selectMoveByMpCost(uint8_t fighterId, uint16_t *array);
void VS_selectPartnerMove(DigimonEntity *entity, FighterData *data,
		      uint8_t fighterId);
void VS_removeFighterCounter(void);
void VS_removeCommandMenu(int32_t id);
void VS_removeFighterStatusBars(int32_t id);
void VS_loadVersusSceneModel(void);
void VS_addVersusModelScene(void);
void VS_removeVersusModelScene(void);
int32_t VS_isVersusModelSceneFinished(void);
void VS_removeResultModelScene(void);
int32_t VS_selectMoveTarget(DigimonEntity *entity, FighterData *data);
void VS_playMoveEffect(DigimonEntity *entity, DigimonEntity *other,
		      FighterData *data);
void VS_addTargetCursor(int32_t fighterId);
int32_t VS_addFinisherAura(Entity *entity, int32_t frames);

void VS__placePlayer1(int32_t stage);
void VS__placePlayer2(int32_t stage);
void VS__drawStatLabelText(void);
void VS__addIntroText(Entity *entity, int32_t id);
void VS__func_800F1DB8(Entity *entity);
void VS__removeIntroText(int32_t id);
void VS__func_800F1E6C(int32_t id);
void VS__func_800F1E9C(Entity *entity, int32_t id);
void VS__renderIntroStatBar(int32_t stat, int32_t value);
void VS__renderIntroNameChar(int16_t x, int16_t y, int16_t size,
			     uint8_t character);
void VS__func_800F23D0(int32_t stage);
void VS__tickIntroStats(int32_t id);
void VS__func_800F277C(int32_t id);
void VS__renderNumber2(int32_t x, int32_t y, int32_t digits, int32_t value,
			int32_t layer);
void VS__tickIntroName(int32_t id);
void VS__renderIntroName(int32_t id);
void VS__combatInit(void);
void VS__func_800F34F0(void);
int32_t VS__checkEndCondition(void);
void VS__digimonAiTickVS(uint8_t fighterId);
void VS__tickFighterStates(void);
void VS__handlePause(void);
int32_t VS__deinitializeCombat(int16_t lostP1, int16_t lostP2);
int32_t VS__isButtonsPressed(int32_t buttons);
void VS__removeCombatObjects(void);
void VS__removePlayerMarket(void);
void VS__resetFlatten(int16_t combatId);
int32_t VS__checkAnyDigimonDead(void);
void VS__func_800F4CB4(uint8_t hasLostP1, uint8_t hasLostP2);
void VS__func_800F4F9C(void);
void VS__faintDigimon(DigimonEntity *entity, FighterData *fighter,
		     uint8_t fighterId);
int32_t VS__func_800F51B8(int32_t value);
void VS__tickAttackState(Entity *entity, FighterData *fighter,
			int32_t fighterId);
void VS__tickHitState(Entity *entity, FighterData *fighter,
			int32_t fighterId);
void VS__tickFlatState(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data, int32_t fighterId);
void VS__tickStunState(Entity *entity);
void VS__tickConfusedState(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data, int32_t fighterId);
void VS__tickSenileState(DigimonEntity *entity, FighterData *data);
void VS__tickChargeState(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data);
void VS__tickCooldownState(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data);
void VS__tickQueuedMove(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data, int32_t fighterId);
int32_t VS__handlePartnerMoveCommand(DigimonEntity *entity, DigimonEntity *other,
			   FighterData *data);
int32_t VS__tickMeleeAttack(DigimonEntity *entity, DigimonEntity *other,
			   FighterData *data, int16_t fighterId);
void VS__tickRangedAttack(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data, int16_t move);
uint32_t VS__entityGetMoveWithHighestDistance(DigimonEntity *entity);
void VS__setWalking(Entity *entity, Stats *stats, uint16_t flags);
void VS__backAwayFromTarget(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data);
void VS__moveTowardLocation(DigimonEntity *entity, VECTOR *location, int16_t dx,
			int16_t dy);
void VS__tickFighterAction(int32_t fighterId);
void VS__confusedRotate(Entity *entity);
void VS__maintainTargetDistance(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data);
void VS__maintainDistanceRange(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data, int32_t min, int32_t max);
int32_t VS__getContactRangeSquared(Entity *a, Entity *b);
void VS__increaseSpeedBuffer(FighterData *fighter, Stats *stats);
int32_t VS__hasAffordableMoves2(uint16_t *array, uint8_t fighterId);
void VS__startWalkingAnimation(Entity *entity, Stats *stats, uint16_t flags);
void VS__initializePlayerMarker(void);
void VS__clearBlockedAttacks(FighterData *fighter);
void VS__findUnblockedRotation(Entity *entity, int16_t *rotationY, int16_t type,
			int16_t oldRotation);
int32_t VS__combatMain(void);
void VS__func_800F7284(void);
void VS__func_800F7338(int32_t id);
void VS__renderPlayerMarker(int32_t id);
void VS__tickPlayerInput();
void VS__tickInput(void);
void VS__tickDigimonP1(int32_t instanceId);
void VS__tickDigimonP2(int32_t instanceId);
void VS__func_800F7AC0(void);
void VS__func_800F7CD8(void);
int32_t VS__func_800F7DCC(uint8_t player, int32_t value);
void VS__func_800F7E48(uint8_t *state);
void VS__func_800F7F0C(uint8_t id);
void VS__func_800F7FD4(uint8_t id);
void VS__func_800F8024(POLY_FT4 *poly);
void VS__initializeVSMode(char *namesP1, char *namesP2);
void VS__func_800F8148(uint8_t id);
void VS__func_800F87E0();
void VS__func_800F9DC8(int32_t id);
int32_t VS__func_800F9E38(uint32_t buttons);
int32_t VS__func_800F9EBC(void);
void VS__func_800FA088(int32_t id);
void VS__func_800FA234(int32_t depth);
int32_t VS__func_800FA4B8(void);
void VS__func_800FA5CC(int32_t id);

extern int16_t MAIN_D_8013527C[2];
extern char MAIN_D_80134500[8];
extern char MAIN_D_80134508[8];
extern char MAIN_D_80124C24[];
extern char MAIN_D_80124C54[];
extern int16_t MAIN_D_80134F24;
extern int16_t MAIN_D_80134F26;
extern uint8_t MAIN_D_80134F28;
extern int16_t MAIN_D_80134F2A;
extern int16_t MAIN_D_80134F2C;
extern VECTOR VS_D_80071744;
extern int16_t MAIN_D_801352A4;
extern int32_t MAIN_D_80134F20;
extern int16_t MAIN_D_801B1C70[];
extern int16_t MAIN_D_801B1C72[];
extern int16_t MAIN_D_801B1C74[];
extern int16_t MAIN_D_801B1C76[];
extern int16_t MAIN_D_801B1C78[];
extern int16_t MAIN_D_801B1C7A[];
extern int16_t MAIN_D_8012F42C[];
extern uint8_t MAIN_D_8012F438[];
extern uint8_t MAIN_D_8012F439[];
extern uint8_t MAIN_D_8012F43A[];
extern uint8_t MAIN_D_80135274;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern int32_t VIEWPORT_DISTANCE;
extern uint8_t MAIN_D_80134F2E;
extern uint8_t MAIN_D_801344F8[4];
extern uint8_t MAIN_D_801344FC[4];
extern uint8_t VS_D_8006FF20[][14];
extern int16_t MAIN_D_80135280[2];
extern int16_t MAIN_D_80134D66;
extern int16_t ENEMY_COUNT;
extern int32_t MAIN_D_80134D74;
extern int32_t MAIN_D_80134D7C[2];
extern int32_t MAIN_D_80134D84;
extern int32_t MAIN_D_80134F40;
extern int32_t MAIN_D_80134F4C;
extern int32_t MAIN_D_8013528C;
extern int32_t MAIN_D_80135290;
extern uint8_t MAIN_D_80134520[8];
extern uint8_t MAIN_D_80134F3C;
extern int16_t INITIAL_COMBAT_STATS[][6];
extern int8_t GAME_STATE;
extern int16_t MAIN_D_80135294;
extern int32_t MAIN_D_80135268;
extern uint8_t *GENERAL_BUFFER_PTR;
extern int16_t MAIN_D_8013527E;
extern uint8_t MAIN_D_80135288;
extern int16_t MAIN_D_80135278[2];
extern uint8_t MAIN_D_80134E78[2];
extern uint8_t MAIN_D_80134F30;
extern uint8_t MAIN_D_80134F3D;
extern uint32_t MAIN_D_80134F34;
extern uint32_t MAIN_D_80134F38;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern uint8_t MAIN_D_80134F44;
extern int16_t MAIN_D_80135264;
extern char *MAIN_D_8013526C;
extern char *MAIN_D_80135270;
extern char MAIN_D_80134518[8];
extern char MAIN_D_8012F458[];
extern uint8_t VS_D_800716A8[];
extern uint8_t VS_D_800716AD[];
extern Entity *MAIN_D_80134D60;
extern int32_t MAIN_D_80134F48;
extern int16_t MAIN_D_80134510[4];
extern int32_t COMBAT_AREA_CENTER_Y;
extern int32_t COMBAT_AREA_CENTER_X;
extern char MAIN_D_8012F48C[];
extern char MAIN_D_8012F4A4[];
extern char MAIN_D_8012F4B8[];
extern char MAIN_D_8012F4CC[];
extern char MAIN_D_8012F4E4[];
extern char MAIN_D_8012F4FC[];
extern uint8_t MAIN_D_801B1C7C[];
extern int8_t MAIN_D_80134F52[2];
extern int32_t MAIN_D_80134F54;
extern uint8_t VS_D_800716B3[];
extern char MAIN_D_8012F510[];
extern char MAIN_D_8012F51C[];
extern int16_t MAIN_D_80134F50;
extern char MAIN_D_8012F464[];
extern char MAIN_D_8012F478[];
extern uint8_t *GENERAL_BUFFER_PTR;
extern uint8_t MAIN_D_801B1CB2[];
extern uint8_t MAIN_D_801B1D02[];
extern uint8_t MAIN_D_80134550[4];
extern uint8_t MAIN_D_80134F58;
extern uint8_t MAIN_D_80134F59;
extern uint8_t MAIN_D_80134F5A;
extern uint8_t MAIN_D_80134F5B;
extern uint8_t MAIN_D_80134F5C;
extern uint8_t VS_D_800716B2[];
extern VsUISprite MAIN_D_8012F650[];
extern VsUISprite MAIN_D_8012F690[];

static void *vs_functions[] = {
	VS__func_800FA5CC,
	VS__func_800FA4B8,
	VS__func_800FA234,
	VS__func_800FA088,
	VS__func_800F9EBC,
	VS__func_800F9E38,
	VS__func_800F9DC8,
	VS__func_800F87E0,
	VS__func_800F8148,
	VS__initializeVSMode,
	VS__func_800F8024,
	VS__func_800F7FD4,
	VS__func_800F7F0C,
	VS__func_800F7E48,
	VS__func_800F7DCC,
	VS__func_800F7CD8,
	VS__func_800F7AC0,
	VS__tickDigimonP2,
	VS__tickDigimonP1,
	VS__tickInput,
	VS__tickPlayerInput,
	VS__renderPlayerMarker,
	VS__func_800F7338,
	VS__func_800F7284,
	VS__combatMain,
	VS__findUnblockedRotation,
	VS__clearBlockedAttacks,
	VS__initializePlayerMarker,
	VS__startWalkingAnimation,
	VS__hasAffordableMoves2,
	VS__increaseSpeedBuffer,
	VS__getContactRangeSquared,
	VS__maintainDistanceRange,
	VS__maintainTargetDistance,
	VS__confusedRotate,
	VS__tickFighterAction,
	VS__moveTowardLocation,
	VS__backAwayFromTarget,
	VS__setWalking,
	VS__entityGetMoveWithHighestDistance,
	VS__tickRangedAttack,
	VS__tickMeleeAttack,
	VS__handlePartnerMoveCommand,
	VS__tickQueuedMove,
	VS__tickCooldownState,
	VS__tickChargeState,
	VS__tickSenileState,
	VS__tickConfusedState,
	VS__tickStunState,
	VS__tickFlatState,
	VS__tickHitState,
	VS__tickAttackState,
	VS__func_800F51B8,
	VS__faintDigimon,
	VS__func_800F4F9C,
	VS__func_800F4CB4,
	VS__checkAnyDigimonDead,
	VS__resetFlatten,
	VS__removePlayerMarket,
	VS__removeCombatObjects,
	VS__isButtonsPressed,
	VS__deinitializeCombat,
	VS__handlePause,
	VS__tickFighterStates,
	VS__digimonAiTickVS,
	VS__checkEndCondition,
	VS__func_800F34F0,
	VS__combatInit,
	VS__renderIntroName,
	VS__tickIntroName,
	VS__renderNumber2,
	VS__func_800F277C,
	VS__tickIntroStats,
	VS__func_800F23D0,
	VS__renderIntroNameChar,
	VS__renderIntroStatBar,
	VS__func_800F1E9C,
	VS__func_800F1E6C,
	VS__removeIntroText,
	VS__func_800F1DB8,
	VS__addIntroText,
	VS__drawStatLabelText,
	VS__placePlayer2,
	VS__placePlayer1,
};

void VS__placePlayer1(int32_t stage)
{
	int16_t startX;

	if (stage == 1) {
		startX = 1800;
	} else {
		startX = 2500;
	}

	setEntityPosition(1, startX, 0, 0);
	setEntityRotation(1, 0, 0x400, 0);
	startAnimation(ENTITY_TABLE[1], 33);
}

void VS__placePlayer2(int32_t stage)
{
	int16_t startX;

	if (stage == 1) {
		startX = -1800;
	} else {
		startX = -2500;
	}

	setEntityPosition(2, startX, 0, 0);
	setEntityRotation(2, 0, 0xc00, 0);
	startAnimation(ENTITY_TABLE[2], 33);
}

void VS__drawStatLabelText(void)
{
	int32_t i;
	int32_t y;
	char *text;

	clearTextArea();
	drawString(MAIN_D_80134500, 0, 0);
	drawString(MAIN_D_80134508, 0, 12);

	for (i = 2, y = 24, text = MAIN_D_80124C24;
	     i < 6;
	     ++i, text += 12, y += 12) {
		drawString(text, 0, y);
		DrawSync(0);
	}

	drawString(MAIN_D_80124C54, 0, 0xf0);
}

void VS__addIntroText(Entity *entity, int32_t id)
{
	int32_t len;

	MAIN_D_80134F24 = 4;
	MAIN_D_80134F26 = 0;

	switch (DIGIMON_DATA[entity->type].special[0]) {
	case 0:
		MAIN_D_80134F28 = 3;
		break;
	case 1:
		MAIN_D_80134F28 = 1;
		break;
	case 2:
		MAIN_D_80134F28 = 6;
		break;
	case 3:
		MAIN_D_80134F28 = 2;
		break;
	case 4:
		MAIN_D_80134F28 = 4;
		break;
	case 5:
		MAIN_D_80134F28 = 0;
		break;
	case 6:
		MAIN_D_80134F28 = 5;
		break;
	default:
		MAIN_D_80134F28 = 0;
		break;
	}

	len = strlen(DIGIMON_DATA[entity->type].name) / 2;
	if (entity->type == 0x4e || entity->type == 0x3c) {
		len = 10;
	}

	MAIN_D_80134F2A = -(len * 16);
	MAIN_D_80134F2C = 68;
	addObject(0x1ab, id, VS__tickIntroName, VS__renderIntroName);
}

void VS__func_800F1DB8(Entity *entity)
{
	if (MAIN_D_801352A4 != -1) {
		entity->posData->location = VS_D_80071744;
		entity->anim.locX = VS_D_80071744.vx << 15;
		entity->anim.locY = VS_D_80071744.vy << 15;
		entity->anim.locZ = VS_D_80071744.vz << 15;
		startAnimation(entity, 0x21);
		MAIN_D_801352A4 = -1;
	}
}

void VS__removeIntroText(int32_t id)
{
	removeObject(0x1ab, id);
}

void VS__func_800F1E6C(int32_t id)
{
	if (MAIN_D_80134F20 != 0) {
		MAIN_D_80134F20 = 0;
		removeObject(0x1a9, id);
	}
}

void VS__func_800F1E9C(Entity *entity, int32_t id)
{
	if (MAIN_D_80134F20 != 1) {
		MAIN_D_80134F20 = 1;
		MAIN_D_801B1C70[0] = -100;
		MAIN_D_801B1C72[0] = -100;
		MAIN_D_801B1C74[0] = -10;
		MAIN_D_801B1C76[0] = -10;
		MAIN_D_801B1C78[0] = -10;
		MAIN_D_801B1C7A[0] = -10;
		addObject(0x1a9, id, VS__tickIntroStats, VS__func_800F277C);
	}
}

void VS__renderIntroStatBar(int32_t stat, int32_t value)
{
	POLY_F4 *prim;

	prim = (POLY_F4 *)GsGetWorkBase();

	SetPolyF4(prim);
	setRGB0(prim, 80, 200, 80);
	setXY4(prim,
	       -50, stat * 16 - 26,
	       value * 100 / MAIN_D_8012F42C[stat] - 50, stat * 16 - 26,
	       -50, stat * 16 - 18,
	       value * 100 / MAIN_D_8012F42C[stat] - 50, stat * 16 - 18);

	GsSetWorkBase((PACKET *)prim);
}

void VS__renderIntroNameChar(int16_t x, int16_t y, int16_t size,
			     uint8_t character)
{
	POLY_GT4 *prim;
	uint8_t u;
	uint8_t v;

	prim = (POLY_GT4 *)GsGetWorkBase();

	MAIN_func_80092BB0(prim);
	prim->tpage = 12;
	prim->clut = GetClut(0, 0x1e0);
	prim->r0 = MAIN_D_8012F438[MAIN_D_80134F28 * 3];
	prim->g0 = MAIN_D_8012F439[MAIN_D_80134F28 * 3];
	prim->b0 = MAIN_D_8012F43A[MAIN_D_80134F28 * 3];
	prim->r1 = MAIN_D_8012F438[MAIN_D_80134F28 * 3];
	prim->g1 = MAIN_D_8012F439[MAIN_D_80134F28 * 3];
	prim->b1 = MAIN_D_8012F43A[MAIN_D_80134F28 * 3];
	prim->r2 = MAIN_D_8012F438[MAIN_D_80134F28 * 3] / 10;
	prim->g2 = MAIN_D_8012F439[MAIN_D_80134F28 * 3] / 10;
	prim->b2 = MAIN_D_8012F43A[MAIN_D_80134F28 * 3] / 10;
	prim->r3 = MAIN_D_8012F438[MAIN_D_80134F28 * 3] / 10;
	prim->g3 = MAIN_D_8012F439[MAIN_D_80134F28 * 3] / 10;
	prim->b3 = MAIN_D_8012F43A[MAIN_D_80134F28 * 3] / 10;

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

void VS__func_800F23D0(int32_t stage)
{
	int32_t x;
	int32_t i;
	uint32_t pad;
	uint32_t prev;

	if (stage == 1) {
		x = 1300;
	} else {
		x = 2000;
	}

	VS__placePlayer1(stage);
	VS__placePlayer2(stage);
	VS__drawStatLabelText();
	VS_startCameraChase(ENTITY_TABLE[1], x, 0);
	VS__addIntroText(ENTITY_TABLE[1], 1);
	stopBGM();
	stopSound();
	playMusic(MAIN_D_80135274, 0);

	i = 0;
	fadeFromBlack(5);

	for (; i < 6; ++i) {
		VS_tickFrame();
	}

	while (ENTITY_TABLE[1]->anim.animFlag & 1) {
		pad = PadRead(1);
		VS_tickFrame();

		if ((pad & ~prev) & 0x40) {
			VS__func_800F1DB8(ENTITY_TABLE[1]);
			prev = pad;
			break;
		}

		prev = pad;
	}

	VS__removeIntroText(1);
	VS__func_800F1E6C(1);
	removeObject(0x1aa, 0);
	VS_startCameraChase(ENTITY_TABLE[2], -x, 1);
	VS__addIntroText(ENTITY_TABLE[2], 2);
	stopBGM();
	stopSound();
	playMusic(MAIN_D_80135274, 1);

	while (ENTITY_TABLE[2]->anim.animFlag & 1) {
		pad = PadRead(1);
		pad = (pad >> 16) & 0xffff;
		prev = (prev >> 16) & 0xffff;

		VS_tickFrame();

		if ((pad & ~prev) & 0x40) {
			prev = pad;
			VS__func_800F1DB8(ENTITY_TABLE[2]);
			break;
		}

		prev = pad;
	}

	VS__removeIntroText(2);
	VS__func_800F1E6C(2);
	removeObject(0x1aa, 0);
	stopBGM();
	stopSound();
}

void VS__tickIntroStats(int32_t id)
{
	Stats *stats;

	MAIN_D_801B1C70[0] += 200;
	MAIN_D_801B1C72[0] += 200;
	MAIN_D_801B1C74[0] += 20;
	MAIN_D_801B1C76[0] += 20;
	MAIN_D_801B1C78[0] += 20;
	MAIN_D_801B1C7A[0] += 20;

	stats = &((DigimonEntity *)ENTITY_TABLE[id])->stats;
	if (stats->current.currentHP < MAIN_D_801B1C70[0]) {
		MAIN_D_801B1C70[0] = stats->current.currentHP;
	}

	if (stats->current.currentMP < MAIN_D_801B1C72[0]) {
		MAIN_D_801B1C72[0] = stats->current.currentMP;
	}

	if (stats->base.off < MAIN_D_801B1C74[0]) {
		MAIN_D_801B1C74[0] = stats->base.off;
	}

	if (stats->base.def < MAIN_D_801B1C76[0]) {
		MAIN_D_801B1C76[0] = stats->base.def;
	}

	if (stats->base.speed < MAIN_D_801B1C78[0]) {
		MAIN_D_801B1C78[0] = stats->base.speed;
	}

	if (stats->base.brain < MAIN_D_801B1C7A[0]) {
		MAIN_D_801B1C7A[0] = stats->base.brain;
	}
}

void VS__func_800F277C(int32_t id)
{
	Stats *stats;
	int32_t i;

	for (i = 0; i < 6; ++i) {
		renderString(0, -100, i * 16 - 28, 48, 12, 0, i * 12, 0, 1);
		VS__renderIntroStatBar((int16_t)i, MAIN_D_801B1C70[i]);
	}

	stats = &((DigimonEntity *)ENTITY_TABLE[id])->stats;
	if (MAIN_D_801B1C70[0] != stats->current.currentHP ||
	    MAIN_D_801B1C72[0] != stats->current.currentMP ||
	    MAIN_D_801B1C74[0] != stats->base.off ||
	    MAIN_D_801B1C76[0] != stats->base.def ||
	    MAIN_D_801B1C78[0] != stats->base.speed ||
	    MAIN_D_801B1C7A[0] != stats->base.brain) {
		playSound(0, 0x16);
	} else {
		for (i = 0; i < 6; ++i) {
			VS__renderNumber2(52, (int16_t)(i * 16 - 28), 4,
					   MAIN_D_801B1C70[i], 3);
		}
	}
}

void VS__renderNumber2(int32_t x, int32_t y, int32_t digits, int32_t value,
			int32_t layer)
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
		setPosDataPolyFT4(prim,
				  x + (((int32_t)width - 1) - i) * 12, y,
				  12, 12);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void VS__tickIntroName(int32_t id)
{
	int32_t len;

	++MAIN_D_80134F24;

	len = strlen(DIGIMON_DATA[ENTITY_TABLE[id]->type].name) / 2;
	if (ENTITY_TABLE[id]->type == 0x4e || ENTITY_TABLE[id]->type == 0x3c) {
		len = 10;
	}

	if (len == MAIN_D_80134F26 && MAIN_D_80134F2E == 3) {
		if (MAIN_D_801352A4 == 0) {
			startAnimation(ENTITY_TABLE[id], 0x23);
			MAIN_D_801352A4 = 20;
		}

		if (MAIN_D_80134F2C >= -71) {
			MAIN_D_80134F2C -= 28;
		} else {
			VS__func_800F1E9C(ENTITY_TABLE[id], id);
		}
	}
}

void VS__renderIntroName(int32_t id)
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

	if (MAIN_D_80134F24 % 4 == 0) {
		if (MAIN_D_80134F26 < charCount) {
			++MAIN_D_80134F26;
			MAIN_D_80134F2E = 0;
		}
	} else if (MAIN_D_80134F2E != 3) {
		++MAIN_D_80134F2E;
	}

	charIndex = 0;
	for (i = 0; i < MAIN_D_80134F26; ++i) {
		character = VS_D_8006FF20[ENTITY_TABLE[entityIndex]->type][charIndex++];
		if (character == 0x3d) {
			character = VS_D_8006FF20[ENTITY_TABLE[entityIndex]->type][charIndex++];
		}

		if (i == MAIN_D_80134F26 - 1) {
			y = MAIN_D_80134F2C - MAIN_D_801344FC[MAIN_D_80134F2E];
			size = MAIN_D_801344F8[MAIN_D_80134F2E];
		} else {
			size = 32;
			y = MAIN_D_80134F2C;
		}

		VS__renderIntroNameChar((int16_t)(MAIN_D_80134F2A + i * 32), y,
					size, character);

		if (character == 0x1f || character == 0x25) {
			character = VS_D_8006FF20[ENTITY_TABLE[entityIndex]->type][charIndex++];
			VS__renderIntroNameChar((int16_t)(MAIN_D_80134F2A + i * 32),
						y, size, character);
		}
	}
}

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
		out = INITIAL_COMBAT_STATS[i];
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
		stats->current.unk2_2 = 0;

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

void VS__func_800F34F0(void)
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
	VS_initializeEFEEngine(GENERAL_BUFFER_PTR);

	for (i = 0; i <= ENEMY_COUNT; ++i) {
		entity = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		entity->stats.current.unk1 = -1;
		*(int8_t *)&entity->stats.current.unk2_1 = -1;

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
		if (VS__checkAnyDigimonDead() == 0) {
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
		    COMBAT_DATA_PTR->fighter[0].hpDamageBuffer > 0) {
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
		    fighter->hpDamageBuffer <= 0) {
			return 0;
		}

		if (VS__checkAnyDigimonDead() != 0) {
			return 0;
		}

		MAIN_D_80135280[0] = MAIN_D_8013527C[0] - ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]])->stats.current.currentHP;
		MAIN_D_80135280[1] = MAIN_D_8013527E - ((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]])->stats.current.currentHP;
		COMBAT_DATA_PTR->fighter[0].hpDamageBuffer = 0;
		COMBAT_DATA_PTR->fighter[1].hpDamageBuffer = 0;

		if (MAIN_D_80135280[0] > MAIN_D_80135280[1]) {
			VS__func_800F4CB4(1, 0);
			return -1;
		}

		if (MAIN_D_80135280[0] < MAIN_D_80135280[1]) {
			VS__func_800F4CB4(0, 1);
			return 1;
		}

		if (MAIN_D_80135280[0] == MAIN_D_80135280[1]) {
			VS__func_800F4CB4(1, 1);
			return 2;
		}
	}

	return 0;
}

void VS__digimonAiTickVS(uint8_t fighterId)
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
		    (70 - MAIN_D_80135278[id] > random(100))) {
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
			if (VS_isMoveUsable(&entity->entity, data,
					     COMBAT_DATA_PTR->player.currentCommand[id] - 8) != 0) {
				data->targetId = otherId;
				if ((entity->stats.base.moves[COMBAT_DATA_PTR->player.currentCommand[id] - 8] != data->queuedAnim) ||
				    (data->moveRange <= 0)) {
					VS_setupQueuedMove(entity, data, fighterId,
							 COMBAT_DATA_PTR->player.currentCommand[id] - 8);
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
				VS_setupQueuedMove(entity, data, fighterId, 3);
			}
			VS_applyChargeRequirement(entity, data,
					 DIGIMON_DATA[entity->entity.type].moves[data->queuedAnim - 0x2e]);
			return;
		}

		if (COMBAT_DATA_PTR->player.currentCommand[id] != 2
		    && COMBAT_DATA_PTR->player.currentCommand[id] != 4) {
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
		move = VS_selectMoveByPower(fighterId, array);
		entity->stats.current.chargeMode = 0;
		break;
	case 4:
		if (VS__hasAffordableMoves2(array, fighterId) == 0) {
			data->cooldown = 0x50;
			data->flags |= 0x800;
			return;
		}
		move = VS_selectMoveByMpCost(fighterId, array);
		entity->stats.current.chargeMode = 2;
		break;
	}

	if (move == -1) {
		VS_selectPartnerMove(entity, data, fighterId);
	} else {
		VS_setupQueuedMove(entity, data, fighterId, move);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/vs", VS__tickFighterStates);

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
		MAIN_func_800E642C();
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
	VS__removeCombatObjects();

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
		stats->base.off = INITIAL_COMBAT_STATS[i][2];
		stats->base.def = INITIAL_COMBAT_STATS[i][3];
		stats->base.speed = INITIAL_COMBAT_STATS[i][4];
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

void VS__removeCombatObjects(void)
{
	int32_t i;
	Entity *entity;

	if (ENTITY_TABLE[1]->type == ENTITY_TABLE[2]->type) {
		VS__removePlayerMarket();
	}
	VS_removeAllStunEffects();
	VS_removeAllFinisherAuras();
	VS_removeAllPoisonEffects();
	VS_removeAllAuraProjectiles();

	for (i = 0; i <= ENEMY_COUNT; ++i) {
		entity = ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		VS_removeMoveEffect(entity, &COMBAT_DATA_PTR->fighter[i]);
	}

	VS_unloadAllEFESlots();
	VS_removeEFEEngine();
}

void VS__removePlayerMarket(void)
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

	*(int8_t *)&entity->flatSprite = -1;
	fighter->flags &= 0xfff7;
	fighter->flatTimer = 0;
	entity->posData->scale.vx = 0x1000;
	entity->posData->scale.vy = 0x1000;
	entity->posData->scale.vz = 0x1000;
}

int32_t VS__checkAnyDigimonDead(void)
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

void VS__func_800F4CB4(uint8_t hasLostP1, uint8_t hasLostP2)
{
	MAIN_D_80134F44 = 0;
	addObject(0x1a2, 0, NULL, (RenderFunction)VS__func_800F7284);
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
		VS__func_800F4F9C();

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
				VS__func_800F4F9C();
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

void VS__func_800F4F9C(void)
{
	RECT finalPos;
	RECT startPos;
	char *name1;
	char *name2;

	name1 = MAIN_D_8013526C + VS_D_800716A8[MAIN_D_80135264] * 64;
	name2 = MAIN_D_80135270 + VS_D_800716AD[MAIN_D_80135264] * 64;

	clearTextArea();
	drawString(MAIN_D_80134518, 6, 0);
	drawString(MAIN_D_8012F458, 0, 12);
	drawString(name1 + 14, (120 - strlen(name1 + 14) * 6) / 2, 24);
	drawString(name2 + 14, (120 - strlen(name2 + 14) * 6) / 2, 36);
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

	createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, NULL,
			    VS__func_800F7338);
}

void VS__faintDigimon(DigimonEntity *entity, FighterData *fighter,
		     uint8_t fighterId)
{
	entity->stats.current.unk2_2 = 1;
	fighter->flags |= 0x8000;

	startAnimation(&entity->entity, 0x2b);
	VS__resetFlatten(fighterId);
	VS_removeStatusEffects(entity, fighter);

	fighter->flags &= 0xff40;
	fighter->flags |= 0x40;
	fighter->moveRange = -1;

	VS_resetFighterAction(fighter);
}

int32_t VS__func_800F51B8(int32_t value)
{
	if (value < 10)
	{
		return 1;
	}

	if (value < 100)
	{
		return 2;
	}

	if (value < 1000)
	{
		return 3;
	}

	return 4;
}

void VS__tickAttackState(Entity *entity, FighterData *fighter,
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
	VS__tickFighterAction(fighterId);
}

void VS__tickHitState(Entity *entity, FighterData *fighter,
			int32_t fighterId)
{
	VS__tickFighterAction(fighterId);
	if ((entity->anim.animFlag & 1) == 0) {
		fighter->invulnerableTimer--;
		if (fighter->invulnerableTimer == 0) {
			entity->anim.animFlag |= 1;
		}
	}
}

void VS__tickFlatState(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data, int32_t fighterId)
{
	if (MAIN_D_80134D74 != 0) {
		handleBattleIdle(entity, &entity->stats, data->flags);
		return;
	}

	if (VS__handlePartnerMoveCommand(entity, other, data) != 0) {
		return;
	}

	VS__tickRangedAttack(entity, other, data, 0x79);

	if ((data->flags & 0x20) != 0) {
		entity->entity.flatSprite = 2;
	}
}

void VS__tickStunState(Entity *entity)
{
	if (entity->anim.animId != 0x22) {
		startAnimation(entity, 0x22);
	}

	entity->anim.animFlag &= 0xfe;
}

void VS__tickConfusedState(DigimonEntity *entity, DigimonEntity *other,
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
		if (VS__tickMeleeAttack(entity, NULL, data, fighterId) != 0) {
			collisionGrace(NULL, &entity->entity, 280, 200);
		}

		if ((data->flags & 0x20) != 0) {
			return;
		}

		if (random(100) < 5) {
			handleBattleIdle(entity, &entity->stats, data->flags);
			VS_startFighterMove(entity, other, data);
		}
	} else {
		range = data->moveRange;
		switch (range) {
		case 1:
			if (VS__tickMeleeAttack(entity, other, data, fighterId) != 0) {
				collisionGrace(&other->entity,
					       &entity->entity, 280, 200);
			}
			break;
		case 2:
		case 3:
			VS__tickRangedAttack(entity, other, data,
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

void VS__tickSenileState(DigimonEntity *entity, FighterData *data)
{
	data->senileTimer--;
	if (data->senileTimer == 0) {
		data->flags &= 0xdfbf;
	} else {
		handleBattleIdle(entity, &entity->stats, data->flags);
	}
}

void VS__tickChargeState(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data)
{
	int32_t result;
	int16_t tech;

	if (MAIN_D_80134D74 != 0) {
		handleBattleIdle(entity, &entity->stats, data->flags);
		return;
	}

	result = VS__handlePartnerMoveCommand(entity, other, data);
	if (data->cooldown != 0) {
		if (result == 0) {
			switch (entity->stats.current.chargeMode) {
			case 0:
				VS__maintainTargetDistance(entity, other, data);
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
				VS__backAwayFromTarget(entity, other, data);
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
			VS__maintainTargetDistance(entity, other, data);
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
			VS__backAwayFromTarget(entity, other, data);
			if (data->speedBuffer == 100) {
				data->flags &= 0xf7ff;
			}
			break;
		}
	}
}

void VS__tickCooldownState(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data)
{
	if (MAIN_D_80134D74 != 0) {
		handleBattleIdle(entity, &entity->stats, data->flags);
		return;
	}

	switch (entity->stats.current.chargeMode) {
	case 0:
	case 1:
		VS__maintainTargetDistance(entity, other, data);
		break;
	case 2:
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__backAwayFromTarget(entity, other, data);
		break;
	}

	if (data->cooldown < 2) {
		data->flags &= 0xefbf;
		data->cooldown = 0;
	}
}

void VS__tickQueuedMove(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data, int32_t fighterId)
{
	int32_t range;

	if (MAIN_D_80134D74 != 0 && MAIN_D_80134D60 != &entity->entity) {
		handleBattleIdle(entity, &entity->stats, data->flags);
		return;
	}

	if (VS__handlePartnerMoveCommand(entity, other, data) != 0) {
		return;
	}

	range = data->moveRange;
	switch (range) {
	case 1:
		if (VS__tickMeleeAttack(entity, other, data, fighterId) != 0) {
			collisionGrace(&other->entity, &entity->entity, 280,
				       200);
		}
		break;
	case 2:
	case 3:
		VS__tickRangedAttack(entity, other, data,
				   DIGIMON_DATA[entity->entity.type].moves[data->queuedAnim - 0x2e]);
		break;
	case 4:
		handleBattleIdle(entity, &entity->stats, data->flags);
		VS_startFighterMove(entity, other, data);
		break;
	}
}

int32_t VS__handlePartnerMoveCommand(DigimonEntity *entity, DigimonEntity *other,
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
			VS__backAwayFromTarget(entity, other, data);
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

int32_t VS__tickMeleeAttack(DigimonEntity *entity, DigimonEntity *other,
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
					MAIN_D_80134F4C = VS_addFinisherAura(&entity->entity, 80);
					MAIN_D_80134F48 = 80;

					return 0;
				}
			}

			if (VS_selectMoveTarget(entity, data) != 0) {
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

void VS__tickRangedAttack(DigimonEntity *entity, DigimonEntity *other,
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
		VS__moveTowardLocation(entity, &other->entity.posData->location,
				   280, 200);
		++data->unk15;
	} else if (distance < maxDistance - minDistance) {
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__backAwayFromTarget(entity, other, data);
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

	VS__startWalkingAnimation(entity, stats, flags);
}

void VS__backAwayFromTarget(DigimonEntity *entity, DigimonEntity *other,
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
			VS__findUnblockedRotation(&entity->entity, rotationY, result,
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
			VS__findUnblockedRotation(&entity->entity, rotationY, result,
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
		VS__findUnblockedRotation(&entity->entity, rotationY, result,
				   initRotation);
	}
}

void VS__moveTowardLocation(DigimonEntity *entity, VECTOR *location, int16_t dx,
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

void VS__tickFighterAction(int32_t fighterId)
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
			entity->stats.current.unk2_2 = 0;
		}

		if (fighter->flags & 0x80) {
			fighter->flags &= 0xff7f;
			VS__clearBlockedAttacks(fighter);
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

	if (random(10) < 8) {
		return;
	}

	rotationY = &entity->posData->rotation.vy;

	*rotationY += random(0x400) - 0x200;
	if (*rotationY < 0) {
		*rotationY += 4096;
	} else {
		*rotationY %= 4096;
	}
}

void VS__maintainTargetDistance(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data)
{
	if ((entity->entity.anim.animId >= 0x23) &&
	    (entity->entity.anim.animId < 0x25)) {
		VS__maintainDistanceRange(entity, other, data, 160000, 320000);
	} else {
		VS__maintainDistanceRange(entity, other, data, 0, 480000);
	}
}

void VS__maintainDistanceRange(DigimonEntity *entity, DigimonEntity *other,
			FighterData *data, int32_t min, int32_t max)
{
	uint32_t actualDistance;
	uint32_t baseDistance;

	actualDistance = VS_getDistanceSquared(&entity->entity, &other->entity);
	baseDistance = VS__getContactRangeSquared(&entity->entity, &other->entity);

	if (actualDistance < baseDistance + min) {
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__backAwayFromTarget(entity, other, data);
	} else if (baseDistance + max < actualDistance) {
		VS__setWalking(&entity->entity, &entity->stats, data->flags);
		VS__moveTowardLocation(entity, &other->entity.posData->location,
				   280, 200);
	} else {
		data->unk16 = 0;
		handleBattleIdle(entity, &entity->stats, data->flags);
		entityLookAtLocation(&entity->entity,
				     &other->entity.posData->location);
	}
}

int32_t VS__getContactRangeSquared(Entity *a, Entity *b)
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
		if (VS_isMoveUsable(entity, fighter, i) != 0) {
			array[i] = 1;
			found = 1;
		} else {
			array[i] = 0;
		}
	}

	return found;
}

void VS__startWalkingAnimation(Entity *entity, Stats *stats, uint16_t flags)
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

	startAnimation(entity, animId);
}

void VS__initializePlayerMarker(void)
{
	addObject(0x1a3, 0, NULL, VS__renderPlayerMarker);
	addObject(0x1a3, 1, NULL, VS__renderPlayerMarker);
}

void VS__clearBlockedAttacks(FighterData *fighter)
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

void VS__findUnblockedRotation(Entity *entity, int16_t *rotationY, int16_t type,
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
			*rotationY = angles[0] + random(0x400);
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
	VS__func_800F34F0();

	while (1) {
		result = VS__checkEndCondition();
		if (result != 0) {
			break;
		}

		VS__digimonAiTickVS(0);
		VS__digimonAiTickVS(1);
		VS__tickFighterStates();
		VS_tickFrame();
		VS__handlePause();
	}

	MAIN_func_800E642C();

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

void VS__func_800F7284(void)
{
	POLY_FT4 *prim = (POLY_FT4 *)GsGetWorkBase();

	SetPolyFT4(prim);
	prim->tpage = 0xd;
	prim->clut = GetClut(16, 480);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setUVDataPolyFT4(prim, 0, 8, 144, 24);
	setPosDataPolyFT4(prim, -72, -12, 144, 24);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 5, prim++);

	GsSetWorkBase((PACKET *)prim);
}

void VS__func_800F7338(int32_t id)
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

	digits = VS__func_800F51B8(MAIN_D_80135280[1]);
	VS__renderNumber2(x + 42 + (48 - digits * 12) / 2, y + 30, digits,
			   MAIN_D_80135280[1], layer);
	digits = VS__func_800F51B8(MAIN_D_80135280[0]);
	VS__renderNumber2(x + 174 + (48 - digits * 12) / 2, y + 30, digits,
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
	prim->tpage = 6;
	prim->clut = GetClut(0, 0x1f2);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	prim->u0 = id * 48 + 0x88;
	prim->v0 = 0xd8;
	prim->u1 = id * 48 + 0xb8;
	prim->v1 = 0xd8;
	prim->u2 = id * 48 + 0x88;
	prim->v2 = 0xf8;
	prim->u3 = id * 48 + 0xb8;
	prim->v3 = 0xf8;
	prim->x0 = sxy[0];
	prim->y0 = sxy[1] - 32;
	prim->x1 = sxy[0] + 48;
	prim->y1 = sxy[1] - 32;
	prim->x2 = sxy[0];
	prim->y2 = sxy[1] + yOffset;
	prim->x3 = sxy[0] + 48;
	prim->y3 = sxy[1] + yOffset;
	AddPrim(ACTIVE_ORDERING_TABLE->org + otz, prim++);

	GsSetWorkBase((PACKET *)prim);
}

void VS__tickInput(void)
{
	if (GAME_STATE == 4) {
		VS__tickPlayerInput();
	}
}

void VS__tickPlayerInput(int32_t player)
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

void VS__func_800F7AC0(void)
{
	uint8_t *entries[2];
	uint8_t *state;
	uint8_t *p;
	int32_t i;
	int32_t j;
	int32_t rows;

	ENTITY_TABLE[0]->isOnScreen = 0;
	loadTIMFile(MAIN_D_8012F48C, GENERAL_BUFFER);
	loadTIMFile(MAIN_D_8012F4A4, GENERAL_BUFFER);
	loadTIMFile(MAIN_D_8012F4B8, GENERAL_BUFFER);
	loadTIMFile(MAIN_D_8012F4CC, GENERAL_BUFFER);
	loadTIMFile(MAIN_D_8012F4E4, GENERAL_BUFFER);
	loadTIMFile(MAIN_D_8012F4FC, GENERAL_BUFFER);

	entries[0] = (uint8_t *)MAIN_D_8013526C;
	entries[1] = (uint8_t *)MAIN_D_80135270;

	for (i = 0; i < 5; ++i) {
		VS_D_800716A8[i] = 0xff;
		VS_D_800716AD[i] = 0xff;
	}

	for (i = 0; i < 2; ++i) {
		state = &MAIN_D_801B1C7C[i * 0x50];
		state[0x2b] = 0;
		state[0x28] = 0;
		state[0x29] = 0;
		state[0x2d] = 0;
		state[0x2e] = 0;
		state[0x2f] = 0;
		state[0x30] = 0;
		state[0x32] = 0;
		state[0x31] = 0;
		state[0x33] = 0;
		state[0x2c] = 0;
		state[0x35] = 0;
		state[0x34] = 0;
		state[0x36] = 0;
		p = &state[0x37];
		for (j = 0; j < 24; ++j) {
			*p = 0;
			++p;
		}

		for (j = 0; j < 40; ++j) {
			if ((entries[i] + j * 64)[0x1c] != 0) {
				state[state[0x2b]] = j;
				++state[0x2b];
			}
		}

		if (state[0x2b] % 4 == 0) {
			state[0x2a] = state[0x2b] / 4;
		} else {
			rows = state[0x2b] / 4;
			++rows;
			state[0x2a] = rows;
		}
	}
}

void VS__func_800F7CD8(void)
{
	int32_t i;

	MAIN_D_80134F52[0] = -1;
	MAIN_D_80134F52[1] = -1;

	clearTextArea();

	addObject(0x1a0, 0, (TickFunction)VS__func_800F8148, VS__func_800F87E0);
	addObject(0x1a0, 1, (TickFunction)VS__func_800F8148, VS__func_800F87E0);

	fadeFromBlack(10);

	for (i = 0; i < 11; ++i) {
		VS_tickFrame();
	}

	MAIN_D_80134F54 = 0;
	while (MAIN_D_80134F54 == 0) {
		VS_tickFrame();
	}

	i = 0;
	fadeToBlack(20);
	for (; i < 21; ++i) {
		VS_tickFrame();
	}

	removeObject(0x1a0, 0);
	removeObject(0x1a0, 1);
}

int32_t VS__func_800F7DCC(uint8_t player, int32_t value)
{
	int32_t i;
	uint8_t *table;

	if (player == 0) {
		table = VS_D_800716A8;
	} else {
		table = VS_D_800716AD;
	}

	for (i = 0; i < VS_D_800716B3[0]; ++i) {
		if (value == table[i]) {
			playSound(0, 0xb);
			return 1;
		}
	}

	return 0;
}

void VS__func_800F7E48(uint8_t *state)
{
	int32_t i;
	int32_t count;

	playSound(0, 3);

	state[0x33] = 0;
	state[0x32] = 0;
	state[0x35] |= 1 << state[0x34];

	count = VS_D_800716B3[0];
	for (i = 0; i < count; ++i) {
		if ((state[0x35] & (1 << i)) == 0) {
			break;
		}
	}

	if (i == VS_D_800716B3[0]) {
		state[0x4f] = state[0x34];
	}

	if (state[0x34] < VS_D_800716B3[0] - 1) {
		++state[0x34];
	}
}

void VS__func_800F7F0C(uint8_t id)
{
	RECT rect;

	if (MAIN_D_80134F52[id] == -1) {
		drawString(MAIN_D_8012F510, 0, 0);
		drawString(MAIN_D_8012F51C, 0, 12);

		rect.x = (id == 0) ? -132 : 22;
		rect.y = 32;
		rect.w = 108;
		rect.h = 36;
		createStaticUIBox(id, 0, 2, &rect, 0, VS__func_800F9DC8);

		MAIN_D_80134F52[id] = 1;
	}
}

void VS__func_800F7FD4(uint8_t id)
{
	if (MAIN_D_80134F52[id] != -1) {
		removeStaticUIBox(id);
		MAIN_D_80134F52[id] = -1;
	}
}

void VS__func_800F8024(POLY_FT4 *poly)
{
	SetPolyFT4(poly);
	setRGB0(poly, 0x80, 0x80, 0x80);
}

void VS__initializeVSMode(char *namesP1, char *namesP2)
{
	int32_t done;

	MAIN_D_8013526C = namesP1;
	MAIN_D_80135270 = namesP2;
	VS__func_800F7AC0();

	MAIN_D_80134F50 = 0;
	done = 0;
	while (done == 0) {
		switch (MAIN_D_80134F50) {
		case 0:
			VS__func_800F9EBC();
			++MAIN_D_80134F50;
			break;
		case 1:
			VS__func_800FA4B8();
			++MAIN_D_80134F50;
			break;
		case 2:
			VS__func_800F7CD8();
			++MAIN_D_80134F50;
			break;
		case 3:
			VS_initializeVS();
			done = 1;
			break;
		}
	}

	loadStackedTIMFile(MAIN_D_8012F464);
	loadTIMFile(MAIN_D_8012F478, GENERAL_BUFFER_PTR);
}

void VS__func_800F8148(uint8_t id)
{
	uint8_t *state;
	uint8_t *table;
	uint32_t input;
	uint32_t previous;
	uint32_t pressed;
	int32_t idx;
	int32_t count;
	int32_t last;
	int32_t row;
	int32_t i;
	int32_t total;

	if (id == 0) {
		table = VS_D_800716A8;
	} else {
		table = VS_D_800716AD;
	}

	if (id == 1) {
		input = POLLED_INPUT;
		previous = POLLED_INPUT_PREVIOUS;
		POLLED_INPUT = (input >> 16) & 0xffff;
		POLLED_INPUT_PREVIOUS = (previous >> 16) & 0xffff;
	}

	state = &MAIN_D_801B1C7C[id * 0x50];
	++state[0x31];
	if (state[0x31] % 8 == 0) {
		state[0x30] = (state[0x30] + 1) & 1;
	}

	switch (state[0x28]) {
	case 0:
		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x8000) {
			state[0x29]--;
			if (state[0x29] == 0xff) {
				state[0x29] = state[0x2a] - 1;
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x2000) {
			++state[0x29];
			state[0x29] %= state[0x2a];
		}

		if ((row = state[0x29]) == (last = state[0x2a] - 1)) {
			count = state[0x2b] - last * 4;
		} else {
			count = 4;
		}

		idx = row * 4;

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x10) {
			if (VS__func_800F7DCC(id, state[idx]) == 0) {
				table[state[0x34]] = state[idx];
				VS__func_800F7E48(state);
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x40) {
			if (VS__func_800F7DCC(id, state[idx + 3]) == 0 &&
			    count == 4) {
				table[state[0x34]] = (state + idx)[3];
				VS__func_800F7E48(state);
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x80) {
			if (VS__func_800F7DCC(id, state[idx + 1]) == 0 &&
			    count >= 2) {
				table[state[0x34]] = (state + idx)[1];
				VS__func_800F7E48(state);
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x20) {
			if (VS__func_800F7DCC(id, state[idx + 2]) == 0 &&
			    count >= 3) {
				table[state[0x34]] = (state + idx)[2];
				VS__func_800F7E48(state);
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x1000) {
			playSound(0, 2);
			if (state[0x34] != 0) {
				state[0x33] = 0;
			}
			if (state[0x34] != 0) {
				state[0x34]--;
			}
		}

		if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x4000) {
			playSound(0, 2);
			if (state[0x34] != VS_D_800716B3[0] - 1) {
				state[0x33] = 0;
			}
			if (state[0x34] < VS_D_800716B3[0] - 1) {
				++state[0x34];
			}
		}

		if (state[0x2e] == state[0x2c] * 24) {
			if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x4) {
				state[0x2f] = 0xff;
				if (state[0x2c] != 0) {
					state[0x2c]--;
				}
			}

			if (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x8) {
				state[0x2f] = 1;
				if (state[0x2c] < 5) {
					++state[0x2c];
				}
			}
		}

		if (state[0x2e] != state[0x2c] * 24) {
			state[0x2e] += state[0x2f] * 4;
		}

		total = VS_D_800716B3[0];
		for (i = 0; i < total; ++i) {
			if ((state[0x35] & (1 << i)) == 0) {
				break;
			}
		}

		if (i == VS_D_800716B3[0]) {
			VS__func_800F7F0C(id);
			++state[0x28];
		}
		break;
	case 1:
		if ((pressed = POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x800) {
			playSound(0, 3);
			++state[0x28];
			state[0x36] = 1;
			VS__func_800F7FD4(id);
		} else if ((pressed & 0x20) || (pressed & 0x80) ||
			   (pressed & 0x10) || (pressed & 0x40) ||
			   (pressed & 0x8000) || (pressed & 0x2000) ||
			   (pressed & 0x1000) || (pressed & 0x4000)) {
			playSound(0, 4);
			state[0x28]--;
			VS__func_800F7FD4(id);
			state[0x35] = state[0x35] & ~(uint8_t)(1 << state[0x4f]);
			table[state[0x4f]] = 0xff;
		}
		break;
	case 2:
		if (MAIN_D_801B1CB2[0] == 1 && MAIN_D_801B1D02[0] == 1) {
			MAIN_D_80134F54 = 1;
		}
		break;
	}

	if (id == 1) {
		POLLED_INPUT = input;
		POLLED_INPUT_PREVIOUS = previous;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/vs", VS__func_800F87E0);

void VS__func_800F9DC8(int32_t id)
{
	renderString(0,
		     UI_BOX_DATA[id].finalPos.x + 6,
		     UI_BOX_DATA[id].finalPos.y + 6,
		     96, 24, 0, 0, 6 - id, 1);
}

int32_t VS__func_800F9E38(uint32_t buttons)
{
	uint32_t input;
	uint32_t previous;

	if (buttons & ((input = POLLED_INPUT) & ~(previous = POLLED_INPUT_PREVIOUS))) {
		return 1;
	}

	POLLED_INPUT = (input >> 16) & 0xffff;
	POLLED_INPUT_PREVIOUS = (previous >> 16) & 0xffff;

	if (buttons & (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS)) {
		POLLED_INPUT = input;
		POLLED_INPUT_PREVIOUS = previous;

		return 2;
	}

	POLLED_INPUT = input;
	POLLED_INPUT_PREVIOUS = previous;

	return 0;
}

int32_t VS__func_800F9EBC(void)
{
	RECT rect;
	int32_t i;
	int32_t count1;
	int32_t count2;
	int32_t j;

	count2 = 0;
	count1 = 0;

	for (i = 0; i < 40; ++i) {
		if (((uint8_t (*)[64])MAIN_D_8013526C)[i][0x1c] != 0) {
			++count1;
		}
		if (((uint8_t (*)[64])MAIN_D_80135270)[i][0x1c] != 0) {
			++count2;
		}
	}

	if (count1 >= 3 && count2 >= 3) {
		if (count1 >= 5 && count2 >= 5) {
			MAIN_D_80134F58 = 2;
		} else {
			MAIN_D_80134F58 = 1;
		}
	} else {
		MAIN_D_80134F58 = 0;
	}

	rect.x = -90;
	rect.y = -70;
	rect.w = 180;
	rect.h = 128;
	MAIN_D_80134F59 = 0;
	MAIN_D_80134F5A = 0;
	MAIN_D_80134F5B = 0;
	MAIN_D_80134F5C = 0;
	createStaticUIBox(0, 1, 0, &rect, VS__func_800FA088,
			  VS__func_800FA234);

	fadeFromBlack(5);

	for (j = 0; j < 6; ++j) {
		VS_tickFrame();
	}

	while (MAIN_D_80134F5B == 0) {
		VS_tickFrame();
	}

	fadeToBlack(5);

	for (j = 0; j < 6; ++j) {
		VS_tickFrame();
	}

	VS_D_800716B3[0] = MAIN_D_80134550[MAIN_D_80134F59];
	removeStaticUIBox(0);

	if (MAIN_D_80134F5B == 1) {
		return 1;
	}

	return 0;
}

void VS__func_800FA088(int32_t id)
{
	uint8_t previous;

	if (MAIN_D_80134F5B != 0) {
		return;
	}

	if (VS__func_800F9E38(0x40) != 0) {
		playSound(0, 3);
		MAIN_D_80134F5B = 1;
		return;
	}

	++MAIN_D_80134F5C;
	if (MAIN_D_80134F5C % 8 == 0) {
		MAIN_D_80134F5A = (MAIN_D_80134F5A + 1) & 1;
	}

	if (VS__func_800F9E38(0x8000) != 0) {
		previous = MAIN_D_80134F59--;
		if (id == 1) {
			if (MAIN_D_80134F59 == 0xff) {
				MAIN_D_80134F59 = 2;
			}

			playSound(0, 2);
			MAIN_D_80134F5A = 0;
		} else {
			if (MAIN_D_80134F59 == 0xff) {
				MAIN_D_80134F59 = MAIN_D_80134F58;
			}

			if (previous != MAIN_D_80134F59) {
				playSound(0, 2);
				MAIN_D_80134F5A = 0;
			}
		}
	}

	if (VS__func_800F9E38(0x2000) != 0) {
		previous = MAIN_D_80134F59++;
		if (id == 1) {
			MAIN_D_80134F59 %= 3;
			playSound(0, 2);
			MAIN_D_80134F5A = 0;
		} else {
			MAIN_D_80134F59 %= MAIN_D_80134F58 + 1;
			if (previous != MAIN_D_80134F59) {
				playSound(0, 2);
				MAIN_D_80134F5A = 0;
			}
		}
	}
}

void VS__func_800FA234(int32_t depth)
{
	POLY_FT4 *prim;
	VsUISprite *sprite;
	int32_t i;
	int32_t j;
	int32_t x;

	prim = (POLY_FT4 *)GsGetWorkBase();
	sprite = MAIN_D_8012F650;

	for (i = 0, j = -1; i < 8; ++sprite, ++i, ++j) {
		SetPolyFT4(prim);
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
		prim->clut = GetClut(48, sprite->clut);

		if (i > 0 && i < 4 && MAIN_D_80134F59 != j) {
			prim->r0 = 0x40;
			prim->g0 = 0x40;
			prim->b0 = 0x40;
		}

		if (i == 4 && MAIN_D_80134F59 != 0) {
			prim->clut = GetClut(48, 0x1f5);
		}

		if (i == 5 && MAIN_D_80134F59 != 1) {
			prim->clut = GetClut(48, 0x1f5);
		}

		if (i == 6 && MAIN_D_80134F59 != 2) {
			prim->clut = GetClut(48, 0x1f5);
		}

		prim->tpage = 7;

		if (i == MAIN_D_80134F59 + 1 && i > 0 && i < 4) {
			setUVDataPolyFT4(prim,
					 sprite->u + MAIN_D_80134F5A * 40,
					 sprite->v, sprite->w, sprite->h);

			if (i == 3 && MAIN_D_80134F5A == 1) {
				prim->clut = GetClut(48, 0x1ef);
			}
		} else {
			setUVDataPolyFT4(prim, sprite->u, sprite->v, sprite->w,
					 sprite->h);
		}

		if (i != 7) {
			x = sprite->x - 90;
		} else {
			x = (sprite->x - 90) + MAIN_D_80134F59 * 56;
		}

		setPosDataPolyFT4(prim, x, sprite->y - 70, sprite->w,
				  sprite->h);

		AddPrim((ACTIVE_ORDERING_TABLE->org + 6) - depth, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

int32_t VS__func_800FA4B8(void)
{
	RECT rect;
	int32_t i;

	rect.x = -90;
	rect.y = -70;
	rect.w = 180;
	rect.h = 128;
	MAIN_D_80134F59 = 0;
	MAIN_D_80134F5A = 0;
	MAIN_D_80134F5B = 0;
	MAIN_D_80134F5C = 0;
	createStaticUIBox(1, 1, 0, &rect, VS__func_800FA088,
			  VS__func_800FA5CC);

	fadeFromBlack(5);

	for (i = 0; i < 6; ++i) {
		VS_tickFrame();
	}

	while (MAIN_D_80134F5B == 0) {
		VS_tickFrame();
	}

	fadeToBlack(5);

	for (i = 0; i < 6; ++i) {
		VS_tickFrame();
	}

	VS_D_800716B2[0] = MAIN_D_80134F59;
	removeStaticUIBox(1);

	if (MAIN_D_80134F5B == 1) {
		return 1;
	}

	return 0;
}

void VS__func_800FA5CC(int32_t id)
{
	POLY_FT4 *prim;
	VsUISprite *sprite;
	int32_t i;
	int32_t posX;

	prim = (POLY_FT4 *)GsGetWorkBase();

	if (id == 0) {
		sprite = MAIN_D_8012F650;
	} else {
		sprite = MAIN_D_8012F690;
	}

	for (i = 0; i < 8; ++i) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->clut = GetClut(48, sprite->clut);
		setUVDataPolyFT4(prim, sprite->u, sprite->v, sprite->w,
				 sprite->h);

		if (i == 1 && MAIN_D_80134F59 != 0) {
			setUVDataPolyFT4(prim, sprite->u + 40, sprite->v,
					 sprite->w, sprite->h);
			prim->clut = GetClut(48, 0x1f3);
		}

		if (i == 2 && MAIN_D_80134F59 != 1) {
			setUVDataPolyFT4(prim, sprite->u + 40, sprite->v,
					 sprite->w, sprite->h);
			prim->clut = GetClut(48, 0x1f3);
		}

		if (i == 3 && MAIN_D_80134F59 != 2) {
			setUVDataPolyFT4(prim, sprite->u + 40, sprite->v,
					 sprite->w, sprite->h);
			prim->clut = GetClut(48, 0x1f3);
		}

		if (i >= 4 && i < 7 && MAIN_D_80134F59 != i - 4) {
			prim->clut = GetClut(48, 0x1ec);
		}

		prim->tpage = 7;
		if (i != 7) {
			posX = sprite->x - 90;
		} else {
			posX = sprite->x - 90 + MAIN_D_80134F59 * 56;
		}

		setPosDataPolyFT4(prim, posX, sprite->y - 70, sprite->w,
				  sprite->h);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 6 - id, prim++);
		++sprite;
	}

	GsSetWorkBase((PACKET *)prim);
}
