#include <libgs.h>
#include <libgte.h>

#include <dw/battle.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/rng.h>
#include <dw/types.h>
#include <dw/vecmath.h>
#include <dw/vs.h>
#include <dw/world_object.h>

extern GsVIEW2 STDVS_VIEW;
extern GsCOORDINATE2 MAIN_D_801B1BBC;
extern SVECTOR STDVS_VIEW_ROTATION[];
extern VECTOR STDVS_VIEW_TRANSLATION;
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t VIEWPORT_DISTANCE;
extern int16_t BATTLE_FRAME_COUNT;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t VS_CAMERA_STATE;
extern int32_t MAIN_D_80135284;
extern uint8_t VS_TIMER;
extern int32_t VS_TIMER_ACTIVE;
extern int16_t VS_DEFAULT_CAM_MIN_DISTANCE;
extern char **VS_FOCUSED_ENTITY;
extern int8_t VS_CAMERA_TIMER;

void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
int32_t processSomeArenaArrays(int32_t count, int32_t t, int32_t *keys, int32_t *values, int32_t *slopes);
int32_t lerp(int32_t a, int32_t b, int32_t lo, int32_t hi, int32_t t);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount, int32_t *digits);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uvX, int32_t uvY, int32_t width, int32_t height);
void VS_applyCamera(void);
void VS_setCameraOrbit(void);
void VS_setCameraYXZ(void);
void VS_setCameraToEntity(void);
void VS_setViewpointRotationFromEntity(void);
void VS_setCameraLookAtEntity(void);
void VS_applyViewpoint(void);
void VS_setCameraSimple(void);
void VS_setCameraParams(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f);
void VS_setVSPhase(int32_t arg);
void VS_tickVSPhase(void);
void VS_removeVSPhase(void);
int32_t VS_getFighterDistance(VECTOR *self, VECTOR *other, VECTOR *target);
void VS_setViewpointFromBone(Entity *entity, SVECTOR *offset, SVECTOR *rot, int32_t dist);
void VS_updateCameraLerp(int32_t t, int8_t flip);
int32_t VS_isPositionNearEntity(Entity *entity, VECTOR *pos);
int32_t VS_interpolateClamped2(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end);
void VS_tickCameraChase(void);
void startAnimation(Entity *entity, uint8_t animId);
void VS_tickCameraIntro(void);
void VS_startCameraIntro(Entity *target, Entity *entity);
void VS_addResultModelScene(Entity *entity);
void VS_removeCameraIntro(void);
void VS_applyEntityViewpoint(void);
void VS_renderCounterDigits(int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer);
void VS_tickFighterCounter(void);
void VS_renderFighterCounter(void);
void initializeSomeArenaArrays(int32_t count, int32_t arg1, int32_t arg2, int32_t *out);

static void *vs_camera_functions[] = {
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
};

CameraChase INTRO_CAMERA_CHASE;
int32_t VS_IS_TIMER_INITIALIZED;

static void *vs_camera_sbss_order[] = {
	&VS_IS_TIMER_INITIALIZED,
	&INTRO_CAMERA_CHASE,
};

SVECTOR INTRO_CAMERA_STAGE1_POS = { 0 };
SVECTOR INTRO_CAMERA_STAGE1_ROT = { 0, -1592, 0, 0 };
SVECTOR MAIN_D_80134A74 = { 0 };
SVECTOR MAIN_D_80134A7C = { 0, -1592, 0, 0 };
SVECTOR MAIN_D_80134A84 = { 0 };
SVECTOR MAIN_D_80134A8C = { 0 };
SVECTOR MAIN_D_80134A94 = { 0 };
SVECTOR MAIN_D_80134A9C = { 0 };
SVECTOR INTRO_CAMERA_STAGE2_POS = { 0 };
SVECTOR INTRO_CAMERA_STAGE2_ROT = { -227, 1479, 0, 0 };
SVECTOR MAIN_D_80134AB4 = { 0 };

// clang-format off

