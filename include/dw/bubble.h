#ifndef DW_BUBBLE_H
#define DW_BUBBLE_H

#include <dw/entity.h>
#include <dw/types.h>

void initializeConditionBubbles(void);
int32_t addConditionBubble(int32_t type, Entity *entity);
void tickConditionBubble(int32_t instanceId);
void renderConditionBubble(int32_t instanceId);
void unsetBubble(int32_t instanceId);

#endif
