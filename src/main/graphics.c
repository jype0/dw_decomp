#include <inline_n.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/attack_object.h>
#include <dw/types.h>

extern _GsFCALL GS_TMD_MAP;
extern int32_t MAIN_D_80137BE8[];
extern int32_t MAIN_D_80137BE4[];
extern AttackObject ATTACK_OBJECTS[];

PACKET *GsTMDfastF3L();
PACKET *GsTMDfastG3L();
PACKET *GsTMDfastF4L();
PACKET *GsTMDfastF4NL();
PACKET *GsTMDfastG4L();
PACKET *GsTMDfastNF4();
PACKET *GsTMDfastTF3L();
PACKET *GsTMDfastTF3NL();
PACKET *GsTMDfastTNF3();
PACKET *GsTMDfastTF4L();
PACKET *GsTMDfastTF4NL();
PACKET *GsTMDfastTNF4();
PACKET *GsTMDfastTG3L();
PACKET *GsTMDfastTG3NL();
PACKET *GsTMDfastTNG3();
PACKET *GsTMDfastTG4L();
PACKET *GsTMDfastTG4NL();
PACKET *GsTMDfastTNG4();
PACKET *GsTMDdivTF3NL();
PACKET *GsTMDdivTNF3();
PACKET *GsTMDdivTG3NL();
PACKET *GsTMDdivTNG3();
PACKET *GsTMDdivTF4L();
PACKET *GsTMDdivTF4NL();
PACKET *GsTMDdivTNF4();
PACKET *GsTMDdivTG4NL();
PACKET *GsTMDdivTNG4();
void setRotTransMatrix(MATRIX *m);
void initializeGsTMDMap(void);
void updateTMDTextureData(int32_t *tmd, int32_t clutX, int32_t x, int32_t y, int32_t tpage);

// clang-format off
AttackObject RESET_ATTACK_OBJECT = {
	0xffffffff,
	0xffffffff,
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	0x00000000,
	0x00000000,
	0x00000000,
};
// clang-format on

void setRotTransMatrix(MATRIX *m)
{
	gte_SetRotMatrix(m);
	gte_SetTransMatrix(m);
}

