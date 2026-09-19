#include <stdlib.h>

#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <mwinline_n.h>

#include <dw/anim.h>
#include <dw/btl.h>
#include <dw/entity.h>
#include <dw/file.h>
#include <dw/font.h>
#include <dw/kar.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/world_object.h>

#include "common.h"

extern int32_t ACTIVE_FRAMEBUFFER;
extern int32_t VIEWPORT_DISTANCE;
extern GsOT GS_ORDERING_TABLE[];
extern GsRVIEW2 GS_VIEWPOINT;

extern int8_t MAP_TILE_X;
extern int8_t MAP_TILE_Y;
extern int8_t MAP_WIDTH[];
extern int16_t CAMERA_Y[];
extern uint8_t MAP_TILE_DATA[];
extern char MAIN_D_80134A38[];
extern char MAIN_D_80134A3C[];

void renderSelectionCursor(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void renderUIBox(int32_t id);

void KAR_tickMatchState(void);
void KAR_renderAimArrow(void);
void KAR_updateRingMarkers(void);
void KAR_checkStonesStopped(void);
int32_t KAR_distance(int32_t x, int32_t y);
void KAR_debugCollisionStone(KarStone *stone, VECTOR *b, VECTOR *a);
void KAR_debugCollisionPair(KarStone *stoneA, VECTOR *a, KarStone *stoneB, VECTOR *b);
void KAR_reflectOffDiagonal(int32_t *p, int32_t dir);
void KAR_turnAimLeft(int32_t delta);
void KAR_turnAimRight(int32_t delta);
int32_t KAR_findUnusedStoneOfType(int32_t key);
void KAR_initializeOrderingTables(void);
int32_t KAR_tickYesNoPrompt(void);
void KAR_resolveStoneCollision(KarStone *stoneA, VECTOR a, KarStone *stoneB, VECTOR b);
void KAR_collideRestingStone(KarStone *resting, VECTOR a, KarStone *mover, VECTOR b);
void KAR_collideMovingStones(KarStone *stoneA, VECTOR a, KarStone *stoneB, VECTOR b);
int32_t KAR_tickHintBox(int32_t n);
int32_t KAR_computeSeparation(VECTOR *out, KarStone *stone, VECTOR a, VECTOR b);
void KAR_collidePeggedStone(KarStone *pegged, VECTOR a, KarStone *mover, VECTOR b);
void KAR_beginAiming(void);
void KAR_renderSprite(KarSprite *sp);
int32_t KAR_findWallContact(VECTOR *out, KarStone *stone, int32_t flag);
int32_t KAR_aimAtRandomStone(void);
int32_t KAR_computeImpactShare(KarStone *stone, VECTOR a, VECTOR b);
int32_t KAR_getWallZone(int16_t x, int16_t z);
void KAR_selectPreviousStone(void);
void KAR_selectNextStone(void);
void KAR_tickStones(int32_t instanceId);
void KAR_renderScene(int32_t instanceId);
void KAR_setupMatch(int32_t arg);
void KAR_renderStoneCursor(void);
void KAR_renderReadyPrompt(void);
void KAR_registerThrownStone(void);
int32_t KAR_chooseOpponentShot(void);
int32_t KAR_tickScoreTally(void);
void KAR_bounceOffWall(void);
void KAR_updateCollisions(void);
void KAR_classifyStoneRings(void);
void KAR_renderScores(void);
void KAR_finishMatch(void);
int32_t KAR_drawHintPagePenguinmon(int32_t idx, int8_t n);
int32_t KAR_drawHintPageMetalMamemon(int32_t idx, int8_t n);
void KAR_renderPowerMeter(void);
int32_t KAR_computeThrowPower(int32_t a, int32_t b, int32_t c);
void MAIN_func_800D8AC8(int32_t diffX, int32_t diffY);
void KAR_setOpponentShot(int32_t row, int32_t val, int32_t b, int32_t c);
void KAR_rotatePoint(SVECTOR *p, int32_t ang);
int32_t KAR_aimBankShot(KarStone *stone, int32_t x, int32_t z);
void KAR_placeAtContact(KarStone *stone, KarPos p);
void KAR_handleAimScroll(void);
int32_t KAR_findClearShotAngle(int32_t x, int32_t z);
int32_t KAR_aimAtStoneInRing(int32_t player, int32_t key, int16_t *outX, int16_t *outZ);
void KAR_renderNamePlates(void);
void KAR_beginThrow(void);

int32_t tickMoveCameraTo(int16_t x, int16_t z, int32_t speed);
void uploadMapTileImages();

static void *kar_functions[] = {
	KAR_renderSprite,
	KAR_rotatePoint,
	KAR_registerThrownStone,
	KAR_computeThrowPower,
	KAR_findUnusedStoneOfType,
	KAR_aimBankShot,
	KAR_aimAtRandomStone,
	KAR_setOpponentShot,
	KAR_aimAtStoneInRing,
	KAR_findClearShotAngle,
	KAR_drawHintPageMetalMamemon,
	KAR_drawHintPagePenguinmon,
	KAR_chooseOpponentShot,
	KAR_tickScoreTally,
	KAR_classifyStoneRings,
	KAR_tickHintBox,
	KAR_tickYesNoPrompt,
	KAR_beginThrow,
	KAR_turnAimRight,
	KAR_turnAimLeft,
	KAR_selectNextStone,
	KAR_selectPreviousStone,
	KAR_beginAiming,
	KAR_placeAtContact,
	KAR_reflectOffDiagonal,
	KAR_findWallContact,
	KAR_getWallZone,
	KAR_debugCollisionPair,
	KAR_computeSeparation,
	KAR_computeImpactShare,
	KAR_debugCollisionStone,
	KAR_collideMovingStones,
	KAR_collideRestingStone,
	KAR_collidePeggedStone,
	KAR_resolveStoneCollision,
	KAR_distance,
	KAR_bounceOffWall,
	KAR_updateCollisions,
	KAR_checkStonesStopped,
	KAR_updateRingMarkers,
	KAR_handleAimScroll,
	KAR_renderNamePlates,
	KAR_renderReadyPrompt,
	KAR_renderPowerMeter,
	KAR_renderScores,
	KAR_renderStoneCursor,
	KAR_renderAimArrow,
	KAR_tickMatchState,
	KAR_tick,
	KAR_finishMatch,
	KAR_renderScene,
	KAR_tickStones,
	KAR_start,
	KAR_setupMatch,
	KAR_initializeOrderingTables,
};

// clang-format off
KarModelIds KAR_D_8005AB80 = { { 0x00000002, 0x00000001, 0x00000000 } };

KarOffTbl KAR_D_8005AB8C = {
	{
		0x03, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00,
		0x00, 0x00,
	},
};

KarSpawnX KAR_D_8005AB98 = { { 0x000002c1, 0x000000eb, 0xffffff15 } };

char KAR_D_8005ABA4[20] = "\\ETCDAT\\KARRING.TMD";

char KAR_D_8005ABB8[24] = "Start GameExplain Game";

char KAR_D_8005ABD0[16] = " Penguinmon ";

char KAR_D_8005ABE0[28] = "First of all, please choose";

char KAR_D_8005ABFC[20] = "your curling stone.";

char KAR_D_8005AC10[32] = "There are heavy, medium, and";

char KAR_D_8005AC30[32] = "light ones, so think about the";

char KAR_D_8005AC50[24] = "best order to use them.";

char KAR_D_8005AC68[28] = "Some stones have four pegs.";

char KAR_D_8005AC84[32] = "They stick to where they land";

char KAR_D_8005ACA4[28] = "and can become very useful.";

char KAR_D_8005ACC0[28] = "Move the Directional Pad ";

char KAR_D_8005ACDC[32] = "left and right to choose the ";

char KAR_D_8005ACFC[28] = "direction of your throw.";

char KAR_D_8005AD18[28] = "Move screen up or down with";

char KAR_D_8005AD34[28] = "Dir. Pad. Use X button to";

char KAR_D_8005AD50[24] = "select throw direction.";

char KAR_D_8005AD68[28] = "Press X button when power";

char KAR_D_8005AD84[28] = "gauge reaches the desired";

char KAR_D_8005ADA0[32] = "level to pick throw strength.";

char KAR_D_8005ADC0[28] = "If your curling stone lands";

char KAR_D_8005ADDC[28] = "and touches the GOOD mark,";

char KAR_D_8005ADF8[20] = "you get two points.";

char KAR_D_8005AE0C[28] = "If you get it on the blue";

char KAR_D_8005AE28[32] = "line, but outside the center";

char KAR_D_8005AE48[24] = "GOOD mark, one point.";

char KAR_D_8005AE60[24] = "If you touch the BAD on";

char KAR_D_8005AE78[28] = "bottom right, you'll lose";

char KAR_D_8005AE94[28] = "two points, so be careful.";

char KAR_D_8005AEB0[28] = "And, in the case of a tie,";

char KAR_D_8005AECC[28] = "So let's start the game!!";

char KAR_D_8005AEE8[24] = "You're not that great!";

char KAR_D_8005AF00[32] = "The battle is just beginning!";

char KAR_D_8005AF20[24] = "Hey you're pretty good!";

char KAR_D_8005AF38[16] = "Yeah! I won!";

char KAR_D_8005AF48[16] = "Awh, I lost. ";

KarStrTbl KAR_D_8005AF58 = {{
	KAR_D_8005ABD0,
	KAR_D_8005ABE0,
	KAR_D_8005ABFC,
	MAIN_D_80134A38,
	KAR_D_8005ABD0,
	KAR_D_8005AC10,
	KAR_D_8005AC30,
	KAR_D_8005AC50,
	KAR_D_8005ABD0,
	KAR_D_8005AC68,
	KAR_D_8005AC84,
	KAR_D_8005ACA4,
	KAR_D_8005ABD0,
	KAR_D_8005ACC0,
	KAR_D_8005ACDC,
	KAR_D_8005ACFC,
	KAR_D_8005ABD0,
	KAR_D_8005AD18,
	KAR_D_8005AD34,
	KAR_D_8005AD50,
	KAR_D_8005ABD0,
	KAR_D_8005AD68,
	KAR_D_8005AD84,
	KAR_D_8005ADA0,
	KAR_D_8005ABD0,
	KAR_D_8005ADC0,
	KAR_D_8005ADDC,
	KAR_D_8005ADF8,
	KAR_D_8005ABD0,
	KAR_D_8005AE0C,
	KAR_D_8005AE28,
	KAR_D_8005AE48,
	KAR_D_8005ABD0,
	KAR_D_8005AE60,
	KAR_D_8005AE78,
	KAR_D_8005AE94,
	KAR_D_8005ABD0,
	KAR_D_8005AEB0,
	MAIN_D_80134A3C,
	KAR_D_8005AECC,
	KAR_D_8005ABD0,
	MAIN_D_80134A38,
	KAR_D_8005AEE8,
	MAIN_D_80134A38,
	KAR_D_8005ABD0,
	KAR_D_8005AF00,
	MAIN_D_80134A38,
	MAIN_D_80134A38,
	KAR_D_8005ABD0,
	KAR_D_8005AF20,
	MAIN_D_80134A38,
	MAIN_D_80134A38,
	KAR_D_8005ABD0,
	KAR_D_8005AF38,
	MAIN_D_80134A38,
	MAIN_D_80134A38,
	KAR_D_8005ABD0,
	KAR_D_8005AF48,
	MAIN_D_80134A38,
	MAIN_D_80134A38,
	(char *)0x00000000,
}};

KarOffTbl KAR_D_8005B04C = {
	{
		0x00, 0x0c, 0x14, 0x18, 0x28, 0x2c, 0x30, 0x34,
		0x38, 0x3c,
	},
};

char KAR_D_8005B058[16] = " MetalMamemon ";

char KAR_D_8005B068[28] = "Yeah. You gotta first pick";

char KAR_D_8005B084[28] = "the type of curling stone.";

char KAR_D_8005B0A0[28] = "I'm a genius, so I can use";

char KAR_D_8005B0BC[28] = "various stones in the right";

char KAR_D_8005B0D8[28] = "order. It's key to winning.";

char KAR_D_8005B0F4[28] = "Stones with four pegs stop";

char KAR_D_8005B110[28] = "and stay where they land,";

char KAR_D_8005B12C[28] = "so they are very valuable.";

char KAR_D_8005B148[32] = "Choose the direction of throw";

char KAR_D_8005B168[28] = "with the Directional Pad,";

char KAR_D_8005B184[32] = "by moving it left and right.";

char KAR_D_8005B1A4[32] = "You can scroll screen moving";

char KAR_D_8005B1C4[32] = "Directional Pad up and down.";

char KAR_D_8005B1E4[28] = "Press X button to throw it.";

char KAR_D_8005B200[28] = "gauge reaches the desired ";

char KAR_D_8005B21C[24] = "Points: If you get the";

char KAR_D_8005B234[24] = "stone on the GOOD mark,";

char KAR_D_8005B24C[28] = "If you get on right bottom";

char KAR_D_8005B268[28] = "BAD mark, it will be minus";

char KAR_D_8005B284[32] = "In the case of a tie, I win!";

char KAR_D_8005B2A4[20] = "Start the game!!";

char KAR_D_8005B2B8[12] = "I'll win!";

char KAR_D_8005B2C4[20] = "I haven't lost yet!";

char KAR_D_8005B2D8[28] = "I can't lose this match!";

char KAR_D_8005B2F4[16] = "Yeah, I won!";

char KAR_D_8005B304[20] = "Awh, I almost won.";

KarStrTbl KAR_D_8005B318 = {{
	KAR_D_8005B058,
	KAR_D_8005B068,
	KAR_D_8005B084,
	&MAIN_D_80134A3C[7],
	KAR_D_8005B058,
	KAR_D_8005B0A0,
	KAR_D_8005B0BC,
	KAR_D_8005B0D8,
	KAR_D_8005B058,
	KAR_D_8005B0F4,
	KAR_D_8005B110,
	KAR_D_8005B12C,
	KAR_D_8005B058,
	KAR_D_8005B148,
	KAR_D_8005B168,
	KAR_D_8005B184,
	KAR_D_8005B058,
	KAR_D_8005B1A4,
	KAR_D_8005B1C4,
	KAR_D_8005B1E4,
	KAR_D_8005B058,
	KAR_D_8005AD68,
	KAR_D_8005B200,
	KAR_D_8005ADA0,
	KAR_D_8005B058,
	KAR_D_8005B21C,
	KAR_D_8005B234,
	KAR_D_8005ADF8,
	KAR_D_8005B058,
	KAR_D_8005AE0C,
	KAR_D_8005AE28,
	KAR_D_8005AE48,
	KAR_D_8005B058,
	KAR_D_8005B24C,
	KAR_D_8005B268,
	KAR_D_8005AE94,
	KAR_D_8005B058,
	KAR_D_8005B284,
	KAR_D_8005B2A4,
	&MAIN_D_80134A3C[7],
	KAR_D_8005B058,
	KAR_D_8005B2B8,
	&MAIN_D_80134A3C[7],
	&MAIN_D_80134A3C[7],
	KAR_D_8005B058,
	KAR_D_8005B2C4,
	&MAIN_D_80134A3C[7],
	&MAIN_D_80134A3C[7],
	KAR_D_8005B058,
	KAR_D_8005B2D8,
	&MAIN_D_80134A3C[7],
	&MAIN_D_80134A3C[7],
	KAR_D_8005B058,
	KAR_D_8005B2F4,
	&MAIN_D_80134A3C[7],
	&MAIN_D_80134A3C[7],
	KAR_D_8005B058,
	KAR_D_8005B304,
	&MAIN_D_80134A3C[7],
	&MAIN_D_80134A3C[7],
	&MAIN_D_80134A3C[7],
}};

KarOffTbl KAR_D_8005B40C = {
	{
		0x00, 0x0c, 0x14, 0x18, 0x28, 0x2c, 0x30, 0x34,
		0x38, 0x3c,
	},
};

KarShotPlan KAR_D_8005B418 = {
	{
		0x00000001, 0x00000000, 0x00000002, 0x00000003,
		0x00000003, 0x00000002, 0x00000000, 0x00000001,
	},
};

KarZones KAR_D_8005B438 = {
	{
		{ 0x0000, 0x06b6, 0x0091 },
		{ 0x0000, 0x06b6, 0x0177 },
		{ 0xfe70, 0x090e, 0x00cd },
		{ 0x0190, 0x090e, 0x00cd },
	},
};

int16_t KAR_D_8005B450[8] = {
	0x0019, 0x0001, 0x0013, 0x0004, 0x0010, 0x0003, 0x000d, 0x0003,
};

int16_t KAR_D_8005B460[12] = {
	0x0000, 0x06b6, 0x0091, 0x0000, 0x06b6, 0x0177, 0xfe70, 0x090e,
	0x00cd, 0x0190, 0x090e, 0x00cd,
};

KarSprite KAR_D_8005B478 = {
	0xff6a,
	0x0000,
	0x0080,
	0x01e6,
	0x0000,
	0x0028,
	0x0018,
	0x0018,
	0x80,
	0x80,
	0x80,
	0x00,
};

KarSprite KAR_D_8005B48C = {
	0xff6a,
	0x0000,
	0x0010,
	0x01e6,
	0x0000,
	0x0028,
	0x0010,
	0x0010,
	0x80,
	0x80,
	0x80,
	0x00,
};

KarDigits KAR_D_8005B4A0 = {
	{
		{ 0x0040, 0x0018 },
		{ 0x0050, 0x0018 },
		{ 0x0060, 0x0018 },
		{ 0x0040, 0x0028 },
		{ 0x0050, 0x0028 },
		{ 0x0060, 0x0028 },
		{ 0x0048, 0x0038 },
		{ 0x0058, 0x0038 },
		{ 0x0048, 0x0048 },
		{ 0x0058, 0x0048 },
		{ 0x0000, 0x0040 },
	},
};

KarSpritePair KAR_D_8005B4CC = {
	{
		{
			0xffce,
			0xffce,
			0x0080,
			0x01e6,
			0x0030,
			0x0018,
			0x0010,
			0x0010,
			0x80,
			0x80,
			0x80,
			0x00,
		},
		{
			0xffce,
			0xffce,
			0x0070,
			0x01e6,
			0x0000,
			0x0018,
			0x0030,
			0x0010,
			0x80,
			0x80,
			0x80,
			0x00,
		},
	},
};

KarSpriteSet KAR_D_8005B4F4 = {
	{
		{
			0xff74,
			0xffa1,
			0x0090,
			0x01e6,
			0x0070,
			0x0018,
			0x0020,
			0x0020,
			0x80,
			0x80,
			0x80,
			0x00,
		},
		{
			0xff7e,
			0xffba,
			0x0070,
			0x01e6,
			0x0018,
			0x0038,
			0x0030,
			0x0020,
			0x80,
			0x80,
			0x80,
			0x00,
		},
		{
			0x0064,
			0xffa1,
			0x00a0,
			0x01e6,
			0x0090,
			0x0018,
			0x0020,
			0x0020,
			0x80,
			0x80,
			0x80,
			0x00,
		},
		{
			0x0050,
			0xffba,
			0x0080,
			0x01e6,
			0x0018,
			0x0038,
			0x0030,
			0x0020,
			0x80,
			0x80,
			0x80,
			0x00,
		},
		{
			0x0000,
			0x0000,
			0x0000,
			0x0000,
			0x0000,
			0x0000,
			0x0000,
			0x0000,
			0x00,
			0x00,
			0x00,
			0x00,
		},
	},
};

KarSprite KAR_D_8005B558 = {
	0x0064,
	0xffa1,
	0x00c0,
	0x01e6,
	0x00b0,
	0x0018,
	0x0020,
	0x0020,
	0x80,
	0x80,
	0x80,
	0x00,
};

KarSprite KAR_D_8005B56C = {
	0xffcc,
	0x0000,
	0x00b0,
	0x01e6,
	0x0068,
	0x003e,
	0x0097,
	0x000a,
	0x80,
	0x80,
	0x80,
	0x00,
};

GsRVIEW2 KAR_D_8005B580 = {
	0x00000000,
	0x00000000,
	0xfffff308,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	NULL,
};
// clang-format on

static void kar__garbage__(void)
{
	int32_t t0 = KAR_D_800638CC[0].length;
	int32_t t1 = KAR_D_800638CC[0].offset;
	int32_t t2 = KAR_D_800638CC[0].point;

	KAR_D_800638CC[0].length = t0 + t1 + t2;
}

void KAR_initializeOrderingTables(void)
{
	GsOT_TAG *tags0 = KAR_D_800637CC;
	GsOT_TAG *tags1 = KAR_D_8006384C;

	KAR_D_800638CC[0].length = 5;
	KAR_D_800638CC[0].org = tags0;
	KAR_D_800638CC[1].length = 5;
	KAR_D_800638CC[1].org = tags1;
}

void KAR_setupMatch(int32_t mode)
{
	int32_t obstacles;
	KarModelIds models;
	KarWeightTbl weights;
	KarOffTbl types;
	KarSpawnX spawnX;
	int32_t p;
	int32_t i;
	int32_t n;
	KarStone *stone;

	obstacles = rand() % 3;
	MAIN_D_8013523A = 0;
	MAIN_D_8013523C = 0;
	MAIN_D_8013523E = 0;

	for (p = 0; p < 3; p++) {
		stone = KAR_D_8005B5A0[p].row.stones;
		rand();
		KAR_D_8005B5A0[p].row.score = 0;
		KAR_D_8005B5A0[p].row.thrown = 0;
		KAR_D_8005B5A0[p].row.unk2 = 0;
		KAR_D_8005B5A0[p].row.unk4 = 0;

		for (i = 0; i < 5;) {
			models = KAR_D_8005AB80;
			weights = MAIN_D_80134A08;
			types = KAR_D_8005AB8C;

			n = rand();
			n = n % 10;
			stone->speed = 0;
			stone->angle = 0;
			stone->ring = 0;
			stone->prevRing = 0;
			if (mode == 0) {
				stone->type = types.start[n];
			} else {
				stone->type = i;
				if (stone->type >= 4) {
					stone->type = 3;
				}
			}
			GsLinkObject4((u_long)(MAIN_D_80135240 + 3), &stone->obj, stone->type * 3 + models.id[p]);
			GsInitCoordinate2(NULL, &stone->coord);
			stone->obj.attribute = 0;
			stone->obj.coord2 = &stone->coord;
			if (p < 2) {
				stone->pos.vy = i * 0xaa - 0x44c;
				stone->pos.vx = (p != 0) ? 0x226 : -0x384 - i * 0x1e;
				stone->pos.vz = 0;
				stone->pos.vy = i * 0xaa - 0x50;
				stone->pos.vx = (p != 0) ? 0x2bc : -0x2bc;
				stone->pos.vz = 0;
				stone->unk94 = 0x96;
				stone->selectPhase = 0;
				stone->unk98 = 0;
				stone->state = -1;
			} else if (i < obstacles) {
				spawnX = KAR_D_8005AB98;
				stone->pos.vy = 0;
				stone->pos.vx = spawnX.x[i] - rand() % 0x13f;
				stone->pos.vz = -(rand() % 0x3e8 + 0x3e8);
				stone->unk94 = 0;
				stone->selectPhase = 0;
				stone->unk98 = 0;
				stone->state = 1;
				stone->speed = 0;
			} else {
				stone->pos.vy = 0;
				stone->pos.vx = 0;
				stone->pos.vz = 0;
				stone->unk94 = 0;
				stone->selectPhase = 0;
				stone->unk98 = 0;
				stone->state = -0x65;
				stone->speed = 0;
			}
			stone->weight = weights.weight[stone->type];
			i++;
			stone++;
		}
	}
}

void KAR_start(void)
{
	MAIN_D_80135240 = KAR_D_8005BFCC;
	SOME_SCRIPT_SYNC_BIT = 0;
	readFile(KAR_D_8005ABA4, MAIN_D_80135240);
	GsMapModelingData(MAIN_D_80135240 + 1);
	KAR_initializeOrderingTables();
	ENTITY_TABLE[0]->isOnMap = 1;
	MAIN_D_80135244 = 0;
	KAR_setupMatch(0);
	addObject(0x1388, 0xf, KAR_tickStones, KAR_renderScene);
	MAIN_D_80135248 = readPStat(0x7a);
	MAIN_D_80135248 = (uint32_t)MAIN_D_80135248 + 2;
}

void KAR_tickStones(int32_t instanceId)
{
	KarPeggedModelIds models;
	KarStone *stone;
	int32_t p;
	int32_t i;
	int16_t move;
	int16_t step;

	for (p = 0; p < 3; p++) {
		stone = KAR_D_8005B5A0[p].row.stones;
		for (i = 0; i < 5; i++, stone++) {
			if (stone->state != 3 && stone->state != 2) {
				stone->target = stone->pos;
			}
			if (MAIN_D_80135244 == 5) {
				if (i == MAIN_D_8013523A && p == MAIN_D_8013523C) {
					stone->selectPhase = stone->selectPhase + 0x14;
				} else {
					stone->selectPhase = 0;
				}
			}
			if (stone->state > 0 && stone->speed >= 0) {
				move = stone->speed / stone->weight;
				stone->speed = (uint32_t)(stone->speed * 98) / 100;
				if (move < 2) {
					stone->speed = 0;
				}
				if (stone->state < 3) {
					step = move;
					stone->pos.vx = stone->pos.vx + move * rcos(stone->angle) / 4096;
					stone->pos.vz = stone->pos.vz + step * rsin(stone->angle) / 4096;
				}
				if (stone->state == 3) {
					if (stone->speed > 0) {
						step = move / 10;
						if ((stone->angle = stone->angle - 0x800) < 0) {
							stone->angle = stone->angle + 0x1000;
						}
						stone->pos.vx = stone->target.vx + step * rcos(stone->angle) / 4096;
						stone->pos.vz = stone->target.vz + step * rsin(stone->angle) / 4096;
						if (step == 0) {
							stone->speed = 0;
						}
					} else {
						stone->speed = 0;
						stone->state = 2;
					}
				}
				if (stone->state == 1 && move <= 0 && stone->type == 3) {
					models = MAIN_D_80134A4C;
					stone->state = 2;
					GsLinkObject4((u_long)(MAIN_D_80135240 + 3), &stone->obj, models.id[p]);
					playSound2(8, 4);
				}
			}
		}
	}

	KAR_updateCollisions();
}

void KAR_renderScene(int32_t instanceId)
{
	MATRIX m;
	KarStone *stone;
	int32_t p;
	int32_t i;
	int32_t state;
	int8_t idx;

	GsClearOt(0, 0xa, &KAR_D_800638CC[ACTIVE_FRAMEBUFFER]);

	for (p = 0; p < 3; p++) {
		stone = KAR_D_8005B5A0[p].row.stones;

		for (i = 0; i < 5;) {
			if ((p == 1) && (MAIN_D_80135244 != 3) && (MAIN_D_80135244 < 0x10)) {
				if (MAIN_D_80135250 != 0) {
					goto next;
				}
				if (MAIN_D_80135244 < 5) {
					goto next;
				}
			}
			if (MAIN_D_80135244 < 4) {
				if (MAIN_D_80135252 < i * 0x3c + 0x3c) {
					goto next;
				}
			}

			if (stone->state == 0) {
				idx = (MAIN_D_8013523C != 0) ? MAIN_D_80135248 : 0;
				if (idx == 2) {
					stone->pos = ENTITY_TABLE[idx]->posData->location;
					stone->pos.vx = stone->pos.vx - 0x50;
				} else if (idx == 0) {
					stone->pos.vx = ENTITY_TABLE[idx]->posData[9].posMatrix.workm.t[0];
					stone->pos.vy = ENTITY_TABLE[idx]->posData[9].posMatrix.workm.t[1] + 0x78;
					stone->pos.vz = ENTITY_TABLE[idx]->posData[9].posMatrix.workm.t[2];
					stone->unk98 = 0;
					stone->selectPhase = 0;
					stone->unk94 = 0;
				} else {
					stone->pos.vx = ENTITY_TABLE[idx]->posData[4].posMatrix.workm.t[0];
					stone->pos.vy = ENTITY_TABLE[idx]->posData[4].posMatrix.workm.t[1] + 0x78;
					stone->pos.vz = ENTITY_TABLE[idx]->posData[4].posMatrix.workm.t[2];
					stone->unk98 = 0;
					stone->selectPhase = 0;
					stone->unk94 = 0;
				}
			}

			RotMatrix((SVECTOR *)&stone->unk94, &stone->coord.coord);
			TransMatrix(&stone->coord.coord, &stone->pos);
			stone->coord.flg = 0;

			if (stone->state < 0) {
				GsSetProjection(0x200);
				GsSetRefView2(&KAR_D_8005B580);
			} else {
				GsSetProjection(VIEWPORT_DISTANCE);
				GsSetRefView2(&GS_VIEWPOINT);
			}

			GsGetLw(stone->obj.coord2, &m);
			GsSetLightMatrix(&m);
			GsGetLs(stone->obj.coord2, &m);
			GsSetLsMatrix(&m);

			state = stone->state;
			if (state < 0) {
				if (stone->state < -0x64) {
					goto next;
				}
				GsSortObject4(&stone->obj, &KAR_D_800638CC[ACTIVE_FRAMEBUFFER], 5, getScratchAddr(0));
			} else {
				if (stone->state >= 0x65) {
					goto next;
				}
				GsSortObject4(&stone->obj, &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER], 2, getScratchAddr(0));
			}
next:
			i++;
			stone++;
		}
	}

	GsSortOt(&KAR_D_800638CC[ACTIVE_FRAMEBUFFER], &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void KAR_finishMatch(void)
{
	int8_t v;

	if (MAIN_D_80135244 == 0x14) {
		return;
	}

	MAIN_D_80135244 = 0x14;
	ENTITY_TABLE[MAIN_D_80135248]->posData->rotation.vy = 0x800;
	ENTITY_TABLE[1]->posData->rotation.vy = 0x800;
	ENTITY_TABLE[0]->posData->rotation.vy = 0x800;
	startAnimation(ENTITY_TABLE[0], 2);
	startAnimation(ENTITY_TABLE[1], 2);
	startAnimation(ENTITY_TABLE[MAIN_D_80135248], 2);
	SOME_SCRIPT_SYNC_BIT = 1;
	removeObject(0x1388, 0xf);
	setTextColor(1);

	v = KAR_D_8005B5A0[0].row.score;
	if (v > KAR_D_8005B5A0[1].row.score) {
		if (KAR_D_8005B5A0[0].row.score >= 0xa) {
			writePStat(0x79, 2);
		} else {
			writePStat(0x79, 1);
		}
	} else {
		writePStat(0x79, 0);
	}
}

void KAR_tick(void)
{
	KAR_tickMatchState();
	KAR_renderAimArrow();
	KAR_renderStoneCursor();
	KAR_renderScores();
	KAR_renderPowerMeter();
	KAR_renderReadyPrompt();
	KAR_renderNamePlates();
	KAR_handleAimScroll();
	KAR_updateRingMarkers();
	KAR_checkStonesStopped();
}

INCLUDE_ASM("asm/kar/nonmatchings/kar", KAR_tickMatchState);

void KAR_renderAimArrow(void)
{
	MATRIX m;
	SVECTOR out;
	SVECTOR prev;
	SVECTOR vec;
	int32_t otz;
	DVECTOR sxy;
	SVECTOR pts[4];
	POLY_FT4 *prim;
	GsOT_TAG *ot;
	int32_t i;
	int32_t j;
	int32_t angle;
	int32_t rot;
	int32_t shade;
	int32_t k;
	int8_t player;
	uint32_t tmp;
	int16_t w;

	player = (MAIN_D_8013523C != 0) ? MAIN_D_80135248 : 0;

	if (MAIN_D_80135244 < 6) {
		return;
	}
	if (MAIN_D_80135244 >= 8) {
		return;
	}

	ot = ACTIVE_ORDERING_TABLE->org;

	if (MAIN_D_80135244 >= 2) {
		if (MAIN_D_80135252-- != 0) {
			MAIN_D_80135252 += 0xbf;
		}
		if (MAIN_D_80135238++ >= 0x28) {
			MAIN_D_80135238 = 0;
		}
	} else {
		MAIN_D_80135238 = 0;
	}

	for (i = 0, j = 1, angle = 0; i < 0xf; i++, angle += 0xbf, j++) {
		j = j;
		vec.vx = 0;
		vec.vy = 0;
		vec.vz = j * -200;
		RotMatrix(&ENTITY_TABLE[player]->posData->rotation, &m);
		ApplyMatrixSV(&m, &vec, &out);
		out.vx += KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].pos.vx;
		out.vz += KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].pos.vz;
		tmp = (MAIN_D_80135252 + (angle / 15)) & 0xff;
		shade = tmp;
		if (tmp >= 0xbf) {
			shade = (shade - 0xbfU) & 0xff;
		}
		while ((out.vx < -0x2ee) || (out.vx >= 0x2ef)) {
			if (out.vx < -0x2ee) {
				w = out.vx;
				if (!otz) {
				}
				w = w + 0x2ee;
				out.vx = out.vx - (int16_t)(w * 2);
			}
			if (out.vx >= 0x2ef) {
				w = out.vx;
				if (!otz) {
				}
				w = w - 0x2ee;
				w = w * 2;
				out.vx = out.vx - w;
			}
		}
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		GsSetLsMatrix(&GsWSMATRIX);
		setRGB0(prim, shade + 0x40, shade + 0x40, shade + 0x40);
		gte_ldv0(&out);
		gte_rtps();
		gte_stsxy(&sxy);
		gte_stszotz(&otz);
		if (i < 0xe) {
			setUVWH(prim, 0x18, 0x38, 16, -16);
			setXY4(prim, sxy.vx - 8, sxy.vy + 8, sxy.vx + 8, sxy.vy + 8, sxy.vx - 8,
			       sxy.vy - 8, sxy.vx + 8, sxy.vy - 8);
		} else {
			rot = ratan2(prev.vz - out.vz, prev.vx - out.vx);
			rot = rot - 0xc00;
			rot = rot % 0x1000;
			pts[0].vx = -8;
			pts[0].vy = 0;
			pts[0].vz = 8;
			pts[1].vx = 8;
			pts[1].vy = 0;
			pts[1].vz = 8;
			pts[2].vx = -8;
			pts[2].vy = 0;
			pts[2].vz = -8;
			pts[3].vx = 8;
			pts[3].vy = 0;
			rot = -rot;
			pts[3].vz = -8;
			for (k = 0; k < 4; k++) {
				KAR_rotatePoint(&pts[k], rot);
			}
			for (k = 0; k < 4; k++) {
				pts[k].vx += sxy.vx;
				pts[k].vz += sxy.vy;
			}
			if (rot == 0x800) {
				setUVWH(prim, 0x28, 0x28, 16, 16);
			} else {
				setUVWH(prim, 0x28, 0x28, 15, 15);
			}
			setXY4(prim, pts[0].vx, pts[0].vz, pts[1].vx, pts[1].vz, pts[2].vx,
			       pts[2].vz, pts[3].vx, pts[3].vz);
		}
		setClut(prim, 0x80, 0x1e6);
		prim->tpage = getTPage(0, 0, 640, 0);
		if (i != (MAIN_D_80135238 / 3)) {
			AddPrim(ot + otz, prim);
		}
		prim++;
		GsSetWorkBase((PACKET *)prim);
		prev = out;
	}
}

