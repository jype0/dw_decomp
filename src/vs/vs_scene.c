#include <libgs.h>
#include <libgte.h>

#include <dw/clock.h>
#include <dw/main.h>
#include <dw/math.h>
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

extern SVECTOR MAIN_D_80134AB4;
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);

extern SVECTOR MAIN_D_80134A84;
extern SVECTOR MAIN_D_80134A8C;
int32_t MAIN_func_800DA63C(int32_t a, int32_t b, int16_t *c, int16_t *d, int16_t *e);
extern int16_t VS_D_800706C8[];
extern int16_t VS_D_80071A0E[];
extern int16_t VS_D_80071A10[];
extern int16_t VS_D_80071A12[];
extern Entity *VS_D_80071A18[];
extern int16_t VS_D_80071A30[];
extern int16_t VS_D_80071A88[];
extern int16_t VS_D_80071AE0[];
int32_t lerp(int32_t a, int32_t b, int32_t lo, int32_t hi, int32_t t);

extern int16_t MAIN_D_80134D66;
extern int32_t MAIN_D_8013528C;

extern SVECTOR MAIN_D_80134AA4;
extern SVECTOR MAIN_D_80134AAC;

int32_t getDistance(int32_t x, int32_t y, int32_t z);

extern int16_t VS_D_800706A8[];
extern int16_t VS_D_800706AA[];
extern int16_t VS_D_800706AC[];

void GsGetTimInfo(unsigned long *tim, GsIMAGE *img);
void readFile(char *path, void *dest);

extern GsOT *ACTIVE_ORDERING_TABLE;
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount, int32_t *digits);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uvX, int32_t uvY, int32_t width, int32_t height);

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
void VS_loadTIMToVRAM(char *path);
int32_t VS_getFirstSpecialMove(int32_t index);
void VS_playVersusIntroSequence(void);
void VS_tickPlaytime(void);
void VS_tickVersusIntro(void);
void VS_renderVersusIntro(void);
void VS_loadArenaTIMToVRAM(char *path, int32_t count);
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
int32_t VS_getFighterDistance(VECTOR *self, VECTOR *other, VECTOR *target);
void VS_setViewpointFromBone(Entity *entity, SVECTOR *offset, SVECTOR *rot, int32_t dist);
void VS_updateCameraLerp(int32_t t, int32_t flip);
int32_t VS_isPositionNearEntity(Entity *entity, VECTOR *pos);
int32_t VS_interpolateClamped2(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end);
void VS_tickCameraChase(void);
void VS_tickCameraIntro(void);
void VS_startCameraIntro(void);
void VS_removeCameraIntro(void);
void VS_applyEntityViewpoint(void);
void VS_renderCounterDigits(int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer);
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

void VS_renderVersusFlash(void)
{
	POLY_FT4 *prim;
	int32_t y;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	prim->tpage = 5;
	prim->clut = GetClut(0x40, 0x1E9);
	setUVDataPolyFT4(prim, 0x30, 0x10, 0x4E, 0x18);

	y = MAIN_D_80135261 * 20 - 0x39;
	setPosDataPolyFT4(prim, -0x27, y, 0x4E, 0x18);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0x1E, prim++);
	GsSetWorkBase((PACKET *)prim);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderFighterNamePlate);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderRoundPips);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderRoundScores);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderVersusBanner);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_renderVersusText);

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_initializeLighting);

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

void VS_loadArenaTIMToVRAM(char *path, int32_t count)
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

void VS_loadTIMToVRAM(char *path)
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

void VS_removeVSPhase(void)
{
	removeObject(0x1a8, 0);
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_selectRandomCamera);

void VS_setRandomViewpoint(Entity *entity, int32_t idx)
{
	VECTOR v;
	VECTOR out;
	MATRIX m;

	if (random(3) != 0) {
		return;
	}
	if (MAIN_D_80135268 == 7) {
		return;
	}

	VIEWPORT_DISTANCE = 0x1f4;
	GS_VIEWPOINT.super = NULL;

	if (idx < 4) {
		MAIN_D_80135298 = (char **)entity;
		MAIN_D_80135268 = 4;
		RotMatrix(&entity->posData->rotation, &m);
		v.vx = VS_D_800706A8[idx * 3];
		v.vy = VS_D_800706AA[idx * 3];
		v.vz = VS_D_800706AC[idx * 3];
		ApplyMatrixLV(&m, &v, &out);
		out.vx += ((Entity *)MAIN_D_80135298)->posData->location.vx;
		out.vz += ((Entity *)MAIN_D_80135298)->posData->location.vz;
		GS_VIEWPOINT.vpx = out.vx;
		GS_VIEWPOINT.vpy = out.vy;
		GS_VIEWPOINT.vpz = out.vz;
	} else {
		MAIN_D_80135268 = 6;
		GS_VIEWPOINT.vpx = VS_D_800706A8[idx * 3];
		GS_VIEWPOINT.vpy = VS_D_800706AA[idx * 3];
		GS_VIEWPOINT.vpz = VS_D_800706AC[idx * 3];
	}

	GS_VIEWPOINT.rz = 0;
}