void initializeGsTMDMap(void)
{
	GS_TMD_MAP.f3[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastF3L;
	GS_TMD_MAP.tf3[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastTF3L;
	GS_TMD_MAP.tf3[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastTF3NL;
	GS_TMD_MAP.ntf3[GsDivMODE_NDIV]			= GsTMDfastTNF3;
	GS_TMD_MAP.g3[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastG3L;
	GS_TMD_MAP.tg3[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastTG3L;
	GS_TMD_MAP.tg3[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastTG3NL;
	GS_TMD_MAP.ntg3[GsDivMODE_NDIV]			= GsTMDfastTNG3;
	GS_TMD_MAP.f4[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastF4L;
	GS_TMD_MAP.tf4[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastTF4L;
	GS_TMD_MAP.tf4[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastTF4NL;
	GS_TMD_MAP.ntf4[GsDivMODE_NDIV]			= GsTMDfastTNF4;
	GS_TMD_MAP.g4[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastG4L;
	GS_TMD_MAP.tg4[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastTG4L;
	GS_TMD_MAP.tg4[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastTG4NL;
	GS_TMD_MAP.ntg4[GsDivMODE_NDIV]			= GsTMDfastTNG4;
	GS_TMD_MAP.tf4[GsDivMODE_DIV][GsLMODE_NORMAL]	= GsTMDdivTF4L;
	GS_TMD_MAP.tf4[GsDivMODE_DIV][GsLMODE_LOFF]	= GsTMDdivTF4NL;
	GS_TMD_MAP.tf3[GsDivMODE_DIV][GsLMODE_LOFF]	= GsTMDdivTF3NL;
	GS_TMD_MAP.tg4[GsDivMODE_DIV][GsLMODE_LOFF]	= GsTMDdivTG4NL;
	GS_TMD_MAP.tg3[GsDivMODE_DIV][GsLMODE_LOFF]	= GsTMDdivTG3NL;
	GS_TMD_MAP.ntg4[GsDivMODE_DIV]			= GsTMDdivTNG4;
	GS_TMD_MAP.ntg3[GsDivMODE_DIV]			= GsTMDdivTNG3;
	GS_TMD_MAP.ntf4[GsDivMODE_DIV]			= GsTMDdivTNF4;
	GS_TMD_MAP.ntf3[GsDivMODE_DIV]			= GsTMDdivTNF3;
	GS_TMD_MAP.f4[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastF4NL;
	GS_TMD_MAP.nf4[GsDivMODE_NDIV]			= GsTMDfastNF4;
}

void initializeAttackObjects(void)
{
	int32_t i;

	for (i = 0; i < 0x10; i++) {
		ATTACK_OBJECTS[i] = RESET_ATTACK_OBJECT;
	}
}

int32_t addAttackObject(int32_t victimId, int32_t active, SVECTOR *pos, int32_t effectId, int32_t subEffectIndex, int32_t casterId)
{
	int32_t i;

	MAIN_D_80137BE8[0] = -1;
	for (i = 0;; i++) {
		if (ATTACK_OBJECTS[i].active == -1) {
			break;
		}
	}
	if (i == 0x10) {
		return 0;
	}
	ATTACK_OBJECTS[i].victimId = victimId;
	ATTACK_OBJECTS[i].active = active;
	ATTACK_OBJECTS[i].position = *pos;
	ATTACK_OBJECTS[i].effectId = effectId;
	ATTACK_OBJECTS[i].subEffectIndex = subEffectIndex;
	ATTACK_OBJECTS[i].casterId = casterId;
	return 1;
}

int32_t popAttackObject(int32_t entityId, AttackObject *out)
{
	int32_t i;
	int32_t k;
	int32_t m;

	MAIN_D_80137BE4[0] = entityId;
	for (i = 0;; i++) {
		if (ATTACK_OBJECTS[i].victimId == entityId) {
			break;
		}
	}
	if (i == 0x10) {
		return 0;
	}
	*out = ATTACK_OBJECTS[i];
	ATTACK_OBJECTS[i] = RESET_ATTACK_OBJECT;
	for (k = 0; k < 0xF; k++) {
		if (ATTACK_OBJECTS[k].active == -1) {
			for (m = k + 1; m < 0x10; m++) {
				if (ATTACK_OBJECTS[m].active != -1) {
					break;
				}
			}
			if (m == 0x10) {
				break;
			}
			ATTACK_OBJECTS[k] = ATTACK_OBJECTS[m];
			ATTACK_OBJECTS[m] = RESET_ATTACK_OBJECT;
		}
	}
	return 1;
}

void updateTMDTextureData(int32_t *tmd, int32_t clutX, int32_t x, int32_t y, int32_t tpage)
{
	uint32_t *prim;
	struct TMD_STRUCT *obj;
	int32_t i;
	int32_t j;
	int32_t nObj;
	int32_t primn;
	int32_t clut;
	int32_t tpages;
	int32_t clutXs;
	uint8_t mode;
	uint32_t len;

	tmd = (int32_t *)((int32_t)tmd + 8);
	nObj = *tmd++;
	obj = (struct TMD_STRUCT *)tmd;
	clut = (y << 8) + x;
	tpages = tpage << 16;
	clutXs = clutX << 16;
	for (i = 0; i < nObj; obj++, i++) {
		primn = obj->primn;
		prim = (uint32_t *)obj->primtop;
		for (j = 0; j < primn; j++) {
			mode = prim[0] >> 24;
			if ((mode & 4) == 0) {
				break;
			}
			len = ((prim[0] & 0xFF00) >> 8) + 1;
			prim[1] += tpages + clut;
			prim[2] = clut + ((prim[2] & 0xFFE0FFFF) + clutXs);
			prim[3] += clut;
			if (mode & 8) {
				prim[4] += clut;
			}
			prim += len;
		}
	}
}
