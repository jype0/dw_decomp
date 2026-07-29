#include <libgs.h>

#include <dw/types.h>
#include <dw/clock.h>
#include <dw/bubble.h>
#include <dw/butterfly.h>
#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/file.h>
#include <dw/item.h>
#include <dw/map_object.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/world_object.h>

#include "common.h"

extern TamerItem TAMER_ITEM;
extern int8_t PARTNER_ANIMATION;
extern int32_t NANIMON_TRIGGER;
extern uint8_t POOP_TO_EAT;

extern int32_t HAS_IMMORTAL_HOUR;
extern int8_t IMMORTAL_HOUR;
extern int32_t HAS_BUTTERFLY;
extern int16_t EVOLUTION_TARGET;
extern uint8_t CURRENT_POOP_ID;
extern uint8_t HAS_USED_EVOITEM;
extern int32_t IS_NATURAL_DEATH;
extern int8_t ITEM_SCOLD_FLAG;
extern int16_t STATUS_UI_OFFSET_X;

extern char MAIN_D_801225AC[];
extern uint8_t MAIN_D_80137C00[2048];
extern GsDOBJ2 POOP_OBJECT;
extern GsCOORDINATE2 POOP_COORDINATES;

extern int8_t PARTNER_STATE;
extern int8_t PARTNER_SUB_STATE;

typedef struct {
	int8_t hungerTimes[8];
	uint8_t energyCap;
	uint8_t energyThreshold;
	int8_t energyUsage;
	int16_t poopTimer;
	int16_t unk2;
	uint8_t poopSize;
	uint8_t favoriteFood;
	int8_t sleepCycle;
	int8_t favoredRegion;
	int8_t trainingType;
	int8_t defaultWeight;
	int16_t viewX;
	int16_t viewY;
	int16_t viewZ;
} RaiseData;

extern RaiseData RAISE_DATA[66];

extern int8_t STOP_DISTANCE_TIMER;
extern uint8_t CURRENT_SCREEN;
extern uint8_t MAP_LAYER_ENABLED;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[2];
extern SVECTOR POOP_ROTATION;

extern int8_t PARTNER_AREA_RESPONSE[];
extern uint16_t CURRENT_FRAME;
extern uint16_t LAST_HANDLED_FRAME;
extern int32_t MAIN_D_80134C6C;
extern int32_t MAIN_D_80134C70;
extern int32_t MAIN_D_80134C74;
extern int32_t BUTTERFLY_ID;
extern int32_t IS_SCRIPT_PAUSED;

void applyMMD(int32_t digimonType, int32_t entityType,
	      EvoModelData *modelData);
void initializeDigimonObject(int32_t type, int32_t instanceId,
			     TickFunction tick);
int32_t getTamerState(void);
void writePStat(int32_t id, int32_t value);
void callScriptSection(int32_t a, int32_t b, int32_t c);
void setEntityPosition(int32_t entityId, int32_t x, int32_t y, int32_t z);
void setEntityRotation(int32_t entityId, int32_t x, int32_t y, int32_t z);
void setupEntityMatrix(int32_t entityId);
void startAnimation(Entity *entity, int32_t animId);

void initializeStatusObjects(void);
void initializePoop(void);
void initializePartner(int32_t type, int32_t posX, int32_t posY,
		       int32_t posZ, int32_t rotX, int32_t rotY,
		       int32_t rotZ);
void setSleepTimes(PartnerPara *para, int32_t type);
void setFoodTimer(int16_t type);
void renderPoop(int32_t instanceId);
void initializeEvolvedPartner(int32_t type, int32_t posX, int32_t posY,
			      int32_t posZ, int32_t rotX, int32_t rotY,
			      int32_t rotZ);
void resetPartnerPara(PartnerPara *para, int32_t type);
void initializeReincarnatedPartner(int32_t type);
void setReincarnateStats(void);
void setupPartnerOnWarp(int32_t posX, int32_t posY, int32_t posZ,
			int32_t rotation);
void tickConditionBoundaries(void);
void tickSleepMechanics(void);
void tickUnhappinessMechanics(void);
void handlePraiseScold(int32_t state);
void handleSleeping(void);
void createPoopPile(int16_t tileX, int16_t tileY);
void sleepRegen(void);
void tickTirednessMechanics(void);
void partnerWillRefuseItem(void);
void tickHungerMechanics(void);
void handleConditionBubble(void);
void partnerHandleFoodFeed(int32_t itemType);
void tickPartnerPoopingMechanic(void);
void detectEdiblePoop(void);
void handleToilet(void);
void handlePoopWeightLoss(int32_t type);
void handleWildPoop(void);
void handleEatingPoop(void);
void tickSicknessMechanics(void);
void tickDeathCondition(void);
void skipHours(void);
void handlePostBattleTiredness(void);
void tickPartnerNormal(void);
void renderStatusBars(void);
void setImmortalHour(void);

