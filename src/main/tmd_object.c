#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/params.h>
#include <dw/tmd_object.h>
#include <dw/trigger.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

#define NUM_MAP_3D_OBJECTS	6

typedef struct {
	VECTOR translation;
	SVECTOR rotation;
	int16_t direction;
	int16_t modelId;
} Map3DObject;

typedef struct {
	VECTOR pos1;
	VECTOR pos2;
	SVECTOR rotation1;
	SVECTOR rotation2;
	uint8_t pad30;
	uint8_t pad31;
	uint8_t pad32;
	uint8_t isSet;
	int32_t pad34;
} WarpCrystalData;

extern uint8_t CURRENT_SCREEN;
extern uint8_t MAP_LAYER_ENABLED;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[2];

int32_t DOORS_DISABLED;
int8_t DOOR_MODEL_IDS[4];
int8_t DOOR_ROTATION_TIMER;
int8_t ACTIVE_DIRT_CART_MODEL;
int16_t tmd_object_unused_e42;
int16_t BIG_BOX_LID_PROGRESS;
int16_t SMALL_BOX_LID_PROGRESS;
int8_t TOY_TOWN_SELECTED_BOX;
int16_t MAIN_D_80134E4A;
int16_t ANGEMON_PEDESTAL_PROGRESS_X;
int16_t ANGEMON_PEDESTAL_PROGRESS_Z;

char TMD_EXTENSION[8] = ".TMD";
SVECTOR DIRT_PILE_ROTATION = { 0, 0, 0, 0 };
SVECTOR TRAINING_POOP_ROTATION = { 0, 0, 0, 0 };
SVECTOR DIRT_PILE_SIZE_ROTATION = { 0, 0, 0, 0 };
RECT MEDAL_RECT = { 582, 432, 6, 32 };
int8_t BOX_CORNER_OFFSETS[8] = { -1, 1, 1, 1, -1, -1, 1, -1 };
int8_t DIRT_PILE_SIZE_COUNTER = 15;

char DOOR_PATH_PREFIX[12] = "\\DOOR\\DOOR";
char DIGIT_CHARS[12] = "0123456789";
char DIRT_CART_TMD[20] = "\\ETCNA\\DOSYA.TMD";
char DIRT_CART_BOX_TMD[16] = "\\ETCNA\\HAKO.TMD";
VECTOR DIRT_PILE_LOCATION = { 2037, 0, 849, 0 };
char DIRT_PILE_TMD[20] = "\\ETCNA\\T_YAMA.TMD";
char CHEST_TMD[16] = "\\ETCNA\\TAKA.TMD";
char WARP_CRYSTAL_TMD[16] = "\\ETCNA\\WARP.TMD";
VECTOR BOULDER_SCALE = { 4096, 4096, 4096, 0 };
char BOULDER_TMD[16] = "\\ETCNA\\IWA.TMD";
char BOULDER_TIM[16] = "\\ETCNA\\IWA1.TIM";
char MEDAL_TMD[20] = "\\ETCNA\\MEDAL.TMD";
VECTOR BIG_BOX_LOCATION = { 500, -600, 600, 0 };
VECTOR SMALL_BOX_LOCATION = { -400, -400, 600, 0 };
char BIG_BOX_TMD[20] = "\\ETCNA\\BIGBOX.TMD";
char SMALL_BOX_TMD[20] = "\\ETCNA\\SMABOX.TMD";
VECTOR GEARBOX_LOCATION = { -100, 0, 1700, 0 };
char GEARBOX_TMD[16] = "\\ETCNA\\GAND.TMD";
VECTOR ANGEMON_PEDESTAL_LOCATION = { 0, 0, 1500, 0 };
char ANGEMON_PEDESTAL_TMD[16] = "\\ETCNA\\ABOX.TMD";
VECTOR TRAINING_POOP_LOCATION = { 1922, 0, 503, 0 };
char TRAINING_POOP_TMD[16] = "\\ETCNA\\TRY.TMD";
VECTOR DOOR_SCALE = { 4096, 4096, 4096, 0 };
VECTOR CHEST_SCALE = { 4096, 4096, 4096, 0 };
VECTOR WARP_CRYSTAL_SCALE = { 4096, 4096, 4096, 0 };
VECTOR MOVE_BOULDER_SCALE = { 4096, 4096, 4096, 0 };