void KAR_renderStoneCursor(void)
{
	KarSprite sprite;

	sprite = KAR_D_8005B478;
	if (MAIN_D_80135244 == 5) {
		sprite.x = (MAIN_D_8013523C != 0) ? 0x64 : -0x78;
		sprite.y = (MAIN_D_8013523A * 30) - 0x2d;
		KAR_renderSprite(&sprite);
	}
}

void KAR_renderScores(void)
{
	KarSprite spr;
	KarDigits dig;
	int32_t i;
	int32_t row;
	int8_t score;
	int32_t scoreW;
	int32_t scoreSign;
	int16_t lo;
	int16_t hi;
	int16_t x;

	spr = KAR_D_8005B48C;
	dig = KAR_D_8005B4A0;
	for (i = 0, row = 0; i < 2; i++, row += 0x364) {
		score = (scoreW = ((int8_t *)KAR_D_8005B5A0)[row]);
		lo = score % 10;
		hi = score / 10;
		scoreSign = scoreW;
		x = (i != 0) ? 0x68 : -0x69;
		if (scoreSign < 0) {
			hi = 10;
		}
		if (lo < 0) {
			lo = -lo;
		}
		spr.u = dig.glyph[lo].u;
		spr.v = dig.glyph[lo].v;
		spr.x = x;
		spr.y = -0x3e;
		KAR_renderSprite(&spr);

		spr.u = dig.glyph[hi].u;
		spr.v = dig.glyph[hi].v;
		spr.x = x - 0x10;
		spr.y = -0x3e;
		KAR_renderSprite(&spr);
	}
}

