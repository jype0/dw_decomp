#include <dw/kar.h>

#pragma optimization_level 4

#pragma section sbss begin
extern void* MAIN_D_80134E88;
#pragma section sbss end

void checkCurlingMap(int32_t mapId)
{
    if (mapId == 136) loadDynamicLibrary(KAR_REL, &MAIN_D_80134E88, false, NULL, NULL);
}
