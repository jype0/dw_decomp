#ifndef DW_FISH_H
#define DW_FISH_H

#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/types.h>

typedef struct {
	uint8_t item;
	uint8_t threshold;
} FishBaitChance;

typedef struct {
	uint16_t tpage;
	uint16_t clut;
	uint8_t u0;
	uint8_t v0;
	uint8_t u1;
	uint8_t v1;
	uint8_t ox;
	uint8_t oy;
} FishSprite;

typedef struct {
	int32_t unk0;
	int32_t lo;
	int32_t hi;
	int32_t moveDelayMin;
	int32_t moveDelayMax;
	int16_t bait;
	int16_t unk16;
} FishKindInfo;

typedef struct {
	DVECTOR *path;
	int32_t pathLength;
	int32_t unk8;
} FishSeadramon;

typedef struct {
	uint8_t unk0;
	uint8_t unk1;
	uint8_t unk2;
	uint8_t unk3;
	int32_t unk4;
	DVECTOR *points;
	uint8_t pointCount;
	uint8_t unkD;
	uint8_t unkE;
	uint8_t unkF;
} FishingSpot;

typedef struct {
	int16_t mapId;
	int16_t unk2;
	int32_t scale;
	int32_t unk8;
	int32_t fishScaleSmall;
	int32_t fishScaleLarge;
	int16_t seadramonTime1;
	int16_t seadramonTime2;
	FishingSpot *fishingSpots;
	int32_t unk1C;
	int32_t (*isWater)(int32_t x, int32_t y);
	void *unk24;
	int16_t castRange;
	int16_t unk2A;
	char *spriteFilePath;
	DVECTOR *seadramonWaypoints;
	int32_t seadramonWaypointCount;
} FishingMapData;

typedef struct {
	int32_t objCount;
	GsDOBJ2 *objects;
	GsDOBJ2 *objectsEnd;
	uint8_t *targetBuffer;
} TMDFileLoadingData;

typedef struct {
	uint8_t active : 1;
	uint8_t kind : 4;
	uint8_t unk5 : 3;
	uint8_t unk1;
	int16_t size;
	int16_t transparency;
	uint16_t unk6;
	VECTOR pos;
	SVECTOR rotation;
	int32_t animPhase;
	int32_t state;
	int32_t framesOnState;
	int32_t lifetime;
	int32_t timeNextMove;
	int32_t timeThisMove;
} Fish;

typedef struct {
	Fish items[33];
	Fish *next;
	int32_t count;
	int32_t unk740;
	int32_t frameCount;
	int32_t escapeTimer;
} FishPool;

typedef struct {
	VECTOR pos;
	int32_t kind;
	int32_t frame;
	int32_t sprite;
	uint32_t active;
	int32_t scale;
} FishRipple;

typedef struct {
	int8_t active;
	int8_t lifetime;
	int16_t sprite;
	VECTOR pos;
} FishMarker;

typedef struct {
	FishRipple items[16];
	int32_t count;
	FishRipple *attractor;
	FishRipple *next;
	int32_t attractRadius;
} FishRipplePool;

typedef struct {
	FishMarker items[16];
	int32_t count;
	FishMarker *next;
} FishMarkerPool;

typedef union {
	int32_t word;
	uint8_t rgb[4];
} FishColor;

typedef struct {
	int32_t display;
	uint32_t unk4[2];
	int32_t level;
	int32_t framesAtLimit;
	int32_t limit;
	FishColor color;
} FishingTension;

typedef struct {
	int32_t shown;
	int16_t boxId;
	RECT startPos;
	RECT finalPos;
	int16_t unk16;
} FishingBox;

typedef struct {
	int32_t shown;
	int32_t totalRows;
	int32_t column;
	int32_t cursorRow;
	int32_t rowOffset;
	int16_t items[30];
	int32_t lastRow;
	int32_t visibleRows;
	RECT startPos;
	RECT finalPos;
	int16_t boxId;
	int16_t unk6A;
	int32_t selectedItem;
} FishingBaitMenu;