void KAR_renderPowerMeter(void)
{
	KarSpritePair arr;
	int32_t i;
	int32_t off;
	int32_t thr;

	arr = KAR_D_8005B4CC;
	if ((MAIN_D_80135244 >= 8) && (MAIN_D_80135244 < 0xa)) {
		arr.sprite[1].x = (MAIN_D_8013523C != 0) ? 0x32 : -0x32;
		arr.sprite[1].clutX = (MAIN_D_8013523C != 0) ? 0x80 : 0x70;

		i = 10;
		thr = 0x9c4;
		off = 0x1e;
		while (i > 0) {
			if (MAIN_D_8013523E >= thr) {
				arr.sprite[0].x = (arr.sprite[1].x + off) - 3;
				if (i == 10) {
					arr.sprite[0].clutX = 0x30;
				} else {
					arr.sprite[0].clutX = 0x70;
				}
				KAR_renderSprite(&arr.sprite[0]);
			}
			i--;
			off -= 3;
			thr -= 0xfa;
		}
		KAR_renderSprite(&arr.sprite[1]);
	}
}

void KAR_renderReadyPrompt(void)
{
	KarSprite sprite;
	uint32_t t;

	sprite = KAR_D_8005B56C;
	if (MAIN_D_80135244 == 3) {
		t = MAIN_D_80135220;
		MAIN_D_80135220 = t + 1;
		if (t >= 0x31) {
			MAIN_D_80135220 = 0x30;
		}
		if ((MAIN_D_80135220 & 8) != 0) {
			KAR_renderSprite(&sprite);
		}
	} else {
		MAIN_D_80135220 = 0;
	}
}

