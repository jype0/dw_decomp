#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/butterfly.h>
#include <dw/entity.h>
#include <dw/fade.h>
#include <dw/main.h>
#include <dw/map.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/pstat.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/utils.h>

#include "common.h"

extern int8_t GAME_STATE;
extern int8_t TAMER_STATE;
extern int8_t TAMER_SUBSTATE;
extern int32_t TRAINING_COMPLETE;
extern int32_t HAS_BUTTERFLY;
extern int32_t BUTTERFLY_ID;
extern int32_t HAS_LEVELS_AWARD_PENDING;
extern int32_t HAS_MEDAL_AWARD_PENDING;
extern int32_t SOME_SCRIPT_SYNC_BIT;
extern uint8_t TEXTBOX_OPEN_TIMER;
extern int8_t INTERACTED_CHEST_STATE;
extern uint8_t TARGET_MAP;
extern uint8_t CURRENT_EXIT;
extern VECTOR STORED_TAMER_POS;
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t MAIN_D_80185BB0[3];
extern int32_t MAIN_D_80185BB4[3];
extern int32_t MAIN_D_80185BBC[3];

extern int32_t PLAYER_SHADOW_ENABLED;
extern uint8_t PICKUP_ITEM_TYPE;
extern uint8_t RECEIVED_ITEM_TYPE;
extern int8_t INTERACTED_CHEST;
extern int8_t LEVELS_INCREASED;
extern int8_t TAKE_ITEM_FRAME_COUNT;
extern int8_t TALKED_TO_ENTITY;
extern int8_t PREVIOUS_CAMERA_POS_INITIALIZED;
extern VECTOR PREVIOUS_CAMERA_POS;
extern int32_t IS_STANDING_ON_DROP;
extern int32_t IS_IN_MENU;
extern int16_t MOVE_TO_DELTA_X;
extern int16_t MAIN_D_80134C9A;
extern uint8_t MAIN_D_80134DF9;
extern int16_t MAIN_D_801386A4[16];
extern uint32_t POLLED_INPUT;
extern uint32_t CHANGED_INPUT;
extern int8_t IS_SCRIPT_PAUSED;
extern int8_t ITEM_SCOLD_FLAG;
extern int8_t FADE_PROTECTION_FULL;
extern int16_t CURRENT_SCRIPT_ID;
extern uint8_t CURRENT_SCREEN;
extern uint8_t ACTIVE_BGM_FONT;
extern int32_t ACTIVE_FRAMEBUFFER;
extern void *ACTIVE_ORDERING_TABLE;
extern uint8_t SKIP_DAYTIME_TRANSITION;
extern uint8_t HAS_ROTATION_DATA[8];
extern uint8_t UNKNOWN_TAMER_DATA[10];
extern uint16_t CURRENT_FRAME;
extern int16_t MINUTE;
extern int16_t HOUR;
extern int16_t DAY;
extern uint8_t YEAR;
extern int32_t MONEY;
extern int8_t NPC_IS_WALKING_TOWARDS[8];
extern uint8_t MAIN_D_801341FC[6];
extern uint8_t MAIN_D_80122D80[];
extern uint8_t MAIN_D_80122D68[];
extern uint8_t MAIN_D_80122D94[];
extern uint8_t MAIN_D_80122DAC[];
extern uint8_t MAIN_D_80122DC8[];
extern uint8_t MAIN_D_80122DDC[];
extern uint8_t MAIN_D_80122DF4[];
extern RECT MAIN_D_801341F4;
extern RECT MAIN_D_80134204;
extern RECT MAIN_D_8013420C;
extern int32_t MAIN_D_801386A0[16];

typedef struct {
	SVECTOR spriteLocation;
	int32_t type;
} WorldItem;

typedef struct {
	WorldItem worldItem;
	int16_t tileX;
	int16_t tileY;
} DroppedItem;

extern DroppedItem DROPPED_ITEMS[11];

typedef struct {
	VECTOR location;
	VECTOR trayLocation;
	SVECTOR rotation;
	SVECTOR trayRotation;
	uint16_t trigger;
	uint8_t item;
	uint8_t isTaken;
	uint8_t tileX;
	uint8_t tileY;
	uint16_t padding;
} Chest;

extern Chest CHEST_ARRAY[8];

typedef struct {
	int16_t spawnX[10];
	int16_t spawnY[10];
	int16_t spawnZ[10];
	int16_t rotation[10];
	uint16_t targetMap[10];
	uint16_t targetExit[10];
} MapWarps;

extern MapWarps MAP_WARPS;

typedef struct {
        int8_t hungerTimes[8];
        int8_t energyCap;
        uint8_t energyThreshold;
        int8_t energyUsage;
        int16_t poopTimer;
        int16_t unk2;
        int8_t poopSize;
        int8_t favoriteFood;
        int8_t sleepCycle;
        int8_t favoredRegion;
        int8_t trainingType;
        int8_t defaultWeight;
        int16_t viewX;
        int16_t viewY;
        int16_t viewZ;
} RaiseData;

extern RaiseData RAISE_DATA[66];

typedef struct
{
	char name[20];
	int32_t value;
	int16_t meritValue;
	int16_t sortingValue;
	uint8_t itemColor;
	uint8_t droppable;
	uint16_t unk;
} Item;

