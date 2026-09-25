#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <mwinline_n.h>

#include <dw/anim.h>
#include <dw/butterfly.h>
#include <dw/clock.h>
#include <dw/dooa.h>
#include <dw/eab.h>
#include <dw/endi.h>
#include <dw/entity.h>
#include <dw/fade.h>
#include <dw/font.h>
#include <dw/item.h>
#include <dw/kar.h>
#include <dw/main.h>
#include <dw/map.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/murd.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/pstat.h>
#include <dw/std.h>
#include <dw/tamer.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/utils.h>

char TAMER_TEXT_I_CANT_HOLD_ANYMORE[] = "I can't hold anymore.";
char TAMER_TEXT_HEY_ITS_EMPTY[] = "Hey! It's empty!";
char TAMER_TEXT_TAMER_LEVEL_WENT_UP[] = "Tamer level went up!!!";
char TAMER_TEXT_TAMER_LEVEL_WENT_DOWN[] = "Tamer level went down!!!";
char TAMER_TEXT_CONGRATULATIONS[] = "Congratulations!";
char TAMER_TEXT_TO_RECOGNIZE_YOUR_GREAT[24] = "To recognize your great";
char TAMER_TEXT_RECORS_THEY_SENT_A_MEDAL[] = "recors, they sent a Medal!";

static void *tamer_data_order[] = {
	TAMER_TEXT_RECORS_THEY_SENT_A_MEDAL,
	TAMER_TEXT_TO_RECOGNIZE_YOUR_GREAT,
	TAMER_TEXT_CONGRATULATIONS,
	TAMER_TEXT_TAMER_LEVEL_WENT_DOWN,
	TAMER_TEXT_TAMER_LEVEL_WENT_UP,
	TAMER_TEXT_HEY_ITS_EMPTY,
	TAMER_TEXT_I_CANT_HOLD_ANYMORE,
};

RECT MAIN_D_801341F4 = {0, 12, 256, 200};
char TAMER_TEXT_WOAH[] = "Woah!";
RECT MAIN_D_80134204 = {0, 12, 256, 200};
RECT MAIN_D_8013420C = {0, 12, 256, 200};

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
extern uint8_t PREVIOUS_EXIT;
extern VECTOR STORED_TAMER_POS;
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t MAIN_D_80185BB0[3];
extern int32_t MAIN_D_80185BB4[3];
extern int32_t MAIN_D_80185BBC[3];

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
extern int32_t IS_SCRIPT_PAUSED;
extern int8_t ITEM_SCOLD_FLAG;
extern int8_t FADE_PROTECTION_FULL;
extern uint16_t CURRENT_SCRIPT_ID;
extern uint8_t CURRENT_SCREEN;
extern uint8_t ACTIVE_BGM_FONT;
extern int32_t ACTIVE_FRAMEBUFFER;
extern void *ACTIVE_ORDERING_TABLE;
extern uint8_t SKIP_DAYTIME_TRANSITION;
extern int8_t HAS_ROTATION_DATA[8];
extern uint8_t UNKNOWN_TAMER_DATA[10];
extern uint16_t CURRENT_FRAME;
extern int16_t MINUTE;
extern int16_t HOUR;
extern int16_t DAY;
extern uint8_t YEAR;
extern int32_t MONEY;
extern int32_t NPC_IS_WALKING_TOWARDS[8];
extern VECTOR MAIN_D_801386A0[8];

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
	int16_t targetMap[10];
	int16_t targetExit[10];
} MapWarps;

extern MapWarps MAP_WARPS;

void entityLookAtLocation(Entity *entity, VECTOR *pos);
void setupEntityMatrix(int32_t entityId);
void fadeToBlack(int32_t frames);
void fadeFromBlack(int32_t frames);
void changeMap(uint8_t mapId, uint8_t exitId);
void addMapNameObject(uint8_t mapId);
void renderString(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t,
                  int32_t, int32_t, int32_t);
void renderTextboxNextArrow(int32_t id);
void setMapLayerEnabled(int32_t enabled);
int32_t isSoundLoaded(int32_t isAsync, int32_t soundId);
void playSound(int32_t vabId, uint32_t note);
void setCameraFollowPlayer(void);
void unsetCameraFollowPlayer(void);
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
int32_t createPoopPile(int16_t tileX, int16_t tileY);
void moveCameraByDiff(VECTOR *prev, VECTOR *cur);
void getRotationDifference(PositionData *data, VECTOR *target,
                           int16_t *outX, int16_t *outY, int16_t *outZ);