typedef struct {
	FishingBox box;
	char *lines[4];
	VECTOR scrollTo;
	int32_t isScrolling;
	int32_t framesOpen;
} FishingView;

typedef struct {
	int32_t shown;
	VECTOR pos;
	int32_t sprite;
} FishingItem;

typedef struct {
	Fish fish;
	int32_t nibbleTime;
	int32_t catchingTime;
	int32_t animMode;
	int32_t powerLeft;
	int32_t powerMax;
	int32_t strikeTimer;
	int32_t escaped;
} HookedFish;

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
	VECTOR pos;
	int32_t state;
	int32_t timer;
	int32_t sprite;
	int32_t mode;
	VECTOR vel;
	int32_t dist;
	int32_t unk34;
	int32_t unk38;
	int32_t gravity;
	int32_t unk40;
} FishingSwimmer;

typedef struct {
	uint32_t header;
	FishingBaitMenu baitMenu;
	FishPool fishPool;
	FishingRod rod;
	HookedFish hooked;
	FishingView view;
	FishingTension tension;
	FishingItem item;
	FishingSwimmer swimmer;
	char messageBuffer[32];
	TMDFileLoadingData rodModel;
	TMDFileLoadingData seadramonModel;
	int32_t fishingDisabled;
	uint32_t mapId;
	FishingMapData *mapData;
	FishRipplePool ripplePool;
	FishMarkerPool markerPool;
	FishSeadramon seadramon;
	uint8_t rodModelBuffer[4096];
	uint8_t seadramonModelBuffer[4096];
	int32_t catchComplete;
	uint32_t trailer;
} FishingData;

extern FishingData *FISHING_DATA_PTR;
extern FishingMapData FISHING_MAP_DATA[];
extern FishingData FISHING_DATA;

extern int8_t MAIN_D_80134958[6];
extern int8_t MAIN_D_80134960[7];
extern int8_t MAIN_D_80134968[3];
extern int8_t MAIN_D_8013496C[5];
extern int16_t MAIN_D_80134974[4];
extern SVECTOR MAIN_D_8013497C;
extern SVECTOR MAIN_D_80134984;
extern SVECTOR MAIN_D_8013498C;
extern SVECTOR MAIN_D_80134994;
extern int8_t MAIN_D_8013499C[8];
extern uint8_t MAIN_D_801349A4[8];
extern uint8_t MAIN_D_801349AC[4];
extern uint8_t MAIN_D_801349B0[6];
extern uint8_t MAIN_D_801349B8[6];
extern SVECTOR MAIN_D_801349C0;
extern SVECTOR MAIN_D_801349C8;
extern SVECTOR MAIN_D_801349D0;
extern char FISH_TEXT_HOOKED[8];

int32_t getBestFishingRod(void);
int32_t hasFoodItem(void);
void setFishingDisabled(void);
void setFishingEnabled(void);
int32_t getFishingAvailability(void);
void initializeFishing(void);
void tickFishing(void);
int32_t isFishing(void);
void deinitializeFishing(void);
void checkFishingMap(uint32_t mapId);

int32_t FISH_isFishableWater1(int32_t x, int32_t y);
int32_t FISH_isFishableWater2(int32_t x, int32_t y);
void FISH_waterTestStub1(void);
void FISH_waterTestStub2(void);
void FISH_loadFishing(FishingRod *rod);
void FISH_init(void);
void FISH_deinitialize(void);
int32_t FISH_tamerTick(void);
void FISH_loadTMDModel(TMDFileLoadingData *model, char *path, uint8_t *buf, uint32_t bufSize);
GsDOBJ2 *FISH_getModelObject(TMDFileLoadingData *model, int32_t idx);
int32_t FISH_remapClamped(int32_t x, int32_t x0, int32_t x1, int32_t y0, int32_t y1);

#endif
