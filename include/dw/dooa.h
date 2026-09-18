#ifndef DW_DOOA_H
#define DW_DOOA_H

#include <libgs.h>
#include <libgte.h>

#include <dw/efe.h>
#include <dw/entity.h>
#include <dw/types.h>

typedef struct {
	int16_t targetY;
	int16_t pad;
	VECTOR pos;
} DooaFlash;

typedef struct {
	int32_t frame;
	Entity *entity;
	int16_t phase;
	int16_t phaseInitPending;
	int16_t fadeLevel;
	int16_t eggSlot;
	DooaFlash flash;
	int32_t modelData[6];
	int16_t sparkleIndex;
	int8_t isModelLoading;
	int8_t unk_3F;
} DooaSequence;

typedef struct {
	int16_t state;
	int16_t prevState;
	Entity *entity;
	int32_t unk_08;
	void *shardBuffer;
	void *shardWrite;
	int32_t shardBytes;
	uint8_t colorR;
	uint8_t colorG;
	uint8_t colorB;
	int8_t flash;
} DooaShardEffect;

extern EfeFlashBuffer DOOA_FLASH_DATA;
extern GsRVIEW2 DOOA_CAMERA_START_VIEW;
extern VECTOR DOOA_SAVED_LOCATION;
extern uint8_t DOOA_SAVED_ENTITY_CLUT[];
extern int16_t DOOA_MODEL_CLUT[];
extern int16_t DOOA_SHARD_CLUT[];
extern GsRVIEW2 DOOA_SAVED_VIEW;
extern VECTOR DOOA_PARTNER_POSITION;
extern int16_t DOOA_SCENE_CLUT[];
extern int16_t DOOA_FADED_CLUT[];
extern GsOT DOOA_ORDERING_TABLE[];
extern int8_t DOOA_SAVED_ENTITY_VISIBILITY[];
extern DooaShardEffect DOOA_SHARD_EFFECT;
extern DooaSequence DOOA_REINCARNATION_SEQ;

extern int16_t EGG_DIGIMON_TYPES[4];
extern SVECTOR MAIN_D_80134BB4;
extern int8_t DOOA_ENTITIES_VISIBLE;

int32_t DOOA_getSequenceState(int32_t unused, int32_t isInitialized);
int32_t DOOA_tick(PartnerEntity *partner, void *buffer, int32_t isInitialized);

#endif