void KAR_renderNamePlates(void)
{
	KarSpriteSet set;
	KarSprite extra;
	KarSprite *p;
	int32_t i;

	set = KAR_D_8005B4F4;
	extra = KAR_D_8005B558;
	p = set.sprite;
	if (MAIN_D_80135248 == 3) {
		p[2] = extra;
	}

	for (i = 0; i < 4; i++, p++) {
		KAR_renderSprite(p);
	}
}

void KAR_handleAimScroll(void)
{
	int16_t dx;
	int16_t dy;

	if (MAIN_D_80135244 != 6) {
		return;
	}
	dx = 0;
	dy = 0;
	if (MAIN_D_8013523C != 0) {
		return;
	}
	if ((MAIN_D_80135254 >= 10) || (MAIN_D_80135254 < -9) ||
	    (MAIN_D_80135250 != 0)) {
		if (POLLED_INPUT & 0x8000) {
			dx -= 8;
		}
		if (POLLED_INPUT & 0x2000) {
			dx += 8;
		}
	}
	if (POLLED_INPUT & 0x1000) {
		dy -= 10;
	}
	if (POLLED_INPUT & 0x4000) {
		dy += 10;
	}
	if ((dx != 0) || (dy != 0)) {
		MAIN_func_800D8AC8(dx, dy);
	}
}

INCLUDE_ASM("asm/kar/nonmatchings/kar", KAR_updateRingMarkers);

void KAR_checkStonesStopped(void)
{
	int32_t p;
	int32_t n;
	int32_t moving;
	KarStone *stone;
	KarStone *stone2;

	moving = 0;
	if (MAIN_D_80135244 != 0xb) {
		return;
	}
	for (p = 0; p < 3; p++) {
		p = p;
		stone = KAR_D_8005B5A0[p].row.stones;
		for (n = 0; n < 5; n++, stone++) {
			if (stone->state > 0) {
				if (stone->speed > 0) {
					moving = 1;
					break;
				}
				stone->speed = 0;
			}
		}
	}
	if (moving == 0) {
		MAIN_D_80135244 = 0xc;
	}
	for (p = 0; p < 3; p++) {
		stone2 = KAR_D_8005B5A0[p].row.stones;
		for (n = 0; n < 5; n++, stone2++) {
			if (stone2->state > 0) {
				if (stone2->pos.vz >= 0xbe) {
					if (stone2->speed <= 0) {
						stone2->state = -0x65;
					}
				}
			}
		}
	}
}

