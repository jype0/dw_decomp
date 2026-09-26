#include <stdio.h>

#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <mwinline_n.h>

#include <dw/anim.h>
#include <dw/btl.h>
#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/file.h>
#include <dw/fish.h>
#include <dw/font.h>
#include <dw/item.h>
#include <dw/params.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/types.h>
#include <dw/ui.h>

#define FISH_SCRATCH		((FishScratch *)getScratchAddr(0))

typedef struct {
	uint32_t unk0[3];
	int32_t sz;
	uint32_t flag;
	int32_t otz;
	int32_t unk18;
	uint32_t unk1C[3];
	int32_t x;
	int32_t y;
	uint32_t unk30[4];
	uint32_t unk40;
	SVECTOR rot;
	uint32_t unk4C[8];
	SVECTOR cur;
	SVECTOR prev;
	SVECTOR pts[4];
	VECTOR v0;
	uint32_t unkAC[4];
	VECTOR v1;
	uint32_t unkCC[4];
	MATRIX m[4];
	u_long ot[1];
} FishScratch;

extern int32_t FISH_D_8007A4D8[];
extern int32_t FISH_D_8007A4E4[];
extern int32_t FISH_D_8007A4F8[];
extern int32_t FISH_D_8007A540[];
extern int32_t FISH_D_8007A554[];
extern int16_t MAIN_D_80154F80[];
extern VECTOR FISH_REEL_TARGET;
extern char FISH_MSG_TOOK_BAIT[];
extern char FISH_MSG_TOO_LATE[];
extern char FISH_MSG_GOT_BAIT[];
extern char FISH_MSG_TOO_FAST[];
extern char FISH_MSG_STILL_HAVE_BAIT[];
extern char FISH_MSG_LENGTH_GREAT[];
extern char FISH_MSG_LENGTH_M_CM[];
extern char FISH_MSG_LENGTH_CM[];
extern char FISH_MSG_GOT_ONE[];
extern char FISH_MSG_LET_IT_GO[];
extern char FISH_MSG_LINE_CUT[];
extern char FISH_MSG_WATCH_TENSION[];
extern char FISH_MSG_BE_CALM[];
extern char FISH_MSG_HOOK_LOST[];
extern char FISH_MSG_STUCK_LINE1[];
extern char FISH_MSG_STUCK_LINE2[];
extern char FISH_MSG_BAG_FULL_LINE1[];
extern char FISH_MSG_BAG_FULL_LINE2[];
extern char FISH_MSG_IT_GOT_STUCK[];
extern int16_t DRAW_OFFSET_LIMIT_Y_MIN;
extern int16_t DRAW_OFFSET_LIMIT_Y_MAX;
extern int16_t DRAW_OFFSET_LIMIT_X_MIN;
extern int16_t DRAW_OFFSET_LIMIT_X_MAX;
extern char FISH_PATH_SEADRAMON_MODEL[];
extern FishBaitChance FISH_BAIT_CHANCES[];
extern FishSprite FISH_SPRITES[];
extern FishKindInfo FISH_KIND_INFO[];
extern MATRIX FISH_ROD_SEGMENT_MATRIX[];
extern DVECTOR FISH_D_8007A890[];
extern SVECTOR FISH_SEADRAMON_HISTORY[];
extern SVECTOR FISH_D_8007A56C[];
extern int8_t PARTNER_STATE;
extern uint8_t TEXTBOX_OPEN_TIMER;
extern int32_t VIEWPORT_DISTANCE;
extern int8_t FISH_D_8007A4B4[];
extern FishSprite FISH_D_8007A340[];
extern uint8_t MAIN_D_80127BDC[];
extern int8_t FISH_BITE_CHANCE[];
extern SVECTOR FISH_D_8007A5BC[];
extern SVECTOR FISH_D_8007A594[];
extern VECTOR FISH_ROD_SEGMENT_POS[];
extern DVECTOR FISH_D_8007A9B0[];
extern char FISH_PATH_ROD_MODEL[];
extern int16_t CAMERA_X[];
extern int16_t CAMERA_Y[];

