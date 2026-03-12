#ifndef DW_PARAMS_H
#define DW_PARAMS_H

#include <dw/types.h>

#define NUM_DIGIMON 180

typedef struct {
	char name[20];
	int32_t boneCount;
	int16_t radius;
	int16_t height;
	uint8_t type;
	uint8_t level;
	uint8_t special[3];
	uint8_t dropItem;
	uint8_t dropChance;
	int8_t moves[16];
	uint8_t padding;
} DigimonPara;

extern DigimonPara DIGIMON_DATA[NUM_DIGIMON];

#endif
