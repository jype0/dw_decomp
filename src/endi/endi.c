#include <stdlib.h>

#include <libgpu.h>

#include <dw/endi.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/main.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/world_object.h>

#define NUM_ENDI_PARTICLES	80

typedef struct {
	int32_t frame;
	Entity *entity;
	int16_t phase;
	int16_t velocity;
} EndingState;

typedef struct {
	int32_t flag;
	SVECTOR base;
	EndingState state;
	int32_t pad;
} EndiData;

typedef struct {
	int16_t active;
	int16_t boneIndex;
	Entity *entity;
	SVECTOR pos;
	SVECTOR worldPos;
} EndiParticle;


int32_t lerp(int32_t a, int32_t b, int32_t c, int32_t d, int32_t t);
void renderFXParticle(SVECTOR *p, int32_t a, RGB8 *c);
void calculateBoneMatrix(Entity *entity, int32_t b, MATRIX *m);

static void ENDI_downloadClut(u_long *clut);
static void ENDI_setClutStp(u_long *clut);
static void ENDI_setModelSemiTrans(Entity *entity, int32_t arg1);
static void ENDI_updateEnding(int32_t objectId);
static void ENDI_renderEndingObject(int32_t objectId);
static void ENDI_startParticles(void);
static void ENDI_fadeClut(u_long *srcClut, Entity *entity, u_long *dstClut,
		   int32_t startFrame, int32_t endFrame, int32_t frame);
static int32_t ENDI_spawnParticle(Entity *entity, int16_t boneIndex);
static void ENDI_releaseParticles(int32_t index);
static void ENDI_getParticleBase(int32_t arg0, SVECTOR *dst);
static void ENDI_setParticleBase(int32_t arg0, SVECTOR *src);
static void ENDI_stopParticles(void);
static void ENDI_clearParticles(void);
static void ENDI_renderParticles(int32_t objectId);
static void ENDI_tickEndingParticles(int32_t objectId);

u_long *ENDI_FADE_CLUT_BUFFER = (u_long *)GENERAL_BUFFER;
u_long *ENDI_CLUT_BUFFER = (u_long *)(GENERAL_BUFFER + 0x304);
RGB8 ENDI_PARTICLE_COLOR = { 0x80, 0x80, 0x80 };

