#include <stdio.h>
#include <string.h>

#include <inline_n.h>
#include <libgs.h>

#include <dw/anim.h>
#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/font.h>
#include <dw/garbage.h>
#include <dw/item.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/particle.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/tamer.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

extern uint8_t MAP_LAYER_ENABLED;
extern InventoryTable DEFAULT_ITEM_AMOUNTS;
extern InventoryTable DEFAULT_ITEM_TYPES;
extern int32_t VIEWPORT_DISTANCE;
extern char IS_SICK_SUFFIX[];
extern uint8_t EVOLUTION_ITEM_TARGET[];
extern int16_t EVOLUTION_TARGET;
extern uint8_t HAS_USED_EVOITEM;

void deleteDroppedItem(int16_t itemId);
void setUVDataPolyFT4(POLY_FT4 *p, int32_t u, int32_t v, int32_t w, int32_t h);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width,
                       int32_t height);
void setItemTexture(POLY_FT4 *prim, int32_t type);
void decreasePoopLevel(void);
void modifyLifetime(int16_t delta);
void reduceTiredness(int16_t amount);
void setTrainingBoost(int32_t flag, int32_t value, int32_t duration);
void addEnergy(int16_t amount);
void addHappiness(int16_t amount);
void addDiscipline(int16_t amount);
void addWeight(int16_t amount);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void renderDroppedItem(int32_t instanceId);
void renderDroppedItemShadow(WorldItem *item);
void handlePoopWeightLoss(int32_t type);
void closeInventoryBoxes(void);
void BTL_healStatusEffect(int32_t arg);
void addEntityText(Entity *entity, int16_t slotId, int16_t color, int32_t value, uint8_t icon);
void addWithLimit(int16_t *value, int16_t amount, int16_t limit);
int32_t handleMedicineHealing(int16_t injuryChance, int16_t sicknessChance);
void handlePortaPotty(void);
void handleItemSickness(int16_t arg);
void addTamerLevel(int32_t chance, int32_t amount);

Inventory INVENTORY;
TamerItem TAMER_ITEM;
DroppedItem DROPPED_ITEMS[11];

int16_t HEAL_AMOUNTS[4] = { 500, 1500, 5000, 9999 };
uint8_t HEAL_EFFECT_VARIANT[4] = { 0, 0, 1, 1 };
#if !defined(VERSION_JP)
char NAME_FORMAT[] = "%s";
#endif

void *item_text_order[] = {
	handleItemSickness,
	setTrainingBoost,
	decreasePoopLevel,
	addWeight,
	addDiscipline,
	addHappiness,
	reduceTiredness,
	addEnergy,
	modifyLifetime,
	handlePortaPotty,
	handleMedicineHealing,
	addWithLimit,
	removeTamerItem,
	initializeInventory,
	pickupItem,
	removeItem,
	giveItem,
	getItemCount,
	renderDroppedItemShadow,
	renderOverworldItem,
	clearDroppedItems,
	deleteDroppedItem,
	spawnItem,
	renderDroppedItem,
	spawnDroppedItems,
	initializeDroppedItems,
	setInventorySize,
	handleHPHealingItem,
	handleMPHealingItem,
	handleDoubleFloppy,
	handleRestore,
	handleStatusItems,
	handleChips,
	handleFood,
	handleEvoItems,
};

static void *item_bss_order[] = {
	DROPPED_ITEMS,
	&TAMER_ITEM,
	&INVENTORY,
};

GARBAGE(handleEvoItems, 15);

void handleEvoItems(int16_t item)
{
	int16_t level;

	if (item >= 0x7d) {
		if (item == 0x7d) {
			EVOLUTION_TARGET = 0x40;
		}
		if (item == 0x7e) {
			EVOLUTION_TARGET = 0x3f;
		}
		if (item == 0x7f) {
			EVOLUTION_TARGET = 0x41;
		}
	} else {
		level = DIGIMON_DATA[EVOLUTION_ITEM_TARGET[item - 0x47]].level - 1;
		if (level != DIGIMON_DATA[ENTITY_TABLE[1]->type].level) {
			return;
		}
		EVOLUTION_TARGET = EVOLUTION_ITEM_TARGET[item - 0x47];
	}
	HAS_USED_EVOITEM = 1;
	removeTamerItem();
	closeInventoryBoxes();
	tamerSetState(6);
	partnerSetState(0xd);
}

