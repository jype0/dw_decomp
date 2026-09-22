#include <stdio.h>
#include <strings.h>

#include <libgs.h>
#include <mwinline_n.h>

#include <dw/anim.h>
#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/font.h>
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
void addEntityText(Entity *entity, int32_t a, int32_t b, int16_t value,
                   int32_t kind);
int32_t handleMedicineHealing(int32_t injuryChance, int32_t sicknessChance);
void handlePortaPotty(void);
void handleItemSickness(int16_t arg);
void addTamerLevel(int32_t chance, int32_t amount);

Inventory INVENTORY;
TamerItem TAMER_ITEM;
DroppedItem DROPPED_ITEMS[11];

int16_t HEAL_AMOUNTS[4] = { 500, 1500, 5000, 9999 };
uint8_t HEAL_EFFECT_VARIANT[4] = { 0, 0, 1, 1 };
char NAME_FORMAT[] = "%s";

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

static void handleEvoItems__garbage__(void)
{
	int32_t t0;
	int32_t t1;
	int32_t t2;
	int32_t t3;
	int32_t t4;
	int32_t t5;

	t0 = DROPPED_ITEMS[0].tileX;
	t1 = DROPPED_ITEMS[1].tileX;
	t2 = DROPPED_ITEMS[2].tileX;
	t3 = DROPPED_ITEMS[3].tileX;
	t4 = DROPPED_ITEMS[4].tileX;
	t5 = DROPPED_ITEMS[5].tileX;
	DROPPED_ITEMS[0].tileX = t0 + 1;
	DROPPED_ITEMS[1].tileX = t1 + 2;
	DROPPED_ITEMS[2].tileX = t2 + 3;
	DROPPED_ITEMS[3].tileX = t3 + 4;
	DROPPED_ITEMS[4].tileX = t4 + 5;
	DROPPED_ITEMS[5].tileX = t5 + 6;
}

void handleEvoItems(int16_t item)
{
	int32_t target;
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
		target = EVOLUTION_ITEM_TARGET[item - 0x47];
		level = DIGIMON_DATA[target].level - 1;
		if (level != DIGIMON_DATA[ENTITY_TABLE[1]->type].level) {
			return;
		}
		EVOLUTION_TARGET = EVOLUTION_ITEM_TARGET[item - 0x47];
	}
	HAS_USED_EVOITEM = 1;
	removeTamerItem();
	closeInventoryBoxes();
	setTamerState(6);
	setPartnerState(0xd);
}

void handleStatusItems(int32_t itemId)
{
	int32_t cured;

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
			cured = handleMedicineHealing(3, 2);
			if (cured == 1) {
				addHealingParticleEffect(ENTITY_TABLE[1], 0);
				return;
			}
			break;
		case 0xe:
			cured = handleMedicineHealing(3, 10);
			if (cured == 1) {
				addHealingParticleEffect(ENTITY_TABLE[1], 0);
			}
		}
	}
}

void handleChips(int32_t chipId)
{
	int16_t lifetime;
	int16_t off;
	int16_t hp;
	int16_t mp;
	int16_t def;
	int16_t speed;
	int16_t brain;
	int16_t zero;

	/* The ROM clears the six stat deltas through one shared temporary. */
	lifetime = 0;
	mp = zero = 0;
	hp = zero = 0;
	brain = zero = 0;
	speed = zero = 0;
	def = zero = 0;
	off = zero = 0;
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
	int16_t amount;

	if (GAME_STATE == 1) {
		BTL_removeDeathCountdown();
	}

	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP == 0) {
		startAnimation(ENTITY_TABLE[1], 0x2c);
	}

	if (type != 0xc) {
		if (type == 0xb) {
			amount = PARTNER_ENTITY.digimonEntity.stats.base.hp / 2;
		}
	} else {
		if (GAME_STATE == 1) {
			BTL_healStatusEffect(0);
		}

		amount = 0x270f;
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
			int16_t amount;
			addEntityText(ENTITY_TABLE[1], 0, 0xb, amount = HEAL_AMOUNTS[idx - 4], 2);
		}
		addHealingParticleEffect(ENTITY_TABLE[1], HEAL_EFFECT_VARIANT[idx - 4]);
	}
}

