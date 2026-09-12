#include <libgte.h>

#include <dw/anim.h>
#include <dw/butterfly.h>
#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/doo.h>
#include <dw/evl.h>
#include <dw/fade.h>
#include <dw/item.h>
#include <dw/kar.h>
#include <dw/main.h>
#include <dw/map.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/pstat.h>
#include <dw/std.h>
#include <dw/tamer.h>
#include <dw/ui.h>
#include <dw/utils.h>

#include "common.h"

extern int32_t HAS_BUTTERFLY;
extern int32_t BUTTERFLY_ID;
extern int8_t PARTNER_STATE;
extern int8_t PARTNER_SUB_STATE;
extern int8_t PARTNER_ANIMATION;
extern int8_t PARTNER_IS_STANDING_STILL;
extern int16_t MAIN_D_80134E28;
extern int16_t EVOLUTION_TARGET;
extern int8_t EMOTION_ANIM_TIMEOUT;
extern int8_t STOP_DISTANCE_TIMER;
extern uint8_t HEALTH_SHOE_FRAMES;
extern uint16_t WILD_POOP_ID;
extern uint8_t POOP_TO_EAT;
extern int32_t SOME_SCRIPT_SYNC_BIT;
extern int16_t MAIN_D_80134E34;

extern int8_t MAIN_STATE;
extern int8_t GAME_STATE;
extern uint8_t CURRENT_SCREEN;

extern int32_t IS_IN_MENU;
extern int32_t CURRENT_MENU;
extern int32_t TARGET_MENU;

extern int32_t MEMORY_CARD_ID;
extern int32_t MEMORY_CARD_SLOT;
extern int32_t CHECKED_MEMORY_CARD;

extern VECTOR STORED_TAMER_POS;

typedef void (*ItemFunction)(int16_t);
extern ItemFunction ITEM_FUNCTIONS[128];

extern int8_t ITEM_TAKE_DISTANCE[68];

void tickMainMenu(int32_t instanceId);
void renderMainMenu(int32_t instanceId);
void renderFeedingItem(int32_t instanceId);

void tickPartnerOverworld(int32_t instanceId);
void tickNormal(void);
void partnerSleep(void);
void partnerPraiseScold(int32_t partnerState);
void partnerFeedItem(void);
void tickPartnerToilet(void);
void partnerWildPoop(void);
void tickPartnerDying(void);
void partnerEatShit(void);
void tickPartnerIdle(void);
void tickPartnerEvolving(void);
void tickPartnerDying2(void);
void tickPartnerWalking(void);
void setPartnerSlowWalking(void);
void setPartnerIdle(void);
int32_t getPartnerTamerCloseness(void);
int32_t checkEatDistance(int32_t distance);
void tickPartnerBattle(int32_t instanceId);
void handleConditionBubble();
int32_t entityIsOffScreen(Entity *entity, int32_t width, int32_t height);
void tickConditionBoundaries();
void unsetCameraFollowPlayer();
void tickPartnerWaypoints();
void entityLookAtLocation(Entity *entity, VECTOR *pos);
void playSound(int32_t vabId, uint32_t note);
void tickPartnerCollision();
void tickPartnerNormal();
void tickPartnerPoopingMechanic();
void detectEdiblePoop();
void fadeToBlack(int32_t frames);
void fadeFromBlack(int32_t frames);
void sleepRegen();
void handleSleeping();
void setCameraFollowPlayer();
void handleSpecialEvolutions(int32_t mode, Entity *partner);
void startGameTime();
void updateTimeOfDay();
void handlePraiseScold();
int32_t partnerWillRefuseItem();
void removeOneSelectedItem();
void partnerHandleFoodFeed(int32_t type);
void createCameraMovement(VECTOR *pos, int32_t instanceId);
void handleToilet();
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
int32_t createPoopPile(int16_t tileX, int16_t tileY);
void handleWildPoop();
void deinitializeFishing();
void removeTriangleMenu();
void closeInventoryBoxes();
void removeUIBox1();
void loadMapSounds2(int32_t mapSound);
void isSoundLoaded(int32_t isAsync, int32_t soundId);
void setFishingEnabled();
void setFishingDisabled();
void handleEatingPoop();
int16_t entityCheckCollision(Entity *source, Entity *entity, int32_t arg2,
                             int32_t arg3);
int32_t random(int32_t limit);
void writePStat(int32_t id, int32_t value);
int32_t readPStat(int32_t id);
void addTamerLevel(int32_t chance, int32_t amount);
int32_t getMapSoundId(int32_t mapId);
void loadMapSounds(int32_t soundId);
void checkShopMap(int32_t mapId);
void checkArenaMap(int32_t mapId);
void readMapTFS(int32_t mapId);