void KAR_updateCollisions(void)
{
	int32_t collided;
	int32_t next;
	VECTOR cur[15];
	VECTOR prev[15];
	int32_t step;
	int32_t i;
	int32_t p;
	int32_t a;
	int32_t b;
	KarStone *stone;
	int32_t tz1;
	int32_t tz2;
	int32_t tx1;
	int32_t tx2;

	collided = 0;

	for (i = 0; i < 15; i++) {
		prev[i].vy = 0;
	}

	KAR_bounceOffWall();

	for (i = 0; i < 15; i++) {
		cur[i].vy = 0;
	}

	if (MAIN_D_80135244 != 0xb) {
		return;
	}

	for (step = 0; step < 0xb; step++) {
		for (p = 0; p < 3; p++) {
			stone = KAR_D_8005B5A0[p].row.stones;

			for (i = 0; i < 5;) {
				if (stone->state > 0) {
					tz1 = stone->target.vz * (10 - step);
					tz2 = stone->pos.vz * step;
					tx1 = stone->target.vx * (10 - step);
					tx2 = stone->pos.vx * step;
					cur[p * 5 + i].vx = (tx1 + tx2) / 10;
					cur[p * 5 + i].vz = (tz1 + tz2) / 10;
					cur[p * 5 + i].vy = stone->target.vy;
				}
				i++;
				stone++;
			}
		}

		for (a = 0, next = 1; a < 15; a++, next++) {
			if (KAR_D_8005B5A0[a / 5].row.stones[a % 5].state > 0) {
				for (b = next; b < 15; b++) {
					if (KAR_D_8005B5A0[b / 5].row.stones[b % 5].state > 0) {
						if (KAR_distance(cur[a].vx - cur[b].vx, cur[a].vz - cur[b].vz) < 0x96) {
							collided = 1;
							if (step == 0) {
								KAR_resolveStoneCollision(&KAR_D_8005B5A0[b / 5].row.stones[b % 5], cur[b],
								                          &KAR_D_8005B5A0[a / 5].row.stones[a % 5], cur[a]);
							} else {
								KAR_resolveStoneCollision(&KAR_D_8005B5A0[b / 5].row.stones[b % 5], prev[b],
								                          &KAR_D_8005B5A0[a / 5].row.stones[a % 5], prev[a]);
							}
						}
					}
				}
			}
		}

		for (i = 0; i < 15; i++) {
			prev[i] = cur[i];
		}

		if ((collided != 0) && (step == 0xa)) {
			step--;
		}
		collided = 0;
	}
}

void KAR_bounceOffWall(void)
{
	KarPos contact;
	KarStoneRow *row;
	KarStone *stone;
	int32_t p;
	int32_t n;
	int16_t zone;

	for (p = 0; p < 3; p++) {
		stone = KAR_D_8005B5A0[p].row.stones;
		for (n = 0; n < 5; n++, stone++) {
			if (stone->state <= 0) {
				continue;
			}
			if (stone->speed == 0) {
				continue;
			}
			zone = KAR_getWallZone(stone->pos.vx, stone->pos.vz);
			if ((zone > 0) && (zone < 6)) {
				playSound2(8, 3);
			}
			switch (zone) {
			case 1:
				stone->pos.vx = stone->pos.vx -
				                (int16_t)(stone->pos.vx + 0x2d5) * 2;
				stone->angle = 0x800 - stone->angle;
				break;
			case 2:
				stone->angle = 0x1800 - stone->angle;
				stone->pos.vx = stone->pos.vx -
				                (int16_t)(stone->pos.vx - 0x2d5) * 2;
				break;
			case 3:
				KAR_findWallContact((VECTOR *)&contact, stone, 0);
				KAR_reflectOffDiagonal((int32_t *)stone, 0);
				KAR_placeAtContact(stone, contact);
				break;
			case 4:
				KAR_findWallContact((VECTOR *)&contact, stone, 1);
				KAR_reflectOffDiagonal((int32_t *)stone, 1);
				KAR_placeAtContact(stone, contact);
				break;
			case 5:
				stone->angle = -stone->angle;
				stone->pos.vz = stone->pos.vz +
				                (int16_t)(-0x9dd - stone->pos.vz) * 2;
				break;
			case 6:
				if (stone->target.vz < 0x465) {
					playSound2(8, 3);
					stone->angle = -stone->angle;
					stone->pos.vz = stone->pos.vz -
					                (int16_t)(stone->pos.vz - 0x465) * 2;
				}
				break;
			}
		}
	}
}

int32_t KAR_distance(int32_t x, int32_t y)
{
	return SquareRoot0((uint32_t)(x * x) + (uint32_t)(y * y));
}

void KAR_resolveStoneCollision(KarStone *stoneA, VECTOR a, KarStone *stoneB, VECTOR b)
{
	if ((int16_t)(stoneA->speed + stoneB->speed) >= 0xbb8) {
		playSound2(8, 1);
	} else {
		playSound2(8, 2);
	}

	if (stoneA->state == 2 || stoneA->state == 3 || stoneB->state == 2 || stoneB->state == 3) {
		if (stoneA->state == 2 || stoneA->state == 3) {
			KAR_collidePeggedStone(stoneA, a, stoneB, b);
		} else {
			KAR_collidePeggedStone(stoneB, b, stoneA, a);
		}
	} else if (stoneA->speed <= 0 || stoneB->speed <= 0) {
		if (stoneA->speed <= 0) {
			KAR_collideRestingStone(stoneA, a, stoneB, b);
		} else {
			KAR_collideRestingStone(stoneB, b, stoneA, a);
		}
	} else {
		KAR_collideMovingStones(stoneB, b, stoneA, a);
	}
}

void KAR_collidePeggedStone(KarStone *pegged, VECTOR a, KarStone *mover, VECTOR b)
{
	int32_t ang1;
	int32_t da;
	int32_t dd;
	int32_t r;
	int32_t t;

	KAR_debugCollisionStone(mover, &b, &a);
	ang1 = ratan2(mover->target.vz - mover->pos.vz, mover->target.vx - mover->pos.vx) & 0xfff;
	r = ratan2(b.vz - a.vz, b.vx - a.vx) & 0xfff;
	da = r * 2 - ang1;
	if (da >= 0x1000) {
		da = da % 0x1000;
	}

	dd = abs(r - ang1);
	dd = dd % 0x400;
	dd = 0x400 - dd;
	r = KAR_distance(mover->pos.vx - b.vx, mover->pos.vz - b.vz);
	r = dd * r / 0x400;
	do {
		mover->pos.vx = b.vx + r * rcos(da) / 4096;
		mover->pos.vz = b.vz + r * rsin(da) / 4096;
		r++;
	} while (KAR_distance(mover->pos.vx - pegged->pos.vx, mover->pos.vz - pegged->pos.vz) < 0x97);

	t = (mover->speed >> 1) * dd / 0x400;
	pegged->speed = t;
	mover->speed = mover->speed - t;
	pegged->angle = ratan2(mover->target.vz - b.vz, mover->target.vx - b.vx);
	mover->angle = da;
	pegged->state = 3;
	mover->target = b;
}

void KAR_collideRestingStone(KarStone *resting, VECTOR a, KarStone *mover, VECTOR b)
{
	VECTOR p1;
	VECTOR p2;
	int32_t d;
	int32_t ang;
	int32_t r;
	int32_t share;
	int32_t n;

	KAR_debugCollisionStone(mover, &b, &a);
	if (resting->speed == 0 && mover->speed == 0) {
		resting->pos = a;
		mover->pos = b;
		return;
	}

	share = KAR_computeImpactShare(mover, b, a);
	KAR_computeSeparation(&p1, mover, b, a);
	resting->pos.vx = resting->pos.vx + p1.vx;
	resting->pos.vz = resting->pos.vz + p1.vz;
	mover->pos.vx = mover->pos.vx - p1.vx;
	mover->pos.vz -= p1.vz;
	d = KAR_distance(resting->pos.vx - mover->pos.vx, resting->pos.vz - mover->pos.vz);
	ang = ratan2(p1.vz, p1.vx);
	n = KAR_distance(p1.vx, p1.vz);
	p1 = resting->pos;
	r = 0;
	p2 = mover->pos;
	if (n != 0) {
		while (d < 0x97) {
			resting->pos.vx = p1.vx + r * rcos(ang) / 4096;
			resting->pos.vz = p1.vz + r * rsin(ang) / 4096;
			mover->pos.vx = p2.vx - r * rcos(ang) / 4096;
			mover->pos.vz = p2.vz - r * rsin(ang) / 4096;
			d = KAR_distance(resting->pos.vx - mover->pos.vx, resting->pos.vz - mover->pos.vz);
			r++;
		}
	} else {
		ang = ratan2(mover->pos.vz - resting->pos.vz, mover->pos.vx - resting->pos.vx);
		while (d < 0x97) {
			mover->pos.vx = p2.vx + r * rcos(ang) / 4096;
			mover->pos.vz = p2.vz + r * rsin(ang) / 4096;
			d = KAR_distance(resting->pos.vx - mover->pos.vx, resting->pos.vz - mover->pos.vz);
			r++;
		}
	}

	resting->target = a;
	mover->target = b;
	mover->speed = mover->speed - share;
	resting->speed = share;
	resting->angle = ratan2(resting->pos.vz - a.vz, resting->pos.vx - a.vx) & 0xfff;
	mover->angle = ratan2(mover->pos.vz - b.vz, mover->pos.vx - b.vx) & 0xfff;
}

