#ifndef DW_DOO_H
#define DW_DOO_H

#include <libgte.h>

#include <dw/entity.h>
#include <dw/types.h>

typedef struct {
	int16_t targetY;
	int16_t pad;
	VECTOR pos;
} DooFlash;

typedef struct {
	int32_t frame;
	Entity *entity;
	int16_t phase;
	int16_t phaseInitPending;
	int16_t fadeLevel;
	int16_t eggSlot;
	DooFlash flash;
	uint8_t modelData[0x18];
	int16_t sparkleIndex;
	int8_t isModelLoading;
	int8_t unk_3F;
} DooSequence;

extern DooSequence DOOA_REINCARNATION_SEQ[];

extern int32_t DOOA_REINCARNATION_MODEL_DATA[];

int32_t DOOA_tick(PartnerEntity *partner, void *buffer, int32_t isInitialized);
int32_t DOOA_getSequenceState(int32_t unused, int32_t isInitialized);

void DOO2_openEggBox(void);
int32_t DOO2_tickEggInput(void);

#endif