static EndiParticle ENDI_PARTICLES[NUM_ENDI_PARTICLES] = {
	{
		0x0, 0x0, NULL,
		{ 0x0, 0x0, 0x0, 0x0 },
		{ 0x0, 0x0, 0x0, 0x0 },
	},
	{
		0x0, 0x0, NULL,
		{ 0x0, 0x0, 0x0, 0x0 },
		{ 0x0, 0x0, 0x0, 0x0 },
	},
	{
		0x0, 0x0, NULL,
		{ 0x0, 0x0, 0x0, 0x0 },
		{ 0x0, 0x0, 0x0, 0x0 },
	},
	{
		0x0, 0x0, NULL,
		{ 0x0, 0x0, 0x0, 0x0 },
		{ 0x0, 0x0, 0x0, 0x0 },
	},
	{
		0x0, 0x0, NULL,
		{ 0xff, 0x0, 0x0, 0x6a82 },
		{ 0x15, 0x0, 0xea8a, 0x4c0 },
	},
	{
		0x0, 0x9638, (Entity *)0xff1497ba,
		{ 0x0, 0x0, 0x8200, 0x156b },
		{ 0x0, 0x8000, 0xc0db, 0x4 },
	},
	{
		0x3800, 0xeb98, (Entity *)0xffb79d,
		{ 0x0, 0x0, 0x6c82, 0x15 },
		{ 0x0, 0x6b87, 0x4c0, 0x0 },
	},
	{
		0x9e38, 0x9e44, (Entity *)0xff78,
		{ 0x0, 0x8200, 0x156d, 0x0 },
		{ 0x8500, 0xc0d1, 0x4, 0x3800 },
	},
	{
		0x99f, 0x3c9f, (Entity *)0xff,
		{ 0x0, 0x6e82, 0x15, 0x0 },
		{ 0x8784, 0x4c0, 0x0, 0x8a39 },
	},
	{
		0x8c2c, 0xff2e, 0,
		{ 0x8200, 0x156f, 0x0, 0x8800 },
		{ 0xc00d, 0x4, 0x3900, 0xc98c },
	},
	{
		0x1e92, 0xff, 0,
		{ 0x7082, 0x15, 0x0, 0xd089 },
		{ 0x4c0, 0x0, 0x9239, 0x9fc1 },
	},
	{
		0xff9f, 0x0, (Entity *)0x82000000,
		{ 0x1571, 0x0, 0x8100, 0xc049 },
		{ 0x4, 0x3900, 0x32a0, 0xfba3 },
	},
	{
		0xff, 0x0, (Entity *)0x72820000,
		{ 0x15, 0x0, 0xbc87, 0x4c0 },
		{ 0x0, 0xa839, 0xa9dd, 0xffeb },
	},
	{
		0x0, 0x0, (Entity *)0x15738200,
		{ 0x0, 0x8a00, 0xc049, 0x4 },
		{ 0x3900, 0x74aa, 0x50b4, 0xff },
	},
	{
		0x0, 0x0, (Entity *)0x157482,
		{ 0x0, 0x9589, 0x4c0, 0x0 },
		{ 0xb439, 0xbae9, 0xff9b, 0x0 },
	},
	{
		0x0, 0x8200, (Entity *)0x1875,
		{ 0x8900, 0xc07a, 0x4, 0x3900 },
		{ 0x2dbb, 0xc0, 0x4600, 0xffe7 },
	},
	{
		0x0, 0x0, (Entity *)0x1b768200,
		{ 0x0, 0x8a00, 0xc041, 0x4 },
		{ 0x3900, 0xc0, 0x4700, 0xc07d },
	},
	{
		0x0, 0xa649, (Entity *)0xff,
		{ 0x0, 0x7782, 0x1b, 0x0 },
		{ 0x7a87, 0x4c0, 0x0, 0xc039 },
	},
	{
		0x0, 0x314a, (Entity *)0x520000c0,
		{ 0xff7c, 0x0, 0x0, 0x8200 },
		{ 0x1b78, 0x0, 0x8700, 0xc0a6 },
	},
	{
		0x4, 0x3900, (Entity *)0x530000c0,
		{ 0xc006, 0x0, 0x7857, 0xff },
		{ 0x0, 0x0, 0x7982, 0x1b },
	},
	{
		0x0, 0xfb83, (Entity *)0x4c0,
		{ 0xc039, 0x0, 0x958, 0xc0 },
		{ 0x5d00, 0xff0c, 0x0, 0x0 },
	},
	{
		0x8200, 0x1b7a, (Entity *)0x8a000000,
		{ 0xc0bb, 0x4, 0x3900, 0xc0 },
		{ 0x5d00, 0xc097, 0x0, 0x63 },
	},
	{
		0xff, 0x0, (Entity *)0x7b820000,
		{ 0x1b, 0x0, 0x7282, 0x4c0 },
		{ 0x0, 0xc039, 0x0, 0x8763 },
	},
	{
		0xc0, 0x6700, (Entity *)0xff02,
		{ 0x0, 0x8200, 0x1a7c, 0x0 },
		{ 0x2a00, 0x4c0, 0x0, 0xc039 },
	},
	{
		0x0, 0x8967, (Entity *)0x6c0000c0,
		{ 0xff49, 0x0, 0x0, 0x8200 },
		{ 0x1b7d, 0x0, 0x8a00, 0xc0c4 },
	},
	{
		0x4, 0x3900, (Entity *)0x6c0000c0,
		{ 0xc0d0, 0x0, 0x6d6d, 0xff },
		{ 0x0, 0x0, 0x7e82, 0x1b },
	},
	{
		0x0, 0x6782, (Entity *)0x4c0,
		{ 0xc039, 0x0, 0x26e, 0xc0 },
		{ 0x6f00, 0xff13, 0x0, 0x0 },
	},
	{
		0x8200, 0x1b7f, (Entity *)0x8a000000,
		{ 0xc02a, 0x4, 0x3900, 0xc0 },
		{ 0x6f00, 0xc0ae, 0x0, 0x474 },
	},
	{
		0xff, 0x0, (Entity *)0x80820000,
		{ 0x1b, 0x0, 0x9c87, 0x4c0 },
		{ 0x0, 0xc039, 0x0, 0x9b74 },
	},
	{
		0xc0, 0x7500, (Entity *)0xffef,
		{ 0x0, 0x8200, 0x1b81, 0x0 },
		{ 0x8700, 0xc0af, 0x4, 0x3900 },
	},
	{
		0xc0, 0x7600, (Entity *)0xc082,
		{ 0xcd76, 0xff, 0x0, 0x0 },
		{ 0x8282, 0x1b, 0x0, 0xf280 },
	},
	{
		0x4c0, 0x0, (Entity *)0xc039,
		{ 0xd176, 0xc0, 0x7700, 0xff48 },
		{ 0x0, 0x0, 0x8200, 0x1b83 },
	},
	{
		0x0, 0x8300, (Entity *)0x4c083,
		{ 0x3900, 0xc0, 0x7700, 0xc0dd },
		{ 0x0, 0x1778, 0xff, 0x0 },
	},
	{
		0x0, 0x8482, (Entity *)0x1b,
		{ 0xae80, 0x4c0, 0x0, 0xc039 },
		{ 0x0, 0xb078, 0xc0, 0x7900 },
	},
	{
		0xff08, 0x0, (Entity *)0x82000000,
		{ 0x1b85, 0x0, 0x8600, 0xc097 },
		{ 0x4, 0x3900, 0xc0, 0x7900 },
	},
	{
		0xc09b, 0x0, (Entity *)0xff4e82,
		{ 0x0, 0x0, 0x8682, 0x1b },
		{ 0x0, 0xaa81, 0x4c0, 0x0 },
	},
	{
		0xc039, 0x0, (Entity *)0xc0e782,
		{ 0x8300, 0xff25, 0x0, 0x0 },
		{ 0x8200, 0x1587, 0x0, 0x8100 },
	},
	{
		0xc0ce, 0x4, (Entity *)0x76833b00,
		{ 0x7784, 0xff, 0x0, 0x0 },
		{ 0x8882, 0x15, 0x0, 0x1a84 },
	},
	{
		0x4c0, 0x0, (Entity *)0x8ed0843b,
		{ 0xffc7, 0x0, 0x0, 0x8200 },
		{ 0x1589, 0x0, 0x8700, 0xc072 },
	},
	{
		0x4, 0x3b00, (Entity *)0xfe90cb8e,
		{ 0xff, 0x0, 0x0, 0x8a82 },
		{ 0x15, 0x0, 0xf980, 0x4c0 },
	},
	{
		0x0, 0x993b, (Entity *)0xff76a527,
		{ 0x0, 0x0, 0x8200, 0x148b },
		{ 0x0, 0x7a00, 0x4c0, 0x0 },
	},
	{
		0xa53b, 0xa67a, (Entity *)0xff1d,
		{ 0x0, 0x8200, 0x158c, 0x0 },
		{ 0x8a00, 0xc07d, 0x4, 0x3d00 },
	},
	{
		0xdc83, 0x9c88, (Entity *)0xff,
		{ 0x0, 0x8d82, 0x15, 0x0 },
		{ 0xf083, 0x4c0, 0x0, 0x883d },
	},
	{
		0x8ab5, 0xff28, (Entity *)0x10000,
		{ 0x8200, 0x158e, 0x0, 0x8600 },
		{ 0xc00c, 0x4, 0x3d00, 0x5d8a },
	},
	{
		0xc58b, 0xff, 0,
		{ 0x8f82, 0x15, 0x0, 0x7c81 },
		{ 0x4c0, 0x0, 0x8b3d, 0x8dfb },
	},
	{
		0xff5d, 0x0, (Entity *)0x82000000,
		{ 0x1590, 0x0, 0x8000, 0xc0db },
		{ 0x4, 0x3d00, 0x948e, 0xb390 },
	},
	{
		0xff, 0x100, (Entity *)0x91820000,
		{ 0x15, 0x0, 0x289, 0x4c0 },
		{ 0x0, 0x903d, 0x94fb, 0xff2c },
	},
	{
		0x0, 0x1, (Entity *)0x15928200,
		{ 0x0, 0x8200, 0xc056, 0x4 },
		{ 0x3d00, 0x5694, 0x6e97, 0xff },
	},
	{
		0x100, 0x0, (Entity *)0x159382,
		{ 0x0, 0x3f85, 0x4c0, 0x0 },
		{ 0x973d, 0x9a88, 0xffbd, 0x0 },
	},
	{
		0x1, 0x8200, (Entity *)0x1594,
		{ 0x8500, 0xc0d7, 0x4, 0x3d00 },
		{ 0xdc9a, 0xf69b, 0xff, 0x100 },
	},
	{
		0x0, 0x9582, (Entity *)0x15,
		{ 0x7289, 0x4c0, 0x0, 0x9c3d },
		{ 0x9ff7, 0xff1b, 0x0, 0x1 },
	},
	{
		0x8200, 0x1596, (Entity *)0x85000000,
		{ 0xc023, 0x4, 0x3d00, 0x1ca0 },
		{ 0x3fa2, 0xff, 0x100, 0x0 },
	},
	{
		0x9782, 0x15, (Entity *)0xe9800000,
		{ 0x4c0, 0x0, 0xa23d, 0xb55b },
		{ 0xffcb, 0x0, 0x0, 0x8200 },
	},
	{
		0x1598, 0x0, (Entity *)0xc0fc8000,
		{ 0x4, 0x3d00, 0x16b6, 0xa7b7 },
		{ 0xff, 0x0, 0x0, 0x9982 },
	},
	{
		0x15, 0x0, (Entity *)0x4c02985,
		{ 0x0, 0xb83d, 0xb9c7, 0xff6a },
		{ 0x0, 0x1, 0x8200, 0x189a },
	},
	{
		0x0, 0x8700, (Entity *)0x4c0fa,
		{ 0x3d00, 0x83b9, 0xc0, 0x4100 },
		{ 0xffd2, 0x0, 0x0, 0x8200 },
	},
	{
		0x1a9b, 0x0, (Entity *)0x4c07000,
		{ 0x0, 0xc03d, 0x0, 0x8342 },
		{ 0xc0, 0x5400, 0xffb7, 0x0 },
	},
	{
		0x0, 0x8200, (Entity *)0x1b9c,
		{ 0x8000, 0xc0a1, 0x4, 0x3d00 },
		{ 0xc0, 0x5400, 0xc0f1, 0x0 },
	},
	{
		0xfc55, 0xff, (Entity *)0x100,
		{ 0x9d82, 0x1b, 0x0, 0x8a89 },
		{ 0x4c0, 0x0, 0xc03d, 0x0 },
	},
	{
		0x2856, 0xc0, (Entity *)0xffe65800,
		{ 0x0, 0x0, 0x8200, 0x1b9e },
		{ 0x0, 0x8100, 0xc027, 0x4 },
	},
	{
		0x3d00, 0xc0, (Entity *)0xc0185900,
		{ 0x0, 0x255b, 0xff, 0x100 },
		{ 0x0, 0x9f82, 0x1b, 0x0 },
	},
	{
		0x9b84, 0x4c0, (Entity *)0xc03d0000,
		{ 0x0, 0x615b, 0xc0, 0x5e00 },
		{ 0xff13, 0x0, 0x1, 0x8200 },
	},
	{
		0x1ba0, 0x0, (Entity *)0xc04b8600,
		{ 0x4, 0x3d00, 0xc0, 0x5e00 },
		{ 0xc041, 0x0, 0x5460, 0xff },
	},
	{
		0x100, 0x0, (Entity *)0x1ba182,
		{ 0x0, 0x7d83, 0x4c0, 0x0 },
		{ 0xc03d, 0x0, 0x7060, 0xc0 },
	},
	{
		0x6100, 0xff21, (Entity *)0x10000,
		{ 0x8200, 0x1ba2, 0x0, 0x8300 },
		{ 0xc0c3, 0x4, 0x3d00, 0xc0 },
	},
	{
		0x6100, 0xc025, (Entity *)0xf2610000,
		{ 0xff, 0x0, 0x0, 0xa382 },
		{ 0x1b, 0x0, 0x5886, 0x4c0 },
	},
	{
		0x0, 0xc03d, (Entity *)0xf6610000,
		{ 0xc0, 0x6300, 0xffb0, 0x0 },
		{ 0x1, 0x8200, 0x1ba4, 0x0 },
	},
	{
		0x8900, 0xc00a, (Entity *)0x3d000004,
		{ 0xc0, 0x6300, 0xc0b4, 0x0 },
		{ 0x6d65, 0xff, 0x100, 0x0 },
	},
	{
		0xa582, 0x1b, (Entity *)0x9e840000,
		{ 0x4c0, 0x0, 0xc03d, 0x0 },
		{ 0x7165, 0xc0, 0x6600, 0xff6a },
	},
	{
		0x0, 0x1, (Entity *)0x15a68200,
		{ 0x0, 0x8600, 0xc070, 0x4 },
		{ 0x3e00, 0x4d83, 0x4789, 0xff },
	},
	{
		0x0, 0x0, (Entity *)0x15a782,
		{ 0x0, 0x368a, 0x4c0, 0x0 },
		{ 0x893e, 0x8a6d, 0xffab, 0x0 },
	},
	{
		0x0, 0x8200, (Entity *)0x15a8,
		{ 0x8100, 0xc06f, 0x4, 0x3e00 },
		{ 0xaf8a, 0x3294, 0xff, 0x0 },
	},
	{
		0x0, 0xa982, (Entity *)0x15,
		{ 0x1a83, 0x4c0, 0x0, 0x943e },
		{ 0x9550, 0xffe5, 0x0, 0x0 },
	},
	{
		0x8200, 0x15aa, (Entity *)0x88000000,
		{ 0xc0e7, 0x4, 0x3e00, 0xe995 },
		{ 0xee9d, 0xff, 0x0, 0x0 },
	},
	{
		0xab82, 0x15, (Entity *)0x84840000,
		{ 0x4c0, 0x0, 0x9e3e, 0xa116 },
		{ 0xffbe, 0x0, 0x0, 0x8200 },
	},
	{
		0x15ac, 0x0, (Entity *)0xc0e28800,
		{ 0x4, 0x3e00, 0xc2a1, 0xf8a2 },
		{ 0xff, 0x0, 0x0, 0xad82 },
	},
	{
		0x15, 0x0, (Entity *)0x4c03d83,
		{ 0x0, 0xa23e, 0xb0fc, 0xff54 },
		{ 0x0, 0x0, 0x8200, 0x15ae },
	},
	{
		0x0, 0x8b00, (Entity *)0x4c028,
		{ 0x3e00, 0x58b0, 0xf4b2, 0xff },
		{ 0x0, 0x0, 0xaf82, 0x15 },
	},
	{
		0x0, 0xe586, (Entity *)0x4c0,
		{ 0xb33e, 0xb6e7, 0xffa8, 0x0 },
		{ 0x0, 0x8200, 0x15b0, 0x0 },
	},
	{
		0x8400, 0xc02a, (Entity *)0x3e000004,
		{ 0x76b7, 0xafb9, 0xff, 0x0 },
		{ 0x0, 0xb182, 0x15, 0x0 },
	},
};

