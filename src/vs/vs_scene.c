#include <libgs.h>
#include <libgte.h>

#include <dw/clock.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/vs.h>

extern uint8_t MAIN_D_80135260;
extern int16_t MAIN_D_8013525C;
extern int16_t MAIN_D_8013525E;
extern uint8_t MAIN_D_80135261;
extern uint8_t MAIN_D_80135262;
extern int32_t MAIN_D_80135268;
extern int32_t MAIN_D_801B1B98[];
extern int32_t MAIN_D_801B1BB8[];
extern int32_t MAIN_D_801B1BBC[];
extern char **MAIN_D_80135298;
extern int8_t MAIN_D_8013529C;
extern int16_t MAIN_D_801B1C0C[];
extern int16_t MAIN_D_801B1C0E[];
extern int16_t MAIN_D_801B1C10[];
extern int32_t MAIN_D_801B1C14[];
extern int32_t MAIN_D_801B1C18[];
extern int32_t MAIN_D_801B1C1C[];
extern GsRVIEW2 GS_VIEWPOINT;
extern int16_t VS_D_80071A0C[];
extern uint8_t MAIN_D_80135288;
extern int32_t MAIN_D_801352A8;
extern int32_t VIEWPORT_DISTANCE;

void VS__tickInput(void);
void VS_resetMatchState(void);
void VS_loadVSAssets(void);
void VS_addInputObjects(void);
void VS_loadFighterEntities(void);
void VS_unloadFighterEntities(void);
void VS_renderVersusFlash(void);
void VS_renderFighterNamePlate(int32_t slot);
void VS_renderRoundPips(int32_t slot);
void VS_renderRoundScores(int32_t slot);
void VS_renderVersusBanner(int32_t slot);
void VS_renderVersusText(void);
void VS_initializeCamera(void);
void VS_initializeLighting(void);
void VS_loadTIMToVRAM(void);
int32_t VS_getFirstSpecialMove(int32_t index);
void VS_playVersusIntroSequence(void);
void VS_tickPlaytime(void);
void VS_tickVersusIntro(void);
void VS_renderVersusIntro(void);
void VS_loadArenaTIMToVRAM(void);
void VS_loadArenaAssets(void);
void VS_unloadArenaAssets(void);
void VS_addArenaRenderers(void);
void VS_renderArenaViewLeft(void);
void VS_renderArenaViewRight(void);
void VS_renderArenaViewFull(void);
void VS_removeArenaRenderers(void);
void VS_applyCamera(void);
void VS_setCameraOrbit(void);
void VS_setCameraYXZ(void);
void VS_setCameraToEntity(void);
void VS_setViewpointRotationFromEntity(void);
void VS_setCameraLookAtEntity(void);
void VS_applyViewpoint(void);
void VS_setCameraSimple(void);
void VS_setCameraParams(int16_t a, int16_t b, int16_t c, int32_t d, int16_t e, int16_t f);
void VS_setVSPhase(int32_t arg);
void VS_tickVSPhase(void);
void VS_removeVSPhase(void);
void VS_getFighterDistance(void);
void VS_setViewpointFromBone(void);
void VS_updateCameraLerp(void);
void VS_isPositionNearEntity(void);
int32_t VS_interpolateClamped2(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end);
void VS_tickCameraChase(void);
void VS_tickCameraIntro(void);
void VS_startCameraIntro(void);
void VS_removeCameraIntro(void);
void VS_applyEntityViewpoint(void);
void VS_renderCounterDigits(int32_t x, int32_t y, int32_t a, uint8_t b, int32_t c);
void VS_tickFighterCounter(void);
void VS_renderFighterCounter(void);
void addObject(int32_t objectId, int32_t instanceId, void *tick, void *render);
void removeObject(int32_t objectId, int32_t instanceId);