void startAnimation(Entity *entity, int32_t animId);
void tickAnimation(Entity *entity);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
void setEntityPosition(int32_t entityId, int32_t x, int32_t y, int32_t z);
void setEntityRotation(int32_t entityId, int32_t rx, int32_t ry, int32_t rz);
void setupEntityMatrix(int32_t entityId);
void stopGameTime(void);
void startGameTime(void);
void fadeToBlack(int32_t frames);
void fadeFromBlack(int32_t frames);
void changeMap(uint8_t mapId, uint8_t exitId);
void addMapNameObject(uint8_t mapId);
int32_t removeObject(int32_t objectId, int32_t instanceId);
void checkMapInteraction(void);
void renderString(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t,
                  int32_t, int32_t, int32_t);
void renderUIBox(int32_t id);
void setMapLayerEnabled(int32_t enabled);
int32_t isSoundLoaded(int32_t isAsync, int32_t soundId);
void thunkLoadMMD(int32_t id, int32_t type);
void initializeDigimonObject(int32_t id, int32_t type, TickFunction tick);
void drawString(char *text, int32_t color, int32_t pos);
void clearTextArea(void);
void clearTextSubArea(RECT *area);
void setTextColor(int32_t color);
void playSound(int32_t vabId, uint32_t note);
int32_t giveItem(int32_t type, int32_t addToInv);
void setCameraFollowPlayer(void);
void unsetCameraFollowPlayer(void);
void pickupItem(int32_t itemType);
int16_t entityCheckCollision(Entity *a, Entity *b, int32_t c, int32_t d);
void collisionGrace(int32_t a, Entity *b, int32_t c, int32_t d);
void setTrigger(uint16_t trigger);
int32_t tickOpenChestTray(int32_t chestId);
int32_t tickCloseChestTray(int32_t chestId);
int32_t hasMove(int32_t moveId);
int32_t hasMedal(uint16_t medal);
int32_t hasDigimonRaised(int32_t digimonId);
int32_t getCardAmount(uint8_t cardId);
void unlockMedal(uint16_t medal);
void closeTriangleMenu(void);
void removeTriangleMenu(void);
void closeInventoryBoxes(void);
void removeUIBox1(void);
void callScriptSection(int32_t, int32_t, int32_t);
void addGameMenu(void);
void setPartnerIdling(void);
int32_t getTileTrigger(VECTOR *pos);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
int32_t createPoopPile(int32_t tileX, int32_t tileY);
void moveCameraByDiff(VECTOR *prev, VECTOR *cur);
void getRotationDifference(PositionData *data, VECTOR *target,
                           int16_t *outX, int16_t *outY, int16_t *outZ);
int32_t rotateEntity(SVECTOR *rot, int16_t *outX, int16_t *outY,
                     int16_t *outZ, int32_t speed);
void tickTamerWaypoints(void);
int32_t addObject(int32_t id, int32_t instance, TickFunction tick,
                  RenderFunction render);
void loadMapDigimon(uint8_t *data, int32_t a);
void tickTamerBattle(int32_t instanceId);
void tickConditionBoundaries(void);
void handlePostBattleTiredness(void);
int32_t getEntityScreenPos(Entity *entity, int32_t flag, int16_t *outPos);
int32_t isUIBoxAvailable(int32_t id);
void playBGM(int16_t bgmId);
void readMapTFS(int32_t mapId);
void updateMinuteHand(int32_t hour, int32_t minute);
void initializeDaytimeTransition(int32_t mode);
int32_t handleBattleStart(int32_t instanceId);
void loadBattleData(int32_t a, int32_t b);
int32_t BTL_battleMain(void);
int32_t isKeyDown(uint32_t key);
void memcpy(uint8_t *dst, uint8_t *src, uint32_t size);
void setupPartnerOnWarp(int16_t x, int16_t y, int16_t z, uint32_t rotationY);
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern GsOT GS_ORDERING_TABLE[2];
extern uint8_t GS_WORK_BASES[2][81920];
extern MATRIX GsWSMATRIX;
extern Item ITEM_PARA[128];
extern int32_t FADE_PROTECTION;
int32_t ENDI_tickEnding(Entity *entity, int32_t isInitialized);
int32_t MURD_tick(Entity *entity, int32_t isInitialized);
int32_t EAB_tick(Entity *entity, int32_t isInitialized);
int32_t DOOA_tick(PartnerEntity *partner, void *data, int32_t isInitialized);

void tickTamerBattle(int32_t instanceId);
void STD_tickTamerTournament(int32_t instanceId);
void tickTamerOverworld(int16_t instanceId);
void tickWalkingState(void);
void tickChangeMap(void);
void tickPickupItem(void);
void tickTakeChest(void);
void tickIdle(void);
void tickTraining(void);
void tickPraiseScold(int8_t state, int8_t unused);
void tickFishing(void);
void KAR_tick(void);
void tickOpening(void);
void tickEnding(void);
void tickSicknessLostLife(void);
void tickMachinedramonSpawn(void);
void tickBattleLostLife(void);
void tickAwardSomething(void);

void initializeTamer(int32_t id, int32_t x, int32_t y, int32_t z,
                     int32_t rx, int32_t ry, int32_t rz);