static void *partner_text_order[] = { MAIN_func_800DF7F8,
	                              callDigimonRoutine,
	                              startPartnerAnimation,
	                              getPartnerState,
	                              MAIN_func_800DF5A0,
	                              checkEatDistance,
	                              setPartnerState,
	                              setPartnerIdle,
	                              updateConditionAnimation,
	                              setPartnerSlowWalking,
	                              getPartnerTamerCloseness,
	                              tickPartnerWalking,
	                              tickPartnerDying2,
	                              tickPartnerEvolving,
	                              tickPartnerIdle,
	                              partnerEatShit,
	                              tickPartnerDying,
	                              partnerWildPoop,
	                              tickPartnerToilet,
	                              partnerFeedItem,
	                              partnerPraiseScold,
	                              partnerSleep,
	                              tickNormal,
	                              tickPartnerOverworld,
	                              tickPartner };

void tickPartner(int32_t instanceId)
{
	if (((GAME_STATE != 0) || (PARTNER_STATE != 1)) &&
	    (HAS_BUTTERFLY == 0)) {
		unsetButterfly(BUTTERFLY_ID);
		HAS_BUTTERFLY = -1;
	}
	switch (GAME_STATE) {
	case 0:
		tickPartnerOverworld(instanceId);
		break;
	case 1:
	case 2:
	case 3:
		tickPartnerBattle(instanceId);
		break;
	case 4:
	case 5:
		STD_tickPartnerTournament(instanceId);
	default:
		break;
	}
}

void tickPartnerOverworld(int32_t instanceId)
{
	int32_t isOffScreen;

	if (IS_IN_MENU == 1) {
		tickAnimation(&PARTNER_ENTITY.digimonEntity.entity);
	} else {
		switch (PARTNER_STATE) {
		case 1:
			tickNormal();
			break;
		case 3:
			partnerSleep();
			break;
		case 4:
		case 15:
			partnerPraiseScold(PARTNER_STATE);
			break;
		case 5:
			partnerFeedItem();
			break;
		case 6:
			tickPartnerToilet();
			break;
		case 7:
			partnerWildPoop();
			break;
		case 8:
			tickPartnerDying();
			break;
		case 9:
			partnerEatShit();
			break;
		case 10:
			handleConditionBubble();
			break;
		case 11:
			tickPartnerIdle();
			break;
		case 13:
			tickPartnerEvolving();
			break;
		case 14:
			tickPartnerDying2();
		default:
			break;
		}

		isOffScreen = entityIsOffScreen(
			&PARTNER_ENTITY.digimonEntity.entity, 320, 240);
		PARTNER_ENTITY.digimonEntity.entity.isOnScreen =
			isOffScreen ^ 1;

		tickConditionBoundaries();
		tickAnimation(&PARTNER_ENTITY.digimonEntity.entity);
	}
}

void tickNormal(void)
{
	tickPartnerCollision();
	tickPartnerWalking();
	tickPartnerNormal();
	tickPartnerPoopingMechanic();
	detectEdiblePoop();
	handleConditionBubble();
}

void partnerSleep(void)
{
	int32_t closeness;

	switch (PARTNER_SUB_STATE) {
	case 0:
		stopGameTime();
		unsetCameraFollowPlayer();
		closeness = getPartnerTamerCloseness();
		if (closeness != 2) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
			               2);
		}
		setTamerState(6);
		tickPartnerWaypoints();
		PARTNER_SUB_STATE = 1;
		break;
	case 1:
		entityLookAtLocation(&TAMER_ENTITY.entity,
		                     &PARTNER_ENTITY.digimonEntity.entity
		                              .posData->location);
		entityLookAtLocation(&PARTNER_ENTITY.digimonEntity.entity,
		                     &TAMER_ENTITY.entity.posData->location);
		closeness = getPartnerTamerCloseness();
		if (closeness == 2) {
			playSound(0, 15);
			startAnimation(&TAMER_ENTITY.entity, 8);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
			               0);
			PARTNER_SUB_STATE = 2;
		}
		break;
	case 2:
		if (TAMER_ENTITY.entity.anim.animFrame >=
		    TAMER_ENTITY.entity.anim.frameCount) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
			               11);
			PARTNER_SUB_STATE = 3;
		}
		break;
	case 3:
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame >=
		    PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
			               9);
			PARTNER_SUB_STATE = 4;
		}
		break;
	case 4:
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame >=
		    PARTNER_ENTITY.digimonEntity.entity.anim.frameCount - 5) {
			fadeToBlack(40);
			PARTNER_SUB_STATE = 5;
		}
		break;
	case 5:
		if (FADE_OUT_CURRENT > 39) {
			sleepRegen();
			handleSleeping();
			if (UI_BOX_DATA[0].state == 0) {
				CHECKED_MEMORY_CARD = 0x10;
				if ((MEMORY_CARD_ID == -1) ||
				    (MEMORY_CARD_SLOT == -1)) {
					CURRENT_MENU = -1;
					TARGET_MENU = -1;
				} else {
					CURRENT_MENU = -2;
					TARGET_MENU = 40;
					MAIN_STATE = 1;
				}
				addObject(5000, 0, tickMainMenu,
				          renderMainMenu);
				PARTNER_SUB_STATE = 6;
			}
		}
		break;
	case 6:
		if (CURRENT_MENU == -1) {
			removeObject(5000, 0);
			fadeFromBlack(40);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
			               0);
			updateTimeOfDay();
			PARTNER_SUB_STATE = 7;
		}
		break;
	case 7:
		if (FADE_IN_CURRENT > 34) {
			PARTNER_STATE = 1;
			setTamerState(0);
			setCameraFollowPlayer();
			handleSpecialEvolutions(
				2, &PARTNER_ENTITY.digimonEntity.entity);
			startGameTime();
		}
		break;
	default:
		break;
	}
}

