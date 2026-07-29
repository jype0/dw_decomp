#ifndef DW_MODEL_H
#define DW_MODEL_H

#include <libgs.h>

#include <dw/types.h>

typedef struct {
	int32_t id;
	uint32_t flags;
	int32_t nobj;
	struct TMD_STRUCT obj[0];
} TMDModel;

typedef struct {
	uint32_t useCount;
	TMDModel *modelPtr;
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

void initializeModelComponents(void);
ModelComponent *getEntityModelComponent(int32_t instance, char type);

#endif
