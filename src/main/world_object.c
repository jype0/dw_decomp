#include <dw/world_object.h>

#pragma optimization_level 4

extern WorldObject WORLD_OBJECTS[];

void initializeWorldObjects(void)
{
	int32_t i;

	for (i = 0; i < NUM_WORLD_OBJECTS; ++i) {
		WORLD_OBJECTS[i].objectId = -1;
		WORLD_OBJECTS[i].instanceId = -1;
		WORLD_OBJECTS[i].tick = NULL;
		WORLD_OBJECTS[i].render = NULL;
	}
}


int32_t addObject(int32_t objectId, int32_t instanceId, TickFunction tick,
		  RenderFunction render)
{
	int32_t i;

	WORLD_OBJECTS[NUM_WORLD_OBJECTS].objectId = -1;

	i = 0;
	while (1) {
		if (WORLD_OBJECTS[i].objectId == -1) {
			break;
		}

		++i;
	}

	if (i == NUM_WORLD_OBJECTS) {
		return 0;
	}

	WORLD_OBJECTS[i].objectId = objectId;
	WORLD_OBJECTS[i].instanceId = instanceId;
	WORLD_OBJECTS[i].tick = tick;
	WORLD_OBJECTS[i].render = render;

	return 1;
}


int32_t removeObject(int32_t objectId, int32_t instanceId)
{
	int32_t i;

	WORLD_OBJECTS[NUM_WORLD_OBJECTS].objectId = objectId;
	WORLD_OBJECTS[NUM_WORLD_OBJECTS].instanceId = instanceId;

	i = 0;
	while (1) {
		if ((WORLD_OBJECTS[i].objectId == objectId) &&
		    (WORLD_OBJECTS[i].instanceId == instanceId)) {
			break;
		}

		++i;
	}

	if (i == NUM_WORLD_OBJECTS) {
		return 0;
	}

	WORLD_OBJECTS[i].objectId = -1;
	WORLD_OBJECTS[i].instanceId = -1;
	WORLD_OBJECTS[i].tick = NULL;
	WORLD_OBJECTS[i].render = NULL;

	return 1;
}

void tickObjects(void)
{
	int32_t i;

	for (i = 0; i < NUM_WORLD_OBJECTS; ++i) {
		if ((WORLD_OBJECTS[i].objectId != -1) &&
		    (WORLD_OBJECTS[i].tick != NULL)) {
			WORLD_OBJECTS[i].tick(WORLD_OBJECTS[i].instanceId);
		}
	}
}

void renderObjects(void)
{
	int32_t i;

	for (i = 0; i < NUM_WORLD_OBJECTS; ++i) {
		if ((WORLD_OBJECTS[i].objectId != -1) &&
		    (WORLD_OBJECTS[i].render != NULL)) {
			WORLD_OBJECTS[i].render(WORLD_OBJECTS[i].instanceId);
		}
	}
}