static EndiData ENDI_DATA = {
	0x04c0aa86,
	{ 0x0, 0x8a3f, 0x8a31, 0xffcc },
	{ 0x0, (Entity *)0x14b28200, 0x0, 0x6300 },
	0x0,
};

static void *const endi_functions[] = {
	ENDI_tickEnding,
	ENDI_renderParticles,
	ENDI_tickEndingParticles,
	ENDI_clearParticles,
	ENDI_stopParticles,
	ENDI_setParticleBase,
	ENDI_getParticleBase,
	ENDI_releaseParticles,
	ENDI_spawnParticle,
	ENDI_fadeClut,
	ENDI_startParticles,
	ENDI_renderEndingObject,
	ENDI_updateEnding,
	ENDI_setModelSemiTrans,
	ENDI_setClutStp,
	ENDI_downloadClut,
};

static void ENDI_setClutStp(u_long *clut)
{
	int32_t i;
	RECT rect;
	int16_t *ptr;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t stp;

	ptr = (int16_t *)clut;
	for (i = 0; i < 384; ++i) {
		r = (*ptr >> 0) & 0x1f;
		g = (*ptr >> 5) & 0x1f;
		b = (*ptr >> 10) & 0x1f;

		if (((r == 0) && (g == 0)) && (b == 0)) {
			stp = 0;
		} else {
			stp = 1;
		}

		*ptr = r;
		*ptr += g << 5;
		*ptr += b << 10;
		*ptr++ += stp << 15;
	}

	setRECT(&rect, 0, 488, 16, 24);
	LoadImage(&rect, clut);

	DrawSync(0);
}

