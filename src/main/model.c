#include <malloc.h>
#include <string.h>

#include <libgs.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/file.h>
#include <dw/params.h>
#include <dw/world_object.h>

#include "common.h"

typedef struct {
	uint32_t useCount;
	void *modelPtr;
	int32_t *animTablePtr;
	void *mmdPtr;
	uint16_t pixelPage;
	uint16_t clutPage;
	uint8_t pixelOffsetX;
	uint8_t pixelOffsetY;
	int16_t modelId;
	int16_t digiType;
	uint16_t pad;
} ModelComponent;

extern ModelComponent NPC_MODEL[5];
extern ModelComponent TAMER_MODEL;
extern ModelComponent PARTNER_MODEL;
extern ModelComponent UNKNOWN_MODEL[16];
extern int32_t NPC_MODEL_TAKEN[5];
extern int32_t UNKNOWN_MODEL_TAKEN[16];
extern void *PARTNER_MODEL_BUFFER;
extern void *TAMER_MODEL_BUFFER;

extern char MAIN_D_8011D190[];

void handleNullModel(void);
void concatStrings(char *dst, char *src1, char *src2);
void initializeModelComponents(void);
void initializePosData(PositionData *posData);
void loadDigimonTexture(int32_t digiType, char *path,
			ModelComponent *component);
ModelComponent *loadMMD(int32_t digiType, int32_t modelType);
void removeEntity(int32_t objectId, int32_t entityId);
void setEntityPosition(int32_t entityId, int32_t x, int32_t y, int32_t z);
void setEntityRotation(int32_t entityId, int16_t x, int16_t y, int16_t z);
void setupEntityMatrix(int32_t entityId);
ModelComponent *thunkLoadMMD(int32_t digiType, int32_t modelType);
void thunkUnloadModel(int32_t digiType, int32_t modelType);
void unloadModel(int32_t digiType, int32_t modelType);
void uploadModelTexture(void *textureData, ModelComponent *component);

void renderFlatDigimon(Entity* entity);
void initializeDigimonObject(int32_t type, int32_t instanceId,
			     TickFunction tick);
void renderDigimon(int32_t instanceId);
void renderWireframed(GsDOBJ2* obj, int32_t wireFrameShare);
void resetFlattenGlobal(void);
ModelComponent *getEntityModelComponent(int32_t instance, int32_t type);
int32_t getEntityType(Entity* entity);
void uploadModelTexture(void *textureData, ModelComponent *component);
void loadMMDAsync(int32_t digimonType, int32_t entityType, uint8_t *buffer,
		  EvoModelData *modelData, uint8_t *readComplete);
void applyMMD(int32_t digimonType, int32_t entityType,
	      EvoModelData *modelData);

static void *model_functions[] = {
	applyMMD,
	loadMMDAsync,
	uploadModelTexture,
	getEntityType,
	getEntityModelComponent,
	unloadModel,
	loadMMD,
	initializeModelComponents,
	handleNullModel,
	concatStrings,
	loadDigimonTexture,
	resetFlattenGlobal,
	renderWireframed,
	setEntityRotation,
	setEntityPosition,
	setupEntityMatrix,
	removeEntity,
	renderDigimon,
	initializeDigimonObject,
	thunkUnloadModel,
	thunkLoadMMD,
	renderFlatDigimon,
	initializePosData,
};