void tickTamer(int16_t instanceId);
void renderItemPickupTextbox(int32_t instanceId);
void renderAwardSomethingTextbox(int32_t instanceId);
void setTamerDirection(int32_t direction);
void setupTamerOnWarp(int32_t x, int32_t y, int32_t z, int32_t rotationY);
int32_t tickEntitySetRotation(uint32_t scriptId, int16_t rotationY);
int32_t getMapRotation(void);
Entity *getEntityFromScriptId(uint8_t *scriptId);
int32_t startBattle(int16_t instanceId);
void advanceBattleTime(int32_t result);
void addTamerLevel(int32_t chance, int32_t amount);
void checkItemPickup(void);
void checkMedalConditions(void);
uint8_t checkChestCollision(void);
void setTamerState(int8_t state);
void setFullState(int8_t state, int8_t substate);
int32_t getTamerState(void);
void startAnimationTamer(int32_t animId);
void checkPendingAwards(void);
int32_t isTrainingComplete(void);
int32_t tickEntityWalkTo(uint32_t scriptId1, uint32_t scriptId2,
			 int32_t targetX, int32_t targetZ,
			 int32_t withCamera);
int32_t tickLookAtEntity(uint32_t scriptId1, uint32_t scriptId2);
int32_t tickEntityMoveTo(uint32_t scriptId1, uint32_t scriptId2,
			 int32_t targetX, int32_t targetZ, int32_t speed,
			 int32_t withCamera);
int32_t tickEntityMoveToAxis(uint32_t scriptId, int32_t target, int32_t axis,
			     int32_t speed, int32_t withCamera);
void loadMapEntities(uint8_t *data, int32_t mapId, int32_t warpIdx);
void tickPickupItem(void);
void tickTakeChest(void);
void tickAwardSomething(void);
void tickWalkingState(void);
void worldPosToScreenPos2(int16_t *x, int16_t *y, int16_t *z);
void tickChangeMap(void);
void tickPraiseScold(int8_t state, int8_t unused);
void tickOpening(void);
void tickEnding(void);
void tickSicknessLostLife(void);
void tickMachinedramonSpawn(void);
void tickBattleLostLife(void);
void tickIdle(void);
void tickTraining(void);

void initializeTamer(int32_t id, int32_t x, int32_t y, int32_t z,
                     int32_t rx, int32_t ry, int32_t rz)
{
	int32_t i;

	PLAYER_SHADOW_ENABLED = 1;
	thunkLoadMMD(id, 2);
	ENTITY_TABLE[0] = &TAMER_ENTITY.entity;
	initializeDigimonObject(id, 0, (TickFunction)tickTamer);
	setEntityPosition(0, x, y, z);
	setEntityRotation(0, rx, ry, rz);
	setupEntityMatrix(0);
	STORED_TAMER_POS.vx = x;
	STORED_TAMER_POS.vy = y;
	STORED_TAMER_POS.vz = z;
	startAnimation(ENTITY_TABLE[0], 0);
	GAME_STATE = 0;
	ENTITY_TABLE[0]->isOnMap = 1;
	ENTITY_TABLE[0]->isOnScreen = 1;
	for (i = 0; i < 8; i = i + 1) {
		HAS_ROTATION_DATA[i] = 0;
	}
	for (i = 0; i < 10; i = i + 1) {
		UNKNOWN_TAMER_DATA[i] = 0;
	}
	PREVIOUS_CAMERA_POS_INITIALIZED = 0;
	IS_STANDING_ON_DROP = 0;
	STORED_TAMER_POS = TAMER_ENTITY.entity.posData->location;
	HAS_LEVELS_AWARD_PENDING = 0;
	HAS_MEDAL_AWARD_PENDING = 0;
	IS_IN_MENU = 0;
	LEVELS_INCREASED = 1;
}

void loadMapEntities(uint8_t *data, int32_t mapId, int32_t warpIdx)
{
	uint8_t *mapDigimonData;
	int16_t spawnY;
	int16_t rotation;
	uint32_t rot;
	int16_t x;
	int16_t rotationY;
	int16_t z;

	memcpy((void *)&MAP_WARPS, data, sizeof(MAP_WARPS));

	x = MAP_WARPS.spawnX[warpIdx];
	spawnY = MAP_WARPS.spawnY[warpIdx];
	z = MAP_WARPS.spawnZ[warpIdx];

	rotationY = MAP_WARPS.rotation[warpIdx];
	rotation = rotationY;

	mapDigimonData = &data[sizeof(MAP_WARPS)];

	setupTamerOnWarp(x, (int32_t)spawnY, z, rotationY);

	if ((rotation <= 0x200) || (rotation > 0xe00)) {
		z += 200;
	} else if ((rotation > 0x200) && (rotation <= 0x600)) {
		x += 200;
	} else {
		if ((rotation > 0x600) && (0xa00 >= rotation)) {
			z -= 200;
		} else {
			if ((0xa00 < rotation) && (rotation <= 0xe00)) {
				x -= 200;
			}
		}
	}

	rot = (rotation + 2048) & 0xfff;
	if (((rotation + (long)2048) < 0) && (rot != 0)) {
		rot -= 4096;
	}

	setupPartnerOnWarp(x, spawnY, z, rot);
	loadMapDigimon(mapDigimonData, mapId);
	STORED_TAMER_POS = TAMER_ENTITY.entity.posData->location;
}

void setupTamerOnWarp(int32_t x, int32_t y, int32_t z, int32_t rotationY)
{
	setEntityPosition(0, x, y, z);
	setEntityRotation(0, TAMER_ENTITY.entity.posData->rotation.vx, rotationY,
	                  TAMER_ENTITY.entity.posData->rotation.vz);
	setupEntityMatrix(0);
	startAnimation(ENTITY_TABLE[0], 0);
}

