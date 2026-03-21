#include <libgte.h>
#include <dw/butterfly.h>
#include <dw/entity.h>
#include <dw/fade.h>
#include <dw/map.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/ui.h>

#include "common.h"

#pragma optimization_level 4

#pragma section sbss begin
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

extern uint8_t *GENERAL_BUFFER_PTR;

extern int8_t MAIN_STATE;
extern int8_t GAME_STATE;
extern uint8_t CURRENT_SCREEN;

extern int32_t IS_IN_MENU;
extern int32_t CURRENT_MENU;
extern int32_t TARGET_MENU;

extern int32_t MEMORY_CARD_ID;
extern int32_t MEMORY_CARD_SLOT;
extern int32_t CHECKED_MEMORY_CARD;
#pragma section sbss end

typedef struct
{
	SVECTOR spriteLocation;
	int32_t type;
} WorldItem;

typedef struct {
	WorldItem worldItem;
	int32_t time;
} TamerItem;

extern TamerItem TAMER_ITEM;
extern VECTOR STORED_TAMER_POS;

typedef void (*ItemFunction)(int16_t);
extern ItemFunction ITEM_FUNCTIONS[128];

extern int8_t ITEM_TAKE_DISTANCE[67];

void tickMainMenu(int32_t instanceId);
void renderMainMenu(int32_t instanceId);
void renderFeedingItem(int32_t instanceId);
void startAnimation(Entity *entity, uint8_t animId);

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
void updateConditionAnimation(void);
int32_t checkEatDistance(int32_t distance);
void setPartnerState(int8_t state);

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
		STD_func_800696E4(instanceId);
	default:
		break;
	}
}

void tickPartnerOverworld(int32_t instanceId)
{
	int32_t isOffScreen;

	if (IS_IN_MENU == 1) {
		tickAnimation(&PARTNER_ENTITY);
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

		isOffScreen = entityIsOffScreen(&PARTNER_ENTITY, 320, 240);
		PARTNER_ENTITY.digimonEntity.entity.isOnScreen =
			isOffScreen ^ 1;

		tickConditionBoundaries();
		tickAnimation(&PARTNER_ENTITY);
	}
}