CameraPreset VS_D_8007063C[9] = {
	{ 0x00e0, 0x0140, 0x0000, 0x0000, 0x00c8, 0x05dc },
	{ 0x00e0, 0x0ec0, 0x0000, 0x0000, 0x00c8, 0x05dc },
	{ 0x0fa0, 0x0140, 0x0000, 0x0000, 0x0104, 0x04b0 },
	{ 0x0fa0, 0x0ec0, 0x0000, 0x0000, 0x0104, 0x04b0 },
	{ 0x00e0, 0x0800, 0x0000, 0x0000, 0x0104, 0x05dc },
	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x00c8, 0x05dc },
	{ 0x00e0, 0x0140, 0x0000, 0x0000, 0x0190, 0x0bb8 },
	{ 0x00e0, 0x0ec0, 0x0000, 0x0000, 0x0190, 0x0bb8 },
	{ 0x00e0, 0x0800, 0x0000, 0x0000, 0x0208, 0x0bb8 },
};

int16_t VS_D_800706A8[5][3] = {
	{ 0xfe00, 0xfe00, 0xfb50 },
	{ 0x0200, 0xfe00, 0xfb50 },
	{ 0xfe00, 0xfe00, 0x04b0 },
	{ 0x0200, 0xfe00, 0x04b0 },
	{ 0x0bb8, 0xfa24, 0x0bb8 },
};

int32_t VS_D_800706C8[22] = {
	0xffffffff, 0x00000000, 0x0000001e, 0x0000002d,
	0x0000003c, 0x00000050, 0x00000064, 0x00000078,
	0x0000008c, 0x000000a0, 0x000000b4, 0x000000c8,
	0x000000dc, 0x000000f0, 0x00000104, 0x00000118,
	0x0000012c, 0x00000140, 0x00000154, 0x00000168,
	0x00000000, 0x00000000,
};

// clang-format on

void VS_applyCamera(void)
{
	STDVS_VIEW.super = NULL;
	RotMatrix(STDVS_VIEW_ROTATION, &STDVS_VIEW.view);
	TransMatrix(&STDVS_VIEW.view, &STDVS_VIEW_TRANSLATION);
	MAIN_D_801B1BBC.flg = 0;
	GsSetView2(&STDVS_VIEW);
}

void VS_setCameraOrbit(void)
{
	VECTOR *a;
	VECTOR *b;
	VECTOR diff;
	int32_t dist;
	int32_t d;
	int32_t ang;

	a = &ENTITY_TABLE[2]->posData->location;
	b = &ENTITY_TABLE[1]->posData->location;
	diff.vx = a->vx - b->vx;
	diff.vy = 0;
	diff.vz = a->vz - b->vz;
	dist = SquareRoot0(diff.vx * diff.vx + diff.vz * diff.vz);
	d = (dist * VIEWPORT_DISTANCE) / 200u;
	if (d < (int16_t)VS_DEFAULT_CAM_MIN_DISTANCE) {
		d = VS_DEFAULT_CAM_MIN_DISTANCE;
	}
	if (d < (int16_t)VS_DEFAULT_CAM_MIN_DISTANCE + 0x12c) {
		MAIN_D_80135284 = 1;
	} else {
		MAIN_D_80135284 = 0;
	}
	if (d > 0x1068) {
		d = 0x1068;
	}
	ang = 0x1000 - _atan(diff.vx, diff.vz);
	STDVS_VIEW_TRANSLATION.vx = b->vx + (int32_t)diff.vx / 2 + (d * diff.vz) / dist;
	STDVS_VIEW_TRANSLATION.vy = -0x3e8;
	STDVS_VIEW_TRANSLATION.vz = b->vz + diff.vz / 2 - (d * diff.vx) / dist;
	STDVS_VIEW_ROTATION[0].vx = _atan(d, -0x2bc) + 0x800;
	STDVS_VIEW_ROTATION[0].vy = ang + 0x800;
	STDVS_VIEW_ROTATION[0].vz = 0;
	STDVS_VIEW.view = GsIDMATRIX;
	STDVS_VIEW.super = &MAIN_D_801B1BBC;
	RotMatrixYXZ(STDVS_VIEW_ROTATION, &MAIN_D_801B1BBC.coord);
	TransMatrix(&MAIN_D_801B1BBC.coord, &STDVS_VIEW_TRANSLATION);
	MAIN_D_801B1BBC.flg = 0;
	GsSetView2(&STDVS_VIEW);
}