Chest CHEST_ARRAY[NUM_CHESTS];
GsCOORDINATE2 DOOR_COORDS[4];
char GENERAL_MESH_BUFFER[4][0x800];
GsDOBJ2 DOOR_OBJECTS[4];
Map3DObject MAP_3D_OBJECTS[NUM_MAP_3D_OBJECTS];
GsDOBJ2 GENERAL_OBJECT[2];
GsCOORDINATE2 GENERAL_COORDS[2];
GsDOBJ2 GENERAL_OBJECT3;
GsCOORDINATE2 GENERAL_COORDS3;
uint8_t CHEST_MESH_BUFFER[0x800];
GsCOORDINATE2 CHEST_COORDS[2];
GsDOBJ2 CHEST_OBJECTS[2];
GsCOORDINATE2 WARP_CRYSTAL_COORDS[2];
GsDOBJ2 WARP_CRYSTAL_OBJECT[2];
WarpCrystalData WARP_CRYSTAL_DATA[5];
char MEDAL_MESH_BUFFER[0x800];
GsDOBJ2 MEDAL_OBJECT;
GsCOORDINATE2 MEDAL_COORDINATES;

int32_t readFile(char *path, void *buffer);
void readMapTFS(int32_t mapId);
int32_t isTriggerSet(uint16_t trigger);
int32_t loadTextureFile(char *path, uint32_t *outTPage,
			uint32_t *outClut);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void setRectangleImpassable(int32_t tileX, int32_t tileY, int32_t size);
int32_t isOffScreen(DVECTOR *xy, int32_t w, int32_t h);

void clearChests(void);
void loadStaticTMD(char *path, char *buffer, GsDOBJ2 *obj,
		   GsCOORDINATE2 *coord);
void renderDoors(int32_t instanceId);
void renderDirtCartModel(int32_t instanceId);
void renderDirtPile(int32_t instanceId);
void renderChest(int32_t instanceId);
void renderWarpCrystals(int32_t instanceId);
void addWarpCrystal(int32_t posX, int32_t posZ);
void renderBoulder(int32_t instanceId);
void renderToyTownBoxes(int32_t instanceId);
void renderGearbox(int32_t instanceId);
void renderAngemonPedestal(int32_t instanceId);
void renderTrainingPoop(int32_t instanceId);
void emptyChests(void);
int32_t isBoxOffScreen(VECTOR *position, int32_t width, int32_t height);

static void *tmd_object_text_order[] = {
	activateMedalTexture,
	setDirtCartModel,
	moveAngemonPedestal,
	somethingToyTown,
	openToyTownBox,
	moveBoulder,
	decreaseDirtPileSize,
	tickCloseChestTray,
	tickOpenChestTray,
	drawObject,
	isBoxOffScreen,
	tickRotateDoor,
	unloadMapParts,
	emptyChests,
	spawnChest,
	renderTrainingPoop,
	loadTrainingPoop,
	renderAngemonPedestal,
	spawnAngemonPedestal,
	renderGearbox,
	spawnGearbox,
	renderToyTownBoxes,
	spawnToyTownBoxes,
	initializeMedalModel,
	renderBoulder,
	projectPosition,
	spawnBoulder,
	addWarpCrystal,
	renderWarpCrystals,
	loadWarpCrystals,
	renderChest,
	initializeChest,
	renderDirtPile,
	loadDirtPileModel,
	renderDirtCartModel,
	loadDirtCartModel,
	renderDoors,
	loadDoors,
	loadStaticTMD,
	clearChests,
};

static void *tmd_object_sbss_order[] = {
	&ANGEMON_PEDESTAL_PROGRESS_Z,
	&ANGEMON_PEDESTAL_PROGRESS_X,
	&MAIN_D_80134E4A,
	&TOY_TOWN_SELECTED_BOX,
	&SMALL_BOX_LID_PROGRESS,
	&BIG_BOX_LID_PROGRESS,
	&tmd_object_unused_e42,
	&ACTIVE_DIRT_CART_MODEL,
	&DOOR_ROTATION_TIMER,
	DOOR_MODEL_IDS,
	&DOORS_DISABLED,
};

static void *tmd_object_bss_order[] = {
	&MEDAL_COORDINATES,
	&MEDAL_OBJECT,
	MEDAL_MESH_BUFFER,
	WARP_CRYSTAL_DATA,
	WARP_CRYSTAL_OBJECT,
	&WARP_CRYSTAL_COORDS,
	CHEST_OBJECTS,
	CHEST_COORDS,
	CHEST_MESH_BUFFER,
	&GENERAL_COORDS3,
	&GENERAL_OBJECT3,
	GENERAL_COORDS,
	GENERAL_OBJECT,
	MAP_3D_OBJECTS,
	DOOR_OBJECTS,
	GENERAL_MESH_BUFFER,
	DOOR_COORDS,
	CHEST_ARRAY,
};