void initializeStatusObjects(void)
{
	initializeConditionBubbles();

	initializeButterfly();
	HAS_BUTTERFLY = -1;

	initializePoop();

	EVOLUTION_TARGET = -1;
	CURRENT_POOP_ID = 0;
	HAS_USED_EVOITEM = 0;
	IS_NATURAL_DEATH = 0;
	ITEM_SCOLD_FLAG = 0;
	STATUS_UI_OFFSET_X = 75;
}

void initializePoop(void)
{
	char *buf;

	buf = (char *)MAIN_D_80137C00;
	readFile(MAIN_D_801225AC, buf);

	GsMapModelingData((unsigned long *)&buf[4]);
	GsLinkObject4((unsigned long)&buf[0xc], &POOP_OBJECT, 0);

	GsInitCoordinate2(NULL, &POOP_COORDINATES);
	POOP_OBJECT.attribute = 0;
	POOP_OBJECT.coord2 = &POOP_COORDINATES;
}

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", initializePartner);

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", setSleepTimes);

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", setFoodTimer);

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", renderPoop);

void initializeEvolvedPartner(int32_t type, int32_t posX, int32_t posY,
			      int32_t posZ, int32_t rotX, int32_t rotY,
			      int32_t rotZ)
{
	removeObject((int16_t)type, 1);
	applyMMD(type, 3, &EVO_SEQUENCE_DATA.modelData);

	ENTITY_TABLE[1] = (Entity *)&PARTNER_ENTITY;

	initializeDigimonObject(type, 1, tickPartner);

	setEntityPosition(1, posX, posY, posZ);
	setEntityRotation(1, rotX, rotY, rotZ);
	setupEntityMatrix(1);

	startAnimation(ENTITY_TABLE[1], 0);

	PARTNER_ENTITY.digimonEntity.stats.current.vabId = 4;
	STOP_DISTANCE_TIMER = 0;
	PARTNER_PARA.condition = 0;

	setSleepTimes(&PARTNER_PARA, type);
	resetPartnerPara(&PARTNER_PARA, type);

	HAS_IMMORTAL_HOUR = 0;
	IMMORTAL_HOUR = -1;
}

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", resetPartnerPara);

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", initializeReincarnatedPartner);

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", setReincarnateStats);

void setupPartnerOnWarp(int32_t posX, int32_t posY, int32_t posZ,
			int32_t rotation)
{
	setEntityPosition(1, posX, posY, posZ);
	setEntityRotation(1, PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vx,
			  rotation,
			  PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vz);
	setupEntityMatrix(1);

	startAnimation(ENTITY_TABLE[1], 0);

	PARTNER_STATE = 1;
	PARTNER_SUB_STATE = 0;
	PARTNER_PARA.areaEffectTimer = 0;
}

