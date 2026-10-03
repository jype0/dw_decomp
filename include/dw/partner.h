#ifndef DW_PARTNER_H
#define DW_PARTNER_H

#include <dw/types.h>

typedef struct {
	int8_t hungerTimes[8];
	uint8_t energyCap;
	uint8_t energyThreshold;
	uint8_t energyUsage;
	int16_t poopTimer;
	int16_t unk2;
	uint8_t poopSize;
	uint8_t favoriteFood;
	uint8_t sleepCycle;
	uint8_t favoredRegion;
	uint8_t trainingType;
	uint8_t defaultWeight;
	int16_t viewX;
	int16_t viewY;
	int16_t viewZ;
} RaiseData;

typedef struct {
	uint8_t map;
	uint8_t x;
	uint8_t y;
	int8_t size;
} PoopPile;

extern RaiseData RAISE_DATA[66];
extern PoopPile WORLD_POOP[];

void partnerTick(int32_t instanceId);
void updateConditionAnimation(void);
void partnerSetState(int32_t state);
void setSomeDyingState(void);
int32_t partnerGetState(void);
void partnerStartAnimation(int32_t animId);
void callDigimonRoutine(int32_t routine);
int32_t getScriptSyncBit(void);

#endif