int32_t rotateEntity(SVECTOR *rot, int16_t *outX, int16_t *outY,
                     int16_t *outZ, int32_t speed);
void tickTamerWaypoints(void);
void loadMapDigimon(uint8_t *data, int32_t a);
void tickTamerBattle(int32_t instanceId);
void tickConditionBoundaries(void);
void handlePostBattleTiredness(void);
int32_t getEntityScreenPos(Entity *entity, int32_t flag, int16_t *outPos);
int32_t isUIBoxAvailable(int32_t id);
void playBGM(int16_t bgmId);
void readMapTFS(int32_t mapId);
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
extern int32_t FADE_PROTECTION;

void tickTamerBattle(int32_t instanceId);
void tickTamerOverworld(int16_t instanceId);
void tickWalkingState(void);
void tickChangeMap(void);
void tickPickupItem(void);
void tickTakeChest(void);
void tickIdle(void);
void tickTraining(void);
void tickPraiseScold(int8_t state, int8_t unused);
void tickFishing(void);
void tickOpening(void);
void tickEnding(void);
void tickSicknessLostLife(void);
void tickMachinedramonSpawn(void);
void tickBattleLostLife(void);
void tickAwardSomething(void);

void tickTamer(int16_t instanceId);
void renderItemPickupTextbox(int32_t instanceId);
void renderAwardSomethingTextbox(int32_t instanceId);
void setTamerDirection(int32_t direction);
void setupTamerOnWarp(int32_t x, int32_t y, int32_t z, int32_t rotationY);
int16_t getMapRotation(void);
void advanceBattleTime(int32_t result);
void addTamerLevel(int32_t chance, int32_t amount);
void checkItemPickup(void);
void checkMedalConditions(void);
uint8_t checkChestCollision(void);
void checkPendingAwards(void);
void tickPickupItem(void);
void tickTakeChest(void);
void tickAwardSomething(void);
void tickWalkingState(void);
void tickChangeMap(void);
void tickPraiseScold(int8_t state, int8_t unused);
void tickOpening(void);
void tickEnding(void);
void tickSicknessLostLife(void);
void tickMachinedramonSpawn(void);
void tickBattleLostLife(void);
void tickIdle(void);
void tickTraining(void);

static void *tamer_functions[] = {
	renderAwardSomethingTextbox,
	isTrainingComplete,
	addTamerLevel,
	checkChestCollision,
	worldPosToScreenPos2,
	tickEntityMoveToAxis,
	tickEntityMoveTo,
	tickEntitySetRotation,
	tickLookAtEntity,
	getEntityFromScriptId,
	tickEntityWalkTo,
	startAnimationTamer,
	getTamerState,
	setFullState,
	advanceBattleTime,
	startBattle,
	renderItemPickupTextbox,
	checkPendingAwards,
	checkMedalConditions,
	checkMapInteraction,
	checkItemPickup,
	setTamerDirection,
	getMapRotation,
	tickAwardSomething,
	tickBattleLostLife,
	tickMachinedramonSpawn,
	tickSicknessLostLife,
	tickEnding,
	tickOpening,
	tickPraiseScold,
	tickTraining,
	tickIdle,
	tickTakeChest,
	tickPickupItem,
	tickChangeMap,
	setTamerState,
	tickWalkingState,
	tickTamerOverworld,
	setupTamerOnWarp,
	loadMapEntities,
	tickTamer,
	initializeTamer,
};