static void ENDI_downloadClut(u_long *clut)
{
	RECT rect;

	setRECT(&rect, 0, 488, 16, 24);
	StoreImage(&rect, clut);

	DrawSync(0);
}


static void ENDI_setModelSemiTrans(Entity *entity, int32_t arg1)
{
	TMDModel *model;
	struct TMD_STRUCT *object;
	struct TMD_STRUCT *obj;
	TMD_P_TG3 *ttg3;
	TMD_P_TG4 *ttg4;
	int32_t i;
	int32_t nobj;
	int32_t primn;
	void *prim;
	int32_t j;
	int32_t type;

	model = getEntityModelComponent(entity->type, 2)->modelPtr;
	nobj = model->nobj;
	obj = model->obj;

	for (i = 0; i < nobj; ++i) {
		object = &obj[i];
		primn = object->primn;
		prim = object->primtop;
		for (j = 0; j < primn; ++j) {
			type = ((uint8_t *)prim)[3];
			switch (type) {
			case 0x34:
				ttg3 = (TMD_P_TG3 *)prim;
				ttg3->cd = type | 2;
			case 0x36:
				ttg3 = (TMD_P_TG3 *)prim;
				ttg3->tpage = (ttg3->tpage & 0xff9f) | 0x20;
				prim = &((TMD_P_TG3 *)prim)[1];
				break;
			case 0x3c:
				ttg4 = (TMD_P_TG4 *)prim;
				ttg4->cd = type | 2;
			case 0x3e:
				ttg4 = (TMD_P_TG4 *)prim;
				ttg4->tpage = (ttg4->tpage & 0xff9f) | 0x20;
				prim = &((TMD_P_TG4 *)prim)[1];
				break;
			}
		}
	}
}

