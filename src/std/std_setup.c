#include <malloc.h>
#include <stdlib.h>

#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/aabb.h>
#include <dw/clock.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/std.h>
#include <dw/types.h>

#include "common.h"

extern int32_t MAIN_D_801350EC;
extern void *MAIN_D_80135130;
extern int8_t GAME_STATE;
extern int32_t VIEWPORT_DISTANCE;
extern SVECTOR MAIN_D_801B1C0C[];
extern VECTOR MAIN_D_801B1C14;
extern GsVIEW2 MAIN_D_801B1B98;
extern int32_t MAIN_D_801B1BBC[];
extern GsF_LIGHT LIGHT_DATA[];
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t ACTIVE_FRAMEBUFFER;
extern uint8_t MAIN_D_801350F8;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern int16_t MAIN_D_801350E4;
extern int32_t MAIN_D_801350F0;
extern uint8_t CURRENT_SCREEN;
extern Entity *MAIN_D_801350E8;
extern int32_t MAIN_D_801350F4;
extern char *STD_D_8007A338[];
extern char *STD_D_8007A304[];
extern char *STD_D_8007A358[];
extern int8_t MAP_COLLISION_DATA[];

void STD_func_80058488(void);
void STD_func_80056E2C(int32_t type, int16_t slot, int32_t tier);
void initializeDigimonObject(int32_t type, int32_t instanceId, void (*tick)(int32_t));
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void scriptLoadModel(int32_t modelId);
void swapByte(uint8_t *a, uint8_t *b);
void STD_loadTIMToVRAM(char *path);
void STD_func_8005D9F4(uint8_t *out, uint8_t *arg);
void STD_initializeCamera(void);
void STD_func_80058254(void);
void STD_func_80057FC4(void);
void STD_func_80057778(void);
void STD_func_80057FD0(int16_t which);
int16_t STD_func_80057510(int32_t a, int16_t b);
void STD_func_80058E28(int32_t arg);
int32_t STD_func_8006314C(Entity *entity, int32_t arg);
void STD_removeCameraIntro(void);
void GsGetTimInfo(unsigned long *tim, GsIMAGE *img);
void readFile(char *path, void *dest);
void STD_loadArenaTIMToVRAM(char *path, int32_t count);
void STD_func_800577B4();
int32_t STD_func_8005DFF8(void);
void STD_func_80058030(void);
void STD_func_80057E5C(int32_t id);
void STD_initializeFinisherAuraModel(char *tim, char *base);
void STD_initializePoisonBubble(void);
void STD_initializeConfusionEffect(char *base);
void STD_initializeStunEffect(char *base);
void initializeBuffModel(TMDModel *model);
void STD_battleTickFrame(void);
int32_t loadTIMFile(char *path, void *buffer);
void removeMapEntities(void);
void STD_func_8006B6F4(void);
void STD_func_80056CA8(int32_t arena, uint8_t *arg);
void STD_func_8005A550(void);
void fadeToBlack(int32_t mode);
void STD_func_80057628(void);
void STD_addEnemyHPBars(void);
void STD_func_8005D964(void);
void STD_func_8005DEEC(int32_t track);
void STD_func_8005DF94(int32_t mode);
void STD_func_8005E5E0(void);
void STD_func_8005E898(void);
void STD_func_80064FCC(unsigned short count);
void STD_func_8006BA18(void);
void STD_func_8006BE64(void);
void STD_func_8006BFB4(void);
void STD_func_8006C630(void);
void STD_func_8006C6D0(void);
void STD_func_8006CCD4(void);
void STD_func_8006D15C(void);
void STD_setVSPhase(int32_t arg);
void fadeFromBlack(int32_t frames);
int32_t loadMapSounds(int32_t mapSoundId);
uint32_t lookupFileSize(char *path);

static void *std_setup_functions[] = {
	STD_func_80058488,
	STD_func_80058254,
	STD_func_80058030,
	STD_func_80057FD0,
	STD_func_80057FC4,
	STD_func_80057E5C,
	STD_loadArenaTIMToVRAM,
	STD_func_800579D8,
	STD_loadTIMToVRAM,
	STD_func_800577B4,
	STD_func_80057778,
	STD_func_80057628,
	STD_initializeCamera,
	STD_func_80057510,
	STD_func_80056E2C,
	STD_func_80056CA8,
};

