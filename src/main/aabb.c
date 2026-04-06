#include <libgte.h>

#include <dw/aabb.h>
#include <dw/entity.h>
#include <dw/params.h>


void unusedAABB(void)
{
}

int32_t findAABBHitEntity(AABB *aabb, Entity *ignoreEntity, int32_t startId)
{
	Entity *entity;
	VECTOR *location;
	SVECTOR center;
	AABB b;

	for (; startId < ENTITY_MAX; ++startId) {
		if ((entity = ENTITY_TABLE[startId]) &&
		    (entity != ignoreEntity)) {
			location = &entity->posData->location;
			center.vx = location->vx;
			center.vy = location->vy -
				(DIGIMON_DATA[entity->type].height >> 1);
			center.vz = location->vz;
			b.center = &center;
			b.extent.vx = DIGIMON_DATA[entity->type].radius >> 1;
			b.extent.vy = DIGIMON_DATA[entity->type].height >> 1;
			b.extent.vz = DIGIMON_DATA[entity->type].radius >> 1;
			if (hasAABBOverlap(aabb, &b) == 1) {
				return startId;
			}
		}
	}

	return -1;
}

int32_t hasAABBOverlap(AABB *a, AABB *b)
{
	if ((b->center->vx + b->extent.vx) <
	    (a->center->vx - a->extent.vx)) {
		return 0;
	} else if ((b->center->vx - b->extent.vx) >
		   (a->center->vx + a->extent.vx)) {
		return 0;
	} else {
		if ((b->center->vy + b->extent.vy) <
		    (a->center->vy - a->extent.vy)) {
			return 0;
		} else if ((b->center->vy - b->extent.vy) >
			   (a->center->vy + a->extent.vy)) {
			return 0;
		} else {
			if ((b->center->vz + b->extent.vz) <
			    (a->center->vz - a->extent.vz)) {
				return 0;
			} else if ((b->center->vz - b->extent.vz) >
				   (a->center->vz + a->extent.vz)) {
				return 0;
			}
		}
	}

	return 1;
}