static void ENDI_updateEnding(int32_t objectId)
{
	Entity *entity;
	EndingState *state;
	int16_t phase;
	SVECTOR base;
	int32_t scale;

	entity = ENDI_DATA.state.entity;
	++ENDI_DATA.state.frame;
	phase = ENDI_DATA.state.phase;
	state = &ENDI_DATA.state;

	if ((phase != 10) && (phase != 3)) {
		if (phase != 2) {
			if (phase != 1) {
				if (phase == 0) {
					ENDI_fadeClut(ENDI_CLUT_BUFFER, entity, ENDI_FADE_CLUT_BUFFER, 0, 60, state->frame);
					ENDI_spawnParticle(entity, state->frame % (DIGIMON_DATA[entity->type].boneCount - 1) + 1);

					if (state->frame >= 60) {
						state->phase = 1;
					}
				}
			} else {
				ENDI_spawnParticle(entity, state->frame % (DIGIMON_DATA[entity->type].boneCount - 1) + 1);

				if (state->frame < 79) {
					scale = lerp(0x1000, 0x999, 60, 78, state->frame);
				} else {
					scale = lerp(0x999, 0x1800, 78, 80, state->frame);
				}

				entity->posData->scale.vx = scale;
				entity->posData->scale.vy = scale;
				entity->posData->scale.vz = scale;

				if (state->frame >= 80) {
					playSound(8, 1);
					state->phase = 2;
					ENDI_releaseParticles(0);
					state->velocity = -450;
				}
			}
		} else {
			ENDI_getParticleBase(0, &base);
			base.vy += state->velocity;
			ENDI_setParticleBase(0, &base);

			state->velocity += 20;
			if (state->velocity >= 0) {
				state->phase = 3;
			}
		}
	} else {
		stopSound();
		ENDI_stopParticles();
		removeObject(0x812, objectId);
		state->frame = -1;
	}
}

