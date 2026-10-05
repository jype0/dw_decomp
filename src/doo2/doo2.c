#include <stdlib.h>

#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/btl.h>
#include <dw/doo2.h>
#include <dw/dooa.h>
#include <dw/input.h>
#include <dw/model.h>
#include <dw/rng.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/world_object.h>

typedef struct {
	int16_t centerX;
	int16_t centerY;
	int16_t centerZ;
	int16_t offsetX;
	int16_t offsetY;
	int16_t offsetZ;
	int16_t frame;
} Doo2Shard;

typedef struct {
	SVECTOR *sourceVertices;
	uint32_t vertexCount;
	SVECTOR *worldVertices;
	int32_t unkC;
	int32_t primitives;
	uint32_t primitiveCount;
	int32_t unk18;
} Doo2ModelDesc;

void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
int32_t addScreenPolyFT4(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
void DOO2_setScratchTop(int32_t size);
void DOO2_tickShardSet(int32_t slot);
void DOO2_releaseShardSet(int32_t slot);
void DOO2_renderTriShard(Doo2Shard *drift, int32_t unused1, int16_t speed, int16_t unused3, ModelComponent *model);
void DOO2_renderQuadShard(Doo2Shard *fragment, int32_t arg1, int16_t duration,
                          int16_t arg3, ModelComponent *sheet);
void DOO2_tickEggBox(void);
void DOO2_renderEggIcons(void);
void DOO2_renderShardSet(int32_t index);
void setRotTransMatrix(MATRIX *m);
void DOO2_renderShardSet(int32_t slot);

static void *doo2_functions[] = {
	DOO2_tickEggInput,
	DOO2_openEggBox,
	DOO2_releaseAllShardSets,
	DOO2_buildShardSet,
	DOO2_resetShardSets,
	DOO2_renderSparkStreak,
	DOO2_renderWireframeModel,
	DOO2_fadeClut,
	DOO2_saveClutTile,
	DOO2_saveModelClut,
	DOO2_renderEggIcons,
	DOO2_tickEggBox,
	DOO2_renderQuadShard,
	DOO2_renderTriShard,
#if defined(VERSION_JP)
	DOO2_releaseShardSet,
	DOO2_renderShardSet,
	DOO2_tickShardSet,
#else
	DOO2_releaseShardSet,
	DOO2_tickShardSet,
	DOO2_renderShardSet,
#endif
	DOO2_setScratchTop,
};

RECT MAIN_D_80134B90 = { -84, -70, 168, 80 };
RECT MAIN_D_80134B98 = { -8, -8, 16, 16 };
Doo2EggIcons EGG_ICONS = { { 0, 2, 3, 1 } };
RECT MAIN_D_80134BA4 = { -8, -8, 16, 16 };

int32_t MAIN_D_80135310;
uint8_t *MAIN_D_80135314;
Doo2ModelVertex *MAIN_D_80135318;
int16_t MAIN_D_8013531C[3];

static void *doo2_sbss_order[] = {
	&MAIN_D_8013531C,
	&MAIN_D_80135318,
	&MAIN_D_80135314,
	&MAIN_D_80135310,
};

ModelComponent DOO2_SHARD_PARAMS = {
	0,
	NULL,
	NULL,
	NULL,
	0x39,
	0,
	0,
	0,
	0,
	0,
	0,
};

// clang-format off
GsSPRITE DOO2_EGG_ICON_SPRITE = {
	0x50000000,			/* attribute */
	0x0,				/* x */
	-0x1e,				/* y */
	0x20,				/* w */
	0x20,				/* h */
	0x39,				/* tpage */
	0x60,				/* u */
	0x0,				/* v */
	0x80,				/* cx */
	0x1ec,				/* cy */
	0x80,				/* r */
	0x80,				/* g */
	0x80,				/* b */
	0x10,				/* mx */
	0x10,				/* my */
	0x1000,				/* scalex */
	0x1000,				/* scaley */
	0x0,				/* rotate */
};

GsSPRITE DOO2_EGG_CURSOR_SPRITE = {
	0x50000000,			/* attribute */
	0x0,				/* x */
	-0x1e,				/* y */
	0x28,				/* w */
	0x28,				/* h */
	0x39,				/* tpage */
	0x60,				/* u */
	0x20,				/* v */
	0x80,				/* cx */
	0x1f0,				/* cy */
	0x80,				/* r */
	0x80,				/* g */
	0x80,				/* b */
	0x14,				/* mx */
	0x14,				/* my */
	0x1000,				/* scalex */
	0x1000,				/* scaley */
	0x0,				/* rotate */
};
// clang-format on

void DOO2_setScratchTop(int32_t size)
{
#if defined(VERSION_JP)
	if ((size & 3) != 0) {
		size += 4 - (size & 3);
	}
#else
	int32_t rem;

	rem = size & 3;
	if (rem != 0) {
		size += 4 - rem;
	}
#endif
	MAIN_D_80135310 = size;
}

void DOO2_renderShardSet(int32_t index)
{
	ModelComponent params;
	ModelComponent *paramsPtr;
	Doo2ShardSet *entry;
	int32_t shards;
	int32_t count;

	entry = &DOO2_SHARD_SETS[index];
	shards = entry->centers;
	params = DOO2_SHARD_PARAMS;
	paramsPtr = &params;
	count = entry->centerCount;
	MAIN_D_80135314 = (uint8_t *)entry->primitives;
	MAIN_D_80135318 = (Doo2ModelVertex *)entry->vertices;
	paramsPtr->modelPtr = (TMDModel *)DOOA_REINCARNATION_SEQ.modelData.modelPtr;
	MAIN_D_8013531C[0] = ((((41 - entry->timer) * 74) / 40) + 54);
	MAIN_D_8013531C[1] = MAIN_D_8013531C[0];
	MAIN_D_8013531C[2] = MAIN_D_8013531C[0];

	while (count-- > 0) {
		if (((int8_t *)MAIN_D_80135314)[3] == 0x34) {
			DOO2_renderTriShard((Doo2Shard *)shards, 0, 80, entry->timer,
			                    paramsPtr);
			shards += 14;
			MAIN_D_80135314 += 0x1c;
		} else if (((int8_t *)MAIN_D_80135314)[3] == 0x3c) {
			DOO2_renderQuadShard((Doo2Shard *)shards, 0, 80, entry->timer,
			                     paramsPtr);
			shards += 14;
			MAIN_D_80135314 += 0x24;
		}
	}
}

void DOO2_tickShardSet(int32_t slot)
{
	Doo2ShardSet *entry;

	entry = &DOO2_SHARD_SETS[slot];
	entry->timer++;
	if (entry->timer >= 41) {
		DOO2_releaseShardSet(slot);
	}
}

void DOO2_releaseShardSet(int32_t slot)
{
	Doo2ShardSet *entry;

	entry = &DOO2_SHARD_SETS[slot];
	entry->timer = -1;
	removeObject(0x609, slot);
}

void DOO2_renderTriShard(Doo2Shard *drift, int32_t unused1, int16_t speed, int16_t unused3, ModelComponent *model)
{
	SVECTOR a;
	SVECTOR b;
	SVECTOR c;
	POLY_FT3 *prim;
	TMD_P_TG3 *tri;
	Doo2ModelVertex *va;
	Doo2ModelVertex *vb;
	Doo2ModelVertex *vc;

	tri = (TMD_P_TG3 *)MAIN_D_80135314;
	prim = (POLY_FT3 *)GsGetWorkBase();
	SetPolyFT3(prim);
	SetSemiTrans(prim, 1);
	setRGB0(prim, MAIN_D_8013531C[0], MAIN_D_8013531C[1], MAIN_D_8013531C[2]);
	prim->tpage = model->pixelPage;
	prim->clut = tri->clut;
	setUV3(prim, tri->tu0, tri->tv0, tri->tu1, tri->tv1, tri->tu2, tri->tv2);
	va = &MAIN_D_80135318[tri->v0];
	vb = &MAIN_D_80135318[tri->v1];
	vc = &MAIN_D_80135318[tri->v2];

	drift->frame++;
	drift->offsetY += (drift->frame + (drift->centerY / (speed * 3)));

	if ((drift->offsetY + ((va->vy + vb->vy + vc->vz) / 3)) > 0) {
		drift->offsetY = -(va->vy + vb->vy + vc->vz) / 3;
	} else {
		drift->offsetX += (drift->centerX / (speed * 3));
		drift->offsetZ += (drift->centerZ / (speed * 3));
	}

	a.vx = va->vx + drift->offsetX;
	a.vy = va->vy + drift->offsetY;
	a.vz = va->vz + drift->offsetZ;
	b.vx = vb->vx + drift->offsetX;
	b.vy = vb->vy + drift->offsetY;
	b.vz = vb->vz + drift->offsetZ;
	c.vx = vc->vx + drift->offsetX;
	c.vy = vc->vy + drift->offsetY;
	c.vz = vc->vz + drift->offsetZ;
	prim->code |= 2;
	addScreenPolyFT3(prim, &a, &b, &c);
}

void DOO2_renderQuadShard(Doo2Shard *fragment, int32_t arg1, int16_t duration,
                          int16_t arg3, ModelComponent *sheet)
{
	SVECTOR a;
	SVECTOR b;
	SVECTOR c;
	SVECTOR d;
	POLY_FT4 *prim;
	TMD_P_TG4 *quad;
	Doo2ModelVertex *pa;
	Doo2ModelVertex *pb;
	Doo2ModelVertex *pc;
	Doo2ModelVertex *pd;

	quad = (TMD_P_TG4 *)MAIN_D_80135314;
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	setRGB0(prim, MAIN_D_8013531C[0], MAIN_D_8013531C[1], MAIN_D_8013531C[2]);
	prim->tpage = sheet->pixelPage;
	prim->clut = quad->clut;
	setUV4(prim, quad->tu0, quad->tv0, quad->tu1, quad->tv1, quad->tu2, quad->tv2, quad->tu3, quad->tv3);
	pa = MAIN_D_80135318 + quad->v0;
	pb = MAIN_D_80135318 + quad->v1;
	pc = MAIN_D_80135318 + quad->v2;
	pd = MAIN_D_80135318 + quad->v3;

	fragment->frame++;
	fragment->offsetY += (fragment->frame + fragment->centerY / (duration * 3));

	if ((fragment->offsetY + ((pa->vy + pb->vy + pc->vz + pd->vy) / 4)) > 0) {
		fragment->offsetY = -(pa->vy + pb->vy + pc->vz + pd->vy) / 4;
	} else {
		fragment->offsetX += (fragment->centerX / (duration * 3));
		fragment->offsetZ += (fragment->centerZ / (duration * 3));
	}

	a.vx = pa->vx + fragment->offsetX;
	a.vy = pa->vy + fragment->offsetY;
	a.vz = pa->vz + fragment->offsetZ;
	b.vx = pb->vx + fragment->offsetX;
	b.vy = pb->vy + fragment->offsetY;
	b.vz = pb->vz + fragment->offsetZ;
	c.vx = pc->vx + fragment->offsetX;
	c.vy = pc->vy + fragment->offsetY;
	c.vz = pc->vz + fragment->offsetZ;
	d.vx = pd->vx + fragment->offsetX;
	d.vy = pd->vy + fragment->offsetY;
	d.vz = pd->vz + fragment->offsetZ;
	prim->code |= 2;
	addScreenPolyFT4(prim, &a, &b, &c, &d);
}

void DOO2_tickEggBox(void)
{
	DooaSequence *seq;

	seq = &DOOA_REINCARNATION_SEQ;
	seq->sparkleIndex = 0;
}

void DOO2_renderEggIcons(void)
{
	DooaSequence *panel = &DOOA_REINCARNATION_SEQ;
	int32_t pri = 1;
	Doo2EggIcons icons;
	int32_t i;

	icons = EGG_ICONS;
	for (i = 0; i < 4; i++) {
		DOO2_EGG_ICON_SPRITE.x = (i * 36) - 54;
		DOO2_EGG_ICON_SPRITE.u = (icons.icon[i] * 32) + 96;
		DOO2_EGG_ICON_SPRITE.cy = i + 492;
		GsSortSprite(&DOO2_EGG_ICON_SPRITE, ACTIVE_ORDERING_TABLE, pri);
	}

	DOO2_EGG_CURSOR_SPRITE.x = (panel->eggSlot * 36) - 54;
	GsSortSprite(&DOO2_EGG_CURSOR_SPRITE, ACTIVE_ORDERING_TABLE, pri);
}

void DOO2_saveModelClut(u_long pixels)
{
	RECT rect;

	setRECT(&rect, 32, 488, 16, 24);
	StoreImage(&rect, (u_long *)pixels);
	DrawSync(0);
}

void DOO2_saveClutTile(u_long pixels, int32_t tile)
{
	RECT rect;

	setRECT(&rect, (tile & 0x3f) * 16, tile >> 6, 16, 4);
	StoreImage(&rect, (u_long *)pixels);
	DrawSync(0);
}

void DOO2_fadeClut(int16_t *srcClut, void *entity, int16_t *dstClut, int32_t startFrame, int32_t endFrame, int32_t frame)
{
	RECT rect;
	int32_t i;
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t stp;
	int16_t *src;
	int16_t *dst;

	src = srcClut;
	dst = dstClut;
	for (i = 0; i < 384; i++) {
		r = *src & 0x1f;
		g = (*src >> 5) & 0x1f;
		b = (*src >> 10) & 0x1f;
		stp = (*src++ >> 15) & 0x1;

		if (frame != startFrame) {
			stp = 1;
		}

		r = (int16_t)r * (endFrame - frame) / (endFrame - startFrame);
		g = (int16_t)g * (endFrame - frame) / (endFrame - startFrame);
		b = (int16_t)b * (endFrame - frame) / (endFrame - startFrame);

		*dst = r;
		*dst += (g << 5);
		*dst += (b << 10);
		*dst++ += (stp << 15);
	}

	setRECT(&rect, 32, 488, 16, 24);
	LoadImage(&rect, (u_long *)dstClut);
	DrawSync(0);
}

void DOO2_renderWireframeModel(GsDOBJ2 *obj, int32_t wireThreshold)
{
	struct TMD_STRUCT *tmd;
	int32_t i;
	uint8_t *tmdPrim;
	int32_t primCount;
	SVECTOR *verts;
	SVECTOR *normals;
	uint8_t *packet;
	CVECTOR lightColor;
	long p;
	long flag;
	long otz;
	MATRIX lw;
	GsCOORDINATE2 *coord;
	TMD_P_TG3 *tri;
	POLY_GT3 *poly3;
	POLY_GT4 *poly4;
	LINE_F2 *close;
	int8_t color;
	TMD_P_TG4 *quad;
	LINE_F4 *line;

	color = WIREFRAME_COLOR_MIN + (rand() % (WIREFRAME_COLOR_MAX - WIREFRAME_COLOR_MIN));

	tmd = (struct TMD_STRUCT *)obj->tmd;
	verts = (SVECTOR *)tmd->vertop;
	normals = (SVECTOR *)tmd->nortop;
	tmdPrim = (uint8_t *)tmd->primtop;
	primCount = tmd->primn;

	packet = GsGetWorkBase();

	lightColor.r = lightColor.g = lightColor.b = 0x80;

	coord = obj->coord2;
	if (coord->flg == 0) {
		coord->flg = 1;
		MulMatrix0(&coord->coord, &coord->super->workm, &coord->workm);
	}

	MulMatrix0(&GsLIGHTWSMATRIX, &coord->workm, &lw);
	SetLightMatrix(&lw);
	CompMatrix(&GsWSMATRIX, &coord->workm, &lw);
	setRotTransMatrix(&lw);

	for (i = 0; i < primCount; i++) {
		if (tmdPrim[3] == 0x34) {
			tri = (TMD_P_TG3 *)tmdPrim;
			if (WIREFRAME_RNG_TABLE[i & 0xf] < wireThreshold) {
				poly3 = (POLY_GT3 *)packet;
				if (0 < RotNclip3(&verts[tri->v0], &verts[tri->v1], &verts[tri->v2],
				                  (long *)&poly3->x0, (long *)&poly3->x1, (long *)&poly3->x2,
				                  &p, &otz, &flag)) {
#if !defined(VERSION_JP)
					otz >>= 2;
					if ((otz > 32) && (otz < 4096)) {
#endif
						NormalColorCol3(&normals[tri->n0], &normals[tri->n1],
						                &normals[tri->n2], &lightColor,
						                (CVECTOR *)&poly3->r0, (CVECTOR *)&poly3->r1,
						                (CVECTOR *)&poly3->r2);
						setUV3(poly3, tri->tu0, tri->tv0, tri->tu1, tri->tv1, tri->tu2, tri->tv2);
						poly3->clut = tri->clut;
						poly3->tpage = tri->tpage;
						setPolyGT3(poly3);
#if defined(VERSION_JP)
						otz >>= 2;
#endif
						AddPrim(ACTIVE_ORDERING_TABLE->org + otz, poly3);
						packet = (uint8_t *)++poly3;
#if !defined(VERSION_JP)
					}
#endif
				}
			} else {
				line = (LINE_F4 *)packet;
				if (0 < RotNclip3(&verts[tri->v0], &verts[tri->v1], &verts[tri->v2],
				                  (long *)&line->x0, (long *)&line->x1, (long *)&line->x2,
				                  &p, &otz, &flag)) {
					line->x3 = line->x0;
					line->y3 = line->y0;
					setLineF4(line);
					line->r0 = line->g0 = line->b0 = color;
					otz >>= 2;
					packet = (uint8_t *)++line;
				}
			}
			tmdPrim = (uint8_t *)++tri;
		} else if (tmdPrim[3] == 0x3c) {
			quad = (TMD_P_TG4 *)tmdPrim;
			if (WIREFRAME_RNG_TABLE[i & 0xf] < wireThreshold) {
				poly4 = (POLY_GT4 *)packet;
				if (0 < RotNclip4(&verts[quad->v0], &verts[quad->v1], &verts[quad->v2],
				                  &verts[quad->v3], (long *)&poly4->x0, (long *)&poly4->x1,
				                  (long *)&poly4->x2, (long *)&poly4->x3,
				                  &p, &otz, &flag)) {
#if !defined(VERSION_JP)
					otz >>= 2;
					if ((otz > 32) && (otz < 4096)) {
#endif
						NormalColorCol3(&normals[quad->n0], &normals[quad->n1],
						                &normals[quad->n2], &lightColor,
						                (CVECTOR *)&poly4->r0, (CVECTOR *)&poly4->r1,
						                (CVECTOR *)&poly4->r2);
						NormalColorCol(&normals[quad->n3], &lightColor,
						               (CVECTOR *)&poly4->r3);
						setUV4(poly4, quad->tu0, quad->tv0, quad->tu1, quad->tv1, quad->tu2, quad->tv2, quad->tu3, quad->tv3);
						poly4->clut = quad->clut;
						poly4->tpage = quad->tpage;
						setPolyGT4(poly4);
#if defined(VERSION_JP)
						otz >>= 2;
#endif
						AddPrim(ACTIVE_ORDERING_TABLE->org + otz, poly4);
						packet = (uint8_t *)++poly4;
#if !defined(VERSION_JP)
					}
#endif
				}
			} else {
				line = (LINE_F4 *)packet;
				if (0 < RotNclip4(&verts[quad->v0], &verts[quad->v1], &verts[quad->v2],
				                  &verts[quad->v3], (long *)&line->x0, (long *)&line->x1,
				                  (long *)&line->x3, (long *)&line->x2,
				                  &p, &otz, &flag)) {
					setLineF4(line);
					line->r0 = line->g0 = line->b0 = color;
					otz >>= 2;
					close = (LINE_F2 *)(line + 1);
					setLineF2(close);
					close->r0 = close->g0 = close->b0 = color;
					setXY2(close, line->x3, line->y3, line->x0, line->y0);
					packet = (uint8_t *)++close;
				}
			}
			tmdPrim += sizeof(TMD_P_TG4);
		} else {
			break;
		}
	}

	GsSetWorkBase(packet);
}

void DOO2_renderSparkStreak(int32_t *pos, SVECTOR *rot)
{
	SVECTOR origin;
	SVECTOR a;
	SVECTOR b;
	MATRIX m;
	POLY_FT3 *prim;

	origin.vx = pos[0];
	origin.vy = pos[1];
	origin.vz = pos[2];
	a.vx = 0;
	a.vy = 0;
	a.vz = customRandom(400, 500);
	RotMatrixZYX(rot, &m);
	ApplyMatrixSV(&m, &a, &a);
	a.vx += origin.vx;
	a.vy += origin.vy;
	a.vz += origin.vz;
	b.vx = a.vx + customRandom(-50, 50);
	b.vy = a.vy + customRandom(-50, 50);
	b.vz = a.vz + customRandom(-50, 50);
	prim = (POLY_FT3 *)GsGetWorkBase();
	SetPolyFT3(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = getTPage(0, 1, 768, 256);
	prim->clut = getClut(192, 489);
	setUV3(prim, 0x5f, 0xa0, 0x5f, 0xa7, 0x30, 0xa0);
	setRGB0(prim, (rand() % 128) + 10, prim->r0, prim->r0);
	addScreenPolyFT3(prim, &b, &origin, &a);
}

void DOO2_resetShardSets(int32_t size)
{
	int32_t i;

	for (i = 0; i < 6; i++) {
		DOO2_SHARD_SETS[i].timer = -1;
	}
	DOO2_setScratchTop(size);
}

int32_t DOO2_buildShardSet(VECTOR *offset, u_long modelList, int32_t modelIndex)
{
	Doo2ShardSet *anim;
	int32_t slot;
	u_long tmd;
	u_long header;
	Doo2ModelDesc *objects;
	int32_t outStart;
	SVECTOR *src;
	int32_t i;
	uint8_t *prim;
	int32_t j;
	SVECTOR *va;
	SVECTOR *vb;
	SVECTOR *vc;
	SVECTOR *vd;
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
#if defined(VERSION_JP)
	int16_t center[3];
#else
	int16_t cx;
	int16_t cy;
	int16_t cz;
#endif
	TMD_P_TG3 *tri;
	TMD_P_TG4 *quad;
	Doo2ModelDesc *model;
	int32_t out;

	out = MAIN_D_80135310;
	tmd = modelList;
	header = tmd;
	objects = (Doo2ModelDesc *)(header + 12);
	model = &objects[modelIndex];

	for (slot = 0; slot < 6; slot++) {
		if (DOO2_SHARD_SETS[slot].timer < 0) {
			break;
		}
	}
	if (slot == 6) {
		return -1;
	}

	anim = &DOO2_SHARD_SETS[slot];
	outStart = out;
	src = model->sourceVertices;
	for (i = 0; i < model->vertexCount; i++) {
		((Doo2ModelVertex *)out)->vx = src->vx + offset->vx;
		((Doo2ModelVertex *)out)->vy = src->vy + offset->vy;
		((Doo2ModelVertex *)out)->vz = src->vz + offset->vz;
		src++;
		out += sizeof(Doo2ModelVertex);
	}

	anim->timer = 0;
	anim->primitiveCount = model->primitiveCount;
	anim->centers = out;
	anim->vertices = outStart;
	anim->primitives = model->primitives;
	anim->centerCount = model->primitiveCount;
	addObject(0x609, slot, DOO2_tickShardSet, DOO2_renderShardSet);

	prim = (uint8_t *)model->primitives;
	for (j = 0; j < model->primitiveCount; j++) {
		switch (((int8_t *)prim)[3]) {
		case 0x34:
			tri = (TMD_P_TG3 *)prim;
			va = &model->worldVertices[tri->n0];
			vb = &model->worldVertices[tri->n1];
			vc = &model->worldVertices[tri->n2];
			p0 = *va;
			p1 = *vb;
			p2 = *vc;
#if defined(VERSION_JP)
			center[0] = (p0.vx + p1.vx + p2.vx) / 3;
			center[1] = (p0.vy + p1.vy + p2.vy) / 3;
			center[2] = (p0.vz + p1.vz + p2.vz) / 3;
			((Doo2Shard *)out)->centerX = center[0];
			((Doo2Shard *)out)->centerY = center[1];
			((Doo2Shard *)out)->centerZ = center[2];
#else
			cx = (p0.vx + p1.vx + p2.vx) / 3;
			cy = (p0.vy + p1.vy + p2.vy) / 3;
			cz = (p0.vz + p1.vz + p2.vz) / 3;
			((Doo2Shard *)out)->centerX = cx;
			((Doo2Shard *)out)->centerY = cy;
			((Doo2Shard *)out)->centerZ = cz;
#endif
			((Doo2Shard *)out)->offsetX = 0;
			((Doo2Shard *)out)->offsetY = 0;
			((Doo2Shard *)out)->offsetZ = 0;
			((Doo2Shard *)out)->frame = 0;
			out += sizeof(Doo2Shard);
			prim = (uint8_t *)((int32_t)prim + (int32_t)sizeof(TMD_P_TG3));
			break;
		case 0x3c:
			quad = (TMD_P_TG4 *)prim;
			va = &model->worldVertices[quad->n0];
			vb = &model->worldVertices[quad->n1];
			vc = &model->worldVertices[quad->n2];
			vd = &model->worldVertices[quad->n3];
			p0 = *va;
			p1 = *vb;
			p2 = *vc;
			p3 = *vd;
#if defined(VERSION_JP)
			center[0] = (p0.vx + p1.vx + p2.vx) / 3;
			center[1] = (p0.vy + p1.vy + p2.vy) / 3;
			center[2] = (p0.vz + p1.vz + p2.vz) / 3;
			((Doo2Shard *)out)->centerX = center[0];
			((Doo2Shard *)out)->centerY = center[1];
			((Doo2Shard *)out)->centerZ = center[2];
#else
			cx = (p0.vx + p1.vx + p2.vx) / 3;
			cy = (p0.vy + p1.vy + p2.vy) / 3;
			cz = (p0.vz + p1.vz + p2.vz) / 3;
			((Doo2Shard *)out)->centerX = cx;
			((Doo2Shard *)out)->centerY = cy;
			((Doo2Shard *)out)->centerZ = cz;
#endif
			((Doo2Shard *)out)->offsetX = 0;
			((Doo2Shard *)out)->offsetY = 0;
			((Doo2Shard *)out)->offsetZ = 0;
			((Doo2Shard *)out)->frame = 0;
			out += sizeof(Doo2Shard);
			prim = (uint8_t *)((int32_t)prim + (int32_t)sizeof(TMD_P_TG4));
			break;
		}
	}

#if !defined(VERSION_JP)
	(void)cx;
	(void)cy;
	(void)cz;
#endif

	MAIN_D_80135310 = out;
	return slot;
}

void DOO2_releaseAllShardSets(void)
{
	int32_t i;

	for (i = 0; i < 6; i++) {
		DOO2_releaseShardSet(i);
	}
}

void DOO2_openEggBox(void)
{
	DooaSequence *seq;
	int32_t boxId;
	RECT startPos;

	seq = &DOOA_REINCARNATION_SEQ;
	boxId = 3;
	startPos = MAIN_D_80134B98;
	seq->eggSlot = 0;
	seq->sparkleIndex = -1;
	createAnimatedUIBox(boxId, 0, 2, &MAIN_D_80134B90, &startPos,
	                    (TickFunction)DOO2_tickEggBox,
	                    (RenderFunction)DOO2_renderEggIcons);
	stopSound();
	playSound(8, 4);
}

int32_t DOO2_tickEggInput(void)
{
	DooaSequence *seq;
	int32_t boxId;
	RECT boxRect;

	seq = &DOOA_REINCARNATION_SEQ;
	boxId = 3;
	boxRect = MAIN_D_80134BA4;

	if (seq->sparkleIndex != 0) {
		return 0;
	}
	if (UI_BOX_DATA[boxId].state == 0) {
		return 1;
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x2000) {
		seq->eggSlot = (seq->eggSlot + 1) % 4;
		playSound(0, 2);
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x8000) {
		seq->eggSlot = (seq->eggSlot + 3) % 4;
		playSound(0, 2);
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON) {
		removeAnimatedUIBox(boxId, &boxRect);
		playSound(0, 3);
	}

	return 0;
}
