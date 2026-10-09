#include <string.h>

#include <libgs.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/fade.h>
#include <dw/file.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/swap.h>
#include <dw/types.h>
#include <dw/vs.h>
#include <dw/world_object.h>

extern int32_t VIEWPORT_DISTANCE;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern uint8_t *GENERAL_BUFFER_PTR;
extern uint16_t PLAYTIME_FRAMES;
extern uint8_t VS_MUSIC;

void loadStackedTIMFile(char *path);
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void initializeBuffModel(TMDModel *model);
void VS_tickPlaytime(void);
void VS_setVSPhase(int32_t arg);
void VS_removeVSPhase(void);
void VS_addArenaRenderers(void);
void VS_removeArenaRenderers(void);
void VS_loadArenaAssets(void);
void VS_initializeFinisherAuraModel(char *tim, char *base);
void VS_initializePoisonBubble(void);
void VS_initializeConfusionEffect(char *base);
void VS_initializeStunEffect(char *base);
void VS__tickDigimonP1(int32_t instanceId);
void VS__tickDigimonP2(int32_t instanceId);
void VS_runDemoCombat(void);

void VS_setupDemoBattle(void);
void VS_cleanupDemoBattle(void);
void VS_loadDemoAssets(void);
void VS_loadDemoFighters(void);
void VS_randomizeDemoFighter(Entity *entity);
void VS_initializeDemoCamera(void);
void VS_initializeDemoLighting(void);
void VS_setupTrialBattle(void);
void VS_randomizeTrialDigimonList(RegisteredDigimon *list, int32_t count);
void VS_randomizeTrialDigimon(RegisteredDigimon *digimon);

static void *vs_demo_functions[] = {
	VS_initializeTrialBattle,
	VS_playDemo,
	VS_randomizeTrialDigimon,
	VS_randomizeTrialDigimonList,
	VS_setupTrialBattle,
	VS_initializeDemoLighting,
	VS_initializeDemoCamera,
	VS_randomizeDemoFighter,
	VS_loadDemoFighters,
	VS_loadDemoAssets,
	VS_cleanupDemoBattle,
	VS_setupDemoBattle,
};

// clang-format off
char VS_PATH_DEMO_ETCDAT_SYSTEM_W_TIM[] = "\\ETCDAT\\SYSTEM_W.TIM";

char VS_PATH_DEMO_STDDAT_TIME_TIM[] = "\\STDDAT\\TIME.TIM";

char VS_PATH_DEMO_STDDAT_TAISEN1_TIM[] = "\\STDDAT\\TAISEN1.TIM";

char VS_PATH_DEMO_ETCDAT_ETCTIM_BIN[] = "\\ETCDAT\\ETCTIM.BIN";

char VS_PATH_DEMO_ETCNA_TITLE256_TIM[] = "\\ETCNA\\TITLE256.TIM";

uint8_t VS_DEMO_DIGIMON[48] = {
	0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
	0x0b, 0x0c, 0x0d, 0x0e, 0x11, 0x12, 0x13, 0x14,
	0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c,
	0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
	0x27, 0x28, 0x29, 0x2a, 0x2d, 0x2e, 0x2f, 0x30,
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
};

char VS_PATH_DEMO_STDDAT_STDTIM_BIN[] = "\\STDDAT\\STDTIM.BIN";

char VS_PATH_DEMO_ETCNA_TITLE2_TIM[] = "\\ETCNA\\TITLE2.TIM";

char VS_PATH_DEMO_STDDAT_TAISEN2_TIM[] = "\\STDDAT\\TAISEN2.TIM";

char VS_PATH_DEMO_STDDAT_16TAISEN_TIM[] = "\\STDDAT\\16TAISEN.TIM";

char VS_PATH_DEMO_STDDAT_TAISEN_F_TIM[] = "\\STDDAT\\TAISEN_F.TIM";

char VS_STR_DIGIMON_1[] = "デジモン１";

char VS_STR_DIGIMON_2[] = "デジモン２";

uint8_t VS_DEMO_MUSIC[3] = { 0x1e, 0x20, 0x1f };
// clang-format on