// clang-format off
int16_t STD_D_800799B4[24] = {
	0x0064, 0x0096, 0x00c8, 0x00fa, 0x012c, 0x0000, 0x00c8, 0x00c8,
	0x00c8, 0x00c8, 0x00c8, 0x00fa, 0x00fa, 0x00fa, 0x00fa, 0x00fa,
	0x00fa, 0x00fa, 0x00fa, 0x00fa, 0x00fa, 0x00fa, 0x015e, 0x0000,
};

char STD_D_800799E4[28] = "\\STDDAT\\GRADE\\GRADED.TIM";

char STD_D_80079A00[28] = "\\STDDAT\\GRADE\\GRADEC.TIM";

char STD_D_80079A1C[28] = "\\STDDAT\\GRADE\\GRADEB.TIM";

char STD_D_80079A38[28] = "\\STDDAT\\GRADE\\GRADEA.TIM";

char STD_D_80079A54[28] = "\\STDDAT\\GRADE\\GRADES.TIM";

char STD_D_80079A70[28] = "\\STDDAT\\GRADE\\GRADER.TIM";

char STD_D_80079A8C[24] = "\\STDDAT\\GRADE\\CAPV1.TIM";

char STD_D_80079AA4[24] = "\\STDDAT\\GRADE\\CAPV2.TIM";

char STD_D_80079ABC[24] = "\\STDDAT\\GRADE\\CAPV3.TIM";

char STD_D_80079AD4[24] = "\\STDDAT\\GRADE\\CAPV4.TIM";

char STD_D_80079AEC[24] = "\\STDDAT\\GRADE\\CAPVO.TIM";

char STD_D_80079B04[24] = "\\STDDAT\\GRADE\\FR.TIM";

char STD_D_80079B1C[24] = "\\STDDAT\\GRADE\\GP.TIM";

char STD_D_80079B34[24] = "\\STDDAT\\GRADE\\TW.TIM";

char STD_D_80079B4C[24] = "\\STDDAT\\GRADE\\CO.TIM";

char STD_D_80079B64[24] = "\\STDDAT\\GRADE\\NT.TIM";

char STD_D_80079B7C[24] = "\\STDDAT\\GRADE\\MT.TIM";

char STD_D_80079B94[24] = "\\STDDAT\\GRADE\\DT.TIM";

char STD_D_80079BAC[24] = "\\STDDAT\\GRADE\\DY.TIM";

char STD_D_80079BC4[24] = "\\STDDAT\\GRADE\\WI.TIM";

char STD_D_80079BDC[24] = "\\STDDAT\\GRADE\\AN.TIM";

char STD_D_80079BF4[24] = "\\STDDAT\\GRADE\\HU.TIM";

char STD_D_80079C0C[24] = "\\STDDAT\\GRADE\\BT.TIM";

char *STD_D_80079C24[23] = {
	STD_D_800799E4,
	STD_D_80079A00,
	STD_D_80079A1C,
	STD_D_80079A38,
	STD_D_80079A54,
	STD_D_80079A70,
	STD_D_80079A8C,
	STD_D_80079AA4,
	STD_D_80079ABC,
	STD_D_80079AD4,
	STD_D_80079AEC,
	STD_D_80079B04,
	STD_D_80079B1C,
	STD_D_80079B34,
	STD_D_80079B4C,
	STD_D_80079B64,
	STD_D_80079B7C,
	STD_D_80079B94,
	STD_D_80079BAC,
	STD_D_80079BC4,
	STD_D_80079BDC,
	STD_D_80079BF4,
	STD_D_80079C0C,
};

char STD_D_80079C80[20] = "\\STDDAT\\STDTIM.BIN";

char STD_D_80079C94[20] = "\\STDDAT\\TIME.TIM";

char STD_D_80079CA8[20] = "\\ETCDAT\\SBOY.TIM";
// clang-format on

