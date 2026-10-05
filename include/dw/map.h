#ifndef DW_MAP_H
#define DW_MAP_H

#include <libgte.h>

#include <dw/types.h>

typedef struct {
	int16_t posX1;
	int16_t posY1;
	int16_t posX2;
	int16_t posY2;
} ToiletData;

typedef struct {
	char filename[10];
	int8_t num8bppImages;
	int8_t num4bppImages;
	int8_t flags;
	int8_t doorsId;
	int8_t toiletId;
	int8_t loadingName;
} MapEntry;

typedef struct {
	uint8_t *imagePtr;
	int16_t tileId;
	int16_t posX;
	int16_t posY;
	int16_t texU;
	int16_t texV;
	int16_t tpage;
	int16_t clut;
} MapTileData;

typedef struct {
	int16_t orderValue;
	int16_t x;
	int16_t y;
	int16_t animSprites[8];
	uint8_t animTimes[8];
	uint8_t timer;
	uint8_t pad;
	int8_t currentFrame;
	int8_t flag;
} LocalMapObjectInstance;

typedef struct {
	MapTileData tiles[35];
	LocalMapObjectInstance objects[188];
	int16_t cameraX;
	int16_t cameraY;
	int8_t width;
	int8_t height;
	int8_t partnerAreaResponse;
	uint8_t pad;
} MapTiles;

typedef struct {
	uint8_t mapId;
	uint8_t mode;
	int16_t startId;
	int16_t count;
	uint16_t trigger;
} MapLightUpdateData;

extern ToiletData TOILET_DATA[];
extern MapEntry MAP_ENTRIES[];
extern VECTOR TOILET_TARGET_POS1;
extern VECTOR TOILET_TARGET_POS2;
extern MapTiles MAP_TILE_DATA;
extern int8_t MAP_TILE_X;
extern int8_t MAP_TILE_Y;
extern MapLightUpdateData MAP_LIGHT_UPDATE_DATA[];

void initializeDrawingOffsets(MapTileData *tiles);
void clearMapObjects(LocalMapObjectInstance *instances);
void calcMapObjectOrder(LocalMapObjectInstance *instances);
void renderMapOverlays(LocalMapObjectInstance *instances, int16_t screenX, int16_t screenY);

#endif