int32_t getDistance(int32_t x, int32_t y, int32_t z);
int32_t customRandom(int32_t a, int32_t b);
void renderSelectionCursor(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
void renderSmallNumber(int32_t color, int32_t n, int32_t x, int32_t y, int32_t value, int32_t layer);
void renderLinePrimitive(uint32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t layer, int32_t semiTrans);
void setCameraFollowPlayer(void);
void unsetCameraFollowPlayer(void);
void toEulerAngles(SVECTOR *out, int32_t x, int32_t y, int32_t z);
void rotateVectorYXZ(SVECTOR *rotation, VECTOR *input, VECTOR *output);
void MAIN_func_800E4038(VECTOR *out, int32_t x, int32_t z, int32_t *f);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void renderUIBox(int32_t id);
void renderItemSprite(int32_t itemId, int32_t x, int32_t y, int32_t depth);
void calculatePosition(GsCOORDINATE2 *coord, MATRIX *matrix);
void matrixToEuler2(MATRIX *m, SVECTOR *out);

int32_t FISH_panCameraTo(int32_t radius, VECTOR *pos, int32_t maxDX, int32_t maxDY);
int32_t FISH_panCameraToTamer(int32_t radius);
int32_t FISH_panCameraWithDPad(void);
int32_t FISH_isScreenPointVisible(int32_t x, int32_t y);
int32_t FISH_projectPoint(VECTOR *point, int32_t *outX, int32_t *outY);
int32_t FISH_isWithinCastRange(VECTOR *pos, int32_t extra);
int32_t FISH_isBaitMenuVisible(void);
void FISH_closeBaitMenu(void);
void FISH_tickBaitMenu(void);
void FISH_drawBaitMenuRow(int32_t row);
void FISH_renderBaitMenu(int32_t boxId);
int32_t FISH_openBaitMenu(void);
void FISH_initBaitMenu(void);
void FISH_closeBaitMenuIfOpen(void);
void FISH_scrollViewToTarget(int32_t unused);
int32_t FISH_isTextBoxVisible(void);
void FISH_closeTextBox(void);
void FISH_tickTextBox(void);
void FISH_renderTextBox(int32_t boxId);
int32_t FISH_showTextBox(char *line0, char *line1, char *line2, char *line3, VECTOR *target);
void FISH_initTextBox(void);
void FISH_shutdownTextBox(void);
void FISH_randomPointInSpot(int32_t index, VECTOR *out);
void FISH_drawStub(void);
void FISH_drawSprite2D(int32_t x, int32_t y, int32_t order, FishSprite *desc, int32_t sx, int32_t sy, int32_t rot, int32_t color, int32_t flags);
void FISH_drawSprite3D(VECTOR *pos, SVECTOR *rot, VECTOR *scale, FishSprite *desc, int32_t color, int32_t flags, int32_t depth);
void FISH_freeMarker(FishMarker *s);
void FISH_tickMarker(FishMarker *s);
void FISH_drawMarker(FishMarker *s);
void FISH_initMarkerPool(void);
FishMarker *FISH_allocMarker(void);
void FISH_spawnMarker(int32_t sprite, VECTOR *pos);
void FISH_tickMarkers(void);
void FISH_drawMarkers(void);
void FISH_clearMarkers(void);
void FISH_freeRipple(FishRipple *s);
void FISH_tickRipple(FishRipple *s);
void FISH_drawRipple(FishRipple *s);
void FISH_initRipplePool(void);
FishRipple *FISH_allocRipple(void);
void FISH_spawnRipple(int32_t kind, VECTOR *pos, int32_t attractRadius, int32_t spread, int32_t scale);
void FISH_tickRipples(void);
void FISH_drawRipples(void);
void FISH_loadSpritesAndSeadramon(TMDFileLoadingData *model);
void FISH_onRodModelLoaded(TMDFileLoadingData *model);
void FISH_resetTension(void);
void FISH_beginTension(void);
int32_t FISH_updateTension(void);
void FISH_drawTensionGauge(void);
void FISH_hideBaitSprite(void);
void FISH_initBaitSprite(void);
void FISH_clearBaitSprite(void);
void FISH_showBaitSprite(VECTOR *pos, int32_t sprite);
int32_t FISH_moveBaitToRodTip(int32_t maxStep);
void FISH_tickBaitSprite(void);
void FISH_drawBaitSprite(void);
void FISH_resetBobberAnim(FishRipple *s);
void FISH_initBobberAnim(FishRipple *s);
void FISH_clearBobberAnim(FishRipple *s);
void FISH_getRodTipPos(VECTOR *out);
void FISH_bobberCommand(FishingSwimmer *s, int32_t command);
void FISH_tickBobberPhysics(void);
void FISH_pushSeadramonSegment(VECTOR *unused, SVECTOR *src);
void FISH_drawSeadramon(VECTOR *start, SVECTOR *rot);
void FISH_spawnSeadramon(Fish *f);
void FISH_tickSeadramon(Fish *f);
void FISH_spawnFish(Fish *f, int32_t kind);
void FISH_tickFish(Fish *f);
void FISH_drawFishSprite(int32_t kind, VECTOR *pos, SVECTOR *rot, int32_t size, int32_t phase, int32_t transparency, int32_t flag);
void FISH_drawFish(Fish *f);
Fish *FISH_allocFish(void);
int32_t FISH_isSeadramonSpawned(void);
void FISH_tickFishPopulation(void);
void FISH_drawFishPool(void);
void FISH_initFishPool(void);
void FISH_fishPoolStub(void);
Fish *FISH_findFishToBite(Fish *pool, VECTOR *loc, int32_t depth);
void FISH_clearHookedFish(void);
void FISH_hookFish(Fish *f);
void FISH_tickHookedFish(int32_t mode);
int32_t FISH_tryStrike(void);
void FISH_drawHookedFish(void);
void FISH_updateRodPose(FishingRod *rod);
void FISH_drawRodSegments(FishingRod *rod);
int32_t FISH_clipLineToRect(SVECTOR *pts, int32_t xMin, int32_t xMax, int32_t yMin, int32_t yMax);
void FISH_drawFishingLine(FishingRod *rod);
void FISH_resetRodState(FishingRod *rod);
void FISH_tickRod(FishingRod *rod);
void FISH_tickFishingWorld(int32_t instanceId);
void FISH_renderFishingWorld(int32_t instanceId);
int32_t FISH_lerpTableByX(int32_t x, int32_t base, DVECTOR *tbl, int32_t n);
int32_t FISH_lerpTableByY(int32_t base, int32_t y, DVECTOR *tbl, int32_t n);
void FISH_getBoneTransform(Entity *e, int32_t bone, VECTOR *outPos, SVECTOR *outRot);
void FISH_drawItemSprite(int32_t itemId, int32_t x, int32_t y, int32_t depth, int32_t unused);
void FISH_endFishing(FishingRod *rod);
void FISH_scrollCameraTo(int32_t x, int32_t y);
void FISH_applySavedCameraScroll(void);
void FISH_enableCameraControl(void);
void FISH_unloadModel(TMDFileLoadingData *model);
void handleTileUpdate(uint32_t mask, int32_t flag);

static void *fish_functions[] = {
	FISH_unloadModel,
	FISH_enableCameraControl,
	FISH_applySavedCameraScroll,
	FISH_scrollCameraTo,
	FISH_tamerTick,
	FISH_deinitialize,
	FISH_init,
	FISH_loadFishing,
	FISH_endFishing,
	FISH_drawItemSprite,
	FISH_getBoneTransform,
	FISH_waterTestStub2,
	FISH_isFishableWater2,
	FISH_waterTestStub1,
	FISH_isFishableWater1,
	FISH_lerpTableByY,
	FISH_lerpTableByX,
	FISH_renderFishingWorld,
	FISH_tickFishingWorld,
	FISH_tickRod,
	FISH_resetRodState,
	FISH_drawFishingLine,
	FISH_clipLineToRect,
	FISH_drawRodSegments,
	FISH_updateRodPose,
	FISH_drawHookedFish,
	FISH_tryStrike,
	FISH_tickHookedFish,
	FISH_hookFish,
	FISH_clearHookedFish,
	FISH_findFishToBite,
	FISH_fishPoolStub,
	FISH_initFishPool,
	FISH_drawFishPool,
	FISH_tickFishPopulation,
	FISH_isSeadramonSpawned,
	FISH_allocFish,
	FISH_drawFish,
	FISH_drawFishSprite,
	FISH_tickFish,
	FISH_spawnFish,
	FISH_tickSeadramon,
	FISH_spawnSeadramon,
	FISH_drawSeadramon,
	FISH_pushSeadramonSegment,
	FISH_tickBobberPhysics,
	FISH_bobberCommand,
	FISH_getRodTipPos,
	FISH_clearBobberAnim,
	FISH_initBobberAnim,
	FISH_resetBobberAnim,
	FISH_drawBaitSprite,
	FISH_tickBaitSprite,
	FISH_moveBaitToRodTip,
	FISH_showBaitSprite,
	FISH_clearBaitSprite,
	FISH_initBaitSprite,
	FISH_hideBaitSprite,
	FISH_drawTensionGauge,
	FISH_updateTension,
	FISH_beginTension,
	FISH_resetTension,
	FISH_onRodModelLoaded,
	FISH_loadSpritesAndSeadramon,
	FISH_drawRipples,
	FISH_tickRipples,
	FISH_spawnRipple,
	FISH_allocRipple,
	FISH_initRipplePool,
	FISH_drawRipple,
	FISH_tickRipple,
	FISH_freeRipple,
	FISH_clearMarkers,
	FISH_drawMarkers,
	FISH_tickMarkers,
	FISH_spawnMarker,
	FISH_allocMarker,
	FISH_initMarkerPool,
	FISH_drawMarker,
	FISH_tickMarker,
	FISH_freeMarker,
	FISH_drawSprite3D,
	FISH_drawSprite2D,
	FISH_drawStub,
	FISH_randomPointInSpot,
	FISH_shutdownTextBox,
	FISH_initTextBox,
	FISH_showTextBox,
	FISH_renderTextBox,
	FISH_tickTextBox,
	FISH_closeTextBox,
	FISH_isTextBoxVisible,
	FISH_scrollViewToTarget,
	FISH_closeBaitMenuIfOpen,
	FISH_initBaitMenu,
	FISH_openBaitMenu,
	FISH_renderBaitMenu,
	FISH_drawBaitMenuRow,
	FISH_tickBaitMenu,
	FISH_closeBaitMenu,
	FISH_isBaitMenuVisible,
	FISH_isWithinCastRange,
	FISH_projectPoint,
	FISH_isScreenPointVisible,
	FISH_panCameraWithDPad,
	FISH_panCameraToTamer,
	FISH_panCameraTo,
};

int8_t MAIN_D_80134958[6] = { 1, 2, 3, 4, 5, -1 };
int8_t MAIN_D_80134960[7] = { 0, 1, 2, 3, 4, 5, -1 };
int8_t MAIN_D_80134968[3] = { 4, 5, -1 };
int8_t MAIN_D_8013496C[5] = { 2, 3, 4, 5, -1 };
int16_t MAIN_D_80134974[4] = { -95, -64, 0, 0 };
SVECTOR MAIN_D_8013497C = { 28, 0, 2, 0 };
SVECTOR MAIN_D_80134984 = { 0, 93, 0, 0 };
SVECTOR MAIN_D_8013498C = { 28, 0, 2, 0 };
SVECTOR MAIN_D_80134994 = { 0, 17, 0, 0 };
int8_t MAIN_D_8013499C[8] = { 1, 2, 3, 4, 5, 4, 3, -1 };
uint8_t MAIN_D_801349A4[8] = { 39, 40, 41, 40, 39, 45, 44, 45 };
uint8_t MAIN_D_801349AC[4] = { 33, 34, 33, 35 };
uint8_t MAIN_D_801349B0[6] = { 41, 42, 40, 44, 43, 45 };
uint8_t MAIN_D_801349B8[6] = { 34, 36, 34, 35, 37, 35 };
SVECTOR MAIN_D_801349C0 = { 1024, 1024, 0, 0 };
SVECTOR MAIN_D_801349C8 = { 1, 19, -48, 0 };
SVECTOR MAIN_D_801349D0 = { 0 };
char MAIN_D_801349D8[] = "Hooked!";
int32_t MAIN_D_801349E0 = 2;

int32_t MAIN_D_801351D8;
int16_t MAIN_D_801351DC[3];

// clang-format off
FishBaitChance FISH_BAIT_CHANCES[6] = {
	{ 0x00, 0x22 },
	{ 0x08, 0x3e },
	{ 0x0b, 0x52 },
	{ 0x12, 0x5c },
	{ 0x16, 0x61 },
	{ 0x71, 0x63 },
};

FishSprite FISH_SPRITES[32] = {
	{ 0x0017, 0x601c, 0xd0, 0xb4, 0xdf, 0xc3, 0x08, 0x0a },
	{ 0x0017, 0x601c, 0x80, 0xd4, 0x9f, 0xe3, 0x10, 0x08 },
	{ 0x0017, 0x601c, 0xa0, 0xd4, 0xbf, 0xe3, 0x10, 0x08 },
	{ 0x0017, 0x601c, 0xc0, 0xd4, 0xdf, 0xe3, 0x10, 0x08 },
	{ 0x0017, 0x601c, 0x80, 0xe4, 0x9f, 0xf3, 0x10, 0x08 },
	{ 0x0017, 0x601c, 0x80, 0xb4, 0x8f, 0xc3, 0x08, 0x08 },
	{ 0x0017, 0x601c, 0x90, 0xb4, 0x9f, 0xc3, 0x08, 0x08 },
	{ 0x0017, 0x601c, 0xa0, 0xb4, 0xaf, 0xc3, 0x08, 0x08 },
	{ 0x0017, 0x601c, 0xb0, 0xb4, 0xbf, 0xc3, 0x08, 0x08 },
	{ 0x0017, 0x601c, 0xc0, 0xb4, 0xcf, 0xc3, 0x08, 0x08 },
	{ 0x0017, 0x601c, 0x80, 0xb4, 0x8f, 0xc3, 0x08, 0x08 },
	{ 0x0017, 0x601c, 0x90, 0xc4, 0x9f, 0xd3, 0x08, 0x08 },
	{ 0x0017, 0x601c, 0xa0, 0xc4, 0xaf, 0xd3, 0x08, 0x08 },
	{ 0x0017, 0x601c, 0xb0, 0xc4, 0xbf, 0xd3, 0x08, 0x08 },
	{ 0x0017, 0x601c, 0xc0, 0xc4, 0xcf, 0xd3, 0x08, 0x08 },
	{ 0x0017, 0x601c, 0xd0, 0xc4, 0xdf, 0xd3, 0x08, 0x08 },
	{ 0x0017, 0x601c, 0xe0, 0xc4, 0xef, 0xd3, 0x08, 0x08 },
	{ 0x0017, 0x609c, 0x80, 0x8c, 0x8f, 0x9b, 0x08, 0x0a },
	{ 0x0017, 0x609c, 0x90, 0x8c, 0x9f, 0x9b, 0x08, 0x0a },
	{ 0x0017, 0x609c, 0xa0, 0x8c, 0xaf, 0x9b, 0x08, 0x0a },
	{ 0x0017, 0x609c, 0xb0, 0x8c, 0xbf, 0x9b, 0x08, 0x0a },
	{ 0x0017, 0x609c, 0xc0, 0x8c, 0xcf, 0x9b, 0x08, 0x0a },
	{ 0x0017, 0x609c, 0xd0, 0x8c, 0xdf, 0x9b, 0x08, 0x0a },
	{ 0x0017, 0x609c, 0xe0, 0x8c, 0xef, 0x9b, 0x08, 0x0a },
	{ 0x0017, 0x609c, 0x80, 0x9c, 0x8f, 0xb3, 0x08, 0x12 },
	{ 0x0017, 0x609c, 0x90, 0x9c, 0x9f, 0xb3, 0x08, 0x12 },
	{ 0x0017, 0x609c, 0xa0, 0x9c, 0xaf, 0xb3, 0x08, 0x12 },
	{ 0x0017, 0x609c, 0xb0, 0x9c, 0xbf, 0xb3, 0x08, 0x12 },
	{ 0x0017, 0x609c, 0xc0, 0x9c, 0xcf, 0xb3, 0x08, 0x12 },
	{ 0x0017, 0x609c, 0xd0, 0x9c, 0xdf, 0xb3, 0x08, 0x12 },
	{ 0x0017, 0x609c, 0xe0, 0x9c, 0xef, 0xb3, 0x08, 0x12 },
	{ 0x0017, 0x605c, 0x10, 0x8c, 0x58, 0xab, 0x22, 0x13 },
};

FishSprite FISH_D_8007A340[17] = {
	{ 0x0017, 0x605c, 0x00, 0x9c, 0x03, 0xa1, 0x00, 0x00 },
	{ 0x0017, 0x609c, 0x00, 0xac, 0x0f, 0xc3, 0x08, 0x05 },
	{ 0x0017, 0x609c, 0x10, 0xac, 0x1f, 0xc3, 0x08, 0x05 },
	{ 0x0017, 0x609c, 0x20, 0xac, 0x2f, 0xc3, 0x08, 0x05 },
	{ 0x0017, 0x609c, 0x00, 0xc4, 0x0f, 0xdb, 0x08, 0x05 },
	{ 0x0017, 0x609c, 0x10, 0xc4, 0x1f, 0xdb, 0x08, 0x05 },
	{ 0x0017, 0x609c, 0x00, 0x8c, 0x0f, 0x9b, 0x08, 0x05 },
	{ 0x0017, 0x609c, 0x30, 0xac, 0x3f, 0xdb, 0x08, 0x03 },
	{ 0x0017, 0x609c, 0x40, 0xac, 0x4f, 0xdb, 0x08, 0x03 },
	{ 0x0017, 0x609c, 0x50, 0xac, 0x5f, 0xdb, 0x08, 0x03 },
	{ 0x0017, 0x609c, 0x60, 0xac, 0x77, 0xdb, 0x0c, 0x03 },
	{ 0x0017, 0x609c, 0x77, 0xac, 0x60, 0xdb, 0x0d, 0x03 },
	{ 0x0017, 0x609c, 0x5f, 0xac, 0x50, 0xdb, 0x09, 0x03 },
	{ 0x0017, 0x609c, 0x4f, 0xac, 0x40, 0xdb, 0x09, 0x03 },
	{ 0x0017, 0x611c, 0x68, 0x8c, 0x77, 0x9b, 0x08, 0x10 },
	{ 0x0017, 0x611c, 0x68, 0x9c, 0x77, 0xab, 0x08, 0x10 },
	{ 0x0017, 0x611c, 0x58, 0x8c, 0x67, 0x9b, 0x08, 0x10 },
};

int8_t FISH_BITE_CHANCE[200] = {
	0x0a, 0x14, 0x00, 0x05, 0x00, 0x00, 0x0a, 0x14,
	0x00, 0x0a, 0x00, 0x00, 0x0a, 0x14, 0x00, 0x0f,
	0x05, 0x00, 0x32, 0x05, 0x0f, 0x00, 0x00, 0x00,
	0x32, 0x05, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x0a,
	0x00, 0x00, 0x00, 0x00, 0x32, 0x05, 0x14, 0x00,
	0x00, 0x00, 0x1e, 0x0a, 0x05, 0x00, 0x00, 0x00,
	0x00, 0x0a, 0x05, 0x00, 0x05, 0x00, 0x14, 0x0a,
	0x05, 0x00, 0x00, 0x00, 0x14, 0x0a, 0x05, 0x00,
	0x00, 0x00, 0x14, 0x0a, 0x05, 0x00, 0x00, 0x00,
	0x14, 0x0a, 0x05, 0x00, 0x00, 0x00, 0x14, 0x0a,
	0x05, 0x00, 0x00, 0x00, 0x14, 0x0a, 0x05, 0x00,
	0x00, 0x00, 0x14, 0x0a, 0x05, 0x00, 0x00, 0x00,
	0x14, 0x0a, 0x05, 0x00, 0x00, 0x00, 0x14, 0x0a,
	0x05, 0x00, 0x00, 0x00, 0x14, 0x0a, 0x05, 0x00,
	0x00, 0x00, 0x14, 0x0a, 0x05, 0x00, 0x00, 0x00,
	0x14, 0x0a, 0x05, 0x00, 0x00, 0x00, 0x14, 0x0a,
	0x05, 0x00, 0x00, 0x00, 0x14, 0x0a, 0x05, 0x00,
	0x00, 0x00, 0x14, 0x0a, 0x05, 0x00, 0x00, 0x00,
	0x00, 0x0a, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0a, 0x14, 0x00, 0x00,
	0x14, 0x00, 0x50, 0x0a, 0x0a, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00,
};

int8_t FISH_D_8007A4B4[20] = {
	0x00, 0x00, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
	0x0c, 0x02, 0x03, 0x04, 0x04, 0x05, 0x05, 0x05,
	0xff, 0x00, 0x00, 0x00,
};

char FISH_PATH_SEADRAMON_MODEL[16] = "\\ETCDAT\\SEA.TMD";

int32_t FISH_D_8007A4D8[3] = {
	0x00000004, 0x00000003, 0xffffffff,
};

int32_t FISH_D_8007A4E4[5] = {
	0x00000004, 0x00000005, 0x00000004, 0x00000003,
	0xffffffff,
};

int32_t FISH_D_8007A4F8[18] = {
	0x00000004, 0x00000005, 0x00000006, 0x00000007,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000007, 0x00000006, 0x00000005, 0x00000004,
	0x00000003, 0xffffffff,
};

int32_t FISH_D_8007A540[5] = {
	0x00000004, 0x00000005, 0x00000006, 0x00000007,
	0xffffffff,
};

int32_t FISH_D_8007A554[6] = {
	0x00000007, 0x00000006, 0x00000005, 0x00000004,
	0x00000003, 0xffffffff,
};

SVECTOR FISH_D_8007A56C[5] = {
	{ 0x0000, 0x0000, 0xff38, 0x0000 },
	{ 0x0000, 0x0000, 0xff51, 0x0000 },
	{ 0x0000, 0x0000, 0xff83, 0x0000 },
	{ 0x0000, 0x0000, 0xff74, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
};

SVECTOR FISH_D_8007A594[5] = {
	{ 0x0000, 0x0000, 0x00fc, 0x0000 },
	{ 0x0000, 0x0000, 0x00b9, 0x0000 },
	{ 0x0000, 0x0000, 0x0065, 0x0000 },
	{ 0x0000, 0x0000, 0x004b, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
};

SVECTOR FISH_D_8007A5BC[5] = {
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
};

char FISH_PATH_ROD_MODEL[16] = "\\ETCDAT\\SAO.TMD";

char FISH_MSG_TOOK_BAIT[] = "They took my bait!";

char FISH_MSG_TOO_LATE[] = "Too late!";

char FISH_MSG_GOT_BAIT[] = "They got my bait!";

char FISH_MSG_TOO_FAST[] = "Too fast!";

char FISH_MSG_STILL_HAVE_BAIT[] = "But I still got my bait!";

char FISH_MSG_LENGTH_GREAT[] = "Great! Total length %d m %d cm";

char FISH_MSG_LENGTH_M_CM[24] = "Total length %d m %d cm";

char FISH_MSG_LENGTH_CM[] = "Total length %d cm";

char FISH_MSG_GOT_ONE[] = "I got one!";

char FISH_MSG_LET_IT_GO[] = "It looks sad. Let it go.";

char FISH_MSG_LINE_CUT[] = "My line got cut!";

char FISH_MSG_WATCH_TENSION[] = "Pay attention to tension gauge";

char FISH_MSG_BE_CALM[] = "and be more calm";

char FISH_MSG_HOOK_LOST[] = "My hook got lost!";

char FISH_MSG_STUCK_LINE1[28] = "Huh?! I wonder if something";

char FISH_MSG_STUCK_LINE2[] = "got stuck? It got stuck!";

char FISH_MSG_BAG_FULL_LINE1[28] = "My bag was so full, I could";

char FISH_MSG_BAG_FULL_LINE2[] = "not carry it.";

char FISH_MSG_IT_GOT_STUCK[] = "It got stuck!";

DVECTOR FISH_D_8007A794[9] = {
	{ 0xfeca, 0xfff3 },
	{ 0xfef5, 0xffe6 },
	{ 0xff6b, 0xffde },
	{ 0x006d, 0xffec },
	{ 0x00c0, 0x000d },
	{ 0x0090, 0x0060 },
	{ 0xffda, 0x006f },
	{ 0xff6b, 0x0061 },
	{ 0xfed8, 0x003f },
};

DVECTOR FISH_D_8007A7B8[7] = {
	{ 0xffa3, 0x0002 },
	{ 0x00c7, 0x0010 },
	{ 0x015c, 0x0030 },
	{ 0x0163, 0x00bf },
	{ 0x012a, 0x00ef },
	{ 0x0004, 0x00ba },
	{ 0xff95, 0x008c },
};

DVECTOR FISH_D_8007A7D4[7] = {
	{ 0xffe0, 0x004a },
	{ 0x008a, 0x004b },
	{ 0x0116, 0x0080 },
	{ 0x0178, 0x00bf },
	{ 0x0178, 0x00fd },
	{ 0x008b, 0x0111 },
	{ 0xffe0, 0x0111 },
};

DVECTOR FISH_D_8007A7F0[8] = {
	{ 0xfe8d, 0x001a },
	{ 0xfefd, 0x001a },
	{ 0xff98, 0x003e },
	{ 0xffe8, 0x0064 },
	{ 0xffe8, 0x00ae },
	{ 0xffd4, 0x00de },
	{ 0xfefd, 0x0100 },
	{ 0xfe8d, 0x0100 },
};

DVECTOR FISH_D_8007A810[4] = {
	{ 0xfe80, 0x00ca },
	{ 0xffdf, 0x00ca },
	{ 0xffdf, 0x0127 },
	{ 0xfe80, 0x0127 },
};

DVECTOR FISH_D_8007A820[4] = {
	{ 0x0032, 0x00f5 },
	{ 0x017f, 0x00f5 },
	{ 0x017f, 0x0127 },
	{ 0x0032, 0x0127 },
};

FishingSpot FISH_D_8007A830[6] = {
	{
		0x1e,
		0x00,
		0x00,
		0x00,
		0x000000ff,
		FISH_D_8007A794,
		0x09,
		0x00,
		0x00,
		0x00,
	},
	{
		0x19,
		0x00,
		0x00,
		0x00,
		0x00ff0000,
		FISH_D_8007A7B8,
		0x07,
		0x00,
		0x00,
		0x00,
	},
	{
		0x12,
		0x00,
		0x00,
		0x00,
		0x0000ff00,
		FISH_D_8007A7D4,
		0x07,
		0x00,
		0x00,
		0x00,
	},
	{
		0x0e,
		0x00,
		0x00,
		0x00,
		0x00404040,
		FISH_D_8007A7F0,
		0x08,
		0x00,
		0x00,
		0x00,
	},
	{
		0x08,
		0x00,
		0x00,
		0x00,
		0x00808080,
		FISH_D_8007A810,
		0x04,
		0x00,
		0x00,
		0x00,
	},
	{
		0x05,
		0x00,
		0x00,
		0x00,
		0x00c0c0c0,
		FISH_D_8007A820,
		0x04,
		0x00,
		0x00,
		0x00,
	},
};

DVECTOR FISH_D_8007A890[16] = {
	{ 0xfe80, 0xffde },
	{ 0xfeac, 0xffde },
	{ 0xfebb, 0xffcf },
	{ 0xfeec, 0xffcc },
	{ 0xfefb, 0xffc1 },
	{ 0xff1e, 0xffa1 },
	{ 0xff34, 0xffa5 },
	{ 0xff96, 0xffac },
	{ 0xffc6, 0xffbb },
	{ 0x003f, 0xffbb },
	{ 0x006d, 0xffc1 },
	{ 0x00e1, 0xffe6 },
	{ 0x0112, 0xfffd },
	{ 0x014c, 0x0000 },
	{ 0x0176, 0x0008 },
	{ 0x0180, 0x0008 },
};

DVECTOR FISH_D_8007A8D0[4] = {
	{ 0xff62, 0xfff1 },
	{ 0x01ab, 0xfff1 },
	{ 0x01ab, 0x0004 },
	{ 0xff61, 0x0004 },
};

DVECTOR FISH_D_8007A8E0[4] = {
	{ 0xfd90, 0x001d },
	{ 0xffec, 0x001d },
	{ 0x0012, 0x0078 },
	{ 0xfd90, 0x0076 },
};

DVECTOR FISH_D_8007A8F0[6] = {
	{ 0xfe0f, 0xfffc },
	{ 0xff41, 0xfffc },
	{ 0xffed, 0x0005 },
	{ 0xffed, 0x001c },
	{ 0xff41, 0x0036 },
	{ 0xfe0f, 0x0036 },
};

DVECTOR FISH_D_8007A908[4] = {
	{ 0xfee0, 0xffe8 },
	{ 0x002f, 0xfff1 },
	{ 0x0032, 0x0000 },
	{ 0xfee0, 0x0000 },
};

DVECTOR FISH_D_8007A918[8] = {
	{ 0xfff7, 0x0010 },
	{ 0x002b, 0x0016 },
	{ 0x0075, 0x0030 },
	{ 0x00a4, 0x004a },
	{ 0x00aa, 0x0073 },
	{ 0x0074, 0x0077 },
	{ 0x000c, 0x0074 },
	{ 0xffec, 0x001a },
};

DVECTOR FISH_D_8007A938[6] = {
	{ 0xfd90, 0xffe7 },
	{ 0xfede, 0xffe7 },
	{ 0xfede, 0xfff4 },
	{ 0xfe1c, 0x0014 },
	{ 0xfd93, 0x0014 },
	{ 0xfd90, 0xfff9 },
};

FishingSpot FISH_D_8007A950[6] = {
	{
		0x1e,
		0x00,
		0x00,
		0x00,
		0x000000ff,
		FISH_D_8007A8D0,
		0x04,
		0x00,
		0x00,
		0x00,
	},
	{
		0x19,
		0x00,
		0x00,
		0x00,
		0x00ff0000,
		FISH_D_8007A8E0,
		0x04,
		0x00,
		0x00,
		0x00,
	},
	{
		0x12,
		0x00,
		0x00,
		0x00,
		0x0000ff00,
		FISH_D_8007A8F0,
		0x06,
		0x00,
		0x00,
		0x00,
	},
	{
		0x0e,
		0x00,
		0x00,
		0x00,
		0x00404040,
		FISH_D_8007A908,
		0x04,
		0x00,
		0x00,
		0x00,
	},
	{
		0x08,
		0x00,
		0x00,
		0x00,
		0x00808080,
		FISH_D_8007A918,
		0x08,
		0x00,
		0x00,
		0x00,
	},
	{
		0x05,
		0x00,
		0x00,
		0x00,
		0x00c0c0c0,
		FISH_D_8007A938,
		0x06,
		0x00,
		0x00,
		0x00,
	},
};

DVECTOR FISH_D_8007A9B0[7] = {
	{ 0x0080, 0x0005 },
	{ 0x0034, 0x000d },
	{ 0x0034, 0x0017 },
	{ 0x007e, 0x0030 },
	{ 0x00e7, 0x0045 },
	{ 0x00f9, 0x004d },
	{ 0x010f, 0x0080 },
};

DVECTOR FISH_D_8007A9CC[8] = {
	{ 0xfd80, 0x0018 },
	{ 0xfec9, 0x0018 },
	{ 0xff2c, 0xffff },
	{ 0xff90, 0xfff8 },
	{ 0x003e, 0xfff5 },
	{ 0x0108, 0xfff2 },
	{ 0x019b, 0xffef },
	{ 0x01cd, 0xffee },
};

char FISH_PATH_FISHING_TIM[20] = "\\ETCDAT\\FISHING.TIM";

char FISH_PATH_FISHING2_TIM[] = "\\ETCDAT\\FISHING2.TIM";

VECTOR FISH_REEL_TARGET = { 0x00000000, 0x00000000, 0x00000000, 0x00000000 };

FishKindInfo FISH_KIND_INFO[7] = {
	{
		0x0000003e,
		0x00000032,
		0x0000003c,
		0x0000001e,
		0x0000003c,
		0xffff,
		0x0000,
	},
	{
		0x0000003f,
		0x00000037,
		0x00000055,
		0x0000001e,
		0x0000003c,
		0xffff,
		0x0000,
	},
	{
		0x00000040,
		0x0000003c,
		0x00000064,
		0x00000028,
		0x0000004b,
		0x0045,
		0x0000,
	},
	{
		0x00000041,
		0x00000046,
		0x00000096,
		0x00000032,
		0x0000005a,
		0x0040,
		0x0000,
	},
	{
		0x00000042,
		0x00000064,
		0x000000e6,
		0x0000003c,
		0x00000078,
		0x0044,
		0x0000,
	},
	{
		0x00000043,
		0x000000d2,
		0x0000012c,
		0x0000003c,
		0x00000078,
		0x0042,
		0x0000,
	},
	{
		0x00000000,
		0x0000012c,
		0x0000012c,
		0x0000003c,
		0x00000078,
		0xffff,
		0x0000,
	},
};

FishingMapData FISHING_MAP_DATA[2] = {
	{
		0x0008,
		0x0000,
		0x00007000,
		0x00000046,
		0x00000155,
		0x000000aa,
		0x270f,
		0x270f,
		FISH_D_8007A830,
		0x00000006,
		FISH_isFishableWater1,
		FISH_waterTestStub1,
		0x012c,
		0x0000,
		FISH_PATH_FISHING_TIM,
		FISH_D_8007A9CC,
		0x00000008,
	},
	{
		0x0006,
		0x0000,
		0x00003800,
		0x00000023,
		0x00000155,
		0x000000aa,
		0x06a4,
		0x0320,
		FISH_D_8007A950,
		0x00000006,
		FISH_isFishableWater2,
		FISH_waterTestStub2,
		0x012c,
		0x0000,
		FISH_PATH_FISHING2_TIM,
		FISH_D_8007A9CC,
		0x00000008,
	},
};
// clang-format on

int32_t FISH_panCameraTo(int32_t radius, VECTOR *pos, int32_t maxDX, int32_t maxDY)
{
	SVECTOR v;
	int32_t cx;
	int32_t cy;
	int32_t dx;
	int32_t dy;
	int32_t ang;
	int32_t ox;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	copyVector(&v, pos);
	gte_ldv0(&v);
	gte_rtps();
	gte_stsxy(&v);

	cx = -DRAWING_OFFSET_X + 0xa0;
	dx = v.vx - cx;
	if (maxDX >= ((dx > 0) ? dx : -dx)) {
		v.vx = -DRAWING_OFFSET_X + 0xa0;
	} else {
		cx = -DRAWING_OFFSET_X + 0xa0;
		maxDX = (cx < v.vx) ? -maxDX : maxDX;
		v.vx += maxDX;
	}

	cy = -DRAWING_OFFSET_Y + 0x78;
	dy = v.vy - cy;
	if (maxDY >= ((dy > 0) ? dy : -dy)) {
		v.vy = -DRAWING_OFFSET_Y + 0x78;
	} else {
		maxDY = ((-DRAWING_OFFSET_Y + 0x78) < v.vy) ? -maxDY : maxDY;
		v.vy += maxDY;
	}

	v.vx = (v.vx < (-DRAW_OFFSET_LIMIT_X_MAX + 0xa0)) ? -DRAW_OFFSET_LIMIT_X_MAX + 0xa0 : (((-DRAW_OFFSET_LIMIT_X_MIN + 0xa0) < v.vx) ? -DRAW_OFFSET_LIMIT_X_MIN + 0xa0 : v.vx);
	v.vy = (v.vy < (-DRAW_OFFSET_LIMIT_Y_MAX + 0x78)) ? -DRAW_OFFSET_LIMIT_Y_MAX + 0x78 : (((-DRAW_OFFSET_LIMIT_Y_MIN + 0x78) < v.vy) ? -DRAW_OFFSET_LIMIT_Y_MIN + 0x78 : v.vy);

	if (((v.vx - (-DRAWING_OFFSET_X + 0xa0)) == 0) && ((v.vy - (-DRAWING_OFFSET_Y + 0x78)) == 0)) {
		return 0;
	}

	ox = v.vx;

	if (radius > 0) {
		if (getDistance(ox - (-DRAWING_OFFSET_X + 0xa0),
		                v.vy - (-DRAWING_OFFSET_Y + 0x78), 0) >= radius) {
			ang = ratan2(v.vy - (-DRAWING_OFFSET_Y + 0x78),
			             v.vx - (-DRAWING_OFFSET_X + 0xa0));
			v.vx = (-DRAWING_OFFSET_X + 0xa0) + ((radius * rcos(ang)) >> 12);
			v.vy = (-DRAWING_OFFSET_Y + 0x78) + ((radius * rsin(ang)) >> 12);
		}
	}

	FISH_scrollCameraTo(v.vx, v.vy);

	return 1;
}

int32_t FISH_panCameraToTamer(int32_t radius)
{
	return FISH_panCameraTo(radius, &TAMER_ENTITY.entity.posData->location, 0, 0);
}

int32_t FISH_panCameraWithDPad(void)
{
	int32_t cx;
	int32_t ylo;
	int32_t cy;
	int16_t x;
	int16_t y;
	int32_t d;

	x = -DRAWING_OFFSET_X + 0xa0;
	cx = -DRAWING_OFFSET_X + 0xa0;
	y = -DRAWING_OFFSET_Y + 0x78;
	cy = -DRAWING_OFFSET_Y + 0x78;

	if ((POLLED_INPUT & 0x1000) != 0) {
		y -= 5;
	}

	if ((POLLED_INPUT & 0x4000) != 0) {
		y += 5;
	}

	if ((POLLED_INPUT & 0x8000) != 0) {
		x -= 5;
	}

	if ((POLLED_INPUT & 0x2000) != 0) {
		x += 5;
	}

	x = (x < (-DRAW_OFFSET_LIMIT_X_MAX + 0xa0)) ? -DRAW_OFFSET_LIMIT_X_MAX + 0xa0 : (((-DRAW_OFFSET_LIMIT_X_MIN + 0xa0) < x) ? -DRAW_OFFSET_LIMIT_X_MIN + 0xa0 : x);
	ylo = -DRAW_OFFSET_LIMIT_Y_MAX;
	y = (y < (ylo + 0x78)) ? ylo + 0x78 : (((-DRAW_OFFSET_LIMIT_Y_MIN + 0x78) < y) ? -DRAW_OFFSET_LIMIT_Y_MIN + 0x78 : y);

	cx = x - cx;
	d = cx;
	d = (d > 0) ? d : -d;
	if (d > 0) {
		goto move;
	}

	d = y - cy;
	d = (d > 0) ? d : -d;
	if (d > 0) {
		goto move;
	}

	return 0;
move:
	FISH_scrollCameraTo(x, y);

	return 1;
}

int32_t FISH_isScreenPointVisible(int32_t x, int32_t y)
{
	int32_t ret;
	int32_t okY;
	int32_t okX;
	int32_t origin;

	origin = 0;
	ret = 0;
	okY = 0;
	okX = 0;

	if (!(x < (-DRAW_OFFSET_LIMIT_X_MAX + origin))) {
		if (!((-DRAW_OFFSET_LIMIT_X_MIN + 0x140) < x)) {
			okX = 1;
		}
	}

	if (okX != 0) {
		if (!(y < (-DRAW_OFFSET_LIMIT_Y_MAX + origin))) {
			okY = 1;
		}
	}

	if (okY != 0) {
		if (!((-DRAW_OFFSET_LIMIT_Y_MIN + 0xf0) < y)) {
			ret = 1;
		}
	}

	return ret;
}

int32_t FISH_projectPoint(VECTOR *point, int32_t *outX, int32_t *outY)
{
	MATRIX *in = &FISH_SCRATCH->m[1];
	MATRIX *out = &FISH_SCRATCH->m[0];

	in->t[0] = point->vx;
	in->t[1] = point->vy;
	in->t[2] = point->vz;

	GsMulCoord0(&GsWSMATRIX, in, out);

	if ((out->t[2] <= 0) || (out->t[2] >= 0x10000)) {
		return 0;
	}

	if (outX != NULL) {
		*outX = out->t[0] * VIEWPORT_DISTANCE / out->t[2];
	}

	if (outY != NULL) {
		*outY = out->t[1] * VIEWPORT_DISTANCE / out->t[2];
	}

	return 1;
}

int32_t FISH_isWithinCastRange(VECTOR *pos, int32_t extra)
{
	int32_t dist;

	dist = getDistance(pos->vx - TAMER_ENTITY.entity.posData->location.vx, 0,
	                   pos->vz - TAMER_ENTITY.entity.posData->location.vz);

	return !((FISHING_DATA_PTR->mapData->castRange + extra) < dist);
}

int32_t FISH_isBaitMenuVisible(void)
{
	FishingBaitMenu *menu = &FISHING_DATA_PTR->baitMenu;
	int32_t isOpen;
	int32_t ret;

	if (menu->boxId <= 0) {
		return 0;
	}

	ret = 1;
	isOpen = ret;
	if ((UI_BOX_DATA[menu->boxId].state != 2) && (UI_BOX_DATA[menu->boxId].state != 3)) {
		isOpen = 0;
	}

	if (isOpen == 0) {
		if (menu->shown == 0) {
			ret = 0;
		}
	}

	return ret;
}

void FISH_closeBaitMenu(void)
{
	FishingBaitMenu *menu = &FISHING_DATA_PTR->baitMenu;

	if (menu->shown != 0) {
		menu->shown = 0;
		removeAnimatedUIBox(menu->boxId, &menu->startPos);
	}
}

void FISH_tickBaitMenu(void)
{
	FishingBaitMenu *menu = &FISHING_DATA_PTR->baitMenu;
	int32_t v;
	int32_t n;
	int32_t w;

	if (POLLED_INPUT == POLLED_INPUT_PREVIOUS) {
		MAIN_D_801351D8++;
	} else {
		MAIN_D_801351D8 = 0;
	}

	if (menu->shown == 0) {
		return;
	}

	menu->lastRow = -1;

	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x1000) != 0) ||
	    ((MAIN_D_801351D8 >= 0xb) && ((POLLED_INPUT & 0x1000) != 0))) {
		playSound(0, 2);
		v = menu->cursorRow;
		v--;
		menu->cursorRow = v;
		if (v < 0) {
			menu->cursorRow = 0;
		}
		if (menu->cursorRow < menu->rowOffset) {
			menu->rowOffset = menu->cursorRow;
			menu->lastRow = menu->rowOffset;
		}
	}

	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x4000) != 0) ||
	    ((MAIN_D_801351D8 >= 0xb) && ((POLLED_INPUT & 0x4000) != 0))) {
		playSound(0, 2);
		w = menu->cursorRow + 1;
		menu->cursorRow = w;
		n = menu->totalRows;
		if (w >= n) {
			menu->cursorRow = n - 1;
		}
		if (menu->cursorRow >= (menu->rowOffset + menu->visibleRows)) {
			menu->rowOffset = menu->cursorRow - menu->visibleRows + 1;
			menu->lastRow = menu->rowOffset + menu->visibleRows - 1;
		}
	}

	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x8000) != 0) ||
	    ((MAIN_D_801351D8 >= 0xb) && ((POLLED_INPUT & 0x8000) != 0))) {
		playSound(0, 2);
		v = menu->column;
		v--;
		menu->column = v;
		if (v < 0) {
			menu->column = 1;
		}
	}

	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x2000) != 0) ||
	    ((MAIN_D_801351D8 >= 0xb) && ((POLLED_INPUT & 0x2000) != 0))) {
		playSound(0, 2);
		v = menu->column;
		v++;
		menu->column = v;
		if (v >= 2) {
			menu->column = 0;
		}
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x40) != 0) {
		playSound(0, 2);
		if (menu->items[menu->column + (menu->cursorRow * 2)] != -1) {
			menu->selectedItem = menu->items[menu->column + (menu->cursorRow * 2)];
			FISH_closeBaitMenu();
		}
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x10) != 0) {
		playSound(0, 2);
		menu->selectedItem = -1;
		FISH_closeBaitMenu();
	}
}