void STD_func_80056CA8(int32_t arena, uint8_t *arg)
{
	if (arena == 0) {
		MAIN_D_801350F8 = 0x1e;
	} else {
		MAIN_D_801350F8 = 0x20;
	}

	GAME_STATE = 5;
	DRAWING_OFFSET_X = 0xa0;
	DRAWING_OFFSET_Y = 0x78;
	STD_D_8007B684[0].length = 2;
	STD_D_8007B684[0].org = STD_D_8007B664;
	STD_D_8007B684[1].length = 2;
	STD_D_8007B684[1].org = STD_D_8007B674;
	STD_D_8007B6AC[0].length = 2;
	STD_D_8007B6AC[0].org = STD_D_8007B6D4;
	STD_D_8007B6AC[1].length = 2;
	STD_D_8007B6AC[1].org = STD_D_8007B6E4;
	removeMapEntities();
	ENTITY_TABLE[0]->isOnScreen = 0;
	STD_func_80057778();
	STD_initializeCamera();
	STD_func_800577B4(arena);
	STD_func_80057E5C(arena);
	STD_func_8006B6F4();
	loadTIMFile(STD_D_80079C24[arg[0]], GENERAL_BUFFER_PTR);
	STD_loadTIMToVRAM(STD_D_80079C80);
	loadTIMFile(STD_D_80079C94, (void *)0x80010000);
	STD_initializeFinisherAuraModel((char *)0x80052ae0, (char *)0x80053800);
	STD_initializePoisonBubble();
	STD_initializeConfusionEffect((char *)0x80054838);
	STD_initializeStunEffect((char *)0x80054d00);
	initializeBuffModel((TMDModel *)0x80055328);
}

void STD_func_80056E2C(int32_t type, int16_t slot, int32_t tier)
{
	uint8_t listB[16];
	uint8_t listA[16];
	Stats *stats;
	int32_t base;
	int32_t mul;
	int32_t div;
	int32_t i;
	int32_t nb;
	int32_t na;
	int32_t tech;
	uint32_t m;

	base = STD_D_800799B4[tier];
	scriptLoadModel(type);
	ENTITY_TABLE[slot + 2] = (Entity *)&NPC_ENTITIES[slot];
	initializeDigimonObject(type, slot + 2, STD_tickNPCTournament);
	switch (DIGIMON_DATA[ENTITY_TABLE[slot + 2]->type].level) {
	case 3:
		mul = div = 1;
		break;
	case 4:
		div = 1;
		mul = 2;
		break;
	case 5:
		div = 10;
		mul = 25;
		break;
	}
	stats = &NPC_ENTITIES[slot].digimonEntity.stats;
	stats->base.off = mul * (base + base * (0x1e - random(0x3d)) / 100) / div;
	stats->base.def = mul * (base + base * (0x1e - random(0x3d)) / 100) / div;
	stats->base.speed = mul * (base + base * (0x1e - random(0x3d)) / 100) / div;
	stats->base.brain = mul * (base + base * (0x1e - random(0x3d)) / 100) / div;
	base = base * 10;
	stats->base.hp = mul * (base + base * (0x1e - random(0x3d)) / 100) / div;
	stats->base.mp = mul * (base + base * (0x1e - random(0x3d)) / 100) / div;
	if (stats->base.off >= 0x3e8) {
		stats->base.off = 0x3e7;
	}
	if (stats->base.def >= 0x3e8) {
		stats->base.def = 0x3e7;
	}
	if (stats->base.speed >= 0x3e8) {
		stats->base.speed = 0x3e7;
	}
	if (stats->base.brain >= 0x3e8) {
		stats->base.brain = 0x3e7;
	}
	if (stats->base.hp >= 0x2710) {
		stats->base.hp = 0x270f;
	}
	if (stats->base.mp >= 0x2710) {
		stats->base.mp = 0x270f;
	}
	stats->current.currentHP = stats->base.hp;
	stats->current.currentMP = stats->base.mp;
	stats->current.chargeMode = 0;
	ENTITY_TABLE[slot + 2]->isOnMap = 1;
	ENTITY_TABLE[slot + 2]->isOnScreen = 1;
	nb = 0;
	na = 0;
	for (i = 0; i < 0xf; i++) {
		tech = (int16_t)entityGetTechFromAnim(ENTITY_TABLE[slot + 2], i + 0x2e);
		if (tech == 0xff) {
			continue;
		}
		if (tech >= 0x3a && tech < 0x71) {
			continue;
		}
		if (stats->base.mp / 10 < MOVE_DATA[tech].mpCost * 3) {
			continue;
		}
		if (MOVE_DATA[tech].range == 4 || tech == 0x2d) {
			listA[na++] = i;
		}
		listB[nb++] = i;
	}
	if (nb != 0) {
		for (i = 0; i < nb; i++) {
			swapByte(&listB[i], &listB[random(nb)]);
		}
		for (i = 0; i < 3; i++) {
			stats->base.moves[i] = 0xff;
			if (i <= nb - 1) {
				stats->base.moves[i] = listB[i] + 0x2e;
			}
		}
	} else {
		stats->base.moves[0] = 0x2e;
		stats->base.moves[1] = 0xff;
		stats->base.moves[2] = 0xff;
	}
	if (na != 0) {
		for (i = 0; i < na; i++) {
			swapByte(&listA[i], &listA[random(na)]);
		}
		if (random(0xa) == 0) {
			stats->base.moves[3] = listA[0] + 0x2e;
		}
	}
	stats->base.moves[3] = 0xff;
	for (i = 0; i < 0x10; i++) {
		if (DIGIMON_DATA[ENTITY_TABLE[slot + 2]->type].moves[i] >= 0x3a &&
		    DIGIMON_DATA[ENTITY_TABLE[slot + 2]->type].moves[i] < 0x71) {
			stats->base.moves[3] = i + 0x2e;
			return;
		}
	}
}