void partnerPraiseScold(int32_t partnerState)
{
	int32_t closeness;

	switch (PARTNER_SUB_STATE) {
	case 0:
		unsetCameraFollowPlayer();
		tickPartnerWaypoints();
		closeness = getPartnerTamerCloseness();
		if (closeness != 2) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
			               2);
		}
		PARTNER_SUB_STATE = 1;
		MAIN_D_80134E28 = 0;
		break;
	case 1:
		++MAIN_D_80134E28;
		entityLookAtLocation(&TAMER_ENTITY.entity,
		                     &PARTNER_ENTITY.digimonEntity.entity
		                              .posData->location);
		entityLookAtLocation(&PARTNER_ENTITY.digimonEntity.entity,
		                     &TAMER_ENTITY.entity.posData->location);
		closeness = getPartnerTamerCloseness();
		if (closeness == 2) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
			               0);
			if (partnerState == 15) {
				playSound(0, 14);
				setTamerState(13);
			} else {
				setTamerState(9);
			}
			PARTNER_SUB_STATE = 2;
		}
		break;
	case 2:
		if (TAMER_ENTITY.entity.anim.animId == 0) {
			handlePraiseScold();
			PARTNER_SUB_STATE = 3;
		}
		break;
	case 3:
		if (PARTNER_ENTITY.digimonEntity.entity.anim.loopCount ==
		    255) {
			PARTNER_ENTITY.digimonEntity.entity.anim.loopCount =
				1;
		}
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame >=
		    PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			setPartnerState(1);
			setTamerState(0);
			setCameraFollowPlayer();
			handleSpecialEvolutions(3, ENTITY_TABLE[1]);
		}
		break;
	default:
		break;
	}
}

void partnerFeedItem(void)
{
	int32_t isClose;
	int32_t type;

	switch (PARTNER_SUB_STATE) {
	case 0:
		isClose = checkEatDistance(
			ITEM_TAKE_DISTANCE[PARTNER_ENTITY.digimonEntity.entity
		                                   .type -
		                           1]);
		if (isClose == 1) {
			PARTNER_SUB_STATE = 1;
		} else {
			PARTNER_SUB_STATE = 2;
		}
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
		PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy +=
			2048;
		tickPartnerWaypoints();
		break;
	case 1:
		entityLookAtLocation(&TAMER_ENTITY.entity,
		                     &PARTNER_ENTITY.digimonEntity.entity
		                              .posData->location);
		isClose = checkEatDistance(
			ITEM_TAKE_DISTANCE[PARTNER_ENTITY.digimonEntity.entity
		                                   .type -
		                           1]);
		if (isClose == 0) {
			PARTNER_SUB_STATE = 3;
		}
		break;
	case 2:
		entityLookAtLocation(&TAMER_ENTITY.entity,
		                     &PARTNER_ENTITY.digimonEntity.entity
		                              .posData->location);
		entityLookAtLocation(&PARTNER_ENTITY.digimonEntity.entity,
		                     &TAMER_ENTITY.entity.posData->location);
		isClose = checkEatDistance(
			ITEM_TAKE_DISTANCE[PARTNER_ENTITY.digimonEntity.entity
		                                   .type -
		                           1]);
		if (isClose == 1) {
			PARTNER_SUB_STATE = 3;
		}
		break;
	case 3:
		entityLookAtLocation(&PARTNER_ENTITY.digimonEntity.entity,
		                     &TAMER_ENTITY.entity.posData->location);
		startAnimationTamer(5);
		playSound(0, 12);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
		addObject(0xfa7, 0, 0, renderFeedingItem);
		PARTNER_SUB_STATE = 4;
		break;
	case 4:
		if (7 < TAMER_ENTITY.entity.anim.animFrame) {
			isClose = partnerWillRefuseItem();
			if (isClose == 0) {
				startAnimation(
					&PARTNER_ENTITY.digimonEntity.entity,
					8);
				PARTNER_SUB_STATE = 5;
			} else {
				startAnimation(
					&PARTNER_ENTITY.digimonEntity.entity,
					13);
				PARTNER_SUB_STATE = 7;
			}
		}
		break;
	case 5:
		if ((PARTNER_ENTITY.digimonEntity.entity.anim.animFrame ==
		     11) &&
		    (PARTNER_ENTITY.digimonEntity.entity.anim.animId == 8)) {
			removeObject(0xfa7, 0);
		}
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame ==
		    15) {
			startAnimation(&TAMER_ENTITY.entity, 0);
		}
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame >=
		    PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			PARTNER_SUB_STATE = 6;
		}
		break;
	case 6:
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame >=
		    PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			type = TAMER_ITEM.worldItem.type;
			PARTNER_STATE = 1;

			setTamerState(0);
			setCameraFollowPlayer();

			if (ITEM_FUNCTIONS[TAMER_ITEM.worldItem.type] !=
			    NULL) {
				ITEM_FUNCTIONS[TAMER_ITEM.worldItem.type](
					TAMER_ITEM.worldItem.type);
			}

			removeOneSelectedItem();
			removeTamerItem();
			partnerHandleFoodFeed(type);
		}
		break;
	case 7:
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame >=
		    PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			removeObject(0xfa7, 0);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
			               0);
			startAnimationTamer(4);
			PARTNER_SUB_STATE = 8;
		}
		break;
	case 8:
		if (TAMER_ENTITY.entity.anim.animFrame >=
		    TAMER_ENTITY.entity.anim.frameCount) {
			setTamerState(0);
			PARTNER_STATE = 1;
			removeTamerItem();
			setCameraFollowPlayer();
		}
		break;
	default:
		break;
	}
}