void KAR_collideMovingStones(KarStone *stoneA, VECTOR a, KarStone *stoneB, VECTOR b)
{
	VECTOR p1;
	VECTOR p2;
	int32_t share1;
	int32_t share2;
	int32_t dx;
	int32_t dz;
	int32_t d;
	int32_t ang;
	int32_t r;

	KAR_debugCollisionPair(stoneA, &a, stoneB, &b);
	share1 = KAR_computeImpactShare(stoneA, a, b);
	share2 = KAR_computeImpactShare(stoneB, b, a);
	KAR_computeSeparation(&p1, stoneB, b, a);
	KAR_computeSeparation(&p2, stoneA, a, b);
	dx = p2.vx - p1.vx;
	dz = p2.vz - p1.vz;
	ang = KAR_distance(dx, dz);
	if (ang != 0) {
		ang = ratan2(dz, dx);
		stoneB->pos.vx = stoneB->pos.vx + dx;
		stoneB->pos.vz = stoneB->pos.vz + dz;
		stoneA->pos.vx = stoneA->pos.vx - dx;
		stoneA->pos.vz = stoneA->pos.vz - dz;
		d = KAR_distance(stoneA->pos.vx - stoneB->pos.vx, stoneA->pos.vz - stoneB->pos.vz);
		p1 = stoneA->pos;
		p2 = stoneB->pos;
		stoneA->speed += (int16_t)(share2 - share1);
		stoneB->speed += (int16_t)(share1 - share2);
		r = 0;
		if (stoneA->speed > stoneB->speed) {
			while (d < 0x97) {
				stoneA->pos.vx = p1.vx + r * rcos(ang) / 4096;
				stoneA->pos.vz = p1.vz + r * rsin(ang) / 4096;
				d = KAR_distance(stoneB->pos.vx - stoneA->pos.vx, stoneB->pos.vz - stoneA->pos.vz);
				r++;
			}
		} else {
			while (d < 0x97) {
				stoneB->pos.vx = p2.vx + r * rcos(ang) / 4096;
				stoneB->pos.vz = p2.vz + r * rsin(ang) / 4096;
				d = KAR_distance(stoneB->pos.vx - stoneA->pos.vx, stoneB->pos.vz - stoneA->pos.vz);
				r++;
			}
		}
		stoneA->target = a;
		stoneB->target = b;
		stoneA->angle = ratan2(stoneA->pos.vz - a.vz, stoneA->pos.vx - a.vx);
		stoneB->angle = ratan2(stoneB->pos.vz - b.vz, stoneB->pos.vx - b.vx);
	} else {
		KAR_distance(stoneA->pos.vx - stoneB->pos.vx, stoneA->pos.vz - stoneB->pos.vz);
		if (stoneB->speed / stoneA->weight < 5 || stoneA->speed / stoneB->weight < 5) {
			stoneB->speed = 0;
			stoneB->pos = stoneB->target;
			stoneA->pos = stoneA->target;
		} else {
			stoneB->pos = b;
			stoneA->pos = a;
		}
	}
}

void KAR_debugCollisionStone(KarStone *stone, VECTOR *b, VECTOR *a)
{
}

int32_t KAR_computeImpactShare(KarStone *stone, VECTOR a, VECTOR b)
{
	int32_t ang;
	int32_t d;
	int32_t ret;

	ang = stone->angle & 0xfff;
	d = (ang - (ratan2(b.vz - a.vz, b.vx - a.vx) & 0xfff)) & 0xfff;
	ret = 0;
	if (d < 0x400 || d > 0xc00) {
		if (d > 0xc00) {
			d = (0x1000 - d) & 0xfff;
		}
		d = 0x400 - (d % 0x400);
		ret = d * stone->speed / 0x400;
	}

	return ret;
}

int32_t KAR_computeSeparation(VECTOR *out, KarStone *stone, VECTOR a, VECTOR b)
{
	VECTOR v;
	int32_t cur;
	int32_t ang;
	int32_t d;
	int32_t r;
	int32_t n;

	cur = stone->angle & 0xfff;
	ang = ratan2(b.vz - a.vz, b.vx - a.vx) & 0xfff;
	d = abs(cur - ang);
	v.vz = 0;
	v.vy = 0;
	v.vx = 0;
	if (d < 0x400 || d > 0xc00) {
		d = d % 0x400;
		r = (0x400 - d) * KAR_distance(stone->pos.vz - a.vz, stone->pos.vx - a.vx) / 0x400;
		do {
			v.vx = r * rcos(ang) / 4096;
			v.vz = r * rsin(ang) / 4096;
			n = KAR_distance(v.vx, v.vz);
			r++;
		} while (n == 0);
	}

	*out = v;
}

void KAR_debugCollisionPair(KarStone *stoneA, VECTOR *a, KarStone *stoneB, VECTOR *b)
{
}

int32_t KAR_getWallZone(int16_t x, int16_t z)
{
	if (z >= 0x465) {
		return 6;
	}

	if (z >= -0x7d0) {
		if (x < -0x2d4) {
			return 1;
		}
		if (x >= 0x2d5) {
			return 2;
		}
	} else {
		if (z + (x * 2) < -0xdc4 && x < 0x1aa) {
			return 3;
		}
		if (z - (x * 2) < -0xdc4 && x >= 0x1a9) {
			return 4;
		}
		if (z < -0x9dc) {
			return 5;
		}
	}

	return 0;
}

int32_t KAR_findWallContact(VECTOR *out, KarStone *stone, int32_t flag)
{
	int32_t t;
	int32_t i;
	int32_t ang;
	VECTOR v;

	i = 0;
	ang = (stone->angle + 0x800) & 0xfff;
	do {
		v.vx = stone->pos.vx + i * rcos(ang) / 4096;
		v.vz = stone->pos.vz + i * rsin(ang) / 4096;
		i++;
		if (flag == 0) {
			t = (v.vz + 0xdc5) + v.vx * 2;
		} else {
			t = (v.vz + 0xdc5) - v.vx * 2;
		}
	} while (t <= 0);

	*out = v;
}

void KAR_reflectOffDiagonal(int32_t *p, int32_t dir)
{
	int32_t cur;
	int32_t ang;

	cur = (p[3] + 0x800) & 0xfff;
	if (dir == 0) {
		ang = ratan2(1, 2) * 2;
	} else {
		ang = ratan2(1, -2) * 2;
	}

	p[3] = ang - cur;
}

void KAR_placeAtContact(KarStone *stone, KarPos p)
{
	int32_t d;

	d = KAR_distance(p.x - stone->target.vx, p.z - stone->target.vz);
	d = KAR_distance(stone->target.vz - stone->pos.vz, stone->target.vx - stone->pos.vx) - d;
	stone->pos.vx = p.x + ((d * rcos(stone->angle)) / 4096);
	stone->pos.vz = p.z + ((d * rsin(stone->angle)) / 4096);
}

void KAR_beginAiming(void)
{
	int8_t idx;
	int8_t n;

	idx = (MAIN_D_8013523C != 0) ? MAIN_D_80135248 : 0;

	MAIN_D_80135244 = 6;
	if (idx == 0) {
		startAnimation(ENTITY_TABLE[0], 0x23);
	} else if (MAIN_D_80135248 == 2) {
		startAnimation(ENTITY_TABLE[MAIN_D_80135248], 0x1c);
	}

	KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].state = 0;
	if (KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].type == 3) {
		n = (MAIN_D_8013523C != 0) ? 1 : 2;
		GsLinkObject4((u_long)(MAIN_D_80135240 + 3), &KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].obj, n);
	}

	MAIN_D_8013524C = -0x400;
	ENTITY_TABLE[idx]->posData->rotation.vy = 0;
}

void KAR_selectPreviousStone(void)
{
	int32_t row;

	if (MAIN_D_8013523A > 0) {
		for (row = MAIN_D_8013523A - 1; row >= 0; row--) {
			if (KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[row].state == -1) {
				MAIN_D_8013523A = row;
				return;
			}
		}
	}
}

void KAR_selectNextStone(void)
{
	int32_t row;

	if (MAIN_D_8013523A < 4) {
		for (row = MAIN_D_8013523A + 1; row < 5; row++) {
			if (KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[row].state == -1) {
				MAIN_D_8013523A = row;
				return;
			}
		}
	}
}

void KAR_turnAimLeft(int32_t delta)
{
	int8_t idx;
	PositionData *pos;

	idx = (MAIN_D_8013523C != 0) ? MAIN_D_80135248 : 0;
	pos = ENTITY_TABLE[idx]->posData;
	pos->rotation.vy = pos->rotation.vy - delta;
	MAIN_D_8013524C += delta;
}

void KAR_turnAimRight(int32_t delta)
{
	int8_t idx;
	PositionData *pos;

	idx = (MAIN_D_8013523C != 0) ? MAIN_D_80135248 : 0;
	pos = ENTITY_TABLE[idx]->posData;
	pos->rotation.vy = pos->rotation.vy - delta;
	MAIN_D_8013524C += delta;
}

void KAR_beginThrow(void)
{
	MAIN_D_80135244 = 7;
	if (MAIN_D_8013523C == 0) {
		startAnimation(ENTITY_TABLE[0], 0x24);
	} else if (MAIN_D_80135248 == 2) {
		startAnimation(ENTITY_TABLE[MAIN_D_80135248], 0x1d);
	} else {
		startAnimation(ENTITY_TABLE[MAIN_D_80135248], 0x1f);
	}
}

int32_t KAR_tickYesNoPrompt(void)
{
	RECT finalPos;
	RECT startPos;
	KarStrPair strs;
	int32_t mode;

	finalPos = MAIN_D_80134A10;
	startPos = MAIN_D_80134A18;
	strs = MAIN_D_80134A20;
	while (MAIN_D_80135250 < 2) {
		mode = MAIN_D_80135250;
		drawString(strs.text[mode], 0, (mode * 13) + 0xdd);
		MAIN_D_80135250 = MAIN_D_80135250 + 1;
		break;
	}

	if (UI_BOX_DATA[0].state == 0) {
		if (MAIN_D_80135250 == 2) {
			createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, NULL, NULL);
			MAIN_D_80135250 = 3;
		} else if (MAIN_D_80135250 == 3 || MAIN_D_80135250 == 4) {
			return (int8_t)(MAIN_D_80135250 - 2);
		}
	}

	if (UI_BOX_DATA[0].state == 1) {
		if ((POLLED_INPUT & 0x40) != 0 && (POLLED_INPUT_PREVIOUS & 0x40) == 0) {
			removeAnimatedUIBox(0, NULL);
			return 0;
		}
		if ((POLLED_INPUT & 0x1000) != 0 && (POLLED_INPUT_PREVIOUS & 0x1000) == 0) {
			MAIN_D_80135250 = (MAIN_D_80135250 == 3) ? 4 : 3;
			playSound(0, 2);
		} else if ((POLLED_INPUT & 0x4000) != 0 && (POLLED_INPUT_PREVIOUS & 0x4000) == 0) {
			MAIN_D_80135250 = (MAIN_D_80135250 == 3) ? 4 : 3;
			playSound(0, 2);
		}
		renderString(0, -0x7c, 0x30, 0x52, 0xd, 0, 0xdc, 6, 1);
		renderString(0, -0x7c, 0x3d, 0x5c, 0xd, 0x53, 0xdc, 6, 1);
		renderSelectionCursor(finalPos.x + 6, finalPos.y + 7 + ((MAIN_D_80135250 - 3) * 13), 0x5e, 0xd, 6);
	}

	return 0;
}