int16_t STD_func_80057510(int32_t a, int16_t b)
{
	int16_t result;

	ENTITY_TABLE[1]->isOnScreen = 1;
	GAME_STATE = 5;
	STD_func_80058E28(b);
	MAIN_D_801350EC = 1;
	result = STD_func_8006314C(ENTITY_TABLE[0], a);
	STD_removeCameraIntro();
	MAIN_D_801350EC = 10;
	return result;
}

void STD_initializeCamera(void)
{
	VIEWPORT_DISTANCE = 500;
	GsSetProjection(500);
	MAIN_D_801B1C0C[0].vx = 100;
	MAIN_D_801B1C0C[0].vy = 0;
	MAIN_D_801B1C0C[0].vz = 0;
	MAIN_D_801B1C14.vx = 0;
	MAIN_D_801B1C14.vy = 500;
	MAIN_D_801B1C14.vz = 3000;
	MAIN_D_801B1B98.super = NULL;
	RotMatrix(MAIN_D_801B1C0C, &MAIN_D_801B1B98.view);
	TransMatrix(&MAIN_D_801B1B98.view, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void STD_func_80057628(void)
{
	RECT rect;
	int32_t i;

	i = 0;
	stopBGM();
	stopSound();
	STD_func_80058488();
	STD_func_8005A550();
	STD_func_80057FC4();
	setRECT(&rect, 0, 0, 0x140, 0x1e0);
	ClearImage(&rect, 0, 0, 0);
	setRECT(&rect, 0x300, 0, 0xff, 0x180);
	ClearImage(&rect, 0, 0, 0);
	setRECT(&rect, 0x180, 0, 0x180, 0x100);
	ClearImage(&rect, 0, 0, 0);
	DrawSync(0);
	ENTITY_TABLE[1]->isOnScreen = 0;
	loadTIMFile(STD_D_80079CA8, GENERAL_BUFFER_PTR);
	fadeToBlack(1);

	for (; i < 0xb; i++) {
		STD_battleTickFrame();
	}

	ENTITY_TABLE[1]->posData->location.vy = 0;
	ENTITY_TABLE[1]->anim.locY = 0;
	ENTITY_TABLE[0]->isOnScreen = 1;
	GAME_STATE = 0;
}

void STD_func_80057778(void)
{
	removeObject(0xfa2, 0);
	removeObject(0xfa0, 0);
	removeObject(0xfa8, 0);
}

void STD_func_800577B4(void)
{
	LIGHT_DATA[0].vx = 1000;
	LIGHT_DATA[0].vy = 1000;
	LIGHT_DATA[0].vz = 1000;
	LIGHT_DATA[0].r = 255;
	LIGHT_DATA[0].g = 255;
	LIGHT_DATA[0].b = 255;
	GsSetFlatLight(0, &LIGHT_DATA[0]);
	LIGHT_DATA[1].vx = -1000;
	LIGHT_DATA[1].vy = 1000;
	LIGHT_DATA[1].vz = -1000;
	LIGHT_DATA[1].r = 160;
	LIGHT_DATA[1].g = 160;
	LIGHT_DATA[1].b = 160;
	GsSetFlatLight(1, &LIGHT_DATA[1]);
	LIGHT_DATA[2].vx = -20;
	LIGHT_DATA[2].vy = 20;
	LIGHT_DATA[2].vz = 200;
	LIGHT_DATA[2].r = 96;
	LIGHT_DATA[2].g = 96;
	LIGHT_DATA[2].b = 96;
	GsSetFlatLight(2, &LIGHT_DATA[2]);
	GsSetAmbient(0x400, 0x400, 0x400);
	GsSetLightMode(0);
}

void STD_loadTIMToVRAM(char *path)
{
	GsIMAGE img;
	int32_t *p;
	int32_t i;

	p = (int32_t *)GENERAL_BUFFER;
	readFile(path, GENERAL_BUFFER);
	for (i = 0; i < 6; i++) {
		p = (int32_t *)((char *)p + 4);
		GsGetTimInfo((u_long *)p, &img);
		p = &p[((img.pw * img.ph) / 2) + 4];
		LoadImage((RECT *)&img.px, img.pixel);
		if ((img.pmode >> 3) & 1) {
			LoadImage((RECT *)&img.cx, img.clut);
			p = &p[((img.cw * img.ch) / 2) + 3];
		}
	}
}

int16_t STD_func_800579D8(uint8_t *arg)
{
	uint8_t local[4];
	int32_t n;
	int32_t m;
	int32_t i;
	int32_t j;
	int16_t arena;

	for (n = 1; n < 8; n++) {
		if (arg[n] == 0x40) {
			arg[n] = 0x36;
		}
		if (arg[n] == 0x3f) {
			arg[n] = 0x30;
		}
		if (arg[n] == 0x41) {
			arg[n] = 0x2a;
		}
	}
	fadeToBlack(5);
	STD_func_80064FCC(5);
	loadMapSounds(0x10);
	if (CURRENT_SCREEN == 0x6a) {
		arena = 1;
	} else {
		arena = 0;
	}
	STD_func_80056CA8(arena, arg);
	STD_func_8005D964();
	STD_func_8005D9F4(local, arg);
	STD_setVSPhase(0xa);
	STD_func_80057FD0(arena);
	STD_func_8005E5E0();
	fadeFromBlack(5);
	STD_func_80064FCC(0x78);
	STD_func_8005E898();
	MAIN_D_801350E4 = 0;
	i = 0;
	j = 0;
	while (MAIN_D_801350E4 != -1 && i != 3) {
		STD_func_8005DEEC(1);
		loadMapSounds(0x10);
		while (STD_func_8005DFF8() == 0) {
			STD_battleTickFrame();
		}
		STD_func_8005DF94(1);
		STD_func_80056E2C(local[i], (int16_t)i, arg[0]);
		MAIN_D_801350E8 = (ENTITY_TABLE + 2)[i];
		MAIN_D_801350E4 = STD_func_80057510((int32_t)ENTITY_TABLE[i + 2], arena);
		j++;
		if (MAIN_D_801350E4 == 1) {
			loadMapSounds(0x10);
			STD_func_8006D15C();
			STD_initializeCamera();
			MAIN_D_801350EC = 0xa;
		} else {
			STD_func_8006CCD4();
		}
		removeMapEntities();
		GAME_STATE = 5;
		i++;
	}
	if (MAIN_D_801350E4 == 1) {
		STD_func_8006BA18();
		STD_func_8005DEEC(2);
		while (STD_func_8005DFF8() == 0) {
			STD_battleTickFrame();
		}
		fadeToBlack(0xa);
		for (m = 0; m < 0xb; m++) {
			STD_battleTickFrame();
		}
		STD_func_8005DF94(2);
		STD_addEnemyHPBars();
		STD_func_8006BE64();
		STD_battleTickFrame();
		STD_battleTickFrame();
		fadeFromBlack(0xf);
		for (m = 0; m < 0x10 || MAIN_D_801350F0 > 0; m++) {
			STD_battleTickFrame();
		}
		STD_func_8006BFB4();
		while (MAIN_D_801350F4 == 0) {
			STD_battleTickFrame();
		}
		STD_func_8006C6D0();
		STD_func_8006C630();
	}
	STD_func_80057628();
	if (MAIN_D_801350E4 == 1) {
		return i;
	}

	return i - 1;
}

void STD_loadArenaTIMToVRAM(char *path, int32_t count)
{
	GsIMAGE img;
	int32_t *p;
	int32_t i;

	p = (int32_t *)GENERAL_BUFFER;
	readFile(path, GENERAL_BUFFER);
	for (i = 0; i < count; i++) {
		p = (int32_t *)((char *)p + 4);
		GsGetTimInfo((u_long *)p, &img);
		p = &p[((img.pw * img.ph) / 2) + 4];
		LoadImage((RECT *)&img.px, img.pixel);
		if ((img.pmode >> 3) & 1) {
			LoadImage((RECT *)&img.cx, img.clut);
			p = &p[((img.cw * img.ch) / 2) + 3];
		}
	}
}

// clang-format off
void STD_func_80057E5C(id)
	uint8_t id;
// clang-format on
{
	int32_t i;
	uint8_t *tims = MAIN_D_801347FC.timCount;

	STD_loadArenaTIMToVRAM(STD_D_8007A338[id], tims[id]);
	MAIN_D_80135130 = malloc3(((int32_t)lookupFileSize(STD_D_8007A304[id]) + 0x7ff) & ~0x7ff);
	readFile(STD_D_8007A304[id], MAIN_D_80135130);
	GsMapModelingData((u_long *)MAIN_D_80135130 + 1);
	for (i = 0; i < MAIN_D_801347FC.modelCount[id]; i++) {
		GsLinkObject4((u_long)((char *)MAIN_D_80135130 + 0xc), &STD_D_8007B97C[i], i);
		GsInitCoordinate2(NULL, &STD_D_8007B83C[i].coord);
		STD_D_8007B97C[i].attribute = 0;
		STD_D_8007B97C[i].coord2 = &STD_D_8007B83C[i].coord;
	}
	readFile(STD_D_8007A358[id], MAP_COLLISION_DATA);
}

void STD_func_80057FC4(void)
{
	free3(MAIN_D_80135130);
}

void STD_func_80057FD0(int16_t which)
{
	switch (which) {
	case 0:
		addObject(0x1a7, 0, (TickFunction)0, (RenderFunction)STD_func_80058030);
		break;
	case 1:
		addObject(0x1a7, 0, (TickFunction)0, (RenderFunction)STD_func_80058254);
		break;
	}
}

void STD_func_80058030(void)
{
	MATRIX m;
	int32_t i;

	GsGetLw(&STD_D_8007B83C[1].coord, &m);
	GsSetLightMatrix(&m);
	GsGetLs(&STD_D_8007B83C[1].coord, &m);
	GsSetLsMatrix(&m);
	GsSortObject4(&STD_D_8007B97C[1], ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0));
	if (STD_func_8005DFF8() == 1) {
		STD_D_8007B97C[0].attribute |= 0x200;
	} else {
		STD_D_8007B97C[0].attribute = 0;
	}
	GsClearOt(0, 0xfff, &STD_D_8007B684[ACTIVE_FRAMEBUFFER]);
	GsClearOt(0, 0xffe, &STD_D_8007B6AC[ACTIVE_FRAMEBUFFER]);
	for (i = 2; i >= 0; i--) {
		if (i == 1) {
			continue;
		}
		GsGetLw(&STD_D_8007B83C[i].coord, &m);
		GsSetLightMatrix(&m);
		GsGetLs(&STD_D_8007B83C[i].coord, &m);
		GsSetLsMatrix(&m);
		switch (i) {
		case 0:
			GsSortObject4(&STD_D_8007B97C[i], &STD_D_8007B6AC[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		case 2:
			GsSortObject4(&STD_D_8007B97C[i], &STD_D_8007B684[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		}
	}
	GsSortOt(&STD_D_8007B684[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSortOt(&STD_D_8007B6AC[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
}

void STD_func_80058254(void)
{
	MATRIX m;
	int32_t i;

	if (STD_func_8005DFF8() == 1) {
		GsGetLw(&STD_D_8007B83C[3].coord, &m);
		GsSetLightMatrix(&m);
		GsGetLs(&STD_D_8007B83C[3].coord, &m);
		GsSetLsMatrix(&m);
		GsSortObject4(&STD_D_8007B97C[3], ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0));
	}
	if (STD_func_8005DFF8() == 1) {
		STD_D_8007B97C[0].attribute |= 0x200;
	} else {
		STD_D_8007B97C[0].attribute = 0;
	}
	GsClearOt(0, 0xfff, &STD_D_8007B684[ACTIVE_FRAMEBUFFER]);
	GsClearOt(0, 0xffe, &STD_D_8007B6AC[ACTIVE_FRAMEBUFFER]);
	for (i = 2; i >= 0; i--) {
		GsGetLw(&STD_D_8007B83C[i].coord, &m);
		GsSetLightMatrix(&m);
		GsGetLs(&STD_D_8007B83C[i].coord, &m);
		GsSetLsMatrix(&m);
		switch (i) {
		case 0:
			GsSortObject4(&STD_D_8007B97C[i], &STD_D_8007B6AC[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		case 1:
		case 2:
			GsSortObject4(&STD_D_8007B97C[i], &STD_D_8007B684[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		}
	}
	GsSortOt(&STD_D_8007B684[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSortOt(&STD_D_8007B6AC[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
}

void STD_func_80058488(void)
{
	removeObject(0x1a7, 0);
}