void handleHPHealingItem(uint8_t idx)
{
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP, HEAL_AMOUNTS[idx], PARTNER_ENTITY.digimonEntity.stats.base.hp);
		if (GAME_STATE == 1) {
			int16_t amount;
			addEntityText(ENTITY_TABLE[1], 0, 0xb, amount = HEAL_AMOUNTS[idx], 1);
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

void spawnDroppedItems(Entity *e, int32_t type)
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
	WorldItem *item;

	if (MAP_LAYER_ENABLED != 0) {
		renderOverworldItem(item = &DROPPED_ITEMS[instanceId].worldItem);
		renderDroppedItemShadow(item);
	}
}

static void spawnItem__garbage__(void)
{
	int32_t t0;
	int32_t t1;
	int32_t t2;
	int32_t t3;
	int32_t t4;
	int32_t t5;
	int32_t t6;
	int32_t t7;
	int32_t t8;
	int32_t t9;

	t0 = DROPPED_ITEMS[0].tileX;
	t1 = DROPPED_ITEMS[1].tileX;
	t2 = DROPPED_ITEMS[2].tileX;
	t3 = DROPPED_ITEMS[3].tileX;
	t4 = DROPPED_ITEMS[4].tileX;
	t5 = DROPPED_ITEMS[5].tileX;
	t6 = DROPPED_ITEMS[6].tileX;
	t7 = DROPPED_ITEMS[7].tileX;
	t8 = DROPPED_ITEMS[8].tileX;
	t9 = DROPPED_ITEMS[9].tileX;
	DROPPED_ITEMS[0].tileX = t0 + 1;
	DROPPED_ITEMS[1].tileX = t1 + 2;
	DROPPED_ITEMS[2].tileX = t2 + 3;
	DROPPED_ITEMS[3].tileX = t3 + 4;
	DROPPED_ITEMS[4].tileX = t4 + 5;
	DROPPED_ITEMS[5].tileX = t5 + 6;
	DROPPED_ITEMS[6].tileX = t6 + 7;
	DROPPED_ITEMS[7].tileX = t7 + 8;
	DROPPED_ITEMS[8].tileX = t8 + 9;
	DROPPED_ITEMS[9].tileX = t9 + 10;
}

void spawnItem(int32_t itemId, int16_t tileX, int16_t tileY)
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
	uint32_t depth;

	GsSetLsMatrix(&GsWSMATRIX);
	gte_ldv0(&item->spriteLocation);
	gte_rtps();
	gte_stsxy(&screen);
	gte_stszotz(&otz);
	width = (uint32_t)(VIEWPORT_DISTANCE << 7) / (depth = (uint32_t)(otz * 4));
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

int32_t getItemCount(int32_t type)
{
	int32_t i;
	uint8_t size;

	size = INVENTORY.size;
	for (i = 0; i < size; i++) {
		if (INVENTORY.types.array[i] == type) {
			return INVENTORY.amounts.array[i];
		}
	}

	return 0;
}

int32_t giveItem(uint32_t item, uint8_t amount)
{
	int16_t used[30];
	int32_t i;
	int32_t j;
	int32_t size;
	uint8_t *count;

	/* The ROM re-reads the inventory size on every pass of the first loop and
	 * reuses the last value read as the bound of the second; the volatile view
	 * is what keeps mwcc from hoisting that load. */
	for (i = 0; i < (size = ((volatile Inventory *)&INVENTORY)->size); i++) {
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
		}
	}

	for (i = 0; i < size; i++) {
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

	return 0;
}

