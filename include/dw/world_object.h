#ifndef DW_WORLD_OBJECT_H
#define DW_WORLD_OBJECT_H

#include <dw/types.h>

#define NUM_WORLD_OBJECTS	128

typedef void (*TickFunction)(int32_t);
typedef void (*RenderFunction)(int32_t);

typedef struct {
	int16_t objectId;
	int16_t instanceId;
	TickFunction tick;
	RenderFunction render;
} WorldObject;

extern WorldObject WORLD_OBJECTS[];

void initializeWorldObjects(void);
int32_t addObject(int32_t objectId, int32_t instanceId, TickFunction tick,
		  RenderFunction render);
int32_t removeObject(int32_t objectId, int32_t instanceId);
void tickObjects(void);
void renderObjects(void);

#endif