static void *vs_scene_functions[] = {
	VS_removeFighterCounter,
	VS_renderFighterCounter,
	VS_tickFighterCounter,
	VS_addFighterCounter,
	VS_renderCounterDigits,
	VS_applyEntityViewpoint,
	VS_removeCameraIntro,
	VS_startCameraIntro,
	VS_tickCameraIntro,
	VS_startCameraChase,
	VS_tickCameraChase,
	VS_interpolateClamped2,
	VS_isPositionNearEntity,
	VS_updateCameraLerp,
	VS_setViewpointFromBone,
	VS_getFighterDistance,
	VS_setRandomViewpoint,
	VS_selectRandomCamera,
	VS_removeVSPhase,
	VS_tickVSPhase,
	VS_setVSPhase,
	VS_setCameraParams,
	VS_setCameraSimple,
	VS_applyViewpoint,
	VS_setCameraLookAtEntity,
	VS_setViewpointRotationFromEntity,
	VS_setCameraToEntity,
	VS_setCameraYXZ,
	VS_setCameraOrbit,
	VS_applyCamera,
	VS_removeArenaRenderers,
	VS_renderArenaViewFull,
	VS_renderArenaViewRight,
	VS_renderArenaViewLeft,
	VS_addArenaRenderers,
	VS_unloadArenaAssets,
	VS_loadArenaAssets,
	VS_loadArenaTIMToVRAM,
	VS_renderVersusIntro,
	VS_tickVersusIntro,
	VS_tickPlaytime,
	VS_playVersusIntroSequence,
	VS_initializeVS,
	VS_getFirstSpecialMove,
	VS_loadTIMToVRAM,
	VS_initializeLighting,
	VS_initializeCamera,
	VS_renderVersusText,
	VS_renderVersusBanner,
	VS_renderRoundScores,
	VS_renderRoundPips,
	VS_renderFighterNamePlate,
	VS_renderVersusFlash,
	VS_unloadFighterEntities,
	VS_loadFighterEntities,
	VS_addInputObjects,
	VS_loadVSAssets,
	VS_resetMatchState,
};

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_resetMatchState);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_loadVSAssets);

void VS_addInputObjects(void)
{
	addObject(0x1b2, 0, VS__tickInput, NULL);
	addObject(0x1b2, 1, VS__tickInput, NULL);
	PLAYTIME_FRAMES = 0;
	addObject(0xfb9, 0, VS_tickPlaytime, NULL);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_loadFighterEntities);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_unloadFighterEntities);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderVersusFlash);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderFighterNamePlate);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderRoundPips);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderRoundScores);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderVersusBanner);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderVersusText);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_initializeCamera);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_initializeLighting);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_loadTIMToVRAM);

int32_t VS_getFirstSpecialMove(int32_t index)
{
	int32_t i;
	uint8_t move;

	for (i = 15; i >= 0; i--) {
		if (DIGIMON_DATA[index].moves[i] != 0xff) {
			move = DIGIMON_DATA[index].moves[i];
			if (move >= 0x3a) {
				return i + 0x2e;
			}
		}
	}
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_initializeVS);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_playVersusIntroSequence);

void VS_tickPlaytime(void)
{
	PLAYTIME_FRAMES++;
}

void VS_tickVersusIntro(void)
{
	uint32_t state;

	state = MAIN_D_80135260;
	switch (state) {
	case 0:
		MAIN_D_8013525E++;
		if (MAIN_D_8013525E == 10) {
			MAIN_D_80135260 = state + 1;
		}
		break;
	case 1:
		MAIN_D_8013525C++;
		if (MAIN_D_8013525C == 20) {
			MAIN_D_80135260 = state + 1;
			MAIN_D_80135262 = 1;
		}
		break;
	case 2:
		MAIN_D_8013525E--;
		if (MAIN_D_8013525E == 0) {
			MAIN_D_80135260 = state + 1;
			MAIN_D_8013525C = 0;
		}
		break;
	case 3:
		MAIN_D_8013525C++;
		if (MAIN_D_8013525C == 10) {
			MAIN_D_80135260 = state + 1;
		}
		break;
	case 4:
		MAIN_D_80135261++;
		if (MAIN_D_80135261 == VS_D_800716B3[0]) {
			MAIN_D_80135260 = state + 1;
		} else {
			MAIN_D_80135260 = 0;
			MAIN_D_8013525C = 0;
		}
		break;
	case 5:
		MAIN_D_8013525C++;
		if (MAIN_D_8013525C == 0x50) {
			MAIN_D_80135262 = 1;
		}
		break;
	}
}

void VS_renderVersusIntro(void)
{
	if (MAIN_D_80135260 == 1) {
		VS_renderVersusFlash();
	}
	VS_renderFighterNamePlate(0);
	VS_renderFighterNamePlate(1);
	VS_renderRoundPips(0);
	VS_renderRoundPips(1);
	VS_renderRoundScores(0);
	VS_renderRoundScores(1);
	VS_renderVersusBanner(0);
	VS_renderVersusBanner(1);
	VS_renderVersusText();
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_loadArenaTIMToVRAM);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_loadArenaAssets);

