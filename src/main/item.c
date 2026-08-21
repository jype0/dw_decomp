#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/item.h>
#include <dw/params.h>
#include <dw/particle.h>
#include <dw/script.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

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

typedef struct {
	uint8_t array[30];
} InventoryTable;

extern RaiseData RAISE_DATA[66];
extern TamerItem TAMER_ITEM;
extern TamerItem DROPPED_ITEMS[];
extern uint8_t INVENTORY_SIZE[];
extern uint8_t MAP_LAYER_ENABLED;
extern InventoryTable INVENTORY_ITEM_TYPES;
extern InventoryTable INVENTORY_ITEM_AMOUNTS;
extern InventoryTable INVENTORY_ITEM_NAMES;
extern InventoryTable DEFAULT_ITEM_AMOUNTS;
extern InventoryTable DEFAULT_ITEM_TYPES;

void setInventorySize(uint8_t size);
void deleteDroppedItem(int16_t itemId);
void removeTamerItem(void);
void decreasePoopLevel(void);
void addWithLimit(int16_t *value, int16_t amount, int16_t limit);
void modifyLifetime(int16_t delta);
void reduceTiredness(int16_t amount);
void setTrainingBoost(int32_t flag, int32_t value, int32_t duration);
void addEnergy(int16_t amount);
void addHappiness(int16_t amount);
void addDiscipline(int16_t amount);
void addWeight(int16_t amount);
int32_t getItemCount(uint8_t type);

void handleEvoItems(int16_t arg);
void handleFood(int16_t arg);
void handleChips(int32_t chipId);
void handleStatusItems(int32_t itemId);
void handleRestore(int16_t arg);
void handleDoubleFloppy(int32_t itemId);
void handleMPHealingItem(unsigned char idx);
void handleHPHealingItem(unsigned char idx);
void initializeDroppedItems(void);
void spawnDroppedItems(Entity *e, int32_t type);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void renderDroppedItem(int32_t instanceId);
void spawnItem(int32_t type, int16_t tileX, int16_t tileY);
void clearDroppedItems(void);
void renderOverworldItem(TamerItem *item);
void renderDroppedItemShadow(TamerItem *item);
int32_t pickupItem(int16_t itemId);
void handlePoopWeightLoss(int32_t type);
int32_t random(int32_t limit);
void clearTextArea(void);
void drawString(char *text, int32_t color, int32_t pos);
void setTextColor(int32_t color);
void setPartnerState(int8_t state);
void closeInventoryBoxes(void);
void BTL_healStatusEffect(int32_t arg);
void BTL_removeDeathCountdown(void);
void startAnimation(Entity *entity, uint8_t animId);
int32_t sprintf(char *dst, char *fmt, ...);
char *strcat(char *dst, char *src);
extern char MAIN_D_80134368;
extern char MAIN_D_80125F64[];
extern uint8_t MAIN_D_80127C5C[];
extern int16_t EVOLUTION_TARGET;
extern uint8_t HAS_USED_EVOITEM;
void addEntityText(Entity *entity, int32_t a, int32_t b, int16_t value,
		   int32_t kind);
extern int16_t MAIN_D_8013435C[4];
extern uint8_t MAIN_D_80134364[8];
extern int8_t GAME_STATE;
void initializeInventory(void);
int32_t handleMedicineHealing(int32_t injuryChance, int32_t sicknessChance);
void handlePortaPotty(void);
void handleItemSickness(int16_t arg);
void addTamerLevel(int32_t chance, int32_t amount);

void *item_order_anchor[] = {
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

INCLUDE_ASM("asm/main/nonmatchings/item", handleEvoItems);

INCLUDE_ASM("asm/main/nonmatchings/item", handleFood);

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
	int16_t s;

	lifetime = 0;
	mp = s = 0U;
	hp = s = 0U;
	brain = s = 0U;
	speed = s = 0U;
	def = s = 0U;
	off = s = 0U;
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
		lifetime = 0xffffffe8;
		break;
	case 0x1e:
		def = 100;
		speed = 100;
		lifetime = 0xffffffe8;
		break;
	case 0x1f:
		hp = 1000;
		mp = 1000;
		lifetime = 0xffffffe8;
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
		addTamerLevel(10, 0xffffffff);
	}
}

void handleRestore(int16_t type)
{
	int16_t amount;

	if (GAME_STATE == 1) {
		BTL_removeDeathCountdown();
	}

	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP == 0) {
		startAnimation(ENTITY_TABLE[1], 0x2C);
	}

	if (type != 0xC) {
		if (type == 0xB) {
			amount = PARTNER_ENTITY.digimonEntity.stats.base.hp / 2;
		}
	} else {
		if (GAME_STATE == 1) {
			BTL_healStatusEffect(0);
		}

		amount = 0x270F;
	}

	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP,
		     amount, PARTNER_ENTITY.digimonEntity.stats.base.hp);
	if (GAME_STATE == 1) {
		addEntityText(ENTITY_TABLE[1], 0, 0xB, amount, 1);
	}

	addHealingParticleEffect(ENTITY_TABLE[1], 1);
}

