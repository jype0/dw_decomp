#include <libgte.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>

#include "common.h"

extern uint8_t MAP_COLLISION_DATA[];
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void loadMapCollisionData(int8_t *src);
int32_t getTileTrigger(VECTOR *pos);
int32_t checkMapCollisionX(Entity *entity, int32_t direction);
int32_t checkMapCollisionZ(Entity *entity, int32_t direction);
void setImpassableRect(int32_t x, int32_t y, int32_t w, int32_t h);
void setImpassableSquare(int32_t x, int32_t y, int32_t r);

static inline int32_t copyValue(int32_t value)
{
	return value;
}

void loadMapCollisionData(int8_t *src)
{
	int32_t i;

	for (i = 0; i < 0x2710; i++) {
		MAP_COLLISION_DATA[i] = *src++;
	}
}

int32_t getTileTrigger(VECTOR *pos)
{
	int16_t tx;
	int16_t tz;

	tx = pos->vx / 100 + 0x32;
	tz = 0x32 - pos->vz / 100;
	if (pos->vx < 0) {
		tx--;
	}
	if (pos->vz > 0) {
		tz--;
	}
	if ((MAP_COLLISION_DATA[tx + tz * 100] != 0) &&
	    (MAP_COLLISION_DATA[tx + tz * 100] != 0x80) &&
	    (MAP_COLLISION_DATA[tx + tz * 100] != 0xFF80)) {
		return (int8_t)MAP_COLLISION_DATA[tx + tz * 100];
	}
	if (MAP_COLLISION_DATA[tx + tz * 100] == 0) {
		return 0;
	} else {
		return -1;
	}
}

int32_t checkMapCollisionX(entity, direction)
Entity *entity;
int8_t direction;
{
	VECTOR *position;
	int16_t tile;
	int16_t pos;
	int16_t rightTile;
	int16_t edgePos;
	int16_t edgeTile;
	int16_t radius;

	position = &entity->posData->location;
	radius = DIGIMON_DATA[entity->type].radius;

	pos = position->vx - radius / 2;
	tile = pos / 100 + 0x32;
	if (pos < 0) {
		tile--;
	}

	pos = position->vx + radius / 2;
	rightTile = pos / 100 + 0x32;
	if (pos < 0) {
		rightTile--;
	}

	if (direction == 0) {
		edgePos = position->vz + radius;
	} else {
		edgePos = position->vz - radius;
	}
	edgeTile = 0x31 - edgePos / 100;
	if (edgePos < 0) {
		edgeTile++;
	}

	for (; tile <= rightTile; tile++) {
		if ((MAP_COLLISION_DATA[tile + edgeTile * 100] & 0x80) != 0) {
			return 1;
		}
	}

	return 0;
}

int32_t checkMapCollisionZ(entity, direction)
Entity *entity;
int8_t direction;
{
	int16_t bottomTile;
	int16_t edgePos;
	int16_t edgeTile;
	VECTOR *position;
	int16_t radius;
	int16_t tile;
	int16_t pos;

	position = &entity->posData->location;
	radius = DIGIMON_DATA[entity->type].radius;
	if (direction == 0) {
		edgePos = position->vx - radius;
	} else {
		edgePos = position->vx + radius;
	}
	edgeTile = edgePos / 100 + 0x32;
	if (edgePos < 0) {
		edgeTile--;
	}

	pos = position->vz + radius / 2;
	tile = 0x31 - pos / 100;
	if (pos < 0) {
		tile++;
	}

	pos = position->vz - radius / 2;
	bottomTile = 0x31 - pos / 100;
	if (pos < 0) {
		bottomTile++;
	}

	for (; tile <= bottomTile; tile++) {
		if ((MAP_COLLISION_DATA[edgeTile + tile * 100] & 0x80) != 0) {
			return 1;
		}
	}

	return 0;
}

void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY)
{
	*outTileX = pos->vx / 100 + 0x32;
	*outTileY = 0x32 - pos->vz / 100;
	if (pos->vx < 0) {
		*outTileX -= 1;
	}
	if (pos->vz > 0) {
		*outTileY -= 1;
	}
}

#if defined(VERSION_JP)
void setImpassableSquare(x, y, radius)
int16_t x;
int16_t y;
int32_t radius;
#else
void setImpassableSquare(int32_t x, int32_t y, int32_t radius)
#endif
{
#if !defined(VERSION_JP)
	int32_t originalRadius;
	int32_t originalY;
#endif
	int32_t tileX;
	int32_t tileY;

#if defined(VERSION_JP)
	for (tileY = y - radius; tileY < y + radius; tileY++) {
		for (tileX = x - radius; tileX < x + radius; tileX++) {
#else
	originalRadius = copyValue(radius);
	originalY = copyValue(y);
	for (tileY = y - radius; tileY < originalY + originalRadius; tileY++) {
		for (tileX = x - originalRadius; tileX < x + radius; tileX++) {
#endif
			MAP_COLLISION_DATA[tileX + tileY * 100] = 0x80;
		}
	}
}

void setImpassableRect(x, y, width, height)
int8_t x;
int8_t y;
int8_t width;
int8_t height;
{
	int32_t tileX;
	int32_t tileY;

	for (tileY = y; tileY < y + height; tileY++) {
		for (tileX = x; tileX < x + width; tileX++) {
			MAP_COLLISION_DATA[tileX + tileY * 100] = 0x80;
		}
	}
}
