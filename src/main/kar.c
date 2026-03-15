#include <dw/kar.h>
#include <dw/utils.h>

#pragma optimization_level 4

#pragma section sbss begin
extern uint8_t KAR_LOADING_COMPLETE;
#pragma section sbss end

void checkCurlingMap(int32_t mapId)
{
    if (mapId == 136) loadDynamicLibrary(KAR_REL, &KAR_LOADING_COMPLETE, DW_FALSE, NULL, NULL);
}