void handleDoubleFloppy(int32_t itemId)
{
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP,
			     0x5DC, PARTNER_ENTITY.digimonEntity.stats.base.hp);
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentMP,
			     0x5DC, PARTNER_ENTITY.digimonEntity.stats.base.mp);
		if (GAME_STATE == 1) {
			addEntityText(ENTITY_TABLE[1], 0, 0xB, 0x5DC, 1);
			addEntityText(ENTITY_TABLE[1], 0, 0xB, 0x5DC, 2);
		}
		addHealingParticleEffect(ENTITY_TABLE[1], 0);
	}
}

void handleMPHealingItem(unsigned char idx)
{
  if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0)
  {
    addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentMP, MAIN_D_8013435C[idx - 4], PARTNER_ENTITY.digimonEntity.stats.base.mp);
    if (GAME_STATE == 1)
    {
      int16_t new_var;
      addEntityText(ENTITY_TABLE[1], 0, 0xB, new_var = MAIN_D_8013435C[idx - 4], 2);
    }
    addHealingParticleEffect(ENTITY_TABLE[1], MAIN_D_80134364[idx - 4]);
  }
}

void handleHPHealingItem(unsigned char idx)
{
  if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0)
  {
    addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP, MAIN_D_8013435C[idx], PARTNER_ENTITY.digimonEntity.stats.base.hp);
    if (GAME_STATE == 1)
    {
      int16_t new_var;
      addEntityText(ENTITY_TABLE[1], 0, 0xB, new_var = MAIN_D_8013435C[idx], 1);
    }
    addHealingParticleEffect(ENTITY_TABLE[1], MAIN_D_80134364[idx]);
  }
}

void setInventorySize(uint8_t size)
{
	INVENTORY_SIZE[0] = size;
}

void initializeDroppedItems(void)
{
	int32_t i;

	TAMER_ITEM.worldItem.type = 0xFF;
	for (i = 0; i < 11; i++) {
		DROPPED_ITEMS[i].worldItem.type = 0xFF;
	}
}

void spawnDroppedItems(Entity *e, int32_t type)
{
	int32_t i;
	TamerItem *it;
	VECTOR *loc;

	loc = &e->posData->location;
	for (i = 0; i < 0xB; i++) {
		it = &DROPPED_ITEMS[i];
		if (it->worldItem.type == 0xFF) {
			it->worldItem.type = type;
			break;
		}
	}
	if (i != 0xB) {
		it->worldItem.spriteLocation.vx = loc->vx;
		it->worldItem.spriteLocation.vy = 0;
		it->worldItem.spriteLocation.vz = loc->vz;
		getModelTile(loc, (int16_t *)((char *)it + 0xC),
			     (int16_t *)((char *)it + 0xE));
		addObject(0x195, i, 0, renderDroppedItem);
	}
}

