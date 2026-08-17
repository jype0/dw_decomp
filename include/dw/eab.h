#ifndef DW_EAB_H
#define DW_EAB_H

#include <dw/entity.h>
#include <dw/types.h>

void EAB_startBuildup(Entity *entity);
int32_t EAB_tick(Entity *entity, int32_t isInitialized);

#endif