void initializeTamer(int32_t type, int32_t posX, int32_t posY, int32_t posZ,
		     int32_t rotX, int32_t rotY, int32_t rotZ)
{
	int32_t i;

	PLAYER_SHADOW_ENABLED = 1;
	thunkLoadMMD(type, 2);

	ENTITY_TABLE[0] = &TAMER_ENTITY.entity;

	initializeDigimonObject(type, 0, (TickFunction)tickTamer);
	setEntityPosition(0, posX, posY, posZ);
	setEntityRotation(0, rotX, rotY, rotZ);
	setupEntityMatrix(0);

	STORED_TAMER_POS.vx = posX;
	STORED_TAMER_POS.vy = posY;
	STORED_TAMER_POS.vz = posZ;

	startAnimation(ENTITY_TABLE[0], 0);

	GAME_STATE = 0;
	ENTITY_TABLE[0]->isOnMap = 1;
	ENTITY_TABLE[0]->isOnScreen = 1;

	for (i = 0; i < 8; ++i) {
		HAS_ROTATION_DATA[i] = 0;
	}

	for (i = 0; i < 10; ++i) {
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

void tickWalkingState(void)
{
	int16_t rawRotation;
	int32_t mapRotation;
	int32_t originalRotation;

	tickTamerWaypoints();

	if ((isKeyDown(0x10) != 0) &&
	    (IS_SCRIPT_PAUSED == 1) &&
	    (FADE_PROTECTION == 0) &&
	    (UI_BOX_DATA[0].state != 1) &&
	    (UI_BOX_DATA[0].frame == 0)) {
		addGameMenu();
		setTamerState(1);
		startAnimation(&TAMER_ENTITY.entity, 0);
		unsetCameraFollowPlayer();

		copyVector(&STORED_TAMER_POS, &TAMER_ENTITY.entity.posData->location);
		IS_IN_MENU = 1;

		stopGameTime();
		setPartnerIdling();
		return;
	}

	if (((POLLED_INPUT & 0x1000) != 0) ||
	    ((POLLED_INPUT & 0x4000) != 0) ||
	    ((POLLED_INPUT & 0x8000) != 0) ||
	    ((POLLED_INPUT & 0x2000) != 0)) {
		if (((POLLED_INPUT & 8) != 0) ||
		    ((POLLED_INPUT & 0x10) != 0)) {
			if (TAMER_ENTITY.entity.anim.animId != 2) {
				startAnimation(&TAMER_ENTITY.entity, 2);
			}
		} else if (TAMER_ENTITY.entity.anim.animId != 3) {
			startAnimation(&TAMER_ENTITY.entity, 3);
		}
		ITEM_SCOLD_FLAG = 0;
	} else if (TAMER_ENTITY.entity.anim.animId != 0) {
		startAnimation(&TAMER_ENTITY.entity, 0);
	}

	rawRotation = getMapRotation();
	mapRotation = rawRotation;
	originalRotation = mapRotation;
	mapRotation = (int16_t)(mapRotation / 0x200);
	if ((originalRotation % 0x200) >= 0x100) {
		mapRotation = (int16_t)(mapRotation + 1);
	}
	mapRotation = (int16_t)(mapRotation * 0x200);

	if ((POLLED_INPUT & 0x1000) != 0) {
		if ((POLLED_INPUT & 0x8000) != 0) {
			setTamerDirection((int16_t)(mapRotation + 0x600));
		} else if ((POLLED_INPUT & 0x2000) != 0) {
			setTamerDirection((int16_t)(mapRotation + 0xa00));
		} else {
			setTamerDirection((int16_t)(mapRotation + 0x800));
		}
	} else if ((POLLED_INPUT & 0x4000) != 0) {
		if ((POLLED_INPUT & 0x8000) != 0) {
			setTamerDirection((int16_t)(mapRotation + 0x200));
		} else if ((POLLED_INPUT & 0x2000) != 0) {
			setTamerDirection((int16_t)(mapRotation + 0xe00));
		} else {
			setTamerDirection((int16_t)mapRotation);
		}
	} else if ((POLLED_INPUT & 0x8000) != 0) {
		setTamerDirection((int16_t)(mapRotation + 0x400));
	} else if ((POLLED_INPUT & 0x2000) != 0) {
		setTamerDirection((int16_t)(mapRotation + 0xc00));
	}

	checkItemPickup();
	checkMapInteraction();
	checkMedalConditions();
	checkPendingAwards();

	copyVector(&STORED_TAMER_POS, &TAMER_ENTITY.entity.posData->location);
}

int16_t getMapRotation(void)
{
	int16_t dz;
	int16_t dx;

	dz = GS_VIEWPOINT.vpz - GS_VIEWPOINT.vrz;
	dx = GS_VIEWPOINT.vpx - GS_VIEWPOINT.vrx;

	return _atan(dz, dx);
}

void setTamerDirection(int32_t direction)
{
	if (0xfff < direction) {
		direction = (direction + -0x1000) * 0x10000 >> 0x10;
	}
	TAMER_ENTITY.entity.posData->rotation.vy = (int16_t)direction;
}

void checkItemPickup(void)
{
	DroppedItem *item;
	int32_t i;
	int16_t tileX;
	int16_t tileY;

	getModelTile(&TAMER_ENTITY.entity.posData->location, &tileX, &tileY);
	PICKUP_ITEM_TYPE = 0xff;

	item = DROPPED_ITEMS;
	for (i = 0; i < 11; ++i) {
		if (item->worldItem.type == 0xff) {
			++item;
			continue;
		}

		if ((item->tileX < tileX - 1) ||
		    (tileX + 1 < item->tileX)) {
			goto next;
		}

		if ((item->tileY < tileY - 1) ||
		    (tileY + 1 < item->tileY)) {
			goto next;
		}

		PICKUP_ITEM_TYPE = i;

		if (IS_STANDING_ON_DROP != 1) {
			setTamerState(7);
			IS_STANDING_ON_DROP = 1;
		}
		break;
next:
		++item;
	}

	if (PICKUP_ITEM_TYPE == 0xff) {
		IS_STANDING_ON_DROP = 0;
	}
}

void checkMapInteraction(void)
{
	int16_t collision;
	uint8_t trigger;

	collision = entityCheckCollision(&PARTNER_ENTITY.digimonEntity.entity,
	                                 &TAMER_ENTITY.entity, 0, 0);
	if ((collision >= 2) && (collision < 10)) {
		TAMER_ENTITY.entity.anim.animFlag |= 2;
		if (((NPC_ENTITIES[collision - 2].autotalk == 1) ||
		     ((NPC_ENTITIES[collision - 2].autotalk == 0) &&
		      ((CHANGED_INPUT & 0x40) != 0))) &&
		    (IS_SCRIPT_PAUSED == 1)) {
			TALKED_TO_ENTITY = collision;
			removeTriangleMenu();
			closeInventoryBoxes();
			removeUIBox1();
			callScriptSection(CURRENT_SCRIPT_ID,
			                  NPC_ENTITIES[TALKED_TO_ENTITY - 2].scriptId, 1);
		}
	} else if (collision == 10) {
		collisionGrace(0, ENTITY_TABLE[0], 0, 0);
	}

	if (TAMER_STATE != 0) {
		return;
	}

	trigger = getTileTrigger(&ENTITY_TABLE[0]->posData->location);
	if (trigger == 120) {
		if (((PARTNER_PARA.condition & 8) != 0) &&
		    (IS_SCRIPT_PAUSED == 1)) {
			callScriptSection(0, 0x4e2, 0);
		}
	} else if ((trigger >= 110) && (trigger < 120)) {
		TARGET_MAP = MAP_WARPS.targetMap[trigger - 110];
		MAIN_D_80134DF9 = trigger - 110;
		CURRENT_EXIT = MAP_WARPS.targetExit[trigger - 110];
		PREVIOUS_EXIT = trigger - 110;

		setTamerState(5);
		unsetCameraFollowPlayer();
		stopGameTime();
	} else if ((trigger > 50) && (trigger < 80)) {
		if (IS_SCRIPT_PAUSED == 1) {
			callScriptSection(CURRENT_SCRIPT_ID, trigger, 0);
		}
	} else if ((trigger >= 80) && (trigger < 110)) {
		if (((CHANGED_INPUT & 0x40) != 0) &&
		    (IS_SCRIPT_PAUSED == 1)) {
			callScriptSection(CURRENT_SCRIPT_ID, trigger, 0);
		}
	}

	trigger = checkChestCollision();
	if ((trigger != 0xff) && ((CHANGED_INPUT & 0x40) != 0)) {
		INTERACTED_CHEST = trigger;
		setTamerState(14);
	}
}

void checkMedalConditions(void)
{
	int32_t i;

	if (hasMedal(5) == 0) {
		for (i = 0; i < 0x39; ++i) {
			if (hasMove(i) == 0) {
				break;
			}
		}

		if (i == 0x39) {
			unlockMedal(5);
			HAS_MEDAL_AWARD_PENDING = 1;
		}
	}

	if (hasMedal(7) == 0) {
		BaseStats *bs = &PARTNER_ENTITY.digimonEntity.stats.base;
		if ((bs->hp == 9999) && (bs->mp == 9999) &&
		    (bs->off == 999) && (bs->def == 999) &&
		    (bs->brain == 999) && (bs->speed == 999)) {
			unlockMedal(7);
			HAS_MEDAL_AWARD_PENDING = 1;
		}
	}

	if ((hasMedal(0xD) == 0) && (MONEY == 999999)) {
		unlockMedal(0xD);
		HAS_MEDAL_AWARD_PENDING = 1;
	}

	if ((hasMedal(0xE) == 0) && (YEAR == 10)) {
		unlockMedal(0xE);
		HAS_MEDAL_AWARD_PENDING = 1;
	}

	if (hasMedal(6) == 0) {
		for (i = 1; i < 62; ++i) {
			if (hasDigimonRaised((uint16_t)i) == 0) {
				break;
			}
		}

		if (i == 62) {
			unlockMedal(6);
			HAS_MEDAL_AWARD_PENDING = 1;

			++TAMER_ENTITY.tamerLevel;
			if (TAMER_ENTITY.tamerLevel >= 11) {
				TAMER_ENTITY.tamerLevel = 10;
			}
		}
	}
	if ((hasMedal(9) == 0) && (PARTNER_PARA.fishCaught >= 100)) {
		unlockMedal(9);
		HAS_MEDAL_AWARD_PENDING = 1;
	}
	if (hasMedal(0xC) == 0) {
		for (i = 0; i < 66; ++i) {
			if (getCardAmount((uint8_t)i) == 0) {
				break;
			}
		}

		if (i == 66) {
			unlockMedal(0xC);
			HAS_MEDAL_AWARD_PENDING = 1;
		}
	}
}

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

	renderTextboxNextArrow(1);
	++TEXTBOX_OPEN_TIMER;
}

int32_t startBattle(int16_t instanceId)
{
	int8_t battleResult;
	int32_t result;
	int32_t battleData;

	GAME_STATE = 1;
	copyVector(&STORED_TAMER_POS, &TAMER_ENTITY.entity.posData->location);

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
		++PARTNER_PARA.battles;
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

int32_t tickEntityWalkTo(uint8_t scriptId, uint8_t targetId, int32_t x, int32_t z, int8_t useCamera)
{
	Entity *e;
	PositionData *pd;
	VECTOR from;
	VECTOR to;
	int16_t fromTileX;
	int16_t fromTileY;
	int16_t toTileX;
	int16_t toTileY;
	int16_t col;

	col = -1;
	e = getEntityFromScriptId(&scriptId);

	if (scriptId >= 2) {
		NPC_IS_WALKING_TOWARDS[scriptId - 2] = 1;
	}

	pd = e->posData;
	from = pd->location;

	if (targetId == 0xFF) {
		to.vx = x;
		to.vy = e->posData->location.vy;
		to.vz = z;
	} else {
		pd = getEntityFromScriptId(&targetId)->posData;
		to = pd->location;
	}

	if ((PREVIOUS_CAMERA_POS_INITIALIZED == 0) && (useCamera == 1)) {
		PREVIOUS_CAMERA_POS = from;
		PREVIOUS_CAMERA_POS_INITIALIZED = 1;
	}

	getModelTile(&from, &fromTileX, &fromTileY);
	getModelTile(&to, &toTileX, &toTileY);
	entityLookAtLocation(e, &to);

	if (useCamera == 1) {
		moveCameraByDiff(&PREVIOUS_CAMERA_POS, &from);
		PREVIOUS_CAMERA_POS = from;
	}

	if (targetId != 0xFF) {
		col = entityCheckCollision(0, e, 0, 0);
	}

	if (((fromTileX == toTileX) && (fromTileY == toTileY)) || ((col != -1) && (col < 9))) {
		PREVIOUS_CAMERA_POS_INITIALIZED = 0;

		if (scriptId >= 2) {
			NPC_IS_WALKING_TOWARDS[scriptId - 2] = 0;
		}

		return 1;
	}

	return 0;
}

int32_t tickLookAtEntity(uint8_t scriptId1, uint8_t scriptId2)
{
	int32_t result;
	Entity *entity;
	int8_t entityId;
	int32_t i;
	int16_t rotX;
	int16_t rotZ;
	int16_t rotY;

	if (scriptId1 == 0xfd) {
		entityId = 0;
	} else if (scriptId1 == 0xfc) {
		entityId = 1;
	} else {
		for (i = 0; i < 8; i++) {
			if (scriptId1 == NPC_ENTITIES[i].scriptId) {
				entityId = i + 2;
				break;
			}
		}
	}

	result = 0;
	switch (HAS_ROTATION_DATA[entityId]) {
	case 0:
		MAIN_D_801386A0[entityId] =
			getEntityFromScriptId(&scriptId2)->posData->location;
		HAS_ROTATION_DATA[entityId] = 1;
		break;
	case 1:
		entity = getEntityFromScriptId(&scriptId1);
		getRotationDifference(entity->posData,
		                      &MAIN_D_801386A0[entityId],
		                      &rotX, &rotY, &rotZ);
		result = rotateEntity(&entity->posData->rotation, &rotX,
				      &rotY, &rotZ, 0x200);
		if (result == 1) {
			HAS_ROTATION_DATA[entityId] = 0;
		}
		break;
	}

	return result;
}

int32_t tickEntitySetRotation(uint32_t scriptId, int16_t rotationY)
{
	Entity *entity;

	entity = getEntityFromScriptId((uint8_t *)&scriptId);
	entity->posData->rotation.vy = rotationY;
	return 1;
}

int32_t tickEntityMoveTo(scriptId1, scriptId2, targetX, targetZ, speed,
			 withCamera)
	uint8_t scriptId1;
	uint8_t scriptId2;
	int32_t targetX;
	int32_t targetZ;
	int8_t speed;
	int8_t withCamera;
{
	/* TODO: get rid of extra declaration */
	extern void setEntityPosition(int32_t entityId, int32_t x, long y,
	                              int32_t z);
	Entity *entity;
	Entity *targetEntity;
	PositionData *position;
	int8_t finishedX;
	int8_t finishedZ;
	int32_t destinationX;

	destinationX = targetX;
	entity = getEntityFromScriptId(&scriptId1);
	if (PREVIOUS_CAMERA_POS_INITIALIZED == 0) {
		if (scriptId2 == 0xff) {
			MOVE_TO_DELTA_X =
				(targetX - entity->posData->location.vx) / speed;
			MAIN_D_80134C9A =
				(targetZ - entity->posData->location.vz) / speed;
		} else {
			targetEntity = getEntityFromScriptId(&scriptId2);
			MOVE_TO_DELTA_X =
				(targetEntity->posData->location.vx -
				 entity->posData->location.vx) / speed;
			MAIN_D_80134C9A =
				(targetEntity->posData->location.vz -
				 entity->posData->location.vz) / speed;
		}
		PREVIOUS_CAMERA_POS_INITIALIZED = 1;
		PREVIOUS_CAMERA_POS = entity->posData->location;
	} else {
		position = entity->posData;
		setEntityPosition(scriptId1,
		                  position->location.vx + MOVE_TO_DELTA_X,
		                  position->location.vy,
		                  position->location.vz + MAIN_D_80134C9A);
		setupEntityMatrix(scriptId1);

		finishedX = finishedZ = 0;
		if (MOVE_TO_DELTA_X < 0) {
			if (entity->posData->location.vx <= destinationX) {
				entity->posData->location.vx = destinationX;
				finishedX = 1;
			}
		} else if (entity->posData->location.vx >= destinationX) {
			entity->posData->location.vx = destinationX;
			finishedX = 1;
		}

		if (MAIN_D_80134C9A < 0) {
			if (entity->posData->location.vz <= targetZ) {
				entity->posData->location.vz = targetZ;
				finishedZ = 1;
			}
		} else if (entity->posData->location.vz >= targetZ) {
			entity->posData->location.vz = targetZ;
			finishedZ = 1;
		}

		if ((finishedX == 1) && (finishedZ == 1)) {
			PREVIOUS_CAMERA_POS_INITIALIZED = 0;
			return 1;
		}
	}

	if (withCamera == 1) {
		moveCameraByDiff(&PREVIOUS_CAMERA_POS,
		                 &entity->posData->location);
		PREVIOUS_CAMERA_POS = entity->posData->location;
	}

	return 0;
}

int32_t tickEntityMoveToAxis(scriptId, target, axis, speed, withCamera)
	uint8_t scriptId;
	int32_t target;
	int32_t axis;
	int32_t speed;
	int8_t withCamera;
{
	Entity *entity;
	long *axisValue;

	entity = getEntityFromScriptId(&scriptId);
	if (axis == 0) {
		axisValue = &entity->posData->location.vx;
	} else if (axis == 1) {
		axisValue = &entity->posData->location.vy;
	} else {
		axisValue = &entity->posData->location.vz;
	}

	if (PREVIOUS_CAMERA_POS_INITIALIZED == 0) {
		MOVE_TO_DELTA_X = (target - *axisValue) / speed;
		PREVIOUS_CAMERA_POS = entity->posData->location;
		PREVIOUS_CAMERA_POS_INITIALIZED = 1;
	} else {
		*axisValue += MOVE_TO_DELTA_X;
		setEntityPosition(scriptId, entity->posData->location.vx,
		                  entity->posData->location.vy,
		                  entity->posData->location.vz);
		setupEntityMatrix(scriptId);

		if (MOVE_TO_DELTA_X < 0) {
			if (*axisValue <= target) {
				*axisValue = target;
				PREVIOUS_CAMERA_POS_INITIALIZED = 0;
				return 1;
			}
		} else if (*axisValue >= target) {
			*axisValue = target;
			PREVIOUS_CAMERA_POS_INITIALIZED = 0;
			return 1;
		}
	}

	if (withCamera == 1) {
		moveCameraByDiff(&PREVIOUS_CAMERA_POS,
		                 &entity->posData->location);
		PREVIOUS_CAMERA_POS = entity->posData->location;
	}
	return 0;
}

Entity *getEntityFromScriptId(uint8_t *scriptId)
{
	int32_t i;

	if (*scriptId == 0xfd) {
		*scriptId = 0;
		return ENTITY_TABLE[0];
	}
	if (*scriptId == 0xfc) {
		*scriptId = 1;
		return ENTITY_TABLE[1];
	}
	for (i = 0; i < 8; i++) {
		if ((ENTITY_TABLE[i + 2] != 0) &&
		    (*scriptId == NPC_ENTITIES[i].scriptId)) {
			*scriptId = i + 2;
			return ENTITY_TABLE[i + 2];
		}
	}
}

static inline int16_t tamer_s16(int16_t a)
{
	return a;
}
void worldPosToScreenPos2(int16_t *x, int16_t *y, int16_t *z)
{
	SVECTOR v;
	SVECTOR sxy;
	int16_t new_var;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	v.vx = *x;
	v.vy = *y;
	v.vz = *z;
	gte_ldv0(&v);
	gte_rtps();
	gte_stsxy((long *)&sxy);
	new_var = sxy.vx;
	*x = tamer_s16(new_var) - (0xA0 - DRAWING_OFFSET_X);
	*y = sxy.vy - (0x78 - DRAWING_OFFSET_Y);
}

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
	renderTextboxNextArrow(1);
	++TEXTBOX_OPEN_TIMER;
}

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
		drawString(TAMER_TEXT_WOAH, 0, 0x24);
		INTERACTED_CHEST_STATE = 0;
		TAKE_ITEM_FRAME_COUNT = 0;
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		available = isUIBoxAvailable(1);
		if (available == 1) {
			setRECT(&targetRect, -130, 42, 262, 59);
			getEntityScreenPos(ENTITY_TABLE[0], 1, screenPos);
			setRECT(&sourceRect,
				screenPos[0] - 5,
				screenPos[1] - 5,
				10, 10);
			RECEIVED_ITEM_TYPE =
				DROPPED_ITEMS[PICKUP_ITEM_TYPE].worldItem.type;
			createAnimatedUIBox(1, 0, 2, &targetRect, &sourceRect, 0,
			                    renderItemPickupTextbox);
			TAMER_SUBSTATE = 2;
		}
		break;
	case 2:
		++TAKE_ITEM_FRAME_COUNT;
		available = isKeyDown(0x40);
		if ((available != 0) && (4 < TAKE_ITEM_FRAME_COUNT)) {
			if (TAKE_ITEM_FRAME_COUNT < 0x3c) {
				playSound(0, 3);
			}
			available = giveItem(DROPPED_ITEMS[PICKUP_ITEM_TYPE].worldItem.type & 0xff, 0);
			if (available == 0) {
				drawString(TAMER_TEXT_I_CANT_HOLD_ANYMORE, 0, 0x18);
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
		++TAKE_ITEM_FRAME_COUNT;
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
		setRECT(&textRect,
			screenPos[0] - 5,
			screenPos[1] - 5,
			10, 10);
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
			drawString(TAMER_TEXT_WOAH, 0, 0x24);
			INTERACTED_CHEST_STATE = 0;
		} else {
			drawString(TAMER_TEXT_HEY_ITS_EMPTY, 0, 0x18);
			INTERACTED_CHEST_STATE = 2;
		}
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		available = isUIBoxAvailable(1);
		if (available == 1) {
			trayResult = tickOpenChestTray(INTERACTED_CHEST);
			if (trayResult == 1) {
				setRECT(&targetRect, -130, 42, 262, 59);
				getEntityScreenPos(ENTITY_TABLE[0], 1,
						   screenPos);
				setRECT(&sourceRect,
					screenPos[0] - 5,
					screenPos[1] - 5,
					10, 10);
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
		++TAKE_ITEM_FRAME_COUNT;
		if (((POLLED_INPUT & PADRdown) != 0) &&
		    (5 < TAKE_ITEM_FRAME_COUNT)) {
			TAKE_ITEM_FRAME_COUNT = 0;
			available = giveItem(RECEIVED_ITEM_TYPE, 0);
			if (available == 0) {
				drawString(TAMER_TEXT_I_CANT_HOLD_ANYMORE, 0, 0x18);
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
		++TAKE_ITEM_FRAME_COUNT;
		if (((POLLED_INPUT & PADRdown) != 0) &&
		    (5 < TAKE_ITEM_FRAME_COUNT)) {
			getEntityScreenPos(&TAMER_ENTITY.entity, 1,
					   screenPos);
			setRECT(&textRect,
				screenPos[0] - 5,
				screenPos[1] - 5,
				10, 10);
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
	switch (TAMER_SUBSTATE) {
	case 0:
		loadDynamicLibrary(MURD_REL, 0, 0, 0, 0);
		MURD_tick((PartnerEntity *)ENTITY_TABLE[1], 0);
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		if ((0 > MURD_tick((PartnerEntity *)ENTITY_TABLE[1], 1)) &&
		    (0 > DOOA_tick((PartnerEntity *)ENTITY_TABLE[1],
				   GENERAL_BUFFER_PTR + 0x4b000, 1))) {
			SOME_SCRIPT_SYNC_BIT = 1;
		}
	default:
		break;
	}
}

void tickMachinedramonSpawn(void)
{
	switch (TAMER_SUBSTATE) {
	case 0:
		EAB_tick(ENTITY_TABLE[2], 0);
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		if (0 > EAB_tick(ENTITY_TABLE[2], 1)) {
			SOME_SCRIPT_SYNC_BIT = 1;
		}
	default:
		break;
	}
}

void tickBattleLostLife(void)
{
	switch (TAMER_SUBSTATE) {
	case 0:
		loadDynamicLibrary(MURD_REL, 0, 0, 0, 0);
		MURD_tick((PartnerEntity *)ENTITY_TABLE[1], 0);
		TAMER_SUBSTATE = 1;
		break;
	case 1:
		if (0 > MURD_tick((PartnerEntity *)ENTITY_TABLE[1], 1)) {
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
				drawString(TAMER_TEXT_TAMER_LEVEL_WENT_UP, 0, 0x78);
			} else {
				setTextColor(3);
				drawString(TAMER_TEXT_TAMER_LEVEL_WENT_DOWN, 0, 0x78);
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
		drawString(TAMER_TEXT_CONGRATULATIONS, 0, 0x78);
		TAMER_SUBSTATE = 2;
		break;
	case 2:
		drawString(TAMER_TEXT_TO_RECOGNIZE_YOUR_GREAT, 0, 0x84);
		TAMER_SUBSTATE = 3;
		break;
	case 3:
		drawString(TAMER_TEXT_RECORS_THEY_SENT_A_MEDAL, 0, 0x90);
		setTextColor(1);
		TAMER_SUBSTATE = 4;
		break;
	case 4:
		available = isUIBoxAvailable(1);
		if (available == 1) {
			setRECT(&targetRect, -130, 42, 262, 59);
			getEntityScreenPos(ENTITY_TABLE[0], 1, screenPos);
			setRECT(&sourceRect,
				screenPos[0] - 5,
				screenPos[1] - 5,
				10, 10);
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
			setRECT(&textRect,
				screenPos[0] - 5,
				screenPos[1] - 5,
				10, 10);
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
