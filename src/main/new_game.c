#include <libgs.h>

#include <dw/anim.h>
#include <dw/btl.h>
#include <dw/entity.h>
#include <dw/garbage.h>
#include <dw/model.h>
#include <dw/script.h>

extern GsF_LIGHT LIGHT_DATA[3];

void tickNewGameJijimon(int32_t instanceId);
void loadNewGameScene(void);
void unloadNewGameScene(void);

int16_t NEW_GAME_JIJIMON_TICK_COUNT;

static void *new_game_functions[] = {
	unloadNewGameScene,
	loadNewGameScene,
	tickNewGameJijimon,
};

void tickNewGameJijimon(int32_t instanceId)
{
	int16_t *p;

	p = &NEW_GAME_JIJIMON_TICK_COUNT;
	if (*p < 0x7530) {
		*p += 1;
	}
	if (*p == 0x23) {
		setEntityRotation(2, 0, 0x71, 0);
		setupEntityMatrix(2);
		startAnimation(ENTITY_TABLE[2], 0);
		writePStat(0xf3, 0);
	}
	tickAnimation(ENTITY_TABLE[2]);
}

GARBAGE(loadNewGameScene, 10);

void loadNewGameScene(void)
{
	int32_t result;
	GsRVIEW2 view;

	thunkLoadMMD(0x75, 0);
	ENTITY_TABLE[2] = &NPC_ENTITIES[0].digimonEntity.entity;
	initializeDigimonObject(0x75, 2, tickNewGameJijimon);
	ENTITY_TABLE[2]->isOnMap = 1;
	ENTITY_TABLE[2]->isOnScreen = 1;
	NEW_GAME_JIJIMON_TICK_COUNT = 0;
	setEntityPosition(2, 0x320, 0x96, 0);
	setEntityRotation(2, 0, 0x400, 0);
	setupEntityMatrix(2);
	startAnimation(ENTITY_TABLE[2], 2);
	GsSetProjection(0x3e8);
	view.vpx = 0;
	view.vpz = -0x1068;
	view.vpy = 0;
	view.vrx = 0;
	view.vry = 0;
	view.vrz = 0;
	view.rz = 0;
	view.super = NULL;
	result = GsSetRefView2(&view);
	DRAWING_OFFSET_X = 0xa0;
	DRAWING_OFFSET_Y = 0xb9;
	LIGHT_DATA[0].vx = 0x1e;
	LIGHT_DATA[0].vy = 0x64;
	LIGHT_DATA[0].vz = 0x1e;
	LIGHT_DATA[0].r = 0x40;
	LIGHT_DATA[0].g = 0x40;
	LIGHT_DATA[0].b = 0x40;
	GsSetFlatLight(0, &LIGHT_DATA[0]);
	LIGHT_DATA[1].vx = -0x1e;
	LIGHT_DATA[1].vy = 0x64;
	LIGHT_DATA[1].vz = 0;
	LIGHT_DATA[1].r = 0x28;
	LIGHT_DATA[1].g = 0x28;
	LIGHT_DATA[1].b = 0x28;
	GsSetFlatLight(1, &LIGHT_DATA[1]);
	LIGHT_DATA[2].vx = 0;
	LIGHT_DATA[2].vy = 0x64;
	LIGHT_DATA[2].vz = -0x1e;
	LIGHT_DATA[2].r = 0x26;
	LIGHT_DATA[2].g = 0x26;
	LIGHT_DATA[2].b = 0x26;
	GsSetFlatLight(2, &LIGHT_DATA[2]);
	GsSetAmbient(0x800, 0x800, 0x800);
}

void unloadNewGameScene(void)
{
	removeEntity(0x75, 2);
	thunkUnloadModel(0x75, 0);
}