void FISH_drawBaitMenuRow(int32_t row)
{
	FishingBaitMenu *menu = &FISHING_DATA_PTR->baitMenu;
	RECT rect;
	RECT area;
	int32_t i;
	int32_t item;

	for (i = 0; i < 2; i++) {
		item = menu->items[(row * 2) + i];
		if (item != -1) {
			item = INVENTORY.types.array[item];
			if (item != 0xff) {
				setRECT(&rect, i * 0x60, (row % menu->visibleRows) * 12, 0x60, 0xc);
				area = rect;
				clearTextSubArea(&area);
				drawString(ITEM_PARA[item].name, rect.x, rect.y);
				DrawSync(0);
			}
		}
	}
}

void FISH_renderBaitMenu(int32_t boxId)
{
	FishingBaitMenu *menu = &FISHING_DATA_PTR->baitMenu;
	UIBoxData *b;
	POLY_FT4 *prim;
	int32_t layer;
	int32_t x;
	int32_t y;
	int32_t row;
	int32_t col;
	int32_t item;
	int32_t q;
	int32_t r;

	layer = 6 - boxId;
	b = &UI_BOX_DATA[boxId];
	if (menu->shown == 0) {
		return;
	}

	y = b->finalPos.y;
	x = b->finalPos.x;

	for (row = 0; row < menu->visibleRows; row++) {
		for (col = 0; col < 2; col++) {
			r = row + menu->rowOffset;
			item = menu->items[col + (r * 2)];
			if (item == -1) {
				continue;
			}
			q = INVENTORY.types.array[item];
			if (q == 0xff) {
				continue;
			}
			FISH_drawItemSprite(q,
			                    x + ((col != 0) ? 0x98 : 8),
			                    y + 0xe + row * 0x12, layer, 0);
			renderString(0, x + ((col != 0) ? 0xab : 0x1b),
			             y + 0x10 + row * 0x12, 0x60, 0xc, col * 0x60,
			             (r % menu->visibleRows) * 12, layer, 1);

			prim = (POLY_FT4 *)GsGetWorkBase();
			prim->tpage = getTPage(0, 0, 896, 256);
			setClut(prim, 256, 484);
			*(int32_t *)&prim->r0 = 0x808080;
			setUVWH(prim, 0x80, 0x9c, 8, 8);
			setXY4(prim, x + ((col != 0) ? 0x10d : 0x7d), y + 0x14 + (row * 0x12), x + ((col != 0) ? 0x10d : 0x7d) + 8, y + 0x14 + (row * 0x12), x + ((col != 0) ? 0x10d : 0x7d), y + 0x1c + (row * 0x12), x + ((col != 0) ? 0x10d : 0x7d) + 8, y + 0x1c + (row * 0x12));
			setPolyFT4(prim);
			addPrim(ACTIVE_ORDERING_TABLE->org + layer, prim);
			GsSetWorkBase((PACKET *)(prim + 1));

			renderSmallNumber(0, 2, x + ((col != 0) ? 0x116 : 0x86),
			                   y + 0x12 + row * 0x12,
			                   (INVENTORY.types.array + item)[0x1e], layer);
		}
	}

	renderSelectionCursor(x + 6 + menu->column * 144, y + 0xd + (menu->cursorRow - menu->rowOffset) * 18,
	                      0x8e, 0x12, layer);

	if (menu->totalRows > menu->visibleRows) {
		UI_BOX_DATA[menu->boxId].rowOffset = menu->rowOffset;
		if (menu->lastRow != -1) {
			FISH_drawBaitMenuRow(menu->lastRow);
			menu->lastRow = -1;
		}
	}
}

int32_t FISH_openBaitMenu(void)
{
	FishingBaitMenu *menu = &FISHING_DATA_PTR->baitMenu;
	DVECTOR pos;
	int32_t n;
	int32_t i;
	int32_t flag;
	int32_t features;
	int32_t k;

	menu->lastRow = -1;
	menu->column = 0;
	menu->cursorRow = 0;
	menu->rowOffset = 0;
	menu->visibleRows = 9;

	n = 0;
	for (i = 0; i < 30U; i++) {
		if (INVENTORY.types.array[i] == 0xff) {
			continue;
		}
		if (ITEM_PARA[INVENTORY.types.array[i]].sortingValue != 2) {
			continue;
		}
		menu->items[n++] = i;
		if (n >= 30U) {
			break;
		}
	}

	menu->totalRows = (n + 1) / 2;
	for (; n < 30U; n++) {
		menu->items[n] = -1;
	}

	if (menu->totalRows <= 0) {
		return 0;
	}

	DrawSync(0);
	clearTextArea();
	for (k = 0; k < menu->visibleRows; k++) {
		FISH_drawBaitMenuRow(k);
	}

	getEntityScreenPos(ENTITY_TABLE[0], 1, (int16_t *)&pos);
	setRECT(&menu->startPos, pos.vx - 5, pos.vy - 5, 10, 10);
	setRECT(&menu->finalPos, -0x98, -0x68, 0x132, 0xb6);
	menu->boxId = 3;

	flag = 0;
	if (menu->totalRows > menu->visibleRows) {
		UI_BOX_DATA[menu->boxId].rowOffset = menu->rowOffset;
		UI_BOX_DATA[menu->boxId].visibleRows = menu->visibleRows;
		UI_BOX_DATA[menu->boxId].totalRows = menu->totalRows;
		flag = 1;
	}

	features = flag ? 6 : 2;
	createAnimatedUIBox(menu->boxId, 0, features, &menu->finalPos, &menu->startPos,
	                    (TickFunction)FISH_tickBaitMenu, (RenderFunction)FISH_renderBaitMenu);
	menu->shown = 1;
}

void FISH_initBaitMenu(void)
{
	FishingBaitMenu *menu = &FISHING_DATA_PTR->baitMenu;

	menu->lastRow = -1;
	menu->shown = 0;
	menu->boxId = -1;
	menu->visibleRows = 9;
}

void FISH_closeBaitMenuIfOpen(void)
{
	if (FISHING_DATA_PTR->baitMenu.shown != 0) {
		FISH_closeBaitMenu();
	}
}

void FISH_scrollViewToTarget(int32_t unused)
{
	FishingView *view = &FISHING_DATA_PTR->view;

	if (view->isScrolling != 0) {
		FISH_panCameraTo(0x10, &view->scrollTo, 0, 0);
		view->isScrolling = 0;
	}
}

int32_t FISH_isTextBoxVisible(void)
{
	FishingBox *box = &FISHING_DATA_PTR->view.box;
	int32_t isOpen;
	int32_t ret;

	if (box->boxId <= 0) {
		return 0;
	}

	ret = 1;
	isOpen = ret;
	if ((UI_BOX_DATA[box->boxId].state != 2) && (UI_BOX_DATA[box->boxId].state != 3)) {
		isOpen = 0;
	}

	if (isOpen == 0) {
		if (box->shown == 0) {
			ret = 0;
		}
	}

	return ret;
}

void FISH_closeTextBox(void)
{
	FishingBox *box = &FISHING_DATA_PTR->view.box;

	if (box->shown != 0) {
		removeStaticUIBox(box->boxId);
	}

	box->shown = 0;
}

void FISH_tickTextBox(void)
{
	FishingView *view = &FISHING_DATA_PTR->view;

	if (view->box.shown == 0) {
		return;
	}

	view->isScrolling = 1;

	if (((view->framesOpen >= 10) && ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x40) != 0)) ||
	    ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x10) != 0)) {
		POLLED_INPUT = 0;
		FISH_closeTextBox();
		playSound(0, 3);
	}

	if (view->framesOpen < 0x64) {
		view->framesOpen++;
	}

	TEXTBOX_OPEN_TIMER = (uint32_t)TEXTBOX_OPEN_TIMER + 1;
}

void FISH_renderTextBox(int32_t boxId)
{
	FishingView *v = &FISHING_DATA_PTR->view;
	UIBoxData *b;
	int32_t i;
	int32_t x;
	int32_t y;
	int32_t layer;

	layer = 6 - boxId;
	b = &UI_BOX_DATA[boxId];
	if (v->box.shown == 0) {
		return;
	}

	x = b->finalPos.x;
	y = b->finalPos.y;
	for (i = 0; i < 4U; i++) {
		if (v->lines[i] != NULL) {
			renderString(0, x + 5, y + (i * 13) + 3, 0xff, 0xc, 0, i * 12, layer, 1);
		}
	}

	renderUIBox(boxId);
}

int32_t FISH_showTextBox(char *line0, char *line1, char *line2, char *line3, VECTOR *target)
{
	FishingView *view = &FISHING_DATA_PTR->view;
	int16_t pos[2];
	int32_t i;

	FISH_closeTextBox();
	POLLED_INPUT = 0;

	view->scrollTo = *target;
	view->lines[0] = line0;
	view->lines[1] = line1;
	view->lines[2] = line2;
	view->lines[3] = line3;
	view->framesOpen = 0;

	DrawSync(0);
	clearTextArea();

	for (i = 0; i < 4U; i++) {
		if (view->lines[i] != NULL) {
			DrawSync(0);
			drawString(view->lines[i], 0, i * 0xc);
			DrawSync(0);
		}
	}

	getEntityScreenPos(ENTITY_TABLE[0], 1, pos);

	setRECT(&view->box.startPos, -DRAWING_OFFSET_X + 0xa0, -DRAWING_OFFSET_Y + 0x78, 1, 1);
	setRECT(&view->box.finalPos, -0x82, 0x2a, 0x106, 0x3b);

	view->box.boxId = 2;

	createStaticUIBox(view->box.boxId, 0, 2, &view->box.finalPos,
	                  (TickFunction)FISH_tickTextBox, (RenderFunction)FISH_renderTextBox);

	view->box.shown = 1;
	TEXTBOX_OPEN_TIMER = 0;
}

void FISH_initTextBox(void)
{
	FishingBox *box = &FISHING_DATA_PTR->view.box;

	box->boxId = -1;
	box->shown = 0;
}

void FISH_shutdownTextBox(void)
{
	FishingBox *box = &FISHING_DATA_PTR->view.box;

	FISH_closeTextBox();
	box->boxId = -1;
}

void FISH_randomPointInSpot(int32_t index, VECTOR *out)
{
	int32_t t;
	DVECTOR *pts[3];
	FishingSpot *spot;
	int32_t j;
	int32_t dup;
	int16_t x;
	int16_t y;
	int32_t lo;

	spot = &FISHING_DATA_PTR->mapData->fishingSpots[index];
	if ((index < 0) || (index >= 6)) {
		out->vx = 0;
		out->vy = 0;
		out->vz = 0;
		return;
	}

retry:
	for (t = 0; t < 3U; t++) {
		do {
			dup = 0;
			pts[t] = &spot->points[customRandom(0, spot->pointCount - 1)];
			for (j = 0; j < t; j++) {
				if (pts[t] == pts[j]) {
					dup = 1;
				}
			}
		} while (dup);
	}

	lo = 0;
	for (t = 0; t < 3U; t++) {
		pts[t]->vx = (pts[t]->vx < (-DRAW_OFFSET_LIMIT_X_MAX + lo)) ? -DRAW_OFFSET_LIMIT_X_MAX + lo : (((-DRAW_OFFSET_LIMIT_X_MIN + 0x140) < pts[t]->vx) ? -DRAW_OFFSET_LIMIT_X_MIN + 0x140 : pts[t]->vx);
		pts[t]->vy = (pts[t]->vy < (-DRAW_OFFSET_LIMIT_Y_MAX + lo)) ? -DRAW_OFFSET_LIMIT_Y_MAX + lo : (((-DRAW_OFFSET_LIMIT_Y_MIN + 0xf0) < pts[t]->vy) ? -DRAW_OFFSET_LIMIT_Y_MIN + 0xf0 : pts[t]->vy);
	}

	t = customRandom(0, 0x1000);
	x = ((pts[0]->vx * t) + (pts[1]->vx * (0x1000 - t))) >> 12;
	y = ((pts[0]->vy * t) + (pts[1]->vy * (0x1000 - t))) >> 12;
	t = customRandom(0, 0x1000);
	x = ((pts[2]->vx * t) + (x * (0x1000 - t))) >> 12;
	y = ((pts[2]->vy * t) + (y * (0x1000 - t))) >> 12;

	if (FISH_isScreenPointVisible(x, y) == 0) {
		goto retry;
	}

	if (FISHING_DATA_PTR->mapData->isWater(x, y) == 0) {
		goto retry;
	}

	MAIN_func_800E4038(out, x, y, &t);
}

void FISH_drawStub(void)
{
}

void FISH_drawSprite2D(int32_t x, int32_t y, int32_t order, FishSprite *desc, int32_t sx, int32_t sy, int32_t rot, int32_t color, int32_t flags)
{
	POLY_FT4 *prim;
	int32_t t;
	int32_t extent;
	int32_t i;

	prim = (POLY_FT4 *)GsGetWorkBase();
	*(int32_t *)&prim->r0 = color;
	prim->u0 = prim->u2 = desc->u0;
	prim->u1 = prim->u3 = desc->u1;
	prim->v0 = prim->v1 = desc->v0;
	prim->v2 = prim->v3 = desc->v1;

	t = (-desc->ox * sx) >> 12;
	FISH_SCRATCH->pts[2].vx = t;
	FISH_SCRATCH->pts[0].vx = t;

	extent = ((desc->u1 - desc->u0) > 0) ? (desc->u1 - desc->u0) : -(desc->u1 - desc->u0);
	t = (sx * (-desc->ox + extent)) >> 12;
	FISH_SCRATCH->pts[3].vx = t;
	FISH_SCRATCH->pts[1].vx = t;

	t = (-desc->oy * sy) >> 12;
	FISH_SCRATCH->pts[1].vy = t;
	FISH_SCRATCH->pts[0].vy = t;

	extent = ((desc->v1 - desc->v0) > 0) ? (desc->v1 - desc->v0) : -(desc->v1 - desc->v0);
	t = (sy * (-desc->oy + extent)) >> 12;
	FISH_SCRATCH->pts[3].vy = t;
	FISH_SCRATCH->pts[2].vy = t;

	FISH_SCRATCH->rot.vx = 0;
	FISH_SCRATCH->rot.vy = 0;
	FISH_SCRATCH->rot.vz = rot;
	RotMatrix(&FISH_SCRATCH->rot, &FISH_SCRATCH->m[2]);

	for (i = 0; i < 4; i++) {
		ApplyMatrix(&FISH_SCRATCH->m[2], &FISH_SCRATCH->pts[i],
		            &FISH_SCRATCH->v0);
		copyVector(&FISH_SCRATCH->pts[i], &FISH_SCRATCH->v0);
		FISH_SCRATCH->pts[i].vx += x;
		FISH_SCRATCH->pts[i].vy += y;
	}

	*(int32_t *)&prim->x0 = *(int32_t *)&FISH_SCRATCH->pts[0];
	*(int32_t *)&prim->x1 = *(int32_t *)&FISH_SCRATCH->pts[1];
	*(int32_t *)&prim->x2 = *(int32_t *)&FISH_SCRATCH->pts[2];
	*(int32_t *)&prim->x3 = *(int32_t *)&FISH_SCRATCH->pts[3];
	prim->tpage = desc->tpage | ((flags & 3) << 5);
	prim->clut = desc->clut;
	setPolyFT4(prim);
	setSemiTrans(prim, flags >> 2);
	addPrim(ACTIVE_ORDERING_TABLE->org + order, prim);
	GsSetWorkBase((PACKET *)(prim + 1));
}

void FISH_drawSprite3D(VECTOR *pos, SVECTOR *rot, VECTOR *scale, FishSprite *desc, int32_t color, int32_t flags, int32_t depth)
{
	POLY_FT4 *prim;
	int32_t t;
	int32_t extent;
	int32_t i;
	int32_t j;

	RotMatrixYXZ(rot, &FISH_SCRATCH->m[1]);
	FISH_SCRATCH->m[1].t[0] = pos->vx;
	FISH_SCRATCH->m[1].t[1] = pos->vy;
	FISH_SCRATCH->m[1].t[2] = pos->vz;
	GsMulCoord0(&GsWSMATRIX, &FISH_SCRATCH->m[1], &FISH_SCRATCH->m[0]);
	GsSetLsMatrix(&FISH_SCRATCH->m[0]);
	prim = (POLY_FT4 *)GsGetWorkBase();

	t = (-desc->ox * scale->vx) >> 12;
	FISH_SCRATCH->pts[2].vx = t;
	FISH_SCRATCH->pts[0].vx = t;

	extent = desc->u1 - desc->u0;
	extent = (extent > 0) ? extent : -extent;
	t = (scale->vx * (-desc->ox + extent)) >> 12;
	FISH_SCRATCH->pts[3].vx = t;
	FISH_SCRATCH->pts[1].vx = t;

	t = (-desc->oy * scale->vy) >> 12;
	FISH_SCRATCH->pts[1].vy = t;
	FISH_SCRATCH->pts[0].vy = t;

	extent = desc->v1 - desc->v0;
	extent = (extent > 0) ? extent : -extent;
	t = (scale->vy * (-desc->oy + extent)) >> 12;
	FISH_SCRATCH->pts[3].vy = t;
	FISH_SCRATCH->pts[2].vy = t;

	for (i = 0; i < 4; i++) {
		FISH_SCRATCH->pts[i].vz = 0;
	}

	FISH_SCRATCH->otz = RotTransPers4(FISH_SCRATCH->pts, &FISH_SCRATCH->pts[1],
	                                  &FISH_SCRATCH->pts[2], &FISH_SCRATCH->pts[3],
	                                  (long *)&prim->x0, (long *)&prim->x1, (long *)&prim->x2, (long *)&prim->x3,
	                                  (long *)&FISH_SCRATCH->sz, (long *)&FISH_SCRATCH->flag);

	if ((FISH_SCRATCH->flag & 0x80000000) != 0) {
		return;
	}

	*(int32_t *)&prim->r0 = color;
	prim->u0 = prim->u2 = desc->u0;
	prim->u1 = prim->u3 = desc->u1;
	prim->v0 = prim->v1 = desc->v0;
	prim->v2 = prim->v3 = desc->v1;
	prim->tpage = desc->tpage | ((flags & 3) << 5);
	prim->clut = desc->clut;
	setPolyFT4(prim);
	setSemiTrans(prim, flags >> 2);
	addPrim(ACTIVE_ORDERING_TABLE->org + (FISH_SCRATCH->otz >> 2) + depth, prim);
	GsSetWorkBase((PACKET *)(prim + 1));
}

void FISH_freeMarker(FishMarker *s)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->markerPool;

	if (s->active != 0) {
		s->active = 0;
		pool->count--;
	}
}

