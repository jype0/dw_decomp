#include <libgs.h>
#include <libgte.h>

#include <dw/battle.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/types.h>
#include <dw/vs.h>
#include <dw/world_object.h>

extern GsVIEW2 MAIN_D_801B1B98;
extern int32_t MAIN_D_801B1BBC[];
extern SVECTOR MAIN_D_801B1C0C[];
extern VECTOR MAIN_D_801B1C14;
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t VIEWPORT_DISTANCE;
extern int16_t MAIN_D_80134D66;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern MATRIX MAIN_D_801B1BC0;
extern int32_t MAIN_D_80135268;
extern int32_t MAIN_D_80135284;
extern uint8_t MAIN_D_80135288;
extern int32_t MAIN_D_8013528C;
extern int16_t MAIN_D_80135294;
extern char **MAIN_D_80135298;
extern int8_t MAIN_D_8013529C;
extern CameraChase MAIN_D_801352A4;
extern int32_t MAIN_D_801352A8;

void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
int32_t processSomeArenaArrays(int32_t count, int32_t t, int32_t *keys, int32_t *values, int32_t *slopes);
int32_t lerp(int32_t a, int32_t b, int32_t lo, int32_t hi, int32_t t);
int32_t getDistance(int32_t x, int32_t y, int32_t z);
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
void VS_updateCameraLerp(int32_t t, int32_t flip);
int32_t VS_isPositionNearEntity(Entity *entity, VECTOR *pos);
int32_t VS_interpolateClamped2(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end);
void VS_tickCameraChase(void);
void startAnimation(Entity *entity, int32_t animId);
void VS_tickCameraIntro(void);
void VS_startCameraIntro(Entity *target, Entity *entity);
void VS_addResultModelScene(Entity *entity);
void VS_removeCameraIntro(void);
void VS_applyEntityViewpoint(void);
void VS_renderCounterDigits(int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer);
void VS_tickFighterCounter(void);
void VS_renderFighterCounter(void);
int32_t customRandom(int32_t lo, int32_t hi);
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

SVECTOR MAIN_D_80134A64 = { 0 };
SVECTOR MAIN_D_80134A6C = { 0, -1592, 0, 0 };
SVECTOR MAIN_D_80134A74 = { 0 };
SVECTOR MAIN_D_80134A7C = { 0, -1592, 0, 0 };
SVECTOR MAIN_D_80134A84 = { 0 };
SVECTOR MAIN_D_80134A8C = { 0 };
SVECTOR MAIN_D_80134A94 = { 0 };
SVECTOR MAIN_D_80134A9C = { 0 };
SVECTOR MAIN_D_80134AA4 = { 0 };
SVECTOR MAIN_D_80134AAC = { -227, 1479, 0, 0 };
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