void VS_setCameraYXZ(void)
{
	VECTOR *a;
	VECTOR *b;

	a = &ENTITY_TABLE[1]->posData->location;
	b = &ENTITY_TABLE[2]->posData->location;
	STDVS_VIEW.view = GsIDMATRIX;
	STDVS_VIEW.super = &MAIN_D_801B1BBC;
	STDVS_VIEW_TRANSLATION.vx = a->vx + (b->vx - a->vx) / 2;
	STDVS_VIEW_TRANSLATION.vz = a->vz + (b->vz - a->vz) / 2;
	STDVS_VIEW_TRANSLATION.vy = -0x1f40;
	STDVS_VIEW_ROTATION[0].vy = STDVS_VIEW_ROTATION[0].vz = 0;
	STDVS_VIEW_ROTATION[0].vx = -0x400;
	RotMatrixYXZ(STDVS_VIEW_ROTATION, &MAIN_D_801B1BBC.coord);
	TransMatrix(&MAIN_D_801B1BBC.coord, &STDVS_VIEW_TRANSLATION);
	MAIN_D_801B1BBC.flg = 0;
	GsSetView2(&STDVS_VIEW);
}

void VS_setViewpointRotationFromEntity(void)
{
	MATRIX *m;

	m = (MATRIX *)(VS_FOCUSED_ENTITY[1] + 0xbc);
	GS_VIEWPOINT.vrx = m->t[0];
	GS_VIEWPOINT.vry = m->t[1];
	GS_VIEWPOINT.vrz = m->t[2];
	GsSetRefView2(&GS_VIEWPOINT);
}

