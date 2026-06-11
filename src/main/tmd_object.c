#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

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

extern Chest CHEST_ARRAY[8];

extern char MAIN_D_8012AAF0[];
extern char MAIN_D_80153EA0[];
extern GsDOBJ2 MEDAL_OBJECT;
extern GsCOORDINATE2 MEDAL_COORDINATES;

typedef struct {
	VECTOR trans;
	SVECTOR rot;
	int16_t unk18;
	int16_t type;
} RotateObject;

extern RotateObject ROTATE_OBJECTS[];

typedef struct {
	VECTOR location;
	VECTOR unk10;
	SVECTOR rot1;
	SVECTOR rot2;
	uint8_t unk30;
	uint8_t unk31;
	uint8_t unk32;
	uint8_t isActive;
	int32_t unk34;
} WarpCrystal;

extern WarpCrystal WARP_CRYSTALS[];

extern uint8_t CURRENT_SCREEN;
extern uint8_t MAP_LAYER_ENABLED;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[2];
extern GsDOBJ2 MAIN_D_801533A8;
extern GsDOBJ2 MAIN_D_801532E8[2];
extern GsDOBJ2 MAIN_D_80153D68[2];
extern GsCOORDINATE2 MAIN_D_80153308;
extern GsCOORDINATE2 MAIN_D_80153358;
extern GsCOORDINATE2 MAIN_D_80153D18;
extern GsCOORDINATE2 MAIN_D_801533B8;
extern int16_t MAIN_D_80134E44;
extern int16_t MAIN_D_80134E46;
extern VECTOR MAIN_D_8012ABDC;
extern char MAIN_D_8012AAA0[];
extern uint8_t MAIN_D_80153408[];
extern GsDOBJ2 MAIN_D_80153CA8[2];
extern GsCOORDINATE2 MAIN_D_80153C08[2];
extern char MAIN_D_8012AA58[];
extern char MAIN_D_8012AA6C[];
extern VECTOR MAIN_D_8012AAC0;
extern char MAIN_D_8012AAD0[];
extern char MAIN_D_8012AAE0[];
extern char MAIN_D_80151200[];
extern char MAIN_D_80151A00[];
extern RECT MAIN_D_80134414;
extern int8_t MAIN_D_8013441C[8];
extern int8_t MAIN_D_80134424;
extern int8_t MAIN_D_80134E40;
extern int8_t MAIN_D_80134E41;
extern int8_t MAIN_D_80134E48;
extern int16_t MAIN_D_80134E4C;
extern int16_t MAIN_D_80134E4E;

int32_t readFile(const char *path, void *buffer);
void readMapTFS(int32_t mapId);
int32_t isTriggerSet(uint16_t trigger);
int32_t loadTextureFile(const char *path, uint32_t *outTPage,
			uint32_t *outClut);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void setRectangleImpassable(int32_t tileX, int32_t tileY, int32_t size);
int32_t isOffScreen(DVECTOR *xy, int32_t w, int32_t h);
void renderObject(GsDOBJ2 *obj, GsOT *ot, int32_t flag);
void MAIN_func_800E057C(int32_t arg);

void spawnBoulder(void);
void MAIN_func_800E16B0(void);
void unloadMapParts(void);

void clearChests(void);
void MAIN_func_800E04FC(void);
void MAIN_func_800E0810(int32_t arg);
void projectPosition(GsCOORDINATE2 *coord, VECTOR *trans, SVECTOR *rot,
		     VECTOR *scale);
void renderBoulder(int32_t arg);
void MAIN_func_800E15B8(int32_t arg);
void MAIN_func_800E1820(int32_t arg);
int32_t tickOpenChestTray(int32_t chestId);
int32_t tickCloseChestTray(int32_t chestId);
void loadStaticTMD(char *path, char *buffer, GsDOBJ2 *obj,
		   GsCOORDINATE2 *coord);
void initializeMedalModel(void);
void emptyChests(void);
int32_t tickRotateObject(int32_t objId, int32_t speed);
void MAIN_func_800E23F8(int32_t a0);
void MAIN_func_800E2400(int32_t a0);
int32_t MAIN_func_800E243C(void);
void MAIN_func_800E249C(int32_t a0);
void MAIN_func_800E2354(int32_t dx, int32_t dz);