int32_t KAR_tickHintBox(int32_t n)
{
	RECT finalPos;
	RECT startPos;
	int8_t mode;

	finalPos = MAIN_D_80134A28;
	startPos = MAIN_D_80134A30;
	if (MAIN_D_80135250 == 0) {
		return 0;
	}

	mode = MAIN_D_80135250;

	if (n < 4) {
		startPos.x = 0x4b;
		startPos.y = -5;
	}

	if (UI_BOX_DATA[0].state == 0) {
		if (mode == 1) {
			createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, NULL, NULL);
			clearTextArea();
			MAIN_D_8013522C = 0;
			return 1;
		}
		return 0;
	}

	if (MAIN_D_80135248 == 2) {
		n = (int8_t)KAR_drawHintPagePenguinmon(n, MAIN_D_8013522C);
	} else {
		n = (int8_t)KAR_drawHintPageMetalMamemon(n, MAIN_D_8013522C);
	}

	if (MAIN_D_8013522C < n * 4) {
		MAIN_D_8013522C++;
	}

	if (UI_BOX_DATA[0].state == 1) {
		if ((POLLED_INPUT & 0x40) != 0 && (POLLED_INPUT_PREVIOUS & 0x40) == 0) {
			MAIN_D_80135250 = MAIN_D_80135250 + 1;
			playSound(0, 3);
			if (n < MAIN_D_80135250) {
				removeAnimatedUIBox(0, NULL);
				MAIN_D_80135250 = -1;
			}
		}
		if (MAIN_D_80135250 != -1) {
			renderString(0, UI_BOX_DATA[0].finalPos.x + 4, UI_BOX_DATA[0].finalPos.y + 2, 0xfc, 0x34, 0,
			             (MAIN_D_80135250 - 1) * 52, 6, 1);
			renderUIBox(0);
		}
	}

	return 1;
}

void KAR_classifyStoneRings(void)
{
	KarStone *stone;
	KarStone *cur;
	int32_t p;
	int32_t i;
	int32_t zi;

	for (p = 0; p < 2; p++) {
		stone = KAR_D_8005B5A0[p].row.stones;
		KAR_D_8005B5A0[p].row.score = 0;
		for (i = 0; i < 5; i++, stone++) {
			cur = stone;
			stone->prevRing = cur->ring;
			stone->ring = 0;
			if (stone->state <= 0) {
				continue;
			}
			if (cur->pos.vz >= -0x564) {
				continue;
			}
			KAR_distance(stone->pos.vx, stone->pos.vz + 0x6a4);
			for (zi = 0; zi < 4; zi++) {
				KarZones z;

				z = KAR_D_8005B438;
				if (KAR_distance(stone->pos.vx + z.ring[zi].dx, cur->pos.vz + z.ring[zi].dz) > z.ring[zi].radius) {
					continue;
				}
				stone->ring = zi + 1;
				if (p != 0) {
					stone->ring = stone->ring | 0x10;
				}
				break;
			}
		}
	}

	KAR_registerThrownStone();
}

int32_t KAR_tickScoreTally(void)
{
	KarTallyValues values;
	int32_t i;
	int32_t j;
	int32_t done;
	int8_t rawRing;
	int8_t player;
	int8_t ring;

	values = MAIN_D_80134A44;
	if ((POLLED_INPUT & 0x40) != 0 && (POLLED_INPUT_PREVIOUS & 0x40) == 0) {
		MAIN_D_8013522D = 1;
	}

	if (MAIN_D_8013522D == 1 && (MAIN_D_8013522E == 2 || MAIN_D_8013522E == 1)) {
		KarStone *stone;

		MAIN_D_80135256 = KAR_D_8005B5A0[0].row.thrown + KAR_D_8005B5A0[1].row.thrown;
		MAIN_D_80135256--;
		KAR_D_8005B5A0[0].row.score = 0;
		KAR_D_8005B5A0[1].row.score = 0;
		while (MAIN_D_80135256 >= 0) {
			stone = KAR_D_800639C0[MAIN_D_80135256];
			rawRing = stone->ring;
			player = (rawRing & 0x10) ? 1 : 0;
			ring = rawRing & 0xf;
			if (stone->state > 0 && ring != 0) {
				KAR_D_8005B5A0[player].row.score += values.value[ring - 1];
			}
			MAIN_D_80135256--;
		}

		for (i = 0; i < 3; i++) {
			KarStone *rowStone;

			rowStone = KAR_D_8005B5A0[i].row.stones;
			for (j = 0; j < 5; j++, rowStone++) {
				if (rowStone->state >= 100) {
					rowStone->state -= 100;
				}
			}
		}
		MAIN_D_80135230 = 0;
		MAIN_D_80135234 = 0;
		MAIN_D_8013522E = 0;
		MAIN_D_8013522D = 0;
		return 1;
	}

	if (MAIN_D_80134A4F != MAIN_D_80135256) {
		KarStone *stone;

		while (MAIN_D_80135256 >= 0) {
			stone = KAR_D_800639C0[MAIN_D_80135256];
			if ((int8_t)(stone->ring & 0xf) != 0) {
				MAIN_D_80134A4F = MAIN_D_80135256;
				MAIN_D_8013522E = 0;
				break;
			}
			MAIN_D_80135256--;
		}
		if (MAIN_D_80135256 < 0) {
			MAIN_D_8013522D = 0;
			return 1;
		}
	}

	if (MAIN_D_8013522E == 0) {
		KarStone *stone;
		KarStone **stones;
		int32_t cameraZ;
		int32_t changed;

		stones = KAR_D_800639C0;
		stone = stones[MAIN_D_80135256];
		changed = stone->ring != stone->prevRing;
		if (changed != 0) {
			cameraZ = stone->pos.vz;
			done = tickMoveCameraTo(stone->pos.vx, cameraZ, 5);
			if ((CAMERA_Y[0] % 0x80) == 0 || (CAMERA_Y[0] % 0x80) >= 0x6a) {
				i = MAP_WIDTH[0];
				j = i;
				i = MAP_TILE_Y;
				uploadMapTileImages(MAP_TILE_DATA, MAP_TILE_X + i * j);
			}
			if (done == 1) {
				MAIN_D_8013522E = 1;
			} else {
				return 0;
			}
		} else {
			MAIN_D_8013522E = 1;
		}
	}

	if (MAIN_D_8013522E == 1) {
		KarStone *stone;
		int8_t ringIndex;

		stone = KAR_D_800639C0[MAIN_D_80135256];
		rawRing = stone->ring;
		player = (rawRing & 0x10) ? 1 : 0;
		ring = rawRing & 0xf;
		ringIndex = ring;
		KAR_D_8005B5A0[player].row.score += values.value[ringIndex - 1];
		MAIN_D_8013522E = 2;
		if (stone->ring != stone->prevRing && (stone->ring & 0xf) != 0) {
			if (ringIndex == 3) {
				playSound2(8, 6);
			} else {
				playSound2(8, 5);
			}
		}
	}

	if (MAIN_D_8013522E == 2 && MAIN_D_80135256 >= 0) {
		KarStone *stone;
		int32_t changed;

		stone = KAR_D_800639C0[MAIN_D_80135256];
		changed = stone->ring != stone->prevRing;
		if (changed != 0 && (stone->ring & 0xf) != 0) {
			if (MAIN_D_80135234++ >= 0xb) {
				if (stone->state < 100) {
					stone->state += 100;
				} else {
					stone->state -= 100;
				}
				MAIN_D_80135234 -= 10;
				if (MAIN_D_80135230++ >= 7) {
					MAIN_D_8013522E = 4;
					MAIN_D_80135230 = 0;
					MAIN_D_80135256--;
				}
			}
		} else {
			MAIN_D_80135256--;
		}

		if (MAIN_D_80135256 < 0) {
			return 1;
		}
		return 0;
	}

	return 0;
}