void tickPartnerToilet(void)
{
	VECTOR *location;
	int16_t toiletId;
	int32_t finished;

	location = &PARTNER_ENTITY.digimonEntity.entity.posData->location;
	toiletId = MAP_ENTRIES[CURRENT_SCREEN].toiletId - 1;

	switch (PARTNER_SUB_STATE) {
	case 0:
		setTamerState(6);
		unsetCameraFollowPlayer();
		TOILET_POS1.vx = TOILET_DATA[toiletId].posX1;
		TOILET_POS1.vy = location->vy;
		TOILET_POS1.vz = TOILET_DATA[toiletId].posY1;
		TOILET_POS2.vx = TOILET_DATA[toiletId].posX2;
		TOILET_POS2.vy = location->vy;
		TOILET_POS2.vz = TOILET_DATA[toiletId].posY2;
		createCameraMovement(&TOILET_POS2, 20);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		PARTNER_SUB_STATE = 1;
		break;
	case 1:
		entityLookAtLocation(&TAMER_ENTITY.entity, location);
		finished = tickEntityWalkTo(0xfc, 0xff, TOILET_POS1.vx,
		                            TOILET_POS1.vz, 0);
		if (finished == 1) {
			PARTNER_SUB_STATE = 2;
		}
		break;
	case 2:
		entityLookAtLocation(&TAMER_ENTITY.entity, location);
		finished = tickEntityWalkTo(0xfc, 0xff, TOILET_POS2.vx,
		                            TOILET_POS2.vz, 0);
		if (finished == 1) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
			               10);
			PARTNER_SUB_STATE = 3;
		}
		break;
	case 3:
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame >=
		    PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			handleToilet();
			createCameraMovement(
				&TAMER_ENTITY.entity.posData->location, 20);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
			               2);
			PARTNER_SUB_STATE = 4;
		}
		break;
	case 4:
		entityLookAtLocation(
			&TAMER_ENTITY.entity,
			(VECTOR *)&location); // BUG: this shouldn't be a
		                              // pointer?
		finished = tickEntityWalkTo(0xfc, 0xff, TOILET_POS1.vx,
		                            TOILET_POS1.vz, 0);
		if (finished == 1) {
			SOME_SCRIPT_SYNC_BIT = 1;
		}
	default:
		break;
	}
}

void partnerWildPoop(void)
{
	int32_t closeness;
	short tileX;
	short tileY;

	switch (PARTNER_SUB_STATE) {
	case 0:
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		setTamerState(6);
		entityLookAtLocation(&TAMER_ENTITY.entity,
		                     &PARTNER_ENTITY.digimonEntity.entity
		                              .posData->location);
		unsetCameraFollowPlayer();
		PARTNER_SUB_STATE = 1;
		break;
	case 1:
		entityLookAtLocation(&PARTNER_ENTITY.digimonEntity.entity,
		                     &TAMER_ENTITY.entity.posData->location);
		closeness = getPartnerTamerCloseness();
		if (closeness > 0) {
			startAnimation(ENTITY_TABLE[1], 10);
			PARTNER_SUB_STATE = 2;
		}
		break;
	case 2:
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame >=
		    PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			getModelTile(&PARTNER_ENTITY.digimonEntity.entity
			                      .posData->location,
			             &tileX, &tileY);
			WILD_POOP_ID = createPoopPile(tileX, tileY);
			handleWildPoop();
			startAnimation(ENTITY_TABLE[1], 12);
			PARTNER_SUB_STATE = 3;
		}
		break;
	case 3:
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame >=
		    PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			setTamerState(0);
			PARTNER_STATE = 1;
			setCameraFollowPlayer();
			addTamerLevel(1, -1);
		}
		break;
	default:
		break;
	}
}