void FISH_tickMarker(FishMarker *s)
{
	if (s->active != 0) {
		if ((s->lifetime = s->lifetime - 1) == 0) {
			FISH_freeMarker(s);
		}
	}
}

void FISH_drawMarker(FishMarker *s)
{
	FishSprite *p;

	if (s->active == 0) {
		return;
	}

	if (FISH_projectPoint(&s->pos, &FISH_SCRATCH->x, &FISH_SCRATCH->y) == 0) {
		return;
	}

	p = FISH_SPRITES + s->sprite;

	FISH_drawSprite2D(FISH_SCRATCH->x, FISH_SCRATCH->y, 0x42, p,
	                  0x1000, 0x1000, 0, 0x808080, 0);
}

void FISH_initMarkerPool(void)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->markerPool;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		pool->items[i].active = 0;
	}

	pool->count = 0;
	pool->next = pool->items;
}

FishMarker *FISH_allocMarker(void)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->markerPool;
	FishMarker *end;
	FishMarker *start;
	FishMarker *p;

	if (pool->count >= 16U) {
		return NULL;
	}

	end = &pool->items[16];
	p = pool->next;
	start = p;

	do {
		if (p->active == 0) {
			pool->next = p;
			return p;
		}

		p++;
		if (p >= end) {
			p = pool->items;
		}
	} while (p != start);

	pool->count = 16;

	return NULL;
}

void FISH_spawnMarker(int32_t sprite, VECTOR *pos)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->markerPool;
	FishMarker *s;

	s = FISH_allocMarker();
	if (s != NULL) {
		pool->count++;
		s->active = 1;
		s->pos = *pos;
		s->sprite = sprite;
		s->lifetime = 10;
	}
}

void FISH_tickMarkers(void)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->markerPool;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		if (pool->items[i].active != 0) {
			FISH_tickMarker(&pool->items[i]);
		}
	}
}

void FISH_drawMarkers(void)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->markerPool;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		if (pool->items[i].active != 0) {
			FISH_drawMarker(&pool->items[i]);
		}
	}
}

void FISH_clearMarkers(void)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->markerPool;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		pool->items[i].active = 0;
	}

	pool->count = 0;
}

void FISH_freeRipple(FishRipple *s)
{
	FishRipplePool *pool = &FISHING_DATA_PTR->ripplePool;

	if (s->active != 0) {
		s->active = 0;
		pool->count--;
	}
}

void FISH_tickRipple(FishRipple *s)
{
	if (s->active != 0) {
		switch (s->kind) {
		case 0:
			s->sprite = MAIN_D_80134958[s->frame / 2];
			break;
		case -1:
			s->sprite = MAIN_D_80134960[s->frame];
			break;
		case -2:
			s->sprite = MAIN_D_80134968[s->frame / 3];
			break;
		case -3:
			s->sprite = MAIN_D_8013496C[s->frame];
			break;
		case -4:
			s->sprite = FISH_D_8007A4B4[s->frame];
			break;
		default:
			FISH_freeRipple(s);
			return;
		}
		s->frame++;
		if (s->sprite == -1) {
			FISH_freeRipple(s);
			return;
		}
		if (s->sprite >= 13) {
			FISH_freeRipple(s);
		}
	}
}

void FISH_drawRipple(FishRipple *s)
{
	VECTOR scale;
	SVECTOR rot;
	FishSprite *desc;

	if (s->active == 0) {
		return;
	}

	if (s->sprite <= 0) {
		return;
	}

	if (s->kind == 0) {
		desc = FISH_SPRITES + (s->sprite - 1);
	} else {
		desc = FISH_SPRITES + (s->sprite + 4);
	}

	rot.vx = 0;
	rot.vy = 0;
	rot.vz = 0;
	scale.vx = (FISHING_DATA_PTR->mapData->scale * s->scale) >> 12;
	scale.vy = (FISHING_DATA_PTR->mapData->scale * s->scale) >> 12;
	scale.vz = (FISHING_DATA_PTR->mapData->scale * s->scale) >> 12;
	FISH_drawSprite3D(&s->pos, &rot, &scale, desc, 0x808080, 5, -10);
}

void FISH_initRipplePool(void)
{
	FishRipplePool *pool = &FISHING_DATA_PTR->ripplePool;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		pool->items[i].active = 0;
	}

	pool->count = 0;
	pool->next = pool->items;
	pool->attractor = NULL;
}

FishRipple *FISH_allocRipple(void)
{
	FishRipplePool *pool = &FISHING_DATA_PTR->ripplePool;
	FishRipple *end;
	FishRipple *start;
	FishRipple *p;

	if (pool->count >= 16U) {
		return NULL;
	}

	end = &pool->items[16];
	p = pool->next;
	start = p;

	do {
		if (p->active == 0) {
			pool->next = p;
			return p;
		}

		p++;
		if (p >= end) {
			p = pool->items;
		}
	} while (p != start);

	pool->count = 16;

	return NULL;
}

void FISH_spawnRipple(int32_t kind, VECTOR *pos, int32_t attractRadius, int32_t spread, int32_t scale)
{
	FishRipplePool *pool = &FISHING_DATA_PTR->ripplePool;
	FishRipple *s;

	s = FISH_allocRipple();
	if (s == NULL) {
		return;
	}

	pool->count++;
	s->active = 1;
	s->pos = *pos;
	s->kind = kind;
	s->frame = 0;
	s->sprite = 0;
	s->pos.vx += customRandom(-spread, spread);
	s->pos.vz += customRandom(-spread, spread);
	s->pos.vy = 0;
	s->scale = scale;

	if (attractRadius != 0) {
		pool->attractor = s;
		pool->attractRadius = attractRadius;
	}
}

void FISH_tickRipples(void)
{
	FishRipplePool *pool = &FISHING_DATA_PTR->ripplePool;
	FishRipple *p;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		if (pool->items[i].active != 0) {
			FISH_tickRipple(&pool->items[i]);
		}
	}

	if (pool->attractor != NULL) {
		p = pool->attractor;
		if (p->active == 0) {
			pool->attractor = NULL;
		}
	}
}

void FISH_drawRipples(void)
{
	FishRipplePool *pool = &FISHING_DATA_PTR->ripplePool;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		if (pool->items[i].active != 0) {
			FISH_drawRipple(&pool->items[i]);
		}
	}
}

void FISH_loadSpritesAndSeadramon(TMDFileLoadingData *model)
{
	uint32_t tpage;
	uint32_t clut;
	FishingData *d;

	loadTextureFile(FISHING_DATA_PTR->mapData->spriteFilePath, &tpage, &clut);

	d = FISHING_DATA_PTR;
	FISH_loadTMDModel(&d->seadramonModel, FISH_PATH_SEADRAMON_MODEL, d->seadramonModelBuffer, 0x1000);
}

void FISH_onRodModelLoaded(TMDFileLoadingData *model)
{
}

void FISH_resetTension(void)
{
	FishingTension *t = &FISHING_DATA_PTR->tension;

	t->display = 0;
	t->level = 0;
	t->framesAtLimit = 0;
	t->color.word = 0xffffff;
}

void FISH_beginTension(void)
{
	FishingTension *t = &FISHING_DATA_PTR->tension;

	FISH_resetTension();
	t->display = 1;
	t->limit = (FISHING_DATA_PTR->rod.swimmerType == 1) ? 0x1388 : 0x2710;
}

inline int32_t FISH_tensionOverLimit(FishingTension *t, int32_t level)
{
	return t->limit < level;
}

int32_t FISH_updateTension(void)
{
	FishingTension *t = &FISHING_DATA_PTR->tension;
	int32_t n;
	int32_t snapSteps;

	n = 0;
	n += (FISHING_DATA_PTR->rod.pressedFrames > 0) ? 2 : 0;
	n += (FISHING_DATA_PTR->hooked.fish.state == 0x32) ? 1 : 0;

	switch (n) {
	case 0:
		t->level = t->level +
		           FISH_remapClamped(FISHING_DATA_PTR->hooked.fish.size, 0x32, 0x12c, -0xa6, -0xa6);
		break;
	case 1:
		t->level = t->level +
		           FISH_remapClamped(FISHING_DATA_PTR->hooked.fish.size, 0x32, 0x12c, 0x29, 0x53);
		break;
	case 2:
		t->level = t->level +
		           FISH_remapClamped(FISHING_DATA_PTR->hooked.fish.size, 0x32, 0x12c, 0x47, 0x7d);
		break;
	case 3:
		t->level = t->level +
		           FISH_remapClamped(FISHING_DATA_PTR->hooked.fish.size, 0x32, 0x12c, 0xa6, 0x1f4);
		break;
	}

	t->level = (t->level < -t->limit) ? -t->limit : (FISH_tensionOverLimit(t, t->level) ? t->limit : t->level);

	if (t->level >= (t->limit / 2)) {
		t->color.rgb[0] = FISH_remapClamped(t->level, t->limit / 2, t->limit, 0xff, 0xff);
		t->color.rgb[1] = FISH_remapClamped(t->level, t->limit / 2, t->limit, 0xff, 0);
		t->color.rgb[2] = FISH_remapClamped(t->level, t->limit / 2, t->limit, 0, 0);
	} else if (t->level >= 0) {
		t->color.rgb[0] = FISH_remapClamped(t->level, 0, t->limit / 2, 0xff, 0xff);
		t->color.rgb[1] = FISH_remapClamped(t->level, 0, t->limit / 2, 0xff, 0xff);
		t->color.rgb[2] = FISH_remapClamped(t->level, 0, t->limit / 2, 0xff, 0);
	} else if (t->level >= (-t->limit / 2)) {
		t->color.rgb[0] = FISH_remapClamped(t->level, -t->limit / 2, 0, 0, 0xff);
		t->color.rgb[1] = FISH_remapClamped(t->level, -t->limit / 2, 0, 0xff, 0xff);
		t->color.rgb[2] = FISH_remapClamped(t->level, -t->limit / 2, 0, 0xff, 0xff);
	} else {
		t->color.rgb[0] = FISH_remapClamped(t->level, -t->limit, -t->limit / 2, 0, 0);
		t->color.rgb[1] = FISH_remapClamped(t->level, -t->limit, -t->limit / 2, 0, 0xff);
		t->color.rgb[2] = FISH_remapClamped(t->level, -t->limit, -t->limit / 2, 0xff, 0xff);
	}

	if (t->level == -t->limit) {
		return 2;
	}

	if (t->level == t->limit) {
		snapSteps = 4 - FISH_remapClamped(FISHING_DATA_PTR->hooked.fish.size, 0x32, 0x12c, 1, 3);
		n = t->framesAtLimit + 1;
		t->framesAtLimit = n;
		if (n == (snapSteps * 5)) {
			return 1;
		}
	} else {
		t->framesAtLimit = 0;
	}

	return 0;
}

void FISH_drawTensionGauge(void)
{
	FishingTension *t = &FISHING_DATA_PTR->tension;
	int32_t *colorp;
	int32_t n;
	int32_t i;
	int32_t off;

	if (t->display == 0) {
		return;
	}
	FISH_drawSprite2D(MAIN_D_80134974[0], MAIN_D_80134974[1], 0xa,
	                  &FISH_SPRITES[31], 0x1000, 0x1000, 0, 0x808080, 0);
	n = FISH_remapClamped(t->level, -t->limit, t->limit, 0, 9);
	colorp = &t->color.word;
	for (i = 0, off = 0, t = (FishingTension *)FISH_D_8007A340; i < n; i++, off += 3) {
		FISH_drawSprite2D(MAIN_D_80134974[0] + off, MAIN_D_80134974[1], 9, (FishSprite *)t,
		                  0x1000, 0x1000, 0, *colorp, 0);
	}
}

void FISH_hideBaitSprite(void)
{
	FISHING_DATA_PTR->item.shown = 0;
}

void FISH_initBaitSprite(void)
{
	FISH_hideBaitSprite();
}

void FISH_clearBaitSprite(void)
{
	FISH_hideBaitSprite();
}

void FISH_showBaitSprite(VECTOR *pos, int32_t sprite)
{
	FishingItem *item = &FISHING_DATA_PTR->item;

	item->shown = 1;
	item->sprite = sprite;
	if (pos != NULL) {
		item->pos = *pos;
	}
}

int32_t FISH_moveBaitToRodTip(int32_t maxStep)
{
	FishingItem *item = &FISHING_DATA_PTR->item;
	VECTOR target;
	VECTOR delta;
	SVECTOR rot;
	MATRIX m;
	int32_t dist;

	FISH_getBoneTransform(&TAMER_ENTITY.entity, 6, &target, &rot);
	RotMatrixYXZ(&rot, &m);
	ApplyMatrix(&m, &MAIN_D_8013497C, &delta);

	addVector(&target, &delta);

	addVector(&target, &MAIN_D_80134984);

	dist = (!maxStep) ? 0 : getDistance(target.vx - item->pos.vx, target.vy - item->pos.vy, target.vz - item->pos.vz);

	if ((maxStep != 0) && (maxStep < dist)) {
		toEulerAngles(&rot, target.vx - item->pos.vx, target.vy - item->pos.vy,
		              target.vz - item->pos.vz);

		delta.vx = 0;
		delta.vy = 0;
		delta.vz = maxStep;

		rotateVectorYXZ(&rot, &delta, &target);

		addVector(&item->pos, &target);
		return 1;
	}

	item->pos = target;

	return 0;
}

void FISH_tickBaitSprite(void)
{
	FishingItem *item = &FISHING_DATA_PTR->item;
	VECTOR pos;
	VECTOR step;
	SVECTOR rot;
	VECTOR probe;
	VECTOR *ip;
	int32_t blocked;

	if (item->shown == 0) {
		return;
	}

	pos = item->pos;

	toEulerAngles(&rot,
	              FISHING_DATA_PTR->swimmer.pos.vx - FISHING_DATA_PTR->rod.lineStart.vx,
	              FISHING_DATA_PTR->swimmer.pos.vy - FISHING_DATA_PTR->rod.lineStart.vy,
	              FISHING_DATA_PTR->swimmer.pos.vz - FISHING_DATA_PTR->rod.lineStart.vz);

	step.vx = 0;
	step.vy = 0;
	step.vz = 0x64;

	rotateVectorYXZ(&rot, &step, ip = &item->pos);

	addVector(&item->pos, &FISHING_DATA_PTR->swimmer.pos);

	if (pos.vy >= 0) {
		return;
	}

	if (item->pos.vy < 0) {
		return;
	}

	if (FISHING_DATA_PTR->rod.fishingState != 8) {
		return;
	}

	blocked = 0;
	probe = item->pos;
	probe.vy = 0;

	if (FISH_projectPoint(&probe, &FISH_SCRATCH->x, &FISH_SCRATCH->y) == 0) {
		blocked = 1;
	} else if ((FISH_isScreenPointVisible(FISH_SCRATCH->x, FISH_SCRATCH->y) == 0) ||
	           (FISHING_DATA_PTR->mapData->isWater(FISH_SCRATCH->x,
	                                               FISH_SCRATCH->y) == 0)) {
		blocked = 1;
	}

	if (blocked != 0) {
		return;
	}

	FISH_spawnRipple(0, ip, 0x64, 0, FISHING_DATA_PTR->mapData->unk8 * 75);
}

void FISH_drawBaitSprite(void)
{
	FishingItem *item = &FISHING_DATA_PTR->item;
	POLY_FT4 *prim;
	MATRIX m1;
	MATRIX m2;
	int32_t r;

	if (item->shown == 0) {
		return;
	}

	if (item->pos.vy >= 0) {
		return;
	}

	copyVector(((VECTOR *)m1.t), &item->pos);
	GsMulCoord0(&GsWSMATRIX, &m1, &m2);

	if (m2.t[2] <= 0) {
		return;
	}

	if (m2.t[2] >= 0x10000) {
		return;
	}

	m2.t[0] = (m2.t[0] * VIEWPORT_DISTANCE) / m2.t[2];
	m2.t[1] = (m2.t[1] * VIEWPORT_DISTANCE) / m2.t[2];
	r = (FISHING_DATA_PTR->mapData->unk8 * VIEWPORT_DISTANCE) / m2.t[2];

	prim = (POLY_FT4 *)GsGetWorkBase();
	*(int32_t *)&prim->r0 = 0x808080;
	prim->tpage = getTPage(0, 0, 320, 0);
	setClut(prim, 224, MAIN_D_80127BDC[item->sprite] + 488);
	setUV0(prim, prim->u2 = (item->sprite % 16) * 16, prim->v1 = (item->sprite / 16) * 16);
	prim->u1 = prim->u3 = prim->u0 + 0xf;
	prim->v2 = prim->v3 = prim->v0 + 0xf;
	setXYWH(prim, m2.t[0] - r, m2.t[1] - r, (r * 2), (r * 2));
	setPolyFT4(prim);
	addPrim(ACTIVE_ORDERING_TABLE->org + (m2.t[2] >> 4), prim);
	GsSetWorkBase((PACKET *)(prim + 1));
}

void FISH_resetBobberAnim(FishRipple *s)
{
	s->kind = 3;
	s->sprite = 0;
	s->frame = 0;
	s->active = 0;
}

void FISH_initBobberAnim(FishRipple *s)
{
	FISH_resetBobberAnim(s);
}

void FISH_clearBobberAnim(FishRipple *s)
{
	FISH_resetBobberAnim(s);
}

void FISH_getRodTipPos(VECTOR *out)
{
	VECTOR v;
	SVECTOR rot;
	MATRIX m;

	FISH_getBoneTransform(&TAMER_ENTITY.entity, 6, &v, &rot);
	RotMatrixYXZ(&rot, &m);
	ApplyMatrix(&m, &MAIN_D_8013498C, out);

	addVector(out, &v);

	addVector(out, &MAIN_D_80134994);
}

void FISH_bobberCommand(FishingSwimmer *s, int32_t command)
{
	VECTOR scale;
	SVECTOR rot;
	FishSprite *desc;
	int32_t t;
	int32_t k;

	switch (command) {
	case 0:
		goto stateDispatch;
	case 1:
		goto drawSwimmer;
	case 2:
		goto snap2;
	case 4:
		goto start4;
	case 5:
		goto start5;
	case 6:
		goto start6;
	case 7:
		goto start7;
	case 11:
		goto snap11;
	case 12:
		goto snap12;
	case 3:
	case 8:
	case 9:
	case 10:
	case 13:
		goto end;
	case 14:
		goto start14;
	default:
		goto end;
	}

stateDispatch:
	switch (s->state) {
	case 3:
		goto clearSprite;
	case 7:
		goto state7;
	case 8:
		goto state8;
	case 9:
		goto state9;
	case 10:
		goto state10;
	case 11:
		goto state11;
	case 12:
		goto state12;
	case 4:
	case 5:
	case 6:
	case 13:
		goto tick;
	case 14:
		goto state14;
	default:
		goto tick;
	}

clearSprite:
	s->sprite = 0;
	goto tick;

start4:
	FISH_getRodTipPos(&s->pos);
	s->sprite = 1;
	s->state = 4;
	s->timer = -1;
	goto tick;

start5:
	s->sprite = 1;
	s->state = 5;
	s->timer = -1;
	goto tick;

start6:
	s->sprite = 1;
	s->state = 6;
	s->timer = -1;
	goto tick;

start7:
	FISH_spawnRipple(-1, &s->pos, 0x64, 0, 0x1000);
	s->state = 7;
	s->timer = -1;
	goto tick;

state7:
	t = (s->timer < 0) ? 0 : ((s->timer >= 8U) ? 7 : s->timer);
	s->sprite = MAIN_D_8013499C[t];
	if (s->sprite > -1) {
		goto tick;
	}

setSprite3:
	k = 3;
setSpriteAdvance:
	s->sprite = k;
	s->state = 8;
	s->timer = -1;
	goto tick;

state8:
	if (customRandom(1, 2) == 1) {
		s->pos.vx += customRandom(-1, 1);
		s->pos.vz += customRandom(-1, 1);
	}

	if (customRandom(1, 0x3c) != 1) {
		goto tick;
	}

	FISH_spawnRipple(-2, &s->pos, 0, 0, 0x1000);
	s->state = 9;
	s->timer = -1;
	goto tick;

state9:
	t = ((s->timer / 3) < 0) ? 0 : (((s->timer / 3) >= 3U) ? 2 : s->timer / 3);
	s->sprite = FISH_D_8007A4D8[t];
	if (s->sprite != -1) {
		goto tick;
	}

	k = 3;
	goto setSpriteAdvance;

snap2:
	playSound(8, 3);
	FISH_spawnRipple(-3, &s->pos, 0, 0, 0x1000);
	s->state = 0xa;
	s->timer = -1;
	goto tick;

state10:
	t = (s->timer < 0) ? 0 : ((s->timer >= 5U) ? 4 : s->timer);
	s->sprite = FISH_D_8007A4E4[t];
	if (s->sprite != -1) {
		goto tick;
	}

	k = 3;
	goto setSpriteAdvance;

snap12:
	playSound(8, 4);
	FISH_spawnRipple(-4, &s->pos, 0, 0, 0x1000);
	s->state = 0xb;
	s->timer = -1;
	goto tick;

state12:
	t = (s->timer < 0) ? 0 : ((s->timer >= 0x12U) ? 0x11 : s->timer);
	s->sprite = FISH_D_8007A4F8[t];
	if (s->sprite != -1) {
		goto tick;
	}

	k = 3;
	goto setSpriteAdvance;

snap11:
	playSound(8, 4);
	FISH_spawnRipple(-4, &s->pos, 0, 0, 0x1000);
	s->state = 0xb;
	s->timer = -1;
	goto tick;

state11:
	t = (s->timer < 0) ? 0 : ((s->timer >= 5U) ? 4 : s->timer);
	s->sprite = FISH_D_8007A540[t];
	if (s->sprite != -1) {
		goto tick;
	}

	s->state = 0xd;
	s->timer = -1;
	goto tick;

start14:
	s->state = 0xe;
	s->timer = -1;
	goto tick;

state14:
	t = (s->timer < 0) ? 0 : ((s->timer >= 6U) ? 5 : s->timer);
	s->sprite = FISH_D_8007A554[t];
	if (s->sprite == -1) {
		goto setSprite3;
	}

tick:
	s->timer++;
	goto end;

drawSwimmer:
	if (s->sprite <= 0) {
		goto end;
	}

	t = (FISHING_DATA_PTR->rod.swimmerType == 1) ? 0x11 : 0x18;
	desc = FISH_SPRITES + (s->sprite - 1 + t);
	rot.vx = 0;
	rot.vy = 0;
	rot.vz = 0;
	scale.vx = FISHING_DATA_PTR->mapData->scale;
	scale.vy = FISHING_DATA_PTR->mapData->scale;
	scale.vz = FISHING_DATA_PTR->mapData->scale;
	FISH_drawSprite3D(&s->pos, &rot, &scale, desc, 0x808080, 0, -10);

end:;
}