void tickConditionBoundaries(void)
{
	if (PARTNER_PARA.happiness > 100) {
		PARTNER_PARA.happiness = 100;
	}

	if (PARTNER_PARA.happiness < -100) {
		PARTNER_PARA.happiness = -100;
	}

	if (PARTNER_PARA.discipline > 100) {
		PARTNER_PARA.discipline = 100;
	}

	if (PARTNER_PARA.discipline < 0) {
		PARTNER_PARA.discipline = 0;
	}

	if (PARTNER_PARA.tiredness > 100) {
		PARTNER_PARA.tiredness = 100;
	}

	if (PARTNER_PARA.tiredness < 0) {
		PARTNER_PARA.tiredness = 0;
	}

	if (PARTNER_PARA.energyLevel >
	    RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyCap) {
		PARTNER_PARA.energyLevel =
			RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyCap;
	}

	if (PARTNER_PARA.energyLevel < 0) {
		PARTNER_PARA.energyLevel = 0;
	}

	if (PARTNER_PARA.weight > 99) {
		PARTNER_PARA.weight = 99;
	}

	if (PARTNER_PARA.weight < 1) {
		PARTNER_PARA.weight = 1;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", tickSleepMechanics);

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", tickUnhappinessMechanics);

void handlePraiseScold(int32_t state)
{
	int16_t oldDiscipline;
	int32_t oldDiscipline2;
	int16_t oldHappiness;
	int16_t disciplineDelta;
	int16_t happinessDelta;
	uint8_t level;

	oldDiscipline = PARTNER_PARA.discipline;
	oldHappiness = PARTNER_PARA.happiness;
	oldDiscipline2 = oldDiscipline;

	if (state == 4) {
		if (ITEM_SCOLD_FLAG == 1) {
			disciplineDelta = 8;
			ITEM_SCOLD_FLAG = 2;
		} else {
			disciplineDelta = 2;
			happinessDelta = -10;
		}

		PARTNER_PARA.refusedFavFood = 0;
		PARTNER_PARA.condition &= ~0x10;

		if (HAS_BUTTERFLY == 0) {
			unsetButterfly(BUTTERFLY_ID);
			HAS_BUTTERFLY = -1;
		}
	} else {
		disciplineDelta = -5;
		happinessDelta = (oldDiscipline / 10) + 2;
	}

	PARTNER_PARA.discipline += disciplineDelta;
	PARTNER_PARA.happiness += happinessDelta;

	if (state == 4) {
		level = DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level;
		NANIMON_TRIGGER = 0;
		if ((level == 3) &&
		    (oldDiscipline2 == 0) &&
		    (oldHappiness == -100)) {
			NANIMON_TRIGGER = 1;
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", handleSleeping);

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", createPoopPile);

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", sleepRegen);

void tickTirednessMechanics(void)
{
	if (((PARTNER_PARA.areaEffectTimer % 1200) == 0) && (PARTNER_PARA.areaEffectTimer > 0)) {
		if (PARTNER_AREA_RESPONSE[0] == 1) {
			PARTNER_PARA.happiness = PARTNER_PARA.happiness + 1;
			PARTNER_PARA.tiredness = PARTNER_PARA.tiredness - 2;
		}
		if (PARTNER_AREA_RESPONSE[0] == 2) {
			PARTNER_PARA.happiness = PARTNER_PARA.happiness - 1;
			PARTNER_PARA.tiredness = PARTNER_PARA.tiredness + 1;
		}
	}
	if (PARTNER_PARA.subTiredness >= 0x3C) {
		PARTNER_PARA.tiredness = PARTNER_PARA.tiredness + 1;
		if (PARTNER_PARA.tiredness >= 0x64) {
			PARTNER_PARA.tiredness = 0x64;
		}
		PARTNER_PARA.subTiredness = 0;
	}
	if (PARTNER_PARA.tiredness >= 0x32) {
		PARTNER_PARA.tirednessHungerTimer = PARTNER_PARA.tirednessHungerTimer + 1;
	} else {
		PARTNER_PARA.tirednessHungerTimer = 0;
	}
	if (PARTNER_PARA.tiredness >= 0x50) {
		PARTNER_PARA.condition |= 2;
		if (((CURRENT_FRAME % 100) == 0) && (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
			PARTNER_PARA.happiness = PARTNER_PARA.happiness - 2;
		}
	} else {
		PARTNER_PARA.condition &= ~2;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", partnerWillRefuseItem);

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", tickHungerMechanics);

void handleConditionBubble(void)
{
	int32_t newBubble;
	uint32_t cond;

	newBubble = -1;

	if ((PARTNER_STATE != 1) && (PARTNER_STATE != 0xa)) {
		return;
	}

	cond = PARTNER_PARA.condition;

	if ((cond & 0x40) && (MAIN_D_80134C6C != 2)) {
		newBubble = 2;
	}

	if ((cond & 0x20) && (MAIN_D_80134C6C != 6)) {
		newBubble = 6;
	}

	if ((cond & 0x08) && (MAIN_D_80134C6C != 1)) {
		newBubble = 1;
	}

	if ((cond & 0x04) && (MAIN_D_80134C6C != 0)) {
		newBubble = 0;
	}

	if ((cond & 0x02) && (MAIN_D_80134C6C != 4)) {
		newBubble = 4;
	}

	if ((cond & 0x01) && (MAIN_D_80134C6C != 3)) {
		newBubble = 3;
	}

	if ((cond & 0x10) && (newBubble == -1) &&
	    (HAS_BUTTERFLY != 0) && (cond == 0x10)) {
		unsetBubble(MAIN_D_80134C70);
		BUTTERFLY_ID = setButterfly(ENTITY_TABLE[1]);
		HAS_BUTTERFLY = 0;
		return;
	} else if ((newBubble != MAIN_D_80134C6C) && (newBubble != -1) &&
		   (MAIN_D_80134C74 >= 50)) {
		if (HAS_BUTTERFLY == 0) {
			unsetButterfly(BUTTERFLY_ID);
			HAS_BUTTERFLY = -1;
			PARTNER_PARA.condition &= ~0x10;
			PARTNER_ENTITY.digimonEntity.entity.anim.loopCount = 1;
		}

		unsetBubble(MAIN_D_80134C70);
		MAIN_D_80134C70 = addConditionBubble(newBubble, ENTITY_TABLE[1]);
		MAIN_D_80134C74 = 0;
		MAIN_D_80134C6C = newBubble;
	}

	MAIN_D_80134C74 += 1;
	if (MAIN_D_80134C74 >= 60) {
		MAIN_D_80134C6C = -1;
	}
}

void partnerHandleFoodFeed(int32_t itemType)
{
	int32_t type;

	if (PARTNER_PARA.condition & 4) {
		if (((itemType < 0x26) || (itemType >= 0x47)) &&
		    (itemType != 0x79) && (itemType != 0x7a)) {
			return;
		}

		type = PARTNER_ENTITY.digimonEntity.entity.type;

		if (!(RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyThreshold >
		      PARTNER_PARA.energyLevel)) {
			PARTNER_PARA.happiness += 5;
			PARTNER_PARA.discipline += 1;
			PARTNER_PARA.condition &= ~4;
			setFoodTimer(PARTNER_ENTITY.digimonEntity.entity.type);
			PARTNER_PARA.starvationTimer = 0;
		}

		if ((TAMER_ITEM.worldItem.type ==
		     RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].favoriteFood) ||
		    !(PARTNER_PARA.energyLevel <
		      RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyThreshold)) {
			PARTNER_ANIMATION = 0xb;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0xb);
		}

		return;
	}

	if (((itemType < 0x26) || (itemType >= 0x47)) &&
	    (itemType != 0x79) && (itemType != 0x7a)) {
		return;
	}

	PARTNER_PARA.happiness -= 3;
	PARTNER_PARA.discipline -= 2;
}

void tickPartnerPoopingMechanic(void)
{
	uint32_t isPoopy;

	if (getTamerState() != 0) {
		return;
	}

	isPoopy = PARTNER_PARA.condition & 8;

	if (!isPoopy && ((CURRENT_FRAME % 200) == 0) &&
	    (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
		PARTNER_PARA.poopLevel -= 1;
	}

	if (!isPoopy && (PARTNER_PARA.poopLevel <= 0)) {
		PARTNER_PARA.condition |= 8;
		PARTNER_PARA.poopingTimer =
			((PARTNER_PARA.discipline + 20) * 20 * 60) / 100;
	}

	if (isPoopy && (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
		PARTNER_PARA.poopingTimer -= 1;
	}

	if (isPoopy && (PARTNER_PARA.poopingTimer <= 0)) {
		setPartnerState(7);
		PARTNER_PARA.poopingTimer = -1;
		ITEM_SCOLD_FLAG = 1;
	}
}

void detectEdiblePoop(void)
{
	int16_t radius;
	int16_t tileX;
	int16_t tileZ;
	int16_t posX;
	int16_t posZ;
	int32_t i;

	if ((PARTNER_ENTITY.digimonEntity.entity.type != 0xb) &&
	    (PARTNER_ENTITY.digimonEntity.entity.type != 0x27)) {
		return;
	}

	for (i = 0; i < 100; ++i) {
		if ((WORLD_POOP[i].x == -1) ||
		    (WORLD_POOP[i].map != CURRENT_SCREEN)) {
			continue;
		}

		if (WORLD_POOP[i].size < 11) {
			radius = 200;
		} else {
			radius = 300;
		}

		tileX = ((WORLD_POOP[i].x - 50) * 100) + 50;
		tileZ = ((50 - WORLD_POOP[i].y) * 100) - 50;

		posX = TAMER_ENTITY.entity.posData->location.vx;
		posZ = TAMER_ENTITY.entity.posData->location.vz;

		if (((tileX - radius) < posX) && ((tileX + radius) > posX) &&
		    ((tileZ - radius) < posZ) && ((tileZ + radius) > posZ)) {
			POOP_TO_EAT = i;
			PARTNER_STATE = 9;
			PARTNER_SUB_STATE = 0;
			return;
		}
	}
}

void handleToilet(void)
{
	PARTNER_PARA.happiness += 2;
	PARTNER_PARA.discipline += 2;
	PARTNER_PARA.poopLevel =
		(RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].poopTimer +
		 RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].poopTimer);
	PARTNER_PARA.condition &= ~8;
	handlePoopWeightLoss(ENTITY_TABLE[1]->type);
}

void handlePoopWeightLoss(int32_t type)
{
	int32_t r;

	r = random(4);

	PARTNER_PARA.weight -= (r + RAISE_DATA[type].poopSize) / 4;
	if (PARTNER_PARA.weight < 1) {
		PARTNER_PARA.weight = 1;
	}
}

void handleWildPoop(void)
{
	PARTNER_PARA.condition &= ~8;
	PARTNER_PARA.careMistakes += 1;
	PARTNER_PARA.happiness -= 10;
	PARTNER_PARA.discipline -= 5;
	PARTNER_PARA.poopLevel =
		(RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].poopTimer +
		 RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].poopTimer);

	handlePoopWeightLoss(ENTITY_TABLE[1]->type);
}

void handleEatingPoop(void)
{
	Stats *stats;
	int16_t healingChance;
	int16_t stat;
	int16_t r;

	stats = &PARTNER_ENTITY.digimonEntity.stats;
	healingChance = 0;

	if (WORLD_POOP[POOP_TO_EAT].size < 11) {
		stats->current.currentHP += (int16_t)(stats->base.hp / 20);
		stats->current.currentMP += (int16_t)(stats->base.mp / 50);
		PARTNER_PARA.weight += 1;
		healingChance = 2;
	} else if (WORLD_POOP[POOP_TO_EAT].size < 14) {
		stats->current.currentHP += (int16_t)(stats->base.hp / 10);
		stats->current.currentMP += (int16_t)(stats->base.mp / 20);
		PARTNER_PARA.weight += 3;
		healingChance = 7;
	} else {
		stats->current.currentHP += (int16_t)(stats->base.hp / 2);
		stats->current.currentMP += (int16_t)(stats->base.mp / 10);
		PARTNER_PARA.weight += 10;
		healingChance = 20;
	}

	stat = stats->current.currentHP;
	if (stats->base.hp < stat) {
		stats->current.currentHP = stats->base.hp;
	}

	stat = stats->current.currentMP;
	if (stats->base.mp < stat) {
		stats->current.currentMP = stats->base.mp;
	}

	stat = PARTNER_PARA.weight;
	if (stat > 99) {
		PARTNER_PARA.weight = 99;
	}

	r = random(100);
	if (r < healingChance) {
		if ((PARTNER_PARA.condition & 0x20) != 0) {
			PARTNER_PARA.condition &= ~0x20;
			PARTNER_PARA.injuryTimer = 0;
		}

		if ((PARTNER_PARA.condition & 0x40) != 0) {
			PARTNER_PARA.condition &= ~0x40;
			PARTNER_PARA.sicknessTimer = 0;
			PARTNER_PARA.areaEffectTimer = 0;
		}
	}

	WORLD_POOP[POOP_TO_EAT].map = 0xff;
	WORLD_POOP[POOP_TO_EAT].x = -1;
	WORLD_POOP[POOP_TO_EAT].y = -1;
	WORLD_POOP[POOP_TO_EAT].size = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", tickSicknessMechanics);

void tickDeathCondition(void)
{
	if ((HAS_IMMORTAL_HOUR == 1) && (IMMORTAL_HOUR == HOUR)) {
		return;
	}

	if (PARTNER_PARA.remainingLifetime > 0) {
		return;
	}

	if (PARTNER_STATE == 8) {
		return;
	}

	if (getTamerState() != 0) {
		return;
	}

	if (IS_SCRIPT_PAUSED != 1) {
		return;
	}

	IS_NATURAL_DEATH = 1;

	writePStat(0xff, 0);
	PARTNER_PARA.remainingLifetime = 0;
	callScriptSection(0, 0x4de, 0);
}

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", skipHours);

void handlePostBattleTiredness(void)
{
	int16_t maxMP;
	int16_t currentMP;
	int16_t diff;
	int16_t diff2;

	PARTNER_PARA.tiredness += 5;

	maxMP = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	diff = (maxMP * 2) / 10;
	currentMP = PARTNER_ENTITY.digimonEntity.stats.current.currentMP;

	diff2 = diff;
	if (currentMP < (maxMP - diff)) {
		diff = maxMP - currentMP;
		/*
		 * NOTE: Vanilla MP factor calculation is broken.
		 * The factor should be ((diff * 10) / maxMP).
		 */
		PARTNER_PARA.tiredness += (diff / maxMP) * 10;
	}

	PARTNER_PARA.foodLevel -= 15;
}

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", tickPartnerNormal);

INCLUDE_ASM("asm/main/nonmatchings/partner_impl", renderStatusBars);

void setImmortalHour(void)
{
	HAS_IMMORTAL_HOUR = 1;
	IMMORTAL_HOUR = HOUR;
}
