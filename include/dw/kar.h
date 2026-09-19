#ifndef DW_KAR_H
#define DW_KAR_H

#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/types.h>

typedef struct {
	int16_t weight[4];
} KarWeightTbl;

typedef struct {
	char *text[2];
} KarStrPair;

typedef struct {
	int8_t value[4];
} KarTallyValues;

typedef struct {
	int8_t id[3];
} KarPeggedModelIds;

typedef struct {
	int32_t state;
	int8_t type;
	int8_t unk5;
	int16_t weight;
	int16_t speed;
	int16_t unkA;
	int32_t angle;
	int8_t ring;
	int8_t prevRing;
	int8_t pad12[2];
	VECTOR target;
	GsDOBJ2 obj;
	GsCOORDINATE2 coord;
	int8_t pad84[16];
	int16_t unk94;
	int16_t selectPhase;
	int16_t unk98;
	int16_t pad9A;
	VECTOR pos;
} KarStone;

typedef struct {
	int8_t score;
	int8_t thrown;
	int16_t unk2;
	uint16_t unk4;
	int8_t pad6[2];
	KarStone stones[5];
} KarStoneRow;

/* Hack to match overlay .bss */
typedef union {
	uint8_t raw[868];
	KarStoneRow row;
} KarStoneRowRaw;

typedef struct {
	struct {
		int16_t dx;
		int16_t dz;
		int16_t radius;
	} ring[4];
} KarZones;

typedef struct {
	char *text[61];
} KarStrTbl;

typedef struct {
	int8_t start[10];
} KarOffTbl;

typedef struct {
	int32_t id[3];
} KarModelIds;

typedef struct {
	int32_t x[3];
} KarSpawnX;

typedef struct {
	int32_t type[2][4];
} KarShotPlan;

typedef struct {
	int32_t x;
	int32_t y;
	int32_t z;
	int32_t pad;
} KarPos;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t clutX;
	int16_t clutY;
	int16_t u;
	int16_t v;
	int16_t w;
	int16_t h;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t pad;
} KarSprite;

typedef struct {
	KarSprite sprite[5];
} KarSpriteSet;

typedef struct {
	KarSprite sprite[2];
} KarSpritePair;

typedef struct {
	struct {
		int16_t u;
		int16_t v;
	} glyph[11];
} KarDigits;
extern KarWeightTbl MAIN_D_80134A08;
extern RECT MAIN_D_80134A10;
extern RECT MAIN_D_80134A18;
extern KarStrPair MAIN_D_80134A20;
extern RECT MAIN_D_80134A28;
extern RECT MAIN_D_80134A30;
extern KarTallyValues MAIN_D_80134A44;
extern KarPeggedModelIds MAIN_D_80134A4C;
extern int8_t MAIN_D_80134A4F;

extern KarZones KAR_D_8005B438;
extern GsRVIEW2 KAR_D_8005B580;
extern uint16_t MAIN_D_80135252;
extern KarModelIds KAR_D_8005AB80;
extern KarOffTbl KAR_D_8005AB8C;
extern KarSpawnX KAR_D_8005AB98;
extern uint8_t MAIN_D_8013522C;
extern KarStrTbl KAR_D_8005AF58;
extern KarOffTbl KAR_D_8005B04C;
extern KarStrTbl KAR_D_8005B318;
extern KarOffTbl KAR_D_8005B40C;
extern KarShotPlan KAR_D_8005B418;
extern KarSprite KAR_D_8005B48C;
extern KarDigits KAR_D_8005B4A0;
extern KarSpritePair KAR_D_8005B4CC;
extern int16_t MAIN_D_8013523E;
extern KarSpriteSet KAR_D_8005B4F4;
extern KarSprite KAR_D_8005B558;
extern KarSprite KAR_D_8005B478;
extern KarSprite KAR_D_8005B56C;
extern uint8_t MAIN_D_80135220;
extern char KAR_D_8005ABA4[];
extern u_long *MAIN_D_80135240;
extern int32_t MAIN_D_80135244;
extern uint8_t MAIN_D_80135248;
extern int16_t MAIN_D_8013523A;
extern int8_t MAIN_D_80135256;
extern int8_t MAIN_D_80135250;
extern int8_t MAIN_D_8013523C;
extern int16_t MAIN_D_80135254;
extern int32_t MAIN_D_8013524C;
extern uint16_t MAIN_D_80135238;
extern int8_t MAIN_D_8013522D;
extern int8_t MAIN_D_8013522E;
extern int32_t MAIN_D_80135230;
extern int32_t MAIN_D_80135234;
extern char KAR_D_8005ABB8[];

extern KarStoneRowRaw KAR_D_8005B5A0[];
extern u_long KAR_D_8005BFCC[];
extern GsOT_TAG KAR_D_800637CC[];
extern GsOT_TAG KAR_D_8006384C[];
extern GsOT KAR_D_800638CC[];
extern int32_t KAR_D_800638F4[];
extern uint8_t KAR_D_80063918[];
extern uint8_t KAR_D_80063924[];
extern int32_t KAR_D_800639B0[];
extern KarStone *KAR_D_800639C0[];

void checkCurlingMap(int32_t mapId);

void KAR_start(void);
void KAR_tick(void);

#endif