void handleStatusItems(int32_t itemId)
{
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		switch (itemId) {
		case 8:
			BTL_healStatusEffect(1);
			return;
		case 9:
			if (GAME_STATE == 1) {
				BTL_healStatusEffect(0);
			}
			handleDoubleFloppy(itemId);
			return;
		case 10:
			COMBAT_DATA_PTR->fighter[0].flags |= 0x100;
			break;
		case 0xd:
			if (handleMedicineHealing(3, 2) == 1) {
				addHealingParticleEffect(ENTITY_TABLE[1], 0);
				return;
			}
			break;
		case 0xe:
			if (handleMedicineHealing(3, 10) == 1) {
				addHealingParticleEffect(ENTITY_TABLE[1], 0);
			}
		}
	}
}

void handleChips(int16_t chipId)
{
	int16_t lifetime;
	int16_t off;
	int16_t def;
	int16_t speed;
	int16_t brain;
	int16_t hp;
	int16_t mp;

	off = def = speed = brain = hp = mp = lifetime = 0;
	switch (chipId) {
	case 0x16:
		if (IS_SCRIPT_PAUSED == 1) {
			removeTamerItem();
			callScriptSection(0, 0x4dd, 0);
		}
		break;
	case 0x17:
		off = 0x32;
		break;
	case 0x18:
		def = 0x32;
		break;
	case 0x19:
		brain = 0x32;
		break;
	case 0x1a:
		speed = 0x32;
		break;
	case 0x1b:
		hp = 500;
		break;
	case 0x1c:
		mp = 500;
		break;
	case 0x1d:
		off = 100;
		brain = 100;
		lifetime = -24;
		break;
	case 0x1e:
		def = 100;
		speed = 100;
		lifetime = -24;
		break;
	case 0x1f:
		hp = 1000;
		mp = 1000;
		lifetime = -24;
		break;
	case 0x20:
		handlePortaPotty();
		return;
	}
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.hp, hp, 9999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.mp, mp, 9999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.off, off, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.def, def, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.speed, speed, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.brain, brain, 999);
	modifyLifetime(lifetime);
	if ((0x1c < chipId) && (chipId < 0x20)) {
		addTamerLevel(10, -1);
	}
}

void handleRestore(int16_t type)
{
	CombatData *combat = COMBAT_DATA_PTR;
	uint16_t *flags = &combat->fighter[0].flags;
	int16_t amount;

	if (GAME_STATE == 1) {
		BTL_removeDeathCountdown();
	}

	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP == 0) {
		startAnimation(ENTITY_TABLE[1], 0x2c);
	}

	switch (type) {
	case 0xb:
		amount = PARTNER_ENTITY.digimonEntity.stats.base.hp / 2;
		break;
	case 0xc:
		if (GAME_STATE == 1) {
			BTL_healStatusEffect(0);
		}

		amount = 0x270f;
		break;
	}

	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP,
	             amount, PARTNER_ENTITY.digimonEntity.stats.base.hp);
	if (GAME_STATE == 1) {
		addEntityText(ENTITY_TABLE[1], 0, 0xb, amount, 1);
	}

	addHealingParticleEffect(ENTITY_TABLE[1], 1);
}

void handleDoubleFloppy(int32_t itemId)
{
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP,
		             0x5dc, PARTNER_ENTITY.digimonEntity.stats.base.hp);
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentMP,
		             0x5dc, PARTNER_ENTITY.digimonEntity.stats.base.mp);
		if (GAME_STATE == 1) {
			addEntityText(ENTITY_TABLE[1], 0, 0xb, 0x5dc, 1);
			addEntityText(ENTITY_TABLE[1], 0, 0xb, 0x5dc, 2);
		}
		addHealingParticleEffect(ENTITY_TABLE[1], 0);
	}
}

