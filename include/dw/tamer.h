#ifndef DW_TAMER_H
#define DW_TAMER_H

#include <dw/entity.h>
#include <dw/types.h>

void checkMapInteraction(void);

Entity *getEntityFromScriptId(uint8_t *scriptId);

int32_t tamerGetState(void);

void initializeTamer(int32_t type, int32_t posX, int32_t posY, int32_t posZ,
		     int32_t rotX, int32_t rotY, int32_t rotZ);

int32_t isTrainingComplete(void);

void loadMapEntities(uint8_t *data, int32_t mapId, int32_t warpIdx);

void tamerSetFullState(int32_t state, int32_t substate);
void tamerSetState(int32_t state);

void startAnimationTamer(int32_t animId);

int8_t startBattle(int16_t instanceId);

int32_t tickEntityMoveTo(/* uint8_t scriptId1, uint8_t scriptId2,
			 int32_t targetX, int32_t targetZ, int8_t speed,
			 int8_t withCamera */);
int32_t tickEntityMoveToAxis(/* uint8_t scriptId, int32_t target,
			     int32_t axis, int32_t speed,
			     int8_t withCamera */);
int32_t tickEntitySetRotation(uint32_t scriptId, int16_t rotationY);
int32_t tickLookAtEntity(uint8_t scriptId1, uint8_t scriptId2);

void worldPosToScreenPos2(int16_t *x, int16_t *y, int16_t *z);

#endif