void FISH_tickBobberPhysics(void)
{
	FishingSwimmer *s;
	VECTOR old;
	int32_t d;

	FISH_bobberCommand(&FISHING_DATA_PTR->swimmer, 0);

	switch (FISHING_DATA_PTR->swimmer.mode) {
	case 0:
		goto end;
	case 1:
		goto rodTip;
	case 2:
	case 3:
	case 4:
		goto physics;
	}
	goto end;

rodTip:
	FISH_getRodTipPos(&FISHING_DATA_PTR->swimmer.pos);
	return;

physics:
	s = &FISHING_DATA_PTR->swimmer;
	old = s->pos;

	d = (s->vel.vx * 300) >> 12;
	s->vel.vx -= ((((s->vel.vx * 300) >> 12) != 0) ? d : ((s->vel.vx > 0) ? 1 : ((s->vel.vx < 0) ? -1 : 0)));
	d = (s->vel.vy * 300) >> 12;
	s->vel.vy -= ((((s->vel.vy * 300) >> 12) != 0) ? d : ((s->vel.vy > 0) ? 1 : ((s->vel.vy < 0) ? -1 : 0)));
	d = (s->vel.vz * 300) >> 12;
	s->vel.vz -= ((((s->vel.vz * 300) >> 12) != 0) ? d : ((s->vel.vz > 0) ? 1 : ((s->vel.vz < 0) ? -1 : 0)));

	s->vel.vy += s->gravity;

	if (s->dist != 0) {
		d = getDistance(s->pos.vx - FISHING_DATA_PTR->rod.lineStart.vx,
		                s->pos.vy - FISHING_DATA_PTR->rod.lineStart.vy,
		                s->pos.vz - FISHING_DATA_PTR->rod.lineStart.vz);
		if (s->dist < d) {
			toEulerAngles(&FISH_SCRATCH->rot,
			              s->pos.vx - FISHING_DATA_PTR->rod.lineStart.vx,
			              s->pos.vy - FISHING_DATA_PTR->rod.lineStart.vy,
			              s->pos.vz - FISHING_DATA_PTR->rod.lineStart.vz);
			FISH_SCRATCH->v0.vx = 0;
			FISH_SCRATCH->v0.vy = 0;
			FISH_SCRATCH->v0.vz = s->dist;
			rotateVectorYXZ(&FISH_SCRATCH->rot, &FISH_SCRATCH->v0, (VECTOR *)s);
			addVector(&s->pos, &FISHING_DATA_PTR->rod.lineStart);
			FISH_SCRATCH->v0.vx = s->pos.vx - old.vx;
			FISH_SCRATCH->v0.vy = s->pos.vy - old.vy;
			FISH_SCRATCH->v0.vz = s->pos.vz - old.vz;
			addVector(&s->vel, &FISH_SCRATCH->v0);
		}
	}

	FISHING_DATA_PTR->swimmer.pos.vx =
		FISHING_DATA_PTR->swimmer.pos.vx + FISHING_DATA_PTR->swimmer.vel.vx;
	FISHING_DATA_PTR->swimmer.pos.vy =
		FISHING_DATA_PTR->swimmer.pos.vy + FISHING_DATA_PTR->swimmer.vel.vy;
	FISHING_DATA_PTR->swimmer.pos.vz =
		FISHING_DATA_PTR->swimmer.pos.vz + FISHING_DATA_PTR->swimmer.vel.vz;

	if (s->dist != 0) {
		d = getDistance(s->pos.vx - FISHING_DATA_PTR->rod.lineStart.vx,
		                s->pos.vy - FISHING_DATA_PTR->rod.lineStart.vy,
		                s->pos.vz - FISHING_DATA_PTR->rod.lineStart.vz);
		if (s->dist < d) {
			toEulerAngles(&FISH_SCRATCH->rot,
			              s->pos.vx - FISHING_DATA_PTR->rod.lineStart.vx,
			              s->pos.vy - FISHING_DATA_PTR->rod.lineStart.vy,
			              s->pos.vz - FISHING_DATA_PTR->rod.lineStart.vz);
			FISH_SCRATCH->v0.vx = 0;
			FISH_SCRATCH->v0.vy = 0;
			FISH_SCRATCH->v0.vz = s->dist;
			rotateVectorYXZ(&FISH_SCRATCH->rot, &FISH_SCRATCH->v0, (VECTOR *)s);
			addVector(&s->pos, &FISHING_DATA_PTR->rod.lineStart);
		}
	}

	FISH_SCRATCH->v0.vx = FISHING_DATA_PTR->swimmer.pos.vx;
	FISH_SCRATCH->v0.vy = 0;
	FISH_SCRATCH->v0.vz = FISHING_DATA_PTR->swimmer.pos.vz;
	if ((FISH_projectPoint(&FISH_SCRATCH->v0, &FISH_SCRATCH->x, &FISH_SCRATCH->y) == 0) ||
	    (FISH_isScreenPointVisible(FISH_SCRATCH->x, FISH_SCRATCH->y) == 0)) {
		s->pos.vx = old.vx;
		s->pos.vz = old.vz;
		s->vel.vx = 0;
		s->vel.vz = 0;
	}

	FISH_tickBaitSprite();
end:;
}

void FISH_pushSeadramonSegment(VECTOR *unused, SVECTOR *src)
{
	int32_t i;

	for (i = 4; i > 0; i--) {
		FISH_SEADRAMON_HISTORY[i] = FISH_SEADRAMON_HISTORY[i - 1];
	}

	FISH_SEADRAMON_HISTORY[0] = *src;
}

void FISH_drawSeadramon(VECTOR *start, SVECTOR *rot)
{
	VECTOR pos;
	GsDOBJ2 *obj;
	int32_t i;

	pos = *start;

	for (i = 0; i < 5; i++) {
		obj = FISH_getModelObject(&FISHING_DATA_PTR->seadramonModel, i);

		FISH_SEADRAMON_HISTORY[i].vx += 0x800;
		FISH_SEADRAMON_HISTORY[i].vz = 0x800 - FISH_SEADRAMON_HISTORY[i].vz;

		RotMatrixYXZ(&FISH_SEADRAMON_HISTORY[i], &FISH_SCRATCH->m[1]);

		FISH_SEADRAMON_HISTORY[i].vx -= 0x800;
		FISH_SEADRAMON_HISTORY[i].vz = 0x800 - FISH_SEADRAMON_HISTORY[i].vz;

		FISH_SCRATCH->v0.vx = 0x1000;
		FISH_SCRATCH->v0.vy = 0x1000;
		FISH_SCRATCH->v0.vz = 0x1000;
		ScaleMatrix(&FISH_SCRATCH->m[1], &FISH_SCRATCH->v0);

		FISH_SCRATCH->m[1].t[0] = pos.vx;
		FISH_SCRATCH->m[1].t[1] = pos.vy;
		FISH_SCRATCH->m[1].t[2] = pos.vz;

		GsMulCoord0(&GsWSMATRIX, &FISH_SCRATCH->m[1], &FISH_SCRATCH->m[0]);

		obj->coord2 = NULL;
		obj->attribute = 0;

		GsSetLsMatrix(&FISH_SCRATCH->m[0]);
		GsSetLightMatrix(&FISH_SCRATCH->m[1]);
		GsSortObject4(obj, ACTIVE_ORDERING_TABLE, 2, FISH_SCRATCH->ot);

		RotMatrixYXZ(&FISH_SEADRAMON_HISTORY[i], &FISH_SCRATCH->m[1]);

		FISH_SCRATCH->v0.vx = 0x1000;
		FISH_SCRATCH->v0.vy = 0x1000;
		FISH_SCRATCH->v0.vz = 0x1000;
		ScaleMatrix(&FISH_SCRATCH->m[1], &FISH_SCRATCH->v0);

		ApplyMatrix(&FISH_SCRATCH->m[1], &FISH_D_8007A56C[i], &FISH_SCRATCH->v0);

		addVector(&pos, &FISH_SCRATCH->v0);
	}
}

void FISH_spawnSeadramon(Fish *f)
{
	FishSeadramon *sea = &FISHING_DATA_PTR->seadramon;

	f->kind = 6;
	f->size = customRandom(FISH_KIND_INFO[f->kind].lo, FISH_KIND_INFO[f->kind].hi);
	f->lifetime = customRandom(0x258, 0x4b0);
	f->state = 0;
	f->framesOnState = 0;
	f->transparency = 0;
	f->timeThisMove = 0;

	sea->path = FISHING_DATA_PTR->mapData->seadramonWaypoints;
	sea->pathLength = FISHING_DATA_PTR->mapData->seadramonWaypointCount;

	f->state = 0;
	MAIN_func_800E4038(&f->pos, sea->path[f->state].vx, sea->path[f->state].vy, NULL);
	f->state++;
	MAIN_func_800E4038(&FISH_SCRATCH->v0, sea->path[f->state].vx, sea->path[f->state].vy, NULL);

	toEulerAngles(&f->rotation, FISH_SCRATCH->v0.vx - f->pos.vx, 0,
	              FISH_SCRATCH->v0.vz - f->pos.vz);
}

void FISH_tickSeadramon(Fish *f)
{
	FishSeadramon *sea = &FISHING_DATA_PTR->seadramon;
	VECTOR target;
	int32_t d;
	int32_t n;

	if (f->state >= sea->pathLength) {
		f->timeThisMove += 0xa;
		f->timeThisMove = (f->timeThisMove < 0) ? 0 : ((f->timeThisMove > 0x32) ? 0x32 : f->timeThisMove);
		FISH_SCRATCH->v0.vx = 0;
		FISH_SCRATCH->v0.vy = 0;
		FISH_SCRATCH->v0.vz = f->timeThisMove;
		rotateVectorYXZ(&f->rotation, &FISH_SCRATCH->v0, &FISH_SCRATCH->v1);
		addVector(&f->pos, &FISH_SCRATCH->v1);
		f->transparency -= 8;
		f->transparency = (f->transparency < 0) ? 0 : ((f->transparency > 0x80) ? 0x80 : f->transparency);
		if (f->transparency <= 0) {
			f->active = 0;
			FISHING_DATA_PTR->fishPool.count--;
			return;
		}
	} else {
		MAIN_func_800E4038(&target, sea->path[f->state].vx, sea->path[f->state].vy, NULL);
		f->transparency += 8;
		f->transparency = (f->transparency < 0) ? 0 : ((f->transparency > 0x80) ? 0x80 : f->transparency);
		toEulerAngles(&FISH_SCRATCH->rot, target.vx - f->pos.vx, 0,
		              target.vz - f->pos.vz);
		FISH_SCRATCH->unk18 = FISH_SCRATCH->rot.vy - f->rotation.vy;
		FISH_SCRATCH->unk18 &= 0xfff;
		n = FISH_SCRATCH->unk18;
		if (n >= 0x800) {
			FISH_SCRATCH->unk18 = n - 0x1000;
		}
		n = FISH_SCRATCH->unk18;
		n = (n > 0) ? n : -n;
		f->timeThisMove += ((n != 0) ? -0xa : 0xa);
		f->timeThisMove = (f->timeThisMove < 0) ? 0 : ((f->timeThisMove > 0x32) ? 0x32 : f->timeThisMove);
		d = FISH_SCRATCH->unk18;
		d = (d < -0x80) ? -0x80 : ((FISH_SCRATCH->unk18 > 0x80) ? 0x80 : d);
		f->rotation.vy += d;
		FISH_SCRATCH->v0.vx = 0;
		FISH_SCRATCH->v0.vy = 0;
		FISH_SCRATCH->v0.vz = f->timeThisMove;
		rotateVectorYXZ(&f->rotation, &FISH_SCRATCH->v0, &FISH_SCRATCH->v1);
		addVector(&f->pos, &FISH_SCRATCH->v1);
		FISH_SCRATCH->unk18 = getDistance(target.vx - f->pos.vx, 0,
		                                  target.vz - f->pos.vz);
		n = (f->timeThisMove > 0xa) ? f->timeThisMove : 0xa;
		if (n >= FISH_SCRATCH->unk18) {
			f->state++;
		}
		f->timeThisMove = ((FISH_SCRATCH->unk18 / 10) > 1) ? ((long)FISH_SCRATCH->unk18 / 10) : 1;
	}

	f->framesOnState++;
	f->animPhase++;
	FISH_pushSeadramonSegment(&f->pos, &f->rotation);
}

void FISH_spawnFish(Fish *f, int32_t kind)
{
	VECTOR v;

	f->kind = kind;
	f->size = customRandom(FISH_KIND_INFO[f->kind].lo, FISH_KIND_INFO[f->kind].hi);
	f->lifetime = customRandom(0x258, 0x4b0);
	f->state = 0;
	f->framesOnState = 0;
	f->transparency = 0;
	FISH_randomPointInSpot(f->kind, &f->pos);
	FISH_randomPointInSpot(f->kind, &v);
	toEulerAngles(&f->rotation, v.vx - f->pos.vx, 0, v.vz - f->pos.vz);
}

void FISH_tickFish(Fish *f)
{
	int32_t e;
	VECTOR step;
	VECTOR out;
	VECTOR target;
	VECTOR sw2;
	FishRipple *p;
	PositionData *pd;
	int32_t d;

	if (f->kind == 6) {
		FISH_tickSeadramon(f);
		return;
	}

	switch (f->state) {
	case 0:
		step.vx = 0;
		step.vy = 0;
		step.vz = 0x1e;
		rotateVectorYXZ(&f->rotation, &step, &out);
		addVector(&f->pos, &out);
		f->transparency += 8;
		f->transparency = (f->transparency < 0) ? 0 : ((f->transparency > 0x80) ? 0x80 : f->transparency);
		if (f->transparency != 0x80) {
			break;
		}
pickNextMove:
		f->timeNextMove = customRandom(FISH_KIND_INFO[f->kind].moveDelayMin,
		                               FISH_KIND_INFO[f->kind].moveDelayMax);
		f->state = 0xa;
		f->framesOnState = -1;
		break;

	case 0xa:
		d = f->lifetime;
		d--;
		f->lifetime = d;
		if (d == 0) {
			goto faceTamer;
		}
		d = f->timeNextMove;
		d--;
		f->timeNextMove = d;
		if (d == 0) {
			goto newTarget;
		}
checkAttractor:
		if (FISHING_DATA_PTR->ripplePool.attractor != NULL) {
			p = FISHING_DATA_PTR->ripplePool.attractor;
			if (getDistance(p->pos.vx - f->pos.vx, 0, p->pos.vz - f->pos.vz) <= FISHING_DATA_PTR->ripplePool.attractRadius) {
				FISHING_DATA_PTR->fishPool.escapeTimer = 0x1e;
				goto faceAttractor;
			}
		}
		if (FISHING_DATA_PTR->rod.bait != FISH_KIND_INFO[f->kind].bait) {
			break;
		}
		d = getDistance(FISHING_DATA_PTR->swimmer.pos.vx - f->pos.vx, 0,
		                FISHING_DATA_PTR->swimmer.pos.vz - f->pos.vz);
		if (d < 0xc9) {
			break;
		}
		if (d < 0x258) {
			goto approachBobber;
		}
		break;
faceAttractor:
		p = FISHING_DATA_PTR->ripplePool.attractor;
		toEulerAngles(&f->rotation, p->pos.vx - f->pos.vx, 0,
		              p->pos.vz - f->pos.vz);
		f->rotation.vy += 0x800;
		f->state = 0x1e;
		f->framesOnState = -1;
		break;
newTarget:
		FISH_randomPointInSpot(f->kind, &target);
		toEulerAngles(&f->rotation, target.vx - f->pos.vx, 0,
		              target.vz - f->pos.vz);
		f->timeThisMove = getDistance(target.vx - f->pos.vx, 0,
		                              target.vz - f->pos.vz) /
		                  8;
		f->timeThisMove = (f->timeThisMove < 0) ? 0 : ((f->timeThisMove > 0x32) ? 0x32 : f->timeThisMove);
		f->timeThisMove = (f->timeThisMove < 0) ? 0 : ((f->size < f->timeThisMove) ? f->size : f->timeThisMove);
		f->state = 0x14;
		f->framesOnState = -1;
		break;
approachBobber:
		sw2 = FISHING_DATA_PTR->swimmer.pos;
		toEulerAngles(&f->rotation, sw2.vx - f->pos.vx, 0, sw2.vz - f->pos.vz);
		f->timeThisMove = getDistance(sw2.vx - f->pos.vx, 0, sw2.vz - f->pos.vz) / 8;
		f->timeThisMove = (f->timeThisMove < 3) ? 3 : ((f->timeThisMove > 0x32) ? 0x32 : f->timeThisMove);
		f->timeThisMove = (f->timeThisMove < 3) ? 3 : ((f->size < f->timeThisMove) ? f->size : f->timeThisMove);
		f->state = 0x14;
		f->framesOnState = -1;
		break;

	case 0x14:
		step.vx = 0;
		step.vy = 0;
		step.vz = f->timeThisMove;
		rotateVectorYXZ(&f->rotation, &step, &out);
		addVector(&f->pos, &out);
		e = f->timeThisMove;
		d = e;
		e >>= 3;
		f->timeThisMove = d - e;
		if (f->timeThisMove < 8) {
			goto pickNextMove;
		}
		goto checkAttractor;

faceTamer:
		pd = TAMER_ENTITY.entity.posData;
		toEulerAngles(&f->rotation, pd->location.vx - f->pos.vx, 0,
		              pd->location.vz - f->pos.vz);
		f->rotation.vy += 0x800;
		f->state = 0x1e;
		f->framesOnState = -1;
		break;

	case 0x1e:
		step.vx = 0;
		step.vy = 0;
		step.vz = 0x28;
		rotateVectorYXZ(&f->rotation, &step, &out);
		addVector(&f->pos, &out);
		f->transparency -= 8;
		f->transparency = (f->transparency < 0) ? 0 : ((f->transparency > 0x80) ? 0x80 : f->transparency);
		if (f->transparency == 0) {
			f->active = 0;
			FISHING_DATA_PTR->fishPool.count--;
		}
		break;
	}

	f->framesOnState++;
	f->animPhase++;
}

void FISH_drawFishSprite(int32_t kind, VECTOR *pos, SVECTOR *rot, int32_t size, int32_t phase, int32_t transparency, int32_t flag)
{
	SVECTOR r;
	VECTOR scale;
	FishSprite *desc;
	int32_t t;
	int32_t shade;

	if (kind == 6) {
		FISH_drawSeadramon(pos, rot);
		return;
	}

	if (flag == 0) {
		if (size >= 0x64) {
			desc = FISH_SPRITES + MAIN_D_801349A4[(phase / 3) & 7];
			t = size * FISHING_DATA_PTR->mapData->fishScaleLarge;
			scale.vz = t;
			scale.vy = t;
			scale.vx = t;
		} else {
			desc = FISH_SPRITES + MAIN_D_801349AC[(phase / 4) & 3];
			t = size * FISHING_DATA_PTR->mapData->fishScaleSmall;
			scale.vz = t;
			scale.vy = t;
			scale.vx = t;
		}
	} else {
		if (size >= 0x64) {
			desc = FISH_SPRITES + MAIN_D_801349B0[(uint32_t)phase % 6];
			t = size * 170;
			scale.vz = t;
			scale.vy = t;
			scale.vx = t;
		} else {
			desc = FISH_SPRITES + MAIN_D_801349B8[(uint32_t)phase % 6];
			t = size * 0x155;
			scale.vz = t;
			scale.vy = t;
			scale.vx = t;
		}
	}

	shade = transparency * 30 / 128;

	r.vx = -0x400;
	r.vy = rot->vy;
	r.vz = 0;

	FISH_drawSprite3D(pos, &r, &scale, desc,
	                  (shade & 0xff) | ((shade & 0xff) << 8) | ((shade & 0xff) << 16), 6, 0x64);
}

void FISH_drawFish(Fish *f)
{
	FISH_drawFishSprite(f->kind, &f->pos, &f->rotation, f->size, f->animPhase, f->transparency, 0);
}

Fish *FISH_allocFish(void)
{
	FishPool *pool = &FISHING_DATA_PTR->fishPool;
	Fish *end;
	Fish *start;
	Fish *p;

	if (pool->count >= 33U) {
		return NULL;
	}

	end = &pool->items[33];
	p = pool->next;
	start = p;

	do {
		if (p->active == 0) {
			pool->next = p;
			return p;
		}

		p++;
		if (p >= end) {
			p = pool->items;
		}
	} while (p != start);

	return NULL;
}

int32_t FISH_isSeadramonSpawned(void)
{
	FishingData *d = FISHING_DATA_PTR;
	Fish *f = d->fishPool.items;
	Fish *end = &d->fishPool.items[33];

	while (f < end) {
		if (f->active != 0) {
			if (f->kind == 6) {
				return 1;
			}
		}
		f++;
	}

	return 0;
}