void tickPartnerDying(void)
{
	int32_t closeness;
	int32_t value;

	switch (PARTNER_SUB_STATE) {
	case 0:
		deinitializeFishing();
		removeTriangleMenu();
		closeInventoryBoxes();
		removeUIBox1();
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
		entityLookAtLocation(&TAMER_ENTITY.entity,
		                     &PARTNER_ENTITY.digimonEntity.entity
		                              .posData->location);
		unsetCameraFollowPlayer();
		loadDynamicLibrary(9, 0, 0, 0, 0);
		loadMapSounds2(19);
		isSoundLoaded(0, 8);
		PARTNER_SUB_STATE = 1;
		break;
	case 1:
		entityLookAtLocation(
			&PARTNER_ENTITY.digimonEntity.entity,
			&(TAMER_ENTITY.entity.posData)->location);
		closeness = getPartnerTamerCloseness();
		if (closeness > 0) {
			isSoundLoaded(0, 8);
			DOOA_tick((PartnerEntity *)ENTITY_TABLE[1],
			          GENERAL_BUFFER_PTR + 0x4b000, 0);
			setFishingDisabled();
			setTamerState(6);
			unsetCameraFollowPlayer();
			PARTNER_SUB_STATE = 2;
		}
		break;
	case 2:
		value = DOOA_tick((PartnerEntity *)ENTITY_TABLE[1],
		                  GENERAL_BUFFER_PTR + 0x4b000, 1);
		if (value == -1) {
			setFishingEnabled();
			PARTNER_PARA.remainingLifetime = 360;
			PARTNER_PARA.sicknessTimer = 0;
			PARTNER_PARA.injuryTimer = 0;
			STORED_TAMER_POS =
				TAMER_ENTITY.entity.posData->location;
			SOME_SCRIPT_SYNC_BIT = 1;
		}
		break;
	default:
		break;
	}
}

void partnerEatShit(void)
{
	int16_t tileX;
	int16_t tileY;
	int32_t finished;

	tileX = (WORLD_POOP[POOP_TO_EAT].x - 50) * 100 + 50;
	tileY = (50 - WORLD_POOP[POOP_TO_EAT].y) * 100 - 50;

	switch (PARTNER_SUB_STATE) {
	case 0:
		startAnimationTamer(0);
		setTamerState(6);
		unsetCameraFollowPlayer();
		startAnimation(ENTITY_TABLE[1], 2);
		tickPartnerWaypoints();
		PARTNER_SUB_STATE = 1;
		break;
	case 1:
		entityLookAtLocation(ENTITY_TABLE[0],
		                     &PARTNER_ENTITY.digimonEntity.entity
		                              .posData->location);
		finished = tickEntityWalkTo(0xfc, 0xff, tileX, tileY, 0);
		if (finished == 1) {
			startAnimation(ENTITY_TABLE[1], 8);
			PARTNER_SUB_STATE = 3;
		}
		break;
	case 3:
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame >=
		    PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			handleEatingPoop();
			setPartnerState(1);
			startAnimation(ENTITY_TABLE[1], 0);
			setTamerState(0);
			setCameraFollowPlayer();
		}
		break;
	default:
		break;
	}
}

void tickPartnerIdle(void)
{
	if (PARTNER_SUB_STATE == 0) {
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
		PARTNER_SUB_STATE = 1;
	}
}

void tickPartnerEvolving(void)
{
	int32_t closeness;
	int32_t soundId;
	int32_t value;

	switch (PARTNER_SUB_STATE) {
	case 0:
		stopGameTime();
		removeTriangleMenu();
		closeInventoryBoxes();
		removeUIBox1();
		setFishingDisabled();
		unsetCameraFollowPlayer();
		entityLookAtLocation(&TAMER_ENTITY.entity,
		                     &PARTNER_ENTITY.digimonEntity.entity
		                              .posData->location);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
		PARTNER_SUB_STATE = 1;
		break;
	case 1:
		entityLookAtLocation(&PARTNER_ENTITY.digimonEntity.entity,
		                     &TAMER_ENTITY.entity.posData->location);
		closeness = getPartnerTamerCloseness();
		if (closeness > 0) {
			getEvoSequenceState((PartnerEntity *)ENTITY_TABLE[1],
			                    GENERAL_BUFFER_PTR, &PARTNER_PARA,
			                    EVOLUTION_TARGET, 0);
			MAIN_D_80134E34 = 0;
			PARTNER_SUB_STATE = 2;
		}
		break;
	case 2:
		value = getEvoSequenceState((PartnerEntity *)ENTITY_TABLE[1],
		                            GENERAL_BUFFER_PTR, &PARTNER_PARA,
		                            EVOLUTION_TARGET, 1);
		if (value == -1) {
			startGameTime();
			EVOLUTION_TARGET = -1;
			soundId = getMapSoundId(CURRENT_SCREEN);
			loadMapSounds(soundId);
			checkShopMap(CURRENT_SCREEN);
			checkArenaMap(CURRENT_SCREEN);
			readMapTFS(CURRENT_SCREEN);
			setFishingEnabled();
			setPartnerState(1);
			if (SOME_SCRIPT_SYNC_BIT == 0) {
				SOME_SCRIPT_SYNC_BIT = 1;
			} else {
				setTamerState(0);
				setCameraFollowPlayer();
			}
		}
		break;
	default:
		break;
	}
}

