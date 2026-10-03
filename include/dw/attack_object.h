#ifndef DW_ATTACK_OBJECT_H
#define DW_ATTACK_OBJECT_H

#include <libgte.h>
#include <dw/types.h>

typedef struct {
	int32_t victimId;
	int32_t active;
	SVECTOR position;
	int32_t effectId;
	int32_t subEffectIndex;
	int32_t casterId;
} AttackObject;

void initializeAttackObjects(void);
int32_t addAttackObject(int32_t victimId, int32_t active, SVECTOR *pos, int32_t effectId, int32_t subEffectIndex,
			int32_t casterId);
int32_t popAttackObject(int32_t entityId, AttackObject *out);

#endif
