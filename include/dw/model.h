#ifndef DW_MODEL_H
#define DW_MODEL_H

#include <libgs.h>

#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/types.h>

typedef struct {
	int8_t objIndex;
	int8_t parentIndex;
} SkeletonBone;

typedef struct {
	int32_t id;
	uint32_t flags;
	int32_t nobj;
	struct TMD_STRUCT obj[0];
} TMDModel;

typedef struct {
	uint32_t useCount;
	TMDModel *modelPtr;
	long *animTablePtr;
	void *mmdPtr;
	uint16_t pixelPage;
	uint16_t clutPage;
	uint8_t pixelOffsetX;
	uint8_t pixelOffsetY;
	int16_t modelId;
	int16_t digiType;
	uint16_t pad;
} ModelComponent;

ModelComponent *applyMMD(int32_t digimonType, int32_t entityType, EvoModelData *modelData);
ModelComponent *getEntityModelComponent(int32_t instance, int32_t type);
int32_t getEntityType(Entity *entity);
void initializeDigimonObject(int32_t type, int32_t instanceId, void (*tick)(int32_t));
void initializeModelComponents(void);
int32_t loadMMDAsync(int32_t digimonType, int32_t entityType, int32_t buffer, EvoModelData *modelData,
                     uint8_t *readComplete);
void removeEntity(int32_t objectId, int32_t entityId);
void resetFlattenGlobal(void);
void setEntityPosition(int32_t entityId, int32_t x, int32_t y, int32_t z);
void setEntityRotation(/* int32_t entityId, int16_t x, int16_t y, int16_t z */);
void setupEntityMatrix(int32_t entityId);
ModelComponent *thunkLoadMMD(int32_t digiType, int32_t modelType);
void thunkUnloadModel(int32_t digiType, int32_t modelType);
void unloadModel(int32_t digiType, int32_t modelType);

extern ModelComponent UNKNOWN_MODEL[16];
extern SkeletonBone *DIGIMON_SKELETONS[];
extern int32_t PARTNER_WIREFRAME_TOTAL;
extern int16_t PARTNER_WIREFRAME_SUB[];
extern int32_t ENTITY1_WIREFRAME_TOTAL;
extern int32_t PLAYER_SHADOW_ENABLED;
extern int16_t WIREFRAME_COLOR_MIN;
extern int16_t WIREFRAME_COLOR_MAX;
extern int8_t WIREFRAME_RNG_TABLE[];

#endif