static void ENDI_renderEndingObject(int32_t objectId)
{
}

static void ENDI_startParticles(void)
{
	ENDI_clearParticles();
	ENDI_DATA.flag = 1;
	addObject(0x813, 0, ENDI_tickEndingParticles, ENDI_renderParticles);
}

static void ENDI_fadeClut(u_long *srcClut, Entity *entity, u_long *dstClut,
			  int32_t startFrame, int32_t endFrame, int32_t frame)
{
	int32_t i;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t stp;
	int16_t *src;
	int16_t *dst;
	RECT rect;

	src = (int16_t *)srcClut;
	dst = (int16_t *)dstClut;

	for (i = 0; i < 384; ++i) {
		int32_t den;
		int32_t num;

		num = endFrame - frame;
		den = endFrame - startFrame;

		r = (*src >> 0) & 0x1f;
		g = (*src >> 5) & 0x1f;
		b = (*src >> 10) & 0x1f;
		++src;

		r = r * num / den;
		g = g * num / den;
		b = b * num / den;

		if (((r == 0) && (g == 0)) && (b == 0)) {
			stp = 0;
		} else {
			stp = 1;
		}

		*dst = r;
		*dst += g << 5;
		*dst += b << 10;
		*dst++ += stp << 15;
	}

	setRECT(&rect, 0, 488, 16, 24);
	LoadImage(&rect, (u_long *)dstClut);

	DrawSync(0);
}