void tickPartnerDying2(void)
{
	int32_t value;
	int32_t soundId;

	switch (PARTNER_SUB_STATE) {
	case 0:
		DOOA_getSequenceState(0, 0);
		PARTNER_SUB_STATE = 1;
		break;
	case 1:
		value = DOOA_getSequenceState(0, 1);
		if (value == -1) {
			soundId = getMapSoundId(CURRENT_SCREEN);
			loadMapSounds(soundId);
			readMapTFS(CURRENT_SCREEN);
			setFishingEnabled();
			PARTNER_PARA.remainingLifetime = 360;
			PARTNER_PARA.evoTimer = 0;
			PARTNER_PARA.sicknessTimer = 0;
			PARTNER_PARA.injuryTimer = 0;
			STORED_TAMER_POS =
				TAMER_ENTITY.entity.posData->location;
			SOME_SCRIPT_SYNC_BIT = 1;
			PARTNER_SUB_STATE = 2;
			writePStat(PSTAT_TIME_SPEED, 0);
		}
		break;
	default:
		break;
	}
}

void tickPartnerWalking(void)
{
	EntityAnim *anim;
	int32_t isUnhappy;
	int8_t closeness;
	int8_t collision;

	anim = &PARTNER_ENTITY.digimonEntity.entity.anim;
	closeness = getPartnerTamerCloseness();
	isUnhappy = PARTNER_PARA.condition & 0x10;

	if (closeness == 0) {
		if (((anim->animId == 2) || (anim->animId == 3)) &&
		    (isUnhappy == 0)) {
			if (anim->loopCount == 0xff) {
				anim->loopCount = 1;
			}
			if (anim->loopCount == 0) {
				PARTNER_ANIMATION = 4;
			}
		} else if ((anim->animId != 4) && (isUnhappy == 0)) {
			PARTNER_ANIMATION = 4;
		} else {
			if (isUnhappy == 0) {
				PARTNER_ANIMATION = 4;
			} else {
				PARTNER_ANIMATION = 2;
			}
			if (PARTNER_ANIMATION != anim->animId) {
				startAnimation(
					&PARTNER_ENTITY.digimonEntity.entity,
					(uint8_t)PARTNER_ANIMATION);
			}
		}
		PARTNER_IS_STANDING_STILL = 1;
	} else if (closeness == 1) {
		if (anim->animId == 4) {
			setPartnerSlowWalking();
		} else if ((anim->animId != 2) && (anim->animId != 3)) {
			if (anim->loopCount == 0xff) {
				anim->loopCount = 1;
			}
			if (anim->animFrame >= anim->frameCount) {
				setPartnerSlowWalking();
			}
		} else if (anim->loopCount == 0) {
			setPartnerSlowWalking();
		}

		EMOTION_ANIM_TIMEOUT = -1;
		PARTNER_IS_STANDING_STILL = 1;
	} else if (closeness == 2) {
		if ((anim->animId == 0) || (anim->animId == 1)) {
			if (PARTNER_IS_STANDING_STILL != 2) {
				updateConditionAnimation();
			}
		}

		if ((anim->animId > 1) && (anim->animId < 5)) {
			collision = entityCheckCollision(
				NULL, &PARTNER_ENTITY.digimonEntity.entity, 0,
				0);
			if (anim->loopCount == 0xff) {
				anim->loopCount = 1;
			}
			if ((anim->loopCount == 0) || (collision == 0)) {
				EMOTION_ANIM_TIMEOUT = random(5) + 1;
				setPartnerIdle();
				STOP_DISTANCE_TIMER = 0;
			}
		}

		if ((anim->animId == 0) || (anim->animId == 1)) {
			if (STOP_DISTANCE_TIMER >
			    anim->frameCount * EMOTION_ANIM_TIMEOUT) {
				updateConditionAnimation();
			}
		}

		if ((anim->animId >= 5) && (anim->animId < 8) &&
		    ((anim->animFlag & 1) != 1)) {
			EMOTION_ANIM_TIMEOUT = random(5) + 1;
			setPartnerIdle();
			STOP_DISTANCE_TIMER = 0;
		} else if ((anim->animId != 0) && (anim->animId != 1)) {
			if ((anim->animFlag & 1) != 1) {
				EMOTION_ANIM_TIMEOUT = random(5) + 1;
				setPartnerIdle();
				STOP_DISTANCE_TIMER = 0;
			}
		}

		STOP_DISTANCE_TIMER++;
		PARTNER_IS_STANDING_STILL = 2;
	}

	if (PARTNER_ANIMATION != anim->animId) {
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
		               (uint8_t)PARTNER_ANIMATION);
		STOP_DISTANCE_TIMER = 0;
	}

	if ((TAMER_ENTITY.entity.anim.animId == 2) &&
	    (getItemCount(0x25) != 0)) {
		if (HEALTH_SHOE_FRAMES >= 20) {
			PARTNER_ENTITY.digimonEntity.stats.current
				.currentHP += 5;
			PARTNER_ENTITY.digimonEntity.stats.current
				.currentMP += 5;

			if (PARTNER_ENTITY.digimonEntity.stats.current
			            .currentHP >
			    PARTNER_ENTITY.digimonEntity.stats.base.hp) {
				PARTNER_ENTITY.digimonEntity.stats.current
					.currentHP =
					PARTNER_ENTITY.digimonEntity.stats
						.base.hp;
			}
			if (PARTNER_ENTITY.digimonEntity.stats.current
			            .currentMP >
			    PARTNER_ENTITY.digimonEntity.stats.base.mp) {
				PARTNER_ENTITY.digimonEntity.stats.current
					.currentMP =
					PARTNER_ENTITY.digimonEntity.stats
						.base.mp;
			}
			HEALTH_SHOE_FRAMES = 0;
		}
		HEALTH_SHOE_FRAMES++;
	}
}

