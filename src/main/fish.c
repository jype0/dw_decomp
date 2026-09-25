#include <memory.h>

#include <dw/clock.h>
#include <dw/fish.h>
#include <dw/item.h>
#include <dw/trigger.h>
#include <dw/utils.h>
#include <dw/world_object.h>

void MAIN_func_800FF900(VECTOR *v);
int32_t isTriggerSet(uint16_t trigger);

int8_t MAIN_D_80134958[6] = { 1, 2, 3, 4, 5, -1 };
int8_t MAIN_D_80134960[7] = { 0, 1, 2, 3, 4, 5, -1 };
int8_t MAIN_D_80134968[3] = { 4, 5, -1 };
int8_t MAIN_D_8013496C[5] = { 2, 3, 4, 5, -1 };
int16_t MAIN_D_80134974[4] = { -95, -64, 0, 0 };
SVECTOR MAIN_D_8013497C = { 28, 0, 2, 0 };
SVECTOR MAIN_D_80134984 = { 0, 93, 0, 0 };
SVECTOR MAIN_D_8013498C = { 28, 0, 2, 0 };
SVECTOR MAIN_D_80134994 = { 0, 17, 0, 0 };
int8_t MAIN_D_8013499C[8] = { 1, 2, 3, 4, 5, 4, 3, -1 };
uint8_t MAIN_D_801349A4[8] = { 39, 40, 41, 40, 39, 45, 44, 45 };
uint8_t MAIN_D_801349AC[4] = { 33, 34, 33, 35 };
uint8_t MAIN_D_801349B0[6] = { 41, 42, 40, 44, 43, 45 };
uint8_t MAIN_D_801349B8[6] = { 34, 36, 34, 35, 37, 35 };
SVECTOR MAIN_D_801349C0 = { 1024, 1024, 0, 0 };
SVECTOR MAIN_D_801349C8 = { 1, 19, -48, 0 };
SVECTOR MAIN_D_801349D0 = { 0 };
char FISH_TEXT_HOOKED[] = "Hooked!";
int32_t MAIN_D_801349E0 = 2;

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
		if (INVENTORY.types.array[i] != 0xFF) {
			if (ITEM_PARA[INVENTORY.types.array[i]].sortingValue == 2) {
				return 1;
			}
		}
	}
	return 0;
}

void setFishingDisabled(void)
{
	if (FISHING_DATA_PTR != NULL) {
		FISHING_DATA_PTR->fishingDisabled = 1;
	}
}

void setFishingEnabled(void)
{
	if (FISHING_DATA_PTR != NULL) {
		FISHING_DATA_PTR->fishingDisabled = 0;
	}
}

int32_t getFishingAvailability(void)
{
	if (FISHING_DATA_PTR == NULL) {
		return 0;
	}

	if (FISHING_DATA_PTR->fishingDisabled != 0) {
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
		loc = FISHING_DATA_PTR->hooked.fish.pos;
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
			FISHING_DATA_PTR->mapData = &FISHING_MAP_DATA[i];
			FISH_init();
		}

		getFishingAvailability();
	}

	startGameTime();
}
