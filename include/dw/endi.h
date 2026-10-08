#ifndef DW_ENDI_H
#define DW_ENDI_H

#include <libgpu.h>

#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/types.h>

#define NUM_ENDI_PARTICLES	80

typedef struct {
	int16_t active;
	int16_t boneIndex;
	Entity *entity;
	SVECTOR pos;
	SVECTOR worldPos;
} EndiParticle;

typedef struct {
	int32_t frame;
	Entity *entity;
	int16_t phase;
	int16_t velocity;
	int32_t pad;
} EndingState;

typedef struct {
	int32_t flag;
	SVECTOR base;
} EndiData;

extern u_long *ENDI_FADE_CLUT_BUFFER;
extern u_long *ENDI_CLUT_BUFFER;
extern RGB8 ENDI_PARTICLE_COLOR;

extern EndiParticle ENDI_PARTICLES[NUM_ENDI_PARTICLES];
extern EndiData ENDI_DATA;
extern EndingState ENDI_STATE;

int32_t ENDI_tickEnding(Entity *entity, int32_t isInitialized);

#endif