static int32_t ENDI_spawnParticle(Entity *entity, int16_t boneIndex)
{
	int32_t i;
	EndiParticle *p;

	for (i = 0; i < NUM_ENDI_PARTICLES; ++i) {
		if (ENDI_PARTICLES[i].active < 0) {
			break;
		}
	}

	p = &ENDI_PARTICLES[i];
	p->active = 0;
	p->boneIndex = boneIndex;
	p->entity = entity;
	p->pos.vx = (rand() % 140) - 70;
	p->pos.vy = (rand() % 80) - 40;
	p->pos.vz = (rand() % 80) - 40;

	return i;
}

static void ENDI_releaseParticles(int32_t index)
{
	MATRIX baseMatrix;
	MATRIX boneMatrix;
	SVECTOR vec;
	EndiParticle *p;
	int32_t i;

	p = &ENDI_PARTICLES[index];
	ENDI_DATA.flag = 0;

	calculateBoneMatrix(p->entity, 0, &baseMatrix);

	ENDI_DATA.base.vx = baseMatrix.t[1];
	ENDI_DATA.base.vy = baseMatrix.t[2];
	ENDI_DATA.base.vz = baseMatrix.t[3];

	for (i = 0; i < NUM_ENDI_PARTICLES; ++i) {
		p = &ENDI_PARTICLES[i];
		if (p->active >= 0) {
			calculateBoneMatrix(p->entity, p->boneIndex, &boneMatrix);

			vec.vx = (p->pos.vx * p->entity->posData->scale.vx) / ONE;
			vec.vy = (p->pos.vy * p->entity->posData->scale.vy) / ONE;
			vec.vz = (p->pos.vz * p->entity->posData->scale.vz) / ONE;
			ApplyMatrixSV(&boneMatrix, &vec, &vec);

			vec.vx += boneMatrix.t[0];
			vec.vy += boneMatrix.t[1];
			vec.vz += boneMatrix.t[2];

			p->pos.vx = vec.vx - (int16_t)ENDI_DATA.base.vx;
			p->pos.vy = vec.vy - ENDI_DATA.base.vy;
			p->pos.vz = vec.vz - ENDI_DATA.base.vz;
		}
	}
}