int16_t VS_D_800706A8[16] = {
	0xfe00, 0xfe00, 0xfb50, 0x0200, 0xfe00, 0xfb50, 0xfe00, 0xfe00,
	0x04b0, 0x0200, 0xfe00, 0x04b0, 0x0bb8, 0xfa24, 0x0bb8, 0x0000,
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
	MAIN_D_801B1B98.super = NULL;
	RotMatrix(MAIN_D_801B1C0C, &MAIN_D_801B1B98.view);
	TransMatrix(&MAIN_D_801B1B98.view, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void VS_setCameraOrbit(void)
{
	VECTOR *a;
	VECTOR *b;
	int32_t dx;
	int32_t dz;
	int32_t dist;
	int32_t d;
	int32_t ang;
	MATRIX *m;
	int32_t limA;
	int32_t limB;
	int32_t limC;

	a = &ENTITY_TABLE[2]->posData->location;
	b = &ENTITY_TABLE[1]->posData->location;
	dx = a->vx - b->vx;
	dz = a->vz - b->vz;
	dist = SquareRoot0(dx * dx + dz * dz);
	d = (dist * VIEWPORT_DISTANCE) / 200u;
	limA = MAIN_D_80135294;
	limB = limB = limA;
	limC = limC = limA;
	if (d < limA) {
		d = limB;
	}
	if (d < limC + 0x12c) {
		MAIN_D_80135284 = 1;
	} else {
		MAIN_D_80135284 = 0;
	}
	if (d > 0x1068) {
		d = 0x1068;
	}
	ang = 0x1000 - _atan(dx, dz);
	MAIN_D_801B1C14.vx = b->vx + dx / 2 + (d * dz) / dist;
	MAIN_D_801B1C14.vy = -0x3e8;
	MAIN_D_801B1C14.vz = b->vz + dz / 2 - (d * dx) / dist;
	MAIN_D_801B1C0C[0].vx = _atan(d, -0x2bc) + 0x800;
	MAIN_D_801B1C0C[0].vy = ang + 0x800;
	MAIN_D_801B1C0C[0].vz = 0;
	MAIN_D_801B1B98.view = GsIDMATRIX;
	MAIN_D_801B1B98.super = (GsCOORDINATE2 *)MAIN_D_801B1BBC;
	RotMatrixYXZ(MAIN_D_801B1C0C, m = m = &MAIN_D_801B1BC0);
	TransMatrix(m, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void VS_setCameraYXZ(void)
{
	VECTOR *a;
	VECTOR *b;
	MATRIX *m;

	a = &ENTITY_TABLE[1]->posData->location;
	b = &ENTITY_TABLE[2]->posData->location;
	MAIN_D_801B1B98.view = GsIDMATRIX;
	MAIN_D_801B1B98.super = (GsCOORDINATE2 *)MAIN_D_801B1BBC;
	MAIN_D_801B1C14.vx = a->vx + (b->vx - a->vx) / 2;
	MAIN_D_801B1C14.vz = a->vz + (b->vz - a->vz) / 2;
	MAIN_D_801B1C14.vy = -0x1f40;
	MAIN_D_801B1C0C[0].vz = 0;
	MAIN_D_801B1C0C[0].vy = 0;
	MAIN_D_801B1C0C[0].vx = -0x400;
	RotMatrixYXZ(MAIN_D_801B1C0C, m = m = &MAIN_D_801B1BC0);
	TransMatrix(m, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void VS_setViewpointRotationFromEntity(void)
{
	MATRIX *m;

	m = (MATRIX *)(MAIN_D_80135298[1] + 0xbc);
	GS_VIEWPOINT.vrx = m->t[0];
	GS_VIEWPOINT.vry = m->t[1];
	GS_VIEWPOINT.vrz = m->t[2];
	GsSetRefView2(&GS_VIEWPOINT);
}

void VS_setCameraLookAtEntity(void)
{
	VECTOR v;
	VECTOR out;
	Entity *self;
	Entity *other;
	VECTOR *selfPos;
	VECTOR *otherPos;
	int32_t dx;
	int32_t dz;

	self = (Entity *)MAIN_D_80135298;
	selfPos = &self->posData->location;
	if (self == ENTITY_TABLE[1]) {
		other = ENTITY_TABLE[2];
	} else {
		other = ENTITY_TABLE[1];
	}
	otherPos = &other->posData->location;
	dx = otherPos->vx - selfPos->vx;
	do {
	} while (0);
	dz = otherPos->vz - selfPos->vz;
	MAIN_D_801B1C0C[0].vy = (-_atan(dz, dx) + 0x800) & 0xfff;
	RotMatrix(MAIN_D_801B1C0C, &MAIN_D_801B1B98.view);
	v = MAIN_D_801B1C14;
	ApplyMatrixLV(&MAIN_D_801B1B98.view,
	              &((Entity *)MAIN_D_80135298)->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix(&MAIN_D_801B1B98.view, &v);
	GsSetView2(&MAIN_D_801B1B98);
}

void VS_applyViewpoint(void)
{
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void VS_setCameraSimple(void)
{
	MAIN_D_801B1C0C[0].vy += 2;
	MAIN_D_801B1C0C[0].vy &= 0xfff;
	MAIN_D_801B1B98.super = NULL;
	RotMatrix(MAIN_D_801B1C0C, &MAIN_D_801B1B98.view);
	TransMatrix(&MAIN_D_801B1B98.view, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void VS_setCameraParams(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f)
{
	MAIN_D_801B1C0C[0].vx = a;
	MAIN_D_801B1C0C[0].vy = b;
	MAIN_D_801B1C0C[0].vz = c;
	MAIN_D_801B1C14.vx = d;
	MAIN_D_801B1C14.vy = e;
	MAIN_D_801B1C14.vz = f;
}

void VS_setVSPhase(int32_t arg)
{
	addObject(0x1a8, 0, (TickFunction)VS_tickVSPhase, NULL);
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

void VS_selectRandomCamera(DigimonEntity *entity, int32_t mode, int32_t sub)
{
	CameraPreset *p;

	if (mode != 5) {
		if (randomLimit(3) != 0) {
			return;
		}
	}
	MAIN_D_801B1B98.super = NULL;
	MAIN_D_80135298 = (char **)entity;
	if (sub != 3) {
		p = &VS_D_8007063C[mode];
	} else {
		p = &VS_D_8007063C[randomLimit(3) + 6];
	}
	VS_setCameraParams(p->unk0, p->unk2, p->unk4, p->unk6, p->unk8, p->unkA);
	if (mode < 5) {
		MAIN_D_80135268 = 3;
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
		v.vy = (&VS_D_800706A8[1])[idx * 3];
		v.vz = (&VS_D_800706A8[2])[idx * 3];
		ApplyMatrixLV(&m, &v, &out);
		out.vx += ((Entity *)MAIN_D_80135298)->posData->location.vx;
		out.vz += ((Entity *)MAIN_D_80135298)->posData->location.vz;
		GS_VIEWPOINT.vpx = out.vx;
		GS_VIEWPOINT.vpy = out.vy;
		GS_VIEWPOINT.vpz = out.vz;
	} else {
		MAIN_D_80135268 = 6;
		GS_VIEWPOINT.vpx = VS_D_800706A8[idx * 3];
		GS_VIEWPOINT.vpy = (&VS_D_800706A8[1])[idx * 3];
		GS_VIEWPOINT.vpz = (&VS_D_800706A8[2])[idx * 3];
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

	rot = MAIN_D_801B1C0C[0];
	rot.vy -= ((Entity *)MAIN_D_80135298)->posData->rotation.vy;
	rot.vy &= 0xfff;
	RotMatrix(&rot, &MAIN_D_801B1B98.view);
	v = MAIN_D_801B1C14;
	ApplyMatrixLV(&MAIN_D_801B1B98.view,
	              &((Entity *)MAIN_D_80135298)->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix(&MAIN_D_801B1B98.view, &v);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
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

void VS_tickCameraChase(void)
{
	SVECTOR off;
	SVECTOR rot;
	CameraChase *cc;
	int32_t dist;
	int32_t d2;
	int32_t slot;
	int32_t i;

	cc = &MAIN_D_801352A4;
	if (MAIN_D_801352A4.timer < 0x14) {
		return;
	}
	if (cc->timer < 0x14) {
		goto inc;
	}
	if (cc->timer == 0x14) {
		startAnimation((Entity *)MAIN_D_80135298, 0x23);
	}
	if (cc->phase == 0) {
		dist = VS_getFighterDistance(&VS_D_80071754, &VS_D_80071744, &((Entity *)MAIN_D_80135298)->posData->location);
		if (dist >= 0x23) {
			VS_D_80071754 = ((Entity *)MAIN_D_80135298)->posData->location;
			cc->phase = 1;
			MAIN_D_80135268 = 8;
		} else {
			off = MAIN_D_80134A64;
			rot = MAIN_D_80134A6C;
			if (cc->side == 0) {
				rot.vy = lerp(-0x638, -0x293, 0, 0x23, dist);
			} else {
				rot.vy = lerp(0x638, 0x293, 0, 0x23, dist);
			}
			rot.vy += ((Entity *)MAIN_D_80135298)->posData->rotation.vy;
			off.vy = (-DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].height * 7) / 10;
			d2 = (((DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].height + DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].radius) / 2) * 0x5aa) / 450;
			VS_setViewpointFromBone((Entity *)MAIN_D_80135298, &off, &rot, d2);
			GS_VIEWPOINT.vpy = (off.vy * 7) / 10;
			if (GS_VIEWPOINT.vpy > -0xb4) {
				GS_VIEWPOINT.vpy = -0xb4;
			}
			goto inc;
		}
	}
	dist = VS_getFighterDistance(&VS_D_80071754, &VS_D_80071744, &((Entity *)MAIN_D_80135298)->posData->location);
	VS_updateCameraLerp(dist, cc->side);
	if (VS_isPositionNearEntity((Entity *)MAIN_D_80135298, &VS_D_80071744) == 1) {
		for (i = 0; i < 3; i++) {
			if (((uint8_t *)MAIN_D_80135298 + i)[0x44] != 0xff) {
				startAnimation((Entity *)MAIN_D_80135298, ((uint8_t *)MAIN_D_80135298 + i)[0x44]);
				break;
			}
		}
		((Entity *)MAIN_D_80135298)->anim.animFlag |= 2;
		cc->timer = -1;
		return;
	}
inc:
	cc->timer++;
}

void VS_startCameraChase(Entity *entity, int32_t dx, int32_t side)
{
	SVECTOR off;
	SVECTOR rot;
	int32_t dist;

	MAIN_D_80135298 = (char **)entity;
	copyVector(&VS_D_80071754, &entity->posData->location);
	VS_D_80071744.vx = VS_D_80071754.vx - dx;
	VS_D_80071744.vy = VS_D_80071754.vy;
	VS_D_80071744.vz = VS_D_80071754.vz;
	startAnimation(entity, 0x21);
	MAIN_D_80135268 = 9;
	MAIN_D_801352A4.timer = 0;
	MAIN_D_801352A4.phase = 0;
	MAIN_D_801352A4.side = side;
	addObject(0x1aa, 0, (TickFunction)VS_tickCameraChase, NULL);
	off = MAIN_D_80134A74;
	rot = MAIN_D_80134A7C;
	if (MAIN_D_801352A4.side == 0) {
		rot.vy = -0x638;
	} else {
		rot.vy = 0x638;
	}
	rot.vy += ((Entity *)MAIN_D_80135298)->posData->rotation.vy;
	off.vy = (-DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].height * 2) / 3;
	dist = (((DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].height + DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].radius) / 2) * 0x5aa) / 450;
	VS_setViewpointFromBone((Entity *)MAIN_D_80135298, &off, &rot, dist);
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
	int16_t dx;
	int16_t dz;
	int32_t d;
	int32_t i;

	VS_D_80071A0C[0] = 0;
	VS_D_80071A18[1] = entity;
	if (target == NULL) {
		if ((Entity *)MAIN_D_80135298 == ENTITY_TABLE[1]) {
			target = ENTITY_TABLE[2];
		} else {
			target = ENTITY_TABLE[1];
		}
	}
	VS_D_80071A18[0] = target;
	MAIN_D_80135268 = 8;
	addObject(0x1ad, 0, (TickFunction)VS_tickCameraIntro, NULL);

	off = MAIN_D_80134A94;
	rot = MAIN_D_80134A9C;
	dx = entity->posData->location.vx - target->posData->location.vx;
	dz = entity->posData->location.vz - target->posData->location.vz;
	VS_D_80071A0C[1] = (-_atan(dz, dx) + 0x7de) & 0xfff;
	rot.vy = VS_D_80071A0C[1];
	VS_D_80071A0C[2] = getDistance(dx, 0, dz);
	VS_D_80071A0C[2] = VS_D_80071A0C[2] + 0x2bc;
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

	d = getDistance(dx, 0, dz) + 0x2bc;
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
	GsSetProjection(0x15e);
	p = MAIN_D_80135298[1] + 0x34;
	GS_VIEWPOINT.vrx = *(int32_t *)(p + 0x14);
	GS_VIEWPOINT.vry = -DIGIMON_DATA[(int32_t)MAIN_D_80135298[0]].height * 2 / 3;
	GS_VIEWPOINT.vrz = *(int32_t *)(p + 0x1c);
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
	width = digits;
	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 832, 0);
		prim->clut = GetClut(0x10, 0x1e0);
		setUVDataPolyFT4(prim, buf[i] * 12, 0x30, 0xc, 0xf);
		setPosDataPolyFT4(prim, x + ((((int32_t)width - 1) - i) * 14), y, 0xc, 0xf);
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

void VS_addFighterCounter(uint8_t arg)
{
	if ((MAIN_D_801352A8 == 0) && (arg != 0)) {
		MAIN_D_80135288 = arg;
		addObject(0x1ac, 0, (TickFunction)VS_tickFighterCounter, (RenderFunction)VS_renderFighterCounter);
		MAIN_D_801352A8 = 1;
	}
}

void VS_tickFighterCounter(void)
{
	if (MAIN_D_8013528C == 1) {
		MAIN_D_80134D66++;
		if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]]->anim.animId != 0x2b) {
			if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->anim.animId != 0x2b) {
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