void tickTamerOverworld(int16_t instanceId)
{
	int8_t state;

	switch (state = TAMER_STATE) {
	case 0:
		tickWalkingState();
		break;
	case 1:
		setTamerState(6);
		break;
	case 5:
		tickChangeMap();
		break;
	case 7:
		tickPickupItem();
		break;
	case 14:
		tickTakeChest();
		break;
	case 6:
		tickIdle();
		break;
	case 8:
		tickTraining();
		break;
	case 9:
	case 13:
		tickPraiseScold(state, state);
		break;
	case 11:
		tickFishing();
		break;
	case 12:
		KAR_tick();
		break;
	case 15:
		tickOpening();
		break;
	case 16:
		tickEnding();
		break;
	case 17:
		tickSicknessLostLife();
		break;
	case 18:
		tickMachinedramonSpawn();
		break;
	case 19:
		tickBattleLostLife();
		break;
	case 20:
		tickAwardSomething();
	default:
		break;
	}

	tickAnimation(&TAMER_ENTITY.entity);
}

void tickTamer(int16_t instanceId)
{
	if (((GAME_STATE != 0) || (TAMER_STATE != 0)) && (HAS_BUTTERFLY == 0)) {
		unsetButterfly(BUTTERFLY_ID);
		HAS_BUTTERFLY = -1;
	}
	switch (GAME_STATE) {
	case 0:
		tickTamerOverworld(instanceId);
		break;
	case 1:
	case 2:
	case 3:
		tickTamerBattle(instanceId);
		break;
	case 4:
	case 5:
		STD_tickTamerTournament(instanceId);
	default:
		break;
	}
}

int32_t getMapRotation(void)
{
	int16_t dz;

	dz = (int16_t)(MAIN_D_80185BB0[0] - MAIN_D_80185BBC[0]);
	return (int16_t)_atan(dz,
	                      (int16_t)(GS_VIEWPOINT.vpx - MAIN_D_80185BB4[0]));
}

void setTamerDirection(int32_t direction)
{
	if (0xfff < direction) {
		direction = (direction + -0x1000) * 0x10000 >> 0x10;
	}
	TAMER_ENTITY.entity.posData->rotation.vy = (int16_t)direction;
}

INCLUDE_ASM("asm/main/nonmatchings/tamer", checkItemPickup);

INCLUDE_ASM("asm/main/nonmatchings/tamer", checkMapInteraction);

INCLUDE_ASM("asm/main/nonmatchings/tamer", checkMedalConditions);

void checkPendingAwards(void)
{
	if ((HAS_LEVELS_AWARD_PENDING == 1) || (HAS_MEDAL_AWARD_PENDING == 1)) {
		setTamerState(0x14);
		stopGameTime();
	}
}

void renderItemPickupTextbox(int32_t instanceId)
{
	if (INTERACTED_CHEST_STATE == 0) {
		renderString(0, 0xffffff83, 0x39, 0x5a, 0xc, 0, 0x24, 5, 0);
		renderString(0xf, 0xffffff83, 0x2d, 0x60, 0xc, 0, 0x18, 5, 0);
	} else if (INTERACTED_CHEST_STATE == 1) {
		renderString(0, 0xffffff83, 0x39, 0xf0, 0xc, 0, 0x18, 5, 0);
	} else {
		renderString(0, 0xffffff83, 0x39, 0x90, 0xc, 0, 0x18, 5, 0);
	}
	renderUIBox(1);
	++TEXTBOX_OPEN_TIMER;
}

int32_t startBattle(int16_t instanceId)
{
	int8_t battleResult;
	int32_t result;
	int32_t battleData;

	GAME_STATE = 1;
	STORED_TAMER_POS.vx = TAMER_ENTITY.entity.posData->location.vx;
	STORED_TAMER_POS.vy = TAMER_ENTITY.entity.posData->location.vy;
	STORED_TAMER_POS.vz = TAMER_ENTITY.entity.posData->location.vz;
	unsetCameraFollowPlayer();
	closeTriangleMenu();
	stopGameTime();
	battleData = handleBattleStart(instanceId);
	loadBattleData(instanceId, battleData);
	battleResult = BTL_battleMain();
	result = battleResult;
	GAME_STATE = 0;
	if (result == -1) {
		PARTNER_PARA.happiness = PARTNER_PARA.happiness - 0x1e;
		PARTNER_PARA.discipline = PARTNER_PARA.discipline - 0x14;
		setPartnerState(-1);
		SKIP_DAYTIME_TRANSITION = 1;
	} else if (result == 0) {
		PARTNER_PARA.happiness = PARTNER_PARA.happiness - 10;
		PARTNER_PARA.discipline = PARTNER_PARA.discipline - 6;
		PARTNER_PARA.tiredness = PARTNER_PARA.tiredness + 2;
		handlePostBattleTiredness();
		SKIP_DAYTIME_TRANSITION = 1;
	} else if (result == 1) {
		playBGM(ACTIVE_BGM_FONT);
		readMapTFS(CURRENT_SCREEN);
		STORED_TAMER_POS = TAMER_ENTITY.entity.posData->location;
		setPartnerState(1);
		PARTNER_PARA.happiness = PARTNER_PARA.happiness + 2;
		handlePostBattleTiredness();
		SKIP_DAYTIME_TRANSITION = 0;
		PARTNER_PARA.battles = PARTNER_PARA.battles + 1;
	}
	tickConditionBoundaries();
	ACTIVE_FRAMEBUFFER = GsGetActiveBuff();
	GsSetWorkBase((PACKET *)GS_WORK_BASES[ACTIVE_FRAMEBUFFER]);
	GsClearOt(0, 0, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER);
	ACTIVE_ORDERING_TABLE = GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER;
	advanceBattleTime(result);

	return result;
}