void tickNormal(void)
{
	MAIN_func_800D4034();
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
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
		}
		setTamerState(6);
		tickPartnerWaypoints();
		PARTNER_SUB_STATE = 1;
		break;
	case 1:
		entityLookAtLocation(&TAMER_ENTITY,
                                     &PARTNER_ENTITY.digimonEntity.entity.posData->location);
		entityLookAtLocation(&PARTNER_ENTITY,
                                     &TAMER_ENTITY.entity.posData->location);
		closeness = getPartnerTamerCloseness();
		if (closeness == 2) {
			playSound(0, 15);
			startAnimation(&TAMER_ENTITY.entity, 8);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
			PARTNER_SUB_STATE = 2;
		}
		break;
	case 2:
		if (TAMER_ENTITY.entity.anim.animFrame >=
                    TAMER_ENTITY.entity.anim.frameCount) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 11);
			PARTNER_SUB_STATE = 3;
		}
		break;
	case 3:
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame >=
		    PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 9);
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
				}
				else {
					CURRENT_MENU = -2;
					TARGET_MENU = 40;
					MAIN_STATE = 1;
				}
				addObject(5000, 0, tickMainMenu, renderMainMenu);
				PARTNER_SUB_STATE = 6;
			}
		}
		break;
	case 6:
		if (CURRENT_MENU == -1) {
			removeObject(5000, 0);
			fadeFromBlack(40);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
			updateTimeOfDay();
			PARTNER_SUB_STATE = 7;
		}
		break;
	case 7:
		if (FADE_IN_CURRENT > 34) {
			PARTNER_STATE = 1;
			setTamerState(0);
			setCameraFollowPlayer();
			handleSpecialEvolutions(2, &PARTNER_ENTITY);
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
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
		}
		PARTNER_SUB_STATE = 1;
		MAIN_D_80134E28 = 0;
		break;
	case 1:
		++MAIN_D_80134E28;
		entityLookAtLocation(&TAMER_ENTITY,
                                     &PARTNER_ENTITY.digimonEntity.entity.posData->location);
		entityLookAtLocation(&PARTNER_ENTITY,
                                     &TAMER_ENTITY.entity.posData->location);
		closeness = getPartnerTamerCloseness();
		if (closeness == 2) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
			if (partnerState == 15) {
				playSound(0, 14);
				setTamerState(13);
			}
			else {
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
		if (PARTNER_ENTITY.digimonEntity.entity.anim.loopCount == 255) {
			PARTNER_ENTITY.digimonEntity.entity.anim.loopCount = 1;
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
		isClose = checkEatDistance(ITEM_TAKE_DISTANCE[PARTNER_ENTITY.digimonEntity.entity.type - 1]);
		if (isClose == 1) {
			PARTNER_SUB_STATE = 1;
		}
		else {
			PARTNER_SUB_STATE = 2;
		}
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
		PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy += 2048;
		tickPartnerWaypoints();
		break;
	case 1:
		entityLookAtLocation(&TAMER_ENTITY,
                                     &PARTNER_ENTITY.digimonEntity.entity.posData->location);
		isClose = checkEatDistance(ITEM_TAKE_DISTANCE[PARTNER_ENTITY.digimonEntity.entity.type - 1]);
		if (isClose == 0) {
			PARTNER_SUB_STATE = 3;
		}
		break;
	case 2:
		entityLookAtLocation(&TAMER_ENTITY,
                                     &PARTNER_ENTITY.digimonEntity.entity.posData->location);
		entityLookAtLocation(&PARTNER_ENTITY,
                                     &TAMER_ENTITY.entity.posData->location);
		isClose = checkEatDistance(ITEM_TAKE_DISTANCE[PARTNER_ENTITY.digimonEntity.entity.type - 1]);
		if (isClose == 1) {
			PARTNER_SUB_STATE = 3;
		}
		break;
	case 3:
		entityLookAtLocation(&PARTNER_ENTITY,
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
				startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 8);
				PARTNER_SUB_STATE = 5;
			}
			else {
				startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 13);
				PARTNER_SUB_STATE = 7;
			}
		}
		break;
	case 5:
		if ((PARTNER_ENTITY.digimonEntity.entity.anim.animFrame == 11) &&
		    (PARTNER_ENTITY.digimonEntity.entity.anim.animId == 8)) {
			removeObject(0xfa7, 0);
		}
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame == 15) {
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

			if (ITEM_FUNCTIONS[TAMER_ITEM.worldItem.type] != NULL) {
				ITEM_FUNCTIONS[TAMER_ITEM.worldItem.type](TAMER_ITEM.worldItem.type);
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
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
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
		entityLookAtLocation(&TAMER_ENTITY, location);
		finished = tickEntityWalkTo(0xfc, 0xff, TOILET_POS1.vx,
                                            TOILET_POS1.vz, 0);
		if (finished == 1) {
			PARTNER_SUB_STATE = 2;
		}
		break;
	case 2:
		entityLookAtLocation(&TAMER_ENTITY, location);
		finished = tickEntityWalkTo(0xfc, 0xff, TOILET_POS2.vx,
                                            TOILET_POS2.vz, 0);
		if (finished == 1) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 10);
			PARTNER_SUB_STATE = 3;
		}
		break;
	case 3:
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame >=
		    PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			handleToilet();
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 20);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
			PARTNER_SUB_STATE = 4;
		}
		break;
	case 4:
		entityLookAtLocation(&TAMER_ENTITY, &location);
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
		entityLookAtLocation(&TAMER_ENTITY,
                                     &PARTNER_ENTITY.digimonEntity.entity.posData->location);
		unsetCameraFollowPlayer();
		PARTNER_SUB_STATE = 1;
		break;
	case 1:
		entityLookAtLocation(&PARTNER_ENTITY,
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
			getModelTile(&PARTNER_ENTITY.digimonEntity.entity.posData->location,
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
		MAIN_func_800E4EB4();
		removeTriangleMenu();
		closeInventoryBoxes();
		removeUIBox1();
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
		entityLookAtLocation(&TAMER_ENTITY,
                                     &PARTNER_ENTITY.digimonEntity.entity.posData->location);
		unsetCameraFollowPlayer();
		loadDynamicLibrary(9, 0, 0, 0, 0);
		loadMapSounds2(19);
		isSoundLoaded(0, 8);
		PARTNER_SUB_STATE = 1;
		break;
	case 1:
		entityLookAtLocation(&PARTNER_ENTITY, &(TAMER_ENTITY.entity.posData)->location);
		closeness = getPartnerTamerCloseness();
		if (closeness > 0) {
			isSoundLoaded(0, 8);
			DOOA_tick(ENTITY_TABLE[1], GENERAL_BUFFER_PTR + 0x4b000, 0);
			setFishingDisabled();
			setTamerState(6);
			unsetCameraFollowPlayer();
			PARTNER_SUB_STATE = 2;
		}
		break;
	case 2:
		value = DOOA_tick(ENTITY_TABLE[1], GENERAL_BUFFER_PTR + 0x4b000, 1);
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
				     &PARTNER_ENTITY.digimonEntity.entity.posData->location);
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
		entityLookAtLocation(&TAMER_ENTITY,
                                     &PARTNER_ENTITY.digimonEntity.entity.posData->location);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
		PARTNER_SUB_STATE = 1;
		break;
	case 1:
		entityLookAtLocation(&PARTNER_ENTITY,
                                     &TAMER_ENTITY.entity.posData->location);
		closeness = getPartnerTamerCloseness();
		if (closeness > 0) {
			getEvoSequenceState(ENTITY_TABLE[1],
                                            GENERAL_BUFFER_PTR, &PARTNER_PARA,
                                            EVOLUTION_TARGET, 0);
			MAIN_D_80134E34 = 0;
			PARTNER_SUB_STATE = 2;
		}
		break;
	case 2:
		value = getEvoSequenceState(ENTITY_TABLE[1],
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
			}
			else {
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
		DOOA_func_80084810(0, 0);
		PARTNER_SUB_STATE = 1;
		break;
	case 1:
		value = DOOA_func_80084810(0, 1);
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
			writePStat(0, 0);
		}
		break;
	default:
		break;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/partner", tickPartnerWalking);

INCLUDE_ASM("asm/main/nonmatchings/partner", getPartnerTamerCloseness);

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

	startAnimation(ENTITY_TABLE[1], PARTNER_ANIMATION);
}

INCLUDE_ASM("asm/main/nonmatchings/partner", updateConditionAnimation);

void setPartnerIdle(void)
{
	if (((PARTNER_ANIMATION != 1) &&
	     (PARTNER_ANIMATION != 0)) ||
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
			       PARTNER_ANIMATION);
	}
}

void setPartnerState(int8_t state)
{
	PARTNER_STATE = state;
	PARTNER_SUB_STATE = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/partner", checkEatDistance);

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

void startPartnerAnimation(int8_t animId)
{
	startAnimation(&PARTNER_ENTITY.digimonEntity.entity, animId);
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
		EVOLUTION_TARGET = readPStat(5);
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
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
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
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
		}
		SOME_SCRIPT_SYNC_BIT = 0;
		break;
	case 10:
		EVOLUTION_TARGET = readPStat(254);
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
