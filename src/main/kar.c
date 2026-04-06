#include <dw/kar.h>
#include <dw/utils.h>

extern uint8_t KAR_LOADING_COMPLETE;

void checkCurlingMap(int32_t mapId)
{
    if (mapId == 136) loadDynamicLibrary(KAR_REL, &KAR_LOADING_COMPLETE, DW_FALSE, NULL, NULL);
}