void VS_initializeCamera(void)
{
	VIEWPORT_DISTANCE = 500;
	GsSetProjection(500);
	MAIN_D_801B1C0C[0] = 100;
	MAIN_D_801B1C0C[1] = 0;
	MAIN_D_801B1C0C[2] = 0;
	MAIN_D_801B1C14[0] = 0;
	MAIN_D_801B1C14[1] = 500;
	MAIN_D_801B1C14[2] = 3000;
	MAIN_D_801B1BB8[0] = 0;
	RotMatrix((SVECTOR *)MAIN_D_801B1C0C, (MATRIX *)MAIN_D_801B1B98);
	TransMatrix((MATRIX *)MAIN_D_801B1B98, (VECTOR *)MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2((GsVIEW2 *)MAIN_D_801B1B98);
}

void VS_setViewpointFromBone(Entity *entity, SVECTOR *offset, SVECTOR *rot, int32_t dist)
{
	MATRIX m1;
	SVECTOR out1;
	MATRIX m2;
	SVECTOR bone;
	MATRIX m3;
	VECTOR v;

	calculateBoneMatrix(entity, offset->pad, &m1);
	ApplyMatrixSV(&m1, offset, &out1);
	GS_VIEWPOINT.vrx = m1.t[0] + out1.vx;
	GS_VIEWPOINT.vry = m1.t[1] + out1.vy;
	GS_VIEWPOINT.vrz = m1.t[2] + out1.vz;
	bone = MAIN_D_80134AB4;
	calculateBoneMatrix(entity, 1, &m2);
	ApplyMatrixSV(&m2, &bone, &bone);
	GS_VIEWPOINT.vry = m2.t[1] + bone.vy;
	RotMatrixZYX(rot, &m3);
	v.vx = 0;
	v.vy = 0;
	v.vz = dist;
	ApplyMatrixLV(&m3, &v, (VECTOR *)&GS_VIEWPOINT);
	GS_VIEWPOINT.vpx += GS_VIEWPOINT.vrx;
	GS_VIEWPOINT.vpy += GS_VIEWPOINT.vry;
	GS_VIEWPOINT.vpz += GS_VIEWPOINT.vrz;
	VIEWPORT_DISTANCE = 0x15e;
}

void VS_setCameraToEntity(void)
{
	SVECTOR rot;
	VECTOR v;
	VECTOR out;

	rot = *(SVECTOR *)MAIN_D_801B1C0C;
	rot.vy -= ((Entity *)MAIN_D_80135298)->posData->rotation.vy;
	rot.vy &= 0xfff;
	RotMatrix(&rot, (MATRIX *)MAIN_D_801B1B98);
	v = *(VECTOR *)MAIN_D_801B1C14;
	ApplyMatrixLV((MATRIX *)MAIN_D_801B1B98,
	              &((Entity *)MAIN_D_80135298)->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix((MATRIX *)MAIN_D_801B1B98, &v);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2((GsVIEW2 *)MAIN_D_801B1B98);
}

int32_t VS_getFighterDistance(VECTOR *self, VECTOR *other, VECTOR *target)
{
	int32_t toTarget;
	int32_t toOther;

	toTarget = getDistance(self->vx - target->vx, self->vy - target->vy, self->vz - target->vz);
	toOther = getDistance(other->vx - self->vx, other->vy - self->vy, other->vz - self->vz);
	return (toTarget * 100) / toOther;
}

void VS_updateCameraLerp(int32_t t, int32_t flip)
{
	SVECTOR off;
	SVECTOR rot;
	int32_t base;
	int32_t dbl;
	int32_t dist;

	off = MAIN_D_80134AA4;
	rot = MAIN_D_80134AAC;
	base = ((((DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].height +
	           DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].radius) /
	          2) *
	         0x62c) /
	        450);
	dbl = base * 2;
	rot.vx = lerp(0x2aa, 0xe3, 0, 0x64, t);
	rot.vy = lerp(0x5c7, 0xa38, 0, 0x64, t);

	if (rot.vy < 0x801) {
		dist = lerp(base, dbl, 0x5c7, 0x800, rot.vy);
	} else {
		dist = lerp(dbl, base, 0x800, 0xa38, rot.vy);
	}

	if (flip != 0) {
		rot.vy = -rot.vy;
	}

	rot.vy += ((Entity *)MAIN_D_80135298)->posData->rotation.vy;
	off.vy = (-DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].height * 2) / 3;
	VS_setViewpointFromBone((Entity *)MAIN_D_80135298, &off, &rot, dist);
}

