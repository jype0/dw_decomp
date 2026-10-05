#include <stdlib.h>

#include <inline_n.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/anim.h>
#include <dw/btl.h>
#include <dw/entity.h>
#include <dw/file.h>
#include <dw/font.h>
#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/kar.h>
#include <dw/map.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/world_object.h>

#include "common.h"

typedef struct {
	int16_t start;
	int16_t count;
} KarRingObject;
/* Starts and counts are four bytes apart in this interleaved table. */
typedef union {
	KarRingObject ring[4];
	int16_t starts[8];
	struct {
		int16_t firstStart;
		int16_t counts[7];
	} countView;
} KarRingObjects;
typedef struct {
	int8_t ring[4];
} KarRingFlags;

extern KarRingFlags MAIN_D_80134A48;
void setMapObjectsFlag(int32_t start, int32_t count, int32_t flag);

extern int32_t ACTIVE_FRAMEBUFFER;
extern int32_t VIEWPORT_DISTANCE;
extern GsOT GS_ORDERING_TABLE[];
extern GsRVIEW2 GS_VIEWPOINT;

extern char MAIN_D_80134A38[];
extern char MAIN_D_80134A3C[];

void renderSelectionCursor(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void renderUIBox(int32_t id);

void KAR_tickMatchState(void);
void KAR_renderAimArrow(void);
void KAR_updateRingMarkers(void);
void KAR_checkStonesStopped(void);
int32_t KAR_distance(long x, long y);
void KAR_debugCollisionStone(KarStone *stone, VECTOR *b, VECTOR *a);
void KAR_debugCollisionPair(KarStone *stoneA, VECTOR *a, KarStone *stoneB, VECTOR *b);
void KAR_reflectOffDiagonal(int32_t *p, int32_t dir);
void KAR_turnAimLeft(int32_t delta);
void KAR_turnAimRight(int32_t delta);
int32_t KAR_findUnusedStoneOfType(int32_t key);
void KAR_initializeOrderingTables(void);
int8_t KAR_tickYesNoPrompt(void);
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
int8_t KAR_tickScoreTally(void);
void KAR_bounceOffWall(void);
void KAR_updateCollisions(void);
void KAR_classifyStoneRings(void);
void KAR_renderScores(void);
void KAR_finishMatch(void);
int32_t KAR_drawHintPagePenguinmon(int32_t idx, int8_t n);
int32_t KAR_drawHintPageMetalMamemon(int32_t idx, int8_t n);
void KAR_renderPowerMeter(void);
int32_t KAR_computeThrowPower(uint32_t a, int32_t b, int32_t c);
void moveCameraByOffset(int32_t diffX, int32_t diffY);
void KAR_setOpponentShot(int8_t row, int32_t val, int16_t b, int16_t c);
void KAR_rotatePoint(SVECTOR *p, int32_t ang);
int32_t KAR_aimBankShot(KarStone *stone, int32_t x, int32_t z);
void KAR_placeAtContact(KarStone *stone, VECTOR p);
void KAR_handleAimScroll(void);
int32_t KAR_findClearShotAngle(int16_t x, int16_t z);
int32_t KAR_aimAtStoneInRing(int32_t player, int32_t key, int16_t *outX, int16_t *outZ);
void KAR_renderNamePlates(void);
void KAR_beginThrow(void);

int32_t tickCameraMoveTo(int16_t x, int16_t z, int32_t speed);
void uploadMapTileImages();
void moveCameraByDiff(VECTOR *from, VECTOR *to);

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

uint8_t MAIN_D_80135220;
int32_t MAIN_D_80135224;
uint32_t MAIN_D_80135228;
uint8_t MAIN_D_8013522C;
int8_t MAIN_D_8013522D;
int8_t MAIN_D_8013522E;
int32_t MAIN_D_80135230;
int32_t MAIN_D_80135234;
uint16_t MAIN_D_80135238;
int16_t MAIN_D_8013523A;
int8_t MAIN_D_8013523C;
int16_t MAIN_D_8013523E;
u_long *MAIN_D_80135240;
int32_t MAIN_D_80135244;
uint8_t MAIN_D_80135248;
int32_t MAIN_D_8013524C;
int8_t MAIN_D_80135250;
uint16_t MAIN_D_80135252;
int16_t MAIN_D_80135254;
int8_t MAIN_D_80135256;
uint8_t MAIN_D_80135257;

static void *kar_sbss_order[] = {
	&MAIN_D_80135257,
	&MAIN_D_80135256,
	&MAIN_D_80135254,
	&MAIN_D_80135252,
	&MAIN_D_80135250,
	&MAIN_D_8013524C,
	&MAIN_D_80135248,
	&MAIN_D_80135244,
	&MAIN_D_80135240,
	&MAIN_D_8013523E,
	&MAIN_D_8013523C,
	&MAIN_D_8013523A,
	&MAIN_D_80135238,
	&MAIN_D_80135234,
	&MAIN_D_80135230,
	&MAIN_D_8013522E,
	&MAIN_D_8013522D,
	&MAIN_D_8013522C,
	&MAIN_D_80135228,
	&MAIN_D_80135224,
	&MAIN_D_80135220,
};

// clang-format off
KarModelIds KAR_D_8005AB80 = { { 0x00000002, 0x00000001, 0x00000000 } };

KarOffTbl KAR_D_8005AB8C = {
	{
		0x03, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00,
		0x00, 0x00,
	},
};

#if !defined(VERSION_JP)
KarSpawnX KAR_D_8005AB98 = { { 0x000002c1, 0x000000eb, 0xffffff15 } };
#endif

char KAR_D_8005ABA4[20] = "\\ETCDAT\\KARRING.TMD";

#if defined(VERSION_JP)
char KAR_STR_START_GAME[] = "ゲームをはじめる";
char KAR_STR_EXPLAIN_GAME[] = "説明を聞く？";
#else
char KAR_D_8005ABB8[] = "Start GameExplain Game";
#endif

KarWeightTbl MAIN_D_80134A08 = { { 30, 25, 35, 30 } };
RECT MAIN_D_80134A10 = { -130, 42, 262, 59 };
RECT MAIN_D_80134A18 = { 75, -5, 10, 10 };
#if defined(VERSION_JP)
KarStrPair MAIN_D_80134A20 = { { KAR_STR_START_GAME, KAR_STR_EXPLAIN_GAME } };
#else
KarStrPair MAIN_D_80134A20 = { { KAR_D_8005ABB8, NULL } };
#endif
RECT MAIN_D_80134A28 = { -130, 42, 262, 59 };
RECT MAIN_D_80134A30 = { 0, 0, 10, 10 };

#if defined(VERSION_JP)
char KAR_STR_EMPTY[] = "";
char KAR_STR_PENGUINMON[] = "ペンモン";
char KAR_PENGUINMON_LINE_1[] = "「まず、カーリングダマのタイプを";
char KAR_PENGUINMON_LINE_2[] = "　選んでね。」";
char KAR_PENGUINMON_LINE_5[] = "「重いタマ、かるいタマ、色々なタマが";
char KAR_PENGUINMON_LINE_6[] = "　あるから使うタイミングを";
char KAR_PENGUINMON_LINE_7[] = "　良く考えてね！」";
char KAR_PENGUINMON_LINE_9[] = "「まれに、タマに４つ足がついているのが";
char KAR_PENGUINMON_LINE_10[] = "　あるんだけど、それは止まったところに";
char KAR_PENGUINMON_LINE_11[] = "　くっつくから、大事に使おう。」";
char KAR_PENGUINMON_LINE_13[] = "「今度は、投げる方向（点線）を";
char KAR_PENGUINMON_LINE_14[] = "　方向キーの左右で選んでね。」";
char KAR_PENGUINMON_LINE_17[] = "「この時、方向キーの上下で";
char KAR_PENGUINMON_LINE_18[] = "　スクロールできるよ！";
char KAR_PENGUINMON_LINE_19[] = "　投げる方向をきめたらａｈｉで決定。」";
char KAR_PENGUINMON_LINE_21[] = "「最後に、投げる強さを決めてね。";
char KAR_PENGUINMON_LINE_22[] = "　パワーゲージに合わせて";
char KAR_PENGUINMON_LINE_23[] = "　ａｈｉで決定。」";
char KAR_PENGUINMON_LINE_25[] = "「得点は、ＧＯＯＤマークに";
char KAR_PENGUINMON_LINE_26[] = "　カーリングダマをのせると２点。」";
char KAR_PENGUINMON_LINE_29[] = "「中心のＧＯＯＤマークの外、";
char KAR_PENGUINMON_LINE_30[] = "　青いラインの中だと１点。」";
char KAR_PENGUINMON_LINE_33[] = "「右下にあるＢＡＤにのせちゃうと";
char KAR_PENGUINMON_LINE_34[] = "　－２点になるから気をつけて！！」";
char KAR_PENGUINMON_LINE_37[] = "「あと、引き分けの場合は";
char KAR_PENGUINMON_LINE_38[] = "　おいらの勝ちにしてね！";
char KAR_PENGUINMON_LINE_39[] = "　それじゃ、ゲームスタートだよ！！」";
char KAR_PENGUINMON_LINE_41[] = "「あんちゃん、ぜんぜんたいしたこと";
char KAR_PENGUINMON_LINE_42[] = "　ないなぁ！」";
char KAR_PENGUINMON_LINE_45[] = "「まだまだ勝負はこれからだい！」";
char KAR_PENGUINMON_LINE_49[] = "「あんちゃん、なかなかやるね！」";
char KAR_PENGUINMON_LINE_53[] = "「へへへ、おいらの勝ちだぁ！」";
char KAR_PENGUINMON_LINE_57[] = "「うう、おいらのまけだよ・・・」";

KarStrTbl KAR_D_8005AF58 = {{
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_1,
	KAR_PENGUINMON_LINE_2,
	KAR_STR_EMPTY,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_5,
	KAR_PENGUINMON_LINE_6,
	KAR_PENGUINMON_LINE_7,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_9,
	KAR_PENGUINMON_LINE_10,
	KAR_PENGUINMON_LINE_11,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_13,
	KAR_PENGUINMON_LINE_14,
	KAR_STR_EMPTY,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_17,
	KAR_PENGUINMON_LINE_18,
	KAR_PENGUINMON_LINE_19,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_21,
	KAR_PENGUINMON_LINE_22,
	KAR_PENGUINMON_LINE_23,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_25,
	KAR_PENGUINMON_LINE_26,
	KAR_STR_EMPTY,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_29,
	KAR_PENGUINMON_LINE_30,
	KAR_STR_EMPTY,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_33,
	KAR_PENGUINMON_LINE_34,
	KAR_STR_EMPTY,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_37,
	KAR_PENGUINMON_LINE_38,
	KAR_PENGUINMON_LINE_39,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_41,
	KAR_PENGUINMON_LINE_42,
	KAR_STR_EMPTY,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_45,
	KAR_STR_EMPTY,
	KAR_STR_EMPTY,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_49,
	KAR_STR_EMPTY,
	KAR_STR_EMPTY,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_53,
	KAR_STR_EMPTY,
	KAR_STR_EMPTY,
	KAR_STR_PENGUINMON,
	KAR_PENGUINMON_LINE_57,
	KAR_STR_EMPTY,
	KAR_STR_EMPTY,
	NULL,
}};

char KAR_STR_BUTTON_MARK_COL11[] = "　　　　　　　　　　　ａ";
char KAR_STR_BUTTON_MARK_COL1[] = "　ａ";
KarStrPair KAR_PENGUINMON_HIGHLIGHTS = { { KAR_STR_BUTTON_MARK_COL11, KAR_STR_BUTTON_MARK_COL1 } };
#else
char MAIN_D_80134A38[] = " ";
char MAIN_D_80134A3C[8] = "I win!";

char KAR_D_8005ABD0[] = " Penguinmon ";

char KAR_D_8005ABE0[28] = "First of all, please choose";

char KAR_D_8005ABFC[20] = "your curling stone.";

char KAR_D_8005AC10[] = "There are heavy, medium, and";

char KAR_D_8005AC30[] = "light ones, so think about the";

char KAR_D_8005AC50[24] = "best order to use them.";

char KAR_D_8005AC68[28] = "Some stones have four pegs.";

char KAR_D_8005AC84[] = "They stick to where they land";

char KAR_D_8005ACA4[28] = "and can become very useful.";

char KAR_D_8005ACC0[] = "Move the Directional Pad ";

char KAR_D_8005ACDC[] = "left and right to choose the ";

char KAR_D_8005ACFC[] = "direction of your throw.";

char KAR_D_8005AD18[28] = "Move screen up or down with";

char KAR_D_8005AD34[] = "Dir. Pad. Use X button to";

char KAR_D_8005AD50[24] = "select throw direction.";

char KAR_D_8005AD68[] = "Press X button when power";

char KAR_D_8005AD84[] = "gauge reaches the desired";

char KAR_D_8005ADA0[] = "level to pick throw strength.";

char KAR_D_8005ADC0[28] = "If your curling stone lands";

char KAR_D_8005ADDC[] = "and touches the GOOD mark,";

char KAR_D_8005ADF8[20] = "you get two points.";

char KAR_D_8005AE0C[] = "If you get it on the blue";

char KAR_D_8005AE28[] = "line, but outside the center";

char KAR_D_8005AE48[] = "GOOD mark, one point.";

char KAR_D_8005AE60[24] = "If you touch the BAD on";

char KAR_D_8005AE78[] = "bottom right, you'll lose";

char KAR_D_8005AE94[] = "two points, so be careful.";

char KAR_D_8005AEB0[] = "And, in the case of a tie,";

char KAR_D_8005AECC[] = "So let's start the game!!";

char KAR_D_8005AEE8[] = "You're not that great!";

char KAR_D_8005AF00[] = "The battle is just beginning!";

char KAR_D_8005AF20[24] = "Hey you're pretty good!";

char KAR_D_8005AF38[] = "Yeah! I won!";

char KAR_D_8005AF48[] = "Awh, I lost. ";

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

#endif

KarOffTbl KAR_D_8005B04C = {
	{
		0x00, 0x0c, 0x14, 0x18, 0x28, 0x2c, 0x30, 0x34,
		0x38, 0x3c,
	},
};

#if defined(VERSION_JP)
char KAR_STR_METALMAMEMON[] = "メタルマメモン";
char KAR_METALMAMEMON_LINE_1[] = "「そうだな。まずはカーリングダマの";
char KAR_METALMAMEMON_LINE_2[] = "　タイプを選ぶ。」";
char KAR_METALMAMEMON_LINE_5[] = "「おれは天才だから、色々なタマを";
char KAR_METALMAMEMON_LINE_6[] = "　タイミングを良く使うことができる。";
char KAR_METALMAMEMON_LINE_7[] = "　それが勝つヒケツさっ！」";
char KAR_METALMAMEMON_LINE_9[] = "「４つ足がついているタマは";
char KAR_METALMAMEMON_LINE_10[] = "　止まったところにくっつくから、";
char KAR_METALMAMEMON_LINE_11[] = "　すっげー大事だぜ。」";
char KAR_METALMAMEMON_LINE_14[] = "　方向キーの左右で選べ。」";
char KAR_METALMAMEMON_LINE_18[] = "　スクロールできるぜ！";
char KAR_METALMAMEMON_LINE_19[] = "　投げる方向をきめたらａｈｉで決定だ。」";
char KAR_METALMAMEMON_LINE_21[] = "「最後に、投げる強さを決めろ。";
char KAR_METALMAMEMON_LINE_23[] = "　これも、ａｈｉで決定だ。」";
char KAR_METALMAMEMON_LINE_33[] = "「右下にあるＢＡＤにのせると";
char KAR_METALMAMEMON_LINE_34[] = "　－２点になるからせいぜい";
char KAR_METALMAMEMON_LINE_35[] = "　気をつけるんだな！」";
char KAR_METALMAMEMON_LINE_37[] = "「同点は、おれの勝ち！！";
char KAR_METALMAMEMON_LINE_38[] = "　それじゃ、ゲームスタートだ」";
char KAR_METALMAMEMON_LINE_41[] = "「フッ、このまま勝つ！」";
char KAR_METALMAMEMON_LINE_45[] = "「まだ負けたわけではない！！」";
char KAR_METALMAMEMON_LINE_49[] = "「この勝負、負けるわけには！」";
char KAR_METALMAMEMON_LINE_53[] = "「はっはっはっ！俺の勝ちだな！」";
char KAR_METALMAMEMON_LINE_57[] = "「くっ・・・あと少しのところを・・・」";

KarStrTbl KAR_D_8005B318 = {{
	KAR_STR_METALMAMEMON,
	KAR_METALMAMEMON_LINE_1,
	KAR_METALMAMEMON_LINE_2,
	KAR_STR_EMPTY,
	KAR_STR_METALMAMEMON,
	KAR_METALMAMEMON_LINE_5,
	KAR_METALMAMEMON_LINE_6,
	KAR_METALMAMEMON_LINE_7,
	KAR_STR_METALMAMEMON,
	KAR_METALMAMEMON_LINE_9,
	KAR_METALMAMEMON_LINE_10,
	KAR_METALMAMEMON_LINE_11,
	KAR_STR_METALMAMEMON,
	KAR_PENGUINMON_LINE_13,
	KAR_METALMAMEMON_LINE_14,
	KAR_STR_EMPTY,
	KAR_STR_METALMAMEMON,
	KAR_PENGUINMON_LINE_17,
	KAR_METALMAMEMON_LINE_18,
	KAR_METALMAMEMON_LINE_19,
	KAR_STR_METALMAMEMON,
	KAR_METALMAMEMON_LINE_21,
	KAR_PENGUINMON_LINE_22,
	KAR_METALMAMEMON_LINE_23,
	KAR_STR_METALMAMEMON,
	KAR_PENGUINMON_LINE_25,
	KAR_PENGUINMON_LINE_26,
	KAR_STR_EMPTY,
	KAR_STR_METALMAMEMON,
	KAR_PENGUINMON_LINE_29,
	KAR_PENGUINMON_LINE_30,
	KAR_STR_EMPTY,
	KAR_STR_METALMAMEMON,
	KAR_METALMAMEMON_LINE_33,
	KAR_METALMAMEMON_LINE_34,
	KAR_METALMAMEMON_LINE_35,
	KAR_STR_METALMAMEMON,
	KAR_METALMAMEMON_LINE_37,
	KAR_METALMAMEMON_LINE_38,
	KAR_STR_EMPTY,
	KAR_STR_METALMAMEMON,
	KAR_METALMAMEMON_LINE_41,
	KAR_STR_EMPTY,
	KAR_STR_EMPTY,
	KAR_STR_METALMAMEMON,
	KAR_METALMAMEMON_LINE_45,
	KAR_STR_EMPTY,
	KAR_STR_EMPTY,
	KAR_STR_METALMAMEMON,
	KAR_METALMAMEMON_LINE_49,
	KAR_STR_EMPTY,
	KAR_STR_EMPTY,
	KAR_STR_METALMAMEMON,
	KAR_METALMAMEMON_LINE_53,
	KAR_STR_EMPTY,
	KAR_STR_EMPTY,
	KAR_STR_METALMAMEMON,
	KAR_METALMAMEMON_LINE_57,
	KAR_STR_EMPTY,
	KAR_STR_EMPTY,
	KAR_STR_EMPTY,
}};

char KAR_STR_BUTTON_MARK_COL5[] = "　　　　　ａ";
KarStrPair KAR_METALMAMEMON_HIGHLIGHTS = { { KAR_STR_BUTTON_MARK_COL11, KAR_STR_BUTTON_MARK_COL5 } };
#else
char KAR_D_8005B058[] = " MetalMamemon ";

char KAR_D_8005B068[] = "Yeah. You gotta first pick";

char KAR_D_8005B084[] = "the type of curling stone.";

char KAR_D_8005B0A0[] = "I'm a genius, so I can use";

char KAR_D_8005B0BC[28] = "various stones in the right";

char KAR_D_8005B0D8[28] = "order. It's key to winning.";

char KAR_D_8005B0F4[] = "Stones with four pegs stop";

char KAR_D_8005B110[] = "and stay where they land,";

char KAR_D_8005B12C[] = "so they are very valuable.";

char KAR_D_8005B148[] = "Choose the direction of throw";

char KAR_D_8005B168[] = "with the Directional Pad,";

char KAR_D_8005B184[] = "by moving it left and right.";

char KAR_D_8005B1A4[] = "You can scroll screen moving";

char KAR_D_8005B1C4[] = "Directional Pad up and down.";

char KAR_D_8005B1E4[28] = "Press X button to throw it.";

char KAR_D_8005B200[] = "gauge reaches the desired ";

char KAR_D_8005B21C[] = "Points: If you get the";

char KAR_D_8005B234[24] = "stone on the GOOD mark,";

char KAR_D_8005B24C[] = "If you get on right bottom";

char KAR_D_8005B268[] = "BAD mark, it will be minus";

char KAR_D_8005B284[] = "In the case of a tie, I win!";

char KAR_D_8005B2A4[] = "Start the game!!";

char KAR_D_8005B2B8[] = "I'll win!";

char KAR_D_8005B2C4[20] = "I haven't lost yet!";

char KAR_D_8005B2D8[] = "I can't lose this match!";

char KAR_D_8005B2F4[] = "Yeah, I won!";

char KAR_D_8005B304[] = "Awh, I almost won.";

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

#endif

KarOffTbl KAR_D_8005B40C = {
	{
		0x00, 0x0c, 0x14, 0x18, 0x28, 0x2c, 0x30, 0x34,
		0x38, 0x3c,
	},
};

KarTallyValues MAIN_D_80134A44 = { { 2, 1, -2, 2 } };
KarRingFlags MAIN_D_80134A48 = { { 0 } };
KarPeggedModelIds MAIN_D_80134A4C = { { 11, 10, 9 } };
int8_t MAIN_D_80134A4F = -1;

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

KarRingObjects KAR_D_8005B450 = {
	{
		{0x0019, 0x0001},
		{0x0013, 0x0004},
		{0x0010, 0x0003},
		{0x000d, 0x0003},
	},
};

KarZones KAR_D_8005B460 = {
	{
		{0x0000, 0x06b6, 0x0091},
		{0x0000, 0x06b6, 0x0177},
		{0xfe70, 0x090e, 0x00cd},
		{0x0190, 0x090e, 0x00cd},
	},
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

GARBAGE(KAR_initializeOrderingTables, 6);

void KAR_initializeOrderingTables(void)
{
	KAR_D_800638CC[0].length = 5;
	KAR_D_800638CC[0].org = KAR_D_800637CC;
	KAR_D_800638CC[1].length = 5;
	KAR_D_800638CC[1].org = KAR_D_8006384C;
}

void KAR_setupMatch(int32_t mode)
{
	int32_t seed;
	int32_t obstacles;
	KarModelIds models;
	KarWeightTbl weights;
	KarOffTbl types;
#if !defined(VERSION_JP)
	KarSpawnX spawnX;
#endif
	int32_t n;
	int32_t p;
	int32_t i;
	KarStone *stone;

	obstacles = rand() % 3;
	MAIN_D_8013523A = 0;
	MAIN_D_8013523C = 0;
	MAIN_D_8013523E = 0;

	for (p = 0; p < 3; p++) {
		stone = KAR_D_8005B5A0[p].row.stones;
		seed = rand();
		KAR_D_8005B5A0[p].row.score = 0;
		KAR_D_8005B5A0[p].row.thrown = 0;
		KAR_D_8005B5A0[p].row.unk2 = 0;
		KAR_D_8005B5A0[p].row.unk4 = 0;

		for (i = 0; i < 5; i++, stone++) {
			models = KAR_D_8005AB80;
			weights = MAIN_D_80134A08;
			types = KAR_D_8005AB8C;

			n = rand();
			stone->speed = 0;
			stone->angle = 0;
			stone->ring = 0;
			stone->prevRing = 0;
			n = n % 10;
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
				stone->rotation.vx = 0x96;
				stone->rotation.vy = 0;
				stone->rotation.vz = 0;
				stone->state = -1;
			} else if (i < obstacles) {
#if defined(VERSION_JP)
				stone->pos.vy = 0;
				stone->pos.vx = rand() % 0x5dc - 0x2ee;
#else
				spawnX = KAR_D_8005AB98;
				stone->pos.vy = 0;
				stone->pos.vx = spawnX.x[i] - rand() % 0x13f;
#endif
				stone->pos.vz = -(rand() % 0x3e8 + 0x3e8);
				stone->rotation.vx = 0;
				stone->rotation.vy = 0;
				stone->rotation.vz = 0;
				stone->state = 1;
				stone->speed = 0;
			} else {
				stone->pos.vy = 0;
				stone->pos.vx = 0;
				stone->pos.vz = 0;
				stone->rotation.vx = 0;
				stone->rotation.vy = 0;
				stone->rotation.vz = 0;
				stone->state = -0x65;
				stone->speed = 0;
			}
			stone->weight = weights.weight[stone->type];
		}
	}
}

void KAR_start(void)
{
#ifdef __MWERKS__
	extern int32_t readPStat(int32_t index);
#endif

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
	MAIN_D_80135248 += 2;
}

void KAR_tickStones(int32_t instanceId)
{
	KarStone *stone;
	int32_t p;
	int32_t i;
	int16_t move;
	uint32_t speed;
	KarPeggedModelIds models;
	int16_t step;

	for (p = 0; p < 3; p++) {
		stone = KAR_D_8005B5A0[p].row.stones;
		for (i = 0; i < 5; i++, stone++) {
			if (stone->state != 3 && stone->state != 2) {
				stone->target = stone->pos;
			}
			if (MAIN_D_80135244 == 5) {
				if (i == MAIN_D_8013523A && p == MAIN_D_8013523C) {
					stone->rotation.vy += 0x14;
				} else {
					stone->rotation.vy = 0;
				}
			}
			if (stone->state > 0 && stone->speed >= 0) {
				move = stone->speed / stone->weight;
				speed = stone->speed * 98;
				stone->speed = speed / 100;
				if (move < 2) {
					stone->speed = 0;
				}
				if (stone->state < 3) {
					stone->pos.vx += move * rcos(stone->angle) / 4096;
					stone->pos.vz += move * rsin(stone->angle) / 4096;
				}
				if (stone->state == 3) {
					if (stone->speed > 0) {
						step = move / 10;
						if ((stone->angle -= 0x800) < 0) {
							stone->angle += 0x1000;
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
					stone->rotation.vx = stone->rotation.vy = stone->rotation.vz = 0;
				} else {
					stone->pos.vx = ENTITY_TABLE[idx]->posData[4].posMatrix.workm.t[0];
					stone->pos.vy = ENTITY_TABLE[idx]->posData[4].posMatrix.workm.t[1] + 0x78;
					stone->pos.vz = ENTITY_TABLE[idx]->posData[4].posMatrix.workm.t[2];
					stone->rotation.vx = stone->rotation.vy = stone->rotation.vz = 0;
				}
			}

			RotMatrix(&stone->rotation, &stone->coord.coord);
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

			if (stone->state < 0) {
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

	if (KAR_D_8005B5A0[0].row.score > KAR_D_8005B5A0[1].row.score) {
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

void KAR_renderAimArrow(void)
{
	MATRIX m;
	SVECTOR out;
	SVECTOR prev;
	SVECTOR vec;
	GsOT_TAG *ot;
	int32_t otz;
	int32_t i;
	DVECTOR sxy;
	SVECTOR pts[4];
	POLY_FT4 *prim;
	int32_t rot;
	int32_t k;
	int16_t w;
	int8_t player;
	uint8_t shade;

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

	for (i = 0; i < 0xf; i++) {
		vec.vx = 0;
		vec.vy = 0;
		vec.vz = (i + 1) * -200;
		RotMatrix(&ENTITY_TABLE[player]->posData->rotation, &m);
		ApplyMatrixSV(&m, &vec, &out);
		out.vx += KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].pos.vx;
		out.vz += KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].pos.vz;
		if ((shade = MAIN_D_80135252 + (i * 0xbf) / 15) >= 0xbf) {
			shade -= 0xbf;
		}
		while ((out.vx < -0x2ee) || (out.vx >= 0x2ef)) {
			if (out.vx < -0x2ee) {
				w = out.vx + 0x2ee;
				out.vx -= w * 2;
			}
			if (out.vx >= 0x2ef) {
				w = out.vx - 0x2ee;
				out.vx -= w * 2;
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
			rot = -rot;
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
	KarStone *stone;

	sprite = KAR_D_8005B478;
	stone = KAR_D_8005B5A0[0].row.stones;
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
	int16_t lo;
	int16_t hi;
	int16_t x;

	spr = KAR_D_8005B48C;
	dig = KAR_D_8005B4A0;
	for (i = 0; i < 2; i++) {
		lo = KAR_D_8005B5A0[i].row.score % 10;
		hi = KAR_D_8005B5A0[i].row.score / 10;
		x = (i != 0) ? 0x68 : -0x69;
		if (KAR_D_8005B5A0[i].row.score < 0) {
			hi = 10;
		}
		if (lo < 0) {
			lo *= -1;
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
	int16_t step;

	arr = KAR_D_8005B4CC;
	step = 250;
	if ((MAIN_D_80135244 >= 8) && (MAIN_D_80135244 < 0xa)) {
		arr.sprite[1].x = (MAIN_D_8013523C != 0) ? 0x32 : -0x32;
		arr.sprite[1].clutX = (MAIN_D_8013523C != 0) ? 0x80 : 0x70;

		for (i = 10; i > 0; i--) {
			if ((step * i) <= MAIN_D_8013523E) {
				arr.sprite[0].x = (arr.sprite[1].x + i * 3) - 3;
				if (i == 10) {
					arr.sprite[0].clutX = 0x30;
				} else {
					arr.sprite[0].clutX = 0x70;
				}
				KAR_renderSprite(&arr.sprite[0]);
			}
		}
		KAR_renderSprite(&arr.sprite[1]);
	}
}

void KAR_renderReadyPrompt(void)
{
	KarSprite sprite;

	sprite = KAR_D_8005B56C;
	if (MAIN_D_80135244 == 3) {
		if (MAIN_D_80135220++ >= 0x31) {
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
		moveCameraByOffset(dx, dy);
	}
}

void KAR_updateRingMarkers(void)
{
	int32_t zoneIndex;
	KarRingObjects objects;
	KarRingFlags flags;
	KarZones zones;
	int32_t distance;
	int8_t mask;
	int32_t p;
#if !defined(VERSION_JP)
	int32_t byteOffset;
#endif
	int32_t stoneIndex;
	KarStone *stone;

	objects = KAR_D_8005B450;
	flags = MAIN_D_80134A48;
	zones = KAR_D_8005B460;
	if (MAIN_D_80135244 == 0xB) {
		MAIN_D_80135257 = 0;
		for (p = 0; p < 2; p++) {
			p = p;
			stone = KAR_D_8005B5A0[p].row.stones;
			for (stoneIndex = 0; stoneIndex < 5; stoneIndex++, stone++) {
				if ((stone->state > 0) && (stone->speed > 0)) {
					for (zoneIndex = 0; zoneIndex < 4; zoneIndex++) {
						/* This never-null guard preserves the compiler's loop weighting. */
						if (!objects.ring) {
						}
						mask = (MAIN_D_80135257 >> zoneIndex) & 1;
						if (mask == 0) {
							distance = KAR_distance(stone->pos.vx + zones.ring[zoneIndex].dx,
							                        stone->pos.vz + zones.ring[zoneIndex].dz);
							if (distance < zones.ring[zoneIndex].radius) {
								mask = 1;
								mask <<= (int8_t)zoneIndex;
								MAIN_D_80135257 |= (int8_t)mask;
								zoneIndex = 4;
							}
						}
					}
				}
			}
		}
#if defined(VERSION_JP)
		for (p = 0; p < 4; p++) {
			if ((MAIN_D_80135257 >> p) & 1) {
				flags.ring[p] = 1;
			}
			setMapObjectsFlag(objects.ring[p].start, objects.ring[p].count, flags.ring[p] ^ 1);
		}
#else
		for (p = 0, byteOffset = 0; p < 4; p++, byteOffset += 4) {
			if ((MAIN_D_80135257 >> p) & 1) {
				flags.ring[p] = 1;
			}
			setMapObjectsFlag(*(int16_t *)((uint8_t *)&objects.starts + byteOffset),
			                  *(int16_t *)((uint8_t *)&objects.countView.counts + byteOffset), flags.ring[p] ^ 1);
			/* Preserve the output loop's weighting as well. */
			if (!objects.ring) {
			}
		}
#endif
	}
	if (MAIN_D_80135244 == 0xD) {
		int8_t ring;

		if ((KAR_D_800639C0[MAIN_D_80135256]->ring != KAR_D_800639C0[MAIN_D_80135256]->prevRing) &&
		    (KAR_D_800639C0[MAIN_D_80135256]->state < 0x65)) {
			ring = KAR_D_800639C0[MAIN_D_80135256]->ring & 0xF;
			if (ring != 0) {
				flags.ring[ring - 1] = 1;
			}
		}
		for (stoneIndex = 0; stoneIndex < 4; stoneIndex++) {
			setMapObjectsFlag(objects.ring[stoneIndex].start, objects.ring[stoneIndex].count,
			                  flags.ring[stoneIndex] ^ 1);
		}
	}
	if ((MAIN_D_80135244 != 0xD) && (MAIN_D_80135244 != 0xB)) {
		for (stoneIndex = 0; stoneIndex < 4; stoneIndex++) {
			setMapObjectsFlag(objects.ring[stoneIndex].start, objects.ring[stoneIndex].count,
			                  flags.ring[stoneIndex] ^ 1);
		}
	}
}

void KAR_checkStonesStopped(void)
{
	int32_t p;
	int32_t n;
	int8_t moving;
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
		stone2 = stone2;
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
	int32_t i;
	int32_t p;
	int32_t step;
	VECTOR cur[15];
	VECTOR prev[15];
	KarStone *stone;
	int32_t tx1;
	int32_t tx2;
	int32_t tz1;
	int32_t tz2;
	int32_t dist;
	int32_t a;
	int32_t b;
#if !defined(VERSION_JP)
	int32_t collided;
#endif

#if !defined(VERSION_JP)
	collided = 0;
#endif

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

			for (i = 0; i < 5; i++, stone++) {
				if (stone->state > 0) {
					tx1 = stone->target.vx * (10 - step);
					tx2 = stone->pos.vx * step;
					tz1 = stone->target.vz * (10 - step);
					tz2 = stone->pos.vz * step;
					cur[p * 5 + i].vx = (tx1 + tx2) / 10;
					cur[p * 5 + i].vz = (tz1 + tz2) / 10;
#if !defined(VERSION_JP)
					cur[p * 5 + i].vy = stone->target.vy;
#endif
				}
			}
		}

		for (a = 0; a < 15; a++) {
			if (KAR_D_8005B5A0[a / 5].row.stones[a % 5].state > 0) {
				for (b = a + 1; b < 15; b++) {
					if (KAR_D_8005B5A0[b / 5].row.stones[b % 5].state > 0) {
						dist = KAR_distance(cur[a].vx - cur[b].vx, cur[a].vz - cur[b].vz);
						if (dist < 0x96) {
#if !defined(VERSION_JP)
							collided = 1;
#endif
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

#if !defined(VERSION_JP)
		if ((collided != 0) && (step == 0xa)) {
			step--;
		}
		collided = 0;
#endif
	}
}

void KAR_bounceOffWall(void)
{
	int32_t p;
	int32_t n;
	VECTOR contact;
	int16_t zone;
	KarStone *stone;
	int16_t d;

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
				d = stone->pos.vx + 0x2d5;
				stone->pos.vx -= d * 2;
				stone->angle = 0x800 - stone->angle;
				break;
			case 2:
				stone->angle = 0x1800 - stone->angle;
				d = stone->pos.vx - 0x2d5;
				stone->pos.vx -= d * 2;
				break;
			case 3:
				KAR_findWallContact(&contact, stone, 0);
				KAR_reflectOffDiagonal((int32_t *)stone, 0);
				KAR_placeAtContact(stone, contact);
				break;
			case 4:
				KAR_findWallContact(&contact, stone, 1);
				KAR_reflectOffDiagonal((int32_t *)stone, 1);
				KAR_placeAtContact(stone, contact);
				break;
			case 5:
				stone->angle = -stone->angle;
				d = -0x9dd - stone->pos.vz;
				stone->pos.vz += d * 2;
				break;
			case 6:
				if (stone->target.vz < 0x465) {
					playSound2(8, 3);
					stone->angle = -stone->angle;
					d = stone->pos.vz - 0x465;
					stone->pos.vz -= d * 2;
				}
				break;
			}
		}
	}
}

int32_t KAR_distance(long x, long y)
{
	int32_t d;
	uint32_t xx;
	uint32_t yy;

	xx = x * x;
	yy = y * y;
	d = SquareRoot0(xx + yy);
	return d;
}

void KAR_resolveStoneCollision(KarStone *stoneA, VECTOR a, KarStone *stoneB, VECTOR b)
{
	int16_t speed;

	speed = stoneA->speed + stoneB->speed;
	if (speed >= 0xbb8) {
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
	int32_t dd;
	int32_t da;
	int32_t r;
	int32_t t;
	int32_t dist;

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
		dist = KAR_distance(mover->pos.vx - pegged->pos.vx, mover->pos.vz - pegged->pos.vz);
	} while (dist < 0x97);

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
	int32_t share;
	int32_t d;
	int32_t ang;
	int32_t n;
	VECTOR p1;
	VECTOR p2;
	int32_t r;

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
	r = 0;
	p1 = resting->pos;
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
		int32_t pushAng;

		pushAng = ratan2(mover->pos.vz - resting->pos.vz, mover->pos.vx - resting->pos.vx);
		while (d < 0x97) {
			mover->pos.vx = p2.vx + r * rcos(pushAng) / 4096;
			mover->pos.vz = p2.vz + r * rsin(pushAng) / 4096;
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
	int32_t ang;
	int32_t d;
	int32_t share1;
	int32_t share2;
	VECTOR p1;
	VECTOR p2;
	VECTOR delta;
	int32_t r;

	KAR_debugCollisionPair(stoneA, &a, stoneB, &b);
	share1 = KAR_computeImpactShare(stoneA, a, b);
	share2 = KAR_computeImpactShare(stoneB, b, a);
	KAR_computeSeparation(&p1, stoneB, b, a);
	KAR_computeSeparation(&p2, stoneA, a, b);
	delta.vx = p2.vx - p1.vx;
	delta.vz = p2.vz - p1.vz;
	ang = KAR_distance(delta.vx, delta.vz);
	if (ang != 0) {
		ang = ratan2(delta.vz, delta.vx);
		stoneB->pos.vx = stoneB->pos.vx + delta.vx;
		stoneB->pos.vz = stoneB->pos.vz + delta.vz;
		stoneA->pos.vx = stoneA->pos.vx - delta.vx;
		stoneA->pos.vz -= delta.vz;
		d = KAR_distance(stoneA->pos.vx - stoneB->pos.vx, stoneA->pos.vz - stoneB->pos.vz);
		r = 0;
		p1 = stoneA->pos;
		p2 = stoneB->pos;
		stoneA->speed += (int16_t)(share2 - share1);
		stoneB->speed += (int16_t)(share1 - share2);
		if (stoneA->speed > stoneB->speed) {
			while (d < 0x97) {
#if defined(VERSION_JP)
				stoneA->pos.vx = p1.vx - r * rcos(ang) / 4096;
				stoneA->pos.vz = p1.vz - r * rsin(ang) / 4096;
#else
				stoneA->pos.vx = p1.vx + r * rcos(ang) / 4096;
				stoneA->pos.vz = p1.vz + r * rsin(ang) / 4096;
#endif
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
		d = KAR_distance(stoneA->pos.vx - stoneB->pos.vx, stoneA->pos.vz - stoneB->pos.vz);
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
	int32_t r;
	int32_t ang2;
	int32_t ang;
	int32_t flag;
	int32_t dist;
	VECTOR base;

	flag = 1;
	base = *b;
	return;

	r = 0;
	ang2 = stone->angle;
	ang2 = (ang2 + 0x800) & 0xfff;
	do {
		flag = (flag != 0) ? 0 : 1;
		if (flag == 0) {
			b->vx = base.vx + r * rcos(ang) / 4096;
			b->vz = base.vz + r * rsin(ang) / 4096;
		} else {
			b->vx = base.vx + r * rcos(ang2) / 4096;
			b->vz = base.vz + r * rsin(ang2) / 4096;
		}
		dist = KAR_distance(b->vx - a->vx, b->vz - a->vz);
	} while (dist < 0x97);
}

int32_t KAR_computeImpactShare(KarStone *stone, VECTOR a, VECTOR b)
{
	int32_t ang;
	int32_t impactAng;
	int32_t d;
	int32_t ret;

	ang = stone->angle & 0xfff;
	impactAng = ratan2(b.vz - a.vz, b.vx - a.vx) & 0xfff;
	d = (ang - impactAng) & 0xfff;
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
	int32_t cur;
	int32_t ang;
	int32_t d;
	int32_t r;
	int32_t dist;
	int32_t n;
	VECTOR v;

	cur = stone->angle & 0xfff;
	ang = ratan2(b.vz - a.vz, b.vx - a.vx) & 0xfff;
	d = abs(cur - ang);
	v.vz = 0;
	v.vy = 0;
	v.vx = 0;
	if (d < 0x400 || d > 0xc00) {
		d = d % 0x400;
		dist = KAR_distance(stone->pos.vz - a.vz, stone->pos.vx - a.vx);
		r = (0x400 - d) * dist / 0x400;
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
	VECTOR baseA;
	VECTOR baseB;
	int32_t angA2;
	int32_t angB2;
	int32_t angA;
	int32_t angB;
	int32_t dist;
	int32_t flag;
	int32_t r;

	flag = 1;
	return;

	baseA = *a;
	baseB = *b;
	angA = stoneA->angle;
	angB = stoneB->angle;
	angA2 = (angA + 0x800) & 0xfff;
	angB2 = (angB + 0x800) & 0xfff;
	r = 0;
	do {
		flag = (flag != 0) ? 0 : 1;
		if (flag == 0) {
			a->vx = baseA.vx + r * rcos(angA) / 4096;
			a->vz = baseA.vz + r * rsin(angA) / 4096;
			b->vx = baseB.vx + r * rcos(angB) / 4096;
			b->vz = baseB.vz + r * rsin(angB) / 4096;
		} else {
			a->vx = baseA.vx + r * rcos(angA2) / 4096;
			a->vz = baseA.vz + r * rsin(angA2) / 4096;
			b->vx = baseB.vx + r * rcos(angB2) / 4096;
			b->vz = baseB.vz + r * rsin(angB2) / 4096;
		}
		dist = KAR_distance(a->vx - b->vx, a->vz - b->vz);
		if (r >= 0x96) {
			*a = baseA;
			*b = baseB;
			break;
		}
	} while (dist < 0x97);
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

// clang-format off
int32_t KAR_findWallContact(out, stone, flag)
	VECTOR *out;
	KarStone *stone;
	int8_t flag;
// clang-format on
{
	int32_t t;
	int32_t i;
	VECTOR v;
	int32_t ang;

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

// clang-format off
void KAR_reflectOffDiagonal(p, dir)
	int32_t *p;
	int8_t dir;
// clang-format on
{
	int32_t cur;
	int32_t ang;

	cur = (p[3] + 0x800) & 0xfff;
	if (dir == 0) {
		ang = ratan2(1, 2);
	} else {
		ang = ratan2(1, -2);
	}

	p[3] = ang * 2 - cur;
}

void KAR_placeAtContact(KarStone *stone, VECTOR p)
{
	int32_t d;

	d = KAR_distance(p.vx - stone->target.vx, p.vz - stone->target.vz);
	d = KAR_distance(stone->target.vz - stone->pos.vz, stone->target.vx - stone->pos.vx) - d;
	stone->pos.vx = p.vx + ((d * rcos(stone->angle)) / 4096);
	stone->pos.vz = p.vz + ((d * rsin(stone->angle)) / 4096);
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

// clang-format off
void KAR_turnAimLeft(delta)
	int16_t delta;
// clang-format on
{
	int8_t idx;

	idx = (MAIN_D_8013523C != 0) ? MAIN_D_80135248 : 0;
	ENTITY_TABLE[idx]->posData->rotation.vy -= delta;
	MAIN_D_8013524C += delta;
}

// clang-format off
void KAR_turnAimRight(delta)
	int16_t delta;
// clang-format on
{
	int8_t idx;

	idx = (MAIN_D_8013523C != 0) ? MAIN_D_80135248 : 0;
	ENTITY_TABLE[idx]->posData->rotation.vy -= delta;
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

int8_t KAR_tickYesNoPrompt(void)
{
	RECT finalPos;
	RECT startPos;
	KarStrPair strs;

	finalPos = MAIN_D_80134A10;
	startPos = MAIN_D_80134A18;
	strs = MAIN_D_80134A20;
	while (MAIN_D_80135250 < 2) {
		drawString(strs.text[MAIN_D_80135250], 0, (MAIN_D_80135250 * 13) + 0xdd);
		MAIN_D_80135250++;
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
		if ((POLLED_INPUT & CONFIRM_BUTTON) != 0 && (POLLED_INPUT_PREVIOUS & CONFIRM_BUTTON) == 0) {
			removeAnimatedUIBox(0, NULL);
		} else {
			if ((POLLED_INPUT & 0x1000) != 0 && (POLLED_INPUT_PREVIOUS & 0x1000) == 0) {
				MAIN_D_80135250 = (MAIN_D_80135250 == 3) ? 4 : 3;
				playSound(0, 2);
			}
#if !defined(VERSION_JP)
			else
#endif
			if ((POLLED_INPUT & 0x4000) != 0 && (POLLED_INPUT_PREVIOUS & 0x4000) == 0) {
				MAIN_D_80135250 = (MAIN_D_80135250 == 3) ? 4 : 3;
				playSound(0, 2);
			}
#if defined(VERSION_JP)
			renderString(0, UI_BOX_DATA[0].finalPos.x + 6, UI_BOX_DATA[0].finalPos.y + 6, 0xfc, 0x1a, 0, 0xdc, 6, 1);
			renderSelectionCursor(finalPos.x + 6, finalPos.y + 7 + ((MAIN_D_80135250 - 3) * 13), 0x80, 0xd, 6);
#else
			renderString(0, -0x7c, 0x30, 0x52, 0xd, 0, 0xdc, 6, 1);
			renderString(0, -0x7c, 0x3d, 0x5c, 0xd, 0x53, 0xdc, 6, 1);
			renderSelectionCursor(finalPos.x + 6, finalPos.y + 7 + ((MAIN_D_80135250 - 3) * 13), 0x5e, 0xd, 6);
#endif
		}
	}

	return 0;
}

int32_t KAR_tickHintBox(int32_t n)
{
	RECT finalPos;
	RECT startPos;

	finalPos = MAIN_D_80134A28;
	startPos = MAIN_D_80134A30;
	if (MAIN_D_80135250 == 0) {
		return 0;
	}

	if (n < 4) {
		startPos.x = 0x4b;
		startPos.y = -5;
	}

	if (UI_BOX_DATA[0].state == 0) {
		if (MAIN_D_80135250 == 1) {
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
		if ((POLLED_INPUT & CONFIRM_BUTTON) != 0 && (POLLED_INPUT_PREVIOUS & CONFIRM_BUTTON) == 0) {
			MAIN_D_80135250++;
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
	int32_t p;
	int32_t dist;
	int32_t i;
	int32_t zi;

	for (p = 0; p < 2; p++) {
		stone = KAR_D_8005B5A0[p].row.stones;
		KAR_D_8005B5A0[p].row.score = 0;
		for (i = 0; i < 5; i++, stone++) {
			stone->prevRing = stone->ring;
			stone->ring = 0;
			if (stone->state <= 0) {
				continue;
			}
			if (stone->pos.vz >= -0x564) {
				continue;
			}
			dist = KAR_distance(stone->pos.vx, stone->pos.vz + 0x6a4);
			for (zi = 0; zi < 4; zi++) {
				KarZones z;

				z = KAR_D_8005B438;
				dist = KAR_distance(stone->pos.vx + z.ring[zi].dx, stone->pos.vz + z.ring[zi].dz);
				if (dist > z.ring[zi].radius) {
					continue;
				}
				stone->ring = zi + 1;
				if (p != 0) {
					stone->ring |= 0x10;
				}
				break;
			}
		}
	}

	KAR_registerThrownStone();
}

int8_t KAR_tickScoreTally(void)
{
	KarTallyValues values;
	int32_t done;
	int32_t j;
	int32_t i;
	KarStone *rowStone;

	values = MAIN_D_80134A44;
	if ((POLLED_INPUT & CONFIRM_BUTTON) != 0 && (POLLED_INPUT_PREVIOUS & CONFIRM_BUTTON) == 0) {
		MAIN_D_8013522D = 1;
	}

	if (MAIN_D_8013522D == 1 && (MAIN_D_8013522E == 2 || MAIN_D_8013522E == 1)) {
		int8_t player;
		int8_t ring;

		MAIN_D_80135256 = KAR_D_8005B5A0[0].row.thrown + KAR_D_8005B5A0[1].row.thrown;
		MAIN_D_80135256--;
		KAR_D_8005B5A0[1].row.score = KAR_D_8005B5A0[0].row.score = 0;
		while (MAIN_D_80135256 >= 0) {
			player = (KAR_D_800639C0[MAIN_D_80135256]->ring & 0x10) ? 1 : 0;
			ring = KAR_D_800639C0[MAIN_D_80135256]->ring & 0xf;
			if (KAR_D_800639C0[MAIN_D_80135256]->state > 0 && ring != 0) {
				KAR_D_8005B5A0[player].row.score += values.value[ring - 1];
			}
			MAIN_D_80135256--;
		}

		for (i = 0; i < 3; i++) {
			rowStone = KAR_D_8005B5A0[i].row.stones;
			for (j = 0; j < 5; j++, rowStone++) {
				if (rowStone->state >= 100) {
					rowStone->state -= 100;
				}
			}
		}
		MAIN_D_80135230 = 0;
		MAIN_D_80135234 = 0;
		MAIN_D_8013522D = MAIN_D_8013522E = 0;
		return 1;
	}

	if (MAIN_D_80134A4F != MAIN_D_80135256) {
		int8_t ring;

		while (MAIN_D_80135256 >= 0) {
			ring = KAR_D_800639C0[MAIN_D_80135256]->ring & 0xf;
			if (ring != 0) {
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
		if (KAR_D_800639C0[MAIN_D_80135256]->ring != KAR_D_800639C0[MAIN_D_80135256]->prevRing) {
			done = tickCameraMoveTo(KAR_D_800639C0[MAIN_D_80135256]->pos.vx,
			                        KAR_D_800639C0[MAIN_D_80135256]->pos.vz, 5);
			if ((MAP_TILE_DATA.cameraY % 0x80) == 0 || (MAP_TILE_DATA.cameraY % 0x80) >= 0x6a) {
				uploadMapTileImages(MAP_TILE_DATA.tiles, MAP_TILE_X + (MAP_TILE_Y * (int16_t)MAP_TILE_DATA.width));
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
		int8_t player;
		int8_t ring;

		player = (KAR_D_800639C0[MAIN_D_80135256]->ring & 0x10) ? 1 : 0;
		ring = KAR_D_800639C0[MAIN_D_80135256]->ring & 0xf;
		KAR_D_8005B5A0[player].row.score += values.value[ring - 1];
		MAIN_D_8013522E = 2;
		if (KAR_D_800639C0[MAIN_D_80135256]->ring != KAR_D_800639C0[MAIN_D_80135256]->prevRing &&
		    (KAR_D_800639C0[MAIN_D_80135256]->ring & 0xf) != 0) {
			if (ring == 3) {
				playSound2(8, 6);
			} else {
				playSound2(8, 5);
			}
		}
	}

	if (MAIN_D_8013522E == 2 && MAIN_D_80135256 >= 0) {
		if (KAR_D_800639C0[MAIN_D_80135256]->ring != KAR_D_800639C0[MAIN_D_80135256]->prevRing &&
		    (KAR_D_800639C0[MAIN_D_80135256]->ring & 0xf) != 0) {
			if (MAIN_D_80135234++ >= 0xb) {
				if (KAR_D_800639C0[MAIN_D_80135256]->state < 100) {
					KAR_D_800639C0[MAIN_D_80135256]->state += 100;
				} else {
					KAR_D_800639C0[MAIN_D_80135256]->state -= 100;
				}
				MAIN_D_80135234 -= 10;
				if (MAIN_D_80135230++ >= 7) {
					MAIN_D_80135230 = 0;
					MAIN_D_8013522E = 4;
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
	KarStone *stone;
	int32_t clear;
	int32_t i;
	int16_t tx;
	int16_t tz;
	int32_t angle;
	int16_t px;
	int16_t pz;

	if (MAIN_D_80135248 == 2) {
		angle = KAR_findClearShotAngle(0, -0x6a4);
		if (angle == -1) {
			if (KAR_D_8005B5A0[0].row.score > KAR_D_8005B5A0[1].row.score) {
				stone = KAR_D_8005B5A0[1].row.stones;
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
				switch (KAR_D_80063914.stone.type) {
				case 0:
				case 1:
				case 2:
					if ((KAR_D_80063914.stone.ring == 3) || (KAR_D_80063914.stone.ring == 0)) {
						angle = ratan2(-0x8fc - pz, -0x190 - px);
						clear = KAR_findClearShotAngle(-0x190, -0x8fc);
						KAR_setOpponentShot(1, angle, -0x190, -0x8fc);
					} else if (KAR_D_80063914.stone.state < 0) {
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
							angle = ratan2(KAR_D_80063914.stone.pos.vz - pz, KAR_D_80063914.stone.pos.vx - px);
							KAR_setOpponentShot(0, angle, (int16_t)KAR_D_80063914.stone.pos.vx, (int16_t)KAR_D_80063914.stone.pos.vz);
						}
					} else {
						angle = KAR_aimBankShot(&KAR_D_80063914.stone, 0x190, -0x8fc);
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
#if defined(VERSION_JP)
	KarStrPair highlights;
#endif
	KarOffTbl offs;

	strs = KAR_D_8005AF58;
#if defined(VERSION_JP)
	highlights = KAR_PENGUINMON_HIGHLIGHTS;
#endif
	offs = KAR_D_8005B04C;
	if ((n % 4) == 0) {
		setTextColor(7);
	} else {
		setTextColor(1);
	}

	drawString(strs.text[offs.start[idx] + n], 0, (n * 13) + 1);
#if defined(VERSION_JP)
	if ((idx == 1) && (n == 7)) {
		setTextColor(10);
		drawString(highlights.text[0], 0, (n * 13) + 1);
	}
	if ((idx == 2) && (n == 3)) {
		setTextColor(10);
		drawString(highlights.text[1], 0, (n * 13) + 1);
	}
#endif

	return (int8_t)((offs.start[idx + 1] - offs.start[idx]) / 4);
}

int32_t KAR_drawHintPageMetalMamemon(int32_t idx, int8_t n)
{
	KarStrTbl strs;
#if defined(VERSION_JP)
	KarStrPair highlights;
#endif
	KarOffTbl offs;

	strs = KAR_D_8005B318;
#if defined(VERSION_JP)
	highlights = KAR_METALMAMEMON_HIGHLIGHTS;
#endif
	offs = KAR_D_8005B40C;
	if ((n % 4) == 0) {
		setTextColor(7);
	} else {
		setTextColor(1);
	}

	drawString(strs.text[offs.start[idx] + n], 0, (n * 13) + 1);
#if defined(VERSION_JP)
	if ((idx == 1) && (n == 7)) {
		setTextColor(10);
		drawString(highlights.text[0], 0, (n * 13) + 1);
	}
	if ((idx == 2) && (n == 3)) {
		setTextColor(10);
		drawString(highlights.text[1], 0, (n * 13) + 1);
	}
#endif

	return (int8_t)((offs.start[idx + 1] - offs.start[idx]) / 4);
}

int32_t KAR_findClearShotAngle(int16_t x, int16_t z)
{
	int32_t n;
	int32_t p;
	int32_t ang;
	int32_t dist;
	int32_t ret;
	KarStone *stone;
	int16_t px;
	int16_t pz;
	int16_t step;

	px = ENTITY_TABLE[MAIN_D_80135248]->posData->location.vx;
	pz = ENTITY_TABLE[MAIN_D_80135248]->posData->location.vz;
	if (MAIN_D_80135248 != 2) {
		px = ENTITY_TABLE[MAIN_D_80135248]->posData[4].posMatrix.workm.t[0];
		pz = ENTITY_TABLE[MAIN_D_80135248]->posData[4].posMatrix.workm.t[2];
	}
	ang = ratan2(z - pz, x - px);
	if (MAIN_D_80135248 == 2) {
		step = 30;
	} else {
		step = 10;
	}
	while (z < pz) {
		px += (int16_t)((step * rcos(ang)) / 4096);
		pz += (int16_t)((step * rsin(ang)) / 4096);
		for (p = 0; p < 3; p++) {
			stone = KAR_D_8005B5A0[p].row.stones;
			for (n = 0; n < 5; n++, stone++) {
				if (stone->state > 0) {
					dist = KAR_distance(px - stone->pos.vx, pz - stone->pos.vz);
					if (dist < 0x97) {
						if (MAIN_D_80135248 == 2) {
							return -1;
						}
						ret = ratan2(stone->pos.vz - z, stone->pos.vx - x);
						return ret;
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

// clang-format off
int32_t KAR_aimAtStoneInRing(player, key, outX, outZ)
	int8_t player;
	int8_t key;
	int16_t *outX;
	int16_t *outZ;
// clang-format on
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

void KAR_setOpponentShot(int8_t row, int32_t val, int16_t b, int16_t c)
{
	int32_t i;
	KarStone *stone;
	int32_t n;
	int32_t power;

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
	power = KAR_computeThrowPower(stone->weight, b, c);
	KAR_D_8005B5A0[1].row.unk2 = power;
}

int32_t KAR_aimAtRandomStone(void)
{
	uint8_t count;
	int32_t i;
	int32_t ret;
	KarStone *cands[5];
	KarStone *stone;
	int16_t px;
	int16_t pz;

	stone = KAR_D_8005B5A0[0].row.stones;
	if (KAR_D_80063914.stone.state < 0) {
		ret = ratan2(-0xc1c, -0x190);
		return ret;
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

	ret = ratan2(pz - 0x578, px - 0x190);
	return ret;
}

// clang-format off
int32_t KAR_aimBankShot(stone, x, z)
	KarStone *stone;
	int16_t x;
	int16_t z;
// clang-format on
{
	int32_t ang;
	int32_t cx;
	int32_t ret;
	int32_t cz;
	int32_t t;

	ang = ratan2(stone->pos.vz - z, stone->pos.vx - x);
	cx = stone->pos.vx + ((rcos(ang) * 150) / 4096);
	cz = stone->pos.vz + ((rsin(ang) * 150) / 4096);
	t = cx + 0x2d5;
	cx = -0x2d5 - t;
	ret = ratan2(cz - ENTITY_TABLE[MAIN_D_80135248]->posData[4].posMatrix.workm.t[2],
		     cx - ENTITY_TABLE[MAIN_D_80135248]->posData[4].posMatrix.workm.t[0]);
	return ret;
}

// clang-format off
int32_t KAR_findUnusedStoneOfType(key)
	int8_t key;
// clang-format on
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

// clang-format off
int32_t KAR_computeThrowPower(a, b, c)
	uint32_t a;
	int16_t b;
	int16_t c;
// clang-format on
{
	int32_t dist;
	uint32_t acc;
	uint32_t value;
	uint32_t cur;

	dist = KAR_distance(0x190 - b, 0x578 - c);
	acc = 0;
	cur = a;
	value = cur * 100;
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

/* Definition order preserves compiler scheduling; kar_functions preserves link order. */
void KAR_tickMatchState(void)
{
	int32_t j;
	int32_t i;
	int8_t prompt;
	int8_t player;

	player = (MAIN_D_8013523C != 0) ? MAIN_D_80135248 : 0;
	switch (MAIN_D_80135244) {
	case 0:
		MAIN_D_80135252 = 0;
		ENTITY_TABLE[0]->posData->location.vx = -0x190;
		ENTITY_TABLE[0]->posData->location.vz = 0x578;
		ENTITY_TABLE[0]->posData->rotation.vy = 0;
		ENTITY_TABLE[MAIN_D_80135248]->posData->location.vx = 0x190;
		ENTITY_TABLE[MAIN_D_80135248]->posData->location.vz = 0x578;
		ENTITY_TABLE[MAIN_D_80135248]->posData->rotation.vy = 0;
		startAnimation(ENTITY_TABLE[0], 0);
		startAnimation(ENTITY_TABLE[MAIN_D_80135248], 0);
		MAIN_D_80135244 = 1;
		clearTextArea();
		MAIN_D_80135250 = 0;
		break;
	case 1:
		prompt = KAR_tickYesNoPrompt();
		if (prompt == 1) {
			MAIN_D_80135244 = 3;
			MAIN_D_80135250 = 0;
		} else if (prompt == 2) {
			clearTextArea();
			MAIN_D_80135244 = 2;
			MAIN_D_80135250 = 0;
			KAR_setupMatch(1);
		}
		break;
	case 3:
		if (MAIN_D_80135220 >= 0x30) {
			if ((uint16_t)(MAIN_D_80135252 += 6) >= 0x169) {
				MAIN_D_80135252 = 0;
				MAIN_D_80135244 = 5;
			}
		}
		break;
	case 2:
		if ((uint16_t)(MAIN_D_80135252 += 6) >= 0x169) {
			MAIN_D_80135252 = 0;
			MAIN_D_80135244 = 4;
		}
		break;
	case 4:
		MAIN_D_80135250 = 1;
		MAIN_D_80135244 = 5;
		KAR_D_8005B5A0[0].row.unk4 = rand() % 5;
		KAR_D_8005B5A0[0].row.unk2 = 0x7D0;
		KAR_D_8005B5A0[0].row.stones[KAR_D_8005B5A0[0].row.unk4].angle = ratan2(-0xC1C, 0x190);
		break;
	case 5:
		if ((MAIN_D_8013523C != 0) || (MAIN_D_80135250 != 0)) {
			if (KAR_tickHintBox(0) != 0) {
				break;
			}
			if (MAIN_D_80135252++ >= 0x10) {
				MAIN_D_80135252 -= 0xF;
				if (MAIN_D_8013523A == KAR_D_8005B5A0[MAIN_D_8013523C].row.unk4) {
					KAR_D_8005B5A0[2].row.stones[4] =
						KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A];
					KAR_D_8005B5A0[2].row.stones[4].state = -0x65;
					KAR_beginAiming();
					MAIN_D_80135252 = 0;
					if (MAIN_D_80135250 != 0) {
						MAIN_D_80135250 = 1;
					}
				}
				if (KAR_D_8005B5A0[MAIN_D_8013523C].row.unk4 > MAIN_D_8013523A) {
					KAR_selectNextStone();
				}
			}
		} else {
			if ((POLLED_INPUT & CONFIRM_BUTTON) && !(POLLED_INPUT_PREVIOUS & CONFIRM_BUTTON)) {
				KAR_beginAiming();
			}
#if !defined(VERSION_JP)
			else
#endif
			if ((POLLED_INPUT & 0x1000) && !(POLLED_INPUT_PREVIOUS & 0x1000)) {
				KAR_selectPreviousStone();
			}
#if !defined(VERSION_JP)
			else
#endif
			if ((POLLED_INPUT & 0x4000) && !(POLLED_INPUT_PREVIOUS & 0x4000)) {
				KAR_selectNextStone();
			}
		}
		MAIN_D_80135254 = 0;
		break;
	case 6:
		if ((MAIN_D_8013523C != 0) || (MAIN_D_80135250 != 0)) {
			int32_t targetAngle;

			targetAngle = KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].angle;
#if defined(VERSION_JP)
			if (KAR_tickHintBox(1) != 0) {
				if ((POLLED_INPUT & 0x2000) && (MAIN_D_8013524C < -0x18F)) {
					if (MAIN_D_80135254 > 0) {
						MAIN_D_80135254++;
					} else {
						MAIN_D_80135254 = 1;
					}
					KAR_turnAimLeft(MAIN_D_80135254);
				}
				if ((POLLED_INPUT & 0x8000) && (MAIN_D_8013524C >= -0x5B6)) {
					if (MAIN_D_80135254 < 0) {
						MAIN_D_80135254--;
					} else {
						MAIN_D_80135254 = -1;
					}
					KAR_turnAimRight(MAIN_D_80135254);
				}
				if (!(POLLED_INPUT & 0x8000) && !(POLLED_INPUT & 0x2000)) {
					MAIN_D_80135254 = 0;
				}
			} else if ((targetAngle <= MAIN_D_8013524C + 0xA) &&
			           (targetAngle >= MAIN_D_8013524C - 0xA)) {
				KAR_beginThrow();
				if (MAIN_D_80135250 != 0) {
					MAIN_D_80135250 = 1;
				}
			} else if (targetAngle > MAIN_D_8013524C) {
				KAR_turnAimLeft(0xA);
			} else {
				KAR_turnAimRight(-0xA);
			}
#else
			if (KAR_tickHintBox(1) != 0) {
				if ((POLLED_INPUT & 0x2000) && (MAIN_D_8013524C < -0x18F)) {
					if (MAIN_D_80135254 > 0) {
						MAIN_D_80135254++;
					} else {
						MAIN_D_80135254 = 1;
					}
					KAR_turnAimLeft(MAIN_D_80135254);
				} else if ((POLLED_INPUT & 0x8000) && (MAIN_D_8013524C >= -0x5B6)) {
					if (MAIN_D_80135254 < 0) {
						MAIN_D_80135254--;
					} else {
						MAIN_D_80135254 = -1;
					}
					KAR_turnAimRight(MAIN_D_80135254);
				} else if (!(POLLED_INPUT & 0x8000) && !(POLLED_INPUT & 0x2000)) {
					MAIN_D_80135254 = 0;
					break;
				} else {
					break;
				}
			}
			if ((MAIN_D_8013524C + 0xA >= targetAngle) &&
			    (targetAngle >= MAIN_D_8013524C - 0xA)) {
				KAR_beginThrow();
				if (MAIN_D_80135250 != 0) {
					MAIN_D_80135250 = 1;
				}
			} else if (MAIN_D_8013524C < targetAngle) {
				KAR_turnAimLeft(0xA);
			} else {
				KAR_turnAimRight(-0xA);
			}
#endif
		} else {
			if ((POLLED_INPUT & CONFIRM_BUTTON) && !(POLLED_INPUT_PREVIOUS & CONFIRM_BUTTON)) {
				KAR_beginThrow();
			}
			if ((POLLED_INPUT & 0x2000) && (MAIN_D_8013524C < -0x18F)) {
				if (MAIN_D_80135254 > 0) {
					MAIN_D_80135254++;
				} else {
					MAIN_D_80135254 = 1;
				}
				KAR_turnAimLeft(MAIN_D_80135254);
			}
			if ((POLLED_INPUT & 0x8000) && (MAIN_D_8013524C >= -0x5B6)) {
				if (MAIN_D_80135254 < 0) {
					MAIN_D_80135254--;
				} else {
					MAIN_D_80135254 = -1;
				}
				KAR_turnAimRight(MAIN_D_80135254);
			}
			if (!(POLLED_INPUT & 0x8000) && !(POLLED_INPUT & 0x2000)) {
				MAIN_D_80135254 = 0;
			}
		}
		break;
	case 7:
		MAIN_D_80135224 = tickCameraMoveTo(
			KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].pos.vx,
			KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].pos.vz, 0x14);
		if (((MAP_TILE_DATA.cameraY % 0x80) == 0) || ((MAP_TILE_DATA.cameraY % 0x80) >= 0x6A)) {
			uploadMapTileImages(MAP_TILE_DATA.tiles, MAP_TILE_X + (MAP_TILE_Y * (int16_t)MAP_TILE_DATA.width));
		}
		if (MAIN_D_80135224 == 1) {
			MAIN_D_80135244 = 8;
		}
		MAIN_D_80135254 = 0;
		break;
	case 8:
		if ((MAIN_D_8013523E += 0x64) >= 0xBB8) {
			MAIN_D_8013523E = 0;
			MAIN_D_80135244 = 6;
			ENTITY_TABLE[0]->posData->location.vx = -0x190;
			ENTITY_TABLE[0]->posData->location.vz = 0x578;
			ENTITY_TABLE[MAIN_D_80135248]->posData->location.vx = 0x190;
			ENTITY_TABLE[MAIN_D_80135248]->posData->location.vz = 0x578;
			if (MAIN_D_8013523C == 0) {
				startAnimation(ENTITY_TABLE[0], 0);
			} else {
				startAnimation(ENTITY_TABLE[MAIN_D_80135248], 0);
			}
		} else if ((MAIN_D_8013523C != 0) || (MAIN_D_80135250 != 0)) {
			if (KAR_tickHintBox(2) != 0) {
				if (KAR_D_8005B5A0[MAIN_D_8013523C].row.unk2 <= MAIN_D_8013523E) {
					MAIN_D_8013523E = KAR_D_8005B5A0[MAIN_D_8013523C].row.unk2;
				}
			} else if (KAR_D_8005B5A0[1].row.unk2 <= MAIN_D_8013523E) {
				MAIN_D_80135244 = 9;
				KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].rotation.vy =
					ENTITY_TABLE[MAIN_D_80135248]->posData->rotation.vy;
				if (MAIN_D_80135250 != 0) {
					MAIN_D_80135250 = 1;
				}
			}
		} else if ((POLLED_INPUT & CONFIRM_BUTTON) && !(POLLED_INPUT_PREVIOUS & CONFIRM_BUTTON)) {
			MAIN_D_80135244 = 9;
			KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].angle =
				MAIN_D_8013524C;
			KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].rotation.vy =
				TAMER_ENTITY.entity.posData->rotation.vy;
		}
		break;
	case 9:
		if (MAIN_D_8013523C == 0) {
			startAnimation(ENTITY_TABLE[0], 0x25);
		} else if (MAIN_D_80135248 == 2) {
			startAnimation(ENTITY_TABLE[MAIN_D_80135248], 0x1E);
		} else {
			startAnimation(ENTITY_TABLE[MAIN_D_80135248], 0x20);
		}
		KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].speed =
			MAIN_D_8013523E + 0x258;
		MAIN_D_80135244 = 0xA;
		MAIN_D_80135252 = 0;
		MAIN_D_80135228 = playSound2(8, 0);
		KAR_D_800638F4 = KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].pos;
		break;
	case 0xA:
		if (MAIN_D_80135252++ >= 3) {
			MAIN_D_80135252 = 0;
			KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].state = 1;
			KAR_D_800638F4 =
				KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].pos;
			MAIN_D_80135244 = 0xB;
		}
		break;
	case 0xB:
		KAR_D_80063904 = KAR_D_800638F4;
		KAR_D_800638F4 =
			KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[MAIN_D_8013523A].pos;
		moveCameraByDiff(&KAR_D_80063904, &KAR_D_800638F4);
		break;
	case 0xC: {
		KarStone *stone;

		thunkStopSoundMask(MAIN_D_80135228);
		KAR_classifyStoneRings();
		KAR_D_8005B5A0[MAIN_D_8013523C].row.thrown++;
		MAIN_D_80135244 = 0xD;
		for (i = 0; i < 2; i++) {
			stone = KAR_D_8005B5A0[i].row.stones;
			KAR_D_8005B5A0[i].row.thrown = 0;
			for (j = 0; j < 5; j++, stone++) {
				if (stone->state != -1) {
					KAR_D_8005B5A0[i].row.thrown++;
				}
			}
		}
		break;
	}
	case 0xD:
		prompt = KAR_tickScoreTally();
		if (prompt) {
		}
		if (prompt != 0) {
			if (MAIN_D_80135250 != 0) {
				MAIN_D_80135244 = 0xE;
			} else {
				MAIN_D_80135244 = 0xF;
			}
		}
		break;
	case 0xE:
		MAIN_D_80135224 = tickCameraMoveTo(0, -0x6A4, 0xA);
		if (((MAP_TILE_DATA.cameraY % 0x80) == 0) || ((MAP_TILE_DATA.cameraY % 0x80) >= 0x6A)) {
			uploadMapTileImages(MAP_TILE_DATA.tiles, MAP_TILE_X + (MAP_TILE_Y * (int16_t)MAP_TILE_DATA.width));
		}
		if (MAIN_D_80135224 == 1) {
			MAIN_D_80135244 = 0xF;
		}
		break;
	case 0xF:
		if (KAR_tickHintBox(3) == 0) {
			TAMER_ENTITY.entity.posData->location.vx = -0x190;
			TAMER_ENTITY.entity.posData->location.vz = 0x578;
			ENTITY_TABLE[MAIN_D_80135248]->posData->location.vx = 0x190;
			ENTITY_TABLE[MAIN_D_80135248]->posData->location.vz = 0x578;
			startAnimation(ENTITY_TABLE[0], 0);
			startAnimation(ENTITY_TABLE[MAIN_D_80135248], 0);
			if (MAIN_D_8013523C == 0) {
				KAR_D_80063914.stone =
					KAR_D_8005B5A0[0].row.stones[MAIN_D_8013523A];
			}
			if (MAIN_D_80135250 != 0) {
				MAIN_D_80135224 = tickCameraMoveTo(ENTITY_TABLE[player]->posData->location.vx,
				                                   ENTITY_TABLE[player]->posData->location.vz, 0x14);
				if (((MAP_TILE_DATA.cameraY % 0x80) == 0) || ((MAP_TILE_DATA.cameraY % 0x80) >= 0x6A)) {
					uploadMapTileImages(MAP_TILE_DATA.tiles, MAP_TILE_X + (MAP_TILE_Y * (int16_t)MAP_TILE_DATA.width));
				}
				if (MAIN_D_80135224 == 1) {
					ENTITY_TABLE[player]->posData->rotation.vy = 0;
					MAIN_D_80135250 = 0;
					MAIN_D_80135244 = 3;
					KAR_setupMatch(0);
					MAIN_D_80135252 = 0;
				}
			} else {
				if (KAR_D_8005B5A0[0].row.thrown == KAR_D_8005B5A0[1].row.thrown) {
					if (KAR_D_8005B5A0[0].row.thrown == 5) {
						MAIN_D_8013523C = 1;
					} else if (KAR_D_8005B5A0[0].row.score < KAR_D_8005B5A0[1].row.score) {
						MAIN_D_8013523C = 0;
					} else {
						MAIN_D_8013523C = 1;
					}
				} else if (KAR_D_8005B5A0[0].row.thrown < KAR_D_8005B5A0[1].row.thrown) {
					MAIN_D_8013523C = 0;
				} else {
					MAIN_D_8013523C = 1;
				}
				for (j = 0; j < 5; j++) {
					if (KAR_D_8005B5A0[MAIN_D_8013523C].row.stones[j].state == -1) {
						MAIN_D_8013523A = j;
						break;
					}
				}
				MAIN_D_80135252 = 0;
				MAIN_D_8013523E = 0;
				MAIN_D_80135244 = 0x10;
			}
		}
		break;
	case 0x10:
		MAIN_D_80135224 = tickCameraMoveTo(ENTITY_TABLE[player]->posData->location.vx,
		                                   ENTITY_TABLE[player]->posData->location.vz, 0x14);
		if (((MAP_TILE_DATA.cameraY % 0x80) == 0) || ((MAP_TILE_DATA.cameraY % 0x80) >= 0x6A)) {
			uploadMapTileImages(MAP_TILE_DATA.tiles, MAP_TILE_X + (MAP_TILE_Y * (int16_t)MAP_TILE_DATA.width));
		}
		if (MAIN_D_80135224 == 1) {
			if ((KAR_D_8005B5A0[0].row.thrown >= 5) && (KAR_D_8005B5A0[1].row.thrown >= 5)) {
				MAIN_D_80135250 = 1;
				clearTextArea();
				MAIN_D_80135244 = 0x13;
				if (KAR_D_8005B5A0[0].row.score > KAR_D_8005B5A0[1].row.score) {
					playSound2(8, 7);
					MAIN_D_80135252 = 8;
				} else {
					playSound2(8, 8);
					MAIN_D_80135252 = 7;
				}
			} else if (MAIN_D_8013523C != 0) {
				MAIN_D_80135244 = 0x11;
			} else {
				MAIN_D_80135244 = 5;
			}
		}
		break;
	case 0x11:
		KAR_chooseOpponentShot();
		MAIN_D_80135244 = 0x13;
		MAIN_D_80135250 = 1;
		MAIN_D_80135252 = 0;
		MAIN_D_8013523E = 0;
		if (KAR_D_8005B5A0[0].row.score > KAR_D_8005B5A0[1].row.score) {
			MAIN_D_80135252 = 5;
		} else if (KAR_D_8005B5A0[0].row.score == KAR_D_8005B5A0[1].row.score) {
			MAIN_D_80135252 = 6;
		} else {
			MAIN_D_80135252 = 4;
		}
		break;
	case 0x12:
		if (UI_BOX_DATA[0].state == 1) {
			MAIN_D_80135244 = 0x13;
		}
		break;
	case 0x13:
		if (KAR_tickHintBox((int8_t)MAIN_D_80135252) == 0) {
			MAIN_D_80135250 = 0;
			if (MAIN_D_80135252 >= 7) {
				KAR_finishMatch();
			} else {
				MAIN_D_80135244 = 5;
				MAIN_D_80135252 = 0;
				MAIN_D_8013523E = 0;
			}
		}
		break;
	case 0x14:
		MAIN_D_80135252 = 0;
		MAIN_D_8013523E = 0;
		KAR_finishMatch();
		break;
	}
}