void advanceBattleTime(int32_t result)
{
	MINUTE += 20;

	CURRENT_FRAME += 400;
	if (MINUTE > 59) {
		++HOUR;
		MINUTE -= 60;

		--PARTNER_PARA.remainingLifetime;
		if (PARTNER_PARA.remainingLifetime < 0) {
			PARTNER_PARA.remainingLifetime = 0;
		}

		if ((PARTNER_PARA.condition & 1) != 0) {
			++PARTNER_PARA.sicknessCounter;
			++PARTNER_PARA.missedSleepHours;
		}

		if ((PARTNER_PARA.condition & 0x40) != 0) {
			++PARTNER_PARA.sicknessTries;
		}

		if ((PARTNER_PARA.condition & 0x20) != 0) {
			++PARTNER_PARA.injuryTimer;
		}

		if ((PARTNER_PARA.condition & 0x40) != 0) {
			++PARTNER_PARA.sicknessTimer;
		}

		if (0x17 < HOUR) {
			++DAY;
			CURRENT_FRAME = MINUTE * 20;
			HOUR = 0;
			if (0x1d < DAY) {
				++YEAR;
				DAY = 0;
				if (YEAR > 99) {
					YEAR = 0;
				}
			}
		}
	}

	if ((PARTNER_PARA.condition & 4) != 0) {
		PARTNER_PARA.starvationTimer -= 40;
		if ((PARTNER_PARA.starvationTimer < 1) &&
		    (PARTNER_PARA.energyLevel <
		     RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyThreshold)) {
			++PARTNER_PARA.careMistakes;
		}
	} else {
		PARTNER_PARA.foodLevel -= 20;
	}

	if ((PARTNER_PARA.condition & 8) != 0) {
		PARTNER_PARA.poopingTimer -= 400;
	} else {
		PARTNER_PARA.poopLevel -= 2;
	}

	updateMinuteHand(HOUR, MINUTE);

	if (result == 1) {
		int8_t flags;

		flags = MAP_ENTRIES[CURRENT_SCREEN].flags & 0x40;
		if (flags == 0) {
			if (HOUR == 0x10) {
				initializeDaytimeTransition(0);
			} else if (HOUR == 0x14) {
				initializeDaytimeTransition(1);
			} else if (HOUR == 6) {
				initializeDaytimeTransition(2);
			}
		}
	}
}

void setFullState(int8_t state, int8_t substate)
{
	TAMER_STATE = state;
	TAMER_SUBSTATE = substate;
}

int32_t getTamerState(void)
{
	return TAMER_STATE;
}

void startAnimationTamer(int32_t animId)
{
	startAnimation(&TAMER_ENTITY.entity, animId);
}

INCLUDE_ASM("asm/main/nonmatchings/tamer", tickEntityWalkTo);

INCLUDE_ASM("asm/main/nonmatchings/tamer", getEntityFromScriptId);

INCLUDE_ASM("asm/main/nonmatchings/tamer", tickLookAtEntity);

int32_t tickEntitySetRotation(uint32_t scriptId, int16_t rotationY)
{
	Entity *entity;

	entity = getEntityFromScriptId((uint8_t *)&scriptId);
	entity->posData->rotation.vy = rotationY;
	return 1;
}

INCLUDE_ASM("asm/main/nonmatchings/tamer", tickEntityMoveTo);

INCLUDE_ASM("asm/main/nonmatchings/tamer", tickEntityMoveToAxis);

INCLUDE_ASM("asm/main/nonmatchings/tamer", worldPosToScreenPos2);

uint8_t checkChestCollision(void)
{
	int32_t i;
	int16_t tileX;
	int16_t tileY;

	getModelTile(&TAMER_ENTITY.entity.posData->location, &tileX, &tileY);

	for (i = 0; i < 8; ++i) {
		if ((CHEST_ARRAY[i].item != 0xff) &&
		    (CHEST_ARRAY[i].tileX - 1) <= tileX &&
		    (tileX <= CHEST_ARRAY[i].tileX + 1) &&
		    (CHEST_ARRAY[i].tileY - 1) <= tileY &&
		    (tileY <= CHEST_ARRAY[i].tileY + 1)) {
			return i;
		}
	}

	return 0xff;
}

void addTamerLevel(int32_t chance, int32_t amount)
{
	int32_t r;

	r = random(100);
	if (r < chance) {
		TAMER_ENTITY.tamerLevel = TAMER_ENTITY.tamerLevel + amount;
		if ((0 <= TAMER_ENTITY.tamerLevel) &&
		    (TAMER_ENTITY.tamerLevel < 11)) {
			clearTextArea();

			if (amount >= 1) {
				LEVELS_INCREASED = 1;
			} else {
				LEVELS_INCREASED = -1;
			}

			HAS_LEVELS_AWARD_PENDING = 1;
		}

		if (TAMER_ENTITY.tamerLevel < 0) {
			TAMER_ENTITY.tamerLevel = 0;
		}

		if (TAMER_ENTITY.tamerLevel > 10) {
			TAMER_ENTITY.tamerLevel = 10;
		}
	}
}