void VS_unloadArenaAssets(void)
{
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_addArenaRenderers);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderArenaViewLeft);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderArenaViewRight);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderArenaViewFull);

void VS_removeArenaRenderers(void)
{
	removeObject(0x1a7, 0);
}

void VS_applyCamera(void)
{
	MAIN_D_801B1BB8[0] = 0;
	RotMatrix((SVECTOR *)MAIN_D_801B1C0C, (MATRIX *)MAIN_D_801B1B98);
	TransMatrix((MATRIX *)MAIN_D_801B1B98, (VECTOR *)MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2((GsVIEW2 *)MAIN_D_801B1B98);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_setCameraOrbit);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_setCameraYXZ);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_setCameraToEntity);

void VS_setViewpointRotationFromEntity(void)
{
	MATRIX *m;

	m = (MATRIX *)(MAIN_D_80135298[1] + 0xbc);
	GS_VIEWPOINT.vrx = m->t[0];
	GS_VIEWPOINT.vry = m->t[1];
	GS_VIEWPOINT.vrz = m->t[2];
	GsSetRefView2(&GS_VIEWPOINT);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_setCameraLookAtEntity);

void VS_applyViewpoint(void)
{
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_setCameraSimple);

void VS_setCameraParams(int16_t a, int16_t b, int16_t c, int32_t d, int16_t e, int16_t f)
{
	MAIN_D_801B1C0C[0] = a;
	MAIN_D_801B1C0E[0] = b;
	MAIN_D_801B1C10[0] = c;
	MAIN_D_801B1C14[0] = d;
	MAIN_D_801B1C18[0] = e;
	MAIN_D_801B1C1C[0] = f;
}

void VS_setVSPhase(int32_t arg)
{
	addObject(0x1a8, 0, VS_tickVSPhase, NULL);
	MAIN_D_80135268 = arg;
	MAIN_D_8013529C = 0;
}

void VS_tickVSPhase(void)
{
	switch (MAIN_D_80135268) {
	case 0:
		VS_applyCamera();
		break;
	case 1:
		VS_setCameraOrbit();
		break;
	case 2:
		VS_setCameraYXZ();
		break;
	case 3:
	case 5:
		VS_setCameraToEntity();
		break;
	case 4:
	case 6:
		VS_setViewpointRotationFromEntity();
		break;
	case 9:
		VS_applyEntityViewpoint();
		break;
	case 7:
		VS_setCameraLookAtEntity();
		break;
	case 8:
		VS_applyViewpoint();
		break;
	case 10:
		VS_setCameraSimple();
		break;
	}
}

void VS_removeVSPhase(void)
{
	removeObject(0x1a8, 0);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_selectRandomCamera);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_setRandomViewpoint);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_getFighterDistance);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_setViewpointFromBone);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_updateCameraLerp);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_isPositionNearEntity);

int32_t VS_interpolateClamped2(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end)
{
	int32_t tmp;

	if (hi < lo) {
		tmp = lo;
		lo = hi;
		hi = tmp;
	}

	t = t < lo ? lo : (hi < t ? hi : t);
	if (lo >= t) {
		return start;
	}

	return start + ((end - start) * (t - lo) / (hi - lo));
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_tickCameraChase);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_startCameraChase);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_tickCameraIntro);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_startCameraIntro);

void VS_removeCameraIntro(void)
{
	removeObject(0x1ad, 0);
	VS_D_80071A0C[0] = -1;
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_applyEntityViewpoint);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderCounterDigits);

void VS_addFighterCounter(uint8_t arg)
{
	if ((MAIN_D_801352A8 == 0) && (arg != 0)) {
		MAIN_D_80135288 = arg;
		addObject(0x1ac, 0, VS_tickFighterCounter, VS_renderFighterCounter);
		MAIN_D_801352A8 = 1;
	}
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_tickFighterCounter);

void VS_renderFighterCounter(void)
{
	VS_renderCounterDigits(-0xd, -0x61, 2, MAIN_D_80135288, 3);
}

void VS_removeFighterCounter(void)
{
	if (MAIN_D_801352A8 != 0) {
		removeObject(0x1ac, 0);
		MAIN_D_801352A8 = 0;
	}
}