void initializePosData(PositionData *posData)
{
	MATRIX *m;

	RotMatrix(&posData->rotation, m = &posData->posMatrix.coord);
	ScaleMatrix(m, &posData->scale);
	TransMatrix(m, &posData->location);
	posData->posMatrix.flg = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/model", renderFlatDigimon);

ModelComponent *thunkLoadMMD(int32_t digiType, int32_t modelType)
{
	return loadMMD(digiType, modelType);
}

void thunkUnloadModel(int32_t digiType, int32_t modelType)
{
	unloadModel(digiType, modelType);
}

INCLUDE_ASM("asm/main/nonmatchings/model", initializeDigimonObject);

INCLUDE_ASM("asm/main/nonmatchings/model", renderDigimon);

void removeEntity(int32_t objectId, int32_t entityId)
{
	Entity *entity;

	removeObject((int16_t)objectId, (int16_t)entityId);
	if ((entityId != 1) && (entityId != 0)) {
		entity = ENTITY_TABLE[entityId];
		free3(entity->anim.momentum);
		free3(entity->posData);
		ENTITY_TABLE[entityId] = NULL;
	}
}

void setupEntityMatrix(int32_t entityId)
{
	MATRIX *m;
	PositionData *posData;

	if ((entityId >= 0) && (entityId < ENTITY_MAX)) {
		posData = ENTITY_TABLE[entityId]->posData;
		RotMatrix(&posData->rotation, m = &posData->posMatrix.coord);
		ScaleMatrix(m, &posData->scale);
		TransMatrix(m, &posData->location);
		posData->posMatrix.flg = 0;
	}
}

void setEntityPosition(int32_t entityId, int32_t x, int32_t y, int32_t z)
{
	VECTOR *location;

	if ((entityId >= 0) && (entityId < ENTITY_MAX)) {
		location = &ENTITY_TABLE[entityId]->posData->location;
		location->vx = x;
		location->vy = y;
		location->vz = z;
		ENTITY_TABLE[entityId]->anim.locX = location->vx << 15;
		ENTITY_TABLE[entityId]->anim.locY = location->vy << 15;
		ENTITY_TABLE[entityId]->anim.locZ = location->vz << 15;
	}
}

void setEntityRotation(int32_t entityId, int16_t x, int16_t y, int16_t z)
{
	SVECTOR *rotation;

	if ((entityId >= 0) && (entityId < ENTITY_MAX)) {
		rotation = &ENTITY_TABLE[entityId]->posData->rotation;
		rotation->vx = x;
		rotation->vy = y;
		rotation->vz = z;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/model", renderWireframed);

INCLUDE_ASM("asm/main/nonmatchings/model", resetFlattenGlobal);

void loadDigimonTexture(int32_t digiType, char *path,
			ModelComponent *component)
{
	void *buffer;
	char fileName[32];

	strrchr(path, '\\');
	strcpy(fileName, MAIN_D_8011D190);

	buffer = malloc3(0x4800);

	readFileSectors(fileName, buffer, digiType * 9, 9);
	uploadModelTexture(buffer, component);

	free3(buffer);
}

void concatStrings(char *dst, char *src1, char *src2)
{
	while (*src1 != '\0') {
		*dst++ = *src1++;
	}

	while (*src2 != '\0') {
		*dst++ = *src2++;
	}

	*dst = '\0';
}

void handleNullModel(void)
{
}

void initializeModelComponents(void)
{
	ModelComponent t;
	int32_t i;

	t.useCount = 0;
	t.modelPtr = NULL;
	t.animTablePtr = NULL;
	t.mmdPtr = NULL;
	t.pixelPage = 0;
	t.clutPage = 0;
	t.pixelOffsetY = 0;
	t.pixelOffsetX = 0;
	t.modelId = -1;
	t.digiType = -1;

	for (i = 0; i < 5; i = i + 1) {
		NPC_MODEL[i] = t;
	}

	for (i = 0; i < 5; i = i + 1) {
		NPC_MODEL_TAKEN[i] = 0;
	}

	for (i = 0; i < 16; i = i + 1) {
		UNKNOWN_MODEL[i] = t;
	}

	for (i = 0; i < 16; i = i + 1) {
		UNKNOWN_MODEL_TAKEN[i] = 0;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/model", loadMMD);

void unloadModel(int32_t digiType, int32_t modelType)
{
	int32_t i;
	ModelComponent *m;

	if (modelType == 0) {
		for (m = NPC_MODEL, i = 0; i < 5; ++m, ++i) {
			if (m->digiType == digiType) {
				break;
			}
		}
		if (i != 5) {
			--m->useCount;
			if (m->useCount == 0) {
				m->digiType = -1;
				if (m->mmdPtr != NULL) {
					free3(m->mmdPtr);
				}
				m->modelPtr = NULL;
				m->animTablePtr = NULL;
				m->mmdPtr = NULL;
				if (m->modelId != -1) {
					NPC_MODEL_TAKEN[m->modelId] = 0;
				}
				m->modelId = -1;
			}
		}
	} else if (modelType == 1) {
		for (m = UNKNOWN_MODEL, i = 0; i < 16; ++m, ++i) {
			if (m->useCount == digiType) {
				break;
			}
		}
		if (i != 16) {
			m->useCount = 0;
			if (m->modelId != -1) {
				UNKNOWN_MODEL_TAKEN[m->modelId] = 0;
			}
			m->modelId = -1;
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/model", getEntityModelComponent);

INCLUDE_ASM("asm/main/nonmatchings/model", getEntityType);

static inline int32_t applyTPageOffset(int32_t tpageOffset,
				       int32_t pixelOffset)
{
	return tpageOffset + pixelOffset;
}

void uploadModelTexture(void *textureData, ModelComponent *component)
{
	GsIMAGE img;
	RECT rect;

	GsGetTimInfo((unsigned long *)textureData + 1, &img);

	img.px = applyTPageOffset((component->pixelPage % 16) * 64,
				  component->pixelOffsetX);
	img.py = applyTPageOffset((component->pixelPage / 16) * 256,
				  component->pixelOffsetY);
	img.cx = (component->clutPage & 0x3f) << 4;
	img.cy = component->clutPage >> 6;

	rect.x = img.px;
	rect.y = img.py;
	rect.w = img.pw;
	rect.h = img.ph;
	LoadImage(&rect, img.pixel);

	if ((img.pmode >> 3) & 1) {
		rect.x = img.cx;
		rect.y = img.cy;
		rect.w = img.cw;
		rect.h = img.ch;
		LoadImage(&rect, img.clut);
	}

	DrawSync(0);
}

INCLUDE_ASM("asm/main/nonmatchings/model", loadMMDAsync);

INCLUDE_ASM("asm/main/nonmatchings/model", applyMMD);