void VS_setupDemoBattle(void)
{
	PLAYTIME_FRAMES = 0;
	addObject(0xfb9, 0, (TickFunction)VS_tickPlaytime, NULL);
	fadeFromBlack(1);
	loadTIMFile(VS_PATH_DEMO_ETCDAT_SYSTEM_W_TIM, GENERAL_BUFFER);
	loadTIMFile(VS_PATH_DEMO_STDDAT_TIME_TIM, GENERAL_BUFFER);
	loadTIMFile(VS_PATH_DEMO_STDDAT_TAISEN1_TIM, GENERAL_BUFFER);
	loadStackedTIMFile(VS_PATH_DEMO_ETCDAT_ETCTIM_BIN);
	VS_loadDemoAssets();
	VS_setVSPhase(1);
	VS_addArenaRenderers();
	VS_loadDemoFighters();
	setEntityPosition(1, 500, 0, 0);
	setEntityRotation(1, 0, 0x400, 0);
	setEntityPosition(2, -500, 0, 0);
	setEntityRotation(2, 0, 0xc00, 0);
	VS_randomizeDemoFighter(ENTITY_TABLE[1]);
	VS_randomizeDemoFighter(ENTITY_TABLE[2]);
	VSLoadSounds();
	((DigimonEntity *)ENTITY_TABLE[1])->stats.current.vabId = 4;
	((DigimonEntity *)ENTITY_TABLE[2])->stats.current.vabId = 5;
	loadDigimonSounds(4, ENTITY_TABLE[1]->type);
	loadDigimonSounds(5, ENTITY_TABLE[2]->type);
}

void VS_cleanupDemoBattle(void)
{
	int32_t type;

	PLAYTIME_FRAMES = 0;
	removeObject(0xfb9, 0);

	type = ENTITY_TABLE[1]->type;
	removeEntity(type, 1);
	thunkUnloadModel(type, 3);

	type = ENTITY_TABLE[2]->type;
	removeEntity(type, 2);
	thunkUnloadModel(type, 0);

	ENTITY_TABLE[2] = NULL;
	ENTITY_TABLE[1] = NULL;
	VS_removeVSPhase();
	VS_removeArenaRenderers();
	loadTIMFile(VS_PATH_DEMO_ETCNA_TITLE256_TIM, GENERAL_BUFFER_PTR);
}

void VS_loadDemoAssets(void)
{
	VS_BATTLE_SETUP.stage = randomLimit(3);
	VS_MUSIC = VS_DEMO_MUSIC[VS_BATTLE_SETUP.stage];
	DRAWING_OFFSET_X = 0xa0;
	DRAWING_OFFSET_Y = 0x78;
	VS_D_800716D4[0].length = 2;
	VS_D_800716D4[0].org = VS_D_800716B4;
	VS_D_800716D4[1].length = 2;
	VS_D_800716D4[1].org = VS_D_800716C4;
	VS_D_800716FC[0].length = 2;
	VS_D_800716FC[0].org = VS_D_80071724;
	VS_D_800716FC[1].length = 2;
	VS_D_800716FC[1].org = VS_D_80071734;
	VS_initializeDemoCamera();
	VS_initializeDemoLighting();
	VS_loadArenaAssets();
	VS_initializeFinisherAuraModel(VS_FINISHER_TIM, VS_FINISHER_MODEL);
	VS_initializePoisonBubble();
	VS_initializeConfusionEffect(VS_CONFUSION_MODEL);
	VS_initializeStunEffect(VS_STUN_MODEL);
	initializeBuffModel(VS_BUFF_MODEL);
	loadStackedTIMFile(VS_PATH_DEMO_STDDAT_STDTIM_BIN);
}

void VS_loadDemoFighters(void)
{
	int32_t type;

	type = VS_DEMO_DIGIMON[randomLimit(0x30)];
	thunkLoadMMD(type, 3);
	ENTITY_TABLE[1] = &PARTNER_ENTITY.digimonEntity.entity;
	initializeDigimonObject(type, 1, VS__tickDigimonP1);

	type = VS_DEMO_DIGIMON[randomLimit(0x30)];
	thunkLoadMMD(type, 0);
	ENTITY_TABLE[2] = &NPC_ENTITIES[0].digimonEntity.entity;
	initializeDigimonObject(type, 2, VS__tickDigimonP2);
}