void renderDroppedItem(int32_t instanceId)
{
	TamerItem *item;

	if (MAP_LAYER_ENABLED != 0) {
		renderOverworldItem(item = &DROPPED_ITEMS[instanceId]);
		renderDroppedItemShadow(item);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/item", spawnItem);

void deleteDroppedItem(int16_t itemId)
{
	removeObject(0x195, itemId);
	DROPPED_ITEMS[itemId].worldItem.type = 0xFF;
}

void clearDroppedItems(void)
{
	int32_t i;

	for (i = 0; i < 11; i++) {
		if (DROPPED_ITEMS[i].worldItem.type != 0xFF) {
			deleteDroppedItem(i);
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/item", renderOverworldItem);

INCLUDE_ASM("asm/main/nonmatchings/item", renderDroppedItemShadow);

int32_t getItemCount(uint8_t type)
{
	int32_t i;
	uint8_t size;

	size = INVENTORY_SIZE[0];
	for (i = 0; i < size; i++) {
		if (INVENTORY_ITEM_TYPES.array[i] == type) {
			return INVENTORY_ITEM_AMOUNTS.array[i];
		}
	}

	return 0;
}

int32_t giveItem(uint32_t item, uint8_t amount)
{
	int16_t used[30];
	int32_t i;
	int32_t j;
	int32_t n;
	uint8_t *p;
	uint8_t *q;

	for (i = 0; i < (n = *(volatile uint8_t *)INVENTORY_SIZE); i++) {
		if (INVENTORY_ITEM_TYPES.array[i] == item) {
			q = &INVENTORY_ITEM_TYPES.array[i] + 0x1E;
			p = q - 0x1E;
			if (q[0] != 0x63) {
				q[0] += amount;
				if (q[0] >= 0x64) {
					q[0] = 0x63;
				}
				return 1;
			}
			return 0;
		}
	}
	for (i = 0; i < n; i++) {
		if (INVENTORY_ITEM_TYPES.array[i] == 0xFF) {
			p = &INVENTORY_ITEM_TYPES.array[i];
			p[0] = item;
			INVENTORY_ITEM_AMOUNTS.array[i] = amount;
			for (j = 0; j < INVENTORY_SIZE[0]; j++) {
				used[j] = 0;
			}
			for (j = 0; j < INVENTORY_SIZE[0]; j++) {
				if (INVENTORY_ITEM_NAMES.array[j] != 0xFF) {
					used[INVENTORY_ITEM_NAMES.array[j]] = 1;
				}
			}
			for (j = 0; j < INVENTORY_SIZE[0]; j++) {
				if (used[j] == 0) {
					INVENTORY_ITEM_NAMES.array[i] = j;
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
  int32_t new_var;
  int32_t i;
  uint8_t *new_var2;
  uint8_t *amt;
  if (type == 0xFF)
  {
    return;
  }
  for (i = 0; i < INVENTORY_SIZE[0]; i++)
  {
    if (INVENTORY_ITEM_TYPES.array[i] == type)
    {
      amt = (&INVENTORY_ITEM_TYPES.array[i]) + 0x1E;
      new_var2 = amt;
      new_var = amount < (*new_var2);
      if (new_var)
      {
        *amt = (*new_var2) - amount;
      }
      else
      {
        *new_var2 = 0;
        INVENTORY_ITEM_TYPES.array[i] = 0xFF;
        INVENTORY_ITEM_NAMES.array[i] = 0xFF;
      }
    }
  }

}

int32_t pickupItem(int16_t itemId)
{
	int32_t *tp;
	int32_t got;

	tp = &DROPPED_ITEMS->worldItem.type;
	got = giveItem((uint8_t)tp[itemId << 2], 1);
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

	for (i = 0; i < 0x1e; ++i) {
		INVENTORY_ITEM_TYPES.array[i] = 0xff;
		INVENTORY_ITEM_AMOUNTS.array[i] = 0;
		INVENTORY_ITEM_NAMES.array[i] = 0xff;
	}

	INVENTORY_SIZE[0] = 10;
	amounts = DEFAULT_ITEM_AMOUNTS;
	types = DEFAULT_ITEM_TYPES;

	for (i = 0; i < 0x1e; ++i) {
		INVENTORY_ITEM_TYPES.array[i] = types.array[i];
		INVENTORY_ITEM_AMOUNTS.array[i] = amounts.array[i];
		INVENTORY_ITEM_NAMES.array[i] = i;
	}

	INVENTORY_SIZE[0] = 0x1e;
}

void removeTamerItem(void)
{
	if (TAMER_ITEM.worldItem.type != 0xFF) {
		removeObject(0x194, 0);
		TAMER_ITEM.worldItem.type = 0xFF;
	}
}

void addWithLimit(int16_t *value, int16_t amount, int16_t limit)
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

	if (((PARTNER_PARA.condition & 0x20) != 0) &&
	    (roll = random(3), (int16_t)roll < injuryChance)) {
		PARTNER_PARA.condition &= 0xffffffdf;
		PARTNER_PARA.injuryTimer = 0;
	}
	if (((PARTNER_PARA.condition & 0x40) != 0) &&
	    (roll = random(10), (int16_t)roll < sicknessChance)) {
		PARTNER_PARA.condition &= 0xffffffbf;
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
	if (PARTNER_PARA.condition & 8) {
		PARTNER_PARA.poopLevel =
			RAISE_DATA[ENTITY_TABLE[1]->type].poopTimer;
		PARTNER_PARA.condition &= ~8;
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
  int32_t new_var;
  int16_t r;
  char buf[0x18];
  r = (int16_t) random(0x64);
  new_var = PARTNER_PARA.condition & 0x40;
  if ((r < chance) && (!new_var))
  {
    PARTNER_PARA.condition |= 0x40;
    PARTNER_PARA.timesBeingSick++;
    PARTNER_PARA.sicknessTimer = 1;
    if (PARTNER_PARA.condition & 0x20)
    {
      PARTNER_PARA.condition &= ~0x20;
      PARTNER_PARA.injuryTimer = 0;
    }
    setTamerState(0x14);
    clearTextArea();
    setTextColor(0xA);
    sprintf(buf, &MAIN_D_80134368, PARTNER_ENTITY.name);
    strcat(buf, MAIN_D_80125F64);
    drawString(buf, 0, 0x78);
  }
}
