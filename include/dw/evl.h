#ifndef DW_EVL_H
#define DW_EVL_H

#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>

typedef struct {
	uint8_t* modelPtr;
	int32_t unk1;
	int32_t unk2;
	int32_t modelSize;
	uint8_t* imagePtr;
	int32_t imageSize;
} EvoModelData;

typedef struct {
	int32_t timer;
	PartnerEntity* partner;
	int16_t unk_0x8;
	int16_t digimonId;
	EvoModelData modelData;
	int32_t hasFinishedLoading;
	PartnerPara* para;
	int16_t evoTarget;
	int16_t state;
	int32_t heightFactor;
	int32_t unk_0x34;
	int32_t unk_0x38;
} EvoSequenceData;

typedef struct {
	int16_t hp;
	int16_t mp;
	int16_t offense;
	int16_t defense;
	int16_t speed;
	int16_t brains;
	int16_t targetDigimon;
} EvoStatsGains;

typedef struct {
  int8_t from[5];
  int8_t to[6];
} EvolutionPath;

typedef struct {
  int16_t digimon;
  int16_t hp;
  int16_t mp;
  int16_t offense;
  int16_t defense;
  int16_t speed;
  int16_t brain;
  int16_t care;
  int16_t weight;
  int16_t discipline;
  int16_t happiness;
  int16_t battles;
  int16_t techs;
  int8_t flags;
  uint8_t padding;
} EvoRequirements;

extern EvoSequenceData EVO_SEQUENCE_DATA;

extern EvoStatsGains EVO_GAINS_DATA[];
extern EvolutionPath EVO_PATHS_DATA[62];
extern EvoRequirements EVO_REQ_DATA[63];

#endif