void handleMPHealingItem(uint8_t idx)
{
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentMP, HEAL_AMOUNTS[idx - 4], PARTNER_ENTITY.digimonEntity.stats.base.mp);
		if (GAME_STATE == 1) {
			addEntityText(ENTITY_TABLE[1], 0, 0xb, HEAL_AMOUNTS[idx - 4], 2);
		}
		addHealingParticleEffect(ENTITY_TABLE[1], HEAL_EFFECT_VARIANT[idx - 4]);
	}
}

void handleHPHealingItem(uint8_t idx)
{
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP, HEAL_AMOUNTS[idx], PARTNER_ENTITY.digimonEntity.stats.base.hp);
		if (GAME_STATE == 1) {
			addEntityText(ENTITY_TABLE[1], 0, 0xb, HEAL_AMOUNTS[idx], 1);
		}
		addHealingParticleEffect(ENTITY_TABLE[1], HEAL_EFFECT_VARIANT[idx]);
	}
}

void setInventorySize(uint8_t size)
{
	INVENTORY.size = size;
}

void initializeDroppedItems(void)
{
	int32_t i;

	TAMER_ITEM.worldItem.type = 0xff;
	for (i = 0; i < 11; i++) {
		DROPPED_ITEMS[i].worldItem.type = 0xff;
	}
}

void spawnDroppedItems(Entity *e, uint8_t type)
{
	int32_t i;
	DroppedItem *it;
	VECTOR *loc;

	loc = &e->posData->location;
	for (i = 0; i < 0xb; i++) {
		it = &DROPPED_ITEMS[i];
		if (it->worldItem.type == 0xff) {
			it->worldItem.type = type;
			break;
		}
	}
	if (i != 0xb) {
		it->worldItem.spriteLocation.vx = loc->vx;
		it->worldItem.spriteLocation.vy = 0;
		it->worldItem.spriteLocation.vz = loc->vz;
		getModelTile(loc, (int16_t *)((char *)it + 0xc),
		             (int16_t *)((char *)it + 0xe));
		addObject(0x195, i, 0, renderDroppedItem);
	}
}

void renderDroppedItem(int32_t instanceId)
{
	if (MAP_LAYER_ENABLED != 0) {
		renderOverworldItem(&DROPPED_ITEMS[instanceId].worldItem);
		renderDroppedItemShadow(&DROPPED_ITEMS[instanceId].worldItem);
	}
}

GARBAGE(spawnItem, 24);

void spawnItem(uint8_t itemId, int16_t tileX, int16_t tileY)
{
	int32_t i;

	for (i = 0; i < 0xb; i++) {
		if (DROPPED_ITEMS[i].worldItem.type == 0xff) {
			DROPPED_ITEMS[i].worldItem.type = itemId;
			DROPPED_ITEMS[i].tileX = tileX;
			DROPPED_ITEMS[i].tileY = tileY;
			DROPPED_ITEMS[i].worldItem.spriteLocation.vx =
				(tileX - 0x32) * 100 + 0x32;
			DROPPED_ITEMS[i].worldItem.spriteLocation.vy =
				ENTITY_TABLE[0]->posData->location.vy;
			DROPPED_ITEMS[i].worldItem.spriteLocation.vz =
				(0x32 - tileY) * 100 - 0x32;
			addObject(0x195, i, 0, renderDroppedItem);
			return;
		}
	}
}

void deleteDroppedItem(int16_t itemId)
{
	removeObject(0x195, itemId);
	DROPPED_ITEMS[itemId].worldItem.type = 0xff;
}

void clearDroppedItems(void)
{
	int32_t i;

	for (i = 0; i < 11; i++) {
		if (DROPPED_ITEMS[i].worldItem.type != 0xff) {
			deleteDroppedItem(i);
		}
	}
}