void removeItem(int32_t type, uint32_t amount)
{
	int32_t i;
	uint8_t *count;
	uint32_t held;

	if (type == 0xff) {
		return;
	}

	for (i = 0; i < INVENTORY.size; i++) {
		if (INVENTORY.types.array[i] == type) {
			count = &INVENTORY.amounts.array[i];
			held = *count;
			if (amount < held) {
				*count = held - amount;
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
	int32_t *type;
	int32_t got;

	/* The ROM indexes the type field of element 0 by whole elements rather
	 * than addressing DROPPED_ITEMS[itemId] and reading the field. */
	type = &DROPPED_ITEMS[0].worldItem.type;
	got = giveItem((uint8_t)type[itemId * (sizeof(DroppedItem) / sizeof(int32_t))], 1);
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
	int16_t sicknessChance;
	int16_t healedHP;
	int16_t healedMP;
	int16_t addedHP;
	int16_t addedMP;
	int16_t trainDuration;
	int16_t discipline;
	int16_t lifetime;
	int16_t addedOffense;
	int16_t addedDefense;
	int16_t addedSpeed;
	int16_t addedBrain;
	int16_t trainFlag;
	int16_t trainValue;
	int16_t zero;
	int32_t effect;

	/* The shared zero preserves the ROM's signed-short initialization. */
	lifetime = 0;
	energy = tiredness = happiness = weight = discipline = zero = 0;
	addedBrain = 0;
	addedHP = addedMP = healedHP = healedMP = zero = 0;
	addedSpeed = zero = 0;
	addedDefense = zero = 0;
	addedOffense = zero = 0;
	trainValue = zero = 0;
	sicknessChance = 0;
	trainDuration = zero = 0;
	trainFlag = zero = 0;

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
		effect = 10;
		addedOffense = effect;
		addedDefense = effect;
		addedSpeed = effect;
		addedBrain = effect;
		addedMP = addedHP = 100;
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
		effect = 20;
		tiredness = effect;
		happiness = effect;
		discipline = effect;
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
		healedMP = healedHP = 1000;
		weight = 2;
		break;
	case 0x38:
		addedOffense = energy = 20;
		weight = 2;
		break;
	case 0x39:
		addedDefense = energy = 20;
		weight = 2;
		break;
	case 0x3a:
		addedSpeed = energy = 20;
		weight = 2;
		break;
	case 0x3b:
		addedBrain = energy = 20;
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
		effect = 1;
		addedOffense = effect;
		addedDefense = effect;
		addedSpeed = effect;
		addedBrain = effect;
		addedMP = addedHP = 10;
		weight = -2;
		break;
	case 0x42:
		energy = 49;
		weight = 5;
		break;
	case 0x43:
		energy = 35;
		healedMP = healedHP = 9999;
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
		effect = 30;
		happiness = effect;
		tiredness = effect;
		sicknessChance = effect;
		weight = 1;
		break;
	case 0x46:
		energy = 50;
		effect = 50;
		happiness = effect;
		tiredness = effect;
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

// clang-format off
void addWithLimit(value, amount, limit)
	int16_t *value;
	int16_t amount;
	int16_t limit;
// clang-format on
{
	*value += amount;
	if (*value > limit) {
		*value = limit;
	}
}

int32_t handleMedicineHealing(int32_t injuryChance, int32_t sicknessChance)
{
	int32_t roll;
	int32_t cured;

	if (((PARTNER_PARA.condition & CONDITION_INJURED) != 0) &&
	    (roll = random(3), (int16_t)roll < injuryChance)) {
		PARTNER_PARA.condition &= ~CONDITION_INJURED;
		PARTNER_PARA.injuryTimer = 0;
	}
	if (((PARTNER_PARA.condition & CONDITION_SICK) != 0) &&
	    (roll = random(10), (int16_t)roll < sicknessChance)) {
		PARTNER_PARA.condition &= ~CONDITION_SICK;
		PARTNER_PARA.sicknessTimer = 0;
		PARTNER_PARA.areaEffectTimer = 0;
		cured = 1;
	} else {
		cured = 0;
	}
	return cured;
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

void setTrainingBoost(int32_t flag, int32_t value, int32_t duration)
{
	PARTNER_PARA.trainBoostFlag = flag;
	PARTNER_PARA.trainBoostValue = value;
	PARTNER_PARA.trainBoostTimer = duration * 1200;
}

void handleItemSickness(int16_t chance)
{
	int32_t isSick;
	int16_t r;
	char buf[0x18];
	r = random(0x64);
	isSick = PARTNER_PARA.condition & CONDITION_SICK;
	if ((r < chance) && (!isSick)) {
		PARTNER_PARA.condition |= CONDITION_SICK;
		PARTNER_PARA.timesBeingSick++;
		PARTNER_PARA.sicknessTimer = 1;
		if (PARTNER_PARA.condition & CONDITION_INJURED) {
			PARTNER_PARA.condition &= ~CONDITION_INJURED;
			PARTNER_PARA.injuryTimer = 0;
		}
		setTamerState(0x14);
		clearTextArea();
		setTextColor(0xa);
		sprintf(buf, NAME_FORMAT, PARTNER_ENTITY.name);
		strcat(buf, IS_SICK_SUFFIX);
		drawString(buf, 0, 0x78);
	}
}