int32_t getPartnerTamerCloseness(void)
{
	int32_t distanceZ;
	VECTOR *tamerLocation;
	VECTOR *partnerLocation;
	int32_t distance;
	int32_t distanceX;
	int32_t sprintDistanceSquared;
	int32_t walkDistance;
	int32_t sprintDistance;

	tamerLocation = &TAMER_ENTITY.entity.posData->location;
	partnerLocation =
		&PARTNER_ENTITY.digimonEntity.entity.posData->location;
	distanceZ = (tamerLocation->vz - partnerLocation->vz) *
	            (tamerLocation->vz - partnerLocation->vz);
	distanceX = (tamerLocation->vx - partnerLocation->vx) *
	            (tamerLocation->vx - partnerLocation->vx);
	distance = distanceX + distanceZ;

	walkDistance =
		DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].radius;
	sprintDistance = walkDistance;
	walkDistance = (walkDistance * 5 / 2) * (walkDistance * 5 / 2);
	sprintDistanceSquared =
		(sprintDistance * 7 / 2) * (sprintDistance * 7 / 2);

	if (sprintDistanceSquared < distance) {
		return 0;
	}
	if (distance >= walkDistance) {
		return 1;
	}
	return 2;
}

void setPartnerSlowWalking(void)
{
	if (((PARTNER_PARA.condition & 0x1) != 0) ||
	    ((PARTNER_PARA.condition & 0x2) != 0) ||
	    ((PARTNER_PARA.condition & 0x20) != 0) ||
	    ((PARTNER_PARA.condition & 0x40) != 0)) {
		PARTNER_ANIMATION = 3;
	} else {
		PARTNER_ANIMATION = 2;
	}

	startAnimation(ENTITY_TABLE[1], (uint8_t)PARTNER_ANIMATION);
}

void updateConditionAnimation(void)
{
	int32_t cond;
	int32_t anim;
	int32_t v;

	v = (cond = PARTNER_PARA.condition);
	anim = PARTNER_ENTITY.digimonEntity.entity.anim.animId;
	if (v == 0) {
		if (PARTNER_PARA.happiness < -0x1E) {
			PARTNER_ANIMATION = 7;
		} else if (PARTNER_PARA.happiness >= 0x1F) {
			PARTNER_ANIMATION = 5;
		} else {
			setPartnerIdle();
		}
	} else if ((cond & 0x20) || (cond & 0x40)) {
		if (anim != 0xE) {
			PARTNER_ANIMATION = 0xE;
		}
	} else if (cond & 8) {
		if (anim != 0x10) {
			PARTNER_ANIMATION = 0x10;
		}
	} else if (cond & 4) {
		if (anim != 0xF) {
			PARTNER_ANIMATION = 0xF;
		}
	} else if (cond & 2) {
		if (anim != 0x12) {
			PARTNER_ANIMATION = 0x12;
		}
	} else if (cond & 1) {
		if (anim != 0x11) {
			PARTNER_ANIMATION = 0x11;
		}
	} else if (cond & 0x10) {
		if (anim != 0x13) {
			PARTNER_ANIMATION = 0x13;
		}
	}
}

