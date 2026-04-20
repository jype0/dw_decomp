#include <memory.h>

#include <dw/clock.h>
#include <dw/fish.h>
#include <dw/trigger.h>
#include <dw/utils.h>
#include <dw/world_object.h>

typedef struct
{
	uint8_t name[20];
	int32_t value;
	int16_t meritValue;
	int16_t sortingValue;
	uint8_t itemColor;
	uint8_t droppable;
	uint16_t unk;
} Item;

extern uint8_t INVENTORY_ITEM_TYPES[];
extern Item ITEM_PARA[];

extern int32_t MAIN_D_801349E0;

int32_t isTriggerSet(uint16_t trigger);

int32_t getBestFishingRod(void)
{
	if (isTriggerSet(TRIGGER_HAVE_AMAZING_ROD) == 1) {
		return 2;
	}

	if (isTriggerSet(TRIGGER_HAVE_OLD_FISHROD) == 1) {
		return 1;
	}

	return 0;
}

int32_t hasFoodItem(void)
{
	int32_t i;
	for (i = 0; (uint32_t)i < 30u; i++) {
		if (INVENTORY_ITEM_TYPES[i] != 0xFF) {
			if (ITEM_PARA[INVENTORY_ITEM_TYPES[i]].sortingValue == 2) {
				return 1;
			}
		}
	}
	return 0;
}

void setFishingDisabled(void)
{
	if (FISHING_DATA_PTR != NULL) {
		FISHING_DATA_PTR->fishingEnabled = 1;
	}
}

void setFishingEnabled(void)
{
	if (FISHING_DATA_PTR != NULL) {
		FISHING_DATA_PTR->fishingEnabled = 0;
	}
}

int32_t hasFishingRod(void)
{
	int32_t bestRod;

	if (FISHING_DATA_PTR == NULL) {
		return 0;
	}

	if (FISHING_DATA_PTR->fishingEnabled != 0) {
		return 0;
	}

	if (getBestFishingRod() == 0 || hasFoodItem() == 0) {
		return 1;
	}

	return 2;
}

void initializeFishing(void)
{
	FISH_loadFishing(&FISHING_DATA_PTR->rod);
}

void tickFishing(void)
{
	VECTOR loc;
	if (FISHING_DATA_PTR != NULL) {
		loc = FISHING_DATA_PTR->hookedLocation;
		if (FISH_tamerTick() != 0) {
			setFishingDisabled();
			MAIN_func_800FF900(&loc);
		}
	}
}

int32_t isFishing(void)
{
	if (FISHING_DATA_PTR == NULL) {
		return 0;
	}

	if (FISHING_DATA_PTR->rod.fishingState == 0) {
		return 0;
	}

	return 1;
}

void deinitializeFishing(void)
{
	if (FISHING_DATA_PTR != NULL) {
		FISH_deinitialize();
		FISHING_DATA_PTR = NULL;
	}
}

void checkFishingMap(uint32_t mapId)
{
	int32_t i;
	uint8_t loadComplete;

	deinitializeFishing();
	if (mapId == 8 || mapId == 6) {
		loadDynamicLibrary(FISH_REL, &loadComplete, 0, NULL, NULL);

		for (i = 0; i < MAIN_D_801349E0; i++) {
			if (FISHING_MAP_DATA[i].mapId == mapId) break;
		}

		FISHING_DATA_PTR = &FISHING_DATA;
		if (FISHING_DATA_PTR != NULL) {
			bzero((uint8_t *)FISHING_DATA_PTR, 0x2D10);
			FISHING_DATA_PTR->header = 0x55DDDD55;
			FISHING_DATA_PTR->trailer = 0x55DDDD55;
			FISHING_DATA_PTR->mapId = mapId;
			FISHING_DATA_PTR->mapDataPtr = &FISHING_MAP_DATA[i];
			FISH_init();
		}

		hasFishingRod();
	}

	startGameTime();
}