void VS_randomizeDemoFighter(Entity *entity)
{
	int32_t nb;
	int32_t na;
	int32_t div;
	int32_t mul;
	uint8_t listB[16];
	uint8_t listA[16];
	int16_t tech;
	Stats *stats;
	int32_t i;
	int16_t base;

	base = 100;
	switch (DIGIMON_DATA[entity->type].level) {
	case 3:
		div = 1;
		mul = 1;
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
	stats = &((DigimonEntity *)entity)->stats;
	stats->base.off = mul * (base + base * (0x1e - randomLimit(0x3d)) / 100) / div;
	stats->base.def = mul * (base + base * (0x1e - randomLimit(0x3d)) / 100) / div;
	stats->base.speed = mul * (base + base * (0x1e - randomLimit(0x3d)) / 100) / div;
	stats->base.brain = mul * (base + base * (0x1e - randomLimit(0x3d)) / 100) / div;
	stats->base.hp = mul * (base * 10 + base * 10 * (0x1e - randomLimit(0x3d)) / 100) / div;
	stats->base.mp = mul * (base * 10 + base * 10 * (0x1e - randomLimit(0x3d)) / 100) / div;
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
	entity->isOnMap = 1;
	entity->isOnScreen = 1;
	nb = 0;
	na = 0;
	for (i = 0; i < 0xf; i++) {
		tech = entityGetTechFromAnim(entity, i + 0x2e);
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
		} else {
			listB[nb++] = i;
		}
	}
	if (nb != 0) {
		for (i = 0; i < nb; i++) {
			swapByte(&listB[i], &listB[randomLimit(nb)]);
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
			swapByte(&listA[i], &listA[randomLimit(na)]);
		}
		if (randomLimit(0xa) == 0) {
			stats->base.moves[2] = listA[0] + 0x2e;
		}
	}
	stats->base.moves[3] = 0xff;
	for (i = 0; i < 0x10; i++) {
		if (DIGIMON_DATA[entity->type].moves[i] >= 0x3a &&
		    DIGIMON_DATA[entity->type].moves[i] < 0x71) {
			stats->base.moves[3] = i + 0x2e;
			return;
		}
	}
}

void VS_initializeDemoCamera(void)
{
	VIEWPORT_DISTANCE = 500;
	GsSetProjection(VIEWPORT_DISTANCE);
	STDVS_CAMERA.rotation.vx = 100;
	STDVS_CAMERA.rotation.vy = 0;
	STDVS_CAMERA.rotation.vz = 0;
	STDVS_CAMERA.translation.vx = 0;
	STDVS_CAMERA.translation.vy = 500;
	STDVS_CAMERA.translation.vz = 3000;
	STDVS_CAMERA.view.super = NULL;
	RotMatrix(&STDVS_CAMERA.rotation, &STDVS_CAMERA.view.view);
	TransMatrix(&STDVS_CAMERA.view.view, &STDVS_CAMERA.translation);
	STDVS_CAMERA.coord.flg = 0;
	GsSetView2(&STDVS_CAMERA.view);
}

void VS_initializeDemoLighting(void)
{
	LIGHT_DATA.light[0].vx = 1000;
	LIGHT_DATA.light[0].vy = 1000;
	LIGHT_DATA.light[0].vz = 1000;
	LIGHT_DATA.light[0].r = 255;
	LIGHT_DATA.light[0].g = 255;
	LIGHT_DATA.light[0].b = 255;
	GsSetFlatLight(0, &LIGHT_DATA.light[0]);
	LIGHT_DATA.light[1].vx = -1000;
	LIGHT_DATA.light[1].vy = 1000;
	LIGHT_DATA.light[1].vz = -1000;
	LIGHT_DATA.light[1].r = 160;
	LIGHT_DATA.light[1].g = 160;
	LIGHT_DATA.light[1].b = 160;
	GsSetFlatLight(1, &LIGHT_DATA.light[1]);
	LIGHT_DATA.light[2].vx = -20;
	LIGHT_DATA.light[2].vy = 20;
	LIGHT_DATA.light[2].vz = 100;
	LIGHT_DATA.light[2].r = 96;
	LIGHT_DATA.light[2].g = 96;
	LIGHT_DATA.light[2].b = 96;
	GsSetAmbient(0x400, 0x400, 0x400);
	GsSetLightMode(0);
}

void VS_setupTrialBattle(void)
{
	int32_t i;

	loadTIMFile(VS_PATH_DEMO_ETCDAT_SYSTEM_W_TIM, GENERAL_BUFFER);
	loadTIMFile(VS_PATH_DEMO_STDDAT_TAISEN1_TIM, GENERAL_BUFFER);
	loadTIMFile(VS_PATH_DEMO_STDDAT_TAISEN2_TIM, GENERAL_BUFFER);
	loadTIMFile(VS_PATH_DEMO_STDDAT_16TAISEN_TIM, GENERAL_BUFFER);
	loadTIMFile(VS_PATH_DEMO_STDDAT_TAISEN_F_TIM, GENERAL_BUFFER);
	loadTIMFile(VS_PATH_DEMO_STDDAT_TIME_TIM, GENERAL_BUFFER);
	for (i = 0; i < 5; i++) {
		VS_BATTLE_SETUP.fighters[0][i] = 0xff;
		VS_BATTLE_SETUP.fighters[1][i] = 0xff;
	}
	VS_BATTLE_SETUP.battleCount = 3;
	for (i = 0; i < VS_BATTLE_SETUP.battleCount; i++) {
		VS_BATTLE_SETUP.fighters[0][i] = VS_BATTLE_SETUP.fighters[1][i] = i;
		strcpy(VS_DIGIMON_P1_PTR[i].name, VS_STR_DIGIMON_1);
		strcpy(VS_DIGIMON_P2_PTR[i].name, VS_STR_DIGIMON_2);
	}
	VS_BATTLE_SETUP.stage = randomLimit(3);
	VS_randomizeTrialDigimonList(VS_DIGIMON_P1_PTR, 3);
	VS_randomizeTrialDigimonList(VS_DIGIMON_P2_PTR, 3);
}