void setPartnerIdle(void)
{
	if (((PARTNER_ANIMATION != 1) && (PARTNER_ANIMATION != 0)) ||
	    (PARTNER_IS_STANDING_STILL != 2)) {
		if ((((PARTNER_PARA.condition & 0x1) != 0) ||
		     ((PARTNER_PARA.condition & 0x2) != 0)) ||
		    (((PARTNER_PARA.condition & 0x20) != 0 ||
		      ((PARTNER_PARA.condition & 0x40) != 0)))) {
			PARTNER_ANIMATION = 1;
		} else {
			PARTNER_ANIMATION = 0;
		}

		startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
		               (uint8_t)PARTNER_ANIMATION);
	}
}

void setPartnerState(int8_t state)
{
	PARTNER_STATE = state;
	PARTNER_SUB_STATE = 0;
}

int32_t checkEatDistance(int32_t distance)
{
	int32_t tamerX;
	int32_t tamerZ;
	int32_t partnerX;
	int32_t partnerZ;
	int32_t targetDistance;
	int32_t partnerDistance;

	targetDistance = distance * 200 / 10 + 160;
	tamerX = (int16_t)TAMER_ENTITY.entity.posData->location.vx;
	tamerZ = (int16_t)TAMER_ENTITY.entity.posData->location.vz;
	partnerX = (int16_t)PARTNER_ENTITY.digimonEntity.entity.posData
	                   ->location.vx;
	partnerZ = (int16_t)PARTNER_ENTITY.digimonEntity.entity.posData
	                   ->location.vz;
	partnerDistance = (tamerX - partnerX) * (tamerX - partnerX) +
	                  (tamerZ - partnerZ) * (tamerZ - partnerZ);
	if (partnerDistance < 0) {
		partnerDistance = -partnerDistance;
	}
	if (targetDistance * targetDistance >= partnerDistance) {
		return 1;
	} else {
		return 0;
	}
}

void MAIN_func_800DF5A0(void)
{
	setTamerState(6);
	PARTNER_STATE = 8;
	PARTNER_SUB_STATE = 2;
	unsetCameraFollowPlayer();
}

int32_t getPartnerState(void)
{
	return PARTNER_STATE;
}

void startPartnerAnimation(int32_t animId)
{
	startAnimation(&PARTNER_ENTITY.digimonEntity.entity, (uint8_t)animId);
}

void callDigimonRoutine(int32_t routine)
{
	switch (routine) {
	case 0:
		setPartnerState(6);
		SOME_SCRIPT_SYNC_BIT = 0;
		break;
	case 1:
		KAR_start();
		setTamerState(12);
		setPartnerState(11);
		SOME_SCRIPT_SYNC_BIT = 0;
		break;
	case 2:
		EVOLUTION_TARGET = readPStat(PSTAT_SUKAMON_BACKUP_DIGIMON);
		setPartnerState(13);
		SOME_SCRIPT_SYNC_BIT = 0;
		break;
	case 3:
		setPartnerState(8);
		SOME_SCRIPT_SYNC_BIT = 0;
		break;
	case 4:
		setPartnerState(14);
		SOME_SCRIPT_SYNC_BIT = 0;
		break;
	case 5:
		setTamerState(15);
		setPartnerState(11);
		SOME_SCRIPT_SYNC_BIT = 0;
		break;
	case 6:
		setTamerState(16);
		setPartnerState(11);
		SOME_SCRIPT_SYNC_BIT = 0;
		break;
	case 7:
		setTamerState(17);
		setPartnerState(12);
		if ((TAMER_ENTITY.entity.anim.animId == 2) ||
		    (TAMER_ENTITY.entity.anim.animId == 3)) {
			startAnimation(&TAMER_ENTITY.entity, 0);
		}
		if ((PARTNER_ENTITY.digimonEntity.entity.anim.animId > 1) &&
		    (PARTNER_ENTITY.digimonEntity.entity.anim.animId < 5)) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
			               0);
		}
		SOME_SCRIPT_SYNC_BIT = 0;
		break;
	case 8:
		setTamerState(18);
		setPartnerState(11);
		SOME_SCRIPT_SYNC_BIT = 0;
		break;
	case 9:
		setTamerState(19);
		setPartnerState(12);
		if ((TAMER_ENTITY.entity.anim.animId == 2) ||
		    (TAMER_ENTITY.entity.anim.animId == 3)) {
			startAnimation(&TAMER_ENTITY.entity, 0);
		}
		if ((PARTNER_ENTITY.digimonEntity.entity.anim.animId > 1) &&
		    (PARTNER_ENTITY.digimonEntity.entity.anim.animId < 5)) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity,
			               0);
		}
		SOME_SCRIPT_SYNC_BIT = 0;
		break;
	case 10:
		EVOLUTION_TARGET = readPStat(PSTAT_254);
		setPartnerState(13);
		SOME_SCRIPT_SYNC_BIT = 0;
	default:
		break;
	}
}

int32_t MAIN_func_800DF7F8(void)
{
	return SOME_SCRIPT_SYNC_BIT;
}