void FISH_tickFishPopulation(void)
{
	FishPool *pool = &FISHING_DATA_PTR->fishPool;
	int32_t counts[6];
	Fish *f;
	Fish *g;
	FishingMapData *map;
	uint8_t kind;
	int32_t i;
	int32_t total;
	int32_t best;
	int32_t bestIdx;

	pool->frameCount++;
	if (pool->escapeTimer > 0) {
		pool->escapeTimer--;
	} else {
		if ((uint32_t)pool->count < 0x20) {
			for (i = 0; i < 6U; i++) {
				counts[i] = 0;
			}
			total = 0;
			f = FISHING_DATA_PTR->fishPool.items;
			for (g = f; g < &FISHING_DATA_PTR->fishPool.items[33]; g++) {
				if (g->active) {
					kind = g->kind;
					if (kind < 6U) {
						total++;
						counts[kind]++;
					}
				}
			}
			for (i = 0; i < 6U; i++) {
				counts[i] = counts[i] * 10000 /
				            (total * FISHING_DATA_PTR->mapData->fishingSpots[i].unk0);
			}
			bestIdx = 0;
			best = 0x2711;
			for (i = 0; i < 6U; i++) {
				if (counts[i] <= best) {
					best = counts[i];
					bestIdx = i;
				}
			}
			f = FISH_allocFish();
			if (f != NULL) {
				f->active = 1;
				pool->count++;
				FISH_spawnFish(f, bestIdx);
			}
		}
		if ((VSync(-1) / 3 % 20) == 0) {
			map = FISHING_DATA_PTR->mapData;
			if ((map->seadramonTime1 != 0x270f) || (map->seadramonTime2 != 0x270f)) {
				if (((map->seadramonTime1 / 100) == HOUR) ||
				    ((map->seadramonTime2 / 100) == HOUR)) {
					if (FISH_isSeadramonSpawned() == 0) {
						if (!FISHING_DATA_PTR->hooked.fish.active) {
							f = FISH_allocFish();
							if (f != NULL) {
								f->active = 1;
								pool->count++;
								FISH_spawnSeadramon(f);
							}
						}
					}
				}
			}
		}
	}

	for (i = 0; i < 0x21U; i++) {
		if (pool->items[i].active) {
			FISH_tickFish(&pool->items[i]);
		}
	}
}

void FISH_drawFishPool(void)
{
	FishPool *pool = &FISHING_DATA_PTR->fishPool;
	int32_t i;

	for (i = 0; i < 33U; i++) {
		if (pool->items[i].active) {
			FISH_drawFish(&pool->items[i]);
		}
	}
}

void FISH_initFishPool(void)
{
	FishPool *pool = &FISHING_DATA_PTR->fishPool;
	int32_t i;

	for (i = 0; i < 33U; i++) {
		pool->items[i].active = 0;
	}

	pool->next = &pool->items[1];
	pool->count = 0;
	pool->unk740 = 0;
	pool->frameCount = 0;
	pool->escapeTimer = 0;
}

void FISH_fishPoolStub(void)
{
}

Fish *FISH_findFishToBite(Fish *pool, VECTOR *loc, int32_t depth)
{
	Fish *f;
	int32_t i;
	int32_t chance;
	int32_t rowOff;

	if (FISHING_DATA_PTR->hooked.fish.active) {
		return NULL;
	}

	i = 0;
	f = pool;
	rowOff = (depth - 0x26) * 6;
	for (; i < 0x21U; i++, f = (Fish *)((int32_t)f + 56)) {
		if (!f->active) {
			continue;
		}
		if (getDistance(f->pos.vx - loc->vx, 0, f->pos.vz - loc->vz) >= 0xc9) {
			continue;
		}
		if (f->kind == 6) {
			if (((VSync(-1) / 3) % 8) != 0) {
				continue;
			} else {
			}
			chance = 0x14;
		} else {
			if (((VSync(-1) / 3) % 20) != 0) {
				continue;
			}
			if ((f->state != 0xa) && (f->state != 0x14)) {
				continue;
			}
			chance = 0;
			if ((depth >= 0x26) && (depth < 0x47)) {
				chance = (FISH_BITE_CHANCE + rowOff)[f->kind];
			}
		}
		FISH_spawnMarker((chance == 0) ? 0x30 : 0x2f, &f->pos);
		if (chance < customRandom(1, 0x64)) {
			continue;
		}
		return f;
	}

	return NULL;
}

void FISH_clearHookedFish(void)
{
	FISHING_DATA_PTR->hooked.fish.active = 0;
}

void FISH_hookFish(Fish *f)
{
	HookedFish *h = &FISHING_DATA_PTR->hooked;

	FISH_clearMarkers();

	h->fish.active = f->active;
	h->fish.kind = f->kind;
	h->fish.size = f->size;
	h->fish.transparency = f->transparency;
	h->fish.pos = f->pos;
	h->fish.rotation = f->rotation;
	h->fish.animPhase = f->animPhase;
	h->fish.state = f->state;
	h->fish.framesOnState = f->framesOnState;
	h->fish.lifetime = f->lifetime;
	h->fish.timeNextMove = f->timeNextMove;
	h->fish.timeThisMove = f->timeThisMove;

	f->active = 0;

	FISHING_DATA_PTR->fishPool.count--;

	toEulerAngles(&FISHING_DATA_PTR->hooked.fish.rotation,
	              FISHING_DATA_PTR->swimmer.pos.vx - h->fish.pos.vx, 0,
	              FISHING_DATA_PTR->swimmer.pos.vz - h->fish.pos.vz);

	h->fish.state = 10;
	h->fish.framesOnState = 0;
	h->fish.animPhase = 0;
	h->animMode = 0;
	h->strikeTimer = 0;

	if (h->fish.size < 0x96) {
		h->nibbleTime = customRandom(0x14, 0x28);
		h->catchingTime = 0x1e;
	} else {
		h->nibbleTime = customRandom(0x28, 0x3c);
		h->catchingTime = 0x3c;
	}

	h->powerLeft = h->fish.size + 0x96;
	h->powerMax = h->powerLeft + 0x64;
	h->escaped = 0;
}

void FISH_tickHookedFish(int32_t mode)
{
	HookedFish *h = &FISHING_DATA_PTR->hooked;
	VECTOR step;
	VECTOR probe;
	int32_t i;
	int32_t k;
	int32_t t;

	if (!h->fish.active) {
		goto ret;
	}

	if (FISH_isTextBoxVisible() != 0) {
		goto ret;
	}

	if (mode == 0) {
		goto stateDispatch;
	}

	if (mode == 0x11) {
		goto caught;
	}

	if (mode == 0x12) {
		goto snapped;
	}

	if (mode == 0xf) {
		goto escape;
	}

	if (mode != 0x10) {
		goto checkKind;
	}

	goto grabbed;

caught:
	h->fish.active = 0;
	goto checkKind;

stateDispatch:
	t = h->fish.state;
	if (t == 0x64) {
		goto seadraChase;
	}

	if (t == 0x5a) {
		goto sinking;
	}

	if (t == 0x32) {
		goto fighting;
	}

	if (t == 0x28) {
		goto struggling;
	}

	if (t == 0x14) {
		goto approaching;
	}

	if (t != 0xa) {
		goto tick;
	}

	if (h->fish.framesOnState == h->nibbleTime) {
		goto biteTaken;
	}

	if (h->fish.size < 0x96) {
		if (customRandom(1, 0x14) == 1) {
			if ((h->nibbleTime - h->fish.framesOnState) >= 0x14) {
				FISH_bobberCommand(&FISHING_DATA_PTR->swimmer, 2);
			}
		}
	} else {
		if (customRandom(1, 0x14) == 1) {
			FISH_bobberCommand(&FISHING_DATA_PTR->swimmer, 0xc);
		}
	}

	FISH_SCRATCH->v0.vx = FISHING_DATA_PTR->swimmer.pos.vx - h->fish.pos.vx;
	FISH_SCRATCH->v0.vy = 0;
	FISH_SCRATCH->v0.vz = FISHING_DATA_PTR->swimmer.pos.vz - h->fish.pos.vz;
	FISH_SCRATCH->unk18 = getDistance(FISH_SCRATCH->v0.vx, FISH_SCRATCH->v0.vy,
	                                  FISH_SCRATCH->v0.vz);
	if (FISH_SCRATCH->unk18 < 4) {
		goto tick;
	}

	toEulerAngles(&FISHING_DATA_PTR->hooked.fish.rotation, FISH_SCRATCH->v0.vx,
	              FISH_SCRATCH->v0.vy, FISH_SCRATCH->v0.vz);
	k = FISH_SCRATCH->unk18;
	k = (k <= 0) ? 1 : ((FISH_SCRATCH->unk18 >= 0xb) ? 0xa : k);
	FISH_SCRATCH->unk18 = k;
	FISH_SCRATCH->v0.vx = 0;
	FISH_SCRATCH->v0.vy = 0;
	FISH_SCRATCH->v0.vz = FISH_SCRATCH->unk18;
	rotateVectorYXZ(&FISHING_DATA_PTR->hooked.fish.rotation, &FISH_SCRATCH->v0,
	                &FISH_SCRATCH->v1);
	addVector(&h->fish.pos, &FISH_SCRATCH->v1);
	goto tick;

biteTaken:
	FISH_bobberCommand(&FISHING_DATA_PTR->swimmer, 0xb);
	FISHING_DATA_PTR->rod.itemTaken = 1;
	removeItem(INVENTORY.types.array[FISHING_DATA_PTR->rod.itemSlot], 1);
	h->fish.state = 0x14;
	h->fish.framesOnState = -1;
	goto tick;

approaching:
	FISH_SCRATCH->v0.vx = FISHING_DATA_PTR->swimmer.pos.vx - h->fish.pos.vx;
	FISH_SCRATCH->v0.vy = 0;
	FISH_SCRATCH->v0.vz = FISHING_DATA_PTR->swimmer.pos.vz - h->fish.pos.vz;
	FISH_SCRATCH->unk18 = getDistance(FISH_SCRATCH->v0.vx, FISH_SCRATCH->v0.vy,
	                                  FISH_SCRATCH->v0.vz);
	if (FISH_SCRATCH->unk18 >= 4) {
		toEulerAngles(&FISHING_DATA_PTR->hooked.fish.rotation, FISH_SCRATCH->v0.vx,
		              FISH_SCRATCH->v0.vy, FISH_SCRATCH->v0.vz);
		k = FISH_SCRATCH->unk18;
		k = (k <= 0) ? 1 : ((FISH_SCRATCH->unk18 >= 0xb) ? 0xa : k);
		FISH_SCRATCH->unk18 = k;
		FISH_SCRATCH->v0.vx = 0;
		FISH_SCRATCH->v0.vy = 0;
		FISH_SCRATCH->v0.vz = FISH_SCRATCH->unk18;
		rotateVectorYXZ(&FISHING_DATA_PTR->hooked.fish.rotation, &FISH_SCRATCH->v0,
		                &FISH_SCRATCH->v1);
		addVector(&h->fish.pos, &FISH_SCRATCH->v1);
	}

	if (h->fish.framesOnState < h->catchingTime) {
		goto tick;
	}

	k = 1;
	goto setEscaped;

grabbed:
	k = 1;
setAnim:
	h->animMode = k;
	h->fish.state = 0x28;
	h->fish.framesOnState = -1;
	goto tick;

struggling:
	if (customRandom(0, 0xa) == 0) {
		playSound(8, 5);
	}

	if (customRandom(1, 0xa) == 1) {
		toEulerAngles(&h->fish.rotation,
		              TAMER_ENTITY.entity.posData->location.vx - h->fish.pos.vx, 0,
		              TAMER_ENTITY.entity.posData->location.vz - h->fish.pos.vz);
		h->fish.rotation.vy = h->fish.rotation.vy +
		                      (int16_t)(customRandom(-0x384, 0x384) + 0x800);
	}

	if (customRandom(1, 4) == 1) {
		FISH_spawnRipple(0, &h->fish.pos, 0x2bc, h->fish.size / 2,
		                 (h->fish.size << 12) / 120);
		FISH_SCRATCH->v0.vx = 0;
		FISH_SCRATCH->v0.vy = 0;
		FISH_SCRATCH->v0.vz = h->fish.size;
		rotateVectorYXZ(&h->fish.rotation, &FISH_SCRATCH->v0, &FISH_SCRATCH->v1);
		addVector(&FISH_SCRATCH->v1, &h->fish.pos);
		FISH_spawnRipple(0, &h->fish.pos, 0x2bc, 0, h->fish.size / 2);
	}

	i = 0;
	goto probeTest;
probeStep:
	step.vx = 0;
	step.vy = 0;
	step.vz = 0x12c;
	rotateVectorYXZ(&h->fish.rotation, &step, &probe);
	addVector(&probe, &FISHING_DATA_PTR->swimmer.pos);
	if (FISH_projectPoint(&probe, &FISH_SCRATCH->x, &FISH_SCRATCH->y) != 0) {
		if (FISH_isScreenPointVisible(FISH_SCRATCH->x, FISH_SCRATCH->y) != 0) {
			if (FISHING_DATA_PTR->mapData->isWater(FISH_SCRATCH->x,
			                                       FISH_SCRATCH->y) != 0) {
				goto probeNext;
			}
		}
	}

	FISH_randomPointInSpot(h->fish.kind, &FISH_SCRATCH->v0);
	toEulerAngles(&h->fish.rotation, FISH_SCRATCH->v0.vx - h->fish.pos.vx, 0,
	              FISH_SCRATCH->v0.vz - h->fish.pos.vz);
probeNext:
	i++;
probeTest:
	if (i < 0x1e) {
		goto probeStep;
	}

	step.vx = 0;
	step.vy = 0;
	t = (h->powerLeft <= (h->powerMax / 10)) ? 0xf : 8;
	step.vz = h->fish.timeThisMove / t;
	rotateVectorYXZ(&h->fish.rotation, &step, &probe);
	addVector(&probe, &FISHING_DATA_PTR->swimmer.pos);
	h->fish.pos = FISHING_DATA_PTR->swimmer.pos = probe;
	h->fish.pos = FISHING_DATA_PTR->swimmer.pos;
	if (FISHING_DATA_PTR->rod.pressedFrames == -1) {
		goto powerCheck;
	}

	if (FISHING_DATA_PTR->rod.pressedFrames >= 0x3d) {
		if ((FISHING_DATA_PTR->rod.pressedFrames % 3) == 0) {
			goto powerCheck;
		}
	}

	if (customRandom(1, 0x14) >= 3) {
		goto stayStruggling;
	}

powerCheck:
	if (customRandom(1, 0x1000) <= ((h->powerLeft << 12) / h->powerMax)) {
		goto nextMove;
	}

stayStruggling:
	if (FISHING_DATA_PTR->rod.fishingState == 0xb) {
		goto tick;
	}

	k = 1;
	goto setEscaped;

nextMove:
	toEulerAngles(&h->fish.rotation,
	              TAMER_ENTITY.entity.posData->location.vx - h->fish.pos.vx, 0,
	              TAMER_ENTITY.entity.posData->location.vz - h->fish.pos.vz);
	h->fish.rotation.vy = h->fish.rotation.vy +
	                      (int16_t)(customRandom(-0x384, 0x384) + 0x800);
	h->fish.timeThisMove = customRandom(0x14, 0x32);
	h->nibbleTime = customRandom(9, 0x13);
	h->fish.state = 0x32;
	h->fish.framesOnState = -1;
	goto tick;

fighting:
	t = h->fish.framesOnState % 2;
	if (t == 0) {
		h->powerLeft--;
		h->powerLeft = (h->powerLeft < 0) ? 0 : ((h->powerMax < h->powerLeft) ? h->powerMax : h->powerLeft);
	}

	if (customRandom(0, 0xa) == 0) {
		playSound(8, 6);
	}

	FISH_spawnRipple(0, &h->fish.pos, 0x2bc, 1, (h->fish.size << 12) / 120);
	i = 0;
	goto probeTest2;
probeStep2:
	step.vx = 0;
	step.vy = 0;
	step.vz = 0x12c;
	rotateVectorYXZ(&h->fish.rotation, &step, &probe);
	addVector(&probe, &FISHING_DATA_PTR->swimmer.pos);
	if (FISH_projectPoint(&probe, &FISH_SCRATCH->x, &FISH_SCRATCH->y) != 0) {
		if (FISH_isScreenPointVisible(FISH_SCRATCH->x, FISH_SCRATCH->y) != 0) {
			if (FISHING_DATA_PTR->mapData->isWater(FISH_SCRATCH->x,
			                                       FISH_SCRATCH->y) != 0) {
				goto probeNext2;
			}
		}
	}

	FISH_randomPointInSpot(h->fish.kind, &FISH_SCRATCH->v0);
	toEulerAngles(&h->fish.rotation, FISH_SCRATCH->v0.vx - h->fish.pos.vx, 0,
	              FISH_SCRATCH->v0.vz - h->fish.pos.vz);
probeNext2:
	i++;
probeTest2:
	if (i < 0x1e) {
		goto probeStep2;
	}

	step.vx = 0;
	step.vy = 0;
	step.vz = h->fish.timeThisMove;
	rotateVectorYXZ(&h->fish.rotation, &step, &probe);
	addVector(&probe, &FISHING_DATA_PTR->swimmer.pos);
	h->fish.pos = FISHING_DATA_PTR->swimmer.pos = probe;
	if (FISHING_DATA_PTR->rod.fishingState != 0xb) {
		goto escape;
	}

	if (h->fish.framesOnState >= h->nibbleTime) {
		goto grabbed;
	}

	goto tick;

escape:
	k = 1;
setEscaped:
	h->escaped = k;
	FISH_bobberCommand(&FISHING_DATA_PTR->swimmer, 7);
	toEulerAngles(&h->fish.rotation,
	              TAMER_ENTITY.entity.posData->location.vx - h->fish.pos.vx, 0,
	              TAMER_ENTITY.entity.posData->location.vz - h->fish.pos.vz);
	h->fish.rotation.vy += 0x800;
	h->fish.state = 0x5a;
	h->fish.framesOnState = -1;
	goto tick;

sinking:
	step.vx = 0;
	step.vy = 0;
	step.vz = 0x28;
	rotateVectorYXZ(&h->fish.rotation, &step, &probe);
	addVector(&h->fish.pos, &probe);
	h->fish.transparency -= 8;
	h->fish.transparency = (h->fish.transparency < 0) ? 0 : ((h->fish.transparency >= 0x81) ? 0x80 : h->fish.transparency);
	if (h->fish.transparency != 0) {
		goto tick;
	}

	h->fish.active = 0;
	goto tick;

snapped:
	h->fish.state = 0x64;
	h->fish.framesOnState = -1;
	goto tick;

seadraChase:
	FISH_SCRATCH->unk18 = getDistance(
		FISH_REEL_TARGET.vx - FISHING_DATA_PTR->hooked.fish.pos.vx, 0,
		FISH_REEL_TARGET.vz - FISHING_DATA_PTR->hooked.fish.pos.vz);
	k = FISH_SCRATCH->unk18;
	k = (k < 0) ? 0 : ((FISH_SCRATCH->unk18 >= 0x33) ? 0x32 : k);
	FISH_SCRATCH->unk18 = k;
	toEulerAngles(&h->fish.rotation, FISH_REEL_TARGET.vx - h->fish.pos.vx, 0,
	              FISH_REEL_TARGET.vz - h->fish.pos.vz);
	FISH_SCRATCH->v0.vx = 0;
	FISH_SCRATCH->v0.vy = 0;
	FISH_SCRATCH->v0.vz = FISH_SCRATCH->unk18;
	rotateVectorYXZ(&h->fish.rotation, &FISH_SCRATCH->v0, &FISH_SCRATCH->v1);
	addVector(&h->fish.pos, &FISH_SCRATCH->v1);
	FISH_spawnRipple(0, &h->fish.pos, 0x2bc, 1, (h->fish.size << 12) / 120);
	if (customRandom(1, 0xa) < 6) {
		playSound(8, 6);
	}

tick:
	h->fish.framesOnState++;
	h->fish.animPhase++;
	if (h->strikeTimer != 0) {
		h->strikeTimer--;
	}

checkKind:
	if (h->fish.kind == 6) {
		FISH_pushSeadramonSegment(&h->fish.pos, &h->fish.rotation);
	}

ret:;
}

int32_t FISH_tryStrike(void)
{
	HookedFish *h = &FISHING_DATA_PTR->hooked;

	if ((h->fish.state == 0x14) && (h->fish.framesOnState < (h->catchingTime / 2))) {
		h->strikeTimer = 0x14;
		FISH_tickHookedFish(0x10);
		return 1;
	}

	FISH_tickHookedFish(0xf);

	return 0;
}

void FISH_drawHookedFish(void)
{
	HookedFish *h = &FISHING_DATA_PTR->hooked;

	if (h->fish.active) {
		FISH_drawFishSprite(h->fish.kind, &h->fish.pos, &h->fish.rotation, h->fish.size,
		                    h->fish.animPhase, h->fish.transparency, (h->fish.state < 0x1f) ? 0 : 1);
	}
}

void FISH_updateRodPose(FishingRod *rod)
{
	MATRIX m1;
	MATRIX m2;
	SVECTOR r;
	MATRIX m3;
	VECTOR d;
	VECTOR lv;
	SVECTOR e;
	int32_t len;
	int32_t ang;
	int32_t sx;
	int32_t sy;

	FISH_getBoneTransform(&TAMER_ENTITY.entity, 9, &FISH_SCRATCH->v0,
	                      &FISH_SCRATCH->rot);
	r.vx = 0x400;
	r.vy = 0x400;
	r.vz = 0;
	RotMatrixYXZ(&r, &m1);
	toEulerAngles(&r, -6, 0x34, 0x127);
	RotMatrixYXZ(&r, &m2);
	MulMatrix0(&m1, &m2, &m3);
	matrixToEuler2(&m3, &MAIN_D_801349C0);
	RotMatrixYXZ(&FISH_SCRATCH->rot, &FISH_SCRATCH->m[2]);
	RotMatrixYXZ(&MAIN_D_801349C0, &FISH_SCRATCH->m[3]);
	MulMatrix(&FISH_SCRATCH->m[2], &FISH_SCRATCH->m[3]);
	matrixToEuler2(&FISH_SCRATCH->m[2], &FISH_SCRATCH->rot);
	ApplyMatrix(&FISH_SCRATCH->m[2], &MAIN_D_801349C8, &FISH_SCRATCH->v1);
	addVector(&FISH_SCRATCH->v0, &FISH_SCRATCH->v1);

	len = (FISHING_DATA_PTR->tension.display != 0) ? (((FISHING_DATA_PTR->tension.level + FISHING_DATA_PTR->tension.limit) << 9) / (FISHING_DATA_PTR->tension.limit * 2)) : 0x80;

	d.vx = FISHING_DATA_PTR->swimmer.pos.vx - FISH_SCRATCH->v0.vx;
	d.vy = FISHING_DATA_PTR->swimmer.pos.vy - FISH_SCRATCH->v0.vy;
	d.vz = FISHING_DATA_PTR->swimmer.pos.vz - FISH_SCRATCH->v0.vz;
	ApplyTransposeMatrixLV(&FISH_SCRATCH->m[2], &d, &lv);
	toEulerAngles(&e, lv.vx, lv.vy, lv.vz);
	sx = lv.vy;
	ang = ratan2(sx, lv.vx);
	sy = (int16_t)((-(len * rsin(ang))) >> 12);
	sx = (int16_t)((len * rcos(ang)) >> 12);

	e.vx = ((e.vx > 0) ? ((e.vx < sy) ? e.vx : (int16_t)sy) : ((sy < e.vx) ? e.vx : (int16_t)sy));
	e.vy = ((e.vy > 0) ? ((e.vy < sx) ? e.vy : (int16_t)sx) : ((sx < e.vy) ? e.vy : (int16_t)sx));

	for (ang = 1; ang < 4U; ang++) {
		FISH_D_8007A5BC[ang].vx = (uint32_t)e.vx >> 2;
		FISH_D_8007A5BC[ang].vy = (uint32_t)e.vy >> 2;
		FISH_D_8007A5BC[ang].vz = (uint32_t)e.vz >> 2;
	}

	for (ang = 0; ang < 5U; ang++) {
		RotMatrixYXZ(&FISH_D_8007A5BC[ang], &FISH_SCRATCH->m[3]);
		MulMatrix(&FISH_SCRATCH->m[2], &FISH_SCRATCH->m[3]);
		FISH_ROD_SEGMENT_POS[ang] = FISH_SCRATCH->v0;
		FISH_ROD_SEGMENT_MATRIX[ang] = FISH_SCRATCH->m[2];
		copyVector(((VECTOR *)FISH_ROD_SEGMENT_MATRIX[ang].t), &FISH_SCRATCH->v0);
		ApplyMatrix(&FISH_SCRATCH->m[2], &FISH_D_8007A594[ang], &FISH_SCRATCH->v1);
		addVector(&FISH_SCRATCH->v0, &FISH_SCRATCH->v1);
	}

	rod->lineStartMirror = rod->lineStart;
	rod->lineStart = FISH_ROD_SEGMENT_POS[4];
}