int32_t KAR_chooseOpponentShot(void)
{
	int16_t tx;
	int16_t tz;
	int32_t angle;
	int16_t px;
	int16_t pz;
	int32_t i;
	int32_t clear;

	if (MAIN_D_80135248 == 2) {
		angle = KAR_findClearShotAngle(0, -0x6a4);
		if (angle == -1) {
			if (KAR_D_8005B5A0[0].row.score > KAR_D_8005B5A0[1].row.score) {
				angle = KAR_aimAtStoneInRing(1, 0x14, &tx, &tz);
				if (angle != -1) {
					KAR_setOpponentShot(0, angle, tx, tz);
				} else {
					angle = KAR_aimAtStoneInRing(0, 2, &tx, &tz);
					if (angle != -1) {
						KAR_setOpponentShot(0, angle, tx, tz);
					} else {
						angle = KAR_findClearShotAngle(-0x190, -0x960);
						if (angle != -1) {
							KAR_setOpponentShot(1, angle, 0, -0x960);
						} else {
							angle = KAR_aimAtRandomStone();
							KAR_setOpponentShot(0, angle, 0, -0x960);
							KAR_D_8005B5A0[1].row.unk2 = 0x960;
						}
					}
				}
			} else {
				angle = KAR_aimAtStoneInRing(0, 2, &tx, &tz);
				if (angle != -1) {
					KAR_setOpponentShot(0, angle, tx, tz);
				} else {
					angle = KAR_findClearShotAngle(-0x190, -0x960);
					if (angle != -1) {
						KAR_setOpponentShot(1, angle, 0, -0x960);
					} else {
						angle = KAR_aimAtRandomStone();
						KAR_setOpponentShot(0, angle, 0, -0x960);
						KAR_D_8005B5A0[1].row.unk2 = 0x960;
					}
				}
			}
		} else {
			KAR_setOpponentShot(1, angle, 0, -0x6a4);
		}
	} else {
		px = ENTITY_TABLE[MAIN_D_80135248]->posData[4].posMatrix.workm.t[0];
		pz = ENTITY_TABLE[MAIN_D_80135248]->posData[4].posMatrix.workm.t[2];

		if (KAR_D_8005B5A0[1].row.thrown == 0) {
			angle = ratan2(-0x8fc - pz, -0x190 - px);
			clear = KAR_findClearShotAngle(-0x190, -0x8fc);
			if (angle == clear) {
				KAR_setOpponentShot(1, angle, -0x190, -0x8fc);
			} else {
				angle = ratan2(-0x6a4 - pz, -px);
				clear = KAR_findClearShotAngle(0, -0x6a4);
				if (angle == clear) {
					KAR_setOpponentShot(1, angle, 0, -0x6a4);
				} else {
					angle = ratan2(-0x6a4 - pz, -px);
					if ((KAR_D_8005B5A0[1].row.thrown % 2) != 0) {
						angle += rand() % 100;
					} else {
						angle -= rand() % 100;
					}
					KAR_setOpponentShot(0, angle, 0, -0x6a4);
				}
			}
		} else {
			for (i = 0; i < 5; i++) {
				if (KAR_D_8005B5A0[1].row.stones[(uint32_t)i].state == -1) {
					if (KAR_D_8005B5A0[1].row.stones[(uint32_t)i].type == 3) {
						KAR_D_8005B5A0[1].row.unk4 = i;
						angle = ratan2(-0x6a4 - pz, -px);
						clear = KAR_findClearShotAngle(0, -0x6a4);
						if (angle == clear) {
							KAR_setOpponentShot(1, angle, 0, -0x6a4);
						} else {
							KAR_D_8005B5A0[1].row.unk4 = 6;
						}
						break;
					}
					KAR_D_8005B5A0[1].row.unk4 = 6;
				}
			}

			if (KAR_D_8005B5A0[1].row.unk4 == 6) {
				switch (((int8_t *)KAR_D_80063918)[0]) {
				case 0:
				case 1:
				case 2:
					if ((((int8_t *)KAR_D_80063924)[0] == 3) || (((int8_t *)KAR_D_80063924)[0] == 0)) {
						angle = ratan2(-0x8fc - pz, -0x190 - px);
						KAR_findClearShotAngle(-0x190, -0x8fc);
						KAR_setOpponentShot(1, angle, -0x190, -0x8fc);
					} else if (KAR_D_800638F4[8] < 0) {
						angle = ratan2(-0x6a4 - pz, -px);
						if ((KAR_D_8005B5A0[1].row.thrown % 2) != 0) {
							angle += rand() % 100;
						} else {
							angle -= rand() % 100;
						}
						KAR_setOpponentShot(0, angle, 0, -0x6a4);
					} else if ((rand() % 2) != 0) {
						angle = ratan2(-0x8fc - pz, -0x190 - px);
						clear = KAR_findClearShotAngle(-0x190, -0x8fc);
						if (angle == clear) {
							KAR_setOpponentShot(1, angle, -0x190, -0x8fc);
						} else {
							angle = ratan2(KAR_D_800639B0[2] - pz, KAR_D_800639B0[0] - px);
							KAR_setOpponentShot(0, angle, (int16_t)KAR_D_800639B0[0], (int16_t)KAR_D_800639B0[2]);
						}
					} else {
						angle = KAR_aimBankShot((KarStone *)&KAR_D_800638F4[8], 0x190, -0x8fc);
						KAR_setOpponentShot(0, angle, 0x190, -0x8fc);
						KAR_D_8005B5A0[1].row.unk2 = 0x960;
					}
					break;
				case 3:
					angle = ratan2(-0x6a4 - pz, -px);
					if ((KAR_D_8005B5A0[1].row.thrown % 2) != 0) {
						angle += rand() % 100;
					} else {
						angle -= rand() % 100;
					}
					KAR_setOpponentShot(0, angle, 0, -0x6a4);
					break;
				}
			}
		}
	}
}

int32_t KAR_drawHintPagePenguinmon(int32_t idx, int8_t n)
{
	KarStrTbl strs;
	KarOffTbl offs;

	strs = KAR_D_8005AF58;
	offs = KAR_D_8005B04C;
	if ((n % 4) == 0) {
		setTextColor(7);
	} else {
		setTextColor(1);
	}

	drawString(strs.text[offs.start[idx] + n], 0, (n * 13) + 1);

	return (int8_t)((offs.start[idx + 1] - offs.start[idx]) / 4);
}

int32_t KAR_drawHintPageMetalMamemon(int32_t idx, int8_t n)
{
	KarStrTbl strs;
	KarOffTbl offs;

	strs = KAR_D_8005B318;
	offs = KAR_D_8005B40C;
	if ((n % 4) == 0) {
		setTextColor(7);
	} else {
		setTextColor(1);
	}

	drawString(strs.text[offs.start[idx] + n], 0, (n * 13) + 1);

	return (int8_t)((offs.start[idx + 1] - offs.start[idx]) / 4);
}

int32_t KAR_findClearShotAngle(int32_t x, int32_t z)
{
	PositionData *pd;
	KarStone *stone;
	int32_t ang;
	int32_t p;
	int32_t n;
	int32_t px;
	int32_t pz;
	int16_t step;

	pd = ENTITY_TABLE[MAIN_D_80135248]->posData;
	px = (int16_t)pd->location.vx;
	pz = (int16_t)pd->location.vz;
	if (MAIN_D_80135248 != 2) {
		px = (int16_t)pd[4].posMatrix.workm.t[0];
		pz = (int16_t)pd[4].posMatrix.workm.t[2];
	}
	ang = ratan2(z - pz, x - px);
	if (MAIN_D_80135248 == 2) {
		step = 30;
	} else {
		step = 10;
	}
	while (z < pz) {
		px = (int16_t)(px + (int16_t)((step * rcos(ang)) / 4096));
		pz = (int16_t)(pz + (int16_t)((step * rsin(ang)) / 4096));
		for (p = 0; p < 3; p++) {
			p = p;
			stone = KAR_D_8005B5A0[p].row.stones;
			for (n = 0; n < 5; n++, stone++) {
				if (stone->state > 0) {
					if (KAR_distance(px - stone->pos.vx, pz - stone->pos.vz) < 0x97) {
						if (MAIN_D_80135248 == 2) {
							return -1;
						}
						return ratan2(stone->pos.vz - z, stone->pos.vx - x);
						/* scheduling barrier: mwcc needs the extra
						 * block here to colour the loop registers */
						do {
						} while (0);
					}
				}
			}
		}
	}
	return ang;
}

int32_t KAR_aimAtStoneInRing(int32_t player, int32_t key, int16_t *outX, int16_t *outZ)
{
	KarStone *stone;
	int32_t i;
	int32_t r;

	stone = KAR_D_8005B5A0[player].row.stones;
	for (i = 0; i < 5; i++, stone++) {
		if ((stone->state < 2) && (stone->ring == key)) {
			r = KAR_findClearShotAngle((int16_t)stone->pos.vx, (int16_t)stone->pos.vz);
			*outX = stone->pos.vx;
			*outZ = stone->pos.vz;
			return r;
		}
	}

	return -1;
}

void KAR_setOpponentShot(int32_t row, int32_t val, int32_t b, int32_t c)
{
	KarStone *stone;
	int32_t i;
	int32_t n;

	for (i = 0; i < 4; i++) {
		KarShotPlan t;

		t = KAR_D_8005B418;
		n = KAR_findUnusedStoneOfType((int8_t)t.type[row][i]);
		if (n >= 0) {
			KAR_D_8005B5A0[1].row.unk4 = n;
			stone = &KAR_D_8005B5A0[1].row.stones[n];
			break;
		}
	}

	stone->angle = val;
	KAR_D_8005B5A0[1].row.unk2 = KAR_computeThrowPower(stone->weight, b, c);
}

int32_t KAR_aimAtRandomStone(void)
{
	int32_t i;
	KarStone *cands[5];
	KarStone *stone;
	uint8_t count;
	int16_t px;
	int16_t pz;

	stone = KAR_D_8005B5A0[0].row.stones;
	if (KAR_D_800638F4[8] < 0) {
		return ratan2(-0xc1c, -0x190);
	}

	for (i = 0, count = 0; i < 5; i++, stone++) {
		if (stone->state == 1) {
			cands[count] = stone;
			count++;
		}
	}

	if (count == 0) {
		px = 0;
		pz = -0x6a4;
	} else {
		uint8_t idx;

		idx = rand() % count;
		px = cands[idx]->pos.vx;
		pz = cands[idx]->pos.vz;
	}

	return ratan2(pz - 0x578, px - 0x190);
}

int32_t KAR_aimBankShot(KarStone *stone, int32_t x, int32_t z)
{
	int32_t ang;
	int32_t cx;
	int32_t cz;
	int32_t *pd;

	ang = ratan2(stone->pos.vz - z, stone->pos.vx - x);
	cx = stone->pos.vx + ((rcos(ang) * 150) / 4096);
	cz = stone->pos.vz + ((rsin(ang) * 150) / 4096);
	cx = -0x2d5 - (cx + 0x2d5);
	pd = (int32_t *)ENTITY_TABLE[MAIN_D_80135248]->posData;

	return ratan2(cz - pd[0x9c], cx - pd[0x9a]);
}

int32_t KAR_findUnusedStoneOfType(int32_t key)
{
	KarStone *stone;
	int32_t i;

	stone = KAR_D_8005B5A0[1].row.stones;
	for (i = 0; i < 5; i++, stone++) {
		if ((stone->state == -1) && (stone->type == key)) {
			return i;
		}
	}

	return -1;
}

int32_t KAR_computeThrowPower(int32_t a, int32_t b, int32_t c)
{
	int32_t dist;
	uint32_t acc;
	uint32_t value;
	uint32_t cur;

	dist = KAR_distance(0x190 - b, 0x578 - c);
	acc = 0;
	cur = a;
	value = a * 100;
	while (acc < dist) {
		acc += cur / a;
		value = value * 100 / 98;
		cur = value / 100;
	}
	if (MAIN_D_80135248 == 2) {
		cur -= 0x258;
	} else {
		cur -= 0x1f4;
	}
	return cur;
}

void KAR_registerThrownStone(void)
{
	MAIN_D_80135256 = KAR_D_8005B5A0[0].row.thrown + KAR_D_8005B5A0[1].row.thrown;
	KAR_D_800639C0[MAIN_D_80135256] =
		&KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A];
	MAIN_D_80134A4F = -1;
}

void KAR_rotatePoint(SVECTOR *p, int32_t ang)
{
	SVECTOR t;

	t = *p;
	p->vx = ((t.vx * rcos(ang)) / 4096) - ((t.vz * rsin(ang)) / 4096);
	p->vz = ((t.vx * rsin(ang)) / 4096) + ((t.vz * rcos(ang)) / 4096);
}

void KAR_renderSprite(KarSprite *sp)
{
	POLY_FT4 *p;

	p = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(p);
	p->tpage = getTPage(0, 0, 640, 0);
	setRGB0(p, sp->r, sp->g, sp->b);
	setClut(p, sp->clutX, sp->clutY);
	setUVWH(p, sp->u, sp->v, sp->w, sp->h);
	setXYWH(p, sp->x, sp->y, sp->w, sp->h);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0xa, p++);
	GsSetWorkBase((PACKET *)p);
}