int32_t isTrainingComplete(void)
{
	if (TRAINING_COMPLETE == 1) {
		TRAINING_COMPLETE = 0;
		return 1;
	}
	return 0;
}

void renderAwardSomethingTextbox(int32_t instanceId)
{
	renderString(0, 0xffffff83, 0x2d, 0xf0, 0x24, 0, 0x78, 5, 0);
	renderUIBox(1);
	++TEXTBOX_OPEN_TIMER;
}

INCLUDE_ASM("asm/main/nonmatchings/tamer", tickWalkingState);

void setTamerState(int8_t state)
{
	TAMER_STATE = state;
	TAMER_SUBSTATE = 0;
}

void tickChangeMap(void)
{
	switch (TAMER_SUBSTATE) {
	case 0:
		fadeToBlack(0x14);
		startAnimationTamer(2);
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		if (FADE_OUT_CURRENT == 10) {
			addMapNameObject(TARGET_MAP);
		}
		if (0x13 < FADE_OUT_CURRENT) {
			changeMap(TARGET_MAP, CURRENT_EXIT);
			STORED_TAMER_POS = TAMER_ENTITY.entity.posData->location;
			fadeFromBlack(0x14);
			removeObject(0xfa1, TARGET_MAP);
			TAMER_SUBSTATE = 2;
		}
		break;
	case 2:
		if (0x13 < FADE_IN_CURRENT) {
			setTamerState(0);
			setPartnerState(1);
			checkMapInteraction();
			STORED_TAMER_POS = TAMER_ENTITY.entity.posData->location;
			startGameTime();
		}
	default:
		break;
	}
}

void tickPickupItem(void)
{
	int32_t available;
	RECT textRect;
	RECT targetRect;
	RECT sourceRect;
	int16_t screenPos[2];

	textRect = MAIN_D_801341F4;
	switch (TAMER_SUBSTATE) {
	case 0:
		startAnimation(ENTITY_TABLE[0], 0xc);
		unsetCameraFollowPlayer();
		clearTextSubArea(&textRect);
		drawString(DIGIMON_DATA[0].name, 0, 0xc);
		drawString(ITEM_PARA[DROPPED_ITEMS[PICKUP_ITEM_TYPE].worldItem.type].name,
		           0, 0x18);
		drawString((char *)MAIN_D_801341FC, 0, 0x24);
		INTERACTED_CHEST_STATE = 0;
		TAKE_ITEM_FRAME_COUNT = 0;
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		available = isUIBoxAvailable(1);
		if (available == 1) {
			targetRect.x = -0x82;
			targetRect.y = 0x2a;
			targetRect.w = 0x106;
			targetRect.h = 0x3b;
			getEntityScreenPos(ENTITY_TABLE[0], 1, screenPos);
			sourceRect.x = screenPos[0] - 5;
			sourceRect.y = screenPos[1] - 5;
			sourceRect.w = 10;
			sourceRect.h = 10;
			RECEIVED_ITEM_TYPE =
				DROPPED_ITEMS[PICKUP_ITEM_TYPE].worldItem.type;
			createAnimatedUIBox(1, 0, 2, &targetRect, &sourceRect, 0,
			                    renderItemPickupTextbox);
			TAMER_SUBSTATE = 2;
		}
		break;
	case 2:
		TAKE_ITEM_FRAME_COUNT = TAKE_ITEM_FRAME_COUNT + 1;
		available = isKeyDown(0x40);
		if ((available != 0) && (4 < TAKE_ITEM_FRAME_COUNT)) {
			if (TAKE_ITEM_FRAME_COUNT < 0x3c) {
				playSound(0, 3);
			}
			available = giveItem(DROPPED_ITEMS[PICKUP_ITEM_TYPE].worldItem.type & 0xff, 0);
			if (available == 0) {
				drawString((char *)MAIN_D_80122D68, 0, 0x18);
				INTERACTED_CHEST_STATE = 1;
				TAKE_ITEM_FRAME_COUNT = 0;
				TAMER_SUBSTATE = 3;
			} else {
				playSound(0, 7);
				TAMER_SUBSTATE = 4;
			}
		}
		break;
	case 3:
		TAKE_ITEM_FRAME_COUNT = TAKE_ITEM_FRAME_COUNT + 1;
		available = isKeyDown(0x40);
		if ((available != 0) && (4 < TAKE_ITEM_FRAME_COUNT)) {
			if (TAKE_ITEM_FRAME_COUNT < 0x3c) {
				playSound(0, 3);
			}
			TAMER_SUBSTATE = 4;
		}
		break;
	case 4:
		getEntityScreenPos(ENTITY_TABLE[0], 1, screenPos);
		textRect.x = screenPos[0] - 5;
		textRect.y = screenPos[1] - 5;
		textRect.w = 10;
		textRect.h = 10;
		removeAnimatedUIBox(1, &textRect);
		if (INTERACTED_CHEST_STATE == 0) {
			pickupItem(PICKUP_ITEM_TYPE);
		}
		setTamerState(0);
		setCameraFollowPlayer();
	default:
		break;
	}
	if (9 < TAKE_ITEM_FRAME_COUNT) {
		TAKE_ITEM_FRAME_COUNT = 10;
	}
}

