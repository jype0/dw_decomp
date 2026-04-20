#include <dw/entity.h>
#include <dw/utils.h>

#include "common.h"

typedef struct {
	int32_t statusFxId;
	int32_t effectSlot[4];
	int32_t unk11;
	int16_t finisherGoal;
	int16_t finisherProgress;
	int16_t poisonTimer;
	int16_t confusionTimer;
	int16_t stunTimer;
	int16_t flatTimer;
	int16_t flatAttackTimer;
	int16_t invulnerableTimer;
	int16_t cooldown;
	int16_t senileTimer;
	int16_t unk15;
	int16_t hpDamageBuffer;
	int16_t mpDamageBuffer;
	int16_t speedBuffer;
	int16_t flags;
	uint8_t moveRange;
	uint8_t targetId;
	uint8_t queuedAnim;
	uint8_t buffsRemaining;
	uint8_t buffPrioTimer;
	uint8_t unk16;
	uint8_t table1[150];
	uint8_t table2[150];
} FighterData;

void damageTick(FighterData* fighter, Stats* stats);
void sortItemsById(uint8_t *data, int32_t count);

INCLUDE_ASM("asm/main/nonmatchings/utils", pauseFrame);

void damageTick(FighterData* fighter, Stats* stats)
{
	if (fighter->hpDamageBuffer > 999) {
		stats->currentHP -= 900;
		fighter->hpDamageBuffer -= 900;
	}

	if (fighter->hpDamageBuffer > 99) {
		stats->currentHP -= 80;
		fighter->hpDamageBuffer -= 80;
	}

	if (fighter->hpDamageBuffer > 9) {
		stats->currentHP -= 6;
		fighter->hpDamageBuffer -= 6;
	}

	if (fighter->hpDamageBuffer > 0) {
		stats->currentHP -= 1;
		fighter->hpDamageBuffer -= 1;
	}

	if (stats->currentHP <= 0) {
		stats->currentHP = 0;
		fighter->hpDamageBuffer = 0;
	}
}

void sortItemsById(uint8_t *data, int32_t count)
{
	int32_t i;
	int32_t j;
	int16_t minIdx;
	uint8_t minVal;
	uint8_t *dst;

	for (i = 0, dst = data; i < count; ++i, ++dst) {
		minVal = data[i];
		minIdx = i;

		for (j = i; j < count; ++j) {
			if (data[j] < minVal) {
				minIdx = j;
				minVal = data[j];
			}
		}

		swapByte(dst, &data[minIdx]);
	}
}

void swapByte(uint8_t *a, uint8_t *b)
{
	uint8_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void swapShort(int16_t *a, int16_t *b)
{
	int16_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void swapInt(int32_t *a, int32_t *b)
{
	int32_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

INCLUDE_ASM("asm/main/nonmatchings/utils", getEntityScreenPos);

INCLUDE_ASM("asm/main/nonmatchings/utils", MAIN_func_800E53B4);

INCLUDE_ASM("asm/main/nonmatchings/utils", initStringFT4);

INCLUDE_ASM("asm/main/nonmatchings/utils", renderNumber);

INCLUDE_ASM("asm/main/nonmatchings/utils", convertValueToDigits);

INCLUDE_ASM("asm/main/nonmatchings/utils", setUVDataPolyFT4);

INCLUDE_ASM("asm/main/nonmatchings/utils", setPosDataPolyFT4);

INCLUDE_ASM("asm/main/nonmatchings/utils", MAIN_func_800E5724);

INCLUDE_ASM("asm/main/nonmatchings/utils", MAIN_func_800E58A4);

INCLUDE_ASM("asm/main/nonmatchings/utils", renderSelectionCursor);

INCLUDE_ASM("asm/main/nonmatchings/utils", renderString);

INCLUDE_ASM("asm/main/nonmatchings/utils", renderItemSprite);

INCLUDE_ASM("asm/main/nonmatchings/utils", setItemTexture);

INCLUDE_ASM("asm/main/nonmatchings/utils", hasMove);

INCLUDE_ASM("asm/main/nonmatchings/utils", learnMove);

INCLUDE_ASM("asm/main/nonmatchings/utils", entityGetTechFromAnim);

INCLUDE_ASM("asm/main/nonmatchings/utils", entityLookAtTile);

INCLUDE_ASM("asm/main/nonmatchings/utils", MAIN_func_800E60E0);

INCLUDE_ASM("asm/main/nonmatchings/utils", isInvisible);

INCLUDE_ASM("asm/main/nonmatchings/utils", loadDynamicLibrary);

INCLUDE_ASM("asm/main/nonmatchings/utils", startMovie);

INCLUDE_ASM("asm/main/nonmatchings/utils", handlePause);

INCLUDE_ASM("asm/main/nonmatchings/utils", MAIN_func_800E642C);

INCLUDE_ASM("asm/main/nonmatchings/utils", createPauseBox);

INCLUDE_ASM("asm/main/nonmatchings/utils", renderPauseBox);

INCLUDE_ASM("asm/main/nonmatchings/utils", MAIN_thunk_func_800D92EC);

INCLUDE_ASM("asm/main/nonmatchings/utils", setMapLayerEnabled);

INCLUDE_ASM("asm/main/nonmatchings/utils", loadStackedTIMFile);

INCLUDE_ASM("asm/main/nonmatchings/utils", playMovie);

INCLUDE_ASM("asm/main/nonmatchings/utils", unlearnMove);

INCLUDE_ASM("asm/main/nonmatchings/utils", isTamerOnScreen);

INCLUDE_ASM("asm/main/nonmatchings/utils", MAIN_func_800E6818);

INCLUDE_ASM("asm/main/nonmatchings/utils", MAIN_func_800E6928);