void renderOverworldItem(WorldItem *item)
{
	DVECTOR screen;
	int32_t otz;
	POLY_FT4 *prim;
	int16_t width;

	GsSetLsMatrix(&GsWSMATRIX);
	gte_ldv0(&item->spriteLocation);
	gte_rtps();
	gte_stsxy(&screen);
	gte_stszotz(&otz);
	width = (uint32_t)(VIEWPORT_DISTANCE << 7) / (uint32_t)(int32_t)(otz * 4);
	otz = otz >> 2;
	if ((0 < otz) && (otz < 0x1000)) {
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 320, 0);
		setItemTexture(prim, item->type);
		if (width >= 0x20) {
			setUVWH(prim, prim->u0, prim->v0, 0xf, 0xf);
		}
		setPosDataPolyFT4(prim, screen.vx - (width >> 1),
		                  screen.vy - (width >> 1), width, width);
		AddPrim(&ACTIVE_ORDERING_TABLE->org[otz], prim++);
		GsSetWorkBase((PACKET *)prim);
	}
}

void renderDroppedItemShadow(WorldItem *item)
{
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
	int32_t otz;
	POLY_FT4 *prim;
	int16_t px;
	int16_t pz;

	GsSetLsMatrix(&GsWSMATRIX);
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = getTPage(1, 2, 832, 256);
	setClut(prim, 0, 0x1e7);
	setUVDataPolyFT4(prim, 0x40, 0x80, 0x3f, 0x3f);
	setRGB0(prim, 0x30, 0x30, 0x30);
	px = item->spriteLocation.vx;
	pz = item->spriteLocation.vz;
	p0.vx = px - 100;
	p0.vy = 0;
	p0.vz = pz - 100;
	p1.vx = px + 100;
	p1.vy = 0;
	p1.vz = pz - 100;
	p2.vx = px - 100;
	p2.vy = 0;
	p2.vz = pz + 100;
	p3.vx = px + 100;
	p3.vy = 0;
	p3.vz = pz + 100;
	gte_ldv3(&p0, &p1, &p2);
	gte_rtpt();
	gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
	gte_stszotz(&otz);
	gte_ldv0(&p3);
	gte_rtps();
	gte_stsxy(&prim->x3);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[0xffd], prim++);
	GsSetWorkBase((PACKET *)prim);
}

// clang-format off
int32_t getItemCount(type)
	uint8_t type;
// clang-format on
{
	int32_t i;

	for (i = 0; i < INVENTORY.size; i++) {
		if (INVENTORY.types.array[i] == type) {
			return INVENTORY.amounts.array[i];
		}
	}

	return 0;
}

// clang-format off
int32_t giveItem(item, amount)
	uint8_t item;
	uint8_t amount;
// clang-format on
{
	int16_t used[30];
	int32_t i;
	int32_t j;
	uint8_t *count;
	int32_t result;

	result = 0;
	for (i = 0; i < INVENTORY.size; i++) {
		if (INVENTORY.types.array[i] == item) {
			count = &INVENTORY.amounts.array[i];
			if (*count != 99) {
				*count += amount;
				if (*count >= 100) {
					*count = 99;
				}
				return 1;
			}
			return 0;
			/* unreachable */
			INVENTORY.types.array[i] = 0xff;
			INVENTORY.amounts.array[i] = 0;
			INVENTORY.names.array[i] = 0xff;
		}
	}

	if (result == 0) {
		for (i = 0; i < INVENTORY.size; i++) {
			if (INVENTORY.types.array[i] == 0xff) {
				INVENTORY.types.array[i] = item;
				INVENTORY.amounts.array[i] = amount;
				for (j = 0; j < INVENTORY.size; j++) {
					used[j] = 0;
				}
				for (j = 0; j < INVENTORY.size; j++) {
					if (INVENTORY.names.array[j] != 0xff) {
						used[INVENTORY.names.array[j]] = 1;
					}
				}
				for (j = 0; j < INVENTORY.size; j++) {
					if (used[j] == 0) {
						INVENTORY.names.array[i] = j;
						break;
					}
				}
				return 1;
			}
		}
	}

	return result;
}