static void ENDI_getParticleBase(int32_t arg0, SVECTOR *dst)
{
	copyVector(dst, &ENDI_DATA.base);
}

static void ENDI_setParticleBase(int32_t arg0, SVECTOR *src)
{
	copyVector(&ENDI_DATA.base, src);
}

static void ENDI_stopParticles(void)
{
	removeObject(0x813, 0);
}

static void ENDI_clearParticles(void)
{
	int32_t i;

	for (i = 0; i < NUM_ENDI_PARTICLES; ++i) {
		ENDI_PARTICLES[i].active = -1;
	}
}

static void ENDI_tickEndingParticles(int32_t objectId)
{
}

static void ENDI_renderParticles(int32_t objectId)
{
	int32_t i;
	EndiParticle *p;
	RGB8 color;
	MATRIX boneMatrix;
	SVECTOR vec;

	for (i = 0; i < NUM_ENDI_PARTICLES; ++i) {
		p = &ENDI_PARTICLES[i];
		if (p->active >= 0) {
			color = ENDI_PARTICLE_COLOR;
			if (ENDI_DATA.flag != 0) {
				boneMatrix = p->entity->posData[p->boneIndex].posMatrix.workm;
				vec.vx = (p->pos.vx * p->entity->posData->scale.vx) / ONE;
				vec.vy = (p->pos.vy * p->entity->posData->scale.vy) / ONE;
				vec.vz = (p->pos.vz * p->entity->posData->scale.vz) / ONE;
				ApplyMatrixSV(&boneMatrix, &vec, &vec);
				vec.vx += (int16_t)boneMatrix.t[0];
				vec.vy += (int16_t)boneMatrix.t[1];
				vec.vz += (int16_t)boneMatrix.t[2];
				copyVector(&p->worldPos, &vec);
			} else {
				p->worldPos.vx = p->pos.vx + ENDI_DATA.base.vx;
				p->worldPos.vy = p->pos.vy + ENDI_DATA.base.vy;
				p->worldPos.vz = p->pos.vz + ENDI_DATA.base.vz;
			}

			color.r = rand() % 70 + 60;
			color.g = rand() % 70 + 60;
			color.b = rand() % 70 + 60;

			renderFXParticle(&p->worldPos, rand() % 15 + 15, &color);
		}
	}
}

int32_t ENDI_tickEnding(Entity *entity, int32_t isInitialized)
{
	EndingState *state;

	state = &ENDI_DATA.state;
	if (isInitialized != 0) {
		return state->frame;
	}

	state->frame = 0;
	state->phase = 0;
	state->entity = entity;
	ENDI_downloadClut(ENDI_CLUT_BUFFER);
	ENDI_setClutStp(ENDI_CLUT_BUFFER);
	ENDI_setModelSemiTrans(ENTITY_TABLE[0], 0x28);
	PLAYER_SHADOW_ENABLED = 0;
	addObject(0x812, 0, ENDI_updateEnding, ENDI_renderEndingObject);
	ENDI_startParticles();

	return playSound(8, 0);
}