void clearChests(void)
{
	int32_t i;

	for (i = 0; i < NUM_CHESTS; ++i) {
		CHEST_ARRAY[i].item = 0xff;
		CHEST_ARRAY[i].isTaken = 0;
	}
}

void loadStaticTMD(char *path, char *buffer, GsDOBJ2 *obj,
		   GsCOORDINATE2 *coord)
{
	readFile(path, buffer);

	GsMapModelingData((unsigned long *)(&buffer[4]));
	GsLinkObject4((unsigned long)&buffer[12], obj, 0);

	GsInitCoordinate2(NULL, coord);

	obj->attribute = 0;
	obj->coord2 = coord;
}

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", loadDoors);

void renderDoors(int32_t instanceId)
{
	Map3DObject doorCopy;
	VECTOR scale;
	int32_t i;
	int32_t slot;
	int16_t modelId;

	scale = DOOR_SCALE;

	if (!MAP_LAYER_ENABLED) {
		return;
	}

	for (i = 0; i < NUM_MAP_3D_OBJECTS; i++) {
		if (MAP_3D_OBJECTS[i].modelId == -1) {
			continue;
		}

		modelId = MAP_3D_OBJECTS[i].modelId;
		for (slot = 0; slot < 4; slot++) {
			if (DOOR_MODEL_IDS[slot] == modelId) {
				break;
			}
		}

		if (isBoxOffScreen(&MAP_3D_OBJECTS[i].translation,
				   700, 800) == 1) {
			continue;
		}

		projectPosition(&DOOR_COORDS[slot],
				&MAP_3D_OBJECTS[i].translation,
				&MAP_3D_OBJECTS[i].rotation, &scale);
		drawObject(&DOOR_OBJECTS[slot],
			   GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);

		if (MAP_3D_OBJECTS[i].modelId == 10) {
			doorCopy = MAP_3D_OBJECTS[i];
			doorCopy.translation.vz -=
				((doorCopy.rotation.vy < 0) ? 1 : -1) * 680;
			doorCopy.rotation.vy = -doorCopy.rotation.vy;
			projectPosition(&DOOR_COORDS[slot],
					&doorCopy.translation,
					&doorCopy.rotation, &scale);
			drawObject(&DOOR_OBJECTS[slot],
				   GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
		}
	}
}

void loadDirtCartModel(void)
{
	loadStaticTMD(DIRT_CART_TMD, GENERAL_MESH_BUFFER[0],
		      &GENERAL_OBJECT[0], &GENERAL_COORDS[0]);
	loadStaticTMD(DIRT_CART_BOX_TMD, GENERAL_MESH_BUFFER[1],
		      &GENERAL_OBJECT[1], &GENERAL_COORDS[1]);

	ACTIVE_DIRT_CART_MODEL = 2;

	addObject(0xfb3, 0, NULL, renderDirtCartModel);
}

void renderDirtPile(int32_t instanceId)
{
	if (DIRT_PILE_SIZE_COUNTER >= 6 && MAP_LAYER_ENABLED) {
		drawObject(&GENERAL_OBJECT3,
			   GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
	}
}

void initializeChest(void)
{
	uint8_t *buf;
	int32_t i;

	clearChests();

	readFile(CHEST_TMD, CHEST_MESH_BUFFER);

	buf = (uint8_t *)CHEST_MESH_BUFFER;
	GsMapModelingData((unsigned long *)&buf[4]);

	GsInitCoordinate2(NULL, &CHEST_COORDS[0]);
	GsInitCoordinate2(&CHEST_COORDS[0], &CHEST_COORDS[1]);

	for (i = 0; i < 2; ++i) {
		GsLinkObject4((unsigned long)&buf[12], &CHEST_OBJECTS[i], i);
		CHEST_OBJECTS[i].attribute = 0;
		CHEST_OBJECTS[i].coord2 = &CHEST_COORDS[i];
	}

	addObject(0xfb5, 0, NULL, renderChest);
}

void renderChest(int32_t instanceId)
{
	VECTOR scale;
	int32_t chestIdx;
	int32_t i;

	scale = CHEST_SCALE;

	if (!MAP_LAYER_ENABLED) {
		return;
	}

	for (chestIdx = 0; chestIdx < NUM_CHESTS; chestIdx++) {
		if (CHEST_ARRAY[chestIdx].item == 0xff) {
			continue;
		}

		if (isBoxOffScreen(&CHEST_ARRAY[chestIdx].location,
				   100, 100) == 1) {
			continue;
		}

		for (i = 0; i < 2; i++) {
			projectPosition(&CHEST_COORDS[i],
					&(&CHEST_ARRAY[chestIdx].location)[i],
					&(&CHEST_ARRAY[chestIdx].rotation)[i],
					&scale);
			drawObject(&CHEST_OBJECTS[i],
				   GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
		}
	}
}

void loadWarpCrystals(int32_t mapId)
{
	uint8_t *buf;
	int32_t i;

	clearChests();

	readFile(WARP_CRYSTAL_TMD, GENERAL_MESH_BUFFER[0]);

	buf = (uint8_t *)GENERAL_MESH_BUFFER[0];
	GsMapModelingData((unsigned long *)&buf[4]);

	GsInitCoordinate2(NULL, &WARP_CRYSTAL_COORDS[0]);
	GsInitCoordinate2(&WARP_CRYSTAL_COORDS[0], &WARP_CRYSTAL_COORDS[1]);

	for (i = 0; i < 2; i++) {
		GsLinkObject4((unsigned long)&buf[12],
			      &WARP_CRYSTAL_OBJECT[i], i);
		WARP_CRYSTAL_OBJECT[i].attribute = 0;
		WARP_CRYSTAL_OBJECT[i].coord2 = &WARP_CRYSTAL_COORDS[i];
	}

	addObject(0xfb7, 0, NULL, renderWarpCrystals);

	for (i = 0; i < 5; i++) {
		WARP_CRYSTAL_DATA[i].isSet = 0;
	}

	if (mapId == 0x65) {
		addWarpCrystal(2237, -1667);
		addWarpCrystal(-2237, -1667);
		addWarpCrystal(2358, 419);
		addWarpCrystal(-2358, 419);
	}

	if (mapId == 0x66) {
		addWarpCrystal(0, 0);
	}

	if (mapId == 0x67) {
		addWarpCrystal(2376, -127);
		addWarpCrystal(1566, -3358);
	}

	if (mapId == 0x68) {
		addWarpCrystal(-2100, 1900);
		addWarpCrystal(2100, 1900);
		addWarpCrystal(-2100, -2100);
		addWarpCrystal(2100, -2100);
	}
}

void renderWarpCrystals(int32_t instanceId)
{
	VECTOR scale;
	int32_t crystalIdx;
	int32_t i;

	scale = WARP_CRYSTAL_SCALE;

	if (!MAP_LAYER_ENABLED) {
		return;
	}

	for (crystalIdx = 0; crystalIdx < 4; crystalIdx++) {
		if (WARP_CRYSTAL_DATA[crystalIdx].isSet == 0) {
			continue;
		}

		for (i = 0; i < 2; i++) {
			if (isBoxOffScreen(&WARP_CRYSTAL_DATA[crystalIdx].pos1,
					   100, 100) == 1) {
				continue;
			}

			projectPosition(&WARP_CRYSTAL_COORDS[i],
					&(&WARP_CRYSTAL_DATA[crystalIdx].pos1)[i],
					&(&WARP_CRYSTAL_DATA[crystalIdx].rotation1)[i],
					&scale);
			drawObject(&WARP_CRYSTAL_OBJECT[i],
				   GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
		}
	}
}

void addWarpCrystal(int32_t posX, int32_t posZ)
{
	int32_t idx;

	for (idx = 0; idx < 5; ++idx) {
		if (WARP_CRYSTAL_DATA[idx].isSet == 1) {
			continue;
		}

		WARP_CRYSTAL_DATA[idx].pos1.vx = posX;
		WARP_CRYSTAL_DATA[idx].pos1.vy = -188;
		WARP_CRYSTAL_DATA[idx].pos1.vz = posZ;
		WARP_CRYSTAL_DATA[idx].pos2.vx = 0;
		WARP_CRYSTAL_DATA[idx].pos2.vy = -188;
		WARP_CRYSTAL_DATA[idx].pos2.vz = 0;
		WARP_CRYSTAL_DATA[idx].rotation1.vx = 0;
		WARP_CRYSTAL_DATA[idx].rotation1.vy = 0;
		WARP_CRYSTAL_DATA[idx].rotation1.vz = 0;
		WARP_CRYSTAL_DATA[idx].rotation2.vx = 0;
		WARP_CRYSTAL_DATA[idx].rotation2.vy = 0;
		WARP_CRYSTAL_DATA[idx].rotation2.vz = 0;
		WARP_CRYSTAL_DATA[idx].isSet = 1;

		return;
	}
}

void spawnBoulder(void)
{
	VECTOR scale;

	scale = BOULDER_SCALE;

	loadStaticTMD(BOULDER_TMD, GENERAL_MESH_BUFFER[0], &GENERAL_OBJECT3,
		      &GENERAL_COORDS3);
	loadTextureFile(BOULDER_TIM, NULL, NULL);

	MAP_3D_OBJECTS[0].translation.vx = -300;
	MAP_3D_OBJECTS[0].translation.vy = -500;
	MAP_3D_OBJECTS[0].translation.vz = 2000;
	MAP_3D_OBJECTS[0].rotation.vx = 0;
	MAP_3D_OBJECTS[0].rotation.vy = 0;
	MAP_3D_OBJECTS[0].rotation.vz = 0;

	projectPosition(&GENERAL_COORDS3, &MAP_3D_OBJECTS[0].translation,
			&MAP_3D_OBJECTS[0].rotation, &scale);

	addObject(0xfb6, 0, NULL, renderBoulder);
}

void projectPosition(GsCOORDINATE2 *coord, VECTOR *trans, SVECTOR *rot,
		     VECTOR *scale)
{
	RotMatrix(rot, &coord->coord);
	ScaleMatrix(&coord->coord, scale);
	TransMatrix(&coord->coord, trans);
	coord->flg = 0;
}

void renderBoulder(int32_t instanceId)
{
	if (MAP_LAYER_ENABLED) {
		drawObject(&GENERAL_OBJECT3,
			   GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
	}
}

void initializeMedalModel(void)
{
	loadStaticTMD(MEDAL_TMD, MEDAL_MESH_BUFFER, &MEDAL_OBJECT,
		      &MEDAL_COORDINATES);
}

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", spawnToyTownBoxes);

void renderToyTownBoxes(int32_t instanceId)
{
	SVECTOR rot;
	int32_t i;

	if (MAP_LAYER_ENABLED) {
		if (TOY_TOWN_SELECTED_BOX == 1) {
			BIG_BOX_LID_PROGRESS -= 100;
			if (BIG_BOX_LID_PROGRESS < -0x400) {
				BIG_BOX_LID_PROGRESS = -0x400;
			}

			rot.vx = BIG_BOX_LID_PROGRESS;
			rot.vy = 0;
			rot.vz = 0;
			RotMatrix(&rot, &WARP_CRYSTAL_COORDS[1].coord);
			WARP_CRYSTAL_COORDS[1].flg = 0;
		}

		if (TOY_TOWN_SELECTED_BOX == 2) {
			SMALL_BOX_LID_PROGRESS -= 100;
			if (SMALL_BOX_LID_PROGRESS < -0x400) {
				SMALL_BOX_LID_PROGRESS = -0x400;
			}

			rot.vx = SMALL_BOX_LID_PROGRESS;
			rot.vy = 0;
			rot.vz = 0;
			RotMatrix(&rot, &GENERAL_COORDS[1].coord);
			GENERAL_COORDS[1].flg = 0;
		}

		for (i = 0; i < 2; ++i) {
			drawObject(&WARP_CRYSTAL_OBJECT[i],
				   GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
			drawObject(&GENERAL_OBJECT[i],
				   GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
		}
	}
}

void spawnGearbox(void)
{
	VECTOR translation;

	translation = GEARBOX_LOCATION;
	TOY_TOWN_SELECTED_BOX = 0;

	loadStaticTMD(GEARBOX_TMD, GENERAL_MESH_BUFFER[0], &GENERAL_OBJECT3,
		      &GENERAL_COORDS3);

	TransMatrix(&GENERAL_COORDS3.coord, &translation);
	GENERAL_COORDS3.flg = 0;

	MAIN_D_80134E4A = 0x294;

	addObject(0xfbc, 0, NULL, renderGearbox);
}

void renderGearbox(int32_t instanceId)
{
	if (MAP_LAYER_ENABLED) {
		drawObject(&GENERAL_OBJECT3,
			   GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
	}
}

void spawnAngemonPedestal(void)
{
	VECTOR translation;

	translation = ANGEMON_PEDESTAL_LOCATION;

	loadStaticTMD(ANGEMON_PEDESTAL_TMD, GENERAL_MESH_BUFFER[0],
		      &GENERAL_OBJECT3, &GENERAL_COORDS3);

	ANGEMON_PEDESTAL_PROGRESS_X = 0;
	ANGEMON_PEDESTAL_PROGRESS_Z = 1500;

	TransMatrix(&GENERAL_COORDS3.coord, &translation);

	GENERAL_COORDS3.flg = 0;

	addObject(0xfbe, 0, NULL, renderAngemonPedestal);
}

void renderAngemonPedestal(int32_t instanceId)
{
	VECTOR trans;

	if (MAP_LAYER_ENABLED) {
		trans.vx = ANGEMON_PEDESTAL_PROGRESS_X;
		trans.vy = 0;
		trans.vz = ANGEMON_PEDESTAL_PROGRESS_Z;
		TransMatrix(&GENERAL_COORDS3.coord, &trans);
		GENERAL_COORDS3.flg = 0;
		drawObject(&GENERAL_OBJECT3,
			   GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
	}
}

void loadTrainingPoop(void)
{
	VECTOR translation;
	VECTOR scale;
	SVECTOR rotation;

	translation = TRAINING_POOP_LOCATION;
	rotation = TRAINING_POOP_ROTATION;

	loadStaticTMD(TRAINING_POOP_TMD, GENERAL_MESH_BUFFER[0],
		      &GENERAL_OBJECT3, &GENERAL_COORDS3);

	scale.vx = 0x1000;
	scale.vy = 0x1000;
	scale.vz = 0x1000;

	RotMatrix(&rotation, &GENERAL_COORDS3.coord);
	ScaleMatrix(&GENERAL_COORDS3.coord, &scale);
	TransMatrix(&GENERAL_COORDS3.coord, &translation);
	GENERAL_COORDS3.flg = 0;

	addObject(0xfbf, 0, NULL, renderTrainingPoop);
}

void renderTrainingPoop(int32_t instanceId)
{
	if (MAP_LAYER_ENABLED) {
		drawObject(&GENERAL_OBJECT3,
			   GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
	}
}

void spawnChest(int32_t posX, int32_t posY, int32_t posZ, int32_t rotation,
		uint8_t item, uint16_t trigger)
{
	int32_t idx;
	int16_t tileX;
	int16_t tileY;
	VECTOR pos;

	for (idx = 0; idx < NUM_CHESTS; ++idx) {
		if (CHEST_ARRAY[idx].item != 0xff) {
			continue;
		}

		if (isTriggerSet(trigger) == 1) {
			CHEST_ARRAY[idx].isTaken = 1;
			CHEST_ARRAY[idx].trayLocation.vx = 0;
			CHEST_ARRAY[idx].trayLocation.vy = 0;
			CHEST_ARRAY[idx].trayLocation.vz = -160;
		} else {
			CHEST_ARRAY[idx].isTaken = 0;
			CHEST_ARRAY[idx].trayLocation.vx = 0;
			CHEST_ARRAY[idx].trayLocation.vy = 0;
			CHEST_ARRAY[idx].trayLocation.vz = 0;
		}

		pos.vx = posX;
		pos.vy = posY;
		pos.vz = posZ;

		CHEST_ARRAY[idx].rotation.vx = 0;
		CHEST_ARRAY[idx].rotation.vy = rotation;
		CHEST_ARRAY[idx].rotation.vz = 0;
		CHEST_ARRAY[idx].trayRotation.vx = 0;
		CHEST_ARRAY[idx].trayRotation.vy = 0;
		CHEST_ARRAY[idx].trayRotation.vz = 0;
		CHEST_ARRAY[idx].trigger = trigger;
		CHEST_ARRAY[idx].item = item;

		getModelTile(&pos, &tileX, &tileY);
		setRectangleImpassable(tileX, tileY, 2);

		CHEST_ARRAY[idx].location.vx = (tileX * 100) - 4950;
		CHEST_ARRAY[idx].location.vy = 0;
		CHEST_ARRAY[idx].location.vz = 4950 - (tileY * 100);
		CHEST_ARRAY[idx].tileX = tileX;
		CHEST_ARRAY[idx].tileY = tileY;

		if (rotation == 0) {
			CHEST_ARRAY[idx].tileY += 4;
		} else if (rotation == 0x800) {
			CHEST_ARRAY[idx].tileY -= 4;
		}

		if (rotation == 0x400) {
			CHEST_ARRAY[idx].tileX -= 4;
			return;
		}

		if (rotation != 0xc00) {
			return;
		}

		CHEST_ARRAY[idx].tileX += 4;
		return;
	}
}

void emptyChests(void)
{
	int32_t i;

	for (i = 0; i < NUM_CHESTS; ++i) {
		CHEST_ARRAY[i].item = 0xff;
	}
}

void unloadMapParts(void)
{
	removeObject(0xfa9, 0);
	removeObject(0xfb7, 0);
	removeObject(0xfb3, 0);
	removeObject(0xfb4, 0);
	removeObject(0xfb6, 0);
	removeObject(0xfbd, 0);
	removeObject(0xfbc, 0);
	removeObject(0xfbe, 0);
	removeObject(0xfbf, 0);

	emptyChests();
}

int32_t tickRotateDoor(int32_t instance, int32_t target)
{
	switch (MAP_3D_OBJECTS[instance].modelId) {
	case 0:
		++DOOR_ROTATION_TIMER;
		if (DOOR_ROTATION_TIMER > 32) {
			DOOR_ROTATION_TIMER = 0;
			return 1;
		}

		MAP_3D_OBJECTS[instance].rotation.vy -= target * 33;

		break;
	case 9:
	case 0xa:
	case 0xb:
	case 0xc:
	case 0xd:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
		++DOOR_ROTATION_TIMER;
		if (DOOR_ROTATION_TIMER > 32) {
			DOOR_ROTATION_TIMER = 0;
			MAP_3D_OBJECTS[instance].rotation.vy =
				MAP_3D_OBJECTS[instance].direction;
			return 1;
		}

		MAP_3D_OBJECTS[instance].rotation.vy += target * 33;

		break;
	case 0xe:
	case 0xf:
		++DOOR_ROTATION_TIMER;
		if (DOOR_ROTATION_TIMER > 32) {
			DOOR_ROTATION_TIMER = 0;
			return 1;
		}

		MAP_3D_OBJECTS[instance].rotation.vy += target * 33;

		break;
	case 5:
	case 0x1f:
	case 0x20:
		++DOOR_ROTATION_TIMER;
		if (DOOR_ROTATION_TIMER > 9) {
			DOOR_ROTATION_TIMER = 0;
			return 1;
		}

		MAP_3D_OBJECTS[instance].translation.vy -= target * 200;

		break;
	case 0x15:
	case 0x16:
	case 0x18:
	case 0x1b:
	case 0x1d:
		++DOOR_ROTATION_TIMER;
		if (DOOR_ROTATION_TIMER > 9) {
			DOOR_ROTATION_TIMER = 0;
			return 1;
		}

		if (CURRENT_SCREEN == 0x81) {
			MAP_3D_OBJECTS[instance].translation.vz += 30;
		} else {
			MAP_3D_OBJECTS[instance].translation.vx += 30;
		}

		break;
	case 0x14:
	case 0x17:
	case 0x19:
	case 0x1c:
	case 0x1e:
		++DOOR_ROTATION_TIMER;
		if (DOOR_ROTATION_TIMER > 9) {
			DOOR_ROTATION_TIMER = 0;
			return 1;
		}

		if (CURRENT_SCREEN == 0x81) {
			MAP_3D_OBJECTS[instance].translation.vz -= 0x1e;
		} else {
			MAP_3D_OBJECTS[instance].translation.vx -= 0x1e;
		}

		break;
	case 0x1a:
		++DOOR_ROTATION_TIMER;

		MAP_3D_OBJECTS[instance].rotation.vx -= target * 110;
		if (MAP_3D_OBJECTS[instance].rotation.vx < -0x400) {
			MAP_3D_OBJECTS[instance].rotation.vx = -0x400;
		}

		if (MAP_3D_OBJECTS[instance].rotation.vx > 0) {
			MAP_3D_OBJECTS[instance].rotation.vx = 0;
		}

		if (DOOR_ROTATION_TIMER > 19) {
			DOOR_ROTATION_TIMER = 0;
			return 1;
		}
	}

	return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", isBoxOffScreen);

void drawObject(GsDOBJ2 *obj, GsOT *ot, int32_t flag)
{
	MATRIX m;

	GsGetLw(obj->coord2, &m);
	GsSetLightMatrix(&m);

	GsGetLs(obj->coord2, &m);
	GsSetLsMatrix(&m);

	GsSortObject4(obj, ot, flag, getScratchAddr(0));
}

int32_t tickOpenChestTray(int32_t chestId)
{
	CHEST_ARRAY[chestId].trayLocation.vz -= 20;

	if (CHEST_ARRAY[chestId].trayLocation.vz < -159) {
		CHEST_ARRAY[chestId].trayLocation.vz = -160;
		CHEST_ARRAY[chestId].isTaken = 1;
		return 1;
	}

	return 0;
}

int32_t tickCloseChestTray(int32_t chestId)
{
	CHEST_ARRAY[chestId].trayLocation.vz += 20;

	if (CHEST_ARRAY[chestId].trayLocation.vz >= 0) {
		CHEST_ARRAY[chestId].trayLocation.vz = 0;
		CHEST_ARRAY[chestId].isTaken = 0;
		return 1;
	}

	return 0;
}

void decreaseDirtPileSize(void)
{
	SVECTOR rotation;
	VECTOR scale;
	int16_t scaleValue;

	rotation = DIRT_PILE_SIZE_ROTATION;

	DIRT_PILE_SIZE_COUNTER -= 1;
	scaleValue = (DIRT_PILE_SIZE_COUNTER * 0x1000) / 15;
	scale.vx = scaleValue;
	scale.vy = scaleValue;
	scale.vz = scaleValue;

	RotMatrix(&rotation, &GENERAL_COORDS3.coord);
	ScaleMatrix(&GENERAL_COORDS3.coord, &scale);

	GENERAL_COORDS3.flg = 0;
}

void moveBoulder(int32_t diffX, int32_t diffZ)
{
	VECTOR scale;

	scale = MOVE_BOULDER_SCALE;

	MAP_3D_OBJECTS[0].translation.vy += diffX;
	MAP_3D_OBJECTS[0].translation.vz += diffZ;
	MAP_3D_OBJECTS[0].rotation.vx -= 10;

	projectPosition(&GENERAL_COORDS3, &MAP_3D_OBJECTS[0].translation,
			&MAP_3D_OBJECTS[0].rotation, &scale);
}

void openToyTownBox(int32_t boxId)
{
	TOY_TOWN_SELECTED_BOX = boxId;
}

void somethingToyTown(int32_t boxId)
{
	TOY_TOWN_SELECTED_BOX = boxId;

	if ((boxId == 0) && (isTriggerSet(TRIGGER_350) == 1)) {
		readMapTFS(0x92);
	}
}

int32_t moveAngemonPedestal(void)
{
	int32_t angle;

	ANGEMON_PEDESTAL_PROGRESS_Z += 10;
	angle = ANGEMON_PEDESTAL_PROGRESS_Z;

	if ((angle % 20) == 0) {
		ANGEMON_PEDESTAL_PROGRESS_X = 5;
	} else {
		ANGEMON_PEDESTAL_PROGRESS_X = -5;
	}

	if (ANGEMON_PEDESTAL_PROGRESS_Z > 1999) {
		ANGEMON_PEDESTAL_PROGRESS_Z = 2000;
		return 1;
	}

	return 0;
}

void setDirtCartModel(int32_t model)
{
	ACTIVE_DIRT_CART_MODEL = model;
}

void activateMedalTexture(uint8_t medalId)
{
	RECT rect;

	rect = MEDAL_RECT;
	rect.x = ((medalId % 9) * 6) + 518;
	rect.y = ((medalId / 9) * 32) + 432;
	MoveImage(&rect, 512, 432);

	DrawSync(0);
}

void loadDirtPileModel(void)
{
	VECTOR translation;
	VECTOR scale;
	SVECTOR rotation;
	int16_t scaleValue;

	translation = DIRT_PILE_LOCATION;
	rotation = DIRT_PILE_ROTATION;

	loadStaticTMD(DIRT_PILE_TMD, GENERAL_MESH_BUFFER[2], &GENERAL_OBJECT3,
		      &GENERAL_COORDS3);

	scaleValue = (DIRT_PILE_SIZE_COUNTER * 0x1000) / 15;
	scale.vx = scaleValue;
	scale.vy = scaleValue;
	scale.vz = scaleValue;

	RotMatrix(&rotation, &GENERAL_COORDS3.coord);
	ScaleMatrix(&GENERAL_COORDS3.coord, &scale);
	TransMatrix(&GENERAL_COORDS3.coord, &translation);

	GENERAL_COORDS3.flg = 0;

	addObject(0xfb4, 0, NULL, renderDirtPile);
}

void renderDirtCartModel(int32_t instanceId)
{
	MATRIX matrix;
	VECTOR translation;
	VECTOR worldPos;
	VECTOR *location;

	if (ACTIVE_DIRT_CART_MODEL == 2) {
		return;
	}

	if (!MAP_LAYER_ENABLED) {
		return;
	}

	location = &PARTNER_ENTITY.digimonEntity.entity.posData->location;
	translation.vx = 0;
	translation.vy = 0;
	translation.vz = DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].radius + 100;

	RotMatrix(&PARTNER_ENTITY.digimonEntity.entity.posData->rotation,
		  &matrix);
	ApplyMatrixLV(&matrix, &translation, &worldPos);

	worldPos.vx += location->vx;
	worldPos.vz += location->vz;

	RotMatrix(&PARTNER_ENTITY.digimonEntity.entity.posData->rotation,
		  &GENERAL_COORDS[ACTIVE_DIRT_CART_MODEL].coord);
	TransMatrix(&GENERAL_COORDS[ACTIVE_DIRT_CART_MODEL].coord, &worldPos);

	GENERAL_COORDS[ACTIVE_DIRT_CART_MODEL].flg = 0;

	drawObject(&GENERAL_OBJECT[ACTIVE_DIRT_CART_MODEL],
		   GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
}