void loadDoors(int32_t mapId);
void renderDoors(int32_t arg);
void MAIN_func_800E06F0(void);
void initializeChest(void);
void renderChest(int32_t arg);
void initializeWarpCrystals(void);
void MAIN_func_800E0CEC(void);
void MAIN_func_800E0E58(int32_t param_1, int32_t param_2);
void MAIN_func_800E1154(void);
void MAIN_func_800E1398(void);
void MAIN_func_800E1510(void);
void MAIN_func_800E1608(void);
void initializeTrainingPoop(void);
void spawnChest(int32_t x, int32_t y, int32_t z, int32_t dir, uint8_t item,
		uint16_t trigger);
int32_t isBoxOffScreen(int32_t *box, int32_t scale, int32_t depth);
void MAIN_func_800E22A0(void);
void activateMedalTexture(uint8_t param_1);

static void *tmd_object_functions[] = {
	activateMedalTexture,
	MAIN_func_800E249C,
	MAIN_func_800E243C,
	MAIN_func_800E2400,
	MAIN_func_800E23F8,
	MAIN_func_800E2354,
	MAIN_func_800E22A0,
	tickCloseChestTray,
	tickOpenChestTray,
	renderObject,
	isBoxOffScreen,
	tickRotateObject,
	unloadMapParts,
	emptyChests,
	spawnChest,
	MAIN_func_800E1820,
	initializeTrainingPoop,
	MAIN_func_800E16B0,
	MAIN_func_800E1608,
	MAIN_func_800E15B8,
	MAIN_func_800E1510,
	MAIN_func_800E1398,
	MAIN_func_800E1154,
	initializeMedalModel,
	renderBoulder,
	projectPosition,
	spawnBoulder,
	MAIN_func_800E0E58,
	MAIN_func_800E0CEC,
	initializeWarpCrystals,
	renderChest,
	initializeChest,
	MAIN_func_800E0810,
	MAIN_func_800E06F0,
	MAIN_func_800E057C,
	MAIN_func_800E04FC,
	renderDoors,
	loadDoors,
	loadStaticTMD,
	clearChests,
};

void clearChests(void)
{
	int32_t i;

	for (i = 0; i < 8; ++i) {
		CHEST_ARRAY[i].item = 0xff;
		CHEST_ARRAY[i].isTaken = 0;
	}
}

void loadStaticTMD(char *path, char *buffer, GsDOBJ2 *obj,
		   GsCOORDINATE2 *coord)
{
	readFile(path, buffer);

	GsMapModelingData((u_long *)(buffer + 4));
	GsLinkObject4((u_long)(buffer + 0xc), obj, 0);
	GsInitCoordinate2(NULL, coord);
	obj->attribute = 0;
	obj->coord2 = coord;
}

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", loadDoors);

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", renderDoors);

void MAIN_func_800E04FC(void)
{
	loadStaticTMD(MAIN_D_8012AA58, MAIN_D_80151200, &MAIN_D_801532E8[0],
		      &MAIN_D_80153308);
	loadStaticTMD(MAIN_D_8012AA6C, MAIN_D_80151A00, &MAIN_D_801532E8[1],
		      &MAIN_D_80153358);

	MAIN_D_80134E41 = 2;

	addObject(0xfb3, 0, NULL, MAIN_func_800E057C);
}

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", MAIN_func_800E057C);

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", MAIN_func_800E06F0);

