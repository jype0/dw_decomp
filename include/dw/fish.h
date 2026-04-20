#ifndef DW_FISH_H
#define DW_FISH_H

#include <libgpu.h>
#include <libgte.h>

#include <dw/types.h>

typedef struct {
	int32_t field0_0x0;
	int32_t field1_0x4;
	DVECTOR* field2_0x8;
	int32_t field3_0xc;
} FishingSpot;

typedef struct {
	int16_t mapId;
	int16_t field1_0x2;
	int32_t field2_0x4;
	int32_t field3_0x8;
	int32_t field4_0xc;
	int32_t field5_0x10;
	int16_t seadramonTime1;
	int16_t seadramonTime2;
	FishingSpot* fishingSpots;
	int32_t field9_0x1c;
	void* field10_0x20;
	void* field11_0x24;
	int16_t field12_0x28;
	int16_t field13_0x2a;
	char* spriteFilePath;
	DVECTOR* seadramonWaypoints;
	int32_t field16_0x34;
} FishingMapData;

typedef struct {
	uint32_t objCount;
	uint8_t* bufferEnd;
	uint8_t* field2_0x8;
	uint8_t* targetBuffer;
} TMDFileLoadingData;

typedef struct {
	uint8_t type;
	uint8_t field1_0x1;
	uint16_t size;
	uint16_t transparency;
	uint16_t field4_0x6;
	VECTOR pos;
	SVECTOR rotation;
	int32_t field7_0x20;
	int32_t target;
	uint16_t field9_0x28;
	uint16_t field10_0x2a;
	int32_t lifetime;
	int32_t timeNextMove;
	int32_t timeThisMove;
} Fish;

typedef struct {
	VECTOR field0_0x0;
	uint32_t field1_0x10;
	int32_t field2_0x14;
	int32_t field3_0x18;
	uint32_t field4_0x1c;
	int32_t field5_0x20;
} FishingStruct1;

typedef struct {
	uint8_t field0_0x0;
	uint8_t field1_0x1;
	uint16_t field2_0x2;
	VECTOR field3_0x4;
} FishingStruct2;

typedef struct {
	int32_t displayRod;
	int32_t fishingState;
	int32_t framesOnState;
	int32_t itemSlot;
	int32_t bait;
	int32_t throwPower;
	int32_t itemTaken;
	int32_t swimmerType;
	int32_t displaySwimmer;
	VECTOR lineStart;
	VECTOR lineStartMirror;
	int32_t displayLine;
	int32_t pressedFrames;
} FishingRod;

typedef struct {
	uint32_t header;
	int32_t field1_0x4;
	int32_t field2_0x8;
	int32_t field3_0xc;
	int32_t field4_0x10;
	int32_t field5_0x14;
	uint8_t field6_0x18[60];
	int32_t field7_0x54;
	int32_t field8_0x58;
	RECT field9_0x5c;
	RECT field10_0x64;
	int16_t field11_0x6c;
	int16_t field12_0x6e;
	int32_t field13_0x70;
	Fish fishes[33];
	Fish* fishPtr;
	int32_t fishCount;
	int32_t field17_0x7b4;
	int32_t frameCount;
	int32_t escapeTimer;
	FishingRod rod;
	uint8_t hookedType;
	uint8_t field22_0x80d;
	int16_t hookedSize;
	int16_t hookedTransparency;
	int16_t field25_0x812;
	VECTOR hookedLocation;
	SVECTOR hookedRotation;
	int32_t hookedFrames;
	int32_t hookedState;
	int32_t hookedMovementFrames;
	int32_t startTime;
	int32_t stopTime;
	int32_t speed;
	int32_t nibbleTime;
	int32_t catchingTime;
	int32_t animMode;
	int32_t powerLeft;
	int32_t powerMax;
	int32_t field39_0x858;
	int32_t fishEscaped;
	int32_t gameFrozen;
	int16_t field42_0x864;
	int16_t field43_0x866;
	int32_t field44_0x868;
	int32_t field45_0x86c;
	int32_t field46_0x870;
	int32_t field47_0x874;
	uint32_t field48_0x878[3];
	int32_t field49_0x884;
	int32_t scrollToX;
	int32_t scrollToY;
	int32_t scrollToZ;
	int32_t field53_0x894;
	int32_t isScrolling;
	int32_t field55_0x89c;
	int32_t displayTension;
	uint32_t field57_0x8a4[2];
	uint32_t tension;
	uint32_t field59_0x8b0;
	int32_t tensionLimit;
	int32_t lineColor;
	int32_t field62_0x8bc;
	VECTOR field63_0x8c0;
	int32_t itemSprite;
	VECTOR swimmerLoc;
	uint32_t field66_0x8e4[3];
	int32_t field67_0x8f0;
	VECTOR field68_0x8f4;
	int32_t field69_0x904;
	int32_t field70_0x908;
	int32_t field71_0x90c;
	int32_t field72_0x910;
	uint32_t field73_0x914;
	uint8_t messageBuffer[32];
	TMDFileLoadingData rodModel;
	TMDFileLoadingData seadraModel;
	int32_t fishingEnabled;
	uint32_t mapId;
	FishingMapData* mapDataPtr;
	FishingStruct1 struct1data[16];
	uint32_t field81_0xba4;
	uint32_t field82_0xba8;
	FishingStruct1* struct1ptr;
	int32_t field84_0xbb0;
	FishingStruct2 struct2data[16];
	uint32_t field86_0xcf4;
	FishingStruct2* struct2ptr;
	DVECTOR* seadramonPath;
	int32_t field89_0xd00;
	int32_t field90_0xd04;
	uint8_t rodModelBuffer[4096];
	uint8_t seadraModelBuffer[4096];
	int32_t field93_0x2d08;
	uint32_t trailer;
} FishingData;

extern FishingData *FISHING_DATA_PTR;
extern FishingMapData FISHING_MAP_DATA[];
extern FishingData FISHING_DATA;

void FISH_init(void);
void FISH_deinitialize(void);
int32_t FISH_tamerTick(void);
void MAIN_func_800FF900(VECTOR *v);
void FISH_loadFishing(FishingRod *fishingRod);

int32_t getBestFishingRod(void);
int32_t hasFoodItem(void);
void setFishingDisabled(void);
void setFishingEnabled(void);
int32_t hasFishingRod(void);
void initializeFishing(void);
void tickFishing(void);
int32_t isFishing(void);
void deinitializeFishing(void);
void checkFishingMap(uint32_t mapId);

#endif