// clang-format off
void removeItem(type, amount)
	uint8_t type;
	uint8_t amount;
// clang-format on
{
	int32_t i;
	uint8_t *count;

	if (type == 0xff) {
		return;
	}

	for (i = 0; i < INVENTORY.size; i++) {
		if (INVENTORY.types.array[i] == type) {
			count = &INVENTORY.amounts.array[i];
			if (amount < *count) {
				*count -= amount;
			} else {
				*count = 0;
				INVENTORY.types.array[i] = 0xff;
				INVENTORY.names.array[i] = 0xff;
			}
		}
	}
}

int32_t pickupItem(int16_t itemId)
{
	uint8_t type;
	int32_t got;

	type = DROPPED_ITEMS[itemId].worldItem.type;
	got = giveItem(type, 1);
	if (got != 0) {
		deleteDroppedItem(itemId);
	}
	return got;
}

void initializeInventory(void)
{
	InventoryTable amounts;
	InventoryTable types;
	int32_t i;

	for (i = 0; i < 30; ++i) {
		INVENTORY.types.array[i] = 0xff;
		INVENTORY.amounts.array[i] = 0;
		INVENTORY.names.array[i] = 0xff;
	}

	INVENTORY.size = 10;
	amounts = DEFAULT_ITEM_AMOUNTS;
	types = DEFAULT_ITEM_TYPES;

	for (i = 0; i < 30; ++i) {
		INVENTORY.types.array[i] = types.array[i];
		INVENTORY.amounts.array[i] = amounts.array[i];
		INVENTORY.names.array[i] = i;
	}

	INVENTORY.size = 30;
}

void removeTamerItem(void)
{
	if (TAMER_ITEM.worldItem.type != 0xff) {
		removeObject(0x194, 0);
		TAMER_ITEM.worldItem.type = 0xff;
	}
}