void MAIN_func_800E0810(int32_t arg)
{
	if (MAIN_D_80134424 >= 6 && MAP_LAYER_ENABLED) {
		renderObject(&MAIN_D_801533A8,
			     GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
	}
}

void initializeChest(void)
{
	unsigned long buf;
	int32_t i;

	clearChests();

	readFile(MAIN_D_8012AAA0, MAIN_D_80153408);

	buf = (unsigned long)MAIN_D_80153408;
	GsMapModelingData((unsigned long *)(buf + 4));

	GsInitCoordinate2(NULL, &MAIN_D_80153C08[0]);
	GsInitCoordinate2(&MAIN_D_80153C08[0], &MAIN_D_80153C08[1]);

	for (i = 0; i < 2; ++i) {
		GsLinkObject4(buf + 0xc, &MAIN_D_80153CA8[i], i);
		MAIN_D_80153CA8[i].attribute = 0;
		MAIN_D_80153CA8[i].coord2 = &MAIN_D_80153C08[i];
	}

	addObject(0xfb5, 0, NULL, renderChest);
}

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", renderChest);

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", initializeWarpCrystals);

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", MAIN_func_800E0CEC);

void MAIN_func_800E0E58(int32_t param_1, int32_t param_2)
{
	int32_t idx;

	for (idx = 0; idx < 5; ++idx) {
		if (WARP_CRYSTALS[idx].isActive == 1) {
			continue;
		}

		WARP_CRYSTALS[idx].location.vx = param_1;
		WARP_CRYSTALS[idx].location.vy = -188;
		WARP_CRYSTALS[idx].location.vz = param_2;
		WARP_CRYSTALS[idx].unk10.vx = 0;
		WARP_CRYSTALS[idx].unk10.vy = -188;
		WARP_CRYSTALS[idx].unk10.vz = 0;
		WARP_CRYSTALS[idx].rot1.vx = 0;
		WARP_CRYSTALS[idx].rot1.vy = 0;
		WARP_CRYSTALS[idx].rot1.vz = 0;
		WARP_CRYSTALS[idx].rot2.vx = 0;
		WARP_CRYSTALS[idx].rot2.vy = 0;
		WARP_CRYSTALS[idx].rot2.vz = 0;
		WARP_CRYSTALS[idx].isActive = 1;

		return;
	}
}

void spawnBoulder(void)
{
	VECTOR scale;

	scale = MAIN_D_8012AAC0;

	loadStaticTMD(MAIN_D_8012AAD0, MAIN_D_80151200, &MAIN_D_801533A8,
		      &MAIN_D_801533B8);
	loadTextureFile(MAIN_D_8012AAE0, NULL, NULL);

	ROTATE_OBJECTS[0].trans.vx = -300;
	ROTATE_OBJECTS[0].trans.vy = -500;
	ROTATE_OBJECTS[0].trans.vz = 2000;
	ROTATE_OBJECTS[0].rot.vx = 0;
	ROTATE_OBJECTS[0].rot.vy = 0;
	ROTATE_OBJECTS[0].rot.vz = 0;

	projectPosition(&MAIN_D_801533B8, &ROTATE_OBJECTS[0].trans,
			&ROTATE_OBJECTS[0].rot, &scale);

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

void renderBoulder(int32_t arg)
{
	if (MAP_LAYER_ENABLED) {
		renderObject(&MAIN_D_801533A8,
			     GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
	}
}

void initializeMedalModel(void)
{
	loadStaticTMD(MAIN_D_8012AAF0, MAIN_D_80153EA0, &MEDAL_OBJECT,
		      &MEDAL_COORDINATES);
}

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", MAIN_func_800E1154);

void MAIN_func_800E1398(void)
{
	SVECTOR rot;
	int32_t i;

	if (MAP_LAYER_ENABLED) {
		if (MAIN_D_80134E48 == 1) {
			MAIN_D_80134E44 -= 100;
			if (MAIN_D_80134E44 < -0x400) {
				MAIN_D_80134E44 = -0x400;
			}
			rot.vx = MAIN_D_80134E44;
			rot.vy = 0;
			rot.vz = 0;
			RotMatrix(&rot, &MAIN_D_80153D18.coord);
			MAIN_D_80153D18.flg = 0;
		}

		if (MAIN_D_80134E48 == 2) {
			MAIN_D_80134E46 -= 100;
			if (MAIN_D_80134E46 < -0x400) {
				MAIN_D_80134E46 = -0x400;
			}
			rot.vx = MAIN_D_80134E46;
			rot.vy = 0;
			rot.vz = 0;
			RotMatrix(&rot, &MAIN_D_80153358.coord);
			MAIN_D_80153358.flg = 0;
		}

		for (i = 0; i < 2; ++i) {
			renderObject(&MAIN_D_80153D68[i],
				     GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER,
				     2);
			renderObject(&MAIN_D_801532E8[i],
				     GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER,
				     2);
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", MAIN_func_800E1510);

void MAIN_func_800E15B8(int32_t arg)
{
	if (MAP_LAYER_ENABLED) {
		renderObject(&MAIN_D_801533A8,
			     GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", MAIN_func_800E1608);

void MAIN_func_800E16B0(void)
{
	VECTOR trans;

	if (MAP_LAYER_ENABLED) {
		trans.vx = MAIN_D_80134E4C;
		trans.vy = 0;
		trans.vz = MAIN_D_80134E4E;
		TransMatrix(&MAIN_D_801533B8.coord, &trans);
		MAIN_D_801533B8.flg = 0;
		renderObject(&MAIN_D_801533A8,
			     GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", initializeTrainingPoop);

void MAIN_func_800E1820(int32_t arg)
{
	if (MAP_LAYER_ENABLED) {
		renderObject(&MAIN_D_801533A8,
			     GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
	}
}

void spawnChest(int32_t x, int32_t y, int32_t z, int32_t dir, uint8_t item,
		uint16_t trigger)
{
	int32_t idx;
	int16_t tileX;
	int16_t tileY;
	long offset;
	VECTOR pos;

	offset = 0x1356;

	for (idx = 0; idx < 8; ++idx) {
		if (CHEST_ARRAY[idx].item != 0xff) {
			continue;
		}

		if (isTriggerSet(trigger) == 1) {
			CHEST_ARRAY[idx].isTaken = 1;
			CHEST_ARRAY[idx].trayLocation.vx = 0;
			CHEST_ARRAY[idx].trayLocation.vy = 0;
			CHEST_ARRAY[idx].trayLocation.vz = -0xa0;
		} else {
			CHEST_ARRAY[idx].isTaken = 0;
			CHEST_ARRAY[idx].trayLocation.vx = 0;
			CHEST_ARRAY[idx].trayLocation.vy = 0;
			CHEST_ARRAY[idx].trayLocation.vz = 0;
		}

		pos.vx = x;
		pos.vy = y;
		pos.vz = z;

		CHEST_ARRAY[idx].rotation.vx = 0;
		CHEST_ARRAY[idx].rotation.vy = dir;
		CHEST_ARRAY[idx].rotation.vz = 0;
		CHEST_ARRAY[idx].trayRotation.vx = 0;
		CHEST_ARRAY[idx].trayRotation.vy = 0;
		CHEST_ARRAY[idx].trayRotation.vz = 0;
		CHEST_ARRAY[idx].trigger = trigger;
		CHEST_ARRAY[idx].item = item;

		getModelTile(&pos, &tileX, &tileY);
		setRectangleImpassable(tileX, tileY, 2);

		CHEST_ARRAY[idx].location.vx = tileX * 100 - 0x1356;
		CHEST_ARRAY[idx].location.vy = 0;
		CHEST_ARRAY[idx].location.vz = 0x1356 - tileY * 100;
		CHEST_ARRAY[idx].tileX = tileX;
		CHEST_ARRAY[idx].tileY = tileY;

		if (dir == 0) {
			CHEST_ARRAY[idx].tileY += 4;
		} else if (dir == 0x800) {
			CHEST_ARRAY[idx].tileY -= 4;
		}

		if (dir == 0x400) {
			CHEST_ARRAY[idx].tileX -= 4;
			return;
		}

		if (dir != 0xc00) {
			return;
		}

		CHEST_ARRAY[idx].tileX += 4;
		return;
	}
}

void emptyChests(void)
{
	int32_t i;

	for (i = 0; i < 8; ++i) {
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

int32_t tickRotateObject(int32_t objId, int32_t speed)
{
	switch (ROTATE_OBJECTS[objId].type) {
	case 0:
		++MAIN_D_80134E40;
		if (MAIN_D_80134E40 > 0x20) {
			MAIN_D_80134E40 = 0;
			return 1;
		}

		ROTATE_OBJECTS[objId].rot.vy -= (int16_t)(speed * 33);

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
		++MAIN_D_80134E40;
		if (MAIN_D_80134E40 > 0x20) {
			MAIN_D_80134E40 = 0;
			ROTATE_OBJECTS[objId].rot.vy =
				ROTATE_OBJECTS[objId].unk18;
			return 1;
		}

		ROTATE_OBJECTS[objId].rot.vy += (int16_t)(speed * 33);

		break;
	case 0xe:
	case 0xf:
		++MAIN_D_80134E40;
		if (MAIN_D_80134E40 > 0x20) {
			MAIN_D_80134E40 = 0;
			return 1;
		}

		ROTATE_OBJECTS[objId].rot.vy += (int16_t)(speed * 33);

		break;
	case 5:
	case 0x1f:
	case 0x20:
		++MAIN_D_80134E40;
		if (MAIN_D_80134E40 > 9) {
			MAIN_D_80134E40 = 0;
			return 1;
		}

		ROTATE_OBJECTS[objId].trans.vy -= speed * 200;

		break;
	case 0x15:
	case 0x16:
	case 0x18:
	case 0x1b:
	case 0x1d:
		++MAIN_D_80134E40;
		if (MAIN_D_80134E40 > 9) {
			MAIN_D_80134E40 = 0;
			return 1;
		}

		if (CURRENT_SCREEN == 0x81) {
			ROTATE_OBJECTS[objId].trans.vz += 0x1e;
		} else {
			ROTATE_OBJECTS[objId].trans.vx += 0x1e;
		}

		break;
	case 0x14:
	case 0x17:
	case 0x19:
	case 0x1c:
	case 0x1e:
		++MAIN_D_80134E40;
		if (MAIN_D_80134E40 > 9) {
			MAIN_D_80134E40 = 0;
			return 1;
		}

		if (CURRENT_SCREEN == 0x81) {
			ROTATE_OBJECTS[objId].trans.vz -= 0x1e;
		} else {
			ROTATE_OBJECTS[objId].trans.vx -= 0x1e;
		}

		break;
	case 0x1a:
		++MAIN_D_80134E40;

		ROTATE_OBJECTS[objId].rot.vx -= (int16_t)(speed * 110);
		if (ROTATE_OBJECTS[objId].rot.vx < -0x400) {
			ROTATE_OBJECTS[objId].rot.vx = -0x400;
		}

		if (ROTATE_OBJECTS[objId].rot.vx > 0) {
			ROTATE_OBJECTS[objId].rot.vx = 0;
		}

		if (MAIN_D_80134E40 > 0x13) {
			MAIN_D_80134E40 = 0;
			return 1;
		}
	}

	return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", isBoxOffScreen);

void renderObject(GsDOBJ2 *obj, GsOT *ot, int32_t flag)
{
	MATRIX m;

	GsGetLw(obj->coord2, &m);
	GsSetLightMatrix(&m);
	GsGetLs(obj->coord2, &m);
	GsSetLsMatrix(&m);
	GsSortObject4(obj, ot, flag, (u_long *)0x1f800000);
}

int32_t tickOpenChestTray(int32_t chestId)
{
	CHEST_ARRAY[chestId].trayLocation.vz -= 0x14;

	if (CHEST_ARRAY[chestId].trayLocation.vz < -0x9f) {
		CHEST_ARRAY[chestId].trayLocation.vz = -0xa0;
		CHEST_ARRAY[chestId].isTaken = 1;

		return 1;
	}

	return 0;
}

int32_t tickCloseChestTray(int32_t chestId)
{
	CHEST_ARRAY[chestId].trayLocation.vz += 0x14;

	if (CHEST_ARRAY[chestId].trayLocation.vz >= 0) {
		CHEST_ARRAY[chestId].trayLocation.vz = 0;
		CHEST_ARRAY[chestId].isTaken = 0;

		return 1;
	}

	return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/tmd_object", MAIN_func_800E22A0);

void MAIN_func_800E2354(int32_t dx, int32_t dz)
{
	VECTOR scale;

	scale = MAIN_D_8012ABDC;

	ROTATE_OBJECTS[0].trans.vy += dx;
	ROTATE_OBJECTS[0].trans.vz += dz;
	ROTATE_OBJECTS[0].rot.vx -= 10;

	projectPosition(&MAIN_D_801533B8, &ROTATE_OBJECTS[0].trans,
			&ROTATE_OBJECTS[0].rot, &scale);
}

void MAIN_func_800E23F8(int32_t a0)
{
	MAIN_D_80134E48 = a0;
}

void MAIN_func_800E2400(int32_t a0)
{
	MAIN_D_80134E48 = a0;

	if (a0 == 0 && isTriggerSet(0x15e) == 1) {
		readMapTFS(0x92);
	}
}

int32_t MAIN_func_800E243C(void)
{
	int32_t angle;

	MAIN_D_80134E4E += 10;
	angle = MAIN_D_80134E4E;

	if ((angle % 20) == 0) {
		MAIN_D_80134E4C = 5;
	} else {
		MAIN_D_80134E4C = -5;
	}

	if (MAIN_D_80134E4E > 1999) {
		MAIN_D_80134E4E = 2000;
		return 1;
	}

	return 0;
}

void MAIN_func_800E249C(int32_t a0)
{
	MAIN_D_80134E41 = a0;
}

void activateMedalTexture(uint8_t param_1)
{
	RECT rect;

	rect = MAIN_D_80134414;
	rect.x = ((param_1 % 9) * 6) + 0x206;
	rect.y = ((param_1 / 9) * 0x20) + 0x1b0;
	MoveImage(&rect, 0x200, 0x1b0);
	DrawSync(0);
}