void VS_randomizeTrialDigimonList(RegisteredDigimon *list, int32_t count)
{
	int32_t i;

	for (i = 0; i < count; i++, list++) {
		list->digimonId = VS_DEMO_DIGIMON[randomLimit(0x30)];
		VS_randomizeTrialDigimon(list);
	}
}

void VS_randomizeTrialDigimon(RegisteredDigimon *digimon)
{
	int32_t nb;
	int32_t na;
	int32_t div;
	int32_t mul;
	uint8_t listB[16];
	uint8_t listA[16];
	Entity entity;
	int16_t tech;
	int32_t i;
	int16_t base;

	base = 100;
	switch (DIGIMON_DATA[digimon->digimonId].level) {
	case 3:
		div = 1;
		mul = 1;
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
	digimon->offense = mul * (base + base * (0x1e - randomLimit(0x3d)) / 100) / div;
	digimon->defense = mul * (base + base * (0x1e - randomLimit(0x3d)) / 100) / div;
	digimon->speed = mul * (base + base * (0x1e - randomLimit(0x3d)) / 100) / div;
	digimon->brains = randomLimit(0x65) + 0x1f4;
	digimon->hp = mul * (base * 10 + base * 10 * (0x1e - randomLimit(0x3d)) / 100) / div;
	digimon->mp = mul * (base * 10 + base * 10 * (0x1e - randomLimit(0x3d)) / 100) / div + 0x7d0;
	if (digimon->offense >= 0x3e8) {
		digimon->offense = 0x3e7;
	}
	if (digimon->defense >= 0x3e8) {
		digimon->defense = 0x3e7;
	}
	if (digimon->speed >= 0x3e8) {
		digimon->speed = 0x3e7;
	}
	if (digimon->brains >= 0x3e8) {
		digimon->brains = 0x3e7;
	}
	if (digimon->hp >= 0x2710) {
		digimon->hp = 0x270f;
	}
	if (digimon->mp >= 0x2710) {
		digimon->mp = 0x270f;
	}
	nb = 0;
	na = 0;
	entity.type = digimon->digimonId;
	for (i = 0; i < 0xf; i++) {
		tech = entityGetTechFromAnim(&entity, i + 0x2e);
		if (tech == 0xff) {
			continue;
		}
		if (tech >= 0x3a && tech < 0x71) {
			continue;
		}
		if (MOVE_DATA[tech].range == 4 || tech == 0x2d) {
			listA[na++] = i;
		} else {
			listB[nb++] = i;
		}
	}
	if (nb != 0) {
		for (i = 0; i < nb; i++) {
			swapByte(&listB[i], &listB[randomLimit(nb)]);
		}
		for (i = 0; i < 3; i++) {
			digimon->moves[i] = 0xff;
			if (i <= nb - 1) {
				digimon->moves[i] = listB[i] + 0x2e;
			}
		}
	} else {
		digimon->moves[0] = 0x2e;
		digimon->moves[1] = 0xff;
		digimon->moves[2] = 0xff;
	}
	if (na != 0) {
		for (i = 0; i < na; i++) {
			swapByte(&listA[i], &listA[randomLimit(na)]);
		}
		if (randomLimit(0xa) == 0) {
			digimon->moves[2] = listA[0] + 0x2e;
		}
	}
}

void VS_playDemo(void)
{
	initializeMusic();
	VS_setupDemoBattle();
	VS_runDemoCombat();
	VS_cleanupDemoBattle();
	finalizeMusic();
}

void VS_initializeTrialBattle(RegisteredDigimon *fightersP1, RegisteredDigimon *fightersP2)
{
	VS_DIGIMON_P1_PTR = fightersP1;
	VS_DIGIMON_P2_PTR = fightersP2;
	VS_setupTrialBattle();
	VS_initializeVS();
	loadStackedTIMFile(VS_PATH_DEMO_ETCDAT_ETCTIM_BIN);
	loadTIMFile(VS_PATH_DEMO_ETCNA_TITLE2_TIM, GENERAL_BUFFER_PTR);
}
