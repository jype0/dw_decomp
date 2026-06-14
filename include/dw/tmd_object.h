#ifndef DW_TMD_OBJECT_H
#define DW_TMD_OBJECT_H

#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/types.h>

#define NUM_CHESTS		8

typedef struct {
	VECTOR location;
	VECTOR trayLocation;
	SVECTOR rotation;
	SVECTOR trayRotation;
	uint16_t trigger;
	uint8_t item;
	uint8_t isTaken;
	uint8_t tileX;
	uint8_t tileY;
	uint16_t padding;
} Chest;

extern Chest CHEST_ARRAY[NUM_CHESTS];
extern GsDOBJ2 MEDAL_OBJECT;
extern GsCOORDINATE2 MEDAL_COORDINATES;

void loadDoors(int32_t doorEntryId);
void loadDirtCartModel(void);
void loadDirtPileModel(void);
void initializeChest(void);
void loadWarpCrystals(int32_t mapId);
void spawnBoulder(void);
void projectPosition(GsCOORDINATE2 *coord, VECTOR *trans, SVECTOR *rot,
		     VECTOR *scale);
void initializeMedalModel(void);
void spawnToyTownBoxes(void);
void spawnGearbox(void);
void spawnAngemonPedestal(void);
void loadTrainingPoop(void);
void spawnChest(int32_t posX, int32_t posY, int32_t posZ, int32_t rotation,
		uint8_t item, uint16_t trigger);
void unloadMapParts(void);
int32_t tickRotateDoor(int32_t instance, int32_t target);
void drawObject(GsDOBJ2 *obj, GsOT *ot, int32_t flag);
int32_t tickOpenChestTray(int32_t chestId);
int32_t tickCloseChestTray(int32_t chestId);
void decreaseDirtPileSize(void);
void moveBoulder(int32_t diffX, int32_t diffZ);
void openToyTownBox(int32_t boxId);
void somethingToyTown(int32_t boxId);
int32_t moveAngemonPedestal(void);
void setDirtCartModel(int32_t model);
void activateMedalTexture(uint8_t medalId);

#endif