int32_t VS_isPositionNearEntity(Entity *entity, VECTOR *pos)
{
	if (pos->vx - 50 > entity->posData->location.vx) {
		goto no;
	}
	if (pos->vx + 50 < entity->posData->location.vx) {
		goto no;
	}
	if (pos->vz - 50 > entity->posData->location.vz) {
		goto no;
	}
	if (entity->posData->location.vz > pos->vz + 50) {
		goto no;
	}
	return 1;
no:
	return 0;
}

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

void VS_tickCameraIntro(void)
{
	SVECTOR off;
	SVECTOR rot;
	int16_t *p;

	p = VS_D_80071A0C;
	if ((p[0] >= 0x1e) && (p[0] < 0x3c)) {
		VS_D_80071A10[0] = lerp(VS_D_80071A10[0], VS_D_80071A12[0], p[0], 0x3c, p[0] + 1);
	}

	if (p[0] >= 0x1e) {
		p[1] += (int16_t)VS_interpolateClamped2(0x1e, 0x3c, p[0], 0, 0x5b);
	}

	off = MAIN_D_80134A84;
	rot = MAIN_D_80134A8C;
	rot.vy = VS_D_80071A0E[0];
	VS_D_80071A10[0] = MAIN_func_800DA63C(0x16, p[0], VS_D_800706C8, VS_D_80071A30, VS_D_80071A88);
	VS_setViewpointFromBone(*(Entity **)&p[6], &off, &rot, VS_D_80071A10[0]);
	GS_VIEWPOINT.vry = (-DIGIMON_DATA[VS_D_80071A18[0]->type].height * 2) / 3;
	GS_VIEWPOINT.vpy = -MAIN_func_800DA63C(0x16, p[0], VS_D_800706C8, VS_D_80071AE0, VS_D_80071A88);
	p[0]++;
}

INCLUDE_ASM("asm/vs/nonmatchings/vs_scene", VS_startCameraIntro);

void VS_removeCameraIntro(void)
{
	removeObject(0x1ad, 0);
	VS_D_80071A0C[0] = -1;
}

void VS_applyEntityViewpoint(void)
{
	char *p;

	VIEWPORT_DISTANCE = 0x15E;
	GsSetProjection(0x15E);
	p = MAIN_D_80135298[1] + 0x34;
	GS_VIEWPOINT.vrx = *(int32_t *)(p + 0x14);
	GS_VIEWPOINT.vry = -DIGIMON_DATA[(int32_t)MAIN_D_80135298[0]].height * 2 / 3;
	GS_VIEWPOINT.vrz = *(int32_t *)(p + 0x1C);
	GsSetRefView2(&GS_VIEWPOINT);
}

void VS_renderCounterDigits(int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer)
{
	POLY_FT4 *prim;
	int32_t i;
	uint32_t width;
	int32_t count;
	int32_t buf[6];

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	prim->tpage = 0xD;
	prim->clut = GetClut(0x10, 0x1E0);
	setUVDataPolyFT4(prim, 0x78, 0x30, 8, 0x12);
	setPosDataPolyFT4(prim, -0x1C, -0x63, 8, 0x12);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	SetPolyFT4(prim);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	prim->tpage = 0xD;
	prim->clut = GetClut(0x10, 0x1E0);
	setUVDataPolyFT4(prim, 0x80, 0x30, 8, 0x12);
	setPosDataPolyFT4(prim, 0x14, -0x63, 8, 0x12);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	width = digits;
	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		SetPolyFT4(prim);
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
		prim->tpage = 0xD;
		prim->clut = GetClut(0x10, 0x1E0);
		setUVDataPolyFT4(prim, buf[i] * 12, 0x30, 0xC, 0xF);
		setPosDataPolyFT4(prim, x + ((((int32_t)width - 1) - i) * 14), y, 0xC, 0xF);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	SetPolyFT4(prim);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	prim->tpage = GetTPage(1, 0, 0x2C0, 0);
	prim->clut = GetClut(0x200, 0xFF);
	setUVDataPolyFT4(prim, 0, 0x78, 0x42, 0x1D);
	setPosDataPolyFT4(prim, -0x21, -0x68, 0x42, 0x1D);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void VS_addFighterCounter(uint8_t arg)
{
	if ((MAIN_D_801352A8 == 0) && (arg != 0)) {
		MAIN_D_80135288 = arg;
		addObject(0x1ac, 0, VS_tickFighterCounter, VS_renderFighterCounter);
		MAIN_D_801352A8 = 1;
	}
}

void VS_tickFighterCounter(void)
{
	if (MAIN_D_8013528C == 1) {
		MAIN_D_80134D66++;
		if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]]->anim.animId != 0x2B) {
			if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->anim.animId != 0x2B) {
				if (MAIN_D_80134D66 % 0x14 == 0) {
					if (MAIN_D_80135288 != 0) {
						MAIN_D_80135288--;
					}
				}
			}
		}
	}
}

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