void tickTakeChest(void)
{
	int32_t available;
	int32_t trayResult;
	RECT textRect;
	RECT targetRect;
	RECT sourceRect;
	int16_t screenPos[2];

	textRect = MAIN_D_80134204;
	switch (TAMER_SUBSTATE) {
	case 0:
		startAnimation(ENTITY_TABLE[0], 0);
		setPartnerState(0xb);
		unsetCameraFollowPlayer();
		entityLookAtLocation(ENTITY_TABLE[0],
				     &CHEST_ARRAY[INTERACTED_CHEST].location);
		clearTextSubArea(&textRect);
		drawString(DIGIMON_DATA[0].name, 0, 0xc);
		if (CHEST_ARRAY[INTERACTED_CHEST].isTaken == DW_FALSE) {
			drawString(ITEM_PARA[CHEST_ARRAY[INTERACTED_CHEST].item].name,
			           0, 0x18);
			drawString((char *)MAIN_D_801341FC, 0, 0x24);
			INTERACTED_CHEST_STATE = 0;
		} else {
			drawString((char *)MAIN_D_80122D80, 0, 0x18);
			INTERACTED_CHEST_STATE = 2;
		}
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		available = isUIBoxAvailable(1);
		if (available == 1) {
			trayResult = tickOpenChestTray(INTERACTED_CHEST);
			if (trayResult == 1) {
				targetRect.x = -0x82;
				targetRect.y = 0x2a;
				targetRect.w = 0x106;
				targetRect.h = 0x3b;
				getEntityScreenPos(ENTITY_TABLE[0], 1,
						   screenPos);
				sourceRect.x = screenPos[0] - 5;
				sourceRect.y = screenPos[1] - 5;
				sourceRect.w = 10;
				sourceRect.h = 10;
				RECEIVED_ITEM_TYPE =
					CHEST_ARRAY[INTERACTED_CHEST].item;
				createAnimatedUIBox(1, 0, 2, &targetRect,
						    &sourceRect, 0,
				                    renderItemPickupTextbox);
				TAKE_ITEM_FRAME_COUNT = 0;
				if (INTERACTED_CHEST_STATE == 0) {
					TAMER_SUBSTATE = 2;
				} else {
					TAMER_SUBSTATE = 4;
				}
			}
		}
		break;
	case 2:
		TAKE_ITEM_FRAME_COUNT = TAKE_ITEM_FRAME_COUNT + 1;
		if (((POLLED_INPUT & PADRdown) != 0) &&
		    (5 < TAKE_ITEM_FRAME_COUNT)) {
			TAKE_ITEM_FRAME_COUNT = 0;
			available = giveItem(RECEIVED_ITEM_TYPE, 0);
			if (available == 0) {
				drawString((char *)MAIN_D_80122D68, 0, 0x18);
				INTERACTED_CHEST_STATE = 1;
				TAMER_SUBSTATE = 3;
			} else {
				setTrigger(CHEST_ARRAY[INTERACTED_CHEST].trigger);
				TAMER_SUBSTATE = 4;
			}
		}
		break;
	case 3:
		available = tickCloseChestTray(INTERACTED_CHEST);
		if (available == 1) {
			TAMER_SUBSTATE = 4;
		}
		break;
	case 4:
		TAKE_ITEM_FRAME_COUNT = TAKE_ITEM_FRAME_COUNT + 1;
		if (((POLLED_INPUT & PADRdown) != 0) &&
		    (5 < TAKE_ITEM_FRAME_COUNT)) {
			getEntityScreenPos(&TAMER_ENTITY.entity, 1,
					   screenPos);
			textRect.x = screenPos[0] - 5;
			textRect.y = screenPos[1] - 5;
			textRect.w = 10;
			textRect.h = 10;
			removeAnimatedUIBox(1, &textRect);
			if (INTERACTED_CHEST_STATE == 0) {
				giveItem(RECEIVED_ITEM_TYPE, 1);
			}
			setTamerState(0);
			setPartnerState(1);
			setCameraFollowPlayer();
		}
	default:
		break;
	}
	if (9 < TAKE_ITEM_FRAME_COUNT) {
		TAKE_ITEM_FRAME_COUNT = 10;
	}
}

void tickIdle(void)
{
	if (TAMER_SUBSTATE == 0) {
		startAnimation(&TAMER_ENTITY.entity, 0);
		TAMER_SUBSTATE = 1;
	}
}

void tickTraining(void)
{
	switch (TAMER_SUBSTATE) {
	case 0:
		startAnimation(&TAMER_ENTITY.entity, 10);
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		entityLookAtLocation(&TAMER_ENTITY.entity,
		                     &PARTNER_ENTITY.digimonEntity.entity.posData->location);
	default:
		break;
	}
}

void tickPraiseScold(int8_t state, int8_t unused)
{
	int32_t type;

	switch (TAMER_SUBSTATE) {
	case 0:
		if (state == 0xd) {
			type = PARTNER_ENTITY.digimonEntity.entity.type;
			if ((DIGIMON_DATA[type].level < 4) ||
			    (type == 0xd) ||
			    (type == 0x1b) ||
			    (type == 0x38)) {
				startAnimation(&TAMER_ENTITY.entity, 0x26);
			} else {
				startAnimation(&TAMER_ENTITY.entity, 0x27);
			}
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
				       0xb);

			TAMER_SUBSTATE = 1;
		} else {
			playSound(0, 0xd);
			startAnimation(&TAMER_ENTITY.entity, 7);
			if (ITEM_SCOLD_FLAG == 1) {
				startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
					       0x19);
			} else {
				startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
					       0xc);
			}
		}
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		if (TAMER_ENTITY.entity.anim.animFrame >=
		    TAMER_ENTITY.entity.anim.frameCount) {
			setTamerState(6);
		}
	default:
		break;
	}
}

