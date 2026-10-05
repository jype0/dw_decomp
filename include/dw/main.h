#ifndef DW_MAIN_H
#define DW_MAIN_H

#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>

#define GENERAL_BUFFER		((uint8_t *)0x80010000)
#define TEXTURE_BUFFER		((uint8_t *)0x80088800)
#define BOSS_EFE_TMD_BUFFER	((uint8_t *)0x80058000)

typedef struct {
	VECTOR playerPos;
	VECTOR partnerPos;
	uint8_t unk20[0x80];
	int32_t partnerType;
	int32_t money;
	PartnerPara partnerPara;
	Stats partnerStats;
	int8_t lives;
	int8_t npcMaps[8];
	uint8_t currentScreen;
	uint8_t previousScreen;
	uint8_t currentExit;
	uint8_t previousExit;
	int8_t tamerWaypointX[30];
	int8_t tamerWaypointY[30];
	int8_t tamerPreviousTileX;
	int8_t tamerPreviousTileY;
	int8_t tamerWaypointCurrent;
	int8_t tamerWaypointCount;
	int8_t tamerStartTileX;
	int8_t tamerStartTileY;
	int8_t tamerWaypointActive;
} SavedState;

extern uint8_t *GENERAL_BUFFER_PTR;
extern SavedState SAVED_STATE;

#endif
