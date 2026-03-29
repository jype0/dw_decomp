#include <dw/evl.h>
#include <dw/world_object.h>

#include "common.h"
#include "dw/entity.h"

#pragma optimization_level 4

void stopBGM(void);
void EVL_initEvoSequence();
void startAnimation(Entity* entity, int32_t animId);
void addConditionBubble(int32_t bubbleType, Entity* entity);
void stopSound(void);
void loadMapSounds2();
void isSoundLoaded();
void loadVLALL();

#pragma section sdata begin
extern void *MAIN_D_801344F4;
extern int8_t EVL_LOADING_COMPLETE;
#pragma section sdata end

void tickEvoSequenceLoading(int32_t instanceId)
{
	EvoSequenceData *data;
	int iVar1;

	data = &EVO_SEQUENCE_DATA;

	switch (data->state) {
	case 0:
		iVar1 = evoSequenceAlwaysTrue(500);
		if ((iVar1 == 1) && (data->timer > 55)) {
			data->state = 1;
			data->timer = 0;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 1);
			addConditionBubble(7, &data->partner->digimonEntity.entity);
		}
		break;
	case 1:
		if ((data->timer > 55) && (EVL_LOADING_COMPLETE == 0)) {
			removeObject(0x809, instanceId);
			stopBGM();
			EVL_initEvoSequence();
			return;
		}
		break;
	default:
		break;
	}

	++data->timer;
	if (data->timer > 30000) {
		data->timer = 30000;
	}
}

void renderEvoSequenceLoading(int32_t instanceId)
{
}

INCLUDE_ASM("asm/main/nonmatchings/evl", getEvoSequenceState);

int evoSequenceAlwaysTrue(int32_t unused)
{
	return 1;
}