void tickOpening(void)
{
	switch (TAMER_SUBSTATE) {
	case 0:
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		setMapLayerEnabled(1);
		SOME_SCRIPT_SYNC_BIT = 1;
	default:
		break;
	}
}

void tickEnding(void)
{
	int32_t value;

	switch (TAMER_SUBSTATE) {
	case 0:
		isSoundLoaded(0, 8);
		ENDI_tickEnding(ENTITY_TABLE[0], 0);
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		value = ENDI_tickEnding(ENTITY_TABLE[0], 1);
		if (0 > value) {
			SOME_SCRIPT_SYNC_BIT = 1;
		}
	default:
		break;
	}
}

void tickSicknessLostLife(void)
{
	int32_t value;

	switch (TAMER_SUBSTATE) {
	case 0:
		loadDynamicLibrary(MURD_REL, 0, 0, 0, 0);
		MURD_tick(ENTITY_TABLE[1], 0);
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		value = MURD_tick(ENTITY_TABLE[1], 1);
		if ((0 > value) &&
		    (value = DOOA_tick((PartnerEntity *)ENTITY_TABLE[1],
		                       GENERAL_BUFFER_PTR + 0x4b000, 1),
		     0 > value)) {
			SOME_SCRIPT_SYNC_BIT = 1;
		}
	default:
		break;
	}
}

void tickMachinedramonSpawn(void)
{
	int32_t value;

	switch (TAMER_SUBSTATE) {
	case 0:
		EAB_tick(ENTITY_TABLE[2], 0);
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		value = EAB_tick(ENTITY_TABLE[2], 1);
		if (0 > value) {
			SOME_SCRIPT_SYNC_BIT = 1;
		}
	default:
		break;
	}
}

void tickBattleLostLife(void)
{
	int32_t value;

	switch (TAMER_SUBSTATE) {
	case 0:
		loadDynamicLibrary(MURD_REL, 0, 0, 0, 0);
		MURD_tick(ENTITY_TABLE[1], 0);
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		value = MURD_tick(ENTITY_TABLE[1], 1);
		if (0 > value) {
			SOME_SCRIPT_SYNC_BIT = 1;
		}
	default:
		break;
	}
}

void tickAwardSomething(void)
{
	int32_t available;
	RECT textRect;
	RECT targetRect;
	RECT sourceRect;
	int16_t screenPos[2];

	textRect = MAIN_D_8013420C;
	switch (TAMER_SUBSTATE) {
	case 0:
		stopGameTime();
		startAnimation(&TAMER_ENTITY.entity, 0);
		setPartnerState(0xb);
		unsetCameraFollowPlayer();
		if (HAS_MEDAL_AWARD_PENDING == 1) {
			TAMER_SUBSTATE = 1;
		} else if (HAS_LEVELS_AWARD_PENDING == 1) {
			clearTextArea();
			if (LEVELS_INCREASED == 1) {
				setTextColor(7);
				drawString((char *)MAIN_D_80122D94, 0, 0x78);
			} else {
				setTextColor(3);
				drawString((char *)MAIN_D_80122DAC, 0, 0x78);
			}
			setTextColor(1);
			TAMER_SUBSTATE = 4;
		} else {
			TAMER_SUBSTATE = 4;
		}
		break;
	case 1:
		clearTextArea();
		setTextColor(7);
		drawString((char *)MAIN_D_80122DC8, 0, 0x78);
		TAMER_SUBSTATE = 2;
		break;
	case 2:
		drawString((char *)MAIN_D_80122DDC, 0, 0x84);
		TAMER_SUBSTATE = 3;
		break;
	case 3:
		drawString((char *)MAIN_D_80122DF4, 0, 0x90);
		setTextColor(1);
		TAMER_SUBSTATE = 4;
		break;
	case 4:
		available = isUIBoxAvailable(1);
		if (available == 1) {
			targetRect.x = -0x82;
			targetRect.y = 0x2a;
			targetRect.w = 0x106;
			targetRect.h = 0x3b;
			getEntityScreenPos(ENTITY_TABLE[0], 1, screenPos);
			sourceRect.x = screenPos[0] - 5;
			sourceRect.y = screenPos[1] - 5;
			sourceRect.w = 10;
			sourceRect.h = 10;
			RECEIVED_ITEM_TYPE =
				CHEST_ARRAY[INTERACTED_CHEST].item;
			createAnimatedUIBox(1, 0, 2, &targetRect, &sourceRect,
					    0, renderAwardSomethingTextbox);
			TAMER_SUBSTATE = 5;
			playSound(0, 7);
		}
		break;
	case 5:
		if ((POLLED_INPUT & PADRdown) != 0) {
			TAKE_ITEM_FRAME_COUNT = 0;
			getEntityScreenPos(&TAMER_ENTITY.entity, 1,
					   screenPos);
			textRect.x = screenPos[0] - 5;
			textRect.y = screenPos[1] - 5;
			textRect.w = 10;
			textRect.h = 10;
			removeAnimatedUIBox(1, &textRect);
			setTamerState(0);
			setPartnerState(1);
			setCameraFollowPlayer();
			startGameTime();
			HAS_MEDAL_AWARD_PENDING = 0;
			HAS_LEVELS_AWARD_PENDING = 0;
			startGameTime();
			setTextColor(1);
		}
	default:
		break;
	}
}
