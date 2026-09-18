#ifndef DW_BATTLE_H
#define DW_BATTLE_H

#include <dw/types.h>

typedef struct {
	int16_t unk0;
	int16_t unk2;
	int16_t unk4;
	int16_t unk6;
	int16_t unk8;
	int16_t unkA;
} CameraPreset;

typedef struct {
	int16_t clut;
	uint8_t u;
	uint8_t v;
	uint8_t w;
	uint8_t h;
	int16_t x;
	int16_t y;
} BarSprite;

#endif