void FISH_drawRodSegments(FishingRod *rod)
{
	GsDOBJ2 *obj;
	int32_t i;

	for (i = 0; i < 5U; i++) {
		GsMulCoord0(&GsWSMATRIX, &FISH_ROD_SEGMENT_MATRIX[i], &FISH_SCRATCH->m[0]);
		if ((FISH_SCRATCH->m[0].t[2] <= 0) ||
		    (FISH_SCRATCH->m[0].t[2] >= 0x10000)) {
			FISH_SCRATCH->flag = 1;
		} else {
			if (i < 4) {
				obj = FISH_getModelObject(&FISHING_DATA_PTR->rodModel, i);
				obj->coord2 = NULL;
				obj->attribute = 0;
				FISH_SCRATCH->v0.vx = 0x1000;
				FISH_SCRATCH->v0.vy = 0x1000;
				FISH_SCRATCH->v0.vz = 0x1000;
				ScaleMatrix(&FISH_SCRATCH->m[0], &FISH_SCRATCH->v0);
				GsSetLsMatrix(&FISH_SCRATCH->m[0]);
				GsSetLightMatrix(&FISH_ROD_SEGMENT_MATRIX[i]);
				GsSortObject4(obj, ACTIVE_ORDERING_TABLE, 2, FISH_SCRATCH->ot);
			}
			FISH_SCRATCH->cur.vx = (FISH_SCRATCH->m[0].t[0] * VIEWPORT_DISTANCE) /
			                       FISH_SCRATCH->m[0].t[2];
			FISH_SCRATCH->cur.vy = (FISH_SCRATCH->m[0].t[1] * VIEWPORT_DISTANCE) /
			                       FISH_SCRATCH->m[0].t[2];
			FISH_SCRATCH->cur.vz = FISH_SCRATCH->m[0].t[2];
			FISH_SCRATCH->flag = 0;
		}
		FISH_SCRATCH->unk40 = FISH_SCRATCH->flag;
		FISH_SCRATCH->prev = FISH_SCRATCH->cur;
	}
}

int32_t FISH_clipLineToRect(SVECTOR *pts, int32_t xMin, int32_t xMax, int32_t yMin, int32_t yMax)
{
	int32_t code[2];
	int32_t i;

	for (i = 0; i < 2; i++) {
		code[i] = ((pts[i].vx < xMin) ? 1 : 0) | ((xMax < pts[i].vx) ? 2 : 0);
	}

	if ((code[0] & code[1]) != 0) {
		return 0;
	}

	for (i = 0; i < 2; i++) {
		if ((code[i] & 1) != 0) {
			pts[i].vy = pts[1 - i].vy + (((xMin - pts[1 - i].vx) * (pts[i].vy - pts[1 - i].vy)) /
			                             (pts[i].vx - pts[1 - i].vx));
			pts[i].vx = xMin;
		}
		if ((code[i] & 2) != 0) {
			pts[i].vy = pts[1 - i].vy + (((xMax - pts[1 - i].vx) * (pts[i].vy - pts[1 - i].vy)) /
			                             (pts[i].vx - pts[1 - i].vx));
			pts[i].vx = xMax;
		}
	}

	for (i = 0; i < 2; i++) {
		code[i] = ((pts[i].vy < yMin) ? 1 : 0) | ((yMax < pts[i].vy) ? 2 : 0);
	}

	if ((code[0] & code[1]) != 0) {
		return 0;
	}

	for (i = 0; i < 2; i++) {
		if ((code[i] & 1) != 0) {
			pts[i].vx = pts[1 - i].vx + (((yMin - pts[1 - i].vy) * (pts[i].vx - pts[1 - i].vx)) /
			                             (pts[i].vy - pts[1 - i].vy));
			pts[i].vy = yMin;
		}
		if ((code[i] & 2) != 0) {
			pts[i].vx = pts[1 - i].vx + (((yMax - pts[1 - i].vy) * (pts[i].vx - pts[1 - i].vx)) /
			                             (pts[i].vy - pts[1 - i].vy));
			pts[i].vy = yMax;
		}
	}

	return 1;
}

void FISH_drawFishingLine(FishingRod *rod)
{
	VECTOR *ends[2];
	SVECTOR pts[2];
	VECTOR cur;
	VECTOR prev;
	int32_t i;
	int32_t j;
	int32_t n;
	int32_t k;
	int32_t lo;

	ends[0] = &rod->lineStart;
	ends[1] = &FISHING_DATA_PTR->swimmer.pos;

	for (i = 0; i < 2; i++) {
		FISH_SCRATCH->m[1].t[0] = ends[i]->vx;
		FISH_SCRATCH->m[1].t[1] = ends[i]->vy;
		FISH_SCRATCH->m[1].t[2] = ends[i]->vz;
		GsMulCoord0(&GsWSMATRIX, &FISH_SCRATCH->m[1], &FISH_SCRATCH->m[0]);
		if ((FISH_SCRATCH->m[0].t[2] <= 0) ||
		    (FISH_SCRATCH->m[0].t[2] >= 0x10000)) {
			pts[i].vz = -1;
		} else {
			pts[i].vx = (FISH_SCRATCH->m[0].t[0] * VIEWPORT_DISTANCE) /
			            FISH_SCRATCH->m[0].t[2];
			pts[i].vy = (FISH_SCRATCH->m[0].t[1] * VIEWPORT_DISTANCE) /
			            FISH_SCRATCH->m[0].t[2];
			pts[i].vz = FISH_SCRATCH->m[0].t[2];
		}
	}

	if (pts[0].vz <= 0) {
		return;
	}

	if (pts[1].vz <= 0) {
		return;
	}

	lo = 0;
	if (FISH_clipLineToRect(pts, -DRAWING_OFFSET_X + lo, -DRAWING_OFFSET_X + 0x140,
	                        -DRAWING_OFFSET_Y + lo, -DRAWING_OFFSET_Y + 0xf0) == 0) {
		return;
	}

	n = (getDistance(pts[1].vx - pts[0].vx, pts[1].vy - pts[0].vy, 0) + 0x40) / 0x40;
	for (j = 0; j <= n; j++) {
		cur.vx = ((pts[0].vx * (n - j)) / n) + ((pts[1].vx * j) / n);
		cur.vy = ((pts[0].vy * (n - j)) / n) + ((pts[1].vy * j) / n);
		cur.vz = ((pts[0].vz * (n - j)) / n) + ((pts[1].vz * j) / n);
		if (j > 0) {
			renderLinePrimitive(*(uint32_t *)&FISHING_DATA_PTR->tension.color, cur.vx, cur.vy,
			                    prev.vx, prev.vy,
			                    ((cur.vz > prev.vz) ? cur.vz : prev.vz) >> 4, 0);
		}
		prev = cur;
	}
}

void FISH_resetRodState(FishingRod *rod)
{
	rod->fishingState = 0;
	rod->displayRod = 0;
	rod->displayLine = 0;
	rod->displaySwimmer = 0;
}

void FISH_tickRod(FishingRod *rod)
{
	SVECTOR rot;
	MATRIX m;
	SVECTOR rot2;
	VECTOR d;
	VECTOR lv;
	MATRIX m2;
	SVECTOR rot3;
	VECTOR d2;
	VECTOR lv2;
	MATRIX m3;
	int32_t n;
	int32_t k;
	int32_t p;
	int32_t t;
	int32_t w;
	int32_t j;
	uint32_t r;
	int32_t moving;
	int32_t metres;
	int32_t centimetres;
	Fish *f;

	if (FISH_isTextBoxVisible()) {
		FISH_scrollViewToTarget(0x40);
		return;
	}

	switch (rod->fishingState) {
	case 2:
		goto state2;
	case 3:
		goto state3;
	case 1:
		goto state1;
	case 0xe96:
		goto stateE96;
	case 6:
		goto state6;
	case 7:
		goto state7;
	case 8:
		goto state8;
	case 9:
		goto state9;
	case 0xa:
		goto stateA;
	case 0xb:
		goto stateB;
	case 0xd:
		goto stateD;
	case 0xe:
		goto stateE;
	case 0x4b0:
		goto state4B0;
	case 0x4b1:
		goto state4B1;
	case 0x44c:
		goto state44C;
	case 0x11af:
		goto state11AF;
	default:
		goto tick;
	}

giveUp:
	if (hasFoodItem() == 0) {
		goto toStateE;
	}
	startAnimation(&TAMER_ENTITY.entity, 0);
	rod->displaySwimmer = 0;
	FISH_clearBaitSprite();
	FISH_initBobberAnim((FishRipple *)&FISHING_DATA_PTR->swimmer.pos);
	rod->displayLine = 0;
	FISHING_DATA_PTR->swimmer.mode = 0;
	rod->fishingState = 2;
	rod->framesOnState = -1;
	goto tick;

state2:
	if (FISH_panCameraToTamer(0x10)) {
		goto tick;
	}
	rod->fishingState = 3;
	rod->framesOnState = -1;
	goto tick;

state3:
	if (rod->framesOnState == 0) {
		FISH_openBaitMenu();
		goto tick;
	}
	if (FISH_isBaitMenuVisible()) {
		goto tick;
	}
	if (FISHING_DATA_PTR->baitMenu.selectedItem == -1) {
		goto toStateE;
	}
	rod->itemSlot = FISHING_DATA_PTR->baitMenu.selectedItem;
	rod->bait = INVENTORY.types.array[rod->itemSlot];
	rod->itemTaken = 0;
	rod->displaySwimmer = 0;
	startAnimation(&TAMER_ENTITY.entity, 0x1b);
	rod->fishingState = 1;
	rod->framesOnState = -1;
	goto tick;

state1:
	if (POLLED_INPUT & 0x8000) {
		TAMER_ENTITY.entity.posData->rotation.vy =
			TAMER_ENTITY.entity.posData->rotation.vy - 0x40;
	}
	if (POLLED_INPUT & 0x2000) {
		TAMER_ENTITY.entity.posData->rotation.vy =
			TAMER_ENTITY.entity.posData->rotation.vy + 0x40;
	}
	TAMER_ENTITY.entity.posData->rotation.vy =
		TAMER_ENTITY.entity.posData->rotation.vy & 0xfff;
	if (TAMER_ENTITY.entity.anim.animFlag & 1) {
		goto tick;
	}
	rod->displaySwimmer = 1;

cast:
	FISH_getRodTipPos(&FISHING_DATA_PTR->swimmer.pos);
	if (rod->itemTaken != 0) {
		goto giveUp;
	}
	startAnimation(&TAMER_ENTITY.entity, 0x1c);
	FISH_bobberCommand(&FISHING_DATA_PTR->swimmer, 4);
	FISHING_DATA_PTR->swimmer.mode = 1;
	rod->displayLine = 1;
	FISH_showBaitSprite(NULL, rod->bait);
	FISH_moveBaitToRodTip(0);
	rod->fishingState = 0xe96;
	rod->framesOnState = -1;
	goto tick;

stateE96:
	if (FISH_panCameraTo(0x10, &FISHING_DATA_PTR->swimmer.pos, 0x28, 0x1e)) {
		goto tick;
	}
	rod->fishingState = 6;
	rod->framesOnState = -1;
	goto tick;

state6:
	FISH_moveBaitToRodTip(0);
	if (POLLED_INPUT & 0x20) {
		FISH_panCameraWithDPad();
		goto tick;
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40) {
		goto throwRod;
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x10) {
		FISH_clearBaitSprite();
		goto toStateE;
	}
	if (POLLED_INPUT & 0x8000) {
		TAMER_ENTITY.entity.posData->rotation.vy =
			TAMER_ENTITY.entity.posData->rotation.vy - 0x40;
	}
	if (POLLED_INPUT & 0x2000) {
		TAMER_ENTITY.entity.posData->rotation.vy =
			TAMER_ENTITY.entity.posData->rotation.vy + 0x40;
	}
	TAMER_ENTITY.entity.posData->rotation.vy =
		TAMER_ENTITY.entity.posData->rotation.vy & 0xfff;
	FISH_panCameraTo(0x10, &FISHING_DATA_PTR->swimmer.pos, 0x28, 0x1e);
	goto tick;

throwRod:
	startAnimation(&TAMER_ENTITY.entity, 0x1d);
	FISHING_DATA_PTR->swimmer.mode = 2;
	FISHING_DATA_PTR->swimmer.vel.vx = 0;
	FISHING_DATA_PTR->swimmer.vel.vy = 0;
	FISHING_DATA_PTR->swimmer.vel.vz = 0;
	FISHING_DATA_PTR->swimmer.dist =
		getDistance(FISHING_DATA_PTR->swimmer.pos.vx - rod->lineStart.vx,
	                    FISHING_DATA_PTR->swimmer.pos.vy - rod->lineStart.vy,
	                    FISHING_DATA_PTR->swimmer.pos.vz - rod->lineStart.vz);
	FISHING_DATA_PTR->swimmer.gravity = 0x1e;
	rod->fishingState = 7;
	rod->framesOnState = -1;
	goto tick;

state7:
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40) {
		goto release;
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x20) {
		rod->framesOnState = 0x14;
		goto release;
	}
	FISH_panCameraTo(0x10, &FISHING_DATA_PTR->swimmer.pos, 0x28, 0x1e);
	if ((TAMER_ENTITY.entity.anim.animFlag & 1) == 0) {
		goto cast;
	}
	goto tick;

release:
	startAnimation(&TAMER_ENTITY.entity, 0x1e);
	FISH_updateRodPose(&FISHING_DATA_PTR->rod);
	rod->throwPower = rod->framesOnState + 5;
	FISHING_DATA_PTR->swimmer.mode = 3;
	copyVector(&FISHING_DATA_PTR->swimmer.pos, &rod->lineStart);
	FISHING_DATA_PTR->swimmer.dist = 0;
	FISHING_DATA_PTR->swimmer.gravity = 3;
	n = (rod->swimmerType == 1) ? 0xc8 : 0x190;
	MAIN_D_801349D0.vz = FISH_remapClamped(rod->throwPower, 0, 0x14, 0x64, n);
	rot.vx = 0;
	rot.vy = TAMER_ENTITY.entity.posData->rotation.vy + 0x800;
	rot.vz = -0xc8;
	RotMatrixYXZ(&rot, &m);
	ApplyMatrix(&m, &MAIN_D_801349D0, &FISHING_DATA_PTR->swimmer.vel);
	playSound(8, 0);
	FISHING_DATA_PTR->swimmer.unk40 = 0;
	rod->fishingState = 8;
	rod->framesOnState = -1;
	goto tick;

state8:
	FISH_panCameraTo(0, &FISHING_DATA_PTR->swimmer.pos, 0x28, 0x1e);
	if (POLLED_INPUT & 0x20) {
		FISH_panCameraWithDPad();
	} else {
		FISH_panCameraTo(0x10, &FISHING_DATA_PTR->swimmer.pos, 0x28, 0x1e);
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40) {
		FISHING_DATA_PTR->swimmer.vel.vx = 0;
		FISHING_DATA_PTR->swimmer.vel.vz = 0;
	}
	if (FISHING_DATA_PTR->swimmer.pos.vy < 0) {
		if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x10) {
			goto cast;
		}
		goto tick;
	}

splash:
	FISH_clearBaitSprite();
	FISHING_DATA_PTR->swimmer.pos.vy = 0;
	if (FISH_projectPoint(&FISHING_DATA_PTR->swimmer.pos, &FISH_SCRATCH->x,
	                      &FISH_SCRATCH->y) == 0) {
		goto cast;
	}
	t = FISH_SCRATCH->x;
	w = FISH_SCRATCH->y;
	if (FISH_isScreenPointVisible(t, w) == 0) {
		goto cast;
	}
	t = FISH_SCRATCH->x;
	w = FISH_SCRATCH->y;
	if (FISHING_DATA_PTR->mapData->isWater(t, w) == 0) {
		goto cast;
	}
	playSound(8, 2);
	getDistance(FISHING_DATA_PTR->swimmer.pos.vx - rod->lineStart.vx, 0,
	            FISHING_DATA_PTR->swimmer.pos.vz - rod->lineStart.vz);
	TAMER_ENTITY.entity.anim.loopCount = 1;
	FISH_bobberCommand(&FISHING_DATA_PTR->swimmer, 7);
	FISHING_DATA_PTR->swimmer.mode = 0;
	rod->fishingState = 9;
	rod->framesOnState = -1;
	goto tick;

state9:
	if (POLLED_INPUT & 0x40) {
		if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40) ||
		    ((TAMER_ENTITY.entity.anim.animFlag & 1) == 0)) {
			startAnimation(&TAMER_ENTITY.entity, 0x20);
		}
		toEulerAngles(&rot2, rod->lineStart.vx - FISHING_DATA_PTR->swimmer.pos.vx, 0,
		              rod->lineStart.vz - FISHING_DATA_PTR->swimmer.pos.vz);
		RotMatrixYXZ(&rot2, &m2);
		d.vx = rod->lineStart.vx - rod->lineStartMirror.vx;
		d.vy = rod->lineStart.vy - rod->lineStartMirror.vy;
		d.vz = rod->lineStart.vz - rod->lineStartMirror.vz;
		ApplyTransposeMatrixLV(&m2, &d, &lv);
		lv.vx = 0;
		lv.vy = 0;
		if (lv.vz > 0) {
			ApplyMatrixLV(&m2, &lv, &d);
			FISHING_DATA_PTR->swimmer.pos.vx =
				FISHING_DATA_PTR->swimmer.pos.vx + d.vx;
			FISHING_DATA_PTR->swimmer.pos.vy =
				FISHING_DATA_PTR->swimmer.pos.vy + d.vy;
			FISHING_DATA_PTR->swimmer.pos.vz =
				FISHING_DATA_PTR->swimmer.pos.vz + d.vz;
		}
		goto nibbleCheck;
	}
	goto releasedPath;

nibbleCheck:
	if (rod->itemTaken != 0) {
		if ((rod->framesOnState >= 0x1f) && ((rod->framesOnState % 20) == 0) &&
		    (customRandom(1, 0x2710) < 0x3e9)) {
			r = customRandom(0, 0x63) & 0xff;
			for (n = 0; n < 6U; n++) {
				if (FISH_BAIT_CHANCES[n].threshold >= r) {
					break;
				}
			}
			if (n >= 6U) {
				n--;
			}
			rod->bait = FISH_BAIT_CHANCES[n].item;
			goto stolen;
		}
	}
	goto visibleCheck;

releasedPath:
	if (((POLLED_INPUT_PREVIOUS & ~POLLED_INPUT) & 0x40) ||
	    ((TAMER_ENTITY.entity.anim.animFlag & 1) == 0)) {
		startAnimation(&TAMER_ENTITY.entity, 0x1f);
	}
	if (rod->itemTaken == 0) {
		j = rod->bait;
		f = FISH_findFishToBite(FISHING_DATA_PTR->fishPool.items,
		                        &FISHING_DATA_PTR->swimmer.pos, j);
		if (f != NULL) {
			FISH_hookFish(f);
			goto hooked;
		}
	}
	goto visibleCheck;

visibleCheck:
	if (FISH_projectPoint(&FISHING_DATA_PTR->swimmer.pos, &FISH_SCRATCH->x,
	                      &FISH_SCRATCH->y) == 0) {
		goto cast;
	}
	t = FISH_SCRATCH->x;
	w = FISH_SCRATCH->y;
	if (FISH_isScreenPointVisible(t, w) == 0) {
		goto cast;
	}
	t = FISH_SCRATCH->x;
	w = FISH_SCRATCH->y;
	if (FISHING_DATA_PTR->mapData->isWater(t, w) == 0) {
		goto cast;
	}
	if (FISH_isWithinCastRange(&FISHING_DATA_PTR->swimmer.pos, 0)) {
		goto cast;
	}
	if (POLLED_INPUT & 0x20) {
		FISH_panCameraWithDPad();
		goto tick;
	}
	FISH_panCameraTo(0x10, &FISHING_DATA_PTR->swimmer.pos, 0x28, 0x1e);
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x10) {
		goto cast;
	}
	goto tick;

hooked:
	rod->fishingState = 0xa;
	rod->framesOnState = -1;
	goto tick;

