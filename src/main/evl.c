#include <dw/anim.h>
#include <dw/bubble.h>
#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/garbage.h>
#include <dw/graphics.h>
#include <dw/main.h>
#include <dw/utils.h>
#include <dw/world_object.h>

#include "common.h"

extern int8_t IS_LOAD_EVL_COMPLETE;

void stopBGM(void);
void stopSound(void);
void loadMapSounds2();
void isSoundLoaded();
void loadVLALL();

void* evl_functions[] = {
	evoSequenceAlwaysTrue,
	getEvoSequenceState,
	renderEvoSequenceLoading,
	tickEvoSequenceLoading
};
void tickEvoSequenceLoading(int32_t instanceId)
{
	EvoSequenceData *data;

	data = &EVO_SEQUENCE_DATA;

	switch (data->state) {
	case 0:
		if ((evoSequenceAlwaysTrue(500) == 1) && (data->timer > 55)) {
			data->state = 1;
			data->timer = 0;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 1);
			addConditionBubble(7, &data->partner->digimonEntity.entity);
		}
		break;
	case 1:
		if ((data->timer > 55) && (IS_LOAD_EVL_COMPLETE == 0)) {
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
	EvoSequenceData *data;

	data = &EVO_SEQUENCE_DATA;
}

GARBAGE(getEvoSequenceState, 20);

int32_t getEvoSequenceState(partner, buffer, para, target, isInitialized)
PartnerEntity *partner;
void *buffer;
PartnerPara *para;
int16_t target;
int16_t isInitialized;
{
	EvoSequenceData *data;
	int32_t instanceId;

	data = &EVO_SEQUENCE_DATA;
	instanceId = 0;
	if (isInitialized != 0) {
		return data->timer;
	}

	data->timer = 0;
	data->partner = partner;
	data->unk_0x8 = 0;
	data->state = 0;
	data->digimonId = EVOLUTION_STATS_GAINS[target].targetDigimon;
	data->para = para;
	data->evoTarget = target;
	data->heightFactor = (DIGIMON_DATA[data->digimonId].height << 12) /
			     DIGIMON_DATA[partner->digimonEntity.entity.type].height;

	startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
	stopSound();
	loadMapSounds2(18);
	isSoundLoaded(0, 8);
	loadVLALL(data->digimonId, GENERAL_BUFFER_PTR);
	loadDynamicLibrary(EVL_REL, (uint8_t *)&IS_LOAD_EVL_COMPLETE, 0, 0, 0);
	IS_LOAD_EVL_COMPLETE = 0;
	addObject(0x809, instanceId, tickEvoSequenceLoading, renderEvoSequenceLoading);

	return (int32_t)buffer;
}

int evoSequenceAlwaysTrue(int32_t unused)
{
	return 1;
}