void handleFood(int16_t itemId)
{
	int16_t energy;
	int16_t happiness;
	int16_t weight;
	int16_t tiredness;
	int16_t discipline;
	int16_t lifetime;
	int16_t addedHP;
	int16_t addedMP;
	int16_t healedHP;
	int16_t healedMP;
	int16_t addedOffense;
	int16_t addedDefense;
	int16_t addedSpeed;
	int16_t addedBrain;
	int16_t trainFlag;
	int16_t trainValue;
	int16_t trainDuration;
	int16_t sicknessChance;

	energy = tiredness = happiness = discipline = weight = lifetime = 0;
	addedHP = addedMP = healedHP = healedMP = addedOffense = addedDefense = addedSpeed = addedBrain = 0;
	trainFlag = trainValue = trainDuration = sicknessChance = 0;

	switch (itemId) {
	case 0x26:
		energy = 12;
		weight = 1;
		break;
	case 0x27:
		energy = 24;
		weight = 2;
		break;
	case 0x28:
		energy = 35;
		tiredness = 5;
		happiness = 3;
		weight = 3;
		break;
	case 0x29:
		energy = 10;
		trainFlag = 0x29;
		trainValue = 12;
		trainDuration = 6;
		weight = -2;
		break;
	case 0x2a:
		energy = 15;
		trainFlag = 0x16;
		trainValue = 12;
		trainDuration = 6;
		weight = 3;
		break;
	case 0x2b:
		energy = 9;
		tiredness = 50;
		weight = 1;
		break;
	case 0x2c:
		energy = 12;
		weight = 1;
		break;
	case 0x2d:
		energy = 19;
		discipline = 50;
		weight = 2;
		break;
	case 0x2e:
		energy = 38;
		addedOffense = 10;
		addedDefense = 10;
		addedSpeed = 10;
		addedBrain = 10;
		addedHP = 100;
		addedMP = 100;
		weight = 4;
		break;
	case 0x2f:
		energy = 22;
		trainFlag = 0x3f;
		trainValue = 15;
		trainDuration = 6;
		weight = 2;
		break;
	case 0x30:
		energy = 30;
		happiness = 50;
		weight = 3;
		break;
	case 0x31:
		energy = 25;
		tiredness = 20;
		happiness = 20;
		discipline = 20;
		weight = 2;
		break;
	case 0x32:
		energy = 40;
		weight = 4;
		break;
	case 0x33:
		energy = 100;
		weight = 10;
		break;
	case 0x34:
		energy = 20;
		healedHP = 9999;
		weight = 2;
		break;
	case 0x35:
		energy = 16;
		healedMP = 9999;
		weight = 2;
		break;
	case 0x36:
		energy = 33;
		weight = -5;
		break;
	case 0x37:
		energy = 24;
		healedHP = 1000;
		healedMP = 1000;
		weight = 2;
		break;
	case 0x38:
		energy = 20;
		addedOffense = 20;
		weight = 2;
		break;
	case 0x39:
		energy = 20;
		addedDefense = 20;
		weight = 2;
		break;
	case 0x3a:
		energy = 20;
		addedSpeed = 20;
		weight = 2;
		break;
	case 0x3b:
		energy = 20;
		addedBrain = 20;
		weight = 2;
		break;
	case 0x3c:
		energy = 20;
		addedHP = 200;
		weight = 2;
		break;
	case 0x3d:
		energy = 20;
		addedMP = 200;
		weight = 2;
		break;
	case 0x3e:
		energy = 8;
		weight = 1;
		break;
	case 0x3f:
		energy = 12;
		weight = 1;
		break;
	case 0x40:
		energy = 22;
		weight = 2;
		break;
	case 0x41:
		energy = 27;
		addedOffense = 1;
		addedDefense = 1;
		addedSpeed = 1;
		addedBrain = 1;
		addedHP = 10;
		addedMP = 10;
		weight = -2;
		break;
	case 0x42:
		energy = 49;
		weight = 5;
		break;
	case 0x43:
		energy = 35;
		healedHP = 9999;
		healedMP = 9999;
		lifetime = 3;
		sicknessChance = 20;
		weight = 4;
		break;
	case 0x44:
		energy = 30;
		sicknessChance = 100;
		weight = 2;
		break;
	case 0x45:
		energy = 15;
		happiness = 30;
		tiredness = 30;
		sicknessChance = 30;
		weight = 1;
		break;
	case 0x46:
		energy = 50;
		happiness = 50;
		tiredness = 50;
		lifetime = 20;
		sicknessChance = 5;
		weight = 3;
		break;
	case 0x79:
		healedMP = 1000;
		break;
	case 0x7a:
		energy = 32;
		healedHP = 1000;
		sicknessChance = 20;
		break;
	}

	if (itemId == RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].favoriteFood) {
		energy = energy * 14 / 10;
		happiness += 2;
	}
	addEnergy(energy);
	reduceTiredness(tiredness);
	addHappiness(happiness);
	addDiscipline(discipline);
	addWeight(weight);
	decreasePoopLevel();
	setTrainingBoost(trainFlag, trainValue, trainDuration);
	handleItemSickness(sicknessChance);
	PARTNER_PARA.remainingLifetime += lifetime;
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.hp, addedHP, 9999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.mp, addedMP, 9999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP, healedHP,
	             PARTNER_ENTITY.digimonEntity.stats.base.hp);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentMP, healedMP,
	             PARTNER_ENTITY.digimonEntity.stats.base.mp);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.off, addedOffense, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.def, addedDefense, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.speed, addedSpeed, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.brain, addedBrain, 999);
}

void addWithLimit(int16_t *value, int16_t amount, int16_t limit)
{
	*value += amount;
	if (*value > limit) {
		*value = limit;
	}
}