stateA:
	FISH_panCameraTo(0x10, &FISHING_DATA_PTR->swimmer.pos, 0x28, 0x1e);
	if ((FISHING_DATA_PTR->hooked.escaped != 0) ||
	    (FISHING_DATA_PTR->hooked.fish.active == 0)) {
		FISH_showTextBox(FISH_MSG_TOOK_BAIT, 0, 0, 0,
		                 &FISHING_DATA_PTR->swimmer.pos);
		goto splash;
	}
	j = POLLED_INPUT_PREVIOUS;
	t = POLLED_INPUT;
	w = ~j;
	p = t & w;
	if (p & 0x40) {
		if (FISH_tryStrike() == 0) {
			if (rod->itemTaken != 0) {
				FISH_showTextBox(FISH_MSG_TOO_LATE,
				                 FISH_MSG_GOT_BAIT, 0, 0,
				                 &FISHING_DATA_PTR->swimmer.pos);
				goto splash;
			}
			FISH_showTextBox(FISH_MSG_TOO_FAST,
			                 FISH_MSG_STILL_HAVE_BAIT, 0, 0,
			                 &FISHING_DATA_PTR->swimmer.pos);
			goto splash;
		}
		FISH_showTextBox(MAIN_D_801349D8, 0, 0, 0,
		                 &FISHING_DATA_PTR->swimmer.pos);
		goto beginTension;
	}
	if ((p & 0x10) == 0) {
		goto tick;
	}
	FISH_tickHookedFish(0xf);
	goto splash;

beginTension:
	FISH_beginTension();
	rod->pressedFrames = 0;
	rod->fishingState = 0xb;
	rod->framesOnState = -1;
	goto tick;

stateB:
	if (POLLED_INPUT & 0x40) {
		if (rod->pressedFrames < 0) {
			rod->pressedFrames = 0;
		}
		rod->pressedFrames++;
		if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40) ||
		    ((TAMER_ENTITY.entity.anim.animFlag & 1) == 0)) {
			startAnimation(&TAMER_ENTITY.entity, 0x22);
		}
		toEulerAngles(&rot3, rod->lineStart.vx - FISHING_DATA_PTR->swimmer.pos.vx, 0,
		              rod->lineStart.vz - FISHING_DATA_PTR->swimmer.pos.vz);
		d2.vx = rod->lineStart.vx - rod->lineStartMirror.vx;
		d2.vy = rod->lineStart.vy - rod->lineStartMirror.vy;
		d2.vz = rod->lineStart.vz - rod->lineStartMirror.vz;
		RotMatrixYXZ(&rot3, &m3);
		ApplyTransposeMatrixLV(&m3, &d2, &lv2);
		lv2.vx = 0;
		lv2.vy = 0;
		lv2.vz = (lv2.vz * 200) / 100;
		w = FISHING_DATA_PTR->hooked.fish.size;
		lv2.vz -= FISH_remapClamped(w, 0x32, 0x12c, 1, 0xa);
		if (lv2.vz > 0) {
			ApplyMatrixLV(&m3, &lv2, &d2);
			d2.vx /= 2;
			d2.vy /= 2;
			d2.vz /= 2;
			FISHING_DATA_PTR->swimmer.pos.vx =
				FISHING_DATA_PTR->swimmer.pos.vx + d2.vx;
			FISHING_DATA_PTR->swimmer.pos.vy =
				FISHING_DATA_PTR->swimmer.pos.vy + d2.vy;
			FISHING_DATA_PTR->swimmer.pos.vz =
				FISHING_DATA_PTR->swimmer.pos.vz + d2.vz;
		}
	} else {
		if (rod->pressedFrames > 0) {
			rod->pressedFrames = 0;
		}
		rod->pressedFrames--;
		if ((POLLED_INPUT_PREVIOUS & ~POLLED_INPUT) & 0x40) {
			startAnimation(&TAMER_ENTITY.entity, 0x21);
		}
	}

	if ((FISH_projectPoint(&FISHING_DATA_PTR->swimmer.pos, &FISH_SCRATCH->x,
	                       &FISH_SCRATCH->y) != 0) &&
	    (FISH_isScreenPointVisible(FISH_SCRATCH->x, FISH_SCRATCH->y) != 0) &&
	    (FISH_isScreenPointVisible(FISH_SCRATCH->x, FISH_SCRATCH->y) != 0) &&
	    (FISHING_DATA_PTR->mapData->isWater(FISH_SCRATCH->x, FISH_SCRATCH->y) == 0)) {
		goto landed;
	}
	t = FISHING_DATA_PTR->hooked.fish.size;
	if (FISH_isWithinCastRange(&FISHING_DATA_PTR->swimmer.pos, t)) {
		goto landed;
	}
	k = FISH_updateTension();
	if (k == 2) {
		goto tensionBroke;
	}
	if (k == 1) {
		FISH_tickHookedFish(0xf);
		FISH_resetTension();
		goto lineSnap;
	}
	goto noTension;

tensionBroke:
	FISH_tickHookedFish(0xf);
	FISH_resetTension();
	goto fishEscaped;

noTension:
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x10) {
		FISH_tickHookedFish(0xf);
		FISH_resetTension();
		goto splash;
	}
	if (POLLED_INPUT & 0x20) {
		FISH_panCameraWithDPad();
		goto tick;
	}
	FISH_panCameraTo(0x10, &FISHING_DATA_PTR->swimmer.pos, 0x28, 0x1e);
	goto tick;

landed:
	FISH_resetTension();
	if (FISHING_DATA_PTR->hooked.fish.kind == 6) {
		FISH_tickHookedFish(0x12);
		goto toState11AF;
	}
	FISH_tickHookedFish(0x11);
	rod->bait = FISHING_DATA_PTR->hooked.fish.kind + 0x3e;
	playSound(8, 7);
	FISH_showBaitSprite(&FISHING_DATA_PTR->swimmer.pos, rod->bait);
	FISH_spawnRipple(0, &FISHING_DATA_PTR->swimmer.pos, 1, 0,
	                 FISHING_DATA_PTR->mapData->unk8 * 10);
	FISHING_DATA_PTR->swimmer.mode = 0;
	rod->fishingState = 0xd;
	rod->framesOnState = -1;
	goto tick;

stateD:
	moving = FISH_moveBaitToRodTip(0x28);
	FISHING_DATA_PTR->swimmer.pos = FISHING_DATA_PTR->item.pos;
	k = FISH_panCameraTo(0x20, &FISHING_DATA_PTR->swimmer.pos, 0x28, 0x1e);
	if (moving != 0) {
		goto tick;
	}
	if (k != 0) {
		goto tick;
	}
	j = rod->bait;
	FISH_showBaitSprite(NULL, j);
	FISH_moveBaitToRodTip(0);
	if (FISHING_DATA_PTR->hooked.fish.size >= 0xc8) {
		metres = FISHING_DATA_PTR->hooked.fish.size / 200;
		centimetres = FISHING_DATA_PTR->hooked.fish.size % 200;
		sprintf(FISHING_DATA_PTR->messageBuffer, FISH_MSG_LENGTH_GREAT, metres,
		        centimetres);
	} else if (FISHING_DATA_PTR->hooked.fish.size >= 0x64) {
		metres = FISHING_DATA_PTR->hooked.fish.size / 100;
		centimetres = FISHING_DATA_PTR->hooked.fish.size % 100;
		sprintf(FISHING_DATA_PTR->messageBuffer, FISH_MSG_LENGTH_M_CM, metres,
		        centimetres);
	} else {
		centimetres = FISHING_DATA_PTR->hooked.fish.size % 100;
		sprintf(FISHING_DATA_PTR->messageBuffer, FISH_MSG_LENGTH_CM, centimetres);
	}
	if (MAIN_D_80154F80[FISHING_DATA_PTR->hooked.fish.kind] <
	    FISHING_DATA_PTR->hooked.fish.size) {
		MAIN_D_80154F80[FISHING_DATA_PTR->hooked.fish.kind] =
			FISHING_DATA_PTR->hooked.fish.size;
	}
	PARTNER_PARA.fishCaught++;
	playSound(8, 8);
	if (giveItem(rod->bait & 0xff, 1) == 0) {
		FISH_showTextBox(ITEM_PARA[rod->bait].name,
		                 FISHING_DATA_PTR->messageBuffer,
		                 FISH_MSG_GOT_ONE, FISH_MSG_LET_IT_GO,
		                 &FISHING_DATA_PTR->item.pos);
	} else {
		FISH_showTextBox(ITEM_PARA[rod->bait].name,
		                 FISHING_DATA_PTR->messageBuffer,
		                 FISH_MSG_GOT_ONE, 0,
		                 &FISHING_DATA_PTR->item.pos);
	}

finish:
	startAnimation(&TAMER_ENTITY.entity, 0x1c);
	FISH_bobberCommand(&FISHING_DATA_PTR->swimmer, 4);
	FISHING_DATA_PTR->swimmer.mode = 1;
	rod->displayLine = 1;
	if (FISHING_DATA_PTR->mapId == 8) {
		n = 0;
	} else if (FISHING_DATA_PTR->mapId == 0xa) {
		n = 0;
	} else {
		n = 0;
	}
	TAMER_ENTITY.entity.posData->rotation.vy = n;
	FISH_showBaitSprite(NULL, rod->bait);
	FISH_moveBaitToRodTip(0);

toStateE:
	rod->fishingState = 0xe;
	rod->framesOnState = -1;
	goto tick;

stateE:
	if (FISH_panCameraToTamer(0x10)) {
		goto tick;
	}
	FISH_endFishing(rod);
	goto tick;

lineSnap:
	playSound(8, 9);
	rod->displayLine = 0;
	FISH_bobberCommand(&FISHING_DATA_PTR->swimmer, 0xe);
	rod->fishingState = 0x4b0;
	rod->framesOnState = -1;
	goto tick;

state4B0:
	if (rod->framesOnState != 0xa) {
		goto tick;
	}
	playSound(8, 0xa);
	FISH_showTextBox(FISH_MSG_LINE_CUT, FISH_MSG_WATCH_TENSION,
	                 FISH_MSG_BE_CALM, 0, &FISHING_DATA_PTR->swimmer.pos);
	goto toStateE;

fishEscaped:
	playSound(8, 9);
	rod->displayLine = 1;
	FISH_bobberCommand(&FISHING_DATA_PTR->swimmer, 0xe);
	rod->fishingState = 0x4b1;
	rod->framesOnState = -1;
	goto tick;

state4B1:
	if (rod->framesOnState != 0xa) {
		goto tick;
	}
	playSound(8, 0xa);
	FISH_showTextBox(FISH_MSG_HOOK_LOST, FISH_MSG_WATCH_TENSION,
	                 FISH_MSG_BE_CALM, 0, &FISHING_DATA_PTR->swimmer.pos);
	goto splash;

stolen:
	playSound(8, 9);
	FISH_showTextBox(FISH_MSG_STUCK_LINE1, FISH_MSG_STUCK_LINE2, 0, 0,
	                 &FISHING_DATA_PTR->swimmer.pos);
	rod->fishingState = 0x44c;
	rod->framesOnState = -1;
	goto tick;

state44C:
	FISH_showBaitSprite(NULL, rod->bait);
	FISH_moveBaitToRodTip(0);
	playSound(8, 8);
	if (giveItem(rod->bait & 0xff, 1) == 0) {
		FISH_showTextBox(ITEM_PARA[rod->bait].name,
		                 FISH_MSG_BAG_FULL_LINE1, FISH_MSG_BAG_FULL_LINE2, 0,
		                 &FISHING_DATA_PTR->item.pos);
	} else {
		FISH_showTextBox(ITEM_PARA[rod->bait].name,
		                 FISH_MSG_IT_GOT_STUCK, 0, 0,
		                 &FISHING_DATA_PTR->item.pos);
	}
	goto finish;

toState11AF:
	rod->fishingState = 0x11af;
	rod->framesOnState = -1;
	goto tick;

state11AF:
	if (FISH_panCameraToTamer(0x10)) {
		goto tick;
	}
	FISH_SCRATCH->unk18 = getDistance(FISH_REEL_TARGET.vx -
	                                          FISHING_DATA_PTR->hooked.fish.pos.vx,
	                                  0,
	                                  FISH_REEL_TARGET.vz -
	                                          FISHING_DATA_PTR->hooked.fish.pos.vz);
	if (FISH_SCRATCH->unk18 >= 0x32) {
		goto tick;
	}
	FISHING_DATA_PTR->catchComplete = 1;

tick:
	rod->framesOnState++;
	FISH_tickBobberPhysics();
}

void FISH_tickFishingWorld(int32_t instanceId)
{
	if (FISHING_DATA_PTR->fishingDisabled != 0) {
		return;
	}

	if (PARTNER_STATE == 0xd) {
		return;
	}

	FISH_updateRodPose(&FISHING_DATA_PTR->rod);

	if (FISH_isTextBoxVisible() != 0) {
		return;
	}

	FISH_tickRipples();
	FISH_tickMarkers();
	FISH_tickFishPopulation();
	FISH_tickHookedFish(0);
}

void FISH_renderFishingWorld(int32_t instanceId)
{
	if (FISHING_DATA_PTR->fishingDisabled != 0) {
		return;
	}

	if (PARTNER_STATE == 0xd) {
		return;
	}

	FISH_drawFishPool();
	FISH_drawHookedFish();
	FISH_drawStub();
	FISH_drawRipples();
	FISH_drawMarkers();

	if (FISHING_DATA_PTR->rod.displayRod == 0) {
		return;
	}

	if (FISHING_DATA_PTR->rod.displaySwimmer != 0) {
		FISH_drawRodSegments(&FISHING_DATA_PTR->rod);
		FISH_bobberCommand(&FISHING_DATA_PTR->swimmer, 1);
	}

	if (FISHING_DATA_PTR->rod.displayLine != 0) {
		FISH_drawFishingLine(&FISHING_DATA_PTR->rod);
	}

	FISH_drawTensionGauge();
	FISH_drawBaitSprite();
}

int32_t FISH_lerpTableByX(int32_t x, int32_t base, DVECTOR *tbl, int32_t n)
{
	if ((x < tbl[0].vx) || (tbl[n - 1].vx < x)) {
		return 0;
	}
	for (n = n - 2; n >= 0; n--) {
		if (x >= tbl[n].vx) {
			break;
		}
	}

	{
		int32_t y0 = tbl[n].vy;
		int32_t x0 = tbl[n].vx;
		return base - (y0 + (((x - tbl[n].vx) * (tbl[n + 1].vy - y0)) / (tbl[n + 1].vx - x0)));
	}
}

int32_t FISH_lerpTableByY(int32_t base, int32_t y, DVECTOR *tbl, int32_t n)
{
	if ((y < tbl[0].vy) || (tbl[n - 1].vy < y)) {
		return 0;
	}
	for (n = n - 2; n >= 0; n--) {
		if (y >= tbl[n].vy) {
			break;
		}
	}

	{
		int32_t x0 = tbl[n].vx;
		int32_t y0 = tbl[n].vy;
		return base - (x0 + (((y - tbl[n].vy) * (tbl[n + 1].vx - x0)) / (tbl[n + 1].vy - y0)));
	}
}

int32_t FISH_isFishableWater1(int32_t x, int32_t y)
{
	if ((-DRAW_OFFSET_LIMIT_Y_MIN + 0xf0) < y) {
		return 0;
	}

	return FISH_lerpTableByX(x, y, FISH_D_8007A890, 0x10) >= 0;
}

void FISH_waterTestStub1(void)
{
}

int32_t FISH_isFishableWater2(int32_t x, int32_t y)
{
	if (x >= 0x1ad) {
		return 0;
	}

	if (y < 6) {
		return 1;
	}

	return FISH_lerpTableByY(x, y, FISH_D_8007A9B0, 7) <= 0;
}

void FISH_waterTestStub2(void)
{
}

void FISH_getBoneTransform(Entity *e, int32_t bone, VECTOR *outPos, SVECTOR *outRot)
{
	MATRIX m;
	long *t;

	if (!(bone < DIGIMON_DATA[e->type].boneCount)) {
		bone = 0;
	}

	RotMatrix(&e->posData->rotation, &e->posData->posMatrix.coord);
	ScaleMatrix(&e->posData->posMatrix.coord, &e->posData->scale);
	TransMatrix(&e->posData->posMatrix.coord, &e->posData->location);

	calculatePosition(&e->posData[bone].posMatrix, &m);

	if (outRot != NULL) {
		matrixToEuler2(&m, outRot);
	}

	if (outPos != NULL) {
		t = m.t;
		outPos->vx = t[0];
		outPos->vy = t[1];
		outPos->vz = t[2];
	}
}

void FISH_drawItemSprite(int32_t itemId, int32_t x, int32_t y, int32_t depth, int32_t unused)
{
	renderItemSprite(itemId, x, y, depth);
}

void FISH_endFishing(FishingRod *rod)
{
	if (rod->displayRod != 0) {
		FISH_applySavedCameraScroll();
		setCameraFollowPlayer();
		startAnimation(&TAMER_ENTITY.entity, 0);
		FISH_unloadModel(&FISHING_DATA_PTR->rodModel);
		rod->fishingState = 0;
		rod->displayLine = 0;
		rod->displayRod = 0;
	}

	FISH_closeBaitMenuIfOpen();
	FISH_shutdownTextBox();
	FISH_resetTension();
	FISH_initBaitSprite();
	FISH_initBobberAnim((FishRipple *)&FISHING_DATA_PTR->swimmer.pos);
}

void FISH_loadFishing(FishingRod *rod)
{
	RECT rect;
	int32_t t;

	if (rod->displayRod != 0) {
		FISH_endFishing(rod);
	}

	unsetCameraFollowPlayer();
	FISH_enableCameraControl();
	rod->fishingState = 2;
	rod->displayRod = 1;
	rod->displaySwimmer = 0;
	rod->displayLine = 0;
	FISH_clearBobberAnim((FishRipple *)&FISHING_DATA_PTR->swimmer.pos);
	FISH_tickHookedFish(0xf);
	FISH_loadTMDModel(&FISHING_DATA_PTR->rodModel, FISH_PATH_ROD_MODEL,
	                  FISHING_DATA_PTR->rodModelBuffer, 0x1000);
	rod->swimmerType = getBestFishingRod();
	t = rod->swimmerType;
	if (t != 1) {
		if (t != 2) {
			rod->swimmerType = 2;
		}
		setRECT(&rect, 0x1c0, 0x1e4, 0x1c, 8);
		MoveImage(&rect, 0x1c0, 0x1dc);
	}
}

void FISH_init(void)
{
	FISHING_DATA_PTR->fishingDisabled = 0;
	FISHING_DATA_PTR->catchComplete = 0;

	FISH_loadSpritesAndSeadramon(&FISHING_DATA_PTR->rodModel);
	FISH_initBaitMenu();
	FISH_initTextBox();
	FISH_resetTension();
	FISH_hideBaitSprite();
	FISH_resetBobberAnim((FishRipple *)&FISHING_DATA_PTR->swimmer.pos);
	FISH_initFishPool();
	FISH_resetRodState(&FISHING_DATA_PTR->rod);
	FISH_clearHookedFish();
	FISH_initRipplePool();
	FISH_initMarkerPool();

	addObject(0x3001, 0, (TickFunction)FISH_tickFishingWorld, (RenderFunction)FISH_renderFishingWorld);
}

void FISH_deinitialize(void)
{
	FISHING_DATA_PTR->fishingDisabled = 0;

	removeObject(0x3001, 0);

	FISH_endFishing(&FISHING_DATA_PTR->rod);
	FISH_initRipplePool();
	FISH_initMarkerPool();
	FISH_clearHookedFish();
	FISH_fishPoolStub();
	FISH_onRodModelLoaded(&FISHING_DATA_PTR->rodModel);
}

int32_t FISH_tamerTick(void)
{
	FISH_tickRod(&FISHING_DATA_PTR->rod);

	if (isFishing() == 0) {
		setTamerState(0);
		startGameTime();
		return 0;
	}

	if (FISHING_DATA_PTR->catchComplete != 0) {
		FISH_tickHookedFish(0x11);
		FISH_endFishing(&FISHING_DATA_PTR->rod);
		setTamerState(0);
		return 1;
	}

	return 0;
}

void FISH_scrollCameraTo(int32_t x, int32_t y)
{
	int32_t dx;
	int32_t dy;
	int32_t near;

	if (MAIN_D_801351DC[2] == 0) {
		return;
	}

	x = (x < (-DRAW_OFFSET_LIMIT_X_MAX + 0xa0)) ? -DRAW_OFFSET_LIMIT_X_MAX + 0xa0 : (((-DRAW_OFFSET_LIMIT_X_MIN + 0xa0) < x) ? -DRAW_OFFSET_LIMIT_X_MIN + 0xa0 : x);
	y = (y < (-DRAW_OFFSET_LIMIT_Y_MAX + 0x78)) ? -DRAW_OFFSET_LIMIT_Y_MAX + 0x78 : (((-DRAW_OFFSET_LIMIT_Y_MIN + 0x78) < y) ? -DRAW_OFFSET_LIMIT_Y_MIN + 0x78 : y);

	x -= (-DRAWING_OFFSET_X + 0xa0);
	y -= (-DRAWING_OFFSET_Y + 0x78);
	DRAWING_OFFSET_X -= x;
	DRAWING_OFFSET_Y -= y;
	CAMERA_X[0] += (int16_t)x;
	CAMERA_Y[0] += (int16_t)y;

	near = 0;
	if (((x > 0) ? x : -x) < 0x7f) {
		if (((y > 0) ? y : -y) < 0x7f) {
			near = 1;
		}
	}

	handleTileUpdate(((x < 0) ? 0x8000U : 0) |
	                         ((x > 0) ? 0x2000 : 0) |
	                         ((y < 0) ? 0x1000 : 0) | ((y > 0) ? 0x4000 : 0),
	                 (near != 0) ? 0 : 1);
}

void FISH_applySavedCameraScroll(void)
{
	if (MAIN_D_801351DC[2] != 0) {
		MAIN_D_801351DC[2] = 0;
		FISH_scrollCameraTo(MAIN_D_801351DC[0], MAIN_D_801351DC[1]);
	}
}

void FISH_enableCameraControl(void)
{
	FISH_applySavedCameraScroll();
	MAIN_D_801351DC[2] = 1;
	MAIN_D_801351DC[0] = -DRAWING_OFFSET_X + 0xa0;
	MAIN_D_801351DC[1] = -DRAWING_OFFSET_Y + 0x78;
}

void FISH_unloadModel(TMDFileLoadingData *model)
{
}