void VS_setCameraLookAtEntity(void)
{
	VECTOR v;
	VECTOR out;
	VECTOR diff;
	VECTOR *selfPos;
	VECTOR *otherPos;
	Entity *other;

	selfPos = &((Entity *)VS_FOCUSED_ENTITY)->posData->location;
	if ((Entity *)VS_FOCUSED_ENTITY == ENTITY_TABLE[1]) {
		other = ENTITY_TABLE[2];
	} else {
		other = ENTITY_TABLE[1];
	}
	otherPos = &other->posData->location;
	diff.vx = otherPos->vx - selfPos->vx;
	diff.vy = 0;
	diff.vz = otherPos->vz - selfPos->vz;
	STDVS_VIEW_ROTATION[0].vy = (-_atan(diff.vz, diff.vx) + 0x800) & 0xfff;
	RotMatrix(STDVS_VIEW_ROTATION, &STDVS_VIEW.view);
	v = STDVS_VIEW_TRANSLATION;
	ApplyMatrixLV(&STDVS_VIEW.view, &((Entity *)VS_FOCUSED_ENTITY)->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix(&STDVS_VIEW.view, &v);
	GsSetView2(&STDVS_VIEW);
}

void VS_applyViewpoint(void)
{
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void VS_setCameraSimple(void)
{
	STDVS_VIEW_ROTATION[0].vy += 2;
	STDVS_VIEW_ROTATION[0].vy &= 0xfff;
	STDVS_VIEW.super = NULL;
	RotMatrix(STDVS_VIEW_ROTATION, &STDVS_VIEW.view);
	TransMatrix(&STDVS_VIEW.view, &STDVS_VIEW_TRANSLATION);
	MAIN_D_801B1BBC.flg = 0;
	GsSetView2(&STDVS_VIEW);
}

void VS_setCameraParams(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f)
{
	STDVS_VIEW_ROTATION[0].vx = a;
	STDVS_VIEW_ROTATION[0].vy = b;
	STDVS_VIEW_ROTATION[0].vz = c;
	STDVS_VIEW_TRANSLATION.vx = d;
	STDVS_VIEW_TRANSLATION.vy = e;
	STDVS_VIEW_TRANSLATION.vz = f;
}

void VS_setVSPhase(int32_t arg)
{
	addObject(0x1a8, 0, (TickFunction)VS_tickVSPhase, NULL);
	VS_CAMERA_STATE = arg;
	VS_CAMERA_TIMER = 0;
}

void VS_tickVSPhase(void)
{
	switch (VS_CAMERA_STATE) {
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

// clang-format off
void VS_selectRandomCamera(entity, mode, sub)
	DigimonEntity *entity;
	int32_t mode;
	uint8_t sub;
// clang-format on
{
	CameraPreset *p;

	if (mode != 5) {
		if (randomLimit(3) != 0) {
			return;
		}
	}
	VS_FOCUSED_ENTITY = (char **)entity;
	STDVS_VIEW.super = NULL;
	if (sub != 3) {
		p = &VS_D_8007063C[mode];
	} else {
		p = &VS_D_8007063C[randomLimit(3) + 6];
	}
	VS_setCameraParams(p->unk0, p->unk2, p->unk4, p->unk6, p->unk8, p->unkA);
	if (mode < 5) {
		VS_CAMERA_STATE = 3;
		return;
	}
	VS_addResultModelScene((Entity *)entity == ENTITY_TABLE[2] ? ENTITY_TABLE[1] : ENTITY_TABLE[2]);
	VS_startCameraIntro(NULL, (Entity *)entity);
}

void VS_setRandomViewpoint(Entity *entity, int32_t idx)
{
	VECTOR v;
	VECTOR out;
	MATRIX m;

	if (randomLimit(3) != 0) {
		return;
	}
	if (VS_CAMERA_STATE == 7) {
		return;
	}

	VIEWPORT_DISTANCE = 0x1f4;
	GS_VIEWPOINT.super = NULL;

	if (idx < 4) {
		VS_FOCUSED_ENTITY = (char **)entity;
		VS_CAMERA_STATE = 4;
		RotMatrix(&((Entity *)VS_FOCUSED_ENTITY)->posData->rotation, &m);
		v.vx = VS_D_800706A8[idx][0];
		v.vy = VS_D_800706A8[idx][1];
		v.vz = VS_D_800706A8[idx][2];
		ApplyMatrixLV(&m, &v, &out);
		out.vx += ((Entity *)VS_FOCUSED_ENTITY)->posData->location.vx;
		out.vz += ((Entity *)VS_FOCUSED_ENTITY)->posData->location.vz;
		GS_VIEWPOINT.vpx = out.vx;
		GS_VIEWPOINT.vpy = out.vy;
		GS_VIEWPOINT.vpz = out.vz;
	} else {
		VS_CAMERA_STATE = 6;
		GS_VIEWPOINT.vpx = VS_D_800706A8[idx][0];
		GS_VIEWPOINT.vpy = VS_D_800706A8[idx][1];
		GS_VIEWPOINT.vpz = VS_D_800706A8[idx][2];
	}

	GS_VIEWPOINT.rz = 0;
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

	rot = STDVS_VIEW_ROTATION[0];
	rot.vy -= ((Entity *)VS_FOCUSED_ENTITY)->posData->rotation.vy;
	rot.vy &= 0xfff;
	RotMatrix(&rot, &STDVS_VIEW.view);
	v = STDVS_VIEW_TRANSLATION;
	ApplyMatrixLV(&STDVS_VIEW.view,
	              &((Entity *)VS_FOCUSED_ENTITY)->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix(&STDVS_VIEW.view, &v);
	MAIN_D_801B1BBC.flg = 0;
	GsSetView2(&STDVS_VIEW);
}

int32_t VS_getFighterDistance(VECTOR *self, VECTOR *other, VECTOR *target)
{
	int32_t toTarget;
	int32_t toOther;

	toTarget = getDistance(self->vx - target->vx, self->vy - target->vy, self->vz - target->vz);
	toOther = getDistance(other->vx - self->vx, other->vy - self->vy, other->vz - self->vz);
	return (toTarget * 100) / toOther;
}

void VS_updateCameraLerp(int32_t t, int8_t flip)
{
	SVECTOR off;
	SVECTOR rot;
	int32_t base;
	int32_t dist;
	int32_t dbl;

	off = INTRO_CAMERA_STAGE2_POS;
	rot = INTRO_CAMERA_STAGE2_ROT;
	base = ((((DIGIMON_DATA[((Entity *)VS_FOCUSED_ENTITY)->type].height +
	           DIGIMON_DATA[((Entity *)VS_FOCUSED_ENTITY)->type].radius) /
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

	rot.vy += ((Entity *)VS_FOCUSED_ENTITY)->posData->rotation.vy;
	off.vy = (-DIGIMON_DATA[((Entity *)VS_FOCUSED_ENTITY)->type].height * 2) / 3;
	VS_setViewpointFromBone((Entity *)VS_FOCUSED_ENTITY, &off, &rot, dist);
}

int32_t VS_isPositionNearEntity(Entity *entity, VECTOR *pos)
{
	if (pos->vx - 50 > entity->posData->location.vx) {
		goto no;
	}
	if (entity->posData->location.vx > pos->vx + 50) {
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

void VS_tickCameraChase(void)
{
	int32_t t;
	SVECTOR off;
	SVECTOR rot;
	int32_t d2;
	CameraChase *cc;
	int32_t dist;
	int32_t i;

	cc = &INTRO_CAMERA_CHASE;
	if (cc->timer < 0x14) {
		return;
	}
	if (cc->timer < 0x14) {
		goto inc;
	}
	if (cc->timer == 0x14) {
		startAnimation((Entity *)VS_FOCUSED_ENTITY, 0x23);
	}
	if (cc->phase == 0) {
		dist = VS_getFighterDistance(&VS_D_80071754, &VS_D_80071744, &((Entity *)VS_FOCUSED_ENTITY)->posData->location);
		if (dist >= 0x23) {
			VS_D_80071754 = ((Entity *)VS_FOCUSED_ENTITY)->posData->location;
			cc->phase = 1;
			VS_CAMERA_STATE = 8;
		} else {
			off = INTRO_CAMERA_STAGE1_POS;
			rot = INTRO_CAMERA_STAGE1_ROT;
			if (cc->side == 0) {
				rot.vy = lerp(-0x638, -0x293, 0, 0x23, dist);
			} else {
				rot.vy = lerp(0x638, 0x293, 0, 0x23, dist);
			}
			rot.vy += ((Entity *)VS_FOCUSED_ENTITY)->posData->rotation.vy;
			off.vy = (-DIGIMON_DATA[((Entity *)VS_FOCUSED_ENTITY)->type].height * 7) / 10;
			d2 = (((DIGIMON_DATA[((Entity *)VS_FOCUSED_ENTITY)->type].height + DIGIMON_DATA[((Entity *)VS_FOCUSED_ENTITY)->type].radius) / 2) * 0x5aa) / 450;
			VS_setViewpointFromBone((Entity *)VS_FOCUSED_ENTITY, &off, &rot, d2);
			GS_VIEWPOINT.vpy = (off.vy * 7) / 10;
			if (GS_VIEWPOINT.vpy > -0xb4) {
				GS_VIEWPOINT.vpy = -0xb4;
			}
			goto inc;
		}
	}
	t = VS_getFighterDistance(&VS_D_80071754, &VS_D_80071744, &((Entity *)VS_FOCUSED_ENTITY)->posData->location);
	VS_updateCameraLerp(t, cc->side);
	if (VS_isPositionNearEntity((Entity *)VS_FOCUSED_ENTITY, &VS_D_80071744) == 1) {
		for (i = 0; i < 3; i++) {
			if (((uint8_t *)VS_FOCUSED_ENTITY + i)[0x44] != 0xff) {
				startAnimation((Entity *)VS_FOCUSED_ENTITY, ((uint8_t *)VS_FOCUSED_ENTITY + i)[0x44]);
				break;
			}
		}
		((Entity *)VS_FOCUSED_ENTITY)->anim.animFlag |= 2;
		cc->timer = -1;
		return;
	}
inc:
	cc->timer++;
}

// clang-format off
void VS_startCameraChase(entity, dx, side)
	Entity *entity;
	int16_t dx;
	int8_t side;
// clang-format on
{
	SVECTOR off;
	SVECTOR rot;
	int32_t dist;

	VS_FOCUSED_ENTITY = (char **)entity;
	copyVector(&VS_D_80071754, &((Entity *)VS_FOCUSED_ENTITY)->posData->location);
	VS_D_80071744.vx = VS_D_80071754.vx - dx;
	VS_D_80071744.vy = VS_D_80071754.vy;
	VS_D_80071744.vz = VS_D_80071754.vz;
	startAnimation((Entity *)VS_FOCUSED_ENTITY, 0x21);
	VS_CAMERA_STATE = 9;
	INTRO_CAMERA_CHASE.timer = 0;
	INTRO_CAMERA_CHASE.phase = 0;
	INTRO_CAMERA_CHASE.side = side;
	addObject(0x1aa, 0, (TickFunction)VS_tickCameraChase, NULL);
	off = MAIN_D_80134A74;
	rot = MAIN_D_80134A7C;
	if (INTRO_CAMERA_CHASE.side == 0) {
		rot.vy = -0x638;
	} else {
		rot.vy = 0x638;
	}
	rot.vy += ((Entity *)VS_FOCUSED_ENTITY)->posData->rotation.vy;
	off.vy = (-DIGIMON_DATA[((Entity *)VS_FOCUSED_ENTITY)->type].height * 2) / 3;
	dist = (((DIGIMON_DATA[((Entity *)VS_FOCUSED_ENTITY)->type].height + DIGIMON_DATA[((Entity *)VS_FOCUSED_ENTITY)->type].radius) / 2) * 0x5aa) / 450;
	VS_setViewpointFromBone((Entity *)VS_FOCUSED_ENTITY, &off, &rot, dist);
	GS_VIEWPOINT.vpy = (off.vy * 3) / 10;
	if (GS_VIEWPOINT.vpy > -0xb4) {
		GS_VIEWPOINT.vpy = -0xb4;
	}
}

void VS_tickCameraIntro(void)
{
	SVECTOR off;
	SVECTOR rot;
	int16_t *p;

	p = VS_D_80071A0C;
	if ((p[0] >= 0x1e) && (p[0] < 0x3c)) {
		VS_D_80071A0C[2] = lerp(VS_D_80071A0C[2], VS_D_80071A0C[3], p[0], 0x3c, p[0] + 1);
	}

	if (p[0] >= 0x1e) {
		p[1] += (int16_t)VS_interpolateClamped2(0x1e, 0x3c, p[0], 0, 0x5b);
	}

	off = MAIN_D_80134A84;
	rot = MAIN_D_80134A8C;
	rot.vy = VS_D_80071A0C[1];
	VS_D_80071A0C[2] = processSomeArenaArrays(0x16, p[0], VS_D_800706C8, VS_D_80071A30, VS_D_80071A88);
	VS_setViewpointFromBone(*(Entity **)&p[6], &off, &rot, VS_D_80071A0C[2]);
	GS_VIEWPOINT.vry = (-DIGIMON_DATA[VS_D_80071A18[0]->type].height * 2) / 3;
	GS_VIEWPOINT.vpy = -processSomeArenaArrays(0x16, p[0], VS_D_800706C8, VS_D_80071AE0, VS_D_80071A88);
	p[0]++;
}

void VS_startCameraIntro(Entity *target, Entity *entity)
{
	SVECTOR off;
	SVECTOR rot;
	SVECTOR delta;
	int32_t d;
	int32_t i;

	VS_D_80071A0C[0] = 0;
	VS_D_80071A18[1] = entity;
	if (target == NULL) {
		if ((Entity *)VS_FOCUSED_ENTITY == ENTITY_TABLE[1]) {
			target = ENTITY_TABLE[2];
		} else {
			target = ENTITY_TABLE[1];
		}
	}
	VS_D_80071A18[0] = target;
	VS_CAMERA_STATE = 8;
	addObject(0x1ad, 0, (TickFunction)VS_tickCameraIntro, NULL);

	off = MAIN_D_80134A94;
	rot = MAIN_D_80134A9C;
	delta.vx = entity->posData->location.vx - target->posData->location.vx;
	delta.vz = entity->posData->location.vz - target->posData->location.vz;
	VS_D_80071A0C[1] = (-_atan(delta.vz, delta.vx) + 0x7de) & 0xfff;
	rot.vy = VS_D_80071A0C[1];
	VS_D_80071A0C[2] = getDistance(delta.vx, 0, delta.vz);
	VS_D_80071A0C[2] += 0x2bc;
	if (VS_D_80071A0C[2] < 0x5dc) {
		VS_D_80071A0C[2] = 0x5dc;
	}
	VS_setViewpointFromBone(target, &off, &rot, VS_D_80071A0C[2]);

	VS_D_80071A0C[3] = DIGIMON_DATA[target->type].radius * 3 * 2;
	GS_VIEWPOINT.vry = (-DIGIMON_DATA[target->type].height * 2) / 3;
	GS_VIEWPOINT.vpy = -DIGIMON_DATA[entity->type].height;
	VS_D_80071A0C[4] = GS_VIEWPOINT.vpy;
	VS_D_80071A0C[5] = GS_VIEWPOINT.vpy * 250 / 100;
	VS_D_80071AE0[0] = DIGIMON_DATA[entity->type].height;
	VS_D_80071AE0[1] = DIGIMON_DATA[entity->type].height;
	VS_D_80071AE0[2] = DIGIMON_DATA[entity->type].height * 200 / 100;
	VS_D_80071AE0[3] = DIGIMON_DATA[target->type].height * 380 / 100;
	VS_D_80071AF0[0] = DIGIMON_DATA[target->type].height * 250 / 100;
	for (i = 5; i < 0x16; i++) {
		VS_D_80071AE0[i] =
			customRandom(0x50, DIGIMON_DATA[target->type].height * 180 / 100);
	}
	initializeSomeArenaArrays(0x16, (int32_t)VS_D_800706C8, (int32_t)VS_D_80071AE0, VS_D_80071A88);

	d = getDistance(delta.vx, 0, delta.vz) + 0x2bc;
	VS_D_80071A30[0] = d;
	VS_D_80071A30[1] = d;
	if (d < 0x5dc) {
		d = 0x5dc;
	}
	VS_D_80071A30[2] = d * 120 / 100;
	VS_D_80071A30[3] = d * 80 / 100;
	VS_D_80071A40[0] = VS_D_80071A0C[3];
	for (i = 5; i < 0x16; i++) {
		VS_D_80071A30[i] =
			customRandom(VS_D_80071A0C[3] * 45 * 2 / 100,
		                     VS_D_80071A0C[3] * 45 * 4 / 100);
	}
}

void VS_removeCameraIntro(void)
{
	removeObject(0x1ad, 0);
	VS_D_80071A0C[0] = -1;
}

void VS_applyEntityViewpoint(void)
{
	char *p;

	VIEWPORT_DISTANCE = 0x15e;
	GsSetProjection(VIEWPORT_DISTANCE);
	p = VS_FOCUSED_ENTITY[1] + 0x34;
	GS_VIEWPOINT.vrx = *(int32_t *)(p + 0x14);
	GS_VIEWPOINT.vry = -DIGIMON_DATA[(int32_t)VS_FOCUSED_ENTITY[0]].height * 2 / 3;
	GS_VIEWPOINT.vrz = *(int32_t *)(p + 0x1c);
	GsSetRefView2(&GS_VIEWPOINT);
}

// clang-format off
void VS_renderCounterDigits(x, y, digits, value, layer)
	int16_t x;
	int16_t y;
	int16_t digits;
	int32_t value;
	int32_t layer;
// clang-format on
{
	POLY_FT4 *prim;
	int32_t i;
	int32_t count;
	int32_t buf[6];

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = getTPage(0, 0, 832, 0);
	prim->clut = GetClut(0x10, 0x1e0);
	setUVDataPolyFT4(prim, 0x78, 0x30, 8, 0x12);
	setPosDataPolyFT4(prim, -0x1c, -0x63, 8, 0x12);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = getTPage(0, 0, 832, 0);
	prim->clut = GetClut(0x10, 0x1e0);
	setUVDataPolyFT4(prim, 0x80, 0x30, 8, 0x12);
	setPosDataPolyFT4(prim, 0x14, -0x63, 8, 0x12);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 832, 0);
		prim->clut = GetClut(0x10, 0x1e0);
		setUVDataPolyFT4(prim, buf[i] * 12, 0x30, 0xc, 0xf);
		setPosDataPolyFT4(prim, x + ((digits - 1) - i) * 14, y, 0xc, 0xf);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = GetTPage(1, 0, 0x2c0, 0);
	prim->clut = GetClut(0x200, 0xff);
	setUVDataPolyFT4(prim, 0, 0x78, 0x42, 0x1d);
	setPosDataPolyFT4(prim, -0x21, -0x68, 0x42, 0x1d);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void VS_addFighterCounter(int32_t arg)
{
	if ((VS_IS_TIMER_INITIALIZED == 0) && (arg != 0)) {
		VS_TIMER = arg;
		addObject(0x1ac, 0, (TickFunction)VS_tickFighterCounter, (RenderFunction)VS_renderFighterCounter);
		VS_IS_TIMER_INITIALIZED = 1;
	}
}

void VS_tickFighterCounter(void)
{
	if (VS_TIMER_ACTIVE == 1) {
		BATTLE_FRAME_COUNT++;
		if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]]->anim.animId != 0x2b) {
			if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->anim.animId != 0x2b) {
				if (BATTLE_FRAME_COUNT % 0x14 == 0) {
					if (VS_TIMER != 0) {
						VS_TIMER--;
					}
				}
			}
		}
	}
}

void VS_renderFighterCounter(void)
{
#if defined(VERSION_JP)
	VS_renderCounterDigits(-0xd, -0x61, 2, VS_TIMER, 0);
#else
	VS_renderCounterDigits(-0xd, -0x61, 2, VS_TIMER, 3);
#endif
}

void VS_removeFighterCounter(void)
{
	if (VS_IS_TIMER_INITIALIZED != 0) {
		removeObject(0x1ac, 0);
		VS_IS_TIMER_INITIALIZED = 0;
	}
}