int32_t handleMedicineHealing(int16_t injuryChance, int16_t sicknessChance)
{
	int16_t roll;

	if (((PARTNER_PARA.condition & CONDITION_INJURED) != 0) && (roll = randomLimit(3), roll < injuryChance)) {
		PARTNER_PARA.condition &= ~CONDITION_INJURED;
		PARTNER_PARA.injuryTimer = 0;
	}
	if (((PARTNER_PARA.condition & CONDITION_SICK) != 0) && (roll = randomLimit(10), roll < sicknessChance)) {
		PARTNER_PARA.condition &= ~CONDITION_SICK;
		PARTNER_PARA.sicknessTimer = 0;
		PARTNER_PARA.areaEffectTimer = 0;
		return 1;
	}
	return 0;
}

void handlePortaPotty(void)
{
	if (PARTNER_PARA.condition & CONDITION_POOPY) {
		PARTNER_PARA.poopLevel =
			RAISE_DATA[ENTITY_TABLE[1]->type].poopTimer;
		PARTNER_PARA.condition &= ~CONDITION_POOPY;
		handlePoopWeightLoss(ENTITY_TABLE[1]->type);
	}
}

void modifyLifetime(int16_t delta)
{
	int16_t *lifetime = &PARTNER_PARA.remainingLifetime;

	*lifetime += delta;
	if (*lifetime < 0) {
		*lifetime = 0;
	}
}

void addEnergy(int16_t amount)
{
	PARTNER_PARA.energyLevel += amount;
	if (PARTNER_PARA.energyLevel >
	    RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyCap) {
		PARTNER_PARA.energyLevel =
			RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyCap;
	}
}

void reduceTiredness(int16_t amount)
{
	PARTNER_PARA.tiredness -= amount;
	if (PARTNER_PARA.tiredness <= 0) {
		PARTNER_PARA.tiredness = 0;
	}
}

void addHappiness(int16_t amount)
{
	PARTNER_PARA.happiness += amount;
	if (PARTNER_PARA.happiness >= 0x64) {
		PARTNER_PARA.happiness = 0x64;
	}
}

void addDiscipline(int16_t amount)
{
	PARTNER_PARA.discipline += amount;
	if (PARTNER_PARA.discipline >= 0x64) {
		PARTNER_PARA.discipline = 0x64;
	}
}

void addWeight(int16_t amount)
{
	PARTNER_PARA.weight += amount;
	if (PARTNER_PARA.weight >= 0x64) {
		PARTNER_PARA.weight = 0x63;
	}

	if (PARTNER_PARA.weight <= 0) {
		PARTNER_PARA.weight = 1;
	}
}

void decreasePoopLevel(void)
{
	PARTNER_PARA.poopLevel--;
}

void setTrainingBoost(flag, value, duration)
int16_t flag;
int16_t value;
int16_t duration;
{
	PARTNER_PARA.trainBoostFlag = flag;
	PARTNER_PARA.trainBoostValue = value;
	PARTNER_PARA.trainBoostTimer = duration * 1200;
}

void handleItemSickness(int16_t chance)
{
	int32_t isSick;
	int16_t r;
#if defined(VERSION_JP)
	int32_t halfLen;
#else
	char buf[0x18];
#endif

	r = randomLimit(0x64);
	isSick = PARTNER_PARA.condition & CONDITION_SICK;
	if ((r < chance) && (!isSick)) {
		PARTNER_PARA.condition |= CONDITION_SICK;
		PARTNER_PARA.timesBeingSick++;
		PARTNER_PARA.sicknessTimer = 1;
		if (PARTNER_PARA.condition & CONDITION_INJURED) {
			PARTNER_PARA.condition &= ~CONDITION_INJURED;
			PARTNER_PARA.injuryTimer = 0;
		}
		tamerSetState(0x14);
		clearTextArea();
		setTextColor(0xa);
#if defined(VERSION_JP)
		drawString(PARTNER_ENTITY.name, 0, 0x78);
		halfLen = strlen(PARTNER_ENTITY.name) / 2;
		setTextColor(1);
		drawString(IS_SICK_SUFFIX, halfLen * 12, 0x78);
#else
		sprintf(buf, NAME_FORMAT, PARTNER_ENTITY.name);
		strcat(buf, IS_SICK_SUFFIX);
		drawString(buf, 0, 0x78);
#endif
	}
}
