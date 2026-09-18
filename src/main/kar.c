#include <libgpu.h>

#include <dw/kar.h>
#include <dw/utils.h>

extern uint8_t KAR_LOADING_COMPLETE;

KarWeightTbl MAIN_D_80134A08 = { { 30, 25, 35, 30 } };
RECT MAIN_D_80134A10 = { -130, 42, 262, 59 };
RECT MAIN_D_80134A18 = { 75, -5, 10, 10 };
KarStrPair MAIN_D_80134A20 = { { KAR_D_8005ABB8, NULL } };
RECT MAIN_D_80134A28 = { -130, 42, 262, 59 };
RECT MAIN_D_80134A30 = { 0, 0, 10, 10 };
char MAIN_D_80134A38[] = " ";
char MAIN_D_80134A3C[8] = "I win!";
KarTallyValues MAIN_D_80134A44 = { { 2, 1, -2, 2 } };
int8_t MAIN_D_80134A48[4] = { 0 };
KarPeggedModelIds MAIN_D_80134A4C = { { 11, 10, 9 } };
int8_t MAIN_D_80134A4F = -1;

void checkCurlingMap(int32_t mapId)
{
	if (mapId == 136) {
		loadDynamicLibrary(KAR_REL, &KAR_LOADING_COMPLETE, DW_FALSE, NULL, NULL);
	}
}
